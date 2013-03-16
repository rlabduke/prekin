#include "PKIN.H"
#include "PKINDLOG.h"
#include <direct.h>

/****openscriptin()**********************************************************/
int openscriptin(void)
{
	int ireturn;

	ireturn = getfilename("dummy", 3); /*3 for scriptin*/
	
	if (ireturn)
	{
		/*if a previous file is open, close it*/
		if(fpscriptin != NULL) fclose(fpscriptin);
		if ((fpscriptin = fopen(ScriptinStr,"r")) == NULL)
		{
			sprintf(alertstr,"Prekin failed to open file: \n%s"
                      ,ScriptinStr);
			DoReportDialog();
			ireturn = 0;
		}
		else /* file opened, read header from it */
		{
			fscanf(fpscriptin ,CRLF"Script file opened: %s",temps);
			pkintextinsert(temps); 
			adjusttext(1);

			Lscriptin = 1; /*still !! */
			Lrangecontrols = 1;
			Lstartarange = 1; /*now go through range controls*/
			ireturn = 1;
		}
	}
	else
	{
		ireturn = 0; /*canceled file select procedure*/
		Lscriptin = 0; /*failed*/
		Lrangecontrols = 0;
		Lrangestart = 1;
	}
	return(ireturn);
}
/*___openscriptin()_________________________________________________________*/

/****getnewfile()************************************************************/
int getnewfile(char namestrs[256],int nfile)
{
	int iflag;
	int ireturn = 0;

      IOerr = 1; 
      /* Begins with no file and this flag set for no legitimate file*/
      while(IOerr == 1)
      {
		while(IOerr == 1)  iflag = getfilename(namestrs,nfile);
        if(iflag == 0) return(0);
                      /* e.g. invoked cancel instead of file name choice */
        IOerr = 1;
        openafile(namestrs,nfile);  /*PKINFILE.C*/
        if(IOerr)
        {
			sprintf(alertstr,"Error trying to open file %s",namestrs);
          DoReportDialog();
          ireturn = 1;  /* try again */
        }  
        else
		{
			SetPrekinCaption(); /* set caption to include current filename */
			ireturn = 2;
		}
      }
      return(ireturn);
}
/*___getnewfile()___________________________________________________________*/

/*ref: Microsoft Windows Guide to Programming, 1990, for file IO stuff*/

/****getfilename()***Specific for PCWindows**********************************/
int getfilename(char namestrs[256], int nfile)
{
    char *cpslash;
    OPENFILENAME ofn;
    char szFile[256],szFileTitle[256];
    UINT i, cbString;
    char chReplace;
    char szFilter[256];
    int ireturn;


    if (*szDirName == '\0') _getcwd(szDirName,sizeof(szDirName));
    szFile[0] = '\0';
    if(nfile==1) strcpy(szFilter,"pdb (*.*)|*.*|");
    if(nfile==3) strcpy(szFilter,"script (*.*)|*.*|");
    if(nfile==4) strcpy(szFilter,"AA library (*.*)|*.*|");
    cbString = strlen(szFilter);
    chReplace = szFilter[cbString-1];
    for (i = 0; szFilter[i] != '\0'; i++)
    {
        if (szFilter[i] == chReplace) szFilter[i] = '\0';
    }
    memset(&ofn,0,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = PrekinWindow;
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;

    if(nfile==1) ofn.lpstrTitle = "PDB File Open";
    if(nfile==3) ofn.lpstrTitle = "Script File Open";
    if(nfile==4) ofn.lpstrTitle = "AA Library File Open";

    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&ofn)) /*this must be a system call! */
    {
        lstrcpy(szDirName,ofn.lpstrFile);
        if ((cpslash = strrchr(szDirName,'\\')) != NULL) *cpslash = '\0';
/*c*/   if(nfile==1) lstrcpy(namestrs,ofn.lpstrFile);
		if(nfile==3) lstrcpy(ScriptinStr,ofn.lpstrFile);
		if(nfile==4) lstrcpy(namestrs,ofn.lpstrFile);
         /* store file name to be opened later */
        ireturn = 1;
    }
    else
    {
        ireturn = 0;
    }
    IOerr = 0;
    return(ireturn);
}
/*___getfilename()__________________________________________________________*/

/****getoutfilename()***Specific for PCWindows****************************/
int getoutfilename(char OutfileStr[256], int nfile)
{
    char *cpslash;
    OPENFILENAME ofn;
    char szFile[256],szFileTitle[256];
    UINT i, cbString;
    char chReplace;
    char szFilter[256];
    int ireturn;


    if (*szDirName == '\0') _getcwd(szDirName,sizeof(szDirName));
    szFile[0] = '\0';
    if(nfile==1) strcpy(szFilter,"kin (*.kin)|*.kin|");
    if(nfile==2) strcpy(szFilter,"script (*.*)|*.*|");
    cbString = strlen(szFilter);
    chReplace = szFilter[cbString-1];
    for (i = 0; szFilter[i] != '\0'; i++)
    {
        if (szFilter[i] == chReplace) szFilter[i] = '\0';
    }
    memset(&ofn,0,sizeof(OPENFILENAME));
    ofn.lStructSize		= sizeof(OPENFILENAME);
    ofn.hwndOwner		= PrekinWindow;
    ofn.lpstrFilter		= szFilter;
    ofn.nFilterIndex	= 1;
    sprintf(szFile,"%s",OutfileStr);
    ofn.lpstrFile		= szFile;
    ofn.nMaxFile		= sizeof(szFile);
    sprintf(szFileTitle,"%s",OutfileStr);
    ofn.lpstrFileTitle	= szFileTitle;
    ofn.nMaxFileTitle	= sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;

    if(nfile==1) ofn.lpstrTitle = "Kinemage Save As";
    if(nfile==2) ofn.lpstrTitle = "Script File Save As";

    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

    if (GetSaveFileName(&ofn)) /*this must be a system call! */
    {
        lstrcpy(szDirName,ofn.lpstrFile);
        if ((cpslash = strrchr(szDirName,'\\')) != NULL) *cpslash = '\0';
/*c*/   strcpy(OutfileStr,ofn.lpstrFile );
         /* store file name to be opened later */
        ireturn = 1;
    }
    else
    {
        ireturn = 0;
    }
    IOerr = 0;
    return(ireturn);
}
/*___getoutfilename()____________________________________________________*/
    
/****openhelpout()**********************************************************/
int openhelpout(void)
{
   int ireturn;

   ireturn = getfilename("dummy", 5); /*5 for helpout*/

   if (ireturn)
   {
      /*if a previous file is open, close it*/
      if(fphelpout != NULL) fclose(fphelpout);
      if ((fphelpout = fopen(helpoutStr,"w")) == NULL)
      {
         sprintf(alertstr,"Prekin failed to open file: \n%s",helpoutStr);
         DoReportDialog();
         ireturn = 0;
      }
      else /* file opened, read header from it */
      {
         Lhelpoutopen = 1;
         ireturn = 1;
      }
   }
   else
   {
      ireturn = 0; /*canceled file select procedure*/
      Lhelpoutopen = 0; /*failed*/
   }
   return(ireturn);
}
/*___openhelpout()_________________________________________________________*/

