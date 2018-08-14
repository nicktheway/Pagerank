/**
 * @brief Test script used for development.
 * 
 * @file test.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 09-08-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/ntwpr_wg.h"
#include "../include/ntwpr.h"
#include "../include/ntw_math.h"
#include "../include/ntw_debug.h"

int main(int argc, char* argv[argc+1])
{

    if (argc != 4)
    {
        printf("Please specify an input and an output file. Also specify the dimension N of the NxN resulting matrix\n");
        return 1;
    }
    int n = atoi(argv[3]);
    struct timespec start, finish;
    // Convert the input from the Stanford's U. form to a WGFile at path argv[2]
    NTWPR_WGF_convertSU(argv[1], argv[2], n);
    
    // Prepared Edges?
    

    FILE* mat = fopen("./data/mat.txt", "w");
    
    // Open the file.
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[2]);
    fprintf(mat, "File: %s, Nodes: %u, Edges: %u\n", argv[1], file->node_num, file->edge_num);
    NTWPR_WGF_exportSM(file, "./data/whatRead.txt", file->node_num);
    // Load it at memory using ntw_crs
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(mat, start, finish, "Load to crs time:");
    
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
    return 0;
}