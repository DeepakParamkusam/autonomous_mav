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
int Sign = 0;
int H_bott = 0;

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
	      // Check if the color is inside the specified values
	      		if ( (dest[1] >= 12)&&(dest[1] <= 176)
			&& (dest[0] <= 112)
			&& (dest[2] <= 160))
	      			{
                		isgrass[x][y]=1;
                		dest[0] = 64;        // U
                		dest[2] = 255;       // V
              			}
	      		else
                		{isgrass[x][y]=0;}
	      		dest += 4;
	    	}
	}

	int m,n;
	int S = 1;
	int WSum = 0;
	for(i = 0; i < H/8; i += 2){
		S = 1;
		WSum = 0;
	  	for(m = W/8-1; m < (W*7/8-1); m++){
	      		S += 1 - isgrass[i][m];
	    	}
	    	Sign = 0;
	    	H_bott = i;
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
