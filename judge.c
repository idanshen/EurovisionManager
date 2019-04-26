#include "judge.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


/** Type for defining the judge */
struct Judge_t{
    char* name;
    int* votes;
    int ID;
};


void judgeDelete(Judge judge){
    free(judge->votes);
    free(judge->name);
    free(judge);
}

Judge judgeCreate(int judgeId, const char *judgeName, int *judgeResults){
    Judge new_judge=malloc(sizeof(*new_judge));
    if(new_judge==NULL){
        return NULL;
    }

    int name_len=(int)strlen(judgeName);
    new_judge->name=malloc(sizeof(char)*name_len+1);
    if(new_judge->name==NULL){
        return NULL;
    }
    strcpy(new_judge->name,judgeName);

    new_judge->votes=malloc(sizeof(int)*10);
    if(new_judge->votes==NULL){
        return NULL;
    }
    for (int i=0; i<10;i++){
        new_judge->votes[i] = judgeResults[i];
    }
    new_judge->ID = judgeId;
    return new_judge;
}

Judge judgeCopy(Judge judge){
    if(!judge) {
        return NULL;
    }
    int id_ptr = judge->ID;
    char* name_ptr = judge->name;
    int* results_ptr = judge->votes;
    Judge new_judge = judgeCreate(id_ptr,name_ptr,results_ptr);
    return new_judge;
}

int* judgeGetVotes(Judge judge){
    return judge->votes;
}