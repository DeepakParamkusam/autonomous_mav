/**
 * @file modules/computer_vision/location.c
 */

// Own header
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modules/computer_vision/lib/vision/image.h"
#include "modules/computer_vision/cv.h"

struct video_listener *listener = NULL;
int Sign = 0;		// dummy control command for the drone
int H_bott = 0;		// output lowest boundary of obstacle for testing

uint8_t color_lum_min = 20;
uint8_t color_lum_max = 255;
uint8_t color_cb_min  = 75;
uint8_t color_cb_max  = 145;
uint8_t color_cr_min  = 167;
uint8_t color_cr_max  = 255;

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
	      // Check if the color of the pixel can be considered green
	      		if ( (dest[1] >= 12)&&(dest[1] <= 176)
			&& (dest[0] <= 112)
			&& (dest[2] <= 160))
	      			{
                		isgrass[x][y]=1;	// 1 for yes, 0 for no
                		dest[0] = 64;           // change the color of the pixel to red for visualization
                		dest[2] = 255;
              			}
	      		else
                		{isgrass[x][y]=0;}
	      		dest += 4;  //go to next pixel
	    	}
	}

	int m,n;
	int S = 1;
	int WSum = 0;
	// calculate by line the number of non-green pixels and their center of mass
	for(i = 0; i < H/8; i += 2){
		S = 1;		// initialize when moving to a new line
		// take into account only the section in the middle
	  	for(m = W/8-1; m < (W*7/8-1); m++){
	      		S += 1 - isgrass[i][m];
	    	}
	    	Sign = 0;	// reset to forward-moving command
	    	H_bott = i;
		// thresholding the number of non-green pixels
		if(S > W/8){
			WSum = 0;
			S = 1;
			// default turning command is CCW
			Sign = -1;
			// calculate the center-of-mass of non-green pixels			
			for(m = W/8-1; m < (W*7/8-1); m++){
		  		for(n = i; n < (i+H/5) ; n++){
		    			WSum += m*(1 - isgrass[n][m]);
		    			S += 1 - isgrass[n][m];
		  		}
		  	}
			WSum = WSum/S;
			// turn CW if the center-of-mass is on the left
			if(WSum < W/2){
		  		Sign = 1;
			}
		break;		// lowest boundary found, break the loop
	      	}
	}
    printf("%d\n",Sign);
}

// Callback function when new image arrives.
struct image_t *grassDet(struct image_t *img);
struct image_t *grassDet(struct image_t *img)
{
  grassDetection(img);
  return img;
}

void colorfilter_init(void)
{
  listener = cv_add_to_device(&COLORFILTER_CAMERA, grassDet);
}
