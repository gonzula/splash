%{
#include <ctype.h>
#include <stdio.h>
#include "output.h"
#include "splash_helper.h"

int yylex();
void yyerror();


%}

%define api.value.type union

%type <Operand> expr
%type <Operand> finished_expr

%token <char100> STR
%token <char100> NUM
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
%token <char100> ID


%right ATT
%left '+' '-'
%right UMINUS
%left '*' '/'
%right '^'

%%

prog    : stat_list { YYACCEPT; }
        | error '\n' { yyerror("Invalid"); yyerrok; }
        ;

stat_list   : stat_list stat '\n' {}
            | stat_list '\n' {}
            |
            ;

stat    : finished_expr  { fprintf(stderr, "<reduced expr>\n"); }
        | attrib  { fprintf(stderr, "<reduced attrib>\n"); }
        ;

attrib  : ID ATT finished_expr  { output_set_variable(stdout, $1); }
        ;

finished_expr : expr  { $$ = $1; output_operand(stdout, $1); }
              ;

expr    : expr[left] '+' expr[right]         { append_operation(&$$, '+', $[left], $[right]);  }
        | expr[left] '-' expr[right]         { append_operation(&$$, '-', $[left], $[right]);  }
        | expr[left] '*' expr[right]         { append_operation(&$$, '*', $[left], $[right]);  }
        | expr[left] '/' expr[right]         { append_operation(&$$, '/', $[left], $[right]);  }
        | expr[left] '^' expr[right]         { append_operation(&$$, '^', $[left], $[right]);  }
        | '(' expr ')'          { $$ = $2; }
        | '-' expr %prec UMINUS { append_minus_op(&$$, $2); }
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
