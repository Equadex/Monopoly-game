#include "Constants.h"

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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
#include "Prison.h"
#include "Trade.h"


//Globala variabler lokala

const int n_max_audio_samples = 1;

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
const int max_rolled_in_row_dices = 3;
const int ant_buttons = 8;
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
void do_action(Chance *card_pile, int id_card, Player* c_player,Player** players, int n_players, Property **tomter, Question *buy_street_Q, int &ID_card, Chance* chans, Chance* allmaning, int dice_1, int dice_2, Prison* prison, bool &dice_used);
void after_movement(Player* c_player, Player** players, int n_players, Question* buy_street_Q, Property **tomter, int dice_1, int dice_2, int &ID_card, Chance *chans, Chance *allmaning, Prison *prison, bool &dice_used, bool pay_double = false);
void show_streets_zones();
void show_error_message(char* message);
bool roll_dices(int &dice_1, int &dice_2, Sprite *dice_sprite_0,Sprite *dice_sprite_1, int &dice_rolled_times, ALLEGRO_SAMPLE_INSTANCE *dice_roll_instance);
int get_player_total_assets(Property** tomter, Player* player_in);


int main(int argc, char *argv[]){
	//Konstanta variabler i main

	//Variabler som beh�vs f�r hantera allegro
	int window_width;
	int	window_height;
	int width = 1280;
	int height = 1000;
	double FPS = 60;
	float mouse_pos_x = 0;
	float mouse_pos_y = 0;
	int frames = 0, gameFPS = 0;
	float gameTime = 0, animationTime = 0;
	double sx, sy, scale, scaleW, scaleH, scaleX, scaleY;

	bool done = false;
	bool draw = false;
	bool OpenGL = false;

	//Keys

	bool roll_dice_key = false;
	bool end_turn_key = false;

	//Variabler n�dv�ndiga f�r spelet

	enum typ{TOMT, SKATT, CHANS, ALLMANING};
	int n_players = 6, current_player = 0;
	int dice_1, dice_2;
	int ID_button_pressed, ID_card;
	int c_player_color[3];
	int tot_free_ant_houses = 32;
	int tot_free_ant_hotels = 12;
	int dice_rolled_times = 0;

	bool dice_used = false;
	bool house_buy = false, draw_street_active = false;
	bool house_sell = false;
	bool sell_street = false;
	bool mortage_street = false;
	bool un_mortage_street = false;
	bool dice_manual_input = false;
	bool player_won = false;
	bool start_screen = true;
	bool intro = true;
	bool debug = false;
	bool player_lost_recent = false;
	bool player_changed_recently = true;
	int draw_street[ant_rutor];
	int n_draw_street;
	int intro_frames = 0, intro_sec = 2;

	//Read command line options

	if(argc > 1){
		for(int i = 0; i < argc; i++){
			if(strcmp(argv[i], "-players") == 0){
				if(i + 1 < argc){
					n_players = std::atoi(argv[i + 1]);
					if(n_players <= 0)
						n_players = 1;
					else if(n_players > max_players)
						n_players = max_players;
				}
			}
			else if(strcmp(argv[i], "-FPS") == 0){
				if(i + 1 < argc){
					FPS = std::atoi(argv[i + 1]);
				}
			}
			else if(strcmp(argv[i], "-OpenGL") == 0){
				if(i + 1 < argc){
					OpenGL = std::atoi(argv[i + 1]);
				}
			}
			else if(strcmp(argv[i], "-skip_intro") == 0){
				intro = false;
			}
			else if(strcmp(argv[i], "-set_intro_time") == 0){
				intro_sec = std::atoi(argv[i + 1]);
			}
			else if(strcmp(argv[i], "-skip_all") == 0){
				intro = false;
				start_screen = false;
			}
			else if(strcmp(argv[i], "-debug") == 0){
				debug = std::atoi(argv[i + 1]);
			}
		}
	}

	Property *tomter[ant_rutor]; //F�lt av tomter
	Player *players[max_players]; //F�lt av spelare
	Button *buttons[ant_buttons]; //F�lt av buttons
	Card *cards[ant_cards], *c_cards[ant_cards / 2], *a_cards[ant_cards / 2]; //Fält av kort
	Sprite *dice_sprite_0;
	Sprite *dice_sprite_1;
	Chance *chans, *allmaning;
	Prison *prison;
	Trade *trade;

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
	
	//Allegro Samples

	ALLEGRO_SAMPLE *monopoly_song = NULL;
	ALLEGRO_SAMPLE *dice_roll = NULL;
	ALLEGRO_SAMPLE *win = NULL;
	ALLEGRO_SAMPLE *police = NULL;
	ALLEGRO_SAMPLE *alert = NULL;

	ALLEGRO_SAMPLE_INSTANCE *start_screen_song = NULL;
	ALLEGRO_SAMPLE_INSTANCE *dice_roll_instance = NULL;
	ALLEGRO_SAMPLE_INSTANCE *win_instance = NULL;
	ALLEGRO_SAMPLE_INSTANCE *police_instance = NULL;
	ALLEGRO_SAMPLE_INSTANCE *alert_instance = NULL;

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
	ALLEGRO_BITMAP *trade_proposal = NULL;
	ALLEGRO_BITMAP *trade_reciver = NULL;
	ALLEGRO_BITMAP *allegro_logo = NULL;
	ALLEGRO_BITMAP *allegro_logo2 = NULL;
	ALLEGRO_BITMAP *allegro_logo3 = NULL;
	ALLEGRO_BITMAP *background = NULL;

	if(!al_init()){ //Initierar allegro bibloteket
		show_error_message("Failed to initilize Allegro");
		return (-1);
	}

	//Tar reda p� sk�rmens storlek och omskalninsf�rh�llanden
	if(!al_get_monitor_info(0, p_info)){
		show_error_message("Failed to get screen info");
		return(-1);
	}
	window_width = p_info->x2; 
	window_height = p_info->y2 - 40;

	sx = window_width / (double)width;
	sy = window_height / (double)height;
	scale = std::min <double>(sx, sy);

	scaleW = width * scale;
	scaleH = height * scale;
	scaleX = (window_width - scaleW) / 2;
	scaleY = (window_height - scaleH) / 2;



	//Skapar och testar display
	//al_set_new_display_flags(ALLEGRO_WINDOWED);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	if(OpenGL)
		al_set_new_display_flags(ALLEGRO_OPENGL);
	display = al_create_display(window_width, window_height);
	if(!display){
		show_error_message("Failed to initilize Display");
		return (-1);
	}


	//Skapar timer
	timer = al_create_timer(1/FPS);

	//init addons
	al_init_image_addon();
	al_install_mouse();
	al_install_keyboard();
	al_install_audio();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	
	if(!al_reserve_samples(n_max_audio_samples))
		return -1;
	srand(time(NULL));

	//Laddar samples

	monopoly_song = al_load_sample("(Come on) Let's Play Monopoly.wav");
	dice_roll = al_load_sample("dice_roll_edit.wav");
	win = al_load_sample("won.wav");
	police = al_load_sample("police.wav");
	alert = al_load_sample("alert.wav");

	start_screen_song = al_create_sample_instance(monopoly_song);
	dice_roll_instance = al_create_sample_instance(dice_roll);
	win_instance = al_create_sample_instance(win);
	police_instance = al_create_sample_instance(police);
	al_set_sample_instance_playmode(start_screen_song, ALLEGRO_PLAYMODE_LOOP);
	alert_instance = al_create_sample_instance(alert);

	al_attach_sample_instance_to_mixer(start_screen_song, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(dice_roll_instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(win_instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(police_instance, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(alert_instance, al_get_default_mixer());

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
	trade_proposal = al_load_bitmap("monopoly_trade_concept.png");
	trade_reciver = al_load_bitmap("monopoly_trade_concept_reciever.png");
	allegro_logo = al_load_bitmap("cooltextUtopia128.png");
	allegro_logo2 = al_load_bitmap("allegro_logo.png");
	allegro_logo3 = al_load_bitmap("allegro_logo3.png");
	background = al_load_bitmap("pixel77-free-vector-metal-pattern-[Konvert].jpg");

	al_convert_mask_to_alpha(allegro_logo, al_map_rgb(255, 255, 255));
	al_convert_mask_to_alpha(allegro_logo2, al_map_rgb(255, 255, 255));
	//Skapar fonts

	ALLEGRO_FONT *arial_16 = al_load_ttf_font("arial.ttf", 16, 0);
	ALLEGRO_FONT *arial_36 = al_load_ttf_font("arial.ttf", 36, 0);
	ALLEGRO_FONT *arial_20 = al_load_ttf_font("arial.ttf", 20, 0);
	ALLEGRO_FONT *arial_10 = al_load_ttf_font("arial.ttf", 12, 0); 
	ALLEGRO_FONT *winner_font = al_load_ttf_font("arial.ttf", 36, 0);
	ALLEGRO_FONT *start_screen_font = al_load_ttf_font("arial.ttf", 112, 0);

	//Temp kod som inte borde se ut s�h�r
	
	//Street_info *test = new Street_info(0, 0, (Street*)tomter[0], street_info, false); //Test, ska tas bort
	create_Street_info(tomter, street_info, street_info_railroad, street_info_el, street_info_water, arial_16, arial_20, arial_10);

	dice_sprite_0 = new Sprite(1091, 6, 81, dice);
	dice_sprite_1 = new Sprite(1172, 6, 81, dice);
	
	chans = new Chance(c_cards, question, button);
	allmaning = new Chance(a_cards, question, button);


	Button *temp[2];
	Button *button_defeat_window[1];
	button_defeat_window[0] = new Button(Question_pos_x_standard + (al_get_bitmap_width(question) / 2) - (al_get_bitmap_width(button) / 2), Question_pos_y_standard + 250, Question_pos_x_standard + (al_get_bitmap_width(question) / 2) - (al_get_bitmap_width(button) / 2) + al_get_bitmap_width(button),  Question_pos_y_standard + 250 + al_get_bitmap_height(button), 1, "OK", button); 
	temp[0] = new Button(165 + 162, 275 + 250, 165 + 162 + 80, 275 + 250 + 25, 1, "Buy", button);
	temp[1] = new Button(600 - 162 + 80, 275 + 250, 600, 275 + 250 + 25, 2, "Auction", button);
	Question *buy_street_Q = new Question(Question_pos_x_standard, Question_pos_y_standard, temp, 2, "Buy or auction?", "This property is owned by the bank and is for sale. Do you want to buy it or let it be sold by auction?", question);
	Auction *auction = new Auction(0, 120, players, n_players, auction_image, button, box, arial_36, arial_16);
	((Tax*)tomter[4])->create_income_tax_question(Question_pos_x_standard, Question_pos_y_standard, button, question); //Skapar fråga till inkomst skatt ruta
	prison = new Prison(button, question, police_instance);
	trade = new Trade(0, 120, tomter, arial_16, arial_36, trade_proposal, trade_reciver, button, box);
	Question defeat_window(Question_pos_x_standard, Question_pos_y_standard, button_defeat_window, 1, "Insufficient money", "You don't have enough money to pay. Money needed: %i. Money available: %i.\n Make sure your amount of money meet the requirement before pressing ok, otherwise you will lose. By losing all your remaining money and property will be transferred to the player or bank you owe money and you can’t continue to play.", question);  
	for(int i = 0; i < n_players; i++){
		players[i]->set_defeat_window(defeat_window);
	}
	defeat_window.set_orignal_message();

	//Skapar event_queue, registrerar k�llor och startar timer
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);
	gameTime = al_get_time();
	animationTime = al_get_time();

	//Spelloop
	while(!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){  //L�ser av koordinater fr�n musen
			mouse_pos_x = (ev.mouse.x * 1 - scaleX) / (double)scale;
			mouse_pos_y = (ev.mouse.y * 1 - scaleY) / (double)scale;

		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || roll_dice_key || end_turn_key && !start_screen){ //N�r musknapp �r nedtryckt
			if(ev.mouse.button == 1 || roll_dice_key || end_turn_key){ //V�nster musknapp
				
				if(roll_dice_key){
						roll_dice_key = false;
						mouse_pos_x = 1000; mouse_pos_y = 50;
					}
					else if(end_turn_key){
						end_turn_key = false;
						mouse_pos_x = 1120; mouse_pos_y = 870;
					}

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
								((Tax*)tomter[4])->pay(players[current_player]);
								break;
							case 2:
								((Tax*)tomter[4])->pay_fee(players[current_player], tomter);
								break;
						}
						((Tax*)tomter[4])->set_question_active(false);
					}
				}
				else if(chans->get_window()->button_pressed(mouse_pos_x, mouse_pos_y)){
					mouse_pos_x = 0;
					chans->get_window()->set_active(false);
					do_action(chans, ID_card, players[current_player], players, n_players, tomter, buy_street_Q, ID_card, chans, allmaning, dice_1, dice_2, prison, dice_used);
				}
				else if(allmaning->get_window()->button_pressed(mouse_pos_x, mouse_pos_y)){
					mouse_pos_x = 0;
					allmaning->get_window()->set_active(false);
					do_action(allmaning, ID_card, players[current_player], players, n_players, tomter, buy_street_Q, ID_card, chans, allmaning, dice_1, dice_2, prison, dice_used);
				}
				else if(prison->window_activated()){
					int button_id = prison->button_pressed(mouse_pos_x, mouse_pos_y);
					mouse_pos_x = 0;
					if(button_id != 0){
						switch(button_id){
						case 1:
							if(prison->get_players_dice_tries(players[current_player]->get_id()) < max_rolled_in_row_dices){ //OM spelaren har slått mindre än 3 slag
								if(!roll_dices(dice_1, dice_2, dice_sprite_0, dice_sprite_1, dice_rolled_times, dice_roll_instance)){
									prison->send_to_prison(players[current_player]);
								}
								dice_used = true;
								prison->register_dice_try(players[current_player]->get_id());
								prison->activate_window(false);

								if(dice_1 == dice_2){
									prison->release_player(players[current_player]);
									players[current_player]->move_Player(dice_1 + dice_2, tomter, players, n_players);
									after_movement(players[current_player], players, n_players, buy_street_Q, tomter, dice_1, dice_2, ID_card, chans, allmaning, prison, dice_used);
									dice_used = false;
								}
							}
							else{
								prison->out_of_dice_tries();//meddelande om att måste välja något annat
							}
							break;
						case 2:
							if(!players[current_player]->pay(prison_fee) && prison->get_players_dice_tries(players[current_player]->get_id()) >= max_rolled_in_row_dices && (players[current_player]->get_flag_c_jail_card() || players[current_player]->get_flag_a_jail_card())){ //Pay fee, if can't and can't thow dices or use card, defeat
								players[current_player]->defeated(0, prison_fee);
							}
							else{
								prison->release_player(players[current_player]);
							}
							break;
						case 3:
							if(players[current_player]->get_flag_c_jail_card() || players[current_player]->get_flag_a_jail_card()){
								if(players[current_player]->get_flag_c_jail_card()){
									players[current_player]->set_flag_c_jail_card(false);
									chans->return_chance_card();
								}
								else{
									players[current_player]->set_flag_a_jail_card(false);
									allmaning->return_chance_card();
								}
								prison->release_player(players[current_player]);
							}
							break;
						}
					}
				}
				else if(trade->get_active())
					trade->pressed(mouse_pos_x, mouse_pos_y, players, n_players);
				else{
					for(int i = 0; i < ant_buttons; i++){ //Kontrollerar vilken knapp som blivit klickad
						if(buttons[i]->Button_pressed(mouse_pos_x, mouse_pos_y)){
							ID_button_pressed = buttons[i]->get_ID();
						}
					}
					for(int i = 0; i < ant_rutor; i++){
						if(tomter[i]->property_button_pressed(mouse_pos_x, mouse_pos_y)){ //Kontrollerar om några gator blivit klickade
							if(tomter[i]->get_typ() == TOMT && house_buy){ //OM husköp
								((Street*)tomter[i])->buy_house(players[current_player], tomter, tot_free_ant_houses, tot_free_ant_hotels);
							}
							else if(tomter[i]->get_typ() == TOMT && house_sell){ //OM husköp
								((Street*)tomter[i])->sell_house(players[current_player], tomter, tot_free_ant_houses, tot_free_ant_hotels);
							}
							else if(tomter[i]->get_typ() == TOMT && sell_street){ //OM sälj gata
								((Street*)tomter[i])->sell_Street(players[current_player], tomter, tot_free_ant_houses, tot_free_ant_hotels);
							}
							else if(tomter[i]->get_typ() == TOMT && mortage_street){//OM inteckna gata
								((Street*)tomter[i])->mortage_street(true, tomter, tot_free_ant_houses, tot_free_ant_hotels);
							}
							else if(tomter[i]->get_typ() == TOMT && un_mortage_street){
								((Street*)tomter[i])->mortage_street(false, tomter, tot_free_ant_houses, tot_free_ant_hotels);
							}
						}
					}
				
					switch(ID_button_pressed){
						case 1: //Sl� t�rningarna
							if(!dice_used){
								int temp[max_players];

								if(!dice_manual_input){
									if(!roll_dices(dice_1, dice_2, dice_sprite_0, dice_sprite_1, dice_rolled_times, dice_roll_instance)){
										prison->send_to_prison(players[current_player]);
									}
								}
								else{
									dice_manual_input = false;
									dice_sprite_0->set_curret_frame(max_tarning - (dice_1)); //Byter bild p� t�rning
									dice_sprite_1->set_curret_frame(max_tarning - (dice_2));
								}

								

								if(dice_1 != dice_2 || players[current_player]->get_flag_prisoned())
									dice_used = true;
								else
									dice_used = false;

								if(!players[current_player]->get_flag_prisoned()){
									players[current_player]->move_Player(dice_1 + dice_2, tomter, players, n_players); //Flyttar spelare
									after_movement(players[current_player], players, n_players, buy_street_Q, tomter, dice_1, dice_2, ID_card, chans, allmaning, prison, dice_used);
								}
								else
									players[current_player]->update_Player(tomter, players, n_players);
								

							}
							break;
						case 2: //Sälja gata
							if(!sell_street)
								sell_street = true;
							else
								sell_street = false;
							
							break;
						case 3:
							if(dice_used){
								if(!player_lost_recent){
									current_player++; //N�sta spelare
									if(current_player > n_players)
										current_player = 0;
									current_player %= n_players;
								}
								else{
									player_lost_recent = false;
									if(current_player >= n_players)
										current_player = 0;
								}

								dice_used = false;
								draw_street_active = false;
								house_sell = false;
								house_buy = false;
								mortage_street = false;
								un_mortage_street = false;
								sell_street = false;
								dice_rolled_times = 0;

								players[current_player]->get_color(c_player_color); //Färg för nuvarrande spelare
								if(players[current_player]->get_flag_prisoned()){
									prison->activate_window(true);
								}
								if(n_players <= 1){
									player_won = true;
									al_play_sample_instance(win_instance);
								}
								player_changed_recently = true;
								al_play_sample_instance(alert_instance);
								animationTime = al_get_time();
							}
							break;
						case 4: //Köpa hus
							if(!house_buy && !house_sell){
								draw_street_active = true;
								house_buy = true;
							}
							else if(house_sell){
								house_sell = false;
								house_buy = true;
							}
							else{
								draw_street_active = false;
								house_buy = false;
							}
							break;
						case 5: //Sälj hus
							if(!house_sell && !house_buy){
								draw_street_active = true;
								house_sell = true;
							}
							else if(house_buy){
								house_buy = false;
								house_sell = true;
							}
							else{
								draw_street_active = false;
								house_sell = false;
							}
							break;
						case 6:
							if(!mortage_street && !un_mortage_street)
								mortage_street = true;
							else
								mortage_street = false;
							break;
						case 7:
							if(!un_mortage_street && !mortage_street)
								un_mortage_street = true;
							else
								un_mortage_street = false;
							break;
						case 8:
							trade->update_trade_buttons(players, n_players);
							trade->set_active(true, n_players);
							trade->set_buyer(players[current_player]);
							break;
							
					}

					//Check if a status box has been pressed

					/*for(int i = 0; i < ant_rutor; i++){

					}*/

					ID_button_pressed = 0;
				}
			}

			//If defeat window is active and no other windows is
			if(defeat_window.get_active() && !prison->window_activated() && !trade->get_active() && !allmaning->get_window()->button_pressed(mouse_pos_x, mouse_pos_y) && !chans->get_window()->button_pressed(mouse_pos_x, mouse_pos_y) && !((Tax*)tomter[4])->get_question()->get_active() && !auction->get_active() && !buy_street_Q->get_active()){
				int button_id = defeat_window.button_pressed(mouse_pos_x, mouse_pos_y);
				switch(button_id){
					case 1:
						if(players[current_player]->player_lost(tomter)){
							for(int i = 0; i < ant_rutor; i++){
								if(tomter[i]->get_typ() == TOMT){
									((Street*)(tomter[i]))->update_status_box();
								}
							}
						
							delete players[current_player];
							for(int i = current_player; i < (n_players - 1); i++){
								players[i] = players[i + 1];
							}
							n_players--;
							player_lost_recent = true;
							players[n_players] = 0;
						}
						defeat_window.set_active(false);
						dice_used = true;
						break;
				}
			}
			else if(ev.mouse.button == 2){
				if(auction->get_active())
					auction->button_pressed(mouse_pos_x, mouse_pos_y, true);
				else if(trade->get_active())
					trade->pressed(mouse_pos_x, mouse_pos_y, players, n_players, true);
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode){
				case ALLEGRO_KEY_ESCAPE:
					done = true;
					break;
				case ALLEGRO_KEY_R:
					if(!start_screen)
						roll_dice_key = true;
					break;
				case ALLEGRO_KEY_E:
					if(!start_screen)
						end_turn_key = true;
					break;
				case ALLEGRO_KEY_SPACE:
					start_screen = false;
					if(!start_screen)
						al_stop_sample_instance(start_screen_song);
					animationTime = al_get_time();
					break;
			}
			if(debug && ev.keyboard.keycode >= ALLEGRO_KEY_0 && ev.keyboard.keycode <= ALLEGRO_KEY_9){
				if(!dice_manual_input)
					dice_1 = ev.keyboard.keycode - ALLEGRO_KEY_0;
				else
					dice_2 = ev.keyboard.keycode - ALLEGRO_KEY_0;
				dice_manual_input = true;
				
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_TIMER){
			draw = true;
			
		}

		if(draw && al_is_event_queue_empty(event_queue)){

			frames++; //R�knar frames
			if(al_get_time() - gameTime >= 1){ //Uppdaterar fps efter 1 sekund 
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}

			al_set_target_bitmap(buffer);
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if(intro){
				if(animationTime + intro_sec < al_get_time()){
					intro = false;
				}
				al_draw_text(winner_font, al_map_rgb(255, 255, 255), 640, 200, ALLEGRO_ALIGN_CENTRE, "Game made by Anders Pehrsson");
				al_draw_text(winner_font, al_map_rgb(255, 255, 255), 640 - (al_get_bitmap_width(allegro_logo3) / 2) , 520, ALLEGRO_ALIGN_RIGHT, "Powered by");
				al_draw_bitmap_region(allegro_logo3, 0, 0, al_get_bitmap_width(allegro_logo3), 120, 640 - (al_get_bitmap_width(allegro_logo3) / 2), 500, 0);
				intro_frames++;
			}
			else if(start_screen){
				if(!al_get_sample_instance_playing(start_screen_song)){
					al_play_sample_instance(start_screen_song);
				}
				al_draw_bitmap(background, 0, 0, 0);
				al_draw_filled_rectangle(220, 130, 1060, 265, al_map_rgb(255, 0, 0));
				al_draw_text(start_screen_font, al_map_rgb(255, 255, 255), 640, 130, ALLEGRO_ALIGN_CENTRE, "2d-MONOPOLY");
				al_draw_text(winner_font, al_map_rgb(255, 255, 255), 600, 640, ALLEGRO_ALIGN_CENTRE, "Press space to start the game");
				//al_draw_text(start_screen_font, al_map_rgb(
			}
			else if(player_won){
				al_draw_textf(winner_font, al_map_rgb(255, 255, 0), 640, 500, ALLEGRO_ALIGN_CENTRE, "Player %i have won the game with %i in total assets!", players[current_player]->get_id(), get_player_total_assets(tomter, players[current_player]));
				al_draw_textf(winner_font, al_map_rgb(255, 255, 0), 640, 230, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
			}
			else{

			n_draw_street = 0;

			//Drawing
			
			al_draw_bitmap(spelplan, 0, 0, 0);
			al_draw_bitmap(spelbrade, 0, 0, 0);
			for(int i = 0; i < ant_rutor && draw_street_active; i++){//Kontrollerar vilka zoner som ägs av nuvarande spelare
				if(tomter[i]->get_typ() == TOMT && ((Street*)tomter[i])->own_zone(tomter, players[current_player])){
					draw_street[n_draw_street] = ((Street*)tomter[i])->get_pos_ruta();
					n_draw_street++;
				}
			}

			for(int i = 0, j = 0; i < ant_rutor; i++){ //Ritar Street_info och markerar gator för köp/sälj
				if(tomter[i]->get_typ()  == TOMT){
					//Ritar street_info
					if((((Street*)tomter[i])->get_Street_info())->get_active())
						(((Street*)tomter[i])->get_Street_info())->draw();

						//Ritar eventuella markeringar

					if(sell_street && ((Street*)tomter[i])->get_Owner() == players[current_player] && ((Street*)tomter[i])->get_mortaged() == false && ((Street*)tomter[i])->undeveloped_zone(tomter)){
						((Street*)tomter[i])->draw(true);
					}
					else if(mortage_street && ((Street*)tomter[i])->get_Owner() == players[current_player] && ((Street*)tomter[i])->get_mortaged() == false){
						((Street*)tomter[i])->draw(true);
					}
					else if(un_mortage_street && ((Street*)tomter[i])->get_Owner() == players[current_player] && ((Street*)tomter[i])->get_mortaged() == true){
						((Street*)tomter[i])->draw(true);
					}

					for(int j = 0; j < n_draw_street && draw_street_active; j++){ //Kontrollerar om gatan är med i en zon som ska ritas
						if(((Street*)tomter[i])->get_pos_ruta() == draw_street[j]){
							if(house_buy && ((Street*)tomter[i])->get_zon() != 1 && ((Street*)tomter[i])->get_zon() != 5) //om köpa hus, inte visa järnväg eller uttilies
								((Street*)tomter[i])->draw(true); //Ritar gata
							else if(house_sell && ((Street*)tomter[i])->get_zon() != 1 && ((Street*)tomter[i])->get_zon() != 5 && ((Street*)tomter[i])->get_ant_houses() > 0){
								((Street*)tomter[i])->draw(true); //Ritar gata
							}
							else //, om inte köpa hus, rita allt
								((Street*)tomter[i])->draw(false);
						}
					}
					((Street*)tomter[i])->draw(false);
					
				}
			}
			for(int i = 0; i < n_players; i++){ //Ritar spelare
				players[i]->draw_player();
			}
			for(int i = 0; i < ant_rutor; i++){ //Ritar status_boxar
				if(tomter[i]->get_typ() == 0)
					((Street*)tomter[i])->draw_status();
			}
			dice_sprite_0->draw(); //Ritar t�rningar
			dice_sprite_1->draw();
			if(defeat_window.get_active()){
				defeat_window.draw(arial_36, arial_16);
			}
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
			else if(prison->window_activated()){
				prison->draw_window(arial_36, arial_16);
			}
			else{
				trade->draw();
			}

			for(int i = 0; i < ant_buttons; i++){ //Ritar knappar
				buttons[i]->draw(arial_16);
			}

			//al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);
			if(players[current_player] != 0){
				al_draw_textf(arial_16, al_map_rgb(c_player_color[0], c_player_color[1] , c_player_color[2]), 15, 940, 0, "Player %i", players[current_player]->get_id());
				al_draw_textf(arial_16, al_map_rgb(0, 0, 0), 100, 940, 0, "Funds: %i", (players[current_player])->get_money());
			}
			//al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 20, 0, "Mouse_x: %lf Mouse_y: %lf", mouse_pos_x, mouse_pos_y);
			if(player_changed_recently && animationTime + 1.5 > al_get_time()){
				al_draw_textf(start_screen_font, al_map_rgb(c_player_color[0], c_player_color[1], c_player_color[2]), 440, 160, ALLEGRO_ALIGN_CENTRE, "Player %i", players[current_player]->get_id());
			}
			else if(animationTime + 1.5 <= al_get_time())
				player_changed_recently = false;
			
			}
			//Skalar om bilden och ritar till backbuffern. Vänder sedan på buffern
			
			al_set_target_backbuffer(display);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_scaled_bitmap(buffer, 0, 0, width, height, scaleX, scaleY, scaleW , scaleH, 0);
			if(debug){
				al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 20, 0, "Mouse_x: %lf Mouse_y: %lf", mouse_pos_x, mouse_pos_y);
				al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);
			}

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
	al_destroy_bitmap(button);
	al_destroy_bitmap(buffer);
	al_destroy_bitmap(auction_image);
	al_destroy_bitmap(box);
	al_destroy_bitmap(street_info);
	al_destroy_bitmap(street_info_railroad);
	al_destroy_bitmap(street_info_el);
	al_destroy_bitmap(street_info_water);
	al_destroy_bitmap(trade_proposal);
	al_destroy_bitmap(trade_reciver);
	al_destroy_bitmap(allegro_logo);
	al_destroy_bitmap(allegro_logo2);
	al_destroy_bitmap(allegro_logo3);
	al_destroy_bitmap(background);
	al_destroy_sample(monopoly_song);
	al_destroy_sample(dice_roll);
	al_destroy_sample(win);
	al_destroy_sample(police);
	al_destroy_font(arial_36);
	al_destroy_font(arial_20);
	al_destroy_font(arial_10);
	al_destroy_font(winner_font);
	al_destroy_font(start_screen_font);
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
		show_error_message("Function getline_char has been called with a too big incoming array. Please debug program to fix this. It's recommended to terminate the program but you can continue on your own risk");
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
	const int displacement_y = 36;

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
					getline_char(line, line2, max_config_extended_length,max_config_extended_length, ',', endfile, r_pos);
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

void do_action(Chance *card_pile, int id_card, Player* c_player,Player** players, int n_players, Property **tomter, Question *buy_street_Q, int &ID_card, Chance* chans, Chance* allmaning, int dice_1, int dice_2, Prison* prison, bool &dice_used){
	int action, action_sum_1, action_sum_2;
	Card** temp_cards = (card_pile->get_cards());
	
	for(int i = 0; i < (ant_cards / 2); i++){ //Search for right card and copy action_values
		if(temp_cards[i]->get_id() == id_card){
			action = temp_cards[i]->get_action();
			action_sum_1 = temp_cards[i]->get_action_s1();
			action_sum_2 = temp_cards[i]->get_action_s2();
			break;
		}
	}
	int p_pos;

	switch (action){
		case 1: //Move player to position
			if(c_player->get_pos_ruta() > action_sum_1)
				c_player->passing_go_check(ant_rutor + 1);
			c_player->set_pos_ruta(action_sum_1);
			c_player->update_Player(tomter, players, n_players);
			after_movement(c_player, players, n_players, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning, prison, dice_used);
			break;
		case 2://Move a certain amount of positions forward or backwords
			c_player->move_Player(action_sum_1, tomter, players, n_players);
			after_movement(c_player, players, n_players, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning, prison, dice_used);
			break;
		case 3: //Bank pays you or take a fee
			c_player->recieve_money(action_sum_1);
			break;
		case 4: //Moved to nearest uttility
			{
			p_pos = c_player->get_pos_ruta();
			bool loop_done = false;
			for(int i = p_pos; !loop_done; i++){
				if(i >= ant_rutor)
					i = 0;
				if(tomter[i]->get_typ() == TOMT){
					if(((Street**)tomter)[i]->get_zon() == 5 ){ //IF belonges to group for uttility
						//check for go
						if(i < c_player->get_pos_ruta())
							c_player->set_flag_passed_go(true);

						c_player->set_pos_ruta(i);
						c_player->update_Player(tomter, players, n_players);
						after_movement(c_player, players, n_players, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning, prison, dice_used, true);
						loop_done = true;
					}
				}
			}
			break;
			}
			
		case 5: //Move to nearest railroad and pay double rent
			{
			p_pos = c_player->get_pos_ruta();

			bool loop_done = false;
			for(int i = p_pos; !loop_done; i++){
				if(i >= ant_rutor)
					i = 0;
				if(tomter[i]->get_typ() == TOMT){
					if(((Street**)tomter)[i]->get_zon() == 1 ){ //IF belonges to group for railroad
						//check for go
						if(i < c_player->get_pos_ruta())
							c_player->set_flag_passed_go(true);

						c_player->set_pos_ruta(i);
						c_player->update_Player(tomter, players, n_players);
						if((((Street**)tomter)[i])->get_Owner() != c_player && (((Street**)tomter)[i])->get_Owner() != 0){ //Pay rent for railroad, will pay again later(thereby doubling the rent)
							(((Street**)tomter)[i])->pay_rent(c_player, tomter);
						}
						after_movement(c_player, players, n_players, buy_street_Q , tomter, dice_1, dice_2, ID_card, chans, allmaning, prison, dice_used, true);
						loop_done = true;
					}
				}
			}
			break;
			}
		case 6:
			for(int i = 0; i < n_players; i++){
				players[i]->pay_player(c_player, action_sum_1);
			}
			break;
		case 7:
			{
				int houses_to_pay = 0, hotels_to_pay = 0;
				for(int i = 0; i < ant_rutor; i++){ //Finds out how many hotels and houses a player has
					if(tomter[i]->get_typ() == TOMT && tomter[i]->get_Owner() == c_player){
						if(((Street*)tomter[i])->get_ant_houses() == max_houses)
							hotels_to_pay++;
						else if(((Street*)tomter[i])->get_ant_houses() < max_houses - 1 && ((Street*)tomter[i])->get_ant_houses() > 0)
							houses_to_pay += ((Street*)tomter[i])->get_ant_houses();
					}
				}
			if(!(c_player->pay(houses_to_pay * action_sum_1 + hotels_to_pay * action_sum_2)))
				c_player->defeated(0, houses_to_pay * action_sum_1 + hotels_to_pay * action_sum_2);
			break;
			}
		case 8:
			prison->send_to_prison(c_player);
			c_player->update_Player(tomter, players, n_players);
			dice_used = true;
			break;
		case 9:
			c_player->set_flag_c_jail_card(true);
			break;
		case 10:
			c_player->set_flag_a_jail_card(true);
			break;
	}
	

}

void after_movement(Player* c_player, Player** players, int n_players, Question* buy_street_Q, Property **tomter, int dice_1, int dice_2, int &ID_card, Chance *chans, Chance *allmaning, Prison *prison, bool &dice_used, bool pay_double){
						
	if((tomter[c_player->get_pos_ruta()]->get_typ() == TOMT)){
		if((tomter[c_player->get_pos_ruta()]->get_Owner()) == 0){ //Om en tomt �r �gd av banken
			buy_street_Q->set_active(true);
		}
		else if((tomter[c_player->get_pos_ruta()]->get_Owner()) != c_player && (tomter[c_player->get_pos_ruta()]->get_Owner()) != 0){ //Om tomten �r en gata och inte �r �gd av dig eller banken
			if(c_player->get_pos_ruta() == 12 || c_player->get_pos_ruta() == 28){ //Om det är en utility(el och vatten)
				(((Street*)tomter[c_player->get_pos_ruta()])->pay_rent(c_player, tomter, dice_1 + dice_2, pay_double));
			}
			else									
				((Street*)tomter[c_player->get_pos_ruta()])->pay_rent(c_player, tomter);
		}
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == SKATT){ //OM skatt
		if(c_player->get_pos_ruta() == 4){
			((Tax*)tomter[c_player->get_pos_ruta()])->set_question_active(true);
		}
		else if(!c_player->get_flag_passed_go())
			((Tax*)tomter[c_player->get_pos_ruta()])->pay(c_player);
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == CHANS){
		ID_card = chans->pick_card();
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == ALLMANING){
		ID_card = allmaning->pick_card();
	}
	else if((tomter[c_player->get_pos_ruta()])->get_typ() == GA_TILL_FANGELSE){
		prison->send_to_prison(c_player);
		dice_used = true;
		c_player->update_Player(tomter, players, n_players);
	}

	if(c_player->get_flag_passed_go()){ //if passed go
		((Tax*)tomter[0])->pay(c_player);
		c_player->set_flag_passed_go(false); //Resets flags
	}
}

bool roll_dices(int &dice_1, int &dice_2, Sprite *dice_sprite_0,Sprite *dice_sprite_1, int &dice_rolled_times, ALLEGRO_SAMPLE_INSTANCE *dice_roll_instance){
	if(dice_rolled_times++ < max_rolled_in_row_dices){
		roll_dice(dice_1);
		roll_dice(dice_2);
		dice_sprite_0->set_curret_frame(max_tarning - (dice_1)); //Byter bild p� t�rning
		dice_sprite_1->set_curret_frame(max_tarning - (dice_2));
		al_play_sample_instance(dice_roll_instance);
		return true;
	}
	return false;
}

void show_error_message(char* message){
	al_show_native_message_box(NULL, "ERROR", "ERROR", message, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

int get_player_total_assets(Property** tomter, Player* player_in){
	int total_funds = 0;
	total_funds += (player_in->get_money());

	for(int i = 0; i < ant_rutor; i++){ //Adding owned property
		if(tomter[i]->get_Owner() == player_in && tomter[i]->get_typ() == TOMT){
			if(((Street*)tomter[i])->get_mortaged()) //if mortaged, add mortaged value
				total_funds += ((((Street*)tomter[i])->get_cost()) / 2);
			else{
				total_funds += (((Street*)tomter[i])->get_cost());
				if(((Street*)tomter[i])->own_zone(tomter)){ //if zone is owned, check for houses and hotels
					total_funds += ((Street*)tomter[i])->get_ant_houses() * ((Street*)tomter[i])->get_building_cost();
				}
			}
		}
	}

	return total_funds;
}