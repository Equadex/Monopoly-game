#ifndef STREET_INFO_H
#define STREET_INFO_H

class Street;

#include "Question.h" //Base class
#include "Street.h" //All information needed about streat needs to be optained from the class Street

class Street_info : public Question{
public:
	Street_info(int pos_x, int pos_y, Street *street, ALLEGRO_BITMAP *image_in, ALLEGRO_BITMAP *image_railroad, ALLEGRO_BITMAP *image_el, ALLEGRO_BITMAP *image_water, ALLEGRO_FONT *normal_text, ALLEGRO_FONT *bigger_text, ALLEGRO_FONT *small_text, bool active = false) : Question(pos_x, pos_y, image_in, active), streat(street), normal_text(normal_text), bigger_text(bigger_text), small_text(small_text), image_railroad(image_railroad), image_el(image_el), image_water(image_water){
		streat->get_colors(colors); //gets the color for the street
	}
	void draw(){
		if(active){
			char temp[1000];
			 streat->get_namn(temp, 1000);
			 
			 

			 if(streat->get_zon() == 1){
				 al_draw_bitmap(image_railroad, pos_x, pos_y, 0);
				 al_draw_text(bigger_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_railroad) / 2, pos_y + 60, ALLEGRO_ALIGN_CENTRE, temp); //Draw name of the street
				 al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 110, 0, "Rent");

				 for(int i = 2; i < 5; i ++) 
					 al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 145 + (i - 2) * 35, 0, "IF %i R.R's are owned", i);
				 for(int i = 0; i < max_houses - 1; i++){ //Draws rents
					al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_railroad) - 20, pos_y + 110 + i * 35, ALLEGRO_ALIGN_RIGHT, "%i", streat->get_rent(i));
				 }

				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + al_get_bitmap_height(image_railroad) - 30, 0, "Mortage value");
				 al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_railroad) - 20, pos_y + al_get_bitmap_height(image_railroad) - 30, ALLEGRO_ALIGN_RIGHT, "%i", (streat->get_cost() / 2));
			 }
			 else if(streat->get_zon() == 5){
				 if(streat->get_pos_ruta() == 12){
					 al_draw_bitmap(image_el, pos_x, pos_y, 0);
				 }
				 else if(streat->get_pos_ruta() == 28){
					 al_draw_bitmap(image_water, pos_x, pos_y, 0);
				}
				 al_draw_line(pos_x + 25, pos_y + 90, pos_x + al_get_bitmap_width(image_el) - 25, pos_y + 90, al_map_rgb(0, 0, 0), 5);
				 al_draw_textf(bigger_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 95, ALLEGRO_ALIGN_CENTRE, temp);
				 al_draw_line(pos_x + 25, pos_y + 123, pos_x + al_get_bitmap_width(image_el) - 25, pos_y + 123, al_map_rgb(0, 0, 0), 5);

				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_el) / 2, pos_y + 140, ALLEGRO_ALIGN_CENTRE, "If one \"Utility\" is owned rent is");
				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_el) / 2, pos_y + 155, ALLEGRO_ALIGN_CENTRE, "4 times amount shown on dice");
					
				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_el) / 2, pos_y + 200, ALLEGRO_ALIGN_CENTRE, "If both \"Utilities\" are owned");
				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_el) / 2, pos_y + 215, ALLEGRO_ALIGN_CENTRE, "rent is 10 times amount");
				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_el) / 2, pos_y + 230, ALLEGRO_ALIGN_CENTRE, "shown on dice");
				 al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + al_get_bitmap_height(image_el) - 40, 0, "Mortage value");
				 al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image_el) - 20, pos_y + al_get_bitmap_height(image_el) - 40, ALLEGRO_ALIGN_RIGHT, "%i", (streat->get_cost() / 2));
			 }
			 else{
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
				for(int i = 0; i < max_houses; i++){ //Draws rents
					al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 60 + i * 20, ALLEGRO_ALIGN_RIGHT, "%i", streat->get_rent(i));
				}

				al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 190, 0, "Houses cost");
				al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 205, 0, "Hotel cost");
				al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 190, ALLEGRO_ALIGN_RIGHT, "%i each", streat->get_building_cost());
				al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 205, ALLEGRO_ALIGN_RIGHT, "%i plus %i houses", streat->get_building_cost(), max_houses-1);

				al_draw_text(normal_text, al_map_rgb(0, 0, 0), pos_x + 20, pos_y + 240, 0, "Mortage value");
				al_draw_textf(normal_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) - 20, pos_y + 240, ALLEGRO_ALIGN_RIGHT, "%i", (streat->get_cost() / 2));

				al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 270, ALLEGRO_ALIGN_CENTRE, "If a player owns all the lots of any");
				al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 285, ALLEGRO_ALIGN_CENTRE, "color-group, the rent is doubled");
				al_draw_text(small_text, al_map_rgb(0, 0, 0), pos_x + al_get_bitmap_width(image) / 2, pos_y + 300, ALLEGRO_ALIGN_CENTRE, "on unimproved lots in that group.");
			}
	
		}
	}
private:
	Street *streat;

	ALLEGRO_BITMAP *image_railroad, *image_el, *image_water;
	ALLEGRO_FONT *normal_text, *bigger_text, *small_text;
	int colors[3];
};

#endif