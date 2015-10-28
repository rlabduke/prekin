/*3456789_123456789_123456789_123456789_123456789_123456789_123456789_12345678*/
#include "PKIN.h" /*#include "prekin.h"*/

/***setuptexts()*************************************************************/
void   setuptexts(void)
{
  int    i;

  for(i=0 ; i<=255 ; i++)
  {
    if(texts[i] == '\n') texts[i] = ' ';
    if(texts[i] == '\r') texts[i] = ' ';
    if(texts[i] == '\0') texts[i] = ' ';
  }
  texts[255] = '\0';
  texts[4] = '0'; /* insure correct number of fields for sscanf */
  texts[9] = '0';
  texts[12] = 'x';
  texts[15] = 'x';
  texts[18] = 'x';
  texts[21] = 'x';
  texts[24] = 'x';  
  texts[27] = 'x';
  texts[30] = 'x';      
  texts[33] = 'x';
  texts[36] = 'x';
  texts[38] = '.';
  texts[39] = '.';
  texts[40] = '.';
  texts[43] = 'x';  /*960815*/
  texts[46] = 'x';  /*960815*/
  texts[49] = 'x';  /*960826*/
  texts[52] = 'x';  /*960826*/

}
/*___setuptexts()___________________________________________________________*/

/***cleanuptexts()***********************************************************/
void   cleanuptexts(void)
{
  int    i;

  for(i=0 ; i<=255 ; i++)
  {
    if(texts[i] == '\n') texts[i] = ' ';
    if(texts[i] == '\r') texts[i] = ' ';
    if(texts[i] == '\0') texts[i] = ' ';
  }
  texts[255] = '\0';
    /* insure correct number of fields for sscanf */
    /*all blank beginning number is set -999, negative limit*/
  if(texts[1] == ' '&&texts[2] == ' '&&texts[3] == ' '&&texts[4] == ' ') 
  { texts[1] = '-'; texts[2] = '9'; texts[3] = '9'; texts[4] = '9';}
    /*all blank ending number is set 9999, positive limit*/
  if(texts[6] == ' '&&texts[7] == ' '&&texts[8] == ' '&&texts[9] == ' ') 
  { texts[6] = '9'; texts[7] = '9'; texts[8] = '9'; texts[9] = '9';}
  if(texts[12] == ' ') texts[12] = 'x';
  if(texts[15] == ' ') texts[15] = 'x';
  if(texts[18] == ' ') texts[18] = 'x';
  if(texts[21] == ' ') texts[21] = 'x';
  if(texts[24] == ' ') texts[24] = 'x';  
  if(texts[27] == ' ') texts[27] = 'x';
  if(texts[30] == ' ') texts[30] = 'x';      
  if(texts[33] == ' ') texts[33] = 'x';
  if(texts[36] == ' ') texts[36] = 'x';
  if(texts[40] == ' ') texts[40] = '.';
  if(texts[43] == ' ') texts[43] = 'x';  /*960815*/
  if(texts[46] == ' ') texts[46] = 'x';  /*960815*/
  if(texts[49] == ' ') texts[49] = 'x';  /*960826*/
  if(texts[52] == ' ') texts[52] = 'x';  /*960826*/

}
/*___cleanuptexts()_________________________________________________________*/

/****dot3numbertexts()*******************************************************/
int    dot3numbertexts()
{
    int    i,ireturn,icomment,identifier,inum,innum;
    char c;
    
    inum = 0;
    innum = 0;
    icomment = 0;
    identifier = 0;

    for(i=0;i<256;i++)
    {
        c = texts[i];
        if(c == '<') 
        {
            icomment = 1;
            innum=0;
        }
        if(c == '{') 
        {
            identifier = 1;
            innum=0;
        }
        if(c == '}') identifier = 0;
        if(c == '>') icomment = 0;
        if(icomment==0 && identifier==0)
        {/*not in a comment or identifier, check for a number*/
            if(c=='0'||c=='1'||c=='2'||c=='3'||c=='4'||c=='5'||c=='6'||
               c=='7'||c=='8'||c=='9'||c=='.'||c=='-')
            {
                 if(innum==0) inum = inum+1; /*counter*/
                 innum = 1;/* actually got a digit of a number, keep it */
            }
            else innum = 0;
        }
        if(innum == 1) temps[i] = c;
        else temps[i] = ' ';
        if(texts[i]=='\0') break;
    }
    temps[i]='\0';
    if(inum == 3) ireturn = 1;
    else ireturn = 0;
    return(ireturn);
}

/*___dot3numbertexts()______________________________________________________*/

/***cleanupfoveatexts()******************************************************/
void   cleanupfoveatexts(void)
{
  int    i;

  for(i=0 ; i<=255 ; i++)
  {
    if(texts[i] == '\n') texts[i] = ' ';
    if(texts[i] == '\r') texts[i] = ' ';
    if(texts[i] == '\0') texts[i] = ' ';
  }
  texts[255] = '\0';
  if(texts[4] == ' ') texts[4] = '0'; 
     /* insure correct number of fields for sscanf */
  if(texts[12] == ' ') texts[12] = '0';
  if(texts[20] == ' ') texts[20] = '0';
  if(texts[28] == ' ') texts[28] = '0';
}
/*___cleanupfoveatexts()____________________________________________________*/

/***cleanupmarkertexts()*****************************************************/
void   cleanupmarkertexts(void)
{
  int    i;

  for(i=0 ; i<=255 ; i++)
  {
    if(texts[i] == '\n') texts[i] = ' ';
    if(texts[i] == '\r') texts[i] = ' ';
    if(texts[i] == '\0') texts[i] = ' ';
  }
  texts[255] = '\0';
  if(texts[4] == ' ') texts[4] = '0'; 
      /* insure correct number of fields for sscanf */
}
/*___cleanupmarkertexts()___________________________________________________*/

