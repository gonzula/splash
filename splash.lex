
%{
#include <string.h>
#include <stdlib.h>

char *
mkcp(char *src) {
    char *dst = (char *)malloc(sizeof(char) * (strlen(src) + 1));
    strcpy(dst, src);
    return dst;
}

%}

%%

[ ] { }

\"([^\"\\]|\\.)*\"|\'([^\'\\]|\\.)*\' { yylval.STR = mkcp(yytext); return STR; }
[0-9]+(\.[0-9]+)? { yylval.NUM = mkcp(yytext); return NUM; }
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

[a-zA-Z_][a-zA-Z_0-9]*  { yylval.ID = mkcp(yytext); return ID; }

\n|.                     { return yytext[0]; }



%%

