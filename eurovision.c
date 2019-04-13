#include "map.h"
#include "eurovision.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
////????? do we need every declaration

struct eurovision_t{
    Map states;
    Map judges;
};

typedef struct state_t{
    char* name;
    char* song;
    int ID;
    Map votes;
}*State;

typedef struct judge_t{
    int ID;
    char* name;
    int* votes;
}*Judge;

//functions needed for judges map
Judge copyJudge(Judge judge);
int * copyJudgeID(int * id);
Judge releaseJudge(Judge judge);
int * releaseJudgeID(int * ID);

//functions needed for states map
Judge copyState(State state);
int * copyStateID(int * id);
Judge releaseState(State state);
int * releaseStateID(int * ID);

//functions needed for states vote map
int * copyVotes(int * votes);
//int * copyStateID(int * id);
Judge releaseVotes(int * votes);
//int * releaseStateID(int * ID);

Eurovision eurovisionCreate(){
    Map judges_map=mapCreate(copyJudge,copyJudgeID,releaseJudge,releaseJudgeID);
    if(judges_map==NULL){
        return NULL;
    }
    Map states_map=mapCreate(copyState,copyStateID,releaseState,releaseStateID);
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
static bool checkIfStateExists(Map states,int* ID){
    if(mapGet(states,&ID)){
        return true;
    }
    return false;
}

static bool checkIfJudgeExists(Map judges,int* ID){
    if(mapGet(judges,&ID)){
        return true;
    }
    return false;
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

static Judge createJudge(int judgeId, const char *judgeName, int *judgeResults){
    Judge new_judge=malloc(sizeof(*new_judge));
    if(new_judge==NULL){
        return NULL;
    }

    int name_len=(int)strlen(judgeName);
    char * new_name=malloc(sizeof(*new_name)*name_len+1);
    if(new_name==NULL){
        return NULL;
    }
    strcpy(new_name,judgeName);

    int * results=malloc(sizeof(int)*10);
    if(results==NULL){
        return NULL;
    }
    for (int i=0; i<10;i++){
        results[i] = judgeResults[i];
    }
    new_judge->ID = judgeId;
    new_judge->name = new_name;
    new_judge->votes = results;
    return new_judge;
}

static void deleteJudge(Judge judge){
    free(judge->votes);
    free(judge->name);
    free(judge);
}

/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
EurovisionResult eurovisionAddState(Eurovision eurovision, int stateId,
                                    const char *stateName,
                                    const char *songName){
    if(!eurovision||!stateName||!songName){
        return EUROVISION_NULL_ARGUMENT;
    }
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    if(checkIfStateExists(eurovision->states,&stateId)){
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
    if(checkIfJudgeExists(eurovision->judges,&judgeId)){
        return EUROVISION_JUDGE_ALREADY_EXIST;
    }
    Judge new_judge = createJudge(judgeId, judgeName,judgeResults);
    if(!new_judge){
        return EUROVISION_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(eurovision->judges, &judgeId, new_judge);
    if(result==MAP_OUT_OF_MEMORY) {
        return EUROVISION_OUT_OF_MEMORY;
    }
    deleteJudge(new_judge);
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