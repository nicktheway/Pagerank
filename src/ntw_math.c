/**
 * @brief Definitions of the functions declared at ntw_math.h
 * 
 * @file ntw_math.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "../include/ntw_math.h"
#include <math.h>

void NTWM_multDV(const uint32_t n, double vector[static n], const double c)
{
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] *= c;
    }
}

void NTWM_addDV(const uint32_t n, double vectorA[static n], const double vectorB[static n])
{
    for (uint32_t i = 0; i < n; i++)
    {
        vectorA[i] += vectorB[i];
    }
}

void NTWM_subDV(const uint32_t n, double vectorA[static n], const double vectorB[static n])
{
    for (uint32_t i = 0; i < n; i++)
    {
        vectorA[i] -= vectorB[i];
    }
}

double NTWM_dotDV(const uint32_t n, const double vectorA[static n], const double vectorB[static n])
{
    double dot = 0;
    for (uint32_t i = 0; i < n; i++)
    {
        dot += vectorA[i] * vectorB[i];
    }
    return dot;
}

double NTWM_sqMagnDV(const uint32_t n, const double vector[static n])
{
    double norm = 0;
    for (uint32_t i = 0; i < n; i++)
    {
        norm += vector[i] * vector[i];
    }
    return norm;
}

void NTWM_normalizeDV(const uint32_t n, double vector[static n])
{
    double norm = 0;
    for (uint32_t i = 0; i < n; i++)
    {
        norm += vector[i] * vector[i];
    }
    
    if (fabs(norm - 0) < NTWM_DOUBLE_PRES) return;
    
    norm = sqrt(norm);
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] /= norm;
    }
}

double NTWM_partialSumDV(const uint32_t n, const uint32_t indices[static n], const double vector[static 1])
{
	double sum = 0;
	for (uint32_t i = 0; i < n; i++)
	{
		sum += vector[indices[i]];
	}
	return sum;
}

void NTWM_assignDV(const uint32_t n, double vectorA[static n], const double vectorB[static n])
{
    for (uint32_t i = 0; i < n; i++)
    {
        vectorA[i] = vectorB[i];
    }
}

void NTWM_reshapeDV(const uint32_t n, double vector[static n], const uint32_t sequence[static n])
{
    double* temp = calloc(n, sizeof *temp);
    if (!temp)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < n; i++)
    {
        temp[i] = vector[sequence[i]];
    }
    NTWM_assignDV(n, vector, temp);

    free(temp);
}

double* NTWM_newZeroVectorD(const uint32_t n)
{
    double* vector = calloc(n, sizeof *vector);
    if (!vector)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    return vector;
}

double* NTWM_newUniVectorD(const uint32_t n, const double value)
{
    double* vector = malloc(n * sizeof *vector);
    if (!vector)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] = value;
    }
    return vector;
}

double* NTWM_newCopyDV(const uint32_t n, const double vector[static n])
{
    double* newCopy = malloc(n * sizeof *newCopy);
    if (!newCopy)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < n; i++)
    {
        newCopy[i] = vector[i];
    }
    return newCopy;
}

void NTWM_printDV(FILE* restrict stream, const uint32_t n, const double vector[static n], const unsigned decimalDigits)
{
    fprintf(stream, "[");
    for (int i = 0; i < n - 1; i++)
    {
        fprintf(stream, "%.*lf\t", decimalDigits, vector[i]);
    }
    fprintf(stream, "%.*lf]\n", decimalDigits, vector[n-1]);
}
