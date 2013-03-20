#include "Player.h"


void Player::move_Player(int steg, Property** tomter, Player** players, int n_players){
		pos_ruta += steg;
		passed_go_check();
		pos_ruta = pos_ruta % ant_rutor;
		update_Player(tomter, players, n_players);
}

void Player::get_pos(int &pos_x_in, int &pos_y_in) const{
	pos_x_in = pos_x;
	pos_y_in = pos_y;
}

void Player::draw_player() const  {
	al_draw_filled_rectangle(pos_x, pos_y, pos_x + square_size, pos_y + square_size, al_map_rgb(color_r, color_g, color_b));
	if(prisoned)
		al_draw_rectangle(pos_x, pos_y, pos_x + square_size, pos_y + square_size, al_map_rgb(128, 128, 128), 5);
}

void Player::update_Player(Property *tomter[], Player **players, int n_players, bool first_update){ 
	int player_IDs[max_players];
	int n_player_setoff = players_on_property(pos_ruta, players, player_IDs, n_players);
	
	int new_pos_x, new_pos_y;
	int temp_pos_x, temp_pos_y;
	int displacement_factor;
	tomter[pos_ruta]->get_player_pos(new_pos_x, new_pos_y);

	//Centrerar spelaren på positionen

	new_pos_x -= square_size / 2;
	new_pos_y -= square_size / 2;

	//Konstant förskutning 

	if(pos_ruta == 0){
		new_pos_y -= 20;
	}
	else if(pos_ruta > 10 && pos_ruta <= 20)
		new_pos_x -= 10;
	else if(pos_ruta > 20 && pos_ruta <= 30)
		new_pos_y -= 10;

	if(first_update){//Första gången endast
		
		if(pos_ruta == 0)
			new_pos_y += 10 * id;
		else if(pos_ruta <= 10)
			new_pos_y += 10 * id;
		else if(pos_ruta > 10 && pos_ruta <= 20)
			new_pos_x -= 10 * id;
		else if(pos_ruta > 20 && pos_ruta <= 30)
			new_pos_y -= 10 * id;
		else if(pos_ruta > 30 && pos_ruta <= 40)
			new_pos_x += 10 * id;
	
	}
	else{
		//Beräknar hur förskjuten spelaren ska vara på rutan

		for(int i = 0; i < n_player_setoff; i++){
			bool loop_done = false;
			int index = 0;

			for(int j = 0; j < n_players; j++){
				if(players[j]->get_id() == player_IDs[i])
					index = j;
			}

			players[index]->get_pos(temp_pos_x, temp_pos_y);
			while(!loop_done){
				if(temp_pos_x == new_pos_x && temp_pos_y == new_pos_y){
					if(pos_ruta == 0){
						new_pos_y += 10;
					}
					else if(pos_ruta <= 10)
						new_pos_y += 10;
					else if(pos_ruta > 10 && pos_ruta <= 20)
						new_pos_x -= 10;
					else if(pos_ruta > 20 && pos_ruta <= 30)
						new_pos_y -= 10;
					else if(pos_ruta > 30 && pos_ruta <= 40)
						new_pos_x += 10;
				}
				else{
					loop_done = true;
				}
			}
		}
	}

	pos_x = new_pos_x;
	pos_y = new_pos_y;
}

bool Player::pay(int sum_to_pay){
	if(sum_to_pay <= pengar){
		pengar -= sum_to_pay;
		return (true);
	}
	else{
		return (false);
	}
}
void Player::pay_player(Player* receiver, int sum_to_pay){
	if(pay(sum_to_pay)){ //If i can pay/get money
		if(!receiver->recieve_money(sum_to_pay)) //If reviever can take the money(or pay them)
			receiver->defeated(this, sum_to_pay);
	}
	else
		defeated(receiver, sum_to_pay);
}

bool Player::recieve_money(int sum_to_get){
	if((pengar + sum_to_get) >= 0){
		pengar += sum_to_get;
		return true;
	}
	else
		return false;
}

int Player::get_money() const{
	return (pengar);
}

void Player::get_color(int *array_in){
	array_in[0] = color_r;
	array_in[1] = color_g;
	array_in[2] = color_b;
}

void Player::defeated(Player *winner, int sum_required){
	//Activates defeat window and give it the numbers it needs(sum_required and pengar)
	
	char **reference_text;
	char reference_text_line[1001];
	reference_text_line[0] = '\0';
	char text_in[1001];
	int n_reference_text;
	
	reference_text = defeat_window->get_original_message(n_reference_text);
	for(int i = 0; i <= n_reference_text; i++){
		int length = strlen(reference_text[i]);
		if(length == 0){
			reference_text[i][0] = '\n';
			reference_text[i][1] = '\0';
		}
		strncat(reference_text_line, reference_text[i], (1000 / (n_reference_text + 1))); //Copy the content from the differnt lines in reference_text to a single line
	}
	sprintf(text_in, reference_text_line, sum_required, pengar);
	defeat_window->set_text(0, text_in);

	defeat_window->set_active(true);

	in_dept_to = winner;
	dept = sum_required;
}

void Player::set_pos_ruta(int pos_ruta_in){
	pos_ruta = pos_ruta_in;
}

int Player::players_on_property(int pos_ruta, Player** players, int *players_IDs, int n_players){
	int player_on_pos_ruta = 0;
	for(int i = 0, j = 0; i < n_players; i++){
		if(players[i]->get_pos_ruta() == pos_ruta){
			player_on_pos_ruta++;
			players_IDs[j] = players[i]->get_id();
			j++;
		}
	}
	return(player_on_pos_ruta);
}

void Player::passed_go_check(){
	if(pos_ruta / ant_rutor >= 1)
		passed_go = true;
}

void Player::passing_go_check(int steg){
	if((pos_ruta + steg) / ant_rutor >= 1)
		passed_go = true;
}
