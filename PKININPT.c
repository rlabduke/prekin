/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/*                    PKININPT.c                       */
#include "PKIN.h"
#include "PKINDLOG.h"

/****textstringin()**********************************************************/
void   textstringin(void)
{
  
  texts[itext] = charin;
  if(charin == '\r' || charin == '\n')
  {
    texts[itext+1] = '\0';
    /*make array of characters into properly ended C string*/

    itext = 0;
    Linein = 1;   /* flag for completed line */
  }
  else if(charin == '\010' || charin == '\020' || charin == '\034')
  {
    itext = itext - 1; /* bs or dle or <- delete one character */
    if(itext < 0) itext = 0;
    
  }
  else
  {
    itext = itext + 1;
  }
}
/*___textstringin()_________________________________________________________*/

/****Getonescriptline()******************************************************/
int    Getonescriptline(void)
{
      int   c;
      int   ireturn;

      for(itext = 0 ; itext <256 ; itext++) texts[itext] = '\0';
      itext = 0;
/*c*/ while ((c = fgetc(fpscriptin)) != EOF)
      {
        if (c == '\r' || c == '\n') 
        {
            if(IPC)  c = '\n';  /* standardize newline char */
            if(IMAC) c = '\r';
        }
        texts[itext] = c;
        itext++;
        if (itext > 254)
        {
          texts[itext] = '\0'; 
          /*make array of characters into properly ended C string*/
          pkintextinsert(texts);  /*PKMCTEXT.C PKPCTEXT.C*/
          adjusttext(0);             /*PKMCTEXT.C PKPCTEXT.C*/
          itext = 0;
          /* Warn the reader about line may be too long: */
/*c*/     sprintf(alertstr,"Line just printed is at limit of character string!");
/*Ms*/    DoReportDialog();

        }
        if (c == '\n' || c == '\r')  /* line in, process it */
        {
          texts[itext] = '\0'; 
          /*make array of characters into properly ended C string*/
          atEOF = 0;    /* not at EOF */
          ireturn = 1;  /* line in is true */  
          return(ireturn);
        }      
      }  /* loop while file not at EOF */
      /* finish out anything in text buffer */
      atEOF = 1;    /* EOF */
      ireturn = 0;
      texts[itext] = '\0';
      if(itext > 1)
      {
        ireturn = 1;    /* line in is true */
      }
      return(ireturn);
}
/*___Getonescriptline()_____________________________________________________*/

/****Getoneinputline()*******************************************************/
int    Getoneinputline(void)
{
   int   c,ireturn;

   for(itext=0; itext<256; itext++) texts[itext] = ' ';
   /*this will avoid left over garbage at ends of short records*/
   /*ATOM and HETATM records should always have at least the x,y,z coords*/
   itext = 0;
   texts[0] = '\0';/*initially a NULL string*/
   if(!ihitend)
   {
      while ((c = fgetc(fpinput)) != EOF)
      { 
         if (c == '\r' || c == '\n') 
         {
            texts[itext] = '\0';
           
            atpdbEOF = 0;    /* not at EOF */
            strcat(texts,EOLO);
            if(itext < 2) ireturn = 0;
            else ireturn = 1; /*return 1 for MOD, TER, END, etc. */
                             /*as well as ATOM and HETATM */
            return(ireturn);  /* break out of this subroutine */
         }
         else
         {
            texts[itext] = c;
            if (itext > 254)
            {
               texts[itext+1] = '\0';
               /*make array of characters into properly ended C string*/
               pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
               adjusttext(0);             /*PKMCTEXT.C PKPCTEXT.C*/
               itext = 0;

               /* Warn the reader about line too long for std PDB: */
               sprintf(alertstr,"Line just printed is too long for std PDB format!");
               DoReportDialog();
            }
            else itext++;
         }   
      }/* loop while file not at EOF */
      /* finish out anything in text buffer */
      /*if line is long enough to be coords*/
   }/*bypass for EOF on previous line*/
/*123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_*/
/*atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89*/
/*atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90*/
   if(itext > 50) /*enough characters to have content on this last line*/
   {
       ireturn = 1;
       atpdbEOF = 0; /*pretend not EOF, let next pass catch it*/
       ihitend = 1; /*but store info that EOF was hit*/
       texts[itext] = '\0';
   }
   else
   {
       ireturn = 0; /*not a valid line of any sort*/
       atpdbEOF = 1;  /* EOF */
       ihitend = 1;
   }
   return(ireturn);
}
/*___Getoneinputline()______________________________________________________*/

/****Getonescratchline()*****************************************************/
int    Getonescratchline(void)
{

    int   ireturn;

    getonetextblockline(&mainscratch,temps); /*PKINSCRT*/
    if(temps[0]=='\0')
    {
        atEOF = 1;  /* EOF */
        ireturn = 0;
    }
    else
    {   
        atEOF = 0;  /* not at EOF */
        ireturn = 1;
    }
      return(ireturn);
}
/*___Getonescratchline()____________________________________________________*/

/****Getoneaalibline()*******************************************************/
int    Getoneaalibline(void)
{

      int   c,ireturn;

      itemp = 0;
      if(!ihitaalibend)
      {
/*c*/    while ((c = fgetc(fpaalib)) != EOF)
        { 
          if (c == '\r' || c == '\n') 
          {
            if(IPC)       c = '\n';  /* standardize newline char */
            else if(IMAC) c = '\r';
            else          c = '\n';  /* standardize newline char */
          }
          temps[itemp] = c;
          if (itemp > 254)
          {
            temps[itemp+1] = '\0'; 
            /*make array of characters into properly ended C string*/
            pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
            adjusttext(0);             /*PKMCTEXT.C PKPCTEXT.C*/
            itemp = 0;
            /* Warn the reader about line too long for std PDB: */
/*c*/       sprintf(alertstr,"Line just printed is too long for std PDB format!");
/*Ms*/      DoReportDialog();
        
          }
          else
          {
            itemp++;
          }
          if (c == '\n' || c == '\r')  /* line in, process it */
          {
            temps[itemp] = '\0';
            ihitaalibend = 0; /*line ended with CR, not EOF*/
            ireturn = 1;
            return(ireturn);  /* break out of this subroutine */
          }      
        }/* loop while file not at EOF */
        /* finish out anything in temp buffer */
        /*if line is long enough to be coords*/
      }/*bypass for EOF on previous line*/
/*123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_*/
/*atom      1  n   arg a   1      26.465  27.452  -2.490  1.00 25.18      4pti  89*/
/*atom      2  ca  arg a   1      25.497  26.862  -1.573  1.00 17.63      4pti  90*/
      if(itemp > 50) /*enough characters to have content on this last line*/
      {
          ireturn = 1;
          ihitaalibend = 1; /*but store info that EOF was hit*/
          temps[itemp] = '\0';
      }
      else
      {
          ireturn = 0; /*not a valid line of any sort, probably EOF*/
          ihitaalibend = 1;      
      }
      return(ireturn);
}
/*___Getoneaalibline()______________________________________________________*/

/****Getoneline**************************************************************/
int    Getoneline(FILE *fileidentifier)
{

      int   c;
      int   itemp,ireturn;

      itemp = 0;
/*c*/ while ((c = fgetc(fileidentifier)) != EOF)
      {
        if (c == '\r' || c == '\n') 
        {
            if(IPC)       c = '\n';  /* standardize newline char */
            else if(IMAC) c = '\r';
            else          c = '\n';  /* standardize newline char */
        }
        temps[itemp] = c;
        if (itemp > 252)
        {
          itemp++;
          if(IPC)       temps[itemp] = '\n';  /*add on a CR*/
          else if(IMAC) temps[itemp] = '\r';  /*add on a CR*/
          else          temps[itemp] = '\n';  /*add on a CR*/
          /* Warn the reader about line may be too long: */
/*c*/      sprintf(alertstr,"Line from file at least at limit"
                            " of string array!"CRLF"%s",temps);
/*Ms*/     DoReportDialog();
          if(IPC)       c = '\n';  /* flag to process a line */
          else if(IMAC) c = '\r';  /* flag to process a line */
          else          c = '\n';  /* flag to process a line */
        }
        itemp++;
        if (c == '\n' || c == '\r')  /* line in, process it */
        {
          temps[itemp] = '\0'; 
          /*make array of characters into properly ended C string*/
          ireturn = 1;
          atEOF = 0;    /* not at EOF */
          return(ireturn);  /* break out of this subroutine */
        }      
      }  /* loop while file not at EOF */
      /* finish out anything in temps buffer */
      ireturn = 0;
      if(itemp > 1) ireturn = 1;
      temps[itemp] = '\0';
      atEOF = 1;  /* EOF */
      return(ireturn);
}
/*___Getoneline()___________________________________________________________*/
  
/****pdbInOutprocessing()*****************************************************/
int  pdbInOutprocessing(char texts[256])
{
/*just doing straight dump of pdb header to output*/
/*and transform of coords then dump of modified ATOM, HETATM records*/

   int inputflag=0; /*keep this ==0 until finished all pdbInOutprocessing*/ 
     /*prekin's flag for finished reading header, atom input now expected*/
     /*Getoneinputline() itself finds EOF*/
   char outs[256];

   if(    CompArgStr(texts,"ATOM",4)     /*PUXMINIT*/
       || CompArgStr(texts,"HETATM",6) )
   {
      rotatepdbatomrecord((char*)texts,(float*)matrix,(float*)rotcenter);
   }
   strcpy(outs,texts);
   fprintf(fpoutput,"%s",outs);

   return(inputflag);
}
/*___pdbInOutprocessing()____________________________________________________*/

/****rotatepdbatomrecord()*****************************************************/
void rotatepdbatomrecord(char texts[256], float matrix[9], float rotcenter[3])
{
    float x,y,z,xx,yy,zz;/*,xxx,yyy,zzz;*/
    /*double a11,a12,a13,a21,a22,a23,a31,a32,a33;*/
    char   temps[256],frags[16],tempx[16],tempy[16],tempz[16],lasts[256];
    int    j=0,igotit=0;

    temps[0] = '\0'; /*initiate a new string as an empty string*/

    for(j=0 ; j<=7 ; j++) frags[j] = texts[j+30];
    frags[8] = '\0';
    igotit =  sscanf(frags,"%8f",&x);
    for(j=0 ; j<=7 ; j++) frags[j] = texts[j+38];
    frags[8] = '\0';
    igotit =  sscanf(frags,"%8f",&y);
    for(j=0 ; j<=7 ; j++) frags[j] = texts[j+46];
    frags[8] = '\0';
    igotit =  sscanf(frags,"%8f",&z);

    if(igotit)
    {
       if(Lmatrix)
       {/*first subtract center*/
          xx = x - rotcenter[1];
          yy = y - rotcenter[2];
          zz = z - rotcenter[3];
          /*then apply matrix*/
          x = xx*matrix[1] + yy*matrix[4] + zz*matrix[7];
          y = xx*matrix[2] + yy*matrix[5] + zz*matrix[8];
          z = xx*matrix[3] + yy*matrix[6] + zz*matrix[9];
       }
       /*add center to coord*/
       x = x + rotcenter[1];
       y = y + rotcenter[2];
       z = z + rotcenter[3];
       /*add translation to coord  050324*/
       x = x + translate[1];
       y = y + translate[2];
       z = z + translate[3];

       sprintf(tempx,"%8.3f",x);
       sprintf(tempy,"%8.3f",y);
       sprintf(tempz,"%8.3f",z);

       /*put an artificial end into local copy of pdb record at xcoord*/
       texts[30] = '\0';
       /*copy last part of pdbrecord, take the whole string for its ending*/
       for(j=0; j<(256-54); j++){lasts[j] = texts[54+j];}
       /*texts has EOLO patched onto it by Getoneinputline()*/
       /*after the \0 which replaces \n or \r of input*/

       /*recreate the pdb atom record*/
       sprintf(temps,"%s%s%s%s%s",texts,tempx,tempy,tempz,lasts);
       strcpy(texts,temps); /*copy record back into original string storage*/
    }
}
/*___rotatepdbatomrecord()____________________________________________________*/


