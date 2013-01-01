#ifndef QUESTION_H
#define QUESTION_H


#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>

#include "Button.h"
#include "Text_field.h"
#include "Player.h"

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
	Window_list(int pos_x, int pos_y, int player_bids[],Player** players, int n_players, ALLEGRO_BITMAP *image_in = 0, bool active = false) : Question(pos_x, pos_y, image_in, active), player_bids(player_bids), n_players(n_players), max_question_length(90), max_question_lines(n_players), max_question_lines_title(n_players), players(players) {
		//Default colors for status for a players bid 
		
		status_color = new int*[n_players];
		for(int i = 0; i < n_players; i++){
			status_color[i] = new int[3];
			status_color[i][0] = 255;
			status_color[i][1] = 0;
			status_color[i][2] = 0;
		}

		//Color for player in list
		player_color = new int*[n_players];

		for(int i = 0; i < n_players; i++){
			player_color[i] = new int[3];
			(players[i])->get_color(player_color[i]);
		}

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
		for(int i = 0; i < n_players; i++){
			delete[] status_color[i];
			delete[] player_color[i];
		};
		delete[] status_color;
		delete[] player_color;


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
		char temp[9];
		for(int i = 0; i < n_players; i++){
			sprintf(temp, "%d", player_bids[i]);
			al_draw_filled_rectangle(pos_x - 20, pos_y + 25 * i, pos_x - 5, pos_y + 25 * i + 15, al_map_rgb(status_color[i][0], status_color[i][1], status_color[i][2]));
			al_draw_text(Text, al_map_rgb(player_color[i][0], player_color[i][1], player_color[i][2]), pos_x, pos_y + i * 25, 0, title[i]);
			al_draw_text(Text, al_map_rgb(player_color[i][0], player_color[i][1], player_color[i][2]), pos_x + 130, pos_y + i * 25, 0, temp);
		}
	}
	void update_status(bool bid, Player *player){
		if(bid){
			status_color[player->get_id()][0] = 0;
			status_color[player->get_id()][1] = 255;
			status_color[player->get_id()][2] = 0;
		}
		else{
			status_color[player->get_id()][0] = 255;
			status_color[player->get_id()][1] = 0;
			status_color[player->get_id()][2] = 0;
		}
	}

private:
	int **status_color;
	int **player_color;
	int *player_bids;
	Player** players;
	int n_players;

	const int max_question_length;
	const int max_question_lines;
	const int max_question_lines_title;
};

#endif