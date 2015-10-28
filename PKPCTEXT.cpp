#include "PKIN.H"
#include "PKINDLOG.h"

static long laststrlen;

/****getmactextselect()******************************************************/
void getmactextselect(void) {;}
/*___getmactextselect()_____________________________________________________*/

/****pkintextreplace()*******************************************************/
void pkintextreplace(char textstr[256])
{
   if (!Lquiet) /* only do this when run in GUI mode */
   {
      inserttexts(textstr,1); /* 1 tells inserttexts() to replace text */
      RedrawWindow(PrekinWindow,NULL,NULL,RDW_ERASE|RDW_UPDATENOW);
   }
}
/*___pkintextreplace()______________________________________________________*/

/****pkintextinsert()********************************************************/
void pkintextinsert(char textstr[256])
{
   if (!Lquiet) /* only do this when run in GUI mode */
   {
      inserttexts(textstr,0); /* 0 tells inserttexts() to add to text */
      InvalidateRect(PrekinWindow,NULL,TRUE);
   }
}
/*___pkintextinsert()_______________________________________________________*/

/****inserttexts()***********************************************************/
void inserttexts(char textstr[256], int replace)  /*<-GetTextFromFile(),MAGETEXT*/
{
   LPSTR textbuf;
   int j;
   long textend,maxtextchar;

   if (Lquiet == 0)
   {
      textbuf = (LPSTR)GetWindowLong(PrekinWindow,TEXTHANDLE);
      maxtextchar = GetWindowLong(PrekinWindow,TEXTLENGTH);

      if(!Limittext)
      {
         textend = maxtextchar;

         /* maxtextchar needs to be adjusted depending on if we */
         /* are inserting or replacing the text on the screen   */
         if (replace) 
            maxtextchar = maxtextchar - laststrlen + (long)strlen(textstr);
         else maxtextchar = maxtextchar + (long)strlen(textstr);

         if (maxtextchar >= 32767L)
         {
            Limittext = 1;
            sprintf(alertstr,"Text about to exceed 32767 characters,\n"
               "no more text will be put into Text Window!\n");
            DoReportDialog();
         }
         else
         {
            /* again, if replacing text adjust the ending */
            if (replace) textend = textend - laststrlen;

            laststrlen = (long)strlen(textstr);/*update*/
            for (j=0; textstr[j] != '\0'; j++)
            {
               textbuf[textend] = textstr[j];
               textend++;
            }
            textbuf[textend] = '\0';
         }
         maxtextchar = textend;
         SetWindowLong(PrekinWindow,TEXTLENGTH,maxtextchar); /*update actual length*/
      }
   }
}
/*___inserttexts()_________________________________________________________*/

/****adjusttext()************************************************************/
void adjusttext(int iforce)    /*see PKMCTEXT.C*/
/*must call this with iforce==0 before iforce==1 to get calc right*/
{
   SCROLLINFO si;
 if (Lquiet == 0)
 {

   makelinestarts((HWND)PrekinWindow);

   si.cbSize = sizeof(SCROLLINFO);
   si.fMask = SIF_ALL;
   si.nPage = linesonascreen(PrekinWindow);
   si.nMin = 0;
   si.nMax = GetWindowLong(PrekinWindow,TEXTLINESTOTAL)-1;
   if ((unsigned)si.nMax <= si.nPage) si.nPos = 0;
   else si.nPos = si.nMax-si.nPage+1;
   SetWindowLong(PrekinWindow,TEXTLINECURRENT,si.nPos);
   SetScrollInfo(PrekinWindow, SB_VERT, &si, TRUE);
 }
}
/*___adjusttext()___________________________________________________________*/

/****SetTextCaption()********************************************************/
/* Set the caption of a "PrekinTextWindow" */
void SetTextCaption(HWND hWnd, LPSTR text)
{
 if (Lquiet == 0)
 {
     SendMessage(hWnd,WM_SETTEXT,0,(DWORD)text);
 }
}
/*___SetTextCaption()_______________________________________________________*/

/****SystemFontSize()*******************************************************/
void SystemFontSize(void)
{
    /* find the system font size */
    HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
    HFONT oldFont;
    TEXTMETRIC tm;
    HDC hDC;

    hDC = GetDC(PrekinWindow);
    oldFont = (HFONT)SelectObject(hDC,hFont);
    GetTextMetrics(hDC,&tm);
    SelectObject(hDC,oldFont);
    ReleaseDC(PrekinWindow,hDC);
    fontheight = tm.tmHeight;
    fontwidth = tm.tmAveCharWidth;
    fontmaxwidth = tm.tmMaxCharWidth;
    fontverticalspace = tm.tmHeight + tm.tmExternalLeading;
}
/*___SystemFontSize()______________________________________________________*/    

/****totalbufferlines()******************************************************/
/* count lines in the buffer for the current window size */
long totalbufferlines(HWND hWnd, HDC hDC, LPSTR s)
{
    LPSTR s0;
    LPSTR e;
    int nlines = 0;
    int nb;
    int i;
    RECT r;
    int windowwidth;

    e = s + GetWindowLong(hWnd,TEXTLENGTH);
    GetClientRect(hWnd,&r);
    windowwidth = (r.right - r.left);
    if (s)
   {
      while (*s && s < e)
      {
         nb = nlinebreaks(s,&s0);
         if (nb == 1) s = s0;
         if (s >= e || *s == '\0') break;
         i = sizeoftextline(hDC, windowwidth, s);
         /* putnchars(s,i); */
         s += i;
         while (*s == ' ') ++s;
         ++nlines;
      }
   }
    return(nlines);
}
/*___totalbufferlines()_____________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****linefromoffset()********************************************************/
/* For a given offset in the text buffer, what line is it on? */
/* This is used so that the top of the window is roughly the */
/* same after a resize. */
long linefromoffset(HWND hWnd, unsigned long *lsbuf, long offset)
{
    long nlines = GetWindowLong(hWnd,TEXTLINESTOTAL);
    long i;
    unsigned long end;
    for (i = 0; i < nlines; ++i) 
    {
        end = lsbuf[i+1];
        if (end > (unsigned)offset) return(i);
    }
    return(0);
}
/*___linefromoffset()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****makelinestarts()********************************************************/
/* Build the linestart array, freeing and allocating memory as necessary. */
/* Unlock the result and store the handle in text window extra bytes. */
long makelinestarts(HWND hWnd)
{
    LPSTR s00,s0,e;
    int nlines = 0;
    int nb,i,windowwidth;
    RECT r;

    HDC hDC = GetDC(hWnd);

    LPSTR s = (LPSTR)GetWindowLong(hWnd,TEXTHANDLE);
    long* lsbuf = (long*)GetWindowLong(hWnd,TEXTLINESTARTHANDLE);
    long topline = GetWindowLong(hWnd,TEXTLINECURRENT);
    unsigned long currentoffset = 0;
    long nlinestotal;

    nlinestotal = totalbufferlines(hWnd,hDC,s)+1;
    GetClientRect(hWnd,&r);
    windowwidth = (r.right - r.left);
    if (lsbuf != 0) 
    {/* there is an existing buffer; kill it */
        currentoffset = lsbuf[topline];
      free(lsbuf);
        SetWindowLong(hWnd,TEXTLINESTARTHANDLE,0);
    }
    SetWindowLong(hWnd,TEXTLINESTOTAL,nlinestotal);
   /* allocate the new array */
   lsbuf = (long *)calloc(nlinestotal+1,sizeof(nlinestotal));
   SetWindowLong(hWnd,TEXTLINESTARTHANDLE,(long)lsbuf);

    /* fill the new array */
    s00 = s;
    e = s + GetWindowLong(hWnd,TEXTLENGTH);
    if (s)
   {
      while (*s && s < e) 
      {
         nb = nlinebreaks(s,&s0);
         if (nb == 1) s = s0;
         if (s >= e || *s == '\0') break;
         i = sizeoftextline(hDC, windowwidth, s);
         if (nlines > nlinestotal) 
         {
            fprintf(stderr,"Nlines exceeded 2\n");
            exit(10);
         }
         lsbuf[nlines] = s-s00;
         s += i;
         while (*s == ' ') ++s;
         ++nlines;
      }
   }
    if (nlines > nlinestotal) 
    {
        fprintf(stderr,"Nlines exceeded 3\n");
        exit(10);
    }
    lsbuf[nlines] = s-s00;
    /* make sure we have the same top line as before */
    topline = linefromoffset(hWnd,(unsigned long *)lsbuf,currentoffset);
    SetWindowLong(hWnd,TEXTLINECURRENT,topline);
    ReleaseDC(hWnd,hDC);
    return(nlines);
}
/*___makelinestarts()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****linesonascreen()********************************************************/
int linesonascreen(HWND hWnd)
{
    RECT r;
    int n=0;
 if (Lquiet == 0)
 {
    GetClientRect(hWnd,&r);
    if (fontverticalspace <= 0) return(0);
    n = (r.bottom - r.top) / fontverticalspace;
 }
    return(n);
}
/*___linesonascreen()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****resetscrollbarvalues()**************************************************/
void resetscrollbarvalues(HWND hWnd)
{
   SCROLLINFO si;
 if (Lquiet == 0)
 {
   si.cbSize = sizeof(SCROLLINFO);
   si.fMask = SIF_ALL;
   si.nPage = linesonascreen(hWnd);
   si.nMin = 0;
   si.nMax = GetWindowLong(hWnd,TEXTLINESTOTAL)-1;
   if ((unsigned)si.nMax <= si.nPage-1)
   {
      si.nPos = 0;
      SetWindowLong(hWnd,TEXTLINECURRENT,si.nPos);
   }
   else if ((((unsigned)si.nMax)-
      (unsigned int)GetWindowLong(hWnd,TEXTLINECURRENT)) < si.nPage)
   {
      si.nPos = si.nMax-si.nPage+1;
      SetWindowLong(hWnd,TEXTLINECURRENT,si.nPos);
   }
   else si.nPos = GetWindowLong(hWnd,TEXTLINECURRENT);
   SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
 }
}
/*___resetscrollbarvalues()_________________________________________________*/

/****RePaint()***************************************************************/
/* repaint the text window */
/* this is intended to turn into the response to a WM_PAINT message */
int RePaint(HWND hWnd, HDC hDC)
{
 if (Lquiet == 0)
 {
  long topline = GetWindowLong(hWnd,TEXTLINECURRENT);
  long totallines = GetWindowLong(hWnd,TEXTLINESTOTAL);
  LPSTR textbuf = (LPSTR)GetWindowLong(hWnd,TEXTHANDLE);
  long* lsbuf = (long*)GetWindowLong(hWnd,TEXTLINESTARTHANDLE);
  int i,nchars;
  char *textstart;
  long start,end;

  int screenline = 0;
  int linesperscreen = linesonascreen(hWnd);

  for (i = topline; i < totallines && screenline < linesperscreen; ++i)
  {
    start = lsbuf[i];
    if (i == totallines) end = GetWindowLong(hWnd,TEXTLENGTH);
    else end = lsbuf[i+1];
   
    nchars = end-start;
    textstart = textbuf+start;
   if (nchars > 0 && (textstart[nchars-1] == EOL)) nchars--;
    if (nchars > 0) TabbedTextOut(hDC,0,screenline*fontverticalspace,
        (LPSTR)textstart,nchars,0,NULL,0);
    ++screenline;
  }
 }
  return(0);
}
/*___RePaint()______________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****nlinebreaks()************************************************************/
/* count empty lines (if any); record character following last line break */
int nlinebreaks(LPSTR s, LPSTR *t)
{
   int breaks = 0;
    LPSTR s0 = s;
   
   if (*s == EOL)
    {
        breaks = 1;
        s0 = s + 1;
    }
    *t = s0;
    return(breaks);
}
/*___nlinebreaks()__________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****sizeoftextline()********************************************************/
/* size in characters of longest well-broken text line that */
/*  can fit within windowwidth */
int sizeoftextline(HDC hDC, int windowwidth, char *p)
{
    char *t = p;
    char *t0 = p;
    DWORD extent;

    for (;;) 
    {
        while (!isspace(*t) && *t != '\0') ++t;
        extent = GetTabbedTextExtent(hDC,p,(t-p),0,NULL);
        if (LOWORD(extent) > windowwidth) 
        {
            if (t0 == p) 
            {/* panic: can't fit even one word on line */
              for ( ; t0 < t; ++t0)
                if(LOWORD(GetTabbedTextExtent(hDC,p,t0-p,0,NULL))>windowwidth)
                    return((t0-p)-1);
            }
            else return(t0-p);
        }
        else 
        {
            t0 = t;  /* set last good break point to current point */
            if (*t == '\0' || *t == EOL) return(t-p);
            ++t;
        }
    }
}
/*___sizeoftextline()_______________________________________________________*/

/****cleartexts()************************************************************/
void cleartexts()
{
   HANDLE buf;
 if (Lquiet == 0)
 {
   /*setup window text buffer*/
    SetWindowLong(PrekinWindow,TEXTLINECURRENT,0);

    /* give up previous buffer, if any */
    buf = (HANDLE)GetWindowLong(PrekinWindow,TEXTHANDLE);
    if (buf != 0) free(buf);
    /* allocate new buffer */
    buf = calloc(32768,sizeof(char));
    SetWindowLong(PrekinWindow,TEXTHANDLE,(long)buf);
    SetWindowLong(PrekinWindow,TEXTLENGTH,0);
    if (buf == 0)
    {
      sprintf(alertstr,"Out of memory in cleartexts");
      DoReportDialog();
    }
 }
}
/*___cleartexts()___________________________________________________________*/

/****completetext()***920614 dcr*********************************************/
void  completetext()    /*<-GetTextFromFile(),MAGETEXT*/
{
 if (Lquiet == 0)
 {
    makelinestarts((HWND)PrekinWindow);
    resetscrollbarvalues(PrekinWindow);
 }
}
/*___completetext()___920614 dcr____________________________________________*/

/****cleanuptextwindow()*****************************************************/
void cleanuptextwindow()
{
   ;
}
/*___cleanuptextwindow()____________________________________________________*/
