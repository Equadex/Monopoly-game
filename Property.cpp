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
	seller->get_money(cost / 2);
	Owner = 0;
	status_owner->set_active(false);
}

void Street::create_status_box(Status_box* status_box){
	status_owner = status_box;
}

void Street::draw_status(){
	status_owner->draw();
}