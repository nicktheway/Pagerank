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
#include <stdint.h>
#include <time.h>

/**
 * @brief Prints: @a message: <elapsed time from @a start to @a finish in ms> to @a stream.
 * 
 * @param stream Where the debug message will be printed. Must be open. 
 * @param start The starting time point.
 * @param finish The end's time point.
 * @param message Debug message to print before the elapsed time.
 * @param endDelim How the debug message will end. Eg: '\n'
 */
void NTW_DEBUG_printElapsedTime(FILE* restrict stream, const struct timespec start, const struct timespec finish, const char message[static 1], const char endDelim);

/**
 * @brief Creates a binary file at @a filePath with the @a array's @a n values.
 * 
 * If unsuccessful will exit(EXIT_FAILURE) with a debug message!
 * 
 * @param filePath 
 * @param n 
 * @param array 
 */
void NTW_DEBUG_printBinaryDoubleArray(const char filePath[static 1], const uint64_t n, const double array[static n]);

/**
 * @brief @a array is printed to @a stream.
 * 
 * @param stream Where the array will be printed. 
 * @param array The array to be printed.
 * @param n The size of the @a array.
 */
void NTW_DEBUG_printArray_uint32(FILE* restrict stream, const uint32_t* const array, const uint64_t n);

/**
 * @brief @a array is printed to @a stream.
 * 
 * @param stream Where the array will be printed. 
 * @param array The array to be printed.
 * @param n The size of the @a array.
 */
void NTW_DEBUG_printArray_uint64(FILE* restrict stream, const uint64_t* const array, const uint64_t n);
#endif