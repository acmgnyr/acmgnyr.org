// PegGame.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>


/* ********************************************************************
Peg Jumping Game
solution by Adam Florence

You are given a 7-by-7 board of holes. Some holes are filled with
pegs, and some are not. You may jump a peg over an adjacent peg,
as long as the hole the jumping peg lands in is unoccupied. The
jumped peg is removed. Your goal is to leave the board with only
one peg in it, and the peg must end up in the specified location.

The board is specified as a 7-by-7 array of characters, with the
following meanings:
   x : this hole may never be occupied by a peg
   e : this hole is initially empty
   o : this hole is initially occupied by a peg
   E : this hole is initially empty, and the last peg should end here
   O : this hole is initially occupied, and the last peg should end here
We are guaranteed that exactly one 'E' or 'O' will appear, and that one
or more 'o' or 'O' will appear.

This web site has many of these games, and demonstrations of possible
solutions:
   http://euler.free.fr/PegSolitaireSolved.htm
Be aware that this site defines a "move" as multiple sequential jumps
by the same peg, and seeks to minimize the total number of moves. Our
problem just seeks any solution.
******************************************************************** */

using namespace std;

ifstream fin;

// ----------------------------------------------------------------- //

// The state of our board
class State
{
public:
   State(void);
   // Default dtor, copy ctor, operator= okay.

   // Try a forward move. Return false if it fails.
   inline bool forward(void);

   // Try a backward move. Return false if it fails.
   inline bool backward(void);

   // Return whether or not board is in winning configuration.
   inline bool State::win(void) const;

   // Print all the moves. Should only be called if win is true.
   inline void State::print(void) const;

private:
   static bool State::forward_helper(int curr, int dir, int& jump, int& land);

   inline void increment(void);
   inline void increment_p(void);

   vector<int> move_p, move_d, move_j, move_l;
   vector<bool> pegs, allow;
   int orig; // original number of pegs
   int num;  // number of pegs remaining
   int last; // where the last peg should end up
   int next_peg, next_dir;
};

State::State(void)
{
  pegs.resize(49);
  allow.resize(49);

  // Read the board. Also count number of pegs.
  num = 0;
  for (int i = 0, index = 0; i < 7; ++i)
     {
     for (int j = 0; j < 7; ++j, ++index)
        {
        char x;
        fin >> x;
        switch (x)
           {
           case 'x' : pegs[index] = false; allow[index] = false; break;
           case 'o' : pegs[index] = true;  allow[index] = true;  break;
           case 'e' : pegs[index] = false; allow[index] = true;  break;
           case 'O' : pegs[index] = true;  allow[index] = true;  last = index;
              break;
           case 'E' : pegs[index] = false; allow[index] = true;  last = index;
              break;
           default :  cout << "Bad input " << x << endl; exit(-1);
           }
        if (pegs[index]) ++num;
        }
     }
  orig = num;

  move_p.resize(num);
  move_d.resize(num);
  move_j.resize(num);
  move_l.resize(num);

  next_peg = 0;
  next_dir = 0;
}

// Try a forward move; that is, see if a peg can be jumped by another peg.
bool State::forward(void)
   {
   const int index = orig - num;

   if (num == 1) return false;

   while (next_peg < 49)
      {
      int jump, land;
      if (! pegs[next_peg])
         // Current space is empty. Of course we can't do anything.
         increment_p();
      else if (forward_helper(next_peg, next_dir, jump, land)
         // Make sure (1) position to be jumped is occupied,
         // (2) position we land in is unoccupied,
         // (3) position we land in is allowed.
         && pegs[jump] && !pegs[land] && allow[land])
            {
            // Save the current state.
            move_p[index] = next_peg;
            move_d[index] = next_dir;
            move_j[index] = jump;
            move_l[index] = land;

            // Make the move.
            pegs[next_peg] = false;
            pegs[jump] = false;
            pegs[land] = true;
            --num;
            next_peg = 0;
            next_dir = 0;

            return true;
            }
      else
         // Couldn't make this move. Increment and try again.
         increment();
      }

   // No more forward moves are possible.
   return false;
   }

// Try a forward move with the current peg and direction.
bool State::forward_helper(int curr, int dir, int& jump, int& land)
   {
   // Current position in (x,y) coordinates.
   const int curr_x = curr % 7;
   const int curr_y = curr / 7;

   // Compute position that is jumped, and position where we land.
   int jump_x = curr_x;
   int jump_y = curr_y;
   int land_x = curr_x;
   int land_y = curr_y;
   switch (dir)
      {
      case 0: // left
         jump_x = curr_x - 1;
         land_x = curr_x - 2;
         break;
      case 1: // up
         jump_y = curr_y - 1;
         land_y = curr_y - 2;
         break;
      case 2: // right
         jump_x = curr_x + 1;
         land_x = curr_x + 2;
         break;
      case 3: // down
         jump_y = curr_y + 1;
         land_y = curr_y + 2;
         break;
      default:
         cout << "Bad direction " << dir << endl;
         exit(-1);
      }

   // Make sure we didn't jump off the board.
   if ((land_x < 0) || (land_x >= 7) ||
       (land_y < 0) || (land_y >= 7)) return false;

   // It's a winning move.
   jump = jump_x + 7 * jump_y;
   land = land_x + 7 * land_y;
   return true;
   }

// Try a backward move; that is, undo the last jump.
bool State::backward(void)
   {
   const int index = orig - num - 1;
   if (index < 0) return false;

   next_peg = move_p[index];
   next_dir = move_d[index];
   const int jump = move_j[index];
   const int land = move_l[index];
   ++num;
   pegs[next_peg] = true;
   pegs[jump] = true;
   pegs[land] = false;

   // Increment.
   increment();

   return true;
   }

void State::increment(void)
   {
   ++ next_dir;
   if (next_dir == 4)
      increment_p();
   }

void State::increment_p(void)
   {
   next_dir = 0;
   ++ next_peg;
   }

// Return whether or not board is in winning configuration.
bool State::win(void) const
   {
   return (num == 1) && pegs[last];
   }

void State::print(void) const
   {
   for (int i = 0; i < orig-1; ++i)
      {
      const int orig = move_p[i];
      const int land = move_l[i];
      const int orig_x = orig % 7 + 1;
      const int orig_y = orig / 7 + 1;
      const int land_x = land % 7 + 1;
      const int land_y = land / 7 + 1;
      cout << (i+1) << ". (" << orig_x << ", " << orig_y << ") to ("
           << land_x << ", " << land_y <<")" << endl;
      }
   }

// ----------------------------------------------------------------- //

void go(int dataset)
{
  cout << "Dataset " << dataset << ":" << endl; 

  // Read the board configuration.
  State s;

  // Depth first search.
  while (1)
     {
     if (s.forward())
        {
        // We took a forward move. See if we're in a winning state.
        if (s.win())
           {
           // We're in the winning state.
           s.print();
           return;
           }
        }
     else if (s.backward())
        {
        // Do nothing.
        }
     else
        {
        // Can't go forward or backward. We're done searching.
        break;
        }
     }

  // There is no solution.
  cout << "No solution." << endl;
}

// ----------------------------------------------------------------- //

int main(void)
{
  // open input
  fin.open("c.in");
  if(!fin.is_open()){
      cout << "Can't open c.in" << endl;
      return 1;
  }
  // Read the number of datasets to follow.
  int num;
  fin >> num;

  // Do identical processing for each dataset.
  for (int i = 1; i <= num; ++i)
  {
    go(i);
    cout << endl;
  }
  fin.close();


  return 0;
}

