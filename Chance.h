#ifndef CHANCE_H
#define CHANCE_H

#include <time.h>
#include <cstdlib>

#include "Card.h"
#include "Question.h"
#include "Button.h"

class Chance{
public:
	Chance(Card **cards, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button) : cards(cards), picked_cards(0){
		ok_button = new Button*[1];
		ok_button[0] = new Button(Question_pos_x_standard + (al_get_bitmap_width(image) / 2) - 40, Question_pos_y_standard + al_get_bitmap_height(image) - (al_get_bitmap_height(image) /6), Question_pos_x_standard + (al_get_bitmap_width(image) / 2) + 40, Question_pos_y_standard + al_get_bitmap_height(image) - (al_get_bitmap_height(image) /6) + 25, 1, "OK", button);
		window = new Question(Question_pos_x_standard, Question_pos_y_standard, ok_button, 1, "", "", image);
		suffle_cards();
	}
	int pick_card(){ //Returns id of card
		char c[] = {"Chance"};
		char a[] = {"Community Chest"};
		if(picked_cards > (ant_cards / 2))
			suffle_cards();
		Card* temp = cards[picked_cards];
		while(temp->get_disabled())
			temp = cards[++picked_cards];


		if(temp->get_action() == 9 || temp->get_action() == 10)
			temp->set_disabled(true);

		//Chaning text in window
		if(temp->get_type())
			window->set_text(c, (temp->get_text()));
		else
			window->set_text(a, (temp->get_text()));
		window->set_active(true);

		return(temp->get_id());
	}
	void suffle_cards(){
		srand(time(NULL));
		Card *temp1;
		for(int i = 0; i < (ant_cards / 2); i++){
			int random_number = i + rand() % ((ant_cards / 2) - i);

			temp1 = cards[i];
			cards[i] = cards[random_number];
			cards[random_number] = temp1;
		}
	}
	Card** get_cards(){return cards;}

	void return_chance_card(){
		for(int i = 0; i < (ant_cards / 2); i++){
			if(cards[i]->get_action() == 9 || cards[i]->get_action() == 10){
				cards[i]->set_disabled(false);
			}
		}
	}

	Question* get_window() const{return window;}
	
private:
	int picked_cards;
	Card **cards;
	Question *window;
	Button **ok_button;
};

#endif