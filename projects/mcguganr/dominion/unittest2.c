/*----------------------------------------------
 * Unit test for scoreFor.
 * Include the following in your makefile:
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
 * ---------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rngs.h"


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
	int i;
	int seed = 1000;
	int numPlayer = 2;
	int p, r, handCount, countFails = 0;
	int k[10] = {adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int maxHandCount = 5;
	// arrays of all point cards
	int curses[MAX_HAND];
	int estates[MAX_HAND];
	int duchys[MAX_HAND];
	int provinces[MAX_HAND];
	int great_halls[MAX_HAND];
	int garden[MAX_HAND];
	for (i = 0; i < MAX_HAND; i++)
	{
		curses[i] = curse;
		estates[i] = estate;
		duchys[i] = duchy;
		provinces[i] = province;
		great_halls[i] = great_hall;
		garden[i] = gardens;
	}

	printf ("----------------------- TESTING scoreFor() ---------------------------\n");
	for (p = 0; p < numPlayer; p++)
	{
		for (handCount = 0; handCount <= maxHandCount; handCount++)
		{
			memset(&G, 23, sizeof(struct gameState));   // clear the game state
			r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
			if(r) countFails += assert(0);
			else{
				G.handCount[p] = handCount;	// set the number of cards on hand
				G.discardCount[p] = MAX_DECK;
				G.deckCount[p] = MAX_DECK;
				memcpy(G.hand[p], curses, sizeof(int) * handCount); // set all the cards to curse in hand, deck, and discard
				memcpy(G.deck[p], curses, sizeof(int) * MAX_DECK);
				memcpy(G.discard[p], curses, sizeof(int) * MAX_DECK);
				printf("Testing all curses -- ");
				countFails += assert(scoreFor(p, &G) == (-handCount - (MAX_DECK*2)));
				printf("Testing all estates -- ");
				memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to curse in hand, deck, and discard
				memcpy(G.deck[p], estates, sizeof(int) * MAX_DECK);
				memcpy(G.discard[p], estates, sizeof(int) * MAX_DECK);
				countFails += assert(scoreFor(p, &G) == (handCount + (MAX_DECK*2)));
				printf("Testing all duchy -- ");
				memcpy(G.hand[p], duchys, sizeof(int) * handCount); // set all the cards to curse in hand, deck, and discard
				memcpy(G.deck[p], duchys, sizeof(int) * MAX_DECK);
				memcpy(G.discard[p], duchys, sizeof(int) * MAX_DECK);
				countFails += assert(scoreFor(p, &G) == (handCount * 3 + (MAX_DECK*2*3)));
				printf("Testing all province -- ");
				memcpy(G.hand[p], provinces, sizeof(int) * handCount); // set all the cards to curse in hand, deck, and discard
				memcpy(G.deck[p], provinces, sizeof(int) * MAX_DECK);
				memcpy(G.discard[p], provinces, sizeof(int) * MAX_DECK);
				countFails += assert(scoreFor(p, &G) == (handCount * 6 + (MAX_DECK*2*6)));
				printf("Testing all great_halls -- ");
				memcpy(G.hand[p], great_halls, sizeof(int) * handCount); // set all the cards to curse in hand, deck, and discard
				memcpy(G.deck[p], great_halls, sizeof(int) * MAX_DECK);
				memcpy(G.discard[p], great_halls, sizeof(int) * MAX_DECK);
				countFails += assert(scoreFor(p, &G) == (handCount + (MAX_DECK*2)));
				printf("Testing all gardens -- ");
				memcpy(G.hand[p], garden, sizeof(int) * handCount); // set all the cards to curse in hand, deck, and discard
				memcpy(G.deck[p], garden, sizeof(int) * MAX_DECK);
				memcpy(G.discard[p], garden, sizeof(int) * MAX_DECK);
				countFails += assert(scoreFor(p, &G) == ((handCount + (MAX_DECK*2))*((handCount + (MAX_DECK*2))/10)));
			}
		}
	}
	printf("--------------------------------- Testing complete. ------------------------------------------\n");
	if(countFails){
		printf("\033[1;31m"); //Set color to green
		printf("%d TESTS FAILED\n", countFails);
		printf("\033[0m"); //Reset color
	} else{
		printf("\033[1;32m"); //Set color to red
		printf("ALL TESTS PASSED\n");
		printf("\033[0m"); //Reset color
	}
	return 0;
}
