#ifndef CARD_H
#define CARD_H

#include "Constants.h"

class Card{
public:
	Card(int id, bool chance, char* text_in) : id(id), chance(chance){
		text = new char[max_card_length];
		
		//Copy text_input to internal char array

		for(int i = 0; i < max_card_length; i++){
			if(text_in[i] != '\0')
				text[i] = text_in[i];
			else
				text[i] = '\0';
		}
	}
	~Card(){
		delete[] text;
	}
	char* get_text() const{return text;}
	bool get_type() const{return chance;} //chance true if chance, otherwise community cheast
	int get_id() const{return id;}
	int get_action() const{return action;}
	int get_action_s1() const{return action_sum_1;}
	int get_action_s2() const{return action_sum_2;}
private:
	bool chance;
	int id;
	char *text;
	int action, action_sum_1, action_sum_2;
};

#endif