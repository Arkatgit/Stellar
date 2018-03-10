/***********************************************
 *Code generation routines - generate language for the
 *target machine.
 *
 *************************************************/
#include "symtab.h"
#include "generator.h"
#include "ast.h" 
#include "scanner.h"
#include<string.h>
#include<stdarg.h>
#include<stdio.h>
#include "output.h"
#include "scanner.h"

#ifdef P_DEBUG
#define PRINT(x) x
#else
#define PRINT(x)
#endif



void EmitAll(struct codegen *g,struct Statement * p);	
void EmitGlobalDeclaration(struct codegen  * g );
static void  translateVarDeclaration( struct codegen * g, struct Symbol  *s );
static void translateArrayDeclaration(struct codegen * g, struct Symbol * s);
static void translateFunctionDeclaration(struct codegen * g,struct Symbol * s);
static char * translateType(struct codegen * g , struct Symbol * s );
 void EmitFunction(struct codegen *g,struct FunctionDef *f );
static char * multStringCat(char * desc , char *src1, ... );
void emitLocalDeclaration(struct codegen * g , struct Symbol *s);
static void translateStatements( struct codegen * g , struct Statement  * p );
static void translateIf(struct codegen * g , struct Statement * e);
static void translateFunctCall(struct codegen * g ,struct Statement *p);
static void translateReturn(struct codegen * g ,struct Statement * p);
static void translateWhile(struct codegen * g , struct Statement *p);
static void translateAssign(struct codegen * g ,  struct Statement * p);
static char * translateExpression(struct codegen * g,struct Expression * e);
static char * translateOperation(struct codegen * g ,struct Expression * e);
static char * translateLogicals(struct codegen * g, struct Expression * e);
static char * translateRelationals(struct codegen *  g, struct Expression * e);
static char * translateArithmetics(struct codegen * g ,struct Expression * e);
static char * translateUnary(struct codegen *g ,struct Expression * e);
static char * translateArray(struct codegen * g ,struct Expression * e);
static char * translateFcall(struct codegen * g ,struct Expression * e);
static char * translateMod(struct codegen *g ,struct Expression * e);
char * maketempVar(void);
char * newStringptr(int size);
static void delString(char * p );
void EmitRemainGlobal(struct codegen * g );

void EmitAll(struct codegen *g,struct Statement * p)
{
	PRINT(printf(" Enter Generator::EmitAll \n");)
	struct output_w * t;
	t = g->writer;
	EmitRemainGlobal(g);
	t->writes(t,"main() {",NULL );
	translateStatements(g,p);
	t->writes(t,"} \n",NULL);
	t->writes(t,NULL);
	PRINT(printf(" Leave Generator::EmitAll \n");)
	return;
}	
/*void EmitInclude(struct codegen * g)
{
	struct output_w  * t;
	t = g->writer;
	t->writes(t , "#include<stdio.h>", NULL);
	t->writes(t, "enum bool{ false , true };",NULL);
}*/


static int m = 0;
void EmitHeading(struct codegen  * g )
{
	struct output_w  * t;
	t = g->writer;
	t->writes(t, "enum bool{ false , true };",NULL);
	t->writes(t, "\n#include \"types.h\" "  ,"\n" ,NULL);
	
}

 void EmitGlobalDeclaration(struct codegen  * g )
{
	PRINT(printf(" Enter Generator::EmitGlobalDeclaration \n");)
	struct Symbol * s;
	enum SymKind a;
	struct output_w  * t;
	t = g->writer;
	//enum to take care of the bool type in source; 
	while( (s = getNextSymb(0)) != NULL )
	{
		PRINT(printf("From symtab -%s  \n",s->t->lexeme);)
		a =  s->k;
		/*if(m == 0)
		{
			//t->writes(t , "#include<stdio.h>", NULL);
			t->writes(t, "enum bool{ false , true };",NULL);
			m = 1;
		}*/
		if(s->t->Tok == Toklib)
		{
			continue;
		}	
		if(a == Var || a == Const )
		{
			if(s->generated == 0)
			{
				translateVarDeclaration(g , s);
				s->generated = 1;
			}	
		}
		else if(a == Array)
		{
			if(s->generated == 0)
			{
				translateArrayDeclaration(g ,s);
				s->generated = 1;
			}	
		}
		else if(a == Func)
		{
			//translateFunctionDeclaration(g , s);
		}
	}
	PRINT(printf(" Leave Generator::EmitGlobalDeclaration \n");)
}

void EmitRemainGlobal(struct codegen * g)
{
	struct Symbol *s;
	
	while( (s = getNextSymb(0)) != NULL )
	{
		PRINT(printf("From symtab -%s  \n",s->t->lexeme);)
		enum SymKind a;
		a =  s->k;
		if(s->t->Tok == Toklib)
		{
			continue;
		}
		if(!s->generated)
		{	
			if(a == Var || a == Const )
			{
				translateVarDeclaration(g , s);
				s->generated = 1;
			}
			else if(a == Array)
			{
				translateArrayDeclaration(g ,s);
				s->generated = 1;
			}
		}	
	}
}

static void  translateVarDeclaration( struct codegen * g, struct Symbol  *s )
{
	PRINT(printf(" Enter Generator::translateVarDeclaration \n");)
	struct output_w  * t;
	enum SymType  a; 
	t = g->writer;
	char * q; 	
	if(s->k == Const)
	{	
	
		if(s->e != NULL)
		{
			q = translateExpression(g, s->e);
		}	
		if( (a = s->type) == integer )
		{
			t->writes( t ,"const int ",s->t->lexeme , NULL);
		}
		else if(a ==  real )
		{ 
			t->writes(t ,"const float ",s->t->lexeme,NULL);  
		}
		else if(a == charac )
		{
			t->writes(t,"const char ", s->t->lexeme , NULL );
		}
		else
		{
			t->writes(t, "const emum bool ",s->t->lexeme ,NULL);
		}
		if(s->e != NULL)
		{
			//q = translateExpression(g, s->e);
			t->writes(t , " = " , q ,";",  NULL);
		}	
		delString(q);
	}
	else
	{
		//Var 
		if(s->e  != NULL)
		{
			q = translateExpression(g, s->e);
		}	
		if( (a = s->type) == integer )
		{
			t->writes( t ," int ",s->t->lexeme , NULL);
		}
		else if(a ==  real )
		{
			t->writes(t ," float ",s->t->lexeme,NULL);  
		}
		else if(a == charac )
		{
			t->writes(t,"char ", s->t->lexeme , NULL );
		}
		else
		{
			t->writes(t, "enum bool ",s->t->lexeme ,NULL);
		}
		if(s->e  != NULL  )
		{
			//q = translateExpression(g, s->e);
			t->writes(t , " = " , q , ";" , NULL);
			delString(q);
		}	
		else
		{
			t->writes(t," ; ", NULL );
		}
	}
	PRINT(printf(" Leave Generator::translateVarDeclaration \n");)
  }

static void translateArrayDeclaration(struct codegen * g, struct Symbol * s)
{
	PRINT(printf(" Enter Generator::translateArrayDeclaration \n");)
	struct output_w  * t;
	t = g->writer;
	enum SymType a;
	char * q;
	q = translateExpression( g , s->e);
	if( (a = s->type) == integer )
	{
		t->writes( t ," int ", NULL );
	}	
	else if(a  ==  real)
	{
		t->writes(t , "float ", NULL);
	}
	else	//bools
	{
		t->writes(t,"enum bool " , NULL);
	}
	
	t->writes(t ,s->t->lexeme,"[",q,"] "," ; ", NULL);
	delString(q);
	PRINT(printf(" Leave Generator::translateArrayDeclaration \n");)
	return;
}

static void translateFunctionDeclaration(struct codegen * g,struct Symbol * s)
{
	PRINT(printf(" Enter Generator::translateFunctionDeclaration \n");)
	struct output_w  * t;
	t =  g->writer;
	enum SymType a;
	struct Symbol * sy;
	int count = 0; 
	char * q;
	if( (a = s->type)  == integer)
	{
		t->writes(t ,"int ",s->t->lexeme ,"(", NULL);
	}
	else if(a == real)
	{
		t->writes(t," float ",s->t->lexeme,"(",  NULL );
	}
	else if(a == charac)
	{
		t->writes(t ,"char ",s->t->lexeme ,"(", NULL);
	}
	else if(a == boolean)
	{
		t->writes(t ,"enum bool ",s->t->lexeme ,"(", NULL);
	}
	else 
	{
		//void
		t->writes(t ,"void ",s->t->lexeme ,"(", NULL);
	}
	while( (sy = getNextSymb(1)) && sy->arg == 1 )
	{
		if(count == 0)
		{
			q = translateType(g , sy);
			count++;
			t->writes(t, q ,NULL);
		}
		else
		{
			t->writes(t,"," , q ,NULL );
		}	
	}
	if(count  == 0)
	{
		t->writes(t," void ",NULL);
	}
	t->writes( t ," ); " , NULL);
	PRINT(printf(" Leave Generator::translateFunctionDeclaration \n");)
}

static char * translateType(struct codegen * g , struct Symbol * s )
{
	PRINT(printf(" Enter Generator::translateType \n");)
	struct output_w  * t;
	t =  g->writer;
	enum SymType a;
	char * d ,* q;
	d = newStringptr(512);
	if( (a = s->type)  == integer)
		{
			strcpy(d , " int " );
		}
	else if(a == real)
	{
		strcpy(d , " float " );
	}
	else if(a == boolean)
	{
		strcpy(d , " enum bool " );
	}
	else
	{
		strcpy(d , "char ");
	}
	d = multStringCat( d ," ", s->t->lexeme , NULL);
	
	if(s->k == Array)
	{ 
		q = translateExpression( g ,  s->e);
		d = multStringCat(d ,"[", q ,"]" , NULL);
		delString(q);
	}
	PRINT(printf(" ---- %s \n",d);)
	PRINT(printf(" Leave Generator::translateType \n");)
	return d;
}

 void EmitFunction(struct codegen *g,struct FunctionDef *f )
{
	PRINT(printf(" Enter Generator::EmitFunction \n");)
	struct Symbol *s ,* w;
	struct output_w  * t;
	t =  g->writer;
	s = f->s;
	char * q;
	int count = 0; 
	switch(s->type)
	{
		case integer:
				t->writes(t," int ",s->t->lexeme , "(" , NULL );
				break;
		case real:		
				t->writes(t," float ",s->t->lexeme , "(" , NULL );
				break;
		case charac:
				t->writes(t," char ",s->t->lexeme , "(" , NULL );
				break;
		case voi:
				t->writes(t," void ",s->t->lexeme, "(", NULL );
				break;
		case boolean:
				t->writes(t, "enum bool ",s->t->lexeme,"(", NULL);
				break;
	}
	
	for(s = s->other_info ; s != NULL   ;  s = s->other_info)
	{
			q = translateType( g , s );
			if(count == 0)
			{
				t->writes(t ,q ,NULL );
				count++; 
			}
			else
			{
				t->writes(t ," ,", q ,NULL);
			}
	}
	t->writes(t," ) { ", NULL );
	
	emitLocalDeclaration(g , s );
	translateStatements(g ,f->body );
	t->writes(t," } ", NULL);
	PRINT(printf(" Leave Generator::EmitFunction \n");)
	return;
} 

static char * multStringCat(char * desc , char *src1, ... )
{
	PRINT(printf(" Enter Generator::multStringCat \n");)
	va_list ap;
	char * a ,* b;
	a  =  strcat(desc , src1);
	va_start(ap,src1);
	for( b = va_arg( ap , char *) ;  b != NULL ; b = va_arg(ap , char *) )
	{
		strcat(desc, b);
	}
	PRINT(printf(" Leave Generator::multStringCat with '%s'\n",a);)
	return a;
}


 void emitLocalDeclaration(struct codegen * g , struct Symbol *s)
{
	PRINT(printf(" Enter Generator::emitLocalDeclaration \n");)
	struct Symbol * p;
	struct output_w * t;
	t =  g->writer;
	char * q ,* v;
	
	while(p = getNextSymb(1))
	{
		if(p->loc == 1)
		{
			continue;
		}
		q =  translateType(g,p);
		if(p->e == NULL)
		{
			t->writes(t , q ,"; ", NULL);
		}
		else
		{
			v = translateExpression(g, p->e);
			if(p->k == Array)
			{
				t->writes(t , q , " ; ",  NULL);
			}
			else
			{
				t->writes(t , q , " = " , v,";",  NULL);
			}	
		}	
	}
	PRINT(printf(" Leave Generator::emitLocalDeclaration \n");)
}
static void translateStatements( struct codegen * g , struct Statement  * p )
{
	PRINT(printf(" Enter Generator::translateStatements \n");)
	while(p != NULL)
	{	
		switch(p->k)
		{
			case If:
				translateIf(g, p);
				break;
			case Assignment:
				translateAssign(g , p);
				break;
			case While:
				translateWhile(g , p);
				break;
			case Return:
				translateReturn(g , p);
				break;
			case FunctionCall:
				translateFunctCall(g, p);
				break;
		
		}
		p= p->next;
	}
		PRINT(printf(" Leave Generator::translateStatements \n");)
}

static void translateIf(struct codegen * g , struct Statement * e)
{
	PRINT(printf(" Enter Generator::translateIf \n");)
	struct output_w * t;
	t = g->writer;
	char * q;
	struct Statement * w;
	q = translateExpression(g , e->stat.i.c);
	t->writes(t,"if( ", NULL );
	t->writes(t , q , " ){", NULL);
	delString(q);
	translateStatements( g ,e->stat.i.then);
	t->writes(t, "}", NULL);
	for(w =  e->stat.i.elseif ; w != NULL ; w = w->next)
	{	
		q = translateExpression(g,w->stat.i.c);
		t->writes(t ," if( ", q ,") {", NULL);
		translateStatements(g,w->stat.i.then);
		t->writes(t, "}", NULL);
		delString(q);
	
	}
	if(e->stat.i.els)
	{
		t->writes(t,"else { ", NULL);
		translateStatements(g,e->stat.i.els);
		t->writes(t," } ", NULL);
	}
	PRINT(printf("  Generator::TranslateIf \n");)	
}
static void translateFunctCall(struct codegen * g ,struct Statement *p)
{
	PRINT(printf(" Enter Generator::translateFunctioncall \n");)
	struct output_w * t;
	t = g->writer;
	char * q ;
	struct Expression * e;
	t->writes(t , p->stat.f.name->t->lexeme,"(", NULL );
	int count = 0 ;
	for( e = p->stat.f.arglist ; e != NULL ; e = e->next )
	{
		//printf("herefcall \n\n\n");
		q = translateExpression(g , e);		
		if(count == 0)
		{
	
			t->writes(t , q , NULL);
			count++;
		}
		else
		{
			t->writes( t , " , ", q , NULL);
		}
		delString(q);
	}
	t->writes(t , " ); ", NULL );
	PRINT(printf(" Leave Generator::Translate FunctionCall \n");)
	return;
}

static void translateReturn(struct codegen * g ,struct Statement * p)
{
	PRINT(printf(" Enter Generator::Retrun \n");)
	struct output_w * t;
	t = g->writer;
	char * q;
	struct Expression *e  = p->stat.r.e;  
	q = translateExpression( g , e);
 	t->writes(t ," return ", q , " ; ", NULL );
	delString(q);
	PRINT(printf(" Leave Generator::translateReturn \n");)
	return;
}
static void translateWhile(struct codegen * g , struct Statement *p)
{
	PRINT(printf(" Enter Generator::translateWhile \n");)
	struct output_w * t;
	t = g->writer;
	char * q;
	char *temp ,temp2 ;
	struct Expression * e =  p->stat.w.c;
	q = translateExpression( g , e);
	//temp = newStringptr(512);
	//strcpy(temp,maketempVar());
	//t->writes(t,"enum bool ",temp , " ; " , NULL  );
	
	t->writes(t," while( ", q , ") { ", NULL );
	translateStatements(g , p->stat.w.body);
	
	temp = translateExpression( g , e);
	t->writes(t, q ," = ",temp, " ; " , NULL);
	t->writes(t," } ", NULL);
	delString(temp);
	delString(q);
	PRINT(printf(" Leave Generator::translateWhile\n");)
	return;
} 

static void translateAssign(struct codegen * g ,  struct Statement * p)
{
	PRINT(printf(" Enter Generator::transltateAssign \n");)
	struct output_w * t;
	t = g->writer;
	char * a;
	char * b;
	a = translateExpression( g , p->stat.a.desc);
	b = translateExpression( g , p->stat.a.src);
	t->writes(t , a , " = " , b , " ; ", NULL);
	delString(a);
	delString(b);
	PRINT(printf(" Enter Generator::translateAssign \n");)
	return;
}

static char * translateExpression(struct codegen * g,struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateExpression \n");)
	enum NodeKind kind;
	char * temp ,  *p ;
	temp = newStringptr(512);
	//printf("here\n");	
	kind = e->kind;
	if(kind == Operand)
	{
		//printf("---here---- %s\n",e->node.operand.t->lexeme);
		
		if(e->node.operand.p == 0)
		{
			strcpy(temp , e->node.operand.s->t->lexeme);
		}
		else if(e->node.operand.p ==1)
		{
			strcpy(temp , e->node.operand.t->lexeme);
		}
		//printf("---here----\n");
	}
	else if(kind == Operator)
	{
		p = translateOperation(g, e);
		strcpy(temp, p);
		delString(p);	
	}
	else
	{
		temp[0] = '\0'; 
	}
	PRINT(printf(" Leave Generator::translateExpression \n");)
	return temp;
}

static char * translateOperation(struct codegen * g ,struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateOperation \n");)
	char * temp, * temp2 ;
	temp = newStringptr(512);
	enum Exop t;
	t = e->node.oper.oper;
	switch(t)
	{
		case And:
		case Or:
		case Not:
		temp2 = translateLogicals(g , e);
		break;
		
		case Eq:
		case Lt:
		case Gt:
		case Lte:
		case Gte:
		case Neq:
		temp2 = translateRelationals(g , e);
		break;
		
		case Plus:
		case Minus:
		case Mul:
		case Div:
		temp2 = translateArithmetics(g , e);
		break;
		
		case Uplus:
		case Uminus:
		temp2  =  translateUnary(g , e);
		break;
		
		case Fcall:
		temp2 = translateFcall(g , e);
		break;
		case Array_Access:
		temp2 = translateArray(g ,e);
		break;
		case Mod:
		temp2 = translateMod(g , e);
		break; 
	}
	strcpy(temp , temp2);
	delString(temp2);
	PRINT(printf(" Leave Generator::translateOperation \n");)
	return temp;	
}

static char * translateLogicals(struct codegen * g, struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateLogicals \n");)
	struct output_w * t;
	t = g->writer;
	char * temp , * temp2 , * temp3;
	enum Exop a;
	temp = newStringptr(512);
	strcpy(temp,maketempVar());
	t->writes(t," enum bool ",temp," ; ",NULL);
	a = e->node.oper.oper;
	struct Expression *  l = e->node.oper.lop;
	struct Expression  * r = e->node.oper.rop;
	if(a == And)
	{
		temp2 = translateExpression(g , l);
		temp3 = translateExpression(g , r);
		t->writes(t , temp ," = (" , temp2, ") && ( ", temp3 ," );",NULL );
	}
	else if(a == Or)
	{
		temp2 = translateExpression(g , l);
		temp3 = translateExpression(g , r);
		t->writes(t ,temp, " =  ( ",temp2 , " ) || ( " ,temp3 ," ) ;" , NULL );
	}
	else
	{				//logical not
		temp2 = translateExpression(g , r);
		t->writes(t ,temp , " = !(" ,temp2,") ", NULL);	
		
	}
	PRINT(printf(" Leave Generator::translateLogicals \n");)
	return temp;
}

static char * translateRelationals(struct codegen *  g, struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateRelationals \n");)
	struct output_w * t;
	t = g->writer;
	struct Expression * l =  e->node.oper.lop;
	struct Expression * r = e->node.oper.rop;
	char * temp ,* temp2 , * temp3 ;
	enum Exop a = e->node.oper.oper;
	temp = newStringptr(512);
	strcpy(temp,maketempVar());
	t->writes(t,"enum bool ",temp , " ; " , NULL  );
	temp2 = translateExpression(g , l );
	temp3 = translateExpression(g , r);	
	
	if(a == Eq)
	{
		t->writes(t ,temp," = ( ", temp2,") == ( ",temp3,") ; ",NULL );
	}
	else if(a ==  Lt)
	{
		t->writes(t ,temp," = ( ", temp2,") < ( ",temp3,") ; ",NULL );
	}
	else if(a ==  Gt)
	{
		t->writes(t ,temp," = ( ", temp2,") > ( ",temp3,") ; ",NULL );
	}
	else if(a ==  Lte)
	{
		t->writes(t ,temp," = ( ", temp2,") <= ( ",temp3,") ; ",NULL );
	}
	else if(a ==  Gte)
	{
		t->writes(t ,temp," = ( ", temp2,") >= ( ",temp3,") ; ",NULL );
	}
	
	else	//not equall
	{
		t->writes(t ,temp," = ( ", temp2,") != ( ",temp3,") ; ",NULL );
	}
	return temp;	
	PRINT(printf(" Leave Generator::relationals \n");)
}

static char * translateArithmetics(struct codegen * g ,struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateArithmetics \n");)
	struct output_w * t;
	t = g->writer;
	char * temp , * temp2, * temp3;
	struct Expression *l , *r;
	enum Exop a = e->node.oper.oper;	
	l =  e->node.oper.lop;
	r = e->node.oper.rop;
	temp = newStringptr(512);
	strcpy(temp , maketempVar());
	t->writes(t, " float " , temp, " ;" ,NULL);
	temp2 = translateExpression(g , l);
	temp3 = translateExpression(g,r ) ;
	if(a == Plus)
	{
		t->writes(t , temp," =  ( ", temp2,") + (",temp3," ); ",NULL );
	}
	else if(a == Minus)
	{
		t->writes(t , temp," =  ( ", temp2,") - (",temp3," ); ",NULL );
	}
	else if(a == Mul)
	{
		t->writes(t , temp," =  ( ", temp2,") * (",temp3," ); ",NULL );
	}
	else 	
	{
		//if(a == Div)
		t->writes(t , temp," =  ( ", temp2,") / (",temp3," ); ",NULL );
	}
	PRINT(printf(" Leave Generator::translateArithmetics \n");)
	return temp;
}

char * translateUnary(struct codegen *g ,struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateUnary \n");)
	struct output_w * t;
	t = g->writer;
	struct Expression * l, * r;
	r = e->node.oper.rop;
	enum Exop a = e->node.oper.oper;
	char * temp , * temp2 ,* temp3;
	temp = newStringptr(512);
	strcpy(temp ,maketempVar());
	t->writes(t," float  ",temp , " ; ",NULL);
	temp2 = translateExpression(g, r );
	if(a == Uminus)
	{
		t->writes(t ," =  -(",temp2,"); ",NULL );
		delString(temp2);
	}
	else
	{
		t->writes(t ," =  (",temp2,"); ",NULL);
		delString(temp2);	
 	}
	PRINT(printf(" leave Generator::translateUnary \n");)
	return temp; 
}

static char * translateArray(struct codegen * g ,struct Expression * e)
{	
PRINT(printf(" Enter Generator::translateArray \n");)
	struct Expression * l , * r;
	char * temp , * temp2 , * temp3;
	l = e->node.oper.lop;
	r = e->node.oper.rop;
	temp = newStringptr(512);
	temp2 = translateExpression(g,l);
	temp3 = translateExpression(g,r);
	multStringCat(temp, temp2,"[" ,temp3, "] ",NULL);
	delString(temp2);
	delString(temp3);
	PRINT(printf(" Leave Generator::translateArray \n");)
	return temp;
}
static char * translateFcall(struct codegen * g ,struct Expression * e)
{ 
    PRINT(printf(" Enter Generator::translateFcall \n");)
    struct Expression *l, * r, * p ;
    char * temp1 , * temp2, * temp3 ;
    l = e->node.oper.lop;
    r  = e->node.oper.rop;
    temp2 = translateExpression(g, l);
    temp1 = newStringptr(512);
    multStringCat(temp1,temp2, "( ", NULL);
    delString(temp2);
    int i = 0;
    for(p = r ; p != NULL;  p = p->next)
    {
		temp2 = translateExpression(g , p);
		if(i == 0)
		{
			multStringCat(temp1,temp2, NULL);
			i++;
		}
		else
		{
			multStringCat(temp1,",",temp2, NULL);
		}
		delString(temp2);
    }
    
    multStringCat(temp1,")",NULL);
	PRINT(printf(" Leave Generator::translateFcall \n");)
    return temp1;
    
}


static char * translateMod(struct codegen *g ,struct Expression * e)
{
	PRINT(printf(" Enter Generator::translateMod \n");)
	char * temp,* temp1 , * temp2 ;
	temp = newStringptr(512);
	struct Expression *l , *r;
	l = e->node.oper.lop;
	r = e->node.oper.rop;
	temp1 = translateExpression(g , l);
	temp2 = translateExpression(g , r);
	multStringCat(temp, "Mod( ",temp1,",",temp2,")",NULL);
	delString(temp1);
	delString(temp2);
	PRINT(printf(" Enter Generator::translateMod \n");)
	return temp;
}

 char * maketempVar(void)
{
	PRINT(printf(" Enter Generator::maketempVar\n");)
	static char Id[32] = "__v";
	static int id  = 0;
	int t, a = 3 ;
	t = id;
	do
	{
		Id[a++] = t % 10 + '0';
		t /= 10;
	}while(t);
	id++;
	Id[ a] = '\0';
	PRINT(printf(" Enter Generator::maketempVar \n");)
	return Id;
}


//stack based memory allocation and dealocation routines

#define HEAP_SIZ 5120
static char  heap[HEAP_SIZ];
char * hptr  = heap;

char * newStringptr(int size)
{
	PRINT(printf(" Enter Generator::newStringptr \n");)
	char * p;
	if( hptr + size - heap < HEAP_SIZ )
	{
		 p = hptr;
		hptr += size;  
	}
	else
	{
		p =  (char *)malloc(sizeof(char) * size); 
	}
	if(p != NULL)
	{
		p[0] = '\0';
	}
	PRINT(printf(" Leave Generator::newStringptr\n");)
	return p;
} 

static void delString(char * p )
{
	PRINT(printf(" Enter Generator::delString\n");)
	if(p >= heap  && p <= heap + HEAP_SIZ)
	{
			hptr = p;
	}
	else
	{
		free(p);
	}
	PRINT(printf(" Leave Generator::delString\n");)
}

#define NUM_WRITERS 10
struct codegen  generators[NUM_WRITERS]; 
static int Current = 0;
struct codegen * newGenerator(struct output_w * writer)
{
	PRINT(printf(" Enter Generator::newGenerator\n");)
	int i = Current;
	if( i < NUM_WRITERS)
	{
		generators[i].writer = writer;	
	}
	Current++;
	PRINT(printf(" Leave Generator::newGenerator\n");)
	return &generators[i];
}

