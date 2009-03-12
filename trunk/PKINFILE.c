                      /*PKINFILE.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINDLOG.h"

/****openafile()*************************************************************/
void   openafile(char namestrs[256],int nfile)
{
  int    i;
  IOerr = 0;  /* Start by presuming no IO error */

#ifdef MACINTOSH /*do this when first get name*/
       PtoCstr((unsigned char *)namestrs); /*m*/
       /* convert input file name from Pascal to C type string */
#endif

       for(i=1;i<256;i++)  /* remove trailing blank from file name */
       {
         if(namestrs[i] == '\0') if(namestrs[i-1] == ' ') namestrs[i-1] ='\0';
         if(namestrs[i] == '\0')  break;
       }

       if(nfile == 1)
       {
           /*if a previous file is open, close it*/
           if(fpinput != NULL && fpinput != stdin) fclose(fpinput); 
           if((fpinput    = fopen(namestrs,"r")) ==NULL)
           {/*failure*/
               IOerr=1;
               Lgetfile = 0; /*flag only for successful open of input file*/
               if(namestrs[0]=='\0')
                  sprintf(alertstr,"No input file selected: "
                                   "Try from File Menu or Quit");
               else
                  sprintf(alertstr,"Prekin openafile failed to open input file: %s\n Use menu File/Open File..."
                                  ,namestrs);
               if(!Lcommanded) DoReportDialog();
           }
           else
           {
               IOerr = 0;
               Lgetfile = 1; /*970417*/  
               atpdbEOF = 0; /* need to look for EOF on PDB input */
               for(i=0; namestrs[i] != '\0' ; i++) NameStr[i] = namestrs[i];
               NameStr[i+1] = '\0';  /*remember name of PDB file */
               Lnewfile = 1;
               Lgot_infilename = 1;
               Lgot_infileopen = 1;
               atEOF = 0;
           }
       }
       else if(nfile == 3)
       {
           /*if a previous file is open, close it*/
           if(fpscriptin != NULL) fclose(fpscriptin);   
           if((fpscriptin = fopen(namestrs,"r")) ==NULL)
           {/*failed, set alternate conditions*/
               IOerr=1;
               Lscriptin = 0;
               Lrangestart = 1;
               sprintf(alertstr,"Prekin failed to open scriptin file: %s"
                               ,namestrs);
               if(!Lcommanded) DoReportDialog(); /*990301*/
           }
           else 
           {
               IOerr = 0;
               Lscriptin = 1;
               Lscriptfileopen = 1;
               Lrangecontrols = 1;
               Lstartarange = 1;
               atEOF = 0;
           }
       }
       else if(nfile == 4)
       {
           /*if a previous file is open, close it*/
           if(fpaalib != NULL) fclose(fpaalib); 
           if((fpaalib    = fopen(namestrs,"r")) ==NULL)
           {/*failure*/
               IOerr=1;
               Laalibopen = 0;
               sprintf(alertstr,"Prekin failed to open aalib file: %s"
                               ,namestrs);
               if(!Lcommanded) DoReportDialog(); /*990301*/
           }
           else
           {
               IOerr = 0;
               Laalibopen = 1;
               atEOF = 0;
           }
       }
#ifdef UNIX_X11  /*990301*/
if(IOerr == 1)
{
   fprintf(stderr,"nfile==%d, Prekin failed to open: |%s|\n",nfile,namestrs);
}
#endif
}
/*___openafile()____________________________________________________________*/

/****prepareinputfile()******************************************************/
void prepareinputfile(void)
{
    /*if a previous file is open, close it*/
    if(fpinput != stdin && fpinput != NULL) 
    {
       fclose(fpinput); 
       Lgetfile = 0; 
       Lgot_infileopen = 0;
    }  
    if(Lin_stdin)
    { 
        fpinput = stdin;
        Lgetfile = 1; /* input file open, ready to get output file*/
        Lgot_infileopen = 1;
    }
    else
    { 
       if ((fpinput = fopen(NameStr,"r")) == NULL)
       {
          Lgetfile = 0; 
          Lgot_infileopen = 0;
          Lgot_infilename = 0; /*since don't recognize this name 061020*/
          Lstartfile = 0; /*reset, given input file failed so ignore it 070726*/
          if(Lquiet) /*cannot do anything about this*/
          {
             Lerr = 1;
             sprintf(alertstr,"Prekin has failed to open input file: %s\n",\
               NameStr);
          }
          else 
          {
             sprintf(alertstr,"Prekin has failed to open input file: %s\n Use menu File/Open File...",NameStr);
          }
          DoReportDialog();
       }
       else
       {/*opened*/
          Lgetfile = 1; /* input file open, ready to get output file*/
          Lgot_infileopen = 1;
#ifdef UNIX_X11
   if(LIOmage) {fprintf(stderr,"Prekin has opened input file: %s\n",NameStr);}
#endif
       }
    }
    Llaunched = 0;   /*used possible launch, clear flags*/
    LAElaunched = 0;
}
/*___prepareinputfile()_____________________________________________________*/
    
/****selectfiles()***********************************************************/
void    selectfiles()
{
   /*enters when Lnewfile==1*/
   Lnewfile = 0; /*so won't come back to this subroutine */
   Lgetfile = 0; /*flag to track actually getting a file*/
   Lnewpass = 0; /*flag: input and output files available for a newpass */

   if(!Lgot_infileopen)
   {
      if(Lgot_infilename){prepareinputfile();}/*sets Lgot_infileopen ___OUTP.c*/
      /*if(!Lgot_infileopen)  use else, let gui request filename 070726*/
      else
      {/*either failed to open or maybe even find the named input file*/
       /*or input file was not named when starting prekin... */
         if(Lquiet) {Lerr = 1;} /*cannot do anything about lack of file name*/
         /*else can get file with GUI...*/
         /*this used to be else getnewfile(NameStr,1); commented out 061020*/
         /*else getnewfile(NameStr,1);061020*/ /* 1 for fpinput ,____FILE.C*/
         /*(getnewfile just trys to open it again) 061020 */
         else {getnewfile(NameStr,1);}  /*070726*/
      }
    
   }
   if(Lgetfile && !Lgot_outfileopen)
   {/*has input file, try for output file*/
      /*first, derive names from input file name, NameStr*/
      makeoutputfilename(); /*also makes molecule name for button label*/
      if(Lgot_outfilename) prepareoutputfile();/*sets Lgot_outfileopen__OUTP.c*/
      if(!Lgot_outfileopen) 
      {
         if(Lquiet) Lerr = 1;
         else openoutput(); /*needs trial OutfileStr, ____OUTP.C*/
      }
   }
   Lgetfile = 0; /*file selection now not an option until finish run*/

   if(!Lquiet) AdjustMenus();  /*PKMCMENU.C*/
   Lnewfile = 0; /*so won't come back to this subroutine */
}
/*___selectfiles()__________________________________________________________*/

/****prepareoutputfile()******************************************************/
void prepareoutputfile(void)
{
    /*if a previous file is open, close it*/
    if(fpoutput != NULL && fpoutput != stdout)
    {
       fclose(fpoutput); 
       Lfpoutput = 0;
       Lgot_outfileopen = 0;
    }
    if(Lout_stdout)
    {
		fpoutput = stdout;
        Lfpoutput = 1; /* flag for successful fpoutput*/
        Lgot_outfileopen = 1;
    }
    else
    { 
        if(Lappend)
            fpoutput = fopen(OutfileStr,"a");
        else
            fpoutput = fopen(OutfileStr,"w");

        if (fpoutput == NULL)
        {
            sprintf(alertstr,"Prekin failed to open output file: \n%s          "
                    ,OutfileStr);
            sprintf(alertstr2," ");
            sprintf(alertstr3," ");
            DoReportDialog();
            Lnewpass = 0;
            Lfpoutput = 0;
            Lgot_outfileopen = 0;
        }
        else
        {/*opened*/
            Lfpoutput = 1; /* flag for successfully opened fpoutput*/
            Lgot_outfileopen = 1;
        }
    }
    if(Lfpoutput)
    {/* file opened, prepare to write header into it*/
        if(Ltestprint)
        {
            sprintf(alertstr,"opened file: %s          "CRLF,OutfileStr);
            pkintextinsert(alertstr);
            adjusttext(1);
        }
        Ldid_outfiletext = 0; /*DO NOT do the @text yet */
        /*wait until chance to get Header stuff from PDB file*/
        inittextblock(&headerstuff); /*PKINSCRT.c*/
        if(LIOmage) inittextblock(&pdbinfostuff); /*PKINSCRT.c*/ /*020514*/
        Lnewpass = 1;
        AdjustMenus();  /*____MENU.C*/
        /*and now go back into the masterloop*/
    }
}
/*___prepareoutputfile()_____________________________________________________*/

/****makeoutputfilename()*****************************************************/
void makeoutputfilename()
{
   /*attempt to get more reasonable molecule name from file name*/
   /* and preserve file name without directory as well*/
   /* and set up output file names for kinemage and putative script file*/
   int   i = 0, j = 0, idot = 0, jdot = 0;
   int   k   = 0; /*counter for last "." in file name*/
   int   kk  = 0; /*counter for penultimate "." in file name, or start of name*/
   int   kkk = 0; /*counter for start of actual file name*/
   int   kkkk= 0; /*counter for start of actual full file name*/
   int   s = -1; /*index of last slash in full input name*/
                 /*Linux needs directory name for its dialog filter*/
   int  itch=0; /*for codestr, pdb file identity in point IDs 060322*/
   
   for(j=0;j<256;j++)
   {
       if(NameStr[j]==':')
       { k = j+1; kk = j+1; kkk = j+1; kkkk = j+1;} /*? name start*/
       if(NameStr[j]=='/' || NameStr[j]=='\\' )
       { 
          k = j+1; 
          kk = j+1; 
          kkk = j+1; 
          idot = 0; /*effective name start*/
          s = j; /*index of last slash*/
       } 

       if(NameStr[j]=='.')
       { 
          idot = 1; 
          jdot = j; /*remember index of last .  040318*/
          kk = k; 
          k = j+1; /*advance kk, k in step*/
       }
       if(NameStr[j]=='\0') break;
   }

   {/*short file name for MAGE to give prekin or probe to read*/
       if(j >= kkk)
       {/*take just file name beyond any directory path info*/
          for(i=0; i<=j; i++ )
          {
             ShortNameStr[i]= NameStr[kkk+i];
             PDBfileStr[i]= NameStr[kkk+i];
             /*if(PDBfileStr[i]== '.') */
             if(idot && kkk+i == jdot) /*position of last . in name  040318*/
             {
                PDBfileStr[i]= '\0'; /*effective str end*/
             }
          }
          ShortNameStr[i] = '\0';
          PDBfileStr[i]= '\0'; /*for sure str end*/
          if(codestr[0] == ' ' || !Lcodestr) /*see -code on commandline...*/
          {
            itch = 0;
            while(PDBfileStr[itch]!='\0')
            {/*strobe out code string limited to MAXNAMECHAR long*/ /*060322*/
               codestr[itch] = PDBfileStr[itch];
               itch++;
               if(itch >= MAXNAMECHAR) {break;}
            }
            codestr[itch] = '\0'; /*longest itch==MAXNAMECHAR*/

             /*strcpy(codestr,PDBfileStr); 060322*/  /*051102*/
             Lcodestr = 1;
          }
       }/*take just file name beyond any directory path info*/
       else 
       {/*nothing recognized beyond directory path info*/
          ShortNameStr[0] = '\0';
       }
   }/*short file name for MAGE to give prekin or probe to read*/

   if(idot == 1)
   {/*there is a dot in fields past the last directory divider*/
      if( j >= k)
      {/*at least one character present after last dot */ 
         if(j-k == 3) /*3 characters, may be stupid ending, or PC extent*/
         {
            /*strip off any 3 char suffix, may just be PC extent*/
            /*if(    (NameStr[k]=='p'&&NameStr[k+1]=='d'&&NameStr[k+2]=='b')  */
            /*    || (NameStr[k]=='e'&&NameStr[k+1]=='n'&&NameStr[k+2]=='t')  */
            /*    || (NameStr[k]=='o'&&NameStr[k+1]=='u'&&NameStr[k+2]=='t') )*/
            {
               j= k-2; 
               k = kk;/*reset to earlier interval*/
            }
         }
         /*take name between penultimate dot (or directory path and final dot*/
         /*or take name as last field from last dot to the end*/
      }/*at least one character present after last dot */ 
      else
      {/*ends with a dot, pathological: strip off dot*/
         j = j-1;
      } 
   }/*there is a dot in fields past the last directory divider*/

   if(Lmolecstr) /*override of molec name from commandline 051114*/
   {
      strcpy(MolNameStr,molecstr);
   }
   else /*derive from file name*/
   {
      /*j and kk set to last effective field of name, make mol name*/
      i = 0;
      while(i<=j-kk && i < 10)
      {/*take up to 10 chars of name field as a molecule name for label*/
         MolNameStr[i]= NameStr[kk+i];
         i++;
      }
      MolNameStr[i] = '\0';      
   }
   /*j and kkk set to last set of name fields stripped of 3 char extent*/
   i = 0;
   /*while(i<=j-kkk)*/ /*short name after last slash*/
   /*011028 use kkkk for full path name rather than short name at kkk*/
   while(i<=j-kkkk) /*full path name, after possible computer name: */
   {/*take all chars of name fields except 3 char extent as output names*/
      if(!Lgot_outfilename) OutfileStr[i] = NameStr[kkkk+i];
      ScriptoutStr[i] = NameStr[kkkk+i];
      if(NameStr[kkkk+i]=='\0') break; /*do not advance i, extant to be added*/
      i++;
   }/*take all chars of name fields except 3 char extent as output names*/

   /*add on extents to output files*/

   if(!Lgot_outfilename)
   {
      OutfileStr[i] = '.';
      OutfileStr[i+1] = 'k';
      OutfileStr[i+2] = 'i';
      OutfileStr[i+3] = 'n';
      OutfileStr[i+4] = '\0';
   }
   ScriptoutStr[i] = '.';
   ScriptoutStr[i+1] = 's';
   ScriptoutStr[i+2] = 'c';
   ScriptoutStr[i+3] = 'p';
   ScriptoutStr[i+4] = '\0';           
   if(s > 0)
   {/*finite length directory path*/
      for(i=0; i<253; i++) /*disaster protection: need room for 3 more char*/
      {
         DirStr[i] = NameStr[kkkk+i]; /*kkkk possibly useless protection*/
         if(i == s) {break;} /*and i not incremented*/
         if(NameStr[i] == '\0') {break;}
      }
      DirStr[i++] = '/'; /*redundant?: puts a slash in position of last slash*/
      DirStr[i++] = '*';
      DirStr[i] = '\0';
   }/*finite length directory path*/
   else if(s==0)
   {/*last slash is char#0, presume in top directory*/
      DirStr[0] = '/';
      DirStr[1] = '*';
      DirStr[2] = '\0';
   }/*last slash is char#0, presume in top directory*/
   else
   {/*no directory path, presume in current directory*/
      DirStr[0] = '.';
      DirStr[1] = '/';
      DirStr[2] = '*';
      DirStr[3] = '\0';
   }/*no directory path, presume in current directory*/
/*
printf("input:  %s\n",NameStr);
printf("short:  %s\n",ShortNameStr);
printf("pdb:    %s\n",PDBfileStr);
printf("direct: %s\n",DirStr);
printf("output: %s\n",OutfileStr);
*/
}
/*___makeoutputfilename()____________________________________________________*/
