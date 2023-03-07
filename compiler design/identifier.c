#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
int check_keyword(char * c)
{

     if(!isalpha(c[0])&&c[0]!='_'){
    return 1;}
    for(int i=1;i<strlen(c);i++)
    {
        if(!isalpha(c[i])&&c[i]!='_'&&!isdigit(c[i])){
        return 1;}
    }
    return 0;
}
int main()
{
    char *input="iuput";
    // printf("enter the identifier\n");
    // scanf("%s",input);
    printf("%d",check_keyword(input));
    if(check_keyword(input)==0)
    printf("valid");
}