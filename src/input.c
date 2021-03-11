#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

char* input_with_allocation() {
	size_t buffer_size = 64;
    size_t buffer_size_divizer = 1;
    size_t offset_in_buffer = 0;
    size_t length;
    char *buffer = (char*)malloc(buffer_size * sizeof(char));
 
    if (buffer == NULL) {
        return NULL;
    }

    buffer[0] = '\0' ;

    while (1) {
        if (fgets(buffer + offset_in_buffer, buffer_size / buffer_size_divizer, stdin) == NULL) {
            fflush(stdout);
            fscanf (stdin , "%*[^\n]");
            free (buffer);
            return NULL;
        } else {
            length = strlen (buffer + offset_in_buffer);
            if (buffer[length - 1 + offset_in_buffer] != '\n') {
                buffer_size *= 2;

                //char * buffer_tmp = buffer;
                buffer = (char*)realloc(buffer, buffer_size) ;
                if (buffer == NULL) {
                    return NULL;
                }

                offset_in_buffer += length;
                buffer_size_divizer = 2; //write to the free part of the buffer
            } else {
                buffer[length - 1 + offset_in_buffer] = '\0';
                break;
            }
        }
    }

    return buffer;
}