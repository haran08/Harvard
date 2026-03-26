AVERAGE_W_COUNT = 100.0


def main():
    text = input("Text: ")

    text_grade = check_text_grade(text)

    if text_grade >= 16:
        print("Grade 16+")
    elif text_grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {round(text_grade)}")


# Functions
def check_text_grade(text: str) -> float:
    n_words = 0
    n_sentences = 0
    n_letters = 0

    for i in text:
        match i:
            # end of 'word'
            case ' ':
                n_words += 1

            # end of 'sentence'
            case '.' | '!' | '?':
                n_sentences += 1

            case ',' | '-' | '_' | '\'':
                continue

            case _:
                n_letters += 1
    n_words += 1

    average_le = n_letters * AVERAGE_W_COUNT / n_words
    average_se = n_sentences * AVERAGE_W_COUNT / n_words

    return 0.0588 * average_le - 0.296 * average_se - 15.8


main()
