#include <stdlib.h>
#include <string.h>

#include "custom_string.h"
#include "mail_check_spam.h"

int main() {
    set_check_word("швейцарские часы");

    str* sender = input_with_allocation();
    if (sender == NULL) {
        return 0;
    }

    str *receiver = input_with_allocation();
    if (receiver == NULL) {
        return 0;
    }

    str *theme = input_with_allocation();
    if (theme == NULL) {
        return 0;
    }

    str *text = input_with_allocation();
    if (text == NULL) {
        return 0;
    }

    mail *my_mail = malloc(sizeof(mail));
    if (my_mail == NULL) {
        return 0;
    }
    fill_mail_data(my_mail, sender, receiver, theme, text);

    errors err = check_mail_data(my_mail);
    if (err != SUCCESSFUL) {
        free_mail(my_mail);
        exit(EXIT_FAILURE);
    }

    free_str(check_word);

    err = write_mail_data(my_mail);
    if (err != SUCCESSFUL) {
        free_mail(my_mail);
        exit(EXIT_FAILURE);
    }

    err = free_mail(my_mail);
    if (err != SUCCESSFUL) {
        exit(EXIT_FAILURE);
    }

    return 0;
}
