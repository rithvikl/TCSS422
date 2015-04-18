#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "palindrome.h"

typedef struct params {
   char **w;
   int n;
   char **f;
   int *pf;
   int start;
} Params;

pthread_mutex_t lock;

int isPal(char *word) {

   int i;
   for(i = 0; i < strlen(word); i++) {
      if(word[i] != word[strlen(word)-1-i])
         return 0;
   }
   return 1;
}

void *run(void* par) {

   Params *p = (Params *) par;
   
   int k, j = *(p->pf);
   for(k = p->start; k < p->start + 3; k++) {
      if(isPal((p->w)[k])) {
         pthread_mutex_lock(&lock);
         (p->f)[j] = (p->w)[k];
         j++;
         (p->pf)++;
         pthread_mutex_unlock(&lock);
      }
   }
   
   return NULL;
}

//should find 38 palindromes
char ** multithreaded_findPalindromes(char ** words, int numOfWords, int * palindromesFound, int numThreads) {

   char **foundPals = malloc(sizeof(**words));
   *foundPals = malloc(sizeof(*words));
   *palindromesFound = 0;

   pthread_t t = malloc(sizeof(pthread_t)*numThreads);
   
   if(pthread_mutex_init(&lock, NULL) != 0) {
      printf("Mutex init failed.\n");
      return NULL;
   }

   //create struct to pass mult params?
   Params *p = malloc(sizeof(Params));
   p->w = words;
   p->n = numOfWords;
   p->f = foundPals;
   p->pf = palindromesFound;

   int x, rc;
   for(x = 0; x < numThreads; x++) {
      p->start = x * (numOfWords/numThreads);
      rc = Pthread_create(&t[x], NULL, run, p);
      if(rc != 0) {
         printf("ERROR: unable to create new thread.\n");
         return NULL;
      }
   }

   for(x = 0; x < numThreads; x++) {
      Pthread_join(t[x], NULL);
   }

   foundPals = p->f;
   palindromesFound = p->pf;
   
//   run(words, numOfWords, foundPals, palindromesFound);
   
   return foundPals;
}
