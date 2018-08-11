/**
 * @brief Web-Graph import/export functions.
 * 
 * Currently supported data:
 * - Files containing sorted edges of web graphs like the ones found at:
 *  <a href="https://snap.stanford.edu/data/#web">Stanford Large Network Dataset Collection</a>  
 * 
 * @file ntwpr_wg.h
 * @author Katomeris Nikolaos
 * @date 09-08-2018
 */

#ifndef NTWPR_WG_H
#define NTWPR_WG_H 1

#include "ntw_crs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/******************* MACRO DEFINITIONS *******************/

/**
 * @brief The character used to indicate comment lines
 *        in the Stanford's (SU) data files.
 */
#define LINE_COMMENT_SU '#'

/**
 * @brief Checks for null file pointers returning debug message.
 * 
 * @param fp The file pointer to check for null.
 */
#define NTWPR_invalid_fp_exit(fp)                                   \
do {                                                                \
    if (!fp)                                                        \
    {                                                               \
        fprintf(stderr,                                             \
            "%s:%d: Invalid file pointer.\n", __FILE__, __LINE__);  \
        exit(EXIT_FAILURE);                                         \
    }                                                               \
} while(0)                                                          \



/******************* TYPE DEFINITIONS *******************/

/**
 * @brief A struct containing a web-graph file and the graphs info.
 * 
 * Used as a "middle" save format. This API uses this type to
 * compute the pagerank of the nodes. Any dataset's pagerank vector can
 * be computed by extracting such struct from the dataset.
 */
typedef struct NTWPR_WGFile
{
    uint32_t edge_num; /**< The number of the graph's edges. */
    uint32_t node_num; /**< The number of the graph's nodes. */
    FILE* edge_data; /**< The edges represented as node pairs */
} NTWPR_WGFile;

/******************* FUNCTION DEFINITIONS *******************/

/**
 * @brief Creates a new WGFile at @a path.
 * 
 * The WGFile should be then closed with NTWPR_WGfclose()
 * 
 * @param path The path of the file.
 * @return NTWPR_WGFile* The struct containing the file created.
 */
NTWPR_WGFile* NTWPR_WGF_fopen(const char path[static 1]);

/**
 * @brief Closes the NTWPR_WGFile stream and frees memory.
 * 
 * @param wgfile The file to be closed.
 * @return int Returns 0 if succussful. On failure EOF is returned.
 */
int NTWPR_WGF_fclose(NTWPR_WGFile* wgfile);

/**
 * @brief Reset the @a wgf struct to read the edges again.
 * 
 * @param wgf The NTWPR_WGFile struct.
 * @return true If successful.
 * @return false If an error occured.
 */
unsigned NTWPR_WGF_rewind(NTWPR_WGFile* const wgf);

/**
 * @brief Exports the full graph matrix of a NTWPR_WGFile to the file at @a exportPath.
 * 
 * Should be used with caution and for relatively small @a nodeNum.
 * (nodeNum @f$< 10000 @f$)
 * 
 * @param wgfp The input NTWPR_WGFile pointer.
 * @param exportPath The output file's path.
 * @param nodeNum The max number of nodes that will be in the graph matrix.
 */
void NTWPR_WGF_exportFM(NTWPR_WGFile* restrict wgfp, const char exportPath[static 1], uint32_t nodeNum);

/**
 * @brief Exports the edges to a text file at @a exportPath.
 * 
 * Extra: Can be loaded into a sparse MATLAB matrix using:
 *      > load -ASCII exported_file_path
 *      > sparce(file_name(:,1)+1, file_name(:,2)+1, ...
 *      ones(size(file_name, 1), 1), node_num, node_num)
 * 
 * @param wgfp The input NTWPR_WGFile pointer.
 * @param exportPath The output file's path.
 * @param nodeNum The max number of nodes that will be in the graph matrix.
 */
void NTWPR_WGF_exportSM(NTWPR_WGFile* restrict wgfp, const char exportPath[static 1], uint32_t nodeNum);

/**
 * @brief Loads a web graph in memory using the CRS data structure.
 * 
 * The CRS structure's memory should be freed with NTWPR_CRSfree() later.
 * 
 * @param wgf The input web graph data.
 * @return ntw_crs* pointing to the loaded CRS or null.
 */
ntw_crs* NTWPR_WGF_load2crs(NTWPR_WGFile* restrict wgf);

/**
 * @brief Converts web graph files from the format Stanford U. (SU) used to
 *        the NTWPR_WGFile' edge_data file format.
 * 
 * Extra: The output can be loaded into MATLAB with: fread(file_id, [2,inf], 'uint32')';
 * 
 * @param suDataPath The input file's path..
 * @param exportPath The output file's path.
 * @param nodeNum The number of nodes that will be in the graph matrix.
 *        (Enter 0 to get all the nodes of the file.)
 */
void NTWPR_WGF_convertSU(const char suDataPath[static 1], const char exportPath[static 1], uint32_t nodeNum);



#endif