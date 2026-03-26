#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char encript_char(char c, char key);

void encript_text(string text, string key, int count, char result[]);

bool validate_key(string key);

int main(int argc, string argv[])
{
    int key[26];

    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else if (!validate_key(argv[1]))
    {
        return 1;
    }

    string a = get_string("plaintext: ");

    int size = strlen(a) + 1;
    char text[size];
    encript_text(a, argv[1], size, text);

    printf("ciphertext: %s\n", text);

    return 0;
}

char encript_char(char c, char key)
{
    int encript_c, z;

    if (islower(c))
    {
        encript_c = c + (tolower(key) - c);

        z = 'z';
    }
    else
    {
        encript_c = c + (toupper(key) - c);

        z = 'Z';
    }

    while (encript_c > z)
    {
        encript_c -= 26;
    }

    return encript_c;
}

void encript_text(string text, string key, int count, char result[])
{
    for (int i = 0; i < count - 1; i++)
    {
        if (!isalpha(text[i]))
        {
            result[i] = text[i];
            continue;
        }

        if (islower(text[i]))
            result[i] = encript_char(text[i], key[text[i] - 'a']);
        else
            result[i] = encript_char(text[i], key[text[i] - 'A']);
    }

    result[count - 1] = '\0';
}

bool validate_key(string key)
{
    char checked[strlen(key)];
    for (int i = 0, l = strlen(key); i < l; i++)
    {
        if (!isalpha(key[i]))
            return false;

        for (int j = i; j >= 0; j--)
        {
            if (checked[j] == key[i])
                return false;
            else if (isupper(key[i]) && toupper(checked[j]) == key[i])
                return false;
            else if (islower(key[i]) && tolower(checked[j]) == key[i])
                return false;
        }

        checked[i] = key[i];
    }

    return true;
}
