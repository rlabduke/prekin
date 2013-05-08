/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h"

/****getmchbatoms()**********************************************************/
void   getmchbatoms(void)
{
/*
  sprintf(alertstr,"top of getmchbatoms: natom= %d, {%s|%s|%s|%d|%s} "
  " %8.3f %8.3f %8.3f "
  ,natom,atom[natom],res[natom],sub[natom],num[natom],rins[natom]
  ,x[natom],y[natom],z[natom]);
  DoReportDialog();
*/

  /*residue-by-residue storage of mc h-bond calculating info */
  if(   atmcpy[1] == 'n' && atmcpy[2] == ' ')
  {
    tfn2 = TRUE;
    n2xyz[1] = x[natom];
    n2xyz[2] = y[natom];
    n2xyz[3] = z[natom];
    inumn2 = natom;
  }
  else if(atmcpy[1] == 'c' && atmcpy[2] == 'a')
  {
    tfca2 = TRUE;
    ca2xyz[1] = x[natom];
    ca2xyz[2] = y[natom];
    ca2xyz[3] = z[natom];
  }
  else if(atmcpy[1] == 'c' && atmcpy[2] == ' ')
  {
    tfc2 = TRUE;
    c2xyz[1] = x[natom];
    c2xyz[2] = y[natom];
    c2xyz[3] = z[natom];
  }
  else if(atmcpy[1] == 'o' && atmcpy[2] == ' ')
  {
    tfo2 = TRUE;
    o2xyz[1] = x[natom];
    o2xyz[2] = y[natom];
    o2xyz[3] = z[natom];
    inumo2 = natom;
  }

#ifdef OLDCODE  /*081112 always calculate H of NH (avoid H name issues) */
  /*might as well use the real h for the hbond: */
  /*else if( atmcpy[1] == 'h' && atmcpy[2] == ' ')*/
  else if(isahydrogen(name[natom],atmcpy,element[natom]) ) /*070520*/
  {
    tfnh2 = TRUE;
    nh2xyz[1] = x[natom];
    nh2xyz[2] = y[natom];
    nh2xyz[3] = z[natom];
  }
#endif
}
/*___getmchbatoms()_________________________________________________________*/

/****storemchbatoms()********************************************************/
void   storemchbatoms(void)
{

  char flag3[4];

/*
sprintf(alertstr,"top of storemchbatoms:");
DoReportDialog();
*/

  /*try to calculate nh atom coord for mc h-bonds for this residue */
  /*using previous residue's c and current n & ca */
  if( !tfnh2 )  /*tfnh2 TRUE means already have nh so just put it in storage*/
  {
    if( !tfc1 )
    {
      /* try to get stored carbonyl c coordinate */
      if(oldatm[0]) /*presume 0 for full mainchain ????? 991129*/
      {
        c1xyz[1] = x[lastat];
        c1xyz[1] = x[lastat];
        c1xyz[1] = x[lastat];
        tfc1 = TRUE;
      }
    }
    if(tfc1 && tfn2 && tfca2)  /* tfc1 from previous residue ! */
    {
      /* calculate coords for the h (section from nhgen.f) */
      /* .4775 jsr fudged "halfway" point: */
      /* c-n-ca define a plane, a point xyz nearly halfway between c & ca */
      /* is opposite the nh position.  A normal vector in the xyz - n2xyz */
      /* direction points one unit out from n toward the nh position.  */
      /* Scale this and add to n2xyz to get nhxyz. (n-h ~ 1.0, so scale=1.0)*/
      xyz[1] = (float)(c1xyz[1] + .4775*(ca2xyz[1] - c1xyz[1]));
      xyz[2] = (float)(c1xyz[2] + .4775*(ca2xyz[2] - c1xyz[2]));
      xyz[3] = (float)(c1xyz[3] + .4775*(ca2xyz[3] - c1xyz[3]));
      lfac = (float)(1.0/(sqrt( (n2xyz[1] - xyz[1])*(n2xyz[1] - xyz[1])
              + (n2xyz[2] - xyz[2])*(n2xyz[2] - xyz[2])
                 + (n2xyz[3] - xyz[3])*(n2xyz[3] - xyz[3]) ) ) );
      nh2xyz[1] = n2xyz[1] + lfac*(n2xyz[1] - xyz[1]);
      nh2xyz[2] = n2xyz[2] + lfac*(n2xyz[2] - xyz[2]);
      nh2xyz[3] = n2xyz[3] + lfac*(n2xyz[3] - xyz[3]);
      tfnh2 = TRUE;
    }
  }
  if(tfnh2 && tfn2)
  {
    /*nh atoms store to scratch file*/
    flag3[0] = atom[inumn2][4];  /* alternate conformation flag */
    if(flag3[0] == ' ') flag3[0] = '_';
      /*must be a connected set of characters */
    flag3[1] =  sub[inumn2][0];  /* alternate subunit flag */
    if(flag3[1] == ' ') flag3[1] = '_';
    flag3[2] = rins[inumn2][0];  /* alternate numbering flag */
    if(flag3[2] == ' ') flag3[2] = '_';
    flag3[3] = '\0';
/*
sprintf(alertstr,"ready to write to hbnscratch space"CRLF
"%s %d %s %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f"CRLF
,res[inumn2],num[inumn2],flag3,n2xyz[1],n2xyz[2],n2xyz[3]
,nh2xyz[1],nh2xyz[2],nh2xyz[3]);
DoReportDialog();
*/

    nhcnt = nhcnt + 1;
    sprintf(temps,"n %s %d %s %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f"EOLO
    ,res[inumn2],num[inumn2],flag3,n2xyz[1],n2xyz[2],n2xyz[3]
    ,nh2xyz[1],nh2xyz[2],nh2xyz[3]);
    putonetextblockline(&hbnscratch,temps);
/*printf("%s\n",temps);*/
  }
  /*finish off this residue as far as mc hbonds goes */
  if(tfo2)
  {
    flag3[0] = atom[inumo2][4];  /* alternate conformation flag */
    if(flag3[0] == ' ') flag3[0] = '_';
      /*must be a connected set of characters */
    flag3[1] =  sub[inumo2][0];  /* alternate subunit flag */
    if(flag3[1] == ' ') flag3[1] = '_';
    flag3[2] = rins[inumo2][0];  /* alternate numbering flag */
    if(flag3[2] == ' ') flag3[2] = '_';
    flag3[3] = '\0';

/*
sprintf(alertstr,"ready to write to hboscratch spaced"CRLF
  "o %s %d %s %8.3f %8.3f %8.3f"CRLF
  ,res[inumo2],num[inumo2],flag3,o2xyz[1],o2xyz[2],o2xyz[3]);
DoReportDialog();
*/
    cocnt = cocnt + 1;
    sprintf(temps,"o %s %d %s %8.3f %8.3f %8.3f"EOLO
    ,res[inumo2],num[inumo2],flag3,o2xyz[1],o2xyz[2],o2xyz[3]);
    putonetextblockline(&hboscratch,temps);
/*printf("%s\n",temps);*/
  }
  /*transfer c coordinate back if expect continuity to next residue */
  if( !endchn)
  {
    tfc1 = tfc2;
    /* so if current residue didn't have a carbonyl c, would be FALSE */
    c1xyz[1] = c2xyz[1];
    c1xyz[2] = c2xyz[2];
    c1xyz[3] = c2xyz[3];
  }
  else
  {
    tfc1 = FALSE;
  }
  /*clear current residue flags */
     tfn2 = FALSE;tfca2 = FALSE;tfc2 = FALSE;tfo2 = FALSE;tfnh2 = FALSE;
}
/*___storemchbatoms()_______________________________________________________*/

/****connecthbonds()*********************************************************/
void   connecthbonds(void)
{
  float  dlimhb,angfac;
  int    icountrewinds;
  int    Lmore,Lscan;
  char  a[3],resco[4],resnh[4],nhflag[4];
  char  flag3[4];
  int    numnh,numco;
  float  nxyz[4],nhxyz[4],oxyz[4];


  /*mc hbonds calculation and output */
  /*find all mc hbonds that fit distance and angle criteria */
  /* write triplets vector:  P n L h L o    */
  dlimhb = 3.5;
  angfac = (float)2.3;
  Lmore = TRUE;
  Lscan = TRUE;
  icountrewinds = 0;

  rewindtextblock(&hbnscratch);
  while( Lmore )
  {/*Lmore loop over all nh's*/
    getonetextblockline(&hbnscratch,temps);
    if(temps[0]=='\0')
    {
       Lmore = 0;
       break;
    }

            /*sscanf for nh stuff */
    sscanf(temps,"%s %s %d %s %f %f %f %f %f %f",a,resnh,&numnh,nhflag
           ,&nxyz[1],&nxyz[2],&nxyz[3],&nhxyz[1],&nhxyz[2],&nhxyz[3]);
/*
sprintf(alertstr,"first nh working= "CRLF"%s %d %f %f %f %f %f %f"
  ,resnh,numnh,nxyz[1],nxyz[2],nxyz[3],nhxyz[1],nhxyz[2],nhxyz[3]);
DoReportDialog();
*/
    Lscan = TRUE;
    rewindtextblock(&hboscratch);
/*---------------------------------------------------------------------*/
      icountrewinds++;

      if(icountrewinds == 1 && IMAC) getmactextselect();
           /*PKMCTEXT.C PKPCTEXT.C*/
          /*macintosh TE variable used in pkintextreplace(temps)*/
                         /*PC dummies and ignores this */
      sprintf(temps,"%d tries",icountrewinds);
               /* . for each co - hn pair processed */
      if(IPC)  pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
      else     pkintextreplace(temps);  /*PKMCTEXT.C PKPCTEXT.C*/

      nLines = nLines-1; /*overwrite for PC control*/

/*---------------------------------------------------------------------*/

    while(Lscan)
    {/*Lscan loop over all co's*/
      getonetextblockline(&hboscratch,temps);
      if(temps[0]=='\0')
      {
         Lscan = 0;
         break;
      }

      /*sscanf for co stuff */
      sscanf(temps,"%s %s %d %s %f %f %f"
             ,a,resco,&numco,flag3,&oxyz[1],&oxyz[2],&oxyz[3]);
/*
sprintf(alertstr,"co testing= "CRLF"%s %d %f %f %f"
        ,resco,numco,oxyz[1],oxyz[2],oxyz[3]);
DoReportDialog();
*/
      /*check distance for possible h-bond from this carbonyl o to working nh */
      if(numco != numnh)  /*avoid nh - oc of same residue! */
      {/*between different residues*/
         disthb = (float)(sqrt(  (nhxyz[1] - oxyz[1])*(nhxyz[1] - oxyz[1])
                       + (nhxyz[2] - oxyz[2])*(nhxyz[2] - oxyz[2])
                       + (nhxyz[3] - oxyz[3])*(nhxyz[3] - oxyz[3]) ) );
         cosnho = (nhxyz[1] - nxyz[1])*(oxyz[1] - nhxyz[1])/disthb
              +   (nhxyz[2] - nxyz[2])*(oxyz[2] - nhxyz[2])/disthb
              +   (nhxyz[3] - nxyz[3])*(oxyz[3] - nhxyz[3])/disthb ;
         dimhb = dlimhb - angfac + angfac*cosnho;

/*
sprintf(alertstr,"disthb= %f  <=  dimhb= %f",disthb,dimhb);
DoReportDialog();
*/
          if(disthb <= dimhb)
          {/*H-BOND*/
              /*shorten ends of displayed vectors slightly for clarity */

              oxyz[1] = (float)(oxyz[1] + 0.3*(nhxyz[1] - oxyz[1]));
              oxyz[2] = (float)(oxyz[2] + 0.3*(nhxyz[2] - oxyz[2]));
              oxyz[3] = (float)(oxyz[3] + 0.3*(nhxyz[3] - oxyz[3]));
              /*mxyz[1] = (float)(nxyz[1] + 0.3*(nhxyz[1] - nxyz[1]));*/
              /*mxyz[2] = (float)(nxyz[2] + 0.3*(nhxyz[2] - nxyz[2]));*/
              /*mxyz[3] = (float)(nxyz[3] + 0.3*(nhxyz[3] - nxyz[3]));*/

              /*write to regular scratch file*/
              typenow=typehb;

              /* distance as author comment */
              /*  U = Unpickable points */

              /*--write from hn to shortened-end o */
              /* note 4 char id: hb__  920302*/
              sprintf(temps,"hb  {%s %d hn} <%4.3f> P U %.3f, %.3f, %.3f "
                            "{%s %d o} L U %.3f, %.3f, %.3f"EOLO
                      ,resnh,numnh,disthb,nhxyz[1],nhxyz[2],nhxyz[3]
                      ,resco,numco,oxyz[1],oxyz[2],oxyz[3]);
              putonetextblockline(&mainscratch,temps);
              ++countxyz; ++pointcnt[typenow];countpoints("hb  ",0);
              ++countxyz; ++pointcnt[typenow];countpoints("hb  ",0);
              Listhb = TRUE;
/*
printf("%s\n",temps);
*/
          }/*H-BOND*/
      }/*between different residues*/
    }/*Lscan loop over all co's*/
  }/*Lmore loop over all nh's*/
}
/*___connecthbonds()________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

