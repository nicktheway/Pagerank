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
    float* v = malloc(webGraph->node_num * sizeof(*v));
    float* pagerank = malloc(webGraph->node_num * sizeof(*pagerank));
    if (!pagerank || !v)
    {
        fprintf(stderr, "%s: Error at allocating memory for the pagerank vector.\n", __func__);

        exit(EXIT_FAILURE);
    }
    // Initialize the pagerank vector uniformely.
    NTWPR_pr_init(webGraph->node_num, pagerank);
    NTWPR_pr_init(webGraph->node_num, v);

    for (int i = 0; i < webGraph->node_num; i++)
    {
        v[i] *= 1-d;
    }

    // Multiply with the negative of the teleportation coefficient.
    NTWPR_CRS_const_mult(webGraph, -d);

    float delta = 1.0f;
    int max_iterations = 500, curr_iteration = 0;
    while (delta > e && curr_iteration++ < max_iterations)
    {
        delta = NTWPR_GS_iter(webGraph, pagerank, v);
    }
    printf("ci: %d\tdelta = %0.3f\n", curr_iteration, delta);

    return pagerank;
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
        vector[i] = 1.0f / nodeNum;
    }
}

inline float NTWPR_GS_iter(const NTWPR_CRS matrix[static 1], float x_vec[static 1], const float b_vec[static 1])
{
    float sqnorm_diff = 0;
    for (uint32_t i = 0; i < matrix->node_num; i++)
    {
        float den = 1.0f;
        float num = b_vec[i];
        const float old_x = x_vec[i];

        for (uint32_t j = matrix->row_ptr[i]; j < matrix->row_ptr[i+1]; j++)
        {
            if (j == i)
            {
                den += matrix->val[j];
            } 
            else
            {
                num -= matrix->val[j] * x_vec[matrix->col_ind[j]];
                // printf("num == %0.3f\t", x_vec[matrix->col_ind[j]]);
            }
        }
        // printf("\nnum = %0.3f\t den=%0.3f\n", num, den);
        x_vec[i] = num / den;
        sqnorm_diff += (x_vec[i] - old_x) * (x_vec[i] - old_x);
    }
    return sqnorm_diff;
}

inline float Vector_sqnorm(uint32_t size, float vector[static size])
{
    float norm = 0;
    for (uint32_t i = 0; i < size; i++)
    {
        norm += vector[i] * vector[i];
    }
    return norm;
} 