#include "Trade_window.h"

Trade_window::Trade_window(int pos_x, int pos_y, Button **buttons, int n_buttons, char *title_in, char *message_in, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *image_in, Property **tomter_in, bool trade_prop_in) : tomter(tomter_in), button_text(button_text), title_font(title_font), n_text_fields(6), street_y_lines(0), street_x_lines(0), max_street_y_lines(5), max_street_x_lines(6), n_buyer_boxes(0), n_seller_boxes(0), trade_prop(trade_prop_in), Question(pos_x, pos_y, buttons, n_buttons, title_in, message_in, image_in){	
	text_fields = new Text_field*[6];
	text_fields[0] = new Text_field(460, pos_y + 405, "Money: %i", button_text);
	text_fields[1] = new Text_field(805, pos_y + 405, "Money: %i", button_text);
	text_fields[2] = new Text_field(40, 220, "What player %i wants to buy of you", button_text);
	text_fields[3] = new Text_field(860, 220, "What player %i is offering in exchange", button_text);
	text_fields[4] = new Text_field(40, 500, "Money: %i", button_text);
	text_fields[5] = new Text_field(490, 500, "Money: %i", button_text);

	buyer_boxes = new Status_box[ant_rutor];
	seller_boxes = new Status_box[ant_rutor];
	
}

void Trade_window::draw(bool &stage_1, bool &stage_2, bool &stage_3, int player_id_selected, Street **seller_streets, Street **buyer_streets, int n_seller_streets, int n_buyer_streets){
	if(active){
		int temp_colors[3];

		al_draw_bitmap(image, pos_x, pos_y, 0);
		for(int i = 0; i < n_buttons; i++){
			buttons[i]->draw(button_text);
		}
		al_draw_text(title_font, al_map_rgb(0, 0, 0), pos_x + 430, pos_y + 40, ALLEGRO_ALIGN_CENTRE, title[0]);
		al_draw_text(button_text, al_map_rgb(0, 0, 0), pos_x + 430, pos_y + 100, ALLEGRO_ALIGN_CENTRE, message[0]);

			
		if(stage_1)
			al_draw_rectangle(pos_x + 15, pos_y + 115, pos_x + 260, pos_y + 440, al_map_rgb(0, 255, 0), 2);
		if(stage_2)
			al_draw_rectangle(pos_x + 260, pos_y + 115, pos_x + 570, pos_y + 440, al_map_rgb(0, 255, 0), 2);
		if(stage_3)
			al_draw_rectangle(pos_x + 610, pos_y + 115, pos_x + 895, pos_y + 440, al_map_rgb(0, 255, 0), 2);

		for(int i = 0; i < n_buttons; i++){
			if(buttons[i]->get_ID() == 11 + player_id_selected){
				al_draw_filled_rectangle(buttons[i]->get_pos_x_1() - 10, buttons[i]->get_pos_y_1() + 10 , buttons[i]->get_pos_x_1() , buttons[i]->get_pos_y_2() - 10, al_map_rgb(0, 255, 0));
			}
		}
		for(int i = 0; i < n_text_fields; i++){
			if(i == 3)
				text_fields[i]->draw(ALLEGRO_ALIGN_RIGHT);
			else
				text_fields[i]->draw();
			if(trade_prop && i > 1)
				break;
		}

		for(int i = 0; i < n_seller_streets; i++){
			seller_boxes[i].draw();
		}
		for(int i = 0; i < n_buyer_streets; i++){
			buyer_boxes[i].draw();
		}

	}
}

void Trade_window::update(int seller_sum, int buyer_sum, Street **seller_streets, Street **buyer_streets, int n_seller_streets, int n_buyer_streets, Player *seller, Player *buyer){
	char temp[256];
	int temp_colors[3];

	if(trade_prop){
		sprintf(temp, text_fields[0]->get_text_template(), seller_sum);
		text_fields[0]->set_text(temp);
		sprintf(temp, text_fields[1]->get_text_template(), buyer_sum);
		text_fields[1]->set_text(temp);
	}
	else{
		sprintf(temp, text_fields[2]->get_text_template(), buyer->get_id());
		text_fields[2]->set_text(temp);
		sprintf(temp, text_fields[3]->get_text_template(), seller->get_id());
		text_fields[3]->set_text(temp);
		sprintf(temp, text_fields[4]->get_text_template(), seller_sum);
		text_fields[4]->set_text(temp);
		sprintf(temp, text_fields[5]->get_text_template(), buyer_sum);
		text_fields[5]->set_text(temp);
	}

	int x , y;
	if(trade_prop){
		for(int i = 0, x = 0, y = 0, n_seller_boxes = 0; i < n_seller_streets && trade_prop; i++){
			seller_streets[i]->get_colors(temp_colors);
			if(temp_colors[0] == 255 && temp_colors[1] == 255 && temp_colors[2] == 255){
				temp_colors[0] = 128; temp_colors[1] = 128; temp_colors[2] = 128;
			}
			seller_boxes[i].init(pos_x + 270 + x * 40, pos_y + 180 + 40 * y, pos_x + 300 + x * 40, pos_y + 210 + 40 * y, 0, 0, seller_streets[i]);
			seller_boxes[i].set_color(temp_colors[0], temp_colors[1], temp_colors[2]);
			seller_boxes[i].set_active(true);
			n_seller_boxes++;
			x++;
			if(x > max_street_x_lines){
				y++;
				if(y > max_street_y_lines)
					break;
				x = 0;
			}
		}
	}

	if(trade_prop){
		for(int i = 0, x = 0, y = 0, n_buyer_boxes = 0; i < n_buyer_streets; i++){
			buyer_streets[i]->get_colors(temp_colors);
			if(temp_colors[0] == 255 && temp_colors[1] == 255 && temp_colors[2] == 255){
				temp_colors[0] = 128; temp_colors[1] = 128; temp_colors[2] = 128;
			}
			buyer_boxes[i].init(pos_x + 630 + x * 40, pos_y + 170 + 40 * y, pos_x + 660 + x * 40, pos_y + 200 + 40 * y, 0, 0, buyer_streets[i]);
			buyer_boxes[i].set_color(temp_colors[0], temp_colors[1], temp_colors[2]);
			buyer_boxes[i].set_active(true);

			n_buyer_boxes++;
			x++;
			if(x > max_street_x_lines){
				y++;
				if(y > max_street_y_lines)
					break;
				x = 0;
			}
		}
	
	}
}