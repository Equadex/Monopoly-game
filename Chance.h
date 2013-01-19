#ifndef CHANCE_H
#define CHANCE_H

#include <time.h>
#include <cstdlib>

#include "Card.h"
#include "Question.h"
#include "Button.h"

class Chance{
public:
	Chance(Card **cards, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button) : cards(cards){
		ok_button = new Button*[1];
		ok_button[0] = new Button(Question_pos_x_standard + (al_get_bitmap_width(image) / 2) - 40, Question_pos_y_standard + al_get_bitmap_height(image) - (al_get_bitmap_height(image) /6), Question_pos_x_standard + (al_get_bitmap_width(image) / 2) + 40, Question_pos_y_standard + al_get_bitmap_height(image) - (al_get_bitmap_height(image) /6) + 25, 1, "OK", button);
		window = new Question(Question_pos_x_standard, Question_pos_y_standard, ok_button, 1, "", "", image);
		srand(time(NULL));
	}
	void pick_card(){
		char c[] = {"Chance"};
		char a[] = {"Community Chest"};
		int random_number = rand() % (ant_cards / 2);
		Card* temp = cards[random_number];

		//Chaning text in window
		if(temp->get_type())
			window->set_text(c, (temp->get_text()));
		else
			window->set_text(a, (temp->get_text()));
		window->set_active(true);
	}
	Question* get_window() const{return window;}
	
private:
	Card **cards;
	Question *window;
	Button **ok_button;
};

#endif