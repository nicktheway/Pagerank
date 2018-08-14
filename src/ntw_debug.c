/**
 * @brief 
 * 
 * @file ntw_debug.c
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 14-08-2018
 */
#include <stdio.h>
#include <time.h>

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