#include "state.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "euroMap.h"
#define ADD 1
#define REMOVE -1
#define EMPTY -1

/** Type for defining the state */
struct State_t{
    char* name;
    char* song;
    int ID;
    Map votes;
};

/////////////////////////////////////////////////////////////////
                 /* helper state functions */
////////////////////////////////////////////////////////////////
//TODO@roy: add comment
static MapKeyElement copyVotesOrID(MapKeyElement n) {
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

//TODO@roy: add comment
static void releaseVotesOrID(MapKeyElement n) {
    free(n);
}

//TODO@roy: add comment
static int compareIDs(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

/////////////////////////////////////////////////////////////////
              /* main state functions */
////////////////////////////////////////////////////////////////

State createState(int stateId,const char* stateName,const char* songName){
    State new_state=malloc(sizeof(*new_state));
    if(new_state==NULL){
        return NULL;
    }
    int name_len=(int)strlen(stateName);
    char * new_name=malloc(sizeof(*new_name)*name_len+1);
    if(new_name==NULL){
        free(new_state);
        return NULL;
    }
    strcpy(new_name,stateName);
    int song_len=(int)strlen(songName);
    char * new_song=malloc(sizeof(*new_song)*song_len+1);
    if(new_song==NULL){
        free(new_state);
        free(new_name);
        return NULL;
    }
    strcpy(new_song,songName);
    new_state->ID = stateId;
    new_state->name = new_name;
    new_state->song = new_song;
    Map new_map=mapCreate(copyVotesOrID,copyVotesOrID,releaseVotesOrID,
            releaseVotesOrID,compareIDs);
    if (new_map==NULL){
        free(new_state);
        free(new_name);
        free(new_song);
        return NULL;
    }
    new_state->votes=new_map;

    return new_state;
}

State stateCopy(State state){
    if (!state){
        return NULL;
    }
    const char * state_name= state->name;
    const char * song_name= state->song;
    int state_id= state->ID;
    State new_state=createState(state_id,state_name,song_name);
    if(!new_state){
        return NULL;
    }
    return new_state;
}

void deleteState(State state){
    free(state->song);
    free(state->name);
    mapDestroy(state->votes);
    free(state);
}

MapResult addOrRemoveNewStateToVotes(State state,int * ID,int action){
    if (state==NULL){
        return MAP_NULL_ARGUMENT;
    }
    if(action==ADD) {
        int new_num_of_votes = 0;
        if (mapPut(state->votes, ID, &new_num_of_votes) != MAP_SUCCESS) {
            return MAP_OUT_OF_MEMORY;
        }
    }
    if(action==REMOVE){
        if(mapRemove(state->votes,ID)!=MAP_SUCCESS){
            return MAP_ITEM_DOES_NOT_EXIST;
        }
    }
    return MAP_SUCCESS;
}

Map getVotesList(State state){
    if(!state){
        return NULL;
    }
    Map state_list = state->votes;
    return state_list;
}

int* stateGetTopTen(State state){
    int* votes = (int*)malloc(sizeof(int)*10);
    int index = 0, same_num_of_votes_counter = 0, last_max_value = -1;
    if (!votes){
        return NULL;
    }
    Map state_votes = mapCopy(getVotesList(state));
    int * max_key;
    int * current_max_value;
    while((mapGetSize(state_votes)>0)&&(index<10)){
        max_key = mapMaxData(state_votes,compareIDs);
        current_max_value = mapGet(state_votes,max_key);
        votes[index]=*max_key;
        if(*current_max_value==0){
            break;
        }
        if(*current_max_value==last_max_value){
            same_num_of_votes_counter++;
            for(int i=index;i+same_num_of_votes_counter>index;i--){
                if(votes[i]<votes[i-1]){
                    int temp=votes[i-1];
                    votes[i-1]=votes[i];
                    votes[i]=temp;
                }
            }
        }
        else{
            same_num_of_votes_counter=0;
        }
        last_max_value=*current_max_value;
        MapResult result=mapRemove(state_votes,max_key);
        if(result!=MAP_SUCCESS){
            mapDestroy(state_votes);
            free(votes);
            return NULL;
        }
        index++;
    }
    if (index<10){
        for (int j = 0; j<10-index; j++){
            votes[index+j] = EMPTY;
        }
    }
    mapDestroy(state_votes);
    return votes;
}

char* stateGetName(State state){
    if (!state){
        return NULL;
    }
    return state->name;
}

int stateGetID(State state){
    if(!state){
        return -1;
    }
    return state->ID;
}