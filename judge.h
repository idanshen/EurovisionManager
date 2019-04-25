#ifndef EUROVISION_C_JUDGE_H
#define EUROVISION_C_JUDGE_H


/**
* a implementation of Judge ADT
*
* Implements a judge type fot the Eurovision managing system.
* The following functions are available:
*   judgeCreate - Allocates a new judge.
*/

/** Type for defining the judge */
typedef struct Judge_t *Judge;


/**
 * judgeCreate: Allocates a new judge.
 * @param judgeId - unique positive number to identify the judge by.
 * @param judgeName - the name of the judge, must be only lowercase letters and
                      spaces.
 * @param judgeResults - array of 10 integers which contains the stateIds of
                         the specific judge votes.
 * @return
 *  NULL - if one of the parameters is NULL or allocations failed.
* 	A new judge in case of success.
 */
Judge judgeCreate(int judgeId, const char *judgeName, int *judgeResults);

/**
 * judgeDelete - Deallocates an existing judge.
 * @param judge - a judge to release its variable's memory
 */
void judgeDelete(Judge judge);

/**
* mapCopy: Creates a copy of target judge.
*
* @param judge - Target judge.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Judge containing the same elements as judge otherwise.
*/
Judge judgeCopy(Judge judge);

/**
 * judgeGetVOtes - helper function to get the votes of a judge
 * @param judge - a judge to return his votes
 * @return
 *  pointer to a int array of size 10
 */
int* judgeGetVotes(Judge judge);

#endif //EUROVISION_C_JUDGE_H
