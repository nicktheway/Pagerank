/**
 * @brief Test script used for development.
 * 
 * @file test.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 09-08-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include "wgimp.h"

int main(int argc, char* argv[argc+1])
{

    if (argc != 5)
    {
        printf("Please specify an input and an output file. Also specify the dimension N of the NxN resulting matrix and 's' for sparse.\n");
        return 1;
    }
    
    FILE* data_file_p;
    FILE* matrix_file_p;
    data_file_p = fopen(argv[1], "r");
    NTW_invalid_fp_exit(data_file_p);
    matrix_file_p = fopen(argv[2], "w");
    NTW_invalid_fp_exit(matrix_file_p);
    int N = atoi(argv[3]);
    if (N <= 0)
    {
        printf("Invalid matrix dimension number\n");
        return 1;
    }

    if (argv[4][0] == 's')
        NTW_expsm(data_file_p, matrix_file_p, N);
    else 
        NTW_expfm(data_file_p, matrix_file_p, N);

    fclose(matrix_file_p);
    fclose(data_file_p);
    return 0;
}