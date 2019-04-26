#include "eurovision.h"
#include "mtm_map/map.h"
#include "judge.h"
#include "state.h"
#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define ADD 1
#define REMOVE -1
#define JUDGES 1
#define STATES 2
#define EMPTY -1
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
    double *copy = malloc(sizeof(*copy));
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
    if (!n) {
        free(n);
    }
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
 * copyString - Function to be used for copying an ID as a key to the map
 * @param n - ID to be copied
 * @return
 *   a copy of the given ID
 */
static ListElement copyString(ListElement n) {
    if (!n) {
        return NULL;
    }
    char *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    copy = strcpy(copy,n);
    return (MapDataElement) copy;
}

/**
 * releaseString - free the memory occupied by a string
 * @param n - a string instance to be released
 */
static void releaseString(ListElement n) {
    free(n);
}

enum Size {FIRST_IS_GREATER=1,SAME_SIZE=0,SECOND_IS_GREATER=-1};
static int sortStrings(ListElement str1,ListElement str2){
    if(!str1|| !str2){
        return LIST_NULL_ARGUMENT;
    }
    if(strcmp(str1,str2)>0){
        return FIRST_IS_GREATER;

    }
    if(strcmp(str1,str2)==0){
        return SAME_SIZE;
    }
    return SECOND_IS_GREATER;

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
 * compareVotes - Function to be used by the map for comparing number of votes
 * @param n1 - a number of votes to be compared
 * @param n2 - a number of votes to be compared
 * @return
 *		A positive integer if the first element is greater;
 * 		0 if they're equal;
 *		A negative integer if the second element is greater.
 */
static int compareVotes(MapKeyElement n1, MapKeyElement n2) {
    double res = (*(double *) n1 - *(double *) n2);
    if (res==0){
        return 0;
    }
    else if (res>0){
        return 1;
    }
    else {
        return -1;
    }
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
            if (judgeGetVotes(cur_judge)[i]==stateId){
                mapRemove(judges,iterator);
                break;
            }
        }
    }
    return EUROVISION_SUCCESS;
}


/**
 * checkName - check if the results contains only non-negative integers
 * @param results -
 * 	the results to validate
 * @return
 *  True if the name contains only non-negative integers
 *  False otherwise
 */
static EurovisionResult checkResults(int* results, Map states){
    for (int i = 0; i<10; i++){
        if (results[i]<0){
            return EUROVISION_INVALID_ID;
        }
        if (!mapGet(states,&results[i])){
            return EUROVISION_STATE_NOT_EXIST;
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

void eurovisionDestroy(Eurovision eurovision){
    mapDestroy(eurovision->states);
    mapDestroy(eurovision->judges);
    free(eurovision);
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
static MapResult updateNewStateVoteMap(Map states,State new_state){
    MAP_FOREACH(int *,iterator,states){
        MapResult res=addOrRemoveNewStateToVotes(new_state,iterator,ADD);
        if(res!=MAP_SUCCESS){
            return res;
        }
    }
    return MAP_SUCCESS;
}
static EurovisionResult updateStatesVoteMaps(Map states,int stateId,
        int action){
    int ID=stateId;
    int num_of_states=mapGetSize(states);
    MAP_FOREACH(int *,IDiterator,states){
        State current_state=mapGet(states,IDiterator);
        if(current_state==NULL){
            return EUROVISION_STATE_NOT_EXIST;
        }
        if(*IDiterator==stateId && num_of_states>1 && action==ADD){
                MapResult update_result = updateNewStateVoteMap(states, current_state);
                if (update_result != MAP_SUCCESS) {
                    return EUROVISION_OUT_OF_MEMORY;
                }
            }
        else {
            if (addOrRemoveNewStateToVotes(current_state, &ID, action)
            != MAP_SUCCESS) {
                if (action == ADD) {
                    return EUROVISION_OUT_OF_MEMORY;
                }
                if (action == REMOVE) {
                    return EUROVISION_STATE_NOT_EXIST;
                }
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
    EurovisionResult update_result=updateStatesVoteMaps(eurovision->states,
            stateId,ADD);
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
    MapResult result=mapRemove(eurovision->states,&stateId);
    if(result==MAP_ITEM_DOES_NOT_EXIST){
        return EUROVISION_STATE_NOT_EXIST;
    }
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
    EurovisionResult res = checkResults(judgeResults, eurovision->states);
    if(res!=EUROVISION_SUCCESS){
        return res;
    }
    if(mapContains(eurovision->judges,&judgeId)){
        return EUROVISION_JUDGE_ALREADY_EXIST;
    }
    if(!checkName(judgeName)){
        return EUROVISION_INVALID_NAME;
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
    else {
        return EUROVISION_OUT_OF_MEMORY;
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
    State taker_state = mapGet(eurovision->states,&stateTaker);
    if (!taker_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    Map state_voting_list=getVotesList(giver_state);
    int curr_vote = *(int*)mapGet(state_voting_list,&stateTaker);
    curr_vote++;
    MapResult result = mapPut(state_voting_list,&stateTaker, &curr_vote);
    if (result==MAP_SUCCESS){ ////// what about other results?
        return EUROVISION_SUCCESS;
    }
    return EUROVISION_OUT_OF_MEMORY;
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
    State taker_state = mapGet(eurovision->states,&stateTaker);
    if (!taker_state){
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
    return EUROVISION_OUT_OF_MEMORY;
}

/**
 * ScoreCalculate - create a map that contains as keys the states IDs and
 * as data the average score given by the given voters - states or judges
 * @param eurovision - an eurovision instance target
 * @param voters_flag - target score type, either JUDGES or STATES
 * @return
 *  NULL if a error acquired (memory allocation, illegal argument etc.)
 *  map contains the average score per state otherwise
 */
static Map ScoreCalculate(Eurovision eurovision, int voters_flag){
    if ((voters_flag!=JUDGES)&&(voters_flag!=STATES)){
        return NULL;
    }
    Map* voters;
    if (voters_flag==JUDGES){
        voters = &eurovision->judges; //TODO:check this
    }
    else{
        voters = &eurovision->states;
    }
    double score[10] = {12,10,8,7,6,5,4,3,2,1};
    double default_value = 0.;
    Map voters_score = mapCopyOnlyKeys(eurovision->states, copyVote,
                                       releaseVote, &default_value);
    if (!voters_score){
        free(voters_score);
        return NULL;
    }
    double score_placeholder;
    double number_of_voters = (double)mapGetSize(*voters);
    int* votes;
    MAP_FOREACH(MapKeyElement, iterator, *voters){
        if (voters_flag==JUDGES){
            Judge cur_judge;
            cur_judge = mapGet(eurovision->judges, iterator);
            votes = judgeGetVotes(cur_judge);
        }
        else{
            State cur_state;
            cur_state = mapGet(eurovision->states, iterator);
            votes = stateGetTopTen(cur_state);
        }
        for (int i=0; i<10;i++){
            if (votes[i]!=EMPTY) {
                score_placeholder = *(double *)mapGet(voters_score, &votes[i]);
                score_placeholder = score_placeholder + score[i];
                MapResult res = mapPut(voters_score, &votes[i], &score_placeholder);
                if (res != MAP_SUCCESS) {
                    mapDestroy(voters_score);
                    return NULL;
                }
            }
        }
    }
    MAP_FOREACH(MapKeyElement, iterator, eurovision->states){
        score_placeholder = *(double*)mapGet(voters_score,iterator);
        score_placeholder = score_placeholder/number_of_voters;
        MapResult res = mapPut(voters_score, iterator, &score_placeholder);
        if (res!=MAP_SUCCESS){
            mapDestroy(voters_score);
            return NULL;
        }
    }
    return voters_score;
}

/**
 * VotesArrayToList - convert array of state IDs to a list of state IDs while
 * keeping the same order
 * @param votes_array - pointer to array of ints to convert
 * @param n  - the length og the array
 * @return
 *  NULL if error acquired, List contains the stateID otherwise
 */
static List VotesArrayToList(int* votes_array, int n){
    List votes = listCreate(copyID,releaseID);
    if (!votes){
        return NULL;
    }
    for (int i = 0; i<n; i++){
        ListResult res = listInsertLast(votes, &votes_array[i]);
        if (res != LIST_SUCCESS){
            free(votes);
            return NULL;
        }
    }
    return votes;
}

/**
 * mapToOrderedList - convert map of state IDs  and number of votes to a list
 * of state IDs ordered by the number of votes the state got
 * @param votes - map contains stateIDs as keys and number of votes os data
 * @return
 *  NULL if error acquired, List contains the stateID otherwise
 */
static List mapToOrderedList(Map votes){
    int same_num_of_votes_counter=0;
    double last_max_value=-1;
    int index=0;
    int size_of_votes=mapGetSize(votes);
    int* ordered_winners=(int*)malloc(sizeof(int)*size_of_votes);
    if(ordered_winners==NULL){
        return NULL;
    }
    int * max_key;
    double * current_max_value;
    while(mapGetSize(votes)!=0){
        max_key=mapMaxData(votes,compareVotes);
        current_max_value=(double*)mapGet(votes,max_key);
        ordered_winners[index]=*(int*)copyID(max_key);
        if(*current_max_value==last_max_value){
            same_num_of_votes_counter++;
            for(int i=index;i+same_num_of_votes_counter>index;i--){
                if(ordered_winners[i]<ordered_winners[i-1]){
                    int temp=ordered_winners[i-1];
                    ordered_winners[i-1]=ordered_winners[i];
                    ordered_winners[i]=temp;
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
            free(ordered_winners);
            return NULL;
        }
        index++;
    }
    mapDestroy(votes);
    List results = VotesArrayToList(ordered_winners, size_of_votes);
    free(ordered_winners);
    if (!results){
        free(results);
        return NULL;
    }
    return results;
}

/**
 * convert List contains stateIDs to List contains the corresponding state names
 * @param keys_list - target list contains the StateIds
 * @param states_map - Map object contain all the state names
 * @return
 *  NULL if error acquired, List contains the state names otherwise
 */
static List keyListToNameList(List keys_list, Map states_map){
    List name_list = listCreate(copyString,releaseString);
    State cur_state;
    char* cur_name;
    LIST_FOREACH(int*, iterator, keys_list){
        cur_state = mapGet(states_map, iterator);
        if (!cur_state){
            free(name_list);
            free(cur_state);
            return NULL;
        }
        cur_name = stateGetName(cur_state);
        ListResult res = listInsertLast(name_list, cur_name);
        if (res!=LIST_SUCCESS){
            free(name_list);
            free(cur_state);
            return NULL;
        }
    }
    return name_list;
}

List eurovisionRunAudienceFavorite(Eurovision eurovision){
    List winners_by_audience=eurovisionRunContest(eurovision, 100);
    if (!winners_by_audience){
        return NULL;
    }
    return winners_by_audience;
}
/**
 * takes two states' names and combine them into one (ordered) string
 * @param str1
 * @param str2
 * @return a new string of the format "FIRST_NAME - SECOND_NAME,"
 */
static char* combineStrings(char* str1, char* str2){
    int len1=(int)strlen(str1);
    int len2=(int)strlen(str2);
    char* new_str=malloc(sizeof(*new_str)*(len1+len2+5));
    if(strcmp(str1,str2)>=0){
        strcpy(new_str,str2);
        strcat(new_str," - ");
        strcat(new_str,str1);
    }
    else{
        strcpy(new_str,str1);
        strcat(new_str," - ");
        strcat(new_str,str2);

    }
    //strcat(new_str,",");

    return new_str;
}
List eurovisionRunGetFriendlyStates(Eurovision eurovision){
    //TODO: fix mapCopy!!!!!!!!
    //Map euro_states=mapCopy(eurovision->states);
    Map euro_states=eurovision->states;
    List friendly_states=listCreate(copyString,releaseString);
    int* top_ten_voted=NULL;
    int* second_top_ten_voted=NULL;
    char* state_name=NULL;
    char* second_state_name=NULL;
    char* new_str=NULL;
    MAP_FOREACH(int *,iterator,euro_states){
        State current_state=mapGet(euro_states,iterator);
        if(!current_state){
            mapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        top_ten_voted=stateGetTopTen(current_state);
        if(!top_ten_voted){
            mapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        if(top_ten_voted[0]==-1 || top_ten_voted[0]<*iterator){
            continue;
        }
        State examined_state=mapGet(euro_states,&top_ten_voted[0]);
        if(!examined_state){
            mapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        second_top_ten_voted=stateGetTopTen(examined_state);
        if(!second_top_ten_voted){
            free(top_ten_voted);
            mapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        if(second_top_ten_voted[0]==(*iterator)){
            state_name=stateGetName(current_state);
            second_state_name=stateGetName(examined_state);
            new_str=combineStrings(state_name,second_state_name);
            ListResult add_res=listInsertLast(friendly_states,new_str);
            if(add_res!=LIST_SUCCESS){
                free(new_str);
                mapDestroy(euro_states);
                listDestroy(friendly_states);
                return NULL;
            }
            free(new_str);
        }
        free(top_ten_voted);
        free(second_top_ten_voted);
    }
    //mapDestroy(euro_states);
    ListResult result=listSort(friendly_states,sortStrings);
    if(result!=LIST_SUCCESS){
        return NULL;
    }
    return friendly_states;
}
List eurovisionRunContest(Eurovision eurovision, int audiencePercent){
    if(!eurovision){
        return NULL;
    }
    if((audiencePercent>100)||(audiencePercent<0)){
        return NULL;
    }
    Map judges_score = ScoreCalculate(eurovision, JUDGES);
    Map states_score = ScoreCalculate(eurovision, STATES);
    double default_value = 0.;
    double judges_score_placeholder, states_score_placeholder, score;
    Map overall_score = mapCopyOnlyKeys(judges_score, copyVote,
                                        releaseVote, &default_value);
    if (!overall_score){
        free(overall_score);
        return NULL;
    }
    MAP_FOREACH(MapKeyElement, iterator, states_score){
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
    mapDestroy(judges_score);
    mapDestroy(states_score);
    List final_results_keys = mapToOrderedList(overall_score);
    List final_results_names = keyListToNameList(final_results_keys,
            eurovision->states);
    free(final_results_keys);
    if (!final_results_names){
        free(final_results_names);
        return NULL;
    }
    return final_results_names;
}

