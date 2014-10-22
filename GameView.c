// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

#define CHAR_PER_TURN 8
#define NO_PLAYER -1
#define CHAR_TO_INT_OFFSET 48

struct gameView {
    int gameScore;
    int health[NUM_PLAYERS];
    Round turnNum;
    LocationID playerLocations[NUM_PLAYERS][TRAIL_SIZE];
    Map map;
};

static PlayerID findLatestPlayer (char c);
static void fillPlayerTrails (GameView g, char *pastPlays, int length);
static int findPlayerHealth (GameView g, char *pastPlays, int length);
static int maturedVampires (GameView g, char *pastPlays, int length);
static int isPlaceSea (char c1, char c2);

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{  
   //Allocate new view and map
   GameView g = malloc(sizeof(struct gameView));
   g->map = newMap();

   //Since each turn is composed of CHAR_PER_TURN (8) characters:
   int length = strlen(pastPlays);
   g->turnNum = (length + CHAR_PER_TURN -1)/CHAR_PER_TURN; //rounds up

   //Fill the past 7 turn trails for each player
   fillPlayerTrails(g, pastPlays, length);

   //Fill the health arrays for each player
   //Function returns the number of times players have died
   int numDeaths = findPlayerHealth(g, pastPlays, length);

   //Find current game score
   int numMatured = maturedVampires(g, pastPlays, length);
   g->gameScore = GAME_START_SCORE - (numDeaths * SCORE_LOSS_HUNTER_HOSPITAL)
                  - (numMatured * SCORE_LOSS_VAMPIRE_MATURES) 
                  - (g->turnNum / NUM_PLAYERS);

   return g;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
   disposeMap(toBeDeleted->map);
   free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    return (currentView->turnNum)/NUM_PLAYERS;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{  
   return ((currentView->turnNum) % NUM_PLAYERS);
}

// Get the current score
int getScore(GameView currentView)
{
    return currentView->gameScore;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
   return currentView->health[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    return currentView->playerLocations[player][0];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
   int i;
   for (i = 0; i < TRAIL_SIZE; i++) {
      trail[i] = currentView->playerLocations[player][i];
   } 
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{    
  if (round == 0) {
     *numLocations = NUM_MAP_LOCATIONS;
     LocationID *locations = malloc(NUM_MAP_LOCATIONS*sizeof(LocationID));
     int i;
     for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        locations[i] = i;
     }
     return locations;
  }
  int j = 0, i;
  LocationID *locations = malloc(NUM_MAP_LOCATIONS*sizeof(LocationID));
  LocationID visited[NUM_MAP_LOCATIONS];

  for (i = 0; i < NUM_MAP_LOCATIONS; ++i)
  {
    locations[i] = -1;
    visited[i] = FALSE;
  }

  if (player == PLAYER_DRACULA)
  {
    visited[ST_JOSEPH_AND_ST_MARYS] = TRUE;
    for (i = 0; i < TRAIL_SIZE; ++i)
    {
      visited[currentView->playerLocations[PLAYER_DRACULA][i]] = TRUE;
    }
  }

  if (visited[from] == FALSE)
  {
    locations[j] = from;
    visited[from] = TRUE;
    j++;
  }

  if (road == TRUE)
  {
    LocationID roadConnections[NUM_MAP_LOCATIONS];
    int roadCount = 0;
    VList curr = currentView->map->connections[from];

    while (curr != NULL)
    {
      if (curr->type == ROAD)
      {
        roadConnections[roadCount] = curr->v;

        roadCount++;
      }
      curr = curr->next;
    }

    for (i = 0; i < roadCount; ++i)
    {
      if (!visited[roadConnections[i]])
      {
        locations[j] = roadConnections[i];
        visited[roadConnections[i]] = TRUE;
        j++;
      }
    }
  }

  if ( (rail == TRUE) && (player != PLAYER_DRACULA) )
  {
    LocationID railConnections[NUM_MAP_LOCATIONS];
    int railCount = 0;
    VList curr = currentView->map->connections[from];

    while (curr != NULL)
    {
      if ((round+player) % 4 != 0)
      {
        if (curr->type == RAIL)
        {
          if ((round+player) % 4 >= 2)
          {
            VList secCurr = currentView->map->connections[curr->v];

            while(secCurr != NULL)
            {

              if (secCurr->type == RAIL)
              {
                if ((round+player) % 4 == 3)
                {
                  VList thiCurr = currentView->map->connections[secCurr->v];

                  while (thiCurr != NULL)
                  {
                    if (thiCurr->type == RAIL)
                    {
                      railConnections[railCount] = thiCurr->v;

                      railCount++;
                    }
                    thiCurr = thiCurr->next;
                  }
                }
                railConnections[railCount] = secCurr->v;

                railCount++;
              }
              secCurr = secCurr->next;
            }
          }
          railConnections[railCount] = curr->v;

          railCount++;
        }
      }
      curr = curr->next;
    }

    for (i = 0; i < railCount; ++i)
    {
      if (!visited[railConnections[i]])
      {
        locations[j] = railConnections[i];
        visited[railConnections[i]] = TRUE;
        j++;
      }
    }
  }

  if (sea == TRUE)
  {
    LocationID seaConnections[NUM_MAP_LOCATIONS];
    int seaCount = 0;
    VList curr = currentView->map->connections[from];

    while (curr != NULL)
    {
      if (curr->type == BOAT)
      {
        seaConnections[seaCount] = curr->v;

        seaCount++;
      }
      curr = curr->next;
    }

    for (i = 0; i < seaCount; ++i)
    {
      if (!visited[seaConnections[i]])
      {
        locations[j] = seaConnections[i];
        visited[seaConnections[i]] = TRUE;
        j++;
      }
    }
  }

  *numLocations = j;

  /* //for checking array and size
    for (int i = 0; i < j; ++i)
    {
      printf("%d\n", locations[i]);
    }

    printf("%d\n", j);
  */

  return locations;
}

int findPathToLocation (GameView g, LocationID start, LocationID end, LocationID path[], TransportID trans[]) {
   int citiesToDest = shortestPath(g->map, start, end, path, trans);
   return citiesToDest;
}

static int findLatestPlayer (char c) {
   PlayerID currPlayer;
   switch (c) {
      case 'G':
         currPlayer = PLAYER_LORD_GODALMING;
      break;
      case 'S':
         currPlayer = PLAYER_DR_SEWARD;
      break;
      case 'H':
         currPlayer = PLAYER_VAN_HELSING;
      break;
      case 'M':
         currPlayer = PLAYER_MINA_HARKER;
      break;
      case 'D':
         currPlayer = PLAYER_DRACULA;
      break;
      default:
         currPlayer = NO_PLAYER;
   }

   return currPlayer;
}

static void fillPlayerTrails (GameView g, char *pastPlays, int length) {
   int i;
   int j;
   //First fill arrays with UNKNOWN LOCATION
   for (i = 0; i < NUM_PLAYERS; i++) {
      for (j = 0; j < TRAIL_SIZE; j++) g->playerLocations[i][j] = UNKNOWN_LOCATION;
   }
   
   //Now fill the trail array with past moves
   //The i variable in the loop indexes the first char for each character,
   //e.g. G for a Goldalming turn
   for (i = length - CHAR_PER_TURN + 1, j = 0; j < TRAIL_SIZE * NUM_PLAYERS
        && i >= 0; i -= CHAR_PER_TURN, j++) {

      PlayerID currPlayer = findLatestPlayer(pastPlays[i]);

      if (currPlayer == PLAYER_DRACULA) {
         if (pastPlays[i+1] == 'C' && pastPlays[i+2] == '?') {
            g->playerLocations[PLAYER_DRACULA][j] = CITY_UNKNOWN;
         } else if (pastPlays[i+1] == 'S' && pastPlays[i+2] == '?') {
            printf("Hello\n");
            printf("j is %d\n", j);
            g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = SEA_UNKNOWN;
         } else if (pastPlays[i+1] == 'H' && pastPlays[i+2] == 'I') {
            g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = HIDE;
         } else if (pastPlays[i+1] == 'D' && (pastPlays[i+2] >= '1' || 
                    pastPlays[i+2] <= '5')) {
            int number = ((int) pastPlays[i+2]) - CHAR_TO_INT_OFFSET;
            switch (number) {
               case 1: 
                  g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = DOUBLE_BACK_1; break;
               case 2:  
                  g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = DOUBLE_BACK_2; break;
               case 3:  
                  g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = DOUBLE_BACK_3; break;
               case 4:  
                  g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = DOUBLE_BACK_4; break;
               case 5:  
                  g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = DOUBLE_BACK_5; break;
               default:  
                  g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = NOWHERE;
            }
         } else if (pastPlays[i+1] == 'T' && pastPlays[i+2] == 'P') {
            g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = TELEPORT;
         } else {
            char abbrev[3] = {pastPlays[i+1], pastPlays[i+2], '\0'};
            g->playerLocations[PLAYER_DRACULA][j/NUM_PLAYERS] = abbrevToID(abbrev);
         }
      } else if (currPlayer != NO_PLAYER) {
         char abbrev[3] = {pastPlays[i+1], pastPlays[i+2], '\0'};
         g->playerLocations[currPlayer][j/NUM_PLAYERS] = abbrevToID(abbrev);
      } else {
         break;
      }
   }
}

static int findPlayerHealth (GameView g, char *pastPlays, int length) {
   int i;
   int j;
   int numDeaths = 0;
   //First, give all players their starting health amounts
   for (i = 0; i < NUM_PLAYERS-1; i++) {
      g->health[i] = GAME_START_HUNTER_LIFE_POINTS;
   }
   g->health[PLAYER_DRACULA] = GAME_START_BLOOD_POINTS;
   //Now, iterate through the past plays string to see if health has been lost
   for (i = 0; i < length; i += CHAR_PER_TURN) {
      PlayerID currPlayer = findLatestPlayer(pastPlays[i]);

      if (currPlayer == PLAYER_DRACULA) {
         
         //If Dracula is in his castle he gains health
         if (pastPlays[i+1] == 'T' || 
                   (pastPlays[i+1] == 'C' && pastPlays[i+2] == 'D')) {
            g->health[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
         //If Dracula does a double back move he may be in a sea
         } else if (pastPlays[i+1] == 'D' && 
                   (pastPlays[i+2] >= '1' || pastPlays[i+2] <= '9')) {
            int offset = -1 + (((int) pastPlays[i+2]) - CHAR_TO_INT_OFFSET)
                            * NUM_PLAYERS * CHAR_PER_TURN;
            if (isPlaceSea(pastPlays[i-offset], pastPlays[i-offset+1])) {
               g->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
            }
         //If Dracula ended his turn on water, he loses life
         } else if (isPlaceSea(pastPlays[i+1], pastPlays[i+2])) {
            g->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
         } 

      } else if (currPlayer != NO_PLAYER) {
         //This loop will detract health for actions 
         //(encountering traps or encountering Dracula)
         //printf("Health of %d is %d\n", currPlayer, g->health[currPlayer]);
         for (j = 3; j < CHAR_PER_TURN-1 ; j++) {
            if (pastPlays[j+i] == 'D') {
               g->health[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
               g->health[currPlayer] -= LIFE_LOSS_DRACULA_ENCOUNTER;
            } else if (pastPlays[j+i] == 'T') {
               g->health[currPlayer] -= LIFE_LOSS_TRAP_ENCOUNTER;
            }
         }
         //Hunter gains health if they rest in a city
         j = i - (CHAR_PER_TURN * NUM_PLAYERS);
         if (pastPlays[i+1] == pastPlays[j+1] && pastPlays[i+2] == pastPlays[j+2]) {
            (g->health[currPlayer]) += LIFE_GAIN_REST;
            if (g->health[currPlayer] > GAME_START_HUNTER_LIFE_POINTS) {
               g->health[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
            }
         }
         //Tests if hunter has died during the turn
         if (g->health[currPlayer] < 0) {
            numDeaths++;
            g->health[currPlayer] = GAME_START_HUNTER_LIFE_POINTS;
         }
      } else {
         return 0;
      }

   }

   return numDeaths;
}

static int maturedVampires (GameView g, char *pastPlays, int length) {
   int numMatured = 0;
   int i;
   //Finds the first Dracula turn
   for (i = 0; (pastPlays[i] != 'D') && (i < length); i += CHAR_PER_TURN);
   //If there is no Dracula turn, there must be 0 matured vampires,
   //so return 0
   if (i == 0) return 0;
   //Otherwise, check each of Dracula's turns to see if a vampie matured
   //The i+= 5 is because the "vampire matured" key character is in 
   //the 6th spot in the dracula turn string
   for (i += 5; i < length; i += (CHAR_PER_TURN * NUM_PLAYERS)) {
      if (pastPlays[i] == 'v') numMatured++;
   }
   return numMatured;
}

//The i in this function is the index in pastPlays where the possible sea
//move occurs
static int isPlaceSea (char c1, char c2) {
   int isSea;

   if (c1 == 'S' && c2 == '?') {
      isSea = TRUE;
   } else if ((c1 == 'C' && c2 == '?') || (c1 == 'H' && c2 == 'I') ||
              (c1 == 'T' && c2 == 'P')) {
      isSea = FALSE;
   } else {
      char abbrev[3] = {c1, c2, '\0'};
      LocationID place = abbrevToID(abbrev);
      if (idToType(place) == SEA) {
         isSea = TRUE;
      } else {
         isSea = FALSE;
      }
   }
   return isSea;
}
