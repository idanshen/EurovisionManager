#include "judge.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


/** Type for defining the judge */
typedef struct judge_t{
    int ID;
    char* name;
    int* votes;
};


void deleteJudge(Judge judge){
    free(judge->votes);
    free(judge->name);
    free(judge);
}

Judge createJudge(int judgeId, const char *judgeName, int *judgeResults){
    Judge new_judge=malloc(sizeof(new_judge));
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

Judge copyJudge(Judge judge){
    if(!judge) {
        return NULL;
    }
    /////// to finish
}
