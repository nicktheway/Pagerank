/**
 * @brief File containing the implementations of ntwpr_wg.h
 * 
 * @file ntwpr_wg.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 10-08-2018
 */
#include "../include/ntwpr_wg.h"
#include "../include/ntw_collections.h"
#include "../include/ntw_debug.h"
#include <stdio.h>

ntw_crs* NTWPR_WGF_load2crsColored(NTWPR_WGFile* restrict wgf, ntw_vector* restrict colors)
{
    const uint32_t node_num = wgf->node_num;
    const uint32_t edge_num = wgf->edge_num; 
    uint32_t* row_ptr = calloc(node_num + 1, sizeof *row_ptr);
	uint32_t* col_ind = calloc(edge_num, sizeof *col_ind);
	double* val = calloc(edge_num, sizeof *val);
    
    uint32_t* nodeColors = calloc(node_num + 1, sizeof *nodeColors);
    uint32_t maxColor = 0;

	if (!val || !col_ind || !row_ptr || !nodeColors)
    {
        fprintf(stderr, "%s: Error when allocating memory for the CRS struct or coloring.\n", __func__);
        exit(EXIT_FAILURE);
    }

    uint32_t edges_read = 0;
    uint32_t curr_row = 0;
    uint32_t edge_nodes[2];

	// Skip node_num and edge_num
    fseek(wgf->edge_data, 2*sizeof(uint32_t), SEEK_SET);
    while (fread(edge_nodes, sizeof(edge_nodes[0]), 2, wgf->edge_data) == 2)
    {
        col_ind[edges_read] = edge_nodes[1];
        val[edges_read] = 1.0;

        if (nodeColors[edge_nodes[1]+1] > maxColor) 
            maxColor = nodeColors[edge_nodes[1]+1];

        if (edge_nodes[0] > curr_row)
        {
            for (uint32_t i = curr_row + 1; i <= edge_nodes[0]; i++)
            {
                row_ptr[i] = edges_read;
                nodeColors[i] = maxColor+1;
                if (nodeColors[i] > colors->length)
                {
                    ntw_vector* newColor = calloc(1, sizeof *newColor);
                    if (!newColor)
                    {
                        fprintf(stderr, "%s: Error when allocating memory for coloring.\n", __func__);
                        exit(EXIT_FAILURE);
                    }
                    NTW_vector_add(colors, newColor);
                }
                NTW_vector_add(colors->data[nodeColors[i]-1], (void *) (uint64_t) i-1);
                maxColor = 0;
            }
            curr_row = edge_nodes[0];
        }
        edges_read++;
    }
	
	for (uint32_t i = curr_row + 1; i <= node_num; i++)
    {
        if (i == curr_row+1) 
        {
            nodeColors[i] = maxColor + 1;
            if (nodeColors[i] > colors->length)
            {
                ntw_vector* newColor = calloc(1, sizeof *newColor);
                if (!newColor)
                {
                    fprintf(stderr, "%s: Error when allocating memory for coloring.\n", __func__);
                    exit(EXIT_FAILURE);
                }
                NTW_vector_add(colors, newColor);
            }
        }
        else 
        {
            nodeColors[i] = 1;
        }
        NTW_vector_add(colors->data[nodeColors[i]-1], (void *) (uint64_t) i-1);
    	row_ptr[i] = edge_num;
    }

    //NTW_DEBUG_printArray_uint32(stderr, nodeColors+1, node_num);
    NTWPR_WGF_rewind(wgf);
    free(nodeColors);
	return NTW_CRS_new(node_num, edge_num, row_ptr, col_ind, val);
}

ntw_crs* NTWPR_WGF_load2crs(NTWPR_WGFile* restrict wgf)
{
    const uint32_t node_num = wgf->node_num;
    const uint32_t edge_num = wgf->edge_num; 
    uint32_t* row_ptr = calloc(node_num + 1, sizeof *row_ptr);
	uint32_t* col_ind = calloc(edge_num, sizeof *col_ind);
	double* val = calloc(edge_num, sizeof *val);
    
	if (!val || !col_ind || !row_ptr)
    {
        fprintf(stderr, "%s: Error when allocating memory for the CRS struct.\n", __func__);
        exit(EXIT_FAILURE);
    }

    uint32_t edges_read = 0;
    uint32_t curr_row = 0;
    uint32_t edge_nodes[2];

	// Skip node_num and edge_num
    fseek(wgf->edge_data, 2*sizeof(uint32_t), SEEK_SET);
    while (fread(edge_nodes, sizeof(edge_nodes[0]), 2, wgf->edge_data) == 2)
    {
        col_ind[edges_read] = edge_nodes[1];
        val[edges_read] = 1.0;
    
        if (edge_nodes[0] > curr_row)
        {
            for (int i = curr_row + 1; i <= edge_nodes[0]; i++)
            {
                row_ptr[i] = edges_read;
            }
            curr_row = edge_nodes[0];
        }
        edges_read++;
    }
	
	for (uint32_t i = curr_row + 1; i <= node_num; i++)
    	row_ptr[i] = edge_num;

    NTWPR_WGF_rewind(wgf);

	return NTW_CRS_new(node_num, edge_num, row_ptr, col_ind, val);
}

NTWPR_WGFile* NTWPR_WGF_fopen(const char path[static 1])
{
    NTWPR_WGFile* wgfile_p = malloc(sizeof(NTWPR_WGFile));
    FILE* edge_data_file = fopen(path, "rb");
    NTWPR_invalid_fp_exit(edge_data_file);
    
    uint32_t NTWPR_size[2];
    if (fread(NTWPR_size, sizeof(NTWPR_size[0]), 2, edge_data_file) != 2)
    {
        fprintf(stderr, "Error at reading edge_data_file size data.\n");
        exit(EXIT_FAILURE);
    }

    fseek(edge_data_file, 0, SEEK_SET);

    wgfile_p->edge_data = edge_data_file;
    wgfile_p->node_num = NTWPR_size[0];
    wgfile_p->edge_num = NTWPR_size[1];

    return wgfile_p;
}

int NTWPR_WGF_fclose(NTWPR_WGFile* wgfile)
{
    int return_val;
    if ((return_val = fclose(wgfile->edge_data)))
    {
        fprintf(stderr, "Error at closing the NTWPR_WGFile\n");
        return return_val;
    }
    free(wgfile);

    return 0;
}

unsigned NTWPR_WGF_rewind(NTWPR_WGFile* const wgf)
{
    return !fseek(wgf->edge_data, 0, SEEK_SET);
}

void NTWPR_WGF_exportFM(NTWPR_WGFile* restrict wgfp, const char exportPath[static 1], uint32_t nodeNum)
{
    FILE* NTWPR_out_fp = fopen(exportPath, "w");

    // Input/Output file checking
    NTWPR_invalid_fp_exit(wgfp->edge_data);
    NTWPR_invalid_fp_exit(NTWPR_out_fp);
 
    uint32_t edge_nodes[2];
    uint32_t line_counter = 0, col_counter = 0;

    if (nodeNum > wgfp->node_num) nodeNum = wgfp->node_num;

    fseek(wgfp->edge_data, 2*sizeof(uint32_t), SEEK_SET);
    while(line_counter < wgfp->node_num)
    {
        if (fread(edge_nodes, sizeof(edge_nodes[0]), 2, wgfp->edge_data) != 2)
        {
            if (line_counter == wgfp->node_num - 1)
            {
                //last node fill remaining zeros:
                edge_nodes[0] = wgfp->node_num;
            }
            else
            {
                fprintf(stderr, "Error at reading wgfp edge data.\n");
                exit(EXIT_FAILURE);
            }
        }

        // Skip edges to nodes outside of the requested range.
        if (edge_nodes[1] >= nodeNum) continue;

        // Fill line with zeros if there are no more edges from this
        // edge_nodes[0]. Next lines too until the next edge_nodes[0].
        while (line_counter < edge_nodes[0] && line_counter < nodeNum)
        {
            while (col_counter < nodeNum)
            {
                if (col_counter != nodeNum - 1)
                    fputs("0 ", NTWPR_out_fp);
                else
                    fputc('0', NTWPR_out_fp);
                col_counter++;
            }

            fputc('\n', NTWPR_out_fp);
            col_counter = 0;
            line_counter++;
        }

        // Skip edges to nodes outside the requested matrix
        // This works because the struct containssorted nodes.
        if (edge_nodes[0] >= nodeNum) break;

        // The nodes are sorted so add zeros to the non-edges before
        // the edge described in the latest read line.
        while(col_counter < edge_nodes[1] && col_counter < nodeNum)
        {
            if (col_counter != nodeNum - 1)
                fputs("0 ", NTWPR_out_fp);
            else
                fputc('0', NTWPR_out_fp);
            col_counter++;
        }

        // Add one to the edge indicated in the latest read line.
        if (edge_nodes[1] != nodeNum-1)
            fputs("1 ", NTWPR_out_fp);
        else
            fputs("1", NTWPR_out_fp);
        
        col_counter++;
    }

    NTWPR_WGF_rewind(wgfp);
    fclose(NTWPR_out_fp);
}

void NTWPR_WGF_exportSM(NTWPR_WGFile* restrict wgfp, const char exportPath[static 1], uint32_t nodeNum)
{
    FILE* NTWPR_out_fp = fopen(exportPath, "w");

    // Input/Output file checking
    NTWPR_invalid_fp_exit(wgfp->edge_data);
    NTWPR_invalid_fp_exit(NTWPR_out_fp);
 
    uint32_t edge_nodes[2];
    uint32_t i = 0;

    if (fseek(wgfp->edge_data, 2*sizeof(uint32_t), SEEK_SET))
    {
        fprintf(stderr, "%s: Error at reading WGFile data.\n", __func__);
        exit(EXIT_FAILURE);
    }

    while(i++ < wgfp->edge_num)
    {
        if (fread(edge_nodes, sizeof(edge_nodes[0]), 2, wgfp->edge_data) != 2)
        {
            fprintf(stderr, "%s: Error at reading WGFile data at iteration: %u.\n", __func__, i);
            exit(EXIT_FAILURE);
        }
        
        if (edge_nodes[0] >= nodeNum) break;
        if (edge_nodes[1] >= nodeNum) continue;

        if (fprintf(NTWPR_out_fp, "%u\t%u\n", edge_nodes[0], edge_nodes[1]) < 0)
        {
            fprintf(stderr, "Error while writing the sparse matrix at the output file\n");
            exit(EXIT_FAILURE);
        }
    }

    NTWPR_WGF_rewind(wgfp);
    fclose(NTWPR_out_fp);
}

void NTWPR_WGF_convert2Transpose(const char origWGFPath[static 1], const char exportWGFPath[static 1])
{
	NTWPR_WGFile* wgfFrom = NTWPR_WGF_fopen(origWGFPath);

	const uint32_t sizes[2] = {	[0] = wgfFrom->node_num,
								[1] = wgfFrom->edge_num, };

	NTWPR_WGEdge* edges = malloc(sizes[1] * sizeof *edges);
	if (!edges)
	{
		fprintf(stderr, "%s: Error at allocating memory needed for convertion.\n", __func__);
		exit(EXIT_FAILURE);
	}
	fseek(wgfFrom->edge_data, 2 * sizeof(uint32_t), SEEK_SET);

	if (fread(edges, sizeof(*edges), sizes[1], wgfFrom->edge_data) != sizes[1])
	{
		fprintf(stderr, "%s: Error at reading web graph data from %s.\n", __func__, origWGFPath);
		exit(EXIT_FAILURE);
	}

	NTWPR_WGF_fclose(wgfFrom);
	// Sort them incrementaly based on edgeB. And transpose them.
	qsort(edges, sizes[1], sizeof(edges[0]), NTWPR_WGF_edgeCompareForT);
	NTWPR_WGF_transposeEdges(sizes[1], edges);
/* DEBUG:
	for (int i = 0; i < edge_num; i++)
		printf("%u\t%u\n", edges[i].nodeA, edges[i].nodeB);
*/

	// Create the new file/replace.
	FILE* fp = fopen(exportWGFPath, "wb");
	if (!fp)
	{
		fprintf(stderr, "%s: Error at creating file at path %s\n", __func__, exportWGFPath);
		exit(EXIT_FAILURE); 
	}
	if (fwrite(sizes, sizeof(sizes[0]), 2, fp) != 2)
	{
		fprintf(stderr, "Error at writing to file at %s\n", exportWGFPath);
		exit(EXIT_FAILURE);
	}
	if (fwrite(edges, sizeof(edges[0]), sizes[1], fp) != sizes[1])
	{
		fprintf(stderr, "Error at writing to file at %s\n", exportWGFPath);
		exit(EXIT_FAILURE);
	}

	fclose(fp);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
void NTWPR_WGF_convertSU(const char suDataPath[static 1], const char exportPath[static 1], uint32_t nodeNum)
{
    FILE* NTWPR_SU_fp = fopen(suDataPath, "r");
    FILE* NTWPR_WGFile_fp = fopen(exportPath, "wb");

    // Input/Output file checks
    NTWPR_invalid_fp_exit(NTWPR_SU_fp);
    NTWPR_invalid_fp_exit(NTWPR_WGFile_fp);

    // Buffer variables and counters.
    char line_buffer[125];
    uint32_t sizes[2];
    uint32_t registered_edges = 0;
    NTWPR_WGEdge* restrict edges;
    
    unsigned read_data_size = 0; //flag . bad name I know.
    do{
        if (!fgets(line_buffer, 125, NTWPR_SU_fp))
        {
            // If EOF has been encountered and no characters are read,
            // fgets() returns null.
            break;
        }

        // Skip comments and write the node and edge number to the output.
        if (line_buffer[0] == LINE_COMMENT_SU)
        {
            if (read_data_size) continue;

            if (sscanf(line_buffer, "%*c %*s %u %*s %u", &sizes[0], &sizes[1]) != 2)
            {
                continue;
            }

            read_data_size = 1;
            // Write the read edge and nodes' number
            if (fwrite(sizes, sizeof(sizes[0]), 2, NTWPR_WGFile_fp) != 2)
            {
                fprintf(stderr, "Error at writing to file at %s\n", exportPath);
                exit(EXIT_FAILURE);
            }
            
            edges = malloc(sizes[1] * sizeof *edges);
            if (!edges)
            {
                fprintf(stderr, "%s: Error at allocating memory for the convertion.\n", __func__);
                exit(EXIT_FAILURE);
            }
            // Read the number of nodes that are in the description of the file.
            if (nodeNum == 0) nodeNum = sizes[0];
            continue;
        }

        if (!read_data_size) 
        {
            fprintf(stderr, "Couldn't read data size at starting comments of %s\n", exportPath);
            exit(EXIT_FAILURE);
        }
        // Read the nodes of the edge from the file's line.
        if (sscanf(line_buffer, "%u %u", &sizes[0], &sizes[1]) != 2)
        {
            fprintf(stderr, "%s:%d: found corrupted edge line in input file: %s", __FILE__, __LINE__, line_buffer);
            exit(EXIT_FAILURE);
        }

        // Stay in range.
        if (sizes[0] >= nodeNum || sizes[1] >= nodeNum) 
        {
            continue;
        }
        
        // If in range, write the sparse data.
        
        edges[registered_edges++] = (NTWPR_WGEdge) {.nodeA = sizes[0], .nodeB = sizes[1]};
        
    } while(!feof(NTWPR_SU_fp));

    qsort(edges, registered_edges, sizeof(edges[0]), NTWPR_WGF_edgeCompare);

    if (fwrite(edges, sizeof(edges[0]), registered_edges, NTWPR_WGFile_fp) != registered_edges)
    {
        fprintf(stderr, "Error at writing to file at %s\n", exportPath);
        exit(EXIT_FAILURE);
    }

    // Write the real number of edges/nodes if not all
    if (nodeNum != UINT32_MAX)
    {
        sizes[0] = nodeNum;
        sizes[1] = registered_edges;
        fseek(NTWPR_WGFile_fp, 0, SEEK_SET);

        if (fwrite(sizes, sizeof(sizes[0]), 2, NTWPR_WGFile_fp) != 2)
        {
            fprintf(stderr, "Error at writing to file at %s\n", exportPath);
            exit(EXIT_FAILURE);
        }
    }

    free(edges);
    fclose(NTWPR_SU_fp);
    fclose(NTWPR_WGFile_fp);
}
#pragma GCC diagnostic pop

void NTWPR_WGF_transposeEdges(const uint64_t n, NTWPR_WGEdge edges[static n])
{
	for (uint64_t i = 0; i < n; i++)
	{
		uint32_t temp = edges[i].nodeA;
		edges[i].nodeA = edges[i].nodeB;
		edges[i].nodeB = temp;
	}
}

int NTWPR_WGF_edgeCompare(const void* restrict edgeA, const void* restrict edgeB)
{
    NTWPR_WGEdge* a = (NTWPR_WGEdge*) edgeA;
	NTWPR_WGEdge* b = (NTWPR_WGEdge*) edgeB;

	return (a->nodeA - b->nodeA);
}

int NTWPR_WGF_edgeCompareForT(const void* restrict edgeA, const void* restrict edgeB)
{
	NTWPR_WGEdge* a = (NTWPR_WGEdge*) edgeA;
	NTWPR_WGEdge* b = (NTWPR_WGEdge*) edgeB;

	return (a->nodeB - b->nodeB);
}
