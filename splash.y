%{
#include <ctype.h>
#include <stdio.h>
#include "splash_helper.h"

int yylex();
void yyerror();

%}

%define api.value.type union

%type <Operand *> expr

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

prog    : stat_list { YYACCEPT; }
        | error '\n' { yyerror("Invalid"); yyerrok; }
        ;

stat_list   : stat_list stat '\n' {}
            | stat_list '\n' {}
            |
            ;

stat    : expr  { fprintf(stderr, "<reduced expr>\n"); }
        ;

expr    : expr[left] '+' expr[right]         { append_operation(&$$, '+', $[left], $[right]);  }
        | expr[left] '-' expr[right]         { append_operation(&$$, '-', $[left], $[right]);  }
        | expr[left] '*' expr[right]         { append_operation(&$$, '*', $[left], $[right]);  }
        | expr[left] '/' expr[right]         { append_operation(&$$, '/', $[left], $[right]);  }
        | expr[left] '^' expr[right]         { append_operation(&$$, '^', $[left], $[right]);  }
        | '(' expr ')'          { $$ = $2; }
        | '-' expr %prec UMINUS {}
        | NUM { append_operand(&$$, number, $1); }
        | ID { append_operand(&$$, variable, $1); }
        ;

%%

#include "lex.yy.c"

int
main() {
    output_header(stdout);
    yyparse();
    output_footer(stdout);
}
