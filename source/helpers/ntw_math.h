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

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Does the multiplication: \f$\vec{V} = c\vec{V}\f$
 * 
 * Doesn't allocate memory.
 * 
 * @param n The size of the vector. If less only the first @a n elements will be multiplied.
 * @param vector The vector \f$\vec{V}\f$, must be at least @a n elements long. 
 * @param c The constant \f$c\f$.
 */
void NTW_multDV(const size_t n, double vector[static n], const double c);

/**
 * @brief Does the addition: \f$\vec{A} = \vec{A}+\vec{B}\f$
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be added.
 * @param vectorA The vector \f$\vec{A}\f$.
 * @param vectorB The vector \f$\vec{B}\f$.
 */
void NTW_addDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Does the substitution: \f$\vec{A} = \vec{A}-\vec{B}\f$
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be substituded.
 * @param vectorA The vector \f$\vec{A}\f$.
 * @param vectorB The vector \f$\vec{B}\f$.
 */
void NTW_subDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Calculates the dot product of @a vectorA and @a vectorB.
 * 
 * If @a n is less than the actual size of the vectors the dot product will
 * be calculated as if the vectors where reduced to their first n elements.
 * 
 * @param n The size of the arrays.
 * @param vectorA The vector \f$\vec{A}\f$
 * @param vectorB The vector \f$\vec{B}\f$
 * @return double The dot product \f$\vec{A} \cdot \vec{B}\f$
 */
double NTW_dotDV(const size_t n, const double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Creates a @a n length zero vector.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size of the vector.
 * @return double* Pointer to the vector: \f$\overbrace{\begin{pmatrix}0 & 0 & \cdots &0\end{pmatrix}}^{n}\f$.
 */
double* NTW_newZeroVectorD(const size_t n);

/**
 * @brief Creates a @a n legth vector with all elements set to @a value.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size \f$n\f$ of the vector.
 * @param value The value \f$v\f$ of each element.
 * @return double* Pointer to the vector: \f$\overbrace{\begin{pmatrix}v & v & \cdots &v\end{pmatrix}}^{n}\f$.
 */
double* NTW_newUniVectorD(const size_t n, const double value);

#endif