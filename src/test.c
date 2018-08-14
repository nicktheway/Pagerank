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
#include "../include/ntw_math.h"

int main(int argc, char* argv[argc+1])
{

    if (argc != 4)
    {
        printf("Please specify an input and an output file. Also specify the dimension N of the NxN resulting matrix\n");
        return 1;
    }
    int n = atoi(argv[3]);
    // Convert the input from the Stanford's U. form to a WGFile at path argv[2]
    NTWPR_WGF_convertSU(argv[1], argv[2], n);
    
<<<<<<< HEAD
    // Prepare Edges
    // NTWPR_WGF_convert2Transpose(argv[2], "./data/new_input.mat");

    FILE* mat = fopen("./data/mat.txt", "w");

    // Open the file.
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[2]);

    // Load it at memory using ntw_crs
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    //NTW_CRS_print(mat, myCRS);
=======
    // Transpose for the Gauss Sneilel.
    NTWPR_WGF_convert2Transpose(argv[2], "./data/new_input.mat");

    // Open the file.
    NTWPR_WGFile* file = NTWPR_WGF_fopen("./data/new_input.mat");

    // Load it at memory using ntw_crs
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);

    // DEBUG print the matrix
    FILE* mat = fopen("./data/mat.txt", "w");
    NTW_CRS_printFullMatrix(mat, myCRS);
>>>>>>> 9bb2f511c0262561d9a5ddc42c75fce9d058a3ea

    // DEBUG print the matrix
    //NTW_CRS_printFullMatrix(mat, myCRS);
    
    double* pr = NTWPR_pagerank(myCRS, 0.85, 1e-12, mat);
    
    fprintf(mat, "\n---START PR RESULT---\n");
<<<<<<< HEAD
    NTWM_printDV(mat, myCRS->node_num, pr, 9);
=======
    NTWM_printDV(mat, myCRS->node_num, pr, 4);
>>>>>>> 9bb2f511c0262561d9a5ddc42c75fce9d058a3ea
    fprintf(mat, "\n---END PR RESULT---\n");

	free(pr);
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
