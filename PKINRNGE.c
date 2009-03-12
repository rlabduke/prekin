                        /*PKINRNGE.c*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"
#include "PKINDLOG.h"
/*Mac and PC modal dialogs let this be all one subroutine */
/*UNIX_X11 mode-less dialogs force breaking into a series of subroutines*/
/****PKINRNGE  common ***************************************************/
static  char  mc[] = "mc"; /*1  mcxl*/ /*mainchain*/
static  char  sc[] = "sc"; /*2  scxl*/ /*sidechain*/
static  char  hb[] = "hb"; /*3  hbxl*/ /*mc H-bonds*/
static  char  hy[] = "hy"; /*4  hyxl*/ /*hydrogens*/
static  char  ca[] = "ca"; /*5  caxl*/ /*pseudo-backbone*/
static  char  ht[] = "ht"; /*6  htxl*/ /*het atoms*/
static  char  wa[] = "wa"; /*7  waxl*/ /*waters*/
static  char  at[] = "at"; /*8  atxl*/ /*atom markers*/
static  char  lb[] = "lb"; /*9  lbxl*/ /*residue labels*/
static  char  ct[] = "ct"; /*A  ctxl*/ /*atom markers on C */
static  char  mK[] = "mK"; /*B  mcCPKl*/ /*CPKs instead*/
static  char  sK[] = "sK"; /*C  scCPKl*/ /*CPKs instead*/
static  char  hK[] = "hK"; /*D  htCPKl*/ /*CPKs instead*/
static  char  ro[] = "ro"; /*E  rotl*/ /*rotation if same resname, else mut*/
static  char  Bc[] = "Bc"; /*F  Bcol*/ /*colored by B val*/
static  char  Bv[] = "Bv"; /*G  Bval*/ /* pointID Bval&Occ*/
static  char  rb[] = "rb"; /*H  ribl*/ /*scope of ribbon*/
static  char  al[] = "al"; /*I  Alpha*/ /*Alpha range*/
static  char  be[] = "be"; /*J  Beta*/ /*Beta range*/
static  char  co[] = "co"; /*K  Coil*/ /*Coil range*/
static  char  bs[] = "bs"; /*L  Best*/ /*Best ribbon over this range*/
static  char  ci[] = "ci"; /*M  cisl*/ /*cis peptide uses ribbon code*/
/*050206 L and M do not seem to be used*/

static  char  blank2[] = " x";
static  char  blank3[] = "...";

int      Lflag, line, idoline, nn1,nn2, iloop, h,i,j,k, ibreak,infield,nfields;
float    fx,fy,fz;
char chr1[3],chr2[3],chr3[3],chr4[3],chr5[3],chr6[3],chr7[3],chr8[3],chr9[3];
char chrA[3],chrB[3],chrC[3],chrD[3],chrE[3],chrF[3],chrG[3],chrH[3],chrI[3];
char chrJ[3],chrK[3];
char    resin[4];


/****rangestartinitialize()**************************************************/
void rangestartinitialize() 
                  /*called from top of parsecommandline() */
                  /*called from top of getranges() when (LD && Lrangestart) */
                  /*LD: Legal Dialogs if not a fully functional command line*/
{
   Lcommanded = 0;
   Lrangestart = 0; /*flag that could get us back to getranges(), then to here*/
   Lendarange = 0;
   Lfinishranges = 0; /* used to proceed after getranges()*/
   
   Lresnumfovea = 0; 
   Lbuiltin = 0;
   Lexternal = 0;
   Llabellist =0;/* Labellist coord pseudo-Externalscript*/
   
   Lrangecontrols = 0;
   LRCDialogInput = 0;
   Ldetails = 0;
   Lgetatommarker = 0;
   Lfocusoption = 0;
   Lfocusoptionend = 0;
   Lfocusdistance = 0;
   Lfocusdistanceend = 0;
   Lresnumfovea = 0;
   resnumfovea = 0;
   FOVEA = FALSE;
   
   Lneedsubunits = 0;
   beginsubunit = 1;
   endsubunit = 1;
   
   Lconditions = 0;
   Lribbon = 0;
   Lautoribbon = 0;
   Lgetribbonparam = 0; /*000621*/
#ifdef OLDPKINRIBNCODE
   for(j=0;j<MAXC;j++) nA[j] = 0; /*index regions of HELIX*/
   kchainA = 0;
   for(j=0;j<MAXC;j++) nB[j] = 0; /*index regions of SHEET*/
   kchainB = 0;
#endif
   
   Lscriptin = 0;  /* there may not be a script file */
   Lscriptfileopen = FALSE;
   Lscriptout = 0;
   Lopenscriptout = 0;
   Laccept = 0;
   Lmodify = 0;
   
   Lallaminoacids = 0;
   Lallbases = 0;
   Lnucleicacid = 0;
   Lnucleicarrows = 0; /*040226*/
   Lkluges = 0;
   
   Lbrowsedefault = 0;
   Nchaincharcode = 0; /*010425 applied in rangecontrol()*/
   for(j=0; j<MAXchaincharcode; j++)
   {/*clear array */
      chaincharcode[j][0] = '\0';/*010425, 2char 070926*/
      chaincharcode[j][1] = '\0';/*010425, 2char 070926*/
      chaincharcode[j][2] = '\0';/*010425, 2char 070926*/
   }
   Nmodelcharcode = 0; /*010425 applied in rangecontrol()*/
   for(j=0; j<(MAXmodelcharcode); j++)
   {/*clear array */
      for(h=0;h<3;h++)
      {
         modelcharcode[j][h] = '\0';/*010425*/
      }
   }

   for(n=0 ; n<MAXR ;n++)  /*clear range logicals */
   {
          resnu1[n] = -999;
          resnu2[n] = 9999;
          
          mcxl[n] = 0; /*6 mainchain*/
          scxl[n] = 0; /*7 sidechain*/
          hbxl[n] = 0; /*8 mc H-bonds*/
          hyxl[n] = 0; /*9 hydrogens*/
          caxl[n] = 0; /*10 pseudo-backbone*/
          htxl[n] = 0; /*11 het atoms*/
          waxl[n] = 0; /*12 waters*/
          atxl[n] = 0; /*13 atom markers*/
          lbxl[n] = 0; /*14 residue labels*/
          ctxl[n] = 0;  /*29 atom markers on C */
          mcCPKl[n] = 0; /*30 CPKs instead*/
          scCPKl[n] = 0; /*31 CPKs instead*/
          htCPKl[n] = 0; /*32 CPKs instead*/
          mcal[n] = 0;
          scal[n] = 0;
          htal[n] = 0;
          waal[n] = 0;
          mccl[n] = 0;
          sccl[n] = 0;
          htcl[n] = 0;
          resl[n] = 0;
          cssl[n] = 0;
          mutl[n] = 0; /*mutation diff resname, derived from rotl*/
          rotl[n] = 0; /*28 rotation same resname*/
          supl[n] = 0; /*superposition of aa's onto ideal std aa*/
          xyzl[n] = 0;
          ribl[n] = 0; /*35 scope of ribbon*/
          Bcol[n] = 0; /*33 colored by B val*/
          Bval[n] = 0; /*34 pointID Bval&Occ*/
          Alpha[n] = 0;
          Beta[n] = 0;
          Coil[n] = 0;
          Best[n] = 0;  /*Best ribbon over this range*/
          cisl[n] = 0; /*cis peptide Ca--Ca emphasized*/
          sitel[n] = 0; /*range has site residues 050928*/
     /*Alpha,Beta,Coil are now part of ribbon sec str ranges from header*/
   }
   n = 0; /*Now treat as global, current range number*/
   mrange = n;/*global number of working ranges this pass*/
   for(j=0;j<256;j++) oldstr[j]='\0';   /*range info strings*/
   for(j=0;j<256;j++) olderstr[j]='\0';
   /*clear newer control logicals not yet integrated into ranges*/
   Lcbetadev = 0;
   Ltaudev = 0;
   Laspectabc = 0;
   Laspectnc = 0;
   Lemphasis = 1; /*default: rotatable vectors emphasized*/
   Lsitefake = 0; /*clear internal selection, leave header Lsite as is 050928*/
   Lcbstubsrib = 0; 
   Lexplicitribbon = 0;
   explicitribbontype = 0;
   explicitribbonidentity[0] = '\0'; /*default: no ID  060929*/
   Lribidentity = 0; /*default, no colorsets defined for rib sec str  060929*/
   Lvariablewidthribbon = 0; /*051208*/
}
/*___rangestartinitialize()_________________________________________________*/

#ifdef PRINTRANGES
printf("code r=1  2  3  4  5  6  7  8  9\n");
printf("mcxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,mcxl[1],mcxl[2],mcxl[3],mcxl[4],mcxl[5],mcxl[6],mcxl[7],mcxl[8],mcxl[9]);
printf("scxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,scxl[1],scxl[2],scxl[3],scxl[4],scxl[5],scxl[6],scxl[7],scxl[8],scxl[9]);
printf("hbxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,hbxl[1],hbxl[2],hbxl[3],hbxl[4],hbxl[5],hbxl[6],hbxl[7],hbxl[8],hbxl[9]);
printf("hyxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,hyxl[1],hyxl[2],hyxl[3],hyxl[4],hyxl[5],hyxl[6],hyxl[7],hyxl[8],hyxl[9]);
printf("caxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,caxl[1],caxl[2],caxl[3],caxl[4],caxl[5],caxl[6],caxl[7],caxl[8],caxl[9]);
printf("htxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,htxl[1],htxl[2],htxl[3],htxl[4],htxl[5],htxl[6],htxl[7],htxl[8],htxl[9]);
printf("waxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,waxl[1],waxl[2],waxl[3],waxl[4],waxl[5],waxl[6],waxl[7],waxl[8],waxl[9]);
printf("atxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,atxl[1],atxl[2],atxl[3],atxl[4],atxl[5],atxl[6],atxl[7],atxl[8],atxl[9]);
printf("lbxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,lbxl[1],lbxl[2],lbxl[3],lbxl[4],lbxl[5],lbxl[6],lbxl[7],lbxl[8],lbxl[9]);
printf("ctxl  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,ctxl[1],ctxl[2],ctxl[3],ctxl[4],ctxl[5],ctxl[6],ctxl[7],ctxl[8],ctxl[9]);
printf("mcal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,mcal[1],mcal[2],mcal[3],mcal[4],mcal[5],mcal[6],mcal[7],mcal[8],mcal[9]);
printf("scal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,scal[1],scal[2],scal[3],scal[4],scal[5],scal[6],scal[7],scal[8],scal[9]);
printf("htal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,htal[1],htal[2],htal[3],htal[4],htal[5],htal[6],htal[7],htal[8],htal[9]);
printf("waal  %d  %d  %d  %d  %d  %d  %d  %d  %d\n"
,waal[1],waal[2],waal[3],waal[4],waal[5],waal[6],waal[7],waal[8],waal[9]);
#endif /*PRINTRANGES*/
/****getranges()*************************************************************/
void getranges()
{/*Router through Dialog boxes and entry to builtin scripts*/
   int LD; /*short logical for OK to invoke a Dialog Box*/
     if(Lcommanded > 0 ) LD = 0;
     else                LD = 1; /*Legal Dialogs possible*/

     if(LD && Lrangestart) 
     {
         rangestartinitialize();/*sets defaults*/
         DoRangeStartDialog();  /*PKINRNGE.C*/ 
         /*sets choice of Lbuiltin && Lbrowsedefault*/
         /*               Lbuiltin && Lbuiltindialog*/
         /*               Lexternal                 */
         /*               Lrangecontrols && Lstartarange   */
         /*               Ldetails && Lfocusoption  */
         /*               Lkluges                   */
     }
     if(LD && Lkluges)    DoKludgesDialog(); /*PKINDLOG.c*/
         /*always sets Lrangestart, so does DoRangeStartDialog() again */
         /*except when sets Lcolorscale*/
     if(LD && Lcolorscale)    DoColorScaleDialog(); /*PKINDLOG.c*/
         /*always sets Lrangestart, so does DoRangeStartDialog() again */
      
     if(    (Lcommanded > 0 && Lbuiltin) 
         || (Lbuiltin && Lbrowsedefault)) builtinscripts(); 
                                                    /*go directly*/
     if(Lcommanded > 0 && !Lbuiltin)
     {
         Lgetranges = 0; /*everything else set by command line*/
         Lfinishranges = 1; /*finishranges() packages subunit choice info */
     }
     if(LD && Lbuiltin && Lbuiltindialog) DoBuiltinScriptsDialog(); 
         /*sets Lbuiltinchoice*/
     if(Lbuiltin && Lbuiltinchoice) builtinscripts(); /*go directly*/
        /*builtinscripts sometimes sets Ldetails to finish off this pass*/
        /*  Lfocusoption may or may not be set also */
      
     if(LD && Lexternal) DoExternalScriptDialog();  /*PKINDLOG.c*/
        /*externalscripts then uses Lrangecontrols*/
      
     if(Lopenscriptin)
     { openscriptin(); Lopenscriptin=0;}

     if(Lrangecontrols && Lstartarange) 
     {if(Ltestprint)printf("Lrangecontrols && Lstartarange\n"); startarange(); }

     if(LD && Lrangecontrols && LRCDialogInput) 
     {if(Ltestprint)printf("Lrangecontrols&& LRCDialogInput\n"); DoRangeControlDialog();
                                                               /*PKINDLOG.c*/ }
     if(Lrangecontrols && Lendarange)
     {
        if(Ltestprint) printf("preendarange(): Lrangecontrols && Lendarange\n"); 
        endarange();

        if(Ltestprint)printf("postendarange():Lrangecontrols %d, Ldetails %d"
        ", Lfocusoption %d\n"
         ,Lrangecontrols,Ldetails,Lfocusoption); 
     }

     if(Lrangecontrols && Lopenscriptout)
     {
        Lfocusoption = 0; /*make sure can't go on until finished write out*/
        if(Ltestprint)printf("Lrangecontrols && Lopenscriptout\n");
        openscriptout(); /*file dialog box PUXMOUTP*/
     }
     if(Lrangecontrols && Lscriptout) 
     {
        if(Ltestprint)printf("Lrangecontrols && Lscriptout\n"); 
        scriptout();  /*.*/ 
     }
        /*rangecontrol sets Ldetails  && Lfocusoption to finish off this pass*/
      
     /* a stdkinemage has already setup details in its own standard way*/
     if(!Lwaiting && LD && Ldetails && Lfocusoption) focusoption();
         /*calls DoFocusOptionsDialog();*/
         /*-> get x,y,z focus point from file,  Lscriptin==1*/
         /*-> get x,y,z focus point from residue number, Lresnumfovea==1*/
         /*-> type in x,y,z focus point */
     if(!Lwaiting && Ldetails && FOVEA && Lscriptin) 
         focusoptionend();
     if(!Lwaiting && LD && Ldetails && FOVEA && Lfocusdistance) 
     {
         if(Ltestprint)printf("about to DoFocusParamDialog()\n");
         DoFocusParamDialog(); /*resnum or x,y,z*/
         /*sets Lconditions */
     }
     if(!Lwaiting && LD && Ldetails && Lconditions && Lgetatommarker ) 
         getatommarker();
         /*selects atom marker type and size, if markers selected*/
     if(!Lwaiting && LD && Ldetails && Lgetribbonparam) { DoRibbonDialog(); }
         /*Lribbon set only as one of builtinscripts, sets Lconditions*/

     if(!Lwaiting && LD && Ldetails && Lconditions && !Lgetatommarker) 
         DoRunConditionsDialog(); /* subunit scope*/
         /*DoRunConditionsDialog can set Lneedsubunits */
     
     if(!Lwaiting && LD && Ldetails 
        &&!Lconditions&&!Lgetatommarker&&!Lfocusoption&&!Lfocusdistance) 
     {/*release from accummulating details of ranges*/
         /*DoRunConditionsDialog is the last of the details*/
         Ldetails = 0; /*this was the last of the details*/
         Lgetranges = 0; /*Now break out of the getranges loop */
         Lfinishranges = 1; /* NOW complete range setups*/
     }
     /*Lwaiting = 1 by routine that opens a non-modal dialog box*/
     /*Lwaiting = 0 by routine that is called back by the dialog box*/
     /* after dialog box business is finished */
     
  /*Lgetranges ==1, main will call getrangedetails() */
  /*when Lwaiting gets set back to 0 */

}/*971029*/
/*___getranges()_______________________________________________________*/

/****builtinscripts()********************************************************/
void builtinscripts()
{
    /*mrange is set == 0 at the beginning of each new pass*/
    /*command line scopes as well as requests for builtin scripts */
    /*can accummulate ranges, where mrange is the highest yet*/
       if(Lcommanded > 0)
       {/*commandline requested a builtin script*/
          ithscript = Lcommanded;
          /* Lbrowsedefault = 0;*/ /*to avoid its dominance*/ /*990402*/
          /*allow browse from the commandline*/
       }
       Lbuiltin = 0; /*do not want to get back to here*/
       Lbuiltinchoice = 0; /*nolonger needed*/

       {/*built-in scripts*/
           if(Lbrowsedefault) /*dominant*/
           {
               ithscript = 1;
               Lbrowsedefault = 1; /*automatic MAGE launch 961117*/
           }
           if (ithscript == 0) 
           {
               Lrangecontrols = 1;  /*and do range controls*/
           }
           else
           {/*choice of built-in scripts*/
             Lrangecontrols = 0; /*not expected to need range controls*/
             switch (ithscript)
             {/*switch (ithscript)*/
                case  1: /*ca-ca, s-s, hets, all subunits */
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  caxl[n] = 1;
                  n=n+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  cssl[n] = 1;
                  n=n+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  htxl[n] = 1;
                  Lfirstsubunit = 0;
                  Lallsubunits = 1;
                  Bval[n] = 1; /*991124*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;
                case  2: /* mc, hb, all subunits 990402 */
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  mcxl[n] = 1;
                  n=n+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  hbxl[n] = 1;
                  Lfirstsubunit = 0;
                  Lallsubunits = 1;
                  Bval[n] = 1; /*991124*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;
                case  3: /* ca-ca and individual residues*/ /*931128 see below*/
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  /*ca-ca is built-in in PKINCRTL.c as first pass */
                  scxl[n] = 1;
                  resl[n]=1;
                  Lallaminoacids = 1; /*flag for doing all residues*/
                  Lfirstsubunit = 0;  
                  Lallsubunits = 1;  /*all subunits 990402*/
                  Bval[n] = 1; /*991124*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;
                case  4: /*old default: -lots mc,sc,ca,hy,ht,all subunits 990402*/
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  mcxl[n] = 1;
                  scxl[n] = 1;
                  caxl[n] = 1;
                  hyxl[n] = 1;
                  htxl[n] = 1;
                  Lfirstsubunit = 0;
                  Lallsubunits = 1;
                  Bval[n] = 1;
                  waxl[n] = 1;
                  waal[n] = 1;
                  Lgetatommarker = 1;
                  Lhylens = 0; /*1:990403,0:050327*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                  if(colorwavec[0] == '\0')
                  {/*water color not yet given, use -lots default*/
                     sprintf(colorwavec,"peachtint");
                  }
                break;
                case  6: /*naba nucleic lots with individual bases */
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  mcxl[n] = 1;
                  scxl[n] = 1;
                  caxl[n] = 1;
                  hyxl[n] = 1;
                  htxl[n] = 1;
                  resl[n]=1;
                  Lallbases = 1; /*flag for doing all residues*/
                  Lnucleicacid = 1;
                  Lfirstsubunit = 0;
                  Lallsubunits = 1;
                  Bval[n] = 1; /*991124*/
                  waxl[n] = 1;
                  waal[n] = 1;
                  Lhylens = 0; /*1:990403,0:050327*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;
                case  7: /*pseudo-backbone and individual bases + atom markers*/
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  scxl[n] = 1;
                  resl[n]=1;
                  atxl[n]=1; /*atom markers, general*/
                  mcal[n]=1; /*atom markers mc */
                  scal[n]=1; /*atom markers sc */
                  htal[n]=1; /*atom markers ht */
                  Lallbases = 1; /*flag for doing all residues*/
                  Lnucleicacid = 1;
                  Lfirstsubunit = 0;
                  Lallsubunits = 1;
                  Bval[n] = 1; /*991124*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;

                case  9: /* ca-ca and individual residues and ranges*/
                  n = mrange+1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  /*ca-ca is built-in in PKINCRTL.c as first pass */
                  caxl[n] = 1; /*931128 for completeness*/
                  caxl[0] = 1; /*931128 for completeness*/
                  scxl[n] = 1;
                  scxl[0] = 1; /*931128 so ranges will have a beginning entry*/
                  resl[n]=1;
                  resl[0]=1; /*931128 so ranges will have a beginning entry*/
                  Lallaminoacids = 1; /*flag for doing all residues*/
                  Lrangecontrols = 1; /*also use range controls*/
                  Lfirstsubunit = 0;  
                  Lallsubunits = 1;  /*all subunits 990402*/
                  Bval[n] = 1; /*991124*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;
                case  10: /*Table group, built on Calpha list */
                   mrange = 1; /*have to start over to do a table*/
                   n = 1;
                   resnu1[n] = -999;
                   resnu2[n] = 9999;
                   caxl[n] = 1;
                   Bval[n] = 1; /*must match cass*/
                   Ltable = 1;
                   Lfirstsubunit = 0;
                   Lallsubunits = 1; /*all subunits is probably nonsense*/
                break;

                case  8: /*ribbon fudged from local geometry */
                  LsplineVRML = 1; /*as a possibility, needs LdumpVRML to act*/
                  n = mrange+1;
                  Lribbon = 1;
                  Lgetribbonparam = 1;
                  resnu1[n] = -999;
                  resnu2[n] = 9999;
                  ribl[n] = 1;
                  Lfirstsubunit = 0;
                  Lallsubunits = 1;
                  Lexplicitribbon = 0;
                  explicitribbontype = 0;
                  explicitribbonidentity[0] = '\0'; /*default: no ID  060929*/
                  Lvariablewidthribbon = 1;
                  Lonestrandcoil = 0;
                  Lbetaarrows = 0;
                  Lfudgedwidth = 1;
                  Lendribbon = 0; /*not using Range Controls for ribbon*/
                  mrange = n;
                  Ltable = 0; /*010511*/
                break;
                case  5: /*arrows == bestribbon ribbon from PDB HEADER records*/
                   LarrowVRML = 1; /*as a possibility, needs LdumpVRML to act*/
                   /*arrows with fat, single spline coil*/
                   /*old idea of variable width coil never really worked*/
                   /*and coil as single spline preferred anyway*/
                   /*Lribonucleic is set in parser or EndBuiltinScriptsDialog*/
                   Lbestribbon = 1;
                   Lautoribbon = 1; 
                      /*specific control to set ranges for each subunit*/
                   Lribbon = 1;
                   Lgetribbonparam = 1;
                   Lexplicitribbon = 1;
                   explicitribbontype = 3;/*COIL*/
                   explicitribbonidentity[0] = '\0'; /*default: no ID  060929*/
                   Lonestrandcoil = 1;/*040924 only difference from bestribbon*/
                     /*041106 Lonestrandcoil set during processing anyway!*/
                   Lbetaarrows = 1;
                   Lfudgedwidth = 0;
                   Ledgedribbon = 1;
                   Lcrosstie = 0;
                   Lskeinedribbon = 0;
                   ribwidcoil = 1;/*explicit range ribbon with beta arrows:*/
                                  /* before 050206 was coil==0*/
/*   ????  050731      mrange = 0;*/ /*have to start over to do autoribbons*/
                   n = mrange+1;
                   ribl[n] = 1;/*default to coil unless specific alpha or beta*/
                   resnu1[n] = -999;
                   resnu2[n] = 9999;
                   Lfirstsubunit = 0;
                   Lallsubunits = 1;
                   Lendribbon = 0; /*not using Range Controls for ribbon*/
                   mrange = n;
                   Ltable = 0; /*010511*/
                   Lblackedge = 1; /*041104*/
                   /*move parser stuff here so can just refer to this option*/
                   /* do this at parser stage choice: Lribonucleic = 0?1;*/
                   Lnucleicarrows = 1;
                   Lc4primenucleics = 1;
                   Lcurvednucleics = 1;
                break;
             }/*switch (ithscript)*/
           }/*choice of built-in scripts*/
       }/*built-in scripts*/
       Ldetails = 1; /*go on to do details for this pass, or finish details */
       
       if(!Lbrowsedefault)
       {
          Lconditions = 1; /*give user chance to select different subunits*/
          /* Lfocusoption set above if script requires it*/
       }
}
/*___builtinscripts_CB()________________________________________________________*/

/****startarange()************************************************************/
void startarange()
{
  static int ilabellist=0; /*used in Labellist coord pseudo-External script*/

  Lstartarange = 0;
  n = n + 1;   /*n could be >0, e.g. set by builtin Script*/
  mrange = n;  /*maximum number of ranges (anticipated)*/
  if(n >= MAXR-1)
  {
     sprintf(alertstr,"number of ranges exceeds %d,"
                      " excess ignored",MAXR-1);
     DoReportDialog();
     nrange = 0;
     mrange = n-1;
  }
  else
  {/*start-a-range*/
     Lstartarange = 0;
    line = 1;   /* flag for input line in for this nth range */
    idoline = 0;  /* processing flag for line */
    if(Lscriptin == 1)
    {/*input from any script file */
      
      idoline = 0;
      while(idoline==0)
      {/*while idoline==0*/
        line = Getonescriptline();
        idoline = line;  /* set to process, but check for comment line: */
        if(Llabellist==1)
        {
            if(ilabellist==0)
            {/*not started reading coord yet*/
                if (texts[0]=='@'&&texts[1]=='l'
                  &&texts[2]=='a'&&texts[3]=='b')
                {/*labellist script, read in until find @labellist */
                    ilabellist = 1;
                    idoline = 0;  /* reset so can get next line */
                    line = 0;
                    scxl[0] = 1;/*so first choice will be sidechains*/
                }
                if(ilabellist==0)
                {
                    idoline = 0;/* reset so can try another line */
                    line = 0;
                }
            }
        }
        else if (texts[0] != ' ')  
        {/*script comment lines start with non-blank char*/
             idoline = 0;  /* reset so can try another line */
             line = 0;
        }
        if( atEOF == 1 )
        {
          Lscriptin = 0;
          Llabellist = 0;
          sprintf(alertstr,"reached end of script"CRLF);
          DoReportDialog();
          idoline = 1; /*break out of loop no matter what state line is*/
        }
        if(line)
        {
          if(Llabellist)
          {
             if(dot3numbertexts() )
             {
                 /* read the line for content as 3 coordinates */
                 sscanf(temps,"%f %f %f"
                        ,&fxyz[n][0],&fxyz[n][1],&fxyz[n][2]);
                 /*set flag for this range*/
                 xyzl[n] = 1;
             }
             else
             {
                 sprintf(alertstr,"did not find 3 dot coord"CRLF);
                 DoReportDialog();
                 idoline = 0;  /* reset so can try another line */
                 line = 0;             
             }
          }
          else
          {
             cleanuptexts();
             /* read the line for content as control fields */
             sscanf(texts,"%d %d %s %s %s %s %s %s %s %s %s "
               "%s %s %s %s %s %s %s %s %s %s %s %s"
              ,&nn1,&nn2,chr1,chr2,chr3,chr4,chr5,chr6,chr7,chr8,chr9
              ,resin,chrA,chrB,chrC,chrD,chrE,chrF,chrG,chrH,chrI,chrJ,chrK);
          }
          Linein = 0;
          itext = 0;
       }
       if(line == 1 && Llabellist==0)    
       {/* script stuff coming in */
         /* interpret and store this line as range controls */
         mrange = n;
         nrange = 1;
         resnu1[n] = nn1;
         resnu2[n] = nn2;
         mcal[n] = 0;
         scal[n] = 0;
         htal[n] = 0;
         waal[n] = 0;
         resl[n] = 0;
         cssl[n] = 0;
         mutl[n] = 0;
         rotl[n] = 0;
         supl[n] = 0;
         ctxl[n] = 0;   
         mcCPKl[n] = 0;
         scCPKl[n] = 0;
         htCPKl[n] = 0;    

/* static  char  mc[] = "mc"; 1 mcxl*/ /*mainchain*/
/* static  char  sc[] = "sc"; 2  scxl*/ /*sidechain*/
/* static  char  hb[] = "hb"; 3  hbxl*/ /*mc H-bonds*/
/* static  char  hy[] = "hy"; 4  hyxl*/ /*hydrogens*/
/* static  char  ca[] = "ca"; 5  caxl*/ /*pseudo-backbone*/
/* static  char  ht[] = "ht"; 6  htxl*/ /*het atoms*/
/* static  char  wa[] = "wa"; 7  waxl*/ /*waters*/
/* static  char  at[] = "at"; 8  atxl*/ /*atom markers*/
/* static  char  lb[] = "lb"; 9  lbxl*/ /*residue labels*/
/* static  char  ct[] = "ct"; A  ctxl*/ /*atom markers on C */
/* static  char  mK[] = "mK"; B  mcCPKl*/ /*CPKs instead*/
/* static  char  sK[] = "sK"; C  scCPKl*/ /*CPKs instead*/
/* static  char  hK[] = "hK"; D  htCPKl*/ /*CPKs instead*/
/* static  char  ro[] = "ro"; E  rotl*/ /*rotation if same resname, else mut*/
/* static  char  Bc[] = "Bc"; F  Bcol*/ /*colored by B val*/
/* static  char  Bv[] = "Bv"; G  Bval*/ /* pointID Bval&Occ*/
/* static  char  rb[] = "rb"; H  ribl*/ /*scope of ribbon*/
/* static  char  al[] = "al"; I  Alpha*/ /*Alpha range*/
/* static  char  be[] = "be"; J  Beta*/ /*Beta range*/
/* static  char  co[] = "co"; K  Coil*/ /*Coil range*/



         if (chr1[0] == 'm' && chr1[1] == 'c') mcxl[n] = 1;
         if (chr2[0] == 's' && chr2[1] == 'c') scxl[n] = 1;
         if (chr3[0] == 'h' && chr3[1] == 'b') hbxl[n] = 1;
         if (chr4[0] == 'h' && chr4[1] == 'y') hyxl[n] = 1;
         if (chr5[0] == 'c' && chr5[1] == 'a') caxl[n] = 1;
         if (chr6[0] == 'h' && chr6[1] == 't') htxl[n] = 1;
         if (chr7[0] == 'w' && chr7[1] == 'a') waxl[n] = 1;
         if (chr8[0] == 'a' && chr8[1] == 't') atxl[n] = 1;
         if (chr9[0] == 'l' && chr9[1] == 'b') lbxl[n] = 1;
         if (resin[0] != '.'&& resin[1] != '.'&& resin[2] != '.')resl[n]=1;
           /* ... used as NO residue choice */
         resid[n][0] = resin[0];
         resid[n][1] = resin[1];
         resid[n][2] = resin[2];
         resid[n][3] = '\0';
         if (chrA[0] == 'c' && chrA[1] == 't') ctxl[n] = 1;     
         if (chrB[0] == 'm' && chrB[1] == 'K') mcCPKl[n] = 1; 
         if (chrC[0] == 's' && chrC[1] == 'K') scCPKl[n] = 1; 
         if (chrD[0] == 'h' && chrD[1] == 'K') htCPKl[n] = 1;     
         if (chrE[0] == 'r' && chrE[1] == 'o') rotl[n] = 1;     
         if (chrF[0] == 'B' && chrF[1] == 'c') Bcol[n] = 1;     
         if (chrG[0] == 'B' && chrG[1] == 'v') Bval[n] = 1;     
         if (chrH[0] == 'r' && chrH[1] == 'b') ribl[n] = 1;     
         if (chrI[0] == 'a' && chrI[1] == 'l') Alpha[n] = 1;     
         if (chrJ[0] == 'b' && chrJ[1] == 'e') Beta[n] = 1;    
         if (chrK[0] == 'c' && chrK[1] == 'o') Coil[n] = 1;    
         
        }/* script stuff coming in */
       
      }/*while idoline==0*/
      /*script line in and interpreted*/
    }/*input from any script file */
    if(Lscriptin == 0 || (Lscriptin && !Laccept) || Lmodify)
    {/* input from dialog boxes */
      if(Llabellist==1) 
         sprintf(newstr,"Output when find coord: %s" ,texts);
      else if(Lscriptin==1) 
         sprintf(newstr,"Accept or Modify script");
      else 
         sprintf(newstr,"New Range ...");
         LRCDialogInput = 1; /*DoRangeControlDialog();*/
             /*successful dialog will set Lendarange=1*/
    }/* input from dialog boxes */
    else
    {
        LRCDialogInput = 0; /*avoids dialog Range Controls*/
        Lendarange = 1; /*directly to process script line*/
    }
  }/*start-a-range*/
}
/*___startarange()___________________________________________________________*/

/****endarange()**************************************************************/
void endarange()
{/*comes here after gets one range of info from script or dialog */
  Lendarange = 0;
  Lflag = 0; /*error or exception flag for this range*/
  if(nrange==0) Lflag = 1; /*for end of range control selection*/
  else if(nrange<0) Lflag = -1;/*backup a range, i.e. undo last range*/
/*printf("endarange: n== %d, nrange== %d, sets Lflag== %d\n",n,nrange,Lflag);*/
  if(Lflag >= 0)
  {/*normal flow, Lflag==0: continuing input, Lflag==1: last run*/
     /*050928 make ribbon controls if---elseif sequence */
     if(Best[n])
     {
       LarrowVRML = 1; /*as a possibility, needs LdumpVRML to act*/
       Lbestribbon = 1;
       Lribonucleic = 1;
       /*from PKINRNGE commanded==5, comment out inappropriate range info*/
                   Lbestribbon = 1;
                   Lautoribbon = 1;
                      /*specific control to set ranges for each subunit*/
                   Lribbon = 1;
                   Lgetribbonparam = 1;
                   Lexplicitribbon = 1;
                   explicitribbontype = 3;/*COIL */
                   explicitribbonidentity[0] = '\0'; /*default: no ID  060929*/
                   Lonestrandcoil = 1;/*040924 only difference from bestribbon*/
                     /*041106 Lonestrandcoil set during processing anyway!*/
                   Lbetaarrows = 1;
                   Lfudgedwidth = 0;
                   Ledgedribbon = 1;
                   Lcrosstie = 0;
                   Lskeinedribbon = 0;
                   ribwidcoil = 1;/*explicit range ribbon with beta arrows:*/
                                  /* before 050206 was coil==0*/
                 /*mrange = 0;*/ /*have to start over to do autoribbons*/
                 /*n = mrange+1;*/
               /*ribl[n] = 1;*//*default to coil unless specific alpha or beta*/
               /*Coil[n] = 1;*//*default to coil unless specific alpha or beta*/
                 /*resnu1[n] = -999;*/
                 /*resnu2[n] = 9999;*/
                   /*Lfirstsubunit = 0; */  /*set these in GUI...*/
                   /*Lallsubunits = 1;  */
                   Lendribbon = 0; /*not using Range Controls for ribbon ????*/
                 /*mrange = n;*/
                   Ltable = 0; /*010511*/
                   Lblackedge = 1; /*041104*/
                   /*move parser stuff here so can just refer to this option*/
                   /* do this at parser stage choice: Lribonucleic = 0?1;*/
                   Lnucleicarrows = 1;
                   Lc4primenucleics = 1;
                   Lcurvednucleics = 1;
     }
     else if(ribl[n])
     {/*generic ribbon controls for ribl split out 050206*/
            Lribbon = 1;
            Lgetribbonparam = 1;
            Lfirstsubunit = 0;
            Lallsubunits = 1;
            Lexplicitribbon = 0;/*050206with full RibbonControl choice*/
            explicitribbontype = 0;
            explicitribbonidentity[0] = '\0'; /*default: no ID  060929*/
            Lonestrandcoil = 0;
            Lbetaarrows = 0;
            Lfudgedwidth = 1;
            Lendribbon = 0; /*not using Range Controls for ribbon*/
            Lribbonalso = 1;/*050206 ribbon whole chain, e.g.with selected sc*/
            LsplineVRML = 1; /*as a possibility, needs LdumpVRML to act*/
     }

     else if(Alpha[n] || Beta[n] || Coil[n])
     {/*reset some param for specific Alpha,Beta control, 050206*/
        /*and add to whatever was read in as HELIX, SHEET in header 050928*/
         if(Nribs < MAXR-2)
         {
            Nribs++;
            ribresnu1[Nribs] = resnu1[n];
            ribresnu2[Nribs] = resnu2[n];
            ribAlpha[Nribs] = Alpha[n];
            ribBeta[Nribs] = Beta[n];
            ribCoil[Nribs] = Coil[n];
            ribchain[Nribs][0] = '+'; /*match any, 2char 070926*/
            ribchain[Nribs][1] = '+'; /*match any, 2char 070926*/
            ribchain[Nribs][2] = '\0'; /* 2char 070926*/
          
            Lribbon = 1;
            Lgetribbonparam = 1;
            Lexplicitribbon = 1;
            explicitribbontype = 3;/*COIL */
            explicitribbonidentity[0] = '\0'; /*default: no ID  060929*/
            Lonestrandcoil = 1;
            Lbetaarrows = 1;
            Lfudgedwidth = 0;
            Ledgedribbon = 1;
            Lcrosstie = 0;
            Lskeinedribbon = 0;
            ribwidcoil = 1; /*explicit range ribbon with beta arrows: */
                             /* before 050206 was coil==0*/
         }
     }

     if(atxl[n])
     {
            if(mcxl[n]) mcal[n] = 1;
            if(scxl[n]) scal[n] = 1;
            if(htxl[n]) htal[n] = 1;
            if(waxl[n]) waal[n] = 1;
            Lgetatommarker = 1;
            LsphereVRML = 1; /*as a possibility, needs LdumpVRML to act*/
     }
     if(ctxl[n]) Lgetatommarker = 1; /*completeness, usually +atxl+mcxl*/

     if(scxl[n]){LvectorVRML = 1;} /*as a possibility, needs LdumpVRML to act*/

     if(resl[n])
     {/*res name given*/
            LvectorVRML = 1; /*as a possibility, needs LdumpVRML to act*/

            /*special case of css: do cys sc all-atom pairs as well as -ss-*/
            if(resid[n][0]=='c' &&resid[n][1]=='s' &&resid[n][2]=='s')
            {
                resid[n][1] = 'y'; /*really look for cys */
                cssl[n] = 1; /*flag for doing all-atom-sc-ss- to this cys*/
            }

            /*special case of all: do all residues in turn*/
            if(n == 1 && resid[n][0]=='a' &&resid[n][1]=='l' &&resid[n][2]=='l')
            {
                /*resid[1][0,1,2] will be set later  */
                Lallaminoacids = 1; /*flag for doing all residues*/
            }
            /*special case of just name for one residue specified, retained*/
            /* for consistency with earlier versions before explicit mutation*/
            if( rotl[n] )
            {/* mutation to the named residue will be made*/
                /*and sc will be set up for CHI rotations*/
                mutl[n] = 1;
                /*scan to see if rotation or mutation specified earlier */
                for(j=1; j<n; j++) if(mutl[j]!=0 || rotl[j]!=0)
                {
                     Lflag=1;
                     sprintf(alertstr,"One bond rotations and/or mutation"
                       " already accepted for this pass, try again.");
                     mutl[n] = 0;
                     rotl[n] = 0;
                }
                /*make sure single residue specified*/
                if( resnu1[n] != resnu2[n] ) 
                { 
                     Lflag = 1;
                     sprintf(alertstr,"Starting and ending residue numbers"
                       " must be the same for mutation and/or bond rotations, "
                       "try again.");
                     mutl[n] = 0;
                     rotl[n] = 0;
                }    
            }/* mutation to the named residue will be made*/
            if( supl[n] )
            {/*superposition of all this named aa onto ideal std aa*/
                /*scan to see if superposition specified earlier */
                for(j=1; j<n; j++) if(supl[j]!=0)
                {
                     Lflag=1;
                     sprintf(alertstr,"One type of aa to be superpositioned"
                       " already accepted for this pass, try again.");
                     supl[n] = 0;
                }
            }/*superposition of all this named aa onto ideal std aa*/
     }/*res name given*/
     else
     {/*NOT a residue name*/
             if(rotl[n])
             {/*rotation requested*/
                if( resnu1[n] == resnu2[n] )
                {/*just one residue specified*/
                   /*residue sc will be set up for CHI rotations*/
                   /*scan to see if rotation or mutation specified earlier */
                   for(j=1; j<n; j++) if(mutl[j]!=0 || rotl[j]!=0)
                   {
                       Lflag=1;
                       sprintf(alertstr,"One bond rotations and/or mutation"
                         " already accepted for this pass, try again.");
                       mutl[n] = 0;
                       rotl[n] = 0;
                    }
                }/*just one residue specified*/
                else
                { 
                     Lflag = 1;
                     sprintf(alertstr,"Starting and ending residue numbers"
                       " must be the same for mutation and/or bond rotations, "
                       "try again.");
                     mutl[n] = 0;
                     rotl[n] = 0;
                }    
             }/*rotation requested*/
     }/*NOT a residue name*/
     nn1 = resnu1[n];
     nn2 = resnu2[n];
     strcpy(chr1,blank2);
     strcpy(chr2,blank2);
     strcpy(chr3,blank2);
     strcpy(chr4,blank2);
     strcpy(chr5,blank2);
     strcpy(chr6,blank2);
     strcpy(chr7,blank2);
     strcpy(chr8,blank2);
     strcpy(chr9,blank2);
     strcpy(resin,blank3);
     strcpy(chrA,blank2);
     strcpy(chrB,blank2);
     strcpy(chrC,blank2);
     strcpy(chrD,blank2);
     strcpy(chrE,blank2);
     strcpy(chrF,blank2);
     strcpy(chrG,blank2);
     strcpy(chrH,blank2);
     strcpy(chrI,blank2);
     strcpy(chrJ,blank2);
     strcpy(chrK,blank2);

/* static  char  mc[] = "mc"; 1 mcxl*/ /*mainchain*/
/* static  char  sc[] = "sc"; 2  scxl*/ /*sidechain*/
/* static  char  hb[] = "hb"; 3  hbxl*/ /*mc H-bonds*/
/* static  char  hy[] = "hy"; 4  hyxl*/ /*hydrogens*/
/* static  char  ca[] = "ca"; 5  caxl*/ /*pseudo-backbone*/
/* static  char  ht[] = "ht"; 6  htxl*/ /*het atoms*/
/* static  char  wa[] = "wa"; 7  waxl*/ /*waters*/
/* static  char  at[] = "at"; 8  atxl*/ /*atom markers*/
/* static  char  lb[] = "lb"; 9  lbxl*/ /*residue labels*/
/* static  char  ct[] = "ct"; A  ctxl*/ /*atom markers on C */
/* static  char  mK[] = "mK"; B  mcCPKl*/ /*CPKs instead*/
/* static  char  sK[] = "sK"; C  scCPKl*/ /*CPKs instead*/
/* static  char  hK[] = "hK"; D  htCPKl*/ /*CPKs instead*/
/* static  char  ro[] = "ro"; E  rotl*/ /*rotation if same resname, else mut*/
/* static  char  Bc[] = "Bc"; F  Bcol*/ /*colored by B val*/
/* static  char  Bv[] = "Bv"; G  Bval*/ /* pointID Bval&Occ*/
/* static  char  rb[] = "rb"; H  ribl*/ /*scope of ribbon*/
/* static  char  al[] = "al"; I  Alpha*/ /*Alpha range*/
/* static  char  be[] = "be"; J  Beta*/ /*Beta range*/



     if (mcxl[n]) strcpy(chr1,mc); /*std C strcpy() */
     if (scxl[n]) strcpy(chr2,sc);
     if (hbxl[n]) strcpy(chr3,hb);
     if (hyxl[n]) strcpy(chr4,hy);
     if (caxl[n]) strcpy(chr5,ca);
     if (htxl[n]) strcpy(chr6,ht);
     if (waxl[n]) strcpy(chr7,wa);
     if (atxl[n]) strcpy(chr8,at);
     if (lbxl[n]) strcpy(chr9,lb);
     if (resl[n]) strcpy(resin,resid[n]);
     if (ctxl[n]) strcpy(chrA,ct);
     if (mcCPKl[n]) strcpy(chrB,mK);
     if (scCPKl[n]) strcpy(chrC,sK);
     if (htCPKl[n]) strcpy(chrD,hK);
     if (rotl[n]) strcpy(chrE,ro);
     if (Bcol[n]) strcpy(chrF,Bc);
     if (Bval[n]) strcpy(chrG,Bv);
     if (ribl[n]) strcpy(chrH,rb);
     if (Alpha[n]) strcpy(chrI,al);
     if (Beta[n]) strcpy(chrJ,be);
     if (Coil[n]) strcpy(chrK,co);

     if (cssl[n]) resin[1] = 's';
              /*change cys back into css for script*/
              /*cssl can only be set if resl is cys specified*/
              /* originally as css*/
  }/*normal flow, Lflag==0: continuing input, Lflag==1: last run*/


  if(   Lflag==0 
             && (   mcxl[n] || scxl[n] || hbxl[n] || hyxl[n] || caxl[n]
                 || htxl[n] || waxl[n] || atxl[n] || lbxl[n] || resl[n] 
                 || mutl[n] || ribl[n] || Alpha[n] || Beta[n] || Coil[n] 
                 || Best[n] || cisl[n] ) )
  {/*Lflag==0, and actual entry for this range, go on to next one */

     for(j=0;j<256;j++) olderstr[j]=oldstr[j];
     sprintf(oldstr,"%3d : %4d %4d %2s %2s %2s %2s %2s %2s %2s %2s %2s "
                       "%3s %2s %2s %2s %2s %2s %2s %2s %2s %2s %2s %2s"
                ,n,nn1,nn2,chr1,chr2,chr3,chr4,chr5,chr6,chr7,chr8,chr9
                ,resin,chrA,chrB,chrC,chrD,chrE,chrF,chrG,chrH,chrI,chrJ,chrK);
     Lstartarange = 1;
  }
  else
  {/*no range specification given, error flag, or end requested*/ 
              if( nrange ) 
              {/*try to continue getting ranges*/
                  /*in middle of specifying ranges: alert user, try again*/
                  if( !Lflag) sprintf(alertstr,"nothing specified for this range,"
                                   " try again.");
                  DoReportDialog();
                  /*note: olderstr and oldstr remain as before*/
                  Lstartarange = 1;

                  n = n - 1;  /*reset n so redo this range*/
                  mrange = n; /*adjust maximum range number*/

              }/*try to continue getting ranges*/
              else /* nrange==0, user wants to quit anyway */
              {/*stop getting range info*/
                  /*mrange has been adjusted, if ==0 then no ranges*/
                  Lstartarange = 0;
                  if(Lopenscriptout) Lrangecontrols = 1; /*still keep this set*/
                  else               Lrangecontrols = 0; /*clear so can go on*/
                  Ldetails = 1; /*set so can do details to finish this pass*/
                  /*stay in getranges for details*/
                  Lgetranges = 1;
                  Lfocusoption = 1; /*user may want this also*/
              }/*stop getting range info*/
  }/*no range specification given, error flag, or end requested*/ 
  if(Lflag < 0)
  {/*backup one range, i.e. undo previous range*/
     nrange = 1;
     if(n>=2) n = n-2;
     else n = 0;
     mrange = n;
     for(j=0;j<256;j++) oldstr[j]=olderstr[j];
     for(j=0;j<256;j++) olderstr[j]='\0'; /*can see back only this far*/
     Lstartarange = 1;  
  }
}
/*___endarange()______________________________________________________________*/

/****scriptout()**************************************************************/
void scriptout(void)
{
 /*--after end of input had been requested */
 /*--script writing option, call after openscriptout file Dialog */
 Lscriptout = 0; /*clear flag*/
 /*Lrangecontrols = 0;*/ /*once through here, go on beyond rangecontrol*/
 Lrangecontrols = 1; /*still in the middle of rangecontrol*/
 Lfocusoption = 1; /*next item on rangecontrol routine*/
 for(n=1 ; n<=mrange ; n++)
 {/*over ranges*/
    nn1 = resnu1[n];
    nn2 = resnu2[n];
    strcpy(chr1,blank2);
    strcpy(chr2,blank2);
    strcpy(chr3,blank2);
    strcpy(chr4,blank2);
    strcpy(chr5,blank2);
    strcpy(chr6,blank2);
    strcpy(chr7,blank2);
    strcpy(chr8,blank2);
    strcpy(chr9,blank2);
    strcpy(resin,blank3);
    strcpy(chrA,blank2);
    strcpy(chrB,blank2);
    strcpy(chrC,blank2);
    strcpy(chrD,blank2);
    strcpy(chrE,blank2);
    strcpy(chrF,blank2);
    strcpy(chrG,blank2);
    strcpy(chrH,blank2);
    strcpy(chrI,blank2);
    strcpy(chrJ,blank2);
    strcpy(chrK,blank2);

    if (mcxl[n]) strcpy(chr1,mc); /*1  mcxl*/ /*mainchain*/
    if (scxl[n]) strcpy(chr2,sc); /*2  scxl*/ /*sidechain*/
    if (hbxl[n]) strcpy(chr3,hb); /*3  hbxl*/ /*mc H-bonds*/
    if (hyxl[n]) strcpy(chr4,hy); /*4  hyxl*/ /*hydrogens*/
    if (caxl[n]) strcpy(chr5,ca); /*5  caxl*/ /*pseudo-backbone*/
    if (htxl[n]) strcpy(chr6,ht); /*6  htxl*/ /*het atoms*/
    if (waxl[n]) strcpy(chr7,wa); /*7  waxl*/ /*waters*/
    if (atxl[n]) strcpy(chr8,at); /*8  atxl*/ /*atom markers*/
    if (lbxl[n]) strcpy(chr9,lb); /*9  lbxl*/ /*residue labels*/
    if (resl[n]) strcpy(resin,resid[n]);
    if (ctxl[n]) strcpy(chrA,ct); /*A  ctxl*/ /*atom markers on C */
    if (mcCPKl[n]) strcpy(chrB,mK); /*B  mcCPKl*/ /*CPKs instead*/
    if (scCPKl[n]) strcpy(chrC,sK); /*C  scCPKl*/ /*CPKs instead*/
    if (htCPKl[n]) strcpy(chrD,hK); /*D  htCPKl*/ /*CPKs instead*/
    if (rotl[n]) strcpy(chrE,ro); /*E  rotl*/ /*rotation if same resname, else mut*/
    if (Bcol[n]) strcpy(chrF,Bc); /*F  Bcol*/ /*colored by B val*/
    if (Bval[n]) strcpy(chrG,Bv); /*G  Bval*/ /* pointID Bval&Occ*/
    if (ribl[n]) strcpy(chrH,rb); /*H  ribl*/ /*scope of ribbon*/
    if (Alpha[n]) strcpy(chrI,al); /*I  Alpha*/ /*Alpha range*/
    if (Beta[n]) strcpy(chrJ,be); /*J  Beta*/ /*Beta range*/
    if (Coil[n]) strcpy(chrK,co); /*K  Coil*/ /*Coil range*/


    if (cssl[n]) resin[1] = 's';
       /*change cys back into css for script*/
       /*cssl can only be set if resl is cys specified*/
       /* originally as css*/
    fprintf(fpscriptout," %4d %4d %2s %2s %2s %2s %2s %2s %2s %2s %2s "
                        "%3s %2s %2s %2s %2s %2s %2s %2s %2s %2s %2s %2s"EOLO
            ,nn1,nn2,chr1,chr2,chr3,chr4,chr5,chr6,chr7,chr8,chr9
            ,resin,chrA,chrB,chrC,chrD,chrE,chrF,chrG,chrH,chrI,chrJ,chrK);
 }/*over ranges*/
 fclose(fpscriptout);
}
/*___scriptout()_____________________________________________________________*/
      
/****getatommarker()**********************************************************/
void getatommarker()
{/*atom markers*/
      Lgetatommarker = 0; /*clear flag*/
      Latmark = FALSE;
      atmarker = (float)0.1;
      for(n=1 ; n<=mrange ; n++)
      {/*test all ways an atom marker could be specified*/
         if(atxl[n]||mcal[n]||scal[n]||htal[n]||waal[n]||ctxl[n])
            Latmark = TRUE;
      }      
      if(Latmark)
      {/*atom mark size*/
          DoAtomMarkerDialog();
      }/*atom mark size*/
}/*atom markers*/
/*___getatommarker()_________________________________________________________*/

/****focusoption()************************************************************/
void focusoption()
{
    /*Lfocusoption stays set until all focus info accummulated*/
    Lscriptin = 0;  /* focus may or may not have a script file too */
    Lscriptfileopen = FALSE;
    FOVEA = FALSE;
    /* fovea, focus distances */
    
    DoFocusOptionsDialog();
}
/*___focusoption()___________________________________________________________*/

/****focusoptionend()********************************************************/
void focusoptionend()
{/*focusoptionend*/
  Lfocusoptionend = 0;

          if(Lscriptin) /*try to open the file*/
          {
/*I*/        while( ( Lflag = getnewfile(ScriptinStr,3) ) == 1 ) ;
                          /* 3 for fpscriptin, use here too */
             if( Lflag == 2) Lscriptin = 1;
             else Lscriptin = 0;
          }
          if(Lscriptin) /*opened the file, get x,y,z point */
          {/*focus point from a file*/
            Lscriptfileopen = TRUE;
            {/*input from script file*/ /*this set of braces unnecessary??*/
               idoline = 0;  /* processing flag for line */
               while( idoline == 0 )
               {/*just get one line with a coordinate triple */
/*I*/             line = Getonescriptline();
                  idoline = line;
                  /* set to process, but check for comment line: */
                  if (texts[0] != ' ')
                  /* comment lines start with non-blank char*/
                  {
                     idoline = 0;  /* reset so can try another line */
                  }
                  else  /* see if line has 3 number fields */
                  {
                      nfields = 0;
                      infield = 0;
                      for(j=1;j<256;j++)
                      {
                          if(  texts[j]=='\n'||texts[j]=='\r') break;
                          if(   texts[j]=='0'||texts[j]=='1'||texts[j]=='2'
                              ||texts[j]=='3'||texts[j]=='4'||texts[j]=='5'
                              ||texts[j]=='6'||texts[j]=='7'||texts[j]=='8'
                              ||texts[j]=='9'||texts[j]=='-'||texts[j]=='.'
                              ||texts[j]=='+' )
                          {
                              if(!infield) {infield=1;nfields++;}
                          }
                          else
                          {
                              infield=0;
                          }
                      }
                      if(nfields < 3)
                      {
                          idoline = 0;  /* reset so can try another line */
                      }
                  }
                  if( atEOF == 1 )
                  {
                     Lscriptin = 0;
/*c*/                sprintf(alertstr,
                       "reached end of file looking for focus point");
                     idoline = 1;
                     /* break out of loop no matter what state line is*/
                  }
               }/*just get one line with a coordinate triple */
            }/*input from script file */
            if(Lscriptfileopen) fclose(fpscriptin);
              /* close script file if there was one */
            /* read the line for content as control fields */
            fx = 0.0; fy = 0.0; fz = 0.0;
            /*sometimes this input doesn't work*/
/*c*/       sscanf(texts,"%f %f %f", &fx,&fy,&fz);
            focus[1] = fx;
            focus[2] = fy;
            focus[3] = fz;
          }/*focus point from a file*/

          Lfocusdistance = 1;
}
/*___focusoptionend()________________________________________________________*/

/****finishranges()***********************************************************/
void finishranges()   /*971029*/
{
    if(Lneedsubunits){ Lallsubunits=0;  Lfirstsubunit=0;}

    /* load gflag character string for control of subunit acceptance */
    /*Lallsubunits*/ /*set up as a default*/
    for(j=1; j<256 ; j++) gflag[j] = ' ';  /* create null set */
    gflag[0] = '1';  /* flag to do all subunits, CR here also means this */
    gflag[255] = '\0'; /* can treat this array as a string */
    Lonesubunit = 0; /*could be more than one subunit*/
    
    if(Lfirstsubunit) /* set for only the first */
    {
       gflag[0] = ' ';  /* set to accept flagged subunit controls */
       gflag[1] = '1';  /* do first subunit */
       for(j=2; j<256 ; j++) gflag[j] = '_';  /* none of the rest */
       gflag[255] = '\0'; /* can treat this array as a string */
       Lonesubunit = 1;
    }
    else if(Lneedsubunits)
    {/*Lneedsubunits*/
          /*    "a set of 1 digit flags controls which subunits or "   */
          /*    "NMR models accepted"CRLF                              */
          /*    "use 1 to flag each subunit or model to be scanned"CRLF*/
          /*    " null set does all coordinates in file.  "CRLF        */
          /*    "  Blanks & zeros are nulls. CR or _ ends scanning"CRLF*/

           if(beginsubunit <= endsubunit && endsubunit <254 )
           {
               for(j=0; j<beginsubunit; j++) gflag[j] = ' ';
               for(j=beginsubunit ; j<=endsubunit ; j++) gflag[j] = '1';
               for(j=endsubunit+1; j<255; j++) gflag[j] = '_';
               gflag[255] = '\0';
               if(beginsubunit == endsubunit) Lonesubunit = 1;
               else Lonesubunit = 0;
           }
           else
           {
               sprintf(alertstr,"problems with subunit specification"
                 CRLF" begin subunit = %d,  end subunit = %d"
                 ,beginsubunit,endsubunit);
           }
    }/*Lneedsubunits*/
    
    Lfinishranges = 0;
    Lnewpassflow = 1; 
}
/*___finishranges()_____________________________________________________________*/

