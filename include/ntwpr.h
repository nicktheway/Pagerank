/**
 * @brief Method for calculating the pagerank of web graphs.
 * 
 * @file ntwpr.h
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#ifndef NTWPR_H
#define NTWPR_H 1
#include "ntwpr_wg.h"

/**
 * @brief Calculates the pagerank of the nodes of a web graph.
 * 
 * If @a it_specific is non 0, the @e parameter won't be taken into account and the algorithm will run
 * @a it_specific iterations.
 * Else if @a it_specific is zero, the algorithm will run until it converges to @a e or the number of iterations
 * grows more than a safety limit.
 * 
 * @param webGraph The web graf in ntw_crs representation.
 * @param c The teleportation coefficient.
 * @param e The convergence's tolerance.
 * @param it_specific If not 0, the algorithm will run @a it_specific iterations.
 * @param stream Opened FILE* stream for printing results.
 * @return double* The calculated pagerank vector.
 */
double* NTWPR_pagerank(ntw_crs webGraph[static 1], const double c, const double e, const int32_t it_specific, FILE* stream);

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
 */
void NTWPR_GS_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1]);

#endif