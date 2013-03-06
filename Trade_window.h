#ifndef TRADE_WINDOW_H
#define TRADE_WINDOW_H

#include "Question.h"
#include "Text_field.h"
#include "Property.h"
#include "Player.h"

class Trade_window : public Question{
public:

private:
	Text_field **text_fields player_list;
	Property **tomter;
	Player **players;
};

#endif