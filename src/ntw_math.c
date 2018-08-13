/**
 * @brief Definitions of the functions declared at ntw_math.h
 * 
 * @file ntw_math.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "../include/ntw_math.h"

void NTW_multDV(const size_t n, double vector[static n], const double c)
{
    for (size_t i = 0; i < n; i++)
    {
        vector[i] *= c;
    }
}

void NTW_addDV(const size_t n, double vectorA[static n], const double vectorB[static n])
{
    for (size_t i = 0; i < n; i++)
    {
        vectorA[i] += vectorB[i];
    }
}

void NTW_subDV(const size_t n, double vectorA[static n], const double vectorB[static n])
{
    for (size_t i = 0; i < n; i++)
    {
        vectorA[i] -= vectorB[i];
    }
}

double NTW_dotDV(const size_t n, const double vectorA[static n], const double vectorB[static n])
{
    double dot = 0;
    for (size_t i = 0; i < n; i++)
    {
        dot += vectorA[i] * vectorB[i];
    }
    return dot;
}

double NTW_sqMagnDV(const size_t n, const double vector[static n])
{
    double norm = 0;
    for (size_t i = 0; i < n; i++)
    {
        norm += vector[i] * vector[i];
    }
    return norm;
}

double NTW_partialSumDV(const size_t n, const size_t indices[static n], const double vector[static 1])
{
	double sum = 0;
	for (size_t i = 0; i < n; i++)
	{
		sum += vector[indices[i]];
	}
	return sum;
}

void NTW_assignDV(const size_t n, double vectorA[static n], const double vectorB[static n])
{
    for (size_t i = 0; i < n; i++)
    {
        vectorA[i] = vectorB[i];
    }
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
    for (size_t i = 0; i < n; i++)
    {
        vector[i] = value;
    }
    return vector;
}

double* NTW_newCopyDV(const size_t n, const double vector[static n])
{
    double* newCopy = malloc(n * sizeof *newCopy);
    if (!newCopy)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < n; i++)
    {
        newCopy[i] = vector[i];
    }
    return newCopy;
}

void NTW_printDV(FILE* restrict stream, const size_t n, const double vector[static n], const unsigned decimalDigits)
{
    fprintf(stream, "[");
    for (int i = 0; i < n - 1; i++)
    {
        fprintf(stream, "%.*lf\t", decimalDigits, vector[i]);
    }
    fprintf(stream, "%.*lf]\n", decimalDigits, vector[n-1]);
}
