
/***PREKINinit.c*************************************************************/
/* PREKIN reads Brookhaven PDB file and produces a file in kinemage format
 *(that can be read by MAGE) ready to be edited into the kinemage as desired
 * by the author.
 * MAGE  shows KINEMAGEs:
 */
/* PREKIN determines the connectivity (covalent and H-bond) of the
 *  coordinates,
 *  sorts the vectors into hierarchical sets,
 *  associates identifying text with each vector,
 *  allows range and focus selection.
 *
 * Attribution Notice:
 * PREKIN program: copyright c David C. Richardson,
 *  1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005
 *                         all rights reserved
 *         Little River Institute, 5820 Old Stony Way, Durham, NC 27705
 *
 * Use and distribution for non-profit educational and research purposes
 * encouraged. Rewrites for this and other platforms encouraged for similar
 * use and distribution.
 *    Contact David C. Richardson, Little River Institute, for details.
 * Concept and MAGE  Developed by David C. Richardson and Jane S. Richardson
 *    "Kinemage" name invented by Caroline P. Usher
 * Grateful acknowledgement of many years of support from NIH, grant GM15000,
 * which included visualization of protein structures and the development of
 * CHAOS: display program written in E&S PS300 function-net language,
 * CHAosIN Fortran program,  etc.
 *    Grateful acknowlegement to Fred Brooks and the UNC GRIP team for
 * many years of exposure to Computer Science and developing my sense of
 * what can and should be done with computer graphics.
 *
 *   Although this code obviously bears resemblance to many examples of .c
 *   and Mac code, one hopes this combination and construction departs from
 *   the examples far enough so that there is no danger of copyright
 *   infringement, nor blame for failings, going back to earlier work.
 *   On the other hand, there is a real intellectual debt owed.
 *   In particular:
 *     Think C development system, examples, and manual, Symantec Corp. 1989
 *      and several timely hints from Think C technical Support!
 *     Apple Inside Macintosh, Addison-Wesley, 1985
 *          (the essentual and eventually preferred ref.)
 *     Several suggestions and a look at real code by Warren Robinett.
 *     Using the Macintosh Toolbox with C,
 *            F.A.Huxham, D.Burnard,J.Takatsuka 2nd Ed. Sybex,1989
 *     Macintosh C Programming Primer, Vol. II, D. Mark, Addison-Wesley, 1990
 *     Robert Weiss who began making useful suggestions even before he signed
 *         on to help rewrite the Mac program for Windows 3.1..
 *     Brent Presley who did the port to Win32 and did much code reconciliation
 *         as well as helped clean up the program flow logic.  971029
 *     J. Michael Word who gave me a lot of tips, pieces of code, and a real
 *         appreciation of good programming practice.
 *     Ian W. Davis whose MolProbity, KiNG, and building prekin-like features
 *         in KiNG, has pushed further development of this old prekin.
 ****************************************************************************/

#include "PKIN.h"
#include "PKINDLOG.h"
#include "PKINRIBB.h" /*051025*/

FILE  *fphelp;

void writeversiontooutput(void); /*041011*/
void writechangestooutput(void); /*040215*/
void writecmdhelptooutput(void); /*040425*/

static char* prekinchanges[] =
{"\r"
,"020514 Cbeta:Superpos,NB:Prekin,IOmage\r"
,"020529 Cbeta:Superpos,NB:Prekin,IOmage,orig-rot-hypertext\r"
,"020614 6.10 Cbeta,IOmage,altmutants\r"
,"021213 6.14 Bval-ribbons\r"
,"030111 6.20 MODEL work\r"
,"030125 6.20 vectorCO :perpendicular vector from Ca--Ca to Carbonyl Oxygen\r"
,"030127 6.20 angles for vectorCO\r"
,"030128 6.20 Calpha---Calpha angle,dihedral,azimuth to CO\r"
,"030129 6.20 pdb output for vectorCO \r"
,"030202 6.20 vectorCO IO pdb CaCO in, full mc pdb out\r"
,"030203 6.20 cis pro\r"
,"030223 clean up of 030221, 030224 6.24 bug in doaxisrot\r"
,"030228,308 6.25 ranges dialog unix rework,PC fail gracefully running quiet\r"
,"030401 6.28 atomradii, 030405 on mutation also\r"
,"030502 6.29 report to stdout \r"
,"030515,19,20,27 6.30 Phosphate perpendiculars to Base by commandline\r"
,"030605,07 6.30 clean up pro mutation & cbetadev; do gray H on hets \r"
,"030706 nterm beta ribbon fudge for rasmol 3D printer\r"
,"030920 6.31 cns H names, extra ring closure atoms called ghost\r"
,"030925 rotatorname incl sub==chainID \r"
,"031001 6.32 mutation full sc for alt conf sc\r"
,"031124 6.33 altb,altc cbetadev even for no alternative mainchain atoms\r"
,"031228 short colorset names: mage limit 14 characters\r"
,"040202 6.34 040211 pperpdump \r"
,"040215 clean up range-external-script, external-script should be obseleted\r"
,"040222 pperptobase removed residue numbering dependence (bug)\r"
,"040226-29 RNA & DNA ribbons, ranges for nucleic acid ribbon with arrowheads\r"
,"040303 6.35 output mutated residue in PDB format\r"
,"040304 matrix, center input --> transformed PDB\r"
,"040318 -cbetadevmax single #, USER MOD limit==100\r"
,"040319,23,24 ribbon perps,avoid duplicate beta strand ends, #chains 255\r"
,"040406 ribbon fakes CO for GFP, rearranged naba,aasc output\r"
,"040419 NOT CARRIED FORWARD: doubled nucleic acid spine guide points\r"
,"040420,23,24 curvature fudged nucleic acid spine guide points\r"
,"040425 html help initiated\r"
,"040504,09 color53 in bestribbon, more levels for huge, multichain molecs\r"
,"040609 6.36 clean up\r"
,"040620 correct axes on pperpplot\r"
,"040705 pperpoutliers only, delta added to dump\r"
,"040902 cmdline water balls, -listmaster \"name\", cmdline color by occ\r"
,"040918 6.40 black edge on explicit structure-type ribbon sections\r"
,"040924 black ribbon edge option\r"
,"041001 Unpickable foot for pperpoutliers muli-crit\r"
,"041011 (MSE selenomet mutation present) thumbnail: -nohetconnect, -version\r"
,"041017 ribbon blackedge clean up, foo atomradii 2.0 if not n,c,o,p,s\r"
,"041020 -ballradius atom marker choice, no balls for hydrogen\r"
,"041023 cis peptide calc reported in -vectorCO command\r"
,"041029 white pt color on hets for colornc \r"
,"041031 new colors for subunits, cbetadev output only bad ones\r"
," -ribnhet: -arrows -colornc -blackedge -nohetconnect -ballradius 0.32 -show \"ht,at\"\r"
,"041102 commandline -vectorCO with cispeptides emphasized\r"
,"041102 GUI ranges: option cispeptides emphasized, commandline: -cispep \r"
,"041103,04 GUI ranges: option bestribbon, bestribbon + cispeptides \r"
,"041105 ribnhet allows Ca-only spline...\r"
,"041106 bestribbon list colors discriminate multiple chains as default\r"
,"      -bestribbon == -arrows , convergent evolution complete!\r"
,"RCSB: -ribnhet: -bestribbon -colornc -blackedge -nohetconnect -ballradius 0.32 -show \"ht,at\"\r"
,"041106 distinguished  -cispeponly vs -ribbon -cispep\r"
,"041107 bug fix: chain specific hetatm HOH residue numbers foul ribbon ranges\r"
,"    Lbestribbon uses variable width ribbon for chain without specifications\r"
,"    -passkeyword \"...\" : keyword to pass to mage, e.g. @whitebackground \r"
,"041115 -cispep: cis-peptide now recognized even if Cterm residue\r"
,"041205 -cbstubs : ribbon---CB stubs for sidechains\r"
,"050121 nmrmodel in bondrot rotatorname, and group and subgroup names.\r"
,"  balllist pointmaster alt a,b,c fixed\r"
,"  existing option now in -help: specify sets off e.g.  -show 'mc(red)[off]'\r"
,"(050125 revived 050324: pperp to nucleotide c1'--n line did not work.\r"
,"050204 multistrand ribbon output in VRML 2.0 format (extrusion) \r"
,"050206,08,09 sidechain stubs for selected sc on ribbon\r"
,"050211 multistrand ribbon output in VRML 1.0 format (cylinder & ball-joint)\r"
,"050212 more ribbon work, including beta strand arrows for VRML output.\r"
,"( NOT IMPLEMENTED make pointID match pdb format atom & res name...)\r"
,"050309 MAXRanges from 101 to 301 for MolProbity Multicrit bad rotamers\r"
,"Maxranges was NOT the problem..., size of ranges char str was the problem.\r"
,"050310 protect total ranges by MAXR, strobe out full strlen(), one # ranges\r"
,"050324 translate added to matrix so can dump PDB biological related coords.\r"
," revived 050125 pperp to nucleotide c1'--n line to test Xueyi's results.\r"
,"050324 is held as distribution version until later stuff settles down... \r"
,"050327 NOfudge and ribbon-guide-points output for how to draw show-n-tell\r"
,"050327 default Lhylens==0 so NOT do lens for hydrogens now is lots default\r"
,"050329 more VRML tinkering: white ribbon, simpler attribute designations\r"
,"050403 CA---CB vector defined in terms of ribbon guide points.\r"
,"050409 code for use of CONECT records started, not yet implemented ...\r"
,"050409 try redefinition of imbedded HETATMs in P or N chains as ATOMS\r"
," THIS WORSE for GFPs! hetatm code much better than atom to get something!\r"
,"050630 resume work from 050409 code... \r"
,"IWD050617: All balls are marked alt B (even those for alt A)\r"
," -050704- cannot reproduce this, need example. Probably already fixed.\r"
,"IWD050617: Color by OCC/B still leaves SS as yellow (Prekin bug?)\r"
," -050704-fixed.\r"
,"050704 Bcol subroutined, applied to -SS- \r"
,"IWD050617: pointmasters not on the start of vectors (those flagged P)\r"
," -050705- Need pointmaster on a P type point,\r"
,"   even though vector is controlled by its L point.\r"
,"   KiNG will still pick that unshown P pt, so need to turn it off also.\r"
,"IWD050617:dangling altB C--N for 1SWU B 29(whenever i has all A and B alts)\r"
," -050705 case of oldconnect carbonyl atoms are alt a and b, to one nitrogen\r"
,"   this would need remembering alts of base for 2 c--n vectors,\r"
,"   and forcing a new P nitrogen to make a non-alt vector to next Calpha.\r"
,"   This would be a lot of work in old prekin to fix a minor annoyance.\r"
,"IWD050617: unwanted @listcolordominant for multi-subunit ribbons in multi-crit\r"
,"  -050705-  pt color is rainbow on each, list color distinguishes subunits.\r"
,"   This was deliberate for RCSB -ribnhet option, now option specific.\r"
,"050705 pointmaster on P matches that on following L, for KiNG picking \r"
,"050705 Llistcolordom flagged only for ribnhet option (as of now).\r"
,"050705 restore use of list for hetatm residues that can be treated as atoms\r"
,"050731 ...accummulate changes for next issue... \r"
,"050916 make usuable version, many loose ends... \r"
,"050917 SITE residues by number,chainID,insertion_code... \r"
,"  prekinz -bestribbon -scope -siterec -cbstubs -scballs -show \"ht,at\" -ballradius 0.32 1RUVbeta.pdb >1RUVbeta.site.kin \r"
,"050920 max ranges to 900: ranges includes ribbon regions and site residues\r"
,"050922 multi-subunits in NMR models correctly associated with their models \r"
,"050923--25 rework Alpha,Beta ranges to work with all the MODELs \r"
,"050925 -ballradius applies to -scballs as well as -show \"at\" \r"
,"050928 rework ranges: separate general ranges, ribbon ranges, site ranges\r"
,"050929 cleaned up PC dialog boxes\r"
,"051005 minor housecleaning\r"
,"051007 alternate PKINRIBBon code... NOT IMPLEMENTED YET...\r"
,"051102 add codestr text to all pointIDs\r"
,"051108 CPK also for nucleic acid mc P\r"
,"051112 continue PKINRIBB coding...\r"
,"051114 -spline :meaning changed 051208, use -strand 1 for single mid strand\r"
,"       -spline :meaning changed back 140520: single mid strand vectorlist\r"
,"       -skeins 140520 shows all ribbon-defining strands\r"
,"       -strand # :  # stranded ribbon, local curvature offsets.\r"
,"       -name: name sustitutes for MolNameStr in chain names\r"
,"051116 rainbow color across atom-hetatm-atom chain changes: colorbyNtoC \r"
,"051126 -newribbon tries out new ribbon code, PKINRIBB\r"
,"051128 fix old ribbon code. alpha,beta,coil masters optional -ribbonmasters\r"
,"051129 -listname \"name\"  :13 char max: {name} for all lists\r"
,"       -nosubgroup   for MolProbity micromanagement\r"
,"051130 fix -colornc\r"
,"051201 ordered masters for water,hets when -nogroup or -onegroup\r"
,"051204,6,8 more PKINRIBB...\r"
,"051209 -spline to show newribbon raw splines, see old ribbon -guide\r"
,"051212 much newribbon stuff, incl -hbonds for ribbons\r"
,"051213\r"
,"prekinz -ribbon -newribbon -strand 5 -scope -siterec  -cbstubs -sc cys  -scope -siterec -scballs -show \"at\" -ballradius 0.32 -scope -sc cys -show sc 1RUVbeta.pdb >1RUVbeta.site.kin\r"
,"prekinz -bestribbon -newribbon -scope -siterec  -cbstubs -sc cys  -scope -siterec -scballs -show \"at\" -ballradius 0.32 -scope -sc cys -show sc 1RUVbeta.pdb >1RUVbeta.site.kin\r"
,"prekinz -bestribbon -newribbon -scope -siterec  -cbstubs -sc cys  -scope -siterec -scballs -show \"ht,at\" -ballradius 0.32 -scope -sc cys -show sc 1RUVbeta.pdb >1RUVbeta.site.kin use for ribnhet, remember to restore quotes\r"
,"old RCSB: -ribnhet: -bestribbon -colornc -blackedge -nohetconnect -ballradius 0.32 -show \"ht,at\"\r"
,"051217\r"
,"prekinz -bestribbon -newribbon -hbonds -scope -siterec  -cbstubs -sc cys  -scope -siterec -scballs -show \"ht,at\" -ballradius 0.32 -scope -sc cys -show sc 3PCC.pdb > 3PCC.kin   mulltiple subunits, some fragmented\r"
,"051222... kinvrmler takes over from prekin for newribbon kinemages\r"
,"060113 guide points, splines to TEST new ribbon\r"
,"060114 Nucleic ribbons A-form vs B-form by existence of O2prime\r"
,"060115 newribbon axial x spline (-axis), and its local density (-density)\r"
,"060120 -nobutton -residuegroups (<1024) (control with animate)\r"
,"060208 pperpoutlier redefined optimally for pperp-to-line. \r"
,"       rinhets: only s,p yellow; other non-cno as bigger gray metals\r"
,"       Virtual BB for nucleic pseudo-backbone\r"
,"060212 unneeded \"space\" removed from master button specification*/\r"
,"6.43.060216 -range cmdline ,-site records, need different range counting\r"
,"       logic obscurely differs for -ribnhet vs -range badrotamers \r"
,"060322 fix segfault: PKINFILE codestr: limit imposed\r"
,"060324 NOGUI option, esp for MolProbity generic Mac with no MOTIF libraries\r"
,"060227 Makefile:  make nogui  now doesNOT specify -lXm \r"
,"060531 seqfault fix: PKINCSUB avoids failure at textblock resize(PKINSCRT)\r"
,"6.45.060929 NUCLEIC fake BETA SHEET names--> colorsets, -newribbon default\r"
,"061001 allow p--p--p as well as p--c4'--p  as nucleic version of ca--ca--ca\r"
,"       reworked connectcaca,connectheavy,oldcon\r"
,"       HETATM nucl: if sc before mc: c1'-n1 part of mc: sc gap to virtualBB\r"
,"061004 arrowhead where ribbon changes rib-identity as well as sec-str end\r"
,"061006 mc and virtual-BB continuity from nucleic to protein residue (tRNA)\r"
,"061011 RIBB fudges a Calpha atom at non-residue Nterm group, e.g. acyl\r"
,"061019 one line NOGUI prekin -version\r"
,"061020 fixed logic with invalid command or file name\r"
,"061021 BETAARROW head logic changed in PKINRIBB.c\r"
,"061029 colorNtoC counter changed for more even blue...red\r"
,"061202 residue connectivity report work ... Lconnectresiduesdump\r"
,"       workarounds needed:  hetatm H given masters and gray color\r"
,"          loop to try treating hetatm in field of atom as if atom\r"
,"070323 VIZ talk Owen Actrachan : qsort is evil...\r"
,"070512 fix help for atom balls, lone previous nucl O3*,(lots for nucs ?)\r"
,"6.50.070520 first cut pdb new atom names\r"
,"070521 nucleic pprep calc, includes epsilon, both use P of next residue.\r"
,"070627 pprepdump incl insertion codes \r"
,"070726 second cut pdb new atom and residue names\r"
,"070727 fixed dumb mistake in setting element name as records are inputed \r"
,"       now use inputed or derived element name for all atoms\r"
,"070728 SITE atoms for newpdb (v3.0), chainID lowercase within prekin\r"
,"ribnhet with site... use \"ht,at,hy\" to show Hydrogens...\r"
,"prekinz -bestribbon -newribbon -scope -siterec  -cbstubs -sc cys  -scope -siterec -scballs -show \"ht,at\" -ballradius 0.32 -scope -sc cys -show sc 1RUV.pdb >1RUV.site.kin\r"
,"070730 prekin broken between 070726 and 070727 for old v2.3 pdb files...\r"
,"     whatelement() at input made more robust for v2.3 and v3.0\r"
,"     HET groups not include recognized hydrogens unless hydrogens requested\r"
,"       ...starting to annotate V3UNSAFE in prekin code...\r"
,"070731 #ifdef GFPchromophore : NOT quite works, NOT implemented\r"
,"070731 first 15 char of atom pointID == cols 13--27 inclusive PDB \r"
,"    NB not all kinemage points are atoms, eg ribbon ptID only incl residue\r"
,"    Nucleotide: pperp only done if find O2' atom, i.e. RNA NOT DNA \r"
,"  Pperp kin outliers indicate 2'<3A, 3'>=3 consistent with MolProbity chart\r"
,"070801 GFP chromophore special case of het within protein chain, ribnhet OK\r"
,"070803 clean up, more work toward ribnhetsite\r"
,"070829 decidemainside flagged for LGFPchromophore (avoid Nucl problem)\r"
,"070926  2 char chainID  \r"
,"6.51.071001 Hybrid36 Atom number as string: was ncnt[n] now Anum[n]\r"
,"071002 PKINHY36.c == hybrid_36_c.c from http://cci.lbl.gov/hybrid_36/\r"
,"071215 v3.0 format mutation library\r"
,"071222 try to find bug in mutation-bondrot stuff...\r"
,"080318 try to figure out 071222, fixed crash on -ribbon: 2char chainID \r"
,"080325 cut -libXp out of Makefile.linux : denegrated, not used\r"
,"       2char chainID cbetadevdump output\r"
,"080326 080318 broke recognizing hydrogen names: element can be either case\r"
,"080329 linux only ribbon crash: need initial NULL splines,chords,etc.\r"
,"080410 fix disulfide code: connectssbonds just guard against same resnumber\r"
,"080915 -range not being recognized: must order commands:\r"
,"FOR A -scope SET -range \"#-#,#-#,...\" -show \"mc,sc,...\" FOR EACH RANGE\r"
,"081112 fix mchb to always calculate its own H of NH (avoid H name issues)\r"
,"081120,081122 fix ribbon failure when using ribnhet -model \"1\"...\r"
,"090219 now try to fix ribnhet failure...\r"
,"090223 dirty trick: hetatm to atom for prot/nuc ribbons within P or N chain\r"
,"090622,\r"
,"090704 ribbon coil needs black outline...\r"
,"090924 vbc3 reconcile 090223, 090304, and 090704...\r"
,"091129 -operp pperp code for RNA uses O3' instead of next residue P\r"
,"100322 incorporate Calpha + Oazimuth angle from PKINRIBN.070731 to PKINRIBB\r"
,"101024 try to recover plain ribbon output -- nothing done for this\r"
,"101225 tinker output for magekinjas: 3 mc, 3 sc, places in PKINCSUB\r"
,"111106 line 876 PKINRIBB discon=0 always to do VTF fakeCa plots\r"
,"121031 compile for only i386\r"
,"130121 flag -ca3rib use perp to ca-1,ca,ca+1 instead of C=O for rib plane\r"
,"       Makefile for 10.8.2, recompile...\r"
,"       compiler logic warning: beware PKINCSBS:1133:cbstubsrib \r"
,"130317 6.54 integrate 090924 vbc3 with 130121 dcr code into svnwork/dcrwork\r"
,"130318 connecthydrogenbyname supersede connecthydrogen by distance\r"
,"130324 ... very tricky, empower -report for suspect connections \r"
,"130329 pperpoutlier limits slightly changed\r"
,"130406 PKINRIBB prune flawed segment end residue, better alloc and tests\r"
,"130415 isahydrogen() elem dominant to name, now elem recognizes D Deuterium\r"
,"130508 jjh added -segid option to use segid instead of chain id\r"
,"       added functionality to read segid field\r"
,"       cleaned up compiler warnings\r"
,"130725 investigate 3D printing setup for ribbons... \r"
,"140520 ribbon code to make minimally specified center spline... \r"
,"       -spline :meaning changed back 140520: single mid strand vectorlist\r"
,"       -skeins 140520 shows all ribbon-defining strands\r"
,"141223 sj changed the RNA delta angle range for RNA pucker analysis to match suitename and data from RNA11 \r"
,"\r"
,"\r"
,"END\r"
}; /*prekinchanges[]*/

/****getversion()*************************************************************/
void getversion()
{
   char *OS;
   char guiness[64];

   PREKINVERSION = (float)6.57;
   sprintf(PREKINDATE,"141223 ");

   OS = (char *)operatingsystem(); /*PUXMLNX.c,PUXMOSX.c,... or ____INIT*/
   guiness[0] = '\0'; /*060324 initialize to take no space in output str*/
#ifdef NOGUI
   sprintf(guiness," NOGUI");
#endif

   if(OS != NULL)
   {
      sprintf(version,"version %.2f,%s dated: %s%s"
         ,PREKINVERSION,OS,PREKINDATE,guiness);
   }
   else
   {
      sprintf(version,"version %.2f dated: %s%s"
         ,PREKINVERSION,PREKINDATE,guiness);
   }
}
/*___getversion()____________________________________________________________*/

static char* prekincmdhelp[] =
{"\r"
,"     This \"help\" is for running prekin from the command line.\r"
,"A partial command line mode (meant for a user to type in) allows specification\r"
,"of input and output files and/or some flags, but invokes the GUI dialog boxes\r"
,"for the rest of the missing information. \r"
,"A fully command-line, or \"quiet\", mode (with no GUI) allows all prekin\r"
,"controls to be specified on the command line; complex examples of such\r"
,"command lines are often generated by a program or may form part of a stored \r"
,"UNIX script, but simple ones such as \"prekin -lots in.pdb >out.kin\" can\r"
,"conveniently be typed by the user.  Some of these prekin flags (like \"-lots\"\r"
,"invoke quite complicated combinations, the same ones available through\r"
,"prekin's GUI built-in scripts dialog box.  Others are like the options in\r"
,"the range-control dialog box.\r"
,"\r"
,"\r"
,"COMMAND LINE options useful in partial as well as full command line mode:\r"
,"-in filename -out filename  \r"
," first unflagged name -> input file, second unflagged name -> output file\r"
," first  \"-\"  flags stdin; second  \"-\" flags stdout, making functional: > >> |\r"
,"-oldpdb :mutations, substitutions: old atom names, default is new v3.1 names\r"
,"-append :opens output in append mode, does NOT write @text,@kinemage,...\r"
,"-notext :does NOT write @text, but does issue @kinemage\r"
,"-nogroup :does NOT write @group...  keyword line\r"
,"-nosubgroup :does NOT write @subgroup...  keyword line\r"
,"-onlygroups :group for each chain, masters for subgroups\r"
,"-onegroup :one group each molecule or model, masters for chains, subgroups\r"
,"-allbuttons :show all group,subgroup,list buttons (overrides -xxxxgroup)\r"
,"-nosubgroupmasters :(default) NOT write a master={} on @subgroup... lines\r"
,"-subgroupmasters :DOES write a master={} on @subgroup... lines\r"
,"-listname \"name\"  :13 char max: {name} for all lists\r"
,"-listmaster \"name\"  :14 char max: master={name} for all list masters\r"
,"  use along with -nogroup and default of -nosubgroupmasters for special \r"
,"  calls to get a single list, e.g. for bad rot sc, or bad rama mc list\r"
,"-nohetconnect : does not connect het-group fragments, e.g. for thumbnail\r"
,"\r"
,"QUIET BUILT-IN SCRIPTS for full command line mode, NO control dialogs\r"
,"e.g.   prekin -lots name.pdb >name.kin  \r"
,"+selections: e.g. prekin -lots -show \"wa(gold)[off]\" name.pdb >name.kin \r"
,"-browse :Launch Mage: Calpha,-ss-(nucleic acid pseudo-backbone),non-H2O hets\r"
,"-cass :Calphas & -ss- (nucleic acid pseudo-backbone) non-H2O hets\r"
,"-mchb :mainchain and its h-bonds\r"
,"-aasc :individual amino acids in separate sets and Calphas\r"
,"-lots :mc, sc, ca, (hy), (hets), H2O balls\r"
,"-naba :individual nucleic acid bases in separate sets and lots else\r"
,"\r"
,"-ribnhet: -bestribbon -colornc -blackedge -nohetconnect -ballradius 0.32 -show \"ht,at\" \r"
,"-ribbonmasters: optional masters for alpha,beta,coil\r"
,"-ribbon :variable width as a function of local curvature\r"
,"-newribbon : using code in PKINRIBB default as of prekin.6.45.060929\r"
,"-oldribbon : using code in PKINRIBN until it finally dies...\r"
,"-hbonds : newribbon pseudo-hbonds approx where PKINHBND would place them\r"
,"-byresidue : newribbon skeined ribbons has vectorlist for each residue\r"
,"-arrowheads : newribbon skeined ribbons, arrowheads on beta (SHEET records)\r"
,"-arrows :arrows on beta, ribbons on alpha, by PDB SHEET,HELIX records\r"
,"-bestribbon :ribbon by PDB SHEET,HELIX if present, else single strand coil\r"
," -bestribbon == -arrows , convergent evolution complete!\r"
,"-rnaribbon : force nucleic acid ribbons to be Aform type\r"
,"-dnaribbon : force nucleic acid ribbons to be Bform type\r"
,"\r"
,"-strand # : # of strands for skeined ribbon, local curvature offsets.\r"
,"-fakecalphas : fake Calphas, no Ca--Ca distance filter\r"
,"\r"
,"  overrides and extras issued after -ribbon or -bestribbon (or -arrows): \r"
,"-also : also do selected sidechains\r"
,"-Aform :do nucleics as A-RNA (rather than B-DNA)\r"
,"-nucbeta :use SHEET records for nucleic ranges with arrow heads\r"
,"-double :do nucleics with doubled interval using c4' \r"
,"-curved :do nucleics with curvature offset \r"
,"-c4prime :do nucleics with curvature offset toward c4' \r"
,"-blackedge :faced ribbon has unpickable black edge strand (bestribbon default)\r"
,"-noblackedge :faced ribbon has same colored edge.\r"
,"-fencepost :(temporary) if ribbons: do fencepost (==BDNA) orientation\r"
,"-noface :(temporary) if ribbons: do not fill in faces\r"
,"-noedge :(temporary) if ribbons: do not do a separate edge strand\r"
,"-cbstubs : ribbon spline stubs to CB for sidechains\r"
,"-cacbvec : CA---CB vector angles to ribbon direction, use with cbstubs \r"
,"   e.g.:  prekin -bestribbon -cbstubs -cacbvec 1bch.pdb > 1bch.cacbvec.kin \r"
,"-cispep     : cispeptides emphasized as extra part of a ribbon kinemage\r"
,"-nofudge : ribbons without fudged width and without curvature fudge offset\r"
,"-guide   : ribbon guide points displayed (also for newribbons)\r"
,"-skeins :  (newribbons) all ribbon-defining splines displayed 140520\r"
,"       -spline :meaning changed back 140520: single mid strand vectorlist\r"
,"       -skeins 140520 shows all ribbon-defining strands\r"
,"-spline :meaning changed back 140520: to single mid strand vectorlist\r"
,"-axis: (newribbons) ribbon axial spline \r"
,"-density : (newribbons) local density of ribbon axial splines \r"
,"-ca3rib : Lca3ribbon perp to ca-1,ca,ca+1 instead of C=O for ribbon plane\r"
,"\r"
,"   set of specialized calculations that use the ribbon code:\r"
,"-cispeponly : cispeptides emphasized as independent part of a kinemage\r"
,"-cispepdump : cispeptides list (just @text, NOT a real kinemage!)\r"
,"-vectorCO :perpendicular vector from Calpha--Calpha to Carbonyl Oxygen\r"
,"  cispeptides highlighted \r"
,"-labels   :label much angle info along with vectorCO vectors\r"
,"-azimuthCO :Calpha--Calpha angle, dihedral, azimuth to Carbonyl Oxygen\r"
,"-phipsiCO :Calpha--Calpha angle,dihedral,azimuthCarbonylOxygen & phi,psi\r"
,"-psiphiCO :Calpha--Calpha angle,dihedral,azimuthCarbonylOxygen & psi,phi\r"
,"-vectCOpdb :only Calpha,azimuth --> pdb format\r"
,"-pdbCACOpdb :pdb format Calpha,azimuth(66-71 f6.2) -->N,Ca,C,O,Cb pdb file\r"
,"\r"
,"   analysis of geometry around Calpha\r"
,"-calphataudev :deviation from ideal tau angle at Calpha\r"
,"-cbetadev :deviation and spread of ideal beta from actual beta\r"
,"-cbdevdump :deviation of ideal beta as : delineated list (NOT a kinemage!)\r"
,"-cbdevdumpmax :just maximum deviation from ideal beta (NOT a kinemage!)\r"
,"\r"
,"   analysis of nucleic acid distance between bases and phosphate\r"
,"-pperptobase :phosphate perpendiculars to base on molecule. (kin)\r"
,"-pperptoline :phosphate perpendiculars to c1'--n of base. (kin)\r"
," pperptobase and pperptoline are alternatives for vectors in a kinemage\r"
," pperpplot OR pperpdump change type of output to 3D plot or text-list\r"
,"-pperpplot :3D plot of phosphate perpendiculars to base vs delta. (kin)\r"
,"-pperpdump :colon delineated dump of phosphate perpendiculars to bases\r"
,"-pperpoutliers :outliers only, modifier of above pperp requests\r"
," SO use either pperptobase or pperptoline, then addition flags if desired.\r"
,"\r"
,"-connectdump :colon delineated dump of residue connections (NOT a kinemage)\r"
,"\r"
,"\r"
,"Otherwise to run in full command line mode with NO control dialog boxes\r"
,"must start with -quiet or -scope followed by options and selections\r"
,"\r"
,"GENERAL OPTIONS:\r"
,"-quiet :forces NO dialog boxes: e.g. prekin -quiet -show ... ...\r"
,"-stdoutreport : prekin -stdoutreport ...:only chains reported to stdout\r"
,"-report :writes report to stderr about an otherwise quiet command line run\r"
,"-mage :persistant GUI expects stdout|persistant pipe to mage\r"
,"-animate :adds \"animate\" to end of @group {} ...\r"
,"-nobutton :adds \"nobutton\" to end of @group {} ...\r"
,"-residuegroups: each residue in separate group, (multi-poses: ligand ATOMs)\r"
,"    less than 1024 hidden groups: -residuegroup -animate -nobutton \r"
,"-alt (-alt_) :selects atoms with NO alternate conformer flag\r"
,"-alta (-altx) :selects atoms with only alternate conformer flag 'a' ('x')\r"
,"-sclists :each sidechain in a separate, nobutton, list\r"
,"-scballs :individual balls on sidechain n,o,s atoms\r"
,"-scrings :triangle tesselation fills sidechain rings\r"
,"-bval :pointID B-values & occupancies\r"
,"-bcol :color by B-values:   PDB col 61...66\r"
,"-ocol :color by occ-values: PDB col 55...60\r"
,"-ucol :color by values:     PDB col 67...72\r"
,"-colorscale \"name,value,name,value,...,name\" : (<=10)color names & (<=9)dividing values\r"
,"   n+1 color names separated by n values  \r"
,"-colornc :ribbons colored spectrally from N to C (also does Nucleics 5' to 3')\r"
,"-color53 :ribbons colored spectrally from 5' to 3' (also does Proteins N to C)\r"
,"-colorabc :ribbons colored by HEADER alpha, beta, coil\r"
,"-colorbval :ribbons colored by Bvalue of Ca or C1\r"
,"-colorcurvature : ribbons colored by fractional extendedness\r"
,"-halfbonds :intermediate point, ca--.--ca, divides Calpha vectors in half\r"
,"-noaspects :aspects present in PDB file NOT outputted to kinemage\r"
,"  (aspects produced by default if present in PDB file)\r"
,"-aspectnc :aspect added for spectral colors from N to C\r"
,"-aspectabc :aspect added for coloring by HEADER alpha, beta, coil\r"
,"-atomradii :individual radii on all c,n,o,s,h atoms\r"
,"-polarHradius #.# :radius for polar H atoms, default==1.0\r"
,"-ballradius #.# :radius for all atom marker balls, default==0.2\r" /*041020*/
,"-segid :uses segid to differentiate chains, rather than chainid\r" /*130508*/
,"\r"
,"SELECTIONS:\r"
,"-focusresidue number :focuses on this numbered residue\r"
,"-focuschainID number :focuses on residue in this chain ('_' for blank)\r"
,"-focusradius number :focus radius for sc (was 8: mc,wa,ht set this+2, ca set +7)\r"
,"-chainnum number : output from that number chain as prekin counts them\r"
,"-chainid \"_,A,...\" : output chain with that PDB chain identifier ('_' for blank)\r"
,"-model \"_,1,...,17,...\" : output chain(s) with that PDB model identifier ('_' for blank)\r"
,"-excludesc \"res,res,...\" :every sidechain except those named here\r"
,"-sc \"res,res,...\" :(implies include) only sidechains named here \r"
,"   (excludesc and sc are incompatible)\r"
,"-scope :starts a new scope of one or more ranges with -show stuff\r"
,"   forces a quiet run (NO control-dialog boxes)\r"
,"-range \"nstart-nend,nstart-nend,...,...\" :sets of res# start-stop ranges\r"
,"-site  \"nstart-nend,nstart-nend,...,...\" :sets of res# start-stop ranges\r"
,"-siterec : SITE records provide sets of res# start-stop ranges\r"
,"\r"
,"-show  \"mc,sc,hy,hb,ca,lb,ht,wa\" \r" /*070512*/
,"                     :can specify global color e.g. \"mc(red),sc(blue)\" \r"
,"                     :can specify off e.g. \"mc(red)[off],sc(blue)\" \r"
," mc,       sc,       hy,       hb,      ca,     lb,     ht,      wa   ,\r"
," mainchain,sidechain,hydrogens,mcHbonds,Calphas,labels, hetatoms,water,\r"
,"-show  \"at,ct,mK,sK,hK,cb\" color by convention, come up ON \r" /*070512*/
," at                 ,ct              ,mK    ,sK    ,hK    ; cb \r"
," atom_markers(balls),balls_on_carbons,mcCPKs,scCPKs,htCPKs; Cb_only \r"
,"\r"
,"FOR A -scope SET -range \"#-#,#-#,...\" TO -show \"mc,sc,...\" FOR EACH RANGE\r"
,"E.G.: prekin -scope -range \"9-38,44-73,91-120,122-158,158-170\" -show \"mc,sc\" 2V2P.pdb >2V2P.kin\r"
,"\r"
,"-rotate number :sets up rotations for this numbered residue\r"
,"-mutate number aa : mutate nth res to  aa ... (&sets up rotations)\r"
,"-pdbout output of mutated residue in pdb format, nothing else.\r"
,"-emphasis: rotatable vectors emphasized as fat lines\r"
,"-noemphasis: rotatable vectors NOT emphasized as fat lines\r"
,"-superpos aa :superpositions all these aa by their ideal Cbeta\r"
," use -pdbout with -matrix, -center, and -translate (e.g. for biol unit PDB)\r"
,"-matrix : #.# #.# #.#  #.# #.# #.#  #.# #.# #.# (==3x3 matrix) \r"
,"        : m11 m12 m13  m21 m22 m23  m31 m32 m33 (idn 1 0 0  0 1 0  0 0 1) \r"
,"-center : #.# #.# #.# (cx cy cz) defaults to 0,0,0\r"
,"-translate : #.# #.# #.# (tx ty tz) defaults to 0,0,0\r"
," when matrix and center given: \r"
," subtract center  xo =  x - cx, yo = y - cy, zo = z - cz\r"
," apply matrix  xm =  xo*m11 + yo*m21 + zo*m31 \r"
,"               xm =  xo*m12 + yo*m22 + zo*m32 \r"
,"               xm =  xo*m13 + yo*m23 + zo*m33 \r"
," add back center  x =  xm + cx, y = ym + cy, z = zm + cz \r"
," when translate given:   x =  x + tx, y = y + ty, z = z + tz \r"
," when only center given: just add center to coords.\r"
,"-passkeyword \"...\" : keyword to pass to mage, e.g. @whitebackground \r"
,"-code \"...\" : codestr in every pointID, \" \":prekin invents PDB code... \r"
,"-name \"...\" : molecstr substitutes for molecule name of groups... \r"
,"\r"
,"NOGUI option: as of prekin.6.44.060324 can be compiled with target nogui\r"
,"   commandline control only, e.g. for MolProbity with no MOTIF libraries\r"
,"\r"
,"-html: writes html formated helpful stuff, then quits.\r"
,"-help: writes this help listing then quits.\r"
,"-version: writes version then quits.\r"
,"-changes: writes chronological list of code changes then quits.\r"
," try: prekin -help | more  OR: prekin -html > prekin.html  THEN use browser\r"
,"\r"
,"END\r"
}; /*prekincmdhelp[]*/

/*this next section seems unmaintainable to me now on 040924 !!!! ???? */
static char* prekinribbonhelp[] =
{"\r"
,"040425 too many options, many superceeded\r"
,"  current nucleic ribbons use midpoint of successive P's as guide pts\r"
,"  need:  -rnaribbon -c4prime\r"
,"  curvature from distance of nearest next P to midpoint of P---P interval\r"
,"  start offset at dist < 9A, scaled from dist 7 -- 9, \r"
,"  offset toward c4' atom as a fraction of distance between midpt and c4'\r"
,"  however allowing the guide point to go 1A beyond the c4' since the\r"
,"  B-splines on a curve never quite get out to their guide points anyway.\r"
,"\r"
,"\r"
,"-ribbon :variable width as a function of local curvature\r"
,"-arrows :arrows on beta, ribbons on alpha, by PDB SHEET,HELIX records\r"
,"-bestribbon :ribbon by PDB SHEET,HELIX if present, else variable width\r"
,"-rnaribbon :bestribbon with nucleic acids as Aform rather than Bform type\r"
,"-dnaribbon :bestribbon with nucleic acids as Bform rather than Aform type\r"
,"-Aform :if ribbons do nucleics as A-RNA rather than B-DNA\r"
,"-nucbeta :if ribbons use SHEET records for nucleic ranges with arrow heads\r"
,"-double :if ribbons do nucleics with doubled interval using c4'\r"
,"-curved :if ribbons do nucleics with curvature offset\r"
,"-c4prime :if ribbons do nucleics with curvature offset toward c4'\r"
,"-blackedge : faced ribbon has unpickable black edge strand \r"
," Ribbon Splines calculated for interval 2--3  of sequentual residues 1234 \r"
," Protein Ribbons are based on crossties in the peptide planes\r"
,"   A = CA[3] - CA[2], B = O[2] - CA[2], C = AxB, D = CxA \r"
,"   crosstie D parallel to peptide plane, perpendicular to A \r"
,"   guide points are displaced both ways from midline along D \r"
,"   fencepost uses C as guide point direction across ribbon\r"
,"\r"
," RNA Ribbons \r"
,"   A = P[3] - P[2], B = c1'[2] - c3'[2], C = AxB, D = CxA \r"
,"   crosstie D parallel to pseudo-plane p,c3',c1',p+1 perpendicular to A \r"
,"   guide points are displaced both ways from midline along D \r"
,"   BNA Ribbons use fencepost C direction for guide point direction \r"
,"\r"
,"-fencepost :(temporary) if ribbons: do fencepost (==BDNA) orientation\r"
,"-noface :(temporary) if ribbons: do not fill in faces\r"
,"-noedge :(temporary) if ribbons: do not do a separate edge strand\r"
,"\r"
,"END\r"
}; /*prekinribbonhelp[]*/

static char* prekinVRMLhelp[] =
{"\r"
,"annotations: 050212...  VRML output for 3D printer\r"
,"run prekin.6.40.050212 (or later) from the GUI\r"
,"Probably should name output kinemage file with .wrl extant\r"
,"(Prekin always thinks that it is writing kinemage files and will\r"
," report things that do not actually appear in the VRML output)\r"
,"Example directions to make a ribbon with disulfide crosslinks,\r"
,"a few sidechains, and arrowheads on beta sheet strands as\r"
,"specified by SHEET records in the pdb file header records.\r"
,"NOTE that usually defaults are set for what you want to do, \r"
,"in doubt, just click \"OK\".\r"
,"PREKIN_DLOG: PREKIN NEW PASS, select \"first: Reset Options\" click \"OK\"\r"
,"PREKIN_DLOG: Various Kludges, select \"VRML output, e.g. for 3D Printer\"  click \"OK\"\r"
,"PREKIN_DLOG: PREKIN NEW PASS, select \"New Ranges\" click \"OK\"\r"
,"PREKIN_DLOG: (Range Control)  type \"cys\" in place of \"...\", select \"sc: sidechain\", select \"at: atom markers or balls (not C or H)\", select \"Ribbon: by curvature over entire range\", click \"OK\".\r"
,"Since option \"OK accepts and comes back for more\" is the default, then again get:\r"
,"PREKIN_DLOG: (Range Control)  type \"his\" in place of \"...\", select \"sc: sidechain\", select \"at: atom markers or balls (not C or H)\", select \"Ribbon: by curvature over entire range\", select \"OK accepts and end ranges\", click \"OK\"\r"
,"(Of course, here one would probably want to do one-by-one just the particularly important histidine sidechains to illustrate the active site.)\r"
,"( in that case, set \"Number of start residue\" = \"Number of end residue\" = residue number of interest as well as that residue 3-letter code.)\r"
,"PREKIN_DLOG: Focus Point Options, select \"No focus at all\", click \"OK\"\r"
,"PREKIN_DLOG: Atom Marker choices:, \"Current radius of ball\" is probably set to something reasonable, and \"atom mark = ball, of radius given above\" should already be selected, click \"OK\".\r"
,"PREKIN_DLOG: Ribbon Control Values, defaults should be set for VRML, particularly \"number of strands\" 5 makes a good solid ribbon, and must use \"Ribbon of long skeins\", click \"OK\".\r"
,"PREKIN_DLOG: Subunit, or molecule, choice:, usually will do only one subunit, but in a file of a monomer that is all subunits of input file, click \"OK\".\r"
,"PREKIN: general report window... now for this example we want arrowheads so need another pass:\r"
,"   select from top menu bar  File/New Pass\r"
,"PREKIN_DLOG: PREKIN NEW PASS, select \"Selection of built-in Scripts\", click \"OK\".\r"
,"PREKIN_DLOG: kinemage built-in scripts, select \"bestribbon: HELIX & SHEET from pdb header,...\", click \"OK\".\r"
,"PREKIN_DLOG: Ribbon Control Values, here we are subverting how prekin does its fancy ribbons and will only get the few vectors of the arrrowheads written out in VRML format,\r"
," the settings should be now set so (near the bottom) should see selected \"Edged, flat faced ribbon (instead of skeins)\", click \"OK\".\r"
,"PREKIN_DLOG: Subunit, or molecule, choice:, select whatever you did last time, then click \"OK\".\r"
,"PREKIN: general report window...  now you have the complete VRML output, so \r"
,"select from top menu bar  File/Quit\r"
,"Find your VRML file and read it into a VRML viewer, or directly into the 3D Printer...\r"

,"\r"
,"\r"
,"\r"
,"END\r"
}; /*prekinVRMLhelp[]*/

/****mainlooprouter()*********************************************************/
void mainlooprouter() /*PKININIT*/ /*common routing in mainloop*/
{
/*
fprintf(stderr,"Prekin enters mainlooprouter() with Lnewfile== %d, Lstartfile== %d, Lnewpass== %d, Lthispass== %d, mrange== %d\n",Lnewfile,Lstartfile,Lnewpass,Lthispass,mrange);
*/
   if(Lhelpoutopen)
   {/*must re-call writehelptohtmlfile() when opened with non-modal dialogs*/
      writehelptohtmlfile(1); /*called with 0 from menu, 1 revisits 040425*/
   }
   if(Lnewfile)
   {
       selectfiles(); /*PKINFILE.C, if successful, sets Lnewpass=1*/
       if(!Lnewpass)
       {/*failed somewhere in the file name and open business*/
           Lgot_infilename = 0; /*ready for new set of files*/
           Lgot_outfilename = 0;
           Lgot_infileopen = 0; /*ready for new set of files*/
           Lgot_outfileopen = 0;
       }
       if(Lquiet)
       {
          if(Lerr) return;
          Lnewpass=0;
          Lgetranges=1;
          if(Lin_stdin) Lthispass = 2;
       }
   }
   if(Lnewpass)
   {
/*
fprintf(stderr,"Prekin gets Lnewpass with Lcommanded: %d, Lbuiltin: %d, Lthispass: %d\n",Lcommanded,Lbuiltin,Lthispass);
*/
     if(Lthispass==1)
     {
        Lnewpass = 0;
        Lnewpassflow = 1;
     }
     else
     {
        Lnewpass = 0;
        Lnewpassflow = 0;/*later used to continue with a newpass*/
        Lrangestart = 1; /*to call rangestartinitialize() from getranges()*/
        Lgetranges = 1;  /*to call getranges()*/
     }
   }
   if(Lgetranges && !Lwaiting)
   {
/*
fprintf(stderr,"Prekin calls getranges with Lcommanded: %d, Lbuiltin: %d\n",Lcommanded,Lbuiltin);
*/
       getranges(); /*PKINRNGE.c*/ /*971029*/
   }
   if(Lfinishranges)
   {
       Lfinishranges = 0;/*971029 reset this here*/
       finishranges(); /*PKINRNGE.c*/ /*971029*/
   }
   if(Lnewpassflow && fpinput!=NULL)  /*971029 new flag to unload Lnewpass */
   {
       /*Lnewpassflow = 0;*/ /*990506 reset this in flowControl() after HEADER */
#ifdef PCWINDOWS
       makecursorwait(); /*990407b Future Feature for Mac, UNIX*/
#endif
if(Ltestprint) printf("mainlooprouter about to call flowControl()WWWWWWWWWWWWWWWWWWWWWWWWW\n");
/*
fprintf(stderr,"    Lribonucleic== %d\n", Lribonucleic);

fprintf(stderr,"    Lfudgedwidth== %d\n", Lfudgedwidth);

fprintf(stderr,"    Lonestrandcoil== %d\n", Lonestrandcoil);

fprintf(stderr,"    Lbetaarrows== %d\n", Lbetaarrows);

fprintf(stderr,"    Lsmoothtwist== %d\n", Lsmoothtwist);

fprintf(stderr,"    LcolorbyNtoC== %d\n", LcolorbyNtoC);

fprintf(stderr,"    LNtermBetaFudge== %d\n", LNtermBetaFudge);

fprintf(stderr,"    LNOfudge== %d\n", LNOfudge);

fprintf(stderr,"    Lribbonguidepts== %d\n", Lribbonguidepts);

fprintf(stderr,"    Lnucleicarrows== %d\n", Lnucleicarrows);

fprintf(stderr,"    Lcrosstie== %d\n", Lcrosstie);

fprintf(stderr,"    Ledgedribbon== %d\n", Ledgedribbon);

fprintf(stderr,"    Lskeinedribbon== %d\n", Lskeinedribbon);
*/


       flowControl(); /*PKINCRTL.c*/
#ifdef PCWINDOWS
       makecursoractive(); /*990407b Future Feature for Mac, UNIX*/
#endif
   }
}
/*___mainlooprouter()________________________________________________________*/

/****initialvalues()*********************************************************/
void initialvalues(void)
{
  int j;

  IDEMO = 0;
           /* IDEMO == 1 for big type, terse comments, set in kludges dialog*/
           /* IDEMO == 0 for regular use prekin, keep this 0 here!*/
  textHeight = 1;
  widMax = 1;
  maxValue=10000;
  minValue = 0;
  Lnewcoord=0;
  Lnewfile=0;
  Lgetfile=0;
  Lcontrols=0;
  Linein=0;
  Loption=0;
  Lallaminoacids = 0;
  Lthispass = 1; /*initial pass, scan for subunit pattern */
  UseSEGIDasChain = 0; /*initialization 130507 JJH*/
  Nchainfirstpass = 0;
  for(j=0; j<256;j++)
     typechainfirstpass[j] =  ' '; /*char str*/  /*991210 Protein, Nucleic,...*/
     kindchainfirstpass[j] =  ' '; /*char str*/ /*060114: space char, Dna, Rna*/
     codechainfirstpass[j][0] = ' '; /*char str*/  /*010426, 2char 070926*/
     codechainfirstpass[j][1] = ' '; /*char str*/  /*010426, 2char 070926*/
     codechainfirstpass[j][2] = '\0'; /*char str*/  /*010426, 2char 070926*/
     /*segidfirstpass[j][0] = ' ';
     segidfirstpass[j][1] = ' ';
     segidfirstpass[j][2] = ' ';
     segidfirstpass[j][3] = ' ';
     segidfirstpass[j][4] = '\0';*/
  Lnewpass = 0;  /*set this when get valid input/output files */
  Lpassposs = 0;
  npass = 1;
  Linclusive = 1; /*usually include given ranges*/
  newkinemage = 1;
  Lallsubunits = 1; /*990403*/
  Lfirstsubunit = 0; /*990403*/
  Lneedsubunits = 0;
  /* fovea, focus distances */
       focus[1] = 0.0;
       focus[2] = 0.0;
       focus[3] = 0.0;
         focusca = focuscadefault = 15.0;
         focusmc = focusmcdefault = 10.0;
         focussc = focusscdefault =  8.0;
         focuswa = focuswadefault = 10.0;
         focusht = focushtdefault = 10.0;
  focuschaincharcode[0] = ' '; /*011109  2char 070926*/
  focuschaincharcode[1] = ' '; /*011109  2char 070926*/
  focuschaincharcode[2] = '\0'; /*011109*/
  nLines = 0;
  Lleftadjatomname = 0;
  Lhetconnect = 1; /*990403*/
  Lribnhet = 0; /*051214 distinguish this call to make ribbons, re RCSB PDB*/
  Lexplanation = 0;
  Lmagekinjas = 0; /*101225*/

  /*ribbon and reduced-representation default parameters*/
  Lribbon = 0;
  Lribbonalso = 0; /*050206*/
  Lbestribbon = 0; /*011030*/
  Lribonucleic = 0; /*040226*/
  LdoRNAribbon = 0; /*060114 force RNA A form ribbon*/
  LdoDNAribbon = 0; /*060114 force DNA B form ribbon*/
  Ldoublenucleics = 0; /*040419*/
  Lcurvednucleics = 0; /*040420*/
  Lc4primenucleics = 0; /*040422*/
  Lnucleicarrows = 0; /*040228*/
  Larrowhead = 0; /*051210*/ /*for newribbon, skeined with arrows*/
  Lbyresidue = 0; /*051210*/ /*for newribbon, skeined with arrows*/
  Lcbstubsrib = 0; /*041205 ribbon---CB stubs to attach sidechain to spline*/
  Lcacbvector = 0; /*050403*/
  for(j=0;j<5;j++){Lcbrib[j]=0;} /*041205*/
  LNOfudge = 0; /*050327*/
  Lribbonguidepts = 0; /*050327*/
  Lribbonrawsplines = 0; /*051208*/
  Lribbonaxisspline = 0; /*060115*/
  Lsplinevector = 0; /*140520*/
  Lribbondensity = 0; /*060115*/
  LvectorCO = 0; /*030125*/
  LvectorCOlabels = 0; /*041023*/
  Lcispeptides = 0; /*041102*/
  Ldumpcispeptides = 0; /*041023*/
  LazimuthCO = 0; /*030128*/
  LphipsiCO = 0; /*030128*/
  LpsiphiCO = 0; /*030128*/
  LvectCOpdb = 0; /*030129*/
  LpdbCACOpdb = 0; /*030202*/
  LfakeCaCaspline = 0; /*111106*/
  fpoutput = 0;

  /*more ribbon default parameters*/
  nstrnd = 2;
  onestrandcoilwidth = 4;  /*integer flag for point width*/
  ribwidcoil = 1.0;
  ribwidalpha = 2.0;
  ribwidbeta = 2.2;
  ribwidnucleic = 3.0;
  ribwidVRML = 2.0; /*050204*/
  Lfudgedwidth = 1;
  LNtermBetaFudge = 1; /*030706*/
  Lcrosstie = 0;
  Ledgedribbon = 1;
  Lskeinedribbon = 0;
  Lbetaarrows = 0;
  Lonestrandcoil = 0;
  Lsmoothtwist = 1;
  Lcolorbycurvature = 0; /*010530x*/
  LcolorbyNtoC = 0; /*010530x*/
  LcolorbyABC = 0;
  LcolorbyBval = 0; /*021213*/
  ichord = 4; /* number of vec/res */
  /*#define CORDER 4 */ /*order of the B-spline defined in PKINRIBN.C*/
  Lpeptideperprib = 0; /*040319*/
  Lnoface = 0; /*040319*/
  Lnoedge = 0; /*040319*/
  Lblackedge = 0; /*040924*/

  /*atom marker controls*/
  Lballs = 1;
  ballradius = (float).2;
  Ldotsonly = 0;
  Lcrosses = 0;
  Latomradii = 0; /*for cavity definition 030330*/
  polarHradius = 1.0; /*for cavity definition 030331*/
  Lexplanation = 0;
  Llaunchkinemage = 0;
  Llaunchcurrentkin = 0;
  Llaunchdiffkin = 0;
  Lskip = 0;
  Lmadekin = 0;
  Lbrowsedefault = 0;
  Lindicator = 0;
  Lquiet = 0;
  Lstdoutreport = 0; /*030502*/
  Lpperptobase = 0; /*030515*/
  Lpperptoc1nline = 0; /*050125 pperp to c1'--nbase line instead of base plane*/
  Lplotpperptobase = 0; /*030519*/
  Ldumppperptobase = 0; /*040202*/
  Lpperpoutliersonly = 0; /*040705*/
  Lkinmasters = 0; /*030519*/
  Lhylens = 0; /* lens on hydrogen lists*/
  Listext = 0; /* extra output at end of pass, controled  from kludges*/
  MolNameStr[0] = '\0';
  ShortNameStr[0] = '\0';
  Lwaiting = 0; /* used in PKININIT/mainlooprouter()*/
  Ltable = 0; /*signel for table group 991122 */

  Lcanlaunchmage = 1; /*move out from PCWINDOWS for everybody  010530*/
    /*MAC also still has its own place to set this*/
#ifdef PCWINDOWS  /*Mac deals with these in its own way*/
  /*folowing values moved from PCinitialvalues(obsolete) */
  Lappleevents = 0;
  Lstartfile = 0; /*presume prekin not launched by a file*/
  Lin_stdin = 0;
  Lout_stdout = 0;
  Lgot_infilename = 0; /*ready for new set of files*/
  Lgot_outfilename = 0;
  Lgot_infileopen = 0; /*ready for new set of files*/
  Lgot_outfileopen = 0;
#endif
  Lcolorscale = 0; /*flag for DoColorScaleDialog, set in Kludges Dialog*/
  Ncolorscale = 10; /*number of colors of the colorscale */
  Ncolorscalebasis = 1;/*1:Bvalue 61...66, 2:occ 55...60, 3:Unusual 67...72*/
  /*Scale by f-stop: 1,1.4,2,2.8,4,5.6,8,11,16,22,32,44,64 */
  /*but we do B cut offs at 30 and 40, so:*/
  colorscale[1].origvalue  =   4.0;
  colorscale[2].origvalue  =   8.0;
  colorscale[3].origvalue  =  11.0;
  colorscale[4].origvalue  =  16.0;
  colorscale[5].origvalue  =  22.0;
  colorscale[6].origvalue  =  30.0;
  colorscale[7].origvalue  =  40.0;
  colorscale[8].origvalue  =  60.0;
  colorscale[9].origvalue  = 100.0;
  colorscale[10].origvalue = 999.0;
  for(j=1; j<=10; j++) colorscale[j].value = colorscale[j].origvalue;
  strcpy(colorscale[1].origcolor,"blue");
  strcpy(colorscale[2].origcolor,"purple");
  strcpy(colorscale[3].origcolor,"magenta");
  strcpy(colorscale[4].origcolor,"hotpink");
  strcpy(colorscale[5].origcolor,"red");
  strcpy(colorscale[6].origcolor,"orange");
  strcpy(colorscale[7].origcolor,"gold");
  strcpy(colorscale[8].origcolor,"yellow");
  strcpy(colorscale[9].origcolor,"yellowtint");
  strcpy(colorscale[10].origcolor,"white");
  for(j=1; j<=10; j++) strcpy(colorscale[j].color,colorscale[j].origcolor);

  Leachsclist = 0;  /*each sidechain in its own list, set in Kludges Dialog*/
  Lballonscpoints=0; /*individual balls on sidechain points, Kludges Dialog*/
  Lfillscrings = 0; /*triangle tesselation fills sidechain rings*/
  Lcbonly = 0; /*ca--cb only part of sidechain outputted*/
  Lhalfbonds = 0; /*intermediate point divides vector into halves*/
  Lnotext = 0; /*still issues @kinemage, but like Lappend does NOT do text*/
  Lallbuttons = 0; /*overrides and cancels Lonlygroups,Lonegroup,Lnogroups */
  Lonlygroups = 1; /*uses dominant groups with masters for subgroups, etc.*/
  Lonegroup = 0; /*one dominant group with masters for chains, subgroups,...*/
  Lnogroups = 0; /*NO groups, but masters for chains, subgroups, etc.*/
  Lnosubgroups = 0; /*NO subgroups 051129*/
  Ngroups = 0; /*counter for Number of groups per pass or MODEL */
  Lhypertextstuff = 0; /*hypertext commands, e.g. mutant rotamer angles */

/*------output color overrides, one for each type of output */
colormcvec[0] = '\0';
colorscvec[0] = '\0';
colorcavec[0] = '\0';
colorhyvec[0] = '\0';
colorhtvec[0] = '\0';
colorhbvec[0] = '\0';
colorwavec[0] = '\0';
colorlbvec[0] = '\0';
colorextra[0] = '\0'; /*041106*/
/*------output ON/OFF overrides, one for each type of output */
offmc[0] = '\0';
offsc[0] = '\0';
offca[0] = '\0';
offhy[0] = '\0';
offht[0] = '\0';
offhb[0] = '\0';
offwa[0] = '\0';
offlb[0] = '\0';

Naspectcode = 0; /*counter for color coded aspects for individual atoms*/
Lallowaspectsin = 1; /*flag for using aspects if present in PDB file*/
Lnosubgroupmasters = 1; /*default 011028*/
Lspeciallistmaster = 0; /*special list master  040902*/
speciallistmaster[0] = '\0';  /*special list master  040902*/
Lspeciallistname = 0; /*special list name  051129*/
speciallistname[0] = '\0';  /*special list name 051129*/

ntypes =0;/*number recognized scratch-flags of individual output lists 011202*/
LIOmage = 0; /*020514*/
LIOpipe = 0; /*020514*/

LdumpVRML = 0;   /*050204*/
LsplineVRML = 0; /*050204*/
LheaderVRML = 0; /*050204*/
LvectorVRML = 0; /*050208*/
LsphereVRML = 0; /*050209*/
LarrowVRML  = 0; /*050212*/
    /*max value of 32 bit float is about 10^38 with 7 significant digits*/
    fxmin =  (float)1E37; /* scaling limits initialized wimperwhump*/
    fxmax = (float)-1E37;
    fymin =  (float)1E37;
    fymax = (float)-1E37;
    fzmin =  (float)1E37;
    fzmax = (float)-1E37;
     /*as of 050204 these min-max values only used for VRML sceen centering*/

ncon = 0; /*firstpass actual number of CONECT atom pairs */ /*050409*/
Lconect = 0; /*defaults to NOT try to use CONECT records 050409, temporary...*/
Llistcolordom = 0; /*050705 ribnhet: to distinguish multiple subunits by color*/
Lsitefake = 0; /*050928 resideues exclude other range controls crt cbstubs*/
Lsite = 0; /*050731 -site == -range excludes other range controls crt cbstubs*/
Nsite = 0; /*050731 number of site residues*/
Lribs = 0; /*050928 */
Nribs = 0; /*050928 number of ribbon secondary structure ranges*/

Lmodel = 0; /*050922 existance of MODEL record*/
Nthmodel = 0; /*050922 sequential MODEL encountered*/
FirstModel[0] = '\0'; /*050923*/

thisribmodelptr = NULL;  /*051025 for Lribbonnew  new ribbon code PKINRIBB*/
thisribsectionptr = NULL;
thisribfragptr = NULL;
thisribresptr = NULL;
Lribbonnew = 1;  /*051102, 060929 = 1 to use the new ribbon code as default*/
Lribbonhbonds = 0;  /*051212 pseudo-hbonds approx where PKINHBND puts them*/
Lcodestr = 0;    /*051102  add text codestr to all pointIDs*/
Lmolecstr = 0;   /*051114 molecstr substitutes for MolNameStr */
Lribbonmasters = 0; /*051128 alpha,beta,coil optional -ribbonmasters */
Lanimate = 0;
Lnobutton = 0; /*060120*/
Lresiduegroups = 0; /*060120*/
Lconnectresiduesdump = 0; /*061202*/
Loldpdb = 0; /*071215 default is new pdb format v3.1 */
Lca3ribbon = 0; /*130121 use ca-1,ca,ca+1 perp instead of C=O for ribbon plane*/

  rangestartinitialize(); /*PKINRNGE*/

}
/*___initialvalues()________________________________________________________*/

/****aboutprekin()***********************************************************/
void aboutprekin()
{
/*c*/  sprintf(alertstr,
             "PREKIN: copyright \251 2005 by David C. Richardson,"
             " Little River Institute"
         CRLF"       dcr@kinemage.biochem.duke.edu"
         CRLF"Biochemistry Dept., Duke University, NC 27710, USA"
         CRLF"%s"
             ,version);
}
/*___aboutprekin()__________________________________________________________*/

/****doexplanation()*********************************************************/
void    doexplanation()
{
   Lexplanation = 0;
   sprintf(temps,"program explanations..."CRLF);   /*c*/
   pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
   adjusttext(0);          /*PKMCTEXT.C PKPCTEXT.C, 0 really resets text*/

   sprintf(temps,"first open a PDB format file"CRLF
       "then choose an output .kin file name."CRLF
       "A series of DIALOG boxes allows selection of parameters"CRLF
       "that will build a kinemage display of the molecule"CRLF
   );
   pkintextinsert(temps);
   adjusttext(4);          /*match CRLFs, quick reset of text*/

 sprintf(temps,"Built-in scripts are provided to automatically make"CRLF
       "a selection of possible starting kinemages."CRLF
       "New Ranges allows very specific control of what will appear in"CRLF
       "the kinemage."CRLF
       "Focus lets sidechains, mainchain, ca-ca connections, and waters"CRLF
       "to be selected within user-set distances of a given coordinate"CRLF
   );
   pkintextinsert(temps);
   adjusttext(8);          /*match CRLFs, quick reset of text*/


   sprintf(temps,CRLF"After reading this, "
       CRLF" select   Open  from the File menu "CRLF);
   pkintextinsert(temps);  /*PKMCTEXT.C PKPCTEXT.C*/
   adjusttext(0);          /*PKMCTEXT.C PKPCTEXT.C*/

}
/*___doexplanation()________________________________________________________*/

/****dissectcommandline()*****************************************************/
void dissectcommandline(char cmdline[256])
{
    int             i,argc,whitespace,inquotes,j,k;
    char  tempstr[256];
    char*  argv[33]; /* 990331 - used for commandline parsing */

    /*dissect the cmdline*/
    argc = 2; /*C commandline 1st argument is at 0 for program name */
    j = 0; /*character counter within an argument starts at 0 */
    whitespace = 0; /*state indicator*/
    inquotes = 0; /*quoted blocks are taken as is*/
    for(i=0; i<256; i++)
    {/*scan in whole commandline*/
       if(inquotes==0 && (cmdline[i]==' '||cmdline[i]==','||cmdline[i]=='\0'))
       {
           if(i==0 || whitespace == 1) /*successive whitespace, NOP*/
           {
               whitespace = 1; /*set or reset*/
           }
           else
           {
               whitespace = 1; /*ends a non-whitespace field*/
               tempstr[j] = '\0'; /*end this argument string*/
               argv[argc-1] = (char *)malloc(sizeof(char)*(j+1));
               for(k=0; k<=j; k++)
               {
                   argv[argc-1][k] = tempstr[k];
               }
               j = 0; /*reset argument character counter*/
               argc++; /*set for next argument, later reset -- for last one*/
           }
       }
       else if(cmdline[i]=='"')
       {
           if(inquotes==0)
           {
               inquotes = 1;
           }
           else
           {
               inquotes = 0; /*next whitespace will close this arg*/
           }
       }
       else
       {
           whitespace = 0;
           tempstr[j] = cmdline[i]; /*put char into argument string*/
           j++;
       }
       if(cmdline[i] == '\0') /*reached end of cmdline*/
       {
           break;
       }
    }/*scan in whole commandline*/
    argc--; /*was set for another argument, reset for the last one*/

/*#define COMMANDLINEREPORT*/
#ifdef COMMANDLINEREPORT
    alertstr[0] = '\0'; /*Null initial alertstr*/
    for(i=0; i<argc; i++)
    {
       sprintf(alertstr,"%s %s",alertstr,argv[i]);
       DoReportDialog();
    }
#endif /*COMMANDLINEREPORT*/

    if(argc>1)
    {
        parsecommandline(&argc, argv);
        /*parsecommandline will set Lnewfile=1 if it finds an input file name*/
        /*specific main loops now required to handle routing inc. Lcommanded*/
        /*if(Lcommanded != 0)*/
        /*{*/
        /*   Lnewpass = 0;*/ /*no need for user interactive choices*/
        /*   Lnewpassflow = 1;*/ /*enter program flow to make kinemage*/
        /*}*/
    }
    if(argc>=1)
    {/*free fake argv allocated char strings*/
       for(i=1; i<argc; i++)
       {/*loop over arguments*/
          /*free(argv[argc-1]);*/
            free(argv[i]);
       }/*loop over arguments*/
    }/*free fake argv allocated char strings*/
}
/*___dissectcommandline()____________________________________________________*/

/****parsecommandline()*******************************************************/
void parsecommandline(int *argc, char** argv)
{
   /*NOTE n is a global that is used in this routine!*/
   char *p;
   char valuestr[32];
   int   h,i,j,k,nbegin,ncount,resnumrotate,resnummutate,ncnt,iok;
   /*int   Lreport = 0;*/ /*made global 130324*/
   int   nnumber = 0,sign = +1,deliniated = 0;
   int   chainnumber=0;
   int   ln=0; /*strlen for ranges of aa   050309*/

   Lnogroups = 0; /*flag for NO @group... keyword line */
   Lappend = 0; /*flag for write on end of existing file*/
   Lquiet = 1; /*flag for NO GUI, do commandline quietly, then quit*/
   /*presume can run silently, reset if commandline not complete*/
   Lcommanded = 0; /*flag for commandline specified internal script*/
   Lcommandsub = 0; /*flag for commandline specified subunit choice*/
                    /*if zero, will go by whatever the script suggests*/
   Laltcontrol = 0;
   rangestartinitialize();/*sets defaults so commandline can set ranges*/
       /* including resnu1[n] = -999; resnu2[n] = 9999;*/
   n=0; /*global for current range up for input*/
   nbegin = 1; /*local for beginning range of a scope*/

   mrange = 0; /*mrange can be set by a command performance==*/
     /*builtin scripts request, so n of a scope has to check this*/
   Lfirstsubunit = 0;
   Lallsubunits = 1;
   Nchaincharcode = 0; /*number of commandline pdb coded chain identifiers*/
   for(j=0; j<MAXchaincharcode; j++)
   {/*clear array */
      chaincharcode[j][0] = '\0';/*2char 070926*/
      chaincharcode[j][1] = '\0';/*2char 070926*/
      chaincharcode[j][2] = '\0';/*010425*/
   }
   Nmodelcharcode = 0; /*number of commandline pdb coded model identifiers*/
   for(j=0; j<(MAXmodelcharcode); j++)
   {/*clear array */
      for(h=0;h<3;h++)
      {
         modelcharcode[j][h] = '\0';/*010425*/
      }
   }
   /*control logicals that may be reset by commandline*/
   Lpdbout = 0;
   Lpdbmutout = 0;
   Lmatrix = 0;
   matrix[0] = 0; /*not used for 9 element matrix*/
   matrix[1] = 1;
   matrix[2] = 0;
   matrix[3] = 0;
   matrix[4] = 0;
   matrix[5] = 1;
   matrix[6] = 0;
   matrix[7] = 0;
   matrix[8] = 0;
   matrix[9] = 1;
   Lrotcenter = 0;
   Ltranslate = 0; /*050324*/
   for(h=0; h<=3; h++){rotcenter[h] = 0; translate[h] = 0;} /*050324*/
   Lmutate = 0;
   LpdbIO = 0;
   Ldid_outfileheader = 0; /*dumps have their own header of fields 040318*/
   Lsingleout = 0; /*some dumps only output once after all chains  040318*/

   for(i=1; i<*argc; i++)
   {/*loop over arguments*/
      p = argv[i];
      if(p[0] == '-')
      {/*flag*/
         /*distinguish naked flag and flagged character string*/

         if(p[1] == '\0')
         {/*naked flag: stdio request*/
            if(!Lgot_infilename)
            {/*first naked flag, interpret as input on stdin*/
               Lin_stdin = 1;
               Lgot_infilename = 1;
               Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
               Lstartfile=1; /*pretend stdin launched prekin*/
            }
            else if(Lgot_infilename && !Lgot_outfilename)
            {/*already have an input file designated, but not yet an outfile*/
               /*so presume output to stdout*/
               Lout_stdout = 1;
               Lgot_outfilename = 1;
               Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
            }
            else
            {/*naked flag with both in & out already specified: ignore!*/
               ;
            }
         }/*naked flag: stdio request*/
         else if(CompArgStr(p+1,"in", 1))
         {
            i = i+1; /*input file name is next character string*/
            strcpy(NameStr,argv[i]);
                /*file name copied into prekin input file Name*/
            Lstartfile=1; /*file launched prekin, use it*/
            Lgot_infilename = 1;
            Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
         }
         else if(CompArgStr(p+1,"out", 1))
         {
            i = i+1; /*output file name is next character string*/

            strcpy(OutfileStr,argv[i]);
                /*file name copied into prekin input file Name*/
            Lgot_outfilename = 1;
            Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
         }
         else if(CompArgStr(p+1,"oldpdb", 6)) /*071215*/
         {
            Loldpdb = 1; /*071215*/
         }
         else if(CompArgStr(p+1,"append", 6))
         {
            Lappend = 1;
         }
         else if(CompArgStr(p+1,"notext", 6))
         {
            Lnotext = 1;
         }
         else if(CompArgStr(p+1,"onlygroups", 9))
         {/*group for each chain, with masters for subgroups*/
            Lonegroup = 1;
            Lnosubgroupmasters = 0;
         }
         else if(CompArgStr(p+1,"onegroup", 8))
         {/*each model: one group with masters for chains, subgroups*/
            Lonegroup = 1;
            Lnosubgroupmasters = 0;
         }
         else if(CompArgStr(p+1,"nogroup", 7))
         {/* -nogroup */
            Lnogroups = 1;
         }
         else if(CompArgStr(p+1,"nosubgroup", 10)) /*051128*/
         {/* -nosubgroup */
            Lnosubgroups = 1;
         }
         else if(CompArgStr(p+1,"allbuttons", 10))
         {
            Lallbuttons = 1;
         }
         else if(CompArgStr(p+1,"nosubgroupmasters", 17))
         {
            Lnosubgroupmasters = 1;
         }
         else if(CompArgStr(p+1,"subgroupmasters", 15))
         {
            Lnosubgroupmasters = 0;
         }
         else if(CompArgStr(p+1,"listmaster", 10))
         {/*-listmaster '...name...' */
            /*  :14 char max: master={name} for all list masters*/
            Lspeciallistmaster = 1;
            i = i+1; /*special list master name is the next character string*/
            p = argv[i];
            for(j=0; j<MAXNAMECHAR; j++)
            {
               if(p[j] == '\0') break;
               speciallistmaster[j] = p[j];
            }
            speciallistmaster[j] = '\0';
         }
         else if(CompArgStr(p+1,"listname", 8))  /*051129*/
         {/*-listname '...name...' :13 char max: list {name} for all lists*/
            /*operates in PKINCOUT/transferout() so overwrites any aname[] */
            Lspeciallistname = 1;
            i = i+1; /*special list name is the next character string*/
            p = argv[i];
            for(j=0; j<MAXNAMECHAR; j++)
            {
               if(p[j] == '\0') break;
               speciallistname[j] = p[j];
            }
            speciallistname[j] = '\0';
         }
         else if(CompArgStr(p+1,"nohetconnect", 8)) /*041011*/
         {
            Lhetconnect = 0;
         }

         else if(CompArgStr(p+1,"report", 6))
         {
            Lreport = 1;
         }
         else if(CompArgStr(p+1,"passkeyword", 11)) /*041107*/
         {/*passkeyword to mage,KiNG*/
            i = i+1; /* characters are in next character string*/
            p = argv[i];
            if(n==0) n=1; /*ensure that this is a viable range*/
            k = 0; /*leading blank(s) filter*/
            j = 0;
            while(p[j]!='\0')
            {/*strobe out characters*/
              if(p[j]=='\0' || j >= MAXKEYWORD) break;/*reached end of string*/
              else if(p[j]==' ' && k==0)
              {/*preceeding white space*/
                 j++; /*look at next character*/
              }
              else
              {/*character encountered */
                 passkeywordstr[k] = p[j];
                 j++;
                 k++; /*past any leading blanks*/
              }
            }/*strobe out characters*/
            if(k>0) {Lpasskeyword = 1;}
         }/*passkeyword to mage,KiNG*/


         else if(CompArgStr(p+1,"version", 7))
         {/*-version: write version then quit*/
            if(Lgot_outfilename)
            {
               prepareoutputfile();
               fphelp = fpoutput;
            }
            else
            {
               fphelp = stdout;
            }
            writeversiontooutput();
            exit(0);
         }
         else if(CompArgStr(p+1,"changes", 6))
         {/*-changes: write changes info then quit*/
            if(Lgot_outfilename)
            {
               prepareoutputfile();
               fphelp = fpoutput;
            }
            else
            {
               fphelp = stdout;
            }
            writechangestooutput();
            exit(0);
         }
         else if(CompArgStr(p+1,"html", 4))/*must precede help in else-if list*/
         {/*-html: write help info to HTML file, then quit*/
            if(Lgot_outfilename)
            {
               prepareoutputfile();
               fphelpout = fpoutput;
            }
            else
            {
               fphelpout = stdout;
            }
            Lhelpoutopen = 1;/*safe: writehelptohtmlfile also checks fphelpout*/
            writehelptohtmlfile(1);
            exit(0);
         }
         else if(CompArgStr(p+1,"help", 1)) /*must come after anything h...*/
         {/*-help: write help info then quit*/
            if(Lgot_outfilename)
            {
               prepareoutputfile();
               fphelp = fpoutput;
            }
            else
            {
               fphelp = stdout;
            }
            writecmdhelptooutput();
            exit(0);
         }

         else if(CompArgStr(p+1,"browse", 6))
         {/*cass: Calphas &-ss- (or nucleic acid pseudo-backbone) non-H2O hets*/
            Lcommanded = 1;
            Lbuiltin = 1;
            Lbrowsedefault = 1; /*automatic MAGE launch 961117*/
         }
         else if(CompArgStr(p+1,"cass", 4))
         {/*cass: Calphas &-ss- (or nucleic acid pseudo-backbone) non-H2O hets*/
            Lcommanded = 1;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"mchb", 4))
         {/*mchb: mainchain and its h-bonds*/
            Lcommanded = 2;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"aasc", 4))
         {/*aasc: individual amino acids in separate sets and Calphas*/
            Lcommanded = 3;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"lots", 4))
         {/*-lots: mc, sc, ca, (hy), (hets), H2O balls*/
            Lcommanded = 4;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"naba", 4))
         {/*naba: individual nucleic acid bases in separate sets and lots else*/
            Lcommanded = 6;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"magekinjas", 10)) /*101225*/
         {/*modified format for feeding current magekinjas parser*/
            Lmagekinjas = 1;
         }
         else if(CompArgStr(p+1,"ribnhet", 7)) /*041031*/
         {/*-ribnhet -arrows -colornc -blackedge -nohetconnect -ballradius 0.32 -show "ht,at"*/
            Lribnhet = 1; /*distinguish this call to make ribbons, re RCSB PDB*/
            Lcommanded = 5;
            Lbuiltin = 1;
            Lribonucleic = 1; /*choice for  A-RNA vs B-DNA must be done here*/
            LcolorbyNtoC = 1;
            Lblackedge = 1;
            Lhetconnect = 0;
            ballradius = (float)0.32;
            htxl[1] = 1;
            atxl[1] = 1;
            htal[1] = 1; /*set this here to simulate rangeend logic*/
            mrange = 1; /*050731*/
            LNtermBetaFudge = 0; /*090219 avoid orphan betaarrow at start frag*/
         }
         else if(CompArgStr(p+1,"ca3rib", 6)) /*130121 for ribbon plane */
         {/*-ca3rib for ca-1,ca,ca+1 perp instead of C=O for ribbon plane*/
            Lca3ribbon = 1;
         }
         else if(CompArgStr(p+1,"ribbonmasters", 12)) /*BEFORE -ribbon !! */
         {/*-ribbonmasters: optional masters for alpha,beta,coil*/
            Lribbonmasters = 1;
         }
         else if(CompArgStr(p+1,"ribbon", 6))
         {/*-ribbon: variable width as a function of local geometry*/
            Lcommanded = 8;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"strand", 6))
         {/*-strand: ==ribbon of n strands, local curvature offsets 051114*/
            Lcrosstie = 0;
            Ledgedribbon = 0;
            Lskeinedribbon = 1;
            nstrnd = 1; /*default until read next entry*/
            Lcommanded = 8;
            Lbuiltin = 1;
            i = i+1; /*number is in next character string*/
            p = argv[i];
            j=0;
            k=0;
            while(j<256)
            {/*strobe out number*/
              if( isdigit(p[j]) ) { word[k] = p[j]; k++; j++; }
              else if(k==0) {/*no char yet*/ j++; }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%d",&nstrnd);
                 j=999; /*end the while loop*/
                 if(nstrnd > 9) {nstrnd = 9;}
                 if(nstrnd < 1) {nstrnd = 1;}
              }
            }/*strobe out residue number*/
         }
         else if(CompArgStr(p+1,"byresidue", 5)) /*051210 */
         {/*-byresidue : newribbon skeined ribbons vectorlist for each residue*/
            Lbyresidue = 1;
         }
         else if(CompArgStr(p+1,"arrowhead", 9)) /*check before arrows 051210 */
         {/*-arrowheads : newribbon skeined ribbons, arrowheads on beta*/
            Larrowhead = 1; /*newribbon  051210*/
         }
         else if(  (CompArgStr(p+1,"bestribbon", 10))
                 ||(CompArgStr(p+1,"arrows", 5))     ) /*combined 041106*/
         {/*-bestribbon by PDB SHEET,HELIX if present, else single spline*/
            /*-arrows legacy designation */
            Lribonucleic = 1; /*choice for  A-RNA vs B-DNA must be done here*/
            Lcommanded = 5;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"rnaribbon", 9)) /*-rnaribbon 040226*/
         {/*bestribbon and do nucleic acid ribbons as A-RNA rather than B-DNA*/
            LdoRNAribbon = 1; /*forces nucleic acid ribbon to be A-form like*/
            LdoDNAribbon = 0;
         }
         else if(CompArgStr(p+1,"dnaribbon", 9)) /*-dnaribbon 040226*/
         {/*bestribbon and do nucleic acid ribbons as B-RNA rather than A-DNA*/
            LdoDNAribbon = 1; /*forces nucleic acid ribbon to be B-form like*/
            LdoRNAribbon = 0;
         }
         else if(CompArgStr(p+1,"oldribbon", 9)) /*060929*/
         {/*-oldribbon using code in PKINRIBN*/
            Lribbonnew = 0;
         }
         else if(CompArgStr(p+1,"newribbon", 9)) /*051116*/
         {/*-newribbon using code in PKINRIBB*/
            Lribbonnew = 1;
         }
         else if(CompArgStr(p+1,"hbonds", 5)) /*051212*/
         {/*-hbonds : newribbon pseudo-hbonds approx where PKINHBND puts them*/
            Lribbonhbonds = 1;
         }
         else if(CompArgStr(p+1,"guide", 5)) /*050327*/
         {/*-guide : ribbon guide points displayed*/
            Lribbonguidepts = 1;
         }
         else if(CompArgStr(p+1,"skeins", 6)) /*140520 changed from -spline*/
         {/*-skeins :  (newribbons) ribbon splines displayed 051209 re -guide*/
            Lribbonrawsplines = 1;
         }
         else if(CompArgStr(p+1,"spline", 6)) /*140520 redefined, see -skeins*/
         {/*-spline :  (newribbons) single mid-spline vectorlist 140520*/
            Lsplinevector = 1;
            Lcrosstie = 0;
            Ledgedribbon = 0;
            Lskeinedribbon = 1;
            nstrnd = 1; 
            Lcommanded = 8;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"axis", 4))
         {/*-axis :  (newribbons) ribbon axial spline  re -guide*/
            Lribbonaxisspline = 1;
         }
         else if(CompArgStr(p+1,"density", 7)) /*060115*/
         {/*-density : (newribbons) density of ribbon axial splines, re -guide*/
            Lribbondensity = 1;
         }
         else if(CompArgStr(p+1,"also", 4)) /*050731*/
         {/*-also: if ribbons: also do selcted sidechains*/
            Lribbonalso = 1;
         }
         else if(CompArgStr(p+1,"Aform", 5)) /*040301*/
         {/*-Aform: if ribbons: do nucleics as A-RNA rather than B-DNA*/
            Lribonucleic = 1; /*overrides when issed after earlier bestribbon*/
         }
         else if(CompArgStr(p+1,"double", 6)) /*040419*/
         {/*-double :if ribbons do nucleics with doubled interval using c4' */
            Ldoublenucleics = 1;
         }
         else if(CompArgStr(p+1,"curved", 6)) /*040420*/
         {/*-curved :if ribbons do nucleics with curvature offset */
            Lcurvednucleics = 1;
         }
         else if(CompArgStr(p+1,"c4prime", 7)) /*040420*/
         {/*-c4prime :if ribbons do nucleics with curvature offset toward c4' */
            Lc4primenucleics = 1;
            Lcurvednucleics = 1;
         }
         else if(CompArgStr(p+1,"fencepost", 9)) /*040319*/
         {/*if protein ribbons: do fencepost orientation*/
            Lpeptideperprib = 1;
         }
         else if(CompArgStr(p+1,"noface", 6)) /*040319*/
         {/*-noface :if ribbons: do not fill in faces*/
            Lnoface = 1;
         }
         else if(CompArgStr(p+1,"noedge", 6)) /*040319*/
         {/*-noedge :(temporary) if ribbons: do not do a separate edge strand*/
            Lnoedge = 1;
         }
         else if(CompArgStr(p+1,"nucbeta", 7)) /*040301*/
         {/*if ribbons: use SHEET records for nucleic ranges with arrow heads*/
            Lnucleicarrows = 1;
         }
         else if(CompArgStr(p+1,"blackedge", 9)) /*040924*/
         {/*faced ribbon has unpickable black edge strand */
            Lblackedge = 1;
         }
         else if(CompArgStr(p+1,"cbstubs", 6)) /*041205*/
         {/*-cbstubs : ribbon spline stubs to CB for sidechains */
            Lcbstubsrib = 1;
         }
         else if(CompArgStr(p+1,"cacbvec", 7)) /*050403*/
         {/*cacbvec : CA---CB vector angles to ribbon direction*/
            Lcacbvector = 1;
         }
         else if(CompArgStr(p+1,"noblackedge", 11)) /*041106*/
         {/*faced ribbon has same color edge strand */
            Lblackedge = 0;
         }
         else if(CompArgStr(p+1,"nofudge", 7)) /*050327*/
         {/* ribbons without fudged width and without curvature fudge offset*/
            LNOfudge = 1;
         }
         else if(CompArgStr(p+1,"vectorCO", 8))
         {/*perpendicular vector from Calpha-Calpha to Carbonyl Oxygen*/
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"labels", 5))
         {/*label much angle info along with vectorCO vectors*/
            LvectorCOlabels = 1; /*deep in ribbon code...*/
         }
         else if(CompArgStr(p+1,"cispepdump", 10)) /*parse before cispep*/
         {/*cispepdump : cispeptides list (just @text, NOT a real kinemage!)*/
            Ldumpcispeptides = 1; /*deep in ribbon code...*/
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"cispeponly", 10))/*041106 parse before cispep*/
         {/*cispep : cispeptides emphasized as independent part of a kinemage*/
            Lcispeptides = 1; /*deep in ribbon code...*/
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            sprintf(colorextra,"hotpink"); /*041106*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"cispep", 6)) /*041102 parse after cispepdump*/
         {/*cispep : cispeptides emphasized as extra part of a ribbon kinemage*/
            Lcispeptides = 1; /*deep in ribbon code: alternative to LvectorCO*/
            extoutl = 1; /*generic extra output...*/
            sprintf(colorextra,"hotpink"); /*041106*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"azimuthCO", 9))
         {/*Calpha--Calpha angle, dihedral, azimuth to Carbonyl Oxygen*/
            LazimuthCO = 1;
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"phipsiCO", 8))
         {/*Calpha--Calpha angle,dihedral,azimuthCarbonylOxygen & phi,psi*/
            LphipsiCO = 1;
            LazimuthCO = 1;
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"psiphiCO", 8))
         {/*Calpha--Calpha angle,dihedral,azimuthCarbonylOxygen & psi,phi*/
            LpsiphiCO = 1;
            LazimuthCO = 1;
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"vectCOpdb", 9))
         {/*only Calpha,azimuth --> pdb format*/
            LvectCOpdb = 1;
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"pdbCACOpdb", 10))
         {/*only Calpha,azimuth --> pdb format*/
            LpdbCACOpdb = 1; /*030202*/
            LvectorCO = 1; /*uses ribbon code...*/
            extoutl = 1; /*generic extra output...*/
            Lcommanded = 8; /*fudge for now  030125*/
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"fakecalphas", 10))
         {/*-fakecalphas :fake calphas: no Calpha--Calpha distance check */
            /*no Ca--Ca distance filter*/
            LfakeCaCaspline = 1; /*111106*/
         }
         /*___________________Command Performance___________________*/
         /*___________________Command Performance___________________*/

         /********************General Options************************/
         else if(CompArgStr(p+1,"mage", 4))
         {
            LIOmage = 1;
            LIOpipe = 1;
         }
         else if(CompArgStr(p+1,"animate", 7))
         {/*-animate*/
            Lanimate = 1;
         }
         else if(CompArgStr(p+1,"nobutton", 8)) /*060120*/
         {/*-nobutton*/
            Lnobutton = 1;
         }
         else if(CompArgStr(p+1,"residuegroups", 12)) /*060120*/
         {/*-residuegroups*/
            Lresiduegroups = 1;
         }
         else if(CompArgStr(p+1,"alt", 3))
         {
            Laltcontrol = Laltcontrol+1;
            if     (p[4]=='\0') altcontrol[Laltcontrol] = ' ';
            else if(p[4]=='_')  altcontrol[Laltcontrol] = ' ';
            else                altcontrol[Laltcontrol] = p[4];
         }
         else if(CompArgStr(p+1,"sclists", 6))
         {/*each sidechain in a separate, nobutton, list*/
            Leachsclist = 1;
         }
         else if(CompArgStr(p+1,"scballs", 6))
         {/*-scballs: individual balls on sidechain n,o,s atoms*/
            Lballonscpoints = 1;
         }
         else if(CompArgStr(p+1,"scrings", 6))
         {/*triangle tesselation fills sidechain rings*/
            Lfillscrings = 1;
         }
         else if(CompArgStr(p+1,"bval", 4))
         {/*-bval: pointID B-values & occupancies*/
            if(n==0) n=1; /*create a range in which to put this control 040902*/
            for(ncount=nbegin;ncount<=n;ncount++) Bval[ncount] = 1;
         }
         else if(CompArgStr(p+1,"bcol", 4))
         {/*color by B-values PDB col 61...66*/
            if(n==0) n=1; /*create a range in which to put this control 040902*/
            Ncolorscalebasis = 1;
            for(ncount=nbegin;ncount<=n;ncount++) { Bcol[ncount] = 1; }
         }
         else if(CompArgStr(p+1,"ocol", 4))
         {/*color by occ-values PDB col 55...60*/
            if(n==0) n=1; /*create a range in which to put this control 040902*/
            Ncolorscalebasis = 2;
            for(ncount=nbegin;ncount<=n;ncount++) { Bcol[ncount] = 1; }
         }
         else if(CompArgStr(p+1,"ucol", 4))
         {/*color by values in PDB col 67...72*/
            if(n==0) n=1; /*create a range in which to put this control 040902*/
            Ncolorscalebasis = 3;
            for(ncount=nbegin;ncount<=n;ncount++) { Bcol[ncount] = 1; }
         }
         /* -colornc ,etc...*/
         else if(CompArgStr(p+1,"colornc", 7) || CompArgStr(p+1,"color53", 7))
         {/*ribbons, or aspect colored spectrally from N to C, or 5' to 3'*/
            LcolorbyNtoC = 1;
         }
         else if(CompArgStr(p+1,"colorabc", 8))
         {/*-colorabc : ribbons, or aspect colored by HEADER alpha, beta, coil*/
            LcolorbyABC = 1;
         }
         else if(CompArgStr(p+1,"colorbval", 9))
         {/*-colorbval : ribbons colored by Bvalue*/
            LcolorbyBval = 1; /*021213*/
         }
         else if(CompArgStr(p+1,"colorcurvature", 9))
         {/*-colorcurvature : ribbons colored by fractional extendedness*/
            Lcolorbycurvature = 1; /*051211*/
         }

         else if(CompArgStr(p+1,"halfbonds", 8))
         {/*intermediate point, ca--.--ca, divides Calpha vectors in half*/
            Lhalfbonds = 1;
         }
         else if(CompArgStr(p+1,"noaspects", 8))
         {/*aspects NOT outputted to kinemage even if present in PDB file*/
            Lallowaspectsin = 0;
         }
         else if(CompArgStr(p+1,"aspectnc", 8))
         {/*aspect added for spectral colors from N to C*/
            Laspectnc = 1;
         }
         else if(CompArgStr(p+1,"aspectabc", 9))
         {/*aspect added for coloring by HEADER alpha, beta, coil*/
            Laspectabc = 1;
         }
         else if(CompArgStr(p+1,"emphasis", 8))
         {/*rotatable vectors emphasized as fat lines*/
            Lemphasis = 1;
         }
         else if(CompArgStr(p+1,"noemphasis", 10))
         {/*rotatable vectors NOT emphasized as fat lines*/
            Lemphasis = 0;
         }
         else if(CompArgStr(p+1,"atomradii", 9))
         {/*individual radii on all c,n,o,s,h atoms*/
            Latomradii = 1;
         }
         else if(  (CompArgStr(p+1,"polarHradius",12))
                 ||(CompArgStr(p+1,"polarhradius",12)) )
         {/*polarHradius #.# :radius for polar H atoms, default==1.0*/
            i = i+1; /*number is in next character string*/
            p = argv[i];
            j=0;
            k=0;
            while(j<256)
            {/*strobe out radius number*/
              if( isdigit(p[j]) || p[j]=='.' )
              {
                 word[k] = p[j];
                 k++;
                 j++;
              }
              else if(k==0)
              {/*no char yet*/
                 j++;
              }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%f",&polarHradius);
                 j=999; /*end the while loop*/
              }
            }/*strobe out radius number*/
         }/*polarHradius*/
         else if( CompArgStr(p+1,"ballradius",10) ) /*041020*/
         {/*ballradius #.# :radius for all atom marker balls, default==0.2\r"*/
            i = i+1; /*number is in next character string*/
            p = argv[i];
            j=0;
            k=0;
            while(j<256)
            {/*strobe out radius number*/
              if( isdigit(p[j]) || p[j]=='.' )
              {
                 word[k] = p[j];
                 k++;
                 j++;
              }
              else if(k==0)
              {/*no char yet*/
                 j++;
              }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%f",&ballradius);
                 j=999; /*end the while loop*/
              }
            }/*strobe out radius number*/
         }/*ballradius*/

         else if(CompArgStr(p+1,"matrix",5))
         {/* m11 m12 m13  m21 m22 m23  m31 m32 m33 (  3x3 matrix) */
           iok = 1;
           for(h=1; h<=9; h++)
           {
             i = i+1; /*number is in next character string*/
             p = argv[i];
             iok = sscanf(p,"%f",&matrix[h]);
             if(iok != 1){break;}
           }
           if(iok != 1)
             {fprintf(stderr,"trouble reading %dth matrix element\n",h);}
           else {Lmatrix = 1;}
           iok = 1;
         }
         else if(CompArgStr(p+1,"center",6))
         {/* centerxyz */
           iok = 1;
           for(h=1; h<=3; h++)
           {
             i = i+1; /*number is in next character string*/
             p = argv[i];
             iok = sscanf(p,"%f",&rotcenter[h]);
             if(iok != 1){break;}
           }
           if(iok != 1)
             {fprintf(stderr,"trouble reading %dth center element\n",h);}
           else {Lrotcenter = 1;}
           iok = 1;
         }
         else if(CompArgStr(p+1,"translate",9)) /*050324*/
         {/* translate xyz */
           iok = 1;
           for(h=1; h<=3; h++)
           {
             i = i+1; /*number is in next character string*/
             p = argv[i];
             iok = sscanf(p,"%f",&translate[h]); /*050324*/
             if(iok != 1){break;}
           }
           if(iok != 1)
             {fprintf(stderr,"trouble reading %dth translate element\n",h);}
           else {Ltranslate = 1;} /*050324*/
           iok = 1;
         }
         /*___________________General Options_______________________*/
         /********************Selections*****************************/

         else if(CompArgStr(p+1,"colorscale", 10))
         {/*colorscale setup*/
            i = i+1; /*color names and dividing values are one string*/
            p = argv[i];

            /*presume color by B-values*/
            if(n==0) n=1; /*create a range in which to put this control 040902*/
            for(ncount=nbegin;ncount<=n;ncount++) Bcol[ncount] = 1;
            ncnt = 0;
            Ncolorscale = 1;/*reset, can be up to 10*/
            j = 0;
            k = 0;
            while(p[j]!='\0')
            {/*strobe out color names and dividing values*/
              if(p[j]==',')
              {
                 if(ncnt == 0)
                 {/*should have a color name*/
                    colorscale[Ncolorscale].color[k] = '\0';
                    ncnt = 1;
                 }
                 else
                 {/*should have a dividing value*/
                    valuestr[k] = '\0';
                    sscanf(valuestr,"%f",&colorscale[Ncolorscale].value);
                    Ncolorscale++;
                    ncnt = 0;
                 }
                 j++;
                 k = 0;
              }
              else if(p[j]==' ' && k==0)
              {/*preceeding white space*/
                 j++; /*look at next character*/
              }
              else if(ncnt==0)
              {/*putative character of a color name encountered */
                 colorscale[Ncolorscale].color[k] = p[j];
                 k++;
                 j++;
              }/*putative character of a color name encountered */
              else
              {/*putative digit of a dividing value encountered */
                  valuestr[k] = p[j];
                  k++;
                  j++;
              }/*putative digit of a dividing value encountered */
            }/*strobe out color names and dividing values*/
            /*should have a final color name*/
            colorscale[Ncolorscale].color[k] = '\0';
         }/*colorscale setup*/

         else if(CompArgStr(p+1,"quiet", 5))
         {/*-quiet: commandline only, no GUI*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
         }
         else if(CompArgStr(p+1,"code", 4))
         {/*-code: code string in all pointIDs*/
            Lcodestr = 1;
            i = i+1; /*code string is the next character string*/
            p = argv[i];
            j = 0;
            while(p[j]!='\0')
            {/*strobe out code string*/
               codestr[j] = p[j];
               j++;
               if(j >= MAXNAMECHAR) {break;}
            }
            codestr[j] = '\0'; /*longest j==MAXNAMECHAR*/
         }
         else if(CompArgStr(p+1,"name", 4))
         {/*-name: name sustitutes for MolNameStr in chain names, etc.*/
            Lmolecstr = 1;
            i = i+1; /*code string is the next character string*/
            p = argv[i];
            j = 0;
            while(p[j]!='\0')
            {/*strobe out code string*/
               molecstr[j] = p[j];
               j++;
               if(j >= MAXNAMECHAR) {break;}
            }
            molecstr[j] = '\0'; /*longest j==MAXNAMECHAR*/
         }
         else if(CompArgStr(p+1,"stdoutreport", 10))
         {
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
            Lstdoutreport = 1;
            Lquiet = 1;
            Lbuiltin = 1;
         }
         else if(CompArgStr(p+1,"scope", 5))
         {/*-scope*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
         }

         else if(   CompArgStr(p+1,"excludesc", 9)
                 || CompArgStr(p+1,"sc", 2) )
         {
            if(CompArgStr(p+1,"excludesc", 9)) resl[n] = -1;
            else                               resl[n] =  1;
            i = i+1; /*residue name(s) is in next character string*/
            p = argv[i];

            j = 0;
            k = 1;
            m = 0;
            while(p[j]!='\0')
            {/*strobe out residue name(s)*/
               if(p[j]==',')
               {
                  j++;
                  k++;
               }
               else
               {
                 resid[n][m] = p[j];
                 m++;
                 j++;
               }
            }
            if(resl[n]<0) resl[n] = -k;
            else          resl[n] =  k;
            resid[n][m] = '\0';

         }

         else if(CompArgStr(p+1,"show", 4))
         {/*-show*/
            if(n==0) n=1;
            i = i+1; /*controls are in next character string*/
            p = argv[i];

            j = 0;
            while(p[j]!='\0')
            {/*strobe out name(s)*/
              if(p[j]==',') j++;
              if(p[j]=='\0'||p[j+1] =='\0') break;
              k = 0;
              if(CompArgStr(p+j,"mc",2))           /*    mc    */
              {
                  for(ncount=nbegin;ncount<=n;ncount++) mcxl[ncount] = 1;
                  if(p[j+2]=='(')     /*) balance parens for editor*/
                  {/*scan for color*/ /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colormcvec[k]=p[j+3+k];
                      colormcvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                      /* ...mc(red),sc... */
                      /*    j+234567...   */
                      /*  k==  0123<-\0 k lags j by 3 */
                  }
                  /*now look for, e.g. mc(red)[off]  */
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offmc[h+1]=p[j+3+k+h];
                      offmc[h+1]='\0';
                      offmc[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }

              }
              else if(CompArgStr(p+j,"sc",2))      /*    sc    */
              {
/*
fprintf(stderr,"sidechain range: nbegin== %d, mrange== %d\n",nbegin,n);
*/
                  for(ncount=nbegin;ncount<=n;ncount++) scxl[ncount] = 1;
                  if(p[j+2]=='(')     /*) balance parens for editor*/
                  {/*scan for color*/ /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorscvec[k]=p[j+3+k];
                      colorscvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offsc[h+1]=p[j+3+k+h];
                      offsc[h+1]='\0';
                      offsc[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }
              }
              else if(CompArgStr(p+j,"cb",2))      /*    cb    */
              {/*Cb_only is a subset of sc*/
                  Lcbonly = 1; /*additional control on sc output*/
                  for(ncount=nbegin;ncount<=n;ncount++) scxl[ncount] = 1;
                  if(p[j+2]=='(')     /*) balance parens for editor*/
                  {/*scan for color*/ /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorscvec[k]=p[j+3+k];
                      colorscvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offsc[h+1]=p[j+3+k+h];
                      offsc[h+1]='\0';
                      offsc[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }
              }
              else if(CompArgStr(p+j,"hy",2))      /*    hy    */
              {
                  for(ncount=nbegin;ncount<=n;ncount++) hyxl[ncount] = 1;
                  if(p[j+2]=='(')      /*) balance parens for editor*/
                  {/*scan for color*/  /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorhyvec[k]=p[j+3+k];
                      colorhyvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offhy[h+1]=p[j+3+k+h];
                      offhy[h+1]='\0';
                      offhy[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }

              }
              else if(CompArgStr(p+j,"hb",2))      /*    hb    */
              {
                  for(ncount=nbegin;ncount<=n;ncount++) hbxl[ncount] = 1;
                  if(p[j+2]=='(')     /*) balance parens for editor*/
                  {/*scan for color*/ /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorhbvec[k]=p[j+3+k];
                      colorhbvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offhb[h+1]=p[j+3+k+h];
                      offhb[h+1]='\0';
                      offhb[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }

              }
              else if(CompArgStr(p+j,"ca",2))      /*    ca    */
              {
                  for(ncount=nbegin;ncount<=n;ncount++) caxl[ncount] = 1;
                  if(p[j+2]=='(')      /*) balance parens for editor*/
                  {/*scan for color*/  /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorcavec[k]=p[j+3+k];
                      colorcavec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offca[h+1]=p[j+3+k+h];
                      offca[h+1]='\0';
                      offca[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }

              }
              else if(CompArgStr(p+j,"ht",2))      /*    ht    */
              {
                  for(ncount=nbegin;ncount<=n;ncount++) htxl[ncount] = 1;
                  if(p[j+2]=='(')      /*) balance parens for editor*/
                  {/*scan for color*/  /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorhtvec[k]=p[j+3+k];
                      colorhtvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offht[h+1]=p[j+3+k+h];
                      offht[h+1]='\0';
                      offht[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }

              }
              else if(CompArgStr(p+j,"wa",2))      /*    wa    */
              {/*water*/
                  for(ncount=nbegin;ncount<=n;ncount++)
                  {
                     waxl[ncount] = 1;
                     waal[ncount] = 1;  /*040902*/
                  }
                  if(p[j+2]=='(')      /*) balance parens for editor*/
                  {/*scan for color*/  /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorwavec[k]=p[j+3+k];
                      colorwavec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offwa[h+1]=p[j+3+k+h];
                      offwa[h+1]='\0';
                      offwa[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }

              }
              else if(CompArgStr(p+j,"lb",2))      /*    lb    */
              {
                  for(ncount=nbegin;ncount<=n;ncount++) lbxl[ncount] = 1;
                  if(p[j+2]=='(')      /*) balance parens for editor*/
                  {/*scan for color*/  /*( balance parens for editor*/
                      for(k=0;p[j+3+k]!=')';k++) colorlbvec[k]=p[j+3+k];
                      colorlbvec[k]='\0';
                      k=k+2; /*so includes count for all chars in (...) */
                  }
                  if(p[j+k+2]=='[')     /*] balance parens for editor*/
                  {/*scan for off*/
                      for(h=0;p[j+3+k+h]!=']';h++) offlb[h+1]=p[j+3+k+h];
                      offlb[h+1]='\0';
                      offlb[0] = ' '; /*blank separator needed for parameters*/
                      k=k+h+2; /*so includes count for all chars in (...) */
                  }
              }
              else if(CompArgStr(p+j,"at",2))
                  for(ncount=nbegin;ncount<=n;ncount++) atxl[ncount] = 1;
              else if(CompArgStr(p+j,"ba",2))
                  for(ncount=nbegin;ncount<=n;ncount++) atxl[ncount] = 1;
              else if(CompArgStr(p+j,"ct",2))
                  for(ncount=nbegin;ncount<=n;ncount++) ctxl[ncount] = 1;
              else if(CompArgStr(p+j,"mK",2))
                  for(ncount=nbegin;ncount<=n;ncount++) mcCPKl[ncount] = 1;
              else if(CompArgStr(p+j,"sK",2))
                  for(ncount=nbegin;ncount<=n;ncount++) scCPKl[ncount] = 1;
              else if(CompArgStr(p+j,"hK",2))
                  for(ncount=nbegin;ncount<=n;ncount++) htCPKl[ncount] = 1;

              j=j+2+k;
            }/*strobe out name(s)*/
            if(atxl[n])
            {
              if(mcxl[n])
                  for(ncount=nbegin;ncount<=n;ncount++) mcal[ncount] = 1;
              if(scxl[n])
                  for(ncount=nbegin;ncount<=n;ncount++) scal[ncount] = 1;
              if(htxl[n])
                  for(ncount=nbegin;ncount<=n;ncount++) htal[ncount] = 1;
              if(waxl[n])
                  for(ncount=nbegin;ncount<=n;ncount++) waal[ncount] = 1;
            }
         }/*show*/
         else if(   CompArgStr(p+1,"range", 4)
                 || CompArgStr(p+1,"site",  4) )
         {/*-range "nstart-nend,nstart-nend,...,..." */
          /*-site  "nstart-nend,nstart-nend,...,..." 050731 */
          /*-siterec exppects to get site residues from pdb header */
          /*:sets of res# start-stop ranges, -range add to other ranges */
          /* -site dominates other ranges by setting sitel[n]...  050731 */

           if(n < MAXR-2) /*050309 protect array*/
           {
              /*060216 ranges given on cmdline need n set to current range#*/
              /*060216 site records need n set to next range number here*/
              if(CompArgStr(p+1,"range", 4))
              {
                 n = mrange; /*for cmdline ranges, n== current range 060216*/
                 mrange++;   /*then increase mrange to be ahead by one 060216*/
              }
              else if(CompArgStr(p+1,"site",4))
              {
                 mrange++;   /*for SITE recs, augment mrange first 060216*/
                 n = mrange; /*then set n == new mrange*/ /*051213*/
              }
           }
           if(n==0){mrange=n=1;} /*insure that there is a range*/
           if(CompArgStr(p+1,"site",4)) /*050731*/
           {
              Lsite = 1;    /* site ranges honored*/
              sitel[n] = 1; /*051213*/
           }
           if(!Lcommanded) Lcommanded = 99;
                    /*command performance as specified here*/

           if(!CompArgStr(p+1,"siterec",7)) /*050917 for SITE record info*/
           {/*site or range residue numbers given in commandline*/
            i = i+1; /*controls are in next character string*/
            p = argv[i];

            ln = strlen(p); /*050309*/

            k = 0;
            j = 0;
            nnumber = 0;
            sign = +1;
            deliniated = 0;
            while(j < ln) /* 050309 was while(j<256) */
            {/*while j strobe out range residue numbers*/
              /*working on jth char index, and n==mrange range*/
              if(Lsite){sitel[n] = 1;} /*050731*/
              /*this routine insensitive to embedded blanks*/
              if(isdigit(p[j])||p[j]=='+')/*disambiguate - for -numbers below*/
              {
                 word[k] = p[j];
                 k++;
                 if(nnumber == 0) nnumber = 1;/*working on first number*/
                 else if(deliniated) nnumber = 2;/*working on second number*/
              }
              else if(p[j]=='-') /*deliniator or minus*/
              {
                 if(nnumber==0 ||deliniated)
                 {/*presume - is minus sign of a number*/
                    sign = -1;
                 }
                 else
                 {/*take - as deliniator between first and last range elements*/
                    word[k]='\0'; /*end number string*/
                    sscanf(word,"%d",&resnu1[n]);
                    resnu1[n] = sign*resnu1[n];
                    sign = +1; /*reset*/
                    k=0;
                    deliniated = 1;
                 }
              }
              if(p[j]==','||p[j]=='\0' ||j==(ln-1) )/*050309 !else, last char*/
              {/* ',' is logical OR for series of ranges*/
                 /*if(nnumber==2) always takes number just before a , as #2*/
                 {
                    word[k]='\0'; /*end number string*/
                    sscanf(word,"%d",&resnu2[n]);
                    resnu2[n] = sign*resnu2[n];
                 }
                 if(nnumber==1) /*was else*/
                 {/*only got one number, so range only has one element*/
                    resnu1[n] = resnu2[n]; /*set number 1 to complete range*/
                 }
                 k=0; /*zero k, but still testing this jth character*/

                 if(p[j]=='\0') /*end the while loop 050309 was j=999*/
                 {
                    j=ln;
                 }
                 else if(p[j]==',') /*get set for another entry*/
                 {
                    if(n < MAXR-2) {n = mrange++;} /*050309 protect array*/
                       /*mrange always one ahead of range number*/
                    sign = +1;
                    nnumber = 0;
                    deliniated = 0;
                 }
              }
              j++; /*now reposition j to try next character no matter what*/
            }/*while j strobe out range residue numbers*/
           }/*site or range residue numbers given in commandline*/
         }/*range*/
         else if(CompArgStr(p+1,"focusresidue", 8))
         {/*focusresidue*/
            i = i+1; /*number is in next character string*/
            p = argv[i];
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
            j=0;
            k=0;
            while(j<256)
            {/*strobe out residue number*/
              if( isdigit(p[j]) )
              {
                 word[k] = p[j];
                 k++;
                 j++;
              }
              else if(k==0)
              {/*no char yet*/
                 j++;
              }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%d",&resnumfovea);
                 j=999; /*end the while loop*/
                 Lresnumfovea = 1; /*set option*/
                 FOVEA = 1; /*apply focus*/
              }
            }/*strobe out residue number*/
         }/*focusresidue*/
         else if(CompArgStr(p+1,"focuschainID", 10))
         {/*focuschainID*/
            i = i+1; /*chainID is in next character string*/
            p = argv[i];
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
            j=0;
            k=0;
            while(j<256)
            {/*strobe out chainID character*/
              if(p[j]=='\0') break; /*reached end of this substring*/
              else if(p[j]==' ' && k==0)
              {/*preceeding white space*/
                 j++; /*look at next character*/
              }
              else
              {/*character encountered */
                 if(p[j]=='_')
                 {/*request for the chain with NO chain identifier*/
                    /*default is blank spaces  2char chainID 070926*/
                    focuschaincharcode[0] = ' ';
                    focuschaincharcode[1] = ' ';
                 }
                 else
                 {/*some character is to be treated as the chain identifier*/
                    if(p[j+1]=='\0' || j==1) /*070926*/
                    {/*last char specified, chainID presumed right justified*/
                      focuschaincharcode[1] = p[j];/*putative chain identifier*/
                    }
                    else if(j==0)
                    {/*possibly a 2 char chainID, just fill in turn*/
                      focuschaincharcode[0] = p[j];/*putative chain identifier*/
                    }
                    else break; /*out of while loop: can use only one chainID*/
                 }
                 j++;
                 k++; /*past any leading blanks*/
              }
            }/*strobe out chainID character*/
         }/*focuschainID*/
         else if(CompArgStr(p+1,"focusradius", 8))
         {/*focusradius*/
            i = i+1; /*number is in next character string*/
            p = argv[i];
            j=0;
            k=0;
            while(j<256)
            {/*strobe out radius number*/
              if( isdigit(p[j]) || p[j]=='.' )
              {
                 word[k] = p[j];
                 k++;
                 j++;
              }
              else if(k==0)
              {/*no char yet*/
                 j++;
              }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%f",&focussc);
                 focusca = focussc + 7;
                 focusmc = focussc + 2;
                 focuswa = focussc + 2;
                 focusht = focussc + 2;
                 j=999; /*end the while loop*/
              }
            }/*strobe out radius number*/
         }/*focusradius*/

         else if(CompArgStr(p+1,"chainnum", 8))
         {/*chainnum choice*/
            i = i+1; /*number is in next character string*/
            p = argv[i];

            /*A succession of chain numbers can be accummulated*/
            /*because number choice is done before a pass by entries in gflag[256]*/
            /*Note: chain methods are incompatible: number filters then charcode filters*/
            j=0;
            k=0;
            while(j<256)
            {/*strobe out chain number*/
              if(p[j]==' ' && k==0)
              {/*preceeding white space*/
                 j++; /*look at next character*/
              }
              else if( isdigit(p[j]) )
              {
                 word[k] = p[j];
                 k++;
                 j++;

              }
              else
              {/*presume whole number is in*/
                if(k > 0)
                {/*number field has at least one character*/
                  word[k]='\0'; /*end number string*/
                  sscanf(word,"%d",&chainnumber);
                  if(chainnumber >0 && chainnumber <255)
                  {/*adjust gflag[] to filter at prekin's recognized chain number*/
                    gflag[0] = 0; /*NOT do all chains*/
                    gflag[chainnumber] = 1; /*DO this numbered chain*/
                  }
                }/*number field has at least one character*/
                j=999; /*end the while loop*/
              }
            }/*strobe out chain number or code character*/
         }/*chainnum choice*/
         else if(CompArgStr(p+1,"chainid", 5)) /*just recognize chain*/
         {/*-chainid choice, -chainID*/
            i = i+1; /* char(s) are in next character string*/
            p = argv[i];
            /*augment number of chain character codes as chars are read in*/
            /*many chain IDs can be chosen by char code in this implementation*/
            /*(and is treated as independent of case)*/
            /*note: chain number choice is done before a pass by entries in gflag[256]*/
            /*the char code is checked during a pass in rangecontrol()*/
            /*The two methods are incompatible: number filters then charcode filters*/

            if(n==0) n=1; /*ensure that this is a viable range*/
            k = 0; /*leading blank(s) filter*/
            j = 0;
            while(p[j]!='\0')
            {/*strobe out chain code character(s)*/
              if(Nchaincharcode == MAXchaincharcode) break;
              chaincharcode[Nchaincharcode][0] = ' ';
              chaincharcode[Nchaincharcode][1] = ' ';
              chaincharcode[Nchaincharcode][2] = '\0';
              if(p[j]=='\0') break; /*reached end of this substring*/
              else if(p[j]==',') j++; /*comma valid effective AND */
              else if(p[j]==' ' && k==0)
              {/*preceeding white space*/
                 j++; /*look at next character*/
              }
              else
              {/*character encountered */
                 if(p[j]=='_')
                 {/*request for the chain with NO chain identifier*/
                    /*default is blank spaces  2char chainID 070926*/
                    chaincharcode[Nchaincharcode][0] = ' ';
                    chaincharcode[Nchaincharcode][1] = ' ';
                 }
                 else
                 {/*some character is to be treated as the chain identifier*/
                    if(p[j+1]=='\0' || j==1) /*070926*/
                    {/*last char specified, chainID presumed right justified*/
                      chaincharcode[Nchaincharcode][1] = p[j];
                         /*putative chain identifier*/
                    }
                    else if(j==0)
                    {/*possibly a 2 char chainID, just fill in turn*/
                      chaincharcode[Nchaincharcode][0] = p[j];
                         /*putative chain identifier*/
                    }
                    else break; /*out of while loop: can use only one chainID*/
                 }
                 j++;
                 k++; /*past any leading blanks*/
              }
            }/*strobe out chain code character*/
            Nchaincharcode++; /*Total is always one > last index*/
         }/*chainid choice*/

         else if(CompArgStr(p+1,"model", 5)) /*just recognize chain*/
         {/*model choice*/
            i = i+1; /* char(s) are in next character string*/
            p = argv[i];
            /*augment number of model character codes as chars are read in*/
            /*many model IDs can be chosen by char code in this implementation*/
            /*(and is treated as independent of case)*/

            if(n==0) n=1; /*ensure that this is a viable range*/
            h = 0;
            k = 0; /*leading blank(s) filter*/
            j = 0;
            while(p[j]!='\0')
            {/*strobe out model code character(s)*/

              if(Nmodelcharcode == MAXmodelcharcode) break;
              else if(p[j]==',')
              {
                 j++; /*comma valid effective AND */
                 h = 0;/*reset character counter for putative 3char model code*/
                 Nmodelcharcode++; /*Total is always one > last index*/
              }
              else if(p[j]==' ' && k==0)
              {/*leading blank*/
                 j++; /*look at next character*/
              }
              else
              {/*character encountered */
                 if(p[j]=='_')
                 {/*request for the model with NO model identifier*/
                    ; /*blank space, leave 3 \0 in an indexed place*/
                 }/*so OK if no model designation*/
                 else
                 {/*some character is to be treated as the chain identifier*/
                    modelcharcode[Nmodelcharcode][h] = p[j];

                      /*putative model identifier: accum left to right*/
                    h++;
                 }
                 j++;
                 k++; /*past any leading blanks*/
              }
            }/*strobe out model code character*/
            Nmodelcharcode++; /*Total is always one > last index*/
         }/*model choice*/

         else if(CompArgStr(p+1,"rotate", 3))
         {/*rotate residue*/
            i = i+1; /*number is in next character string*/
            p = argv[i];

            n = mrange; /*force single residue range for this range*/
            j=0;
            k=0;
            while(j<256)
            {/*strobe out residue number*/
              if( isdigit(p[j]) )
              {
                 word[k] = p[j];
                 k++;
                 j++;
              }
              else if(k==0)
              {/*no char yet*/
                 j++;
              }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%d",&resnumrotate);
                 j=999; /*end the while loop*/
                 rotl[n] = 1; /*set option*/
                 resnu1[n]=resnumrotate;
                 resnu2[n]=resnumrotate;
                 resid[n][0] = '.';/* .  consistent with dialog box spec. */
                 resid[n][1] = '.';
                 resid[n][2] = '.';
                 resid[n][3] = '\0';
                 resl[n]=0; /*NOT named so will NOT do a mutation!*/
                 mutl[n]=0;
              }
            }/*strobe out residue number*/
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/

         }/*rotate residue*/
         else if(CompArgStr(p+1,"mutate", 3))
         {/*-mutate residue*/
            i = i+1; /*number is in next character string*/
            p = argv[i];

            n = mrange; /*force single residue range for this range*/
            j=0;
            k=0;
            while(j<256)
            {/*strobe out residue number*/
              if( isdigit(p[j]) )
              {
                 word[k] = p[j];
                 k++;
                 j++;
              }
              else if(k==0)
              {/*no char yet*/
                 j++;
              }
              else
              {/*presume whole number is in*/
                 word[k]='\0'; /*end number string*/
                 sscanf(word,"%d",&resnummutate);
                 resnu1[n]=resnummutate;
                 resnu2[n]=resnummutate;
                 j=999; /*end the while loop*/
              }
            }/*strobe out residue number*/
            i = i+1; /*residue name(s) is in next character string*/
            p = argv[i];

            j = 0;
            k = 1;
            m = 0;
            while(p[j]!='\0')
            {/*strobe out residue name(s)*/
               if(p[j]==',')
               {
                  j++;
                  k++;
               }
               else
               {
                 resid[n][m] = p[j];
                 m++;
                 j++;
               }
            }/*strobe out residue name(s)*/
            resid[n][m] = '\0';;

            rotl[n]=1; /*set option*/
            resl[n]=1; /* named so will do a mutation!*/
            mutl[n]=1;
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
            Lmutate = 1;
         }/*mutate residue*/
         else if(CompArgStr(p+1,"pdbout", 6))
         {
            Lpdbout = 1;
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
         }

         else if(CompArgStr(p+1,"superpos", 8)) /*superpos aa */
         {/*superpositions all these aa by their ideal Cbeta*/
            i = i+1; /*residue name(s) is in next character string*/
            p = argv[i];

            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            j = 0;
            k = 1;
            m = 0;
            while(p[j]!='\0')
            {/*strobe out residue name*/
               if(p[j]==',')
               {
                  m = 0; /*silently take last res name if more than one given*/
                  j++;
                  k++;
               }
               else
               {
                 resid[n][m] = p[j];
                 m++;
                 j++;
               }
            }/*strobe out residue name*/
            resid[n][m] = '\0';;
            Lsuperpos = 1;
            supl[n]=1; /*set option*/
            resl[n]=1; /* named so will do a superposition!*/
            mcxl[n]=1;
            scxl[n]=1;
            Bval[n] = 1;
            if(!Lcommanded) Lcommanded = 99;
               /*command performance as specified here*/
         }/*superpositions all of this aa onto ideal aa in std coords*/

         else if(CompArgStr(p+1,"calphataudev", 12))
         {/*- :deviation from ideal tau angle at Calpha*/
            Ltaudev = 1;
            /*simulate -scope flag so -calphataudev can be used alone*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }

         else if(CompArgStr(p+1,"cbetadev", 8))
         {/*- :deviation and spread of ideal beta from actual beta*/
            Lcbetadev = 1;
            /*simulate -scope flag so -cbetadev can be used alone*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }

         else if(CompArgStr(p+1,"segid", 5))
         {/*- :use segid field instead of chainid field*/
            UseSEGIDasChain = 1;
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }

         else if(CompArgStr(p+1,"cbdevdump", 9))
         {/*- :deviation of ideal beta as : delineated list*/
            if(CompArgStr(p+1,"cbdevdumpmax", 12)) /*040318*/
            {/*just output the maximum value of cbetadeviation*/
               Lcbdevdumpmax = 1;
               cbetadevmax = 0; /*set initial value*/
               Lsingleout = 1; /*only output after all chains processed*/
            }
            else {Lcbdevdumpmax = 0;}
            Lcbdevdump = 1; /*kind of stuff to dump*/
            Ldump = 1;      /*output control for colon-delineated-list dump*/
            Lcbetadev = 1;  /*use this kinemage stuff for dump*/
            /*011027 uses PKINCSUB/mutateresidue & PKINANGL/constructbeta*/
            /*simulate -scope flag so -cbdevdump can be used alone*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }         
         
         else if(CompArgStr(p+1,"operptoline",11 )) /*091129*/
         {/*O3' instead of phosphate perpendiculars to base C1'--N19*/
            Loperptoc1nline = 1; /*091129 mimics Lpperptoc1nline */
            Lpperptobase = 1; /*uses machinery of pperptobase*/ 
            /*simulate -scope flag so -operptoline can be used alone*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }
         else if(CompArgStr(p+1,"pperptobase",11 ))
         {/*phosphate perpendiculars to base*/
            Lpperptobase = 1;
            /*simulate -scope flag so -pperptobase can be used alone*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }
         else if(CompArgStr(p+1,"pperptoline",11 )) /*050125*/
         {/*phosphate perpendiculars to c1'--nbase line*/
            Lpperptoc1nline = 1; /*050125 additional logical*/
            Lpperptobase = 1; /*uses machinery of pperptobase*/
            /*simulate -scope flag so -pperptobase can be used alone*/
            n = mrange+1; /*new range*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }
         else if(CompArgStr(p+1,"pperpplot", 9 ))/*050125 demoted to mere flag*/
         {/*3D plot of phosphate perpendiculars to base vs delta*/
            Lplotpperptobase = 1;
         }
         else if(CompArgStr(p+1,"pperpdump", 9 ))/*050125 demoted to mere flag*/
         {/*colon delineated dump of phosphate perpendiculars to bases*/
            Lplotpperptobase = 1;
            Ldumppperptobase = 1; /*extra flag for pure dump*/
         }
         else if(CompArgStr(p+1,"pperpoutliers", 12 ))/*050125 demoted to flag*/
         {/*outliers only for:*/
            /*3D plot of phosphate perpendiculars to base vs delta   OR*/
            /*colon delineated dump of phosphate perpendiculars to bases*/
            /*real kinemage 3D vectors e.g. for MultiCrit */
            Lpperpoutliersonly = 1;
         }
         else if(CompArgStr(p+1,"connectdump", 7)) /*061202*/
         {/*- :connectivity of residues as : delineated list*/
            Lconnectresiduesdump = 1; /*kind of stuff to dump*/
            Ldump = 1;      /*output control for colon-delineated-list dump*/
            /*simulate -scope flag so -...dump can be used alone*/
            n = mrange+1; /*new range*/
            mcxl[n] = 1; /*connectivity along mainchain*/
            nbegin = n; /*in case ranges sets up more than one for show set*/
            mrange = n; /*at least*/
            if(!Lcommanded) Lcommanded = 99; /*command performance*/
         }
         else
         {/*unrecognized flag: cannot be quiet about this... 061020*/
            Lquiet = 0;
         }
         /*___________________Selections____________________________*/

      }/*flag*/
      else if(!Lgot_infilename)
      {/*presume unflagged character str is a file name, dangerous as it is*/
          strcpy(NameStr,argv[i]);
          /*file name copied into prekin input file Name*/
          Lstartfile=1; /*file launched prekin, use it*/
          Lgot_infilename = 1;
          Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
      }
      else if(Lgot_infilename && !Lgot_outfilename)
      {/*presume unflagged character str is a file name*/
          strcpy(OutfileStr,argv[i]);
          /*file name copied into prekin input file Name*/
          Lgot_outfilename = 1;
          Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
      }

   }/*loop over arguments*/

   /*reconcile combined modes, specific dominates general*/
   if(Lpdbout && Lmutate)
   {
      Lpdbmutout = 1;
   }
   else if(Lpdbout && (Lmatrix || Lrotcenter || Ltranslate))/*050324 translate*/
   {
      LpdbIO = 1;
   }

   if     ( LIOmage ) /*020514*/
   {
       Lout_stdout = 1; /*output must be to stdout*/
       Lgot_outfilename = 1;
       Lnewfile=1; /*WHY??*/ /*file is known, selectfiles() to open or prepare*/
       Lbrowsedefault = 0;
       Lcommanded = 1;
       Lbuiltin = 1;
       Lquiet = 0;
   }
   else if( Lcommanded>0 && Lgot_infilename && Lgot_outfilename )
   {
       Lquiet = 1; /*can run silently with NO  GUI  !!!!  */
   }
   else if(Lgot_infilename && Lgot_outfilename)
   {
       Lbrowsedefault = 1; /*automatic MAGE launch 961117*/
       Lcommanded = 1; /*browser script as commandline default*/
       Lbuiltin = 1;
       Lquiet = 1;
   }
   else if(Lgot_infilename && !Lgot_outfilename)
   {/*already have an input file designated, but not yet an outfile*/
      /*Lcommanded may or may not be set*/
      if(Lcommanded == 0)
      {
         /*DO NOT presume output to stdout*/
         /*Set flags so GUI implimented and get dialog for output file !!!! */
         Lout_stdout = 0;
         Lgot_outfilename = 0;
         Lquiet = 0; /*Must set up GUI and ask for output file name*/
      }
      else
      {/*in command performance: run silently, run deep, */
         /*so presume output to stdout*/
         Lout_stdout = 1;
         Lgot_outfilename = 1;
         Lnewfile=1;   /*file is known, selectfiles() to open or prepare*/
         Lquiet = 1; /*can run silently with NO  GUI  !!!!  */
         /*040705 ???? are there any commandline cases for which there should*/
         /*NOT be ranges specified? i.e. mrange==0, or would it be safe to */
         /*set mrange++ so things like Bval (i.e. -bval) will be picked up */
         /*in the rangecontrol section of PKINCSUB ????*/
      }
   }

#ifdef UNIX_X11
   if(Lreport != 0)
   {
       fprintf(stderr,"PREKIN %s, called with %d arguments: (Lcommanded==%d)\n"
          ,version,(*argc-1),Lcommanded);
       for(i=1; i<*argc; i++)
       {/*loop over arguments*/
          p = argv[i];
          fprintf(stderr,"%s\n", p);
       }/*loop over arguments*/
       /*Ltestprint = 1;*/
   }
#endif  /*UNIX_X11*/

#ifdef NOGUI  /*060324 re compile with NOGUI, requires Lquiet=1*/
   if(!Lquiet)
   {
      fprintf(stderr,"PREKIN %s ,I/O??: try 'prekin -h'\n",version); /*one line 061020*/
      /*fprintf(stderr,"for usage do: prekin -h\n");*/
      exit(0);
   }
#endif /* NOGUI 060324 re compile with NOGUI, requires Lquiet=1*/
}
/*___parsecommandline()______________________________________________________*/

/****writeversiontooutput()***************************************************/
void writeversiontooutput() /*write version, fphelp defaults to stdout 041011*/
{
   fprintf(fphelp,"PREKIN %s\n",version);
}
/*___writeversiontooutput()__________________________________________________*/

/****writechangestooutput()***************************************************/
void writechangestooutput() /*0400215 re:MAGEINIT/writechangestostdout 031011*/
{
   int more = 1, j=0, Nth=0;

   fprintf(fphelp,"PREKIN %s\n",version);

   while(more)
   {/*loop over all text for this call*/
      for(j=0; j<255; j++) /*save a space for safety '\0' */
      {/*load transfer string*/
         temps[j] =  prekinchanges[Nth][j];
         if(temps[j] == '\r')
         {/*ASCII Carraige Return*/
            temps[j] = '\0'; /*make into a C string*/
            /*platform specific End-Of-Line character(s) will be added later*/
         }
         if(temps[j] == '\0'){ break;} /*separate end of text piece*/
      }/*load transfer string*/
      /*check for END before writing to stdout*/
      if(temps[0] =='E' && temps[1] =='N' && temps[2] =='D'){more = 0;}
      temps[j+1] = '\0'; /*safety if runs off end of temps str*/
      fprintf(fphelp,"%s"EOLO,temps);
      Nth++; /*local, not static, so will get reset ==0 on new call*/
   }/*loop over all text for this call*/
   fprintf(fphelp,"PREKIN %s\n",version);
}
/*___writechangestooutput()__________________________________________________*/

/****writecmdhelptooutput()***************************************************/
void writecmdhelptooutput() /*040425 re:writechangestostdout*/
{
   int more = 1, j=0, Nth=0;

   fprintf(fphelp,"PREKIN %s\n",version);

   while(more)
   {/*loop over all text for this call*/
      for(j=0; j<255; j++) /*save a space for safety '\0' */
      {/*load transfer string*/
         temps[j] =  prekincmdhelp[Nth][j];
         if(temps[j] == '\r')
         {/*ASCII Carraige Return*/
            temps[j] = '\0'; /*make into a C string*/
            /*platform specific End-Of-Line character(s) will be added later*/
         }
         if(temps[j] == '\0'){ break;} /*separate end of text piece*/
      }/*load transfer string*/
      /*check for END before writing to stdout*/
      if(temps[0] =='E' && temps[1] =='N' && temps[2] =='D'){more = 0;}
      temps[j+1] = '\0'; /*safety if runs off end of temps str*/
      fprintf(fphelp,"%s"EOLO,temps);
      Nth++; /*local, not static, so will get reset ==0 on new call*/
   }/*loop over all text for this call*/
   fprintf(fphelp,"PREKIN %s\n",version);
}
/*___writecmdhelptooutput()__________________________________________________*/

/****writehelptohtmlfile()****************************************************/
void writehelptohtmlfile(int mode) /*040425*/
{
   int more=0, i=0, j=0, k=0, Nth=0, ihelp=0, nhelp=0, it=0;
   int nboxes=0, Lbox = 0, n=0;
   char bigword[512];  /*can expand confused hypertext: <comment> */

 /*called originally with mode==0 and Lhelpoutopen==0 */
 /*UNIX modal dialog sets Lhelpoutopen==1 and allows re-call with mode==1*/
 if(mode==0){openhelpout();} /*PKMCFILE,PKPCFILE,PUXMOUTP*/
 if(Lhelpoutopen && fphelpout!=NULL) /*excessive protection !?*/
 {

 /*Bryan's DOCTYPE first line  050920 */
 fprintf(fphelpout,"<!DOCTYPE HTML PUBLIC ");  /*no line feed*/
 fprintf(fphelpout,"\"-//W3C//DTD HTML 4.01 Transitional//EN\"\n");/*no gt*/
 fprintf(fphelpout,"     \"http://www.w3.org/TR/html4/loose.dtd\">\n"); /* gt*/
 fprintf(fphelpout,"<html>\n");
 fprintf(fphelpout,"<!--help output written by prekin-->\n");
 fprintf(fphelpout,"<head>\n");
 fprintf(fphelpout,"   <title>Prekin Internal Help</title>\n");
 fprintf(fphelpout," <style type=\"text/css\">\n");
 fprintf(fphelpout," body {font-family: Verdana, Helvetica, sans-serif&#059;}\n");
 fprintf(fphelpout,"    p {font-size: 1.0em&#059;}\n");
   fprintf(fphelpout," </style>\n");
   fprintf(fphelpout,"</head>\n");
   fprintf(fphelpout,"<body>\n");
   fprintf(fphelpout,"<h2><a name=\"pt0\">Prekin Internal Help:</a></h2>\n");
   fprintf(fphelpout,"<p>%s</p>\n",version);

   k = 1;  /*over all nboxes, currently 1*/
   nboxes = 1;
  /*begin list of menu help items -------------------------------*/
   fprintf(fphelpout, "<h3>");

  /*menuaboutmutation(); mutations and rotations */
   fprintf(fphelpout, /*menuaboutmutation();*/
      "<a href=\"#pt%d\">mutations and rotations</a><br />\n",k++);

  /*iterate, augmenting k, through the help dialog boxes*/

  /*end of list of menu help items -------------------------------*/
   fprintf(fphelpout,"<br />\n");
  /*continue with prekin internal help sections -------------------------*/

  /*step through the help sections contained in giant character arrays*/
   fprintf(fphelpout, "<a href=\"#pt50\">commandline help</a><br />\n");
   fprintf(fphelpout, "<a href=\"#pt60\">Ribbon Help</a><br />\n");
   fprintf(fphelpout, "<a href=\"#pt70\">VRML Help</a><br />\n");
   nhelp = 3;
   fprintf(fphelpout,"</h3>");
  /*end of listing of help items -------------------------------------*/

   for(n=1; n<=nboxes; n++)
   {/*loop through all help menu dialog boxes*/
      k = 0; /*for bigword[]*/
      it = 1; /*controls flow in menu-->dlog/html subroutines */
      Lbox = 1; /*to get started into the while loop*/

      while(Lbox) /*it from 1 to whatever is needed, returns global temps[]*/
      {/*within a box doing it number of entries*/

         if     (n== 1) {Lbox = menuaboutmutations(it);}/*mutations, rotations*/
         else {Lbox = 0;}

         if(Lbox)
         {/*menu dialog box contents*/
            if(it==1)
            {  /*word written by menu dialog routine in PKINMENU*/
               fprintf(fphelpout,"<h3><a name=\"pt%d\">%s</a>\n",n,word);
               fprintf(fphelpout,"<a href=\"#pt0\">^TOP^</a></h3>\n");
            }
            fprintf(fphelpout,"<p>");
            k = 0;
            for(i=0; i<=255; i++)
            {
               bigword[k] = temps[i];
               if(bigword[k] == '\r' ||bigword[k] == '\n'||bigword[k] == '\0')
               {
                  bigword[k] = '\0';
                  fprintf(fphelpout,"%s",bigword);
                  fprintf(fphelpout,"<br />\n");
                  if(temps[i] == '\0') break; /*test real string for end*/
                  k = 0;
               }
               else if(bigword[k] == '<')
               {/*expand to avoid confusion with hypertext tag*/
                  bigword[k++] = '&';
                  bigword[k++] = 'l';
                  bigword[k++] = 't';
                  bigword[k++] = ';';
               }
               else if(bigword[k] == '>')
               {/*expand to avoid confusion with hypertext tag*/
                  bigword[k++] = '&';
                  bigword[k++] = 'g';
                  bigword[k++] = 't';
                  bigword[k++] = ';';
               }
               else
               {
                  k++;
               }
            }
            fprintf(fphelpout,"</p>");
            it++;
         }/*menu dialog box contents*/
      }/*within a box doing it number of entries*/
   }/*loop through all help menu dialog boxes*/

   for(ihelp = 1; ihelp <= nhelp; ihelp++)
   {/*loop over help sections*/

     if(ihelp==1){fprintf(fphelpout,"<h3><a name=\"pt50\">command help</a>\n");}
     if(ihelp==2){fprintf(fphelpout,"<h3><a name=\"pt60\">ribbon help</a>\n");}
     if(ihelp==3){fprintf(fphelpout,"<h3><a name=\"pt70\">VRML help</a>\n");}

     fprintf(fphelpout,"<a href=\"#pt0\">^TOP^</a></h3>\n");
     Nth = 0;
     more = 1;
     fprintf(fphelpout,"<p>");
     while(more)
     {/*loop over all text for this call*/
        k = 0;  /*for bigword[] */
        for(j=0; j<255; j++) /*save a space for safety '\0' */
        {/*load transfer string*/

            if     (ihelp == 1) {temps[j] = prekincmdhelp[Nth][j]; }
            else if(ihelp == 2) {temps[j] = prekinribbonhelp[Nth][j]; }
            else if(ihelp == 3) {temps[j] = prekinVRMLhelp[Nth][j]; }

            if(temps[j] == '\r')
            {/*ASCII Carraige Return*/
               temps[j]=EOL; /*platform specific End-Of-Line character*/
            }
            bigword[k] = temps[j]; /*transfer to expandable charstr*/
            if(bigword[k] == '<')
            {/*expand to avoid confusion with hypertext flag*/
               bigword[k++] = '&';
               bigword[k++] = 'l';
               bigword[k++] = 't';
               bigword[k++] = ';';
            }
            else if(bigword[k] == '>')
            {/*expand to avoid confusion with hypertext flag*/
               bigword[k++] = '&';
               bigword[k++] = 'g';
               bigword[k++] = 't';
               bigword[k++] = ';';
            }
            else
            {
               k++;
            }
            if(temps[j] == '\0'){ break;} /*separate end of text piece*/
         }/*load transfer string*/
         /*check for END before fprintf()*/
         if(temps[0] =='E' && temps[1] =='N' && temps[2] =='D'){more = 0;}
         temps[j] = '\0'; /*safety if runs off end of temps str*/
         bigword[k] = '\0';
         fprintf(fphelpout,"<br />%s",bigword);/*use expandable str*/
         Nth++;
      }/*loop over all text for this call*/
      fprintf(fphelpout,"<br />\n</p>");
   }/*loop over help sections*/
   fprintf(fphelpout,"<h3><a href=\"#pt0\">^TOP^</a></h3>\n");

   fprintf(fphelpout,"</body>\n");
   fprintf(fphelpout,"</html>\n");
   fclose(fphelpout);
   fphelpout = NULL;
   Lhelpoutopen = 0;
 }
}
/*___writehelptohtmlfile()___________________________________________________*/

