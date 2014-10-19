// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "HunterView.h"

#define MINA_LOOP_SIZE 5

void decideHunterMove(HunterView gameState)
{
   //Get some information from game state
   PlayerID player = whoAmI(gameState);
   Round r = giveMeTheRound(gameState);
   
   //For first round only, sets starting locations for each player
   if (r == 0) {
      char *startLocations[] = {"AM", "CN", "MA", "MU"};
      switch (player) {
         case PLAYER_LORD_GODALMING:
            registerBestPlay(startLocations[0], "NOTHING");
         case PLAYER_DR_SEWARD:
            registerBestPlay(startLocations[1], "NOTHING");
         case PLAYER_VAN_HELSING:
            registerBestPlay(startLocations[2], "NOTHING");
         case PLAYER_MINA_HARKER:
            registerBestPlay(startLocations[3], "NOTHING");
      }
      return;
   }

   //If not first round, check where the player can go
   int numLocations;
   int *locations = whereCanIgo(gameState, &numLocations, TRUE, TRUE, TRUE);
   //Register bad move to start off with
   char *badMove = idToAbbrev(*locations);
   registerBestPlay(badMove, "NOTHING");

   //Now try and pick better move
   LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
   LocationID currLocation = whereIs(gameState, player);

   LocationID path[NUM_MAP_LOCATIONS];
   TransportID trans[NUM_MAP_LOCATIONS];
   int numCitiesToCastle = howCanIGetThere(gameState, currLocation, CASTLE_DRACULA, path, trans);
   
   numCitiesToCastle++;//Just to make sure the compiler doesn't complain

   //If we know Dracula is at Castle Dracula, head towards the castle
   if (dracLocation == CASTLE_DRACULA && player == PLAYER_MINA_HARKER) {
      registerBestPlay(idToAbbrev(path[1]), "NOTHING");
   //If we know Dracula is at sea, we can spread our hunters around port
   //cities to try and catch him when he comes back
   } else if (dracLocation == SEA_UNKNOWN) {
      //go to ports

   //Mina Harker goes to next Location on her pre-defined loop
   //around Castle Dracula if we don't have any relevant information
   } else if (player == PLAYER_MINA_HARKER) {
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
      registerBestPlay(idToAbbrev(move), "NOTHING");
   //Other AI hunters circle around map if we don't have any relevant info
   } else {
      //Pick random number between 0 and numLocations
      //char *move = idToAbbrev(locations[ranNumber]);
   }

}
