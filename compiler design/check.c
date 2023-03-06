#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
int main()
{
    char input[20];
    printf("enter the identifier\n");
    scanf("%s",input);
    if(!isalpha(input[0])||input[0]!='_')
    printf("%c",input[0]);

}