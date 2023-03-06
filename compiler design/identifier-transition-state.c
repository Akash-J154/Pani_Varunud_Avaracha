#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
enum State { START, IDENTIFIER };
bool is_valid_identifier(const char* str) {
 enum State state = START;
 while (*str) {
 switch (state) {
 case START:
 if (isalpha(*str) || *str == '_') {
 state = IDENTIFIER;
 } else {
 return false;
 }
 break;
 case IDENTIFIER:
 if (isalnum(*str) || *str == '_') {
 // stay in IDENTIFIER state
 } else {
 return false;
 }
 break;
 default:
 return false;
 }
 str++;
 }
 return state == IDENTIFIER;
}
int main() {
 const char* valid_identifier = "compiler_design";
 const char* invalid_identifier = "123compiler";
 printf("%s is %svalid identifier\n", valid_identifier, is_valid_identifier(valid_identifier) ? "" : "not ");
 printf("%s is %svalid identifier\n", invalid_identifier, is_valid_identifier(invalid_identifier) ? "" : 
"not ");
 return 0;
}