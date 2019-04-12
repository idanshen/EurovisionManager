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
    int votes[10];
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
