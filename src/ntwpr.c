/**
 * @brief File containing the definitions of the functions declared at ntwpr.h
 * 
 * @file ntwpr.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "../include/ntwpr.h"
#include "../include/ntw_math.h"

double* NTWPR_pagerank(ntw_crs webGraph[static 1], double c, double e, FILE* stream)
{
    // For more readable code wgSize <- webGraph->node_num
    const uint32_t wgSize = webGraph->node_num;

	/** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */

    // Make the matrix a probability matrix.
    NTW_CRS_colNorm(webGraph);
	/** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */
    
    // Multiply with the teleportation coefficient.
    NTW_CRS_cmult(webGraph, -c);
    /** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */

    // Create the initial pagerank vector (unified) and the b vector.
    double* pagerank = NTWM_newUniVectorD(wgSize, 1.0 / wgSize);
	
	// Create the b vector.
    double* b = NTWM_newUniVectorD(wgSize, (1 - c) / wgSize);
    /** DEBUG:* NTWM_printDV(stdout, wgSize, b, 4); */

	// Start the Gauss-Sneidel Algorithm.
    double delta = 1.0;
    uint32_t emptyRows;
    size_t *d = NTW_CRS_getEmptyRowIndices(webGraph, &emptyRows);
    
    unsigned max_iterations = 50, curr_iteration = 0;
    while (delta > e && curr_iteration++ < max_iterations)
    {
        delta = NTWPR_GS_iter(webGraph, pagerank, b, emptyRows, d);
    }
    fprintf(stdout, "DEBUG: Converged after #%u iterations.\tDelta = %0.2e\n", curr_iteration - 1, delta);

	// Clear allocated vars except from pagerank of course.
    free(b);

    return pagerank;
}

inline double NTWPR_GS_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1], const uint32_t m, const size_t d[static m])
{
    double sqnorm_diff = 0;
    
    for (uint32_t i = 0; i < matrix->node_num; i++)
    {
        double den = 1.0;
        
        //const double dv = NTWM_partialSumDV(m, d, x_vec);
        double num = b_vec[i];// + 0.85*dv / matrix->node_num;
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
    
    // NTWM_printDV(stdout, matrix->node_num, x_vec, 3);
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
