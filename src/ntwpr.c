/**
 * @brief File containing the definitions of the functions declared at ntwpr.h
 * 
 * @file ntwpr.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "../include/ntwpr.h"
#include "../include/ntw_math.h"

double* NTWPR_pagerank(ntw_crs webGraph[static 1], double d, double e)
{
    // For better readability.
    const uint32_t wgSize = webGraph->node_num;

    // Make the matrix a probability matrix.
    //NTW_CRS_rowNormUnif(webGraph);
    NTW_CRS_colNorm(webGraph);

    // Multiply with the teleportation coefficient.
    NTW_CRS_cmult(webGraph, -d);
    NTW_CRS_printFullMatrix(stdout, webGraph);
    // Create the initial pagerank vector (unified) and the b vector.
    double* pagerank = NTW_newUniVectorD(wgSize, 1.0 / wgSize);

    double* b = NTW_newUniVectorD(wgSize, (1-d)/wgSize);
    NTW_printDV(stdout, wgSize, b, 4);
    double delta = 1.0;
    int max_iterations = 50, curr_iteration = 0;
    while (delta > e && curr_iteration++ < max_iterations)
    {
        delta = NTWPR_GS_iter(webGraph, pagerank, b);
    }
    printf("ci: %d\tdelta = %0.3f\n", curr_iteration, delta);

    free(b);
    return pagerank;
}

inline double NTWPR_GS_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1])
{
    double sqnorm_diff = 0;
    for (uint32_t i = 0; i < matrix->node_num; i++)
    {
        double den = 1.0;
        double num = b_vec[i];
        const double old_xi = x_vec[i];

        for (uint32_t j = matrix->row_ptr[i]; j < matrix->row_ptr[i+1]; j++)
        {
            if (matrix->col_ind[j] == i)
            {
                den += matrix->val[j];
            }
            else
            {
                num -= matrix->val[j] * x_vec[matrix->col_ind[j]];
            }
        }
        // printf("\nnum = %0.3f\t den=%0.3f\n", num, den);
        x_vec[i] = num / den;
        sqnorm_diff += (x_vec[i] - old_xi) * (x_vec[i] - old_xi);
    }
    return sqnorm_diff;
}

void NTWPR_pr_init(const uint32_t nodeNum, double vector[static nodeNum])
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
