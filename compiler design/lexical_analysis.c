#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
char complete[1000];
char sentencedelimiter[] = "{;}";
char worddelimiter[] = " [(])=*==,'";

int check_keyword(char *c)
{
    char keywords[32][10] = {"break", "case", "char", "const", "continue", "default",
                             "do", "double", "else", "float", "for", "goto",
                             "if", "int", "long", "return", "short", "signed",
                             "sizeof", "static", "struct", "switch",
                             "unsigned", "void", "while", "FILE"};
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(c, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
int check_variable(char *c)
{
    if (!isalpha(c[0]) && c[0] != '_')
    {
        return 0;
    }
    for (int i = 1; i < strlen(c); i++)
    {
        if (!isalpha(c[i]) && c[i] != '_' && !isdigit(c[i]))
        {
            return 0;
        }
    }
    return 1;
}
int check_constant(char *c)
{
    for (int i = 0; i < strlen(c); i++)

    {
        if (!isdigit(c[i]))
            return 0;
    }
    return 1;
}
int main()
{
    FILE *input;
    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("the input file is empty \n");
        exit(0);
    }

    while (fgets(complete, 1000, input) != NULL)
    {
        printf("<%c,symbol>\n", complete[strlen(complete) - 2]);
        char *sentence = strtok(complete, sentencedelimiter);

        while (sentence != NULL)
        {

            for (int k = 0; k < strlen(sentence) - 1; k++)
            {
                if (!isalpha(sentence[k]) && !isdigit(sentence[k]) && sentence[k] != '_' && sentence[k] != ' ' && sentence[k] != '!' && sentence[k] != '*' && sentence[k] != '/' && sentence[k] != '-' && sentence[k] != '=' && sentence[k] != '+')
                    printf("<%c,symbol>\n", sentence[k]);
                if (sentence[k] == '*' && sentence[k] == '+' && sentence[k] == '-' && sentence[k] == '/' && sentence[k] != '=')
                    printf("<%c,operator>", sentence[k]);
            }
            for (int i = 0; i < strlen(sentence); i++)
            {
                if (sentence[i] == '/' && sentence[i + 1] == '*' || sentence[i] == '/' && sentence[i + 1] == '/')
                {
                    printf("comment line %s\n", sentence);
                    sentence = strtok(NULL, "*/");
                    break;
                }
            }
            char *word = strtok(sentence, worddelimiter); // Split the sentence into words
            while (word != NULL)
            {
                // printf("%s\n",word);
                if (check_keyword(word))
                    printf("<%s,keyword>\n", word);
                else if (check_variable(word))
                {

                    printf("<%s,identifier>\n", word);
                }
                else if (check_constant(word))
                    printf("<%s,constant>\n", word);

                word = strtok(NULL, worddelimiter); // Get the next word
            }
            sentence = strtok(NULL, sentencedelimiter);
        }
    }
    return 0;
}
