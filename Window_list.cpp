#include "Window_list.h"

Window_list::Window_list(int pos_x, int pos_y, int player_bids[],Player** players, int &n_players, ALLEGRO_BITMAP *image_in , bool active) : n_players(n_players), Question(pos_x, pos_y, image_in, active), player_bids(player_bids), max_question_length(90), max_question_lines(n_players), max_question_lines_title(n_players), players(players) {
	//Default colors for status for a players bid 
		
	status_color = new int*[n_players];
	for(int i = 0; i < n_players; i++){
		status_color[i] = new int[3];
		status_color[i][0] = 255;
		status_color[i][1] = 0;
		status_color[i][2] = 0;
	}

		//Color for player in list

		player_color = new int*[n_players];
	for(int i = 0; i < n_players; i++){
		player_color[i] = new int[3];
		(players[i])->get_color(player_color[i]);
	}

	//Reserving memory
	
	title = new char*[max_question_lines_title];
	for(int i = 0; i < max_question_lines_title; i++){
		title[i] = new char[max_question_length];
	}
	message = new char*[max_question_lines];
	for(int i = 0; i < max_question_lines; i++){
		message[i] = new char[max_question_length];
	}

	//Creates text to list

	for(int i = 0; i < max_question_lines; i++){
		char temp[] = "Player ";
		int temp_length = strlen(temp);
		strncpy(title[i], temp, max_question_length - 1);
		if(temp_length >= max_question_length)
			title[i][max_question_length-1] = '\0';
		else
			title[i][temp_length] = 48 + i;
		message[i][0] = '\0';
	}
}

Window_list::~Window_list(){
	for(int i = 0; i < n_players; i++){
		delete[] status_color[i];
		delete[] player_color[i];
	};
	delete[] status_color;
	delete[] player_color;


	//Releasing memory
	for(int i = 0; i < max_question_lines_title; i++){
		delete[] title[i];
	}
	delete[] title;
	for(int i = 0; i < max_question_lines; i++){
		delete[] message[i];
	}
	delete[] message;	
}

void Window_list::draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text){
char temp[9];
	for(int i = 0; i < n_players; i++){
		sprintf(temp, "%d", player_bids[i]);
		al_draw_filled_rectangle(pos_x - 20, pos_y + 25 * i, pos_x - 5, pos_y + 25 * i + 15, al_map_rgb(status_color[players[i]->get_id()][0], status_color[players[i]->get_id()][1], status_color[players[i]->get_id()][2]));
		al_draw_text(Text, al_map_rgb(player_color[i][0], player_color[i][1], player_color[i][2]), pos_x, pos_y + i * 25, 0, title[i]);
		al_draw_text(Text, al_map_rgb(player_color[i][0], player_color[i][1], player_color[i][2]), pos_x + 130, pos_y + i * 25, 0, temp);
	}
}

void Window_list::update_status(bool bid, Player *player){
	if(bid){
		status_color[player->get_id()][0] = 0;
		status_color[player->get_id()][1] = 255;
		status_color[player->get_id()][2] = 0;
	}
	else{
		status_color[player->get_id()][0] = 255;
		status_color[player->get_id()][1] = 0;
		status_color[player->get_id()][2] = 0;
	}
}

void Window_list::update_names(){
	for(int i = 0; i < n_players; i++){
		char temp[] = "Player ";
		int temp_length = strlen(temp);
		strncpy(title[i], temp, max_question_length - 1);
		if(temp_length >= max_question_length)
			title[i][max_question_length-1] = '\0';
		else
			title[i][temp_length] = 48 + players[i]->get_id();
		message[i][0] = '\0';
	}
}

void Window_list::update_colors(){
	for(int i = 0; i < n_players; i++){
		player_color[i] = new int[3];
		(players[i])->get_color(player_color[i]);
	}
}