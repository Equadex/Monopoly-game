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
	int read_chars = 0;
	int total_chars = strlen(title_in);

	for(int i = 0; i < max_question_lines_title && !done; i++){
		int word_compensator = 0;

		if(max_question_length < total_chars && total_chars != 0) //If there it's in a middle of a world(not space)
				while(title_in[0 - word_compensator + (i + 1) * (max_question_length - 1)] != ' ')//Back of until there is a space
					word_compensator++;

		for(int j = 0, temp_read_chars = 0; j < max_question_length && !done; j++){
			//getline_char(title_in, title[i], max_question_length, max_question_length, '\0', , j);

			if(max_question_length > (j + word_compensator) && (j + read_chars) < total_chars){
				title[i][j] = title_in[j + read_chars];
				temp_read_chars++;
			}
			else if((j + read_chars) >= total_chars){ //if all characters are read
				title[i][j] = '\0';
				read_chars = temp_read_chars;
				n_title = i;
				done = true;
			}
			else{ //if need a new line
				title[i][j] = '\0';
				read_chars = temp_read_chars;
				n_title = i;
				break;
			}
			
		}
	}
	done = false;
	read_chars = 0;
	total_chars = strlen(message_in);

	for(int i = 0; !done && i < max_question_lines; i++){
		int word_compensator = 0;

		if(max_question_length < (total_chars - read_chars) && total_chars != 0) //If there it's in a middle of a world(not space)
				while(message_in[0 - word_compensator + (i + 1) * (max_question_length - 1)] != ' ')//Back of until there is a space
					word_compensator++;

		for(int j = 0, temp_read_chars = 0; j < max_question_length && !done; j++){
			if(max_question_length > (j + word_compensator) && (j + read_chars) < total_chars){
				message[i][j] = message_in[j + read_chars];
				temp_read_chars++;
			}
			else if((j + read_chars) >= total_chars){ //if all characters are read
				message[i][j] = '\0';
				read_chars = temp_read_chars;
				n_message = i;
				done = true;
			}
			else{
				message[i][j] = '\0';
				read_chars = temp_read_chars;
				break;
			}
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

