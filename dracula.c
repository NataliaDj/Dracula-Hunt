// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#define PLACES 12

void decideDraculaMove(DracView gameState)
{
	//Get some information from game statec
   //PlayerID player = whoAmI(gameState);
   //LocationID currLocation = whereIs(gameState, player);
   Round r = giveMeTheRound(gameState);

   //LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   //LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   //LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   //LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);
   LocationID places[PLACES] = {MARSEILLES, GENOA, MILAN, ZURICH, GENEVA, STRASBOURG, 
      BRUSSELS, LE_HAVRE, NANTES, BORDEAUX, TOULOUSE};
   

   if (r == 0) {
      //Go furthest away from h
      registerBestPlay("MR", "In Marseilles");
      return;
      //Prefer port cities
      //registerBestPlay("CD", "NOTHING");
   }
   int *numLocations = NULL;
   LocationID *locations = whereCanIgo(gameState, numLocations, TRUE,FALSE);
   int choice = locations[0];

   for (int i = 0; i < PLACES; ++i)
   {
      for (int j = 0; j < *numLocations; ++i)
      {
         if (places[i]==locations[j])
         {
            choice = places[i];
            break;
         }
      }
   }
   

   //Register bad move
   char *move = idToAbbrev(choice);
   registerBestPlay(move, "NOTHING");
}
