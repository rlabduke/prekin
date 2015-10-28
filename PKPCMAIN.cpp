/****PROGRAM: Prekin.c****STRUCTURE connector: PDB to KIN*******************/

#define EXTERN
#include "PKIN.h"
#include "PKINDLOG.h"
#include "PKINMENU.h"


/****WinMain()**************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
/*HINSTANCE hInstance;     current instance
HINSTANCE hPrevInstance;   previous instance
LPSTR lpCmdLine;           command line
int nCmdShow;              show-window type (open/icon) */
{
    MSG msg;                             /* message*/

    if (*lpCmdLine != '\0') filearg = lpCmdLine;
    else filearg = NULL;

	IPC = 1;
	IMAC = 0;

    /*Ltestprint = TRUE;*/  /*test option, compile as TRUE to invoke,*/
    Ltestprint = 0;  /* as FALSE for distribution, can also set in Kludges */  
    Lerr =0;

	initialvalues(); /*starting general values */
	getversion();

	if (filearg) 
   {
      dissectcommandline(filearg);
   }
   
   if (Lquiet == 0)
	{/*running with GUI*/
		if (!InitInstance(hInstance, nCmdShow)) 
      {
         return (FALSE);
      }
		beginnings();
	}
	else 
   {/*running quiet from commandline*/
      for (;;) 
      {
         mainlooprouter();
         if(Lerr) break;   /*030228 to fail gracefully*/
      }
      return(0);
   }

   /*Drop through to here if running NOT Lquiet, i.e. using GUI */
   /*Acquire and dispatch messages until a WM_QUIT message is received.*/
   /* Master endless loop for GUI*/
   /*this GetMessage stuff should be equivalent to Mac HandleEvent stuff*/
   /* besides working as the endless Loop */
   while (GetMessage(&msg,    /* message structure                     */
        NULL,              /* handle of window receiving the message*/
        0,              /* lowest message to examine */
        0))             /* highest message to examine*/
   {
      mainlooprouter(); /*PKININIT, common router*/
      if(Lerr) break;
      if(Lquiet==0) /*was Lcommanded 990402*/
      {
         if (!TranslateAccelerator (PrekinWindow, PrekinAccel, &msg))
         {
            TranslateMessage(&msg);    /* Translates virtual key codes */
            DispatchMessage(&msg);     /* Dispatches message to window */
         }
      }
   }
   return (msg.wParam); /* Returns the value from PostQuitMessage*/
}
/*___WinMain_______________________________________________________________*/

/****MainWndProc()***********************************************************/
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message,
							 WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
    PAINTSTRUCT ps;
	int iadjust;
	long linecount, currentline, totallines;
	LPSTR textbuf;


    switch (message) 
    {
	case WM_CREATE:
		currentline = GetWindowLong(hWnd,TEXTLINECURRENT);
		totallines = GetWindowLong(hWnd,TEXTLINESTOTAL);
		textbuf = (LPSTR)GetWindowLong(hWnd,TEXTHANDLE);
    case WM_PAINT:
		hDC = BeginPaint(PrekinWindow,&ps);
		RePaint(hWnd, hDC);
		EndPaint(PrekinWindow,&ps);
		return(0);
    case WM_COMMAND:
      /* message: command from application menu? */
      switch (LOWORD(wParam)) 
      {                               /*processing routines in PKPCMENU.C */
        case PKIN_FILEOPENFILE:
			PKIN_FileOpenFile();
			break;
        case PKIN_FILENEWPASS:
			PKIN_FileNewPass();
			break;
		case PKIN_LAUNCHCURRENT:
			launchcurrentkinemage();
			break;
        case PKIN_FILEQUIT:
			PKIN_FileQuit();
			break;


        case PKIN_OPTIONSABOUTOPTIONS:
			PKIN_OptionsAboutOptions();
			break;
        case PKIN_OPTIONSMUTATIONS:
			PKIN_OptionsMutations();
			break;
        case PKIN_OPTIONSHELPHTML:
			writehelptohtmlfile(0); /*PKININIT.c*/
			break;

        default: return(DefWindowProc(hWnd, message, wParam, lParam));
      }
      return(0);
    case WM_SIZE:
        /* calculate new line start array and resetscrollbars done in completetext*/
		if (wParam != SIZE_MINIMIZED) completetext();
		return (DefWindowProc(hWnd,message,wParam,lParam));

	case WM_VSCROLL:
		linecount = linesonascreen(hWnd);
		/* messages from the scroll bar */
		currentline = GetWindowLong(hWnd, TEXTLINECURRENT);
		iadjust = 1;
		switch (LOWORD(wParam))

		{
			case SB_LINEDOWN:
				if (currentline == (GetWindowLong(hWnd,TEXTLINESTOTAL) - linecount))
					iadjust = 0; /* already at bottom, don't redraw */
				else ++currentline;
				break;
			case SB_LINEUP:
				if (currentline == 0) iadjust = 0;

				    /* already at top, don't redraw */
				else --currentline;
				break;
			case SB_PAGEDOWN:
				if (currentline == (GetWindowLong(hWnd,TEXTLINESTOTAL) - linecount))
					iadjust = 0; /* already at bottom, don't redraw */
				else currentline += linecount-1;
				break;
			case SB_PAGEUP:
				if (currentline == 0) iadjust = 0;
				    /* already at top, don't redraw */
				else currentline -= linecount+1;
				break;
			case SB_THUMBTRACK:
				if (currentline != HIWORD(wParam)) currentline = HIWORD(wParam);
				else iadjust = 0;
				break;
			default:
				iadjust = 0;
				return (DefWindowProc(hWnd,message,wParam,lParam));
		}
		if(iadjust)
		{
			if (currentline < 0) currentline = 0;
			if (currentline > (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount))
				currentline = (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount);
			SetWindowLong(hWnd,TEXTLINECURRENT,currentline);
			resetscrollbarvalues(hWnd);
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return(0);
	case WM_MOUSEWHEEL:
		linecount = linesonascreen(hWnd);
		if (linecount <= GetWindowLong(hWnd,TEXTLINESTOTAL)-1)
		{/*don't do any of this if screen not full of text*/
			currentline = GetWindowLong(hWnd,TEXTLINECURRENT);
			iadjust = 1; /* by default redraw the screen */
			if(((short)HIWORD(wParam) > 0) && (currentline > 0)) currentline--;
			else if(((short)HIWORD(wParam) < 0) && 
				(currentline < (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount)))
				currentline++;
			else if (currentline < 0) currentline = 0;
			else if (currentline > (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount))
				currentline = (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount);
			else iadjust = 0; /* at top or bottom of text, don't redraw */
			if (iadjust)
			{
				SetWindowLong(hWnd,TEXTLINECURRENT,currentline);
				resetscrollbarvalues(hWnd);
				InvalidateRect(hWnd,NULL,TRUE);
			}
		}
		return(0);
	case WM_KEYDOWN:
        linecount = linesonascreen(hWnd);
		if (linecount <= GetWindowLong(hWnd,TEXTLINESTOTAL)-1)
		{/*don't do any of this if screen not full of text*/
			currentline = GetWindowLong(hWnd,TEXTLINECURRENT);
			iadjust = 1;
			switch (wParam)
			{
				case VK_HOME:
					if (currentline == 0) iadjust = 0;
					    /* already at top, don't redraw */
					else currentline = 0;
					break;
				case VK_END:
					if (currentline == (GetWindowLong(hWnd,TEXTLINESTOTAL) - linecount))
						iadjust = 0; /* already at bottom, don't redraw */
					else currentline = GetWindowLong(hWnd,TEXTLINESTOTAL) - linecount;
					break;
				case VK_PRIOR:
					if (currentline == 0)iadjust = 0;
					    /* already at top, don't redraw */
				    else currentline -= linecount+1;
					break;
				case VK_NEXT:
				    if (currentline == (GetWindowLong(hWnd,TEXTLINESTOTAL) - linecount))
						iadjust = 0; /* already at bottom, don't redraw */
				    else currentline += linecount-1;
				    break;
				case VK_UP:
				    if (currentline == 0)iadjust = 0;
					    /* already at top, don't redraw */
				    else --currentline;
				    break;
				case VK_DOWN:
				    if (currentline == (GetWindowLong(hWnd,TEXTLINESTOTAL) - linecount))
						iadjust = 0; /* already at bottom, don't redraw */
				    else ++currentline;
				    break;

				default:
					iadjust = 0;
				    return(DefWindowProc(hWnd, message, wParam, lParam));
			}
			if (iadjust==1)
			{
				if (currentline < 0) currentline = 0;
				if (currentline > (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount))
					currentline = (GetWindowLong(hWnd,TEXTLINESTOTAL)-linecount);
				SetWindowLong(hWnd,TEXTLINECURRENT,currentline);
				resetscrollbarvalues(hWnd);
				InvalidateRect(hWnd,NULL,TRUE);
			}
		}
		return(0);
	case WM_DESTROY:
        if (GetWindowLong(hWnd,TEXTHANDLE))
			free((HANDLE)GetWindowLong(hWnd,TEXTHANDLE));
        if (GetWindowLong(hWnd,TEXTLINESTARTHANDLE))
			free((HANDLE)GetWindowLong(hWnd,TEXTLINESTARTHANDLE));
		PostQuitMessage(0);
		return(0);
    default:                          /* Passes it on if unproccessed    */
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
}
/*___MainWndProc()__________________________________________________________*/

/****SetPrekinCaption()******************************************************/
/* Set the caption of the "PrekinWindow" */
void SetPrekinCaption()
{
	SetTextCaption(PrekinWindow,lstrcat(dirstrip(NameStr)," - Prekin"));
}
/*___SetPrekinCaption()_____________________________________________________*/

/****dirstrip()**************************************************************/
char* dirstrip (char oldNameStr[256])
{
	int n,i,h;
	static char TestStr[256];
	/* static necessary b/c we return the address of the char array */

 	n = -1; /*keeps track of last \ directory boundary in full path name*/
    for(i=0;i<256;i++)
    {/*isolate local, simple name from Full Path Name*/
        if(oldNameStr[i] == '\\') n=i;
        if(oldNameStr[i] == '\0') break;
    }
    h = 0; /*index for working version of input name*/
    /*n is either==0, or set to first char of simple name in the Full Path name*/
    for(i=n+1;i<256;i++)   /*just work in actual local file name part*/
    {/*make copy of working part of input file name, flagged for last . */
      TestStr[h] = oldNameStr[i]; /*this has exact name of current input file*/
      if(TestStr[h] == '\0') break;
      h++;
    }
return (TestStr);
}
/*___dirstrip()_____________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****getkeydownstate()*******************************************************/
int getkeydownstate()
{
    return(0);
}
/*___getkeydownstate()______________________________________________________*/

/****TickCount()-dummy-******************************************************/
long TickCount()
{
    return(0);
}
/*___TickCount()-dummy-_____________________________________________________*/

/****makecursorwait()********************************************************/
void makecursorwait(void)
{
	SetCursor(LoadCursor(NULL,IDC_WAIT));
}
/*___makecursorwait()_______________________________________________________*/

/****makecursoractive()******************************************************/
void makecursoractive(void)
{
	SetCursor(LoadCursor(NULL,IDC_ARROW));
}
/*___makecursoractive()_____________________________________________________*/

