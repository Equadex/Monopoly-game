#ifndef STREET_INFO_H
#define STREET_INFO_H

class Street;

#include "Question.h" //Base class
#include "Street.h" //All information needed about streat needs to be optained from the class Street

class Street_info : public Question{
public:
	Street_info(int pos_x, int pos_y, Street *street, ALLEGRO_BITMAP *image_in, ALLEGRO_FONT *normal_text, ALLEGRO_FONT *bigger_text, ALLEGRO_FONT *small_text, bool active = false) : Question(pos_x, pos_y, image_in, active), streat(street), normal_text(normal_text), bigger_text(bigger_text), small_text(small_text){
		streat->get_colors(colors); //gets the color for the street
	}
	void draw(){
		if(active){
			char temp[1000];
			 streat->get_namn(temp, 1000);
			 
			 


			al_draw_bitmap(image, pos_x, pos_y, 0);
			al_draw_filled_rectangle(pos_x + 5, pos_y + 5, pos_x + (al_get_bitmap_width(image) - 5), pos_y + 50, al_map_rgb(colors[0], colors[1], colors[2])); 
			al_draw_text(bigger_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 25, ALLEGRO_ALIGN_CENTRE, temp); //Draw name of the street
			al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 10, ALLEGRO_ALIGN_CENTRE, "Title deed");

			al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 60, 0, "Rent - no buildings");
			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 80, 0, "         - with 1 house");
			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 100, 0, "         - with 2 houses");
			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 120, 0, "         - with 3 houses");
			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 140, 0, "         - with 4 houses");
			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 160, 0, "Rent with hotel");
			for(int i = 0; i < max_houses + 1; i++){ //Draws rents
				al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 60 + i * 20, ALLEGRO_ALIGN_RIGHT, "%i", streat->get_rent(i));
			}

			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 190, 0, "Houses cost");
			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 205, 0, "Hotel cost");
			al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 190, ALLEGRO_ALIGN_RIGHT, "%i each", streat->get_building_cost());
			al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 205, ALLEGRO_ALIGN_RIGHT, "%i plus %i houses", streat->get_building_cost(), max_houses-1);

			al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 240, 0, "Mortage value");
			al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 240, ALLEGRO_ALIGN_RIGHT, "%i", (streat->get_cost() / 2));

			al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 270, ALLEGRO_ALIGN_CENTRE, "If a player owns all the land in a");
			al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 285, ALLEGRO_ALIGN_CENTRE, "particular colorgroup, the rent is");
			al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 300, ALLEGRO_ALIGN_CENTRE, "double for vacant sites in the group");
	
		}
	}
private:
	Street *streat;

	ALLEGRO_FONT *normal_text, *bigger_text, *small_text;
	int colors[3];
};

#endif