#include "Status_box.h"


void Status_box::set_color(int color_r, int color_g, int color_b){
	Status_box::color_r = color_r; Status_box::color_g = color_g; Status_box::color_b = color_b;
}

void Status_box::set_active(bool active){
	Status_box::active = active;
}

void Status_box::draw(){
	if(active)
		al_draw_filled_rectangle(pos_x_1 + pos_x_displacement, pos_y_1 + pos_y_displacement, pos_x_2 + pos_x_displacement, pos_y_2 + pos_y_displacement, al_map_rgb(color_r, color_g, color_b));
}