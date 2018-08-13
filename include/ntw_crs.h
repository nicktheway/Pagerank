/**
 * @brief File containing CRS (compressed row storage) type and functions.
 * 
 * @file ntw_crs.h
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */

#ifndef NTW_CRS_H
#define NTW_CRS_H 1

#include <stdio.h>
#include <stdint.h>

/**
 * @brief A Compressed Row Storage struct.
 * 
 * Used to save web graph tables in RAM.
 * 
 * The represented matrix can be easily converted into a probability matrix
 * and the dangling nodes (all zero rows) can be found easily as well because
 * row_ptr[i] == row_ptr[i+1] if the node i is dangling.
 * 
 * See <a href="http://netlib.org/linalg/html_templates/node91.html#SECTION00931100000000000000">CRS</a> 
 * 
 */
typedef struct ntw_crs
{
    uint32_t node_num;  /**< The number of nodes of the graph. */
    uint32_t edge_num;  /**< The number of non zero elements (edges) */
    uint32_t* restrict row_ptr;  /**< A pointer to the val vector indicating the start of a matrix' row. */
    uint32_t* restrict col_ind;  /**< A vector containing the columns of the val's values. */
    double* restrict val;        /**< The non zero values of the matrix. */
} ntw_crs;

/**
 * @brief Creates a new CRS structure to store a matrix.
 * 
 * Should be freed using the NTW_CRS_free when non needed anymore.
 * 
 * @param nodeNum The number of nodes (rows) of the matrix.
 * @param edgeNum The number of non-zero elements of the matrix.
 * @param rowPtr Array of @a nodeNum + 1 elements with the indeces of the @a val where each row begins and the end of the @a val array.
 * @param colInd Array of @a edgeNum elements with the columns of each non zero element of the matrix.
 * @param val Array of @a edgeNum elements with the non zero values of the matrix.
 * @return ntw_crs* The created struct or null if unsuccessful.
 */
ntw_crs* NTW_CRS_new(const uint32_t nodeNum, const uint32_t edgeNum, uint32_t rowPtr[static nodeNum+1], uint32_t colInd[static edgeNum], double val[static edgeNum]);

/**
 * @brief Unloads a ntw_crs from memory (frees allocated memory).
 * 
 * @param crs 
 */
void NTW_CRS_free(ntw_crs* crs);

/**
 * \TODO...
 * @brief Creates a new ntw_crs storing the transpose of @a crs.
 * 
 * Allocates memory for the new ntw_crs that should be freed using the NTW_CRS_free when non needed anymore.
 * 
 * @param crs
 * @return ntw_crs* Pointer to the created struct or null if unsuccessful. 
 */
ntw_crs* NTW_CRS_newTranspose(const ntw_crs crs[static 1]);

/**
 * @brief Multiplies the values of the sparse table in the CRS form 
 *          @a crs with the constant value @a c.
 * 
 * @param crs The ntw_crs representation of the sparse table.
 * @param c The constant multiplicator.
 */
void NTW_CRS_cmult(ntw_crs crs[static 1], const double c);

/**
 * @brief Calculates the product of the CRS table @a crs
 *          with the @a vector and stores the result in @a product. 
 * 
 * @param crs The CRS representation of the matrix @f$A@f$.
 * @param vector The vector @f$v@f$.
 * @param product The product @f$A*v@f$.
 */
void NTW_CRS_vmult(const ntw_crs crs[static 1], const double vector[static 1], double product[static crs->node_num]);

/**
 * @brief Calculates the product of the CRS table @a crs 
 *          with the @a vector and return the result.
 * 
 * The memory of the result should be freed afterwards.
 * 
 * @param crs The CRS representation of the matrix @f$A@f$.
 * @param vector The vector @f$v@f$.
 * @return double* The product vector @f$A*v@f$.
 */
double* NTW_CRS_vmultAlloc(const ntw_crs crs[static 1], const double vector[static 1]);

/**
 * @brief Calculates the product of the transpose of the CRS table @a crs
 *          with the @a vector and stores the result in @a product. 
 * 
 * @param crs The CRS representation of the matrix @f$A@f$.
 * @param vector The vector @f$v@f$.
 * @param product The product @f$A^T*v@f$.
 */
void NTW_CRS_vmultTranspose(const ntw_crs crs[static 1], const double vector[static 1], double product[static 1]);

/**
 * @brief Normalizes the rows of the CRS table so they sum to one.
 * 
 * @param crs The CRS representation of the table.
 */
void NTW_CRS_rowNorm(ntw_crs crs[static 1]);

/**
 * @brief Normalizes the rows of the CRS table giving all the
 *          edges in each row the same value in order to sum to one.
 * 
 * @param crs The CRS representation of the table.
 */
void NTW_CRS_rowNormUnif(ntw_crs crs[static 1]);

/**
 * @brief Normalizes the columns of the CRS table so they sum to one.
 *
 * O(crs->edge_num) complexity.
 * 
 * @param crs The CRS representation of the table.
 */
void NTW_CRS_colNorm(ntw_crs crs[static 1]);

/**
 * @brief Returns the number of empty rows of the @a crs.
 *
 * @param crs
 * @return uint32_t The number of empty rows in the crs matrix.
 */
uint32_t NTW_CRS_getEmptyRowsNum(const ntw_crs crs[static 1]);

/**
 * @brief Returns a vector with the indices of empty rows of the @a crs.
 *
 * @param crs
 * @param outIndicesNum Pass the adress of a uint32_t to be filled with the size of the returned array.
 * @return uint32_t The number of empty rows in the crs matrix.
 */
uint32_t* NTW_CRS_getEmptyRowIndices(const ntw_crs crs[static 1], uint32_t* restrict outIndicesNum);

/**
 * @brief Returns the value at (@a row, @a col) of the matrix of the CRS.
 * 
 * Use it sparingly because it's relatively slow. (Depends on how sparse the
 * row is with O(n) where n is the non-zero elements of the row).
 * 
 * @param crs The CRS data.
 * @param row The row of the returned value.
 * @param col The column of the returned value.
 * @return double The value at [@a row, @a col]
 */
double NTW_CRS_valueAt(const ntw_crs crs[static 1], uint32_t row, uint32_t col);

/**
 * @brief A function for visualizing a CRS.
 * 
 * Useful for debugging.
 * 
 * @param stream Where will the output go.
 * @param crs The CRS to visualize.
 */
void NTW_CRS_print(FILE* restrict stream, const ntw_crs crs[static 1]);

/**
 * @brief A function for printing the full matrix of the CRS.
 * 
 * Useful for debugging. Avoid using it for @f$nodes > 10000@f$.
 * 
 * @param stream Where will the output go.
 * @param crs The CRS of the matrix that will be visualized.
 */
void NTW_CRS_printFullMatrix(FILE* restrict stream, const ntw_crs crs[static 1]);

#endif
