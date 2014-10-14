// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "HunterView.h"

void decideHunterMove(HunterView gameState)
{
   //Get some information from game state
   PlayerID player = whoAmI(gameState);
   int numLocations;
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
   int *locations = whereCanIgo(gameState, &numLocations, TRUE, TRUE, TRUE);
   //Register bad move to start off with
   char *move = idToAbbrev(*locations);
   registerBestPlay(move, "NOTHING");

   //Now try and pick better move
   LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
   if (dracLocation == CASTLE_DRACULA && player == PLAYER_MINA_HARKER) {
      //head towards the castle
   } else if (dracLocation == SEA_UNKNOWN) {
      //go to ports
   } else if (player == PLAYER_MINA_HARKER) {
      //circle around Castle Dracula
   } else {
      //circle around randomly
   }

}
