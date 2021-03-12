#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "mail_check_spam.h"

int main() {
    char *new_sender = input_with_allocation();
    if (new_sender == NULL) {
        return 0;
    }

    char *new_receiver = input_with_allocation();
    if (new_receiver == NULL) {
        return 0;
    }
    char *new_theme = input_with_allocation();
    if (new_theme == NULL) {
        return 0;
    }

    char *new_text = input_with_allocation();
    if (new_text == NULL) {
        return 0;
    }

    mail *my_mail = malloc(sizeof(mail));
    if (my_mail == NULL) {
        return 0;
    }

    errors err = check_and_fill_mail_data(my_mail, new_sender, new_receiver, new_theme, new_text);
    if (err != SUCCESSFUL) {
        exit(EXIT_FAILURE);
    }

    free(new_sender);
    free(new_receiver);
    free(new_theme);
    free(new_text);

    err = write_mail_data(my_mail);
    if (err != SUCCESSFUL) {
        exit(EXIT_FAILURE);
    }

    err = free_mail(my_mail);
    if (err != SUCCESSFUL) {
        exit(EXIT_FAILURE);
    }

    return 0;
}
