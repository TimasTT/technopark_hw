#ifndef INCLUDE_MAIL_CHECK_SPAM_H_
#define INCLUDE_MAIL_CHECK_SPAM_H_

typedef enum {
    ERR_CHECK_WORD_FOUND,

    ERR_BAD_ALLOC,
    ERR_INVALID_INPUT,

    SUCCESSFUL
} errors;

typedef struct mail_t {
    char *sender;
    char *receiver;
    char *theme;
    char *text;
} mail;

errors write_mail_data(mail *my_mail);

errors check_and_fill_mail_data(mail *my_mail, const char *sender_check, const char *receiver_check,
                                const char *theme_check, const char *text_check);

errors free_mail(mail *my_mail);

#endif  // INCLUDE_MAIL_CHECK_SPAM_H_
