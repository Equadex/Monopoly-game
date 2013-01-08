#ifndef PLAYER_H
#define PLAYER_H
#include "Property.h" //Used for coordinates and transfer ownership if player loses 
#include <allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>

//class Property; //Should not be forward declared!

class Player {
public:
	Player (int pos_x, int pos_y, int pos_ruta, int id, int pengar, int color1, int color2, int color3):pos_x(pos_x), pos_y(pos_y), pos_ruta(pos_ruta), id(id), pengar(pengar), color_r(color1), color_g(color2), color_b(color3), alive(true), square_size(40){}
	
	void move_Player(int steg);
	void get_pos(int &pos_x, int &pos_y) const;
	void update_Player(Property *tomter[],Player **players, int *player_IDs, int n_player_setoff, bool first_update = false); 
	int get_pos_ruta(){return pos_ruta;}
	int get_id(){return id;}

	void draw_player() const;

	bool pay(int sum_to_pay);
	void recieve_money(int sum_to_get);
	void get_color(int *color_out){
		color_out[0] = color_r; color_out[1] = color_g; color_out[2] = color_b;
	}
	int get_money() const;
	void get_color(int *array_in) const;

	void defeated(Player *winner, Property* tomter[]);
private:
	int pos_x;
	int pos_y;
	int pos_ruta;
	int pengar;
	int id;
	int color_r, color_g, color_b;
	const int square_size;
	bool alive;
};
#endif