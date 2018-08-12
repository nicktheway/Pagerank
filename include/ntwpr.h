/**
 * @brief Methods for calculating the pagerank of web graphs.
 * 
 * @file ntwpr.h
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#ifndef NTWPR_H
#define NTWPR_H 1
#include "ntwpr_wg.h"

/**
 * @brief Initializes the pagerank vector uniformely.
 * 
 * @param nodeNum The number of nodes (size of the vector).
 * @param vector The vector that will be initialized.
 */
void NTWPR_pr_init(const uint32_t nodeNum, double vector[static nodeNum]);

/**
 * @brief Calculates the pagerank of the nodes of a web graph.
 * 
 * @param webGraph The web graf in ntw_crs representation.
 * @param d The teleportation coefficient.
 * @param e The converge tolerance.
 * @return double* The calculated pagerank vector.
 */
double* NTWPR_pagerank(ntw_crs webGraph[static 1], double d, double e);

/**
 * @brief Calculates one iteration of @f$(I-A)x = b@f$
 * 
 * Uses the Gauss-Sneidel method, using forward substitution.
 * 
 * \f[\vec{x}^{k+1} = \frac{1}{1-a_{ii}}(b_i - \sum_{j=1}^{i-1} a_{ij}\vec{x}_j^k + \sum_{j=i+1}^{n} a_{ij}\vec{x}_j^k)\f]
 * 
 * @param matrix The sparse matrix A in CRS form.
 * @param x_vec The vector x that will be replaced.
 * @param b_vec The vector b.
 * @return double The squared norm difference of the old and new @a x_vec.
 */
double NTWPR_GS_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1]);

/**
 * @brief Returns the squered norm of a vector.
 * 
 * @param size The vector's size.
 * @param vector The vector.
 * @return double The vector's second norm squared.
 */
double Vector_sqnorm(uint32_t size, double vector[static size]);
#endif