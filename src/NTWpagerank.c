/**
 * @brief Calculates the pagerank vector of a web graph saved in an
 *          NTWPR_WGFile form.
 * 
 * Documentation for the whole API behind this program can be found at:
 *  https://nicktheway.github.io/Pagerank/html/index.html
 * 
 * @file NTWpagerank.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 26-09-2018
 */
#include <unistd.h>
#include "../include/ntwprp.h"
#include "../include/ntw_debug.h"
#include "../include/ntw_math.h"

/**
 * @brief Τhe max number of specific iterations that are allowed.
 * 
 */
const uint32_t NTWPR_Max_Iterations_Limit = 150;

/**
 * @brief Refactored main()'s code that handles command line arguments.
 * 
 * @param argc The number of command line arguments.
 * @param argv The command line arguments as char arrays.
 * @param threadNum Address of the number of threads variable.
 * @param delta Address of the convergence's delta variable.
 * @param tel_coeff Address of the teleportation coefficient's variable.
 * @param it_number Address of the specific_iterations variable.
 * @param log_file_path Address of the log file's path variable.
 * @param pagerank_file_path Address of the output pagerank's file path variable.
 * @param wg_file_path Address of the input web graph's file path variable.
 */
void parseArguments(int argc, char * const argv[argc+1], int* threadNum, double* delta, double* tel_coeff, 
                        int32_t *it_number, char** log_file_path, char** pagerank_file_path, char** wg_file_path);

/**
 * @brief Calculates the pagerank of a web graph.
 * 
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return int Usually 0 for successful termination, 1 for not.
 */
int main(int argc, char * const argv[argc+1])
{
    // Default values.
    int specificNumberOfThreads = 0; // 0 -> dynamic number, not chosen by the user.
    double delta = 1e-12;
    double tel_coeff = 0.15;
    int32_t specific_iterations = 0; // If non zero, the pagerank calculation will run exactly that many iterations.
    char* log_file_path = "./log.txt";
    char* pagerank_file_path = "./pagerank.data";
    char* wg_file_path = (void *) 0; // Must be provided as argument by the user.

    // Get wg_file_path. Possibly, update values from the default ones to the specified ones.
    parseArguments(argc, argv, &specificNumberOfThreads, &delta, &tel_coeff, &specific_iterations, &log_file_path, &pagerank_file_path, &wg_file_path);
    
    // Try to open the file at wg_file_path.
    NTWPR_WGFile* file = NTWPR_WGF_fopen(wg_file_path);

    // Create/open file to store logs (times mainly).
    FILE* log_fp = fopen(log_file_path, "w");
    if (!log_fp)
    {
        fprintf(stderr, "Error at log file creation, possibly a missing folder\n");
        return EXIT_FAILURE;
    }
    
    // Log file header. Describes the web graph.
    fprintf(log_fp, "File: %s, Nodes: %u, Edges: %u\n", wg_file_path, file->node_num, file->edge_num);

    // Structs used for time point values.
    struct timespec start, finish;

    // Load the web-graph to the memory in crs format.
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_crs* myCRS = NTWPR_WGF_load2crs(file);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Load to crs time", '\n');
    
    // Data coloring
    clock_gettime(CLOCK_MONOTONIC, &start);
    ntw_vector* colors = NTW_CRS_getColoredGroups(myCRS);
    ntw_CRSReshapeSequence* reshape_seq = NTW_CRS_getColorOptimizedIds(colors, myCRS->node_num);
    NTW_CRS_IdReshape(myCRS, reshape_seq);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    fprintf(log_fp, "Color groups: %u\n", colors->length);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Data coloring and matrix reshaping time", '\n');

    // Calculate the web-graph's pagerank.
    fprintf(log_fp, "NTWPR_pagerank:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    double* pr = NTWPR_colored_pagerank(myCRS, tel_coeff, delta, specific_iterations, colors, log_fp);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    NTW_DEBUG_printElapsedTime(log_fp, start, finish, "Whole pagerank time", '\n');
    
    // Return to the starting ids.
    NTWM_reshapeDV(reshape_seq->node_num, pr, reshape_seq->look_up);

    // Print the pagerank array to the file with pagerank_file_path.
    NTW_DEBUG_printBinaryDoubleArray(pagerank_file_path, myCRS->node_num, pr);

    // Clean up and terminate.
    for (uint32_t i = 0; i < colors->length; i++)
    {
        NTW_vector_free(colors->data[i]);
    }
    free(pr);
    NTW_CRS_reshapeSec_free(reshape_seq);
    NTW_vector_free(colors);
    NTWPR_WGF_fclose(file);
    NTW_CRS_free(myCRS);
    fclose(log_fp);

    return EXIT_SUCCESS;
}

void parseArguments(int argc, char * const argv[argc+1], int* threadNum, double *delta, double *tel_coeff, 
                        int32_t *it_number, char** log_file_path, char** pagerank_file_path, char** wg_file_path)
{
    /**
     * Argument parsing.
     * opting and optarg are defined in <unistd.h>
     */
    int opt = 0; 
    while (optind < argc) 
    {
        if ((opt = getopt(argc, argv, "l:o:E:d:t:i:")) != -1)
        {
            switch (opt) 
            {
                case 'l':
                    *log_file_path = optarg;
                    break;
                case 'o':
                    *pagerank_file_path = optarg;
                    break;
                case 't':
                    *threadNum = atoi(optarg);
                    if (*threadNum <= 0)
                    {
                        printf("The number of threads [-t thread_number] must be a positive integer value.\n"
                                "Currently entered value: %s\n", optarg);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'E':
                    if (sscanf(optarg, "%lf", delta) != 1 || *delta <= 1e-36 || *delta >= 1e-1)
                    {
                        printf("The convergence_delta must be in (1e-36, 0.1). Scientific notation (eg. 1e-4) is also supported.\n"
                                "Currently entered value: %s\n", optarg);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'd':
                    if (sscanf(optarg, "%lf", tel_coeff) != 1 || *tel_coeff <= 0 || *tel_coeff >= 1)
                    {
                        printf("The teleportation_coefficient must be in (0, 1).\nCurrently entered value: %s\n", optarg);
                        exit(EXIT_FAILURE);
                    }
                    break;
                case 'i':
                    *it_number = atoi(optarg);
                    if (*it_number <= 0 || *it_number > NTWPR_Max_Iterations_Limit)
                    {
                        printf("The specified number of iterations must be a positive integer not bigger than %u\n", NTWPR_Max_Iterations_Limit);
                        exit(EXIT_FAILURE);
                    }
                    break;
                default:
                    fprintf(stderr, "Usage: %s input_NTW_WGFile_path [-E convergence_delta][-d teleportation_coefficient][-i specific_iterations_number]\n"
                                    "\t\t\t\t\t\t[-t specific_thread_number][-o pagerank_vector_binary_file_path][-l log_text_file_path]\n",
                            argv[0]);
                    exit(EXIT_FAILURE);
            }
        }
        else
        {
            if (!*wg_file_path)
                *wg_file_path = argv[optind];
            else {
                fprintf(stderr, "Unsupported number of non-flagged arguments.\n");
                exit(EXIT_FAILURE);
            }
            optind++;
        }
    }

    if (!*wg_file_path) 
    {
        fprintf(stderr, "Mandatory WebGraph file argument has not been provided.\n");
        exit(EXIT_FAILURE);
    }

    // Set the number of threads.
    if (*threadNum > 0)
    {
        omp_set_dynamic(0);
        omp_set_num_threads(*threadNum);
    }
}
