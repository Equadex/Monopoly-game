#ifndef PROPERTY_H
#define PROPERTY_H

//#include "Player.h" //Should not be here
#include "Constants.h"
#include <string>

class Player;


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


#endif