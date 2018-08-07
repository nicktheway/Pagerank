#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void create_complete_matrix(FILE* data_file_p, FILE* matrix_file_p, int N);
void create_sparce_matric(FILE* data_file_p, FILE* matrix_file_p, int N);

int main(int argc, char** argv)
{

    if (argc != 5)
    {
        printf("Please specify an input and an output file. Also specify the dimension N of the NxN resulting matrix and 's' for sparse.\n");
        return 1;
    }
    
    FILE* data_file_p;
    FILE* matrix_file_p;
    data_file_p = fopen(argv[1], "r");
    matrix_file_p = fopen(argv[2], "w");
    int N = atoi(argv[3]);
    if (N <= 0)
    {
        printf("Invalid matrix dimension number\n");
        return 1;
    }

    if (argv[4][0] == 's')
        create_sparce_matric(data_file_p, matrix_file_p, N);
    else 
        create_complete_matrix(data_file_p, matrix_file_p, N);

    fclose(matrix_file_p);
    fclose(data_file_p);
    return 0;
}

void create_complete_matrix(FILE* data_file_p, FILE* matrix_file_p, int N)
{
    char line_buffer[125];
    int node_a, node_b;
    int line_counter = 0, col_counter;

    do{
        fgets(line_buffer, 124, data_file_p);

        // Skip comments
        if (line_buffer[0] == '#') continue;

        // Read numbers
        sscanf(line_buffer, "%d %d", &node_a, &node_b);

        while(col_counter < node_b && col_counter < N)
        {
            if (col_counter != N - 1)
                fputs("0 ", matrix_file_p);
            else
                fputc('0', matrix_file_p);
            col_counter++;
        }

        if (node_b >= N) continue;

        while (line_counter < node_a && line_counter < N)
        {
            while (col_counter < N)
            {
                if (col_counter != N - 1)
                    fputs("0 ", matrix_file_p);
                else
                    fputc('0', matrix_file_p);
                col_counter++;
            }
            // if (col_counter == N - 1) fputc('0', matrix_file_p);

            fputc('\n', matrix_file_p);
            col_counter = 0;
            line_counter++;
        }
 
        // Skip edges to nodes outside the requested matrix
        // The files contain sorted nodes.
        if (node_a >= N) break;

        while(col_counter < node_b && col_counter < N)
        {
            if (col_counter != N - 1)
                fputs("0 ", matrix_file_p);
            else
                fputc('0', matrix_file_p);
            col_counter++;
        }
        
        if (node_b != N-1)
            fputs("1 ", matrix_file_p);
        else
            fputs("1", matrix_file_p);
        
        col_counter++;

    } while(!feof(data_file_p));
}

void create_sparce_matric(FILE* data_file_p, FILE* matrix_file_p, int N)
{
    char line_buffer[125];
    int node_a, node_b;
    int line_counter = 0, col_counter;
    
    do{
        fgets(line_buffer, 124, data_file_p);

        // Skip comments
        if (line_buffer[0] == '#') continue;

        // Read numbers
        sscanf(line_buffer, "%d %d", &node_a, &node_b);

        if (node_b >= N) continue;
        if (node_a >= N) break;
        // Write sparce data
        fprintf(matrix_file_p, "%d %d\n", node_a + 1, node_b + 1);

    } while(!feof(data_file_p));
}