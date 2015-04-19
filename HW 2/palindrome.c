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
pthread_mutex_t lock;

int isPal(char *word) {

   int i;
   for(i = 0; i < strlen(word); i++) {
      if(word[i] != word[strlen(word)-1-i])
         return 0;
   }
   return 1;
}

void *run(void* s) {

   //printf("numWords: %d, palsFound: %d, start: %d\n", p->n, *p->pf, p->start);
//   pthread_mutex_lock(&lock);
   int start = (int) s;
   int end = start + (p->n/p->nT);

   printf("%d - %d\n", start, end);

   //Checks start of each twice
   int k, count = 0;
   for(k = start; k < end; k++) {
      if(isPal((p->w)[k])) {
//         pthread_mutex_lock(&lock);
//         printf("%d\n", (p->pf)+count);
         (p->f)[(p->pf)+count] = (p->w)[k];
//         printf("%s\n", (p->f)[(p->pf)+count]);
         count++;
//         pthread_mutex_unlock(&lock);
//         count++;
      }

   }
   p->pf += count;
   printf("palsFound = %d\n", p->pf);
//   pthread_mutex_unlock(&lock);
   return NULL;
}

//should find 38 palindromes
char ** multithreaded_findPalindromes(char ** words, int numOfWords, int * palindromesFound, int numThreads) {

//   char **foundPals = malloc(sizeof(**words));
   char *foundPals[numOfWords];// = malloc(sizeof(**words));
//   *foundPals = malloc(sizeof(*words));
//   foundPals = malloc(sizeof(words));

   pthread_t t[numThreads];
   
//   if(pthread_mutex_init(&lock, NULL) != 0) {
//      printf("Mutex init failed.\n");
//      return NULL;
//   }

   //create struct to pass mult params?
   p = malloc(sizeof(Params));
   p->w = words;
   p->n = numOfWords;
   p->f = foundPals;
   p->pf = 0;
   p->nT = numThreads;

   int x, rc, start = 0;
   for(x = 0; x < numThreads; x++) {
      start = x * (numOfWords/numThreads);
      rc = pthread_create(&t[x], NULL, run, (void*)start);
      if(rc != 0) {
         printf("ERROR: unable to create new thread.\n");
         return NULL;
      }
      pthread_join(t[x], NULL);
   }

/*
   for(x = 0; x < numThreads; x++) {
      pthread_join(t[x], NULL);
   }
*/

   
   *foundPals = *p->f;
   palindromesFound = &(p->pf);
   int i;
   for(i = 0; i < 38; i++){
      printf("%s\n", foundPals[i]);
   }
   printf("%d\n", *palindromesFound);

   return foundPals;
}
