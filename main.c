#include "eurovision.h"
#include <stdio.h>
#include <stdlib.h>
#include "eurovisionTests.h"

#define TEST(t)                                                                \
  do {                                                                         \
    printf("Testing %s ...\n", #t);                                            \
    if ((t()))                                                                 \
      printf("Test %s: SUCCESS\n", #t);                                        \
    else                                                                       \
      printf("Test %s: FAIL\n", #t);                                           \
  } while (0);

int main(int argc, char *argv[]) {
    TEST(testAddState)
    TEST(testRemoveState)
    TEST(testAddJudge)
    TEST(testRemoveJudge)
    TEST(testAddVote)
    TEST(testRemoveVote)
    TEST(testRunContest)
    TEST(testRunAudienceFavorite)
//    TEST(testRunGetFriendlyStates)
    return 0;
}
/*
int main(){
    Eurovision eur = eurovisionCreate();
    int votes[10] = {0};
    EurovisionResult res1 = eurovisionAddJudge(eur,1,"idan",votes);
    res1 = eurovisionAddJudge(eur,2,"idan",votes);
    res1 = eurovisionAddJudge(eur,3,"idan",votes);
    printf("res1: %d\n", (int)res1);
    EurovisionResult res2 = eurovisionAddJudge(eur,2,"idan",votes);
    printf("res2: %d\n", (int)res2);
    EurovisionResult res3 = eurovisionRemoveJudge(eur,1);
    printf("res3: %d\n", (int)res3);
    return 1;
}
 */

