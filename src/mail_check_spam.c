#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mail_check_spam.h"

char check_word[] = "швейцарские часы";

errors write_mail_data(mail *my_mail) {
    if (my_mail == NULL || my_mail->sender == NULL || my_mail->receiver == NULL ||
        my_mail->theme == NULL || my_mail->text == NULL) {
        return ERR_INVALID_INPUT;
    }

    printf("sender: %s\n", my_mail->sender);
    printf("receiver: %s\n", my_mail->receiver);
    printf("theme: %s\n", my_mail->theme);
    printf("text: %s\n", my_mail->text);

    return SUCCESSFUL;
}

errors check_and_fill_mail_data(mail *my_mail, const char *sender_check, const char *receiver_check, const char *theme_check, const char *text_check) {
    if (my_mail == NULL || sender_check == NULL || receiver_check == NULL ||
        theme_check == NULL || text_check == NULL) {
        return ERR_INVALID_INPUT;
    }

    if (strstr(sender_check, check_word)) {
        printf("error, check word found in sender\n");
        return ERR_CHECK_WORD_FOUND;
    } else {
        my_mail->sender = (char *) malloc(strlen(sender_check) * sizeof(char) + 1);
        if (my_mail->sender == NULL) {
            return ERR_BAD_ALLOC;
        }
        strcpy(my_mail->sender, sender_check);
    }

    my_mail->receiver = (char *) malloc(strlen(receiver_check) * sizeof(char) + 1);
    if (my_mail->receiver == NULL) {
        free(my_mail->sender);
        return ERR_BAD_ALLOC;
    }
    strcpy(my_mail->receiver, receiver_check);

    if (strstr(theme_check, check_word)) {
        printf("error, check word found in theme\n");
        free(my_mail->sender);
        free(my_mail->receiver);
        return ERR_CHECK_WORD_FOUND;
    } else {
        my_mail->theme = (char *) malloc(strlen(theme_check) * sizeof(char) + 1);
        if (my_mail->theme == NULL) {
            free(my_mail->sender);
            free(my_mail->receiver);
            return ERR_BAD_ALLOC;
        }
        strcpy(my_mail->theme, theme_check);
    }

    if (strstr(text_check, check_word)) {
        printf("error, check word found in text\n");
        free(my_mail->sender);
        free(my_mail->receiver);
        free(my_mail->theme);
        return ERR_CHECK_WORD_FOUND;
    } else {
        my_mail->text = (char *) malloc(strlen(text_check) * sizeof(char) + 1);
        if (my_mail->text == NULL) {
            free(my_mail->sender);
            free(my_mail->receiver);
            free(my_mail->theme);
            return ERR_BAD_ALLOC;
        }
        strcpy(my_mail->text, text_check);
    }

    return SUCCESSFUL;
}

errors free_mail(mail *my_mail) {
    if (my_mail == NULL) {
        return ERR_INVALID_INPUT;
    }

    if (my_mail->sender != NULL) {
        free(my_mail->sender);
        my_mail->sender = NULL;
    }

    if (my_mail->receiver != NULL) {
        free(my_mail->receiver);
        my_mail->receiver = NULL;
    }

    if (my_mail->theme != NULL) {
        free(my_mail->theme);
        my_mail->theme = NULL;
    }

    if (my_mail->text != NULL) {
        free(my_mail->text);
        my_mail->text = NULL;
    }
    free(my_mail);

    return SUCCESSFUL;
}