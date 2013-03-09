#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Sprite{
public:
	Sprite(int pos_x, int pos_y, int pixel_interval, ALLEGRO_BITMAP *image = 0) : pos_x(pos_x), pos_y(pos_y), pixel_intervals(pixel_interval), current_frame(0), image(image) {}
	void draw();
	void set_curret_frame(int value){current_frame = value;}
private:
	int pos_x;
	int pos_y;
	int pixel_intervals;
	int current_frame;
	ALLEGRO_BITMAP *image;
};

#endif