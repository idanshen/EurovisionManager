#include "eurovision.h"
#include "euroMap.h"
#include "judge.h"
#include "state.h"
#include "list.h"
#include "set.h"
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
#define DEFAULT_VALUE 0.0
typedef double Score;
typedef int ID;
static const Score score[10] = {12,10,8,7,6,5,4,3,2,1};

/*
 * enum that helps with comparing two strings lexicographically by using strcmp
 */
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


struct eurovision_t{
    Map states;
    Map judges;
};

/////////////////////////////////////////////////////////////////
                  /* helper eurovision functions */
////////////////////////////////////////////////////////////////

/**
 * copyScore - Function to be used for copying an Score as a key to the map
 * @param n - Score to be copied
 * @return
 *   a copy of the given Score
 */
static MapKeyElement copyScore(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    Score *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(Score *) n;
    return (MapDataElement) copy;
}

/**
 * releaseScore - free the memory occupied by a Score instance
 * @param n - a Score instance to be released
 */
static void releaseScore(MapKeyElement n) {
    free(n);
}

/**
 * compareScores - Function to be used by the map for comparing number of votes
 * @param n1 - a number of votes to be compared
 * @param n2 - a number of votes to be compared
 * @return
 *		A positive integer if the first element is greater;
 * 		0 if they're equal;
 *		A negative integer if the second element is greater.
 */
static int compareScores(MapKeyElement n1, MapKeyElement n2) {
    Score res = (*(Score *) n1 - *(Score *) n2);
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
    *copy = *(ID *) n;
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
    return (*(ID *) n1 - *(ID *) n2);
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

    int string_len=(int)strlen((char*)n);
    char *copy = malloc(sizeof(char)*string_len+1);
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
EurovisionResult judgeRemoveByVote(Eurovision eurovision, ID stateId){
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    Set to_remove = setCreate(copyID,releaseID,compareIDs);
    Judge cur_judge;
    MAP_FOREACH(ID*, iterator, eurovision->judges){
        cur_judge = (Judge) EuroMapGet(eurovision->judges, iterator);
        for (int i = 0; i<10; i++){
            if (judgeGetVotes(cur_judge)[i]==stateId){
                setAdd(to_remove,(SetElement)iterator);
                break;
            }
        }
    }
    EurovisionResult res;
    SET_FOREACH(ID*,iterator,to_remove){
        res = eurovisionRemoveJudge(eurovision, *iterator);
        if (res!=EUROVISION_SUCCESS){
            return EUROVISION_JUDGE_NOT_EXIST;
        }
    }
    setDestroy(to_remove);
    return EUROVISION_SUCCESS;
}


/**
 * checkName - check if the results contains only non-negative integers and
 * there isn't duplicates
 * @param results -
 * 	the results to validate
 * @return
 *  True if the results contains only non-negative integers and
 *  there isn't duplicates. False otherwise
 */
static EurovisionResult checkResults(int* results, Map states){
    for (int i = 0; i<10; i++){
        if (results[i]<0){
            return EUROVISION_INVALID_ID;
        }
        if (!EuroMapGet(states, &results[i])){
            return EUROVISION_STATE_NOT_EXIST;
        }
        for (int j=0; j<10;j++){
            if ((results[i]==results[j])&&(i!=j)){
                return EUROVISION_INVALID_ID;
            }
        }
    }
    return EUROVISION_SUCCESS;
}

/**
 * combineStrings: takes two states' names and combine them into one (ordered)
 * string
 * @param str1 - first string to combine
 * @param str2 - second string to combine
 * @return a new string of the format "FIRST_NAME - SECOND_NAME,"
 */
static char* combineStrings(char* str1, char* str2){
    int len1=(int)strlen(str1);
    int len2=(int)strlen(str2);
    char* new_str=malloc(sizeof(*new_str)*(len1+len2+5));
    if(new_str==NULL){
        return NULL;
    }
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
    return new_str;
}

/**
 * keyListToNameList - convert List contains stateIDs to List contains the
 * corresponding state names
 * @param keys_list - target list contains the StateIds
 * @param states_map - Map object contain all the state names
 * @return
 *  NULL if error acquired, List contains the state names otherwise
 */
static List keyListToNameList(List IDs_list, Map states_map){
    List name_list = listCreate(copyString,releaseString);
    State cur_state;
    char* cur_name;
    LIST_FOREACH(ID*, iterator, IDs_list){
        cur_state = EuroMapGet(states_map, iterator);
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
    Score last_max_value=-1;
    int index=0;
    int size_of_votes= EuroMapGetSize(votes);
    ID* ordered_winners=(int*)malloc(sizeof(int)*size_of_votes);
    if(ordered_winners==NULL){
        return NULL;
    }
    ID * max_key;
    Score * current_max_value;
    while(EuroMapGetSize(votes)!=0){
        max_key= EuroMapMaxData(votes, compareScores);
        current_max_value=(Score*) EuroMapGet(votes, max_key);
        ordered_winners[index]=*max_key;
        if(*current_max_value==last_max_value){
            same_num_of_votes_counter++;
            for(int i=index;i+same_num_of_votes_counter>index;i--){
                if(ordered_winners[i]<ordered_winners[i-1]){
                    ID temp=ordered_winners[i-1];
                    ordered_winners[i-1]=ordered_winners[i];
                    ordered_winners[i]=temp;
                }
            }
        }
        else{
            same_num_of_votes_counter=0;
        }
        last_max_value=*current_max_value;
        MapResult result= EuroMapRemove(votes, max_key);
        if(result!=MAP_SUCCESS){
            free(ordered_winners);
            return NULL;
        }
        index++;
    }
    List results = VotesArrayToList(ordered_winners, size_of_votes);
    free(ordered_winners);
    if (!results){
        free(results);
        return NULL;
    }
    return results;
}

/**
 * addScoreFromVote - take a list of top10 states and give add the required
 * amount of score to their overall sum.
 * the 1st state get 12 points, the 2nd 10, the 3rd 8, th 4th 7 etc.
 * @param voters_score  - map contain the current sum of votes per state
 * @param votes - array of 10 ints represent state IDs ordered by order of win
 * @return
 *  EUROVISION_OUT_OF_MEMORY if an error acquired during mapPut
 *  EUROVISION_SUCCESS otherwise
 */
static EurovisionResult addScoreFromVote(Map voters_score, int* votes){
    Score score_placeholder;
    for (int i=0; i<10;i++){
        if (votes[i]!=EMPTY) {
            score_placeholder = *(Score *) EuroMapGet(voters_score, &votes[i]);
            score_placeholder = score_placeholder + score[i];
            MapResult res = EuroMapPut(voters_score, &votes[i], &score_placeholder);
            if (res != MAP_SUCCESS) {
                return EUROVISION_OUT_OF_MEMORY;
            }
        }
    }
    return EUROVISION_SUCCESS;
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
        voters = &eurovision->judges;
    }
    else{
        voters = &eurovision->states;
    }
    Score score_placeholder, default_value = DEFAULT_VALUE;
    double number_of_voters = (double) EuroMapGetSize(*voters);
    if(voters_flag==STATES){
        number_of_voters=number_of_voters-1;
    }
    Map voters_score = EuroMapCopyOnlyKeys(eurovision->states, copyScore,
                                           releaseScore, &default_value);
    if (!voters_score){
        free(voters_score);
        return NULL;
    }
    if (number_of_voters==0){
        return voters_score;
    }
    MAP_FOREACH(MapKeyElement, iterator, *voters){
        int* votes;
        if (voters_flag==JUDGES){
            Judge cur_judge;
            cur_judge = EuroMapGet(eurovision->judges, iterator);
            votes = judgeGetVotes(cur_judge);
        }
        else{
            State cur_state;
            cur_state = EuroMapGet(eurovision->states, iterator);
            votes = stateGetTopTen(cur_state);
        }
        EurovisionResult res = addScoreFromVote(voters_score, votes);
        if (res!=EUROVISION_SUCCESS){
            EuroMapDestroy(voters_score);
            return NULL;
        }
        if(voters_flag==STATES){
            free(votes);
        }
    }
    MAP_FOREACH(MapKeyElement, iterator, eurovision->states){
        score_placeholder = *(Score*) EuroMapGet(voters_score, iterator);
        score_placeholder = score_placeholder/number_of_voters;
        MapResult res = EuroMapPut(voters_score, iterator, &score_placeholder);
        if (res!=MAP_SUCCESS){
            EuroMapDestroy(voters_score);
            return NULL;
        }
    }
    return voters_score;
}

/**
 * updateNewStateVoteMap - update the voting list of a new added state so that
 * it will have every state's ID in it. Each state's score is initialized to
 * zero.
 * @param states - a map object containing all the states in the eurovision
 * @param new_state - pointer to a new state object
 * @return
 * MAP_SUCCESS the voting list was correctly updated.
 * MAP_ITEM_DOES_NOT_EXIST new state isn't in the states map
 * MAP_NULL_ARGUMENT a null argument was given
 * MAP_OUT_OF_MEMORY can't allocate memory in new state's voting map
 */
static MapResult updateNewStateVoteMap(Map states,State new_state){
    MAP_FOREACH(ID*, iterator, states){
        MapResult res=addOrRemoveNewStateToVotes(new_state,iterator,ADD);
        if(res!=MAP_SUCCESS){
            return res;
        }
    }
    ID new_state_ID=stateGetID(new_state);
    MapResult res= EuroMapSetIterator(states, &new_state_ID);
    if(res!=MAP_SUCCESS){
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    return MAP_SUCCESS;
}

/**
 * update each state's voting list. updates are made when a new state is added,
 * at which case it will be added to every state's list with the score of zero,
 * or when a state is taken out of the eurovision, at which case it will be
 * deleted from every state's voting list.
 * @param states - a map object containing all the eurovision states.
 * @param stateId - the ID of the state that is added or removed.
 * @param action - can be either ADD, when we are adding a new state, or REMOVE
 * when we are removing an existing state.
 * @return EUROVISION_SUCESS every state's voting list was correctly updated.
 * EUROVISION_STATE_NOT_EXIST a certain state wasn't found in the states map,
 * or the function was requested to delete a state which already doesn't exist.
 * EUROVISION_OUT_OF_MEMORY problem with allocating memory in a state's voting
 * list.
 */
static EurovisionResult updateStatesVoteMaps(Map states,ID stateId,
                                             int action){
    ID cur_ID=stateId;
    int num_of_states= EuroMapGetSize(states);
    MAP_FOREACH(ID*, IDiterator, states){
        State current_state= EuroMapGet(states, IDiterator);
        if(current_state==NULL){
            return EUROVISION_STATE_NOT_EXIST;
        }
        if(*IDiterator==stateId && num_of_states>1 && action==ADD){
            MapResult update_result = updateNewStateVoteMap(states,
                    current_state);
            if (update_result != MAP_SUCCESS) {
                return EUROVISION_OUT_OF_MEMORY;
            }
        }
        else {
            if (addOrRemoveNewStateToVotes(current_state, &cur_ID, action)
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
/**
 * combineFriendlyStates - put two friendly states into a list according to
 * the format "State1 - State2". the states are lexicographically ordered.
 * @param first_state - a pointer to State object
 * @param second_state - another pointer to State object
 * @param friendly_states - a list containing all the strings of friendly
 * states according to the format
 * @return  LIST_SUCCESS if the new string is put into the list
 * LIST_OUT_OF_MEMORY problem allocating memory for the string or list
 */
static ListResult combineFriendlyStates(State first_state,State second_state
        ,List friendly_states) {
    char *state_name = stateGetName(first_state);
    char *second_state_name = stateGetName(second_state);
    char *str = combineStrings(state_name, second_state_name);
    if (str == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    ListResult add_res = listInsertLast(friendly_states, str);
    if (add_res != LIST_SUCCESS) {
        listDestroy(friendly_states);
    }
    free(str);
    return add_res;
}

/////////////////////////////////////////////////////////////////
                  /* main eurovision functions */
////////////////////////////////////////////////////////////////

Eurovision eurovisionCreate(){
    Map judges_map=
            euroMapCreate(copyJudge, copyID, releaseJudge, releaseID, compareIDs);
    if(judges_map==NULL){
        return NULL;
    }
    Map states_map=
            euroMapCreate(copyState, copyID, releaseState, releaseID, compareIDs);
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
    EuroMapDestroy(eurovision->states);
    EuroMapDestroy(eurovision->judges);
    free(eurovision);
}

EurovisionResult eurovisionAddState(Eurovision eurovision, ID stateId,
                                    const char *stateName,
                                    const char *songName){
    if(!eurovision||!stateName||!songName){
        return EUROVISION_NULL_ARGUMENT;
    }
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    if(!checkName(stateName)||!checkName(songName)){
        return EUROVISION_INVALID_NAME;
    }
    if(EuroMapContains(eurovision->states, &stateId)){
        return EUROVISION_STATE_ALREADY_EXIST;
    }
    State new_state=createState(stateId,stateName,songName);
    if(!new_state){
        return EUROVISION_OUT_OF_MEMORY;
    }
    MapResult result= EuroMapPut(eurovision->states, &stateId, new_state);
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

EurovisionResult eurovisionRemoveState(Eurovision eurovision, ID stateId){
    if(stateId<0){
        return EUROVISION_INVALID_ID;
    }
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    MapResult result= EuroMapRemove(eurovision->states, &stateId);
    if(result==MAP_ITEM_DOES_NOT_EXIST){
        return EUROVISION_STATE_NOT_EXIST;
    }
    EurovisionResult update_result=updateStatesVoteMaps(eurovision->states,
            stateId,REMOVE);
    EurovisionResult judge_result=judgeRemoveByVote(eurovision,stateId);
    if(judge_result!=EUROVISION_SUCCESS){
        return judge_result;
    }
    return update_result;

}

EurovisionResult eurovisionAddJudge(Eurovision eurovision, ID judgeId,
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
    if(EuroMapContains(eurovision->judges, &judgeId)){
        return EUROVISION_JUDGE_ALREADY_EXIST;
    }
    if(!checkName(judgeName)){
        return EUROVISION_INVALID_NAME;
    }
    Judge new_judge = judgeCreate(judgeId, judgeName,judgeResults);
    if(!new_judge){
        return EUROVISION_OUT_OF_MEMORY;
    }
    MapResult result = EuroMapPut(eurovision->judges, &judgeId, new_judge);
    if(result==MAP_OUT_OF_MEMORY) {
        return EUROVISION_OUT_OF_MEMORY;
    }
    judgeDelete(new_judge);
    return EUROVISION_SUCCESS;
}

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, ID judgeId){
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    if(judgeId<0){
        return EUROVISION_INVALID_ID;
    }
    MapResult result = EuroMapRemove(eurovision->judges, &judgeId);
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

EurovisionResult eurovisionAddVote(Eurovision eurovision, ID stateGiver,
                                   ID stateTaker){
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    if((stateGiver<0) || (stateTaker<0)){
        return EUROVISION_INVALID_ID;
    }
    State giver_state = EuroMapGet(eurovision->states, &stateGiver);
    if (!giver_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    State taker_state = EuroMapGet(eurovision->states, &stateTaker);
    if (!taker_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    if (stateGiver==stateTaker){
        return EUROVISION_SAME_STATE;
    }
    Map state_voting_list=getVotesList(giver_state);
    int curr_vote = *(int*) EuroMapGet(state_voting_list, &stateTaker);
    curr_vote++;
    MapResult result = EuroMapPut(state_voting_list, &stateTaker, &curr_vote);
    if (result==MAP_SUCCESS){
        return EUROVISION_SUCCESS;
    }
    return EUROVISION_OUT_OF_MEMORY;
}

EurovisionResult eurovisionRemoveVote(Eurovision eurovision, ID stateGiver,
                                      ID stateTaker){
    if(!eurovision){
        return EUROVISION_NULL_ARGUMENT;
    }
    if((stateGiver<0) || (stateTaker<0)){
        return EUROVISION_INVALID_ID;
    }
    State giver_state = EuroMapGet(eurovision->states, &stateGiver);
    if (!giver_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    State taker_state = EuroMapGet(eurovision->states, &stateTaker);
    if (!taker_state){
        return EUROVISION_STATE_NOT_EXIST;
    }
    if (stateGiver==stateTaker){
        return EUROVISION_SAME_STATE;
    }
    Map state_voting_list=getVotesList(giver_state);
    int curr_vote = *(int*) EuroMapGet(state_voting_list, &stateTaker);
    if (curr_vote>0){
        curr_vote--;
    }
    MapResult result = EuroMapPut(state_voting_list, &stateTaker, &curr_vote);
    if (result==MAP_SUCCESS){
        return EUROVISION_SUCCESS;
    }
    return EUROVISION_OUT_OF_MEMORY;
}

List eurovisionRunGetFriendlyStates(Eurovision eurovision){
    Map euro_states=eurovision->states;
    List friendly_states=listCreate(copyString,releaseString);
    ID* second_top_ten_voted=NULL,*top_ten_voted=NULL;
    MAP_FOREACH(ID*, iterator, euro_states){
        State current_state= EuroMapGet(euro_states, iterator);
        if(!current_state){
            EuroMapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        top_ten_voted=stateGetTopTen(current_state);
        if(!top_ten_voted){
            EuroMapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        if(top_ten_voted[0]==-1 || top_ten_voted[0]<*iterator){
            free(top_ten_voted);
            continue;
        }
        State examined_state= EuroMapGet(euro_states, &top_ten_voted[0]);
        if(!examined_state){
            EuroMapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        second_top_ten_voted=stateGetTopTen(examined_state);
        if(!second_top_ten_voted){
            free(top_ten_voted);
            EuroMapDestroy(euro_states);
            listDestroy(friendly_states);
            return NULL;
        }
        if(second_top_ten_voted[0]==(*iterator)){
            ListResult add_res=combineFriendlyStates(current_state,
                    examined_state,friendly_states);
            if(add_res!=LIST_SUCCESS){
                EuroMapDestroy(euro_states);
                return NULL;
            }
        }
        free(top_ten_voted);
        free(second_top_ten_voted);
    }
    ListResult result=listSort(friendly_states,sortStrings);
    if(result!=LIST_SUCCESS){
        return NULL;
    }
    return friendly_states;
}

List eurovisionRunContestMain(Eurovision eurovision, int audiencePercent){
    if(!eurovision){
        return NULL;
    }
    if((audiencePercent>100)||(audiencePercent<0)){
        return NULL;
    }
    Map judges_score = ScoreCalculate(eurovision, JUDGES);
    Map states_score = ScoreCalculate(eurovision, STATES);
    Score default_value = DEFAULT_VALUE;
    Score judges_score_placeholder, states_score_placeholder, score;
    Map overall_score = EuroMapCopyOnlyKeys(judges_score, copyScore,
                                            releaseScore, &default_value);
    if (!overall_score){
        free(overall_score);
        return NULL;
    }
    MAP_FOREACH(MapKeyElement, iterator, states_score){
        judges_score_placeholder = *(Score*) EuroMapGet(judges_score, iterator);
        states_score_placeholder = *(Score*) EuroMapGet(states_score, iterator);
        score = ((Score)audiencePercent/100)*states_score_placeholder +
                (1-((Score)audiencePercent/100))*judges_score_placeholder;
        MapResult res = EuroMapPut(overall_score, iterator, &score);
        if (res!=MAP_SUCCESS){
            EuroMapDestroy(judges_score);
            EuroMapDestroy(states_score);
            EuroMapDestroy(overall_score);
            return NULL;
        }
    }
    EuroMapDestroy(judges_score);
    EuroMapDestroy(states_score);
    List final_results_keys = mapToOrderedList(overall_score);
    List final_results_names = keyListToNameList(final_results_keys,
            eurovision->states);
    listDestroy(final_results_keys);
    if (!final_results_names){
        listDestroy(final_results_names);
        return NULL;
    }
    EuroMapDestroy(overall_score);
    return final_results_names;
}

List eurovisionRunAudienceFavorite(Eurovision eurovision){
    List winners_by_audience=eurovisionRunContestMain(eurovision, 100);
    if (!winners_by_audience){
        return NULL;
    }
    return winners_by_audience;
}

List eurovisionRunContest(Eurovision eurovision, int audiencePercent){
    if((audiencePercent>100)||(audiencePercent<1)){
        return NULL;
    }
    List results = eurovisionRunContestMain(eurovision, audiencePercent);
    if (!results){
        return NULL;
    }
    return results;
}