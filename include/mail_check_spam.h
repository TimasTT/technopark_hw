#ifndef INCLUDE_MAIL_CHECK_SPAM_H_
#define INCLUDE_MAIL_CHECK_SPAM_H_

#include "custom_string.h"

typedef enum {
    ERR_CHECK_WORD_FOUND,

    ERR_BAD_ALLOC,
    ERR_INVALID_INPUT,

    SUCCESSFUL
} errors;

typedef struct mail_t {
    str* sender;
    str* receiver;
    str* theme;
    str* text;
} mail;

str* check_word;

void set_check_word(char* s);

errors fill_mail_data(mail *my_mail, str *sender_check, str *receiver_check,
                      str *theme_check, str *text_check);

errors check_mail_data(mail *my_mail);

errors write_mail_data(mail *my_mail);

errors free_mail(mail *my_mail);

#endif  // INCLUDE_MAIL_CHECK_SPAM_H_
