/**
 * @brief Test script used for development.
 * 
 * @file test.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 09-08-2018
 */
#include <stdio.h>
#include <stdlib.h>
#include "ntwpr_wg.h"

int main(int argc, char* argv[argc+1])
{

    if (argc != 4)
    {
        printf("Please specify an input and an output file. Also specify the dimension N of the NxN resulting matrix\n");
        return 1;
    }
    int n = atoi(argv[3]);
    NTWPR_SU2WG(argv[1], argv[2], n);
    NTWPR_WGFile* file = NTWPR_WGF_fopen(argv[2]);

    NTWPR_CRS* myCRS = NTWPR_load2crs(file);
    FILE* mat = fopen("mat.txt", "w");
    NTWPR_CRS_print(mat, myCRS);
    NTWPR_expsm(file, "full_mat.txt", 40);


    // Product testing
    float *b = malloc(myCRS->node_num * sizeof(*b));
    for (int i = 0; i < myCRS->node_num; i++){
        b[i] = i;
    }
    float *c = calloc(myCRS->node_num, sizeof(*c));
    NTWPR_CRS_product_non_alloc(myCRS, b, c);
    fprintf(mat, "\n---START PRODUCT RESULT---\n");
    for (int i = 0; i < myCRS->node_num; i++){
        fprintf(mat, "%.2f\t", c[i]);
    }
    fprintf(mat, "\n---END PRODUCT RESULT---\n");

    NTWPR_CRS_unified_rnorm(myCRS);
    NTWPR_CRS_printfm(mat, myCRS);
    NTWPR_WGF_fclose(file);
    NTWPR_CRS_free(myCRS);
    fclose(mat);
    return 0;
}