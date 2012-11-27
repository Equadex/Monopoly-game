#ifndef QUESTION_H
#define QUESTION_H

#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>

#include "Button.h"

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, std::string title_in, std::string message_in, ALLEGRO_BITMAP *image_in, bool active = false) : pos_x(pos_x), pos_y(pos_y), buttons(buttons),n_buttons(n_buttons), title(0), message(0), image(image_in), active(active) {
		const int max_question_length = 90;
		const int max_question_lines = 10;
		bool done = false;
		n_title = 0; n_message = 0;

		for(int j = 0; j < 3; j++){
			for(int i = 0; i < max_question_length && !done; i++){
				if(title_in[i] == '\0'){
					n_title += i;
					done = true;
				}
				temp_title = title_in[i];
			}
		}
		title = temp_title;
		done = false;

		for(int j = 0; j < 3; j++){
			for(int i = 0; i < max_question_length && !done; i++){
				if(message_in[i] == '\0'){
					n_title += i;
					done = true;
				}
				temp_message[j][i] = message_in[i];
			}
		}
		message = temp_message;
		
	}
	void set_active(bool active_in){active = active_in;}
	bool get_active() const{return active;}
	
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);
	int button_pressed(int mouse_pos_x, int mouse_pos_y);
private:
	bool active;
	ALLEGRO_BITMAP *image;
	int pos_x;
	int pos_y;
	int n_buttons;
	Button **buttons;
	std::string *title;
	int n_title, n_message;
	std::string *message;
};

#endif