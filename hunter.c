// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

/* MESSAGES GUIDE
1st Letter: Either Y or N, for "Yes, we have a message" or "No message"

2nd Letter: Either Y or N, for "Yes, we know something about Draculas 
            location" or "No we don't know his location"

3rd Letter: Either Y or N, if our knowledge of his location is absolute
            (exact) or vague

4th Letter: If last letter was Y, then we put how we know about his location
            T = Found something in trail, E = encountered Dracula. If 
            the last letter was N, then we put another message in this letter

5th/6th Letter: We give the Location abbrevation for the location information
                we know
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "HunterView.h"

#define MINA_LOOP_SIZE 5

//Constants for knowledge about Draculas position
#define NOT_KNOWN 0
#define KNOWN_ABSOLUTE 1
#define KNOWN_VAGUE 2

void decideHunterMove(HunterView gameState)
{
   //Get some information from game state
   PlayerID player = whoAmI(gameState); 
   Round r = giveMeTheRound(gameState);
   
   //For first round only, sets starting locations for each player
   if (r == 0) {
      char *startLocations[] = {"AM", "CN", "ZU", "CD"};
      switch (player) {
         case PLAYER_LORD_GODALMING:
            registerBestPlay(startLocations[0], "NOTHING");
            break;
         case PLAYER_DR_SEWARD:
            registerBestPlay(startLocations[1], "NOTHING");
            break;
         case PLAYER_VAN_HELSING:
            registerBestPlay(startLocations[2], "NOTHING");
            break;
         case PLAYER_MINA_HARKER:
            registerBestPlay(startLocations[3], "NOTHING");
            break;
      }
      return;
   }

   //If not first round, check where the player can go
   int numLocations;
   LocationID *locations = whereCanIgo(gameState, &numLocations, TRUE, TRUE, TRUE);
   //Register bad move to start off with
   char *badMove = idToAbbrev(*locations);
   registerBestPlay(badMove, "NOTHING");

   //Now try and pick better move
   LocationID currLocation = whereIs(gameState, player);

   /*int dracLocationKnown;
   //LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
   char *message = giveMeMessage(gameState);
   for (int i = 0; i < MAX_MESSAGE_SIZE; i++) {
      printf("message[%d] = %c\n", i, message[i]);
   }
   if (message[0] == 'Y' && message[1] == 'Y') {

   }*/


   LocationID path[NUM_MAP_LOCATIONS];
   TransportID trans[NUM_MAP_LOCATIONS];

   //If Mina Harker, stay at Castle Dracula
   if (player == PLAYER_MINA_HARKER) {
      if (currLocation == CASTLE_DRACULA) {
         registerBestPlay("CD", "NOTHING");
      } else {
         int numCitiesToCastle = howCanIGetThere(gameState, currLocation, CASTLE_DRACULA, path, trans);
         numCitiesToCastle++;//Just to make sure the compiler doesn't complain
         registerBestPlay(idToAbbrev(path[1]), "NOTHING");
      }
   /*
      LocationID minaLoop[] = {AMSTERDAM, PARIS, LONDON, MUNICH, MADRID};
      LocationID move;
      int i;
      for (i = 0; i < MINA_LOOP_SIZE; i++) {
         move = minaLoop[i];
         if (move == currLocation) {
            if (i == MINA_LOOP_SIZE - 1) {
               move = minaLoop[0];
            } else {
               move = minaLoop[i+1];
            }
         }
      }
      registerBestPlay(idToAbbrev(move), "NOTHING");*/


   //Other AI hunters circle around map if we don't have any relevant info
   } else {
      /*LocationID trail[TRAIL_SIZE];
      giveMeTheTrail(gameState, player, trail);
      if (locations[numLocations/2] != trail[0] && locations[numLocations/2] != trail[1]) {
         registerBestPlay(idToAbbrev(locations[numLocations/2]), "NOTHING");
      }
		*/

      int adjacent;  // stores amount of possible places
      // find where players can go
      LocationID *neighbour = whereCanTheyGo(gameState, &adjacent, player, 
         TRUE, TRUE, TRUE);

		/*
      // port cities are found by checking if they can go in the sea from a city
      LocationID *portCity = whereCanTheyGo(gameState, &adjacent, player, 
         FALSE, FALSE, TRUE);
		*/
		
      LocationID trail[TRAIL_SIZE]; // stores past 6 plays
      // places the past plays into trail array
      giveMeTheTrail(gameState, player, trail);

      int i;   // used in "for" loop
      // find next place to go to
      for (i = 0; i < adjacent; i++) {
         // check for if they've already been to a city
         if (neighbour[i] != trail[1] && neighbour[i] != trail[0]
            && isPortCity(gameState, neighbour[i], player) == TRUE) {
            registerBestPlay(idToAbbrev(neighbour[i]), "seashells by the seashore");
            break;
         }
      }

      // no adjacent port cities were found, so randomly choose a city
      int numLocations; // stores number of locations
      int randomCity;    // city chosen at random
      // find what cities can be connected to by roads and rails
		int *otherCities = whereCanTheyGo(gameState, &numLocations, player, TRUE, TRUE, FALSE);      
		// Picks a different city if players've already been to city last turn
      do {
         randomCity = otherCities[rand() % numLocations];
      } while (randomCity == trail[1] || randomCity == trail[0]);

      registerBestPlay(idToAbbrev(randomCity), "Doesn't seem to be a boat for miles");
   }

}
