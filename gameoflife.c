/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int get(char ch)  {
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	else {
		if (ch >= 'a' && ch <= 'f') {
			return ch - 'a' + 10;
		}
		else {
			return ch - 'A' + 10;
		}
	}
}

uint32_t convert(char* rules) {
	uint32_t ans = 0;

	for (int i = 2; rules[i]; i ++) {
		ans = ans * 16 + get(rules[i]);
	}

	return ans;
}

void setBit(uint8_t* a, int i, int val) {
	if (val) {
		*a |= 1 << i;
	}
	else {
		*a &= ~(1 << i);
	}
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color* ans = (Color*) malloc(sizeof (Color));
	Color* cur = image->image[row * image->cols + col];
	int d[8][2] = {
		{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1}, {0, 1},
		{1, -1}, {1, 0}, {1, 1}
	};

	//	R
	for (int i = 0; i < sizeof(ans->R) * 8; i ++) {
		int is_live = (cur->R >> i) & 1;
		int cnt = 0;
		for (int j = 0; j < 8; j ++) {
			int nrow = (d[j][0] + row + image->rows) % image->rows;
			int ncol = (d[j][1] + col + image->cols) % image->cols;
			int idx = nrow * image->cols + ncol;
			
			if ((image->image[idx]->R >> i) & 1) {
				cnt ++;
			}
		}

		if ((rule >> (is_live * 9 + cnt)) & 1) setBit(&ans->R, i, 1);
		else setBit(&ans->R, i, 0);
	}

	// G
	for (int i = 0; i < sizeof(ans->G) * 8; i ++) {
		int is_live = (cur->G >> i) & 1;
		int cnt = 0;
		for (int j = 0; j < 8; j ++) {
			int nrow = (d[j][0] + row + image->rows) % image->rows;
			int ncol = (d[j][1] + col + image->cols) % image->cols;
			int idx = nrow * image->cols + ncol;
			
			if ((image->image[idx]->G >> i) & 1) {
				cnt ++;
			}
		}

		if ((rule >> (is_live * 9 + cnt)) & 1) setBit(&ans->G, i, 1);
		else setBit(&ans->G, i, 0);
	}

	// B
	for (int i = 0; i < sizeof(ans->B) * 8; i ++) {
		int is_live = (cur->B >> i) & 1;
		int cnt = 0;
		for (int j = 0; j < 8; j ++) {
			int nrow = (d[j][0] + row + image->rows) % image->rows;
			int ncol = (d[j][1] + col + image->cols) % image->cols;
			int idx = nrow * image->cols + ncol;
			
			if ((image->image[idx]->B >> i) & 1) {
				cnt ++;
			}
		}

		if ((rule >> (is_live * 9 + cnt)) & 1) setBit(&ans->B, i, 1);
		else setBit(&ans->B, i, 0);
	}

	return ans;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	if (!image) {
		return NULL;
	}

	//YOUR CODE HERE
	int cols = image->cols;
	int rows = image->rows;
	Image* img = (Image*) malloc(sizeof(Image));

	img->image = (Color**) malloc(sizeof (Color*) * cols * rows);
	img->cols = cols;
	img->rows = rows;

	for (int i = 0; i < rows; i ++) {
		for (int j = 0; j < cols; j ++) {
			img->image[i * cols + j] = evaluateOneCell(image, i, j, rule);
		}
	}

	return img;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc < 3) {
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}

	char* filename = argv[1];
	char* rules = argv[2];

	if (!filename || rules[0] != '0' || rules[1] != 'x') {
		printf("文件名为空!!!");
		return -1;
	}

	uint32_t rule = convert(rules);

	Image* img = readData(filename);

	if (!img) {
		printf("文件读取失败!!!\n");
		return -1;
	}
	
	Image* next_life = life(img, rule);
	writeData(next_life);
	freeImage(next_life);
	freeImage(img);
}
