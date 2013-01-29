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

	if(houses == 0 && own_zone(tomter) && group != 1){//grupp 1 = järnväg
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
		if(guest->pay(temp_cost))//Pay, else defeat
			Owner->recieve_money(temp_cost);
		else
			guest->defeated(Owner, tomter);
	}
	else if(group == 1){
		int rail_roads = n_street_in_zone(tomter);
		
		if(guest->pay(rent[rail_roads - 1]))//Pay, else defeat
			Owner->recieve_money(rent[rail_roads - 1]);
		else
			guest->defeated(Owner, tomter);
	}
	else if(guest->pay(rent[houses] * double_pay_factor)){ //Normal street, guest pay and return true if succesful
		Owner->recieve_money(rent[houses] * double_pay_factor);
	}
	else{
		guest->defeated(Owner, tomter);
	}
}

int Street::get_rent(int houses){
	return rent[houses];
}

bool Street::own_zone(Property* tomter[], Player* Owner_in){
	if(Owner_in == 0)
		Owner_in = Owner;
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_typ() == 0 && ((Street*)tomter[i])->get_zon() == get_zon()){ //Om det är en gata och är i samma zon
			if(tomter[i]->get_Owner() != Owner_in){ //Om denna gata inte ägs av samma ägare
				return (false);
			}
		}
	}
	return (true);
}

int Street::n_street_in_zone(Property* tomter[]){
	int n_owned = 0;
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_typ() == 0 && ((Street*)tomter[i])->get_zon() == get_zon()){ //Om det är en gata och är i samma zon
			if(tomter[i]->get_Owner() == Owner){ //Om denna gata ägs av samma ägare
				n_owned++;
			}
		}
	}
	return (n_owned);
}

void Street::draw(bool mark){
	int temp_color[3];
	if(Owner!= 0)
		Owner->get_color(temp_color);
	
	
	if(Owner!= 0 && mark){
		al_draw_line(pos_x_1, pos_y_1, pos_x_1, pos_y_2, al_map_rgb(temp_color[0], temp_color[1], temp_color[2]), 5);
		al_draw_line(pos_x_1, pos_y_1, pos_x_2, pos_y_1, al_map_rgb(temp_color[0], temp_color[1], temp_color[2]), 5);
		al_draw_line(pos_x_1, pos_y_2, pos_x_2, pos_y_2, al_map_rgb(temp_color[0], temp_color[1], temp_color[2]), 5);
		al_draw_line(pos_x_2, pos_y_1, pos_x_2, pos_y_2, al_map_rgb(temp_color[0], temp_color[1], temp_color[2]), 5);
	}

	for(int i = 0; i < houses && houses < max_houses; i++){
		al_draw_filled_rectangle( pos_x_1 + 18 * i + 3, pos_y_1 + 5, pos_x_1 + 18 * i + 15, pos_y_1 + 20, al_map_rgb(0, 255, 0));
	}
	if(houses == max_houses){
		al_draw_filled_rectangle(pos_x_1 + (pos_x_2 - pos_x_1) / 2 - 10, pos_y_1 + 5, pos_x_1 + (pos_x_2 - pos_x_1) / 2 + 10, pos_y_1 + 20, al_map_rgb(255, 0, 0));
	}
}

void Street::buy_house(Player* buyer, Property **tomter, int &tot_free_ant_houses, int &tot_free_ant_hotels){
	if(buyer == Owner && own_zone(tomter) && houses < max_houses && buyer->pay(building_cost)) {
		houses++;
		if(houses == max_houses)
			tot_free_ant_hotels--;
		else
			tot_free_ant_houses--;
	}
}