#ifndef STREET_H
#define STREET_H

class Street_info;

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "Property.h" //Base class
#include "Status_box.h" //Needed for updating status_box with info about owner and such
#include "Player.h" //Needed to make pay and other things

class Street : public Property {
public:
	Street(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ, int cost, int building_cost, int rent[], int group, int color_r, int color_g, int color_b) : Property(pos_x_1, pos_x_2, pos_y_1, pos_y_2, pos_ruta, namn, typ), cost(cost), building_cost(building_cost), group(group), status_owner(0), houses(0), color_r(color_r), color_g(color_g), color_b(color_b), mortaged(false) {
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
	int get_ant_houses() const{
		return houses;
	}
	void get_colors(int colors[]){
		colors[0] = color_r;
		colors[1] = color_g;
		colors[2] = color_b;
	}
	bool get_mortaged() const{ return mortaged;}

	void buy_Street(Player* buyer, bool trade = false, int cost_in = -1);
	void sell_Street(Player* seller, Property **tomter, int &tot_free_ant_houses, int &tot_free_ant_hotels);
	void buy_house(Player* buyer, Property **tomter, int &tot_free_ant_houses, int &tot_free_ant_hotels);
	void sell_house(Player* seller, Property **tomter, int &tot_free_ant_houses, int &tot_free_ant_hotels);
	void create_status_box(Status_box* status_box);
	void create_street_info(Street_info* street_in_info){
		info = street_in_info;
	}
	void mortage_street(bool mortage_in, Property **tomter, int &tot_free_ant_houses, int &tot_free_ant_hotels);
	void draw_status();
	void pay_rent(Player *guest, Property* tomter[], int dice = 0, bool full_pay_factor = false);
	bool own_zone(Property* tomter[], Player* Owner_in = 0);
	bool undeveloped_zone(Property* tomter[]);
	bool button_pressed(int mouse_pos_x, int mouse_pos_y){
		if(info != 0 ){ //if pressed and not null pointer
			return (status_owner->pressed(mouse_pos_x, mouse_pos_y));
		}
		return false;
	}
	void draw(bool mark = false);
	void update_status_box();

protected:
	const int cost;
	const int building_cost;
	int rent[max_houses + 1];
	const int group;
	int houses;
	Status_box *status_owner;
	Street_info *info;
	const int color_r, color_g, color_b;
	bool mortaged;

	int n_street_in_zone(Property* tomter[]);
	bool ok_to_build_house(Property **tomter);
	bool ok_to_sell_house(Property **tomter);
private:
};

#endif