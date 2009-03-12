#include "PKIN.h"

/****CompArgStr()***********jmw/utility***************************************/
int CompArgStr(char *str, char *arg, int min) 
{
   int i, max;
   char s, a;

   if (!str || !arg) return 0;

   max = strlen(arg);

   for(i=0; i<max; i++) 
   {
      s = toupper(str[i]);
      a = toupper(arg[i]);

      if (i >= min && (   s == '\0' || s == ',' || s == '(' || s == '.'
                       || s == '+'  || s == '-' || isdigit(s))) 
      {
          break; /* good ending point */
      }
      else if (s != a) 
      {
          i = 0; /* failed to match */
          break;
      }
   }
   return i;
}
/*___CompArgStr()____________________________________________________________*/


/*****floatfromstr()*********************************************************/
float    floatfromstr(char ins[256])
{
        int            Lstart,n,m,OK;
        char         s[256];
        float        freturn;

        freturn = 0.0;
        Lstart = 1;
        n = 0;
        m = 0;
        OK = 1;
        if(ins[0] != '\0')  /* not zero length pascal string */
        {
            while(OK)
            {
                /* start at position 0 of c str */
                if(ins[n] == ' ' && Lstart)  ;
                else if(ins[n] == '0' || ins[n] == '1' || ins[n] == '2' ||
                        ins[n] == '3' || ins[n] == '4' || ins[n] == '5' ||
                        ins[n] == '6' || ins[n] == '7' || ins[n] == '8' ||
                        ins[n] == '9' || ins[n] == '.' || ins[n] == '-'   ) 
                {
                    s[m] = ins[n];
                    m++;
                    Lstart = 0;
                }
                else
                {
                    s[m] = '\0'; 
                    OK = 0;
                }
                n++;
            }

/*c*/       sscanf(s,"%f",&freturn);
        }
        return(freturn);
}
/*___floatfromstr()_________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****intfromstr()************************************************************/
int   intfromstr(char  ins[256])
{
        int            Lstart,n,m,OK;
        char         s[256];
        int            ireturn;

        ireturn = 0;
        Lstart = 1;
        n = 0;
        m = 0;
        OK = 1;
        if(ins[0] != '\0')  /* not zero length pascal string */
        {
            while(OK)
            {
                /* start at position 0 of c str */
                if(ins[n] == ' ' && Lstart)  ;
                else if(ins[n] == '0' || ins[n] == '1' ||
                        ins[n] == '2' || ins[n] == '3' || 
                        ins[n] == '4' || ins[n] == '5' ||
                        ins[n] == '6' || ins[n] == '7' || 
                        ins[n] == '8' || ins[n] == '9' ||
                        ins[n] == '-'                      )
                {
                    s[m] = ins[n];
                    m++;
                    Lstart = 0;
                }
                else
                {
                    s[m] = '\0'; 
                    OK = 0;
                }
                n++; 
            }
/*c*/       sscanf(s,"%d",&ireturn);
        }
        return(ireturn);
}
/*___intfromstr()___________________________________________________________*/

/****matrixinvert()**********************************************************/
int matrixinvert(int N)
{
   /*MATINV.SUB pg 427, Programming in Basic for Engineers, Kamal B. Rojiani*/
   /*1988 PWS-KENT (Wadsworth) Boston  ISBN 0-534-91899-9                   */
   /*double A[ROWA][COLA]; */ /* matrix  ROWA == COLA */
   /*double C[ROWA][COLA]; */ /* inverse of A */
   /*double DET;*/ /*determinant*/ /*orig. not initialized before being used*/
   double DET = 1.0; /*determinant = product of PIVOTs */ 
   double TEMP,PIVOT,MULTIPLIER;
   int    NPIVOT = 0; /*number of times rows are pivited*/
   int    ROWA,COLA,ROWC;
   int i,j,PASS,MAXROW,ROW,COL;

   

   if(N>NMATRIX)
   {
      return(0);
   }
   ROWA = N;
   COLA = N;
   ROWC = N;

   /*store identity matrix in C, this will be replaced by the inverse*/
   for(i=1; i<= ROWC; i++)
   {
     for(j=1; j<= ROWC; j++)
     {
        if(i==j) C[i][j] = 1;
        else     C[i][j] = 0;
     }
   }
   /*computation*/
   for(PASS=1; PASS<= COLA; PASS++)
   {/*PASS*/
     /*partial pivoting, For each pass find the largest element in the column*/
     /*containing the pivot element.  The pivot column is PASS.  MAXROW is */
     /*row containing the largest element in the pivot column.  */
     /*It is initially set equal to the pivot row.*/
     
     MAXROW = PASS;
     for(ROW = PASS; ROW<= ROWA; ROW++)
     {
        if(abs((int)A[ROW][PASS]) > abs((int)A[MAXROW][PASS])) MAXROW = ROW;
     }
     if(MAXROW != PASS)
     {/*Interchange pivot row*/ 
       /*with row containing largest element in the */
       /*pivot column in both A and C */
       NPIVOT++;
       for(COL=1; COL<= COLA; COL++)
       {
          TEMP = A[PASS][COL];
          A[PASS][COL] = A[MAXROW][COL];
          A[MAXROW][COL] = TEMP;
          TEMP = C[PASS][COL];
          C[PASS][COL] = C[MAXROW][COL];
          C[MAXROW][COL] = TEMP;
       }
     }/*Interchange pivot row*/ 
     /*Compute determinant and check for singularity */
     /*Determinant is the product of the pivot elements*/
     /*Pivot element is A[PASS][PASS] */
     PIVOT = A[PASS][PASS];
     DET = DET*PIVOT;
     if(DET == 0.0)
     {/*matrix is singular*/
        printf("matrix singular\n");
        return(0);
     }/*matrix is singular*/
     /*Normalization. Divide pivot row by the pivot element*/
     /* so pivot element is reduced to 1 */
     for(COL=1; COL <= COLA; COL++)
     {
        A[PASS][COL] = A[PASS][COL]/PIVOT;
        C[PASS][COL] = C[PASS][COL]/PIVOT;
     }
     /*Elimination. Reduce all elements in pivot column to zero*/
     for(ROW=1; ROW<= ROWA; ROW++)
     {/*ROW*/
       if(ROW != PASS)  /*what happens if ROW==PASS???? */
       {
         MULTIPLIER = A[ROW][PASS];
         for(COL=1; COL<= COLA; COL++)
         {/*COL*/
           A[ROW][COL] = A[ROW][COL] - A[PASS][COL]*MULTIPLIER;
           C[ROW][COL] = C[ROW][COL] - C[PASS][COL]*MULTIPLIER;
               /*MULTIPLIER is how information from A gets into C */
         }/*COL*/
       }
     }/*ROW*/
   }/*PASS*/  
   if(NPIVOT%2) DET = -DET; /*if NPIVOT odd ... */
   return(1); /*1 for success*/   
}
/*___matrixinvert()_________________________________________________________*/

/****DoPerpendicularToPlane()************************************************/
void DoPerpendicularToPlane(float fcxyz[3][7])
{

double A1X,A1Y,A1Z,A2X,A2Y,A2Z;
double B1X,B1Y,B1Z,B2X,B2Y,B2Z;
double C1X,C1Y,C1Z;
double T,AP,BP,CP,DP;
    
    /*PI = 3.14159; already defined*/ 
    /*Matrix inverter can't handle point 0,0,0*/
    /*and is flaky if any coord == 0, sooooo add PI to all coord*/
    /*and hope that that doesn't inadventantly set some coord==0 */
    
    /*three points A1X,Y,Z   A2X,Y,Z   B1X,Y,Z define a plane*/
    /*fourth point  B2X,Y,Z is not on this plane, find perpendicular*/
    /*from this point to the plane at C1X,Y,Z */
    
    A1X = fcxyz[0][1]+PI;
    A1Y = fcxyz[1][1]+PI;
    A1Z = fcxyz[2][1]+PI;
    A2X = fcxyz[0][2]+PI;
    A2Y = fcxyz[1][2]+PI;
    A2Z = fcxyz[2][2]+PI;
    B1X = fcxyz[0][3]+PI;
    B1Y = fcxyz[1][3]+PI;
    B1Z = fcxyz[2][3]+PI;
    B2X = fcxyz[0][4]+PI;
    B2Y = fcxyz[1][4]+PI;
    B2Z = fcxyz[2][4]+PI;

    
    /*Equation of a plane from the earliest 3 points
      A1X*AP + A1Y*BP + A1Z*CP + DP = 0

      A2X*AP + A2Y*BP + A2Z*CP + DP = 0
      B1X*AP + B1Y*BP + B1Z*CP + DP = 0
      rearrange into normal equation form:
      A1X*AP + A1Y*BP + A1Z*CP = -DP
      A2X*AP + A2Y*BP + A2Z*CP = -DP
      B1X*AP + B1Y*BP + B1Z*CP = -DP
            [AP]   [-DP]             [-DP] = [AP]
      [   ]*[BP] = [-DP]   so  [ i ]*[-DP] = [BP]  
            [CP]   [-DP]             [-DP] = [CP]
      load 3x3 matrix  A
    */
    A[1][1] = A1X; A[1][2] = A1Y; A[1][3] = A1Z;
    A[2][1] = A2X; A[2][2] = A2Y; A[2][3] = A2Z;
    A[3][1] = B1X; A[3][2] = B1Y; A[3][3] = B1Z;
/*
printf("A %.3f,%.3f,%.3f\n",A[1][1],A[1][2],A[1][3]);
printf("A %.3f,%.3f,%.3f\n",A[2][1],A[2][2],A[2][3]);
printf("A %.3f,%.3f,%.3f\n",A[3][1],A[3][2],A[3][3]);
*/
    matrixinvert(3); /* global C inverse of global A */
/*
printf("C %.3f,%.3f,%.3f\n",C[1][1],C[1][2],C[1][3]);
printf("C %.3f,%.3f,%.3f\n",C[2][1],C[2][2],C[2][3]);
printf("C %.3f,%.3f,%.3f\n",C[3][1],C[3][2],C[3][3]);
*/
    /*
      now C is inverse of A
      AP = C[1][1]*(-DP) + C[1][2]*(-DP) + C[1][3]*(-DP)
      BP = C[2][1]*(-DP) + C[2][2]*(-DP) + C[2][3]*(-DP)
      CP = C[3][1]*(-DP) + C[3][2]*(-DP) + C[3][3]*(-DP)
      and equation of plane is
      X*AP + Y*BP + Z*CP + DP = 0
        X*(-DP)*(C[1][1] + C[1][2] + C[1][3])
       +Y*(-DP)*(C[2][1] + C[2][2] + C[2][3])
       +Z*(-DP)*(C[3][1] + C[3][2] + C[3][3])
       = -DP
      multiply through by -DP
        X*(C[1][1] + C[1][2] + C[1][3])
       +Y*(C[2][1] + C[2][2] + C[2][3])
       +Z*(C[3][1] + C[3][2] + C[3][3])
       = 1
      direction components of normal to this plane:
    */
    AP =  (C[1][1] + C[1][2] + C[1][3]);
    BP =  (C[2][1] + C[2][2] + C[2][3]);
    CP =  (C[3][1] + C[3][2] + C[3][3]);
    /*for a given case
      A1X*AP + A1Y*BP + A1Z*CP = -DP
    */
    DP =  -A1X*AP - A1Y*BP - A1Z*CP;
    /*
      find line through point B2X,B2Y,B2Z that has these direction components
      and then find point on the plane where this line intersects the plane.
      C1X = B2X + AP*T
      C1Y = B2Y + BP*T
      C1Z = B2Z + CP*T
      C1X*AP + C1Y*BP + C1Z*CP + D = 0;
      
      AP*(B2X + AP*T) + BP*(B2Y + BP*T) + CP*(B2Z + CP*T) + DP = 0;
      AP*B2X + AP*AP*T + BP*B2Y + BP*BP*T + CP*B2Z + CP*CP*T + DP = 0;
      T*(AP*AP + BP*BP + CP*CP) = -D -AP*B2X -BP*B2Y -CP*B2Z
      T = (-DP -AP*B2X -BP*B2Y -CP*B2Z)/(AP*AP + BP*BP + CP*CP)
      substituting results from above
    */
    T =   (-DP -B2X*AP-B2Y*BP-B2Z*CP)/( AP*AP+BP*BP+CP*CP );
    C1X = B2X + AP*T;
    C1Y = B2Y + BP*T;
    C1Z = B2Z + CP*T;

    fcxyz[0][5] = (float)(C1X-PI);
    fcxyz[1][5] = (float)(C1Y-PI);
    fcxyz[2][5] = (float)(C1Z-PI);
/*
printf(" %.3f,%.3f,%.3f   ,%.3f,%.3f,%.3f   ,%.3f,%.3f,%.3f\n"
,A1X,A1Y,A1Z,A2X,A2Y,A2Z,B1X,B1Y,B1Z);
printf(" %.3f,%.3f,%.3f   ,%.3f,%.3f,%.3f\n"
,B2X,B2Y,B2Z,C1X,C1Y,C1Z);
*/

}
/*___DoPerpendicularToPlane()_______________________________________________*/

/****DoPerpendicularToLine()**************************************************/
void DoPerpendicularToLine(float A1[3], float A2[3], float B1[3], float B2[3])
{/*050125,050324 revived DoPerpendicularToLine()*/

   /*construct perpendicular from point to line*/
   /*perpendicular from 3rd point to line of points 1 and 2 */
   /* fourth point is on line at perpendicular to third point*/
   /*050125 returns that constructed fourth point in index 5 of input array*/

   float  A1X,A1Y,A1Z,A2X,A2Y,A2Z,B1X,B1Y,B1Z,ADX,ADY,ADZ,B2X,B2Y,B2Z,ART;

    A1X = A1[0];
    A1Y = A1[1];
    A1Z = A1[2];
    A2X = A2[0];
    A2Y = A2[1];
    A2Z = A2[2];
    B1X = B1[0];
    B1Y = B1[1];
    B1Z = B1[2];
    ADX = A2X - A1X;
    ADY = A2Y - A1Y;
    ADZ = A2Z - A1Z;
/*arbitrary point on line A
   B2X = A1X + ADX*ART;B2Y = A1Y + ADY*ART;B2Z = A1Z + ADZ*ART;

   direction components of line B1 --- B2
   BDX = A1X-B1X+ADX*ART; BDY = A1Y-B1Y+ADY*ART; BDZ = A1Z-B1Z+ADZ*ART;
   for this to be perpendicular to line A, sum of product of components ==0
   ADX*(A1X-B1X+ADX*ART)+ADY*(A1Y-B1Y+ADY*ART)+ADZ*(A1Z-B1Z+ADZ*ART)=0;
   rearrange
   ADX*A1X-ADX*B1X+ADX*ADX*ART
   +ADY*A1Y-ADY*B1Y+ADY*ADY*ART
   +ADZ*A1Z-ADZ*B1Z+ADZ*ADZ*ART=0;
   regroup
   ART*(ADX*ADX++ADY*ADY+ADZ*ADZ)
         =ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z;
*/
         ART =  (ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z)
               /(ADX*ADX+ADY*ADY+ADZ*ADZ);

         B2X = A1X + ADX*ART;
         B2Y = A1Y + ADY*ART;
         B2Z = A1Z + ADZ*ART;

    B2[0] = B2X;
    B2[1] = B2Y;
    B2[2] = B2Z;

   return;
}
/*___DoPerpendicularToLine()_________________________________________________*/


