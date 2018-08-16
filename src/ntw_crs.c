/**
 * @brief The definitions for the functions declared at ntw_crs.h .
 * 
 * @file ntw_crs.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/ntw_crs.h"

ntw_crs* NTW_CRS_new(const uint32_t nodeNum, const uint32_t edgeNum, uint32_t rowPtr[static nodeNum+1], uint32_t colInd[static edgeNum], double val[static edgeNum])
{
    ntw_crs* const newCrs = malloc(sizeof(*newCrs));
    if (newCrs)
    {
        newCrs->node_num = nodeNum;
        newCrs->edge_num = edgeNum;
        newCrs->row_ptr = rowPtr;
        newCrs->col_ind = colInd;
        newCrs->val = val;
    }

    return newCrs;
}

void NTW_CRS_free(ntw_crs* crs)
{
    free(crs->val);
    free(crs->col_ind);
    free(crs->row_ptr);

    free(crs);
}

void NTW_CRS_cmult(ntw_crs crs[static 1], const double c)
{
    for (uint32_t i = 0; i < crs->edge_num; i++)
    {
        crs->val[i] *= c;
    }
}

void NTW_CRS_vmult(const ntw_crs crs[static 1], const double vector[static 1], double product[static crs->node_num])
{
    for (uint32_t ntw_i = 0; ntw_i < crs->node_num; ntw_i++)
    {
        for (uint32_t ntw_j = crs->row_ptr[ntw_i]; ntw_j < crs->row_ptr[ntw_i + 1]; ntw_j++)
        {
            product[ntw_i] += crs->val[ntw_j] * vector[crs->col_ind[ntw_j]];
        }
    }
}

double* NTW_CRS_vmultAlloc(const ntw_crs crs[static 1], const double vector[static 1])
{
    double* product = calloc(crs->node_num, sizeof(*product));
    if (!product)
    {
        fprintf(stderr, "%s: Error at memory allocation.\n", __func__);
        exit(EXIT_FAILURE);
    }
    NTW_CRS_vmult(crs, vector, product);

    return product;
}

void NTW_CRS_vmultTranspose(const ntw_crs crs[static 1], const double vector[static 1], double product[static 1])
{
	for (uint32_t j = 0; j < crs->node_num; j++)
	{
		for (uint32_t i = crs->row_ptr[j]; i < crs->row_ptr[j + 1]; i++)
		{
			product[crs->col_ind[i]] += crs->val[i] * vector[j];
		}
	}
}

void NTW_CRS_stochasticizeRows(ntw_crs crs[static 1])
{
    for (uint32_t i = 0; i < crs->node_num; i++)
    {
        const uint32_t diff_value = crs->row_ptr[i+1] - crs->row_ptr[i];
        if (diff_value == 0 || diff_value == 1) continue;
        
        const double u_value = 1.0f / diff_value;
        for (uint32_t j = crs->row_ptr[i]; j < crs->row_ptr[i+1]; j++)
        {
            crs->val[j] = u_value;
        }
    }
}

void NTW_CRS_stochasticizeCols(ntw_crs crs[static 1])
{
	// Allocate memory array to store the number of non zero elements in each column.
	uint32_t* counter = calloc(crs->node_num, sizeof *counter);
	if (!counter)
	{
		fprintf(stderr, "%s: There is not enough memory available for this process\n", __func__);
		exit(EXIT_FAILURE);
	}
	for (uint32_t i = 0; i < crs->edge_num; i++)
	{
		counter[crs->col_ind[i]]++;
	}
	for (uint32_t i = 0; i < crs->edge_num; i++)
	{
		if (counter[crs->col_ind[i]])
			crs->val[i] = 1.0 / counter[crs->col_ind[i]];
	}
	free(counter);
}

uint32_t NTW_CRS_getEmptyRowsNum(const ntw_crs crs[static 1])
{
	uint32_t counter = 0;
	for (uint32_t i = 0; i < crs->node_num; i++)
	{
		counter += (crs->row_ptr[i] == crs->row_ptr[i+1]);	
	}
	return counter;
}

size_t* NTW_CRS_getEmptyRowIndices(const ntw_crs crs[static 1], uint32_t* restrict outIndicesNum)
{
	*outIndicesNum = NTW_CRS_getEmptyRowsNum(crs);
	if (*outIndicesNum == 0) 
		return (void *) 0;

	size_t* emptyRows = malloc(*outIndicesNum * sizeof *emptyRows);
	if (!emptyRows)
	{
		fprintf(stderr, "%s: Error at allocating memory for the row indices\n", __func__);
		exit(EXIT_FAILURE);
	}
	uint32_t counter = 0;
	for (uint32_t i = 0; i < crs->node_num; i++)
	{
		if (crs->row_ptr[i] == crs->row_ptr[i+1])
		{
			emptyRows[counter++] = i;
			if (counter == *outIndicesNum) break;
		}
	}

	return emptyRows;
}

double NTW_CRS_valueAt(const ntw_crs crs[static 1], uint32_t row, uint32_t col)
{
    for (uint32_t i = crs->row_ptr[row]; i < crs->row_ptr[row+1]; i++)
    {
        if (crs->col_ind[i] == col)
            return crs->val[i];
        else if (crs->col_ind[i] > col)
            return 0;
    }
    return 0;
}

void NTW_CRS_print(FILE* restrict stream, const ntw_crs crs[static 1])
{
    fprintf(stream, "Nodes: %u, Edges: %u\nColumn indeces:\n", crs->node_num, crs->edge_num);
    for (uint32_t i = 0; i < crs->edge_num; i++)
    {
        fprintf(stream, "%u\t", crs->col_ind[i]);
    }
    fprintf(stream, "\nValues:\n");
    for (uint32_t i = 0; i < crs->edge_num; i++)
    {
        fprintf(stream, "%.2f\t", crs->val[i]);
    }
    fprintf(stream, "\nRow pointers:\n");
    for (uint32_t i = 0; i <= crs->node_num; i++)
    {
        fprintf(stream, "%u\t", crs->row_ptr[i]);
    }
    fprintf(stream, "\n");
}

void NTW_CRS_printFullMatrix(FILE* restrict stream, const ntw_crs crs[static 1])
{
	// Parse rows
	for (uint32_t i = 0; i < crs->node_num; i++)
    {
		// The column index of the element to print
		uint32_t curr_col = 0;

		// Print row i (if it is not all-zero)
		for (uint32_t k = crs->row_ptr[i]; k < crs->row_ptr[i+1]; k++)
        {
			uint32_t j = crs->col_ind[k];

			// Print zeros between the non-zeros of the line
			while(curr_col < j)
            {
				fprintf(stream, "%.2f\t", 0.0f);
				curr_col++;
			}
			// print the nonzero value
			fprintf(stream, "%.2f\t", crs->val[k]);;
			curr_col++;
		}
		
        // Print the trailing zeroes of this row
		while (curr_col < crs->node_num)
        {
			fprintf(stream, "%.2f\t", 0.0f);
			curr_col++;
		}

		fprintf(stream, "\n");
	}
	fprintf(stream, "\n");
}
