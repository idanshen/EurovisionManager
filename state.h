#include "map.h" //////////TODO: check if this is ok

#ifndef EUROVISION_C_STATE_H
#define EUROVISION_C_STATE_H


/**
* a implementation of State ADT
*
* Implements a state type for the Eurovision managing system.
* The following functions are available:
*   stateCreate - Allocates a new state.
*/

/** Type for defining the state */
typedef struct State_t *State;


/**
 * stateCreate: Allocates a new state.
 * @param judgeId - unique positive number to identify the judge by.
 * @param judgeName - the name of the judge, must be only lowercase letters and
                      spaces.
 * @param judgeResults - array of 10 integers which contains the stateIds of
                         the specific judge votes.
 * @return
 *  NULL - if one of the parameters is NULL or allocations failed.
* 	A new judge in case of success.
 */
State createState(int stateId,const char* stateName,const char* songName);

/**
 * judgeDelete - Deallocates an existing judge.
 * @param judge - a judge to release its variable's memory
 */
void deleteState(State state);

/**
* mapCopy: Creates a copy of target judge.
*
* @param judge - Target judge.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Judge containing the same elements as judge otherwise.
*/
State stateCopy(State state);

/**
 * addNewStateToVotes: add new state to each state's votes list
 * @param state
 * @param ID
 * @return
 * MAP_SUCCESS - new state was added to an existing's votes list.
 * MAP_OUT_OF_MEMORY - MAP_OUT_OF_MEMORY if an allocation failed.
 * MAP_NULL_ARGUMENT - if a NULL pointer was sent.
 */
MapResult addOrRemoveNewStateToVotes(State state,int * ID,int action);

Map getVotesList(State state);

/**
 * stateGetTopTen: create ordered array with the top 10 states based on the
 * votes of the target state
 * @param state - target state
 * @return
 *  pointer to int array of size 10 that contains the stateIDs by order.
 *  if the target state voted for less than 10 other states, the array will
 *  contain EMPTY (-1) in the remaining places in the array
 */
int* stateGetTopTen(State state);

/**
 * Gets the name of the target state
 * @param state - target state
 * @return
 *  pointer to the state name;
 */
char* stateGetName(State state);

#endif //EUROVISION_C_STATE_H