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
#include "../include/ntw_debug.h"

int main(int argc, char const *argv[argc+1])
{
    if (argc != 2)
    {
        printf("Usage: %s input_NTW_WGFile_path\n", argv[0]);

        return EXIT_FAILURE;
    }

    FILE* log_fp = fopen("./results/logs/log.txt", "w");
    
    // Open the file.
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[1]);
    fprintf(log_fp, "File: %s, Nodes: %u, Edges: %u\n", argv[1], file->node_num, file->edge_num);
    //NTWPR_WGF_exportSM(file, "./results/whatRead.txt", file->node_num);

    // Structs used for time point values.
    struct timespec start, finish;

    // Load it at memory using ntw_crs
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Load to crs time", '\n');
    
    fprintf(log_fp, "NTWPR_pagerank:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    double* pr = NTWPR_pagerank(myCRS, 0.85, 1e-12, log_fp);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Whole pagerank time", '\n');
    
    NTW_DEBUG_printBinaryDoubleArray("./results/pageranks/result.data", myCRS->node_num, pr);

	free(pr);
    NTWPR_WGF_fclose(file);
    NTW_CRS_free(myCRS);
    fclose(log_fp);

    return EXIT_SUCCESS;
}
