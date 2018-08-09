/**
 * @brief Web-Graph import/export functions.
 * 
 * Currently supported data:
 * - Files containing sorted edges of web graphs like the ones found at:
 *  <a href="https://snap.stanford.edu/data/#web">Stanford Large Network Dataset Collection</a>  
 * 
 * @file wgimp.h
 * @author Katomeris Nikolaos
 * @date 09-08-2018
 */

#ifndef WGIMP_H
/**
 * @brief Macro used to not redifine header definitions
 *        when this file is included to other sources.
 */
#define WGIMP_H 1

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief The character used to indicate comment lines
 *        in the data files.
 * 
 */
#define DF_LINE_COMMENT '#'

/**
 * @brief Checks for null file pointers returning debug message.
 * 
 * @param fp The file pointer to check for null.
 */
#define NTW_invalid_fp_exit(fp)                                     \
do {                                                                \
    if (!fp)                                                        \
    {                                                               \
        fprintf(stderr,                                             \
            "%s:%d: Invalid file pointer.\n", __FILE__, __LINE__);  \
        exit(EXIT_FAILURE);                                         \
    }                                                               \
} while(false)                                                      \

/**
 * @brief Exports the full graph matrix to @a ntw_out_fp.
 * 
 * Should be used with caution and for relatively small @a ntw_node_num.
 * (ntw_node_num @f$< 1000 @f$)
 * 
 * @param ntw_in_fp The input file pointer.
 * @param ntw_out_fp The output file pointer.
 * @param ntw_node_num The number of nodes that will be in the graph matrix.
 */
void NTW_expfm(FILE* ntw_in_fp, FILE* ntw_out_fp, size_t ntw_node_num);

/**
 * @brief Exports the edges of @a ntw_node_num nodes for easy
 *        sparse matrix creation.
 * 
 * The output can be loaded into MATLAB to create a sparse matrix
 * using MATLAB's sparse() function.
 * 
 * @param ntw_in_fp The input file pointer.
 * @param ntw_out_fp The output file pointer.
 * @param ntw_node_num The number of nodes that will be in the graph matrix.
 */
void NTW_expsm(FILE* ntw_in_fp, FILE* ntw_out_fp, size_t ntw_node_num);

#endif