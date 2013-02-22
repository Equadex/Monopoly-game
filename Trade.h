#ifndef TRADE_H
#define TRADE_H

#include "Trade_window.h"
#include "Button.h"

class Trade{
public:
	Trade(int pos_x, int pos_y){
		buttons = new *Button[8 + max_players];
		//buttons[0] = new Button(pos_x + 
		
	}
private:
	Trade_window *window_proposition;
	Trade_window *window_approval;
	Button **buttons;
};

#endif