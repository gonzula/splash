%{
#include <ctype.h>
#include <stdio.h>
#include "splash_helper.h"

int yylex();
void yyerror();

%}

%define api.value.type union

%type <char *> expr

%token <char *> STR
%token <char *> NUM
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
%token <char *> ID


%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS

%%

prog    : stat_list 'f' { puts("stats_list"); }
        | error '\n' { yyerror("Invalid"); yyerrok; }
        ;

stat_list   : stat_list stat '\n' {}
            | stat_list '\n' {}
            |
            ;

stat    : expr  { puts("expr"); }
        ;

expr    : expr[left] '+' expr[right]         { append_operation(&$$, '+', $[left], $[right]);  }
        | expr[left] '-' expr[right]         { append_operation(&$$, '-', $[left], $[right]);  }
        | expr[left] '*' expr[right]         { append_operation(&$$, '*', $[left], $[right]);  }
        | expr[left] '/' expr[right]         { append_operation(&$$, '/', $[left], $[right]);  }
        | expr[left] '^' expr[right]         { append_operation(&$$, '^', $[left], $[right]);  }
        | '(' expr ')'          { append_operand(&$$, $2); }
        | '-' expr %prec UMINUS {}
        | NUM { append_operand(&$$, $1); }
        | ID { append_operand(&$$, $1); }
        ;

%%

#include "lex.yy.c"
