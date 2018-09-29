/**
 * @brief Parallel implementations of the ntw_math.h functions.
 * 
 * @file ntw_mathp.h
 * @author Nikolaos Katomeris, 8551, ngkatomer@auth.gr
 * @date 30-09-2018
 */
#ifndef NTW_MATHP_H
#define NTW_MATHP_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

/**
 * @brief Difference for two double vars to be safely 
 *          considered equal.
 * 
 */
#define NTWMP_DOUBLE_PRES 1e-30

/**
 * @brief Does the multiplication: \f$\vec{V} = c\vec{V}\f$
 * 
 * Doesn't allocate memory.
 * 
 * @param n The size of the vector. If less only the first @a n elements will be multiplied.
 * @param vector The vector \f$\vec{V}\f$, must be at least @a n elements long. 
 * @param c The constant \f$c\f$.
 */
void NTWMP_multDV(const uint32_t n, double vector[static n], const double c);

/**
 * @brief Does the addition: \f$\vec{A} = \vec{A}+\vec{B}\f$
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be added.
 * @param vectorA The vector \f$\vec{A}\f$.
 * @param vectorB The vector \f$\vec{B}\f$.
 */
void NTWMP_addDV(const uint32_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Does the substitution: \f$\vec{A} = \vec{A}-\vec{B}\f$
 * 
 * @param n The size of the vectors. If less only the first @a n elements will be substituded.
 * @param vectorA The vector \f$\vec{A}\f$.
 * @param vectorB The vector \f$\vec{B}\f$.
 */
void NTWMP_subDV(const uint32_t n, double vectorA[static n], const double vectorB[static n]);

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
double NTWMP_dotDV(const uint32_t n, const double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Returns the squered magnitude of the @a vector.
 * 
 * @param n The size of the vector.
 * @param vector The vector \f$\vec{V}\f$
 * @return double \f$\|\vec{V}\|^{2}\f$
 */
double NTWMP_sqMagnDV(const uint32_t n, const double vector[static n]);

/**
 * @brief Normalizes the @a vector.
 * 
 * @param n The size of the @a vector.
 * @param vector 
 */
void NTWMP_normalizeDV(const uint32_t n, double vector[static n]);

/**
 * @brief Normalizes the @a vector so that its elements sum to 1.
 * 
 * @param n The size of the @a vector.
 * @param vector 
 */
void NTWMP_normalizeSumDV(const uint32_t n, double vector[static n]);

/**
 * @brief Assign the values of vectorB to vectorA
 *          \f[\vec{A} = \vec{B}\f]
 * 
 * @param n The size of the two vectors.
 * @param vectorA The \f$\vec{A}\f$.
 * @param vectorB The \f$\vec{B}\f$.
 */
void NTWMP_assignDV(const uint32_t n, double vectorA[static n], const double vectorB[static n]);

/**
 * @brief Creates a @a n legth vector with all elements set to @a value.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size \f$n\f$ of the vector.
 * @param value The value \f$v\f$ of each element.
 * @return double* Pointer to the vector: \f$\overbrace{\begin{pmatrix}v & v & \cdots &v\end{pmatrix}}^{n}\f$.
 */
double* NTWMP_newUniVectorD(const uint32_t n, const double value);

/**
 * @brief Create a copy/subcopy of the @a vector.
 * 
 * Allocates memory that should be freed later.
 * 
 * @param n The size of the copied vector;
 * @param vector 
 * @return double* Pointer to the new copy.
 */
double* NTWMP_newCopyDV(const uint32_t n, const double vector[static n]);


#endif
