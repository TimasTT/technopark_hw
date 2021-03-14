#include <gtest/gtest.h>
#include <string>

extern "C" {
    #include <custom_string.h>
    #include <mail_check_spam.h>
}

TEST(CustomStringTest, MakeEmptyStr) {
    str* str = make_empty_str();
    ASSERT_NE(str, nullptr);

    ASSERT_STRCASEEQ(nullptr, str->string);
    ASSERT_EQ(0, str->length);

    free_str(str);
}

TEST(CustomStringTest, MakeFillStr) {
    char s[] = "Timofey";
    str* str = make_filled_str(s);
    ASSERT_NE(str, nullptr);

    ASSERT_STRCASEEQ(s, str->string);
    ASSERT_EQ(strlen(s), str->length);

    free_str(str);
}

/*TEST(CustomStringTestm, InputStr) {
    str* str = input_with_allocation();
    ASSERT_NE(str, nullptr);

    ASSERT_STRCASEEQ(str->string, "Timofey");

    free_str(str);
}*/

TEST(MailCheck, CorrectInput) {
    set_check_word("швейцарские часы");

    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);

    str* sender = make_filled_str("Timofey");
    ASSERT_NE(sender, nullptr);
    str* receiver = make_filled_str("Anton");
    ASSERT_NE(receiver, nullptr);
    str* theme = make_filled_str("Course Project");
    ASSERT_NE(theme, nullptr);
    str* text = make_filled_str("Hello, this is course project");
    ASSERT_NE(text, nullptr);

    ASSERT_EQ(fill_mail_data(my_mail, sender, receiver, theme, text), SUCCESSFUL);
    ASSERT_EQ(check_mail_data(my_mail), SUCCESSFUL);

    ASSERT_EQ(my_mail->sender->length, sender->length);
    ASSERT_EQ(my_mail->receiver->length, receiver->length);
    ASSERT_EQ(my_mail->theme->length, theme->length);
    ASSERT_EQ(my_mail->text->length, text->length);
    ASSERT_STRCASEEQ(my_mail->sender->string, sender->string);
    ASSERT_STRCASEEQ(my_mail->receiver->string, receiver->string);
    ASSERT_STRCASEEQ(my_mail->theme->string, theme->string);
    ASSERT_STRCASEEQ(my_mail->text->string, text->string);

    free_str(check_word);
    ASSERT_EQ(free_mail(my_mail), SUCCESSFUL);
}

TEST(MailCheck, IncorrectInputSender) {
    set_check_word("швейцарские часы");

    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);

    str* sender = make_filled_str("Timofey швейцарские часы");
    ASSERT_NE(sender, nullptr);
    str* receiver = make_filled_str("Anton");
    ASSERT_NE(receiver, nullptr);
    str* theme = make_filled_str("Course Project");
    ASSERT_NE(theme, nullptr);
    str* text = make_filled_str("Hello, this is course project");
    ASSERT_NE(text, nullptr);

    ASSERT_EQ(fill_mail_data(my_mail, sender, receiver, theme, text), SUCCESSFUL);
    ASSERT_EQ(check_mail_data(my_mail), ERR_CHECK_WORD_FOUND);

    free_str(check_word);
    ASSERT_EQ(free_mail(my_mail), SUCCESSFUL);
}

TEST(MailCheck, IncorrectInputTheme) {
    set_check_word("швейцарские часы");

    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);

    str* sender = make_filled_str("Timofey");
    ASSERT_NE(sender, nullptr);
    str* receiver = make_filled_str("Anton");
    ASSERT_NE(receiver, nullptr);
    str* theme = make_filled_str("Course швейцарские часы Project");
    ASSERT_NE(theme, nullptr);
    str* text = make_filled_str("Hello, this is course project");
    ASSERT_NE(text, nullptr);

    ASSERT_EQ(fill_mail_data(my_mail, sender, receiver, theme, text), SUCCESSFUL);
    ASSERT_EQ(check_mail_data(my_mail), ERR_CHECK_WORD_FOUND);

    free_str(check_word);
    ASSERT_EQ(free_mail(my_mail), SUCCESSFUL);
}

TEST(MailCheck, IncorrectInputText) {
    set_check_word("швейцарские часы");

    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);

    str* sender = make_filled_str("Timofey");
    ASSERT_NE(sender, nullptr);
    str* receiver = make_filled_str("Anton");
    ASSERT_NE(receiver, nullptr);
    str* theme = make_filled_str("Course Project");
    ASSERT_NE(theme, nullptr);
    str* text = make_filled_str("Hello, this швейцарские часы is course project");
    ASSERT_NE(text, nullptr);

    ASSERT_EQ(fill_mail_data(my_mail, sender, receiver, theme, text), SUCCESSFUL);
    ASSERT_EQ(check_mail_data(my_mail), ERR_CHECK_WORD_FOUND);

    free_str(check_word);
    ASSERT_EQ(free_mail(my_mail), SUCCESSFUL);
}

TEST(MailCheck, WriteMail) {
    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);

    str* sender = make_filled_str("Timofey");
    ASSERT_NE(sender, nullptr);
    str* receiver = make_filled_str("Anton");
    ASSERT_NE(receiver, nullptr);
    str* theme = make_filled_str("Course Project");
    ASSERT_NE(theme, nullptr);
    str* text = make_filled_str("Hello, this is course project");
    ASSERT_NE(text, nullptr);

    ASSERT_EQ(fill_mail_data(my_mail, sender, receiver, theme, text), SUCCESSFUL);
    ASSERT_EQ(write_mail_data(my_mail), SUCCESSFUL);

    ASSERT_EQ(free_mail(my_mail), SUCCESSFUL);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}