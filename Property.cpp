#include "Property.h"

void Property::get_player_pos(int &pos_x_in, int &pos_y_in) const{
	pos_x_in = pos_x_1 + (pos_x_2 - pos_x_1) / 2;
	pos_y_in = pos_y_1 + (pos_y_2 - pos_y_1) / 2;
}

int Property::get_pos_ruta() const{
	return (pos_ruta);
}

void Property::set_Owner(Player *Owner){
	Property::Owner = Owner;
}

Player* Property::get_Owner(){
	return (Owner);
}

int Property::get_typ() const{
	return (typ);
}

int Street::get_zon() const{
	return (group);
}

void Street::buy_Street(Player* buyer, bool trade){
	if(Owner == 0 || trade){
		if(buyer->pay(cost)){
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

void Street::pay_rent(Player *guest, Property* tomter[]){
	int double_pay_factor = 1;
	if(houses == 0 && this->own_zone(guest, tomter)){
		double_pay_factor = 2;
	}
	if(guest->pay(rent[houses] * double_pay_factor)){
		Owner->recieve_money(rent[houses] * double_pay_factor);
	}
	else{
		guest->defeated(Owner, tomter);
	}
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