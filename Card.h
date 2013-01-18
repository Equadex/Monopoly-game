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
private:
	bool chance;
	int id;
	char *text;
};

#endif