/**
 * @brief File containing the definitions of the functions declared at ntwpr.h
 * 
 * @file ntwpr.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "ntwpr.h"

float* NTWPR_pagerank(NTWPR_CRS webGraph[static 1], float d, float e)
{
    NTWPR_CRS_unified_rnorm(webGraph);
    float* pagerank = malloc(webGraph->node_num * sizeof(*pagerank));
    if (!pagerank)
    {
        fprintf(stderr, "%s: Error at allocating memory for the pagerank vector.\n", __func__);

        exit(EXIT_FAILURE);
    }
    // Initialize the pagerank vector uniformely.
    NTWPR_pr_init(webGraph->node_num, pagerank);

    // Multiply with the negative of the teleportation coefficient.
    NTWPR_CRS_const_mult(webGraph, -d);

    float delta = 1.0f;
    int max_iterations = 500, curr_iteration = 0;
    while (delta > e && curr_iteration++ < max_iterations)
    {
        
    }


    return (void *) 0;
}

inline void NTWPR_pr_init(const uint32_t nodeNum, float vector[static nodeNum])
{
    if (!nodeNum) 
    {
        fprintf(stderr, "nodeNum must be positive!\n");
        exit(EXIT_FAILURE);
    }
    // Compiler will optimize out the division at every iteration.
    for (uint32_t i = 0; i < nodeNum; i++)
    {
        vector[i] = 1 / nodeNum;
    }
}
