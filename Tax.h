#include "Property.h"
#include "Player.h"

#ifndef TAX_H
#define TAX_H

class Tax : public Property{
public:
	Tax(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ, int fee) : Property(pos_x_1, pos_x_2, pos_y_1, pos_y_2, pos_ruta, namn, typ), fee(fee){
		//constructor
	}
	void pay(Player* visitor, Property** tomter){
		if(!visitor->pay(fee)) //Player pays, if player cannot, player is defeated
			visitor->defeated(NULL, tomter);
	}
private:
	int fee;
};

#endif