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
void NTWPR_pr_init(const uint32_t nodeNum, float vector[static nodeNum]);

/**
 * @brief Calculates the pagerank of the nodes of a web graph.
 * 
 * @param webGraph The web graf in NTWPR_CRS representation.
 * @param d The teleportation coefficient.
 * @param e The converge tolerance.
 * @return float* The calculated pagerank vector.
 */
float* NTWPR_pagerank(NTWPR_CRS webGraph[static 1], float d, float e);

/**
 * @brief Calculates one iteration of @f$(I-A)x = b@f$
 * 
 * @param matrix The sparse matrix A in CRS form.
 * @param x_vec The vector x that will be replaced.
 * @param b_vec The vector b.
 * @return float The squared norm difference of the old and new @a x_vec.
 */
float NTWPR_GS_iter(const NTWPR_CRS matrix[static 1], float x_vec[static 1], const float b_vec[static 1]);

/**
 * @brief Returns the squered norm of a vector.
 * 
 * @param size The vector's size.
 * @param vector The vector.
 * @return float The vector's second norm squared.
 */
float Vector_sqnorm(uint32_t size, float vector[static size]);
#endif