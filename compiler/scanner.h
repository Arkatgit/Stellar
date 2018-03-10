//Header file for the lexical analyzer..
#ifndef __SCANNER_H__
#define __SCANNER_H__

//Token Class

enum Tokens  {
                 Tokerr = -1,
                 Tokeoln,
				 Tokeof,
                 Toklparen,
                 Tokrparen,
                 Tokfunction,
                 Tokint,
                 Tokfloat,
                 Tokchar,
                 Tokbool,
                 Tokarray,
                 Tokvoid,
                 Toklbrace,
                 Tokrbrace,
                 Toksemi,
                 Tokcolumn,
                 Tokvar,
                 Tokconst,
                 Tokif,
                 Tokthen,
                 Tokelse,
                 Tokwhile,
                 Tokdo,
                 Tokequal,
                 Toknequal, 				 
                 Tokreturn,
                 Tokand,
                 Tokor,
                 Tokcompare,
                 Tokless,
                 Tokgreater,
                 Toklthan_eq,
                 Tokgthan_eq,
                 Tokplus,
                 Toktimes,
                 Tokminus,
                 Tokdivide,
             	 Tokmod,
                 Toknot,
                 Toktrue,
                 Tokfalse,
				 Toknumber,
                 Tokvariable,
                 Tokelseif,
                 Tokcolon,
                 Toklarr,
                 Tokrarr,
                 Tokcall,
				 Tokcharconst,
				Toklib,	
   };
       
enum TokenKind
	 {
			None = -1,
            Real,
            Integer,	
     };
	 
//Information about a single Token
struct Token_info
	  {
	       enum Tokens Tok;
           int col;
           int line;
           char lexeme[256];
		   enum TokenKind Kind; 
           union {
		            int ival;
                    double rval;
   					
                  }value;
	  }; 				   
	           
        
 struct scanner 
               {
                  void (*Drive)(char *);
                  struct Token_info *(*GetNextToken)(void);
               };
    			   
 extern struct scanner LEX;
 #endif
