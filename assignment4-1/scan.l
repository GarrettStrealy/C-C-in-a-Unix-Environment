/*
* Date: 10/19/2020
* Author: Garrett Strealy
* Email: gjs160430@utdallas.edu
* Course: CS 3377.001 Fall 2020
* Version: 1.0
* Copyright: 2020, all rights reserved
* Description: lex program that scans tokens
*/

%option noyywrap
%option nounput
%option noinput

%{
#include <stdlib.h>
#include "y.tab.h"
%}

%%

[ \t\r]+      { /* eat up whitespace */ }
I+V*          {yylval.str=strdup(yytext); return ROMANTOKEN; }
[A-Za-z]{2,}  {yylval.str=strdup(yytext); return NAMETOKEN;}
[0-9]+[A-Z]+  {yylval.str=strdup(yytext); return IDENTIFIERTOKEN;}
[A-Z]|[A-Z]\. {yylval.str=strdup(yytext); return NAME_INITIAL_TOKEN;}
Sr\.          {yylval.str=strdup(yytext); return SRTOKEN;}
Jr\.          {yylval.str=strdup(yytext); return JRTOKEN;}
[\n]          {return EOLTOKEN;}
[0-9]+        {yylval.value=atoi(yytext); return INTTOKEN;}
","           {return COMMATOKEN;}
"-"           {return DASHTOKEN;}
"#"           {return HASHTOKEN;}

%%
