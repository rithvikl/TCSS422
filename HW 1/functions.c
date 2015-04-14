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

//Issues with amount = 5 and 10
char * rotate(const char * str, int amount) {

   char *copy = malloc(sizeof(str)); 
   
   const char *get = str;

   int count = amount % 5;
   
   if(amount <= 0) get -= count;
   else if(amount > 0) get += strlen(str) - count;

   int i;
   for(i = 0; i < strlen(str); i++) {
      copy[i] = *get;
      if (*get == 'o') get = str;
      else get++;
   }

   return copy;
}

int readAndDisplayBookInformation(const char * path) {

   FILE *infile = fopen(path, "r");   
  
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

//How to get random with no duplicates?
void initializeAndShuffleDeck(struct Card deck[52]) {

   time_t now;
   srand((unsigned) time(NULL));
   struct Card *temp = malloc(sizeof(struct Card));

   int i, rNum;
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
//            printf("%d, %d, %c%c ", i, j, temp->rank, temp->suit);
         rNum = rand() % 51;
         while(deck[rNum].suit == 'c' && deck[rNum].suit == 'd' && deck[rNum].suit == 'h' && deck[rNum].suit == 's') {
            rNum = rNum + 1;
            if(rNum == 51) rNum = 0;
         }
         deck[rNum] = *temp;
      }
   }
}

struct ListNode * findWords(const char board[4][4]) {
   
   struct ListNode *front = malloc(sizeof(struct ListNode));
   struct ListNode *prev = malloc(sizeof(struct ListNode));
   char *cur = malloc(17); //should be dynamic
   int i, j;
   for(i = 0; i < 4; i++) {
      for(j = 0; j < 4; j++) {
         cur[0] = board[i][j]; 
         recurse(board, cur, i, j, front, prev); 
      }
   }
//   struct ListNode *test = malloc(sizeof(struct ListNode));
//   test->word = "hello";
//   test->next = NULL;
//   return test;
   return front;
}

void recurse(const char board[4][4], char *current, int row, int col, struct ListNode *front, struct ListNode *prev) {

//   printf("%s\n", current);
   if(current != NULL) {
      if(isWord(current)) {
         struct ListNode *temp = malloc(sizeof(struct ListNode));
         temp->word = current;
         if(front == NULL) {
            front = temp;
            prev = temp;
         } else {
            prev->next = temp;
            prev = prev->next;
         }
      }

      if(isPrefix(current)) {
         if(col < 3) {
            strcat(current, &board[row][col+1]);
            recurse(board, current, row, col+1, front, prev);
         }
         if(row < 3) {
            strcat(current, &board[row+1][col]);
            recurse(board, current, row+1, col, front, prev);
         }
         if(row < 3 && col < 3) {
            strcat(current, &board[row+1][col+1]);
            recurse(board, current, row+1, col+1, front, prev);
         }
         if(col > 0) {
            strcat(current, &board[row][col-1]);
            recurse(board, current, row, col-1, front, prev);
         }
         if(row > 0) {
            strcat(current, &board[row-1][col]);
            recurse(board, current, row-1, col, front, prev);
         }
         if(row > 0 && col > 0) {
            strcat(current, &board[row-1][col-1]);
            recurse(board, current, row-1, col-1, front, prev);
         }
      }

      if(!isWord(current) && !isPrefix(current)) {
         int i = 0;
         while(current[i] != '\0') i++;
         current[i-2] = '\0';
      }
   }
}
