#include "map.h"
#include "judge.h"
#include "eurovision.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
////????? do we need every declaration
//TODO: declare ID as int

struct eurovision_t{
    Map states;
    Map judges;
};

typedef struct State_t{
    char* name;
    char* song;
    int ID;
    Map votes;
}*State;

//functions needed for states map
static Judge copyState(State state);
static Judge releaseState(State state);

//functions needed for states vote map
static int * copyVotes(int * votes);
static Judge releaseVotes(int * votes);

static State createState(int stateId,const char* stateName
        ,const char* songName){
    State new_state=malloc(sizeof(*new_state));
    if(new_state==NULL){
        return NULL;
    }
    int name_len=(int)strlen(stateName);
    char * new_name=malloc(sizeof(*new_name)*name_len+1);
    if(new_name==NULL){
        return NULL;
    }
    int song_len=(int)strlen(songName);
    char * new_song=malloc(sizeof(*new_song)*song_len+1);
    if(new_song==NULL){
        return NULL;
    }
    new_state->ID = stateId;
    new_state->name = new_name;
    new_state->song=new_song;
    Map new_map=malloc(sizeof(*new_map));
    if (new_map==NULL){
        return NULL;
    }
    return new_state;
}
static void deleteState(State state){
    free(state->song);
    free(state->name);
    mapDestroy(state->votes);
    free(state);
    return;

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
    return copy;
}

/**
 * releaseID - free the memorey occupied by a ID instance
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
    judgeDelete(n);
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

static EurovisionResult updateStatesVoteMaps(Map states,int Id,
        State new_state){
    int i=0;
    MAP_FOREACH(int *,iterator,states){
        if(*iterator==Id){
            return EUROVISION_SUCCESS;
        }
        State current_state = mapGet(states,iterator);
        MapResult result=mapPut(current_state->votes,&Id,&i);
        if (result!=MAP_SUCCESS){
            return EUROVISION_OUT_OF_MEMORY;
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
    State new_state=createState(stateId,stateName,songName);
    if(!new_state){
        return EUROVISION_NULL_ARGUMENT; ///////////// NO!
    }
    MapResult result=mapPut(eurovision->states,&stateId,new_state);
    if(result==MAP_OUT_OF_MEMORY) {
        return EUROVISION_OUT_OF_MEMORY;
    }
    if(updateStatesVoteMaps(eurovision->states,stateId,new_state)!=
    EUROVISION_SUCCESS){
        return EUROVISION_OUT_OF_MEMORY;
    };
    State first_state= mapGetFirst(eurovision->states); /////// need to add checks for errors
    Map updated_states_votes= mapCopy(first_state->votes);
    MAP_FOREACH(int *,iterator,eurovision->states){
        if(*iterator==stateId){
            State current_state = mapGet(eurovision->states,iterator);
            current_state->votes=updated_states_votes;

        }
    }
    deleteState(new_state);
    return EUROVISION_SUCCESS;

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
    int curr_vote = *(int*)mapGet(giver_state->votes,&stateTaker);
    curr_vote++;
    MapResult result = mapPut(giver_state->votes,&stateTaker, &curr_vote);
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
    int curr_vote = *(int*)mapGet(giver_state->votes,&stateTaker);
    if (curr_vote>0){
        curr_vote--;
    }
    MapResult result = mapPut(giver_state->votes,&stateTaker, &curr_vote);
    if (result==MAP_SUCCESS){ ////// what about other results?
        return EUROVISION_SUCCESS;
    }
}