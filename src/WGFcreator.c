/**
 * @brief This script produces a program that can convert web graph files
 *          to WGFiles that can be opened with NTWPR_WGF_fopen() and are
 *          used by the NTWPR API.
 * 
 * @file WGFcreator.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 13-08-2018
 */
#include <stdio.h>
#include <time.h>
#include "../include/ntwpr_wg.h"
#include "../include/ntw_debug.h"

int main(int argc, const char* argv[argc+1])
{
    int n = 0;
    if (argc != 4 || ((n = atoi(argv[3])) == 0 && !(argv[3][0] == '0' && argv[3][1] == '\0')))
    {
        printf("Usage: %s input_path output_path node_num\nWhere node_num must be a positive integer indicating the node number or zero for all nodes.\n", argv[0]);

        return EXIT_FAILURE;
    }
    
    // Structs used for time point values.
    struct timespec start, finish;

    // Convert the input from the Stanford's U. form to a WGFile at path argv[2]
    clock_gettime(CLOCK_MONOTONIC, &start);
    NTWPR_WGF_convertSU(argv[1], argv[2], n);
    clock_gettime(CLOCK_MONOTONIC, &finish);

    fprintf(stdout, "Convertion successfully completed!\n");
    NTW_DEBUG_printElapsedTime(stdout, start, finish, "Convertion time", '\n');
    
    return EXIT_SUCCESS;
}