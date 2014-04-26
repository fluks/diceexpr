%option noyywrap

%{
#include <assert.h>
#include "y.tab.h"
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

[-+d<>]         return *yytext;

[ \t\n]         ;

.               { return INVALID_CHARACTER; }

%%

void
set_scan_string(const char *expr) {
    assert(expr != NULL);

    yy_scan_string(expr); 
}

void
delete_buffer() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}
