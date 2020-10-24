/*
* Filename: main.c
* Author: Garrett Strealy
* Email: gjs160430@utdallas.edu
* Date: 10/19/2020
* Course: CS3377.001 Fall 2020
* Version: 1.0
* Description:
*      ./scanner calls yylex().
*       ./parser calls yyparse().
*/

#include <stdio.h> //needed for printf()
#include "y.tab.h" //needed for ENUM
#include <string.h>

int yyparse(void);
int yylex(void);
extern char *yytext;

int main(int argc, char *argv[]){
    
  if(strcmp(argv[0],"./scanner") == 0){
    printf("Operating in scan mode\n\n");
    int token = yylex();
    
    //just call the lexical scanner until we reach EOF
    while(token!=0){
      switch(token){
      case NAME_INITIAL_TOKEN:
	printf("yylex returned NAME_INITIAL_TOKEN token (%s",yytext);
	printf(")\n");
	break;
      case NAMETOKEN:
	printf("yylex returned NAMETOKEN token (%s", yytext);
	printf(")\n");
	break;
      case EOLTOKEN:
	printf("yylex returned EOLTOKEN token (%d",token);
	printf(")\n");
	break;
      case INTTOKEN:
	printf("yylex returned INTTOKEN token (%s", yytext);
	printf(")\n");
	break;
      case HASHTOKEN:
	printf("yylex returned HASHTOKEN token (%d",token);
	printf(")\n");
	break;
      case COMMATOKEN:
	printf("yylex returned COMMATOKEN token (%d",token);
	printf(")\n");
	break;
      case ROMANTOKEN:
	printf("yylex returned ROMANTOKEN token (%d",token);
	printf(")\n");
	break;
      case SRTOKEN:
        printf("yylex returned SRTOKEN token (%s",yytext);
        printf(")\n");
        break;
      case JRTOKEN:
        printf("yylex returned JRTOKEN token (%s",yytext);
        printf(")\n");
        break;
      case DASHTOKEN:
	printf("yylex returned DASHTOKEN token (%d",token);
	printf(")\n");
	break;
      case IDENTIFIERTOKEN:
	printf("yylex returned IDENTIFIERTOKEN token (%s",yytext);
	printf(")\n");
	break;
      default:
	printf("other\n");
	break;
      } // end of switch
      
      token = yylex();
    } //end of while
  } // end of if statement
  
  if(strcmp(argv[0], "./parser") == 0){
    printf("Operating in parse mode\n\n");
    int value = yyparse();
    switch (value){
    case 0:
      printf("Parse Successful!\n");
      break;
    case 1:
      printf("Parse Failed\n");
      break;
    case 2:
      printf("Out of memory\n");
    default:
      printf("Unrecognized result from yyparse()\n");
      break;
    }
  } // end of parser statement
  
  return 0;
} // end of main
