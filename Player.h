#ifndef PLAYER_H
#define PLAYER_H
#include "Constants.h"
#include "Question.h"
#include "Property.h" //Used for coordinates and transfer ownership if player loses 
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

//class Property; //Should not be forward declared!

class Player {
public:
	Player (int pos_x, int pos_y, int pos_ruta, int id, int pengar, int color1, int color2, int color3, Question *defeat_window = 0):pos_x(pos_x), pos_y(pos_y), pos_ruta(pos_ruta), id(id), pengar(pengar), color_r(color1), color_g(color2), color_b(color3), alive(true), square_size(40), passed_go(false), prisoned(false), c_jail_card(false), a_jail_card(false), defeat_window(defeat_window), in_dept_to(0), dept(0){}
	void move_Player(int steg, Property** tomter, Player** players, int n_players);
	void get_pos(int &pos_x, int &pos_y) const;
	void update_Player(Property *tomter[],Player **players, int n_players, bool first_update = false); 
	int get_pos_ruta() const{return pos_ruta;}
	int get_id() const{return id;}
	int get_flag_prisoned() const{return prisoned;}
	bool get_flag_passed_go() const{return passed_go;}
	bool get_flag_a_jail_card() const{return a_jail_card;}
	bool get_flag_c_jail_card() const{return c_jail_card;}
	void set_flag_passed_go(bool set_value){passed_go = set_value;}
	void set_pos_ruta(int pos_ruta_in);
	void set_flag_prisoned(bool value){prisoned = value;}
	void set_flag_c_jail_card(bool value){c_jail_card = value;}
	void set_flag_a_jail_card(bool value){a_jail_card = value;}
	void set_defeat_window(Question &defeat_window_in){
		defeat_window = &defeat_window_in;
	}
	

	void draw_player() const;

	bool pay(int sum_to_pay);
	void pay_player(Player* receiver, int sum_to_pay);
	bool recieve_money(int sum_to_get);
	void get_color(int *color_out) const{
		color_out[0] = color_r; color_out[1] = color_g; color_out[2] = color_b;
	}
	int get_money() const;
	void get_color(int *array_in);

	void defeated(Player *winner, int sum_required);
	void passing_go_check(int steg);
	bool player_lost(Property **tomter){
		if(dept > pengar){
			if(!in_dept_to == 0)
				in_dept_to->recieve_money(pengar);
			else
				pengar = 0;
			for(int i = 0; i < ant_rutor; i++){
				if(tomter[i]->get_Owner() == this && tomter[i]->get_typ() == TOMT){
					tomter[i]->set_Owner(in_dept_to);
				}
			}
			alive = false;
			return true;
		}
		else{
			if(in_dept_to == 0)
				pay(dept);
			else
				pay_player(in_dept_to, dept);
			dept = 0;
			in_dept_to = 0;
			return false;
		}
	}

private:
	int pos_x;
	int pos_y;
	int pos_ruta;
	int pengar;
	int id;
	int color_r, color_g, color_b;
	const int square_size;
	bool alive;
	
	Question* defeat_window;

	bool passed_go; //Flags
	bool prisoned;
	char own_jail_card;
	bool c_jail_card, a_jail_card;

	Player* in_dept_to;
	int dept;

	int players_on_property(int pos_ruta, Player** players, int *players_IDs, int n_players);
	void passed_go_check();
};
#endif