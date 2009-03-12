#include "PKIN.H"
#include "PKINDLOG.h"
#include "PKINMENU.H"

/****operatingsystem()********************************************************/
char* operatingsystem()
{
   return(" MS PC,");
}
/*___operatingsystem()_______________________________________________________*/

/****beginnings()*************************************************************/
void beginnings()
{ 
        IOerr = 1;    /* Begins asumming no file */ 
            /* and various flags set for no legitimate file */
        if(Lgot_infilename && !Lgot_outfilename || Lcommanded != 0) /* invoked by a file */
        {
            Lnewfile = 1;
            sprintf(word,"%s"
              CRLF"A request for a PDB file has launched PREKIN"
			  CRLF"Input file is: %s",version,NameStr);
            pkintextinsert(word); /*PKMCTEXT.C*/
        
            Lgot_infilename = 1;  /*972123*/
        }
        else /*Llaunched == 0*/
        {/*NOT launched with a known file name*/
		   aboutprekin();
		   DoInitDialog();  /*PKPCMENU.C*/
		   if(Lexplanation)
		   {
			   doexplanation(); /*PKINMAIN.C*/
			   Lnewfile = 0;
		   }
           if(Lnewfile == 0)
           {
              sprintf(word,"%s"
                           CRLF"Use File pull-down menu above"    
                           " to Open a PDB File."CRLF,version);
               pkintextinsert(word); /*PKMCTEXT.C*/
           }
           else /*Lnewfile==1 so will actually put up a new file dialog*/
           {
              sprintf(word,"%s"
                        CRLF"Select a PDB-type File"CRLF,version);

              pkintextinsert(word); /*PKMCTEXT.C*/
           }
        }/*NOT launched with a known file name*/
		adjusttext(0);
}
/*___beginnings()____________________________________________________________*/

/****InitInstance()*********************************************************/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
        /* called at initialization time for every instance of */
        /* this application & does initialization tasks that   */
        /* cannot be shared by multiple instances.             */

    RECT   rect;
	int iret;
    WNDCLASSEX  wc;

    /* Save the instance handle in global variable, which will be used in  */
    /* many subsequent calls from this application to Windows.            */

    hInst = hInstance;
    DisplaySizeX = max(500,(int)(.75*(GetSystemMetrics(SM_CXSCREEN))));
    DisplaySizeY = max(400,(int)(.75*(GetSystemMetrics(SM_CYSCREEN))));


	/* load the menu accelerators to key commands for Open, etc... */
	PrekinAccel = LoadAccelerators(hInst, "PrekinAccelerator");

    /* define and register the Main window class */
	wc.cbSize			= sizeof (wc);
	wc.style			= CS_HREDRAW | CS_VREDRAW;/* Class style(s).             */
    wc.lpfnWndProc		= MainWndProc;		/* Function to retrieve messages for */
											/* windows of this class.            */
    wc.cbClsExtra		= 0;                /* No per-class extra data.          */
    wc.cbWndExtra		= 20;               /* per-window extra data.         */
    wc.hInstance		= hInst;            /* Application that owns the class.  */
    wc.hIcon			= LoadIcon(hInst, "PrekinIcon");
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= "PrekinMenu";		/* Name of menu resource in .RC file.*/
    wc.lpszClassName	= "PrekinMainClass";/*Name used in call to CreateWindow. */
	wc.hIconSm			= LoadIcon(hInst, "PrekinIcon"); /*Icon used in main window's UL corner */

    iret				= RegisterClassEx(&wc);	/* Register the window class */
    if (iret == 0)		return(FALSE);			/*quit on failure.			 */

    /* Create a main window for this application instance.  */
    PrekinWindow = CreateWindow(
        "PrekinMainClass",     /* See RegisterClass() call.     */
        "Prekin",              /* Text for window title bar.    */
        WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_VSCROLL,  /* Window style.*/
		0,                  /* Grab whole display surface */
        0,
        DisplaySizeX,
        DisplaySizeY,
        NULL,                /* Overlapped windows have no parent. */
        NULL,                /* Use the window class menu.         */
        hInst,               /* This instance owns this window.    */
        NULL                 /* Pointer not needed.                */
    );

    /* If window could not be created, return "failure" */
    if (!PrekinWindow) return (FALSE);

    /* Make the window visible and update its client area. */
    ShowWindow(PrekinWindow, nCmdShow);  /* Show the window */
    GetClientRect(PrekinWindow,&rect);
    DisplaySizeX = rect.right;
    DisplaySizeY = rect.bottom;

    SystemFontSize();

	AdjustMenus();
	cleartexts();

    return (TRUE);    /* Returns the value from PostQuitMessage */
}
/*___InitInstance()_________________________________________________________*/
