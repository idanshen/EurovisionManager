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
    TEST(testRunGetFriendlyStates)


    //TEST(test1_runContest);
    //TEST(test1_runAudience);

    TEST(test1_runAudience);
    TEST(test1_runFriendly);
    TEST(test2_runContest);
    TEST(test2_runAudience);
    TEST(test2_runFriendly);
    //TEST(test3_runContest);
    TEST(test3_runAudience);
    TEST(test3_runFriendly);
    //TEST(testRunAudienceFavorite)
    //TEST(testRunGetFriendlyStates)

    return 0;
}

