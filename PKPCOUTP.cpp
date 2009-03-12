#include "PKIN.h"
#include "PKINDLOG.h"

/****openoutput()**********************************************************/
int openoutput(void)
{
    int wAction,ireturn,iaccept;

    ireturn = 0; /*no kin file opened yet*/
    
    iaccept = 0;
    while(iaccept == 0)
    {/*iaccept==0*/
        iaccept = getoutfilename(OutfileStr, 1);
        if (iaccept == 0) wAction = IDCANCEL;
        else  /*iaccept==1*/
        {   /*use PCWindows routine to test for existance */
            kinfile = OpenFile(OutfileStr,&kinofstruct,OF_EXIST);
			ireturn = 1;
        }
        if(wAction == IDCANCEL)
        {
			/* reset the caption to default, since no pass available */
			SetTextCaption(PrekinWindow,"Prekin");
            ireturn = 0;
        }
        iaccept = 1;
    }/*iaccept==0*/

	if (ireturn)
	{
		/*if a previous file is open, close it*/
		if(fpoutput != NULL) fclose(fpoutput); 

        /*now use vanilla C way to really open the file*/
        if ((fpoutput = fopen(OutfileStr,"wb")) == NULL)   /*c*/
        {
			sprintf(alertstr,"Prekin failed to open file: %s          ",OutfileStr);
            DoReportDialog();
            ireturn = 0;
        }
        else /* file opened, write header into it */
        {
            if(Ltestprint)
            {
	            sprintf(alertstr,"opened file: %s          ",OutfileStr);
                DoReportDialog();
            }
			/* file opened, prepare to write header into it*/
			Ldid_outfiletext = 0; /*DO NOT do the @text yet */
			/*wait until chance to get Header stuff from PDB file*/
			inittextblock(&headerstuff); /*PKINSCRT.c*/
			/* disposetextblock(&headerstuff);*/ /*PKINSCRT.c*/ 
			Lnewpass = 1;
			AdjustMenus();  /*____MENU.C*/
			Lfpoutput = 1; /*flag for successfully opened fpoutput*/
			Lgot_outfilename = 1; /*to keep book */
			Lgot_outfileopen = 1; /*to keep book */
			ireturn = 1;
		}
	}
    return(ireturn);
}
/*___openoutput()___________________________________________________________*/

/****openscriptout()*********************************************************/
int    openscriptout(void)
{
    int wAction,ireturn,iaccept,ibreak;

    ireturn = 0; /*no scp file opened yet*/
    Lscriptout = 0;

    iaccept = 0;
    while(iaccept == 0)
    {/*iaccept==0*/
        iaccept = getoutfilename( OutfileStr, 2);

        if (iaccept == 0) wAction = IDCANCEL;
        else  /*iaccept==1*/
        {
            scriptoutfile = OpenFile(OutfileStr,&scriptoutofstruct,OF_EXIST);
        }
        if(wAction == IDCANCEL)
        {
                sprintf(alertstr,"No script file selected");
                DoReportDialog();
                ibreak = 1;
                iaccept = 0;
        }
        else
        {
            ibreak = 1;
            iaccept = 1;
        }
    }/*ibreak==0*/
    if(iaccept==1)
    {
        /*if a previous file is open, close it*/
/*c*/   if(fpscriptout != NULL) fclose(fpscriptout); 

/*c*/   if ((fpscriptout = fopen(OutfileStr,"wb")) == NULL)
        {
            ireturn = 0;
/*c*/       sprintf(alertstr,"Prekin failed to open output script file:\n %s"
                 ,OutfileStr);
            DoReportDialog();
        }
        else  /* file opened, write header into it */
        {
/*c*/        fprintf(fpscriptout,"{script for coord file: %s }"EOLO,NameStr);
             ireturn = 1;
             Lscriptout = 1;
        }
    }
    return(ireturn);
}
/*___openscriptout()________________________________________________________*/
    
