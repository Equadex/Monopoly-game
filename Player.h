#ifndef PLAYER_H
#define PLAYER_H
#include "Property.h" //Used for coordinates and transfer ownership if player loses 
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

//class Property; //Should not be forward declared!

class Player {
public:
	Player (int pos_x, int pos_y, int pos_ruta, int id, int pengar, int color1, int color2, int color3):pos_x(pos_x), pos_y(pos_y), pos_ruta(pos_ruta), id(id), pengar(pengar), color_r(color1), color_g(color2), color_b(color3), alive(true), square_size(40), passed_go(false), prisoned(false), c_jail_card(false), a_jail_card(false){}
	
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
	

	void draw_player() const;

	bool pay(int sum_to_pay);
	void pay_player(Player* receiver, int sum_to_pay, Property** tomter);
	bool recieve_money(int sum_to_get);
	void get_color(int *color_out) const{
		color_out[0] = color_r; color_out[1] = color_g; color_out[2] = color_b;
	}
	int get_money() const;
	void get_color(int *array_in);

	void defeated(Player *winner, Property* tomter[]);
	void passing_go_check(int steg);

private:
	int pos_x;
	int pos_y;
	int pos_ruta;
	int pengar;
	int id;
	int color_r, color_g, color_b;
	const int square_size;
	bool alive;
	

	bool passed_go; //Flags
	bool prisoned;
	char own_jail_card;
	bool c_jail_card, a_jail_card;

	int players_on_property(int pos_ruta, Player** players, int *players_IDs, int n_players);
	void passed_go_check();
};
#endif