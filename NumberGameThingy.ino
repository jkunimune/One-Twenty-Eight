/*
  NumberGameThingy.pde
  by Justin Kunimune
 
 Example file using the The Meggy Jr Simplified Library (MJSL)
  from the Meggy Jr RGB library for Arduino
   
 That weird number game people are playing now.
   
   
 
 Version 1.25 - 12/2/2008
 Copyright (c) 2008 Windell H. Oskay.  All right reserved.
 http://www.evilmadscientist.com/
 
 This library is free software: playeryou can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at playeryour option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 playeryou should have received a copy of the GNU General Public License
 along with this library.  If not, see <http://www.gnu.org/licenses/>.
 	  
 */

 
 
 
 

/*
 * Adapted from "Blink,"  The basic Arduino example.  
 * http://www.arduino.cc/en/Tutorial/Blink
 */

#include <MeggyJrSimple.h>
byte board[4][4];      // The 4x4 array of squares
byte boardold[4][4];      // Remembers what the array looked like one turn ago for comparison


void setup()              
{
  MeggyJrSimpleSetup();
  Serial.begin(9600);
  EditColor(1, 10, 0, 0);      // Stretches out the spectrum for longer games
  EditColor(2, 12, 1, 0);
  EditColor(3, 10, 2, 0);
  EditColor(4, 0, 8, 0);
  EditColor(5, 0, 9, 5);
  EditColor(6, 0, 0, 11);
  EditColor(7, 7, 0, 10);
  EditColor(8, 10, 0, 7);
  EditColor(9, 15, 7, 3);
  EditColor(10, 1, 0, 0);
  
  for (int x = 3; x < 5; x ++)      // puts a lone white square in the middle as a title screen
    for (int y = 3; y < 5; y ++)
      DrawPx(x, y, 9);
  DisplaySlate();
  
  while (!Button_A)      // waits for player input
  {
    CheckButtonsDown();
  }
  randomSeed(millis());      // randomseeds the game
  
  reset();      // initializes the game with one block
  SpawnPiece();      // adds another block.  Upon restarting, this piece is provided by natural spawning
  DisplayBoard();      // shows the two blocks
}


void loop()                 
{
  do
    CheckButtonsPress();      // waits for player input
  while ((!Button_Right) && (!Button_Left) && (!Button_Up) && (!Button_Down));      // waits for player input
  
  for (int x = 0; x < 4; x ++)      // saves the previous board
    for (int y = 0; y < 4; y ++)
      boardold[x][y] = board[x][y];
  
  if (Button_Right)      // runs 'swipe' methods in the proper direction
    SwipeRight();
  if (Button_Left)
    SwipeLeft();
  if (Button_Up)
    SwipeUp();
  if (Button_Down)
    SwipeDown();
    
  if (Full() && !Moved())      // you lose if the screen is full and you cannot find another move
    GameOver();
  if (Won())      // fanfares if there is white
    Victory();
    
  if (!Full() && Moved())      // puts a piece down if there is space and you can still move
    SpawnPiece();
  DisplayBoard();      // shows the board
  delay(100);      // delays to prevent accidental double-taps
}


void SwipeUp()
{
  Tone_Start(ToneC4, 100);      // plays a tone corresponding to the 'up' direction
  
  for (int i = 0; i < 4; i ++)      // four times,
    for (int x = 3; x >= 0; x --)      // for all tiles
      for (int y = 2; y >=0; y --)      // except for the top ones,
        if (board[x][y+1] == 0)      // if there is an empty space above it,
        {
          board[x][y+1] = board[x][y];      // slide the tile up
          board[x][y] = 0;
          delay(10);        // and delay and display
          DisplayBoard();    // for easy animation.
        }
        
  for (int x = 3; x >= 0; x --)      // for all tiles
    for (int y = 2; y >=0; y --)      // except for the top ones,
      if (board[x][y])
        if (board[x][y+1] == board[x][y])      // if the color is non-zero and matches the one above it,
        {
          board[x][y+1] ++;      // combine them to form the next color
          board[x][y] = 0;
          delay(10);          // and delay and display
          DisplayBoard();      // for easy animation.
        }
        
/*  for (int i = 0; i < 4; i ++)      // this alters the mechanics slightly,
    for (int x = 3; x >= 0; x --)      // but apparently deviates from the actual game.
      for (int y = 2; y >=0; y --)
        if (board[x][y+1] == 0)
        {
          board[x][y+1] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }*/
}


void SwipeDown()      // same thing but down
{
  Tone_Start(ToneG4, 100);
  
  for (int i = 0; i < 4; i ++)
    for (int x = 0; x < 4; x ++)
      for (int y = 1; y < 4; y ++)
        if (board[x][y-1] == 0)
        {
          board[x][y-1] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
        
  for (int x = 0; x < 4; x ++)
    for (int y = 1; y < 4; y ++)
      if (board[x][y])
        if (board[x][y-1] == board[x][y])
        {
          board[x][y-1] ++;
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
        
/*  for (int i = 0; i < 4; i ++)
    for (int x = 0; x < 4; x ++)
      for (int y = 1; y < 4; y ++)
        if (board[x][y-1] == 0)
        {
          board[x][y-1] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }*/
}


void SwipeLeft()      // same thing but left
{
  Tone_Start(ToneE4, 100);
  
  for (int i = 0; i < 4; i ++)
    for (int x = 1; x < 4; x ++)
      for (int y = 0; y < 4; y ++)
        if (board[x-1][y] == 0)
        {
          board[x-1][y] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
        
  for (int x = 1; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y])
        if (board[x-1][y] == board[x][y])
        {
          board[x-1][y] ++;
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
  
/*  for (int i = 0; i < 4; i ++)
    for (int x = 1; x < 4; x ++)
      for (int y = 0; y < 4; y ++)
        if (board[x-1][y] == 0)
        {
          board[x-1][y] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }*/
}


void SwipeRight()      // same thing but right
{
  Tone_Start(ToneF4, 100);
  
  for (int i = 0; i < 4; i ++)
    for (int x = 2; x >= 0; x --)
      for (int y = 3; y >=0; y --)
        if (board[x+1][y] == 0)
        {
          board[x+1][y] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }

  for (int x = 2; x >= 0; x --)
    for (int y = 3; y >=0; y --)
      if (board[x][y])
        if (board[x+1][y] == board[x][y])
        {
          board[x+1][y] ++;
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
        
/*  for (int i = 0; i < 4; i ++)
    for (int x = 2; x >= 0; x --)
      for (int y = 3; y >=0; y --)
        if (board[x+1][y] == 0)
        {
          board[x+1][y] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }*/
}


void reset()      // initializes the game with one piece.  A second piece will
{                 // usally appear by natural spawning.
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      board[x][y] = 0;
  
  SpawnPiece();
  DisplayBoard();
}


void SpawnPiece()      // places a red piece on a random dark tile
{
  int rx;
  int ry;
  do
  {
    rx = random(4);
    ry = random(4);
  }
  while (board[rx][ry]);
  board[rx][ry] = 1;
  
  for (int x = 0; x < 2; x ++)      // causes the new piece to fade in
    for (int y = 0; y < 2; y ++)
      DrawPx(rx*2+x, ry*2+y, 10);
  DisplaySlate();
  delay(90);
}


void DisplayBoard()
{
  for (int x = 0; x < 4; x ++)        // translates the gameboard to the gameslate
    for (int y = 0; y < 4; y ++)      // and displays
      for (int w = 0; w < 2; w ++)
        for (int h = 0; h < 2; h ++)
          DrawPx(x*2+w, y*2+h, board[x][y]);
  DisplaySlate();
}


boolean Full()      // is the screen full?
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 0)
        return false;
  return true;
}


boolean Won()      // have you made white?
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 9)
        return true;
  return false;
}


boolean Moved()      // does the gameboard look different from when the turn started?
{                    // or did nothing happen?
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] != boardold[x][y])
        return true;
  return false;
}


byte Score()      // determines the greatest color the player attained.
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 8)
        return 8;
        
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 7)
        return 7;
        
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 6)
        return 6;
        
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 5)
        return 5;
        
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 4)
        return 4;
        
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 3)
        return 3;
        
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 2)
        return 2;
        
  return 1;
}


void GameOver()      // Flashes red and buzzes menacingly
{                    // (copied from FireballSurvival)
  DisplayBoard();
  delay(200);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      DrawPx(i, j, Red);
  DisplaySlate();
  Tone_Start(ToneB2,400);
  delay(500);
  Tone_Start(ToneB2,400);
  delay(500);
  Tone_Start(ToneB2,500);
  delay(500);
  
  ClearSlate();
  for (int x = 3; x < 5; x ++)      // shows the highest color the player attained
    for (int y = 3; y < 5; y ++)
      DrawPx(x, y, Score());
  DisplaySlate();
  delay(2000);
  
  reset();
}


void Victory()      // Flashes green and fanfares
{                   // (copied from FireballSurvival)
  DisplayBoard();
  delay(500);
  for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
      DrawPx(i,j,Green);
  DisplaySlate();
  Tone_Start(ToneC5,140);
  delay(140);
  Tone_Start(ToneE5,140);
  delay(140);
  Tone_Start(ToneC6,150);
  delay(150);
  reset();
}
