#include "diamondSquare.h"

void diamondSquare(int arr[][DIM], int size, int currSize)
{
	int half = currSize / 2;
	if (half < 1)
		return;
	//square steps
	for (int z = half; z < size; z += currSize)
		for (int x = half; x < size; x += currSize)
			squareStep(arr, size, x % size, z % size, half);
	// diamond steps
	int col = 0;
	for (int x = 0; x < size; x += half)
	{
		col++;
		//If this is an odd column.
		if (col % 2 == 1)
			for (int z = half; z < size; z += currSize)
				diamondStep(arr, size, x % size, z % size, half);
		else
			for (int z = 0; z < size; z += currSize)
				diamondStep(arr, size, x % size, z % size, half);
	}
	diamondSquare(arr, size, currSize / 2);
}

void squareStep(int arr[][DIM], int size, int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0 && z - reach >= 0)
	{
		avg += arr[x - reach][z - reach];
		count++;
	}
	if (x - reach >= 0 && z + reach < size)
	{
		avg += arr[x - reach][z + reach];
		count++;
	}
	if (x + reach < size && z - reach >= 0)
	{
		avg += arr[x + reach][z - reach];
		count++;
	}
	if (x + reach < size && z + reach < size)
	{
		avg += arr[x + reach][z + reach];
		count++;
	}
	avg += random(reach);
	avg /= count;
	arr[x][z] = avg;
}

void diamondStep(int arr[][DIM], int size, int x, int z, int reach)
{
	int count = 0;
	float avg = 0.0f;
	if (x - reach >= 0)
	{
		avg += arr[x - reach][z];
		count++;
	}
	if (x + reach < size)
	{
		avg += arr[x + reach][z];
		count++;
	}
	if (z - reach >= 0)
	{
		avg += arr[x][z - reach];
		count++;
	}
	if (z + reach < size)
	{
		avg += arr[x][z + reach];
		count++;
	}
	avg += random(reach);
	avg /= count;
	arr[x][z] = (int)avg;
}

float random(int range) {
	return (rand() % (range * 2)) - range;
}
