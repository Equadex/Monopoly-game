#ifndef PRISON_H
#define PRISON_H

#include <cstdio>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Player.h"
#include "Question.h"
#include "Button.h"

class Prison{
public:

	

	Prison(ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *question_image, ALLEGRO_SAMPLE_INSTANCE* police_instance);
	void send_to_prison(Player* prisoner);
	void activate_window(bool value){
		window->set_active(value);
	}
	bool window_activated()const {return window->get_active();}
	int button_pressed(int mouse_pos_x, int mouse_pos_y){
		return (window->button_pressed(mouse_pos_x, mouse_pos_y));
	}
	int get_players_dice_tries(int player_id){
		return players_dice_tries[player_id];
	}
	void out_of_dice_tries(){
		window->set_text("Can't do that!", "You can only throw the dice 3 times to get out of jail. Now you need to use a card or pay");
	}
	void reset_dice_tries(int player_id){
		players_dice_tries[player_id] = 0;
	}
	void register_dice_try(int player_id, int number_of_tries = 1){
		players_dice_tries[player_id] += number_of_tries;
	}
	void draw_window(ALLEGRO_FONT *title, ALLEGRO_FONT *text){
		window->draw(title, text);
	}
	void release_player(Player *prisoner);


private:
	Question* window;
	int* players_dice_tries;
	const int n_buttons;
	Button** temp;
	char** button_message;
	ALLEGRO_SAMPLE_INSTANCE* police_instance;
};

#endif