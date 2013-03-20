#ifndef WINDOW_LIST_H
#define WINDOW_LIST_H

#include "Question.h" //Base class
#include "Player.h" //Needed to get id of player

class Window_list : public Question{
public:
	Window_list(int pos_x, int pos_y, int player_bids[],Player** players, int &n_players, ALLEGRO_BITMAP *image_in = 0, bool active = false);
	~Window_list(); 
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);
	void update_status(bool bid, Player *player);
	void update_names();
	void update_colors();

private:
	int **status_color;
	int **player_color;
	int *player_bids;
	Player** players;

	int &n_players;
	const int max_question_length;
	const int max_question_lines;
	const int max_question_lines_title;
};

#endif