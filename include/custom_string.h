#ifndef INCLUDE_CUSTOM_STRING_H_
#define INCLUDE_CUSTOM_STRING_H_

typedef struct str_t {
    char* string;
    size_t length;
} str;

str* make_empty_str();

str* make_filled_str(const char* s);

str* input_with_allocation();

void free_str(str *s);

#endif  // INCLUDE_CUSTOM_STRING_H_
