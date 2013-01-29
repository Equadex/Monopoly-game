#ifndef PROPERTY_H
#define PROPERTY_H

#include "Constants.h"
#include "Button.h"
#include <string>


class Player;


class Property{
public:
	Property();
	Property(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ): pos_x_1(pos_x_1), pos_x_2(pos_x_2), pos_y_1(pos_y_1), pos_y_2(pos_y_2), pos_ruta(pos_ruta), namn(namn), typ(typ), Owner(NULL) {
		button = new Button(pos_x_1, pos_y_1, pos_x_2, pos_y_2, button_id_property_start + pos_ruta);
	};
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
	bool property_button_pressed(int mouse_pos_x, int mouse_pos_y);
	

	void set_Owner(Player *Owner);
	Player* get_Owner();
protected:
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int pos_ruta;
	int typ;
	std::string namn;
	Player *Owner;
	Button *button;
private:

};


#endif