/**
 * @brief Definitions of the functions declared at ntw_math.h
 * 
 * @file ntw_math.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "ntw_math.h"

void NTW_multDV(const size_t n, double vector[static n], const double c)
{
    for (int i = 0; i < n; i++)
    {
        vector[i] *= c;
    }
}

void NTW_addDV(const size_t n, double vectorA[static n], const double vectorB[static n])
{
    for (int i = 0; i < n; i++)
    {
        vectorA[i] += vectorB[i];
    }
}

void NTW_subDV(const size_t n, double vectorA[static n], const double vectorB[static n])
{
    for (int i = 0; i < n; i++)
    {
        vectorA[i] -= vectorB[i];
    }
}

double NTW_dotDV(const size_t n, const double vectorA[static n], const double vectorB[static n])
{
    double dot = 0;
    for (int i = 0; i < n; i++)
    {
        dot += vectorA[i] * vectorB[i];
    }
    return dot;
}

double* NTW_newZeroVectorD(const size_t n)
{
    double* vector = calloc(n, sizeof *vector);
    if (!vector)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    return vector;
}

double* NTW_newUniVectorD(const size_t n, const double value)
{
    double* vector = malloc(n * sizeof *vector);
    if (!vector)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
    {
        vector[i] = value;
    }
    return vector;
}