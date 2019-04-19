#include "eurovision.h"
#include "map.h"
#include "judge.h"
#include "state.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define ADD 1
#define REMOVE -1
//TODO: declare ID as int

struct eurovision_t{
    Map states;
    Map judges;
};


/**
 * copyVote - Function to be used for copying an Vote as a key to the map
 * @param n - Vote to be copied
 * @return
 *   a copy of the given Vote
 */
static MapKeyElement copyVote(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    char *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(double *) n;
    return (MapDataElement) copy;
}

/**
 * releaseVote - free the memory occupied by a vote instance
 * @param n - a vote instance to be released
 */
static void releaseVote(MapKeyElement n) {
    free(n);
}

/**
 * copyID - Function to be used for copying an ID as a key to the map
 * @param n - ID to be copied
 * @return
 *   a copy of the given ID
 */
static MapKeyElement copyID(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return (MapDataElement) copy;
}

/**
 * releaseID - free the memory occupied by a ID instance
 * @param n - a ID instance to be released
 */
static void releaseID(MapKeyElement n) {
    free(n);
}

/**
 * compareIDs - Function to be used by the map for comparing IDs
 * @param n1 - a ID to be compared
 * @param n2 - a ID to be compared
 * @return
 *		A positive integer if the first element is greater;
 * 		0 if they're equal;
 *		A negative integer if the second element is greater.
 */
static int compareIDs(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

/**
 * releaseJudge - free the memory occupied by a Judge instance
 * @param n - a Judge instance to be released
 */
static void releaseJudge(MapDataElement n) {
    judgeDelete((Judge)n);
}

/**
 * copyJudge - helper function to wrap judgeCopy for map use
 * @param judge - Target judge.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Judge containing the same elements as judge otherwise.
 */
static MapDataElement copyJudge(MapDataElement judge){
    Judge to_copy = (Judge)judge;
    return (MapDataElement) judgeCopy(to_copy);
}

/**
 * releaseState - free the memory occupied by a State instance
 * @param n - a State instance to be released
 */
static void releaseState(MapDataElement n) {
    deleteState((State)n);
}

/**
 * copyState - helper function to wrap stateCopy for map use
 * @param state - Target state.
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A State containing the same elements as state otherwise.
 */
static MapDataElement copyState(MapDataElement state){
    State to_copy = (State)state;
    return (MapDataElement) stateCopy(to_copy);
}


/**
 * checkName - check if the name contains only lowercase letters and spaces
 * @param name -
 * 	the name to validate
 * @return
 *  True if the name contains only lowercase letters and spaces
 *  False otherwise
 */
static bool checkName(const char* name){
    bool flag = true;
    for (int i = 0; i<strlen(name); i++){
        if ((name[i] < 'a') || (name[i] > 'z')){
            if (name[i] != ' '){
                flag = false;
            }
        }
    }
    return flag;
}

/**
 * judgeRemoveByVote - remove all the judges which voted to a specific state
 *                     should be used as part of eurovisionRemoveState function
 * @param judges - map instance contains all the current judges
 * @param stateID - the state ID to be used to determined which judge to delete
 * @return
 *  EUROVISION_INVALID_ID - in case of invalid state ID
 *  EUROVISION_SUCCESS - in case of success
 */
EurovisionResult judgeRemoveByVote(Map judges, int stateId){
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    Judge cur_judge;
    MAP_FOREACH(int*, iterator, judges){
        cur_judge = (Judge)mapGet(judges, iterator);
        for (int i = 0; i<10; i++){
            if (judgeGetVOtes(cur_judge)[i]==stateId){
                mapRemove(judges,iterator);
                break;
            }
        }
    }
    return EUROVISION_SUCCESS;
}

/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

Eurovision eurovisionCreate(){
    Map judges_map=mapCreate(copyJudge,copyID,releaseJudge,releaseID,compareIDs);
    if(judges_map==NULL){
        return NULL;
    }
    Map states_map=mapCreate(copyState,copyID,releaseState,releaseID,compareIDs);
    if(states_map==NULL){
        return NULL;
    }
    Eurovision new_eurovision=malloc(sizeof(*new_eurovision));
    if(new_eurovision==NULL){
        return NULL;
    }
    new_eurovision->judges=judges_map;
    new_eurovision->states=states_map;
    return new_eurovision;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
static EurovisionResult updateStatesVoteMaps(Map states,int stateId,
        int action){
    int ID=stateId;
    MAP_FOREACH(int *,IDiterator,states){
        State current_state=mapGet(states,IDiterator);
        if(current_state==NULL){
            return EUROVISION_STATE_NOT_EXIST;
        }
        if(addOrRemoveNewStateToVotes(current_state,&ID,action)!=MAP_SUCCESS){
            if(action==ADD) {
                return EUROVISION_OUT_OF_MEMORY;
            }
            if(action==REMOVE){
                return EUROVISION_STATE_NOT_EXIST;
            }
        }
    }
    return EUROVISION_SUCCESS;

}

EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId,
                                    const char *stateName,
                                    const char *songName){
    if(!eurovision||!stateName||!songName){
        return EUROVISION_NULL_ARGUMENT;
    }
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    if(mapContains(eurovision->states,&stateId)){
        return EUROVISION_STATE_ALREADY_EXIST;
    }
    if(!checkName(stateName)||!checkName(songName)){
        return EUROVISION_INVALID_NAME;
    }
    State new_state=createState(stateId,stateName,songName);
    if(!new_state){
        return EUROVISION_OUT_OF_MEMORY;
    }
    MapResult result=mapPut(eurovision->states,&stateId,new_state);
    if(result==MAP_OUT_OF_MEMORY) {
        deleteState(new_state);
        return EUROVISION_OUT_OF_MEMORY;
    }
    EurovisionResult update_result=updateStatesVoteMaps(eurovision->states,stateId,ADD);
    if(update_result!=EUROVISION_SUCCESS){
        deleteState(new_state);
        return update_result==EUROVISION_OUT_OF_MEMORY? \
        EUROVISION_OUT_OF_MEMORY:EUROVISION_STATE_NOT_EXIST;
    }
    deleteState(new_state);
    return EUROVISION_SUCCESS;

}

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    //TODO: function to remove every judge who voted to the state
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    mapRemove(eurovision->states,&stateId);
    EurovisionResult update_result=updateStatesVoteMaps(eurovision->states, \
            stateId,REMOVE);
    return update_result;

}


EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,
                                    const char *judgeName,
                                    int *judgeResults){
    if(!eurovision||!judgeName||!judgeResults){
        return EUROVISION_NULL_ARGUMENT;
    }
    if(judgeId<0){
        return EUROVISION_INVALID_ID;
    }
    if(!checkName(judgeName)){
        return EUROVISION_INVALID_NAME;
    }
    if(mapContains(eurovision->judges,&judgeId)){
        return EUROVISION_JUDGE_ALREADY_EXIST;
    }
    Judge new_judge = judgeCreate(judgeId, judgeName,judgeResults);
    if(!new_judge){
        return EUROVISION_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(eurovision->judges, &judgeId, new_judge);
    if(result==MAP_OUT_OF_MEMORY) {
        return EUROVISION_OUT_OF_MEMORY;
    }
    judgeDelete(new_judge);
    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    if(judgeId<0){
        return EUROVISION_INVALID_ID;
    }
    MapResult result = mapRemove(eurovision->judges,&judgeId);
    if (result == MAP_ITEM_DOES_NOT_EXIST){
        return EUROVISION_JUDGE_NOT_EXIST;
    }
    else if (result == MAP_SUCCESS){
        return EUROVISION_SUCCESS;
    }
}

EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver,
                                   int stateTaker){
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    if((stateGiver<0) || (stateTaker<0)){
        return EUROVISION_INVALID_ID;
    }
    if (stateGiver==stateTaker){
        return EUROVISION_SAME_STATE;
    }
    State giver_state = mapGet(eurovision->states,&stateGiver);
    if (!giver_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    Map state_voting_list=getVotesList(giver_state);
    //int curr_vote = *(int*)mapGet(giver_state->votes,&stateTaker);
    int curr_vote = *(int*)mapGet(state_voting_list,&stateTaker);
    curr_vote++;
    //MapResult result = mapPut(giver_state->votes,&stateTaker, &curr_vote);
    MapResult result = mapPut(state_voting_list,&stateTaker, &curr_vote);
    if (result==MAP_SUCCESS){ ////// what about other results?
        return EUROVISION_SUCCESS;
    }
}


EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver,
                                      int stateTaker){
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    if((stateGiver<0) || (stateTaker<0)){
        return EUROVISION_INVALID_ID;
    }
    if (stateGiver==stateTaker){
        return EUROVISION_SAME_STATE;
    }
    State giver_state = mapGet(eurovision->states,&stateGiver);
    if (!giver_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    Map state_voting_list=getVotesList(giver_state);
    //int curr_vote = *(int*)mapGet(giver_state->votes,&stateTaker);
    int curr_vote = *(int*)mapGet(state_voting_list,&stateTaker);
    if (curr_vote>0){
        curr_vote--;
    }
    //MapResult result = mapPut(giver_state->votes,&stateTaker, &curr_vote);
    MapResult result = mapPut(state_voting_list,&stateTaker, &curr_vote);
    if (result==MAP_SUCCESS){ ////// what about other results?
        return EUROVISION_SUCCESS;
    }
}

/**
 * judgesScoreCalculate - create a map that contains as keys the states IDs and
 * as data the average score given by the judges
 * @param eurovision - an eurovision instance target
 * @return
 *  map contains the average judges score per state
 */
static Map judgesScoreCalculate(Eurovision eurovision){
    int score[10] = {12,10,8,7,6,5,4,3,2,1};
    double default_value = 0.;
    Map judges_score = mapCopyOnlyKeys(eurovision->states, copyVote,
                                       releaseVote, &default_value);
    if (!judges_score){
        free(judges_score);
        return NULL;
    }
    Judge cur_judge;
    double score_placeholder;
    double number_of_judges = (double)mapGetSize(eurovision->judges);
    MAP_FOREACH(MapKeyElement, iterator, eurovision->judges){
        cur_judge = mapGet(eurovision->judges, iterator);
        int* votes =judgeGetVOtes(cur_judge);
        for (int i=0; i<10;i++){
            score_placeholder = *(double*)mapGet(judges_score, &votes[i]);
            score_placeholder = score_placeholder+score[i];
            MapResult res = mapPut(judges_score, &votes[i], &score_placeholder);
            if (res!=MAP_SUCCESS){
                mapDestroy(judges_score);
                return NULL;
            }
        }
    }
    MAP_FOREACH(MapKeyElement, iterator, judges_score){
        score_placeholder = *(double*)mapGet(judges_score,iterator);
        score_placeholder = score_placeholder/number_of_judges;
        MapResult res = mapPut(judges_score, iterator, &score_placeholder);
        if (res!=MAP_SUCCESS){
            mapDestroy(judges_score);
            return NULL;
        }
    }
    return judges_score;
}
static Map statesScoreCalculate(Eurovision eurovision){
    if(eurovision==NULL){
        return NULL;
    }
    int default_value=0;
    Map total_num_of_votes=mapCopyOnlyKeys(eurovision->states, copyID,releaseID,
            &default_value); //TODO: check if we need compare func
    if(total_num_of_votes==NULL){
        return NULL;
    }
    MAP_FOREACH(int *,iterator,eurovision->states){
        State current_state=mapGet(eurovision->states,iterator);
        if(current_state==NULL){
            return NULL;
        }
        Map current_votes=getVotesList(current_state);
        MAP_FOREACH(int *, state_iterator,current_votes){
            int * added_score=mapGet(current_votes,state_iterator);
            int * current_score=mapGet(total_num_of_votes,state_iterator); //check if use of iterator is ok
            *current_score+=*added_score;
            mapPut(total_num_of_votes,iterator,current_score);
        }
    }

List eurovisionRunContest(Eurovision eurovision, int audiencePercent){
    if(!eurovision){
        return NULL;
    }
    if((audiencePercent>100)||(audiencePercent<0)){
    return NULL;
    }
    Map judges_score = judgesScoreCalculate(eurovision);
    Map states_score = statesScoreCalculate(eurovision);
    double default_value = 0.;
    double judges_score_placeholder, states_score_placeholder, score;
    Map overall_score = mapCopyOnlyKeys(judges_score, copyVote,
                                       releaseVote, &default_value);
    if (!overall_score){
        free(overall_score);
        return NULL;
    }
    MAP_FOREACH(MapKeyElement, iterator, overall_score){
        judges_score_placeholder = *(double*)mapGet(judges_score, iterator);
        states_score_placeholder = *(double*)mapGet(states_score, iterator);
        score = ((double)audiencePercent/100)*states_score_placeholder +
                (1-(double)audiencePercent/100)*judges_score_placeholder;
        MapResult res = mapPut(overall_score,iterator,&score);
        if (res!=MAP_SUCCESS){
            mapDestroy(judges_score);
            mapDestroy(states_score);
            mapDestroy(overall_score);
            return NULL;
        }
    }
    List final_results = mapToList(overall_score);
    return final_results;
}
    return total_num_of_votes;

}

static int* convertVotesMapToArray(Map votes){
    int same_num_of_votes_counter=0;
    int last_max_value=0;
    int indx=0;
    int size_of_votes=mapGetSize(votes);
    int* winners_by_audience=malloc(sizeof(*winners_by_audience)*size_of_votes);
    if(winners_by_audience==NULL){
        return NULL;
    }
    while(mapGetSize(votes)!=0){
        int * max_key=mapMaxData(votes,compareIDs);
        int * current_max_value=mapGet(votes,max_key);
        if(*current_max_value==last_max_value){
            same_num_of_votes_counter++;
            for(int i=indx;i-same_num_of_votes_counter<i;i--){
                if(winners_by_audience[i]<winners_by_audience[i-1]){
                    int temp=winners_by_audience[i-1];
                    winners_by_audience[i-1]=winners_by_audience[i];
                    winners_by_audience[i]=temp;
                }
            }
        }
        else{
            same_num_of_votes_counter=0;
        }
        last_max_value=*current_max_value;
        MapResult result=mapRemove(votes,max_key);
        if(result!=MAP_SUCCESS){
            mapDestroy(votes);
            free(winners_by_audience);
            return NULL;
        }
        indx++;

    }
    mapDestroy(votes);
    return winners_by_audience;
}

List eurovisionRunAudienceFavorite(Eurovision eurovision){
    if(mapGetSize(eurovision->states)==0){
        List empty_list=listCreate(copyID,releaseID);
        return empty_list;
    }
    Map sum_of_votes=statesScoreCalculate(eurovision);
    if(sum_of_votes==NULL){
        return NULL;
    }
    int size_of_arr=mapGetSize(sum_of_votes);
    int * arr_of_votes=convertVotesMapToArray(sum_of_votes);
    if(arr_of_votes==NULL){
        return NULL;
    }
    List winners_by_audience=listCreate(copyID,releaseID); //create new ID functions for list(?)
    for(int i=0;i<size_of_arr;i++){
        ListResult result=listInsertAfterCurrent(winners_by_audience,
                arr_of_votes); //need to add special case for first?
        if(result!=LIST_SUCCESS) {
            free(arr_of_votes);
            listDestroy(winners_by_audience);
            return NULL;
        }
    }

    return winners_by_audience;
}

