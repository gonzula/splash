%{
#include <ctype.h>
#include <stdio.h>

/*#define YYSTYPE char[100]*/

int yylex();
void yyerror();

%}

%define api.value.type union

%token <char const *> STR
%token <char const *> NUM
%token ATT
%token IF
%token ELSE
%token AND
%token OR
%token EQ
%token LT
%token LE
%token GT
%token GE
%token <char const *> ID


%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS

%%

prog    : stat_list 'f' { puts("stats_list"); }
        | error '\n' { yyerror("Invalid"); yyerrok; }
        ;

stat_list   : stat_list stat '\n' { puts("stat_list \'\\n\' stat");}
            | stat_list '\n' { puts("stat_list \'\\n\'"); }
            |
            ;

stat    : expr  { puts("expr"); }
        ;

expr    : expr '+' expr         { puts("+"); }
        | expr '-' expr         { puts("-"); }
        | expr '*' expr         { puts("*"); }
        | expr '/' expr         { puts("/"); }
        | expr '^' expr         { puts("^"); }
        | '(' expr ')'          { puts("()"); }
        | '-' expr %prec UMINUS { puts("MINUS"); }
        | value
        ;

value   : NUM { printf("NUM: %s\n", $1); }
        | ID { printf("ID: %s\n", $1); }
        ;

%%

#include "lex.yy.c"
