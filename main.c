#include "eurovision.h"
#include <stdio.h>

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
