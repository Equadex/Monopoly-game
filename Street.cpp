#include "Street.h"

int Street::get_zon() const{
	return (group);
}

int Street::get_cost() const{
	return (cost);
}

void Street::buy_Street(Player* buyer, bool trade, int cost_in){
	int temp_cost;
	if(cost_in != 0){
		temp_cost = cost_in;
	}
	else
		temp_cost = cost;


	if(Owner == 0 || trade){
		if(buyer->pay(temp_cost)){
			Owner = buyer;
			if(status_owner != 0){
				int temp[3];
				buyer->get_color(temp); 
				Owner = buyer;
				status_owner->set_color(temp[0], temp[1], temp[2]);
				status_owner->set_active(true);
			}
		}
	}
}

void Street::sell_Street(Player* seller){
	seller->recieve_money(cost / 2);
	Owner = 0;
	status_owner->set_active(false);
}

void Street::create_status_box(Status_box* status_box){
	status_owner = status_box;
}

void Street::draw_status(){
	status_owner->draw();
}

void Street::pay_rent(Player *guest, Property* tomter[], int dice){
	int double_pay_factor = 1;
	int util_factor = 4; //util is special for electric company and water works
	int util_factor_full = 10;
	bool util_full_factor = false;

	if(houses == 0 && this->own_zone(guest, tomter)){
		double_pay_factor = 2;
		if(pos_ruta == 12 || pos_ruta == 28)
			util_full_factor = true;
	}
	if(pos_ruta == 12 || pos_ruta == 28){ //If it is a utility
		int temp_cost;
		if(util_full_factor)
			temp_cost = dice * util_factor_full;
		else
			temp_cost = dice * util_factor;
		if(guest->pay(temp_cost))
			Owner->recieve_money(temp_cost);
	}
	else if(guest->pay(rent[houses] * double_pay_factor)){ //guest pay and return true if succesful
		Owner->recieve_money(rent[houses] * double_pay_factor);
	}
	else{
		guest->defeated(Owner, tomter);
	}
}

int Street::get_rent(int houses){
	return rent[houses];
}

bool Street::own_zone(Player *player, Property* tomter[]){
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_typ() == 0 && tomter[i]->get_Owner() != player){ //Om det är en gata och inte redan ägs utav spelaren
			if(((Street*)tomter[i])->get_zon() == this->get_zon()){ //Om denna gata och den gata den jämför med är i samma zon
				return (false);
			}
		}
	}
	return (true);
}