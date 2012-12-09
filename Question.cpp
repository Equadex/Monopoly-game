#include "Question.h"

Question::Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, bool active) : pos_x(pos_x), pos_y(pos_y), buttons(buttons),n_buttons(n_buttons), image(image_in), active(active)  {
	//Reserving memory
	
	title = new char*[max_question_lines_title];
	for(int i = 0; i < max_question_lines_title; i++){
		title[i] = new char[max_question_length];
	}
		message = new char*[max_question_lines];
	for(int i = 0; i < max_question_lines; i++){
		message[i] = new char[max_question_length];
	}
	//Delar upp texterna i flera texter
	bool done = false;
	for(int i = 0; i < max_question_lines_title && !done; i++){
		for(int j = 0; j < max_question_length && !done; j++){
			
			//getline_char(title_in, title[i], max_question_length, max_question_length, '\0', , j);
			if(title_in[j + i * max_question_length] == '\0'){
				done = true;
				title[i][j] = title_in[j + i * max_question_length];
				n_title = i;
			}
			else if(j == (max_question_length - 1))
				title[i][j] = '\0';
			else
				title[i][j] = title_in[j + i * (max_question_length - 1)];
		}
	}
		done = false;
		for(int i = 0; !done && i < max_question_lines; i++){
		for(int j = 0; j < max_question_length && !done; j++){
			if(message_in[j + i * max_question_length] == '\0'){
				done = true;
				message[i][j] = message_in[j + i * max_question_length];
				n_message = i;
			}
			else if(j == (max_question_length - 1))
				message[i][j] = '\0';
			else
				message[i][j] = message_in[j + i * (max_question_length - 1)];
		}
	}
}

Question::~Question(){
	//Releasing memory
	for(int i = 0; i < max_question_lines_title; i++){
		delete[] title[i];
	}
	delete[] title;
	for(int i = 0; i < max_question_lines; i++){
		delete[] message[i];
	}
	delete[] message;
}
void Question::draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text){
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

	//al_draw_text(Title, al_map_rgb(0, 0, 0), (pos_x + al_get_bitmap_width(image) / 2), (pos_y + al_get_bitmap_height(image) / 10), ALLEGRO_ALIGN_CENTRE, "");
	//al_draw_text(Text, al_map_rgb(0, 0, 0), (pos_x + al_get_bitmap_width(image) / 2), (pos_y + 4 * (al_get_bitmap_height(image) / 10)), ALLEGRO_ALIGN_CENTRE, "");
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

void Auction_window::update(char* name, int c_bid, int p_bid, int c_player){
	char temp[256];
	std::sprintf(temp, text_fields[0]->get_text_template(), name, c_bid);
	text_fields[0]->set_text(temp);
	std::sprintf(temp, text_fields[1]->get_text_template(), p_bid);
	text_fields[1]->set_text(temp);
	std::sprintf(temp, text_fields[2]->get_text_template(), c_player);
	text_fields[2]->set_text(temp);
}