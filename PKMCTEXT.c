/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"

/****pkintextinsert()******************************************************/
void pkintextinsert(char string[256])
{
 if (Lquiet == 0)
 {
    TEInsert(string,strlen(string), textText);
    mactextselect = (*textText)->selEnd; 
 }
}
/*___pkintextinsert()_____________________________________________________*/
/****inserttexts()******************************************************/
void inserttexts(char string[256], int replace)
{
 if (Lquiet == 0)
 {
    replace = replace;
    TEInsert(string,strlen(string), textText);
 }
}
/*___inserttexts()_____________________________________________________*/
/****pkintextreplace()******************************************************/
void pkintextreplace(char string[256])
{
    long newend;
 if (Lquiet == 0)
 {    
    newend = (*textText)->selEnd;
    TESetSelect(mactextselect,newend,textText);
    TEDelete(textText);
    TEInsert(string,strlen(string), textText);
 }
}
/*___pkintextreplace()_____________________________________________________*/
/****getmactextselect()******************************************************/
void getmactextselect(void)
{
 if (Lquiet == 0)
 {
    mactextselect = (*textText)->selEnd;
 }
}
/*___getmactextselect()_____________________________________________________*/

/****adjusttext()************************************************************/
void adjusttext(int iforce)
/*must call this with iforce==0 before iforce==1 to get calc right*/
{
 if (Lquiet == 0)
 {
    if (iforce > 0)
    {
        if(thenoldLines+iforce >= theviewLines)  /* would be off the bottom now*/
        {/*force a scroll if line would be off bottom from earlier calculation*/

/*m*/      TEScroll(0,-(iforce*textHeight),textText); 
               /* scroll iforce number of lines */
/*m*/      SetControlValue(vtextScroll,maxValue);

        }   
        thenoldLines = thenoldLines + iforce;
    }
    else /*calculate number of lines needed for a scroll to stay on screen*/
    {
/*m*/  TECalText(textText);
      thenLines = (*textText)->nLines;  /* total lines of text */
      thendiff = thenLines - thenoldLines; 
               /*number of lines moved since last adjustment*/
      theviewLines = (textdRect.bottom - textdRect.top)/textHeight; 
                /* lines viewed in window */
      thenoldLines = thenLines;

      if (thenLines >= (theviewLines)) 
      {
/*m*/      TEScroll(0,-(thendiff*textHeight),textText); /* scroll ndiff lines */
/*m*/      SetControlValue(vtextScroll,maxValue);
      }

    }
 }
}
/*___adjusttext()___________________________________________________________*/

/****cleanuptextwindow()*****************************************************/
void cleanuptextwindow()
{
 if (Lquiet == 0)
 {
   settextTextarea();
 }
}
/*___cleanuptextwindow()____________________________________________________*/