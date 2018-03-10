/***************************************************************************
 *Reccurssive descent Parser                                                *
 *Checks for syntactic Correctness of input                                 *
 *It employs the panic mode error recorvery techqnic in case of errors      * 
 *Checks for semantics correctness                                          *
 *Construct parse trees for well formed inputs                              *
 *                                                                          * 
 *                                                                          *
 ****************************************************************************
*/
 
#include "scanner.h"
#include<stdio.h>
#include "ast.h"
#include "symtab.h"
#include "parser.h"
#include "semantics.h"
#include "input.h"
#include "generator.h"
#include "error.h"
#include<time.h>
/*Fuction Prototypes for Recurssive Calls*/

 struct FunctionDef * FUNC  = NULL;
 struct FunctionDef * FUNC2  = NULL;

static struct Statement * Program(void);
struct Statement * ToplevelStatement(void);
static void FunctionDef(void);	 	 
static struct Symbol * FormalArgumentList(void);
static struct Statement * Statements(void);
static void Datadef(void);
static struct Statement * ExecStatement(void);
static struct Statement * IfStatement(void);
static struct Statement * WhileStatement(void);
static struct Statement * AssignmentStatement(void);
static struct Statement * ReturnStatement(void);
static struct Statement * FunctionCal(void);	
static struct Expression * Expr(void);
static struct Expression * LogicalExpr(void);
static struct Expression * RelExpr(void);
static struct Expression * AddExpr(void);
static struct Expression * MulExpr(void);
static struct Expression * UnaryExpr(void);
static struct Expression * PrimaryExpr(void);
static struct Statement * Program(void);
static enum Type assignType(struct Symbol *p);
#ifdef P_DEBUG
#define PPRINT(x) x
#else
#define PPRINT(x)
#endif
/*Program               --> {ToplevelStatement} EOI
ToplevelStatement     -->FunctionDef|Statements
FunctionDeF          --> "function" ("int"|"float"|"char"|"void"|"bool") Ident "(" "void"| FormalArgumentList { ":" FormalArgumentList} ")" 
                           "{"  {Statements}  "}"
FormalArgumentList    -->  ("int"|"float"|"char"|"bool")  Ident["["Expr"]"] 
Statements            --> Datadef|ExecStament
DataDef				--> "var" ("int"|"float"|"char"|"bool")  Ident [":=" Expr] ";"
				  | "const" ("int"|"float"|"char"|"bool") Ident ":=" Expr ";"
                  |"array"  ("int"|"float"|"bool") Ident "[" Expr "]";   
ExecStament           -->IfStatement |WhileStatement|  AssignmentStatement | ReturnStatement| FunctionCall                      
                          
IfStatement			--> "if" Expr "then" "{" {ExecStatement} "}"
						{ "elseif" Expr "then" "{" {ExecStatement} "}" }
				 	      [ "else" "{" {ExecStatement} "}" ]
WhileStatement       -->"while"  Expr "do" "{" {ExecStatement} "}" 

AssignmentStatement	--> Ident["["Expr"]" ] ":=" Expr ";"
ReturnStatement		--> "return" [ Expr] ";"
FunctionCall            -->"call" Ident"(" [ Expr { ":" Expr } ] ")"  
Expr				 --> LogicalExpr
LogicalExpr			--> RelExpr { ("and" | "or") RelExpr}
RelExpr				--> AddExpr { ("=" | "<" | ">" | "<=" | ">=" | "!=") AddExpr }
AddExpr				--> MulExpr { ("+" | "-") MulExpr }
MulExpr				--> UnaryExpr { ("*" | "/" | "mod") UnaryExpr }
UnaryExpr			--> PrimaryExpr
				       | ("+" | "-" | "!") UnaryExpr
PrimaryExpr			--> number
					   | Ident[ "(" [ Expr { ":" Expr } ] ")"]
					   |Ident "[" Expr "]"
				       | "true"
				       | "false"
                       | "'"Character "'"					   
				       | "(" LogicalExpr ")"
Ident				--> "a" - "z" {"a" - "z" "0" - "9" "_"}
number				--> integer
				   | real
integer				--> "0" - "9" {"0" - "9"}
real				--> integer "." integer
*/
#define EXPRFIRST_SET(t) t == Toknumber||t == Tokvariable ||t == Toktrue ||t == Tokfalse || t == Tokcharconst ||t == Toklparen \
                         ||t == Toktrue||t ==Tokminus||t == Toknot     
						 
/**Library functions which are loaded into the symbol table prior to parsing*/
						 
struct Token_info  Lib[] =	{
							  {Toklib, 0 ,0 ,"addints",0},
							  {Toklib, 0 ,0 ,"multiplyints",0},
							  {Toklib, 0 ,0 ,"dupintarray",0},
							  {Toklib, 0 ,0 ,"dupfloatarray",0},
							  {Toklib, 0 ,0 ,"sortintarray",0},
							  {Toklib, 0 ,0 ,"sortfloatarray",0},
							  {Toklib, 0 ,0 ,"factorial",0},
							  {Toklib, 0 ,0 ," permutation",0},
							  {Toklib, 0 ,0 ,"combination",0},
							  {Toklib, 0 ,0 ,"ap_nterm",0},
							  {Toklib, 0 ,0 ,"ap_sumnterms",0},
							  {Toklib, 0 ,0 ,"gp_nterm",0},
							  {Toklib, 0 ,0 ,"gp_suminfinity",0},
							  {Toklib,0, 0,"get_int",0},
							  {Toklib,0, 0,"ShowMeanTable",0},
							  {Toklib,0, 0,"get_float",0},
							  {Toklib,0, 0,"printfloat",0},
							  {Toklib,0, 0,"printbool",0},
							  {Toklib,0, 0,"mini",0},
							  {Toklib,0, 0,"maxi",0},
							  {Toklib,0, 0,"abs_val",0},
							  {Toklib,0, 0,"get_char",0},
							  {Toklib,0, 0,"SL",0},
							  {Toklib,0, 0,"AG",0},
							  {Toklib,0, 0,"PC",0},
							  {Toklib,0, 0,"GC",0},
							  {Toklib,0, 0,"PIE",0},
							  {Toklib,0, 0,"ShowStdTable",0},
							  {Toklib,0, 0,"a_circle",0},
							  {Toklib,0, 0,"c_circle",0},
							  {Toklib,0, 0,"a_rectangle",0},
							  {Toklib,0, 0,"c_rectangle",0},
							  {Toklib,0, 0,"a_square",0},
							 {Toklib,0, 0,"c_square",0},
							 {Toklib,0, 0,"v_cylinder",0},
							};

#define LIBSIZE (sizeof(Lib)/sizeof(Lib[0]))
							
static struct Token_info *t;


static void InitLib(void)
{
	int i;
	for(i = 0 ; i < LIBSIZE ; i++)
	{
		SYMTAB.insertName(&Lib[i]);
	}
}

struct output_w * out;  
struct codegen * gen; 

#ifdef P_DEBUG
main()
{

   LEX.Drive("debug6.txt");
   //struct FunctionDef *f;
   struct Statement *s;
    out = newOutput("outfile22.txt");
	gen = newGenerator(out);
   //EmitGlobalDeclaration(gen);
   s = Program();
   SEM.statementSemantics(s);   // Perform Semantics on statements
   printf("here\n\n\n");
    EmitAll(gen,s);
   printf("---compiled----\n");
}
#endif

#ifndef P_DEBUG
int main(int argc,  char * argv[])
{
	int state = 0;
	int c;
	clock_t start , finish ;
	if(argc <  2)
	{
		fprintf(stderr,"Input files required\n");
		return;
	}
	
	start = clock();
              
        if(strstr(argv[1] , ".r" )  == NULL )
        { 
           error("File must end with the extension .r\n") ;  
           exit(1) ; 
        } 
	LEX.Drive(argv[1]);
	struct Statement *s;
    out = newOutput("outfile22.c");
	gen = newGenerator(out);
	EmitHeading(gen);
   //EmitGlobalDeclaration(gen);
   s = Program();
   SEM.statementSemantics(s);    // Perform Semantics on statements
   if(!getecount())
   {
		EmitAll(gen,s);
		//system("cd R:/PP_REP/PROJECT1");
		system("gcc -o outfile22.exe   outfile22.c  library.o");
		printf("\n---source compiled----\n\n");
		finish = clock();
		printf("---compile time : %f secs----\n\n\n", (double)(finish - start)/CLOCKS_PER_SEC);
   }
	if(getecount())
	{
		printerrors();
	}
	errorlogger();
   //printf("main -end\n");

}
#endif



static struct Statement * Program(void)
{
/*Program               --> {ToplevelStatement} EOI */
	struct Statement * p = NULL;
	struct Statement *s;
	struct Statement *s1;
	//struct FunctionDef *f1;
	//struct FunctionDef *f2 = NULL;
	//struct FunctionDef *f3;
	
	SYMTAB.init();
	InitLib();
    PPRINT(printf("Enter Program\n");)  
     t = LEX.GetNextToken();
	while(t->Tok != Tokeof)
	{
	    s = ToplevelStatement();
		if(s != NULL)
		{
			if(p == NULL)
			{
				p = s1 = s;	
			}
			else
			{
				s1->next = s;
				s1 = s; 	
			}
		}
	/*	
		if( f1 != NULL)
		{
			if(f2 == NULL)
			{
				f2 = f3 =  f1;	
			}
			else
			{
				f3->next = f1;
				f3 = f1;
			}
		}
		  
    }
	*f = f2;
  */
	}  
 PPRINT(printf("Leave Program \n");)
	return p;
}

struct Statement * ToplevelStatement(void)
{
  /*ToplevelStatement     -->FunctionDef|Statements*/
	struct Statement * p = NULL;
	//struct FunctionDef *f1  = NULL;
	PPRINT(printf("Enter Toplevelstatement  with \n");)
	if(t->Tok == Tokfunction)
	{
		FunctionDef();	
	}
 	else if(t->Tok == Tokvar||t->Tok == Tokconst||t->Tok == Tokarray||t->Tok==Tokif||t->Tok==Tokwhile||t->Tok==Tokvariable||t->Tok== Tokreturn||t->Tok==Tokcall)
	{
		p = Statements();
	}
    else
    {
		//fprintf(stderr,"Error:Line %d Col %d  '%s' does not form a valid ToplevelStatement\n",t->line,t->col,t->lexeme);
		error("Error:Line %d Col %d  '%s' does not form a valid ToplevelStatement\n",t->line,t->col,t->lexeme);
		//panic mode error recovery
        do
         {
		     t = LEX.GetNextToken();
         }while(t->Tok !=Tokfunction && t->Tok !=Tokvar && t->Tok != Tokconst && t->Tok != Tokeof && t->Tok !=Tokarray && t->Tok !=Tokif
		      &&t->Tok !=Tokwhile&& t->Tok!=Tokvariable && t->Tok !=Tokreturn);  		 
    }	
	PPRINT(printf("Leave Toplevelstatement \n");)
	return p;
}  


static void  FunctionDef(void)
{
 /*FunctionDeF          --> "function" ("int"|"float"|"char"|"void"|"bool") Ident "(" "void"| FormalArgumentList { ":" FormalArgumentList} ")"*/
  //"{"  {Statements}  "}"
 //FormalArgumentList    -->  ("int"|"float"|"char"|"bool")  Ident["["Expr"]"]
	PPRINT(printf("Enter Functiondefinition \n");)
	struct FunctionDef *p;
	struct Statement *s;
	struct Statement *s1;
	struct Symbol *sy, *m;
	struct Symbol *temp;
	enum SymType f;
	t = LEX.GetNextToken();
	//printf("\n\n\nFunction after   -- %s\n\n",t->lexeme);
	p = AST_ALLOC.newFunctionDef();
	if(t->Tok == Tokint||t->Tok ==Tokfloat||t->Tok==Tokchar||t->Tok ==Tokvoid||t->Tok==Tokbool)
   	{
		f = t->Tok == Tokint? integer:t->Tok == Tokfloat? real :t->Tok == Tokchar? charac :t->Tok == Tokvoid? voi :boolean; 
		t = LEX.GetNextToken();

	}
    else
    {
	   //fprintf(stderr,"Error:line %d Col %d return type expected before '%s'\n",t->line,t->col,t->lexeme);
	   error("Error:line %d Col %d return type expected before '%s'\n",t->line,t->col,t->lexeme);
    }
    if(t->Tok == Tokvariable)
    {
		if((sy = SYMTAB.findName(t->lexeme)) != NULL)
		{
			//printf("\n\n\nFunction search   -- %s\n\n",t->lexeme);
			//fprintf(stderr,"Error:Line %d  Col %d  '%s' is already defined on  Line %d Col %d therefore cannot be used as a function name\n",t->line,t->col,t->lexeme,sy->t->line,sy->t->col);
			error("Error:Line %d  Col %d  '%s' is already defined on  Line %d Col %d therefore cannot be used as a function name\n",
			t->line,t->col,t->lexeme,sy->t->line,sy->t->col);
		}
		else
		{
			sy = SYMTAB.insertName(t);
			sy->k = Func;
			sy->type = f;
			sy->scope = SYMTAB.getScope();
			//GLOBAL FUNC = sy;
		}
		p->s = sy;
		FUNC = p;
		t = LEX.GetNextToken();  
    }
    else
    {
		//fprintf(stderr,"Error:Line %d Col %d variable name expected before '%s'\n",t->line,t->col,t->lexeme);
		error("Error:Line %d Col %d variable name expected before '%s'\n",t->line,t->col,t->lexeme);
    }
   if(t->Tok == Toklparen)
    {
		t = LEX.GetNextToken();
		if(!getecount())
		{
			EmitGlobalDeclaration(gen);
		}	
		SYMTAB.openScope();
    }
   else
    {
		error("Error:Line %d Col %d '(' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error:Line %d Col %d '(' expected before '%s'\n",t->line,t->col,t->lexeme);
    }
    if(t->Tok == Tokvoid)
	{
		t = LEX.GetNextToken();
	} 
 	else if(t->Tok == Tokint||t->Tok == Tokfloat || t->Tok ==Tokchar ||t->Tok == Tokbool)
    {
	  //FormalArgumentList    -->  ("int"|"float"|"char"|"bool")  Ident["["Expr"]"]
		sy = FormalArgumentList();
		if(sy != NULL)
		{
			p->s->other_info = m = sy; 
		}
       while(t->Tok == Tokcolon)
       {
			t = LEX.GetNextToken();
			if(t->Tok == Tokint||t->Tok == Tokfloat || t->Tok ==Tokchar ||t->Tok == Tokbool)
			{
			    sy =  FormalArgumentList();
				if(sy != NULL)
				{
					m->other_info = sy;
					m = sy;
				}		
            }
			else
            {
				error("Error: Line %d Col %d Type expected before  '%s' \n",t->line,t->col,t->lexeme);
				//fprintf(stderr,"Error: Line %d Col %d Type expected before  '%s' \n",t->line,t->col,t->lexeme);
            } 			
			
       }  	   
	//m->other_info = NULL;
    }
	else
    {
		error("Error: Line %d Col %d  Formal parameters or 'void' expected before '%s' \n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  Formal parameters or 'void' expected before '%s' \n",t->line,t->col,t->lexeme);
    }
	if(t->Tok == Tokrparen)
	{
		t = LEX.GetNextToken();
	}	
	else
	{
		error("Error: Line %d Col %d  ')' expected before '%s' \n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  ')' expected before '%s' \n",t->line,t->col,t->lexeme);
	}
	
	if(t->Tok == Toklbrace)
	{
		t = LEX.GetNextToken();	
	}
	else
	{
		error("Error: Line %d Col %d  '{' expected before '%s' \n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '{' expected before '%s' \n",t->line,t->col,t->lexeme);
	}
	while(t->Tok == Tokvar||t->Tok == Tokconst||t->Tok == Tokarray||t->Tok == Tokif||t->Tok == Tokwhile||t->Tok == Tokcall||t->Tok == Tokreturn||t->Tok ==Tokvariable)
	{
		s  = Statements();
		if(s != NULL)
		{
			if(p->body == NULL)
			{
				p->body = s1 = s ; 
			}
			else
			{
				s1->next = s;
				s1 = s;
			}
		}
	}
	if(t->Tok == Tokrbrace)
	{
		SEM.functDefSemantics(p);	//Perform function Definition semantics before closing scope
		//emitLocalDeclaration(gen,p->s);
		if(!getecount())
		{
			EmitFunction(gen,p);
		}	
		SYMTAB.closeScope();
		t  = LEX.GetNextToken();	
	}
	else
	{
		error("Error: Line %d Col %d  '}' expected before '%s' \n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '}' expected before '%s' \n",t->line,t->col,t->lexeme);
		//SYMTAB.closeScope();
	}
	
	FUNC = NULL;
	PPRINT(printf("Leave FunctionDef\n");)
}	 	 
 static struct Symbol * FormalArgumentList(void)
{ 
	PPRINT(printf("Enter formal\n");)
 	//FormalArgumentList    -->  ("int"|"float"|"char"|"bool")  Ident["["Expr"]"]
	struct Symbol *sy = NULL;
	enum SymType f;
	enum SymKind d;
	int check  = 0;
	struct Expression *e = NULL;
	struct Token_info * q;
	f = t->Tok == Tokint? integer : t->Tok == Tokfloat? real :t->Tok == Tokchar? charac:boolean;    	
	t = LEX.GetNextToken();
    if(t->Tok == Tokvariable)
    {
		if((sy = SYMTAB.findName(t->lexeme)) != NULL && SYMTAB.getScope() != 0)
		{
			error("Error: Line %d Col %d  '%s' is already on Line %d Col %d",t->col,t->line,t->lexeme,sy->t->col,sy->t->line);
			//fprintf(stderr,"Error: Line %d Col %d  '%s' is already on Line %d Col %d",t->col,t->line,t->lexeme,sy->t->col,sy->t->line);
		}
		else
		{
			//sy = SYMTAB.insertName(t);
			//sy->type = f;
			check = 1;  //not inserting now it might be an array
			q = t;      //save if
		}
		t = LEX.GetNextToken();
        }
        else
        {
			error("Error: Line %d Col %d  '%s' is already on Line %d Col %d",t->col,t->line,t->lexeme,sy->t->col,sy->t->line);
			//fprintf(stderr,"Error:Line %d  Col %d  Identifier expected befor '%s'\n",t->line,t->col,t->lexeme);
        }
        if(t->Tok == Toklarr)
        {
			t = LEX.GetNextToken();
            e = Expr();
			check = 2;
            if(t->Tok == Tokrarr)
            {
				t = LEX.GetNextToken();
            }
           else
            {
				error("Error: Line %d Col %d ']' expected before '%s'\n",t->line,t->col,t->lexeme);
				//fprintf(stderr,"Error: Line %d Col %d ']' expected before '%s'\n",t->line,t->col,t->lexeme);
            }			
		}
	if(check)
	{
		sy = SYMTAB.insertName(q);
	    sy->type = f;
		sy->e = e;
		sy->arg = 1;
		sy->loc = 1;
		sy->scope = SYMTAB.getScope();
		if(check == 2)
		{
			sy->k  = Array;
		}
		else
		{
			sy->k = Var;
		}
	}
	PPRINT(printf("leave formal\n");)
		return sy;
}
    
static struct Statement * Statements(void)
{
  /*Statements            --> Datadef|ExecStament*/
   PPRINT(printf("Enter statement    %s\n",t->lexeme);)
	//t = LEX.GetNextToken();
	struct Statement * p  = NULL;
	if(t->Tok == Tokvar || t->Tok == Tokconst || t->Tok== Tokarray)
	{
	     Datadef();
	}
    else if(t->Tok == Tokif ||t->Tok == Tokwhile||t->Tok == Tokvariable||t->Tok == Tokreturn||t->Tok == Tokcall)
    {
		p = ExecStatement();
    }
	PPRINT(printf("Leave Statement \n");)
	return p;
}
static void Datadef(void)
{
	PPRINT(printf("Enter datadef \n");)
 /*DataDef				--> "var" ("int"|"float"|"char"|"bool")  Ident [":=" Expr] ";"
				  | "const" ("int"|"float"|"char"|"bool") Ident ":=" Expr ";"
                  |"array"  ("int"|"float"|"bool") Ident "[" Expr "]";   
ExecStament           -->IfStatement |WhileStatement|  AssignmentStatement | ReturnStatement|FunctionCall*/
	enum SymKind s;
	enum SymType ty;
	struct Symbol *sym;
	struct Expression *e = NULL;
	if(t->Tok == Tokvar)
	{	
		s = Var; 
		t = LEX.GetNextToken();
		if(t->Tok == Tokint || t->Tok == Tokfloat ||t->Tok == Tokchar ||t->Tok == Tokbool)
		{
			ty = t->Tok == Tokint? integer:t->Tok == Tokfloat? real:t->Tok == Tokchar? charac:boolean;    
			t = LEX.GetNextToken();
		}
		else
		{
			error("Error: Line %d Col %d  type expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  type expected before '%s'\n",t->line,t->col,t->lexeme);
		}	
		if(t->Tok == Tokvariable)
		{
			if((sym = SYMTAB.findName(t->lexeme)) != NULL && (sym->scope == SYMTAB.getScope()) || sym  &&  sym->scope == 5)
			{
				if(sym->scope == 5)
				{
					error("Error : Line %d Col %d '%s' is already defined in the library\n",t->line,t->col,t->lexeme);
					//fprintf(stderr,"Error : Line %d Col %d '%s' is already defined in the library\n",t->line,t->col,t->lexeme);
				}
				else
				{
					error("Error : Line %d Col %d '%s' is already defined on Line %d Col %d\n",t->line,t->col,t->lexeme,sym->t->line,sym->t->col);
					//fprintf(stderr,"Error : Line %d Col %d '%s' is already defined on Line %d Col %d\n",t->line,t->col,t->lexeme,sym->t->line,sym->t->col);
				}
			}
			else
			{
				sym = SYMTAB.insertName(t);
				sym->k = s;
				sym->type = ty;
				sym->scope = SYMTAB.getScope(); 
			}
			t = LEX.GetNextToken();
		}
		else  	
		{
			error("Error: Line %d Col %d  Identifier expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  Identifier expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		if(t->Tok == Tokequal)
		{
			t = LEX.GetNextToken();
			if(sym != NULL)
		    sym->e = Expr();
			
		}
		if(t->Tok == Toksemi)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
		}  	
	}	
	//| "const" ("int"|"float"|"char"|"bool") Ident ":=" Expr ";"
	
	else if(t->Tok == Tokconst)
	{
		t = LEX.GetNextToken();
		if(t->Tok == Tokint || t->Tok == Tokfloat ||t->Tok == Tokchar ||t->Tok == Tokbool)
		{
			ty = t->Tok == Tokint? integer :t->Tok == Tokfloat ? real :t->Tok == Tokchar? charac:boolean; 
			t = LEX.GetNextToken();
		}
		else
		{
			error("Error: Line %d Col %d  type expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  type expected before '%s'\n",t->line,t->col,t->lexeme);
		}	
		if(t->Tok == Tokvariable)
		{
			if((sym = SYMTAB.findName(t->lexeme)) != NULL && (sym->scope == SYMTAB.getScope() ) || sym && sym->scope == 5)
			{	
				if(sym->scope == 5)
				{
					error("Error : Line %d Col %d '%s' is already defined in the library\n",t->line,t->col,t->lexeme);
					//fprintf(stderr,"Error : Line %d Col %d '%s' is already defined in the library\n",t->line,t->col,t->lexeme);
				}
				else
				{
					error("Error: Line %d Col %d '%s' is already defined on Line %d Col %d",t->col,t->line,t->lexeme,sym->t->line,sym->t->col);
					//fprintf(stderr,"Error: Line %d Col %d '%s' is already defined on Line %d Col %d",t->col,t->line,t->lexeme,sym->t->line,sym->t->col);
				}	
			}
			else
			{
				sym = SYMTAB.insertName(t);
				sym->k = Const;
				sym->type = ty;
				sym->scope = SYMTAB.getScope();
				
			}
			t = LEX.GetNextToken();
		}
		else  	
		{
			error("Error: Line %d Col %d  Identifier expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  Identifier expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		if(t->Tok == Tokequal)
		{
			t = LEX.GetNextToken(); 
		}
		else
        {
			//fprintf(stderr,"Error :Line %d  Col %d  ':=' expected  before '%s' NB:constants are supposed to be initialized\n",t->line,t->col,t->lexeme);
			error("Error :Line %d  Col %d  ':=' expected  before '%s' \nNB:constants are supposed to be initialized\n",t->line,t->col,t->lexeme);
        } 
        e =  Expr();
		if(sym != NULL)
		sym->e = e;
			
		if(t->Tok == Toksemi)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
       		
	}
	
	//|"array"  ("int"|"float"|"bool") Ident "[" Expr "]";
	else if(t->Tok == Tokarray)
    {
		t = LEX.GetNextToken();
		if(t->Tok == Tokint|| t->Tok == Tokfloat ||t->Tok == Tokbool)
		{
			ty = t->Tok == Tokint? integer :t->Tok == Tokfloat ? real:boolean;
			t = LEX.GetNextToken();
        }
		else
        {
			error("Error: Line %d Col %d  type expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  type expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		if(t->Tok == Tokvariable)
		{
			if((sym = SYMTAB.findName(t->lexeme)) != NULL  && (sym->scope == SYMTAB.getScope())|| sym && sym->scope == 5)
			{
				if(sym->scope == 5)
				{
					error("Error : Line %d Col %d '%s' is already defined in the library\n",t->line,t->col,t->lexeme);
					//fprintf(stderr,"Error : Line %d Col %d '%s' is already defined in the library\n",t->line,t->col,t->lexeme);
				}
				else
				{
					error("Error: Line %d Col %d '%s' is already defined on Line %d Col %d",t->line,t->col,t->lexeme,sym->t->line,sym->t->col);
					//fprintf(stderr,"Error: Line %d Col %d '%s' is already defined on Line %d Col %d",t->line,t->col,t->lexeme,sym->t->line,sym->t->col);
				}	
			}
			else
			{
				sym = SYMTAB.insertName(t);
				sym->k = Array;
				sym->type = ty;
				sym->scope = SYMTAB.getScope();
			}
			t = LEX.GetNextToken();
		}
		else  	
		{
			error("Error: Line %d Col %d  Identifier expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  Identifier expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		if(t->Tok == Toklarr)
		{ 
			t = LEX.GetNextToken();
        }
		else
		{
			error("Error: Col %d Line %d '[' expected before '%s' \n",t->col,t->line,t->lexeme);
			//fprintf(stderr,"Error: Col %d Line %d '[' expected before '%s' \n",t->col,t->line,t->lexeme);
		}
		e = Expr();
		if(sym != NULL)
		sym->e = e;
		if(t->Tok == Tokrarr)
		{ 
			t = LEX.GetNextToken();
        }
		else
		{
			error("Error: Col %d Line %d ']' expected before '%s' \n",t->col,t->line,t->lexeme);
			//fprintf(stderr,"Error: Col %d Line %d ']' expected before '%s' \n",t->col,t->line,t->lexeme);
		}
		if(t->Tok == Toksemi)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
    }
	PPRINT(printf("Leave Datadeff\n");)
}	

static struct Statement * ExecStatement(void)
{
	PPRINT(printf("Enter ExecStatement \n");)
	struct Statement *p = NULL;
 /*ExecStament           -->IfStatement |WhileStatement|  AssignmentStatement | ReturnStatement|FunctionCall */
	if(t->Tok ==Tokif)
	{
		p = IfStatement();
	}
	else if(t->Tok == Tokwhile)
	{
		p = WhileStatement();
	}
	else if(t->Tok == Tokvariable)
	{
		p = AssignmentStatement();
	}
	else if(t->Tok == Tokreturn)
	{
		p = ReturnStatement();
	}
	else if(t->Tok == Tokcall)
	{
		p = FunctionCal();
	}
	PPRINT(printf("leave ExecStatement \n");)
	return p;	
}

static struct Statement * IfStatement(void)
{
	/*IfStatement			--> "if" Expr "then" "{" {ExecStatement} "}"
						{ "elseif" Expr "then" "{" {ExecStatement} "}" }
				 	      [ "else" "{" {ExecStatement} "}" ]
*/
    PPRINT(printf("Enter ifstatement \n");)
	struct Statement *p,  *n, *n1, *n2 = NULL;
	struct Statement *a, *a1, *a3, *a2 = NULL; 
	struct Expression *e;
	struct Statement *d, *d1,*d2 = NULL;  
	p = AST_ALLOC.newStatement();
	p->k = If;
	p->line = t->line;
	t = LEX.GetNextToken();
	e = Expr();
	p->stat.i.c  = e;
	p->stat.i.elseif = NULL;
	if(t->Tok == Tokthen)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error:  Line:%d  Col:%d   'then'  expected before '%s'  \n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"ERROR:  Line:%d  Col:%d   'then'  expected before '%s'  \n",t->line,t->col,t->lexeme);
	}
	if(t->Tok == Toklbrace)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	while(t->Tok == Tokif ||t->Tok == Tokwhile||t->Tok == Tokvariable||t->Tok == Tokreturn||t->Tok == Tokcall)
	{
		n =  ExecStatement();
		if(n2 == NULL)
		{
			n2 = n1 =  n;
		}
		else
		{
			n1->next = n;
			n1 = n;
		}
		
	}
	p->stat.i.then = n2;
	//elseif
	if(t->Tok == Tokrbrace)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	
	while(t->Tok == Tokelseif)
	{
		a = AST_ALLOC.newStatement();
		a->k = If;
		a->line = t->line;
		t = LEX.GetNextToken();
		e =  Expr();
		a->stat.i.c = e ;
		if(t->Tok == Tokthen)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("error:  Line:%d  Col:%d   'then'  expected before '%s'  \n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"error:  Line:%d  Col:%d   'then'  expected before '%s'  \n",t->line,t->col,t->lexeme);
		}
		if(t->Tok == Toklbrace)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		
		while(t->Tok == Tokif ||t->Tok == Tokwhile||t->Tok == Tokvariable||t->Tok == Tokreturn||t->Tok == Tokcall)
		{
			a1 = ExecStatement();
			if(a1 != NULL)
			{
				if(a2 == NULL)
				{
				  a2 = a3 = a1;
				}
				else
				{
					a3->next = a1;
					a3 = a1; 	
				}
			}
		}
		a->stat.i.then = a2;
		a->stat.i.els = NULL;  	//set the else part to NULL because elseif statement has no else part
		if(a != NULL)	
		{
			if(p->stat.i.elseif == NULL )    //Add elseif statement to the parent list
			{
				p->stat.i.elseif = a1 = a; 
			}
			else
			{
				a1->next = a;
				a1 = a;
			}
		}
			
		if(t->Tok == Tokrbrace)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		
	}
	//printf("\n\n\nEnter elseif    lexeme  %s \n\n\n",t->lexeme);
	if(t->Tok == Tokelse)     //[ "else" "{" {ExecStatement} "}" ]
	{
		t = LEX.GetNextToken();
		if(t->Tok == Toklbrace)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
		
		while(t->Tok == Tokif ||t->Tok == Tokwhile||t->Tok == Tokvariable||t->Tok == Tokreturn||t->Tok == Tokcall)
		{
			d = ExecStatement();
			if(d != NULL)
			{
				if(d2 == NULL)
				{
					d2 = d1 = d; 
				}
				else
				{
					d1->next = d;
					d1 = d;
				}
			}
		}
		p->stat.i.els = d2;
		if(t->Tok == Tokrbrace)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
	}
	PPRINT(printf("Leave ifstatement\n");)
	return p;
}	

static struct Statement * WhileStatement(void)
{	
  /*WhileStatement       -->"while"  Expr "do" "{" {ExecStatement} "}"*/
	struct Statement * p ,* p1,*p2, *p3 ;
	p1 = NULL;
	struct Expression *e ;
	p = AST_ALLOC.newStatement();
	p->k = While;
	p->line = t->line;
	PPRINT(printf("Enter while statement\n");)
	t = LEX.GetNextToken();
	e = Expr();
	p->stat.w.c = e ; 
	if(t->Tok == Tokdo)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  'do' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  'do' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	if(t->Tok == Toklbrace)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '{' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	while(t->Tok == Tokif ||t->Tok == Tokwhile||t->Tok == Tokvariable||t->Tok == Tokreturn||t->Tok == Tokcall)
		{
			p2 = ExecStatement();
			if(p2 != NULL)
			{
				if(p1 == NULL)
				{
					p1 = p3 = p2; 
				}
				else
				{
					p3->next = p2;
					p3 = p2;
				}
			}
				
		}
		p->stat.w.body = p1;
	if(t->Tok == Tokrbrace)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '}' expected before '%s'\n",t->line,t->col,t->lexeme);
	}		

	PPRINT(printf("Leave while\n");)
	return p;
}

static struct Statement * AssignmentStatement(void)
{
 /*AssignmentStatement	--> Ident["["Expr"]" ] ":=" Expr ";"*/
	PPRINT(printf("Enter Assign \n\n");)
	struct Expression *e;
	struct Symbol * symb;
	struct Expression * b, *b1, *b2;
	struct Statement *p;
	p = AST_ALLOC.newStatement();
	b = AST_ALLOC.newExpression();
	p->k = Assignment;
	p->line = t->line;
	//symb  && symb->scope != 0 && symb && symb->scope  !=  SYMTAB.getScope()
	if((symb = SYMTAB.findName(t->lexeme)) == NULL)
	{
		error("error: Line %d   Col %d  target of assignment '%s'  is not defined or if defined may be out of reach \n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d   Col %d  target of assignment '%s'  is not defined or if defined may be out of reach \n",t->line,t->col,t->lexeme);
	}
	//else
	//{
		t = LEX.GetNextToken();
		
		if(t->Tok == Toklarr)
		{
			b->kind = Operator;
			b->node.oper.oper = Array_Access;
			b->node.oper.line = t->line;
			b->node.oper.col  = t->col;
			b1 = AST_ALLOC.newExpression();
			b1->kind = Operand;
			if(symb != NULL)
			b1->t = assignType(symb);
			b1->node.operand.s = symb;
			b1->node.operand.p = 0;
			b->node.oper.lop = b1;  	
			t = LEX.GetNextToken();
			e = Expr();
			b->node.oper.rop = e;
			if(t->Tok == Tokrarr)
			{
				t = LEX.GetNextToken();
			}
			else
			{
				error("error: Line %d Col %d  ']' expected before '%s'\n",t->line,t->col,t->lexeme);
				//fprintf(stderr,"Error: Line %d Col %d  ']' expected before '%s'\n",t->line,t->col,t->lexeme);
			}
		}
		else
		{
			b->kind = Operand;	
			if(symb != NULL)	
			b->t = assignType(symb);
			b->node.operand.s = symb;
			b->node.operand.p = 0;
			
		}
		p->stat.a.desc = b;
	//}
	
	//t = LEX.GetNextToken();
/*	
	if(t->Tok == Toklarr)
	{
		t = LEX.GetNextToken();
		
		e = Expr();

		if(t->Tok == Tokrarr)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			fprintf(stderr,"Error: Line %d Col %d  ']' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
	}
	*/
	if(t->Tok == Tokequal)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  ':=' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  ':=' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
     e = Expr();
	 p->stat.a.src = e;
	if(t->Tok == Toksemi)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	PPRINT(printf("Leave Assign\n\n");)
	return p;
}	
static struct Statement * ReturnStatement(void)
{
/*ReturnStatement		--> "return" [ Expr] ";" */
	struct Expression *e = NULL;
	struct Statement *p;
	p = AST_ALLOC.newStatement();
	p->k = Return;
	p->line  =  t->line;
	PPRINT(printf("Enter Return");)
	t = LEX.GetNextToken();
	
	if(EXPRFIRST_SET(t->Tok))
	{
		e = Expr();
	} 
	p->stat.r.e = e;
	if(t->Tok == Toksemi)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	PPRINT(printf("Leavae Return \n");)
	return p;	
}
static struct Statement * FunctionCal( void)
{	
/* FunctionCall            -->"call" Ident"(" [ Expr { ":" Expr } ] ")"*/

	PPRINT(printf("Enter Fuctioncall\n");)
	struct Statement *p = NULL ;
	struct Expression *e, *e1, *e2;
    struct Expression  *e3 ;
	e1 = NULL;	
	struct Symbol *sym; 	
	p = AST_ALLOC.newStatement();
	p->k = FunctionCall;
	p->line = t->line;
	t = LEX.GetNextToken();
	if(t->Tok == Tokvariable)
	{
		if((sym = SYMTAB.findName(t->lexeme)) == NULL )
		{
			error("error: Line %d Col %d  ';' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d '%s' is not  a defined function \n",t->line,t->col,t->lexeme);
		}
		else
		{
			p->stat.f.name = sym;
			//FUNC2 = sym;
		}
			t = LEX.GetNextToken();
			
	}
	else
	{
		error("error: Line %d Col %d  Indetifier expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  Indetifier expected before '%s'\n",t->line,t->col,t->lexeme);
	} 
	if(t->Tok == Toklparen)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  '(' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '(' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	if(EXPRFIRST_SET(t->Tok))
	{
		e = Expr();
		if(e1 == NULL)
		{
			p->stat.f.arglist = e1 = e;
		}		
		while(t->Tok == Tokcolon)
		{
			//printf("Enter colon side\n");
			t = LEX.GetNextToken();
			//printf("\n\n\n\n  --- %s -- \n\n",t->lexeme);
			e3 = Expr();
			if(e3 != NULL)
			{
				//printf("\n\n\n\n\nafter expr t->tok =  %s  \n\n\n",t->lexeme);
				e1->next = e3;
				e1 = e3;
			}	
		}
		
	}
	if(t->Tok == Tokrparen)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d  ')' expected before '%s'\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  ')' expected before '%s'\n",t->line,t->col,t->lexeme);
	}
	
	if(t->Tok == Toksemi)
	{
		t = LEX.GetNextToken();
	}
	else
	{
		error("error: Line %d Col %d ';' expected before '%s'",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d ';' expected before '%s'",t->line,t->col,t->lexeme);
	}
	PPRINT(printf("Leave Fuctionccal \n");)
	return p;
}	

static struct Expression * Expr(void)
{
  /*Expr				 --> LogicalExpr*/
	PPRINT(printf("Enter Expr with '%s' \n",t->lexeme);)
	struct Expression * e = NULL;	
  if(EXPRFIRST_SET(t->Tok))
	{
		e = LogicalExpr();
	}
	else
	{
		error("error: Line %d Col %d  '%s' does not form a correct expression\n",t->line,t->col,t->lexeme);
		//fprintf(stderr,"Error: Line %d Col %d  '%s' does not form a correct expression\n",t->line,t->col,t->lexeme);
		t = LEX.GetNextToken();	
	}
	PPRINT(printf("Leave Expr \n");)
	return e;
} 
static struct Expression * LogicalExpr(void)
{
/*LogicalExpr			--> RelExpr { ("and" | "or") RelExpr}*/
	PPRINT(printf("Enter Logical Expr \n");)
	struct Expression *e;
	struct Expression *e1,*e2;
	e = RelExpr();
	while(t->Tok == Tokand || t->Tok == Tokor)
	{
		e2 = AST_ALLOC.newExpression();
		e2->kind = Operator;
		e2->node.oper.oper = t->Tok == Tokor ? Or : And ; 
		e2->node.oper.line = t->line;
		e2->node.oper.col = t->col;
		e2->node.oper.lop = e;
		t = LEX.GetNextToken();
		e1 = RelExpr();
		e2->node.oper.rop = e1;
		e = e2;
	}
	
	PPRINT(printf("Leave Logical Expr\n");)
	return e;	
}
static struct Expression * RelExpr(void)
{
	/*RelExpr				--> AddExpr { ("=" | "<" | ">" | "<=" | ">=" | "!=") AddExpr }*/
	struct Expression *e,*e1,*e2;
	PPRINT(printf("Enter realExapr\n");)
	e = AddExpr();
	while(t->Tok == Tokcompare || t->Tok == Tokless ||t->Tok == Tokgreater||t->Tok == Toklthan_eq||t->Tok ==Tokgthan_eq||t->Tok == Toknequal)
	{  
		e2 = AST_ALLOC.newExpression();
		e2->kind = Operator;
		e2->node.oper.line = t->line;
		e2->node.oper.col = t->col;
		e2->node.oper.oper = t->Tok ==Tokless? Lt:t->Tok==Tokcompare?Eq:t->Tok==Tokgreater?Gt:t->Tok==Toklthan_eq?Lte:t->Tok==Tokgthan_eq ? Gte: Neq;
		e2->node.oper.lop  =  e;
		t = LEX.GetNextToken();
		e1 =  AddExpr();
		e2->node.oper.rop = e1;
		e = e2;
	}
	PPRINT(printf("Leave relexpr\n");)
	return e;
}
static struct Expression * AddExpr(void)
{
 /*AddExpr				--> MulExpr { ("+" | "-") MulExpr }*/
	PPRINT(printf("Enter AddExpr \n");)
	struct Expression *e,*e1,*e2;
	
	 e = MulExpr();
	while(t->Tok == Tokplus || t->Tok == Tokminus)
	{ 
		e2 = AST_ALLOC.newExpression();
		e2->kind = Operator;
		e2->node.oper.line = t->line;
		e2->node.oper.col = t->col;
		e2->node.oper.oper = t->Tok == Tokplus? Plus : Minus;
		e2->node.oper.lop = e; 	
		t = LEX.GetNextToken();
		e1 = MulExpr();
		e2->node.oper.rop = e1;
		e = e2;
	}
	PPRINT(printf("Leave Addexpr\n");)
	return e;
}	
static struct Expression * MulExpr(void)
{
 PPRINT(printf("Enter MulExpr \n");)
/*MulExpr				--> UnaryExpr { ("*" | "/" | "mod") UnaryExpr }*/
	struct Expression *e,*e1,*e2;
	e = UnaryExpr();
	while(t->Tok == Toktimes || t->Tok == Tokdivide||t->Tok == Tokmod )
	{
		e2 = AST_ALLOC.newExpression();
		e2->kind = Operator;
		e2->node.oper.oper = t->Tok == Toktimes ? Mul:t->Tok == Tokdivide? Div : Mod ;
		e2->node.oper.lop = e;	
		e2->node.oper.line = t->line;
		e2->node.oper.col = t->col;
		t = LEX.GetNextToken();
		e1 = UnaryExpr();
		e2->node.oper.rop = e1;
		e = e2;
	}
	PPRINT(printf("Leave MulExpr  \n");)
	return e;
}
static struct Expression * UnaryExpr(void)
{	
	PPRINT(printf("Enter Unary expr  \n");)
	struct Expression *e; 
/*UnaryExpr			--> PrimaryExpr
				       | ("+" | "-" | "!") UnaryExpr*/
	if(t->Tok == Toknumber||t->Tok == Tokvariable ||t->Tok == Toktrue||
	   t->Tok == Tokfalse||t->Tok == Toklparen||t->Tok == Tokcharconst)
	{
		e = PrimaryExpr();
	}
	else if(t->Tok == Tokplus||t->Tok == Tokminus ||t->Tok == Toknot)
	{	
		e = AST_ALLOC.newExpression();
		e->kind = Operator;
		e->node.oper.oper = t->Tok == Tokplus ? Uplus :t->Tok == Tokminus? Uminus : Not;
		e->node.oper.line = t->line;
		e->node.oper.col = t->col;	
		t = LEX.GetNextToken();
		e->node.oper.rop  = UnaryExpr();
	}
	PPRINT(printf("Leave Unary  \n");)
	return e;
}
static struct Expression * PrimaryExpr(void)
{					   
/*PrimaryExpr			--> number
				       | Ident
					   | Ident[ "(" [ Expr { ":" Expr } ] ")"]
				       | "true"
				       | "false"
                       | "'"Character "'"					   
				       | "(" LogicalExpr ")"
					   | Ident "[" Expr "]"
Ident				--> "a" - "z" {"a" - "z" "0" - "9" "_"}
number				--> integer
				   | real
integer				--> "0" - "9" {"0" - "9"}
real				--> integer "." integer*/

	struct Expression *e = NULL, * e1, *hd = NULL;
	struct Expression *e2;
	struct Symbol * p;
	int once = 0;
	PPRINT(printf("Enter Primary \n");)
	if(t->Tok == Toknumber)
	{
		e = AST_ALLOC.newExpression();
		e->kind = Operand;
		e->node.operand.t = t;
		e->node.operand.p = 1;
		if(t->Kind == Real)
		{
			e->t = Real_t;
		}
		else if(t->Kind == Integer)	
		{
			e->t = Integer_t;
		}
		t = LEX.GetNextToken();
	}
	else if(t->Tok == Tokvariable)
	{
		if((p = SYMTAB.findName(t->lexeme)) != NULL )
		{
			t = LEX.GetNextToken();
			if(t->Tok == Toklparen)
			{
			 //printf("\n\n\n  call --\n\n\n");
				e =  AST_ALLOC.newExpression();
				e->kind = Operator;
				e->node.oper.oper = Fcall;
				e2 = AST_ALLOC.newExpression();
				e2->kind = Operand;
				e2->node.operand.s = p;
				e2->node.operand.p = 0;
				e2->t = assignType(p);	
				e->node.oper.lop = e2;
				e->t = e2->t;
				t = LEX.GetNextToken();
			//	do
				//{
					if(EXPRFIRST_SET(t->Tok))
					{
						e2 = Expr();
						hd = e1 = e2;
						while(t->Tok == Tokcolon)
						{
							t = LEX.GetNextToken();
							e2 = Expr();
							e1->next = e2;
							e1 = e2;
						}
					}
				
				//} while(t->Tok == Tokcolon);
				e->node.oper.rop = hd;
			
			    if(t->Tok == Tokrparen)
				{
					t = LEX.GetNextToken();
				}
				else
				{
					error("error: Line %d Col %d ')' expected before '%s' \n",t->line,t->col,t->lexeme);
					//fprintf(stderr,"Error: Line %d Col %d ')' expected before '%s' \n",t->line,t->col,t->lexeme);
				}
			}
			else if(t->Tok == Toklarr)
			{
				e = AST_ALLOC.newExpression();
				e->kind = Operator;
				e->node.oper.oper = Array_Access;
				e2 = AST_ALLOC.newExpression();
				e2->kind = Operand;
				e2->node.operand.s = p;
				e2->node.operand.p = 0;
				e2->t = assignType(p);
				e->node.oper.lop = e2;
				t = LEX.GetNextToken();
				e->node.oper.rop = Expr();
				//t = LEX.GetNextToken();
				
				if(t->Tok == Tokrarr)
				{
					t = LEX.GetNextToken();
				}
				else
				{
					error("error: Line %d Col %d  ']' expected  before '%s' \n",t->line,t->col,t->lexeme);
					//fprintf(stderr,"Error: Line %d Col %d  ']' expected  before '%s' \n",t->line,t->col,t->lexeme);
				}
			}
			else
			{
				e =  AST_ALLOC.newExpression();
				e->kind = Operand;
				e->node.operand.s = p;
				e->node.operand.p = 0;
				e->t = assignType(p);
			}
		}
		else
		{
			error("error: Line %d Col %d  '%s' undefined  \n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  '%s' undefined  \n",t->line,t->col,t->lexeme);
			t = LEX.GetNextToken();
		}
	}
	else if(t->Tok == Toktrue||t->Tok == Tokfalse)
	{
		e = AST_ALLOC.newExpression();
		e->kind = Operand;
		e->node.operand.t = t;
		e->node.operand.p = 1;
		e->t = Boolean_t;
		t = LEX.GetNextToken();
	}
	else if(t->Tok == Tokcharconst)
	{
		e = AST_ALLOC.newExpression();
		e->kind = Operand;
		e->t = Charconst_t;
		e->node.operand.t = t;
		e->node.operand.p = 1;
		t = LEX.GetNextToken();
	}
	else if(t->Tok == Toklparen)
	{
		t = LEX.GetNextToken();
		e = LogicalExpr();
		if(t->Tok == Tokrparen)
		{
			t = LEX.GetNextToken();
		}
		else
		{
			error("error: Line %d Col %d  ')' expected before '%s'\n",t->line,t->col,t->lexeme);
			//fprintf(stderr,"Error: Line %d Col %d  ')' expected before '%s'\n",t->line,t->col,t->lexeme);
		}
   }
   PPRINT(printf("Leave Primary \n");)
   return e;
}   
static enum Type assignType(struct Symbol *p)
{
	PPRINT(printf("\n\n Assigning Type to ' %s'  type = %d \n\n",p->t->lexeme,p->type);)
	if(p->type == voi)
	{
		return Void_t;
	}
 	if(p->type == integer)
	{
		return Integer_t;
	}
	if(p->type == real)
	{
		return Real_t;
	}	
	if(p->type == charac)
	{
		return Charconst_t;
	}	
	if(p->type == boolean)
	{
		return Boolean_t;
	}	

}
	
	
	
