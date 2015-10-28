#include "PKIN.h"
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

/****idealizedCalphaAngles()**************************************************/
void idealizedCalphaAngles(char targetresidue[4],float* dist,float* angleCAB,float* dihedralNCAB, float* angleNAB, float* dihedralCNAB, float* angleideal)
{
    if(     targetresidue[0] == 'a'
         && targetresidue[1] == 'l'
         && targetresidue[2] == 'a')
    {
        *dist = (float)1.536;
        *angleCAB = (float)110.1;
        *dihedralNCAB = (float)122.9;
        *angleNAB = (float)110.6;
        *dihedralCNAB = (float)-122.6;
        *angleideal = (float)111.2;
    }
    else if(     targetresidue[0] == 'p'
              && targetresidue[1] == 'r'
              && targetresidue[2] == 'o')
    {
        *dist = (float)1.530;
        *angleCAB = (float)112.2;
        *dihedralNCAB = (float)115.1;
        *angleNAB = (float)103.0;
        *dihedralCNAB = (float)-120.7;
        *angleideal = (float)111.8; /*pro and gly different*/
    }
    else if(  (     targetresidue[0] == 'v'
                 && targetresidue[1] == 'a'
                 && targetresidue[2] == 'l')
              ||(   targetresidue[0] == 't'
                 && targetresidue[1] == 'h'
                 && targetresidue[2] == 'r')
              ||(   targetresidue[0] == 'i'
                 && targetresidue[1] == 'l'
                 && targetresidue[2] == 'e') )
    {/*branched beta*/
        *dist = (float)1.540;
        *angleCAB = (float)109.1;
        *dihedralNCAB = (float)123.4;
        *angleNAB = (float)111.5;
        *dihedralCNAB = (float)-122.0;
        *angleideal = (float)111.2;
    }
    else if(     targetresidue[0] == 'g'
              && targetresidue[1] == 'l'
              && targetresidue[2] == 'y')
    {
        *dist = (float)1.10; /*CB == HA1*/
        *angleCAB = (float)109.3;
        *dihedralNCAB = (float)121.6;
        *angleNAB = (float)109.3;
        *dihedralCNAB = (float)-121.6;
        *angleideal = (float)112.5; /*gly and pro different*/
    }
    else
    {/*all other aa*/
        *dist = (float)1.530;
        *angleCAB = (float)110.1;
        *dihedralNCAB = (float)122.8;
        *angleNAB = (float)110.5;
        *dihedralCNAB = (float)-122.6;
        *angleideal = (float)111.2;
    }
}
/*___idealizedCalphaAngles()_________________________________________________*/

/****constructbeta()**********************************************************/
int constructbeta(float betaideal[3],char targetresidue[4]
                 ,float altalpha[3], float altnite[3], char* altchoice)
{
   /*compute an idealized Cbeta position from an average of construction*/
   /*of Cbeta from both directions*/

   int i=0, j=0, k=0, n=0, ireturn=0;
   float  dist=0,angleCAB=0,angleNAB=0,dihedralNCAB=0,dihedralCNAB=0;
   float  betaxyz[3],betaNCAB[3],betaCNAB[3],betadist=0;
   /*Let 1st 1,2,3,4 array indecies correspond to N,CA,C,CB atoms*/
   /*Let 1,2,3,... indecies correspond to primary,secondary,tertiary conf*/
   #define MAXA 8
   int   L[5][MAXA] ={{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}};
   float X[5][MAXA] ={{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}};
   float Y[5][MAXA] ={{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}};
   float Z[5][MAXA] ={{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}};
   float O[5][MAXA] ={{0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}};
   char  calt[5][MAXA] ={{' ',' ',' ',' ',' ',' ',' ',' '}
              , {' ',' ',' ',' ',' ',' ',' ',' '}, {' ',' ',' ',' ',' ',' ',' ',' '}
              , {' ',' ',' ',' ',' ',' ',' ',' '}, {' ',' ',' ',' ',' ',' ',' ',' '}};
   char  charac[MAXA] ={' ',' ',' ',' ',' ',' ',' ',' '};
   /*charac: last read alt char of nth alt conf atoms, should all be the same*/
   char  ouralt[MAXA] ={' ',' ',' ',' ',' ',' ',' ',' '};
   /*ouralt: our renaming as a,b,c,d,e,... in our best guess at order*/
   char  ournam[MAXA] ={' ','a','b','c','d','e','f','g'};/*starting at index==1*/
   int nam=0; /*running index of existing alts*/
   int nw=4; /*accum max index of wierd alt conf designator*/
   int m=0;  /*running index for wierd alt conf*/
   int nconf=0; /*counter for number of conformers*/
#define N 1
#define C 2
#define A 3
#define B 4 /*which makes Cbeta as the last (dominant) choice when enumerating*/
   float fcxyz[4][3] ={{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
   int   jcbeta=1; /*so atom info defaults to first atom of res if gly*/
   int   Lglycine=0;
   char  colr[16],cntl[5];
   float dev=0,raddev=0;
   double anx=0,any=0,anz=0,acx=0,acy=0,acz=0;
   double ctx=0,cty=0,ctz=0,ccx=0,ccy=0,ccz=0,ncx=0,ncy=0,ncz=0,vx=0,vy=0,vz=0;
   double anmag=0,acmag=0,cosangle=0,disttau=0,tauoffset=0;
   float angle=0,angleideal=(float)109.6, angletaudev=0, dihedralNABB=0;
   int ipad=0,resnum=0;
   char pdbstr[16],resstr[8],substr[8],resins;
#ifdef LABELSPSEUDOTAU
   float raddel=0,del=0;
#endif
   char ptm=' ', altchar=' ', nowaltchar=' ';

   if(*altchoice!='a'&&*altchoice!='b'&&*altchoice!='c'&&*altchoice!='1'&&*altchoice!='2')
   {/*filter for those alt conf characters this routine can handle*/
      *altchoice = ' '; /*safety: default to primary conf*/
   }
   Lglycine = 1; /*presume special case*/

   /*Allow for up to 3 alternate conformations of sidechain*/
   /*where mainchain may or may not have alt. conf.*/

   for(j=1; j<=maxatm; j++)
   {/*j: find CB & the three mc atoms of orig residue to base CB calc*/
      /*scheme is NOT logically complete, but maybe practically complete*/
      /*idea is to rank alternate conformers as primary,secondary,tertiary*/
      /*expectation is alt indicators in order as ' ','b','c' or 'a','b','c' */
      /*allowance for ' ','a','b'  as well as some simple out of order cases*/
      /* 3 alt conf are recognized, hopefully confusions ignored gracefully*/

      if     (    (atom[j][1]=='n') && (atom[j][2]==' ') ) {/*N*/ n = N;}
      else if(    (atom[j][1]=='c') && (atom[j][2]=='a') ) {/*Calpha*/ n = A;}
      else if(    (atom[j][1]=='c') && (atom[j][2]==' ') ) {/*Carbonyl*/ n = C;}
      else if(    (atom[j][1]=='c') && (atom[j][2]=='b')
               && ((atom[j][3]==' ') || (atom[j][3]=='1')) ) /*use 1 of AIB*/
      {/*Cbeta*/
         n = B;
         Lglycine = 0; /*some sort of Cbeta is specified*/
         nconf++; /*counter for number of conformers*/
      }
      else {/*ignore*/ n = 0;}

      if(n>0)
      {/*atom is grist for Cbeta mill*/
         if(atom[j][4]==' ') /*alt char in 5th place (index==4) of name*/
         {/*alt==' '*/
            /*putatively a primary conformation*/
            if(L[n][1]==0)
            {/*first occurance of putative primary conformer*/
                 charac[1] = atom[j][4];
                 calt[n][1]=atom[j][4]; L[n][1]=j;
                    X[n][1]=x[j]; Y[n][1]=y[j]; Z[n][1]=z[j]; O[n][1]=o[j];
            }
            else if(calt[n][1]=='a'||calt[n][1]=='1') /*earlier a alt*/
            {/*recategorize primary vs secondary conformer, blank then a*/
             /*possibly scheme uses ' ', 'a' for primary, secondary confs*/
              if(L[n][2]==0)
              {/*not previously encountered a secondary conf, so...*/
                 k=L[n][1];
                 charac[2] = charac[1];
                 calt[n][2]=calt[n][1]; L[n][2]=k; /*reassign as secondary*/
                    X[n][2]=x[k]; Y[n][2]=y[k]; Z[n][2]=z[k]; O[n][2]=o[k];
                 charac[1] = atom[j][4];
                 calt[n][1]=atom[j][4]; L[n][1]=j; /*assign as primary*/
                    X[n][1]=x[j]; Y[n][1]=y[j]; Z[n][1]=z[j]; O[n][1]=o[j];
              }
              else if(L[n][3]==0)
              {/*not previously encountered a tertiary conf, so...*/
                 k=L[n][2];
                 charac[3] = charac[2];
                 calt[n][3]=calt[n][2]; L[n][3]=k; /*reassign as tertiary*/
                    X[n][3]=x[k]; Y[n][3]=y[k]; Z[n][3]=z[k]; O[n][3]=o[k];
                 k=L[n][1];
                 charac[2] = charac[1];
                 calt[n][2]=calt[n][1]; L[n][2]=k; /*reassign as secondary*/
                    X[n][2]=x[k]; Y[n][2]=y[k]; Z[n][2]=z[k]; O[n][2]=o[k];
                 charac[1] = atom[j][4];
                 calt[n][1]=atom[j][4]; L[n][1]=j; /*assign as primary*/
                    X[n][1]=x[j]; Y[n][1]=y[j]; Z[n][1]=z[j]; O[n][1]=o[j];
              }
              /*anything more confusing is ignored*/
            }
            else if(calt[n][1]==' ')
            {/*second occurance of ' ' treated as secondary conf*/
              if(L[n][2]==0)
              {/*not previously encountered a secondary conf, so...*/
                 charac[2] = atom[j][4];
                 calt[n][2]=atom[j][4]; L[n][2]=j;
                    X[n][2]=x[j]; Y[n][2]=y[j]; Z[n][2]=z[j]; O[n][2]=o[j];
              }
            }
            /*anything more confusing is ignored*/
         }/*alt==' '*/
         else if( (atom[j][4]=='a')||(atom[j][4]=='1') )
         {/*alt=='a'||'1'*/
            /*putatively a primary conformation*/
            if(L[n][1]==0)
            {/*first occurance of putative primary conformer*/
                 charac[1] = atom[j][4];
                 calt[n][1]=atom[j][4]; L[n][1]=j;
                    X[n][1]=x[j]; Y[n][1]=y[j]; Z[n][1]=z[j]; O[n][1]=o[j];
            }
            else if(calt[n][1]==' ') /*can't handle mixed a,b with 1,2*/
            {/*possibly scheme uses ' ', 'a' for primary, secondary confs*/
              if(L[n][2]==0)
              {/*not previously encountered a secondary conf, so...*/
                 charac[2] = atom[j][4];
                 calt[n][2]=atom[j][4]; L[n][2]=j;
                    X[n][2]=x[j]; Y[n][2]=y[j]; Z[n][2]=z[j]; O[n][2]=o[j];
              }
            }
            /*anything more confusing is ignored*/
         }/*alt=='a'||'1'*/
         else if( (atom[j][4]=='b')||(atom[j][4]=='2') )
         {/*alt=='b'||'2'*/
            /*putatively a secondary conformation, can go as 2nd or 3rd*/
            /*but some pathologies have first alt==b, which we rename as a*/
            if(L[n][1]==0)
            {/*first occurance of any conformer*/
                 charac[1] = atom[j][4];
                 calt[n][1]=atom[j][4]; L[n][1]=j;
                    X[n][1]=x[j]; Y[n][1]=y[j]; Z[n][1]=z[j]; O[n][1]=o[j];
            }
            else if(L[n][2]==0)
            {/*first occurance of putative secondary conformer, normal case*/
                 charac[2] = atom[j][4];
                 calt[n][2]=atom[j][4]; L[n][2]=j;
                    X[n][2]=x[j]; Y[n][2]=y[j]; Z[n][2]=z[j]; O[n][2]=o[j];
            }
            else if(calt[n][2]=='a'||calt[n][2]=='1') /*presume a,b OR 1,2*/
            {/*recategorize secondary vs tertiary conformer here*/
             /*possibly scheme uses ' ', 'a', 'b' for prime, sec., tert. confs*/
              if(L[n][3]==0)
              {/*not previously encountered a tertiary conf, so...*/
                 charac[3] = atom[j][4];
                 calt[n][3]=atom[j][4]; L[n][3]=j;
                    X[n][3]=x[j]; Y[n][3]=y[j]; Z[n][3]=z[j]; O[n][3]=o[j];
              }
            }
            /*anything more confusing is ignored*/
         }/*alt=='b'||'2'*/
         else if( (atom[j][4]=='c')||(atom[j][4]=='3') )
         {/*alt=='c'||'3'*/
            /*putatively a tertiary conformation*/
            /*but some pathologies do not adhere to a,b,c*/
            if(L[n][1]==0)
            {/*first occurance of any conformer*/
                 charac[1] = atom[j][4];
                 calt[n][1]=atom[j][4]; L[n][1]=j;
                    X[n][1]=x[j]; Y[n][1]=y[j]; Z[n][1]=z[j]; O[n][1]=o[j];
            }
            else if(L[n][2]==0)
            {/*first occurance of putative secondary conformer, normal case*/
                 charac[2] = atom[j][4];
                 calt[n][2]=atom[j][4]; L[n][2]=j;
                    X[n][2]=x[j]; Y[n][2]=y[j]; Z[n][2]=z[j]; O[n][2]=o[j];
            }
            else if(L[n][3]==0)
            {/*first occurance of putative tertiary conformer, usual case*/
                 charac[3] = atom[j][4];
                 calt[n][3]=atom[j][4]; L[n][3]=j;
                    X[n][3]=x[j]; Y[n][3]=y[j]; Z[n][3]=z[j]; O[n][3]=o[j];
            }
            else if(L[n][nw]==0) /*nw initialized as 4, next index past 3 a,b,c */
            {/*first occurance of this designated putative conformer*/
                 charac[nw] = atom[j][4];
                 calt[n][nw]=atom[j][4]; L[n][nw]=j;
                    X[n][nw]=x[j]; Y[n][nw]=y[j]; Z[n][nw]=z[j]; O[n][nw]=o[j];
            }
            /*this works if c is the 4th alt, otherwise may mess up */
            /*anything more confusing is ignored*/
         }/*alt=='c'||'3'*/
         else /*atom[j][4] not blank nor a,b,c,1,2,3 but some other character*/
         {/*alt==wierd*/
            /*putatively a wierdly designated alternate conformation*/
            if(L[n][nw]==0) /*nw initialized as 4, next index past 3 a,b,c */
            {/*first occurance of this designated putative conformer*/
                 charac[nw] = atom[j][4];
                 calt[n][nw]=atom[j][4]; L[n][nw]=j;
                    X[n][nw]=x[j]; Y[n][nw]=y[j]; Z[n][nw]=z[j]; O[n][nw]=o[j];
            }
            else /*see if atom[j][4] matches previous atom's alt char*/
            {
                for(m=4; m<=nw; m++)
                {
                   if(atom[j][4]==charac[m])
                   {
                      break; /*carrying m on to next section of code*/
                   }
                }
                if(m>nw)
                {/*new wierd alt char*/
                   if(nw < MAXA-1)
                   {
                       nw++;
                       m=nw;
                       charac[m] = atom[j][4];
                   }
                }
                if(m<MAXA)
                {/*there is room to load this conformation*/
                   /*presume new type with same alt char*/
                   /*overwrite any previous atom of this type and alt char*/
                   calt[n][m]=atom[j][4]; L[n][m]=j;
                   X[n][m]=x[j]; Y[n][m]=y[j]; Z[n][m]=z[j]; O[n][m]=o[j];
                }
            }
            /*anything more confusing is ignored*/
         }/*alt==wierd*/
      }/*atom is grist for Cbeta mill*/
   }/*j: find CB & the three mc atoms of orig residue to base CB calc*/

   if(L[N][1] != 0 && L[A][1] != 0 &&  L[C][1] != 0)
   {/*got N,CA,C: compute idealized Cbeta position*/
      /*make sure that all sidechain confs have mainchain atoms*/
      if(L[N][1] != 0)
      {
        for(m=2; m<=nw; m++)
        {
          if(L[N][m]==0) { X[N][m]=X[N][1]; Y[N][m]=Y[N][1]; Z[N][m]=Z[N][1]; }
        }
      }
      if(L[A][1] != 0)
      {
        for(m=2; m<=nw; m++)
        {
          if(L[A][m]==0) { X[A][m]=X[A][1]; Y[A][m]=Y[A][1]; Z[A][m]=Z[A][1]; }
        }
      }
      if(L[C][1] != 0)
      {
        for(m=2; m<=nw; m++)
        {
          if(L[C][m]==0) { X[C][m]=X[C][1]; Y[C][m]=Y[C][1]; Z[C][m]=Z[C][1]; }
        }
      }
      if(L[B][1] != 0)
      {
        for(m=2; m<=nw; m++)
        {
          if(L[B][m]==0) { X[B][m]=X[B][1]; Y[B][m]=Y[B][1]; Z[B][m]=Z[B][1]; }
        }
      }
/*
Idealizing the Cb:
Ala, from  C: 1.536 Å, 110.1,  122.9 (i.e., N,C,Ca,Cb)
Ala, from  N: 1.536 Å, 110.6, -122.6 (i.e., C,N,Ca,Cb)
Pro, from  C: 1.530 Å, 112.2,  115.1
Pro, from  N: 1.530 Å, 103.0, -120.7
Val, from  C: 1.540 Å, 109.1,  123.4 (Thr, Ile)
Val, from  N: 1.540 Å, 111.5, -122.0
Leu, from  C: 1.530 Å, 110.1,  122.8 (Met, Phe, Ser)
Leu, from  N: 1.530 Å, 110.5, -122.6
*/
    /*only matters what residue it is to be mutated to*/
    /*i.e. always newresidue */
    idealizedCalphaAngles(targetresidue,&dist,&angleCAB,&dihedralNCAB,&angleNAB,&dihedralCNAB,&angleideal);

    if(Lcbetadev)
    {
      nam = 1; /*first indexed alt*/
      for(i=1; i<MAXA; i++) /*050404*/
      {/*sanity check and renaming of alts for our cbdevdump output*/
         if(L[B][i]!=0)
         {/*an ith alt of cbeta exists*/
            ouralt[i] = ournam[nam];
            nam++;
         }
      }
    }
    /*for(i=1; i<=3; i++)*/
    for(i=1; i<MAXA; i++) /*050404*/
    {/*check for primary,secondary,tertiary, ..., alt conformers*/
 /*alternate ideal conformers only meaningful if mainchain has alternate conf*/
 /*so check that at least one of mc has this alt*/
 /*EXCEPT: for cbetadev calc deviation even if no alt mainchain atoms 031124*/
     /*if((Lcbetadev && L[B][i]!=0) || (L[N][i]!=0 || L[A][i]!=0 || L[C][i]!=0 ) )*/
     if(   (Lcbetadev && L[B][i]!=0)  /*and there is an alt CB to compare to*/
        || (!Lcbetadev && L[N][i]!=0) /*at least one mc atom has an alt conf*/
        || (!Lcbetadev && L[A][i]!=0)
        || (!Lcbetadev && L[C][i]!=0) )
     {/*construct Cbeta conformer*/
      fcxyz[0][0] = X[N][i];
      fcxyz[0][1] = Y[N][i];
      fcxyz[0][2] = Z[N][i];
      fcxyz[1][0] = X[C][i];
      fcxyz[1][1] = Y[C][i];
      fcxyz[1][2] = Z[C][i];
      fcxyz[2][0] = X[A][i];
      fcxyz[2][1] = Y[A][i];
      fcxyz[2][2] = Z[A][i];

      constructfourth(fcxyz,dist,angleCAB,dihedralNCAB);
      betaNCAB[0] = fcxyz[3][0];
      betaNCAB[1] = fcxyz[3][1];
      betaNCAB[2] = fcxyz[3][2];

      fcxyz[0][0] = X[C][i];
      fcxyz[0][1] = Y[C][i];
      fcxyz[0][2] = Z[C][i];
      fcxyz[1][0] = X[N][i];
      fcxyz[1][1] = Y[N][i];
      fcxyz[1][2] = Z[N][i];
      fcxyz[2][0] = X[A][i];
      fcxyz[2][1] = Y[A][i];
      fcxyz[2][2] = Z[A][i];
      constructfourth(fcxyz,dist,angleNAB,dihedralCNAB);
      betaCNAB[0] = fcxyz[3][0];
      betaCNAB[1] = fcxyz[3][1];
      betaCNAB[2] = fcxyz[3][2];

      betaxyz[0] = (betaNCAB[0]+betaCNAB[0])/2;
      betaxyz[1] = (betaNCAB[1]+betaCNAB[1])/2;
      betaxyz[2] = (betaNCAB[2]+betaCNAB[2])/2;

      betadist=distan(X[A][i],Y[A][i],Z[A][i],betaxyz[0],betaxyz[1],betaxyz[2]);
      if(betadist != dist)
      {/*recompute distance of averaged beta position*/
         fcxyz[3][0] = betaxyz[0] - X[A][i];
         fcxyz[3][1] = betaxyz[1] - Y[A][i];
         fcxyz[3][2] = betaxyz[2] - Z[A][i];
         betaxyz[0] = X[A][i] + fcxyz[3][0]*dist/betadist;
         betaxyz[1] = Y[A][i] + fcxyz[3][1]*dist/betadist;
         betaxyz[2] = Z[A][i] + fcxyz[3][2]*dist/betadist;

      }

      /*for mutation, match altconf character with primary,secondary,tertiary */
      nowaltchar = ' '; /*default for primary conformer*/
      for(n=N; n<=B; n++)
      {/*vote on what is the alt conf character for this enumerated conformer*/
         if(calt[n][i]!=' ') {nowaltchar = calt[n][i];}
         /*so a blank will not override an earlier different character*/
         /*beta, if present, dominant over others*/
      }
      if( (i==1 && *altchoice==' ')||(nowaltchar == *altchoice) )
      {/*nth alt conformer used for idealized mutation*/
         *altchoice = nowaltchar;
         betaideal[0] = betaxyz[0];
         betaideal[1] = betaxyz[1];
         betaideal[2] = betaxyz[2];
         altalpha[0] = X[A][i]; /*supposedly the appropriate Calpha for this conf*/
         altalpha[1] = Y[A][i];
         altalpha[2] = Z[A][i];
         altnite[0] = X[N][i]; /*supposedly the appropriate Nitrogen for this conf*/
         altnite[1] = Y[N][i];
         altnite[2] = Z[N][i];
      }
      ireturn = 1;

      if(Lcbetadev > 0 && !Lglycine)
      {/*output of deviation of C-beta from ideal*/

#ifdef LABELSPSEUDOTAU
         /*compute spread from the N and C directions*/
         /*a measure of non-ideal tau*/
         del = distan( betaNCAB[0],betaNCAB[1],betaNCAB[2]
                      ,betaCNAB[0],betaCNAB[1],betaCNAB[2]);
         if(del > .004) {raddel = del/2;}
         else {raddel = (float)0.002;}/*minimal: 0 gets default radius*/
#endif /*LABELSPSEUDOTAU*/

         /*compute the deviation from ideal beta position*/
         if(Lglycine)
         {/*artifact: don't output CB on gly, kept in for bookkeeping*/
            raddev = (float)0.002;/*minimal: 0 gets default radius*/
            dev = 0;
            /*ptm = 't'; 041031 no cbetadev pointmasters*/
         }
         else
         {
            dev =
              distan( X[B][i],Y[B][i],Z[B][i],betaxyz[0],betaxyz[1],betaxyz[2]);
            if(dev > .002) {raddev = dev;}
            else {raddev = (float)0.002;}/*minimal: 0 gets default radius*/
            /*041031 no cbetadev pointmasters*/
            /*if    (dev < .05) {ptm = 't'; }*/ /*changed from a 030605*/
            /*else if(dev < .1) {ptm = 'u'; }*/ /*cbetadev*/
            /*else if(dev < .2) {ptm = 'v'; }*/
            /*else if(dev < .3) {ptm = 'w'; }*/
            /*else if(dev < .4) {ptm = 'x'; }*/
            /*else if(dev < .5) {ptm = 'y'; }*/
            /*else              {ptm = 'z'; }*/
         }
       /*dihedral N---CA---CBideal---CBactual    011105 */
       dihedralNABB = (float)dihedral4pt(
                 (double)X[N][i],(double)Y[N][i],(double)Z[N][i]
                ,(double)X[A][i],(double)Y[A][i],(double)Z[A][i]
                ,(double)betaxyz[0],(double)betaxyz[1],(double)betaxyz[2]
                ,(double)X[B][i],(double)Y[B][i],(double)Z[B][i] );


       if(!Lcbdevdump && dev >= .25) /*041031 only bad cbetadev, no ptmasters*/
       {/*NOT a cbeta deviation dump, write scratch for kinemage*/
          Listcbdev=TRUE;
          cbdevoutl=1;
          {
             if(i==1)
             {
                sprintf(cntl,"cbda"); /*Cbeta deviation BALLS, a conformation */
                typenow=typecbda;
                sprintf(colr,"gold");
                jcbeta = L[B][i];
             }
             else if(i==2)
             {
                sprintf(cntl,"cbdb"); /*Cbeta deviation BALLS, b conformation */
                typenow=typecbdb;
                sprintf(colr,"pink");
             }
             else /*if(i==3) but 040504 any beyond 3 get lumped into c*/
             {
                sprintf(cntl,"cbdc"); /*Cbeta deviation BALLS, c conformation */
                typenow=typecbdc;
                sprintf(colr,"red");
             }
             /*041031 no cbetadev pointmasters*/
             /*sprintf(temps,"%s{ cb %s %s %d%s %.3f %.2f}"*/
             /*              " '%c' r=%.3f %s  %.3f, %.3f, %.3f"EOLO*/
             /*  ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]*/
             /*  ,dev,dihedralNABB*/
             /*  ,ptm,raddev,colr,betaxyz[0],betaxyz[1],betaxyz[2]);*/
             sprintf(temps,"%s{ cb  %s %s%4d%s %.3f %.2f}"
                           " r=%.3f %s  %.3f, %.3f, %.3f"EOLO
               ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
               ,dev,dihedralNABB
               ,raddev,colr,betaxyz[0],betaxyz[1],betaxyz[2]);
             ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
             putonetextblockline(&mainscratch,temps);
          }
#ifdef LABELSPSEUDOTAU
          sprintf(cntl,"cbdm"); /*Cbeta construction mismatch BALLS */
          typenow=typecbdm;
          sprintf(colr,"sky");
        sprintf(temps,"%s{ cb  %s %s%4d%s %.3f} r=%.3f %s  %.3f, %.3f, %.3f"EOLO
           ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
           ,del,raddel,colr,betaxyz[0],betaxyz[1],betaxyz[2]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
          putonetextblockline(&mainscratch,temps);

          sprintf(cntl,"cbdv"); /*Cbeta construction mismatch VECTORS */
          typenow=typecbdv;
          sprintf(colr,"hotpink");
          sprintf(temps,"%s{ cb  %s %s%4d%s %.3f}P %s  %.3f, %.3f, %.3f"
                          "{ cb  %s %s%4d%s %.3f}L %s  %.3f, %.3f, %.3f"EOLO
            ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
            ,del,colr,betaNCAB[0],betaNCAB[1],betaNCAB[2]
            ,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
            ,del,colr,betaCNAB[0],betaCNAB[1],betaCNAB[2]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
          putonetextblockline(&mainscratch,temps);

          sprintf(cntl,"cbdl"); /*Cbeta deviation LABELS */
          typenow=typecbdl;
          sprintf(colr,"gold");
          sprintf(temps,"%s{ cb  %s %s%4d%s %.3f %.3f} %s"
             "< %.3f, %.3f, %.3f; %.3f, %.3f, %.3f >"
             "  %.3f, %.3f, %.3f"EOLO
            ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
            ,dev,del,colr
            ,betaNCAB[0],betaNCAB[1],betaNCAB[2]
            ,betaCNAB[0],betaCNAB[1],betaCNAB[2]
            ,betaxyz[0],betaxyz[1],betaxyz[2]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
          putonetextblockline(&mainscratch,temps);
#endif /*LABELSPSEUDOTAU*/
          if     (i==1) sprintf(cntl,"cbde"); /*Calpha Cbeta VECTORS */
          else if(i==2) sprintf(cntl,"cbdf"); /*Calpha Cbeta VECTORS */
          /*else if(i==3) sprintf(cntl,"cbdg");*/ /*040504 3+ as c */
          else          sprintf(cntl,"cbdg"); /*Calpha Cbeta VECTORS */
          typenow=typecbde;
          sprintf(colr,"peach");
          /*NB: white space between z {...} : {...}P x,y,z {...}L */
          sprintf(temps,"%s{ ca  %s %s%4d%s}P %s  %.3f, %.3f, %.3f"
                         " { cb  %s %s%4d%s}L %s  %.3f, %.3f, %.3f"EOLO
            ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
            ,colr,X[A][i],Y[A][i],Z[A][i]
            ,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
            ,colr,betaxyz[0],betaxyz[1],betaxyz[2]);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
          ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
          putonetextblockline(&mainscratch,temps);
       }/*NOT a dump, write scratch for kinemage*/
       else if(Lcbdevdumpmax) /*040318*/
       {/*just accummulate the maximum value of cbeta deviation */
          if(dev > cbetadevmax) {cbetadevmax = dev;}
       }
       else if(Lcbdevdump) /*must check after Lcbdevdumpmax*/
       {/*write scratch for colon delineated dump of deviation*/
          Ldump = 1; /*dump equivalent of kinemage List...*/
          /*This dump never goes through Listext...  but other dumps use the*/
          /*regular machinery in PKINCSBS to calc output info*/
          /*also inhibit tau deviation claculation and output*/
          Ltaudev = 0;
          sprintf(cntl,"dump"); /*colon-delineated-list dump*/
          ipad = 0;
          for(j=0;j<7;j++)
          {/*taking just the first 7 char of the stripped input file name*/
             pdbstr[j] = PDBfileStr[j];
             if(pdbstr[j] == '\0') {ipad = 1;}
             if(ipad) {pdbstr[j] = ' ';}
          }
          pdbstr[7] = '\0';
          ipad = 0;
          for(j=0;j<3;j++)
          {
             resstr[j] = res[jcbeta][j];
             if(resstr[j] == '\0') {ipad = 1;}
             if(ipad) {resstr[j] = ' ';}
          }
          resstr[3] = '\0';
          /* chain ID string */
          /*substr[0] = sub[jcbeta][0];
          substr[1] = sub[jcbeta][1];
          substr[2] = '\0';*/
          substr[0] = sub[jcbeta][0];
          substr[1] = sub[jcbeta][1];
          substr[2] = sub[jcbeta][2];
          substr[3] = sub[jcbeta][3];
          substr[4] = sub[jcbeta][4];
          /*substr[1] = '\0'; go to 2char, 080325*/
          resnum = num[jcbeta];
          resins = (char)rins[jcbeta][0]; /*rins is a C string*/

          /*if     (i==1 && L[B][2]==0 && L[B][3]==0 ) {altchar=' ';}*/ /*one conf*/
          if(nconf <=1) {altchar=' ';} /*one conf*/
          /*else if(i==1) altchar='a';*/ /*at least another alt conf*/
          /*else if(i==2) altchar='b';*/ /*Standard ' ' or a,b,c designation*/
          /*else if(i==3) altchar='c';*/
          else {altchar = ouralt[i];} /*at least another alt conf*/
          /*040504 MAXA==8 so 7 alts, original put on end of line*/
          /*note: charac[i] is actual alt char from pdb file, now put on end*/
          /*cntl stripped in output, insertion code on end of residue number*/
          sprintf(temps,"%s%s :%c:%s:%s:%4d%c:%7.3f:%7.2f:%7.2f:%c:"EOLO
          ,cntl,PDBfileStr,altchar,resstr,substr,resnum,resins,dev,dihedralNABB,O[B][i],charac[i]);
          /*,cntl,pdbstr,altchar,resstr,substr,resnum,resins,dev,dihedralNABB,O[B][i]);*/
          putonetextblockline(&mainscratch,temps);

       }/*write scratch for colon delineated dump of deviation*/

      }/*output of deviation of C-beta from ideal*/
      if(Ltaudev)
      {/*tau angle deviation*/
         Listtaudev = 1;
         anx = (double)X[N][i] - (double)X[A][i]; /*vector CA-->N*/
         any = (double)Y[N][i] - (double)Y[A][i];
         anz = (double)Z[N][i] - (double)Z[A][i];

         acx = (double)X[C][i] - (double)X[A][i]; /*vector CA-->C*/
         acy = (double)Y[C][i] - (double)Y[A][i];
         acz = (double)Z[C][i] - (double)Z[A][i];
         /*normalize vectors*/
         anmag = sqrt( anx*anx + any*any + anz*anz );
         anx = anx/anmag;
         any = any/anmag;
         anz = anz/anmag;
         acmag = sqrt( acx*acx + acy*acy + acz*acz );
         acx = acx/acmag;
         acy = acy/acmag;
         acz = acz/acmag;
         /*get angle from normalized vectors*/
         cosangle = dotproduct(anx,any,anz,acx,acy,acz);
         angle = (float)(acos(cosangle)*360/(2*3.14159));
         /*an-->  ac--> both outof Calpha, __/ */
         /*dotproduct is projection of one onto the other so */
         /*cosangle translates directly to tau angle*/
         angletaudev = angle - angleideal;
         if(angletaudev >= 0)
         {
            sprintf(colr,"sky");
         }
         else
         {
            sprintf(colr,"pink");
         }
         sprintf(cntl,"tau ");
         typenow=typetau;
         /*           ctxyz:   .  N  */
         /*ctr tau position: C__/   */
         ctx = (X[A][i] +anx +X[A][i] +acx)/2;
         cty = (Y[A][i] +any +Y[A][i] +acy)/2;
         ctz = (Z[A][i] +anz +Z[A][i] +acz)/2;
         /*Calpha---ctr tau position*/
         disttau = sqrt((X[A][i]-ctx)*(X[A][i]-ctx)+(Y[A][i]-cty)*(Y[A][i]-cty)+(Z[A][i]-ctz)*(Z[A][i]-ctz));

         /*calculate offset based on tau angle deviation*/
         /*Linear*/
         if(angletaudev > 0) {angle = angletaudev/2;}
         else if(angletaudev < 0) {angle = -angletaudev/2;}
         else {angle = 0;}
         tauoffset = disttau*tan(angle*2*3.14159/360);
         /*vector from tau ctr position to unit vector point along CA---N */
         vx = X[A][i]+anx - ctx;
         vy = Y[A][i]+any - cty;
         vz = Z[A][i]+anz - ctz;
         /*magnitude (distance) from tau ctr point to that unit vector*/
         disttau = sqrt(vx*vx + vy*vy + vz*vz);
         /*components of the offset point, are vx/dist, vy/dist, vz/dist*/
         /*offset point toward N: */
         ncx = ctx + tauoffset*vx/disttau;
         ncy = cty + tauoffset*vy/disttau;
         ncz = ctz + tauoffset*vz/disttau;
         /*offset point toward C: */
         ccx = ctx - tauoffset*vx/disttau;
         ccy = cty - tauoffset*vy/disttau;
         ccz = ctz - tauoffset*vz/disttau;

         if(tauoffset < .002)
         {tauoffset = (float)0.002;}/*minimal: 0 gets default radius*/

         sprintf(cntl,"taub");
         typenow=typetaub;
         sprintf(temps
           ,"%s{ tau %s %s%4d%s %.3f}P B r=%.4f %s  %.3f, %.3f, %.3f"EOLO
           ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
           ,angletaudev,tauoffset,colr
           ,ctx,cty,ctz); /*ctr tau position*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
         putonetextblockline(&mainscratch,temps);

         sprintf(cntl,"tauv"); /*NB pointID has no atom name field*/
         sprintf(temps
         ,"%s{%s %s%4d%s:i}P U %.3f, %.3f, %.3f L U %s  %.3f, %.3f, %.3f "
         "P U %.3f, %.3f, %.3f L U %s %.3f, %.3f, %.3f\n"
         ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
        ,X[A][i],Y[A][i],Z[A][i],colr,ccx,ccy,ccz,X[A][i],Y[A][i],Z[A][i],colr,ncx,ncy,ncz);
         putonetextblockline(&mainscratch,temps);

         /*Square*/
         angle = (angletaudev*angletaudev)/2;
         tauoffset = disttau*tan(angle*2*3.14159/360);
         /*vector from tau ctr position to unit vector point along CA---N */
         vx = X[A][i]+anx - ctx;
         vy = Y[A][i]+any - cty;
         vz = Z[A][i]+anz - ctz;
         /*magnitude (distance) from tau ctr point to that unit vector*/
         disttau = sqrt(vx*vx + vy*vy + vz*vz);
         /*components of the offset point, are vx/dist, vy/dist, vz/dist*/
         /*offset point toward N: */
         ncx = ctx + tauoffset*vx/disttau;
         ncy = cty + tauoffset*vy/disttau;
         ncz = ctz + tauoffset*vz/disttau;
         /*offset point toward C: */
         ccx = ctx - tauoffset*vx/disttau;
         ccy = cty - tauoffset*vy/disttau;
         ccz = ctz - tauoffset*vz/disttau;

         if(tauoffset < .002)
         {tauoffset = (float)0.002;}/*minimal: 0 gets default radius*/

         sprintf(cntl,"taus"); /*NB pointID has no atom name field*/
         typenow=typetaus;
         sprintf(temps
           ,"%s{ tau %s %s%4d%s %.3f}P B r=%.4f %s  %.3f, %.3f, %.3f"EOLO
           ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
           ,angletaudev,tauoffset,colr
           ,ctx,cty,ctz); /*ctr tau position*/
         ++countxyz;++pointcnt[typenow];countpoints(cntl,0);
         putonetextblockline(&mainscratch,temps);

         sprintf(cntl,"tauw");
         sprintf(temps  /*NB pointID has no atom name field*/
         ,"%s{%s %s%4d%s:i}P U %.3f, %.3f, %.3f L U %s  %.3f, %.3f, %.3f "
         "P U  %.3f, %.3f, %.3f L U %s %.3f, %.3f, %.3f\n"
         ,cntl,res[jcbeta],sub[jcbeta],num[jcbeta],rins[jcbeta]
        ,X[A][i],Y[A][i],Z[A][i],colr,ccx,ccy,ccz,X[A][i],Y[A][i],Z[A][i],colr,ncx,ncy,ncz);
         putonetextblockline(&mainscratch,temps);

      }/*tau angle deviation*/
     }/*construct Cbeta conformer*/
    }/*check for a,b,c conformers*/
   }/*got N,CA,C: compute idealized Cbeta position*/
   else
   {
      ireturn = 0;
   }
   return(ireturn);
}
/*___constructbeta()_________________________________________________________*/

/****constructgamma()*********************************************************/
int constructgamma(float gammaxyz[3],char oldresidue,char newresidue)
{/*shift gamma atom of ref structure into ideal geometrical position */
   /*so putting ideal sidechain into position will not distort geometry*/
   int Lnn = 0, ireturn=0, j=0, Ldihedral=0;
   float  distance=0,angle=0,dihedral=0;
   double cosangle=0;
   float  bmag=0;
   float ax=0,ay=0,az=0,bx=0,by=0,bz=0;
   float fcxyz[4][3] ={{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};

   /*distance and angle identically match ideal aa of library array*/
   /*dihdral is fudged from real gamma atom position, if there is one*/

   /*get vectors from ideal sidechain*/ /*do piecemeal for easier proofing*/
   /*CA->CB->CG:  b,a */ /* IDEAL: Y[x,y,z][ca,cb,cg] i.e. the mutant*/
   fcxyz[1][0] = (float)Y[1][1]; /*second atom of 4: CA ideal*/
   fcxyz[1][1] = (float)Y[2][1];
   fcxyz[1][2] = (float)Y[3][1];
   fcxyz[2][0] = (float)Y[1][2]; /*third atom of 4: CB ideal*/
   fcxyz[2][1] = (float)Y[2][2];
   fcxyz[2][2] = (float)Y[3][2];
   fcxyz[3][0] = (float)Y[1][3]; /*fourth atom of 4: CG ideal*/
   fcxyz[3][1] = (float)Y[2][3]; /*of course, gamma atom might be C, O, S*/
   fcxyz[3][2] = (float)Y[3][3];

   ax = (fcxyz[3][0]-fcxyz[2][0]); /*ultimate vector CB-->CG*/
   ay = (fcxyz[3][1]-fcxyz[2][1]);
   az = (fcxyz[3][2]-fcxyz[2][2]);

   bx = (fcxyz[1][0]-fcxyz[2][0]); /*penultimate vector CB-->CA*/
   by = (fcxyz[1][1]-fcxyz[2][1]);
   bz = (fcxyz[1][2]-fcxyz[2][2]);
   /*get ideal distance from ideal sidechain*/
   distance = (float)sqrt( ax*ax + ay*ay + az*az );
   ax = ax/distance;
   ay = ay/distance;
   az = az/distance;
   bmag = (float)sqrt( bx*bx + by*by + bz*bz );
   bx = bx/bmag;
   by = by/bmag;
   bz = bz/bmag;
   /*get ideal angle from the ideal sidechain normalized vectors*/
   cosangle = dotproduct(ax,ay,az,bx,by,bz);
   angle = (float)(acos(cosangle)*360/(2*3.14159));
/*
sprintf(alertstr,CRLF"constr CG with dist: %.3f, ang: %.3f, from cosangle: %.3f"CRLF
                               ,distance,angle,cosangle);
pkintextinsert(alertstr);
adjusttext(1);
*/
   {/*look for the actual mainchain N for dihedral definition*/
      for(j=1; j<=maxatm; j++)
      {/*find the mainchain N*/
         if(   (atom[j][1]=='n')
            && (atom[j][2]==' '))
         {
            Lnn = 1;
            fcxyz[0][0] = x[j]; /*first atom of 4, real mainchain N*/
            fcxyz[0][1] = y[j];
            fcxyz[0][2] = z[j];
            break;
         }
      }/*find the mainchain N*/
      if(Lnn)
      {/*found old real mainchain N from which to calculate a dihedral*/
         /*get practical dihedral using new beta but old gamma atom position*/
         /* e.g. --one presumes that the gamma atom was in electron density-- */
         fcxyz[1][0] = (float)X[1][1]; /*second atom of 4: CA real*/
         fcxyz[1][1] = (float)X[2][1];
         fcxyz[1][2] = (float)X[3][1];
         fcxyz[2][0] = (float)X[1][2]; /*third atom of 4: CB calculated */
         fcxyz[2][1] = (float)X[2][2]; /*already inserted into X array*/
         fcxyz[2][2] = (float)X[3][2];
         if(  newresidue=='A'
            ||oldresidue=='A'
            ||oldresidue=='G'
            ||(oldresidue=='P' && newresidue!='P') )
         {/*staggered h's for new Ala*/
            /*force trans for longer residue on old Gly,Ala or Pro*/
            dihedral = 180;
            Ldihedral = 1;
         }
         else
         {/*use dihedral based on old structure*/
            fcxyz[3][0] = (float)X[1][3]; /*fourth atom of 4: CG real original*/
            fcxyz[3][1] = (float)X[2][3]; /*of course, gamma atom might be C, O, S*/
            fcxyz[3][2] = (float)X[3][3];
            dihedral = (float)dihedral4pt(
                 (double)fcxyz[0][0],(double)fcxyz[0][1],(double)fcxyz[0][2]
                ,(double)fcxyz[1][0],(double)fcxyz[1][1],(double)fcxyz[1][2]
                ,(double)fcxyz[2][0],(double)fcxyz[2][1],(double)fcxyz[2][2]
                ,(double)fcxyz[3][0],(double)fcxyz[3][1],(double)fcxyz[3][2] );
            Ldihedral = 1;
         }/*use dihedral based on old structure*/
      }/*found old real mainchain N from which to calculate a dihedral*/
   }/*look for the actual mainchain N for dihedral definition*/
/*
sprintf(alertstr,CRLF"constr CG with dist: %.3f, ang: %.3f, dih: %.3f"CRLF
                               ,distance,angle,dihedral);
pkintextinsert(alertstr);
adjusttext(1);
*/
   if(Ldihedral)
   {/*use 3 atoms + distance,angle,diheral to compute 4th atom position*/
      constructfourth(fcxyz,distance,angle,dihedral);
      /*uses 1st,2nd,3rd atoms [0,1,2], etc. rtns: 4th atom in index[3]*/
      gammaxyz[0] = fcxyz[3][0];
      gammaxyz[1] = fcxyz[3][1];
      gammaxyz[2] = fcxyz[3][2];
      ireturn = 1;
   }/*use 3 atoms + distance,angle,diheral to compute 4th atom position*/
   return(ireturn);
}/*shift gamma atom of ref structure*/
/*___constructgamma()________________________________________________________*/

/****constructfourth()********************************************************/
void   constructfourth(float fcxyz[4][3],float dist,float angle,float dihedral)
{/*construct at distance,angle,dihedral*/
   double ax=0,ay=0,az=0,bx=0,by=0,bz=0,cx=0,cy=0,cz=0,dx=0,dy=0,dz=0,cmag=0;
   float angledhdrl=0;

   /*calculate the new line from points 1,2,3, */
   /* dist 3-4, angle 2-3-4, dihedral 1-2-3-4 */
   /* cross:  (2->3)X(2->1) ==> (2->5), aXb ==> c at dihedral 90*/
   ax = fcxyz[2][0] - fcxyz[1][0];
   ay = fcxyz[2][1] - fcxyz[1][1];
   az = fcxyz[2][2] - fcxyz[1][2];
   bx = fcxyz[0][0] - fcxyz[1][0];
   by = fcxyz[0][1] - fcxyz[1][1];
   bz = fcxyz[0][2] - fcxyz[1][2];
   /* c = a x b */
   docrossproduct(ax,ay,az,bx,by,bz,&cx,&cy,&cz);/*return vector is c*/ /*.*/

/*
        printf("cross product:"
         CRLF"%f = %f x %f - %f x %f"
         CRLF"%f = %f x %f - %f x %f"
         CRLF"%f = %f x %f - %f x %f"
        ,cx,ay,bz,az,by
        ,cy,az,bx,ax,bz
        ,cz,ax,by,ay,bx               );
*/
   /*normalization factor*/
   cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) ); /*c*/

   /*get components of c normalized to correct distance*/
   if(cmag > 0.000001)
   {
       cx = cx*dist/cmag;
       cy = cy*dist/cmag;
       cz = cz*dist/cmag;
   }
   /*place point along c at correct distance from 3rd point,*/
   /* c is a xproduct, an so it is at a dihedral of 90 to a,b plane*/
   cx = cx + fcxyz[2][0];
   cy = cy + fcxyz[2][1];
   cz = cz + fcxyz[2][2];
   /*save x,y,z of point on c which is at the correct distance from 3rd point*/
   dx = cx;
   dy = cy;
   dz = cz;

   /*rotate d(xyz) around 2->3 for correct dihedral*/
   angledhdrl = dihedral - 90; /*since is perpendicular to a,b plane*/
   /*actual dihedral defined from a,b plane*/
   ax = fcxyz[1][0];
   ay = fcxyz[1][1];
   az = fcxyz[1][2];
   bx = fcxyz[2][0];
   by = fcxyz[2][1];
   bz = fcxyz[2][2];
   doaxisrot(&dx,&dy,&dz,(float)angledhdrl, ax, ay, az, bx, by, bz);
   /*now d(xyz) is at the correct dihedral*/

   /*Adjust d(xyz) so it is at the correct angle to 2->3 */
   /*rotate pt 4 around perpendicular to vectors 2->3 and 3->4 */
   /* cross:  (3->4)X(3->2) ==> (3->6), aXb ==> c at 90 to plane*/
   ax = dx - fcxyz[2][0];
   ay = dy - fcxyz[2][1];
   az = dz - fcxyz[2][2];
   bx = fcxyz[1][0] - fcxyz[2][0];
   by = fcxyz[1][1] - fcxyz[2][1];
   bz = fcxyz[1][2] - fcxyz[2][2];

   /* c = a x b */
   docrossproduct(ax,ay,az,bx,by,bz,&cx,&cy,&cz);/*return vector is c*/ /*.*/

   /*normalization factor*/
   cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) ); /*c*/
   if(cmag > 0.000001)
   {
       cx = cx/cmag;
       cy = cy/cmag;
       cz = cz/cmag;
   }

   /*rotate around 3->perpendicular for correct angle*/
   angledhdrl = 90 - angle;
   /*actual angle defined in plane from 0*/
   /*but 4th point was placed at 90 to 3->2 vector*/
   ax = fcxyz[2][0]; /*define a as the 3rd point*/
   ay = fcxyz[2][1];
   az = fcxyz[2][2];
   cx = cx + fcxyz[2][0]; /*move rotation vector to 3rd point*/
   cy = cy + fcxyz[2][1];
   cz = cz + fcxyz[2][2];
   bx = cx; /*define b as point on rotation vector*/
   by = cy;
   bz = cz;
   /*rotate point d around vector 3->(point on rotation vector)*/
   doaxisrot(&dx,&dy,&dz,(float)angledhdrl, ax, ay, az, bx, by, bz);

   /*load actual passed array with this newly determined 4th point*/
   fcxyz[3][0] = (float)dx;
   fcxyz[3][1] = (float)dy;
   fcxyz[3][2] = (float)dz;


}/*construct at distance,angle,dihedral*/
/*___constructfourth()_______________________________________________________*/

/****construct4th()***********************************************************/
void construct4th(float pt1[4],float pt2[4],float pt3[4],float pt4[4],float dist,float angle,float dihedral)
{/*construct at distance,angle,dihedral*/
   /*same as PKINANGL/constructforth except organization of calling params*/

   double ax=0,ay=0,az=0,bx=0,by=0,bz=0,cx=0,cy=0,cz=0,dx=0,dy=0,dz=0,cmag=0;
   float  angledhdrl=0;

   /*calculate the new line from points 1,2,3, */
   /* dist 3-4, angle 2-3-4, dihedral 1-2-3-4 */
   /* cross:  (2->3)X(2->1) ==> (2->5), aXb ==> c at dihedral 90*/
   ax = (double)(pt3[1] - pt2[1]);
   ay = (double)(pt3[2] - pt2[2]);
   az = (double)(pt3[3] - pt2[3]);
   bx = (double)(pt1[1] - pt2[1]);
   by = (double)(pt1[2] - pt2[2]);
   bz = (double)(pt1[3] - pt2[3]);
           /* c = a x b */
   docrossproduct(ax,ay,az,bx,by,bz,&cx,&cy,&cz);
           /*normalization factor*/
/*
        printf("cross product:"
                         CRLF"%f = %f x %f - %f x %f"
                         CRLF"%f = %f x %f - %f x %f"
                         CRLF"%f = %f x %f - %f x %f"
                        ,cx,ay,bz,az,by
                        ,cy,az,bx,ax,bz
                        ,cz,ax,by,ay,bx               );
*/
   cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) );

   /*scale c to length == distance*/
   if(cmag > 0.000001)
   {
       cx = cx*dist/cmag;
       cy = cy*dist/cmag;
       cz = cz*dist/cmag;
   }
   /*place 4th point at correct distance from 3rd point,*/
   dx = cx + (double)pt3[1];
   dy = cy + (double)pt3[2];
   dz = cz + (double)pt3[3];

   /*rotate around 2->3 for correct dihedral*/
   /*angledhdrl = dihedralconstruct - 90.0;*/
   /* as xproduct, c is at a dihedral of 90 to original vectors*/
   angledhdrl = dihedral - (float)90.0;

   ax = (double)pt2[1];
   ay = (double)pt2[2];
   az = (double)pt2[3];
   bx = (double)pt3[1];
   by = (double)pt3[2];
   bz = (double)pt3[3];
   doaxisrot(&dx,&dy,&dz,angledhdrl, ax, ay, az , bx, by, bz);

   /* cross:  (3->4)X(3->2) ==> (3->6), aXb ==> c at dihedral 90*/
   ax = dx - (double)pt3[1];
   ay = dy - (double)pt3[2];
   az = dz - (double)pt3[3];
   bx = (double)(pt2[1] - pt3[1]);
   by = (double)(pt2[2] - pt3[2]);
   bz = (double)(pt2[3] - pt3[3]);
      /* c = a x b */
   docrossproduct(ax,ay,az,bx,by,bz,&cx,&cy,&cz);
      /*normalization factor*/
   cmag = sqrt( (cx)*(cx) + (cy)*(cy) + (cz)*(cz) );
   if(cmag > 0.000001)
   {
       cx = cx/cmag;
       cy = cy/cmag;
       cz = cz/cmag;
   }

      /*rotate around 3->6 for correct angle*/
   angledhdrl = (float)90.0 - angle;
   ax = (double)pt3[1];
   ay = (double)pt3[2];
   az = (double)pt3[3];
   cx = cx + (double)pt3[1];
   cy = cy + (double)pt3[2];
   cz = cz + (double)pt3[3];
   bx = cx;
   by = cy;
   bz = cz;
   doaxisrot(&dx,&dy,&dz ,angledhdrl, ax, ay, az , bx, by, bz);

   pt4[1] = (float)dx;
   pt4[2] = (float)dy;
   pt4[3] = (float)dz;

}/*construct at distance,angle,dihedral*/
/*___construct4th()__________________________________________________________*/

/****perpendicular()**********************************************************/
void perpendicular(float f1xyz[4],float f2xyz[4],float f3xyz[4],float f4xyz[4])
{
   /*construct perpendicular from point to line*/
   /*perpendicular from 3rd point to line of points 1 and 2 */
   /* fourth point is on line at perpendicular to third point*/

   float  A1X,A1Y,A1Z,A2X,A2Y,A2Z,B1X,B1Y,B1Z,ADX,ADY,ADZ,B2X,B2Y,B2Z,ART;

        A1X = f1xyz[1];A1Y = f1xyz[2];A1Z = f1xyz[3];
        A2X = f2xyz[1];A2Y = f2xyz[2];A2Z = f2xyz[3];
        B1X = f3xyz[1];B1Y = f3xyz[2];B1Z = f3xyz[3];
        ADX = A2X - A1X;ADY = A2Y - A1Y;ADZ = A2Z - A1Z;
/*arbitrary point on line A
   B2X = A1X + ADX*ART;B2Y = A1Y + ADY*ART;B2Z = A1Z + ADZ*ART;

   direction components of line B1 --- B2
   BDX = A1X-B1X+ADX*ART; BDY = A1Y-B1Y+ADY*ART; BDZ = A1Z-B1Z+ADZ*ART;
   for this to be perpendicular to line A, sum of product of components ==0
   ADX*(A1X-B1X+ADX*ART)+ADY*(A1Y-B1Y+ADY*ART)+ADZ*(A1Z-B1Z+ADZ*ART)=0;
   rearrange
   ADX*A1X-ADX*B1X+ADX*ADX*ART
   +ADY*A1Y-ADY*B1Y+ADY*ADY*ART
   +ADZ*A1Z-ADZ*B1Z+ADZ*ADZ*ART=0;
   regroup
   ART*(ADX*ADX++ADY*ADY+ADZ*ADZ)
         =ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z;
*/
         ART =  (ADX*B1X-ADX*A1X+ADY*B1Y-ADY*A1Y+ADZ*B1Z-ADZ*A1Z)
               /(ADX*ADX+ADY*ADY+ADZ*ADZ);

         B2X = A1X + ADX*ART;
         B2Y = A1Y + ADY*ART;
         B2Z = A1Z + ADZ*ART;
         f4xyz[1] = B2X;
         f4xyz[2] = B2Y;
         f4xyz[3] = B2Z;
}
/*___perpendicular()_________________________________________________________*/

/****docrossproduct()*********************************************************/
void    docrossproduct( double ax,double ay,double az
                      ,double bx,double by,double bz
                      ,double* dxp,double* dyp, double* dzp)
{
    double gx,gy,gz;
    gx = (ay)*(bz) - (az)*(by);
    gy = (az)*(bx) - (ax)*(bz);
    gz = (ax)*(by) - (ay)*(bx);
    /*(*rx) = gx;*/
    /*(*ry) = gy;*/
    /*(*rz) = gz;*/
    *dxp = gx;
    *dyp = gy;
    *dzp = gz;
    if(Ltest>1)
    {
        sprintf(alertstr,"cross product:"
                         CRLF"%f = %f = %f x %f - %f x %f"
                         CRLF"%f = %f = %f x %f - %f x %f"
                         CRLF"%f = %f = %f x %f - %f x %f"
                        ,*dxp,gx,ay,bz,az,by
                        ,*dyp,gy,az,bx,ax,bz
                        ,*dzp,gz,ax,by,ay,bx               );
            pkintextinsert(alertstr); /*PKMCTEXT.C PKPCTEXT.C*/
            adjusttext(4);
    }
}
/*___docrossproduct()________________________________________________________*/

/****crossproduct()***********************************************************/
void crossproduct(double ax,double ay,double az,double bx,double by,double bz)
{
    double gx,gy,gz;

    gx = (ay)*(bz) - (az)*(by);
    gy = (az)*(bx) - (ax)*(bz);
    gz = (ax)*(by) - (ay)*(bx);
    /*(*rx) = gx;*/
    /*(*ry) = gy;*/
    /*(*rz) = gz;*/
    dblx = gx;
    dbly = gy;
    dblz = gz;
        if(Ltestprint)
        {
            sprintf(alertstr,    "cross product routine:"CRLF
                                "    %f = %f x %f - %f x %f"CRLF
                                "    %f = %f x %f - %f x %f"CRLF
                                "    %f = %f x %f - %f x %f"CRLF,
                                    dblx,(ay),(bz),(az),(by),
                                    dbly,(az),(bx),(ax),(bz),
                                    dblz,(ax),(by),(ay),(bx)    );
            pkintextinsert(alertstr); /*PKMCTEXT.C PKPCTEXT.C*/
            adjusttext(4);
        }
}
/*___crossproduct()__________________________________________________________*/

/****crossproduct3array()*****************************************************/
void crossproduct3array(double *a, double *b, double *g)
{
    g[0] = (a[1])*(b[2]) - (a[2])*(b[1]);
    g[1] = (a[2])*(b[0]) - (a[0])*(b[2]);
    g[2] = (a[0])*(b[1]) - (a[1])*(b[0]);
}
/*___crossproduct3array()____________________________________________________*/

/****dotproduct()***********************************************************/
double  dotproduct(double ax,double ay,double az,double bx,double by,double bz)
{
    return(ax*bx + ay*by + az*bz);
}
/*___dotproduct()__________________________________________________________*/

/****dotproduct3array()*******************************************************/
double  dotproduct3array(double a[3],double b[3])
{
    return(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}
/*___dotproduct3array()______________________________________________________*/

/****dihedral4pt()************************************************************/
double    dihedral4pt( double p1x,double p1y,double p1z
                      ,double p2x,double p2y,double p2z
                      ,double p3x,double p3y,double p3z
                      ,double p4x,double p4y,double p4z)
{
        double    angle,angledhdrl,dot,dmag,emag,fmag;
        double    ax,ay,az,bx,by,bz,cx,cy,cz;
        double    dx,dy,dz,ex,ey,ez;
        double    fx,fy,fz;
      /*double    dotproduct(double,double,double,double,double,double);*/

        /* a,b,c 3 vectors between 4 points, */
        /* d,e   2 crossproduct vectors referenced as pointers*/
        /* f     a crossproduct vector  referenced as pointer */
        /* if d = a x b and e = b x c, then d orth a & b, e orth b & c */
        /* d & e both perpendicular to b */
        /* so d dot e is the dihedral, that is */
        /*   same angle as a to c if a & c were projected on a plane*/

        /* angle defined between two vectors with a common base*/
        /* so subtract middle point from the two end points */
        ax = p1x - p2x;
        ay = p1y - p2y;
        az = p1z - p2z;
        bx = p3x - p2x;
        by = p3y - p2y;
        bz = p3z - p2z;

        /* d = a x b */
/*.*/    crossproduct(ax,ay,az,bx,by,bz); /*return vector is dblx,dbly,dblz */
        dx = dblx;
        dy = dbly;
        dz = dblz;

        /* angle defined between two vectors with a common base*/
        /* so subtract middle point from the two end points, */
        /* redefining b for cross product around third point (b = -b)*/
        bx = p2x - p3x;   /* -b as far as d is concerned */
        by = p2y - p3y;
        bz = p2z - p3z;
        cx = p4x - p3x;
        cy = p4y - p3y;
        cz = p4z - p3z;

        /* e = b x c */
/*.*/    crossproduct(bx,by,bz,cx,cy,cz); /*return vector is dblx,dbly,dblz */
        ex = dblx;
        ey = dbly;
        ez = dblz;

        /*Now for d dot e  pass values to dotproduct()*/
/*.*/    dot = dotproduct(dx,dy,dz,ex,ey,ez);
        /*normalization factors*/
/*c*/    dmag = sqrt( (dx)*(dx) + (dy)*(dy) + (dz)*(dz) );
/*c*/    emag = sqrt( (ex)*(ex) + (ey)*(ey) + (ez)*(ez) );
        if(dmag*emag <0.0001)
        {
            angle = 0.0;
        }
/*c*/    else angle = acos( dot/(dmag*emag) );
        angledhdrl = angle*360.0/(2*3.14159);
        if(Ltestprint)
        {
/*c*/        sprintf(alertstr,    "angle = %f  radians"CRLF"angle = %f  degrees"CRLF
                        ,angle,angledhdrl);
                pkintextinsert(alertstr); /*PKMCTEXT.C PKPCTEXT.C*/
                adjusttext(2);
        }

        /* Now, need to establish correct handedness */
        /* d x -b is vector pointing in same sort of direction as orig a */
        /* f = d x -b = d x b as b was redefined for e calculation */
/*.*/    crossproduct(dx,dy,dz,bx,by,bz); /*return vector is dblx,dbly,dblz */
        fx = dblx;
        fy = dbly;
        fz = dblz;

        /*Now for f dot e  pass values to dotproduct()*/
/*.*/    dot = dotproduct(fx,fy,fz,ex,ey,ez);

        /*normalization factors*/
/*c*/    fmag = sqrt( (fx)*(fx) + (fy)*(fy) + (fz)*(fz) );

        if(fmag*emag <0.0001)
        {
            angle = 0.0;
        }
/*c*/    else angle = acos( dot/(fmag*emag) );
        if(Ltestprint)
        {
/*c*/        sprintf(alertstr,    "angle = %f  radians"CRLF,angle    );
             pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
             adjusttext(1);              /*PKMCTEXT.C PKPCTEXT.C*/
        }

        angle = angle*360.0/(2*3.14159);
        if(angle > 90.0) angledhdrl = -angledhdrl;
        if(Ltestprint)
        {
/*c*/        sprintf(alertstr,    "signed angle = %f  degrees"CRLF
                        ,angledhdrl);
             pkintextinsert(alertstr);  /*PKMCTEXT.C PKPCTEXT.C*/
             adjusttext(1);             /*PKMCTEXT.C PKPCTEXT.C*/
        }

        return(angledhdrl);

}
/*___dihedral4pt()___________________________________________________________*/

/****angle3pt()*************************************************************/
double    angle3pt( double p1x,double p1y,double p1z,
                    double p2x,double p2y,double p2z,
                    double p3x,double p3y,double p3z )
{
        double    ax,ay,az,bx,by,bz;
        double    amag,bmag,dot;
        double    angle;

            /* angle defined between two vectors with a common base*/
            /* so subtract middle point from the two end points */
            ax = p1x - p2x;
            ay = p1y - p2y;
            az = p1z - p2z;
            bx = p3x - p2x;
            by = p3y - p2y;
            bz = p3z - p2z;

        /* these have to be normalized by dividing by their magnitudes, */
        /*this can be done either before taking the dot product or after*/
/*.*/    dot = (double)dotproduct(ax,ay,az,bx,by,bz);
/*c*/    amag = sqrt(ax*ax + ay*ay + az*az);
/*c*/    bmag = sqrt(bx*bx + by*by + bz*bz);
        if(amag*bmag <0.0001)
        {
            angle = 0.0;
        }
/*c*/    else angle = acos( dot/(amag*bmag) );
        angle = angle*360.0/(2*3.14159);
        return(angle);
}
/*___angle3pt()____________________________________________________________*/

/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
/****doaxisrot()*************************************************************/
void    doaxisrot(double *dxp,double *dyp,double *dzp,float theta
    , double ax,double ay,double az,double bx,double by,double bz)
{
   double xx,yy,zz,cosn1,cosn2,cosn3,costheta,sintheta;
   double fx1,fy1,fz1,fx2,fy2,fz2;
   double a11,a12,a13,a21,a22,a23,a31,a32,a33;
   int    LOK=1;

   xx = bx - ax;
   yy = by - ay;
   zz = bz - az;

   if(xx*xx + yy*yy + zz*zz > 0.000001)
   {
       cosn1 = (xx)/sqrt(xx*xx + yy*yy + zz*zz);
       cosn2 = (yy)/sqrt(xx*xx + yy*yy + zz*zz);
       cosn3 = (zz)/sqrt(xx*xx + yy*yy + zz*zz);
   }
   else
   {/*xx and yy and zz all must be very small, so*/
      /*cosn1,2,3 all very small and no way to normalize them*/ /*egam 030217*/
      /*and all off diagonal terms are very small*/
      /*also all diagonal terms use the square so even the sign not used*/
      cosn1 = xx;
      cosn2 = yy;
      cosn3 = zz;
      LOK = 0; /*not worth doing the matrix operation*/
   }
   while(theta >  360) theta = theta - 360;
   while(theta < -360) theta = theta + 360;
   if(theta >  180) theta = theta - 360;
   if(theta < -180) theta = theta + 360;

   if(theta > -.0001 && theta < 0.0001) /*030224*/
   {/*angle very small, off diagonal terms very small*/
      LOK = 0; /*not worth doing the matrix operation*/
   }

   if(LOK)
   {/*do matrix operation*/
      costheta = cos( (double)theta*3.14159*2/360);
      sintheta = sin( (double)theta*3.14159*2/360);

      a11=( cosn1*cosn1 + (1-cosn1*cosn1)*costheta);
      a12=( cosn1*cosn2*(1-costheta) + cosn3*sintheta );
      a13=( cosn1*cosn3*(1-costheta) - cosn2*sintheta );

      a21=( cosn1*cosn2*(1-costheta) - cosn3*sintheta );
      a22=( cosn2*cosn2 + (1-cosn2*cosn2)*costheta);
      a23=( cosn2*cosn3*(1-costheta) + cosn1*sintheta );

      a31=( cosn1*cosn3*(1-costheta) + cosn2*sintheta );
      a32=( cosn2*cosn3*(1-costheta) - cosn1*sintheta );
      a33=( cosn3*cosn3 + (1-cosn3*cosn3)*costheta);
      /*det=a11*(a22*a33-a23*a32)-a12*(a21*a33-a23*a31)+a13*(a21*a32-a22*a31);*/

      fx1 = ( *dxp - bx );
      fy1 = ( *dyp - by );
      fz1 = ( *dzp - bz );

      fx2 = (   fx1*a11 + fy1*a21 + fz1*a31 );
      fy2 = (   fx1*a12 + fy1*a22 + fz1*a32 );
      fz2 = (   fx1*a13 + fy1*a23 + fz1*a33 );

      *dxp = (float)(fx2 + bx);
      *dyp = (float)(fy2 + by);
      *dzp = (float)(fz2 + bz);
    }/*do matrix operation*/
}
/*___doaxisrot()____________________________________________________________*/
/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

int  constructcircle(float Cxyz[SEGMENTS][3], float Axyz[3], float Bxyz[3], float radius)
{
   /*taken from MAGEUTIL/arrowtoscreen() */

   /*circle is drawn around second of two points at angle=90 and at the radius*/

   int   ierr=0;
   float angle = 0;
   double fx0=0, fy0=0, fz0=0;
   double cmag=0,fx3=0,fy3=0,fz3=0,fx4=0,fy4=0,fz4=0;
   int   j=0;
   double fx1=(double)Axyz[0];
   double fy1=(double)Axyz[1];
   double fz1=(double)Axyz[2];
   double fx2=(double)Bxyz[0];
   double fy2=(double)Bxyz[1];
   double fz2=(double)Bxyz[2];
   double dxs=0, dys=0, dzs=0;
   double dblx=0,dbly=0,dblz=0;


   if( (fx2 == fx1) && (fy2 == fy1) && (fz2 == fz1) )
   {
      ierr=1;
      return(ierr); /*can't do circle*/
   }

   /* 1-------------->2   */
   /*Need 3rd point to give a dihedral direction for the circle radii*/
   /*Make off-axis point offset from the two points:*/
   /* use lesser of dimension change as the dimension to offset*/
   /*this stupid routine ignores sign change in any dimension*/

   /* 0                   */
   /* |                   */
   /* 1-------------->2   */
   dxs = (fx2 - fx1)*(fx2 - fx1);
   dys = (fy2 - fy1)*(fy2 - fy1);
   dzs = (fz2 - fz1)*(fz2 - fz1);
   if( dxs < dys)
   {/*del x < del y*/
      if(dxs < dzs) {/*x smallest*/ fx0 = fx1 + 10; fy0 = fy1; fz0 = fz1; }
      else {/*z smallest*/ fx0 = fx1; fy0 = fy1; fz0 = fz1 + 10; }
   }
   else
   {/* del y < del x*/
      if(dys < dzs) {/*y smallest*/ fx0 = fx1; fy0 = fy1 + 10; fz0 = fz1; }
      else {/*z smallest*/ fx0 = fx1; fy0 = fy1; fz0 = fz1 + 10; }
   }
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */

   /*calculate a new line from 2 using points 0,1,2 */

   docrossproduct( (double)(fx2-fx1),(double)(fy2-fy1),(double)(fz2-fz1)
                  ,(double)(fx1-fx0),(double)(fy1-fy0),(double)(fz1-fz0)
                  ,&dblx,&dbly,&dblz);
      /*return vector: dblx,dbly,dblz*/
   cmag = (float)sqrt( (dblx)*(dblx) + (dbly)*(dbly) + (dblz)*(dblz) );
   /*normalize and make c vector the desired length*/
   if(cmag < 0.000001) /*can't do circle*/
   {
      ierr=1;
      return(ierr); /*can't do circle*/
   }
   fx3 = dblx*(double)radius/cmag;
   fy3 = dbly*(double)radius/cmag;
   fz3 = dblz*(double)radius/cmag;
   /* 0                   */
   /* |                   */
   /* 1-------------->2   */
   /*                 |   */
   /*                 3   , where 2-->3 perpendicular to 0--1--2 */

   /*place point 3 at correct distance from point #2 */
   /* as xproduct it is at a dihedral and angle of 90 to 1--->2*/
   fx3 = fx3 + fx2;
   fy3 = fy3 + fy2;
   fz3 = fz3 + fz2;

   /*now have one of the radii of the circle*/
      Cxyz[0][0] = (float)fx3;
      Cxyz[0][1] = (float)fy3;
      Cxyz[0][2] = (float)fz3;
   /*and can calculate the other radii*/
   for(j=1; j<SEGMENTS; j++)
   {/*each time do same angle rotation from moving fx3,fy3,fz3 */
      /*rotate around 1-->2 for correct dihedral*/
      doaxisrot( &fx3,&fy3,&fz3
                ,(360/SEGMENTS)
                ,(double)fx1,(double)fy1,(double)fz1
                ,(double)fx2,(double)fy2,(double)fz2);
      /*store the radius vector*/
      Cxyz[j][0] = (float)fx3;
      Cxyz[j][1] = (float)fy3;
      Cxyz[j][2] = (float)fz3;
   }
   ierr = 0;
   return(ierr);
}
/*___constructcircle()_______________________________________________________*/

