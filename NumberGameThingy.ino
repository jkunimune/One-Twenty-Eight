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
byte board[4][4];
byte boardold[4][4];


void setup()              
{
  MeggyJrSimpleSetup();
  Serial.begin(9600);
  
  for (int x = 3; x < 5; x ++)
    for (int y = 3; y < 5; y ++)
      DrawPx(x, y, White);
  DisplaySlate();
  
  while (!Button_A)
  {
    CheckButtonsDown();
  }
  randomSeed(millis());
  
  reset();
  SpawnPiece();
  DisplayBoard();
}


void loop()                 
{
  do
    CheckButtonsPress();
  while ((!Button_Right) && (!Button_Left) && (!Button_Up) && (!Button_Down));
  
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      boardold[x][y] = board[x][y];
  
  if (Button_Right)
    SwipeRight();
  if (Button_Left)
    SwipeLeft();
  if (Button_Up)
    SwipeUp();
  if (Button_Down)
    SwipeDown();
    
  if (Full() && !Moved())
    GameOver();
  if (Won())
    Victory();
    
  if (!Full() && Moved())
    SpawnPiece();
  DisplayBoard();
  delay(100);
}


void SwipeUp()
{
  Tone_Start(ToneC4, 100);
  
  for (int i = 0; i < 4; i ++)
    for (int x = 3; x >= 0; x --)
      for (int y = 2; y >=0; y --)
        if (board[x][y+1] == 0)
        {
          board[x][y+1] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
        
  for (int x = 3; x >= 0; x --)
    for (int y = 2; y >=0; y --)
      if (board[x][y])
        if (board[x][y+1] == board[x][y])
        {
          board[x][y+1] ++;
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }
        
/*  for (int i = 0; i < 4; i ++)
    for (int x = 3; x >= 0; x --)
      for (int y = 2; y >=0; y --)
        if (board[x][y+1] == 0)
        {
          board[x][y+1] = board[x][y];
          board[x][y] = 0;
          delay(10);
          DisplayBoard();
        }*/
}


void SwipeDown()
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


void SwipeLeft()
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


void SwipeRight()
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


void reset()
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      board[x][y] = 0;
  
  SpawnPiece();
  DisplayBoard();
}


void SpawnPiece()
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
}


void DisplayBoard()
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      for (int w = 0; w < 2; w ++)
        for (int h = 0; h < 2; h ++)
          DrawPx(x*2+w, y*2+h, board[x][y]);
  DisplaySlate();
}


boolean Full()
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 0)
        return false;
  return true;
}


boolean Won()
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] == 7)
        return true;
  return false;
}


boolean Moved()
{
  for (int x = 0; x < 4; x ++)
    for (int y = 0; y < 4; y ++)
      if (board[x][y] != boardold[x][y])
        return true;
  return false;
}


void GameOver()
{
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
  reset();
}


void Victory()
{
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
