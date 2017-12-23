%{
#include "classes.h"
#include <bits/stdc++.h>
#include <time.h>
#include <sys/time.h>
  extern "C" int yylex();
  extern "C" int yyparse();
  extern "C" FILE *yyin;
  extern "C" int line_num;
  extern union Node yylval;
  extern "C" int errors;
  void yyerror(const char *s);
  class Prog* start = NULL;
  class Visitor* t = new Traverse();
  class Visitor* v = new Evaluate();
  int errors=0;
%}

%start Program

%token IF ELSE FOR WHILE
%token DBLOCK
%token CBLOCK
%token GOTO GOT PRINT PRINTLN READ
%token COMMA SC COLON
%token <value>  TYPE ID TEXT
%token <number> INTEGER
%token <value> AND OR
%token <value> '+' '-' '*' '/' '%'

%token <value> LT GT LE GE
%token <value> EQUAL NOTEQUAL
%token <value> ASSIGN


%left EQUAL NOTEQUAL
%left AND OR
%left LT GT LE GE
%left '+' '-'
%left '*' '/' '%'

%type <prog> Program
%type <fields> Declarations
%type <field> Declaration
%type <vars> Variables
%type <var> Variable

%type <stmts> Statements
%type <stmt> Statement
%type <st> State


%type <assignment> Assignment
%type <block> Block
%type <readstmt> Readstmt
%type <readvars> varnames
%type <readvar> varname

%type <printstmt> Printstmt

%type <arthexpr> ArthExpr
%type <boolexpr> BoolExpr

%type <printvars> varnames1
%type <printvar> varname1

%type <location> Location
%type <expr> Expr
%type <literal> Literal;

%%

Program:
	DBLOCK '{' Declarations '}' CBLOCK '{' Statements '}' {  $$ = new Prog($3,$7);  start = $$;}
	;

Declarations: {$$ = new Decls();}
	| Declarations Declaration SC {$$->push_back($2);}
	;

Declaration:
	TYPE Variables {$$ = new Decl(string($1),$2);}
	;

Variables:
	Variable {$$ = new Vars();$$->push_back($1);}
	| Variables COMMA  Variable { $$->push_back($3); }
	;

Variable:
	ID { $$ = new Var(string("Normal"),string($1));}
	| ID '[' INTEGER ']' { $$ = new Var(string("Array"),string($1),$3);}
  ;

Statements: {$$ = new Stmts();}
	| Statements State {$$->push_back($2);}
	;

State: Statement {$$ = new NormalSt(string("normal"),string("None"),$1);}
| ID COLON Statement {$$ = new NormalSt(string("goto"),string($1),$3);}
;

Statement:
	Assignment {$$ = $1;}
  |Block {$$ = $1;}
  |IF BoolExpr Block {$$ = new Ifstmt($2,$3,NULL);}
  |IF  BoolExpr  Block ELSE Block {$$ = new Ifstmt($2,$3,$5);}
  |WHILE BoolExpr  Block {$$ = new Whilestmt($2,$3);}
  |FOR ID ASSIGN Expr COMMA Expr COMMA INTEGER Block {$$=new Forstmt(string($2),$4,$6,$8,$9);}
  |FOR ID ASSIGN Expr COMMA Expr Block {$$ = new Forstmt(string($2),$4,$6,1,$7);}
  |Readstmt {$$ = $1;}
  |Printstmt {$$=$1;}
  |GOTO ID SC {$$ = new Goto(string($2),NULL);}
  |GOTO ID IF BoolExpr SC {$$ = new Goto(string($2),$4);}
	;


Readstmt : READ varnames SC{$$ = new Readdecl($2);}

Printstmt : PRINT TEXT COMMA varnames1 SC{$$ = new Printdecl(string($2),$4,string("None"));}
          | PRINTLN TEXT COMMA varnames1 SC {$$ = new Printdecl(string($2),$4,string("Newline"));}
          | PRINT varnames1 SC {$$ = new Printdecl(string("None"),$2,string("None"));}
          | PRINTLN varnames1 SC{$$ = new Printdecl(string("None"),$2,string("Newline"));}

varnames1 : varname1 {$$ = new Printvars();$$->push_back($1);}
      | varnames1 COMMA varname1 {$$->push_back($3);}

varname1 : ID {$$ = new Printvar(string("Normal"),string($1));}
        | ID '[' Expr ']' {$$ = new Printvar(string("Array"),string($1),$3);}


varnames : varname {$$ = new Readvars();$$->push_back($1);}
      | varnames COMMA varname {$$->push_back($3);}

varname : ID {$$ = new Readvar(string("Normal"),string($1));}
        | ID '[' Expr ']' {$$ = new Readvar(string("Array"),string($1),$3);}



Block: '{' Statements '}'{$$ = new Block($2);}

Assignment:
	Location ASSIGN Expr SC {$$ = new Assignment($1,string($2),$3);}
	;

Location:
	ID {$$ = new Location(string($1),string("Normal"));}
	| ID '[' Expr ']'{$$ = new Location(string($1),string("Array"),$3);}
	;

Expr:
	Location {$$ = $1;}
	| Literal {$$ = $1;}
  | ArthExpr {$$=$1;}
  |BoolExpr {$$=$1;}
  ;

ArthExpr	: Expr '+' Expr {$$ = new ArthExpr($1,string($2),$3);}
	| Expr '-' Expr {$$ = new ArthExpr($1,string($2),$3);}
	| Expr '*' Expr {$$ = new ArthExpr($1,string($2),$3);}
	| Expr '/' Expr {$$ = new ArthExpr($1,string($2),$3);}
	| Expr '%' Expr {$$ = new ArthExpr($1,string($2),$3);}
  | '(' ArthExpr ')' {$$ =$2;}
  ;
BoolExpr: Expr LT Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr GT Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr LE Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr GE Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr EQUAL Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr NOTEQUAL Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr OR Expr {$$ = new BoolExpr($1,string($2),$3);}
	| Expr AND Expr {$$ = new BoolExpr($1,string($2),$3);}
  | '(' BoolExpr ')' {$$ =$2;}
	;


Literal: INTEGER {$$ = new intLiteral($1);}
	;

%%
int main(int argc, char **argv) {
  if(argc == 1) {
		printf("No Input File Given\n");
		exit(-1);
	}

	FILE *input = fopen(argv[1], "r");

	if (input == NULL){
		printf("Can't open the given file!\n");
		exit(-1);
	}
	yyin = input;
	do {
		yyparse();
	} while (!feof(yyin));
	if(start){
    start->accept(t); /* Traversals */
    start->accept(v);
    start->Code_gen();
    start->startCode_gen();
	}
}
void yyerror(const char *s){
	errors++;
	printf("Error:%s at %d\n",s,line_num);
  exit(-1);
}
double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
