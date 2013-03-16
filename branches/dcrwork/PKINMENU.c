                      /*PKINMENU.c*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****menuabout()**************************************************************/
void menuabout(void)
{
   aboutprekin();
   DoReportDialog(); 
}
/*___menuabout()_____________________________________________________________*/

/****aboutlaunching()*********************************************************/
void aboutlaunching(void)
{
   sprintf(alertstr,
           CRLF"Launch current kinemage will ask the system to launch MAGE"
           CRLF"and show whatever kinemage file this PREKIN run last made."
           );
   sprintf(alertstr2," ");
   sprintf(alertstr3," ");
   DoReportDialog();
}
/*___aboutlaunching()________________________________________________________*/

/****aboutoptions()***********************************************************/
void aboutoptions(void)
{;}
/*___aboutoptions()__________________________________________________________*/

/****menuaboutmutations()*****************************************************/
int  menuaboutmutations(int it)
{
   int irtn=0;

   sprintf(alertstr,
     "PREKIN 2.0+ :mutation & CHI rotations:"
      CRLF"range with nn1=nn2 & only a 3-letter residue code."
      CRLF"residue is always mutated/substituted to standard geometry."
      CRLF"Commandline: -mutate # RES :forces mutation to standard geometry."
      CRLF"aa standard geometry is compiled into this version of prekin"
      CRLF"MAGE 2.0+ does nested rotations around given P,L vectors."
     );
   if(it == 0)
   {
      DoReportDialog();
   }
   else
   {
      sprintf(word,"mutations and rotations");
      if     (it==1) {sprintf(temps,"%s",alertstr ); irtn=1;}
      else if(it==2) {sprintf(temps,"%s",alertstr2); irtn=1;}
      else if(it==3) {sprintf(temps,"%s",alertstr3); irtn=1;}
      else           {sprintf(temps," ");            irtn=0;}
   }

   return(irtn);
}
/*___menuaboutmutations()____________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
