
/****constructcircle()********************************************************/
int  constructcircle(float A[3], float B[3], float radius, float C[SEGMENTS][3])
{
   /*taken from MAGEUTIL/arrowtoscreen() */

   /*circle is drawn raound second of two points at angle=90 and radius*/
  
   int   ierr=0;
   float angle = 0;
   float fx0=0, fy0=0, fz0=0;
   float cmag=0,fx3=0,fy3=0,fz3=0,fx4=0,fy4=0,fz4=0;
   int   j=0;
   float fx1=(float)A[0];
   float fy1=(float)A[1];
   float fz1=(float)A[2];
   float fx2=(float)B[0];
   float fy2=(float)B[1];
   float fz2=(float)B[2];
   float dxs=0, dys=0, dzs=0;
   double dblx=0,dbly=0,dblz=0;


   if( (x2 == x1) && (y2 == y1) && (z2 == z1) )
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
   
   crossproduct( (double)(fx2-fx1),(double)(fy2-fy1),(double)(fz2-fz1)
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
   fx3 = (float)dblx*radius/cmag;
   fy3 = (float)dbly*radius/cmag;
   fz3 = (float)dblz*radius/cmag;
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
      C[0][0] = fx3;
      C[0][1] = fy3;
      C[0][2] = fz3;
   /*and can calculate the other radii*/
   for(j=1; j<SEGMENTS; j++)
   {
      /*rotate around 1-->2 for correct dihedral*/
      doaxisrot( &fx3,&fy3,&fz3
                ,(360/SEGMENTS)
                ,(double)x1,(double)y1,(double)z1
                ,(double)x2,(double)y2,(double)z2);
      /*store the radius vector*/
      C[j][0] = fx3;
      C[j][1] = fy3;
      C[j][2] = fz3;
   }
   ierr = 0;
   return(ierr);
}
/*___constructcircle()_______________________________________________________*/

