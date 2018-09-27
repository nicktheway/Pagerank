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
#include <omp.h>

double* NTWPR_pagerank(ntw_crs webGraph[static 1], const double c, const double e, FILE* stream)
{
    struct timespec start, finish;
    // For more readable code wgSize <- webGraph->node_num
    const uint32_t wgSize = webGraph->node_num;

	/** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */
    clock_gettime(CLOCK_MONOTONIC, &start);
    // Make the matrix a probability matrix.
    NTW_CRS_stochasticizeCols(webGraph);
	/** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(stream, start, finish, "Make graph stochastic time", '\n');
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
    // Array to store converged nodes.
    char* d = calloc(wgSize, sizeof *d);
    double *dd = NTWM_newZeroVectorD(wgSize);
    
    unsigned max_iterations = 150, curr_iteration = 1;
    while (delta > e && curr_iteration < max_iterations)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        delta = NTWPR_GS_iter(webGraph, pagerank, b, d, dd);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        NTW_DEBUG_printElapsedTime(stream, start, finish, "Iteration", '\t');
        fprintf(stream, "Convergence's delta: %0.2e\n", delta);

        if (curr_iteration % 10 == 0)
        {
            for (uint32_t i = 0; i < wgSize; i++)
            {
                if (dd[i] < e) d[i] = 1;
                dd[i] = 0;
            }
        }

        curr_iteration++;
    }
    fprintf(stdout, "DEBUG: Converged after #%u iterations.\tDelta = %0.2e\n", curr_iteration - 1, delta);

	// Clear allocated vars except from pagerank of course.
    free(b);
    free(d);
    free(dd);

    return pagerank;
}

double NTWPR_GS_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1], char d[static 1], double dd[static 1])
{
    double sqnorm_diff = 0;
    
    for (uint32_t i = 0; i < matrix->node_num; i++)
    {
        if (d[i]) continue; // Makes it slower when resetted, wrong when not?
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
        const double partDiff = (x_vec[i] - old_xi) * (x_vec[i] - old_xi);
        sqnorm_diff += partDiff;

        dd[i] += partDiff;
    }
    
    // NTWM_printDV(stdout, matrix->node_num, x_vec, 3);
    return sqnorm_diff;
}

double* NTWPR_colored_pagerank(ntw_crs webGraph[static 1], const double c, const double e, const ntw_vector* const colors, FILE* stream)
{
    struct timespec start, finish;
    // For more readable code wgSize <- webGraph->node_num
    const uint32_t wgSize = webGraph->node_num;

	/** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */
    clock_gettime(CLOCK_MONOTONIC, &start);
    // Make the matrix a probability matrix.
    NTW_CRS_stochasticizeCols(webGraph);
	/** DEBUG:* NTWM_CRS_printFullMatrix(stdout, webGraph); */
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(stream, start, finish, "Make graph stochastic time", '\n');
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
    // Array to store converged nodes.
    char* d = calloc(wgSize, sizeof *d);
    double *dd = NTWM_newZeroVectorD(wgSize);
    
    unsigned max_iterations = 150, curr_iteration = 1;
    while (delta > e && curr_iteration < max_iterations)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        delta = NTWPR_GS_parallel_iter(webGraph, pagerank, b, d, dd, colors);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        NTW_DEBUG_printElapsedTime(stream, start, finish, "Iteration", '\t');
        fprintf(stream, "Convergence's delta: %0.2e\n", delta);

        if (curr_iteration % 10 == 0)
        {
            for (uint32_t i = 0; i < wgSize; i++)
            {
                if (dd[i] < e) d[i] = 1;
                dd[i] = 0;
            }
        }
        
        curr_iteration++;
    }
    fprintf(stdout, "DEBUG: Converged after #%u iterations.\tDelta = %0.2e\n", curr_iteration - 1, delta);

	// Clear allocated vars except from pagerank of course.
    free(b);
    free(d);
    free(dd);

    return pagerank;
}

double NTWPR_GS_parallel_iter(const ntw_crs matrix[static 1], double x_vec[static 1], const double b_vec[static 1], char d[static 1], double dd[static 1], const ntw_vector* const colors)
{
    double sqnorm_diff = 0;
    uint64_t first_group_node = 0;
    uint64_t next = 0;
    for (uint64_t color = 0; color < colors->length; color++)
    {
        const uint64_t groupSize = ((ntw_vector*) colors->data[color])->length;
        next = first_group_node + groupSize;
        #pragma omp parallel for if (groupSize > 1000) reduction (+:sqnorm_diff)
        for (uint64_t i = first_group_node; i < next; i++)
        {
            // uint64_t i = (uint64_t) ((ntw_vector*) colors->data[color])->data[groupEl];
            if (d[i]) continue;
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
            const double partDiff = (x_vec[i] - old_xi) * (x_vec[i] - old_xi);
            sqnorm_diff += partDiff;

            dd[i] += partDiff;
        }
        first_group_node = next;
    }
    
    // NTWM_printDV(stdout, matrix->node_num, x_vec, 3);
    return sqnorm_diff;
}