#include "Property.h"
#include "Street.h"
#include "Player.h"
#include "Question.h"
#include "Button.h"

#ifndef TAX_H
#define TAX_H

class Tax : public Property{
public:
	Tax(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ, int fee);
	void pay(Player* visitor){
		if(!visitor->pay(fee)) //Player pays, if player cannot, player is defeated
			visitor->defeated(NULL, fee);
	}
	void create_income_tax_question(int pos_x_in, int pos_y_in, ALLEGRO_BITMAP* button, ALLEGRO_BITMAP* question);
	Question* get_question() const{
		return choose;
	}
	void set_question_active(bool active){
		choose->set_active(active);
	}
	void pay_fee(Player* visitor, Property** tomter);
private:
	int fee;
	Question* choose;
	Button** temp_buttons;
};

#endif