#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mail_check_spam.h"

void set_check_word(char* s) {
    check_word = make_filled_str(s);
}

errors write_mail_data(mail *my_mail) {
    if (my_mail == NULL || my_mail->sender == NULL || my_mail->receiver == NULL ||
        my_mail->theme == NULL || my_mail->text == NULL) {
        return ERR_INVALID_INPUT;
    }

    printf("sender: %s\n", my_mail->sender->string);
    printf("receiver: %s\n", my_mail->receiver->string);
    printf("theme: %s\n", my_mail->theme->string);
    printf("text: %s\n", my_mail->text->string);

    return SUCCESSFUL;
}

errors fill_mail_data(mail *my_mail, str *sender, str *receiver,
                      str *theme, str *text) {
    if (my_mail == NULL || sender == NULL || receiver == NULL ||
        theme == NULL || text == NULL) {
        return ERR_INVALID_INPUT;
    }

    my_mail->sender = sender;
    if (my_mail->sender == NULL) {
        return ERR_BAD_ALLOC;
    }

    my_mail->receiver = receiver;
    if (my_mail->receiver == NULL) {
        return ERR_BAD_ALLOC;
    }

    my_mail->theme = theme;
    if (my_mail->theme == NULL) {
        return ERR_BAD_ALLOC;
    }

    my_mail->text = text;
    if (my_mail->text == NULL) {
        return ERR_BAD_ALLOC;
    }

    return SUCCESSFUL;
}

errors check_mail_data(mail *my_mail) {
    if (my_mail == NULL) {
        return ERR_INVALID_INPUT;
    }

    if (strstr(my_mail->sender->string, check_word->string)) {
        return ERR_CHECK_WORD_FOUND;
    }

    if (strstr(my_mail->theme->string, check_word->string)) {
        return ERR_CHECK_WORD_FOUND;
    }

    if (strstr(my_mail->text->string, check_word->string)) {
        return ERR_CHECK_WORD_FOUND;
    }

    return SUCCESSFUL;
}

errors free_mail(mail *my_mail) {
    if (my_mail == NULL) {
        return ERR_INVALID_INPUT;
    }

    if (my_mail->sender != NULL) {
        free_str(my_mail->sender);
        my_mail->sender = NULL;
    }

    if (my_mail->receiver != NULL) {
        free_str(my_mail->receiver);
        my_mail->receiver = NULL;
    }

    if (my_mail->theme != NULL) {
        free_str(my_mail->theme);
        my_mail->theme = NULL;
    }

    if (my_mail->text != NULL) {
        free_str(my_mail->text);
        my_mail->text = NULL;
    }
    free(my_mail);

    return SUCCESSFUL;
}
