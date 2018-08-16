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

    FILE* mat = fopen("./data/mat.txt", "w");
    
    // Open the file.
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[1]);
    fprintf(mat, "File: %s, Nodes: %u, Edges: %u\n", argv[1], file->node_num, file->edge_num);
    //NTWPR_WGF_exportSM(file, "./data/whatRead.txt", file->node_num);

    // Structs used for time point values.
    struct timespec start, finish;

    // Load it at memory using ntw_crs
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(mat, start, finish, "Load to crs time");
    
    fprintf(mat, "NTWPR_pagerank:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    double* pr = NTWPR_pagerank(myCRS, 0.85, 1e-24, mat);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(mat, start, finish, "Whole pagerank time");
    
    fprintf(mat, "\n---START PR RESULT---\n");
    NTW_DEBUG_printBinaryDoubleArray("./data/cA.data", myCRS->node_num, pr);
    fprintf(mat, "\n---END PR RESULT---\n");

	free(pr);
    NTWPR_WGF_fclose(file);
    NTW_CRS_free(myCRS);
    fclose(mat);

    return EXIT_SUCCESS;
}
