/**
 * @brief File containing the definitions of the functions declared at ntwpr.h
 * 
 * @file ntwpr.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */
#include "../include/ntwpr.h"
#include "../include/ntw_math.h"
#include "../include/ntw_debug.h"
#include <math.h>
#include <omp.h>

double* NTWPR_pagerank(ntw_crs webGraph[static 1], const double c, const double e, FILE* stream)
{
    struct timespec start, finish;
    // For more readable code wgSize <- webGraph->node_num
    const uint32_t wgSize = webGraph->node_num;

	
    clock_gettime(CLOCK_MONOTONIC, &start);
    // Make the matrix a probability matrix.
    NTW_CRS_stochasticizeCols(webGraph);
	
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(stream, start, finish, "Make graph stochastic time", '\n');
    // Multiply with the teleportation coefficient.
    NTW_CRS_cmult(webGraph, -(1 - c));

    // Create the initial pagerank vector (unified) and the b vector.
    double* pagerank = NTWM_newUniVectorD(wgSize, 1.0 / wgSize);
	
    // Vector for checking the product error.
    double* prPagerank = NTWM_newUniVectorD(wgSize, 1.0 / wgSize);

	// Create the b vector.
    double* b = NTWM_newUniVectorD(wgSize, c / wgSize);

	// Start the Gauss-Sneidel Algorithm.
    double delta = 1.0;
    
    unsigned max_iterations = 150, curr_iteration = 1;
    while (delta > e && curr_iteration <= max_iterations)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);

        NTWPR_GS_iter(webGraph, pagerank, b);
        NTWM_normalizeSumDV(wgSize, pagerank);
        NTWM_subDV(wgSize,prPagerank,pagerank);
        delta = NTWM_sqMagnDV(wgSize, prPagerank);
        NTWM_assignDV(wgSize, prPagerank, pagerank);
        curr_iteration++;

        clock_gettime(CLOCK_MONOTONIC, &finish);
        NTW_DEBUG_printElapsedTime(stream, start, finish, "Iteration", '\t');
        fprintf(stream, "Convergence's delta: %0.2e\n", delta); 
    }
    fprintf(stdout, "DEBUG: Converged after #%u iterations.\tDelta = %0.2e\n", curr_iteration - 1, delta);

	// Clear allocated vars except from pagerank of course.
    free(b);
    free(prPagerank);

    return pagerank;
}

void NTWPR_GS_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1])
{  
    for (uint32_t i = 0; i < matrix->node_num; i++)
    {
        double den = 1.0;
        double num = b_vec[i];

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

        x_vec[i] = num / den;
    }
}

double* NTWPR_colored_pagerank(ntw_crs webGraph[static 1], const double c, const double e, const ntw_vector* const colors, FILE* stream)
{
    struct timespec start, finish;
    // For more readable code wgSize <- webGraph->node_num
    const uint32_t wgSize = webGraph->node_num;

	// DEBUG: NTWM_CRS_printFullMatrix(stdout, webGraph);
    clock_gettime(CLOCK_MONOTONIC, &start);
    // Make the matrix a probability matrix.
    NTW_CRS_stochasticizeCols(webGraph);
	// DEBUG: NTWM_CRS_printFullMatrix(stdout, webGraph);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(stream, start, finish, "Make graph stochastic time", '\n');
    // Multiply with the teleportation coefficient.
    NTW_CRS_cmult(webGraph, -(1 - c));
    // DEBUG: NTWM_CRS_printFullMatrix(stdout, webGraph);

    // Create the initial pagerank vector (unified) and the b vector.
    double* pagerank = NTWM_newUniVectorD(wgSize, 1.0 / wgSize);
    double* prPagerank = NTWM_newUniVectorD(wgSize, 1.0 / wgSize);
	
	// Create the b vector.
    double* b = NTWM_newUniVectorD(wgSize, c / wgSize);
    // DEBUG: NTWM_printDV(stdout, wgSize, b, 4);

	// Start the Gauss-Sneidel Algorithm.
    double delta = 1.0;
    
    unsigned max_iterations = 150, curr_iteration = 1;
    while (delta > e && curr_iteration <= max_iterations)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);

        NTWPR_GS_parallel_iter(webGraph, pagerank, b, colors);
        NTWM_normalizeSumDV(wgSize, pagerank);
        NTWM_subDV(wgSize,prPagerank,pagerank);
        delta = NTWM_sqMagnDV(wgSize, prPagerank);
        NTWM_assignDV(wgSize, prPagerank, pagerank);
        curr_iteration++;

        clock_gettime(CLOCK_MONOTONIC, &finish);
        NTW_DEBUG_printElapsedTime(stream, start, finish, "Iteration", '\t');
        fprintf(stream, "Convergence's delta: %0.2e\n", delta);
    }
    fprintf(stdout, "DEBUG: Converged after #%u iterations.\tDelta = %0.2e\n", curr_iteration - 1, delta);

	// Clear allocated vars except from pagerank of course.
    free(b);
    free(prPagerank);

    return pagerank;
}

void NTWPR_GS_parallel_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1], const ntw_vector* const colors)
{
    uint32_t first_group_node = 0;
    uint32_t next = 0;
    for (uint32_t color = 0; color < colors->length; color++)
    {
        const uint32_t groupSize = ((ntw_vector*) colors->data[color])->length;
        next = first_group_node + groupSize;
        #pragma omp parallel for if (groupSize > 40)
        for (uint32_t i = first_group_node; i < next; i++)
        {
            double den = 1.0;
            double num = b_vec[i];

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

            x_vec[i] = num / den;
        }
        first_group_node = next;
    }
}
