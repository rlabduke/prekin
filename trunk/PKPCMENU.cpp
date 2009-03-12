#include "PKIN.H"
#include "PKINMENU.H"
#include "PKINDLOG.h"

#include "process.h"

/*static HANDLE ghfile;*/
/*static LPSTR file0buf; */
/*static char *textstart;*/
/*static int havetext; */

/***lstrncmp()****************************************************************/
int lstrncmp(char *s, char *t, int n)
{
while (n > 0) {
    if (*s > *t) return(1);
    if (*s < *t) return(-1);
    if (*s == '\0' && *t == '\0') return(0);
    ++s;
    ++t;
    --n;
    }
return(0);
}
/*__lstrncmp()_______________________________________________________________*/

/****enableamenuitem()********************************************************/
void    enableamenuitem(int flag, int menuitem)
{
	if(flag) EnableMenuItem(GetMenu(PrekinWindow),menuitem,MF_ENABLED);
	else EnableMenuItem(GetMenu(PrekinWindow),menuitem,MF_DISABLED|MF_GRAYED);
}
/*___enableamenuitem()_______________________________________________________*/

/****PKIN_FileOpenFile()******************************************************/
void PKIN_FileOpenFile(void)
{
    Lnewfile = 1;
}
/*___PKIN_FileOpenFile()_____________________________________________________*/

/****PKIN_FileNewPass()*******************************************************/
void PKIN_FileNewPass(void)
{
    Lgetfile = 1; /*must reset to 1 if continue with new pass on same file*/
    Lrangestart=1;
    Lnewpass = 1;
	npass++;
	rewind(fpinput);
	Linclusive = 1;
}
/*___PKIN_FileNewPass()______________________________________________________*/

/****PKIN_FileQuit()**********************************************************/
void PKIN_FileQuit(void)
{
    SendMessage(PrekinWindow,WM_SYSCOMMAND,SC_CLOSE,0);
}
/*___PKIN_FileQuit()_________________________________________________________*/

/****launchcurrentkinemage()*************************************************/
void launchcurrentkinemage(void)
{
	if(fpinput!=0) fclose(fpinput);
	if(fpoutput!=0) fclose(fpoutput);
	sprintf(word,"mage %s",OutfileStr);
	execlp("mage","mage",OutfileStr,NULL);
    exit(0);
}
/*___launchcurrentkinemage()________________________________________________*/

/****PKIN_OptionsAboutOptions()**********************************************/
void PKIN_OptionsAboutOptions(void)
{
    sprintf(alertstr,"\nNo menu options.");
    DoReportDialog();
}
/*___PKIN_OptionaAboutOptions()_____________________________________________*/

/****PKIN_OptionsMutations()*************************************************/
void PKIN_OptionsMutations(void) 
{
   menuaboutmutations(0); /*PKINMENU.c*/
}
/*___PKIN_OptionsMutations()________________________________________________*/

/****AdjustMenus()************************************************************/
void AdjustMenus()
{
	enableamenuitem(!Lgetfile, PKIN_FILEOPENFILE);
	enableamenuitem(Lpassposs, PKIN_FILENEWPASS);
	enableamenuitem(Lmadekin, PKIN_LAUNCHCURRENT);
	enableamenuitem(1, PKIN_OPTIONSMUTATIONS);
	enableamenuitem(1, PKIN_OPTIONSHELPHTML);
}
/*___AdjustMenus()___________________________________________________________*/

