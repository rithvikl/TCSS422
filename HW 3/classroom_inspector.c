#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "classroom_inspector.h"
#include "util.h"

void inspect_classroom(const char * building, const char * room, const char * time_schedule_url, int retrieval_threads, int analysis_threads) {

   // A small example of using the two functions in util.h.
   char buffer[80], * content;

   if (getMIMEType(time_schedule_url, buffer, 80)) {
      printf("%s\n\n", buffer);
/*
      if ((content = getContent(time_schedule_url)) != NULL) {
         printf("%s\n", content);
         free(content);
      }
*/
      
      if ((content = getContent(time_schedule_url)) != NULL) {
         char *parse = content;
	 while(strncmp(parse, "<LI>", 4) != 0 && strncmp(parse, "<li>", 4) != 0) parse++;
	 char *found_url = calloc(1, 50*sizeof(char));
	 strncpy(found_url, parse, 51);
	 printf("%s\n", found_url);
         //strncpy url into char *
	 //get content of that URL
	 //parse that url for desired room
         //printf("%s\n", parse); 
         free(content);
      }

/*
      if ((content = getContent("http://www.washington.edu/students/timeschd/T/SPR2015/turb.html")) != NULL) {
         printf("%s\n", content);
         free(content);
      }
*/
   }
}
