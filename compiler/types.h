#include<math.h>
extern void printbool(int);
extern void printfloat(float);
extern float get_float( void);
extern int get_int( void);
extern int addints(int a,...);
extern int multiplyints(int a,...); 
extern void dupintarray(int *a,int *b,int  l);
extern void dupfloatarray(float *a,float *b,int  l);
extern int search_index(float *a,int l , float key );
extern void sortintarray(int a[],int l);
extern void sortfloatarray(float a[],int l);
extern int  factorial(int a);
extern int permutation(int n,int r);
extern int combination(int n,int r) ;
extern float ap_nterm(float a,float d,float n); 
extern float ap_sumnterms(float a,float d,float n);
extern float gp_nterm(float a,float r, float n );
extern float gp_suminfinity(float a,float r);
extern float ShowMeanTable(float * a ,int len );
extern float ShowStdTable(float * a,int len );
#define PIE	(3.14159265358979323846)

//Gravitational Constant	
#define	 GC  (6.67 * pow(10, -11) ) 

//Planks Constant

#define PC	( 6.626 * pow(10,-34))

//Avagadro Constant

#define AC  (6.022 * pow( 10 ,23) )

//Faradays Constant

#define FC  ( 9.649 * pow(10,4))


//acceleration due to gravity

#define AG ( 9.8) 

//speed of light 
 
#define SL  (2.9979 * pow(10,8))


/** General Routines **/

#define mini(a,b)   ((a < b) ?  a : b)

#define maxi(a,b)   ((a > b) ?   a : b) 

#define abs_val(a)  ((a < 0 ) ? -(a) : (a)) 

#define  get_char()     getchar()
	

/**Areas, Circumference  and Volumes */
	
	
#define  a_circle(r)   (3.14159265358979323846 * pow(r,2))

#define  c_circle(r)  	(2 * 3.14159265358979323846 * r )

#define  a_rectangle(l,b) ( l * b)

#define  c_rectangle(l ,b) (2*l +  2* b)
	
#define  a_square(a)  pow(a,2)

#define  c_square(a)  (4 * a)

#define v_cylinder(r,h) (3.14159265358979323846 * pow(r,2) * h )	
	
	
 
 
