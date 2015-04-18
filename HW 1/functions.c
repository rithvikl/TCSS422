#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functions.h"
#include "lexicon.h"

void multiply(int num1, int denom1, int num2, int denom2, int * rNum, int * rDenom) {

   *rNum = num1 * num2;
   *rDenom = denom1 * denom2;
   int n;
   if(*rNum < *rDenom) n = *rNum;
   else n = *rDenom;
   if (n < 0) n = n * -1;
   while(n > 0) {
      if(*rNum % n == 0 && *rDenom % n == 0) {
         *rNum = *rNum / n; 
         *rDenom = *rDenom / n;
         break;
      }
      n--;
   }
   if (*rNum < 0) {
      if(*rDenom < 0) *rDenom = *rDenom * -1;
   }
   if (*rDenom < 0) {
      *rDenom = *rDenom * -1;
      if(*rNum > 0) *rNum = *rNum * -1;
   }
}

char * rotate(const char * str, int amount) {

   int len = strlen(str);
   char *copy = malloc(len*sizeof(char)); 
   const char *get = str;
   int count = amount % len;
   if(amount < 0) get -= count;
   else if(amount > 0 && count != 0) get += len - count;
   int i;
   for(i = 0; i < len; i++) {
      copy[i] = *get;
      if (*get == str[len-1]) get = str;
      else get++;
   }
   return copy;
}

int readAndDisplayBookInformation(const char * path) {

   FILE *infile = fopen(path, "r");
   if(infile == NULL) return 1;
   char line[80];
   char *token;
   while(fgets(line, 80, infile)) {
      char *newline = strchr(line, '\r');
      if(newline) *newline = 0;
      token = strtok(line, ",");
      printf("\"%s\" ", token);
      token = strtok(NULL, ",");
      printf("by %s ", token);
      token = strtok(NULL, ",");
      printf("(%s)\n", token);
   }
   fclose(infile);
   return 0;   
}

void initializeAndShuffleDeck(struct Card deck[52]) {

   time_t now;
   srand((unsigned) time(NULL));
   struct Card *temp = malloc(sizeof(struct Card));
   int i, count = 0;
   for(i = 2; i < 15; i++) {
      switch(i) {
         case 2: temp->rank = '2'; break;
         case 3: temp->rank = '3'; break;
         case 4: temp->rank = '4'; break;
         case 5: temp->rank = '5'; break;
         case 6: temp->rank = '6'; break;
         case 7: temp->rank = '7'; break;
         case 8: temp->rank = '8'; break;
         case 9: temp->rank = '9'; break;
         case 10: temp->rank = '0'; break;
         case 11: temp->rank = 'J'; break;
         case 12: temp->rank = 'Q'; break;
         case 13: temp->rank = 'K'; break;
         case 14: temp->rank = 'A'; break;
      }
      int j;
      for(j = 0; j < 4; j++) {
         switch(j) {
            case 0: temp->suit = 'c'; break;
            case 1: temp->suit = 'd'; break;
            case 2: temp->suit = 'h'; break;
            case 3: temp->suit = 's'; break;
         }
         deck[count] = *temp;
         count++;
      }
   }
   int x, y;
   struct Card *temp2 = malloc(sizeof(struct Card)); 
   for(x = 51; x > 0; x--) {
      y = rand() % 51;
      *temp2 = deck[x];
      deck[x] = deck[y];
      deck[y] = *temp2;
   }
}

struct ListNode * findWords(const char board[4][4]) {

   struct ListNode *front = (struct ListNode*)malloc(sizeof(struct ListNode));
   front->next = NULL;
   front->word = NULL;
   State curState = (State) malloc(sizeof(st_obj));
   int x, y;
   for(x = 0; x < 4; x++) {
      for(y = 0; y < 4; y++) {
         curState->curBoard[x][y] = 0;
      } 
   }
   int n;
   for(n = 0; n < 16; n++) {
      curState->curWord[n] = '\0';
   }
   int i, j;
   for(i = 0; i < 4; i++) {
      for(j = 0; j < 4; j++) {
         curState->curWord[0] = board[i][j];
         curState->curBoard[i][j] = 1;
         curState->curLen = 1;
         recurse(board, i, j, &front, curState);
         int x, y;
         for(x = 0; x < 4; x++) {
            for(y = 0; y < 4; y++) {
               curState->curBoard[x][y] = 0;
            } 
         } 
      }
   }
   struct ListNode *cur = (struct ListNode*)malloc(sizeof(struct ListNode));
   cur = front;
   while(cur->next->word != NULL) {
      cur = cur->next;
   }
   cur->next = NULL;
   return front;
}

void recurse(const char board[4][4], int row, int col, struct ListNode **front, State curState) {

   if(isWord(curState->curWord)) {
      struct ListNode *temp = malloc(sizeof(struct ListNode));
      temp->word = malloc(curState->curLen*sizeof(char));
      int z;
      for(z = 0; z < curState->curLen; z++) {
         temp->word[z] = curState->curWord[z];
      }
      temp->next = *front;
      *front = temp;
   }
   if(isPrefix(curState->curWord)) {
      if(col < 3) {
         if(curState->curBoard[row][col+1] == 0) {
            curState->curWord[curState->curLen] = board[row][col+1];
            curState->curLen++;
            curState->curBoard[row][col+1] = 1;
            recurse(board, row, col+1, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row][col+1] = 0;
         }
      }
      if(row < 3) {
         if(curState->curBoard[row+1][col] == 0) {
            curState->curWord[curState->curLen] = board[row+1][col];
            curState->curLen++;
            curState->curBoard[row+1][col] = 1;
            recurse(board, row+1, col, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row+1][col] = 0;
         }
      }
      if(row < 3 && col < 3) {
         if(curState->curBoard[row+1][col+1] == 0) {
            curState->curWord[curState->curLen] = board[row+1][col+1];
            curState->curLen++;
            curState->curBoard[row+1][col+1] = 1;
            recurse(board, row+1, col+1, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row+1][col+1] = 0;
         }
      }
      if(col > 0) {
         if(curState->curBoard[row][col-1] == 0) {
            curState->curWord[curState->curLen] = board[row][col-1];
            curState->curLen++;
            curState->curBoard[row][col-1] = 1;
            recurse(board, row, col-1, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row][col-1] = 0;
         }
      }
      if(row > 0) {
         if(curState->curBoard[row-1][col] == 0) {
            curState->curWord[curState->curLen] = board[row-1][col];
            curState->curLen++;
            curState->curBoard[row-1][col] = 1;
            recurse(board, row-1, col, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row-1][col] = 0;
         }
      }
      if(row > 0 && col > 0) {
         if(curState->curBoard[row-1][col-1] == 0) {
            curState->curWord[curState->curLen] = board[row-1][col-1];
            curState->curLen++;
            curState->curBoard[row-1][col-1] = 1;
            recurse(board, row-1, col-1, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row-1][col-1] = 0;
         } 
      }
      if(row < 3 && col > 0) {
         if(curState->curBoard[row+1][col-1] == 0) {
            curState->curWord[curState->curLen] = board[row+1][col-1];
            curState->curLen++;
            curState->curBoard[row+1][col-1] = 1;
            recurse(board, row+1, col-1, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row+1][col-1] = 0;
         } 
      }
      if(row > 0 && col < 3) {
         if(curState->curBoard[row-1][col+1] == 0) {
            curState->curWord[curState->curLen] = board[row-1][col+1];
            curState->curLen++;
            curState->curBoard[row-1][col+1] = 1;
            recurse(board, row-1, col+1, front, curState);
            curState->curLen--;
            curState->curWord[curState->curLen] = '\0';
            curState->curBoard[row-1][col+1] = 0;
         } 
      }
   }
}
