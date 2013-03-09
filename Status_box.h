#ifndef STATUS_BOX_H
#define STATUS_BOX_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

class Street;

class Status_box{
public:
	Status_box() {}
	Status_box(int pos_x_1, int pos_y_1, int pos_x_2, int pos_y_2, Street* street, int pos_x_displacement = 0, int pos_y_displacement = 0) : pos_x_1(pos_x_1), pos_x_2(pos_x_2), pos_y_1(pos_y_1), pos_y_2(pos_y_2), pos_x_displacement(pos_x_displacement), pos_y_displacement(pos_y_displacement), color_r(255), color_g(0), color_b(0), active(false) {
	}
	void init(int pos_x_1_in, int pos_y_1_in, int pos_x_2_in, int pos_y_2_in, int pos_x_displacement_in = 0, int pos_y_displacement_in = 0, Street *street_in = 0){pos_x_1 = pos_x_1_in; pos_x_2 = pos_x_2_in; pos_y_1 = pos_y_1_in; pos_y_2 = pos_y_2_in; pos_x_displacement = pos_x_displacement_in; pos_y_displacement = pos_y_displacement_in; active = false; color_r = 255; color_g = 0; color_b = 0; street = street_in;}
	void set_color(int color_r, int color_g, int color_b);
	void set_active(bool active);
	Street* get_street(){return street;}
	bool pressed(int mouse_pos_x, int mouse_pos_y){
		if(pos_x_1 <= mouse_pos_x && pos_x_2 >= mouse_pos_x && pos_y_1 <= mouse_pos_y && pos_y_2 >= mouse_pos_y)
			return true;
		else
			return false;
	}


	void draw();
private:
	bool active;
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int pos_x_displacement, pos_y_displacement;
	int color_r, color_g, color_b;
	Street *street;

};

#endif