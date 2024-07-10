%{

#include "fqg_name_token_table.h"

%}

%option noyywrap
%option prefix="fqg_name"


%%
[(]                     { return TT_OPENING_PARANTHESIS; }
[)]                     { return TT_CLOSING_PARANTHESIS; }
[[]                     { return TT_OPENING_BRACKETS;    }
[]]                     { return TT_CLOSING_BRACKETS;    } 
[=]                     { return TT_EQ;    } 
[+]                     { return TT_PLUS;    } 
[-]                     { return TT_MINUS;    } 
[,]                     { return TT_COMMA;               }
[*]                     { return TT_ASTERISK;            }
[:]                     { return TT_COLON;            }
[ \t]+                  { return TT_WHITESPACE;          }
[_]                     { return TT_UNDERSCORE;          }
[#]                     { return TT_POUND;          }

[a-zA-Z0-9][[a-zA-Z0-9]{-}[dwf|swf|dxg]*   { return TT_IDENTIFIER;          }

[a-zA-Z][[a-zA-Z0-9]*/[\,|\)]   { return TT_ID_INFLEXION;          }

^(dwf)                 { return TT_TYPE_DWF; }
^(swf)                 { return TT_TYPE_SWF; }
^(dxg)                 { return TT_TYPE_SWF; }

[{]                     { return TT_OPENING_CURLY;       }
[}]                     { return TT_CLOSING_CURLY;       }
<<EOF>>                     { return TT_END_OF_FILE;         }
\n                          { return TT_END_OF_LINE;         }
.                           {return TT_UNRECOGNIZED;          }
%%
// [a-zA-Z0-9]{-}[(dwf|swf|dxg)][[a-zA-Z0-9]*   { return TT_IDENTIFIER;          } /* 8 */
