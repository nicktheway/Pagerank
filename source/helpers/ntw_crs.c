/**
 * @brief The definitions for the functions declared at ntw_crs.h .
 * 
 * @file ntw_crs.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 11-08-2018
 */

ntw_crs* NTW_CRS_new(const uint32_t nodeNum, const uint32_t edgeNum, const uint32_t rowPtr[static nodeNum+1], const uint32_t colInd[static edgeNum], const double val[static edgeNum])
{
    ntw_crs* newCrs = malloc(sizeof(*newCrs));
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
    for (int i = 0; i < crs->edge_num; i++)
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

void NTW_CRS_rowNorm(ntw_crs crs[static 1])
{
    for (int i = 0; i < crs->node_num; i++)
    {
        uint32_t diff_value = crs->row_ptr[i+1] - crs->row_ptr[i];
        for (int j = crs->row_ptr[i]; j < crs->row_ptr[i+1]; j++)
        {
            crs->val[j] /= diff_value;
        }
    }
}

void NTW_CRS_rowNormUnif(ntw_crs crs[static 1])
{
    for (int i = 0; i < crs->node_num; i++)
    {
        uint32_t diff_value = crs->row_ptr[i+1] - crs->row_ptr[i];
        if (diff_value == 0 || diff_value == 1) continue;
        
        register double u_value = 1.0f / diff_value;
        for (int j = crs->row_ptr[i]; j < crs->row_ptr[i+1]; j++)
        {
            crs->val[j] = u_value;
        }
    }
}

double NTW_CRS_valueAt(const ntw_crs crs[static 1], uint32_t row, uint32_t col)
{
    for (int i = crs->row_ptr[row]; i < crs->row_ptr[row+1]; i++)
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
    for (int i = 0; i < crs->edge_num; i++)
    {
        fprintf(stream, "%u\t", crs->col_ind[i]);
    }
    fprintf(stream, "\nValues:\n");
    for (int i = 0; i < crs->edge_num; i++)
    {
        fprintf(stream, "%.2f\t", crs->val[i]);
    }
    fprintf(stream, "\nRow pointers:\n");
    for (int i = 0; i <= crs->node_num; i++)
    {
        fprintf(stream, "%u\t", crs->row_ptr[i]);
    }
    fprintf(stream, "\n");
}

void NTW_CRS_printFullMatrix(FILE* restrict stream, const ntw_crs crs[static 1])
{
	// Parse rows
	for (int i = 0; i < crs->node_num; i++)
    {
		// The column index of the element to print
		int curr_col = 0;

		// Print row i (if it is not all-zero)
		for (int k = crs->row_ptr[i]; k < crs->row_ptr[i+1]; k++)
        {
			int j = crs->col_ind[k];

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