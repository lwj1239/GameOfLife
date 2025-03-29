/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	int idx = row * col;
	Color* ans = (Color*) malloc(sizeof(Color));
	Color* cur = image->image[idx];

	if (cur->B & 1) {
		ans->R = ans->G = ans->B = 255;
	}else {
		ans->B = ans->G = ans->R = 255;
	}
	
	return ans;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	int cols = image->cols;
	int rows = image->rows;
	Image* img = (Image*) malloc(sizeof(Image));

	img->image = (Color**) malloc(sizeof (Color*) * cols * rows);
	img->cols = cols;
	img->rows = rows;

	for (int i = 0; i < rows; i ++) {
		for (int j = 0; j < cols; j ++) {
			img->image[i * cols + j] = evaluateOnePixel(image, i, j);
		}
	}

	return img;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	char* filename = argv[1];
	if (!filename) {
		printf("文件名为空");
		return -1;
	}

	Image* img = readData(filename);

	if (!img) {
		printf("图片创建失败");
		return -1;
	}

	Image* secret_img = steganography(img);
	writeData(secret_img);
	freeImage(img);
	freeImage(secret_img);
}