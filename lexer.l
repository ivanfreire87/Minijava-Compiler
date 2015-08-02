%{
#include "includes.hh"
#include "symtab.hh"
#include "ast.hh"
#include "ast_utils.hh"
#include "utils.hh"
#include "parser.tab.h"

int errors = 0;
int lines = 1;

int yyerror(char *s);
%}

letter 						[a-zA-Z]
digit 						[0-9]
space						" "
tab							"\t"
comma						","
semicolon					";"
open-braces					"{"
close-braces				"}"
open-parenthesis			"("
close-parenthesis			")"
open-squarebrackets			"["
close-squarebrackets		"]"
dot							"."
equal						"="
exclamation					"!"

public						"public"
static						"static"
void						"void"
main						"main"
class						"class"
extends						"extends"
string						"String"
int							"int"
boolean						"boolean"
true						"true"
false						"false"
if							"if"
else						"else"
while						"while"
length						"length"
this						"this"
new							"new"
print						"System.out.println"
return						"return"
readers						"Readers"
intreader					"IntReader"
readint						"readInt"
		
identifier 					{letter}({letter}|{digit}|_)*
integer						{digit}+
binoperator 				(&&|<|\+|-|\*)

%x multilinecomment
%x onelinecomment

%%
{integer}							{ yylval.ival = atoi(yytext); return DIGIT; }				

{public}							{ return PUBLIC; }						
{static}							{ return STATIC; }
{void}								{ return VOID; }
{main}								{ return MAIN; }
{class}								{ return CLASS; }
{extends}							{ return EXTENDS; }
{string}							{ return STRING; }
{int}								{ return INT; }
{boolean}							{ return BOOLEAN; }
{true}								{ return TRUE; }
{false}								{ return FALSE; }
{if}								{ return IF; }
{else}								{ return ELSE; }
{while}								{ return WHILE; }
{length}							{ return LENGTH; }	
{this}								{ return THIS; }
{new}								{ return NEW; }
{print}								{ return PRINT; }
{return}							{ return RETURN; }
{readers}						    { return READERS; }
{intreader}							{ return INTREADER; }
{readint}							{ return READINT; }
	
{space}+							{ }
{tab}+ 								{ }
{comma}								{ return COMMA; }
{semicolon}							{ return SEMICOLON; }			
{open-braces}						{ return OBRACES; }
{close-braces}						{ return CBRACES; }
{open-parenthesis}					{ return OPAR; }
{close-parenthesis}					{ return CPAR; }	
{open-squarebrackets}				{ return OSQUAREPAR; }
{close-squarebrackets}				{ return CSQUAREPAR; }
{dot}								{ return DOT; }
{equal}								{ return EQUAL; }
{exclamation}						{ return EXCL; }
	
{binoperator}					 	{ yylval.vval = new string(yytext); return OP;}
{identifier}						{ yylval.vval = new string(yytext); return ID; }

"/*"								BEGIN(multilinecomment);
<multilinecomment>[^*\n]*
<multilinecomment>"*"+[^*/\n]*	
<multilinecomment>\n				{ lines++; }
<multilinecomment>"*"+"/"			BEGIN(INITIAL);
"//"								BEGIN(onelinecomment);
<onelinecomment>[^\n]*	
<onelinecomment>\n					BEGIN(INITIAL);	{ lines++; } 
\n           						{ lines++; }
<<EOF>> 							{ yyterminate(); }
.           						{ cout << "Unrecognized token \"" << yytext << "\" in line: " << lines << endl; errors++; }

%%

