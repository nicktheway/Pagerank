/**
 * @brief File containing the implementations of wgimp.h
 * 
 * @file wgimp.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 09-08-2018
 */
#include "wgimp.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void NTW_expfm(FILE* ntw_in_fp, FILE* ntw_out_fp, size_t ntw_node_num)
{
    // Input/Output file checking
    NTW_invalid_fp_exit(ntw_in_fp);
    NTW_invalid_fp_exit(ntw_out_fp);

    char line_buffer[125]; 
    int node_a, node_b;
    int line_counter = 0, col_counter;

    do{
        if (!fgets(line_buffer, 125, ntw_in_fp))
        {
            // If EOF has been encountered and no characters are read,
            // fgets() returns null.
            break;
        }

        // Skip comments
        if (line_buffer[0] == DF_LINE_COMMENT) continue;

        // Read the nodes of the edge from the file's line.
        if (sscanf(line_buffer, "%d %d", &node_a, &node_b) != 2)
        {
            fprintf(stderr, "%s:%x: found corrupted edge line in input file."
                " Skipping...", __FILE__, __LINE__);
            continue;
        }

        // Skip nodes if they are not in requested range.
        if (node_b >= ntw_node_num) continue;
        
        // Fill line with zeros if there are no more edges from this
        // node_a. Next lines too until the next node_a.
        while (line_counter < node_a && line_counter < ntw_node_num)
        {
            while (col_counter < ntw_node_num)
            {
                if (col_counter != ntw_node_num - 1)
                    fputs("0 ", ntw_out_fp);
                else
                    fputc('0', ntw_out_fp);
                col_counter++;
            }

            fputc('\n', ntw_out_fp);
            col_counter = 0;
            line_counter++;
        }
 
        // Skip edges to nodes outside the requested matrix
        // This works because the files contain sorted nodes.
        if (node_a >= ntw_node_num) break;

        // The nodes are sorted so add zeros to the non-edges before
        // the edge described in the latest read line.
        while(col_counter < node_b && col_counter < ntw_node_num)
        {
            if (col_counter != ntw_node_num - 1)
                fputs("0 ", ntw_out_fp);
            else
                fputc('0', ntw_out_fp);
            col_counter++;
        }
        
        // Add one to the edge indicated in the latest read line.
        if (node_b != ntw_node_num-1)
            fputs("1 ", ntw_out_fp);
        else
            fputs("1", ntw_out_fp);
        
        col_counter++;

    } while(!feof(ntw_in_fp));
}

void NTW_expsm(FILE* ntw_in_fp, FILE* ntw_out_fp, size_t ntw_node_num)
{
    // Input/Output file checks
    NTW_invalid_fp_exit(ntw_in_fp);
    NTW_invalid_fp_exit(ntw_out_fp);

    // Buffer variables and counters.
    char line_buffer[125];
    int node_a, node_b;
    int line_counter = 0, col_counter;
    
    do{
        if (!fgets(line_buffer, 125, ntw_in_fp))
        {
            // If EOF has been encountered and no characters are read,
            // fgets() returns null.
            break;
        }

        // Skip comments
        if (line_buffer[0] == DF_LINE_COMMENT) continue;

        // Read the nodes of the edge from the file's line.
        if (sscanf(line_buffer, "%d %d", &node_a, &node_b) != 2)
        {
            fprintf(stderr, "%s:%x: found corrupted edge line in input file."
                " Skipping...", __FILE__, __LINE__);
            continue;
        }

        // Stay in range.
        if (node_b >= ntw_node_num) continue;
        if (node_a >= ntw_node_num) break;

        // If in range, write the sparce data.
        fprintf(ntw_out_fp, "%d %d\n", node_a + 1, node_b + 1);

    } while(!feof(ntw_in_fp));
}