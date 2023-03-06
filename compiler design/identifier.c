#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
int main()
{
    char input[20];
    printf("enter the identifier\n");
    scanf("%s",input);
    if(!isalpha(input[0])&&input[0]!='_'){
    printf("It is not a valid identifier");
    exit(0);}
    for(int i=0;i<sizeof(input);i++)
    {
        if(isalpha(input[i])&&input[i]!='_'&&isdigit(input[i])){
        printf("It is not a valid identifier\n");
        exit(0);}
    }
    printf("It is a valid identifier\n");

}