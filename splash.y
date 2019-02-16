%{
#include <ctype.h>
#include <stdio.h>
#include "output.h"
#include "splash_helper.h"

int yylex();
void yyerror();

%}

%define api.value.type union

%type <Operand> expr_
%type <Operand> expr
%type <Comparison> comp

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

prog        : stat_list { YYACCEPT; }
            | error '\n' { yyerror("Invalid"); yyerrok; }
            ;

stat_list   : stat_list stat '\n' {}
            | stat_list '\n' {}
            |
            ;

stat        : expr  { fprintf(stderr, "<reduced expr_>\n"); }
            | attrib  { fprintf(stderr, "<reduced attrib>\n"); }
            | cond  { fprintf(stderr, "<reduced cond>\n"); }
            ;

attrib      : ID ATT expr  { place_set_variable($1); }
            ;

cond        : IF comp { append_conditional($2); }
            '{'
                stat_list
            '}' opt_else
            ;

opt_else    : ELSE '{' stat_list '}'
            |
            ;

comp        : expr EQ expr_  { append_comparison(&$$, CompOpEQ, $1, $3); }
            | expr LT expr_  { append_comparison(&$$, CompOpLT, $1, $3); }
            | expr GT expr_  { append_comparison(&$$, CompOpGT, $1, $3); }
            | '(' comp ')'  { $$ = $2; }
            ;

expr        : expr_  { $$ = $1; place_operand($1); }
            ;

expr_       : expr_[left] '+' expr_[right]  { append_operation(&$$, '+', $[left], $[right]);  }
            | expr_[left] '-' expr_[right]  { append_operation(&$$, '-', $[left], $[right]);  }
            | expr_[left] '*' expr_[right]  { append_operation(&$$, '*', $[left], $[right]);  }
            | expr_[left] '/' expr_[right]  { append_operation(&$$, '/', $[left], $[right]);  }
            | expr_[left] '^' expr_[right]  { append_operation(&$$, '^', $[left], $[right]);  }
            | '(' expr_ ')'                 { $$ = $2; }
            | '-' expr_ %prec UMINUS        { append_minus_op(&$$, $2); }
            | NUM                           { append_operand(&$$, number, $1); }
            | ID                            { append_operand(&$$, variable, $1); }
            ;

%%

#include "lex.yy.c"

int
main() {
    init_parse();
    yyparse();
    end_parse();
}
