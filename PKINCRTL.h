/*                      PKINCRTL.h                      */
/*only PKINCRTL defines PKINCRTL */

/*070801 LspecialResidue for now only LGFPchromophore*/
#ifndef PKINCRTL
 extern int  LGFPchromophore,Lunknownresidue,Lunuseableresidue;
#else
 int  LGFPchromophore = 0, Lunknownresidue=0, Lunuseableresidue=0;  /*070801*/
#endif


/*from J. Michael Word: probe/select.h,select.c 001119*/
typedef struct {char * rlist; char * alist; int bits;} ResidueAndAtomPair;

/* AAList commented out...
static char *AAList = ":GLY:ALA:VAL:PHE:PRO:MET:ILE:LEU:ASP:GLU:LYS:\
ARG:SER:THR:TYR:HIS:CYS:ASN:GLN:TRP:ASX:GLX:ACE:FOR:NH2:NME:MSE:AIB:ABU:PCA:\
MLY:CYO:M31:";*/
  /*mly added 001114 for myosin 2MYS methylated lysine*/
  /*cyo added 010708 for S-LECTIN 1SLT oxidized cys*/
  /*m3l added 041011 for methylated lysine== Methyl 3 Lysine*/

  /*****************************************************************/
  /*use of character array and C utility strstr() from select.c    */
  /*probeV2.3.5.source, Oct 14, 2000:  J. Michael Word             */
  /*****************************************************************/
  /*char *strstr(cs,ct) return pointer to first occurrence of */
  /*string ct in cs, or NULL if not present*/

#ifndef PKINCRTL
 extern char *aaList;
#else
 char *aaList = ":gly:ala:val:phe:pro:met:ile:leu:asp:glu:lys:\
arg:ser:thr:tyr:his:cys:asn:gln:trp:asx:glx:ace:for:nh2:nme:mse:aib:abu:pca:\
mly:cyo:m3l:dgn:csd:";
#endif
  /*mly added 001114 for myosin 2MYS methylated lysine*/
  /*cyo added 010708 for S-LECTIN 1SLT oxidized cys*/
  /*m3l added 041011 for methylated lysine== Methyl 3 Lysine*/
  /*dgn added 051116 for D GLN re 1yj1.pdb*/
  /*csd added 070730 for alt A oxidized (SO2) of alt B CYS 123 in 1DIN.pdb */
  /*cro NOT added 070730 GFP type chromaphore from 1EMA.pdb, wierd het better*/
  /*    prekin can't do mc vs sc by atom names, disconnected het looks better*/
  /*    ribbon would break anyway ???? */

#ifndef PKINCRTL
 extern char *NAList; /** NAList UPPER CASE List */
#else
 char *NAList = ":  C:  G:  A:  T:  U:CYT:GUA:ADE:THY:URA:URI:\
CTP:CDP:CMP:GTP:GDP:GMP:ATP:ADP:AMP:TTP:TDP:TMP:UTP:UDP:UMP:\
GSP:H2U:PSU:4SU:1MG:2MG:M2G:5MC:5MU:T6A:1MA:RIA:\
OMC:OMG: YG:  I:7MG:C  :G  :A  :T  :U  :YG :I  :\
 rC: rG: rA: rT: rU: dC: dG: dA: dT: dU: DC: DG: DA: DT: DU:"; /*070726*/
#endif
/*CCP4 prefix for ribo and deoxyribo, 200707 PDB flags just deoxy 070726 */

#ifndef PKINCRTL
 extern char *naList; /** naList lower case List */
#else
 char *naList = ":  c:  g:  a:  t:  u:cyt:gua:ade:thy:ura:uri:\
ctp:cdp:cmp:gtp:gdp:gmp:atp:adp:amp:ttp:tdp:tmp:utp:udp:ump:\
gsp:h2u:psu:4su:1mg:2mg:m2g:5mc:5mu:t6a:1ma:ria:\
omc:omg: yg:  i:7mg:c  :g  :a  :t  :u  :yg :i  :\
 rc: rg: ra: rt: ru: dc: dg: da: dt: du: dc: dg: da: dt: du:";  /*070726*/
#endif
/*7mg added 001114 for tRNA 1EHZ, uri 040228*/
/*4su added 061001 for tRNA, 3rd tRNA in 1JGO.pdb*/
/*should match uppercase version: NAList*/

/* used to treat similar nucleic acid bases similarly */
/*     rearranged version of Mike's table*/
 enum{ baseA=2,baseTU=3,baseG=4,baseC=5,baseOther=6};/*PREKIN internal codes*/
#ifndef PKINCRTL
 extern ResidueAndAtomPair NAbaseGrouping[];
#else
 ResidueAndAtomPair NAbaseGrouping[] = {
":  A:ADE:ATP:ADP:AMP:1MA:RIA:T6A:A  :",                 "", baseA,
":  U:URA:UTP:UDP:UMP:PSU:4SU:  T:THY:TTP:TDP:TMP:5MU:U  :T  :URI:H2U:", "", baseTU,
":  G:GUA:GTP:GDP:GMP:GSP:1MG:2MG:M2G:7MG:OMG:G  :",     "", baseG,
":  C:CYT:CTP:CDP:CMP:5MC:OMC:C  :",                     "", baseC,
":  I:I  :",                                         "", baseOther,/*like A?? */
": YG:YG :",                                         "", baseOther,/*purine*/
0, 0, 0
};
#endif
/*A type: pink*/
/*T,U type: sky*/
/*G type: sea*/
/*C type: yellow*/
/*misc type: white*/

#ifdef SOMEDAYNEEDTHIS  /*070731*/
#ifndef PKINCRTL
 extern char *pyrimidineRNAList; /** NAList and naList categorized */ /*070731*/
#else
 char *pyrimidineRNAList = ":  C:  c:  U:  u: RC: rc: RU: ru: RT: rt:\
";
#endif
#ifndef PKINCRTL
 extern char *purineRNAList; /** NAList and naList categorized */  /*070731*/
#else
 char *purineRNAList = ":  G:  g:  A:  a: RG: rg:\
";
#endif
#ifndef PKINCRTL
 extern char *pyrimidineDNAList; /** NAList and naList categorized */ /*070731*/
#else
 char *pyrimidineDNAList = ": DC: dc: DU: ru: DT: dt:\
";
#endif
#ifndef PKINCRTL
 extern char *purineDNAList; /** NAList and naList categorized */  /*070731*/
#else
 char *purineDNAList = ":  G:  g:  A:  a: DG: dg: DA: da:\
";
#endif
#endif /* SOMEDAYNEEDTHIS */  /*070731*/
