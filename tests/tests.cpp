#include <gtest/gtest.h>

extern "C" {
	#include <input.h>
	#include <mail_check_spam.h>
}

TEST(CheckSpamTest, SuccessfulInput) {
    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);
    char sender[] = "Timofey";
    char receiver[] = "Anton";
    char theme[] = "Course Project";
    char text[] = "Hello, this is course project";
    ASSERT_EQ(check_and_fill_mail_data(my_mail, sender, receiver, theme, text), SUCCESSFUL);
    ASSERT_STRCASEEQ(my_mail->sender, sender);
    ASSERT_STRCASEEQ(my_mail->receiver, receiver);
    ASSERT_STRCASEEQ(my_mail->theme, theme);
    ASSERT_STRCASEEQ(my_mail->text, text);
    free_mail(my_mail);
}

TEST(CheckSpamTest, IncorrectInput) {
    mail *my_mail = (mail*)malloc(sizeof(mail));
    ASSERT_NE(my_mail, nullptr);
    char sender[] = "Timofey";
    char receiver[] = "Anton";
    char theme[] = "Course Project";
    char text[] = "Hello, this швейцарские часы is course project";
    ASSERT_EQ(check_and_fill_mail_data(my_mail, sender, receiver, theme, text), ERR_CHECK_WORD_FOUND);
    free_mail(my_mail);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}