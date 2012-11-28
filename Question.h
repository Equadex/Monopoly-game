#ifndef QUESTION_H
#define QUESTION_H


#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>

#include "Button.h"

const int max_question_length = 90;
const int max_question_lines = 10;
const int max_question_lines_title = 3;

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, bool active = false) : pos_x(pos_x), pos_y(pos_y), buttons(buttons),n_buttons(n_buttons), image(image_in), active(active)  {
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
				if(title_in[j + i * max_question_length] == '/0'){
					done = true;
					title[i][j] = title_in[j + i * max_question_length];
					n_title = i;
				}
				title[i][j] = title_in[j + i * max_question_length];
			}
		}

		done = false;

		for(int i = 0; !done && i < max_question_lines; i++){
			for(int j = 0; j < max_question_length && !done; j++){
				if(message_in[j + i * max_question_length] == '/0'){
					done = true;
					message[i][j] = message_in[j + i * max_question_length];
					n_message = i;
				}
				message[i][j] = message_in[j + i * max_question_length];
			}
		}
	}
	~Question(){
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
	char **title;
	int n_title, n_message;
	char **message;
};

#endif