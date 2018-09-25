/**
 * @brief Calculated the pagerank vector of a web graph saved in an
 *          NTWPR_WGFile form.
 * 
 * @file NTWPRcalc.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 16-08-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/ntwpr.h"
#include "../include/ntw_collections.h"
#include "../include/ntw_debug.h"

/**
 * @brief A program that calculates pageranks of web-graph files.
 * 
 * @param argc The number of arguments passed when executing the program.
 * @param argv The input arguments.
 * @return int Usually zero if the program terminated successfully, 1 if not.
 */
int main(int argc, char const *argv[argc+1])
{
    const double delta_default = 1e-18;
    const double tel_coeff_default = 0.85;
    const char log_file_path[] = "./results/logs/log.txt";
    const char pagerank_file_path[] = "./results/pageranks/result.data";

    if (argc != 2 && argc != 3 && argc != 4)
    {
        printf("Usage: %s input_NTW_WGFile_path (convergence_delta) (teleportation_coefficient)\n"
                "Arguments inside () are optional."
                "The <teleportation_coefficient> cannot be set without setting the <convergence_delta> as well!\n"
                "If not provided, the following default values will be used:\n"
                "convergence_delta: %0.2E\nteleportation_coefficient: %0.2f ", argv[0], delta_default, tel_coeff_default);

        return EXIT_FAILURE;
    }
    double delta = -1;
    double tel_coeff = -1;
   
    switch (argc)
    {
        case 4:
            // Check for a valid telep_coeff argument. No break on purpose.
            if (sscanf(argv[3], "%lf", &tel_coeff) != 1 || tel_coeff <= 0 || tel_coeff >= 1)
            {
                printf("The teleportation_coefficient must be in (0, 1).\nCurrent value: %lf\n", tel_coeff);
                return EXIT_FAILURE;
            }
        case 3:
            // Check for a valid delta argument.
            if (sscanf(argv[2], "%lf", &delta) != 1 || delta <= 1e-36 || delta >= 1e-1)
            {
                printf("The convergence_delta must be in (1e-36, 0.1). Input values like 1e-4 are also supported.\n"
                        "Current value: %0.2lE\n", delta);
                return EXIT_FAILURE;
            }
            if (argc != 4) tel_coeff = tel_coeff_default;
            break;
        default:
            delta = delta_default;
            tel_coeff = tel_coeff_default;
            break;
    }
    
    // Try to open the file.
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[1]);

    // Create/open file to store logs (times).
    FILE* log_fp = fopen(log_file_path, "w");
    if (!log_fp)
    {
        fprintf(stderr, "Error at log file creation, possibly a missing folder\n");
        return EXIT_FAILURE;
    }
    
    
    fprintf(log_fp, "File: %s, Nodes: %u, Edges: %u\n", argv[1], file->node_num, file->edge_num);
    //NTWPR_WGF_exportSM(file, "./results/whatRead.txt", file->node_num);

    // Structs used for time point values.
    struct timespec start, finish;

    // Load it at memory using ntw_crs
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Load to crs time", '\n');
    
    // Data coloring
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_vector* colors = NTW_CRS_getColoredGroups(myCRS);
    ntw_CRSReshapeSequence* reshape_seq = NTW_CRS_getColorOptimizedIds(colors, myCRS->node_num);
    NTW_CRS_IdReshape(myCRS, reshape_seq);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    fprintf(log_fp, "Color groups: %lu\n", colors->length);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Data coloring and matrix reshaping time", '\n');
    
    // Pagerank
    fprintf(log_fp, "NTWPR_pagerank:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    double* pr = NTWPR_pagerank(myCRS, tel_coeff, delta, colors, log_fp);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Whole pagerank time", '\n');
    
    NTW_DEBUG_printBinaryDoubleArray(pagerank_file_path, myCRS->node_num, pr);

    // No memory leaks
    for (uint64_t i = 0; i < colors->length; i++)
    {
        //fprintf(log_fp, "\t%lu\n", ((ntw_vector *) colors->data[i])->length);
        //NTW_DEBUG_printArray_uint64(log_fp,(uint64_t *)((ntw_vector *) colors->data[i])->data, ((ntw_vector *) colors->data[i])->length);
        NTW_vector_free(colors->data[i]);
        free(colors->data[i]);
    }
    
    free(reshape_seq);
    NTW_vector_free(colors);
    free(colors);
	free(pr);
    NTWPR_WGF_fclose(file);
    NTW_CRS_free(myCRS);
    fclose(log_fp);

    return EXIT_SUCCESS;
}
