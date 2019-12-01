#ifndef DIAMOND_SQUARE_H
#define DIAMOND_SQUARE_H

#include <cstdlib>

#define DIM 129
#define NUM_VERTICES DIM * DIM
#define SCALE_FACTOR 16

void diamondSquare(int arr[][DIM], int size, int currSize);
void squareStep(int arr[][DIM], int size, int x, int z, int reach);
void diamondStep(int arr[][DIM], int size, int x, int z, int reach);
float random(int range);

#endif