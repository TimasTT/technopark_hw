#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "custom_string.h"

str* make_empty_str() {
    str* new_string = malloc(sizeof(str));
    if (new_string == NULL) {
        return NULL;
    }

     new_string->string = NULL;

    new_string->length = 0;

    return new_string;
}

str* make_filled_str(const char* s) {
    if (s == NULL) {
        return NULL;
    }

    str* new_string = make_empty_str();

    new_string->length = strlen(s);

    new_string->string = malloc(new_string->length * sizeof(char) + 1);
    if (new_string->string == NULL) {
        return NULL;
    }
    snprintf(new_string->string, new_string->length + 1, "%s", s);

    return new_string;
}

str* input_with_allocation() {
    size_t buffer_size = 4;
    size_t buffer_size_divizer = 1;
    size_t offset_in_buffer = 0;
    size_t length;
    str *buffer = make_empty_str();
    buffer->string = malloc(buffer_size * sizeof(char));
    if (buffer->string == NULL) {
        return NULL;
    }

    buffer->string[0] = '\0';
    buffer->length++;

    while (1) {
        if (fgets(buffer->string + offset_in_buffer, buffer_size / buffer_size_divizer, stdin) == NULL) {
            fflush(stdout);
            fscanf(stdin , "%*[^\n]");
            free(buffer);
            return NULL;
        } else {
            length = strlen(buffer->string + offset_in_buffer);
            if (buffer->string[length - 1 + offset_in_buffer] != '\n') {
                buffer_size *= 2;

                str* buffer_tmp = make_empty_str();
                buffer_tmp->string = realloc(buffer->string, buffer_size);
                buffer_tmp->length = buffer_size;
                if (buffer_tmp->string == NULL) {
                    free_str(buffer_tmp);
                    return NULL;
                } else {
                    buffer->string = buffer_tmp->string;
                    buffer->length = buffer_tmp->length;
                    free(buffer_tmp);
                }

                offset_in_buffer += length;
                buffer_size_divizer = 2;  // write to the free part of the buffer
            } else {
                buffer->string[length - 1 + offset_in_buffer] = '\0';
                break;
            }
        }
    }

    return buffer;
}

void free_str(str* s) {
    free(s->string);
    free(s);
}
