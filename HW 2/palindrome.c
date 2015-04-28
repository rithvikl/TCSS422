#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "palindrome.h"

typedef struct params {
   char **w;
   int n;
   char **f;
   int pf;
   int nT;
} Params;

Params *p;

int isPal(char *word) {

   int i;
   for(i = 0; i < strlen(word); i++) {
      if(word[i] != word[strlen(word)-1-i])
         return 0;
   }
   return 1;
}

void *run(void* s) {

   int start = *((int*)s);
   int end = start + (p->n/p->nT);
   int k, count = 0;
   for(k = start; k < end; k++) {
      if(isPal((p->w)[k])) {
         (p->f)[(p->pf)+count] = (p->w)[k];
         count++;
      }

   }
   p->pf += count;
   return NULL;
}

char ** multithreaded_findPalindromes(char ** words, int numOfWords, int * palindromesFound, int numThreads) {

   if(words == NULL) return NULL;

   char **foundPals = malloc(sizeof(char*)*numOfWords);
   if(foundPals == NULL) return NULL;

   pthread_t t[numThreads];
   
   p = malloc(sizeof(Params));
   p->w = words;
   p->n = numOfWords;
   p->f = foundPals;
   p->pf = 0;
   p->nT = numThreads;

   int x, rc, start = 0;
   for(x = 0; x < numThreads; x++) {
      start = x * (numOfWords/numThreads);
      rc = pthread_create(&t[x], NULL, run, (void*)&start);
      if(rc != 0) {
         printf("ERROR: unable to create new thread.\n");
         return NULL;
      }
      pthread_join(t[x], NULL);
   }

   foundPals = p->f;
   *palindromesFound = p->pf;
   return foundPals;
}
