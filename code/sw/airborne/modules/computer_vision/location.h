#include <stdint.h>

extern int Sign;
extern int H_bott;
extern void grassDetection(struct image_t *image);

#ifndef COLORFILTER_CV_PLUGIN_H
#define COLORFILTER_CV_PLUGIN_H

// Module functions
extern void colorfilter_init(void);
extern uint8_t color_lum_min;
extern uint8_t color_lum_max;

extern uint8_t color_cb_min;
extern uint8_t color_cb_max;

extern uint8_t color_cr_min;
extern uint8_t color_cr_max;
extern struct video_listener *listener;

#endif /* COLORFILTER_CV_PLUGIN_H */
