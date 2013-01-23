#include "Constants.h"

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5/allegro_opengl.h>

#include "Player.h"
#include "Property.h"
#include "Street.h"
#include "Button.h"
#include "Status_box.h"
#include "Street_info.h"
#include "Sprite.h"
#include "Question.h"
#include "Auction.h"
#include "Tax.h"
#include "Card.h"
#include "Chance.h"


//Globala variabler lokala

const int max_config_line_length = 128;
const int max_config_extended_length = 350;
const int max_name_length = 64;
const int property_int_data_count = 19;
const int property_variable_count = property_int_data_count + 1;
const int button_int_data_count = 5;
const int button_variable_data_count = button_int_data_count + 1;
const int status_box_int_data_count = 5;
const int cards_int_data_count = 5;
const int cards_variable_count = cards_int_data_count + 1;

const int startpengar = 1500;
const int start_ruta = 0;
const int max_tarning = 6;
const int ant_buttons = 4;
const int ant_status_box = 28;

//Funktionsdeklartioner

void read_Property_data(Property *tomter[]);
void getline_char(char line_in[],char line_out[],int out_length, int max_read, char stop, bool &endfile, int start = 0);
void create_players(int n_players, Player *players[], Property *tomter[]);
void create_Street_info(Property **tomter, ALLEGRO_BITMAP *street_info_bitmap, ALLEGRO_BITMAP *street_info_railroad, ALLEGRO_BITMAP *street_info_el, ALLEGRO_BITMAP *steet_info_water, ALLEGRO_FONT *normal_text, ALLEGRO_FONT *bigger_text, ALLEGRO_FONT *small_text);
void roll_dice(int &dice);
void read_Button_data(Button* buttons[]);
void read_Status_box_data(Property **tomter);
void auction();
int players_on_property(int pos_ruta, Player** players, int *players_IDs, int n_players);
void color_to_comp_color(const int color[], int color_comp[]);
void al_set_street_info_false(Property *tomter[], bool full = false); //Full = true resets all, false only the first
void read_card_data(Card *cards[]);
void seperate_cards(Card** cards, Card** cards_1, Card** cards_2, int n1, int n2, int n);
void do_action(Chance *card_pile, int id_card, Player* c_player,Player** players, int n_players, Property **tomter, Question *buy_street_Q, int &ID_card, Chance* chans, Chance* allmaning, int dice_1, int dice_2);
void after_movement(Player* c_player, Question* buy_street_Q, Property **tomter, int dice_1, int dice_2, int &ID_card, Chance *chans, Chance *allmaning);

int main(){
	//Konstanta variabler i main

	//Variabler som beh�vs f�r hantera allegro
	int window_width;
	int	window_height;
	int width = 1280;
	int height = 1000;
	double FPS = 120;
	float mouse_pos_x = 0;
	float mouse_pos_y = 0;
	int frames = 0, gameFPS = 0;
	float gameTime = 0;
	double sx, sy, scale, scaleW, scaleH, scaleX, scaleY;

	bool done = false;
	bool draw = false;

	//Variabler n�dv�ndiga f�r spelet

	enum typ{TOMT, SKATT, CHANS, ALLMANING};
	int n_players = 6, current_player = 0;
	int dice_1, dice_2;
	int ID_button_pressed, ID_card;
	int c_player_color[3];

	bool dice_used = false;

	Property *tomter[ant_rutor]; //F�lt av tomter
	Player *players[max_players]; //F�lt av spelare
	Button *buttons[ant_buttons]; //F�lt av buttons
	Card *cards[ant_cards], *c_cards[ant_cards / 2], *a_cards[ant_cards / 2]; //Fält av kort
	Sprite *dice_sprite_0;
	Sprite *dice_sprite_1;
	Chance *chans, *allmaning;

	read_Property_data(tomter); //L�ser in data till tomter
	create_players(n_players, players, tomter); //Skapar spelare
	read_Button_data(buttons); //L�ser in buttons koodinater och id
	read_Status_box_data(tomter); //L�ser in status boxarnas positioner och gatunummer
	read_card_data(cards); //Läser in kortdata
	seperate_cards(cards, c_cards, a_cards, ant_cards / 2, ant_cards / 2, ant_cards); //Delar upp korten i två korthögar till chans och allmäning

	players[current_player]->get_color(c_player_color); //Färg för nuvarrande spelare

	//Allegro variabler

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_MONITOR_INFO info;
	ALLEGRO_MONITOR_INFO* p_info = &info;

	//Allegro Bitmaps

	ALLEGRO_BITMAP *buffer = NULL;
	ALLEGRO_BITMAP *spelplan = NULL;
	ALLEGRO_BITMAP *spelbrade = NULL;
	ALLEGRO_BITMAP *dice = NULL;
	ALLEGRO_BITMAP *question = NULL;
	ALLEGRO_BITMAP *button = NULL;
	ALLEGRO_BITMAP *auction_image = NULL;
	ALLEGRO_BITMAP *box = NULL;
	ALLEGRO_BITMAP *street_info = NULL;
	ALLEGRO_BITMAP *street_info_railroad = NULL;
	ALLEGRO_BITMAP *street_info_el = NULL;
	ALLEGRO_BITMAP *street_info_water = NULL;

	if(!al_init()){ //Initierar allegro bibloteket
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize Allegro" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	//Tar reda p� sk�rmens storlek och omskalninsf�rh�llanden
	if(!al_get_monitor_info(0, p_info)){
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to get screen info" , NULL, ALLEGRO_MESSAGEBOX_ERROR); 
		return(-1);
	}
	window_width = p_info->x2; 
	window_height = p_info->y2 - 30;

	sx = window_width / (double)width;
	sy = window_height / (double)height;
	scale = std::min <double>(sx, sy);

	scaleW = width * scale;
	scaleH = height * scale;
	scaleX = (window_width - scaleW) / 2;
	scaleY = (window_height - scaleH) / 2;



	//Skapar och testar display
	//al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_new_display_flags(ALLEGRO_OPENGL);
	display = al_create_display(scaleW, scaleH);
	if(!display){
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize Display" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}


	//Skapar timer
	timer = al_create_timer(1/FPS);

	//init addons
	al_init_image_addon();
	al_install_mouse();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	
	srand(time(NULL));

	//Laddar bitmaps
	spelplan = al_load_bitmap("spelplan.bmp");
	spelbrade = al_load_bitmap("monopoly.jpg");
	dice = al_load_bitmap("dice_sprite.bmp");
	question = al_load_bitmap("Question.bmp");
	button = al_load_bitmap("button.bmp");
	buffer = al_create_bitmap(width, height);
	auction_image = al_load_bitmap("Auction.bmp");
	box = al_load_bitmap("box.bmp");
	street_info = al_load_bitmap("street_info.bmp");
	street_info_railroad = al_load_bitmap("street_info_railroad.bmp");
	street_info_el = al_load_bitmap("street_info_el.bmp");
	street_info_water = al_load_bitmap("street_info_water.bmp");
	//Skapar fonts

	ALLEGRO_FONT *arial_16 = al_load_ttf_font("arial.ttf", 16, 0);
	ALLEGRO_FONT *arial_36 = al_load_ttf_font("arial.ttf", 36, 0);
	ALLEGRO_FONT *arial_20 = al_load_ttf_font("arial.ttf", 20, 0);
	ALLEGRO_FONT *arial_10 = al_load_ttf_font("arial.ttf", 12, 0); 

	//Temp kod som inte borde se ut s�h�r
	
	//Street_info *test = new Street_info(0, 0, (Street*)tomter[0], street_info, false); //Test, ska tas bort
	create_Street_info(tomter, street_info, street_info_railroad, street_info_el, street_info_water, arial_16, arial_20, arial_10);

	dice_sprite_0 = new Sprite(1091, 6, 81, dice); 
	dice_sprite_1 = new Sprite(1172, 6, 81, dice);
	
	chans = new Chance(c_cards, question, button);
	allmaning = new Chance(a_cards, question, button);


	Button *temp[2];
	temp[0] = new Button(165 + 162, 275 + 250, 165 + 162 + 80, 275 + 250 + 25, 1, "Buy", button);
	temp[1] = new Button(600 - 162 + 80, 275 + 250, 600, 275 + 250 + 25, 2, "Auction", button);
	Question *buy_street_Q = new Question(Question_pos_x_standard, Question_pos_y_standard, temp, 2, "Buy or auction?", "This property is owned by the bank and is for sale. Do you want to buy it or let it be sold by auction?", question);
	Auction *auction = new Auction(0, 120, players, n_players, auction_image, button, box, arial_36, arial_16);
	((Tax*)tomter[4])->create_income_tax_question(Question_pos_x_standard, Question_pos_y_standard, button, question); //Skapar fråga till inkomst skatt ruta

	//Skapar event_queue, registrerar k�llor och startar timer
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_start_timer(timer);
	gameTime = al_get_time();

	//Spelloop
	while(!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){  //L�ser av koordinater fr�n musen
			mouse_pos_x = (ev.mouse.x * 1 ) / (double)scale;
			mouse_pos_y = (ev.mouse.y * 1 ) / (double)scale;

		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ //N�r musknapp �r nedtryckt
			if(ev.mouse.button == 1){ //V�nster musknapp
				
				for(int i = 0; i < ant_rutor; i++){
					if(tomter[i]->get_typ()  == TOMT && ((Street*)tomter[i])->button_pressed(mouse_pos_x, mouse_pos_y)){ // Om tomten är av gatutyp OCH Om knapp nedtryckt(statusboxen till den gatan)
						if((((Street*)tomter[i])->get_Street_info())->get_active()){ //OM aktiv, sätt till inaktiv och tvärtom
							(((Street*)tomter[i])->get_Street_info())->set_active(false);
						}
						else{
							al_set_street_info_false(tomter);
							(((Street*)tomter[i])->get_Street_info())->set_active(true);
						}
					}
				} //Ny kod för street_info ska läggas in här


				if(buy_street_Q->get_active()){
					int ID_button_pressed_temp = buy_street_Q->button_pressed(mouse_pos_x, mouse_pos_y);
					if(ID_button_pressed_temp != 0){
						switch(ID_button_pressed_temp){
							case 1:
								if(((Street*)tomter[players[current_player]->get_pos_ruta()])->get_cost() > players[current_player]->get_money()){ //If property cost more than the player has, do case 2(auction)
									auction->set_property(((Street*)tomter[players[current_player]->get_pos_ruta()]));
									auction->set_active(true);
								}
								else
									((Street*)tomter[players[current_player]->get_pos_ruta()])->buy_Street(players[current_player]); //Buy street
								break;
							case 2:
								auction->set_property(((Street*)tomter[players[current_player]->get_pos_ruta()]));
								auction->set_active(true);
								break;
						}
						buy_street_Q->set_active(false);
					}
				}
				else if(auction->get_active()){
					auction->button_pressed(mouse_pos_x, mouse_pos_y);
				}
				else if(((Tax*)tomter[4])->get_question()->get_active()){ //Om inkomst skatt är aktiv
					int ID_button_pressed_temp = ((Tax*)tomter[4])->get_question()->button_pressed(mouse_pos_x, mouse_pos_y);
					
					if(ID_button_pressed_temp != 0){
						switch(ID_button_pressed_temp){
							case 1:
								((Tax*)tomter[4])->pay(players[current_player], tomter);
								break;
							case 2:
								((Tax*)tomter[4])->pay_fee(players[current_player], tomter);
								break;
						}
						((Tax*)tomter[4])->set_question_active(false);
					}
				}
				else if(chans->get_window()->button_pressed(mouse_pos_x, mouse_pos_y)){
					chans->get_window()->set_active(false);
					do_action(chans, ID_card, players[current_player], players, n_players, tomter, buy_street_Q, ID_card, chans, allmaning, dice_1, dice_2);
				}
				else if(allmaning->get_window()->button_pressed(mouse_pos_x, mouse_pos_y)){
					allmaning->get_window()->set_active(false);
					do_action(allmaning, ID_card, players[current_player], players, n_players, tomter, buy_street_Q, ID_card, chans, allmaning, dice_1, dice_2);
				}
				else{
					for(int i = 0; i < ant_buttons; i++){ //Kontrollerar vilken knapp som blivit klickad
						if(buttons[i]->Button_pressed(mouse_pos_x, mouse_pos_y)){
							ID_button_pressed = buttons[i]->get_ID();
						}
					}
				
					switch(ID_button_pressed){
						case 1: //Sl� t�rningarna
							if(!dice_used){
								int temp[max_players];
								roll_dice(dice_1); roll_dice(dice_2);
								
								dice_sprite_0->set_curret_frame(max_tarning - (dice_1)); //Byter bild p� t�rning
								dice_sprite_1->set_curret_frame(max_tarning - (dice_2));

								players[current_player]->move_Player(dice_1 + dice_2, tomter, players, n_players); //Flyttar spelare

								after_movement(players[current_player], buy_street_Q, tomter, dice_1, dice_2, ID_card, chans, allmaning);

								if(dice_1 != dice_2)
									dice_used = true;
								else
									dice_used = false;
							}
							break;
						case 2: //K�per tomt
							if(tomter[players[current_player]->get_pos_ruta()]->get_typ() == 0){
								((Street*)tomter[players[current_player]->get_pos_ruta()])->buy_Street(players[current_player]);
							}
							break;
						case 3:
							if(dice_used){
								current_player = (current_player + 1) % n_players; //N�sta spelare
								dice_used = false;
								players[current_player]->get_color(c_player_color); //Färg för nuvarrande spelare
							}
							break;
						case 4: //S�lja gata
							if(tomter[players[current_player]->get_pos_ruta()]->get_typ() == 0){
								((Street*)tomter[players[current_player]->get_pos_ruta()])->sell_Street(players[current_player]);
							}
							break;
					}
					ID_button_pressed = 0;
				}
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_TIMER){
			
			frames++; //R�knar frames
			if(al_get_time() - gameTime >= 1){ //Uppdaterar fps efter 1 sekund 
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}
			draw = true;
			
		}

		if(draw){
			
			//Drawing
			al_set_target_bitmap(buffer);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(spelplan, 0, 0, 0);
			al_draw_bitmap(spelbrade, 0, 0, 0);
			for(int i = 0; i < n_players; i++){ //Ritar spelare
				players[i]->draw_player();
			}
			for(int i = 0; i < ant_rutor; i++){ //Ritar status_boxar
				if(tomter[i]->get_typ() == 0)
					((Street*)tomter[i])->draw_status();
			}
			dice_sprite_0->draw(); //Ritar t�rningar
			dice_sprite_1->draw();
			if(buy_street_Q->get_active()) //Ritar k�pfr�gan om den �r aktiv
				buy_street_Q->draw(arial_36, arial_16);
			else if(auction->get_active()){//Ritar auktionsfönstret
				auction->draw();
			}
			else if((((Tax*)tomter[4])->get_question())->get_active()){ //Om inkomst skatt fråga är aktiv
				(((Tax*)tomter[4])->get_question())->draw(arial_36, arial_16);
			}
			else if((chans->get_window())->get_active()) //Om chans ruta är aktiv
				(chans->get_window())->draw(arial_36, arial_16);
			else if((allmaning->get_window())->get_active()) //Om allmäning ruta är aktiv
				(allmaning->get_window())->draw(arial_36, arial_16);

			for(int i = 0; i < ant_buttons; i++){ //Ritar knappar
				buttons[i]->draw(arial_16);
			}
			for(int i = 0; i < ant_rutor; i++){ //Ritar Street_info
				if(tomter[i]->get_typ()  == TOMT){
					if((((Street*)tomter[i])->get_Street_info())->get_active())
						(((Street*)tomter[i])->get_Street_info())->draw();
				}
			}

			//al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);
			al_draw_textf(arial_16, al_map_rgb(c_player_color[0], c_player_color[1] , c_player_color[2]), 15, 940, 0, "Player %i", current_player);
			al_draw_textf(arial_16, al_map_rgb(0, 0, 0), 100, 940, 0, "Funds: %i", (players[current_player])->get_money());
			//al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 20, 0, "Mouse_x: %lf Mouse_y: %lf", mouse_pos_x, mouse_pos_y);

			//Skalar om bilden och ritar till backbuffern. V�nder sedan p� buffern

			al_set_target_backbuffer(display);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_scaled_bitmap(buffer, 0, 0, width, height, 0, 0, scaleW , scaleH, 0);
			al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 20, 0, "Mouse_x: %lf Mouse_y: %lf", mouse_pos_x, mouse_pos_y);
			al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);
			al_flip_display();
			draw = false;
		}
		
	}
	//Destruktorer
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_bitmap(spelplan);
	al_destroy_bitmap(spelbrade);
	al_destroy_bitmap(dice);
	al_destroy_bitmap(question);
}

void read_Property_data(Property *tomter[]){
	//�ppnar fil f�r data till klassen Property
	std::ifstream file("property_config.txt"); 
	//Tillf�llig variabler f�r initiering av objekten
	char namn[max_name_length];
	namn[0] = '/0';
	enum intdata {posx1, posx2, posy1, posy2, posruta};
	int intdata[property_int_data_count];
	char line[max_config_line_length];
	char line2[max_config_line_length];
	bool endfile = false;
	
	if(!file){  //Kontrollerar att filen �r �ppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize load Property file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_rutor; i++){
			
			file.getline(line, (max_config_line_length-1));
			while(line[0] == '#'){ //L�ser in ny rad om f�rsta tecknet �r kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			for(int j = 0, r_pos = 0; j < property_variable_count; j++){
				if(!endfile){ //Kontrollerar om nollf�rtecken tidigare hittats
					getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
				}
				r_pos += strlen(line2) + 1; //R�knar hur m�nga tecken som har l�sts och anv�nds f�r att veta var n�sta inl�sning ska ske i str�ngen

				if(j < property_int_data_count){
					intdata[j] = std::atoi(line2);
				}
				else{
					strncpy(namn, line2, max_name_length-1);
				}
			}
			
			if(intdata[5] == TOMT){
				int temp_rent_array[max_houses + 1];

				for(int k = 0; k < (max_houses + 1); k++){
					temp_rent_array[k] = intdata[8 + k];
				}
				tomter[i] = new Street(intdata[0], intdata[2], intdata[1], intdata[3], intdata[4], namn,intdata[5] , intdata[7], intdata[14],temp_rent_array , intdata[6], intdata[16],intdata[17],intdata[18]);
			}
			else if(intdata[5] == SKATT){
				tomter[i] = new Tax(intdata[0], intdata[2], intdata[1], intdata[3], intdata[4], namn, intdata[5], intdata[15]);
			}
			else{
				tomter[i] = new Property(intdata[0], intdata[2], intdata[1], intdata[3], intdata[4], namn, intdata[5]); //Skapar tomt
			}
			endfile = false;
		
		}
}

void getline_char(char line_in[],char line_out[],int out_length, int max_read, char stop, bool &endfile, int start){
	bool size_ok = true;
	if(out_length < (strlen(line_in))){
		size_ok = false;
	}
	for(int i = start, j = 0; i < max_read+start && size_ok && start < strlen(line_in); i++, j++){
		if(line_in[i] == stop){
			line_out[j] = '\0';
			break;
		}
		else if(line_in[i] == '\0'){
			endfile = true;
			line_out[j] = '\0';
			break;
			}
		else{
			line_out[j] = line_in[i];
		}
	}
}

void create_players(int n_players, Player *players[], Property *tomter[]){
	//Definerar f�rger till de olika spelarna

	int player_colors[max_players * 3];
	player_colors[0] = 255; player_colors[1] = 131; player_colors[2] = 43;
	//player_colors[0] = al_map_rgb(255, 131, 43);
	player_colors[3] = 255; player_colors[4] = 50; player_colors[5] = 71;
	//player_colors[1] = al_map_rgb(255, 206, 102);
	//player_colors[2] = al_map_rgb(166, 213, 68);
	player_colors[6] = 166; player_colors[7] = 213; player_colors[8] = 68;
	//player_colors[3] = al_map_rgb(255, 91, 255);
	player_colors[9] = 255; player_colors[10] = 91; player_colors[11] = 255;
	//player_colors[4] = al_map_rgb(155, 0, 155);
	player_colors[12] = 155; player_colors[13] = 0; player_colors[14] = 155;
	//player_colors[5] = al_map_rgb(36, 166, 145);
	player_colors[15] = 0; player_colors[16] = 0; player_colors[17] = 255;
	
	for(int i = 0; i < n_players; i++){ //Skapar spelare
		int pos_x, pos_y, pos_ruta;

		tomter[start_ruta]->get_player_pos(pos_x, pos_y);  //Ger startposition f�r spelaren
		pos_ruta = tomter[start_ruta]->get_pos_ruta();

		int temp_r = player_colors[(i * 3)];
		int temp_g = player_colors[(i * 3) + 1];
		int temp_b = player_colors[(i * 3) + 2];

		players[i] = new Player(pos_x, pos_y, pos_ruta, i, startpengar, player_colors[(i * 3)], player_colors[(i * 3) + 1], player_colors[(i * 3) + 2]);
	}
	//Uppdaterar spelarnas positioner
	
	for(int i = 0; i < n_players; i++){
		players[i]->update_Player(tomter, players, n_players, true);
	}
}

void roll_dice(int &dice){
	dice = rand() % max_tarning + 1;
}

void read_Button_data(Button* buttons[]) {
	//�ppnar fil f�r data till klassen Property
	std::ifstream file("button_config.txt"); 
	//Tillf�llig variabler f�r initiering av objekten
	int intdata[button_int_data_count];
	char line[max_config_line_length];
	char line2[max_config_line_length];
	char label[max_config_line_length];
	bool endfile = false;

	if(!file){  //Kontrollerar att filen �r �ppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize load Property file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_buttons; i++){
		file.getline(line, (max_config_line_length-1));
		for(int j = 0, r_pos = 0; j < button_variable_data_count; j++){
			while(line[0] == '#'){ //L�ser in ny rad om f�rsta tecknet �r kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			if(!endfile){ //Kontrollerar om nollf�rtecken tidigare hittats
				getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
			}
			r_pos += strlen(line2) + 1; //R�knar hur m�nga tecken som har l�sts och anv�nds f�r att veta var n�sta inl�sning ska ske i str�ngen
			
			if(j < button_int_data_count){
				intdata[j] = std::atoi(line2);
			}
			else
				strncpy(label, line2, max_name_length-1);
		}
		buttons[i] = new Button(intdata[0], intdata[1], intdata[2], intdata[3], intdata[4], label);
		endfile = false;
	}
}

void read_Status_box_data(Property** streets){
	//�ppnar fil f�r data till klassen Property
	std::ifstream file("Status_box_config.txt"); 
	//Tillf�llig variabler f�r initiering av objekten
	int intdata[status_box_int_data_count];
	char line[max_config_line_length];
	char line2[max_config_line_length];
	bool endfile = false;
	const int displacement_x = 0;
	const int displacement_y = 51;

	if(!file){  //Kontrollerar att filen �r �ppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize Status_box file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_status_box; i++){
		file.getline(line, (max_config_line_length-1));
		for(int j = 0, r_pos = 0; j < status_box_int_data_count; j++){
			while(line[0] == '#'){ //L�ser in ny rad om f�rsta tecknet �r kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			if(!endfile){ //Kontrollerar om nollf�rtecken tidigare hittats
				getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
			}
			r_pos += strlen(line2) + 1; //R�knar hur m�nga tecken som har l�sts och anv�nds f�r att veta var n�sta inl�sning ska ske i str�ngen
			intdata[j] = std::atoi(line2);
		}
		
		((Street*)streets[intdata[4]])->create_status_box(new Status_box(intdata[0], intdata[1], intdata[2], intdata[3], ((Street*)streets[intdata[4]]), displacement_x, displacement_y ));
		endfile = false;
	}
}

void auction(Question* buy_street_Q){
	buy_street_Q->set_active(false);
}

int players_on_property(int pos_ruta, Player** players, int *players_IDs, int n_players){
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

void create_Street_info(Property **tomter, ALLEGRO_BITMAP *street_info_bitmap, ALLEGRO_BITMAP *street_info_railroad, ALLEGRO_BITMAP *street_info_el, ALLEGRO_BITMAP *steet_info_water, ALLEGRO_FONT *normal_text, ALLEGRO_FONT *bigger_text, ALLEGRO_FONT *small_text){
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_typ() == TOMT){
			((Street*)tomter[i])->create_street_info(new Street_info(1020, 460, ((Street*)tomter[i]), street_info_bitmap, street_info_railroad, street_info_el, steet_info_water, normal_text, bigger_text, small_text));
		}
	}
}

void al_set_street_info_false(Property *tomter[], bool full){
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_typ()  == TOMT){ // Om tomten är av gatutyp
			if((((Street*)tomter[i])->get_Street_info())->get_active()){
				(((Street*)tomter[i])->get_Street_info())->set_active(false);
				if(!full)
					break;
			}
		}
	}
}

void read_card_data(Card *cards[]){
	//�ppnar fil f�r data till klassen Property
	std::ifstream file("cards_config.txt"); 
	//Tillf�llig variabler f�r initiering av objekten
	char namn[max_card_length];
	namn[0] = '/0';
	int intdata[cards_int_data_count];
	char line[max_config_extended_length];
	char line2[max_config_extended_length];
	bool endfile = false;
	
	if(!file){  //Kontrollerar att filen �r �ppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize load Cards file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_cards; i++){
			
			file.getline(line, (max_config_extended_length-1));
			while(line[0] == '#'){ //L�ser in ny rad om f�rsta tecknet �r kommentartecken
				file.getline(line, (max_config_extended_length-1));
			}
			for(int j = 0, r_pos = 0; j < cards_variable_count; j++){
				if(!endfile){ //Kontrollerar om nollf�rtecken tidigare hittats
					getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
				}
				r_pos += strlen(line2) + 1; //R�knar hur m�nga tecken som har l�sts och anv�nds f�r att veta var n�sta inl�sning ska ske i str�ngen

				if(j < cards_int_data_count){
					intdata[j] = std::atoi(line2);
				}
				else{
					strncpy(namn, line2, max_card_length-1);
				}
			}
			
			cards[i] = new Card(intdata[0], (!((bool)intdata[1])),intdata[2], intdata[3], intdata[4], namn);
			

			endfile = false;
		
		}
}

void seperate_cards(Card** cards, Card** cards_1, Card** cards_2, int n1, int n2, int n){
	for(int i = 0; i < n; i++){
		if(i < n1)
			cards_1[i] = cards[i];
		else if((i - n1) < n2)
			cards_2[i - n1] = cards[i];
	}
}

void do_action(Chance *card_pile, int id_card, Player* c_player,Player** players, int n_players, Property **tomter, Question *buy_street_Q, int &ID_card, Chance* chans, Chance* allmaning, int dice_1, int dice_2){
	int action, action_sum_1, action_sum_2;
	Card** temp_cards = (card_pile->get_cards());
	
	for(int i = 0; i < (ant_cards / 2); i++){ //Search for right card and copy action_values
		if(temp_cards[i]->get_id() == id_card){
			action = temp_cards[i]->get_action();
			action_sum_1 = temp_cards[i]->get_action_s1();
			action_sum_2 = temp_cards[i]->get_action_s2();
		}
	}
	int p_pos;

	switch (action){
		case 1: //Move player to position
			c_player->set_pos_ruta(action_sum_1);
			c_player->update_Player(tomter, players, n_players);
			after_movement(c_player, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning);
			break;
		case 2://Move a certain amount of positions forward or backwords
			c_player->move_Player(action_sum_1, tomter, players, n_players);
			after_movement(c_player, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning);
			break;
		case 3: //Bank pays you or take a fee
			c_player->recieve_money(action_sum_1);
			break;
		case 4: //Moved to neares uttility
			p_pos = c_player->get_pos_ruta();
				
			for(int i = p_pos; i < ant_rutor; i++){
				if(tomter[i]->get_typ() == TOMT){
					if(((Street**)tomter)[i]->get_zon() == 5 ){ //IF belonges to group for uttility
						c_player->set_pos_ruta(i);
						c_player->update_Player(tomter, players, n_players);
						after_movement(c_player, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning);
					}
				}
			}
			break;
		case 5: //Move to nearest railroad and pay double rent
			p_pos = c_player->get_pos_ruta();

			for(int i = p_pos; i < ant_rutor; i++){
				if(tomter[i]->get_typ() == TOMT){
					if(((Street**)tomter)[i]->get_zon() == 5 ){ //IF belonges to group for railroad
						c_player->set_pos_ruta(i);
						c_player->update_Player(tomter, players, n_players);
						if((((Street**)tomter)[i])->get_Owner() != c_player && (((Street**)tomter)[i])->get_Owner() != 0){ //Pay rent for railroad, will pay again later(thereby doubling the rent)
							(((Street**)tomter)[i])->pay_rent(c_player, tomter);
						}
						after_movement(c_player, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning);
					}
				}
			}
			break;
		case 6:

			break;
	}
	

}

void after_movement(Player* c_player, Question* buy_street_Q, Property **tomter, int dice_1, int dice_2, int &ID_card, Chance *chans, Chance *allmaning){
	if(c_player->get_flag_passed_go()){ //if passed go
		((Tax*)tomter[0])->pay(c_player, tomter);
		c_player->set_flag_passed_go(false); //Resets flags
	}
						
	if((tomter[c_player->get_pos_ruta()]->get_typ() == TOMT)){
		if((tomter[c_player->get_pos_ruta()]->get_Owner()) == 0){ //Om en tomt �r �gd av banken
			buy_street_Q->set_active(true);
		}
		else if((tomter[c_player->get_pos_ruta()]->get_Owner()) != c_player && (tomter[c_player->get_pos_ruta()]->get_Owner()) != 0){ //Om tomten �r en gata och inte �r �gd av dig eller banken
			if(c_player->get_pos_ruta() == 12 || c_player->get_pos_ruta() == 28){ //Om det är en utility(el och vatten)
				(((Street*)tomter[c_player->get_pos_ruta()])->pay_rent(c_player, tomter, dice_1 + dice_2));
			}
			else									
				((Street*)tomter[c_player->get_pos_ruta()])->pay_rent(c_player, tomter);
		}
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == SKATT){ //OM skatt
		if(c_player->get_pos_ruta() == 4){
			((Tax*)tomter[c_player->get_pos_ruta()])->set_question_active(true);
		}
		else
			((Tax*)tomter[c_player->get_pos_ruta()])->pay(c_player, tomter);
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == CHANS){
		ID_card = chans->pick_card();
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == ALLMANING){
		ID_card = allmaning->pick_card();
	}
}