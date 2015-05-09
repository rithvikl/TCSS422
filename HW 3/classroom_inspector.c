#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "classroom_inspector.h"
#include "util.h"

typedef struct results {
  char **urls;
  char **m_times;
  char **m_classes;
  char **t_times;
  char **t_classes;
  char **w_times;
  char **w_classes;
  char **th_times;
  char **th_classes;
  char **f_times;
  char **f_classes;
  int page_count;
} Results;

Results *r;


void inspect_classroom(const char * building, const char * room, const char * time_schedule_url, int retrieval_threads, int analysis_threads) {

   char buffer[80], *initial;

   if (getMIMEType(time_schedule_url, buffer, 80)) {

      // Get content of time schedule initial page
      if ((initial = getContent(time_schedule_url)) != NULL) {

         r = calloc(1, sizeof(Results));
         if(r == NULL) EXIT_FAILURE;

         r->urls = calloc(1, sizeof(char*)*64);
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

         char *page;

         char *comp = calloc(1, 12*sizeof(char));
         strcat(comp, building);
         if(strcasecmp(building, "ADMC") == 0) strcat(comp, " ");
         else strcat(comp, "  ");
         strcat(comp, room);
         
         int mon = 0, tue = 0, wed = 0, thu = 0, fri = 0;
         
         // parse downloaded web page
         int i;
         for(i = 0; i < r->page_count; i++) {
	    if ((page = getContent((r->urls)[i])) != NULL) {

               char *parse_page = page;

               while(strncasecmp(parse_page, "Created by", 10) != 0) {

                  if(strcasecmp(building, "ADMC") == 0) {
                     if(strncasecmp(parse_page, comp, 11) == 0) {
                        // Duplicate code goes here
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
                           // T LIT 200 is listed twice on time schedule
                           if(strncmp(day_checker, "M", 1) == 0) {
                              (r->m_times)[mon] = time_slot;
                              (r->m_classes)[mon] = class_name;
                              mon++;
                           } else if (strncmp(day_checker, "T", 1) == 0) {
                              if(strncmp(day_checker, "Th", 2) == 0) {
                                 (r->t_times)[tue] = time_slot;
                                 (r->t_classes)[tue] = class_name;
                                 tue++;
                              } else {
                                 (r->th_times)[thu] = time_slot;
                                 (r->th_classes)[thu] = class_name;
                                 thu++;
                              }
                           } else if (strncmp(day_checker, "W", 1) == 0) {
                              (r->w_times)[wed] = time_slot;
                              (r->w_classes)[wed] = class_name;
                              wed++;
                           } else if (strncmp(day_checker, "F", 1) == 0) {
                              (r->f_times)[fri] = time_slot;
                              (r->f_classes)[fri] = class_name;
                              fri++;
                           }
                           day_checker++;
                        }
                     }
                  } else {
                     if(strncasecmp(parse_page, comp, 8) == 0) { 
                        // Duplicate code goes here
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
//                        name_getter+=6;
                        while(strncmp(name_getter, " ", 1) != 0) name_getter++;
                        name_getter++;

                        strncat(class_name, name_getter, 3);
                        
//                        printf("%s\n", class_name);

                        char *time_getter = parse_page;
                        time_getter -= 11;

                        char *time_slot = calloc(1, 12*sizeof(char));
                        strncat(time_slot, time_getter, 10);
//                        printf("%s\n", time_slot);

                        char *day_checker = parse_page;
                        day_checker -= 18;
//                        printf("%.6s\n", day_checker);

                        int k;
                        for(k = 0; k < 6; k++) {
                           // T LIT classes always getting added twice
                           if(strncmp(day_checker, "M", 1) == 0) {
                              (r->m_times)[mon] = time_slot;
                              (r->m_classes)[mon] = class_name;
                              mon++;
                           } else if (strncmp(day_checker, "T", 1) == 0) {
                              if(strncmp(day_checker, "Th", 2) == 0) {
                                 (r->t_times)[tue] = time_slot;
                                 (r->t_classes)[tue] = class_name;
                                 tue++;
                              } else {
                                 (r->th_times)[thu] = time_slot;
                                 (r->th_classes)[thu] = class_name;
                                 thu++;
                              }
                           } else if (strncmp(day_checker, "W", 1) == 0) {
                              (r->w_times)[wed] = time_slot;
                              (r->w_classes)[wed] = class_name;
                              wed++;
                           } else if (strncmp(day_checker, "F", 1) == 0) {
                              (r->f_times)[fri] = time_slot;
                              (r->f_classes)[fri] = class_name;
                              fri++;
                           }
                           day_checker++;
                        }
                     }
                  }
                  parse_page++;

	       }
	    }
         }
         printf("----------------\n");
         printf("Report for %s %s\n\n", building, room);
         if((r->m_times)[0] != NULL) printf("M");
         int z;
         for(z = 0; z < mon; z++) {
            printf("\t%s\t", (r->m_times)[z]);
            printf("%s\n", (r->m_classes)[z]);
         }
         if((r->t_times)[0] != NULL) printf("T");
         for(z = 0; z < tue; z++) {
            printf("\t%s\t", (r->t_times)[z]);
            printf("%s\n", (r->t_classes)[z]);
         }
         if((r->w_times)[0] != NULL) printf("W");
         for(z = 0; z < wed; z++) {
            printf("\t%s\t", (r->w_times)[z]);
            printf("%s\n", (r->w_classes)[z]);
         }
         if((r->th_times)[0] != NULL) printf("Th");
         for(z = 0; z < thu; z++) {
            printf("\t%s\t", (r->th_times)[z]);
            printf("%s\n", (r->th_classes)[z]);
         }
         if((r->f_times)[0] != NULL) printf("F");
         for(z = 0; z < fri; z++) {
            printf("\t%s\t", (r->f_times)[z]);
            printf("%s\n", (r->f_classes)[z]);
         }
         printf("\n");
      }
   }
}
