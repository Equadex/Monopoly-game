#include "Button.h"

int Button::Button_pressed(int pos_x, int pos_y) const{
 	if(pos_x_1 <= pos_x && pos_x_2 >= pos_x && pos_y_1 <= pos_y && pos_y_2 >= pos_y){
		return (ID);
	}
	else
		return (0);
}

int Button::get_ID() const{
	return (ID);
}

void Button::draw(ALLEGRO_FONT *font){
	if(image != 0)
		al_draw_bitmap(image, pos_x_1, pos_y_1, 0);

	al_draw_text(font, al_map_rgb(0, 0, 0), pos_x_1 + (pos_x_2 - pos_x_1) / 2, pos_y_1 + (pos_y_2 - pos_y_1) * (1 / (float)3) , ALLEGRO_ALIGN_CENTRE, label);

	if(draw_own_edge){
		al_draw_line(pos_x_1, pos_y_1, pos_x_1, pos_y_2, al_map_rgb(255, 255, 255), 5);
		al_draw_line(pos_x_2, pos_y_1, pos_x_2, pos_y_2, al_map_rgb(255, 255, 255), 5);
		al_draw_line(pos_x_1, pos_y_1, pos_x_2, pos_y_1, al_map_rgb(255, 255, 255), 5);
		al_draw_line(pos_x_1, pos_y_2, pos_x_2, pos_y_2, al_map_rgb(255, 255, 255), 5);
	}

	//Made for debugging. Prints bounderies of the button
	al_draw_line(pos_x_1, pos_y_1, pos_x_1, pos_y_2, al_map_rgb(255, 0, 255), 5);
	al_draw_line(pos_x_2, pos_y_1, pos_x_2, pos_y_2, al_map_rgb(255, 0, 255), 5);
	al_draw_line(pos_x_1, pos_y_1, pos_x_2, pos_y_1, al_map_rgb(255, 0, 255), 5);
	al_draw_line(pos_x_1, pos_y_2, pos_x_2, pos_y_2, al_map_rgb(255, 0, 255), 5);
}