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

   int start = (int) s;
   int end = start + (p->n/p->nT);

//   printf("%d - %d\n", start, end);

   
   int k, count = 0;
   for(k = start; k < end; k++) {
      if(isPal((p->w)[k])) {
         /* TODO: p->w is an array of arrays, p->f is an array of arrays.
            So the next line should take the word thats in p->w and
            put it in p->f. After the first few, I get a seg fault. Is that
            because of the **foundPals malloc in multithreaded_palindromes?
            */
         (p->f)[(p->pf)+count] = (p->w)[k];
         count++;
      }

   }
   p->pf += count;
//   printf("palsFound = %d\n", p->pf);
   return NULL;
}

//should find 38 palindromes
char ** multithreaded_findPalindromes(char ** words, int numOfWords, int * palindromesFound, int numThreads) {

   if(words == NULL) return NULL;

   // I think this malloc is the problem?
   char **foundPals = malloc(sizeof(words[0])*numOfWords);

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
      // TODO: Casting start to (void*) gives a warning, but idk how to
      // pass to thread function without casting it?
      rc = pthread_create(&t[x], NULL, run, (void*)start);
      if(rc != 0) {
         printf("ERROR: unable to create new thread.\n");
         return NULL;
      }
      pthread_join(t[x], NULL);
   }

   
   foundPals = p->f;
   palindromesFound = &p->pf;
   int i;
   for(i = 0; i < 38; i++){
        printf("%s\n", *foundPals+i);
   }
//   printf("%d\n", *palindromesFound);

   return foundPals;
}
