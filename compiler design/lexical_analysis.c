#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main()
{
    FILE *input;
    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("the input file is empty \n");
        exit(0);
    }
    char complete[1000];
    char sentencedelimiter[]={"{;}"};
    char worddelimiter[]={" [(])=*==,"};
    while (fgets(complete, 1000, input) != NULL)
    {
        char *sentence = strtok(complete,sentencedelimiter);
        while (sentence != NULL)
        {
            
            char *word = strtok(sentence, worddelimiter); // Split the sentence into words
            while (word != NULL)
            {
                printf("%s*&^\n", word);            // Print each word
                word = strtok(NULL, worddelimiter); // Get the next word
            }
            sentence = strtok(NULL,sentencedelimiter);
        }
    }
}
