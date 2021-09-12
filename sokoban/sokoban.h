// by Yunghsien Chung (hiiragi4000)

// This file defines the structs needed and declares some functions.
// All functions declared in this file are defined in 'sokoban.c'.

#ifndef SOKOBAN_H
#define SOKOBAN_H

#include<stdbool.h>
#include<stdio.h>

// UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3
typedef enum{
   UP, DOWN, LEFT, RIGHT
}Direction;

// push: whether a box is pushed in the move
typedef struct{
   Direction dir;
   bool push;
}Move;

// a dynamic array of |Move|, similar to |std::vector<Move>| in c++
// use |data[i]| to access the i-th element
// we have the access to |data[0...capacity-1]|
// the data are stored in |data[0...size-1]|
typedef struct{
   Move *data;
   int size, capacity;
}Vector_Move;

#define HEIGHT_MAX 15
#define WIDTH_MAX 15
#define AREA_MAX 50
typedef struct{
   int height, width;
   char board[HEIGHT_MAX][WIDTH_MAX+2]; // +2 for LF and NULL characters
   // m_cnt: number of '$'s
   // the player is at (p_row, p_col)
   int m_cnt, p_row, p_col;
}Board;

// initialize |*this|
extern void VM_init(Vector_Move *this);

// append |mv| to the back of |*this|
// similar to |std::vector<Move>::push_back| in c++
extern void VM_push_back(Vector_Move *this, Move mv);

// append |(Move){dir, push}| to the back of |*this|
// similar to |std::vector<Move>::emplace_back| in c++
extern void VM_emplace_back(Vector_Move *this, Direction dir, bool push);

// remove the last element of |*this|
// similar to |std::vector<Move>::pop_back| in c++
extern void VM_pop_back(Vector_Move *this);

// make |*this| empty
// similar to |std::vector<Move>::clear| in c++
extern void VM_clear(Vector_Move *this);

// read a |Board| from |fin|
extern bool Board_read(Board *this, FILE *fin);

// whether (i, j) is inside the board |*this| and is not '#'
extern bool Board_inside(Board const *this, int i, int j);

// make a move on the board |*this| in the direction |dir|
// the function returns true iff |dir| is valid
// if |history| is nonNULL and |dir| is valid, append the move to |*history|
extern bool Board_do_move(Board *this, Direction dir, Vector_Move *history);

// given the last move |mv|, undo the board |*this|
// the function asserts the validity of |mv|
extern void Board_undo_move(Board *this, Move mv);

#endif
