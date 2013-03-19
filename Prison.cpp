#include "Prison.h"

Prison::Prison(ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *question_image):n_buttons(3) {
	char temp2[300];
	temp = new Button*[n_buttons];
	button_message = new char*[n_buttons];
	button_message[0] = "Throw dices";
	sprintf(temp2, "Pay(%i)", prison_fee);
	button_message[1] = temp2;
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
		players_dice_tries[i] = 0;
}

void Prison::send_to_prison(Player* prisoner){
	prisoner->set_flag_prisoned(true);
	prisoner->set_pos_ruta(10);
}

void Prison::release_player(Player *prisoner){
	prisoner->set_flag_prisoned(false);
	reset_dice_tries(prisoner->get_id());
	activate_window(false);
	window->set_text("Prison", "You are in jail.");
}