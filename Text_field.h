#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include <allegro5\allegro5.h>
#include <allegro5\allegro_font.h>

class Text_field{
public:
	Text_field(int pos_x, int pos_y, char *text, ALLEGRO_FONT *font_in) : pos_x(pos_x), pos_y(pos_y), text(text), font(font_in){}

	void draw();
private:
	char *text;
	int pos_x, pos_y;
	ALLEGRO_FONT *font;
};

#endif