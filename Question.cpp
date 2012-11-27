#include "Question.h"

void Question::draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text){
	al_draw_bitmap(image, pos_x, pos_y, 0);
	for(int i = 0; i < n_buttons; i++){
		buttons[i]->draw();
	}
	al_draw_text(Title, al_map_rgb(0, 0, 0), (pos_x + al_get_bitmap_width(image) / 2), (pos_y + al_get_bitmap_height(image) / 10), ALLEGRO_ALIGN_CENTRE, "");
	al_draw_text(Text, al_map_rgb(0, 0, 0), (pos_x + al_get_bitmap_width(image) / 2), (pos_y + 4 * (al_get_bitmap_height(image) / 10)), ALLEGRO_ALIGN_CENTRE, "");
}

int Question::button_pressed(int mouse_pos_x, int mouse_pos_y){
	if(active){
		int button_id;
		for(int i = 0; i < n_buttons; i++){
			button_id = buttons[i]->Button_pressed(mouse_pos_x, mouse_pos_y);
			if(button_id != 0){
				return (button_id);
			}
		}
		return (0);
	}
	return (0);
}