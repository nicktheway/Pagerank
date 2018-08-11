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
/**
 * @brief Macro used to not redifine header definitions
 *        when this file is included to other sources.
 */
#define NTWPR_WG_H 1

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * @brief The character used to indicate comment lines
 *        in the Stanford's (SU) data files.
 * 
 */
#define DF_SU_LINE_COMMENT '#'

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
} while(false)                                                      \



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
 * @brief Reset the @a NTWPR_WGF struct to read the edges again.
 * 
 * @param NTWPR_WGF The NTWPR_WGFile struct.
 * @return true If successful.
 * @return false If an error occured.
 */
bool NTWPR_WGF_rewind(NTWPR_WGFile* const NTWPR_WGF);

/**
 * @brief Loads a web graph in memory using the CRS data structure.
 * 
 * The CRS structure's memory should be freed with NTWPR_CRSfree() later.
 * 
 * @param ntwpr_wgfp The input web graph data.
 * @return ntw_crs* pointing to the loaded CRS or null.
 */
ntw_crs* NTWPR_load2crs(NTWPR_WGFile* restrict ntwpr_wgfp);



/**
 * @brief Converts web graph files from the format Stanford U. (SU) used to
 *        the NTWPR_WGFile' edge_data file format.
 * 
 * Extra: The output can be loaded into MATLAB with: fread(file_id, [2,inf], 'uint32')';
 * 
 * @param SU_WGD_path The input file's path..
 * @param exp_path The output file's path.
 * @param NTWPR_node_num The number of nodes that will be in the graph matrix.
 *        (Enter 0 to get all the nodes of the file.)
 */
void NTWPR_SU2WG(const char SU_WGD_path[static 1], const char exp_path[static 1], uint32_t NTWPR_node_num);


/**
 * @brief Exports the full graph matrix of a NTWPR_WGFile to the file at @a exp_path.
 * 
 * Should be used with caution and for relatively small @a NTWPR_node_num.
 * (NTWPR_node_num @f$< 10000 @f$)
 * 
 * @param wgfp The input NTWPR_WGFile pointer.
 * @param exp_path The output file's path.
 * @param NTWPR_node_num The max number of nodes that will be in the graph matrix.
 */
void NTWPR_expfm(NTWPR_WGFile* restrict wgfp, const char exp_path[static 1], uint32_t NTWPR_node_num);

/**
 * @brief Exports the edges to a text file at @a exp_path.
 * 
 * Extra: Can be loaded into a sparse MATLAB matrix using:
 *      > load -ASCII exported_file_path
 *      > sparce(file_name(:,1)+1, file_name(:,2)+1, ...
 *      ones(size(file_name, 1), 1), node_num, node_num)
 * 
 * @param wgfp The input NTWPR_WGFile pointer.
 * @param exp_path The output file's path.
 * @param NTWPR_node_num The max number of nodes that will be in the graph matrix.
 */
void NTWPR_expsm(NTWPR_WGFile* restrict wgfp, const char exp_path[static 1], uint32_t NTWPR_node_num);
#endif