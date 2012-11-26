#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

class Button{
public:
	Button(int pos_x1, int pos_y1, int pos_x2, int pos_y2, int ID, ALLEGRO_BITMAP *image = 0) : pos_x_1(pos_x1), pos_y_1(pos_y1), pos_x_2(pos_x2), pos_y_2(pos_y2), ID(ID), image(image) {}
	int Button_pressed(int pos_x, int pos_y) const;
	int get_ID() const;
	void draw();
private:
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int ID;
	ALLEGRO_BITMAP *image;
};

#endif