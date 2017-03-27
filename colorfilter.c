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
#include "modules/computer_vision/colorfilter.h"
#include <stdio.h>

#include "modules/computer_vision/lib/vision/image.h"

struct video_listener *listener = NULL;
struct video_listener *listener2 = NULL;

// Filter Settings
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

// Result
int color_count = 0;
int color_count_l = 0;
int color_count_r = 0;
int color_count_g = 0;

// Function
struct image_t *colorfilter_bottom(struct image_t *img);
struct image_t *colorfilter_bottom(struct image_t *img)
{
  // Filter
  color_count_g = image_yuv422_colorfilt(img, img,
                                       color_lum_min_g, color_lum_max_g,
                                       color_cb_min_g, color_cb_max_g,
                                       color_cr_min_g, color_cr_max_g
                                      );

  return img; // Colorfilter did not make a new image
}

struct image_t *colorfilter_func(struct image_t *img);
struct image_t *colorfilter_func(struct image_t *img)
{
  // Filter
  color_count = image_yuv422_colorfilt(img, img,
                                       color_lum_min, color_lum_max,
                                       color_cb_min, color_cb_max,
                                       color_cr_min, color_cr_max
                                      );
  color_count_l = image_yuv422_colorfilt_l(img, img,
                                       color_lum_min, color_lum_max,
                                       color_cb_min, color_cb_max,
                                       color_cr_min, color_cr_max
                                      );
  color_count_r = image_yuv422_colorfilt_r(img, img,
                                       color_lum_min, color_lum_max,
                                       color_cb_min, color_cb_max,
                                       color_cr_min, color_cr_max
                                      );

  return img; // Colorfilter did not make a new image
}

void colorfilter_init(void)
{
  listener = cv_add_to_device(&COLORFILTER_CAMERA, colorfilter_func);
  listener2 = cv_add_to_device(&COLORFILTER_BOTTOM, colorfilter_bottom);
}
