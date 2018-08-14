/**
 * @brief Contains the declarations of useful debugging functions.
 * 
 * @file ntw_debug.h
 * @author Katomeris Nikolaos, 8551, ngkatomer@auth.gr
 * @date 14-08-2018
 */
#ifndef NTW_DEBUG_H
#define NTW_DEBUG_H 1

#include <stdio.h>
#include <time.h>

/**
 * @brief Prints: @a message: <elapsed time from @a start to @a finish in ms> to @a stream.
 * 
 * @param stream Where the debug message will be printed. Must be open. 
 * @param start The starting time point.
 * @param finish The end's time point.
 * @param message Debug message to print before the elapsed time.
 */
void NTW_DEBUG_printElapsedTime(FILE* restrict stream, const struct timespec start, const struct timespec finish, const char message[static 1]);

/**
 * @brief Creates a binary file at @a filePath with the @a array's @a n values.
 * 
 * If unsuccessful will exit(EXIT_FAILURE) with a debug message!
 * 
 * @param filePath 
 * @param n 
 * @param array 
 */
void NTW_DEBUG_printBinaryDoubleArray(const char filePath[static 1], const size_t n, const double array[static n]);

#endif