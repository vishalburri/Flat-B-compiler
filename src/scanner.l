%{

#include <bits/stdc++.h>
#include "classes.h"
#include "parser.tab.h"
#define YY_DECL extern "C" int yylex()
extern union Node yylval;
using namespace std;
int line_num = 0;
%}
digit [0-9]
hex_digit {digit}|[a-fA-F]
%%

"declblock" {return DBLOCK;}

"codeblock" {return CBLOCK;}

"while" {return WHILE;}

"int" { yylval.value = strdup(yytext); return TYPE; }

"for" { return FOR; }

"if" { return IF; }

"else" { return ELSE; }

"print" {return PRINT;}

"read" {return READ;}

"goto" {return GOTO;}

"got" {return GOT;}

"println" {return PRINTLN;}


[a-zA-Z][a-zA-Z0-9_]* { yylval.value = strdup(yytext);return ID; }

"0x"{hex_digit}+|{digit}+ { yylval.number = atoi(yytext);return INTEGER; }

\".*\" { yylval.value = strdup(yytext);return TEXT;}


"+" { yylval.value = strdup(yytext);return '+'; }
"-" { yylval.value = strdup(yytext);return '-'; }
"/" { yylval.value = strdup(yytext);return '/'; }
"*" { yylval.value = strdup(yytext);return '*'; }
"%" { yylval.value = strdup(yytext);return '%'; }

"<" { yylval.value = strdup(yytext);return LT; }
">" { yylval.value = strdup(yytext);return GT; }
"<=" { yylval.value = strdup(yytext);return LE; }
">=" {yylval.value = strdup(yytext);return GE; }
"==" { yylval.value = strdup(yytext);return EQUAL; }
"!=" { yylval.value = strdup(yytext);return NOTEQUAL; }
"=" { yylval.value = strdup(yytext);return ASSIGN; }
"&&" { yylval.value = strdup(yytext);return AND; }
"||" { yylval.value = strdup(yytext);return OR; }

"{"		return '{';
"}"		return '}';
"["		return '[';
"]"		return ']';
"("		return '(';
")" 		return ')';
","		return COMMA;
";"  		return SC;
":"  		return COLON;

[ \t]						/* Ignore whitespaces tabs and newlines */
"//".*						/* Inline comments So Ignore */
\n { line_num++; }
%%
