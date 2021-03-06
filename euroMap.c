#include "euroMap.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define ERROR -1

/** Type for defining a node in linked list */
typedef struct node_t {
    void *key;
    void *data;
    struct node_t *next;
} *Node;

/** Type for defining the map */
struct Map_t {
    Node base;
    Node node_iterator;
    copyMapDataElements copyData;
    copyMapKeyElements copyKey;
    freeMapDataElements freeData;
    freeMapKeyElements freeKey;
    compareMapKeyElements compareKey;
};

/////////////////////////////////////////////////////////////////
               /* helper map functions */
////////////////////////////////////////////////////////////////

/**
 * helper function to release the memory allocated by node
 * @param map - map to take freeKey and freeData from
 * @param node - target node
 */
static void freeNode(Map map, Node node){
    map->freeKey(node->key);
    map->freeData(node->data);
    free(node);
}

/////////////////////////////////////////////////////////////////
                /* main map functions */
////////////////////////////////////////////////////////////////

Map euroMapCreate(copyMapDataElements copyDataElement,
                  copyMapKeyElements copyKeyElement,
                  freeMapDataElements freeDataElement,
                  freeMapKeyElements freeKeyElement,
                  compareMapKeyElements compareKeyElements) {
    if ((!copyDataElement) || (!copyKeyElement)
    || (!freeDataElement) || (!compareKeyElements)) {
        return NULL;
    }
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        free(map);
        return NULL;
    }
    map->base = NULL;
    map->node_iterator=NULL;

    map->copyData = copyDataElement;
    map->copyKey = copyKeyElement;
    map->freeData = freeDataElement;
    map->freeKey = freeKeyElement;
    map->compareKey = compareKeyElements;
    return map;
}

int EuroMapGetSize(Map map){
    if (map==NULL){
        return ERROR;
    }
    int counter=0;
    Node next_ptr=map->base;
    if(next_ptr==NULL){
        return 0;
    }
    while(next_ptr!=NULL){
        counter+=1;
        next_ptr=next_ptr->next;
    }
    return counter;
}

bool EuroMapContains(Map map, MapKeyElement element){
    if(map==NULL || element==NULL){
        return false;
    }
    Node first_node=map->base;
    if (!map->base){
        return false;
    }
    MapKeyElement current_key=first_node->key;
    if((map->compareKey(current_key,element))==0){
        return true;
    }
    Node next_node=first_node->next;
    while(next_node!=NULL){
        current_key=next_node->key;
        if((map->compareKey(current_key,element))==0){
            return true;
        }
        next_node=next_node->next;
    }
    return false;
}

MapResult EuroMapClear(Map map){
    if(map==NULL){
        return MAP_NULL_ARGUMENT;
    }
    Node node=map->base;
    while(node!=NULL){
        EuroMapRemove(map, map->base->key);
        node=map->base;
    }
    return MAP_SUCCESS;
}

void EuroMapDestroy(Map map){
    if(map==NULL){
        return;
    }
    EuroMapClear(map);
    free(map);
}

Map EuroMapCopy(Map map){
    if(map==NULL) {
        return NULL;
    }

    copyMapDataElements newCopyData=map->copyData;
    copyMapKeyElements newCopyKey=map->copyKey;
    freeMapDataElements newFreeData=map->freeData;
    freeMapKeyElements newFreeKey=map->freeKey;
    compareMapKeyElements newCompareKey=map->compareKey;
    Map new_map= euroMapCreate(newCopyData, newCopyKey, newFreeData, newFreeKey,
                               newCompareKey);
    MAP_FOREACH(MapKeyElement, iterator, map){
        MapDataElement data_ptr= EuroMapGet(map, iterator);
        EuroMapPut(new_map, iterator, data_ptr);
    }

    return new_map;
}

MapResult EuroMapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement){
    if((!keyElement) || (!dataElement)|| (!map)){
        return MAP_NULL_ARGUMENT;
    }
    Node new = malloc(sizeof(*new));
    if(!new) {
        free(new);
        return MAP_OUT_OF_MEMORY;
    }
    new->key = map->copyKey(keyElement);
    new->data = map->copyData(dataElement);
    new->next = NULL;
    if (!map->base){
        map->base = new;
        map->node_iterator=new;
        return MAP_SUCCESS;
    }
    else {
        if (map->compareKey(map->base->key, keyElement)==0){
            free(map->base->data);
            map->base->data = map->copyData(dataElement);
            freeNode(map, new);
            return MAP_SUCCESS;
        } else if (map->compareKey(map->base->key, keyElement)>0){
            Node temp = map->base;
            map->base = new;
            new->next = temp;
            return MAP_SUCCESS;
        }
        MAP_FOREACH(MapKeyElement, iterator, map) {
            if (!map->node_iterator->next) {
                map->node_iterator->next = new;
                return MAP_SUCCESS;
            } else if
            (map->compareKey(map->node_iterator->next->key, keyElement)==0){
                free(map->node_iterator->next->data);
                map->node_iterator->next->data = map->copyData(dataElement);
                freeNode(map, new);
                return MAP_SUCCESS;
            } else if
            (map->compareKey(map->node_iterator->next->key, keyElement)>0){
                new->next = map->node_iterator->next;
                map->node_iterator->next = new;
                return MAP_SUCCESS;
            }
        }
    }
    return MAP_SUCCESS;
}

MapDataElement EuroMapGet(Map map, MapKeyElement keyElement){
    if((!keyElement) || (!map)){
        return NULL;
    }
    Node iter = map->base;
    while(iter->next){
        if (map->compareKey(iter->key,keyElement) == 0) {
            return iter->data;
        }
        iter = iter->next;
    }
    if (map->compareKey(iter->key,keyElement) == 0) {
        return iter->data;
    }
    return NULL;
}

MapResult EuroMapRemove(Map map, MapKeyElement keyElement){
    if((!keyElement) || (!map)){
        return MAP_NULL_ARGUMENT;
    }
    if(!map->base){
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if (map->compareKey(map->base->key, keyElement)==0){
        map->freeKey(map->base->key);
        map->freeData(map->base->data);
        Node temp_pointer = map->base->next;
        free(map->base);
        map->base = temp_pointer;
        return MAP_SUCCESS;
    }
    MAP_FOREACH(MapKeyElement, iterator, map){
        if(map->node_iterator->next){
            if (map->compareKey(map->node_iterator->next->key, keyElement)==0){
                map->freeKey(map->node_iterator->next->key);
                map->freeData(map->node_iterator->next->data);
                Node temp_pointer = map->node_iterator->next->next;
                free(map->node_iterator->next);
                map->node_iterator->next = temp_pointer;
                return MAP_SUCCESS;
            }
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement EuroMapGetFirst(Map map){
    if (!map) {
        return NULL;
    }
    if (!map->base){
        return NULL;
    }
    map->node_iterator = map->base;
    return map->node_iterator->key;
}

MapKeyElement EuroMapGetNext(Map map){
    if (!map) {
        return NULL;
    }
    if(!map->node_iterator->next) {
        return NULL;
    }
    map->node_iterator = map->node_iterator->next;
    return map->node_iterator->key;
}

MapKeyElement EuroMapMaxData(Map map, compareMapDataElements compare){
    MapDataElement current;
    MapKeyElement max_key = EuroMapGetFirst(map);
    MapDataElement max = EuroMapGet(map, max_key);
    if (max==NULL){
        return NULL;
    }
    MAP_FOREACH(MapKeyElement, iterator, map){
        current = EuroMapGet(map, iterator);
        if (compare(current,max)>0){
            max = current;
            max_key = iterator;
        }
    }
    return max_key;
}


Map EuroMapCopyOnlyKeys(Map map, copyMapDataElements newCopyData,
                        freeMapDataElements newFreeData, MapDataElement defaultValue){
    if(map==NULL) {
        return NULL;
    }
    copyMapKeyElements newCopyKey=map->copyKey;
    freeMapKeyElements newFreeKey=map->freeKey;
    compareMapKeyElements newCompareKey=map->compareKey;
    Map new_map= euroMapCreate(newCopyData, newCopyKey, newFreeData, newFreeKey,
                               newCompareKey);

    Node next_ptr=map->base;
    if(next_ptr==NULL){
        return new_map;
    }
    while(next_ptr!=NULL){
        EuroMapPut(new_map, next_ptr->key, defaultValue);
        next_ptr=next_ptr->next;
    }
    return new_map;
}

MapResult EuroMapSetIterator(Map map, MapKeyElement spot){
    MAP_FOREACH(MapKeyElement, iterator, map){
        if(map->compareKey(iterator,spot)==0){
            return MAP_SUCCESS;
        }
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}