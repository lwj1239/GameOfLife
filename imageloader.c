/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE* fp = fopen(filename, "r");

	if (!fp) {
		printf("文件打开失败!!!\n");
		return NULL;
	}

	Image* image = (Image*)malloc(sizeof(Image));
	char s[20];
	uint32_t max_val;

	fscanf(fp, "%s", s);
	fscanf(fp, "%u %u", &image->cols, &image->rows);
	fscanf(fp, "%u", &max_val);

	int n = image->rows * image->cols;
	image->image = (Color**) malloc(sizeof(Color*) * n);
 
	for (int i = 0; i < n; i ++) {
		image->image[i] = (Color*) malloc(sizeof(Color));
		unsigned int r, g, b;
		fscanf(fp, "%u %u %u", &r, &g, &b);
		image->image[i]->R = (uint8_t)r;
		image->image[i]->G = (uint8_t)g;
		image->image[i]->B = (uint8_t)b;
	}

	fclose(fp);

	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n%d %d\n255\n", image->cols, image->rows);
	for (int i = 0; i < image->rows; i ++) {
		for (int j = 0; j < image->cols; j ++) {
			Color c = *(image->image[i * image->cols + j]);
			printf("%3u %3u %3u", (uint8_t)c.R, (uint8_t)c.G, (uint8_t)c.B);
			if (j != image->cols - 1) {
				printf("   ");
			}
		}
		printf("\n");
	}
}

void freeImage(Image *image) {
    if (!image) return;
    for (int i = 0; i < image->cols * image->rows; i++) {
        free(image->image[i]);  // 释放每个Color
    }
    free(image->image);  // 释放指针数组
    free(image);         // 释放Image结构体
}