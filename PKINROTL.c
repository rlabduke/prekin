/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*                     PKINCOUT.c           */
#include "PKIN.h"

/*#define ROTINFO*/

/*prototypes not defined in common*/
float findrotationangle(char tryatom[5][6]);
void  findrotationaxis(int,int, char tryatom[5][6]);
void  findrotationriders(int,int, char tryatom[5][6]);
int   findatomnumber(char tryatom[6]);
int   proteinmainchainname(char, char, char, char); /*040504*/

/****getresiduecode()********************************************************/
char   getresiduecode(char u, char v, char w) /*returns 1-letter code*/
{
   char creturn; /*local*/
   
   if     (u=='a' && v=='l' && w=='a') creturn='a';
   else if(u=='c' && v=='y' && w=='s') creturn='c';
   else if(u=='a' && v=='s' && w=='p') creturn='d';
   else if(u=='g' && v=='l' && w=='u') creturn='e';
   else if(u=='p' && v=='h' && w=='e') creturn='f';
   else if(u=='g' && v=='l' && w=='y') creturn='g';
   else if(u=='h' && v=='i' && w=='s') creturn='h';
   else if(u=='i' && v=='l' && w=='e') creturn='i';
   else if(u=='l' && v=='y' && w=='s') creturn='k';
   else if(u=='l' && v=='e' && w=='u') creturn='l';
   else if(u=='m' && v=='e' && w=='t') creturn='m';
   else if(u=='m' && v=='s' && w=='e') creturn='M'; /*030605*/
   else if(u=='a' && v=='s' && w=='n') creturn='n';
   else if(u=='p' && v=='r' && w=='o') creturn='p';
   else if(u=='g' && v=='l' && w=='n') creturn='q';
   else if(u=='a' && v=='r' && w=='g') creturn='r';
   else if(u=='s' && v=='e' && w=='r') creturn='s';
   else if(u=='t' && v=='h' && w=='r') creturn='t';
   else if(u=='v' && v=='a' && w=='l') creturn='v';
   else if(u=='t' && v=='r' && w=='p') creturn='w';
   else if(u=='t' && v=='y' && w=='r') creturn='y';
   else if(u=='a' && v=='i' && w=='b') creturn='b';
   else creturn='x'; 
   
   return(creturn);
}
/*___getresiduecode()_______________________________________________________*/

/****getresiduerotations()****************************************************/
void   getresiduerotations()
{
   /*findrotationaxis() and findrotationriders() actually write to scratch*/

   int    i,nth,level;
   char   tryatom[5][6]; /*tryatom[][3,4] alternate possibilities for 4th char*/
                         /* index: 0,1,2,3   1st,2nd,3rd,4th char of atom name*/
   /*int    n1;*/
   int    n2,n3,n4;
   
   double dihedral4pt(double,double,double,double,double,double
                              ,double,double,double,double,double,double);
       /*atom naming conventions are just wierd enough that it seemed*/
       /*easier to set up a lot of special cases for the rotations.*/
       /*base cases on what this residue is supposed to be*/
       /*get into one letter code*/

   i = rotoutl; /*rotoutl here is range number that defines rotated residue*/
   
   rescode = getresiduecode(resid[i][0], resid[i][1], resid[i][2]);
 
   /*reuse rotoutl now to flag number of rotatable bonds */
   /*including hydrogens, and including branch rotations*/

   rotoutl2 = 0;/*branch occurs at nth rotation (always the last for nat. aa)*/
      /*global*/
   branch = ' '; /*branch is at the nth (as a character) level of rotation*/
      /*global*/
      
   nth = 0; /*nth rotation being processed*/
   level = 0; /*rotation level of axis under consideration*/
   
   
   if(rescode=='g'||rescode=='x') /*proline needs rotations to show its rotatmers*/
   {
      rotoutl = 0; nth = 0; level = 0;
   }
   else if(rescode=='a')   /*alanine */
   {
         rotoutl = 0; nth = 0; level = 0;
     if( hyoutl)
     {   rotoutl = 1; nth = 1; level = 1;
      
         /*                  01234 but sidechain already screened for alt conf*/
         sprintf(tryatom[1]," n   "); /*so fourth tryatom char used as extra */
         sprintf(tryatom[2]," ca  "); /*test for atom[j][3]*/
         sprintf(tryatom[3]," cb  "); /*historically, this allowed several */
         sprintf(tryatom[4]," hb 1"); /*residues to be logically combined*/

         /* h at index 1 of tryatom represents a hydrogen, isahydrogenname(atom) */
         /* then can recognized if atoms match as elements    071222*/

         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HB_,2HB_,3HB_*/
           /*newpdb: , HB1, HB2, HB3,*/
     }
   }
   else if(rescode=='b')/*aib amino iso butaric acid: alanine with extra methyl */
   {
         rotoutl = 0; nth = 0; level = 0;
     if( hyoutl)
     {   rotoutl = 2; rotoutl2 = 2; branch = '1'; /*globals*/
         nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb 1");
         sprintf(tryatom[4]," hb 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HB1,2HB1,3HB1*/
         /*second axis:*/
         nth = 2; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb22");
         sprintf(tryatom[4]," hb22");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HB2,2HB2,3HB2*/
     }
   }
   else if(rescode=='c') /*      cystine        */
   {   rotoutl = 1; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," sg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl)
         {/*find S */
            findrotationriders(nth,level, tryatom); /*_SG_*/
         }
      if( hyoutl)
      {  rotoutl = 2; 
         /*has hydrogen riders on first axis*/
         sprintf(tryatom[4]," hb 1");
         findrotationriders(nth,level, tryatom); /*1HB_,2HB_*/
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," sg  ");
         sprintf(tryatom[4]," hg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         /*final hydrogen is the only rider on second axis*/
         findrotationriders(nth,level, tryatom); /*_HG_*/

      }
   }
   else if(rescode=='s') /*      serine         */
   {   rotoutl = 1; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," og 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl)
         {/*find O */
            findrotationriders(nth,level, tryatom); /*_OG_*/
         }
      if( hyoutl)
      {  rotoutl = 2;
         /*has hydrogen riders on first axis*/
         sprintf(tryatom[4]," hb 1");
         findrotationriders(nth,level, tryatom); /*1HB_,2HB_*/
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," og  ");
         sprintf(tryatom[4]," hg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         /*final hydrogen is the only rider on second axis*/
         findrotationriders(nth,level, tryatom); /*_HG_*/
      }
   }
   else if(rescode=='t') /*      threonine      */
   {   rotoutl = 1; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," og 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( !hyoutl)
         {/*need to get _OG1 and _CG2 */
            findrotationriders(nth,level, tryatom); /*_OG1*/
            sprintf(tryatom[4]," cg22");
            findrotationriders(nth,level, tryatom); /*_CG2*/
         }
      if( hyoutl)
      {  rotoutl = 3; rotoutl2 = 3; branch = '2'; /*globals*/
         /*has hydrogen riders on first axis*/
         sprintf(tryatom[4]," hb 1");
         findrotationriders(nth,level, tryatom); /*1HB_,2HB_*/
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," og 1");
         sprintf(tryatom[4]," hg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         /*final hydrogen is the only rider on second axis*/
         findrotationriders(nth,level, tryatom); /*_HG_*/
         /*third, branch rotation:*/
         nth = 3; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg22");
         sprintf(tryatom[4],"1hg22");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         sprintf(tryatom[4]," hg22");
         findrotationriders(nth,level, tryatom); /*1HG2,2HG2,3HG2*/
      }
   }
   else if(rescode=='v') /*      valine         */
   {   rotoutl = 1; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl)
         {
            sprintf(tryatom[4]," cg12");/*look for both 1&2 in char [3]*/
            findrotationriders(nth,level, tryatom); /*_CG1,_CG2*/
         }
      if( hyoutl)
      {  rotoutl = 3; rotoutl2 = 3; branch = '2'; /*globals*/
         /*has hydrogen riders on first axis*/
         sprintf(tryatom[4]," hb 1");
         findrotationriders(nth,level, tryatom); /*1HB_*/
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg 1");
         sprintf(tryatom[4]," hg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HG1,2HG1,3HG1*/
         /*third, branch rotation:*/
         nth = 3; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg22");
         sprintf(tryatom[4],"1hg22");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HG2,2HG2,3HG2*/
      }
   }
   else if(rescode=='d'||rescode=='n') /* aspartate, asparagine */
   {   rotoutl = 2; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");/*4th char:alternate test for atom[j][3]*/
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_*/
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg  ");
         sprintf(tryatom[4]," od 1");/*both asp,asn have od */
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*_OD1*/
         if(rescode=='d')
         {/*aspartic acid, find the other oxygen*/
            sprintf(tryatom[4]," od22");
            findrotationriders(nth,level, tryatom); /*_OD2*/
         }
         if(rescode=='n')
         {/*asparagine, find the nitrogen*/
            sprintf(tryatom[4]," nd22");
            findrotationriders(nth,level, tryatom); /*_ND2*/
            if(hyoutl)
            {/*find hydrogens on the nitrogen*/
               sprintf(tryatom[4]," hd22");
               findrotationriders(nth,level, tryatom); /*1HD2,2HD2*/
            }
         }
   } 
   else if(rescode=='f'||rescode=='h'||rescode=='w'||rescode=='y')/*aromatics*/
   {  rotoutl = 2; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");/*4th char:alternate test for atom[j][3]*/
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,2HB_*/
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg  ");
         if(rescode=='h')
            sprintf(tryatom[4]," nd 1");
         else
            sprintf(tryatom[4]," cd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         /*lots of stuff rides on this second axis*/
         sprintf(tryatom[4],"  d  ");  /*permissive request: no element ID*/
         findrotationriders(nth,level, tryatom); /*xxDx*/
         sprintf(tryatom[4],"  e  ");
         findrotationriders(nth,level, tryatom); /*xxEx*/
         sprintf(tryatom[4],"  z  ");
         findrotationriders(nth,level, tryatom); /*xxZx*/
      if( hyoutl && rescode=='y')
      {  rotoutl = 3; nth = 3; level = 3;
         sprintf(tryatom[1]," ce 1");
         sprintf(tryatom[2]," cz  ");
         sprintf(tryatom[3]," oh  ");
         sprintf(tryatom[4]," hh 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         /*the _HH_ is the only atom riding on 3rd tyr axis*/
         findrotationriders(nth,level, tryatom); /*_HH_*/
      }
      else
      {
         sprintf(tryatom[4],"  h  ");
         findrotationriders(nth,level, tryatom); /*xxHx*/      
      }
   }
   else if(rescode=='l'||rescode=='i') /*  leucine, isoleucine  */
   {  rotoutl = 2; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl && rescode=='i')
         {/*another carbon rides on first axis*/
            sprintf(tryatom[4]," cg22");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg 1");
         sprintf(tryatom[4]," cd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl)
         {
            findrotationriders(nth,level, tryatom); /*_CD1*/   
            if(rescode=='l')
            {
               /*another carbon rides on second axis*/
               sprintf(tryatom[4]," cd22"); 
               findrotationriders(nth,level, tryatom); /*_CD2*/   
            }
         }

         if( hyoutl)
         {/*hydrogens ride on second axis*/
            sprintf(tryatom[4]," hg 1");
            findrotationriders(nth,level, tryatom); /*1HG_,(2HG_)*/   
         }
      if( hyoutl)
      {  rotoutl = 4; 
         /*third axis:*/
         nth = 3; level = 3;
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg 1");
         sprintf(tryatom[3]," cd 1");
         sprintf(tryatom[4]," hd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on third axis*/
            sprintf(tryatom[4]," hd 1");
            findrotationriders(nth,level, tryatom); /*1HD1,2HD1,3HD1*/   
         }
        /*fourth axis:*/
         nth = 4; level = 4;
        if(rescode=='l')/*                      leucine  */
        {             rotoutl2 = 4; branch = '3'; /*globals*/
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg  ");
         sprintf(tryatom[3]," cd22");
         sprintf(tryatom[4],"1hd22");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HD2,2HD2,3HD2*/
        }
        if(rescode=='i')/*                  isoleucine  */
        {             rotoutl2 = 4; branch = '2'; /*globals*/
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg22");
         sprintf(tryatom[4],"1hg22");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*1HG2,2HG2,3HG2*/
        }
      }
   }
   else if(rescode=='e'||rescode=='q') /* glutamate, glutamine */
   {   rotoutl = 3; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");/*4th char:alternate test for atom[j][3]*/
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg  ");
         sprintf(tryatom[4]," cd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on second axis*/
            sprintf(tryatom[4]," hg 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*third axis:*/
         nth = 3; level = 3;
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg  ");
         sprintf(tryatom[3]," cd 1");
         sprintf(tryatom[4]," oe 1");/*both glu,gln have oe */
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*_OE1*/
         if(rescode=='e')
         {/*glutamic acid, find the other oxygen*/
            sprintf(tryatom[4]," oe22");
            findrotationriders(nth,level, tryatom); /*_OE2*/
         }
         if(rescode=='q')
         {/*glutamine, find the nitrogen*/
            sprintf(tryatom[4]," ne22");
            findrotationriders(nth,level, tryatom); /*_NE2*/
            if(hyoutl)
            {/*find hydrogens on the nitrogen*/
               sprintf(tryatom[4]," he22");
               findrotationriders(nth,level, tryatom); /*1HD2,2HD2*/
            }
         }

   }
   else if(rescode=='m'||rescode=='M')     /* methionine,selenoMet */
   {   rotoutl = 3; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg 1");
         if(rescode=='m'){sprintf(tryatom[4]," sd 1");}
         else {sprintf(tryatom[4],"sed 1");}
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on second axis*/
            sprintf(tryatom[4]," hg 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*third axis:*/
         nth = 3; level = 3;
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg 1");
         if(rescode=='m'){sprintf(tryatom[3]," sd 1");}
         else {sprintf(tryatom[3],"sed 1");}
         sprintf(tryatom[4]," ce 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl)
         {
            findrotationriders(nth,level, tryatom); /*_CE_*/   
         }
      if( hyoutl )
      {  rotoutl = 4; 
         /*fourth axis:*/
         nth = 4; level = 4;
         sprintf(tryatom[1]," cg 1");
         if(rescode=='m'){sprintf(tryatom[2]," sd 1");}
         else {sprintf(tryatom[2],"sed 1");}
         sprintf(tryatom[3]," ce 1");
         sprintf(tryatom[4],"1he 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         {/*hydrogens ride on fourth axis*/
            sprintf(tryatom[4]," he 1");
            findrotationriders(nth,level, tryatom); /*1HE_,2HE_,3HE_*/   
         }
      }
   }
   else if(rescode=='r')             /* arginine*/
   {   rotoutl = 4; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg 1");
         sprintf(tryatom[4]," cd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on second axis*/
            sprintf(tryatom[4]," hg 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*third axis:*/
         nth = 3; level = 3;
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg 1");
         sprintf(tryatom[3]," cd 1");
         sprintf(tryatom[4]," ne 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on third axis*/
            sprintf(tryatom[4]," hd 1");
            findrotationriders(nth,level, tryatom); /*1HD_,2HD_*/   
         }
         /*fourth axis:*/
         nth = 4; level = 4;
         sprintf(tryatom[1]," cg 1");
         sprintf(tryatom[2]," cd 1");
         sprintf(tryatom[3]," ne 1");
         sprintf(tryatom[4]," cz 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         /*guanidinium group rides on fourth axis*/
         sprintf(tryatom[4]," cz 1"); 
         findrotationriders(nth,level, tryatom); /*_CZ_*/
         sprintf(tryatom[4]," nh 1"); 
         findrotationriders(nth,level, tryatom); /*_NH1*/
         sprintf(tryatom[4]," nh22"); 
         findrotationriders(nth,level, tryatom); /*_NH2*/
         
         if( hyoutl)
         {/*hydrogens ride on fourth axis*/
            sprintf(tryatom[4]," he 1");
            findrotationriders(nth,level, tryatom); /*1HE_*/ 
            sprintf(tryatom[4]," hh 1");
            findrotationriders(nth,level, tryatom); /*1HH1,2HH1*/  
            sprintf(tryatom[4]," hh 2");
            findrotationriders(nth,level, tryatom); /*1HH1,2HH1*/  
         }
   }
   else if(rescode=='k')             /*  lysine */
   {  rotoutl = 4; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg 1");
         sprintf(tryatom[4]," cd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on second axis*/
            sprintf(tryatom[4]," hg 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*third axis:*/
         nth = 3; level = 3;
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg 1");
         sprintf(tryatom[3]," cd 1");
         sprintf(tryatom[4]," ce 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on third axis*/
            sprintf(tryatom[4]," hd 1");
            findrotationriders(nth,level, tryatom); /*1HD_,2HD_*/   
         }
         /*fourth axis:*/
         nth = 4; level = 4;
         sprintf(tryatom[1]," cg 1");
         sprintf(tryatom[2]," cd 1");
         sprintf(tryatom[3]," ce 1");
         sprintf(tryatom[4]," nz 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if(!hyoutl) 
            findrotationriders(nth,level, tryatom); /*_NZ_*/
         if( hyoutl)
         {/*hydrogens ride on fourth axis*/
            sprintf(tryatom[4]," he 1");
            findrotationriders(nth,level, tryatom); /*1HE_,2HE_*/   
         }

      if( hyoutl )
      {  rotoutl = 5; 
         /*fifth axis:*/
         nth = 5; level = 5;
         sprintf(tryatom[1]," cd 1");
         sprintf(tryatom[2]," ce 1");
         sprintf(tryatom[3]," nz 1");
         sprintf(tryatom[4],"1hz 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         sprintf(tryatom[4]," hz 1");
         findrotationriders(nth,level, tryatom); /*1HH_,2HH_,3HH_*/
      }
   }/*lys*/
   else if(rescode=='p')             /*  proline */
   {  rotoutl = 3; nth = 1; level = 1;
         sprintf(tryatom[1]," n   ");
         sprintf(tryatom[2]," ca  ");
         sprintf(tryatom[3]," cb  ");
         sprintf(tryatom[4]," cg 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on first axis*/
            sprintf(tryatom[4]," hb 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*second axis:*/
         nth = 2; level = 2;
         sprintf(tryatom[1]," ca  ");
         sprintf(tryatom[2]," cb  ");
         sprintf(tryatom[3]," cg 1");
         sprintf(tryatom[4]," cd 1");
         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         if( hyoutl)
         {/*hydrogens ride on second axis*/
            sprintf(tryatom[4]," hg 1");
            findrotationriders(nth,level, tryatom); /*1HB_,(2HB_)*/   
         }
         /*third axis:*/
         nth = 3; level = 3;
         sprintf(tryatom[1]," cb  ");
         sprintf(tryatom[2]," cg 1");
         sprintf(tryatom[3]," cd 1");
         sprintf(tryatom[4]," ne 1"); /*special proline dummy n atom*/
         /*atoms are connected by distance, so to override the usual*/
         /*base atom for CD which is N, the first atom close enough to*/
         /*be connected, one must specify the base atom for CD and NE */
         /*as special cases*/
         /*n1 = findatomnumber(tryatom[1]);*/
         n2 = findatomnumber(tryatom[2]);
         n3 = findatomnumber(tryatom[3]);
         n4 = findatomnumber(tryatom[4]);
         base[n3] = n2;
         base[n4] = n3;

         bondangle[nth] = findrotationangle(tryatom);
         findrotationaxis(nth,level, tryatom);
         findrotationriders(nth,level, tryatom); /*_NE_*/ 
         if( hyoutl)
         {/*hydrogens ride on third axis*/
            sprintf(tryatom[4]," hd 1");
            findrotationriders(nth,level, tryatom); /*1HD_,2HD_*/   
         }
   }/*pro*/

   if(Ltestprint > 0)
   {
      sprintf(alertstr,CRLF"residue identified as: %c, with %d rotatable bonds"
           CRLF,rescode,rotoutl);
           pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
           adjusttext(2);          /*PKMCTEXT.C PKPCTEXT.C*/
   }
}
/*___getresiduerotations()___________________________________________________*/

/****findrotationangle()******************************************************/
float findrotationangle(char tryatom[5][6])
{
   int    i,icountatoms,j,LOK=1;
  /* char   tryatom[5][6];*/
   
   double fxyz[3][5],dihedral4pt(double,double,double,double,double,double
                              ,double,double,double,double,double,double);
   char   atomname[5][6];
   float  bondangle;
   

       icountatoms = 0;
       for(i=1;i<=4;i++) sprintf(atomname[i],"    ");
       /*find 4 linked atoms to define a dihedral rotation angle*/
       for(i=1; i<=2; i++) 
       {/*atoms 1&&2: check char 1&&2 vs. (tryatom[1,2][1]&&tryatom[1,2][2])*/
         for(j=1; j<=maxatm; j++)
         {
           /*if( atom[j][1] != 'h' && atom[j][1] != 'd' && atom[j][1] != 'q')*/
           if( !isahydrogenname(atom[j]))
           {/*atoms 1,2 cannot be hydrogens*/ 
            if(  (   atom[j][1]==tryatom[i][1]
                   &&atom[j][2]==tryatom[i][2]  ) )
            {
               fxyz[0][i] = x[j];fxyz[1][i] = y[j];fxyz[2][i] = z[j];
               icountatoms = icountatoms + 1;
               sprintf(atomname[icountatoms],"%s",atom[j]);
               break;
            }
           }
         }
       }
       for(i=3; i<=4; i++) 
       {/*atoms 3&&4: check char2 vs. (tryatom[3,4][2])*/
        /* also check char3 vs. (tryatom[3,4][3]||tryatom[3,4][4])*/
         for(j=1; j<=maxatm; j++)
         {
           /*if(  (i==3 &&atom[j][1] !='h'&&atom[j][1] !='d'&&atom[j][1] !='q'))*/
           if( i==3 && (!isahydrogenname(atom[j]) ) )
           {/*atom  3 cannot be hydrogen*/ 
              if(  (atom[j][2]==tryatom[i][2]) 
                 &&(atom[j][3]==tryatom[i][3] || atom[j][3]==tryatom[i][4]) )
              {
                 fxyz[0][i] = x[j];fxyz[1][i] = y[j];fxyz[2][i] = z[j];
                 icountatoms = icountatoms + 1;
                 sprintf(atomname[icountatoms],"%s",atom[j]);
                 break;
              }
           }
           else if(i==4)
           {
              LOK = 0;
              if(tryatom[i][1]=='h')
              {/*hydrogen as fourth atom*/
                 /*if(atom[j][1]=='h'||atom[j][1]=='d'||atom[j][1]=='q') LOK=1;*/
                 if(isahydrogenname(atom[j])) LOK=1;
              }
              else
              {/*not looking for a hydrogen*/
                 if(atom[j][1]==tryatom[i][1]) LOK = 1;
              }
              if(  LOK 
                 &&(atom[j][2]==tryatom[i][2]) 
                 &&(atom[j][3]==tryatom[i][3] || atom[j][3]==tryatom[i][4]) )
              {
                 fxyz[0][i] = x[j];fxyz[1][i] = y[j];fxyz[2][i] = z[j];
                 icountatoms = icountatoms + 1;
                 sprintf(atomname[icountatoms],"%s",atom[j]);
                 break;
              }
           }
         }
       }
       if(icountatoms==4) 
       {
           bondangle = (float)(dihedral4pt(    
                                   fxyz[0][1],fxyz[1][1],fxyz[2][1],
                                   fxyz[0][2],fxyz[1][2],fxyz[2][2],
                                   fxyz[0][3],fxyz[1][3],fxyz[2][3],
                                   fxyz[0][4],fxyz[1][4],fxyz[2][4] ));
       }
       else bondangle = 0.0;
       if(Ltestprint > 0)
       {
           sprintf(alertstr,CRLF"bondrot dihedral is %.1f from coords:"CRLF
               "%.3f, %.3f, %.3f"CRLF
               "%.3f, %.3f, %.3f"CRLF
               "%.3f, %.3f, %.3f"CRLF
               "%.3f, %.3f, %.3f"CRLF
               ,bondangle
               ,fxyz[0][1],fxyz[1][1],fxyz[2][1]
               ,fxyz[0][2],fxyz[1][2],fxyz[2][2]
               ,fxyz[0][3],fxyz[1][3],fxyz[2][3]
               ,fxyz[0][4],fxyz[1][4],fxyz[2][4]
               );
           pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
           adjusttext(6);          /*PKMCTEXT.C PKPCTEXT.C*/
          
       }
#ifdef ROTINFO
       {
         sprintf(alertstr,"%d atoms: %s, %s, %s, %s; angle==%.2f"CRLF
         ,icountatoms
         ,atomname[1],atomname[2],atomname[3],atomname[4]
         ,bondangle);
         pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
         adjusttext(2);          /*PKMCTEXT.C PKPCTEXT.C*/
       }
#endif

   return(bondangle);
}
/*___findrotationangle()_____________________________________________________*/

/****findrotationaxis()*******************************************************/
void findrotationaxis(int nthrot, int level, char tryatom[5][6])
{
static  char  cntl[] = "    "; /* 4 blank characters */
   char rds[16];
   int j,k;
    char status[16]; /*030605*/

               /*||(tryatom[3][1]=='h'&&(atom[j][1]=='d'||atom[j][1]=='q')) )*/
level = level; /*avoids unused error message*/
   for(j=1 ; j<=maxatm ; j++)
   {/*scan over all atoms to find rotation axis*/
      if(isc[j] > 0 ) /*only look at recognized sc, inc. alt conf. info.*/
      {/*recognized sidechain atom*/ /*3rd atom of diheral & its base is axis*/ 
#ifdef OLDCODE
         /*check explicitly for hydrogen*/
         if(  (atom[j][0] == tryatom[3][0])  
            &&(  (atom[j][1] == tryatom[3][1])
               ||(isahydrogenname(tryatom[3])&&(isahydrogenname(atom[j]))) )
            &&(atom[j][2] == tryatom[3][2])
            &&(atom[j][3] == tryatom[3][3]||atom[j][3] == tryatom[3][4])   
           ) 
#endif
         if(  !isahydrogenname(atom[j])   /*hydrogen cannot be head of an axis*/
            && atom[j][1] == tryatom[3][1]  /*element match */
            && atom[j][2] == tryatom[3][2]  /*1st position match*/
            &&(atom[j][3] == tryatom[3][3] || atom[j][3] == tryatom[3][4])  
                   /*2nd position match*/
         )
         {/*e.g. _CB_ will have _CA_ as its base*/

            k = base[j];
   
            if(Latomradii)
            {
               if(atom[j][1]=='h') 
               {
                  if(atom[k][1]=='o'||atom[k][1]=='n')
                  {
                      sprintf(rds," r=%.2f ",polarHradius);
                  }
                  else
                  {
                      sprintf(rds," r=1.17 ");
                  }
               }
               else if(atom[j][1]=='o') sprintf(rds," r=1.4 ");
               else if(atom[j][1]=='n') sprintf(rds," r=1.55 ");
               else if(atom[j][1]=='c') sprintf(rds," r=1.75 ");
               else if(atom[j][1]=='s') sprintf(rds," r=1.8 ");
               else if(atom[j][1]=='p') sprintf(rds," r=1.8 ");
               else rds[0] = '\0';
            }
            else  
            {
               rds[0] = '\0';
            }

  if(  res[j][0]=='p' &&res[j][1]=='r' &&res[j][2]=='o'
     &&atom[j][1]=='n' &&atom[j][2]=='e')
  {
     sprintf(status," ghost"); /*Mage not to send atom ne to probe 030506*/
  }
  else if(atstyle[j] == 1)
  {
     sprintf(status," ghost"); /*Mage not to send extra atoms to probe 030920*/
  }
  else
  {
     status[0] = '\0'; /*no status flag*/
  }
            sprintf(cntl,"ro%1d ",nthrot); /*|ro#_|*/
            /*make pointID fit PDB format spacing, 2char sub uses space 071222*/ 
            /*format(a6,i5,1x,a5,a3,1x,a1,i4,a1,3x,3f8.3,2f6.2)*/
            sprintf(temps,"%s{%5s%3s%2s%4d%1s} P %.3f, %.3f, %.3f "
                            "{%5s%3s%2s%4d%1s} L%s%s %.3f, %.3f, %.3f"EOLO
             ,cntl,atom[k],res[k],sub[k],num[k],rins[k],x[k],y[k],z[k]
                  ,atom[j],res[j],sub[j],num[j],rins[j],rds,status
                  ,x[j],y[j],z[j]);
            putonetextblockline(&mainscratch,temps); /*PDB ptID*/
            ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*P point*/
            ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
            Listrot = rotoutl;
            Listrot2 = rotoutl2;
            if(hyoutl) Listhy = 1;
#ifdef ROTINFO
sprintf(alertstr,"rot_%1d(%d-->%d):%s{%5s%3s%2s%4d%1s} P %.3f, %.3f, %.3f "
                        "{%5s%3s%2s%4d%1s} L%s %.3f, %.3f, %.3f"CRLF
      ,nthrot,k,j,cntl,atom[k],res[k],sub[k],num[k],rins[k],x[k],y[k],z[k]
                  ,atom[j],res[j],sub[j],num[j],rins[j],rds,x[j],y[j],z[j]);
pkintextinsert(alertstr);  /*____TEXT.C*/
adjusttext(1);             /*____TEXT.C*/
#endif
            break; /*just find the first instance of this atom*/
          }/*e.g. _CB_ will have _CA_ as its base*/
      }/*recognized sidechain atom*/                               
   }/*scan over all atoms to find rotation axis*/
}
/*___findrotationaxis()______________________________________________________*/

/****findatomnumber()*******************************************************/
int findatomnumber(char tryatom[6])
{
                /*||(tryatom[1]=='h'&&(atom[j][1]=='d'||atom[j][1]=='q')))*/
   int j=0,ireturn=-1;
   for(j=1 ; j<=maxatm ; j++)
   {/*scan over all atoms in residue to find atom number for this named atom*/
          if(  (atom[j][0] == tryatom[0])  /*so can get consistent hydrogen*/
             &&(  (atom[j][1] == tryatom[1])
                ||(isahydrogenname(tryatom)&&(isahydrogenname(atom[j]))) )
             &&(atom[j][2] == tryatom[2])
             &&(atom[j][3] == tryatom[3]||atom[j][3] == tryatom[4])   
            ) 
          {
            ireturn = j;
            break; /*just find the first instance of this atom*/
          }
   }/*scan over all atoms to find rotation axis*/
   return(ireturn);
}
/*___findatomnumber()______________________________________________________*/

/****findrotationriders()*****************************************************/
void findrotationriders(int nthrot, int level, char tryatom[5][6])
{
static  char  cntl[] = "    "; /* 4 blank characters */
   char rds[16];
   char status[16]; /*030605*/
   int  j,k;
   char greekchar  = ' ';
   char branchchar = ' ';
   int  match=0;

level = level; /*avoids unused error message*/
                  /*||( tryatom[4][1]=='h'&&(atom[j][1]=='d'||atom[j][1]=='q')))*/
/*4th atom of a dihedral defines the level of all the connected riding atoms*/
/*unfortunately the position-specific code of the oldpdb is broken... */
/*however, the third character of the oldcode or its equivalent is the level */
/*of heavy atoms that rides directly on the rotation axis */
/*while hydrogens are of the next level below */
/*tryatom is define here so that the third index is the old definition*/
/*Standardizing tryatom as idealized oldpdb simplifies the logic here */ 
/*element, greek, branch, Htwig */
/*oldpdb element[1], greek[2], branch[3], Htwig[0]*/
/*newpdb element[0,1], greek[1,2], branch[2,3], Htwig[3,] */

   for(j=1 ; j<=maxatm ; j++)
   {/*scan over all atoms to find atoms only riding on this axis*/
      match = 0;
      if(isc[j] > 0 ) /*only look at recognized sc, inc. alt conf. info.*/
      {/*recognized sidechain atom*/                              
         /*if(tryatom[4][0]==' ' && tryatom[4][1]=='h')*/ /*oldpdb standard*/
         if(isahydrogenname(tryatom[4]))
         {  /*fourth tryatom is a hydrogen, only move hydrogens with this axis*/
            /*move all other hydrogens at this level*/
            if(isahydrogenname(atom[j]))  /*_hgt,hgbt*/
            {  /*test atom is a hydrogen*/
               if(atom[j][0]==' ') /*_hgt*/
               {
                  greekchar  = atom[j][2];
                  branchchar = ' '; /*only one branch with Htwigs*/
               }
               else 
               {
                  greekchar  = atom[j][1];
                  branchchar = atom[j][2];
               }
               if(  (greekchar  == tryatom[4][2]) 
                  &&(branchchar == tryatom[4][3] || branchchar == tryatom[4][4]) )
               {
                  match=1;
/*fprintf(stderr,"level %d: %s ISA Hydrogen finds hydrogen: %s\n",level,tryatom[4],atom[j]);*/
               }      
            }
         }
         else /*fourth tryatom is NOT a hydrogen*/
         {  
            if(isahydrogenname(atom[j]))  /*_hnm,hnmo*/
            {  /*test atom is a hydrogen*/
               /*move hydrogens at level of third tryatom*/
               if(atom[j][0]==' ') 
               {
                  greekchar  = atom[j][2]; /*_hbt*/
                  branchchar = ' '; /*only one branch with Htwigs*/
               }
               else 
               {
                  greekchar  = atom[j][1]; /*hgbt*/
                  branchchar = atom[j][2];
               }
               if(  (greekchar  == tryatom[3][2]) 
                  &&(branchchar == tryatom[3][3] || branchchar == tryatom[3][4]) )
               {
                  match=1;
/*fprintf(stderr,"level %d: %s NOT Hydrogen finds hydrogen: %s\n",level,tryatom[4],atom[j]);*/
               }      
            }
            else /*test atom is NOT a hydrogen*/
            {
               /*move all heavy atoms at this heavy-atom fourth tryatom level*/ 
                  greekchar  = atom[j][2]; /*_egb*/
                  branchchar = atom[j][3];
               if(  (greekchar  == tryatom[4][2]) 
                  &&(branchchar == tryatom[4][3] || branchchar == tryatom[4][4]) )
               {
                  match=1;
/*fprintf(stderr,"level %d: %s NOT Hydrogen finds NOT hydrogen: %s\n",level,tryatom[4],atom[j]);*/
               }      
            } 
         }

         if(match)
         {/*match*/
            k = base[j];
            if(Latomradii)
            {
               /*if(atom[j][1]=='h') */
               if(isahydrogenname(atom[j]))  /*071222*/
               {
                  if(atom[k][1]=='o'||atom[k][1]=='n')
                  {
                      sprintf(rds," r=%.2f ",polarHradius);
                  }
                  else
                  {
                      sprintf(rds," r=1.17 ");
                  }
               }
               else if(atom[j][1]=='o') sprintf(rds," r=1.4 ");
               else if(atom[j][1]=='n') sprintf(rds," r=1.55 ");
               else if(atom[j][1]=='c') sprintf(rds," r=1.75 ");
               else if(atom[j][1]=='s') sprintf(rds," r=1.8 ");
               else if(atom[j][1]=='p') sprintf(rds," r=1.8 ");
               else rds[0] = '\0';
            }
            else  
            {
               rds[0] = '\0';
            }

  if(  res[j][0]=='p' &&res[j][1]=='r' &&res[j][2]=='o'
     &&atom[j][1]=='n' &&atom[j][2]=='e')
  {
     sprintf(status," ghost"); /*Mage not to send atom ne to probe 030506*/
  }
  else if(atstyle[j] == 1)
  {
     sprintf(status," ghost"); /*Mage not to send extra atoms to probe 030920*/
  }
  else
  {
     status[0] = '\0'; /*no status flag*/
  }
#ifdef OLDCODE
            if(tryatom[4][1]=='h') sprintf(cntl,"ro%1dh",nthrot); /*|ro#h|*/
            else if(  (tryatom[4][1]==' ')
                    &&(isahydrogenname(atom[j])) )
                                   sprintf(cntl,"ro%1dh",nthrot); /*|ro#h|*/
            else                   sprintf(cntl,"ro%1d ",nthrot); /*|ro#_|*/
#endif
            if(tryatom[4][1]=='h') sprintf(cntl,"ro%1dh",nthrot); /*|ro#h|*/
            else                   sprintf(cntl,"ro%1d ",nthrot); /*|ro#_|*/
            /*make pointID fit PDB format spacing*/ 
            /*format(a6,i5,1x,a5,a3,1x,a1,i4,a1,3x,3f8.3,2f6.2)*/
            sprintf(temps,"%s{%5s%3s%2s%4d%1s} P %.3f, %.3f, %.3f "
                            "{%5s%3s%2s%4d%1s} L%s%s %.3f, %.3f, %.3f"EOLO
             ,cntl,atom[k],res[k],sub[k],num[k],rins[k],x[k],y[k],z[k]
                  ,atom[j],res[j],sub[j],num[j],rins[j],rds,status
                  ,x[j],y[j],z[j]);
            putonetextblockline(&mainscratch,temps); /*PDB ptID*/
            ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*P point*/
            ++countxyz;++pointcnt[typenow];countpoints(cntl,0); /*L point*/
#ifdef ROTINFO
sprintf(alertstr,"ride%1d:%s{%5s%3s%2s%4d%1s} P %.3f, %.3f, %.3f "
                        "{%5s%3s%2s%4d%1s} L%s %.3f, %.3f, %.3f"CRLF
      ,nthrot,cntl,atom[k],res[k],sub[k],num[k],rins[k],x[k],y[k],z[k]
                  ,atom[j],res[j],sub[j],num[j],rins[j],rds,x[j],y[j],z[j]);
pkintextinsert(alertstr);  /*____TEXT.C*/
adjusttext(1);             /*____TEXT.C*/
#endif
         }/*match*/
      }/*recognized sidechain atom*/                               
   }/*scan over all atoms to find atoms only riding on this axis*/
}
/*___findrotationriders()____________________________________________________*/

#ifdef ROTATIONDIAGRAM
                  1HZ_:lys                       _NH1:arg 
                   |                              |
                   |                              |
                   |                              |
                   |                              |
       1HE_:met   _NZ_:lys                       _CZ_:arg 
         <         \                              |
           <        \                             |
             <       \                            |
               <      \                           |
 fifth           <     \                          |
                   <    \                         |
                     <   \                        |
                       <  \                       |
                         < \                      |
             HD21:leu    _CE_:met    HD11:leu   _NE_:arg   _OE1:glu   _NE_:pro
             1HD2:leu    _CE_:met    1HD1:leu   _NE_:arg   _OE1:glu   _NE_:pro
               <           *<:lys      \.         \          \ :gln      |
                  <            * <         \.        \         \         |
                     <             *   <       \.       \        \       |
                        <              *    <      \.      \       \     |
                 4         <               *     <     \.     \     |    |
                              <                *      <    \.    \   |   |
                                 <                 *       <   \.  \  |  |
                                    <                  *        <  \.\ | |
          _HG1:thr                      <                   *        < \\_:pro
_HG_:cys  _HG_:ser  HG21:val  HG11:val  _CD2:leu  _OD1:asp  _SD_:met  _CD1:leu
_HG_:cys  _HG_:ser  1HG2:val  1HG1:val  _CD2:leu  _OD1:asp  _SD_:met  _CD1:leu
 <         <         <  :thr    *        \         |  :asn   /        _CD_:ile
  <          <         <:ile     *        \        |        /          >  :glu 
    <          <         <        *        \       |       /         >    :gln 
      <          <         <        *       \      |      /        >      :lys 
        <          <         <        *      \ 3   |     /       >        :arg 
          <          <         <        *     \    |    /      >          :his 
            <          <         <        *    \   |   /     >            :phe 
              <          <         <        *   \  |  /    >              :tyr 
                <         _OG1:thr   <        *          >                :trp 
    _HB2:ala   _SG_:cys   _OG_:ser  _CG2_:val    _CG1:val :ile 
    1HB_:ala   _SG_:cys   _OG_:ser  _CG2_:val    _CG1:val :ile 
     <           \         |         /   :thr    _CG_:leu :asp :asn :glu :gln 
       <          \        |        /    :ile  >     :lys :arg :met :
         <         \       |       /         >       :his :phe :tyr :trp 
           <        \      |      /        >
             <       \     |  2  /       >
               <      \    |    /      >
                 <     \   |   /    > 
                    <    \ | /  >
                          _CB_:ala 
                          /   :pro 
                       1 /
                        /
                      _CA_:gly 
                      /
                     /
                    /
                  _N__
#endif
#ifdef AADIAGRAM

oldpdb
H

Z

E                                                         3HD1
                                                          2HD1
D                 3HG2             3HG1  3HG2        _CD1 1HD1
                  2HG2             2HG1  2HG2         |   2HG1
G                 1HG2 _CG2   _CG1 1HG1  1HG2 _CG2   _CG1 1HG1
         3HB_              \ /                    \ /
B   2HB_ _CB_ 1HB_    2HB_ _CB_ 1HB_         2HB_ _CB_ 1HB_
          |                 |                      |
A        _CA_              _CA_                   _CA_
         ala               val                    ile

newpdb
H

Z

E                                                         HD13
                                                          HD12
D                 HG23             HG13  HG23        _CD1 HD11
                  HG22             HG12  HG22         |   HG12
G                 HG21 _CG2   _CG1 HG11  HG21 _CG2   _CG1 HG11
         _HB3              \ /                    \ /
B   _HB2 _CB_ _HB1    _HB3 _CB_ _HB2         _HB3 _CB_ _HB2
          |                 |                      |
A        _CA_              _CA_                   _CA_
         ala               val                    ile

#endif
/****writescrotscratch()******************************************************/
void writescrotscratch()
{
   char residuecode[4];
   int  j;

   residuecode[0] = resid[rotoutl][0];
   residuecode[1] = resid[rotoutl][1];
   residuecode[2] = resid[rotoutl][2];
   residuecode[3] = '\0';

   getresiduerotations(); /*1-letter code in char rescode*/
   /*which writes to scratch and reuses rotoutl for number of chi angles*/

   if(!mutoutl && (Listrot && Lcommanded) )
   {
      rotamerhypertext(residuecode);
      if(rotoutl > 0)
      {
         word[0] = '\0';
         sprintf(word,"*{original: ");
         for(j=1; j<=rotoutl;j++)
         {
           sprintf(temps,"%s rot={%s %d} %.2f "
                          ,word,rotatorname,j,bondangle[j]);
           sprintf(word,"%s",temps);
         }
         sprintf(temps,"%s}*\n",word);
         putonetextblockline(&hypertextstuff,temps); /*PKINSCRTS*/
      }
   }
}
/*___writescrotscratch()_____________________________________________________*/

/****rotamerhypertext()*******************************************************/
void rotamerhypertext(char res[4])
{/*add rotamer hypertext lines to text window*/
   /*MAGE text parser originally did ignore extra @text*/
   /*MAGE_5_75 kinemage parser appends @text stuff to text window*/

   int j,k,nth,inputline,Lnew,Lline,Nangles,index;
   char anglestr[256],hold[256];
   int angle[5];

   if(Lhypertextstuff==0)
   {
      inittextblock(&hypertextstuff); /*PKINSCRT.c*/
      Lhypertextstuff = 1;
   }
   sprintf(temps,"@text"EOLO); 
   putonetextblockline(&hypertextstuff,temps); /*PKINSCRTS*/
   /*PKINCSUB:rotatorname=="%s %s %d",resid[j],sub[maxatm],num[maxatm]*/
   sprintf(temps,"%s rotamers library hypertext:"EOLO,rotatorname); /*030925*/
   putonetextblockline(&hypertextstuff,temps); /*PKINSCRTS*/
   Lnew = 1;
   Lline = 0;
   inputline = 1; /*used both in while and as a return flag*/
   while(inputline==1)
   {/*get ext_ra entries, line-by-line*/
      inputline = getonerotamerline(Lnew);
      Lnew = 0;
      if(inputline==1)
      {/*valid line*/
         if(temps[0] == '@' && temps[1] == '@')
         {/*format such that can append stored @group kin stuff*/
            if(   temps[2] == res[0] 
               && temps[3] == res[1]
               && temps[4] == res[2])
            {
               Lline = 1;
               word[0] = temps[6];
               word[1] = '\0';
               Nangles = atoi(word);
   sprintf(alertstr,    CRLF"found %s rotamers with %d angles"CRLF
      ,resid[mutoutl],Nangles);
   pkintextinsert(alertstr); /*PKMCTEXT.C PKPCTEXT.C*/
   adjusttext(1);
               if(Nangles > 4) Nangles = 4;
            }
            else 
            {
               Lline = 0;
            }
         }
         else if(Lline==1)
         {/*has encountered the first line of target aa rotamers*/
            /*first, get the name (and %) for this rotamer*/
            for(j=0;j<255;j++)
            {
               if(temps[j] == ':')/*colon is delimitator*/
               {
                  word[j] = '\0';
                  break;
               }
               else
               {
                  word[j] = temps[j];
               }
            }
            /*get angle values for Nangles*/
            for(nth=1; nth<=Nangles;nth++)
            {
               for(index=0;index<256;index++) anglestr[index]='\0';
               index = 0; /*reset for getting each angle*/
               for(k=j+1;k<255;k++)
               {
                  if(  isdigit(temps[k]) 
                     ||temps[k]=='-'||temps[k]=='.'||temps[k]=='+')
                  {
                     anglestr[index++] = temps[k];
                  }
                  else if(index==0)
                  {/*not reached number field yet*/
                     ;
                  }
                  else
                  {/*have some sort of number for this angle*/
                     break;
                  }
               }
               j = k;
               anglestr[index+1] = '\0';
               angle[nth] = atoi(anglestr);
               /*030925 global PKINCSUB:rotatorname == resid,sub,num*/
               sprintf(hold," rot={%s %d} %d",rotatorname,nth,angle[nth]);
               strcat(word,hold);
            }
                     
            sprintf(hold,"*{%s}*"EOLO,word);
            putonetextblockline(&hypertextstuff,hold); /*PKINSCRTS*/
   sprintf(alertstr,    CRLF"*{%s}*"CRLF,word);
   pkintextinsert(alertstr); /*PKMCTEXT.C PKPCTEXT.C*/
   adjusttext(1);

         }
      }/*valid line*/
   }/*get rotamer entries, line-by-line*/
   sprintf(temps," "EOLO);/*blank line at end of hypertext section*/
   putonetextblockline(&hypertextstuff,temps); /*PKINSCRTS*/
}/*add rotamer hypertext lines to text window*/
/*___rotamerhypertext()______________________________________________________*/

/****mutatepdbrecords()*******************************************************/
void mutatepdbrecords(int liblines[2], float anglerot1, float anglerot2, double cbcbaxis[3], double cacbcalc[3], double caxyz[3])
{
   double xyz[3]={0,0,0};
   int    n=0,j=0,igotit=0;
   char   frags[256],pdbstr[256],cntl[5];
   float  mutx=0,muty=0,mutz=0;

   /*see PKINCSUB/threepointimpose() for explaination of angles ...*/

   Listpdb = 1;
   sprintf(cntl,"dump"); /*avoids all kinemage stuff*/
   /*liblines[0] beginning index in the library for this residue type*/
   /*liblines[1] ending    index in the library for this residue type*/
   if(liblines[0] <= liblines[1] && liblines[0] > 0)
   {/*there is at least one library line for this residue type*/
     /*rotate x,y,z of these records around ca--cb axis and move to new calpha*/
      for(n=liblines[0]; n<=liblines[1]; n++)
      {/*get one-by-one the library pdb records for this residue type*/
         if(Loldpdb) /*was getonelibraryline() 071215 */
            igotit = getoldlibraryline(0,n); /*not-Lnew, this-index*/
         else
            igotit = getnewlibraryline(0,n);

         if(igotit)
         {/*library atom record*/
            /*PDB format record atom name at indices 12,13,14,15*/
            if(!proteinmainchainname(temps[12],temps[13],temps[14],temps[15]))
            {/*just do sidechain atoms*/ /*040504*/
               /*mainchain N and CA and ideal CB used to orient sidechain*/
              /*mutated/idealized sidechain to be placed on original mainchain*/
               /*split out the coordinates for this ideal residue type*/
               for(j=0 ; j<=23 ; j++) frags[j] = temps[j+30];
               frags[24] = '\0';
               igotit =  sscanf(frags,"%8f %8f %8f",&mutx,&muty,&mutz);
               if(igotit)
               {/*xyz coordinates abstracted, rotate and translate them*/
                  xyz[0] = (double)mutx;
                  xyz[1] = (double)muty;
                  xyz[2] = (double)mutz;

                  doaxisrot(&xyz[0],&xyz[1],&xyz[2],anglerot1
                      ,0,0,0,cbcbaxis[0],cbcbaxis[1],cbcbaxis[2]);

                  doaxisrot(&xyz[0],&xyz[1],&xyz[2],anglerot2
                      ,0,0,0,cacbcalc[0],cacbcalc[1],cacbcalc[2]);

                  /*change origin to old CA position*/
                  mutx = (float)xyz[0] + (float)caxyz[0];
                  muty = (float)xyz[1] + (float)caxyz[1];
                  mutz = (float)xyz[2] + (float)caxyz[2];

                  /*recreate the pdb record*/
                  temps[30] = '\0'; /*artificial end at x coord*/
                  for(j=0 ; j<=232 ; j++) 
                  {
                     frags[j] = temps[j+54];
                     if(frags[j]=='\0') break;
                  }
                  frags[j] = '\0';
                  sprintf(pdbstr,"%s%s%8.3f%8.3f%8.3f%s"EOLO
                     ,cntl,temps,mutx,muty,mutz,frags);
                  ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
                  putonetextblockline(&mainscratch, pdbstr);
               }/*xyz coordinates abstracted, rotate and translate them*/
            }/*just do sidechain atoms*/
         }/*library atom record*/
      }/*get one-by-one the library pdb records for this residue type*/
   }/*there is at least one library line for this residue type*/
}
/*___mutatepdbrecords()______________________________________________________*/

/****proteinmainchainname()***************************************************/
int  proteinmainchainname(char name1, char name2, char name3, char name4)/*040504*/
{
   int mainchain = 0; /*default is name not indentify a mainchain atom*/

   if(name1==' ' && name4==' ')
   {/*elements of mainchain all one character, with no final number*/
      if(name2=='N' && name3==' ') {mainchain = 1;}
      else if(name2=='C' && name3=='A') {mainchain = 1;}
      else if(name2=='C' && name3==' ') {mainchain = 1;}
      else if(name2=='O' && name3==' ') {mainchain = 1;}
      else if(name2=='H' && name3==' ') {mainchain = 1;}
      else if(name2=='H' && name3=='A') {mainchain = 1;}
   }
   return(mainchain);
}
/*___proteinmainchainname()__________________________________________________*/

/****getoldlibraryline()******************************************************/
int  getoldlibraryline(int Lnew, int Nin) /*was getonelibraryline < 071215 */
{
int j=0,ireturn=0;
static int Nth=0;
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,a2,i4,a1,3x,3&8.3) Anum & 2char chainID*/
/*0123456789_123456789_123456789_123456789_123456789_123 == jth character*/
/*123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_1234*/
/*ATOM     14 1HB  ASN     1       0.201  -0.913   1.946  1.00  0.00           H   new*/

/*static char  AALIB[][85] = {""*/ /*}this form modifiable, requires <=85 char*/
/*form: char* effectively makes string constants, of any length, efficiently*/
/*ALA must be first residue since ALA used as a separator between other res*/
/*once a residue found that matches target, then change from that signals end*/
static char*  AALIB[] = {""
,"ATOM      1  CA  ALA A   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA A   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA A   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ALA A   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ALA A   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ALA A   1       0.550  -1.193  -0.774  1.00  0.00"
,"ATOM      1  O   ALA A   1       0.022  -2.301  -0.683  1.00  0.00"
,"ATOM      1  CB  ALA A   1       0.530   0.001   1.426  1.00  0.00"
,"ATOM      1  H   ALA A   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ALA A   1       0.341   0.916  -0.504  1.00  0.00"
,"ATOM      1 1HB  ALA A   1       1.630   0.001   1.409  1.00  0.00"
,"ATOM      1 2HB  ALA A   1       0.172   0.899   1.950  1.00  0.00"
,"ATOM      1 3HB  ALA A   1       0.172  -0.897   1.951  1.00  0.00"
,"ATOM      1  N   ALA A   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA A   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA R   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA R   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA R   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA R   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ARG R   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ARG R   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ARG R   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ARG R   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ARG R   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  ARG R   1       0.246   1.277   2.204  1.0   0.00"
,"ATOM      1  CD  ARG R   1       0.805   1.209   3.616  1.0   0.00"
,"ATOM      1  NE  ARG R   1       0.536   2.429   4.372  1.0   0.00"
,"ATOM      1  CZ  ARG R   1       0.916   2.627   5.629  1.0   0.00"
,"ATOM      1  NH1 ARG R   1       1.586   1.684   6.277  1.0   0.00"
,"ATOM      1  NH2 ARG R   1       0.625   3.770   6.236  1.0   0.00"
,"ATOM      1  H   ARG R   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ARG R   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  ARG R   1       0.100  -0.851   1.976  1.0   0.00"
,"ATOM      1 2HB  ARG R   1       1.624  -0.160   1.406  1.0   0.00"
,"ATOM      1 1HG  ARG R   1       0.684   2.136   1.675  1.0   0.00"
,"ATOM      1 2HG  ARG R   1      -0.840   1.445   2.245  1.0   0.00"
,"ATOM      1 1HD  ARG R   1       0.367   0.347   4.140  1.0   0.00"
,"ATOM      1 2HD  ARG R   1       1.891   1.038   3.570  1.0   0.00"
,"ATOM      1  HE  ARG R   1       0.032   3.161   3.914  1.0   0.00"
,"ATOM      1 1HH1 ARG R   1       1.805   0.823   5.818  1.0   0.00"
,"ATOM      1 2HH1 ARG R   1       1.872   1.833   7.223  1.0   0.00"
,"ATOM      1 1HH2 ARG R   1       0.120   4.480   5.747  1.0   0.00"
,"ATOM      1 2HH2 ARG R   1       0.911   3.920   7.182  1.0   0.00"
,"ATOM      1  N   ALA R   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA R   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA R   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA N   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA N   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA N   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ASN N   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ASN N   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ASN N   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ASN N   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ASN N   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  ASN N   1       0.186   1.268   2.186  1.0   0.00"
,"ATOM      1  OD1 ASN N   1      -0.124   2.295   1.582  1.0   0.00"
,"ATOM      1  ND2 ASN N   1       0.233   1.199   3.511  1.0   0.00"
,"ATOM      1  H   ASN N   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ASN N   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  ASN N   1       0.128  -0.867   1.973  1.0   0.00"
,"ATOM      1 2HB  ASN N   1       1.629  -0.119   1.411  1.0   0.00"
,"ATOM      1 1HD2 ASN N   1       0.011   2.004   4.062  1.0   0.00"
,"ATOM      1 2HD2 ASN N   1       0.491   0.343   3.959  1.0   0.00"
,"ATOM      1  N   ALA N   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA N   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA N   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA D   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA D   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA D   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ASP D   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ASP D   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ASP D   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ASP D   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ASP D   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  ASP D   1       0.292   1.315   2.147  1.0   0.00"
,"ATOM      1  OD1 ASP D   1      -0.028   2.311   1.464  1.0   0.00"
,"ATOM      1  OD2 ASP D   1       0.418   1.349   3.389  1.0   0.00"
,"ATOM      1  H   ASP D   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ASP D   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  ASP D   1       0.060  -0.813   1.999  1.0   0.00"
,"ATOM      1 2HB  ASP D   1       1.616  -0.209   1.416  1.0   0.00"
,"ATOM      1  N   ALA D   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA D   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA D   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA C   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA C   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA C   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   CYS C   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  CYS C   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   CYS C   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   CYS C   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  CYS C   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  SG  CYS C   1       0.146   1.492   2.376  1.0   0.00"
,"ATOM      1  H   CYS C   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  CYS C   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  CYS C   1       0.127  -0.872   1.964  1.0   0.00"
,"ATOM      1 2HB  CYS C   1       1.629  -0.125   1.402  1.0   0.00"
,"ATOM      1  HG  CYS C   1       0.641   1.390   3.573  1.0   0.00"
,"ATOM      1  N   ALA C   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA C   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA C   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA Q   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA Q   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA Q   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   GLN Q   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  GLN Q   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   GLN Q   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   GLN Q   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  GLN Q   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  GLN Q   1       0.246   1.278   2.204  1.0   0.00"
,"ATOM      1  CD  GLN Q   1       0.790   1.240   3.618  1.0   0.00"
,"ATOM      1  OE1 GLN Q   1       0.972   0.168   4.196  1.0   0.00"
,"ATOM      1  NE2 GLN Q   1       1.054   2.414   4.180  1.0   0.00"
,"ATOM      1  H   GLN Q   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  GLN Q   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  GLN Q   1       0.099  -0.850   1.977  1.0   0.00"
,"ATOM      1 2HB  GLN Q   1       1.624  -0.161   1.406  1.0   0.00"
,"ATOM      1 1HG  GLN Q   1       0.685   2.132   1.668  1.0   0.00"
,"ATOM      1 2HG  GLN Q   1      -0.841   1.442   2.238  1.0   0.00"
,"ATOM      1 1HE2 GLN Q   1       1.416   2.453   5.111  1.0   0.00"
,"ATOM      1 2HE2 GLN Q   1       0.890   3.259   3.672  1.0   0.00"
,"ATOM      1  N   ALA Q   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA Q   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA Q   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA E   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA E   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA E   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   GLU E   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  GLU E   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   GLU E   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   GLU E   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  GLU E   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  GLU E   1       0.246   1.278   2.204  1.0   0.00"
,"ATOM      1  CD  GLU E   1       0.790   1.240   3.618  1.0   0.00"
,"ATOM      1  OE1 GLU E   1       1.224   0.155   4.060  1.0   0.00"
,"ATOM      1  OE2 GLU E   1       0.784   2.296   4.285  1.0   0.00"
,"ATOM      1  H   GLU E   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  GLU E   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  GLU E   1       0.099  -0.850   1.977  1.0   0.00"
,"ATOM      1 2HB  GLU E   1       1.624  -0.161   1.406  1.0   0.00"
,"ATOM      1 1HG  GLU E   1       0.685   2.132   1.668  1.0   0.00"
,"ATOM      1 2HG  GLU E   1      -0.841   1.442   2.238  1.0   0.00"
,"ATOM      1  N   ALA E   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA E   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA E   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA G   0      -3.648   0.915  -0.467  1.00  0.00"
,"ATOM      1  C   ALA G   0      -2.128   1.019  -0.520  1.00  0.00"
,"ATOM      1  O   ALA G   0      -1.580   2.001  -1.021  1.00  0.00"
,"ATOM      1  N   GLY G   1      -1.451   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  GLY G   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   GLY G   1       0.580  -1.175  -0.762  1.00  0.00"
,"ATOM      1  O   GLY G   1       0.073  -2.293  -0.678  1.00  0.00"
,"ATOM      1  H   GLY G   1      -1.872  -0.808   0.412  1.00  0.00"
,"ATOM      1 1HA  GLY G   1       0.363   0.938  -0.446  1.00  0.00"
,"ATOM      1 2HA  GLY G   1       0.364  -0.025   1.038  1.00  0.00"
,"ATOM      1  N   ALA G   2       1.614  -0.958  -1.536  1.00  0.00"
,"ATOM      1  CA  ALA G   2       2.237  -2.013  -2.327  1.00  0.00"
,"ATOM      1  H   ALA G   2       2.047  -0.063  -1.640  1.00  0.00"
,"ATOM      1  CA  ALA H   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA H   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA H   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   HIS H   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  HIS H   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   HIS H   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   HIS H   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  HIS H   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  HIS H   1       0.205   1.241   2.202  1.0   0.00"
,"ATOM      1  ND1 HIS H   1       0.775   2.464   1.923  1.0   0.00"
,"ATOM      1  CD2 HIS H   1      -0.637   1.449   3.241  1.0   0.00"
,"ATOM      1  CE1 HIS H   1       0.298   3.370   2.757  1.0   0.00"
,"ATOM      1  NE2 HIS H   1      -0.560   2.780   3.566  1.0   0.00"
,"ATOM      1  H   HIS H   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  HIS H   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  HIS H   1       0.128  -0.871   1.967  1.0   0.00"
,"ATOM      1 2HB  HIS H   1       1.629  -0.123   1.405  1.0   0.00"
,"ATOM      1  HD1 HIS H   1       1.445   2.638   1.200  1.0   0.00"
,"ATOM      1  HD2 HIS H   1      -1.265   0.692   3.733  1.0   0.00"
,"ATOM      1  HE1 HIS H   1       0.571   4.436   2.775  1.0   0.00"
,"ATOM      1  HE2 HIS H   1      -1.072   3.231   4.297  1.0   0.00"
,"ATOM      1  N   ALA H   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA H   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA H   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA I   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA I   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA I   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ILE I   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ILE I   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ILE I   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ILE I   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ILE I   1       0.565  -0.015   1.433  1.0   0.00"
,"ATOM      1  CG1 ILE I   1       0.196   1.277   2.165  1.0   0.00"
,"ATOM      1  CD1 ILE I   1       0.502   1.248   3.646  1.0   0.00"
,"ATOM      1  CG2 ILE I   1       2.081  -0.139   1.407  1.0   0.00"
,"ATOM      1  H   ILE I   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ILE I   1       0.318   0.927  -0.499  1.00  0.00"
,"ATOM      1  HB  ILE I   1       0.129  -0.878   1.957  1.0   0.00"
,"ATOM      1 1HG1 ILE I   1       0.739   2.116   1.705  1.0   0.00"
,"ATOM      1 2HG1 ILE I   1      -0.877   1.473   2.025  1.0   0.00"
,"ATOM      1 1HD1 ILE I   1       0.211   2.208   4.100  1.0   0.00"
,"ATOM      1 2HD1 ILE I   1      -0.061   0.433   4.122  1.0   0.00"
,"ATOM      1 3HD1 ILE I   1       1.580   1.086   3.795  1.0   0.00"
,"ATOM      1 1HG2 ILE I   1       2.466  -0.148   2.437  1.0   0.00"
,"ATOM      1 2HG2 ILE I   1       2.363  -1.075   0.902  1.0   0.00"
,"ATOM      1 3HG2 ILE I   1       2.509   0.715   0.863  1.0   0.00"
,"ATOM      1  N   ALA I   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA I   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA I   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA L   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA L   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA L   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   LEU L   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  LEU L   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   LEU L   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   LEU L   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  LEU L   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  LEU L   1       0.230   1.243   2.271  1.0   0.00"
,"ATOM      1  CD1 LEU L   1       0.711   1.057   3.702  1.0   0.00"
,"ATOM      1  CD2 LEU L   1       0.923   2.466   1.692  1.0   0.00"
,"ATOM      1  H   LEU L   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  LEU L   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  LEU L   1       0.126  -0.877   1.955  1.0   0.00"
,"ATOM      1 2HB  LEU L   1       1.627  -0.130   1.394  1.0   0.00"
,"ATOM      1  HG  LEU L   1      -0.860   1.390   2.258  1.0   0.00"
,"ATOM      1 1HD1 LEU L   1       0.481   1.961   4.286  1.0   0.00"
,"ATOM      1 2HD1 LEU L   1       0.201   0.192   4.151  1.0   0.00"
,"ATOM      1 3HD1 LEU L   1       1.797   0.885   3.705  1.0   0.00"
,"ATOM      1 1HD2 LEU L   1       0.691   3.346   2.308  1.0   0.00"
,"ATOM      1 2HD2 LEU L   1       2.010   2.303   1.683  1.0   0.00"
,"ATOM      1 3HD2 LEU L   1       0.570   2.635   0.663  1.0   0.00"
,"ATOM      1  N   ALA L   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA L   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA L   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA K   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA K   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA K   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   LYS K   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  LYS K   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   LYS K   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   LYS K   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  LYS K   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  LYS K   1       0.246   1.277   2.204  1.0   0.00"
,"ATOM      1  CD  LYS K   1       0.805   1.209   3.616  1.0   0.00"
,"ATOM      1  CE  LYS K   1       0.516   2.486   4.387  1.0   0.00"
,"ATOM      1  NZ  LYS K   1       1.058   2.433   5.773  1.0   0.00"
,"ATOM      1  H   LYS K   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  LYS K   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  LYS K   1       0.100  -0.851   1.976  1.0   0.00"
,"ATOM      1 2HB  LYS K   1       1.624  -0.160   1.406  1.0   0.00"
,"ATOM      1 1HG  LYS K   1       0.684   2.136   1.675  1.0   0.00"
,"ATOM      1 2HG  LYS K   1      -0.840   1.445   2.245  1.0   0.00"
,"ATOM      1 1HD  LYS K   1       0.368   0.350   4.144  1.0   0.00"
,"ATOM      1 2HD  LYS K   1       1.892   1.041   3.574  1.0   0.00"
,"ATOM      1 1HE  LYS K   1       0.952   3.343   3.855  1.0   0.00"
,"ATOM      1 2HE  LYS K   1      -0.572   2.652   4.426  1.0   0.00"
,"ATOM      1 1HZ  LYS K   1       0.848   3.288   6.247  1.0   0.00"
,"ATOM      1 2HZ  LYS K   1       0.642   1.668   6.265  1.0   0.00"
,"ATOM      1 3HZ  LYS K   1       2.049   2.306   5.738  1.0   0.00"
,"ATOM      1  N   ALA K   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA K   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA K   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA M   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA M   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA M   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   MET M   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  MET M   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   MET M   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   MET M   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  MET M   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  MET M   1       0.205   1.258   2.219  1.0   0.00"
,"ATOM      1  SD  MET M   1       0.993   2.730   1.544  1.0   0.00"
,"ATOM      1  CE  MET M   1       2.708   2.414   1.953  1.0   0.00"
,"ATOM      1  H   MET M   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  MET M   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  MET M   1       0.127  -0.871   1.966  1.0   0.00"
,"ATOM      1 2HB  MET M   1       1.629  -0.125   1.404  1.0   0.00"
,"ATOM      1 1HG  MET M   1      -0.886   1.402   2.228  1.0   0.00"
,"ATOM      1 2HG  MET M   1       0.521   1.125   3.265  1.0   0.00"
,"ATOM      1 1HE  MET M   1       3.331   3.247   1.596  1.0   0.00"
,"ATOM      1 2HE  MET M   1       2.814   2.320   3.044  1.0   0.00"
,"ATOM      1 3HE  MET M   1       3.035   1.481   1.471  1.0   0.00"
,"ATOM      1  N   ALA M   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA M   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA M   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA M   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA M   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA M   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   MSE M   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  MSE M   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   MSE M   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   MSE M   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  MSE M   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  MSE M   1       0.205   1.258   2.219  1.0   0.00"
,"ATOM      1 SED  MSE M   1       1.047   2.831   1.498  1.0   0.00"
,"ATOM      1  CE  MSE M   1       2.865   2.414   1.975  1.0   0.00"
,"ATOM      1  H   MSE M   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  MSE M   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  MSE M   1       0.127  -0.871   1.966  1.0   0.00"
,"ATOM      1 2HB  MSE M   1       1.629  -0.125   1.404  1.0   0.00"
,"ATOM      1 1HG  MSE M   1      -0.886   1.402   2.228  1.0   0.00"
,"ATOM      1 2HG  MSE M   1       0.521   1.125   3.265  1.0   0.00"
,"ATOM      1 1HE  MSE M   1       3.529   3.222   1.637  1.0   0.00"
,"ATOM      1 2HE  MSE M   1       2.943   2.309   3.067  1.0   0.00"
,"ATOM      1 3HE  MSE M   1       3.163   1.470   1.495  1.0   0.00"
,"ATOM      1  N   ALA M   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA M   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA M   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA F   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA F   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA F   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   PHE F   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  PHE F   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   PHE F   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   PHE F   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  PHE F   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  PHE F   1       0.205   1.245   2.204  1.0   0.00"
,"ATOM      1  CD1 PHE F   1       1.005   2.370   2.106  1.0   0.00"
,"ATOM      1  CD2 PHE F   1      -0.908   1.294   3.025  1.0   0.00"
,"ATOM      1  CE1 PHE F   1       0.702   3.517   2.816  1.0   0.00"
,"ATOM      1  CE2 PHE F   1      -1.215   2.439   3.736  1.0   0.00"
,"ATOM      1  CZ  PHE F   1      -0.409   3.551   3.631  1.0   0.00"
,"ATOM      1  H   PHE F   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  PHE F   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  PHE F   1       0.127  -0.871   1.967  1.0   0.00"
,"ATOM      1 2HB  PHE F   1       1.629  -0.124   1.405  1.0   0.00"
,"ATOM      1  HD1 PHE F   1       1.894   2.352   1.458  1.0   0.00"
,"ATOM      1  HD2 PHE F   1      -1.556   0.410   3.113  1.0   0.00"
,"ATOM      1  HE1 PHE F   1       1.347   4.405   2.729  1.0   0.00"
,"ATOM      1  HE2 PHE F   1      -2.102   2.462   4.386  1.0   0.00"
,"ATOM      1  HZ  PHE F   1      -0.653   4.464   4.195  1.0   0.00"
,"ATOM      1  N   ALA F   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA F   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA F   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA P   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA P   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA P   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   PRO P   1      -1.466   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  PRO P   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   PRO P   1       0.566  -1.218  -0.723  1.00  0.00"
,"ATOM      1  O   PRO P   1      -0.181  -2.059  -1.222  1.00  0.00"
,"ATOM      1  CB  PRO P   1       0.344  -0.001   1.491  1.0   0.00"
,"ATOM      1  CG  PRO P   1      -0.847  -0.666   2.144  1.0   0.00"
,"ATOM      1  CD  PRO P   1      -2.010  -0.194   1.291  1.0   0.00"
,"ATOM      1  NE  PRO P   1      -1.384   0.013  -0.026  1.0   0.00"
,"ATOM      1  HA  PRO P   1       0.434   0.857  -0.537  1.00  0.00"
,"ATOM      1 1HB  PRO P   1       1.273  -0.555   1.690  1.0   0.00"
,"ATOM      1 2HB  PRO P   1       0.492   1.021   1.870  1.0   0.00"
,"ATOM      1 1HG  PRO P   1      -0.755  -1.762   2.138  1.0   0.00"
,"ATOM      1 2HG  PRO P   1      -0.960  -0.359   3.194  1.0   0.00"
,"ATOM      1 1HD  PRO P   1      -2.815  -0.942   1.247  1.0   0.00"
,"ATOM      1 2HD  PRO P   1      -2.454   0.734   1.682  1.0   0.00"
,"ATOM      1  N   ALA P   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA P   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA P   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA S   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA S   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA S   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   SER S   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  SER S   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   SER S   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   SER S   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  SER S   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  OG  SER S   1       0.133  -1.167   2.128  1.0   0.00"
,"ATOM      1  H   SER S   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  SER S   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  SER S   1       0.172   0.892   1.964  1.0   0.00"
,"ATOM      1 2HB  SER S   1       1.634   0.061   1.418  1.0   0.00"
,"ATOM      1  HG  SER S   1       0.498  -1.142   3.059  1.0   0.00"
,"ATOM      1  N   ALA S   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA S   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA S   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA T   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA T   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA T   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   THR T   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  THR T   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   THR T   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   THR T   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  THR T   1       0.565  -0.015   1.433  1.0   0.00"
,"ATOM      1  OG1 THR T   1       0.152  -1.214   2.100  1.0   0.00"
,"ATOM      1  CG2 THR T   1       0.058   1.185   2.218  1.0   0.00"
,"ATOM      1  H   THR T   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  THR T   1       0.318   0.927  -0.499  1.00  0.00"
,"ATOM      1  HB  THR T   1       1.663   0.026   1.376  1.0   0.00"
,"ATOM      1  HG1 THR T   1       0.521  -1.224   3.030  1.0   0.00"
,"ATOM      1 1HG2 THR T   1       0.471   1.158   3.237  1.0   0.00"
,"ATOM      1 2HG2 THR T   1       0.377   2.112   1.718  1.0   0.00"
,"ATOM      1 3HG2 THR T   1      -1.039   1.154   2.267  1.0   0.00"
,"ATOM      1  N   ALA T   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA T   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA T   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  C   ALA W   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA W   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   TRP W   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  TRP W   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   TRP W   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   TRP W   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  TRP W   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  TRP W   1       0.203   1.244   2.198  1.0   0.00"
,"ATOM      1  CD1 TRP W   1      -0.857   1.429   3.038  1.0   0.00"
,"ATOM      1  CD2 TRP W   1       0.936   2.476   2.193  1.0   0.00"
,"ATOM      1  NE1 TRP W   1      -0.834   2.701   3.557  1.0   0.00"
,"ATOM      1  CE2 TRP W   1       0.261   3.363   3.053  1.0   0.00"
,"ATOM      1  CE3 TRP W   1       2.095   2.910   1.545  1.0   0.00"
,"ATOM      1  CZ2 TRP W   1       0.705   4.664   3.283  1.0   0.00"
,"ATOM      1  CZ3 TRP W   1       2.536   4.200   1.772  1.0   0.00"
,"ATOM      1  CH2 TRP W   1       1.845   5.067   2.636  1.0   0.00"
,"ATOM      1  H   TRP W   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  TRP W   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  TRP W   1       0.128  -0.870   1.968  1.0   0.00"
,"ATOM      1 2HB  TRP W   1       1.629  -0.122   1.406  1.0   0.00"
,"ATOM      1  HD1 TRP W   1      -1.620   0.671   3.265  1.0   0.00"
,"ATOM      1  HE1 TRP W   1      -1.501   3.082   4.197  1.0   0.00"
,"ATOM      1  HE3 TRP W   1       2.645   2.239   0.867  1.0   0.00"
,"ATOM      1  HZ2 TRP W   1       0.164   5.342   3.958  1.0   0.00"
,"ATOM      1  HZ3 TRP W   1       3.446   4.555   1.268  1.0   0.00"
,"ATOM      1  HH2 TRP W   1       2.225   6.086   2.794  1.0   0.00"
,"ATOM      1  N   ALA W   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA W   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA W   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA Y   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA Y   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA Y   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   TYR Y   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  TYR Y   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   TYR Y   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   TYR Y   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  TYR Y   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  TYR Y   1       0.203   1.253   2.212  1.0   0.00"
,"ATOM      1  CD1 TYR Y   1       1.005   2.383   2.118  1.0   0.00"
,"ATOM      1  CD2 TYR Y   1      -0.913   1.302   3.038  1.0   0.00"
,"ATOM      1  CE1 TYR Y   1       0.709   3.531   2.825  1.0   0.00"
,"ATOM      1  CE2 TYR Y   1      -1.225   2.441   3.752  1.0   0.00"
,"ATOM      1  CZ  TYR Y   1      -0.413   3.555   3.645  1.0   0.00"
,"ATOM      1  OH  TYR Y   1      -0.716   4.696   4.353  1.0   0.00"
,"ATOM      1  H   TYR Y   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  TYR Y   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1 1HB  TYR Y   1       0.127  -0.871   1.967  1.0   0.00"
,"ATOM      1 2HB  TYR Y   1       1.629  -0.124   1.405  1.0   0.00"
,"ATOM      1  HD1 TYR Y   1       1.892   2.363   1.469  1.0   0.00"
,"ATOM      1  HD2 TYR Y   1      -1.559   0.416   3.124  1.0   0.00"
,"ATOM      1  HE1 TYR Y   1       1.355   4.418   2.741  1.0   0.00"
,"ATOM      1  HE2 TYR Y   1      -2.112   2.462   4.403  1.0   0.00"
,"ATOM      1  HH  TYR Y   1      -1.547   4.545   4.887  1.0   0.00"
,"ATOM      1  N   ALA Y   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA Y   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA Y   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA V   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA V   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA V   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   VAL V   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  VAL V   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   VAL V   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   VAL V   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  VAL V   1       0.565  -0.015   1.433  1.0   0.00"
,"ATOM      1  CG1 VAL V   1       2.081  -0.139   1.407  1.0   0.00"
,"ATOM      1  CG2 VAL V   1       0.200   1.269   2.162  1.0   0.00"
,"ATOM      1  H   VAL V   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  VAL V   1       0.318   0.927  -0.499  1.00  0.00"
,"ATOM      1  HB  VAL V   1       0.128  -0.879   1.956  1.0   0.00"
,"ATOM      1 1HG1 VAL V   1       2.466  -0.148   2.437  1.0   0.00"
,"ATOM      1 2HG1 VAL V   1       2.363  -1.075   0.902  1.0   0.00"
,"ATOM      1 3HG1 VAL V   1       2.509   0.715   0.863  1.0   0.00"
,"ATOM      1 1HG2 VAL V   1       0.611   1.241   3.183  1.0   0.00"
,"ATOM      1 2HG2 VAL V   1       0.619   2.130   1.621  1.0   0.00"
,"ATOM      1 3HG2 VAL V   1      -0.895   1.366   2.210  1.0   0.00"
,"ATOM      1  N   ALA V   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA V   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA V   2       2.055  -0.074  -1.617  1.00  0.00"
,"END"
,"                                                                                     "};
/*pad out end so read to j==79 will not run off end of the array! */

   if(Lnew) Nth = 1;
   else if(Nin > 0) Nth = Nin; /*040303*/
   else Nth++;

   for(j=0; j<80; j++)
   {
      temps[j] =  AALIB[Nth][j];
      if(temps[j] == '\0') break;
   }
   temps[j+1] = '\0'; 
   if(temps[0] == 'E' && temps[1] == 'N' && temps[2] == 'D') ireturn = 0;
   else ireturn = Nth; /*040303 return index into library instead of just 1 */
   return(ireturn);
}
/*___getoldlibraryline()_____________________________________________________*/

/****getnewlibraryline()******************************************************/
int  getnewlibraryline(int Lnew, int Nin) /* from getonelibraryline 071215*/
{
int j=0,ireturn=0;
static int Nth=0;
/*name[n],Anum[n],atom[n],res[n],sub[n],&num[n],rins[n],&x[n],&y[n],&z[n]*/
/*format(a6,a5,1x,a5,a3,a2,i4,a1,3x,3&8.3) Anum & 2char chainID*/
/*0123456789_123456789_123456789_123456789_123456789_123 == jth character*/
/*123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_1234*/
/*ATOM     14  HB1 ASN     1       0.201  -0.913   1.946  1.00  0.00           H   new*/

/*static char  AALIB[][85] = {""*/ /*}this form modifiable, requires <=85 char*/
/*form: char* effectively makes string constants, of any length, efficiently*/
/*ALA must be first residue since ALA used as a separator between other res*/
/*once a residue found that matches target, then change from that signals end*/
static char*  AALIB[] = {""
,"ATOM      1  CA  ALA A   0      -3.674   0.874  -0.453  1.00  0.00           C  "
,"ATOM      1  C   ALA A   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA A   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ALA A   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ALA A   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ALA A   1       0.550  -1.193  -0.774  1.00  0.00"
,"ATOM      1  O   ALA A   1       0.022  -2.301  -0.683  1.00  0.00"
,"ATOM      1  CB  ALA A   1       0.530   0.001   1.426  1.00  0.00"
,"ATOM      1  H   ALA A   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ALA A   1       0.341   0.916  -0.504  1.00  0.00"
,"ATOM      1  HB1 ALA A   1       1.630   0.001   1.409  1.00  0.00"
,"ATOM      1  HB2 ALA A   1       0.172   0.899   1.950  1.00  0.00"
,"ATOM      1  HB3 ALA A   1       0.172  -0.897   1.951  1.00  0.00"
,"ATOM      1  N   ALA A   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA A   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA R   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA R   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA R   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA R   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ARG R   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ARG R   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ARG R   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ARG R   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ARG R   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  ARG R   1       0.246   1.277   2.204  1.0   0.00"
,"ATOM      1  CD  ARG R   1       0.805   1.209   3.616  1.0   0.00"
,"ATOM      1  NE  ARG R   1       0.536   2.429   4.372  1.0   0.00"
,"ATOM      1  CZ  ARG R   1       0.916   2.627   5.629  1.0   0.00"
,"ATOM      1  NH1 ARG R   1       1.586   1.684   6.277  1.0   0.00"
,"ATOM      1  NH2 ARG R   1       0.625   3.770   6.236  1.0   0.00"
,"ATOM      1  H   ARG R   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ARG R   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 ARG R   1       0.100  -0.851   1.976  1.0   0.00"
,"ATOM      1  HB3 ARG R   1       1.624  -0.160   1.406  1.0   0.00"
,"ATOM      1  HG2 ARG R   1       0.684   2.136   1.675  1.0   0.00"
,"ATOM      1  HG3 ARG R   1      -0.840   1.445   2.245  1.0   0.00"
,"ATOM      1  HD2 ARG R   1       0.367   0.347   4.140  1.0   0.00"
,"ATOM      1  HD3 ARG R   1       1.891   1.038   3.570  1.0   0.00"
,"ATOM      1  HE  ARG R   1       0.032   3.161   3.914  1.0   0.00"
,"ATOM      1 HH11 ARG R   1       1.805   0.823   5.818  1.0   0.00"
,"ATOM      1 HH12 ARG R   1       1.872   1.833   7.223  1.0   0.00"
,"ATOM      1 HH21 ARG R   1       0.120   4.480   5.747  1.0   0.00"
,"ATOM      1 HH22 ARG R   1       0.911   3.920   7.182  1.0   0.00"
,"ATOM      1  N   ALA R   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA R   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA R   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA N   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA N   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA N   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ASN N   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ASN N   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ASN N   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ASN N   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ASN N   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  ASN N   1       0.186   1.268   2.186  1.0   0.00"
,"ATOM      1  OD1 ASN N   1      -0.124   2.295   1.582  1.0   0.00"
,"ATOM      1  ND2 ASN N   1       0.233   1.199   3.511  1.0   0.00"
,"ATOM      1  H   ASN N   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ASN N   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 ASN N   1       0.128  -0.867   1.973  1.0   0.00"
,"ATOM      1  HB3 ASN N   1       1.629  -0.119   1.411  1.0   0.00"
,"ATOM      1 HD21 ASN N   1       0.011   2.004   4.062  1.0   0.00"
,"ATOM      1 HD22 ASN N   1       0.491   0.343   3.959  1.0   0.00"
,"ATOM      1  N   ALA N   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA N   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA N   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA D   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA D   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA D   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ASP D   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ASP D   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ASP D   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ASP D   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ASP D   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  ASP D   1       0.292   1.315   2.147  1.0   0.00"
,"ATOM      1  OD1 ASP D   1      -0.028   2.311   1.464  1.0   0.00"
,"ATOM      1  OD2 ASP D   1       0.418   1.349   3.389  1.0   0.00"
,"ATOM      1  H   ASP D   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ASP D   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 ASP D   1       0.060  -0.813   1.999  1.0   0.00"
,"ATOM      1  HB3 ASP D   1       1.616  -0.209   1.416  1.0   0.00"
,"ATOM      1  N   ALA D   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA D   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA D   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA C   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA C   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA C   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   CYS C   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  CYS C   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   CYS C   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   CYS C   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  CYS C   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  SG  CYS C   1       0.146   1.492   2.376  1.0   0.00"
,"ATOM      1  H   CYS C   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  CYS C   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 CYS C   1       0.127  -0.872   1.964  1.0   0.00"
,"ATOM      1  HB3 CYS C   1       1.629  -0.125   1.402  1.0   0.00"
,"ATOM      1  HG  CYS C   1       0.641   1.390   3.573  1.0   0.00"
,"ATOM      1  N   ALA C   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA C   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA C   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA Q   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA Q   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA Q   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   GLN Q   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  GLN Q   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   GLN Q   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   GLN Q   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  GLN Q   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  GLN Q   1       0.246   1.278   2.204  1.0   0.00"
,"ATOM      1  CD  GLN Q   1       0.790   1.240   3.618  1.0   0.00"
,"ATOM      1  OE1 GLN Q   1       0.972   0.168   4.196  1.0   0.00"
,"ATOM      1  NE2 GLN Q   1       1.054   2.414   4.180  1.0   0.00"
,"ATOM      1  H   GLN Q   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  GLN Q   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 GLN Q   1       0.099  -0.850   1.977  1.0   0.00"
,"ATOM      1  HB3 GLN Q   1       1.624  -0.161   1.406  1.0   0.00"
,"ATOM      1  HG2 GLN Q   1       0.685   2.132   1.668  1.0   0.00"
,"ATOM      1  HG3 GLN Q   1      -0.841   1.442   2.238  1.0   0.00"
,"ATOM      1 HE21 GLN Q   1       1.416   2.453   5.111  1.0   0.00"
,"ATOM      1 HE22 GLN Q   1       0.890   3.259   3.672  1.0   0.00"
,"ATOM      1  N   ALA Q   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA Q   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA Q   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA E   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA E   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA E   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   GLU E   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  GLU E   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   GLU E   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   GLU E   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  GLU E   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  GLU E   1       0.246   1.278   2.204  1.0   0.00"
,"ATOM      1  CD  GLU E   1       0.790   1.240   3.618  1.0   0.00"
,"ATOM      1  OE1 GLU E   1       1.224   0.155   4.060  1.0   0.00"
,"ATOM      1  OE2 GLU E   1       0.784   2.296   4.285  1.0   0.00"
,"ATOM      1  H   GLU E   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  GLU E   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 GLU E   1       0.099  -0.850   1.977  1.0   0.00"
,"ATOM      1  HB3 GLU E   1       1.624  -0.161   1.406  1.0   0.00"
,"ATOM      1  HG2 GLU E   1       0.685   2.132   1.668  1.0   0.00"
,"ATOM      1  HG3 GLU E   1      -0.841   1.442   2.238  1.0   0.00"
,"ATOM      1  N   ALA E   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA E   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA E   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA G   0      -3.648   0.915  -0.467  1.00  0.00"
,"ATOM      1  C   ALA G   0      -2.128   1.019  -0.520  1.00  0.00"
,"ATOM      1  O   ALA G   0      -1.580   2.001  -1.021  1.00  0.00"
,"ATOM      1  N   GLY G   1      -1.451   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  GLY G   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   GLY G   1       0.580  -1.175  -0.762  1.00  0.00"
,"ATOM      1  O   GLY G   1       0.073  -2.293  -0.678  1.00  0.00"
,"ATOM      1  H   GLY G   1      -1.872  -0.808   0.412  1.00  0.00"
,"ATOM      1  HA2 GLY G   1       0.363   0.938  -0.446  1.00  0.00"
,"ATOM      1  HA3 GLY G   1       0.364  -0.025   1.038  1.00  0.00"
,"ATOM      1  N   ALA G   2       1.614  -0.958  -1.536  1.00  0.00"
,"ATOM      1  CA  ALA G   2       2.237  -2.013  -2.327  1.00  0.00"
,"ATOM      1  H   ALA G   2       2.047  -0.063  -1.640  1.00  0.00"
,"ATOM      1  CA  ALA H   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA H   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA H   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   HIS H   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  HIS H   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   HIS H   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   HIS H   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  HIS H   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  HIS H   1       0.205   1.241   2.202  1.0   0.00"
,"ATOM      1  ND1 HIS H   1       0.775   2.464   1.923  1.0   0.00"
,"ATOM      1  CD2 HIS H   1      -0.637   1.449   3.241  1.0   0.00"
,"ATOM      1  CE1 HIS H   1       0.298   3.370   2.757  1.0   0.00"
,"ATOM      1  NE2 HIS H   1      -0.560   2.780   3.566  1.0   0.00"
,"ATOM      1  H   HIS H   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  HIS H   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 HIS H   1       0.128  -0.871   1.967  1.0   0.00"
,"ATOM      1  HB3 HIS H   1       1.629  -0.123   1.405  1.0   0.00"
,"ATOM      1  HD1 HIS H   1       1.445   2.638   1.200  1.0   0.00"
,"ATOM      1  HD2 HIS H   1      -1.265   0.692   3.733  1.0   0.00"
,"ATOM      1  HE1 HIS H   1       0.571   4.436   2.775  1.0   0.00"
,"ATOM      1  HE2 HIS H   1      -1.072   3.231   4.297  1.0   0.00"
,"ATOM      1  N   ALA H   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA H   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA H   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA I   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA I   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA I   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   ILE I   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  ILE I   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   ILE I   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   ILE I   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  ILE I   1       0.565  -0.015   1.433  1.0   0.00"
,"ATOM      1  CG1 ILE I   1       0.196   1.277   2.165  1.0   0.00"
,"ATOM      1  CD1 ILE I   1       0.502   1.248   3.646  1.0   0.00"
,"ATOM      1  CG2 ILE I   1       2.081  -0.139   1.407  1.0   0.00"
,"ATOM      1  H   ILE I   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  ILE I   1       0.318   0.927  -0.499  1.00  0.00"
,"ATOM      1  HB  ILE I   1       0.129  -0.878   1.957  1.0   0.00"
,"ATOM      1 HG12 ILE I   1       0.739   2.116   1.705  1.0   0.00"
,"ATOM      1 HG13 ILE I   1      -0.877   1.473   2.025  1.0   0.00"
,"ATOM      1 HD11 ILE I   1       0.211   2.208   4.100  1.0   0.00"
,"ATOM      1 HD12 ILE I   1      -0.061   0.433   4.122  1.0   0.00"
,"ATOM      1 HD13 ILE I   1       1.580   1.086   3.795  1.0   0.00"
,"ATOM      1 HG21 ILE I   1       2.466  -0.148   2.437  1.0   0.00"
,"ATOM      1 HG22 ILE I   1       2.363  -1.075   0.902  1.0   0.00"
,"ATOM      1 HG23 ILE I   1       2.509   0.715   0.863  1.0   0.00"
,"ATOM      1  N   ALA I   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA I   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA I   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA L   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA L   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA L   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   LEU L   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  LEU L   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   LEU L   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   LEU L   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  LEU L   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  LEU L   1       0.230   1.243   2.271  1.0   0.00"
,"ATOM      1  CD1 LEU L   1       0.711   1.057   3.702  1.0   0.00"
,"ATOM      1  CD2 LEU L   1       0.923   2.466   1.692  1.0   0.00"
,"ATOM      1  H   LEU L   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  LEU L   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 LEU L   1       0.126  -0.877   1.955  1.0   0.00"
,"ATOM      1  HB3 LEU L   1       1.627  -0.130   1.394  1.0   0.00"
,"ATOM      1  HG  LEU L   1      -0.860   1.390   2.258  1.0   0.00"
,"ATOM      1 HD11 LEU L   1       0.481   1.961   4.286  1.0   0.00"
,"ATOM      1 HD12 LEU L   1       0.201   0.192   4.151  1.0   0.00"
,"ATOM      1 HD13 LEU L   1       1.797   0.885   3.705  1.0   0.00"
,"ATOM      1 HD21 LEU L   1       0.691   3.346   2.308  1.0   0.00"
,"ATOM      1 HD22 LEU L   1       2.010   2.303   1.683  1.0   0.00"
,"ATOM      1 HD23 LEU L   1       0.570   2.635   0.663  1.0   0.00"
,"ATOM      1  N   ALA L   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA L   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA L   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA K   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA K   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA K   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   LYS K   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  LYS K   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   LYS K   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   LYS K   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  LYS K   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  LYS K   1       0.246   1.277   2.204  1.0   0.00"
,"ATOM      1  CD  LYS K   1       0.805   1.209   3.616  1.0   0.00"
,"ATOM      1  CE  LYS K   1       0.516   2.486   4.387  1.0   0.00"
,"ATOM      1  NZ  LYS K   1       1.058   2.433   5.773  1.0   0.00"
,"ATOM      1  H   LYS K   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  LYS K   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 LYS K   1       0.100  -0.851   1.976  1.0   0.00"
,"ATOM      1  HB3 LYS K   1       1.624  -0.160   1.406  1.0   0.00"
,"ATOM      1  HG2 LYS K   1       0.684   2.136   1.675  1.0   0.00"
,"ATOM      1  HG3 LYS K   1      -0.840   1.445   2.245  1.0   0.00"
,"ATOM      1  HD2 LYS K   1       0.368   0.350   4.144  1.0   0.00"
,"ATOM      1  HD3 LYS K   1       1.892   1.041   3.574  1.0   0.00"
,"ATOM      1  HE2 LYS K   1       0.952   3.343   3.855  1.0   0.00"
,"ATOM      1  HE3 LYS K   1      -0.572   2.652   4.426  1.0   0.00"
,"ATOM      1  HZ1 LYS K   1       0.848   3.288   6.247  1.0   0.00"
,"ATOM      1  HZ2 LYS K   1       0.642   1.668   6.265  1.0   0.00"
,"ATOM      1  HZ3 LYS K   1       2.049   2.306   5.738  1.0   0.00"
,"ATOM      1  N   ALA K   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA K   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA K   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA M   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA M   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA M   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   MET M   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  MET M   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   MET M   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   MET M   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  MET M   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  MET M   1       0.205   1.258   2.219  1.0   0.00"
,"ATOM      1  SD  MET M   1       0.993   2.730   1.544  1.0   0.00"
,"ATOM      1  CE  MET M   1       2.708   2.414   1.953  1.0   0.00"
,"ATOM      1  H   MET M   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  MET M   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 MET M   1       0.127  -0.871   1.966  1.0   0.00"
,"ATOM      1  HB3 MET M   1       1.629  -0.125   1.404  1.0   0.00"
,"ATOM      1  HG2 MET M   1      -0.886   1.402   2.228  1.0   0.00"
,"ATOM      1  HG3 MET M   1       0.521   1.125   3.265  1.0   0.00"
,"ATOM      1  HE1 MET M   1       3.331   3.247   1.596  1.0   0.00"
,"ATOM      1  HE2 MET M   1       2.814   2.320   3.044  1.0   0.00"
,"ATOM      1  HE3 MET M   1       3.035   1.481   1.471  1.0   0.00"
,"ATOM      1  N   ALA M   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA M   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA M   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA M   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA M   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA M   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   MSE M   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  MSE M   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   MSE M   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   MSE M   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  MSE M   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  MSE M   1       0.205   1.258   2.219  1.0   0.00"
,"ATOM      1 SED  MSE M   1       1.047   2.831   1.498  1.0   0.00"
,"ATOM      1  CE  MSE M   1       2.865   2.414   1.975  1.0   0.00"
,"ATOM      1  H   MSE M   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  MSE M   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 MSE M   1       0.127  -0.871   1.966  1.0   0.00"
,"ATOM      1  HB3 MSE M   1       1.629  -0.125   1.404  1.0   0.00"
,"ATOM      1  HG2 MSE M   1      -0.886   1.402   2.228  1.0   0.00"
,"ATOM      1  HG3 MSE M   1       0.521   1.125   3.265  1.0   0.00"
,"ATOM      1  HE1 MSE M   1       3.529   3.222   1.637  1.0   0.00"
,"ATOM      1  HE2 MSE M   1       2.943   2.309   3.067  1.0   0.00"
,"ATOM      1  HE3 MSE M   1       3.163   1.470   1.495  1.0   0.00"
,"ATOM      1  N   ALA M   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA M   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA M   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA F   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA F   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA F   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   PHE F   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  PHE F   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   PHE F   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   PHE F   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  PHE F   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  PHE F   1       0.205   1.245   2.204  1.0   0.00"
,"ATOM      1  CD1 PHE F   1       1.005   2.370   2.106  1.0   0.00"
,"ATOM      1  CD2 PHE F   1      -0.908   1.294   3.025  1.0   0.00"
,"ATOM      1  CE1 PHE F   1       0.702   3.517   2.816  1.0   0.00"
,"ATOM      1  CE2 PHE F   1      -1.215   2.439   3.736  1.0   0.00"
,"ATOM      1  CZ  PHE F   1      -0.409   3.551   3.631  1.0   0.00"
,"ATOM      1  H   PHE F   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  PHE F   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 PHE F   1       0.127  -0.871   1.967  1.0   0.00"
,"ATOM      1  HB3 PHE F   1       1.629  -0.124   1.405  1.0   0.00"
,"ATOM      1  HD1 PHE F   1       1.894   2.352   1.458  1.0   0.00"
,"ATOM      1  HD2 PHE F   1      -1.556   0.410   3.113  1.0   0.00"
,"ATOM      1  HE1 PHE F   1       1.347   4.405   2.729  1.0   0.00"
,"ATOM      1  HE2 PHE F   1      -2.102   2.462   4.386  1.0   0.00"
,"ATOM      1  HZ  PHE F   1      -0.653   4.464   4.195  1.0   0.00"
,"ATOM      1  N   ALA F   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA F   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA F   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA P   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA P   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA P   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   PRO P   1      -1.466   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  PRO P   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   PRO P   1       0.566  -1.218  -0.723  1.00  0.00"
,"ATOM      1  O   PRO P   1      -0.181  -2.059  -1.222  1.00  0.00"
,"ATOM      1  CB  PRO P   1       0.344  -0.001   1.491  1.0   0.00"
,"ATOM      1  CG  PRO P   1      -0.847  -0.666   2.144  1.0   0.00"
,"ATOM      1  CD  PRO P   1      -2.010  -0.194   1.291  1.0   0.00"
,"ATOM      1  NE  PRO P   1      -1.384   0.013  -0.026  1.0   0.00"
,"ATOM      1  HA  PRO P   1       0.434   0.857  -0.537  1.00  0.00"
,"ATOM      1  HB2 PRO P   1       1.273  -0.555   1.690  1.0   0.00"
,"ATOM      1  HB3 PRO P   1       0.492   1.021   1.870  1.0   0.00"
,"ATOM      1  HG2 PRO P   1      -0.755  -1.762   2.138  1.0   0.00"
,"ATOM      1  HG3 PRO P   1      -0.960  -0.359   3.194  1.0   0.00"
,"ATOM      1  HD2 PRO P   1      -2.815  -0.942   1.247  1.0   0.00"
,"ATOM      1  HD3 PRO P   1      -2.454   0.734   1.682  1.0   0.00"
,"ATOM      1  N   ALA P   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA P   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA P   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA S   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA S   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA S   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   SER S   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  SER S   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   SER S   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   SER S   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  SER S   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  OG  SER S   1       0.133  -1.167   2.128  1.0   0.00"
,"ATOM      1  H   SER S   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  SER S   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 SER S   1       0.172   0.892   1.964  1.0   0.00"
,"ATOM      1  HB3 SER S   1       1.634   0.061   1.418  1.0   0.00"
,"ATOM      1  HG  SER S   1       0.498  -1.142   3.059  1.0   0.00"
,"ATOM      1  N   ALA S   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA S   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA S   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA T   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA T   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA T   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   THR T   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  THR T   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   THR T   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   THR T   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  THR T   1       0.565  -0.015   1.433  1.0   0.00"
,"ATOM      1  OG1 THR T   1       0.152  -1.214   2.100  1.0   0.00"
,"ATOM      1  CG2 THR T   1       0.058   1.185   2.218  1.0   0.00"
,"ATOM      1  H   THR T   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  THR T   1       0.318   0.927  -0.499  1.00  0.00"
,"ATOM      1  HB  THR T   1       1.663   0.026   1.376  1.0   0.00"
,"ATOM      1  HG1 THR T   1       0.521  -1.224   3.030  1.0   0.00"
,"ATOM      1 HG21 THR T   1       0.471   1.158   3.237  1.0   0.00"
,"ATOM      1 HG22 THR T   1       0.377   2.112   1.718  1.0   0.00"
,"ATOM      1 HG23 THR T   1      -1.039   1.154   2.267  1.0   0.00"
,"ATOM      1  N   ALA T   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA T   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA T   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  C   ALA W   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA W   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   TRP W   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  TRP W   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   TRP W   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   TRP W   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  TRP W   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  TRP W   1       0.203   1.244   2.198  1.0   0.00"
,"ATOM      1  CD1 TRP W   1      -0.857   1.429   3.038  1.0   0.00"
,"ATOM      1  CD2 TRP W   1       0.936   2.476   2.193  1.0   0.00"
,"ATOM      1  NE1 TRP W   1      -0.834   2.701   3.557  1.0   0.00"
,"ATOM      1  CE2 TRP W   1       0.261   3.363   3.053  1.0   0.00"
,"ATOM      1  CE3 TRP W   1       2.095   2.910   1.545  1.0   0.00"
,"ATOM      1  CZ2 TRP W   1       0.705   4.664   3.283  1.0   0.00"
,"ATOM      1  CZ3 TRP W   1       2.536   4.200   1.772  1.0   0.00"
,"ATOM      1  CH2 TRP W   1       1.845   5.067   2.636  1.0   0.00"
,"ATOM      1  H   TRP W   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  TRP W   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 TRP W   1       0.128  -0.870   1.968  1.0   0.00"
,"ATOM      1  HB3 TRP W   1       1.629  -0.122   1.406  1.0   0.00"
,"ATOM      1  HD1 TRP W   1      -1.620   0.671   3.265  1.0   0.00"
,"ATOM      1  HE1 TRP W   1      -1.501   3.082   4.197  1.0   0.00"
,"ATOM      1  HE3 TRP W   1       2.645   2.239   0.867  1.0   0.00"
,"ATOM      1  HZ2 TRP W   1       0.164   5.342   3.958  1.0   0.00"
,"ATOM      1  HZ3 TRP W   1       3.446   4.555   1.268  1.0   0.00"
,"ATOM      1  HH2 TRP W   1       2.225   6.086   2.794  1.0   0.00"
,"ATOM      1  N   ALA W   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA W   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA W   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA Y   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA Y   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA Y   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   TYR Y   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  TYR Y   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   TYR Y   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   TYR Y   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  TYR Y   1       0.536  -0.000   1.433  1.0   0.00"
,"ATOM      1  CG  TYR Y   1       0.203   1.253   2.212  1.0   0.00"
,"ATOM      1  CD1 TYR Y   1       1.005   2.383   2.118  1.0   0.00"
,"ATOM      1  CD2 TYR Y   1      -0.913   1.302   3.038  1.0   0.00"
,"ATOM      1  CE1 TYR Y   1       0.709   3.531   2.825  1.0   0.00"
,"ATOM      1  CE2 TYR Y   1      -1.225   2.441   3.752  1.0   0.00"
,"ATOM      1  CZ  TYR Y   1      -0.413   3.555   3.645  1.0   0.00"
,"ATOM      1  OH  TYR Y   1      -0.716   4.696   4.353  1.0   0.00"
,"ATOM      1  H   TYR Y   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  TYR Y   1       0.336   0.917  -0.507  1.00  0.00"
,"ATOM      1  HB2 TYR Y   1       0.127  -0.871   1.967  1.0   0.00"
,"ATOM      1  HB3 TYR Y   1       1.629  -0.124   1.405  1.0   0.00"
,"ATOM      1  HD1 TYR Y   1       1.892   2.363   1.469  1.0   0.00"
,"ATOM      1  HD2 TYR Y   1      -1.559   0.416   3.124  1.0   0.00"
,"ATOM      1  HE1 TYR Y   1       1.355   4.418   2.741  1.0   0.00"
,"ATOM      1  HE2 TYR Y   1      -2.112   2.462   4.403  1.0   0.00"
,"ATOM      1  HH  TYR Y   1      -1.547   4.545   4.887  1.0   0.00"
,"ATOM      1  N   ALA Y   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA Y   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA Y   2       2.055  -0.074  -1.617  1.00  0.00"
,"ATOM      1  CA  ALA V   0      -3.674   0.874  -0.453  1.00  0.00"
,"ATOM      1  C   ALA V   0      -2.156   1.004  -0.521  1.00  0.00"
,"ATOM      1  O   ALA V   0      -1.630   1.992  -1.033  1.00  0.00"
,"ATOM      1  N   VAL V   1      -1.458   0.000   0.000  1.00  0.00"
,"ATOM      1  CA  VAL V   1       0.000   0.000   0.000  1.00  0.00"
,"ATOM      1  C   VAL V   1       0.551  -1.198  -0.766  1.00  0.00"
,"ATOM      1  O   VAL V   1       0.024  -2.306  -0.667  1.00  0.00"
,"ATOM      1  CB  VAL V   1       0.565  -0.015   1.433  1.0   0.00"
,"ATOM      1  CG1 VAL V   1       2.081  -0.139   1.407  1.0   0.00"
,"ATOM      1  CG2 VAL V   1       0.200   1.269   2.162  1.0   0.00"
,"ATOM      1  H   VAL V   1      -1.871  -0.808   0.419  1.00  0.00"
,"ATOM      1  HA  VAL V   1       0.318   0.927  -0.499  1.00  0.00"
,"ATOM      1  HB  VAL V   1       0.128  -0.879   1.956  1.0   0.00"
,"ATOM      1 HG11 VAL V   1       2.466  -0.148   2.437  1.0   0.00"
,"ATOM      1 HG12 VAL V   1       2.363  -1.075   0.902  1.0   0.00"
,"ATOM      1 HG13 VAL V   1       2.509   0.715   0.863  1.0   0.00"
,"ATOM      1 HG21 VAL V   1       0.611   1.241   3.183  1.0   0.00"
,"ATOM      1 HG22 VAL V   1       0.619   2.130   1.621  1.0   0.00"
,"ATOM      1 HG23 VAL V   1      -0.895   1.366   2.210  1.0   0.00"
,"ATOM      1  N   ALA V   2       1.615  -0.968  -1.528  1.00  0.00"
,"ATOM      1  CA  ALA V   2       2.239  -2.027  -2.312  1.00  0.00"
,"ATOM      1  H   ALA V   2       2.055  -0.074  -1.617  1.00  0.00"
,"END"
,"                                                                                     "};
/*pad out end so read to j==79 will not run off end of the array! */

   if(Lnew) Nth = 1;
   else if(Nin > 0) Nth = Nin; /*040303*/
   else Nth++;

   for(j=0; j<80; j++)
   {
      temps[j] =  AALIB[Nth][j];
      if(temps[j] == '\0') break;
   }
   temps[j+1] = '\0'; 
   if(temps[0] == 'E' && temps[1] == 'N' && temps[2] == 'D') ireturn = 0;
   else ireturn = Nth; /*040303 return index into library instead of just 1 */
   return(ireturn);
}
/*___getnewlibraryline()_____________________________________________________*/

/****getonekinemageline()*****************************************************/
int getonekinemageline(int Lnew)
{
int j=0,ireturn=0;
static int Nth=0;

static char*  KINEMAGELIB[] = {""
,"@@asn"
,"@group {rotamers} "
,"@subgroup {std} dominant"
,"@vectorlist {sc} color= cyan "
,"{ ca  asn   1 } P 0.000, 0.000, 0.000 "
,"{ cb  asn   1 } L 0.536, 0.000, 1.433"
,"{ cg  asn   1 } L 0.085, 1.212, 2.224"
,"{ od1 asn   1 } L -0.030, 2.311, 1.682"
,"{ cg  asn   1 } P 0.085, 1.212, 2.224 "
,"{ nd2 asn   1 } L -0.174, 1.014, 3.511"
,"@vectorlist {sc H} color= gray master= {merH}"
,"{ cb  asn   1 } P 0.536, 0.000, 1.433 {1hb  asn   1 } L 0.201, -0.913, 1.946"
,"{ cb  asn   1 } P 0.536, 0.000, 1.433 {2hb  asn   1 } L 1.635, -0.029, 1.409"
,"{ nd2 asn   1 } P -0.174, 1.014, 3.511 {1hd2 asn   1 } L -0.477, 1.776, 4.083"
,"{ nd2 asn   1 } P -0.174, 1.014, 3.511 {2hd2 asn   1 } L -0.066, 0.103, 3.908"
,"@@asp"
,"@group {rotamers} "
,"@subgroup {std} dominant"
,"@vectorlist {sc} color= cyan  "
,"{ ca  asp   1 } P 0.000, 0.000, 0.000 "
,"{ cb  asp   1 } L 0.536, 0.000, 1.433"
,"{ cg  asp   1 } L 0.085, 1.212, 2.224"
,"{ od1 asp   1 } L -0.217, 2.251, 1.599"
,"{ cg  asp   1 } P 0.085, 1.212, 2.224 "
,"{ od2 asp   1 } L 0.032, 1.123, 3.469"
,"@vectorlist {sc H} color= gray master= {merH}"
,"{ cb  asp   1 } P 0.536, 0.000, 1.433 {1hb  asp   1 } L 0.201, -0.913, 1.946"
,"{ cb  asp   1 } P 0.536, 0.000, 1.433 {2hb  asp   1 } L 1.635, -0.029, 1.409"
,"@@end"
,"END"
,"                                                                              "
,"                                                                              "
,"                                                                              "
,"                                                                              "};
/*pad out end records so read to j==255 will not run off end of the array! */
   if(Lnew) Nth = 1;
   else Nth++;

   for(j=0; j<256; j++)
   {
      temps[j] =  KINEMAGELIB[Nth][j];
      if(temps[j] == '\0') break;
   }
   temps[j+1] = '\0'; 
   if(temps[0] == 'E' && temps[1] == 'N' && temps[2] == 'D') ireturn = 0;
   else ireturn = 1;
   return(ireturn);
}
/*___getonekinemageline()____________________________________________________*/

/****getonerotamerline()*****************************************************/
int getonerotamerline(int Lnew)
{
int j=0,ireturn=0;
static int Nth=0;

static char*  ROTAMERLIB[] = {""
,"@@arg 4"
,"ptp85i  <1%:  62  180   65   85"
,"ptp180i  1%:  62  180   65 -175"
,"ptt85i   2%:  62  180  180   85"
,"ptt180i  2%:  62  180  180  180"
,"ptt-85i  2%:  62  180  180  -85 "
,"ptm180i  1%:  62  180  -65  175  "
,"ptm-85i  1%:  62  180  -65  -85"
,"tpp85i   1%:-177   65   65   85"
,"tpp180i  1%:-177   65   65 -175"
,"tpt85i   2%:-177   65  180   85"
,"tpt180i  2%:-177   65  180  180"
,"ttp85i   4%:-177  180   65   85"
,"ttp180i  3%:-177  180   65 -175"
,"ttp-105i 1%:-177  180   65 -105"
,"ttt85i   2%:-177  180  180   85"
,"ttt180i  4%:-177  180  180  180"
,"ttt-85i  3%:-177  180  180  -85"
,"ttm105i  1%:-177  180  -65  105"
,"ttm180i  1%:-177  180  -65  175"
,"ttm-85i  3%:-177  180  -65  -85"
,"mtp85i   2%: -67  180   65   85"
,"mtp180i  5%: -67  180   65 -175"
,"mtp-105i 1%: -67  180   65 -105"
,"mtt85i   4%: -67  180  180   85"
,"mtt180i  9%: -67  180  180  180"
,"mtt-85i  6%: -67  180  180  -85"
,"mtm105i  2%: -67  180  -65  105"
,"mtm180i  5%: -67  180  -65  175"
,"mtm-85i  6%: -67 -167  -65  -85"
,"mmt85i   1%: -62  -68  180   85"
,"mmt180i  2%: -62  -68  180  180"
,"mmt-85i  2%: -62  -68  180  -85"
,"mmm180i  1%: -62  -68  -65  175"
,"mmm-85i  2%: -62  -68  -65  -85"
,"@@lys 4"
,"ptpt  1%:  62  180   68  180"
,"pttp  1%:  62  180  180   65"
,"pttt  2%:  62  180  180  180"
,"pttm  1%:  62  180  180  -65"
,"ptmt <1%:  62  180  -68  180"
,"tptp  1%:-177   68  180   65"
,"tptt  3%:-177   68  180  180"
,"tptm  1%:-177   68  180  -65"
,"ttpp  1%:-177  180   68   65"
,"ttpt  2%:-177  180   68  180"
,"tttp  4%:-177  180  180   65"
,"tttt 13%:-177  180  180  180"
,"tttm  3%:-177  180  180  -65"
,"ttmt  2%:-177  180  -68  180"
,"ttmm <1%:-177  180  -68  -65"
,"mptt <1%: -90   68  180  180"
,"mtpp  1%: -67  180   68   65"
,"mtpt  3%: -67  180   68  180"
,"mttp  3%: -67  180  180   65"
,"mttt 20%: -67  180  180  180"
,"mttm  5%: -67  180  180  -65"
,"mtmt  3%: -67  180  -68  180"
,"mtmm  1%: -67  180  -68  -65"
,"mmtp  1%: -62  -68  180   65"
,"mmtt  6%: -62  -68  180  180"
,"mmtm  1%: -62  -68  180  -65"
,"mmmt  1%: -62  -68  -68  180"
,"@@met 3"
,"ptp   2%:  62  180   75"
,"ptm   3%:  62  180  -75"
,"tpp   5%:-177   65   75"
,"tpt   2%:-177   65  180"
,"ttp   5%:-177  180   75"
,"ttt   3%:-177  180  180"
,"ttm   7%:-177  180  -75"
,"mtp  17%: -67  180   75"
,"mtt   8%: -67  180  180"
,"mtm  11%: -67  180  -75"
,"mmp   3%: -65  -65  103"
,"mmt   2%: -65  -65  180"
,"mmm  19%: -65  -65  -70"
,"@@mse 3"
,"ptp   2%:  62  180   75"
,"ptm   3%:  62  180  -75"
,"tpp   5%:-177   65   75"
,"tpt   2%:-177   65  180"
,"ttp   5%:-177  180   75"
,"ttt   3%:-177  180  180"
,"ttm   7%:-177  180  -75"
,"mtp  17%: -67  180   75"
,"mtt   8%: -67  180  180"
,"mtm  11%: -67  180  -75"
,"mmp   3%: -65  -65  103"
,"mmt   2%: -65  -65  180"
,"mmm  19%: -65  -65  -70"
,"@@glu 3"
,"Spt-60i   :  62  180  -60"
,"pt-20i  5%:  62  180  -20"
,"Spt60i    :  62  180   60"
,"pm0i    2%:  70  -80  0"
,"tp10i   6%:-177   65   10"
,"Stt-60i   :-177  180  -60"
,"tt 0i  24%:-177  180  0"
,"Stt60i    :-177  180   60"
,"tm-20i  1%:-177  -80  -25"
,"mp0i    6%: -65   85  0   "
,"Smt-60i   : -67  180  -60"
,"mt-10i 33%: -67  180  -10"
,"Smt60i    : -67  180   60"
,"mm-40i 13%: -65  -65  -40"
,"Smm0i     : -65  -75  0"
,"@@gln 3"
,"Spt-60i   :  62  180  -60"
,"pt 20i  4%:  62  180   20"
,"Spt60i    :  62  180   60"
,"pm0i    2%:  70  -75  0"
,"tp-100i 2%:-177   65 -100"
,"tp60i   9%:-177   65   60"
,"Stt-60i   :-177  180  -60"
,"tt 0 i 16%:-177  180  0"
,"Stt60i    :-177  180   60"
,"mp0i    3%: -65   85  0"
,"Smt-60i   : -67  180  -60"
,"mt -30i35%: -67  180  -25"
,"Smt60i    : -67  180   60"
,"mm-40i 15%: -65  -65  -40"
,"mm100i  3%: -65  -65  100"
,"@@asp 2"
,"Sp-50i    :  62  -50"
,"p-10i  10%:  62  -10"
,"p30i    9%:  62   30"
,"St-30i    :-170  -30"
,"t0i    21%:-177  0"
,"t70i    6%:-177   65"
,"m -20i 51%: -70  -15"
,"Sm-60i    : -65  -60"
,"@@asn 2"
,"Sp-50i    :  62  -50"
,"p-10i   7%:  62  -10"
,"p30i    9%:  62   30"
,"St-80i    :-174  -80"
,"t-20i  12%:-174  -20"
,"t30i   15%:-177   30"
,"m-20i  39%: -65  -20"
,"m-80i   8%: -65  -75"
,"m120i   4%: -65  120"
,"@@ile 2"
,"pp    1%:  62  100"
,"pt   13%:  62  170"
,"tp    2%:-177   66"
,"tt    8%:-177  165"
,"mp    1%: -65  100"
,"mt   60%: -65  170"
,"mm   15%: -57  -60"
,"@@leu 2"
,"pp    1%:  62   80"
,"tp   29%:-177   65"
,"tt    2%:-172  145"
,"mp    2%: -85   65"
,"mt   59%: -65  175"
,"@@his 2"
,"p-80i  9%:  62  -75"
,"p80i   4%:  62   80"
,"t-160i 5%:-177 -165"
,"t-80i 11%:-177  -80"
,"t60i  16%:-177   60"
,"m-70i 29%: -65  -70"
,"m170i  7%: -65  165"
,"m80i  13%: -65   80"
,"@@trp 2"
,"p-90i  11%:  62  -90"
,"p90i    6%:  62   90"
,"t-105i 16%:-177 -105"
,"t90i   18%:-177   90"
,"m-90i   5%: -65  -90"
,"m0i     8%: -65   -5"
,"m95i   32%: -65   95"
,"@@tyr 2"
,"p90i  13%:  62   90"
,"t80i  34%:-177   80"
,"m-85i 43%: -65  -85"
,"m -30i 9%: -65  -30"
,"Sm35i    : -85   30"
,"@@phe 2"
,"p90i  13%:  62   90"
,"t80i  33%:-177   80"
,"m-85i 44%: -65  -85"
,"m -30i 9%: -65  -30"
,"Sm35i    : -85   30"
,"@@pro 3"
,"Cg-endo 44%: 27.6 -35.8  29.3"
,"Cg-exo  43%:-27.9  39.0 -34.3"
,"@@thr 1"
,"p 49%:  62"
,"t  7%:-175"
,"m 43%: -65"
,"@@val 1"
,"p  6%:  63"
,"t 73%: 175"
,"m 20%: -60"
,"@@ser 1"
,"p 48%:  62"
,"t 22%:-177"
,"m 29%: -65"
,"@@cys 1"
,"p 23%:  62"
,"t 26%:-177"
,"m 50%: -65"
,"@@end 0"
,"END"
,"                                                                              "
,"                                                                              "
,"                                                                              "
,"                                                                              "};
/*pad out end records so read to j==255 will not run off end of the array! */
   if(Lnew) Nth = 1;
   else Nth++;

   for(j=0; j<256; j++)
   {
      temps[j] =  ROTAMERLIB[Nth][j];
      if(temps[j] == '\0') break;
   }
   temps[j+1] = '\0'; 
   if(temps[0] == 'E' && temps[1] == 'N' && temps[2] == 'D') ireturn = 0;
   else ireturn = 1;
   return(ireturn);
}
/*___getonerotamerline()____________________________________________________*/

