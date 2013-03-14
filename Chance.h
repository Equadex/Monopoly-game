#ifndef CHANCE_H
#define CHANCE_H

#include <time.h>
#include <cstdlib>

#include "Card.h"
#include "Question.h"
#include "Button.h"

class Chance{
public:
	Chance(Card **cards, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button);
	int pick_card(); //Returns id of card
	void suffle_cards();
	Card** get_cards(){return cards;}

	void return_chance_card();

	Question* get_window() const{return window;}
	
private:
	int picked_cards;
	Card **cards;
	Question *window;
	Button **ok_button;
};

#endif