/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/

#include "PKIN.h"

/****superpos()**************************************************************/
int superpos(void)
{

  double   EMM[4][4], V[4];
  double  ADJ[4][4], UU[4][4];
  double  UMEM[4][3], RSQMEM[3];
  int     I,J,IALT,icycle,isingular,iregular,K,L,M,N,KK;
  double  E0,UMOD,D;
  double  ALPHA,ALPHA2,VR,RSQ,DET,SUM,TRACE1,TRACE2,ADJMAX,CRT,ATEST;
  char    word[256];
/*  double  CHT,SHT,VMAX,HT,S[5][5];*/
/*  int     JMAX; */
  int     ireturn=1; /*presume successful, try to identify failure...*/

/* THIS ROUTINE  FINDS THE ORTHOGONAL MATRIX RELATING TWO VECTOR SETS,*/
/* ASSUMING UNIT WEIGHTS                                              */
/* C version, dcr 1992, uses only the iterative method,               */
/* avoiding fortran coded library routines of the Eigenvalue method   */
/* R. DIAMOND                                                         */
/* MRC LAB OF MOLECULAR BIOLOGY                                       */
/* HILLS ROAD                                                         */
/* CAMBRIDGE CB2 2QH                                                  */
/* ENGLAND                                                            */

/* REFERENCE ACTA CRYST A, (1988), A44, 211-216                  */
/* THIS VERSION DOES NOT INCLUDE THE CHIRALITY TEST DESCRIBED IN */
/* ACTA CRYST A, (1990), A46, 423                                */

/*  SUBROUTINE ROTATE(X,Y,NN,           !INPUTS             */
/*  1                 IM,JM,KM,CRIT,        !CONTROLS       */
/*  2                 R,U,THETA,E,XBAR,YBAR,    !OUTPUTS    */
/*  3                 NCYCLE,ISTAT)         !REPORTS        */

/* INPUTS*/

/* X(3,*) AND Y(3,*) ARE INPUT VECTOR SETS, (GIVEN IN CORRESPONDING ORDER) */
/* NN IS THE NUMBER OF POINTS IN EACH                                      */

/* CONTROLS*/

/* IF IM = 0 X AND Y ARE ASSUMED TO BE REFERRED TO THEIR CENTROIDS */
/*    ELSE   X AND Y ARE FIRST BROUGHT TO THEIR CENTROIDS          */
/* IF KM = 0 SILENT OPERATION                                      */
/*    ELSE   INTERMEDIATE RESULTS ARE SENT TO UNIT KM,             */
/*              which is now the textwindow                        */
/* CRIT IS A CRITERION DETERMINING ACCURACY, RELEVANT ONLY TO ITERATION.*/
/*    ITERATION TERMINATES IF DELTA ALPHA .LT. CRIT*ALPHA          */
/*          OR IF ALPHA .LT. CRIT                                  */
/*          OR IF TEN CYCLES ARE EXCEEDED                          */
/*    DETERMINANT OF (ALPHA*I-Q) IS TREATED AS ZERO IF THE SMALLEST*/
/*    EIGENVALUE OF (ALPHA*I-Q) IS LESS THAN CRIT*(MEAN OF THE REMAINING*/
/*    EIGENVALUES). THIS ALSO TERMINATES ITERATION.                */
/*    RECOMMENDED VALUE FOR CRIT IS 10**-6                         */

/* OUTPUTS*/

/* R(3,3) IS THE ORTHOGONAL MATRIX BEST SATISFYING X=R*Y                    */
/* U(3) CONTAINS THE DIRECTION COSINES OF THE AXIS                          */
/* THETA IS THE ROTATION ANGLE IN DEGREES                                   */
/* NOTE THAT -U AND -THETA TOGETHER DEFINE THE SAME ROTATION AS U AND THETA */
/* E IS SUM OVER I, J AND K OF (X(I,J)-R(I,K)*Y(K,J))**2                    */
/* XBAR AND YBAR ARE THE CENTROIDS OF THE INPUT X AND Y                     */
/* X AND Y ARE MODIFIED BY SUBTRACTION OF THESE IF IM .NE. 0.               */
/* PRESENT IMPLEMENTATION ASSUMES UNIT WEIGHTS                              */

/* REPORTS*/

/* NCYCLE IS THE NUMBER OF CYCLES USED IN ITERATION       */
/* ISTAT = 0 NORMAL TERMINATION                           */
/*  1 DET (ALPHA*I-Q) HAS BEEN TREATED AS ZERO            */
/*  2 ADJOINT(ALPHA*I-Q)*V ALSO VANISHES                  */
/*  3 ADJOINT VANISHES, IDENTITY RETURNED                 */
/*  4 ALPHA IS CONVERGING ON THE WRONG STATIONARY VALUE,  */
/*    IDENTITY RETURNED                                   */
/*  5 NN .LT. 1, NO ACTION                                */

/*Note extra added to index so original FORTRAN convention of counting*/
/*from 1 can be preserved                                */
/*  SUBROUTINE ROTATE(X,Y,NN,           !INPUTS          */
/*  1                 IM,JM,KM,CRIT,        !CONTROLS    */
/*  2                 R,U,THETA,E,XBAR,YBAR,    !OUTPUTS */
/*  3                 NCYCLE,ISTAT)         !REPORTS     */
/* above variables defined in .h file as common           */
/*float P[5][5];  also needed                             */
/* DIMENSION X(3,*),Y(3,*),S(4,4),P(4,4),EMM(3,3),V(3),         */
/* 1R(3,3),U(3),ADJ(3,3),UU(3,3),VALUE(4),VECTOR(4,4),WORK(20), */
/* 2RHO(4),XBAR(3),YBAR(3),UMEM(3,2),RSQMEM(2)                  */
/* define PI 3.141593 in .h file*/
    

    /*KM = 1;*/ /*report as it goes along*/
    KM = 0; /*not report as it goes along*/
    if(CRIT < 0.0 ) 
    {  
        CRIT = -CRIT;  /*CRIT=abs(CRIT);*/
    }
    if(KM != 0)
    {

/*c*/       sprintf(word,CRLF"entering superposition routine with CRIT == %15.6f",CRIT);
/*Tx*/      pkintextinsert(word);
    }
    ISTAT=0;
    if(NN < 1)
    {
        ISTAT=5;
        return(ireturn=0);
    }
    for(I=1 ; I<=3; I++)
    {
       XBAR[I]=0.0;
       YBAR[I]=0.0;
    }
    E0=0.0;

    IM = 1; /* so must find centroid itself*/

    
    if(IM == 0)
    {/*CENTERING already done*/
        for(I=1 ; I <= NN ; I++)
        {
            for(J=1 ; J <= 3 ; J++)
            {
                E0=E0+(X[J][I]-Y[J][I])*(X[J][I]-Y[J][I]);
            }
        }
    }/*CENTERING already done*/
    else
    {/*REFER TO CENTROIDS*/
        /* REFER TO CENTROIDS*/
        for(I=1; I<=3; I++)
        {
            for(L=1; L<=NN; L++)
            {
                XBAR[I]=XBAR[I]+X[I][L];
                YBAR[I]=YBAR[I]+Y[I][L];
            }
            XBAR[I]=XBAR[I]/NN;
            YBAR[I]=YBAR[I]/NN;
        }
        if(KM != 0)
        {
/*c*/      sprintf(word,CRLF"XBAR: %f, %f, %f,  YBAR:  %f, %f, %f"
                      ,XBAR[1],XBAR[2],XBAR[3],YBAR[1],YBAR[2],YBAR[3]);
/*Tx*/     pkintextinsert(word);
/*c*/      sprintf(word,CRLF"X AND Y ARE");
/*Tx*/     pkintextinsert(word);
        }
/*  BRING TO CENTROID AND EVALUATE INITIAL RESIDUAL E0 */
        for(I=1; I<=NN; I++)
        {
            for(J=1; J<=3; J++)
            {
                 X[J][I]=X[J][I]-XBAR[J];
                 Y[J][I]=Y[J][I]-YBAR[J];
                 E0=E0+(X[J][I]-Y[J][I])*(X[J][I]-Y[J][I]);
            }
            if(KM != 0)
            {
                /*WRITE(KM,56)(X(J,I),J=1,3),(Y(J,I),J=1,3)*/
                /*FORMAT(3F10.5,5X,3F10.5)*/
/*c*/           sprintf(word,CRLF"%10.5f,%10.5f,%10.5f,    %10.5f,%10.5f,%10.5f"
                   ,X[1][I],X[2][I],X[3][I],Y[1][I],Y[2][I],Y[3][I]);
/*Tx*/          pkintextinsert(word);
            }
        }
    }/*REFER TO CENTROIDS*/

/* CONSTRUCT M*/

    for(I=1; I<=3; I++)
    {
        for(J=1; J<=3; J++)
        {
            SUM=0.0;
            for(K=1; K<=NN; K++)
            {
                SUM=SUM+Y[I][K]*X[J][K];
            }
            EMM[I][J]=SUM;
        }
    }

/* CONSTRUCT P*/

    SUM=0.0;
    for(I=1; I<=3; I++)
    {
        SUM=SUM+EMM[I][I];
    }
    for(I=1; I<=3; I++)
    {
        for(J=1; J<=3; J++)
        {
            P[I][J]=(float)(EMM[I][J]+EMM[J][I]);
        }
        P[I][I]=(float)(P[I][I]-2.0*SUM);
    }
    for(I=1; I<=3; I++)
    {
        J=I+1;
        if(J>=4)
        {
          J=1;
        }
        K=J+1;
        if(K>=4)
        {
          K=1;
        }
        V[I]=EMM[J][K]-EMM[K][J];
        P[I][4]=(float)V[I];
        P[4][I]=(float)V[I];
    }
    P[4][4]=0.0;
    
    if(KM != 0)
    {/*KM != 0: OUTPUT M AND P*/
       /* OUTPUT M AND P*/
       for(I=1;I<=3;I++)
       {
/*c*/      sprintf(word,CRLF" M[%d][J]: %15.5f, %15.5f, %15.5f"
                      ,I,EMM[I][1],EMM[I][2],EMM[I][3]); 
/*Tx*/     pkintextinsert(word);
       }
/*c*/  sprintf(word,CRLF);
/*Tx*/ pkintextinsert(word);
       for(I=1;I<=3;I++)
       {
/*c*/      sprintf(word,CRLF" P[%d][J]: %15.5f, %15.5f, %15.5f, %15.5f"
                      ,I,P[I][1],P[I][2],P[I][3],P[I][4]);
/*Tx*/     pkintextinsert(word);
       }
/*c*/  sprintf(word,CRLF);
/*Tx*/ pkintextinsert(word);

    }/*KM != 0: OUTPUT M AND P*/
    
    /*-----------------------------------------------------------*/
    /*  ITERATION method IS USED, section below                  */

    NCYCLE=-1;
    ALPHA=E0/2.0;
    RSQMEM[1]=-1.0;
    RSQMEM[2]=-1.0;
    IALT=0;
    isingular = 0; /*presume matrix will not be singular */
    iregular = 1;  /*presume well behaved matrix, general axis, etc. */
    icycle = 1;    /*icycle true until internal conditions set it false*/
    while(icycle)  
    {/*while(icycle) loop */
        /* BEGIN CYCLE*/

        NCYCLE=NCYCLE+1;
        IALT=IALT+1;
        if(IALT >= 3)
        {
          IALT=1;
        }
        /* FORM (ALPHA*I-Q) AND ITS TRACE*/

        TRACE1=0.0;
        for(I=1; I<=3; I++)
        {
            for(J=1; J<=3; J++)
            {
                EMM[I][J]=-P[I][J];
            }
            EMM[I][I]=EMM[I][I]+ALPHA;
            TRACE1=TRACE1+EMM[I][I];
        }
        /* FORM ADJOINT OF(ALPHA*I-Q), ITS TRACE AND THE DETERMINANT*/

        TRACE2=0.0;
        DET=0.0;
        if(KM != 0)
        {
/*c*/       sprintf(word,CRLF" (ALPHA*I-Q), ADJOINT AND DETERMINANT");
/*Tx*/      pkintextinsert(word);
        }
        ADJMAX=0.0;
        for(I=1; I<=3; I++)
        {
            J=I+1;
            if(J >= 4)
            {
              J=1;
            }
            K=J+1;
            if(K >= 4)
            {
              K=1;
            }
            DET=DET+EMM[I][1]*(EMM[J][2]*EMM[K][3]-EMM[K][2]*EMM[J][3]);
            for(L=1; L<=3; L++)
            {
                M=L+1;
                if(M >= 4)
                {
                  M=1;
                }
                N=M+1;
                if(N >= 4)
                {
                  N=1;
                }
                ADJ[L][I]=EMM[J][M]*EMM[K][N]-EMM[J][N]*EMM[K][M];
                /*ADJMAX=max(ADJMAX,abs(ADJ[L][I]));*/
                ATEST = ADJ[L][I];
                if( ATEST < 0.0 ) 
                {
                  ATEST = -ATEST;
                }
                if( ADJMAX < ATEST ) 
                {
                  ADJMAX = ATEST;
                }
               
            }
            TRACE2=TRACE2+ADJ[I][I];
        
            if(KM != 0)
            {
/*c*/           sprintf(word,
                   CRLF"M[%d][J]      %13.5f, %13.5f, %13.5f"
                   CRLF"ADJ[L][%d]    %13.5f, %13.5f, %13.5f"
                   CRLF"DET           %13.5f"
                   ,I,EMM[I][1],EMM[I][2],EMM[I][3]
                   ,I,ADJ[1][I],ADJ[2][I],ADJ[3][I]
                   ,DET);
/*Tx*/          pkintextinsert(word);
            }
        }
        
        /*COMPARE SMALLEST EIGENVALUE WITH */
        /* CRIT*(MEAN OF OTHER TWO EIGENVALUES)*/

        CRT=0.5*CRIT*TRACE1*TRACE2;
        
        if(KM != 0)
        {
/*c*/       sprintf(word,CRLF"CRT == CRITERION FOR DETERMINANT == %15.5f",CRT);
/*Tx*/      pkintextinsert(word);
/*c*/       sprintf(word,CRLF"CRT=0.5*CRIT*TRACE1*TRACE2 == 0.5*%15.5f*%15.5f*%15.5f"
                  ,CRIT,TRACE1,TRACE2);
/*Tx*/      pkintextinsert(word);
        }
        if(DET < -CRT*1000.0)
        {
            /* CONVERGING ON WRONG STATIONARY VALUE*/
            ireturn = 0;
            if(KM != 0)
            {
/*c*/           sprintf(word,CRLF"DET < -CRT*1000.0, %15.5f < %15.5f "
                              "ITERATION CONVERGING ON WRONG "
                             "STATIONARY VALUE, IDENTITY RETURNED",DET,-CRT*1000.0);
/*Tx*/          pkintextinsert(word);
            }
            ISTAT=4;
            for(I=1; I<=3; I++) 
            {
               U[I]=0.0;
            }
            icycle = 0; /*do no more cycle stuff */
            /*this is, in effect, old 148, old 150, going on to */
            /* regular processing at old 139 since isingular == 0 */
        }
        if(!icycle) 
        {
            break; /*break out of cycle loop*/
        }
        
        if(DET <  CRT)
        {
            /* SINGULAR CASE  THETA = 180 DEGREES*/
            ISTAT=1;
            if(KM != 0)
            {
/*c*/           sprintf(word,CRLF"DET <  CRT, (ALPHA*I-Q) IS SINGULAR, "
                             "IMPLYING 180 DEG ROTATION");
/*Tx*/          pkintextinsert(word);
            }
            icycle = 0; /*do no more cycle stuff*/
            isingular = 1; /*this will, in effect, goto old 135 */
        }
        if(!icycle) 
        {
            break; /*break out of cycle loop*/
        }
        /* FORM ROTATION VECTOR U AND NEW ALPHA*/

        VR=0.0;
        RSQ=0.0;
        for(I=1; I<=3; I++)
        {
            SUM=0.0;
            for(J=1; J<=3; J++)
            {
                SUM=SUM+ADJ[I][J]*V[J];
            }
            U[I]=SUM/DET;
            VR=VR+U[I]*V[I];
            RSQ=RSQ+U[I]*U[I];
        }
        ALPHA2=(VR+ALPHA*RSQ)/(1.0+RSQ);
        if(KM != 0)
        {
/*c*/       sprintf(word,
                   CRLF"U %13.5f, %13.5f, %13.5f"
                   CRLF"V %13.5f, %13.5f, %13.5f"
                   CRLF"VR %13.5f, RSQ %13.5f, ALPHA2 %13.5f"
                   ,U[1],U[2],U[3]
                   ,V[1],V[2],V[3]
                   ,VR,RSQ,ALPHA2);
/*Tx*/      pkintextinsert(word);
        }

        /* TEST FOR COMPLETION*/
        
        ATEST = ALPHA-ALPHA2;
        if ( ATEST < 0.0) 
        {
            ATEST = -ATEST;
        }
        if( ATEST < ALPHA*CRIT
            || ALPHA2 < CRIT
            || NCYCLE > 10     )
        {    
            icycle=0; /* NORMAL TERMINATION*/
        }
        else
        {/*maybe not ready to terminate*/
            ALPHA=ALPHA2;
            if(KM != 0)
            {
/*c*/           sprintf(word,CRLF" ALPHA, VR, RSQ, DET, AND NCYCLE"
                   CRLF" %15.5f, %15.5f, %15.5f, %15.5f, %d"
                   ,ALPHA,VR,RSQ,DET,NCYCLE);
/*Tx*/          pkintextinsert(word);
            }
            /* TEST FOR OSCILLATION*/
            ATEST = RSQ-RSQMEM[IALT];
            if ( ATEST < 0.0) 
            {
                ATEST = -ATEST;
            }
            /*if(abs(RSQ-RSQMEM[IALT]) < CRIT*RSQ)*/
            if( ATEST < CRIT*RSQ)
            {/*OSCILLATION TERMINATION*/
                /* OSCILLATION, RESULT IS MEAN OF LAST TWO CYCLES*/

                for(I=1; I<=3; I++)
                {
                    U[I]=0.5*(UMEM[I][1]+UMEM[I][2]);
                }
                if(KM != 0)
                {
/*c*/               sprintf(word,CRLF" ITERATION TERMINATED BY ALTERNATION");
/*Tx*/              pkintextinsert(word);
                }
                icycle = 0;
            }/*OSCILLATION TERMINATION*/
            else
            {/*going to do another cycle*/
                RSQMEM[IALT]=RSQ;
                for(I=1; I<=3; I++)
                {
                    UMEM[I][IALT]=U[I];
                }
                /* END OF this CYCLE*/
                /*and icycle == 1 */ 
            }/*going to do another cycle*/    
        }/*maybe not ready to terminate*/
        /* NORMAL TERMINATION of interation cycles, */
    }/*while(icycle) loop */
    
    if(KM != 0) /*c version allows this print choice under all conditions */
    {
/*c*/   sprintf(word,CRLF" ALPHA, U AND NCYCLE ARE"
                   CRLF" %15.5f"
                   CRLF" %15.5f, %15.5f, %15.5f"
                   CRLF" %d"
                   ,ALPHA
                   ,U[1],U[2],U[3]
                   ,NCYCLE);
/*Tx*/  pkintextinsert(word);
    }

    /* cycles done, check for special cases of termination */
    if(isingular)
    {/*isingular==1 */   /*SINGULAR CASE*/
        /*old 135*/ /*enters old 135 with KK=0 */
        /* cycles until KK==4 or D large enough to go regular case */
        KK = 0;
        while(KK < 4)  /*(old 135)*/
        {/*KK<4 cycles*/
            D=0.0; 
            for(I=1; I<=3; I++)
            {
                SUM=0.0;
                for(J=1; J<=3; J++) 
                {
                    SUM=SUM+ADJ[I][J]*V[J];
                }
                U[I]=SUM;
                D=D+SUM*SUM;
            }
            if(D < CRIT*ADJMAX*ADJMAX*ADJMAX) /*(D.LT.CRIT*ADJMAX**3)*/
            {/*(D < CRIT*ADJMAX**3)*/
                /* PRINCIPAL AXIS CASE*/
                if(KM != 0)
                {
/*c*/               sprintf(word,CRLF" ADJOINT TIMES VECTOR V ALSO VANISHES,"
                           " IMPLYING ROTATION AXIS IS AN EIGENVECTOR OF Q");
/*Tx*/              pkintextinsert(word);
                }
                KK=KK+1;
                if(KK >= 4)
                {
                    /* ADJOINT IS NULL, RETURN IDENTITY*/
                    ireturn = 0;
                    if(KM != 0)
                    {
/*c*/                   sprintf(word,CRLF" ADJOINT VANISHES, RETURN IDENTITY");
/*Tx*/                  pkintextinsert(word);
                    }
                    ISTAT=3;
                    for(I=1; I<=3; I++) 
                    {
                        U[I]=0.0;
                    }
                    iregular = 1; /*reset here for safety*/
                    /* effective: GO TO old 139 */
                    /* CONSTRUCT R using general routine*/
                    
                    /*now break out of this singular handling routine*/
                    /*and go CONSTRUCT R using the general routine*/
                    /* now have to break out of this imbedded loop */
                    /*for robustness, given this logic, use a goto */
                    goto process;
                  
                }
                for(I=1;I<=3;I++) V[I]=0.0;
                V[KK]=sqrt(ADJMAX);
                ISTAT=2;
                /*continue KK cycles, if any left (old 135)*/
            }/*(D < CRIT*ADJMAX**3)*/
            else 
            {/*D >= CRIT*ADJMAX**3 */
                break; 
            }
        }/*KK<4 cycles*/
        
        /*either KK==4 or D large enough to proceed anyway*/
        /*for this case, construct R, and set THETA=180*/
        for(I=1; I<=3; I++)
        {
            for(J=1; J<=3; J++)
            {
                R[I][J]=2.0*U[I]*U[J]/D;
            }
            R[I][I]=R[I][I]-1.0;
        }
        THETA=180.0;
        UMOD=sqrt(D);
        ISTAT=1;
        /* R matrix constructed, and THETA set = 180 */
        iregular = 0; /* will skip regular construction of R matrix */
        /* effective GO TO old 131 */
        /*scale U to make direction cosines of rotation axis*/
    }/*isingular==1 */

    process:  ;

    if(iregular)
    {/* iregular == 1 , regular conditions */
        /* old 139 */ /* CONSTRUCT R by regular proceedure*/

        D=1.0;
        for(I=1; I<=3; I++)
        {
            for(J=1; J<=3; J++)
            {
                UU[I][J]=U[I]*U[J];
            }
            D=D+UU[I][I];
        }
        for(I=1; I<=3; I++)
        {
            J=I+1;
            if(J >= 4)
            {
              J=1;
            }
            K=J+1;
            if(K >= 4)
            {
              K=1;
            }
            R[I][I]=(1.0+UU[I][I]-UU[J][J]-UU[K][K])/D;
            R[I][J]=2.0*(UU[I][J]-U[K])/D;
            R[J][I]=2.0*(UU[I][J]+U[K])/D;
        }

        /* ESTABLISH DIRECTION COSINES AND ANGLE*/

        UMOD=sqrt(D-1.0);
        THETA=360.0*atan(UMOD)/PI;
        if(UMOD == 0.0)
        {
          UMOD=1.0;
        }
    }/* iregular == 1 , regular conditions */

    /*all conditions eventually come through here...*/
    /*old 131 */ /*scale U to make direction cosines of rotation axis*/ 
    for(I=1; I<=3; I++) 
    {
        U[I]=U[I]/UMOD;
    } 
    E=E0-2.0*ALPHA;
    
    if(KM != 0)
    {
/*c*/   sprintf(word,CRLF" DIRECTION COSINES AND ROTATION ANGLE"
                         CRLF"    %10.5f, %10.5f, %10.5f, %10.3f"
                      ,U[1],U[2],U[3],THETA);
/*Tx*/  pkintextinsert(word);
/*c*/   sprintf(word,CRLF" R");
        
/*Tx*/  pkintextinsert(word);
        for(I=1; I<=3; I++)
        {
          /* R(3,3) IS THE ORTHOGONAL MATRIX BEST SATISFYING X=R*Y  */
          /* U(3) CONTAINS THE DIRECTION COSINES OF THE AXIS        */
          /* THETA IS THE ROTATION ANGLE IN DEGREES                 */
          /*-----------------------------------------------------------*/   

/*c*/       sprintf(word,CRLF
                         " %10.5f, %10.5f, %10.5f"
                      ,R[I][1],R[I][2],R[I][3]);
/*Tx*/      pkintextinsert(word);
        }
    }
    return(ireturn);
}
/*___superpos()_____________________________________________________________*/

