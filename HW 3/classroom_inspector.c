#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "classroom_inspector.h"
#include "util.h"

typedef struct results {
  char **urls;
  char **pages;
  char **m_times;
  char **m_classes;
  int mon;
  char **t_times;
  char **t_classes;
  int tue;
  char **w_times;
  char **w_classes;
  int wed;
  char **th_times;
  char **th_classes;
  int thu;
  char **f_times;
  char **f_classes;
  int fri;
  int page_count;
  char *comp;
  const char *build;
  const char *room;
  int r_threads;
  int a_threads;
  int remain;
} Results;

Results *r;

pthread_mutex_t lock;

void pause() {

   int p;
   for(p = 0; p < 20000000; p++);
}

void *get_info (char *parse_page) {

   char *name_getter = parse_page;
   while(strncmp(name_getter, "NAME=", 5) != 0) name_getter--;
   while(strncmp(name_getter, ">", 1) != 0) name_getter++;
   name_getter++;

   int n = 0;
   char *temp_count = name_getter;
   while(strncmp(temp_count, "&", 1) != 0) {
      n++;
      temp_count++;
   }

   char *class_name = calloc(1, 11*sizeof(char));
   strncat(class_name, name_getter, n);
   strcat(class_name, " ");

   while(strncmp(name_getter, "&nbsp", 5) != 0) name_getter++;
   while(strncmp(name_getter, " ", 1) != 0) name_getter++;
   name_getter++;

   strncat(class_name, name_getter, 3);
                        
   char *time_getter = parse_page;
   time_getter -= 11;

   char *time_slot = calloc(1, 12*sizeof(char));
   strncat(time_slot, time_getter, 10);

   char *day_checker = parse_page;
   day_checker -= 18;

   int k;
   for(k = 0; k < 6; k++) {
      if(strncmp(day_checker, "M", 1) == 0) {
         if(r->mon > 0) {
            if(strcmp((r->m_times)[r->mon -1], time_slot) == 0 && 
                strcmp((r->m_classes)[r->mon -1], class_name) == 0) continue;
         }
         (r->m_times)[r->mon] = time_slot;
         (r->m_classes)[r->mon] = class_name;
         (r->mon)++;
      } else if (strncmp(day_checker, "T", 1) == 0) {
         if(strncmp(day_checker, "Th", 2) == 0) {
            if(r->thu > 0) {
               if(strcmp((r->th_times)[r->thu -1], time_slot) == 0 && 
                   strcmp((r->th_classes)[r->thu -1], class_name) == 0) continue;
            }
            (r->th_times)[r->thu] = time_slot;
            (r->th_classes)[r->thu] = class_name;
            (r->thu)++;
         } else {
            if(r->tue > 0) {
               if(strcmp((r->t_times)[r->tue -1], time_slot) == 0 && 
                   strcmp((r->t_classes)[r->tue -1], class_name) == 0) continue;
            }
            (r->t_times)[r->tue] = time_slot;
            (r->t_classes)[r->tue] = class_name;
            (r->tue)++;
         }
      } else if (strncmp(day_checker, "W", 1) == 0) {
         if(r->wed > 0) {
            if(strcmp((r->w_times)[r->wed -1], time_slot) == 0 && 
                strcmp((r->w_classes)[r->wed -1], class_name) == 0) continue;
         }
         (r->w_times)[r->wed] = time_slot;
         (r->w_classes)[r->wed] = class_name;
         (r->wed)++;
      } else if (strncmp(day_checker, "F", 1) == 0) {
         if(r->fri > 0) {
            if(strcmp((r->f_times)[r->fri -1], time_slot) == 0 && 
                strcmp((r->f_classes)[r->fri -1], class_name) == 0) continue;
         }
         (r->f_times)[r->fri] = time_slot;
         (r->f_classes)[r->fri] = class_name;
         (r->fri)++;
      }
      day_checker++;
   }
   return NULL;
}

void *find_room(void* id) {

   char *parse_page;
   int i, t_num = *((int*)id);
   int start = t_num * ((r->page_count) / (r->a_threads));
   int end = start + ((r->page_count) / (r->a_threads)) - 1;

   for(i = start; i <= end; i++) {

      parse_page = (r->pages)[i];
      while(strncasecmp(parse_page, "Created by", 10) != 0) {

         if(strcasecmp(r->build, "ADMC") == 0 || strcasecmp(r->build, "WGB") == 0) {
            if(strncasecmp(parse_page, r->comp, 11) == 0) {
               get_info(parse_page);
            }
         } else {
            if(strncasecmp(parse_page, r->comp, 8) == 0) {
               get_info(parse_page);
            }
         }
         parse_page++;
      }
   }

   pthread_mutex_lock(&lock);
   if(r->remain != 0) {
      parse_page = (r->pages)[(r->page_count)-(r->remain)];
      (r->remain)--;
      
      while(strncasecmp(parse_page, "Created by", 10) != 0) {

         if(strcasecmp(r->build, "ADMC") == 0 || strcasecmp(r->build, "WGB") == 0) {
            if(strncasecmp(parse_page, r->comp, 11) == 0) {
               get_info(parse_page);
            }
         } else {
            if(strncasecmp(parse_page, r->comp, 8) == 0) {
               get_info(parse_page);
            }
         }
         parse_page++;
      }
             
   }
   pthread_mutex_unlock(&lock);
   return NULL;
}

void *download_pages(void* id) {

   char *page;
   int i, t_num = *((int*)id);
   int start = t_num * ((r->page_count) / (r->r_threads));
   int end = start + ((r->page_count) / (r->r_threads)) - 1;

   for(i = start; i <= end; i++) {
      if ((page = getContent((r->urls)[i])) != NULL) {
         (r->pages)[i] = page;         
      }
   }

   pthread_mutex_lock(&lock);
   
   if(r->remain != 0) {
      if ((page = getContent((r->urls)[(r->page_count)-(r->remain) - 1])) != NULL) {
         (r->remain)--;
         (r->pages)[(r->page_count)-(r->remain) - 1] = page;
          
      }
   }
   pthread_mutex_unlock(&lock);
   return NULL;
}

int getNum(char *num) {
   
   char *numParse = num;
   char *res = calloc(1, sizeof(char*));
   int count = 0;
   while(strncmp(numParse, "-", 1) != 0) {
      count++;
      numParse++;
   }
   strncat(res, num, count);
   int val = atoi(res);
   if(val < 800) val += 1200;
   return val;
}

void sort(char** times, char** classes, int count) {

   int i, j;
   for(i = 0; i < count - 1; i++) {
      for(j = 0; j < count - i - 1; j++) {
         int val1 = getNum(times[j]);
         int val2 = getNum(times[j+1]);
         if(val1 > val2) {
            char *temp = times[j];
            times[j] = times[j+1];
            times[j+1] = temp;
            temp = classes[j];
            classes[j] = classes[j+1];
            classes[j+1] = temp;
         }
      }
   }
}

void inspect_classroom(const char * building, const char * room, const char * time_schedule_url, int retrieval_threads, int analysis_threads) {

   char buffer[80], *initial;

   if (getMIMEType(time_schedule_url, buffer, 80)) {

      // Get content of time schedule initial page
      if ((initial = getContent(time_schedule_url)) != NULL) {

         r = calloc(1, sizeof(Results));
         if(r == NULL) EXIT_FAILURE;

         r->urls = calloc(1, sizeof(char*)*100);
         r->pages = calloc(1, sizeof(char*)*100);
         r->m_times = calloc(1, sizeof(char*)*100);
         r->m_classes = calloc(1, sizeof(char*)*100);
         r->t_times = calloc(1, sizeof(char*)*100);
         r->t_classes = calloc(1, sizeof(char*)*100);
         r->w_times = calloc(1, sizeof(char*)*100);
         r->w_classes = calloc(1, sizeof(char*)*100);
         r->th_times = calloc(1, sizeof(char*)*100);
         r->th_classes = calloc(1, sizeof(char*)*100);
         r->f_times = calloc(1, sizeof(char*)*100);
         r->f_classes = calloc(1, sizeof(char*)*100);

         char *parse = initial;
	 r->page_count = 0;
         r->build = building;
         r->room = room;
         
         // Parse initial page
	 while(strncasecmp(parse, "Created by", 10) != 0 && r->page_count < 64) {

            // Find URLs
            while(strncasecmp(parse, "<li><a href=", 12) != 0) parse++;
	    parse+=12;

	    char *counter = parse;
	    int count = 0;

	    while(strncasecmp(counter, ">", 1) != 0) {
	       counter++;
	       count++;
            }

	    char *found_url = calloc(1, count*sizeof(char));
	    strncpy(found_url, parse, count);


            // Save found url in array
	    char *orig_url = "http://www.washington.edu/students/timeschd/T/SPR2015/";
            char *follow = calloc(1, 90*sizeof(char));
            strcat(follow, orig_url);
	    strcat(follow, found_url);
            (r->urls)[r->page_count] = follow;

            (r->page_count)++;
	    parse++;

	 }
                  
         r->mon = 0; 
         r->tue = 0;
         r->wed = 0; 
         r->thu = 0; 
         r->fri = 0;

         r->r_threads = retrieval_threads;
         r->a_threads = analysis_threads;

         r->remain = (r->page_count) % (r->r_threads);
        
         r->comp = calloc(1, 12*sizeof(char));
         strcat(r->comp, building);
         if(strcasecmp(building, "ADMC") == 0) strcat(r->comp, " ");
         else if(strcasecmp(building, "CP") == 0) strcat(r->comp, "   ");
         else strcat(r->comp, "  ");
         strcat(r->comp, room);

         pthread_t thread[retrieval_threads];
         int rv;

         rv = pthread_mutex_init(&lock, NULL);
         if(rv != 0) {
            printf("Unable to create lock.\n");
            EXIT_FAILURE;
         }

         // Download pages into array
         int x;
         for(x = 0; x < retrieval_threads; x++) {
            rv = pthread_create(&thread[x], NULL, download_pages, (void*)&x);
            if(rv != 0) {
               printf("Unable to create threads.\n");
               EXIT_FAILURE;
            }
            pause();
         }

         for(x = 0; x < retrieval_threads; x++) {
            pthread_join(thread[x], NULL);
         }

         r->remain = (r->page_count) % (r->a_threads);

         // Analyze pages for classroom info
         for(x = 0; x < analysis_threads; x++) {
            rv = pthread_create(&thread[x], NULL, find_room, (void*)&x);
            if(rv != 0) {
               printf("Unable to create threads.\n");
               EXIT_FAILURE;
            }
            pause();
         }

         for(x = 0; x < analysis_threads; x++) {
            pthread_join(thread[x], NULL);
         }
         pthread_mutex_destroy(&lock);

         // sort arrays
         sort(r->m_times, r->m_classes, r->mon);
         sort(r->t_times, r->t_classes, r->tue);
         sort(r->w_times, r->w_classes, r->wed);
         sort(r->th_times, r->th_classes, r->thu);
         sort(r->f_times, r->f_classes, r->fri);
         
         printf("Report for %s %s\n\n", building, room);
         if((r->m_times)[0] != NULL) printf("M");
         int z;
         for(z = 0; z < r->mon; z++) {
            printf("\t%s\t", (r->m_times)[z]);
            printf("%s\n", (r->m_classes)[z]);
         }
         if((r->t_times)[0] != NULL) printf("T");
         for(z = 0; z < r->tue; z++) {
            printf("\t%s\t", (r->t_times)[z]);
            printf("%s\n", (r->t_classes)[z]);
         }
         if((r->w_times)[0] != NULL) printf("W");
         for(z = 0; z < r->wed; z++) {
            printf("\t%s\t", (r->w_times)[z]);
            printf("%s\n", (r->w_classes)[z]);
         }
         if((r->th_times)[0] != NULL) printf("Th");
         for(z = 0; z < r->thu; z++) {
            printf("\t%s\t", (r->th_times)[z]);
            printf("%s\n", (r->th_classes)[z]);
         }
         if((r->f_times)[0] != NULL) printf("F");
         for(z = 0; z < r->fri; z++) {
            printf("\t%s\t", (r->f_times)[z]);
            printf("%s\n", (r->f_classes)[z]);
         }
         printf("\n");
      }
   }
}
