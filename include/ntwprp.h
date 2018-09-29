/**
 * @brief Parallel method for calculating the pagerank of web graphs.
 * 
 * @file ntwprp.h
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 30-09-2018
 */
#ifndef NTWPRP_H
#define NTWPRP_H 1
#include "ntwpr_wg.h"
#include <omp.h>

/**
 * @brief Calculates the pagerank of the nodes of a web graph.
 * 
 * @param webGraph The web graf in ntw_crs representation.
 * @param c The teleportation coefficient.
 * @param e The convergence's tolerance.
 * @param colors The color groups of the nodes.
 * @param stream Opened FILE* stream for printing results.
 * @return double* The calculated pagerank vector.
 */
double* NTWPR_colored_pagerank(ntw_crs webGraph[static 1], const double c, const double e, const ntw_vector* const colors, FILE* stream);

/**
 * @brief Calculates one iteration of @f$(I-A)x = b@f$
 * 
 * Uses the Gauss-Sneidel method, using forward substitution.
 * 
 * \f[\vec{x}_i^{k+1} = \frac{1}{1-a_{ii}}(b_i - \sum_{j=1}^{i-1} a_{ij}\vec{x}_j^{k+1} - \sum_{j=i+1}^{n} a_{ij}\vec{x}_j^k)\f]
 * 
 * @param matrix The sparse matrix A in CRS form.
 * @param x_vec The vector x that will be replaced.
 * @param b_vec The vector b.
 * @param colors The color groups of the nodes.
 */
void NTWPR_GS_parallel_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1], const ntw_vector* const colors);

/**
 * @brief Returns the squered norm of a vector.
 * 
 * @param size The vector's size.
 * @param vector The vector.
 * @return double The vector's second norm squared.
 */
double Vector_sqnorm(uint32_t size, double vector[static size]);
#endif
