/*----------------------------------------------
 * Unit test for getCost.
 * Include the following in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 * ---------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"

//From interface.h
void cardNumToName(int card, char *name){
	 switch(card){
	  case curse: strcpy(name,"Curse");
	    break;
	  case estate: strcpy(name,"Estate");
	    break;
	  case duchy: strcpy(name,"Duchy");
	    break;
	  case province: strcpy(name,"Province");
	    break;
	  case copper: strcpy(name,"Copper");
	    break;
	  case silver: strcpy(name,"Silver");
	    break;
	  case gold: strcpy(name,"Gold");
	    break;
	  case adventurer: strcpy(name,"Adventurer");
	    break;
	  case council_room: strcpy(name,"Council Room");
	    break;
	  case feast: strcpy(name,"Feast");
	    break;
	  case gardens: strcpy(name,"Gardens");
	    break;
	  case mine: strcpy(name,"Mine");
	    break;
	  case remodel: strcpy(name,"Remodel");
	    break;
	  case smithy: strcpy(name,"Smithy");
	    break;
	  case village: strcpy(name,"Village");
	    break;
	  case baron: strcpy(name,"Baron");
	    break;
	  case great_hall: strcpy(name,"Great Hall");
	    break;
	  case minion: strcpy(name,"Minion");
	    break;
	  case steward: strcpy(name,"Steward");
	    break;
	  case tribute: strcpy(name,"Tribute");
	    break;
	  case ambassador: strcpy(name,"Ambassador");
	    break;
	  case cutpurse: strcpy(name,"Cutpurse");
	    break;
	  case embargo: strcpy(name,"Embargo");
	    break;
	  case outpost: strcpy(name,"Outpost");
	    break;
	  case salvager: strcpy(name,"Salvager");
	    break;
	  case sea_hag: strcpy(name,"Sea Hag");
	    break;
	 case treasure_map: strcpy(name,"Treasure Map");
	    break;

	 default: strcpy(name,"?");
	}
}

int assert(int arg){
	if(arg){
		printf("\033[1;32m"); //Set color to green
		printf("TEST PASSED\n");
		printf("\033[0m"); //Reset color
		return 0;
	}
	printf("\033[1;31m"); //Set color to red
	printf("TEST FAILED\n");
	printf("\033[0m"); //Reset color
	return 1;
}

int main(void){
	//Initialize card costs
	int* costsInOrder = (int*)malloc(27 * sizeof(int));
	costsInOrder[0] = 0;
	costsInOrder[1] = 2;
	costsInOrder[2] = 5;
	costsInOrder[3] = 8;
	costsInOrder[4] = 0;
	costsInOrder[5] = 3;
	costsInOrder[6] = 6;
	costsInOrder[7] = 6;
	costsInOrder[8] = 5;
	costsInOrder[9] = 4;
	costsInOrder[10] = 4;
	costsInOrder[11] = 5;
	costsInOrder[12] = 4;
	costsInOrder[13] = 4;
	costsInOrder[14] = 3;
	costsInOrder[15] = 4;
	costsInOrder[16] = 3;
	costsInOrder[17] = 5;
	costsInOrder[18] = 3;
	costsInOrder[19] = 5;
	costsInOrder[20] = 3;
	costsInOrder[21] = 4;
	costsInOrder[22] = 2;
	costsInOrder[23] = 5;
	costsInOrder[24] = 4;
	costsInOrder[25] = 4;
	costsInOrder[26] = 4;
	
	int i, countFails = 0;
	char name[15];
	for(i=0; i<27; i++){
		memset(name, '\0', 15);
		cardNumToName(i, name);
		printf("Testing card: %s -- ", name);
		countFails += assert(getCost(i) == costsInOrder[i]);
	}

	printf("Testing lower boundary condition -- ");
	countFails += assert(getCost(-1) == -1);
	printf("Testing upper boundary condition -- ");
	countFails += assert(getCost(27) == -1);
	
	printf("--------------------------------Testing complete.-----------------------------------\n");

	if(countFails){
		printf("\033[1;31m"); //Set color to red
		printf("%d TESTS FAILED\n", countFails);
		printf("\033[0m"); //Reset color
	} else {
		printf("\033[1;32m"); //Set color to green
		printf("ALL TESTS PASSED\n");
		printf("\033[0m"); //Reset color
	}

	free(costsInOrder);
	return 0;
}
