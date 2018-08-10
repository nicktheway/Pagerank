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
inline void NTWPR_pr_init(const uint32_t nodeNum, float vector[static nodeNum]);

/**
 * @brief Calculates the pagerank of the nodes of a web graph.
 * 
 * @param webGraph The web graf in NTWPR_CRS representation.
 * @param d The teleportation coefficient.
 * @param e The converge tolerance.
 * @return float* The calculated pagerank vector.
 */
float* NTWPR_pagerank(NTWPR_CRS webGraph[static 1], float d, float e);

#endif