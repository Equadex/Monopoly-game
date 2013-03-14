#ifndef STREET_INFO_H
#define STREET_INFO_H

class Street;

#include "Question.h" //Base class
#include "Street.h" //All information needed about streat needs to be optained from the class Street

class Street_info : public Question{
public:
	Street_info(int pos_x, int pos_y, Street *street, ALLEGRO_BITMAP *image_in, ALLEGRO_BITMAP *image_railroad, ALLEGRO_BITMAP *image_el, ALLEGRO_BITMAP *image_water, ALLEGRO_FONT *normal_text, ALLEGRO_FONT *bigger_text, ALLEGRO_FONT *small_text, bool active = false);
	void draw();
private:
	Street *streat;

	ALLEGRO_BITMAP *image_railroad, *image_el, *image_water;
	ALLEGRO_FONT *normal_text, *bigger_text, *small_text;
	int colors[3];
};

#endif