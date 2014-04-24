%{
    #include "y.tab.h"
    void yyerror(char *);
%}

%%

[0-9]       {
                yylval = atoi(yytext);
                return INTEGER;
            }

[1-9][0-9]+ {
                yylval = atoi(yytext);
                return INTEGER;
            }

[-+d<>\n]       return *yytext;

[ \t]           ;

.               yyerror("Invalid character");

%%

int yywrap() {
    return 1;
}
