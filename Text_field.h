#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <string>

class Text_field{
public:
	Text_field(int pos_x, int pos_y, char *text, ALLEGRO_FONT *font_in) : pos_x(pos_x), pos_y(pos_y), text_template(text), font(font_in){}

	const char* get_text_template(){
		return(text_template);
	}

	void set_text(char* text_in){
		text = text_in;
	}

	void draw(int special_output = 0){
		al_draw_text(font, al_map_rgb(0, 0, 0), pos_x, pos_y, special_output, text.c_str());
	}
private:
	const char *text_template;
	std::string text;
	int pos_x, pos_y;
	ALLEGRO_FONT *font;
};

#endif