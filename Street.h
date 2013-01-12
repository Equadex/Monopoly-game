#ifndef STREET_H
#define STREET_H

class Street_info;

#include "Property.h" //Base class
#include "Status_box.h" //Needed for updating status_box with info about owner and such
#include "Player.h" //Needed to make pay and other things

class Street : public Property {
public:
	Street(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ, int cost, int building_cost, int rent[], int group, int color_r, int color_g, int color_b) : Property(pos_x_1, pos_x_2, pos_y_1, pos_y_2, pos_ruta, namn, typ), cost(cost), building_cost(building_cost), group(group), status_owner(0), houses(0), color_r(color_r), color_g(color_g), color_b(color_b) {
		//Kopierar fält
		for(int i = 0; i < max_houses + 1; i++){
			Street::rent[i] = rent[i];
		}
	}
	int get_zon() const;
	int get_cost() const;
	int get_rent(int houses);
	Street_info* get_Street_info() const{
		return (info);
	}
	int get_building_cost() const{
		return building_cost;
	}
	void get_colors(int colors[]){
		colors[0] = color_r;
		colors[1] = color_g;
		colors[2] = color_b;
	}

	void buy_Street(Player* buyer, bool trade = false, int cost_in = 0);
	void sell_Street(Player* seller);
	void create_status_box(Status_box* status_box);
	void create_street_info(Street_info* street_in_info){
		info = street_in_info;
	}
	void draw_status();
	void pay_rent(Player *guest, Property* tomter[], int dice = 0);
	bool own_zone(Property* tomter[]);
	bool button_pressed(int mouse_pos_x, int mouse_pos_y){
		if(info != 0 ){ //if pressed and not null pointer
			return (status_owner->pressed(mouse_pos_x, mouse_pos_y));
		}
	}

protected:
	int cost;
	int building_cost;
	int rent[max_houses + 1];
	int group;
	int houses;
	Status_box *status_owner;
	Street_info *info;
	int color_r, color_g, color_b;

	int n_street_in_zone(Property* tomter[]);
private:
};

#endif