/**
 * @brief A C implementation imitating C++'s vector<int>
 * 
 * @file ntw_collections.h
 * @author Nikolaos Katomeris, 8551, ngkatomer@auth.gr
 * @date 24-09-2018
 */
#ifndef NTW_COLLECTIONS_H
#define NTW_COLLECTIONS_H 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief The ntw_vector declaration. Use carefully.
 * 
 */
typedef struct NTW_Vector
{
    void** data; /**< The actual vector with the data in memory. */
    uint64_t capacity; /**< The @a data 's memory limit. */
    uint64_t length; /**< The number elements in the vector. */
} ntw_vector;

/**
 * @brief Get the element with @a index of the @a vec.
 * 
 *      Like vec[index]. 
 * 
 * @param vec The vector.
 * @param index The index.
 * @return void* The returned element. Cast to the right type to use.
 */
void* NTW_vector_get(ntw_vector *vec, uint64_t index);

/**
 * @brief Add a new @a element to the @a vec.
 * 
 * @param vec The vector to which end the elemend will be appended.
 * @param element The new element (cast to void*).
 */
void NTW_vector_add(ntw_vector* vec, void* element);

/**
 * @brief Release the vector @a vec from memory. 
 * 
 *  Destructor.
 * 
 * @param vec The vector to be fread.
 */
void NTW_vector_free(ntw_vector *vec); 

#endif