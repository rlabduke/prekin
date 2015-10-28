                      /*PUXMTEXT.c*/
#include "PKIN.h"
#include "PKINDLOG.h"

#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
/*varible needed between inserttext and replacetext*/
static XmTextPosition position;
static long           laststrlen;
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****cleartexts()*************************************************************/
void    cleartexts() 
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
    XmTextReplace(textwindow,
                   0,
                   XmTextGetLastPosition(textwindow),
                   " ");
    /*reset text position*/
    position = XmTextGetLastPosition(textwindow);
    maxtextchar = 0;
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___cleartexts()____________________________________________________________*/

/****inserttexts()************************************************************/
void    inserttexts(char texts[256], int dummy) 
{
  dummy = dummy; /*PC uses 0 insert, 1 replace*/
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
        if(!Limittext)
        {
            laststrlen = (long)strlen(texts);
            maxtextchar = maxtextchar + laststrlen;
            if(maxtextchar >= 32767L)
            {
                Limittext = 1;
                printf("Text about to exceed 32767 characters, \n"
                    "   no more text will be put into Text Window!\n");
            }
            else
            {
                position = XmTextGetLastPosition(textwindow);
                XmTextInsert(textwindow, position, texts);
                /*advance position*/
                position = XmTextGetLastPosition(textwindow);
                /*set laststrlength=0 as will not overwrite this new text*/
                laststrlen = 0;
            }
        }
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___inserttexts()___________________________________________________________*/

/****pkintextinsert()********************************************************/
void pkintextinsert(char texts[256])
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)
  {
     inserttexts(texts,0);
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___pkintextinsert()________________________________________________________*/

/****pkintextreplace()********************************************************/
void pkintextreplace(char texts[256])
{/*does this from the last position set by inserttexts*/
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
        if(!Limittext)
        {
            maxtextchar = maxtextchar - laststrlen + (long)strlen(texts);
            if(maxtextchar >= 32767L)
            {
                Limittext = 1;
                printf("Text about to exceed 32767 characters, \n"
                    "   no more text will be put into Text Window!\n");
            }
            else
            {
                laststrlen = (long)strlen(texts);/*update*/
                XmTextReplace(textwindow,
                   position,
                   XmTextGetLastPosition(textwindow),
                   texts);
                /*position is end of last inserttext*/
            }
        }
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___pkintextreplace()________________________________________________________*/

/****completetext()*********************************************************/
void    completetext() 
{
    ;
}
/*___completetext()________________________________________________________*/

/****adjusttext()*************************************************************/
void adjusttext(int iadjust)
{
#ifdef UNIX_MOTIF  /*060324 re compile with NOGUI, requires Lquiet=1*/
  if(Lquiet==0)  /*was Lcommanded before 990402*/
  {
     XmTextScroll(textwindow,iadjust);
  }
#endif /* UNIX_MOTIF 060324 re compile with NOGUI, requires Lquiet=1*/
 /*this should NOT do any scrolling unless reach bottom of text window*/
 /*so needs to check height and lines available, etc... */
}
/*___adjusttext()____________________________________________________________*/

/****getmactextselect()******************************************************/
void getmactextselect(void)
{
    ;
}
/*___getmactextselect()_____________________________________________________*/

                      /*PUXMTEXT.c*/
/****cleanuptextwindow()*****************************************************/
void cleanuptextwindow()
{
   ;
}
/*___cleanuptextwindow()____________________________________________________*/


