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
#include "Owner.h"
#include "Button.h"
#include "Status_box.h"

//Globala variabler lokala

const int max_config_line_length = 128;
const int max_name_length = 64;
const int property_int_data_count = 15;
const int property_variable_count = property_int_data_count + 1;
const int button_int_data_count = 5;
const int status_box_int_data_count = 5;
const int startpengar = 1500;
const int start_ruta = 0;
const int max_tarning = 6;
const int ant_buttons = 3;
const int ant_status_box = 28;

//Funktionsdeklartioner

void read_Property_data(Property *tomter[]);
void getline_char(char line_in[],char line_out[],int out_length, int max_read, char stop, bool &endfile, int start = 0);
void create_players(int n_players, Player *players[], Property *tomter[]);
void roll_dice(int &dice);
void read_Button_data(Button* buttons[]);
void read_Status_box_data(Status_box* status_boxes[], Property** streets);

int main(){
	//Konstanta variabler i main

	//Variabler som beh�vs f�r hantera allegro

	int width = 1280;
	int height = 1000;
	double FPS = 120;
	int mouse_pos_x = 0;
	int mouse_pos_y = 0;
	int frames = 0, gameFPS = 0;
	float gameTime = 0;

	bool done = false;
	bool draw = false;

	//Variabler n�dv�ndiga f�r spelet

	enum typ{TOMT, SKATT, CHANS, ALLMANING};
	int n_players = 2, current_player = 0;
	int dice_1, dice_2;
	int ID_button_pressed;

	bool dice_used = false;

	Property *tomter[ant_rutor]; //F�lt av tomter
	Player *players[max_players]; //F�lt av spelare
	Button *buttons[ant_buttons]; //F�lt av buttons
	Status_box *status_boxes[ant_status_box]; //F�lt av status boxes (de som presenterar f�r spelaren vilka gator de �ger)


	read_Property_data(tomter); //L�ser in data till tomter
	create_players(n_players, players, tomter); //Skapar spelare
	read_Button_data(buttons); //L�ser in buttons koodinater och id
	read_Status_box_data(status_boxes, tomter); //L�ser in status boxarnas positioner och gatunummer
	

	//Allegro variabler

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_BITMAP *spelplan = NULL;
	ALLEGRO_BITMAP *spelbrade = NULL;

	if(!al_init()){ //Initierar allegro bibloteket
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize Allegro" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return (-1);
	}
	//Skapar och testar display
	display = al_create_display(width, height);
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

	//Skapar fonts

	ALLEGRO_FONT *arial_16 = al_load_ttf_font("arial.ttf", 16, 0);

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
			mouse_pos_x = ev.mouse.x;
			mouse_pos_y = ev.mouse.y;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ //N�r musknapp �r nedtryckt
			if(ev.mouse.button == 1){ //V�nster musknapp
				
				for(int i = 0; i < ant_buttons; i++){ //Kontrollerar vilken knapp som blivit klickad
					if(buttons[i]->Button_pressed(mouse_pos_x, mouse_pos_y)){
						ID_button_pressed = buttons[i]->get_ID();
					}
				}
				
				switch(ID_button_pressed){
					case 1: //Sl� t�rningarna
						if(!dice_used){
							roll_dice(dice_1); roll_dice(dice_2); 
							players[current_player]->move_Player(dice_1 + dice_2, tomter);
							dice_used = true;
						}
						break;
					case 2: //K�per tomt
						if(tomter[players[current_player]->get_pos_ruta()]->get_typ() == 0){
							((Street*)tomter[players[current_player]->get_pos_ruta()])->buy_Street(players[current_player]);
						}
						break;
					case 3:
						current_player = (current_player + 1) % n_players; //N�sta spelare
						dice_used = false;
						break;
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
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(spelplan, 0, 0, 0);
			al_draw_bitmap(spelbrade, 0, 0, 0);
			for(int i = 0; i < n_players; i++){
				players[i]->draw_player();
			}
			for(int i = 0; i < ant_status_box; i++){
				status_boxes[i]->draw();
			}
			al_draw_textf(arial_16, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);
			al_draw_textf(arial_16, al_map_rgb(0, 0, 0), 15, 940, 0, "Player %i", current_player);
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
	//Definerar f�rger till de olika spelarna

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

		tomter[start_ruta]->get_player_pos(pos_x, pos_y);  //Ger startposition f�r spelaren
		pos_ruta = tomter[start_ruta]->get_pos_ruta();
		players[i] = new Player(pos_x, pos_y, pos_ruta, i, startpengar, player_colors[i + (i * 3)], player_colors[i + (i * 3) + 1], player_colors[i + (i * 3)] + 2);
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
	bool endfile = false;

	if(!file){  //Kontrollerar att filen �r �ppen
		al_show_native_message_box(NULL, "ERROR", "ERROR", "Failed to initilize load Property file config" , NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ant_buttons; i++){
		file.getline(line, (max_config_line_length-1));
		for(int j = 0, r_pos = 0; j < button_int_data_count; j++){
			while(line[0] == '#'){ //L�ser in ny rad om f�rsta tecknet �r kommentartecken
				file.getline(line, (max_config_line_length-1));
			}
			if(!endfile){ //Kontrollerar om nollf�rtecken tidigare hittats
				getline_char(line, line2, max_config_line_length,max_config_line_length, ',', endfile, r_pos);
			}
			r_pos += strlen(line2) + 1; //R�knar hur m�nga tecken som har l�sts och anv�nds f�r att veta var n�sta inl�sning ska ske i str�ngen
			intdata[j] = std::atoi(line2);
		}
		buttons[i] = new Button(intdata[0], intdata[1], intdata[2], intdata[3], intdata[4]);
		endfile = false;
	}
}

void read_Status_box_data(Status_box* status_boxes[], Property** streets){
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
		status_boxes[i] = new Status_box(intdata[0], intdata[1], intdata[2], intdata[3], (Street*)streets[intdata[4]], displacement_x, displacement_y);
		((Street*)streets[intdata[4]])->set_Status_box_owner(status_boxes[i]);
		endfile = false;
	}
}