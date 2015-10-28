/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****getnewfile()************************************************************/
int  getnewfile(char namestrs[256],int nfile)  
{
  int    ilocalflag,ireturn;
  static    short        *imessage, *ninfiles; /*see if launched by a file*/
      

      ilocalflag = 0;     
      IOerr = 1; 
      /* Begins with no file and this flag set for no legitimate file*/
      while(IOerr == 1)
      {
/*m*/   InitCursor();

        if(Llaunched && !LAElaunched) /*can be==1 only on first entering program*/
        {/*get name of file into global NameStr*/
            ilocalflag = 0; /*should not be able to get here under OS later than 6 */
#ifdef SYSTEM6
            getnamestroffile(); /*file that launched PREKIN, sys6*/
            strcpy(namestrs, (const char *)NameStr);
            CtoPstr(namestrs);
            ilocalflag = 1;
#endif /*SYSTEM6*/
        }
/*.*/   else if(LAElaunched)
        {
            strcpy(namestrs, (const char *)NameStr);  
            /*name of file already in global NameStr */
            CtoPstr(namestrs);
            ilocalflag = 1;
        }
        else 
        {
           while(IOerr == 1)  ilocalflag = getfilename(namestrs,nfile);
           /*returns with Pascal type file name in namestrs*/
        }
        if(ilocalflag == 0) return(0); 
                      /* e.g. invoked cancel instead of file name choice */
        Llaunched = 0;   /*clear flags so could select file if launch fails*/
        LAElaunched = 0;

        IOerr = 1;
/*m*/   /*SetCursor(*waitcursor);*/

        openafile(namestrs,nfile);  /*PKINFILE.C, expects Pascal type name string*/
        if(IOerr)
        {
/*c*/     sprintf(alertstr,"Error trying to open file %s",namestrs); 
/*Ms*/    DoReportDialog();
          ireturn = 1;  /* try again */
        }  
        else
        {
           ireturn = 2;
        }
      }
      return(ireturn);
}
/*___getnewfile()___________________________________________________________*/

/****getfilename()***Specific for Macinosh**********************PREKIN*******/
int    getfilename(char namestrs[256], int i)
{
  Point       where;
  SFTypeList  typeList;
  SFReply     reply;
  int         ireturn;
  
/*
SFReply
    good:    Boolean;
    fType:   OSType;
    vRefNum: Integer;
    version: Integer;
    fName:   Str63;
*/

    if(i==4) 
    {
        sprintf(alertstr,"Now choose a file with coords for %s;"
        CRLF"preferably use PREKINAA.LIB",resid[mutoutl]); 
        DoReportDialog();
    }
  
/*m*/  SetPt( &where, 85, 100 ); /* position of display box on screen */
   
       /* invoke the standard dialog for finding a file */
/*m*/  SFGetFile( where, "\p", NULL,
                 -1, typeList,        /* -1 means display all files */
                 NULL, &reply );
       /*The prompt string seems to go to an off-screen placed text field*/
       /*see I-523 of original Inside Macintosh*/
       if( reply.good )
       {
         /* In newer hierarchical directory system,*/
         /* reply.vRefNum holds directory number */
/*m*/    SetVol( "\p", reply.vRefNum ); /* change to directory of file */

/*c*/    strcpy(namestrs, (const char *)reply.fName);
         /* store file name to be opened later */
         ireturn = 1;
       }
       else
       {
          ireturn = 0; /*canceled file select procedure*/
       }
       IOerr = 0;
       return(ireturn);
}
/*___getfilename()__________________________________________________________*/

#ifdef SYSTEM6
/****ifilelaunched()****************************/
int        ifilelaunched()
{
static    short        *imessage, *ninfiles;
        /*Mac-portable needs these to be static*/
        /*see if MAGE was launched by clicking a file on a system 6 machine*/
/*m*/    CountAppFiles(imessage, ninfiles);

        return(*ninfiles);
}
/*___ifilelaunched()___________________________*/

/****getnamestroffile()**************************/

void    getnamestroffile()
{
    AppFile    filestart;

/*m*/        GetAppFiles( 1 , &filestart);
/*m*/        ClrAppFiles( 1 );
/*m*/        SetVol( "\p", filestart.vRefNum ); 
                /* change to directory of file */
/*m*/        PtoCstr(filestart.fName);
/*c*/        strcpy(NameStr, (const char *)filestart.fName); 
}
/*___getnamestroffile()__________________________*/
#endif /*SYSTEM6*/

/**** ConvertFilename(&fss) from RasMol2.5 Roger Sayle**********************/
            /*this is the magic that some how recovers the full path name*/
            /*even though the vRefNum and parID return seemingly useless*/
/*static void ConvertFilename(FSSpec *fss) */
void ConvertFilename(FSSpec *fss) 
{
    register char *src;
    register char *dst;
    register int i;
    char buffer[256];
    
    Str255 dirname;
    DirInfo dinfo;
    
    src = buffer;
    dinfo.ioDrParID = fss->parID;
    dinfo.ioNamePtr = dirname;
    do {
        dinfo.ioVRefNum = fss->vRefNum;
        dinfo.ioFDirIndex = -1;
        dinfo.ioDrDirID = dinfo.ioDrParID;
        PBGetCatInfo((CInfoPBPtr)&dinfo,0);
        
        *src++ = ':';
        for( i=dirname[0]; i; i-- )
            *src++ = dirname[i];
    } while( dinfo.ioDrDirID != 2 );
    
    /* Reverse the file path! */
    dst = NameStr;   /*MAGE input file name*/          /*dst = Filename;*/
    while( src != buffer )
        *dst++ = *(--src);
    for( i=1; i<=fss->name[0]; i++ )
        *dst++ = fss->name[i];
    *dst = '\0';                     /* C type ending for string ! */
}
/*___ConvertFilename()_______________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****openscriptin()***********************************************************/
int    openscriptin(void)
{

      Point    where;
      SFTypeList  typeList;
      SFReply   reply;
      int      ireturn;
  
/*m*/ SetPt( &where, 85, 100 ); /* position of display box on screen */

  
        /* invoke the standard dialog for finding a file */
/*m*/   SFGetFile( where, "\p", NULL,
                 -1, typeList,        /* -1 means display all files */
                 NULL, &reply );
        if( reply.good ) 
        {
           /* In newer hierarchical directory system,*/
           /* reply.vRefNum holds directory number */
/*m*/      SetVol( "\p", reply.vRefNum );   /* change to directory of file */
/*c*/      strcpy(ScriptinStr, (const char *)reply.fName);  
           /* store file name to be opened later */
/*m*/      PtoCstr((unsigned char *)ScriptinStr);    
           /* convert input file name from Pascal to C type string */

           /*if a previous file is open, close it*/
/*c*/      if(fpscriptin != NULL) fclose(fpscriptin); 

/*c*/      if ((fpscriptin = fopen(ScriptinStr,"r")) == NULL)
           {
/*c*/         sprintf(alertstr,"Prekin failed to open file: \n%s                    "
                      ,ScriptinStr);
/*Ms*/        DoReportDialog();
              ireturn = 0;
           }
           else /* file opened, read header from it */
           {
/*c*/        fscanf(fpscriptin ,CRLF"Script file opened: %s",temps);
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
/*___openscriptin()__________________________________________________________*/

/****openhelpout()***********************************************************/
int    openhelpout(void) /*040425*/
{
      Point    where;
      SFTypeList  typeList;
      SFReply   reply;
      int      ireturn;
   
   SetPt( &where, 85, 100 ); /* position of display box on screen */

   /* invoke the standard dialog for finding a file */
   SFGetFile( where, "\p", NULL,
              -1, typeList,        /* -1 means display all files */
              NULL, &reply );
   if( reply.good ) 
   {
      /* In newer hierarchical directory system,*/
      /* reply.vRefNum holds directory number */
      SetVol( "\p", reply.vRefNum );   /* change to directory of file */
      strcpy(helpoutStr, (const char *)reply.fName);  
      /* store file name to be opened later */
      PtoCstr((unsigned char *)helpoutStr);    
      /* convert input file name from Pascal to C type string */

      /*if a previous file is open, close it*/
      if(fphelpout != NULL) fclose(fphelpout); 

      if ((fphelpout = fopen(helpoutStr,"w")) == NULL)
      {
         sprintf(alertstr,"Prekin failed to open file: \n%s",helpoutStr);
         DoReportDialog();
         ireturn = 0;
      }
      else /* file opened */
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
/*___openhelpout()__________________________________________________________*/

