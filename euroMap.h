#ifndef EURO_MAP_H_
#define EURO_MAP_H_


#include <stdbool.h>

/**
* Generic Map Container
*
* Special implementation of a map container type for the eurovision program.
* The map has an internal iterator for external use. For all functions
* where the state of the iterator after calling that function is not stated,
* it is undefined. That is you cannot assume anything about it.
*
* The following functions are available:
*   mapCreate		- Creates a new empty map
*   mapDestroy		- Deletes an existing map and frees all resources
*   mapCopy		- Copies an existing map
*   mapGetSize		- Returns the size of a given map
*   mapContains	- returns weather or not a key exists inside the map.
*   				  This resets the internal iterator.
*   mapPut		    - Gives a specific key a given value.
*   				  If the key exists, the value is overridden.
*   				  This resets the internal iterator.
*   mapGet  	    - Returns the data paired to a key which matches the given key.
*					  Iterator status unchanged
*   mapRemove		- Removes a pair of (key,data) elements for which the key
*                    matches a given element (by the key compare function).
*   				  This resets the internal iterator.
*   mapGetFirst	- Sets the internal iterator to the first key in the
*   				  map, and returns it.
*   mapGetNext		- Advances the internal iterator to the next key and
*   				  returns it.
*	 mapClear		- Clears the contents of the map. Frees all the elements of
*	 				  the map using the free function.
*	 mapMaxData     - find the element in the map with the biggest value.
*	 mapCopyOnlyKeys- Creates a copy of target map but replace the data element
*                     type and reset all data element to default value.
* 	 MAP_FOREACH	- A macro for iterating over the map's elements.
*/

/** Type for defining the map */
typedef struct Map_t *Map;

/** Type used for returning error codes from map functions */
typedef enum MapResult_t {
    MAP_SUCCESS,
    MAP_OUT_OF_MEMORY,
    MAP_NULL_ARGUMENT,
    MAP_ITEM_ALREADY_EXISTS,
    MAP_ITEM_DOES_NOT_EXIST
} MapResult;

/** Data element data type for map container */
typedef void *MapDataElement;

/** Key element data type for map container */
typedef void *MapKeyElement;

/** Type of function for copying a data element of the map */
typedef MapDataElement(*copyMapDataElements)(MapDataElement);

/** Type of function for copying a key element of the map */
typedef MapKeyElement(*copyMapKeyElements)(MapKeyElement);

/** Type of function for deallocating a data element of the map */
typedef void(*freeMapDataElements)(MapDataElement);

/** Type of function for deallocating a key element of the map */
typedef void(*freeMapKeyElements)(MapKeyElement);


/**
* Type of function used by the map to identify equal key elements.
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
typedef int(*compareMapKeyElements)(MapKeyElement, MapKeyElement);

/**
* Type of function used by the map to identify equal data elements.
* This function should return:
* 		A positive integer if the first element is greater;
* 		0 if they're equal;
*		A negative integer if the second element is greater.
*/
typedef int(*compareMapDataElements)(MapDataElement, MapDataElement);

/**
* mapCreate: Allocates a new empty map.
*
* @param copyDataElement - Function pointer to be used for copying data elements into
*  	the map or when copying the map.
* @param copyKeyElement - Function pointer to be used for copying key elements into
*  	the map or when copying the map.
* @param freeDataElement - Function pointer to be used for removing data elements from
* 		the map
* @param freeKeyElement - Function pointer to be used for removing key elements from
* 		the map
* @param compareKeyElements - Function pointer to be used for comparing key elements
* 		inside the map. Used to check if new elements already exist in the map.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Map in case of success.
*/
Map euroMapCreate(copyMapDataElements copyDataElement,
                  copyMapKeyElements copyKeyElement,
                  freeMapDataElements freeDataElement,
                  freeMapKeyElements freeKeyElement,
                  compareMapKeyElements compareKeyElements);

/**
* mapDestroy: Deallocates an existing map. Clears all elements by using the
* stored free functions.
*
* @param map - Target map to be deallocated. If map is NULL nothing will be
* 		done
*/
void EuroMapDestroy(Map map);

/**
* mapCopy: Creates a copy of target map.
* Iterator values for both maps is undefined after this operation.
*
* @param map - Target map.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Map containing the same elements as map otherwise.
*/
Map EuroMapCopy(Map map);

/**
* mapGetSize: Returns the number of elements in a map
* @param map - The map which size is requested
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the number of elements in the map.
*/
int EuroMapGetSize(Map map);

/**
* mapContains: Checks if a key element exists in the map. The key element will be
* considered in the map if one of the key elements in the map it determined equal
* using the comparison function used to initialize the map.
*
* @param map - The map to search in
* @param element - The element to look for. Will be compared using the
* 		comparison function.
* @return
* 	false - if one or more of the inputs is null, or if the key element was not found.
* 	true - if the key element was found in the map.
*/
bool EuroMapContains(Map map, MapKeyElement element);

/**
*	mapPut: Gives a specified key a specific value.
*  Iterator's value is undefined after this operation.
*
* @param map - The map for which to reassign the data element
* @param keyElement - The key element which need to be reassigned
* @param dataElement - The new data element to associate with the given key.
*      A copy of the element will be inserted as supplied by the copying function
*      which is given at initialization and old data memory would be
*      deleted using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent as map
* 	MAP_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
* 	MAP_SUCCESS the paired elements had been inserted successfully
*/
MapResult EuroMapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement);

/**
*	mapGet: Returns the data associated with a specific key in the map.
*			Iterator status unchanged
*
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found and whos data
we want to get.
* @return
*  NULL if a NULL pointer was sent or if the map does not contain the requested key.
* 	The data element associated with the key otherwise.
*/
MapDataElement EuroMapGet(Map map, MapKeyElement keyElement);

/**
* 	mapRemove: Removes a pair of key and data elements from the map. The elements
*  are found using the comparison function given at initialization. Once found,
*  the elements are removed and deallocated using the free functions
*  supplied at initialization.
*  Iterator's value is undefined after this operation.
*
* @param map -
* 	The map to remove the elements from.
* @param keyElement
* 	The key element to find and remove from the map. The element will be freed using the
* 	free function given at initialization. The data element associated with this key
*  will also be freed using the free function given at initialization.
* @return
* 	MAP_NULL_ARGUMENT if a NULL was sent to the function
*  MAP_ITEM_DOES_NOT_EXIST if an equal key item does not already exists in the map
* 	MAP_SUCCESS the paired elements had been removed successfully
*/
MapResult EuroMapRemove(Map map, MapKeyElement keyElement);

/**
*	mapGetFirst: Sets the internal iterator (also called current key element) to
*	the first key element in the map. There doesn't need to be an internal order
*  of the keys so the "first" key element is any key element.
*	Use this to start iterating over the map.
*	To continue iteration use mapGetNext
*
* @param map - The map for which to set the iterator and return the first
* 		key element.
* @return
* 	NULL if a NULL pointer was sent or the map is empty.
* 	The first key element of the map otherwise
*/
MapKeyElement EuroMapGetFirst(Map map);

/**
*	mapGetNext: Advances the map iterator to the next key element and returns it.
*	The next key element is any key element not previously returned by the iterator.
* @param map - The map for which to advance the iterator
* @return
* 	NULL if reached the end of the map, or the iterator is at an invalid state
* 	or a NULL sent as argument
* 	The next key element on the map in case of success
*/
MapKeyElement EuroMapGetNext(Map map);


/**
* mapClear: Removes all key and data elements from target map.
* The elements are deallocated using the stored free functions.
* @param map
* 	Target map to remove all element from.
* @return
* 	MAP_NULL_ARGUMENT - if a NULL pointer was sent.
* 	MAP_SUCCESS - Otherwise.
*/
MapResult EuroMapClear(Map map);

/**
 * mapMaxData: find the element in the map with the biggest value and return
 * its key. use to supplied compareMapDataElements function to compare data
 * elements.
 * ADDED FOR QUESTION 2, NOT A PART OF QUESTION 1
 * @param map - map instance to search within
 * @param compare - function to compare between two data elements
 * @return
 *  NULL if the map is empty
 *  the key of the data element with the biggest value otherwise
 */
MapKeyElement EuroMapMaxData(Map map, compareMapDataElements compare);

/**
 * mapCopyOnlyKeys: Creates a copy of target map but replace the data element
 * type and reset all data element to default value.
 * ADDED FOR QUESTION 2, NOT A PART OF QUESTION 1
 * @param map - Target map.
 * @param newCopyData - Function pointer to be used for copying data elements into
*  	the map or when copying the map.
 * @param newFreeData - Function pointer to be used for removing data elements from
* 		the map
 * @param defaultValue - the default value for all the data elements in the map
 * @return
 * 	NULL if a NULL was sent or a memory allocation failed.
 * 	A Map containing the required elements as map otherwise.
 */
Map EuroMapCopyOnlyKeys(Map map, copyMapDataElements newCopyData,
                        freeMapDataElements newFreeData, MapDataElement defaultValue);

/**
 * SetIterator - set a map's iterator to a specific point according to a key
 * given to the function.
 * @param map - a map object
 * @param spot - a key element for the iterator to point to
 * @return
 * MAP_SUCCESS if the iterator points to spot
 * MAP_ITEM_DOES_NOT_EXIST if spot doesn't exist in map
 */
MapResult EuroMapSetIterator(Map map, MapKeyElement spot);
/*!
* Macro for iterating over a map.
* Declares a new iterator for the loop.
*/
#define MAP_FOREACH(type, iterator, map) \
    for(type iterator = (type) EuroMapGetFirst(map) ; \
        iterator ;\
        iterator = EuroMapGetNext(map))

#endif /* EURO_MAP_H_ */