// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

#define PLACES 11
#define INN 9
#define OUT 17
#define SPECIAL 4
#define WEST 8
#define EAST 9
#define SOUTH 6

static int countHunter (LocationID locations[], int length, DracView gameState);
static int possibleHunterLoc(LocationID locations[], int distance, DracView gameState);

void decideDraculaMove(DracView gameState)
{
   //Locations
   LocationID dracLocation = whereIs(gameState, PLAYER_DRACULA);
   LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);

   //Array of dracula's path
   LocationID inner[INN] = {NUREMBURG, COLOGNE, BRUSSELS, FRANKFURT, PARIS, 
      CLERMONT_FERRAND, GENEVA, ZURICH, STRASBOURG};
   LocationID outer[OUT] = {NUREMBURG, LEIPZIG, BERLIN, HAMBURG, BRUSSELS, 
      LE_HAVRE, NANTES, BORDEAUX, SARAGOSSA, TOULOUSE, CLERMONT_FERRAND, MARSEILLES, 
      GENOA, VENICE, MUNICH, VIENNA, PRAGUE};
   LocationID special[SPECIAL] = {NUREMBURG, COLOGNE, BRUSSELS, CLERMONT_FERRAND};

   //LocationID changeToWest[1] = {SARAGOSSA};
   //LocationID changeToEast[1] = {VIENNA};
   
   LocationID west[WEST] = {TOULOUSE, SARAGOSSA, SANTANDER, MADRID, LISBON, CADIZ, GRANADA, ALICANTE};
   LocationID east[EAST] = {VIENNA, BUDAPEST, SZEGED, BELGRADE, BUCHAREST, SOFIA, VALONA, SARAJEVO, ZAGREB};
   LocationID south[SOUTH] = {BARI, NAPLES, FLORENCE, VENICE, GENOA, MILAN};

   //Starting Move
   int score = giveMeTheScore(gameState);
   Round r = giveMeTheRound(gameState);
   printf("round = %d\n", r);
   if (r == 0) {
      registerBestPlay("NU", "It is time to begin");
      return;
   }

   //Dracula follow's his path
   int numLocations = 0;
   LocationID *locations = whereCanIgo(gameState, &numLocations, TRUE, FALSE);
   printf("numLocations = %d\n", numLocations);
   int i, j, circle = OUT;
   int choice = locations[0];

   //if (drac is in special locations)
   // Checks the locations of the hunters and decide on a route
   // (inner or outer circle)
   //else
   // checks if dracula is in the inner or outer circle
   // Follows along the path

   int specialCase = FALSE;
   for (i = 0; i < SPECIAL; ++i)
   {
      if (dracLocation == special[i])
      {
         specialCase = TRUE;
      }
   }

   if (specialCase == TRUE)
   {
      printf("In specC\n");
      int innie = countHunter(inner, INN, gameState);
      int outie = countHunter(outer, OUT, gameState);
      if (innie < outie)
      {
         circle = INN;
      }
   } 
   else if (dracLocation == SARAGOSSA)
   {
      int outie = countHunter (outer, OUT, gameState);
      int westie = countHunter (west, WEST, gameState);
      if (westie < outie)
      {
         circle = WEST;
      }
   }
   else if (dracLocation == VIENNA)
   {
      int outie = countHunter (outer, OUT, gameState);
      int eastie = countHunter (east, EAST, gameState);
      if (eastie < outie)
      {
         circle = EAST;
      }
   } else if (dracLocation == MUNICH) {
      // counts the possible locations that the hunters can get to 
      // tries to escape from it 
      int outie = possibleHunterLoc(outer, OUT, gameState);
      int eastie = possibleHunterLoc(east, EAST, gameState);
      if (eastie < outie) {
         circle = EAST;
      } else {
         //go to the outer circle
         circle = INN;
      }
   } else if (dracLocation == FLORENCE) {
      //when its out of the circle 
      int southie = possibleHunterLoc(south, SOUTH, gameState);
      int outie = possibleHunterLoc(outer, OUT, gameState);
      if (southie == 0) {
         // if there are no hunters whose move will be in the south, it'll be safe to make a move there 
         //since it's in the corner of the map
         circle = SOUTH; 
      } else if (outie < southie) {
         circle = OUT; 
      }

   } else if (dracLocation == ZURICH) {
      int innie = possibleHunterLoc(inner, INN, gameState);
      int outie = possibleHunterLoc(outer, OUT, gameState);
      int eastie = possibleHunterLoc(east, EAST, gameState);
      if (innie < outie) {
         circle = INN;
      } else if (innie > outie) {
         circle = OUT;
      } else if (eastie <outie) {
         circle = EAST;
      } else if (eastie > outie) {
         circle = OUT;
      }
   }
   else
   {
      for (i = 0; i < INN; ++i)
      {
         if (locations[i] == dracLocation)
         {
            circle = INN;
            break;
         }
      }
      for (i = 0; i < WEST; ++i)
      {
         if (locations[i] == dracLocation)
         {
            circle = WEST;
            break;
         }
      }
      for (i = 0; i < EAST; ++i)
      {
         if (locations[i] == dracLocation)
         {
            circle = EAST;
            break;
         }
      }
   }


   int isFound = FALSE;
   if (circle == INN)
   {
      for (i = 0; i < INN; ++i)
      {
         for (j = 0; j < numLocations; ++j)
         {
            if (inner[i]==locations[j])
            {
               choice = inner[i];
               isFound = TRUE;
               break;
            }
         }

         if (isFound == TRUE)
         {
            break;
         }
      }
   } 
   else if (circle == WEST)
   {
      for (i = 0; i < WEST; ++i)
      {
         for (j = 0; j < numLocations; ++j)
         {
            if (west[i]==locations[j])
            {
               choice = west[i];
               isFound = TRUE;
               break;
            }
         }

         if (isFound == TRUE)
         {
            break;
         }
      }
   }
   else if (circle == EAST)
   {
      for (i = 0; i < EAST; ++i)
      {
         for (j = 0; j < numLocations; ++j)
         {
            if (east[i]==locations[j])
            {
               choice = east[i];
               isFound = TRUE;
               break;
            }
         }

         if (isFound == TRUE)
         {
            break;
         }
      }
   }
   else
   {
      for (i = 0; i < OUT; ++i)
      {
         for (j = 0; j < numLocations; ++j)
         {
            if (outer[i]==locations[j])
            {
               choice = outer[i];
               isFound = TRUE;
               break;
            }
         }

         if (isFound == TRUE)
         {
            break;
         }
      }
   }
  
   //Figuring out Dracula's message
   char *name = idToName(choice);
   if ((r % 13 == 0) && (r != 0))
   {
      name = "Children of the night, be free!";
   } 
   else if (godLocation == dracLocation)
   {
      name = "Lord";
   } 
   else if (sewLocation == dracLocation)
   {
      name = "Dr";
   } 
   else if (vanLocation == dracLocation)
   {
      name = "Van";
   } 
   else if (minLocation == dracLocation)
   {
      name = "Mina";
   } 
   else if (r == 1)
   {
      name = "My plans are in motion";
   } 
   else if (score < 50)
   {
      name = "Muahaha! The world is in the palm of my hand";
   } 
   else if (score < 100)
   {
      name = "Merely seconds to success!";
   }

   //Returns the move
   char *move = idToAbbrev(choice);
   registerBestPlay(move, name);
}

int countHunter (LocationID locations[], int length, DracView gameState)
{
   LocationID godLocation = whereIs(gameState, PLAYER_LORD_GODALMING);
   LocationID sewLocation = whereIs(gameState, PLAYER_DR_SEWARD);
   LocationID vanLocation = whereIs(gameState, PLAYER_VAN_HELSING);
   LocationID minLocation = whereIs(gameState, PLAYER_MINA_HARKER);
   int i, count = 0;
   for (i = 0; i < length; ++i)
   {
      if ( (godLocation == locations[i]) ||
           (sewLocation == locations[i]) ||
           (vanLocation == locations[i]) ||
           (minLocation == locations[i]) )
      {
         count++;
      }
   }

   return count;
}
int possibleHunterLoc(LocationID locations[], int distance, DracView gameState) {
   int numLocations = 0;
   LocationID *godLocation = whereCanTheyGo(gameState, &numLocations, PLAYER_LORD_GODALMING, TRUE, TRUE, FALSE);
   LocationID *sewLocation = whereCanTheyGo(gameState, &numLocations, PLAYER_DR_SEWARD, TRUE, TRUE, FALSE);
   LocationID *vanLocation = whereCanTheyGo(gameState, &numLocations, PLAYER_VAN_HELSING, TRUE, TRUE, FALSE);
   LocationID *minLocation = whereCanTheyGo(gameState, &numLocations, PLAYER_MINA_HARKER, TRUE, TRUE, FALSE);

   //count the possible places where the hunters is able to reach from their current position 
   int i;
   int count = 0;
   for (i = 0; i < distance; i++) {
      if ((*godLocation == locations[i]) || (*sewLocation == locations[i]) || (*vanLocation == locations[i]) || (*minLocation == locations[i])) {
         count++;
   }
}

   return count;
}