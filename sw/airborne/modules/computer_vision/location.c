/*
 * Copyright (C) 2015
 *
 * This file is part of Paparazzi.
 *
 * Paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * Paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file modules/computer_vision/colorfilter.c
 */

// Own header
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modules/computer_vision/lib/vision/image.h"
#include "modules/computer_vision/cv.h"

struct video_listener *listener = NULL;
int Sign = 0;
int H_bott = 0;

uint8_t color_lum_min = 20;
uint8_t color_lum_max = 255;
uint8_t color_cb_min  = 75;
uint8_t color_cb_max  = 145;
uint8_t color_cr_min  = 167;
uint8_t color_cr_max  = 255;

uint8_t color_lum_min_g = 20;
uint8_t color_lum_max_g = 255;
uint8_t color_cb_min_g  = 0;
uint8_t color_cb_max_g  = 128;
uint8_t color_cr_min_g  = 0;
uint8_t color_cr_max_g  = 128;


void grassDetection(struct image_t *img);
void grassDetection(struct image_t *img)
{
	int H = img->w/2;
	int W = img->h;
	uint8_t *dest = img->buf;
	int isgrass[H][W];
	int i=0,j=0;
	for (uint16_t y = 0; y < W; y ++) {
		for (uint16_t x = 0; x < H; x++) {
	      // Check if the color is inside the specified values
	      		if ( (dest[1] >= 12)&&(dest[1] <= 176)
			&& (dest[0] <= 112)
			&& (dest[2] <= 160))
	      			{
                		isgrass[x][y]=1;
                		dest[0] = 64;        // U
                		dest[2] = 255;        // V
              			}
	      		else
                		{isgrass[x][y]=0;}
	      		dest += 4;
	    	}
	}
	/*/
	int i=0,j=0;
	int W = 1000;
	int H = 1000;
	int isgrass[H][W];
	for (uint16_t y = 0; y < H; y++) {
	    for (uint16_t x = 0; x < W; x++) {
	    // Check if the color is inside the specified values
		if (y<H/2) {
		isgrass[y][x]=0;}
	else {isgrass[y][x]=1;}
	}
	}
/*/
	int m,n;
	int S = 1;
	int WSum = 0;
	for(i = 0; i < H/8; i += 2){
		S = 1;
		WSum = 0;
	  	for(m = W/8-1; m < (W*7/8-1); m++){
	      		S += 1 - isgrass[i][m];
//	      WSum += m*(1 - isgrass[i][m]);
	    	}
	    	Sign = 0;
	    	H_bott = i;
	    //if( (S > W*11/20) && (CoW - W/2) < W/5){
		if(S > W/8){
			WSum = 0;
			S = 1;
			Sign = -1;
			for(m = W/8-1; m < (W*7/8-1); m++){
		  		for(n = i; n < (i+H/5) ; n++){
		    			WSum += m*(1 - isgrass[n][m]);
		    			S += 1 - isgrass[n][m];
		  		}
		  	}
			WSum = WSum/S;
			if(WSum < W/2){
		  		Sign = 1;
			}
		break;
	      	}
	}
	//    return img;
printf("%d\n",Sign);
}

struct image_t *grassDet(struct image_t *img);
struct image_t *grassDet(struct image_t *img)
{
  grassDetection(img);

  return img;
}

void colorfilter_init(void)
{
//  listener = cv_add_to_device(&COLORFILTER_CAMERA, colorfilter_func);
//  listener2 = cv_add_to_device(&COLORFILTER_BOTTOM, colorfilter_bottom);
  listener = cv_add_to_device(&COLORFILTER_CAMERA, grassDet);
}
