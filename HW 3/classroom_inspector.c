#include <stdio.h>
#include <stdlib.h>
#include "classroom_inspector.h"
#include "util.h"

void inspect_classroom(const char * building, const char * room, const char * time_schedule_url, int retrieval_threads, int analysis_threads) {
	// A small example of using the two functions in util.h.
	char buffer[80], * content;

	if (getMIMEType(time_schedule_url, buffer, 80)) {
		printf("%s\n\n", buffer);
		if ((content = getContent(time_schedule_url)) != NULL) {
			printf("%s\n", content);
			free(content);
		}
	}
}
