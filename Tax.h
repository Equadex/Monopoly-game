#include "Property.h"
#include "Street.h"
#include "Player.h"
#include "Question.h"
#include "Button.h"

#ifndef TAX_H
#define TAX_H

class Tax : public Property{
public:
	Tax(int pos_x_1, int pos_x_2, int pos_y_1, int pos_y_2, int pos_ruta, std::string namn, int typ, int fee) : Property(pos_x_1, pos_x_2, pos_y_1, pos_y_2, pos_ruta, namn, typ), fee(fee), choose(0) {
		//constructor
		if(pos_ruta == 4)
			temp_buttons = new Button*[2];
	}
	void pay(Player* visitor, Property** tomter){
		if(!visitor->pay(fee)) //Player pays, if player cannot, player is defeated
			visitor->defeated(NULL, tomter);
	}
	void create_income_tax_question(int pos_x_in, int pos_y_in, ALLEGRO_BITMAP* button, ALLEGRO_BITMAP* question){
		if(pos_ruta == 4){
		
		temp_buttons[0] = new Button(pos_x_in + 162, pos_y_in + 250, 165 + 162 + 80, 275 + 250 + 25, 1, "Fee", button);
		temp_buttons[1] = new Button(pos_x_in + 435 - 162 + 80, pos_y_in + 250, 600, 275 + 250 + 25, 2, "10 %", button);

		choose = new Question(pos_x_in, pos_y_in, temp_buttons, 2, "Pay fee or 10 % ?", "You have to pay income tax. Do you want to pay a fee of 200 or 10 % of your assets (money + property + houses + hotels)?", question);
		}
	}
	Question* get_question() const{
		return choose;
	}
	void set_question_active(bool active){
		choose->set_active(active);
	}
	void pay_fee(Player* visitor, Property** tomter){
		int total_funds = 0, total_fee;
		total_funds += (visitor->get_money());

		for(int i = 0; i < ant_rutor; i++){ //Adding owned property
			if(tomter[i]->get_Owner() == visitor && tomter[i]->get_typ() == TOMT){
				if(((Street*)tomter[i])->get_mortaged()) //if mortaged, add mortaged value
					total_funds += ((((Street*)tomter[i])->get_cost()) / 2);
				else{
					total_funds += (((Street*)tomter[i])->get_cost());
					if(((Street*)tomter[i])->own_zone(tomter)){ //if zone is owned, check for houses and hotels
						total_funds += ((Street*)tomter[i])->get_ant_houses() * ((Street*)tomter[i])->get_building_cost();
					}
				}
			}
		}
		total_fee = total_funds / 10;
		if(!visitor->pay(total_fee)) //Player pays, if player cannot, player is defeated
			visitor->defeated(NULL, tomter);
		
	}
private:
	int fee;
	Question* choose;
	Button** temp_buttons;
};

#endif