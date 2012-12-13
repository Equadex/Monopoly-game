#ifndef QUESTION_H
#define QUESTION_H


#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>

#include "Button.h"
#include "Text_field.h"

const int max_question_length = 90;
const int max_question_lines = 10;
const int max_question_lines_title = 3;

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, bool active = false);
	Question(int pos_x, int pos_y, ALLEGRO_BITMAP *image_in, bool active = false): pos_x(pos_x), pos_y(pos_y), image(image_in), active(active) {}
	~Question();
	void set_active(bool active_in){active = active_in;}
	bool get_active() const{return active;}
	
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);
	int button_pressed(int mouse_pos_x, int mouse_pos_y);

protected:
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

class Auction_window : public Question{
public:
	Auction_window(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, Text_field** text_fields_in, int ant_text_fields_in, bool active = false) : Question(pos_x, pos_y, buttons, n_buttons, title_in, message_in, image_in, active), text_fields(text_fields_in), ant_text_fields(ant_text_fields_in){}
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);

	void update(char* name, int c_bid, int p_bid, int c_player);
protected:
	Text_field **text_fields;
	int ant_text_fields;
private:
};

class Window_list : public Question{
public:
	Window_list(int pos_x, int pos_y, char* title_in, char* message_in, int color_r, int color_g, int color_b, int player_bids[], int n_players, ALLEGRO_BITMAP *image_in = 0, bool active = false) : Question(pos_x, pos_y, image_in, active), player_bids(player_bids), n_players(n_players), max_question_length(90), max_question_lines(n_players), max_question_lines_title(n_players) {
		color = new int[3];
		color[0] = color_r;
		color[1] = color_g;
		color[2] = color_b;

		//Reserving memory
		
		title = new char*[max_question_lines_title];
		for(int i = 0; i < max_question_lines_title; i++){
			title[i] = new char[max_question_length];
		}
		message = new char*[max_question_lines];
		for(int i = 0; i < max_question_lines; i++){
			message[i] = new char[max_question_length];
		}

		//Creates text to list

		for(int i = 0; i < max_question_lines; i++){
			char temp[] = "Player ";
			int temp_length = strlen(temp);
			strncpy(title[i], temp, max_question_length - 1);
			if(temp_length >= max_question_length)
				title[i][max_question_length-1] = '\0';
			else
				title[i][temp_length] = 48 + i;
			message[i][0] = '\0';
		}
	}
	~Window_list(){
		delete[] color;

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
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text){

	}
	void update(){

	}

private:
	int *color;
	int *player_bids;
	int n_players;

	const int max_question_length;
	const int max_question_lines;
	const int max_question_lines_title;
};

#endif