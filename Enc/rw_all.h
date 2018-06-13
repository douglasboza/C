#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int quantidade_pixels_ppm;
int quantidade_pixels_bmp;


#ifndef RW_ALL_H
#define RW_ALL_H

void char_binary(char c, int *binario); 

unsigned int int_to_bin(int n);

int binary_to_int(int n);

unsigned int set_bits(char bit_in, unsigned int bits_out);

void desloca_zeros_inicio(char* binario);

int binary_to_text(char *inputFile, char *outputFile);

#endif
