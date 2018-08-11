/**
 * @brief Header containing useful math functions, especially
 *          for vectors.
 * 
 * @file ntw_math.h
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */

#ifndef NTW_MATH_H
#define NTW_MATH_H 1

#include <stdio.h> // for perror
#include <stdlib.h> // for size_t

/**
 * @brief Multiplies the @a vector by the constant @a c. Stores the reuslt
 *          in place.
 * 
 * Doesn't allocate memory.
 * 
 * @param n The size of the vector. If less only the first @a n elements will be multiplied.
 * @param vector The vector, must be at least @a n elements long. 
 * @param c The constant.
 */
void NTW_multDV(const size_t n, double vector[static n], const double c);

/**
 * @brief Adds the vectors @a vectorA and @a vectorB and stores the result
 *          in the @a vectorA
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be added.
 * @param vectorA The first  vector that will be modified.
 * @param vectorB The second vector.
 */
void NTW_addDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Subs the @a vectorB from the @a vectorA and stores the result
 *          in the @a vectorA
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be substituded.
 * @param vectorA The first  vector that will be modified.
 * @param vectorB The second vector.
 */
void NTW_subDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Calculates the dot product of @a vectorA and @a vectorB.
 * 
 * If @a n is less than the actual size of the vectors the dot product will
 * be calculated as if the vectors where reduced to their first n elements.
 * 
 * @param n The size of the arrays.
 * @param vectorA 
 * @param vectorB 
 * @return double The dot product @f$\bold{vectorA} \cdot \bold{vectorB} @f$
 */
double NTW_dotDV(const size_t n, const double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Creates a @a n length zero vector.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size of the vector.
 * @return double* Pointer to the vector.
 */
double* NTW_newZeroVectorD(const size_t n);

/**
 * @brief Creates a @a n legth vector with all elements set to @a value.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size of the vector.
 * @param value The value of each elements.
 * @return double* Pointer to the vector.
 */
double* NTW_newUniVectorD(const size_t n, const double value);

#endif