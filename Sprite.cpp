#include "Sprite.h"

void Sprite::draw(){
	al_draw_bitmap_region(image, pixel_intervals * current_frame, 0, pixel_intervals, al_get_bitmap_height(image), pos_x, pos_y, 0);
}