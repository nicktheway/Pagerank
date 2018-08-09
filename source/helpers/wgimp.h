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
 * @brief A Compressed Row Storage struct.
 * 
 * Used to save web graph tables in RAM.
 * See <a href="http://netlib.org/linalg/html_templates/node91.html#SECTION00931100000000000000">CRS</a> 
 * 
 */
typedef struct NTW_CRS
{
    double* val; /**< The non zero values of the matrix.*/
    size_t* col_ind; /**< A vector containing the columns of the val's values.*/
    size_t* row_ptr; /**< A pointer to the val vector indicating the start of a matrix' row.*/
} NTW_CRS;

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

/**
 * @brief Loads a web graph in memory using the CRS data structure.
 * 
 * @param ntw_in_fp The input file pointer, must be like the ones exported using the NTW_expsm()
 * @param ntw_CRS_out Pointer to the allocated memory structure containing the wab graph.
 * @return true     Indicates that the web graph was successfully loaded.
 * @return false    An error occured, the web graph will not be in the memory.
 */
bool NTW_load2crs(FILE *ntw_in_fp, NTW_CRS* ntw_CRS_out);
#endif