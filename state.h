#ifndef EUROVISION_C_STATE_H
#define EUROVISION_C_STATE_H

#include "euroMap.h"

/**
* a implementation of State ADT
*
* Implements a state type for the Eurovision managing system.
* The following functions are available:
* stateCreate - Allocates a new state.
* deleteState - Deallocates an existing state.
* stateCopy - Creates a copy of target state.
* addOrRemoveNewStateToVotes - add new state to each state's votes list.
* stateGetTopTen - create ordered array with the top 10 states based on the
   votes of the target state.
* stateGetName - Gets the name of the target state.
* getVotesList - helper function to get the inner votes map of a state.
* stateGetID - helper function to get the state ID.
*/

/** Type for defining the state */
typedef struct State_t *State;


/**
 * stateCreate: Allocates a new state.
 * @param stateId - unique positive number to identify the state by.
 * @param stateName - the name of the state, must be only lowercase letters and
                      spaces.
 * @param stateResults - array of 10 integers which contains the stateIds of
                         the specific state votes.
 * @return
 *  NULL - if one of the parameters is NULL or allocations failed.
* 	A new state in case of success.
 */
State createState(int stateId,const char* stateName,const char* songName);

/**
 * deleteState - Deallocates an existing state.
 * @param state - a state to release its variable's memory
 */
void deleteState(State state);

/**
* stateCopy: Creates a copy of target state.
*
* @param state - Target state.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A State containing the same elements as state otherwise.
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

/**
 * helper function to get the inner votes map of a state
 * @param state - target state
 * @return
 *  state's votes map
 */
Map getVotesList(State state);

/**
 * helper function to get the state ID
 * @param state  - target state
 * @return
 *  state's ID
 */
int stateGetID(State state);

#endif //EUROVISION_C_STATE_H