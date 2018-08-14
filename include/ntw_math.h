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
 * @brief Difference for two double vars to be safely 
 *          considered equal.
 * 
 */
#define NTWM_DOUBLE_PRES 1e-30

/**
 * @brief Does the multiplication: \f$\vec{V} = c\vec{V}\f$
 * 
 * Doesn't allocate memory.
 * 
 * @param n The size of the vector. If less only the first @a n elements will be multiplied.
 * @param vector The vector \f$\vec{V}\f$, must be at least @a n elements long. 
 * @param c The constant \f$c\f$.
 */
void NTWM_multDV(const size_t n, double vector[static n], const double c);

/**
 * @brief Does the addition: \f$\vec{A} = \vec{A}+\vec{B}\f$
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be added.
 * @param vectorA The vector \f$\vec{A}\f$.
 * @param vectorB The vector \f$\vec{B}\f$.
 */
void NTWM_addDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Does the substitution: \f$\vec{A} = \vec{A}-\vec{B}\f$
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be substituded.
 * @param vectorA The vector \f$\vec{A}\f$.
 * @param vectorB The vector \f$\vec{B}\f$.
 */
void NTWM_subDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

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
double NTWM_dotDV(const size_t n, const double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Returns the squered magnitude of the @a vector.
 * 
 * @param n The size of the vector.
 * @param vector The vector \f$\vec{V}\f$
 * @return double \f$\|\vec{V}\|^{2}\f$
 */
double NTWM_sqMagnDV(const size_t n, const double vector[static n]);
<<<<<<< HEAD

/**
 * @brief Normalizes the @a vector.
 * 
 * @param n The size of the @a vector.
 * @param vector 
 */
void NTWM_normalizeDV(const size_t n, double vector[static n]);
=======
>>>>>>> 9bb2f511c0262561d9a5ddc42c75fce9d058a3ea

/**
 * @brief Returns \f$\sum_{i=0}^{n}V(ind[i])\f$
 * 
 * @param n The number \f$n\f$  of the sumation elements.
 * @param indices The @a n indices of the vector that will be in the sum.
 * @param vector The vector \f$\vec{V}\f$
 * @return double The partial sum of the @a vector elements.
 */
double NTWM_partialSumDV(const size_t n, const size_t indices[static n], const double vector[static 1]);

/**
 * @brief Assign the values of vectorB to vectorA
 *          \f[\vec{A} = \vec{B}\f]
 * 
 * @param n The size of the two vectors.
 * @param vectorA The \f$\vec{A}\f$.
 * @param vectorB The \f$\vec{B}\f$.
 */
void NTWM_assignDV(const size_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Creates a @a n length zero vector.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size of the vector.
 * @return double* Pointer to the vector: \f$\overbrace{\begin{pmatrix}0 & 0 & \cdots &0\end{pmatrix}}^{n}\f$.
 */
double* NTWM_newZeroVectorD(const size_t n);

/**
 * @brief Creates a @a n legth vector with all elements set to @a value.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size \f$n\f$ of the vector.
 * @param value The value \f$v\f$ of each element.
 * @return double* Pointer to the vector: \f$\overbrace{\begin{pmatrix}v & v & \cdots &v\end{pmatrix}}^{n}\f$.
 */
double* NTWM_newUniVectorD(const size_t n, const double value);

/**
 * @brief Create a copy/subcopy of the @a vector.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size of the copied vector;
 * @param vector 
 * @return double* Pointer to the new copy.
 */
double* NTWM_newCopyDV(const size_t n, const double vector[static n]);

/**
 * @brief Prints a double @a vector to the @a stream.
 * 
 * @param stream The stream where the @a vector will be printed.
 * @param n The size of the @a vector.
 * @param vector The vector that will be printed.
 * @param decimalDigits The accuracy of the printed elements.
 */
void NTWM_printDV(FILE* restrict stream, const size_t n, const double vector[static n], const unsigned decimalDigits);

#endif
