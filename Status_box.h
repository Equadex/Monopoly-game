#ifndef STATUS_BOX_H
#define STATUS_BOX_H
#include <allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>
#include "Property.h"

class Property;
class Street;

class Status_box{
public:
	Status_box() {}
	Status_box(int pos_x_1, int pos_y_1, int pos_x_2, int pos_y_2, int pos_x_displacement = 0, int pos_y_displacement = 0) : pos_x_1(pos_x_1), pos_x_2(pos_x_2), pos_y_1(pos_y_1), pos_y_2(pos_y_2), pos_x_displacement(pos_x_displacement), pos_y_displacement(pos_y_displacement), color_r(255), color_g(0), color_b(0), active(false) {}
	//void init(int pos_x_1_in, int pos_y_1_in, int pos_x_2_in, int pos_y_2_in, int pos_x_displacement_in = 0, int pos_y_displacement_in = 0){pos_x_1_in = pos_x_1; pos_x_2_in = pos_x_2_in; pos_y_1_in = pos_y_1; pos_y_2_in; pos_x_displacement_in = pos_x_displacement; active = false; color_r = 255; color_g = 0; color_b = 0;}
	void set_color(int color_r, int color_g, int color_b);
	void set_active(bool active);

	void draw();
private:
	bool active;
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int pos_x_displacement, pos_y_displacement;
	int color_r, color_g, color_b;
};

#endif}