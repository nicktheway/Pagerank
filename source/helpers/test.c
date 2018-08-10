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
    NTWPR_WGFile* file = NTWPR_WGfopen(argv[2]);

    NTWPR_CRS* myCRS = NTWPR_load2crs(file);
    FILE* mat = fopen("mat.txt", "w");
    NTWPR_CRSprint(mat, myCRS);
    NTWPR_expsm(file, "full_mat.txt", 40);
    NTWPR_WGfclose(file);
    NTWPR_CRSfree(myCRS);
    fclose(mat);
    return 0;
}