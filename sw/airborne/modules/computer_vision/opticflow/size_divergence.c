/*
 * Copyright (C) 2015 Guido de Croon <guido.de.croon@gmail.com>
 *
 * From:
 * Characterization of Flow Field Divergence for Vertical Landing Control of MAVs
 * by H.W. Ho and G.C.H.E. de Croon (submitted)
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
 * along with Paparazzi; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file modules/computer_vision/opticflow/size_divergence.c
 * @brief Calculate divergence from flow vectors by looking at line sizes beteween the points.
 *
 * Uses optical flow vectors as determined with a corner tracker and Lucas Kanade to estimate divergence.
 */

#include "size_divergence.h"
#include <stdlib.h>

#define NO_DIV 0.0

/**
 * Get divergence from optical flow vectors based on line sizes between corners
 * @param[in] vectors    The optical flow vectors
 * @param[in] count      The number of optical flow vectors
 * @param[in] n_samples  The number of line segments that will be taken into account. 0 means all line segments will be considered.
 * @return divergence
 */
float get_size_divergence(struct flow_t *vectors, int count, int n_samples)
{
  float min_flow;
  float flow_c;
  float distance;
  float *divs;
  unsigned int sample = 0;
  float dx;
  float dy;
  float mean_divergence;
  int n_elements;
  unsigned int i;

  dx = (float)vectors[0].flow_x;
  dy = (float)vectors[0].flow_y;
  min_flow = sqrt(dx * dx + dy * dy);

  // TODO: if count!/(2!*count-2!) lower than n_samples, do full sampling
  if (count < 2) {
    return NO_DIV;
  }
  n_elements = count;
  divs = (float *) malloc(sizeof(float) * n_elements);

  for (i = 1; i < count; i++) {
    dx = (float)vectors[i].flow_x;
    dy = (float)vectors[i].flow_y;
    if (sqrt(dx * dx + dy * dy) < min_flow) {
        min_flow = sqrt(dx * dx + dy * dy);
        sample = i;
    }
  }
  for (i = 0; i < count; i++) {
    if (i == sample) {divs[i] = 0;}
    else {
      dx = (float)vectors[i].flow_x;
      dy = (float)vectors[i].flow_y;
      flow_c = sqrt(dx * dx + dy * dy);
      dx = (float)vectors[i].pos.x - (float)vectors[sample].pos.x;
      dy = (float)vectors[i].pos.y - (float)vectors[sample].pos.y;
      distance = sqrt(dx * dx + dy * dy);
      divs[i] = flow_c / distance;
    }
  }

  mean_divergence = get_mean(divs, n_elements);

//  if (n_samples == 0) {
//    // divs will contain the individual divergence estimates:
//    n_elements = (count * count - count) / 2;
//    divs = (float *) malloc(sizeof(float) * n_elements);
//
//    // go through all possible lines:
//    sample = 0;
//    for (i = 0; i < count; i++) {
//      for (j = i + 1; j < count; j++) {
//        // distance in previous image:
//        dx = (float)vectors[i].pos.x - (float)vectors[j].pos.x;
//        dy = (float)vectors[i].pos.y - (float)vectors[j].pos.y;
//        distance_1 = sqrt(dx * dx + dy * dy);
//
//        // distance in current image:
//        dx = (float)vectors[i].pos.x + (float)vectors[i].flow_x - (float)vectors[j].pos.x - (float)vectors[j].flow_x;
//        dy = (float)vectors[i].pos.y + (float)vectors[i].flow_y - (float)vectors[j].pos.y - (float)vectors[j].flow_y;
//        distance_2 = sqrt(dx * dx + dy * dy);
//
//        // calculate divergence for this sample:
//        divs[sample] = (distance_2 - distance_1) / distance_1;
//        sample++;
//      }
//    }
//
//    // calculate the mean divergence:
//    mean_divergence = get_mean(divs, n_elements);
//
//    // free the memory of divs:
//    free(divs);
//  } else {
//    // vector that will contain individual divergence estimates:
//    divs = (float *) malloc(sizeof(float) * n_samples);
//
//    // take random samples:
//    for (sample = 0; sample < n_samples; sample++) {
//      // take two random indices:
//      i = rand() % count;
//      j = rand() % count;
//      // ensure it is not the same index:
//      while (i == j) {
//        j = rand() % count;
//      }
//
//      // distance in previous image:
//      dx = (float)vectors[i].pos.x - (float)vectors[j].pos.x;
//      dy = (float)vectors[i].pos.y - (float)vectors[j].pos.y;
//      distance_1 = sqrt(dx * dx + dy * dy);
//
//      // distance in current image:
//      dx = (float)vectors[i].pos.x + (float)vectors[i].flow_x - (float)vectors[j].pos.x - (float)vectors[j].flow_x;
//      dy = (float)vectors[i].pos.y + (float)vectors[i].flow_y - (float)vectors[j].pos.y - (float)vectors[j].flow_y;
//      distance_2 = sqrt(dx * dx + dy * dy);
//
//
//      // calculate divergence for this sample:
//      divs[sample] = (distance_2 - distance_1) / distance_1;
//    }
//
//    // calculate the mean divergence:
//    mean_divergence = get_mean(divs, n_samples);
//    // free the memory of divs:
//    free(divs);
//  }

  free(divs);
  // return the calculated divergence:
  return mean_divergence;
}

/**
 * Get the sample mean of a vector of floats
 * @param[in] numbers     Vector of numbers
 * @param[in] n_elements  Number of elements
 * @return mean
 */
float get_mean(float *x, int n)
{
  float temp;
  int i, j;
  // the following two loops sort the array x in ascending order
  for(i=0; i<n-1; i++) {
    for(j=i+1; j<n; j++) {
      if(x[j] < x[i]) {
        // swap elements
        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }
    }
  }
  if(n%2==0) {
  // if there is an even number of elements, return mean of the two elements in the middle
    return((x[n/2] + x[n/2 - 1]) / 2.0);
  }
  else {
  // else return the element in the middle
    return x[n/2];
  }
}
