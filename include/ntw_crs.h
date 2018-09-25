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
#include "ntw_collections.h"

/**
 * @brief A Compressed Row Storage struct.
 * 
 * Used to save web graph tables in RAM.
 * 
 * The represented matrix can be easily converted into a probability matrix
 * and the dangling nodes (all zero rows) can be found easily as well because
 * row_ptr[i] == row_ptr[i+1] if the node i is dangling.
 * 
 * See <a href="http://netlib.org/linalg/html_templates/node91.html">CRS Format</a> 
 * 
 */
typedef struct ntw_crs
{
    uint32_t node_num;  /**< The number of nodes of the graph. */
    uint32_t edge_num;  /**< The number of non zero elements (edges) */
    uint32_t* restrict row_ptr;  /**< A vector for storing the indices of the @a val vector that start a row. */
    uint32_t* restrict col_ind;  /**< A vector for storing the columns of the non zero values. */
    double* restrict val;        /**< The non zero values of the matrix. */
} ntw_crs;

/**
 * @brief A struct for storing web-graph edges.
 *
 * Useful for preparing edges before applying a Pagerank algorithm.
 * For example, the gauss sneidel method needs sorted edges.
 */
typedef struct ntw_CRSEdge
{
	uint32_t nodeA; /**< The first node of the edge. */
	uint32_t nodeB; /**< The second node of the edge. */
} ntw_CRSEdge;

/**
 * @brief A struct for fast crs id reshaping.
 * 
 */
typedef struct ntw_CRSReshapeSequence
{
    uint32_t node_num; /**< The number of elements of @a new_pos and @a look_up arrays. */
    uint32_t* restrict new_pos; /**< The new id at each index. */
    uint32_t* restrict look_up; /**< Where the old id will be. */
} ntw_CRSReshapeSequence;

/**
 * @brief Creates a new CRS structure to store a matrix.
 * 
 * Should be freed using the NTW_CRS_free when non needed anymore.
 * 
 * @param nodeNum The number of nodes (rows) of the matrix.
 * @param edgeNum The number of non-zero elements of the matrix (edges).
 * @param rowPtr Array of @a nodeNum + 1 elements with the indeces of the @a val where each row begins and the end of the @a val array (where last row ends).
 * @param colInd Array of @a edgeNum elements with the columns of each non zero element of the matrix.
 * @param val Array of @a edgeNum elements with the non zero values of the matrix.
 * @return ntw_crs* The created struct or 0 (NULL) if unsuccessful.
 */
ntw_crs* NTW_CRS_new(const uint32_t nodeNum, const uint32_t edgeNum, uint32_t rowPtr[static nodeNum+1], uint32_t colInd[static edgeNum], double val[static edgeNum]);

/**
 * @brief Unloads a ntw_crs from memory (frees allocated memory).
 * 
 * @param crs Pointer to the structure that will be freed.
 */
void NTW_CRS_free(ntw_crs* crs);

/**
 * @brief Multiplies the values of the sparse table in the CRS form 
 *          @a crs with the constant value @a c.
 * 
 * @param crs The ntw_crs representation of the sparse table.
 * @param c The constant multiplier.
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
 * Not cache efficient function. Therefore slow.
 * 
 * @param crs The CRS representation of the matrix @f$A@f$.
 * @param vector The vector @f$v@f$.
 * @param product The product @f$A^T*v@f$.
 */
void NTW_CRS_vmultTranspose(const ntw_crs crs[static 1], const double vector[static 1], double product[static 1]);

/**
 * @brief Makes the rows of the CRS table stochastic.
 * 
 * Their elements will then sum to 1 (at each row).
 * 
 * @param crs The CRS representation of the table.
 */
void NTW_CRS_stochasticizeRows(ntw_crs crs[static 1]);

/**
 * @brief Makes the columns of the CRS table stochastic.
 * 
 * Their elements will then sum to 1 (at each column).
 * 
 * @param crs The CRS representation of the table.
 */
void NTW_CRS_stochasticizeCols(ntw_crs crs[static 1]);

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
 * @return uint64_t The number of empty rows in the crs matrix.
 */
uint64_t* NTW_CRS_getEmptyRowIndices(const ntw_crs crs[static 1], uint32_t* restrict outIndicesNum);

/**
 * @brief Divides the @a crs matrix into groups with lower-only dependencies.
 * 
 * For example: a node with color 3 will only have dependencies with nodes of colors 1 and 2.
 * 
 * @param crs The crs representation of the matrix.
 * @return ntw_vector* A vector containing the groups in ntw_vector datasets.
 */
ntw_vector* NTW_CRS_getColoredGroups(const ntw_crs* const crs);

/**
 * @brief How the crs should be reshaped for color optimization?
 * 
 * Rebasing the crs based on the returned sequence should reduce cache-misses 
 * resulting from color based access to the matrix.
 * 
 * Don't forget to free() the returned sequence when not needed anymore.
 * 
 * @param colors The color groups of the nodes.
 * @param nodes The number of nodes in the matrix.
 * @return The optimized index based sequence of nodes.
 */
ntw_CRSReshapeSequence* NTW_CRS_getColorOptimizedIds(ntw_vector* colors, const uint32_t nodes);

/**
 * @brief Reshapes the crs matrix' rows based on @a rowIdArray.
 * 
 * Assumes that the @a rowIdArray has at least @a crs->node_num elements.
 * 
 * @param crs The crs to be reshaped.
 * @param rowIdArray A vector with the id's of the new row sequence.
 */
void NTW_CRS_RowReshape(ntw_crs* restrict crs, const uint32_t* const rowIdArray);

/**
 * @brief Reshapes the crs matrix' id's based on @a reshapeData.
 * 
 * @param crs The crs to be reshaped.
 * @param reshapeData How the id's will change.
 */
void NTW_CRS_IdReshape(ntw_crs* restrict crs, const ntw_CRSReshapeSequence* const reshapeData);

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

/**
 * @brief Transposes the edges of a graph described with @a edges.
 * 
 * @param n The number of edges in the graph.
 * @param edges The edges of the graph.
 */
void NTW_CRS_transposeEdges(const uint64_t n, ntw_CRSEdge edges[static n]);

/**
 * @brief Compares two edges, used for sorting with qsort().
 * 
 * @param edgeA
 * @param edgeB
 * @return int edgeA->nodeB - edgeB->nodeB
 */
int NTW_CRSEdgeCompareForT(const void* restrict edgeA, const void* restrict edgeB);

#endif
