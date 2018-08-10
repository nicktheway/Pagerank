/**
 * @brief File containing the implementations of ntwpr_wg.h
 * 
 * @file ntwpr_wg.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 10-08-2018
 */
#include "ntwpr_wg.h"

NTWPR_CRS* NTWPR_load2crs(NTWPR_WGFile* restrict ntwpr_wgfp)
{
    NTWPR_CRS* new_crs = calloc(1, sizeof(NTWPR_CRS));
    if (!new_crs)
    {
        fprintf(stderr, "Error when allocating memory for the CRS struct.\n");
        exit(EXIT_FAILURE);
    }
    new_crs->node_num = ntwpr_wgfp->node_num;
    new_crs->edge_num = ntwpr_wgfp->edge_num;

    new_crs->val = calloc(new_crs->edge_num, sizeof(float));
    new_crs->col_ind = calloc(new_crs->edge_num, sizeof(uint32_t));
    new_crs->row_ptr = calloc(new_crs->node_num + 1, sizeof(uint32_t));

    if (!new_crs->val || !new_crs->col_ind || !new_crs->row_ptr)
    {
        fprintf(stderr, "%s: Error when allocating memory for the CRS struct.\n", __func__);
        exit(EXIT_FAILURE);
    }

    uint32_t edges_read = 0;
    uint32_t curr_row = 0;
    uint32_t edge_nodes[2];
    fseek(ntwpr_wgfp->edge_data, 2*sizeof(uint32_t), SEEK_SET);
    while (fread(edge_nodes, sizeof(edge_nodes[0]), 2, ntwpr_wgfp->edge_data) == 2)
    {
        new_crs->col_ind[edges_read] = edge_nodes[1];
        new_crs->val[edges_read] = 1.0f;
        if (edge_nodes[0] > curr_row)
        {
            for (int i = curr_row + 1; i <= edge_nodes[0]; i++)
            {
                new_crs->row_ptr[i] = edges_read;
            }
            curr_row = edge_nodes[0];
        }
        edges_read++;
    }
    new_crs->row_ptr[new_crs->node_num] = new_crs->edge_num;

    NTWPR_WGrewind(ntwpr_wgfp);
    return new_crs;
}

void NTWPR_CRSfree(NTWPR_CRS* ntwpr_crs)
{
    free(ntwpr_crs->val);
    free(ntwpr_crs->col_ind);
    free(ntwpr_crs->row_ptr);

    free(ntwpr_crs);
}

void NTWPR_CRSprint(FILE* stream, NTWPR_CRS* ntwpr_crs)
{
    fprintf(stream, "Nodes: %u, Edges: %u\nColumn indeces:\n", ntwpr_crs->node_num, ntwpr_crs->edge_num);
    for (int i = 0; i < ntwpr_crs->edge_num; i++)
    {
        fprintf(stream, "%u\t", ntwpr_crs->col_ind[i]);
    }
    fprintf(stream, "\nValues:\n");
    for (int i = 0; i < ntwpr_crs->edge_num; i++)
    {
        fprintf(stream, "%.2f\t", ntwpr_crs->val[i]);
    }
    fprintf(stream, "\nRow pointers:\n");
    for (int i = 0; i <= ntwpr_crs->node_num; i++)
    {
        fprintf(stream, "%u\t", ntwpr_crs->row_ptr[i]);
    }
    fprintf(stream, "\n");
}

NTWPR_WGFile* NTWPR_WGfopen(const char path[static 1])
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

int NTWPR_WGfclose(NTWPR_WGFile* wgfile)
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

bool NTWPR_WGrewind(NTWPR_WGFile* const NTWPR_WGF)
{
    return !fseek(NTWPR_WGF->edge_data, 0, SEEK_SET);
}

void NTWPR_expfm(NTWPR_WGFile* restrict wgfp, const char exp_path[static 1], uint32_t NTWPR_node_num)
{
    FILE* NTWPR_out_fp = fopen(exp_path, "w");

    // Input/Output file checking
    NTWPR_invalid_fp_exit(wgfp->edge_data);
    NTWPR_invalid_fp_exit(NTWPR_out_fp);
 
    uint32_t edge_nodes[2];
    uint32_t line_counter = 0, col_counter = 0;

    if (NTWPR_node_num > wgfp->node_num) NTWPR_node_num = wgfp->node_num;

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
        if (edge_nodes[1] >= NTWPR_node_num) continue;

        // Fill line with zeros if there are no more edges from this
        // edge_nodes[0]. Next lines too until the next edge_nodes[0].
        while (line_counter < edge_nodes[0] && line_counter < NTWPR_node_num)
        {
            while (col_counter < NTWPR_node_num)
            {
                if (col_counter != NTWPR_node_num - 1)
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
        if (edge_nodes[0] >= NTWPR_node_num) break;

        // The nodes are sorted so add zeros to the non-edges before
        // the edge described in the latest read line.
        while(col_counter < edge_nodes[1] && col_counter < NTWPR_node_num)
        {
            if (col_counter != NTWPR_node_num - 1)
                fputs("0 ", NTWPR_out_fp);
            else
                fputc('0', NTWPR_out_fp);
            col_counter++;
        }

        // Add one to the edge indicated in the latest read line.
        if (edge_nodes[1] != NTWPR_node_num-1)
            fputs("1 ", NTWPR_out_fp);
        else
            fputs("1", NTWPR_out_fp);
        
        col_counter++;
    }

    NTWPR_WGrewind(wgfp);
    fclose(NTWPR_out_fp);
}

void NTWPR_expsm(NTWPR_WGFile* restrict wgfp, const char exp_path[static 1], uint32_t NTWPR_node_num)
{
    FILE* NTWPR_out_fp = fopen(exp_path, "w");

    // Input/Output file checking
    NTWPR_invalid_fp_exit(wgfp->edge_data);
    NTWPR_invalid_fp_exit(NTWPR_out_fp);
 
    uint32_t edge_nodes[2];
    uint32_t i = 0;

    fseek(wgfp->edge_data, 2*sizeof(uint32_t), SEEK_SET);
    while(i++ < wgfp->edge_num)
    {
        if (fread(edge_nodes, sizeof(edge_nodes[0]), 2, wgfp->edge_data) != 2)
        {
            fprintf(stderr, "Error at reading wgfp edge data.\n");
            exit(EXIT_FAILURE);
        }
        
        if (edge_nodes[0] >= NTWPR_node_num) break;
        if (edge_nodes[1] >= NTWPR_node_num) continue;

        if (fprintf(NTWPR_out_fp, "%u\t%u\n", edge_nodes[0], edge_nodes[1]) < 0)
        {
            fprintf(stderr, "Error while writing the sparse matrix at the output file\n");
            exit(EXIT_FAILURE);
        }
    }

    NTWPR_WGrewind(wgfp);
    fclose(NTWPR_out_fp);
}

void NTWPR_SU2WG(const char SU_WGD_path[static 1], const char exp_path[static 1], uint32_t NTWPR_node_num)
{
    FILE* NTWPR_SU_fp = fopen(SU_WGD_path, "r");
    FILE* NTWPR_WGFile_fp = fopen(exp_path, "wb");

    // Input/Output file checks
    NTWPR_invalid_fp_exit(NTWPR_SU_fp);
    NTWPR_invalid_fp_exit(NTWPR_WGFile_fp);

    // Buffer variables and counters.
    char line_buffer[125];
    uint32_t edge_nodes[2];
    uint32_t registered_edges = 0;

    if (NTWPR_node_num == 0) NTWPR_node_num = UINT32_MAX;
    bool read_data_size = false;
    do{
        if (!fgets(line_buffer, 125, NTWPR_SU_fp))
        {
            // If EOF has been encountered and no characters are read,
            // fgets() returns null.
            break;
        }

        // Skip comments and write the node and edge number to the output.
        if (line_buffer[0] == DF_SU_LINE_COMMENT)
        {
            if (read_data_size) continue;

            if (sscanf(line_buffer, "%*c %*s %u %*s %u", &edge_nodes[0], &edge_nodes[1]) != 2)
            {
                continue;
            }

            read_data_size = true;
            // Write the read edge and nodes' number
            if (fwrite(edge_nodes, sizeof(edge_nodes[0]), 2, NTWPR_WGFile_fp) != 2)
            {
                fprintf(stderr, "Error at writing to file at %s\n", exp_path);
                exit(EXIT_FAILURE);
            }
            continue;
        }

        // Read the nodes of the edge from the file's line.
        if (sscanf(line_buffer, "%u %u", &edge_nodes[0], &edge_nodes[1]) != 2)
        {
            fprintf(stderr, "%s:%d: found corrupted edge line in input file: %s", __FILE__, __LINE__, line_buffer);
            exit(EXIT_FAILURE);
        }

        // Stay in range.
        if (edge_nodes[1] >= NTWPR_node_num) continue;
        if (edge_nodes[0] >= NTWPR_node_num) break;

        // If in range, write the sparce data.
        registered_edges++;
        if (fwrite(edge_nodes, sizeof(edge_nodes[0]), 2, NTWPR_WGFile_fp) != 2)
        {
            fprintf(stderr, "Error at writing to file at %s\n", exp_path);
            exit(EXIT_FAILURE);
        }

    } while(!feof(NTWPR_SU_fp));

    // Write the real number of edges/nodes if not all
    if (NTWPR_node_num != UINT32_MAX)
    {
        edge_nodes[0] = NTWPR_node_num;
        edge_nodes[1] = registered_edges;
        fseek(NTWPR_WGFile_fp, 0, SEEK_SET);

        if (fwrite(edge_nodes, sizeof(edge_nodes[0]), 2, NTWPR_WGFile_fp) != 2)
        {
            fprintf(stderr, "Error at writing to file at %s\n", exp_path);
            exit(EXIT_FAILURE);
        }
    }

    fclose(NTWPR_SU_fp);
    fclose(NTWPR_WGFile_fp);
}