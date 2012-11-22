#ifndef PLAYER_H
#define PLAYER_H
#include "Property.h"
#include <allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>

class Property;

class Player {
public:
	Player (int pos_x, int pos_y, int pos_ruta, int id, int pengar, int color1, int color2, int color3):pos_x(pos_x), pos_y(pos_y), pos_ruta(pos_ruta), id(id), pengar(pengar), color_r(color1), color_g(color2), color_b(color3){}
	
	void move_Player(int steg, Property *tomter[]);
	void get_pos(int &pos_x, int &pos_y) const;
	void update_Player(Property *tomter[]);
	int get_pos_ruta(){return pos_ruta;}

	void draw_player() const;

	bool pay(int sum_to_pay);
	void get_money(int sum_to_get);
private:
	int pos_x;
	int pos_y;
	int pos_ruta;
	int pengar;
	int id;
	int color_r, color_g, color_b;
};
#endif