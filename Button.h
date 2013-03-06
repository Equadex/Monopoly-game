#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>

class Button{
public:
	Button(){}
	Button(int pos_x1, int pos_y1, int pos_x2, int pos_y2, int ID, const char* label_in = "", ALLEGRO_BITMAP *image = 0) : pos_x_1(pos_x1), pos_y_1(pos_y1), pos_x_2(pos_x2), pos_y_2(pos_y2), ID(ID), image(image) {
		const int max_label_length = 128;
		label = new char[max_label_length];

		strncpy(label, label_in, max_label_length - 1);
		label[max_label_length - 1] = '\0';
	}
	int Button_pressed(int pos_x, int pos_y) const;
	int get_ID() const;
	void draw(ALLEGRO_FONT *font = 0);
	int get_pos_x_1(){return pos_x_1;}
	int get_pos_y_1(){return pos_y_1;}
private:
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int ID;
	char *label;
	ALLEGRO_BITMAP *image;
};

#endif