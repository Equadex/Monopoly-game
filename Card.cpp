#include "Card.h"

Card::Card(int id, bool chance, int action, int action_sum_1, int action_sum_2, char* text_in) : id(id), chance(chance), action(action), action_sum_1(action_sum_1), action_sum_2(action_sum_2), disabled(false){
	text = new char[max_card_length];
	
	//Copy text_input to internal char array

	for(int i = 0; i < max_card_length; i++){
		if(text_in[i] != '\0')
			text[i] = text_in[i];
		else{
			text[i] = '\0';
			break;
		}
	}
}