%{
#include <ctype.h>
#include <stdio.h>
#include "output.h"
#include "splash_helper.h"

extern FILE *yyin;
int yylex();
void yyerror();

%}

%define api.value.type union

%type <Operand> expr_
%type <Operand> expr
%type <Operand> param
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

%left LT LE EQ
%right ATT
%left '+' '-'
%right UMINUS
%left '*' '/' '%'
%right '^'

%%

prog        : stat_list { YYACCEPT; }
            | error '\n' { yyerror("Invalid"); yyerrok; }
            ;

stat_list   : stat_list stat '\n' {}
            | stat_list '\n' {}
            |
            ;

stat        : expr  { DEBUGPRINT("<reduced expr_>\n"); }
            | attrib  { DEBUGPRINT("<reduced attrib>\n"); }
            | cond  { DEBUGPRINT("<reduced cond>\n"); }
            ;

attrib      : ID ATT expr  { place_set_variable($1); }
            ;

cond        : IF comp { increment_if_count(); append_cond_control(); append_conditional($2); }
            '{'
                stat_list
            '}' { close_scope(); }
            opt_else_if
            ;

opt_else_if : ELSE IF comp { append_else(); append_conditional($3); }
            '{'
                stat_list
            '}' { close_scope(); close_scope(); }
            opt_else_if
            | opt_else
            ;

opt_else    : ELSE { append_else(); }
            '{'
            stat_list
            '}' { close_scope(); }
            |
            ;

comp        : expr_ EQ expr_  { append_comparison(&$$, CompOpEQ, $1, $3); }
            | expr_ LT expr_  { append_comparison(&$$, CompOpLT, $1, $3); }
            | expr_ GT expr_  { append_comparison(&$$, CompOpGT, $1, $3); }
            | '(' comp ')'  { $$ = $2; }
            ;

expr        : expr_  { $$ = $1; place_operand($1); }
            ;

expr_       : expr_[left] '+' expr_[right]  { append_operation(&$$, '+', $[left], $[right]);  }
            | expr_[left] '-' expr_[right]  { append_operation(&$$, '-', $[left], $[right]);  }
            | expr_[left] '*' expr_[right]  { append_operation(&$$, '*', $[left], $[right]);  }
            | expr_[left] '/' expr_[right]  { append_operation(&$$, '/', $[left], $[right]);  }
            | expr_[left] '%' expr_[right]  { append_operation(&$$, '%', $[left], $[right]);  }
            | expr_[left] '^' expr_[right]  { append_operation(&$$, '^', $[left], $[right]);  }
            | '(' expr_ ')'                 { $$ = $2; }
            | '-' expr_ %prec UMINUS        { append_minus_op(&$$, $2); }
            | NUM                           { append_operand(&$$, number, $1); }
            | ID                            { append_operand(&$$, variable, $1); }
            | ID '(' param ')'              { append_func_call(&$$, $1, $3); }
            | STR                           { append_operand(&$$, string, $1); }
            ;

param       : expr_
            |                               { append_null_operand(&$$); }
            ;

%%

#include "lex.yy.c"

int
main(int argc, char *argv[]) {
    FILE *fp = init_parse(argc, argv);
    if (!fp) {
        return EXIT_FAILURE;
    }
    yyin = fp;
    yyparse();
    end_parse();
    yylex_destroy();
    return EXIT_SUCCESS;
}
