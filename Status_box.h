#ifndef STATUS_BOX_H
#define STATUS_BOX_H
#include <allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>

class Status_box{
public:
	Status_box() {}
	Status_box(int pos_x_1, int pos_y_1, int pos_x_2, int pos_y_2) : pos_x_1(pos_x_1), pos_x_2(pos_x_2), pos_y_1(pos_y_1), pos_y_2(pos_y_2), color_r(0), color_g(0), color_b(0), active(false) {}
	void set_color(int color_r, int color_g, int color_b);
	void set_active(bool active);

	void draw();
private:
	bool active;
	int pos_x_1, pos_x_2;
	int pos_y_1, pos_y_2;
	int color_r, color_g, color_b;
};

#endif