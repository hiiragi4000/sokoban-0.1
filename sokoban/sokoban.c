// by Yunghsien Chung (hiiragi4000)

// This file defines the functions declared in 'sokoban.h'.
// You don't need to know much about how it works.

#include"sokoban.h"
#include<assert.h>
#include<stdbool.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>

int const Di[]={-1, 1, 0, 0}, Dj[]={0, 0, -1, 1};

void VM_init(Vector_Move *this){
   this->data = NULL;
   this->size = this->capacity = 0;
}

#define SOKOBAN_VM_ALLOC do{\
   if(this->size == this->capacity){\
      if(this->capacity == 0){\
         this->data = malloc(sizeof*this->data);\
         this->capacity = 1;\
      }else{\
         this->data = realloc(this->data, 2*this->capacity*sizeof*this->data);\
         this->capacity *= 2;\
      }\
   }\
}while(0)

void VM_push_back(Vector_Move *this, Move mv){
   SOKOBAN_VM_ALLOC;
   this->data[this->size++] = mv;
}

void VM_emplace_back(Vector_Move *this, Direction dir, bool push){
   SOKOBAN_VM_ALLOC;
   this->data[this->size++] = (Move){dir, push};
}

#undef SOKOBAN_VM_ALLOC

void VM_pop_back(Vector_Move *this){
   if(this->size-- == 0){
      assert(false);
   }
}

void VM_clear(Vector_Move *this){
   this->size = 0;
}

bool Board_read(Board *this, FILE *fin){
   if(fscanf(fin, "%d%d ", &this->height, &this->width) != 2){
      return false;
   }
   this->m_cnt = 0;
   for(int i=0; i<=this->height-1; ++i){
      if(!fgets(this->board[i], WIDTH_MAX+2, fin)){
         assert(0);
      }
      this->board[i][this->width] = 0;
      for(int j=0; j<=this->width-1; ++j) switch(this->board[i][j]){
         case '@':
         case '+':
            this->p_row = i, this->p_col = j;
            break;
         case '$':
            ++this->m_cnt;
            break;
      }
   }
   return true;
}

bool Board_inside(Board const *this, int i, int j){
   return 0<=i && i<=this->height-1 && 0<=j && j<=this->width-1 && this->board[i][j]!='#';
}

bool Board_do_move(Board *this, Direction dir, Vector_Move *history){
   if(!Board_inside(this, this->p_row+Di[dir], this->p_col+Dj[dir])){
      return false;
   }
   char *curr = &this->board[this->p_row][this->p_col], *next = &this->board[this->p_row+Di[dir]][this->p_col+Dj[dir]];
   // "@-", "@.", "+-", "+."
   if(*next=='-' || *next=='.'){
      *curr = (*curr=='@')? '-': '.';
      *next = (*next=='-')? '@': '+';
      this->p_row += Di[dir], this->p_col += Dj[dir];
      if(history){
         VM_emplace_back(history, dir, false);
      }
      return true;
   }
   // "@$?", "@*?", "+$?", "+*?"
   if(!Board_inside(this, this->p_row+2*Di[dir], this->p_col+2*Dj[dir])){
      return false;
   }
   char *nnext = &this->board[this->p_row+2*Di[dir]][this->p_col+2*Dj[dir]];
   if(*nnext=='$' || *nnext=='*'){
      return false;
   }
   *curr = (*curr=='@')? '-': '.';
   if(*next == '$'){
      *next = '@';
      --this->m_cnt;
   }else *next = '+';
   if(*nnext == '-'){
      *nnext = '$';
      ++this->m_cnt;
   }else *nnext = '*';
   this->p_row += Di[dir], this->p_col += Dj[dir];
   if(history){
      VM_emplace_back(history, dir, true);
   }
   return true;
}

void Board_undo_move(Board *this, Move mv){
   assert(Board_inside(this, this->p_row-Di[mv.dir], this->p_col-Dj[mv.dir]));
   char *curr = &this->board[this->p_row][this->p_col], *prev=&this->board[this->p_row-Di[mv.dir]][this->p_col-Dj[mv.dir]];
   if(*prev == '-'){
      *prev = '@';
   }else{
      assert(*prev == '.');
      *prev = '+';
   }
   if(mv.push){
      assert(Board_inside(this, this->p_row+Di[mv.dir], this->p_col+Dj[mv.dir]));
      char *next = &this->board[this->p_row+Di[mv.dir]][this->p_col+Dj[mv.dir]];
      if(*curr == '@'){
         *curr = '$';
         ++this->m_cnt;
      }else{
         *curr = '*';
      }
      if(*next == '$'){
         *next = '-';
         --this->m_cnt;
      }else{
         assert(*next == '*');
         *next = '.';
      }
   }else *curr = (*curr=='@')? '-': '.';
   this->p_row -= Di[mv.dir], this->p_col -= Dj[mv.dir];
}
