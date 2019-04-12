#include "map.h"
#include "eurovision.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct eurovision_t{
    Map states;
    Map judges;
};

struct state_t{
    char* name;
    char* song;
    int ID; //might remove
    Map votes;
}*State;

struct judge_t{
    int ID;
    char* name;
    int[10] votes;
}*Judge;


Eurovision eurovisionCreate(){
    Map states_map=mapCreate(); //need to decide which functions to use
    Map judges_map=mapCreate(); // same
}

