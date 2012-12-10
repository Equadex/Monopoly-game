#ifndef PROPERTY_H
#define PROPERTY_H

#include "Player.h"
#include "Constants.h"
#include "Status_box.h"
#include <string>

class Player;
class Status_box;

class Property{
public:
	Property();
	Property(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ): pos_x_1(pos_x_1), pos_x_2(pos_x_2), pos_y_1(pos_y_1), pos_y_2(pos_y_2), pos_ruta(pos_ruta), namn(namn), typ(typ), Owner(NULL) {};
	void get_player_pos(int &pos_x_in, int &pos_y_in) const;
	int get_pos_ruta() const;
	int get_typ() const;
	void get_namn(char array_out[], int length){
		char temp[100];
		int i = 0;
		for(; i < 100 && i < namn.length(); i++){
			temp[i] = namn[i];
		}
		temp[i] = '\0';
		strncpy(array_out, temp, length);
	}
	

	void set_Owner(Player *Owner);
	Player* get_Owner();
protected:
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int pos_ruta;
	int typ;
	std::string namn;
	Player *Owner;
private:

};

class Street : public Property {
public:
	Street(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ, int cost, int building_cost, int rent[], int group) : Property(pos_x_1, pos_x_2, pos_y_1, pos_y_2, pos_ruta, namn, typ), cost(cost), building_cost(building_cost), group(group), status_owner(0),houses(0) {
		//Kopierar fält
		for(int i = 0; i < max_houses + 1; i++){
			Street::rent[i] = rent[i];
		}
	}
	int get_zon() const;

	void buy_Street(Player* buyer, bool trade = false, int cost_in = 0);
	void sell_Street(Player* seller);
	void create_status_box(Status_box* status_box);
	void draw_status();
	void pay_rent(Player *guest, Property* tomter[]);
	bool own_zone(Player *player, Property* tomter[]);
protected:
	int cost;
	int building_cost;
	int rent[max_houses + 1];
	int group;
	int houses;
	Status_box *status_owner;
private:
};


#endif