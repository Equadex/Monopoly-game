#include "Auction_window.h"

void Auction_window::draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text){
	al_draw_bitmap(image, pos_x, pos_y, 0);
	for(int i = 0; i < n_buttons; i++){
		buttons[i]->draw(Text);
	}

	for(int i = 0; i < max_question_lines_title && n_title >= i; i++){
		al_draw_text(Title, al_map_rgb(0, 0, 0), (pos_x + al_get_bitmap_width(image) / 2), (30 * i + pos_y + al_get_bitmap_height(image) / 10), ALLEGRO_ALIGN_CENTRE, title[i]);
	}

	for(int i = 0; i < max_question_lines && n_message >= i; i++){
		al_draw_text(Text, al_map_rgb(0, 0, 0), (pos_x + al_get_bitmap_width(image) / 2), (15 * i + pos_y + 4 * (al_get_bitmap_height(image) / 10)), ALLEGRO_ALIGN_CENTRE, message[i]);
	}

	for(int i = 0; i < ant_text_fields; i++){
		text_fields[i]->draw();
	}
}

void Auction_window::update(char* name, int c_bid, int p_bid, int c_player, int c_player_funds){
	char temp[256];
	sprintf(temp, text_fields[0]->get_text_template(), name, c_bid);
	text_fields[0]->set_text(temp);
	sprintf(temp, text_fields[1]->get_text_template(), p_bid);
	text_fields[1]->set_text(temp);
	sprintf(temp, text_fields[2]->get_text_template(), c_player);
	text_fields[2]->set_text(temp);
	sprintf(temp, text_fields[3]->get_text_template(), c_player_funds);
	text_fields[3]->set_text(temp);
}