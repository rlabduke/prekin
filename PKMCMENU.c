/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#include "PKIN.h"
#include "PKINDLOG.h"

#define kFinderSig         'FNDR'
#define kAEFinderEvents    'FNDR'
#define kSystemType        'MACS'
#define kAEOpenSelection   'sope'
#define keySelection       'fsel'
#define ktargetSignature   'MAGE' /*NOT actually used !!!! */

enum  {
  appleID = 1,
  fileID,
  optiID
  };

enum  { openItem = 1, passItem, launchItem, quitItem };
enum  { aboutItem = 1,mutrotItem,makehelphtmlItem};

/****SetUpMenus()************************************************************/
void   SetUpMenus()  /*Set up the menus hardwired*/
{

/*m*/  InsertMenu(appleMenu = NewMenu(appleID, "\p\024"), 0);

/*m*/  InsertMenu(fileMenu = NewMenu(fileID, "\pFile"), 0);

/*m*/  InsertMenu(optiMenu = NewMenu(optiID, "\pPREKIN"), 0);

/*m*/  DrawMenuBar();

/*m*/  AppendMenu(appleMenu, "\pAbout PEKIN...;(-;");
       AppendResMenu(appleMenu, 'DRVR');
/*m*/  AppendMenu(fileMenu, "\pOpen...;"
                            "New Pass;"
                            "Launch MAGE with this kin;"
                            "Quit/Q;"
                            );
/*m*/  AppendMenu(optiMenu,"\pAbout options...;"
                           "mutations...;"
                           "Help->HTML...;"
                           );

}
/*___SetUpMenus()___________________________________________________________*/

/****AdjustMenus()***********************************************************/
void   AdjustMenus()
{
    register WindowPeek wp = (WindowPeek) FrontWindow();
    short kind = wp ? wp->windowKind : 0;
    Boolean DA = kind < 0;
    
    
/*.*/  enable(appleMenu, 1, 1); /* PREKIN info in apple menu always enabled */
/*.*/  enable(fileMenu, openItem, !Lgetfile);
/*.*/  enable(fileMenu, passItem, Lpassposs);
/*.*/  enable(fileMenu, launchItem, Lmadekin&&Lappleevents); 
           /*must have make kinemage before launch it*/
/*.*/  enable(optiMenu, aboutItem, 1);
/*.*/  enable(optiMenu, mutrotItem, 1);

}
/*___AdjustMenus()__________________________________________________________*/

/****enable()****************************************************************/
void enable(MenuHandle menu, int item, int ok)
{
    if (ok)
/*m*/    EnableItem(menu, item);
    else
/*m*/    DisableItem(menu, item);
}
/*___enable()_______________________________________________________________*/

/*****HandleMenu(mSelect)****************************************************/
void   HandleMenu (long  mSelect)
/*System routines MenuSelect() and MenuKey() return system defined variable:*/
/*  mSelect,  the high word is the menu ID, the low word is the menu item */
{

  int      menuID = HiWord(mSelect);
  int      menuItem = LoWord(mSelect);
  Str255    name;
  GrafPtr    savePort;
  
  switch (menuID)
  {
    case  appleID:
      switch (menuItem)
      {
         case 1:
           sprintf(alertstr,"PREKIN helps make Kinemages "
              "\015 Copyright \251 1999 David C. Richardson,"
              "\015  Little River Institute, 5820 Old Stony Way, "
              "Durham NC 27705 "
              "\015\015%s"
              "\015\015Mutations use Robert Diamond's superposition code"
              ,version
              );
           DoReportDialog();  
         break;

         default:
           GetPort(&savePort);
           /*GetItem(appleMenu, menuItem, name);*/
           GetMenuItemText(appleMenu, menuItem, name);
           OpenDeskAcc(name);
           SetPort(savePort);
         break;
       }
    break;
    case  fileID:
      switch (menuItem)
      {
        case  openItem:
          Lnewfile = 1;
        break;
      
        case  passItem:
          Lgetfile = 1; /*must reset to 1 if continue with new pass on same file*/
          Lnewpass = 1;
          Lrangestart = 1;
          npass++;
          rewind(fpinput);
          Linclusive = 1;
        break;
        case launchItem:
          launchcurrentkinemage();
        break;
        case  quitItem:
          if(fpinput!=0) fclose(fpinput);
          if(fpoutput!=0) fclose(fpoutput);
          ExitToShell();
        break;
      }
    break;

    case  optiID:
      switch (menuItem)
      {
        case aboutItem:
           sprintf(alertstr,
              CRLF"If AppleEvents are implemented,"
              CRLF"can launch MAGE with the current kinemage");
           DoReportDialog();  
        break;  

        case mutrotItem:
           menuaboutmutations(0); /*PKINMENU.c*/
        break;            

        case makehelphtmlItem:
           writehelptohtmlfile(0); /*PKININIT.c*/
        break;            

      }
    break;  
    }
}
/*___HandleMenu()___________________________________________________________*/

/****launchcurrentkinemage()**************************************************/
void launchcurrentkinemage()
{
    
    OSErr ireturned;

    if(fpinput!=0) fclose(fpinput);    /*961117*/
    if(fpoutput!=0) fclose(fpoutput);  /*961117*/

    ireturned = OpenSelection(&(outputfileinfo.sfFile));

    ExitToShell();  /*961117*/
}            
/*___launchcurrentkinemage()_______________________________________________________*/

/****OpenSelection()**********************************************************/
OSErr OpenSelection(FSSpecPtr theDoc)
{
    /*this routine comes from the THINK C examples */
    /*that Roger Sayle printed out for me*/
    AppleEvent        aeEvent;
    AEDesc            myAddressDesc;
    AEDesc            aeDirDesc;
    AEDesc            listElem;
    AEDesc            fileList;
    FSSpec            dirSpec;
    AliasHandle         dirAlias;
    AliasHandle         fileAlias;
    ProcessSerialNumber process;
    OSErr               myErr;
/*
sprintf(alertstr
,"PREKIN will tell the Finder to launch MAGE with this kinemage"
 ", then PREKIN will close.");
DoReportDialog(); 
*/
    /*process seems to be the Finder, to which later AE will be sent*/
    if(FindAProcess(kFinderSig,kSystemType,&process))  return procNotFound;
    
    /*construct myAddressDesc for the Finder as a process*/
    myErr = AECreateDesc(typeProcessSerialNumber,(Ptr)&process,
                      sizeof(process),&myAddressDesc);
    if(myErr)  return myErr;
    
    /*idea is to simulate a double click on a specific file*/
    /*So create an AE that goes to the Finder and requests kAEOpenSelection*/   
    myErr = AECreateAppleEvent(kAEFinderEvents,kAEOpenSelection,
              &myAddressDesc,kAutoGenerateReturnID,kAnyTransactionID,&aeEvent);
    if(myErr)  return myErr;
    
    /*recast PREKIN output file spec into a form that will travel with AE */
    FSMakeFSSpec(theDoc->vRefNum,theDoc->parID,nil,&dirSpec);
    NewAlias(nil,&dirSpec,&dirAlias);
    NewAlias(nil,theDoc,&fileAlias);
    
    /*Set up a List to hold document information*/
    myErr = AECreateList(nil,0,false,&fileList);
    if(myErr) return myErr;
    HLock((Handle)dirAlias);
    AECreateDesc(typeAlias,(Ptr)*dirAlias,GetHandleSize((Handle)dirAlias),&aeDirDesc);
    HUnlock((Handle)dirAlias);
    if((myErr = AEPutParamDesc(&aeEvent,keyDirectObject,&aeDirDesc)) == noErr)
    {
        AEDisposeDesc(&aeDirDesc);
        HLock((Handle)fileAlias);
        AECreateDesc(typeAlias,(Ptr)*fileAlias,
               GetHandleSize((Handle)fileAlias),&listElem);
        HUnlock((Handle)fileAlias);
        DisposeHandle((Handle)fileAlias);
        myErr = AEPutDesc(&fileList,0,&listElem);
    }
    if(myErr)  return myErr;

    AEDisposeDesc(&listElem);
    myErr = AEPutParamDesc(&aeEvent,keySelection,&fileList);
    if(myErr) return myErr;
    myErr = AEDisposeDesc(&fileList);
    
    /*now send to Finder an AE that Finder can treat as a file clicked*/
    myErr = AESend(&aeEvent,nil,
          kAENoReply+kAEAlwaysInteract+kAECanSwitchLayer,
          kAENormalPriority,kAEDefaultTimeout,nil,nil);
          AEDisposeDesc(&aeEvent);
  
    return(myErr);
}
/*___OpenSelection()_________________________________________________________*/

/****FindAProcess()***********************************************************/
OSErr FindAProcess(OSType typeToFind, OSType creatorToFind
                  ,ProcessSerialNumberPtr processSN)
{
    ProcessInfoRec      tempInfo;
    FSSpec              procSpec;
    Str31               processName;
    OSErr               myErr = noErr;
    
    processSN->lowLongOfPSN = kNoProcess;
    processSN->highLongOfPSN = kNoProcess;
    
    tempInfo.processInfoLength = sizeof(ProcessInfoRec);
    tempInfo.processName = (StringPtr)&processName;
    tempInfo.processAppSpec = &procSpec;
    
    while((tempInfo.processSignature != creatorToFind ||
            tempInfo.processType != typeToFind) ||
            myErr != noErr)
    {
        myErr = GetNextProcess(processSN);
        if(myErr == noErr) GetProcessInformation(processSN, &tempInfo);
    }
    return(myErr);
}
/*___FindAProcess()__________________________________________________________*/    

#ifndef POWERMAC
/*
 *  exec - transfer to another application
 *
 *  This handles all the exec  functions.  Arguments, etc., are not
 *  passed to the new task.
 *
 */

int exec(char *filename, ...)
{
	char pname[FILENAME_MAX];
	static struct { unsigned char *name; long flag; } buf;

	buf.name = __c2p(filename, pname);
	_exiting(1);
	asm {
		lea		buf,a0
		_Launch
		_ExitToShell
	}
}
#endif /*NOT POWERMAC*/

        
