#ifndef PRISON_H
#define PRISON_H

#include <allegro5\allegro.h>

#include "Player.h"
#include "Question.h"
#include "Button.h"

class Prison{
public:

	

	Prison(ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *question_image):n_buttons(3) {
		temp = new Button*[n_buttons];
		button_message = new char*[n_buttons];

		button_message[0] = "Throw dices";
		button_message[1] = "Pay";
		button_message[2] = "Use card";
		int space_between_buttons = (al_get_bitmap_width(question_image) / (n_buttons + 1));
		int button_width = al_get_bitmap_width(button_image);
		for(int i = 0; i < n_buttons; i++){
			int temp_pos_x = Question_pos_x_standard + space_between_buttons * (i + 1) - (button_width / 2);
			int temp_pos_y = Question_pos_y_standard + 250;
			temp[i] = new Button(temp_pos_x, temp_pos_y, temp_pos_x + al_get_bitmap_width(button_image), temp_pos_y + al_get_bitmap_height(button_image), i + 1, button_message[i], button_image);
		}

		window = new Question(Question_pos_x_standard, Question_pos_y_standard, temp, n_buttons, "Prison", "You are in jail.", question_image);

		players_dice_tries = new int[max_players];
		for(int i = 0; i < max_players; i++)
			players_dice_tries[0] = 0;
	}
	void send_to_prison(Player* prisoner){
		prisoner->set_flag_prisoned(true);
		prisoner->set_pos_ruta(10);
	}
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
		window->set_text("Can't to that!", "You can only throw the dice 3 times to get out of jail. Now you need to use a card or pay");
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
	void release_player(Player *prisoner){
		prisoner->set_flag_prisoned(false);
		reset_dice_tries(prisoner->get_id());
		activate_window(false);
	}


private:
	Question* window;
	int* players_dice_tries;
	const int n_buttons;
	Button** temp;
	char** button_message;
};

#endif