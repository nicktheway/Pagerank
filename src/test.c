/**
 * @brief Test script used for development.
 * 
 * @file test.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 09-08-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/ntwpr_wg.h"
#include "../include/ntwpr.h"

int main(int argc, char* argv[argc+1])
{

    if (argc != 4)
    {
        printf("Please specify an input and an output file. Also specify the dimension N of the NxN resulting matrix\n");
        return 1;
    }
    int n = atoi(argv[3]);
    NTWPR_WGF_convertSU(argv[1], argv[2], n);
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[2]);

    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    FILE* mat = fopen("./../data/mat.txt", "w");
    //NTW_CRS_print(mat, myCRS);
    // NTWPR_WGF_exportSM(file, argv[2], n);

    if (!mat)
    {
        perror("ERROR");
        return EXIT_FAILURE;
    }

    double* pr = NTWPR_pagerank(myCRS, 0.85, 1e-6);
    fprintf(mat, "\n---START PR RESULT---\n");
    for (int i = 0; i < myCRS->node_num; i++){
        fprintf(mat, "%.2f\t", 100000*pr[i]);
    }
    fprintf(mat, "\n---END PR RESULT---\n");
    // NTW_CRS_printFullMatrix(mat, myCRS);
    NTWPR_WGF_fclose(file);
    NTW_CRS_free(myCRS);
    fclose(mat);
    return 0;
}

    /*
    // Product testing
    float *b = malloc(myCRS->node_num * sizeof(*b));
    for (int i = 0; i < myCRS->node_num; i++){
        b[i] = i;
    }
    float *c = calloc(myCRS->node_num, sizeof(*c));
    NTW_CRS_vmult(myCRS, b, c);
    fprintf(mat, "\n---START PRODUCT RESULT---\n");
    for (int i = 0; i < myCRS->node_num; i++){
        fprintf(mat, "%.2f\t", c[i]);
    }
    fprintf(mat, "\n---END PRODUCT RESULT---\n");
    */
    // NTW_CRS_rowNormUnif(myCRS);