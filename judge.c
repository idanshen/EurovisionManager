#include "judge.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


/** Type for defining the judge */
struct Judge_t{
    int ID;
    char* name;
    int* votes;
};


void judgeDelete(Judge judge){
    free(judge->votes);
    free(judge->name);
    free(judge);
}

Judge judgeCreate(int judgeId, const char *judgeName, int *judgeResults){
    Judge new_judge=malloc(sizeof(new_judge));
    if(new_judge==NULL){
        return NULL;
    }

    int *id = malloc(sizeof(int));
    if (!id) {
        return NULL;
    }
    *id = judgeId;

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

int* judgeGetVOtes(Judge judge){
    return judge->votes;
}