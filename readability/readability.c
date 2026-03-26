#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Constant
const float average_words_count = 100.0;

// Prototype
float check_text_grade(string text);

// Main
int main(void)
{
    string text = get_string("Text: ");

    float text_grade = check_text_grade(text);

    if (text_grade >= 16)
        printf("Grade 16+\n");
    else if (text_grade < 1)
        printf("Before Grade 1\n");
    else
        printf("Grade %i\n", (int) round(text_grade));

    return 0;
}

float check_text_grade(string text)
{
    int n_letters = 0;
    int n_sentences = 0;
    int n_words = 0;
    float average_letters_by_words_count;
    float average_sentences_by_words_count;

    for (int i = 0, l = strlen(text); i < l + 1; i++)
    {
        char _ = text[i];

        switch (text[i])
        {
                // characters to end 'words'.
            case ' ':
            case '\0':
                n_words++;
                break;

            case '.':
            case '!':
            case '?':
                n_sentences++;
                break;

            case ',':
            case '-':
            case '_':
            case '\'':
                break;

            default:
                n_letters++;
                break;
        }
    }

    average_letters_by_words_count = n_letters * average_words_count / n_words;
    average_sentences_by_words_count = n_sentences * average_words_count / n_words;

    return 0.0588 * average_letters_by_words_count - 0.296 * average_sentences_by_words_count -
           15.8;
}
