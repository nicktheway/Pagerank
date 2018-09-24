#include "../include/ntw_collections.h"

void* NTW_vector_get(ntw_vector *vec, uint64_t index)
{
	if (index >= vec->length) 
    {
        fprintf(stderr, "%s: Tried to access out of bounds element\n", __func__);
		exit(EXIT_FAILURE);
	}

	return vec->data[index];
}

void NTW_vector_add(ntw_vector* vec, void* element)
{
	if (vec->capacity == 0) 
    {
		vec->capacity = 4;
		vec->data = calloc(vec->capacity, sizeof *vec->data);
        if (!vec->data) 
        {
            fprintf(stderr, "%s: Error at allocating memory for the vector.\n", __func__);
            exit(EXIT_FAILURE);
        }
	}

	if (vec->length == vec->capacity) 
    {
		vec->capacity *= 2;
		vec->data = realloc(vec->data, vec->capacity * sizeof *vec->data);
        if (!vec->data) 
        {
            fprintf(stderr, "%s: Error at allocating memory for the vector.\n", __func__);
            exit(EXIT_FAILURE);
        }
	}

	vec->data[vec->length] = element;
	vec->length++;
}


void NTW_vector_free(ntw_vector *vec)
{
	free(vec->data);
}