#ifndef STREET_INFO_H
#define STREET_INFO_H

class Street;

#include "Question.h" //Base class
#include "Street.h" //All information needed about streat needs to be optained from the class Street

class Street_info : public Question{
public:
	Street_info(int pos_x, int pos_y, Street *street, ALLEGRO_BITMAP *image_in, bool active = false) : Question(pos_x, pos_y, image_in, active), streat(street){
		
	}
	void draw(){
		if(active){
			char temp[1000];
			 streat->get_namn(temp, 1000);


			al_draw_bitmap(image, pos_x, pos_y, 0);
			/*al_draw_filled_rectangle(pos_x + 5, pos_y + 5, pos_x + (al_get_bitmap_width(image) - 5), pos_y + 50, al_map_rgb(255, 255, 255));
			al_draw_text(headline, al_map_rgb(255, 255, 255), pos_x + al_get_bitmap_width(image) / 2, pos_y + 35, ALLEGRO_ALIGN_CENTRE, temp);
			al_draw_text(bigger_text, al_map_rgb(255, 255, 255), pos_x + al_get_bitmap_width(image) / 2, pos_y + 15, ALLEGRO_ALIGN_CENTRE, "Title deed");

			al_draw_textf(normal_text, al_map_rgb(255, 255, 255), pos_x + 20, pos_y + 60, 0, "Rent\t - no buildings");
			al_draw_text(normal_text, al_map_rgb(255, 255, 255), pos_x + 20, pos_y + 80, 0, "    \t - with 1 house");
			al_draw_text(normal_text, al_map_rgb(255, 255, 255), pos_x + 20, pos_y + 100, 0, "    \t - with 2 houses");
			al_draw_text(normal_text, al_map_rgb(255, 255, 255), pos_x + 20, pos_y + 120, 0, "    \t - with 3 houses");
			al_draw_text(normal_text, al_map_rgb(255, 255, 255), pos_x + 20, pos_y + 140, 0, "    \t - with 4 houses");
			al_draw_text(normal_text, al_map_rgb(255, 255, 255), pos_x + 20, pos_y + 160, 0, "Rent with hotel");
			for(int i = 0; i < max_houses; i++){
				al_draw_textf(normal_text, al_map_rgb(255, 255, 255), pos_x + al_get_bitmap_width(image) - 20, pos_y + 60 + i * 20, ALLEGRO_ALIGN_RIGHT, "%i", streat->get_rent(i));
			}*/
	
		}
	}
private:
	Street *streat;

	ALLEGRO_FONT *normal_text, *bigger_text, *headline;
};

#endif