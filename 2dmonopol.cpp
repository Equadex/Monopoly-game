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

#include "Player.h"
#include "Property.h"
#include "Button.h"
#include "Status_box.h"
#include "Sprite.h"
#include "Question.h"
#include "Auction.h"

//Globala variabler lokala

const int max_config_line_length = 128;
const int max_name_length = 64;
const int property_int_data_count = 15;
const int property_variable_count = property_int_data_count + 1;
const int button_int_data_count = 5;
const int button_variable_data_count = button_int_data_count + 1;
const int status_box_int_data_count = 5;
const int startpengar = 1500;
const int start_ruta = 0;
const int max_tarning = 6;
const int ant_buttons = 4;
const int ant_status_box = 28;

//Funktionsdeklartioner

void read_Property_data(Property *tomter[]);
void getline_char(char line_in[],char line_out[],int out_length, int max_read, char stop, bool &endfile, int start = 0);
void create_players(int n_players, Player *players[], Property *tomter[]);
void roll_dice(int &dice);
void read_Button_data(Button* buttons[]);
void read_Status_box_data(Property** streets);
void auction();

int main(){
	//Konstanta variabler i main

	//Variabler som behövs för hantera allegro
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

	//Variabler nödvändiga för spelet

	enum typ{TOMT, SKATT, CHANS, ALLMANING};
	int n_players = 2, current_player = 0;
	int dice_1, dice_2;
	int ID_button_pressed;

	bool dice_used = false;

	Property *tomter[ant_rutor]; //Fält av tomter
	Player *players[max_players]; //Fält av spelare
	Button *buttons[ant_buttons]; //Fält av buttons
	Sprite *dice_sprite_0;
	Sprite *dice_sprite_1;

	read_Property_data(tomter); //Läser in data till tomter
	create_players(n_players, players, tomter); //Skapar spelare
	read_Button_data(buttons); //Läser in buttons koodinater och id
	read_Status_box_data(tomter); //Läser in status boxarnas positioner och gatunummer


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

	if(!al_init()){ //Initierar allegro bibloteket
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize Allegro" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}

	//Tar reda på skärmens storlek och omskalninsförhållanden
	if(!al_get_monitor_info(0, p_info)){
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to get screen info" , NULL, ALLEGRO_MESSAGEBOX_ERROR); 
		return(-1);
	}
	window_width = p_info->x2; 
	window_height = p_info->y2 - 30;

	//window_width = 1920;
	//window_height = 1050;

	sx = window_width / (double)width;
	sy = window_height / (double)height;
	scale = std::min <double>(sx, sy);

	scaleW = width * scale;
	scaleH = height * scale;
	scaleX = (window_width - scaleW) / 2;
	scaleY = (window_height - scaleH) / 2;



	//Skapar och testar display
	//al_set_new_display_flags(ALLEGRO_WINDOWED);
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

	//Skapar fonts

	ALLEGRO_FONT *arial_16 = al_load_ttf_font("arial.ttf", 16, 0);
	ALLEGRO_FONT *arial_36 = al_load_ttf_font("arial.ttf", 36, 0);

	//Temp kod som inte borde se ut såhär

	dice_sprite_0 = new Sprite(1091, 6, 81, dice); 
	dice_sprite_1 = new Sprite(1172, 6, 81, dice);

	Button *temp[2];
	temp[0] = new Button(165 + 162, 275 + 250, 165 + 162 + 80, 275 + 250 + 25, 1, "Buy", button);
	temp[1] = new Button(600 - 162 + 80, 275 + 250, 600, 275 + 250 + 25, 2, "Auction", button);
	Question *buy_street_Q = new Question(165, 275, temp, 2, "Buy or auction?", "This property is owned by the bank and is for sale. Do you want to buy it or let it be sold by auction?", question);
	Auction *auction = new Auction(165, 275, players, n_players, auction_image, arial_36, arial_16);

	//Skapar event_queue, registrerar källor och startar timer
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
		if(ev.type == ALLEGRO_EVENT_MOUSE_AXES){  //Läser av koordinater från musen
			mouse_pos_x = (ev.mouse.x * 1 ) / (double)scale;
			mouse_pos_y = (ev.mouse.y * 1 ) / (double)scale;
			//mouse_pos_x = ev.mouse.x;
			//mouse_pos_y = ev.mouse.y;

		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ //När musknapp är nedtryckt
			if(ev.mouse.button == 1){ //Vänster musknapp
				
				if(buy_street_Q->get_active()){
					int ID_button_pressed_temp = buy_street_Q->button_pressed(mouse_pos_x, mouse_pos_y);
					if(ID_button_pressed_temp != 0){
						switch(ID_button_pressed_temp){
							case 1:
								((Street*)tomter[players[current_player]->get_pos_ruta()])->buy_Street(players[current_player]);
								break;
							case 2:
								auction->set_property(((Street*)tomter[players[current_player]->get_pos_ruta()]));
								auction->set_active(true);
								break;
						}
						buy_street_Q->set_active(false);
					}
				}
				else{
					for(int i = 0; i < ant_buttons; i++){ //Kontrollerar vilken knapp som blivit klickad
						if(buttons[i]->Button_pressed(mouse_pos_x, mouse_pos_y)){
							ID_button_pressed = buttons[i]->get_ID();
						}
					}
				
					switch(ID_button_pressed){
						case 1: //Slå tärningarna
							if(!dice_used){
								roll_dice(dice_1); roll_dice(dice_2);
								
								dice_sprite_0->set_curret_frame(max_tarning - (dice_1)); //Byter bild på tärning
								dice_sprite_1->set_curret_frame(max_tarning - (dice_2));
	
								players[current_player]->move_Player(dice_1 + dice_2, tomter); //Flyttar spelare
								if((tomter[players[current_player]->get_pos_ruta()]->get_typ() == 0) && (tomter[players[current_player]->get_pos_ruta()]->get_Owner()) == 0){ //Om en tomt är ägd av banken
									buy_street_Q->set_active(true);
								}
								else if((tomter[players[current_player]->get_pos_ruta()]->get_typ() == 0) && (tomter[players[current_player]->get_pos_ruta()]->get_Owner()) != players[current_player] && (tomter[players[current_player]->get_pos_ruta()]->get_Owner()) != 0){ //Om tomten är en gata och inte är ägd av dig eller banken
									((Street*)tomter[players[current_player]->get_pos_ruta()])->pay_rent(players[current_player], tomter);
								}
								dice_used = true;
							}
							break;
						case 2: //Köper tomt
							if(tomter[players[current_player]->get_pos_ruta()]->get_typ() == 0){
								((Street*)tomter[players[current_player]->get_pos_ruta()])->buy_Street(players[current_player]);
							}
							break;
						case 3:
							if(dice_used){
								current_player = (current_player + 1) % n_players; //Nästa spelare
								dice_used = false;
							}
							break;
						case 4: //Sälja gata
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
			
			frames++; //Räknar frames
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
			dice_sprite_0->draw(); //Ritar tärningar
			dice_sprite_1->draw();
			if(buy_street_Q->get_active()) //Ritar köpfrågan om den är aktiv
				buy_street_Q->draw(arial_36, arial_16);
			else if(auction->get_active()){
				auction->draw();
			}

			for(int i = 0; i < ant_buttons; i++){ //Ritar knappar
				buttons[i]->draw(arial_16);
			}
			//al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);
			al_draw_textf(arial_16, al_map_rgb(0, 0, 0), 15, 940, 0, "Player %i", current_player);
			al_draw_textf(arial_16, al_map_rgb(0, 0, 0), 100, 940, 0, "Funds: %i", (players[current_player])->get_money());
			//al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 20, 0, "Mouse_x: %lf Mouse_y: %lf", mouse_pos_x, mouse_pos_y);

			//Skalar om bilden och ritar till backbuffern. Vänder sedan på buffern

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
	//Öppnar fil för data till klassen Property
	std::ifstream file("property_config.txt"); 
	//Tillfällig variabler för initiering av objekten
	char namn[max_name_length];
	namn[0] = '/0';
	enum intdata {posx1, posx2, posy1, posy2, posruta};
	int intdata[property_int_data_count];
	char line[max_config_line_length];
	char line2[max_config_line_length];
	bool endfile = false;
	
	if(!file){  //Kontrollerar att filen är öppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize load Property file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_rutor; i++){
			
			file.getline(line, (max_config_line_length-1));
			while(line[0] == '#'){ //Läser in ny rad om första tecknet är kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			for(int j = 0, r_pos = 0; j < property_variable_count; j++){
				if(!endfile){ //Kontrollerar om nollförtecken tidigare hittats
					getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
				}
				r_pos += strlen(line2) + 1; //Räknar hur många tecken som har lästs och används för att veta var nästa inläsning ska ske i strängen

				if(j < property_int_data_count){
					intdata[j] = std::atoi(line2);
				}
				else{
					strncpy(namn, line2, max_name_length-1);
				}
			}
			
			if(intdata[5] == 0){
				int temp_rent_array[max_houses + 1];

				for(int k = 0; k < (max_houses + 1); k++){
					temp_rent_array[k] = intdata[8 + k];
				}
				tomter[i] = new Street(intdata[0], intdata[2], intdata[1], intdata[3], intdata[4], namn,intdata[5] , intdata[7], intdata[14],temp_rent_array , intdata[6]);
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
	//Definerar färger till de olika spelarna

	int player_colors[max_players * 3];
	player_colors[0] = 255; player_colors[1] = 131; player_colors[2] = 43;
	//player_colors[0] = al_map_rgb(255, 131, 43);
	player_colors[3] = 255; player_colors[4] = 206; player_colors[5] = 102;
	//player_colors[1] = al_map_rgb(255, 206, 102);
	//player_colors[2] = al_map_rgb(166, 213, 68);
	player_colors[6] = 166; player_colors[7] = 213; player_colors[8] = 68;
	//player_colors[3] = al_map_rgb(255, 91, 255);
	player_colors[9] = 255; player_colors[10] = 91; player_colors[11] = 255;
	//player_colors[4] = al_map_rgb(155, 0, 155);
	player_colors[12] = 155; player_colors[13] = 0; player_colors[14] = 155;
	//player_colors[5] = al_map_rgb(36, 166, 145);
	player_colors[15] = 36; player_colors[16] = 166; player_colors[17] = 145;
	
	for(int i = 0; i < n_players; i++){ //Skapar spelare
		int pos_x, pos_y, pos_ruta;

		tomter[start_ruta]->get_player_pos(pos_x, pos_y);  //Ger startposition för spelaren
		pos_ruta = tomter[start_ruta]->get_pos_ruta();
		players[i] = new Player(pos_x, pos_y, pos_ruta, i, startpengar, player_colors[i + (i * 3)], player_colors[i + (i * 3) + 1], player_colors[i + (i * 3)] + 2);
	}
}

void roll_dice(int &dice){
	dice = rand() % max_tarning + 1;
}

void read_Button_data(Button* buttons[]) {
	//Öppnar fil för data till klassen Property
	std::ifstream file("button_config.txt"); 
	//Tillfällig variabler för initiering av objekten
	int intdata[button_int_data_count];
	char line[max_config_line_length];
	char line2[max_config_line_length];
	char label[max_config_line_length];
	bool endfile = false;

	if(!file){  //Kontrollerar att filen är öppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize load Property file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_buttons; i++){
		file.getline(line, (max_config_line_length-1));
		for(int j = 0, r_pos = 0; j < button_variable_data_count; j++){
			while(line[0] == '#'){ //Läser in ny rad om första tecknet är kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			if(!endfile){ //Kontrollerar om nollförtecken tidigare hittats
				getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
			}
			r_pos += strlen(line2) + 1; //Räknar hur många tecken som har lästs och används för att veta var nästa inläsning ska ske i strängen
			
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
	//Öppnar fil för data till klassen Property
	std::ifstream file("Status_box_config.txt"); 
	//Tillfällig variabler för initiering av objekten
	int intdata[status_box_int_data_count];
	char line[max_config_line_length];
	char line2[max_config_line_length];
	bool endfile = false;
	const int displacement_x = 0;
	const int displacement_y = 51;

	if(!file){  //Kontrollerar att filen är öppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize Status_box file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_status_box; i++){
		file.getline(line, (max_config_line_length-1));
		for(int j = 0, r_pos = 0; j < status_box_int_data_count; j++){
			while(line[0] == '#'){ //Läser in ny rad om första tecknet är kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			if(!endfile){ //Kontrollerar om nollförtecken tidigare hittats
				getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
			}
			r_pos += strlen(line2) + 1; //Räknar hur många tecken som har lästs och används för att veta var nästa inläsning ska ske i strängen
			intdata[j] = std::atoi(line2);
		}
		
		((Street*)streets[intdata[4]])->create_status_box(new Status_box(intdata[0], intdata[1], intdata[2], intdata[3], displacement_x, displacement_y));
		endfile = false;
	}
}

void auction(Question* buy_street_Q){
	buy_street_Q->set_active(false);
}