#include "Text_field.h"


void Text_field::draw(){
	al_draw_text(font, al_map_rgb(0, 0, 0), pos_x, pos_y, 0, text);
}