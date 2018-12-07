
%{
#include <string.h>
#include <stdlib.h>

%}

%%

[ ] { }

#.*?\n              {}  /* comment */

\"([^\"\\]|\\.)*\"|\'([^\'\\]|\\.)*\' { strcpy(yylval.STR.value, yytext); return STR; }
[0-9]+(\.[0-9]+)? { strcpy(yylval.NUM.value, yytext); return NUM; }
\:\=            { return ATT; }

if             { return IF;   }
otherwise|else { return ELSE; }

and|&&          { return AND; }
or|\|\|         { return OR;  }

equals|==       { return EQ;  }
less_than|<     { return LT;  }
less_or_equal_than|<=  { return LE; }
greater_than|>         { return GT; }
greater_or_equal_than|>=  { return GE; }

[a-zA-Z_][a-zA-Z_0-9]*  { strcpy(yylval.ID.value, yytext); return ID; }

\n|.                     { return yytext[0]; }



%%

