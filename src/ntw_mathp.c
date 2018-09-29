/**
 * @brief 
 * 
 * @file ntw_mathp.c
 * @author Nikolaos Katomeris, 8551, ngkatomer@auth.gr
 * @date 30-09-2018
 */
#include "../include/ntw_mathp.h"
#include <math.h>

void NTWMP_multDV(const uint32_t n, double vector[static n], const double c)
{
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] *= c;
    }
}

void NTWMP_addDV(const uint32_t n, double vectorA[static n], const double vectorB[static n])
{
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vectorA[i] += vectorB[i];
    }
}

void NTWMP_subDV(const uint32_t n, double vectorA[static n], const double vectorB[static n])
{
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vectorA[i] -= vectorB[i];
    }
}

double NTWMP_dotDV(const uint32_t n, const double vectorA[static n], const double vectorB[static n])
{
    double dot = 0;
    #pragma omp parallel for reduction (+:dot)
    for (uint32_t i = 0; i < n; i++)
    {
        dot += vectorA[i] * vectorB[i];
    }
    return dot;
}

double NTWMP_sqMagnDV(const uint32_t n, const double vector[static n])
{
    double norm = 0;
    #pragma omp parallel for reduction (+:norm)
    for (uint32_t i = 0; i < n; i++)
    {
        norm += vector[i] * vector[i];
    }
    return norm;
}

void NTWMP_normalizeDV(const uint32_t n, double vector[static n])
{
    double norm = 0;
    #pragma omp parallel for reduction (+:norm)
    for (uint32_t i = 0; i < n; i++)
    {
        norm += vector[i] * vector[i];
    }
    
    if (fabs(norm) < NTWMP_DOUBLE_PRES)
    {
        fprintf(stderr, "%s: The vector is zero, didn't normalize.\n", __func__);
        return;
    }
    
    norm = sqrt(norm);
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] /= norm;
    }
}

void NTWMP_normalizeSumDV(const uint32_t n, double vector[static n])
{
    double norm = 0;
    #pragma omp parallel for reduction (+:norm)
    for (uint32_t i = 0; i < n; i++)
    {
        norm += vector[i];
    }
    
    if (fabs(norm) < NTWMP_DOUBLE_PRES)
    {
        fprintf(stderr, "%s: Vector elements add up to zero, didn't normalize.\n", __func__);
        return;
    }

    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] /= norm;
    }
}

void NTWMP_assignDV(const uint32_t n, double vectorA[static n], const double vectorB[static n])
{
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vectorA[i] = vectorB[i];
    }
}

double* NTWMP_newUniVectorD(const uint32_t n, const double value)
{
    double* vector = malloc(n * sizeof *vector);
    if (!vector)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        vector[i] = value;
    }
    
    return vector;
}

double* NTWMP_newCopyDV(const uint32_t n, const double vector[static n])
{
    double* newCopy = malloc(n * sizeof *newCopy);
    if (!newCopy)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel for
    for (uint32_t i = 0; i < n; i++)
    {
        newCopy[i] = vector[i];
    }
    return newCopy;
}