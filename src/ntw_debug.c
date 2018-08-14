/**
 * @brief 
 * 
 * @file ntw_debug.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 14-08-2018
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void NTW_DEBUG_printElapsedTime(FILE* restrict stream, const struct timespec start, const struct timespec finish, const char message[static 1])
{
    time_t seconds = finish.tv_sec - start.tv_sec; 
    long nano_seconds = finish.tv_nsec - start.tv_nsec; 
    
    if (start.tv_nsec > finish.tv_nsec) {
        seconds--; 
        nano_seconds += 1e9; 
    } 
    fprintf(stream, "%s: %lf ms\n", message, seconds * 1000 + (double)nano_seconds/1e6);
}

void NTW_DEBUG_printBinaryDoubleArray(const char filePath[static 1], const size_t n, const double array[static n])
{
    FILE* fp = fopen(filePath, "wb");
    if (!fp)
    {
        fprintf(stderr, "%s: Error at opening file at path: %s\n", __func__, filePath);
        exit(EXIT_FAILURE);
    }

    if (fwrite(array, sizeof array[0], n, fp) != n)
    {
        fprintf(stderr, "%s: Error at writing array to the file at path: %s\n", __func__, filePath);
        exit(EXIT_FAILURE);
    }

    if (fclose(fp))
    {
        fprintf(stderr, "%s: Error at closing the file at path: %s\n", __func__, filePath);
        exit(EXIT_FAILURE);
    }
}