/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****openoutput()************************************************************/
int    openoutput(void)
{

  Point    where;
  SFReply   reply; /*StandardFileReply  outputfileinfo*/
  int      ireturn;
  
  unsigned long    mage1,mage2,mage3,mage4; /*OSType  4byte integer*/
  FInfo     filefinderinfo;


#define NOTNEWWAY
#define NOTNEWWAYTELL
#define OLDWAY
#define NOTOLDWAYTELL
  
  mage1 = ('M'*16777216); /*shift <<24, doesn't work*/
  mage2 = ('A'*65536);    /*shift <<16, doesn't work*/
  mage3 = ('G'<<8);
  mage4 = 'E';
  

/*m*/  SetPt( &where, 85, 100 ); /* position of display box on screen */

/*m*/  CtoPstr(OutfileStr);  
       /* convert output file name from C to Pascal type string */
    
       /* invoke the standard dialog for finding a file */
#ifdef OLDWAY
/*m*/  SFPutFile( where, "\p output kinemage file:", 
                  (unsigned char *)OutfileStr, NULL, &reply );
#endif
#ifdef NEWWAY       
/*m*/  StandardPutFile( "\p output kinemage file:", 
                  (unsigned char *)OutfileStr, &outputfileinfo );
#endif
#ifdef OLDWAY
       if( reply.good )
#endif
#ifdef NEWWAY
       if( outputfileinfo.sfGood ) 
#endif
       {/*good outputfile info*/
         /* In newer hierarchical directory system, now the OLDWAY,*/
         /*reply.vRefNum holds directory number */
#ifdef OLDWAY
/*m*/    SetVol( "\p", reply.vRefNum );   /* change to directory of file */
/*c*/    strcpy(OutfileStr, (const char *)reply.fName);
#endif
#ifdef NEWWAY
/*m*/    /*SetVol( "\p", outputfileinfo.sfFile.vRefNum );*/   /* change to directory of file */

/*m*/    /*SetVol( "\p", outputfileinfo.sfFile.parID );*/   /* change to directory of file */
         /*outputfileinfo.sfType = mage1+mage2+mage3+mage4;*/
/*c*/    strcpy(OutfileStr, (const char *)outputfileinfo.sfFile.name);
         /*store file name to be opened later*/
#endif
/*m*/    PtoCstr((unsigned char *)OutfileStr);    
         /* convert file name from Pascal to C type string */
#ifdef OLDWAYTELL
sprintf(alertstr,"OLDWAY: OutfileStr== %s\015 reply.vRefNum== %d"
                 ,OutfileStr,reply.vRefNum);
DoReportDialog();
#endif

#ifdef NEWWAYTELL
sprintf(alertstr,"NEWWAY: OutfileStr== %s\015 outputfileinfo.sfFile.vRefNum== %d"
                 "\015 outputfileinfo.sfFile.parID== %d"
                 ,OutfileStr,outputfileinfo.sfFile.vRefNum,outputfileinfo.sfFile.parID);
DoReportDialog();
#endif





         /*if a previous file is open, close it*/
/*c*/    if(fpoutput != NULL) fclose(fpoutput); 
        
         if(Lappend)
            fpoutput = fopen(OutfileStr,"a");
         else
            fpoutput = fopen(OutfileStr,"w");

         if (fpoutput == NULL) 
         {
/*c*/      sprintf(alertstr,"Prekin failed to open file: \n%s          ",OutfileStr);
/*Ms*/     DoReportDialog();
           ireturn = 0;
         }
         else /* file opened, write header into it */
         {
#ifdef OLDWAY
           FSMakeFSSpec(reply.vRefNum, 0, reply.fName, &(outputfileinfo.sfFile));
           FSpGetFInfo(&(outputfileinfo.sfFile), &filefinderinfo);
           /*change creator*/
           filefinderinfo.fdCreator = mage1+mage2+mage3+mage4;
           FSpSetFInfo(&(outputfileinfo.sfFile), &filefinderinfo);
#endif           
#ifdef NEWWAY
           FSpGetFInfo(&(outputfileinfo.sfFile), &filefinderinfo);
           /*change creator*/
           filefinderinfo.fdCreator = mage1+mage2+mage3+mage4;
           FSpSetFInfo(&(outputfileinfo.sfFile), &filefinderinfo);
#endif           

           if(Ltestprint)
           {
/*c*/         sprintf(alertstr,"opened file: %s          ",OutfileStr);
/*Ms*/        DoReportDialog();
           }
           Lfpoutput = 1; /*flag for successfully opened fpoutput*/
           
           /* file opened, prepare to write header into it*/
           Ldid_outfiletext = 0; /*DO NOT do the @text yet */
           /*wait until chance to get Header stuff from PDB file*/
           inittextblock(&headerstuff); /*PKINSCRT.c*/
           /* disposetextblock(&headerstuff);*/ /*PKINSCRT.c*/
 
           Lnewpass = 1;
           AdjustMenus();  /*____MENU.C*/
           /*and now go back into the masterloop*/

           ireturn = 1;
         }



         Lgot_outfilename = 1; /*to keep book */
         /*prepareoutputfile();*/ /*this is already done*/
         Lfpoutput = 1; /* flag for successfully opened fpoutput*/
         Lgot_outfileopen = 1; /*to keep book */

       }/*good outputfile info*/
       else
       {
         ireturn = 0; /*canceled file select procedure*/
       }      
       return(ireturn);
}
/*___openoutput()___________________________________________________________*/


/****openscriptout()*********************************************************/
int    openscriptout(void)
{

      Point    where;
      SFReply   reply;
      int      ireturn;
  
/*m*/ SetPt( &where, 85, 100 ); /* position of display box on screen */

/*m*/   CtoPstr(ScriptoutStr);  
        /* convert output file name from C to Pascal type string */
  
        /* invoke the standard dialog for finding a file */
/*m*/   SFPutFile( where, "\p output Script file:", 
        (unsigned char *)ScriptoutStr, NULL, &reply );
        
        if( reply.good ) 
        {
           /* In newer hierarchical directory system,*/
           /* reply.vRefNum holds directory number */
/*m*/      SetVol( "\p", reply.vRefNum );   /* change to directory of file */
/*c*/      strcpy(ScriptoutStr, (const char *)reply.fName);  
           /* store file name to be opened later */
/*m*/      PtoCstr((unsigned char *)ScriptoutStr);    
           /* convert input file name from Pascal to C type string */

           /*if a previous file is open, close it*/
/*c*/      if(fpscriptout != NULL) fclose(fpscriptout); 

/*c*/      if ((fpscriptout = fopen(ScriptoutStr,"w")) == NULL)
           {
/*c*/         sprintf(alertstr,"Prekin failed to open file: \n%s                    "
                      ,ScriptoutStr);
/*Ms*/        DoReportDialog();
              ireturn = 0;
           }
           else /* file opened, write header into it */
           {
/*c*/        fprintf(fpscriptout,"{script for coord file: %s }"EOLO,NameStr);
             ireturn = 1;
           }
         }
         else
         {
           ireturn = 0; /*canceled file select procedure*/
         }      
         return(ireturn);
}
/*___openscriptout()________________________________________________________*/
   
