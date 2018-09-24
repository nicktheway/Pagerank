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
 * @brief The ntw_vector declaration.
 * 
 */
typedef struct NTW_Vector
{
    void** data; /**< The actual vector with the data in memory. */
    uint64_t capacity; /**< The @a data 's memory limit. */
    uint64_t length; /**< The number elements in the vector. */
} ntw_vector;

void* NTW_vector_get(ntw_vector *vec, uint64_t index);
void NTW_vector_add(ntw_vector* vec, void* element);
void NTW_vector_free(ntw_vector *vec); 

#endif