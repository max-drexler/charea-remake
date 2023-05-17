/* 5/23/2022 I went through the McIDAS sensor list and added several more sensors that have been added --DPR */
/* 8/09/2018 Noticed new entries for GOES-17-GOES19 and NOAA20-NOAA23 so I added those --DPR */
/* 2/15/2018 FIrst found a typoe for meteosat-11 then I changed it to met11 to be in line with our other met data */
 /* 2/13/2018 fixed transpostion of himawari-9 and himawaricast8 which was also mislabeled as "himawaricast" */
/* 2/8/2018 added goes-16-l2 products and found trmm wasn't listed -DPR */
/* 2/2/208 added himawari-9, and himawaricast9 */
/* 9/12/2016 added insat3d-img, insat3d-snd, landsat1-8,and himawaricast8*/
/* 6/5/2015 changed mtsat-3 to himawari-8 */
/* 9/19/2013 updated Sensor Table (saSensorTab) with
   more sats */
/* 6/22/2006 updated the Sensor Table (saSensorTab) with
   a whole slew of new sats */
/* 10/31/2001 re-added some code to check for little endian
   stuff that somehow got removed over the ages. --dpr */
/* 5/30/2001 made a change to check for more then 32 bands 
   if iAreaDir[2] is greater than or equal to 101, which
   is terra and above --dpr */

/*Probably around version 4.0 added more modis support
  and changed output if charea itself is just run with no file
  to usage 5/16/2001 */
/*charea.c reads the areadir of AREA format files and echos
 back a name in a sensor_band_ccyy_jday_time format.  Supports
 gregorian dates with the -g flag and returns name of month with
 -m flag */
  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BAND1 1 
#define BAND2 2
#define BAND3 3
#define BAND4 4
#define BAND5 5
#define BAND6 6
#define BAND7 7
#define BAND8 8
#define BAND9 9
#define BAND10 10
#define BAND11 11
#define BAND12 12
#define BAND13 13
#define BAND14 14
#define BAND15 15
#define BAND16 16
#define BAND17 17
#define BAND18 18
#define BAND19 19
#define BAND20 20
#define BAND21 21
#define BAND22 22
#define BAND23 23
#define BAND24 24
#define BAND25 25
#define BAND26 26
#define BAND27 27
#define BAND28 28
#define BAND29 29
#define BAND30 30
#define BAND31 31
#define BAND32 32
#define BAND33 1
#define BAND34 2
#define BAND35 3
#define BAND36 4
#define BAND37 5
#define BAND38 6

static int iaGregDayTab[2][13] =
{
   {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
   {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static int iaJulDayTab[2][13] =
{
   {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
   {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

  static char *saMonthName[] =
   {
      "Illegal month",
      "jan", "feb", "mar",
      "apr", "may", "jun",
      "jul", "aug", "sep",
      "oct", "nov", "dec"
   };



int get_year(int jday);
int get_jday(int jday);
int cmp_bits(unsigned long iBands);
int cmp_bits2(unsigned long iBands);
int get_month( int jday, int iLeap);
int get_greg(int jday, int iLeap);
int swap_int_4(int *);

static char *saSensorTab[]=
   {
/*0-5    */      "Derived_Data", "Test_Pattern", "graphics",      "Misc",       "PDUS_MET_VIS", "PDUS_MET_IR",
/*6-11   */      "PDUS_MET_WV",  "Radar",        "Misc_Aircraft", "Raw_MET",    "composite",    "blank",
/*12-17  */      "GMS_VIS",      "GMS_IR",       "ATS6-VIS",      "ATS6-IR",    "sms1-vis",     "sms1-ir",
/*18-23  */      "sms2-vis",     "sms2-ir",      "goes1-vis",     "goes1-ir",   "goes2-vis",    "goes2-ir",
/*24-29  */      "goes3-vis",    "goes3-ir",     "goes4-vis",     "goes4-ir",   "goes5-vis",    "goes5-ir",
/*30-35  */      "goes6-vis",    "goes6-ir",     "goes7-vis",     "goes7-ir",   "fy-2b",        "fy-2c",
/*36-41  */      "fy-2d",        "fy-2e",        "fy-2f",         "fy-2g",      "fy-2h",        "tirosn",
/*42-47  */      "noaa6",        "noaa7",        "noaa8",         "noaa9",      "venus",        "voyager1",
/*48-53  */      "voyager2",     "galileo",      "hubble",        "met8",       "met9",         "met10",
/*54-59  */      "met3",         "met4",         "met5",          "met6",       "met7",         "blank",
/*60-65  */      "noaa10",       "noaa11",       "noaa12",        "noaa13",     "noaa14",       "noaa15",
/*66-71  */      "noaa16",       "noaa17",       "noaa18",        "noaa19",     "goes8",        "goes8sndr",
/*72-77  */      "goes9",        "goes9sndr",    "goes10",        "goes10sndr", "goes11",       "goes11sndr",
/*78-83  */      "goes12",       "goes12sndr",   "erbe",          "blank",      "gms4",         "gms5",
/*84-89  */      "mtsat-1r",     "mtsat-2",      "himawari-8",       "dmspf8",     "dmspf9",       "dmspf10",
/*90-95  */      "dmspf11",      "dmspf12",      "dmspf13",       "dmspf14",    "dmspf15",      "fy1b",
/*96-101 */      "fy1c",         "fy1d",         "blank",         "blank",      "blank",        "terra",         
/*102-107*/      "terra-cld",    "terra-geo",    "terra-aer",     "blank",      "terra-top",    "terra-atm",
/*108-113*/      "terra-gus",    "terra-ret",    "blank",         "aqua",       "aqua-cld",     "aqua-geo",     
/*114-119*/      "aqua-aer",     "blank",        "aqua-top",      "aqua-atm",   "aqua-gus",     "aqua-ret",
/*120-125*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*126-131*/      "blank",        "blank",        "terra-sst",     "terra-lst",  "blank",        "blank",
/*132-137*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*138-143*/      "aqua-sst",     "aqua-lst",     "blank",         "blank",      "blank",        "blank",
/*144-149*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*150-155*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*156-161*/      "blank",        "blank",        "blank",         "blank",      "terra-ndvi",   "terra-cref",
/*162-167*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*168-173*/      "blank",        "blank",        "aqua-ndvi",     "aqua-cref",  "blank",        "blank",
/*174-179*/      "ewsg1",        "ewsg1sndr",    "ewsg2",         "ewsg2sndr",  "ewsg3",        "ewsg3sndr",
/*180-185*/      "goes13",       "goes13sndr",   "goes14",        "goes14sndr", "goes15",       "goes15sndr",       
/*186-191*/      "goes16",       "goes16-l2",    "goes17",        "goes17-l2",  "goes18",       "goes18-l2",
/*192-197*/      "goes19",       "goes19-l2",    "blank",         "dmsp_f16",   "dmsp_f17",     "blank",
/*198-203*/      "blank",        "blank",        "airs-l1b",      "blank",      "blank",        "blank",
/*204-209*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*210-215*/      "amsre-l1b",    "amsre-rain",   "blank",         "blank",      "blank",        "blank",
/*216-221*/      "amsu_a-lwp",   "blank",        "blank",         "blank",      "trmm",         "gms1",
/*222-227*/      "gms2",         "gms3",         "blank",         "blank",      "blank",        "blank",
/*228-233*/      "blank",        "blank",        "kalpana1",      "insat3d-img","insat3d-snd",  "blank",
/*234-239*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*240-245*/      "metopa",       "metopb",       "metopc",        "blank",      "blank",        "blank",
/*246-251*/      "blank",        "blank",        "blank",         "blank",      "coms1",        "blank",
/*252-257*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*258-263*/      "blank",        "blank",        "blank",         "landsat1",   "landsat2",     "landsat3",
/*264-269*/      "landsat4",     "landsat5",     "landsat6",      "landsat7",   "landsat8",     "blank",
/*270-275*/      "blank",        "blank",        "blank",         "blank",      "blank",        "fy3a",
/*276-281*/      "fy3b",         "fy3c",         "blank",         "blank",      "blank",        "blank",
/*282-287*/      "blank",        "blank",        "blank",         "blank",      "himawaricast8","himawari-9",  
/*288-293*/      "himawaricast9","himawaricast8/9","blank",       "blank",      "blank",        "blank",
/*294-299*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*300-305*/      "npp-viirs",    "noaa20",       "noaa21",        "noaa22",     "noaa23",       "blank",
/*306-311*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*312-317*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*318-223*/      "blank",        "blank",        "snppsdr",       "noaa20sdr",  "noaa21sdr",    "noaa22sdr",
/*324-329*/      "noaa23sdr",    "snppedr",      "noaa20edr",     "noaa21edr",  "noaa22edr",    "noaa23edr",
/*330-335*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*336-341*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*342-347*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*348-353*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*354-359*/      "met11",        "blank",        "blank",         "blank",      "blank",        "blank",
/*360-365*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*366-371*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*372-377*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*378-383*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*384-389*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*390-395*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*396-401*/      "blank",        "blank",        "blank",         "blank",      "s_pole_comp",  "n_pole_comp",
/*402-407*/      "blank",        "blank",        "blank",         "blank",      "blank",        "blank",
/*408-413*/      "blank",        "blank",        "megha-tropic",  "blank",      "blank",        "blank",

   };

static char *saBitMap[]=
   {
      "0", "1", "2", "12", "3", "13", "23", "123", "4", "14", "24", "124","34", "134", "234", "1234", "5",
      "15", "25", "125", "35", "135", "235", "1235", "45", "145", "245", "1245","345", "1345", "2345", "12345"
   };
   char sBands[80]="-";
   int iMultiBand=0;
   int iModisFlag=0;
   



int main (int argc, char *argv[])
{

   FILE *fp_in;
   int iAreaDir[64];
   int i=0;
   char sErrorOut[80];
   char cToken;
   int iYear, iJday, iTime, iBand, iTotalArgs, iMonth=0;
   int iLeap, iGreg=0;
   static char sSndr[]="sndr";

   while (--argc > 0 && (*++argv)[0] == '-')
   {
      while ( cToken = *++argv[0])
      {
         switch (cToken)
         {
            case 'm':
             
               iMonth=1;
               break;

            case 'g':

               iGreg=1;
               break;
            
            default:
    
               printf("\nusage:charea [-mg] AREA0001\n");
               exit(2);
            
               break;
         }   
            
      }
   }
         

       

   if (( fp_in = fopen (argv[0], "r")) == NULL)
   {
      printf("\nusage:charea [-mg] AREA0001\n");
      exit(1);
   }

   fread(iAreaDir, sizeof(int), 64, fp_in);
   if ( iAreaDir[1] != 4 ) /*Test to see if bits are flipped */
   {
      for( i=0;i<64;++i)
      {
         swap_int_4(&iAreaDir[i]);
      }
   }


   iYear=(get_year(iAreaDir[3]));
   iJday=( iAreaDir[3] % 1000); 
   iTime=( iAreaDir[4] / 100);
   iLeap= iYear%4 == 0 && iYear%100 !=0 || iYear%400 == 0;
   if(strstr(saSensorTab[iAreaDir[2]],sSndr) != NULL)
   {
      if (( iMonth == 1 ) && ( iGreg == 0 ))
      {   
         printf("%s_%s_%04d_%03d_%04d",saSensorTab[iAreaDir[2]],saMonthName[get_month(iJday,iLeap)],iYear,iJday,iTime);
      }
      else if (( iGreg == 1 ) && ( iMonth == 0 ))
      {
         printf("%s_%04d_%02d_%02d_%04d",saSensorTab[iAreaDir[2]],iYear,get_month(iJday,iLeap),get_greg(iJday,iLeap),
                     iTime);
      }
      else if (( iGreg == 1 ) && ( iMonth == 1 ))
      {
         printf("%s_%s_%04d_%02d_%02d_%04d",saSensorTab[iAreaDir[2]],saMonthName[get_month(iJday,iLeap)],
              iYear,get_month(iJday,iLeap),get_greg(iJday,iLeap),iTime);
      }
      else
      {
         printf("%s_%04d_%03d_%04d",saSensorTab[iAreaDir[2]],iYear,iJday,iTime);
      }
   }
   else
   {
      if((iAreaDir[2] >= 101) && (iAreaDir[2] <= 138))
      {
         iModisFlag=1;
      }
      cmp_bits(iAreaDir[18]);
      if ((iAreaDir[19] != 0) && ( iModisFlag == 1 )) 
         
      {
         cmp_bits2(iAreaDir[19]);
      }
      if(( iMonth == 1 ) && ( iGreg == 0 ))
      {
         printf("%s_%s_%s_%04d_%03d_%04d",saSensorTab[iAreaDir[2]],saMonthName[get_month(iJday,iLeap)],sBands,iYear,
            iJday,iTime);
      }
      else if (( iGreg == 1 ) && ( iMonth == 0 ))
      {
         printf("%s_%s_%04d_%02d_%02d_%04d",saSensorTab[iAreaDir[2]],sBands,iYear,get_month(iJday,iLeap),
                   get_greg(iJday,iLeap),iTime);
      }
      else if (( iGreg == 1 ) && ( iMonth == 1 ))
      {
         printf("%s_%s_%s_%04d_%02d_%02d_%04d",saSensorTab[iAreaDir[2]],saMonthName[get_month(iJday,iLeap)],
                      sBands,iYear,get_month(iJday,iLeap),get_greg(iJday,iLeap),iTime);
      }
      else
      {
         printf("%s_%s_%04d_%03d_%04d",saSensorTab[iAreaDir[2]],sBands,iYear,iJday,iTime);
      }
   }
/*   printf("\n%s",sBands); */
   printf("\n");
   return(0);
}

int get_month(int jday,int leap)
{
  int count=0;
  for( count=0; count <=12; ++count)
  {
    if ( jday >  iaJulDayTab[leap][count] && jday <= iaJulDayTab[leap][count + 1])
    {
       return(count + 1 );
    }
  }
}

int get_year(int jday)
{
   int iYear, ccyy;

   iYear=jday / 1000;
   if(iYear >= 100)
   {
      return ((iYear % 100) + 2000);
   }
   else
   {
      return(1900 + iYear);
   }
}
int get_greg(int jday, int iLeap)
{
   int i;
   int date;
   
   for(i=0; i <= 12; ++i)
   {
     if (( jday > iaJulDayTab[iLeap][i] ) && ( jday <= iaJulDayTab[iLeap][i+1]))
     {
        date=jday - iaJulDayTab[iLeap][i];
     }
   }
   return(date);
}
int swap_int_4(int *tni4)                  /* 4 byte signed integers   */
{
 *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));
}

cmp_bits2(unsigned long iBands)
{
   int i=0,y=0;
   unsigned long mask =0x00000001;
   char sSort[80];

   for (i=1;i<7;++i)
   {

      if (mask & iBands)
      {
         switch(i)
         {
            case BAND33:
            {
               if (iMultiBand > 0 )
               {
                  strcat(sBands,"33");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"33");
                  ++iMultiBand;
                  break;
               }
            }
            case BAND34:
            {
               if (iMultiBand > 0 )
               {
                  strcat(sBands,"34");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"34");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND35:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"35");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"35");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND36:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"36");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"36");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND37:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"37");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"37");
                  ++iMultiBand;
                  break;
               }

           }
           case BAND38:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"38");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"38");
                  ++iMultiBand;
                  break;
               }
           }
           default:
           {
              continue;
           }
         } /*switch*/
      } /*if*/
/*DEBUG: where 'corrupt' was created      */
      if(iModisFlag > 0 && iMultiBand == 38 )
      {
         strcpy(sBands,"all");
         return;
      } 

      mask <<=1;

   }  /*for*/
}

cmp_bits(unsigned long iBands)
{
   int i=0,y=0;
   unsigned long mask =0x00000001;
   char sSort[80];

   for (i=1;i<39;++i)
   {
      if (mask & iBands)
      {
         switch(i)
         {
            case BAND1:
            {
               if (iMultiBand > 0 )
               {
                  strcat(sBands,"1");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"1");
                  ++iMultiBand;
                  break;
               }
            }
            case BAND2:
            {
               if (iMultiBand > 0 )
               {
                  strcat(sBands,"2");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"2");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND3:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"3");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"3");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND4:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"4");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"4");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND5:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"5");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"5");
                  ++iMultiBand;
                  break;
               }

           }
           case BAND6:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"6");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"6");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND7:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"7");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"7");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND8:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"8");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"8");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND9:

           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"9");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"9");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND10:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"10");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"10");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND11:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"11");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"11");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND12:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"12");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"12");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND13:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"13");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"13");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND14:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"14");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"14");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND15:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"15");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"15");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND16:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"16");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"16");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND17:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"17");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"17");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND18:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"18");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"18");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND19:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"19");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"19");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND20:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"20");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"20");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND21:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"21");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"21");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND22:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"22");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"22");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND23:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"23");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"23");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND24:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"24");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"24");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND25:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"25");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"25");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND26:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"26");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"26");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND27:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"27");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"27");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND28:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"28");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"28");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND29:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"29");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"29");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND30:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"30");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"30");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND31:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"31");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"31");
                  ++iMultiBand;
                  break;
               }
           }
           case BAND32:
           {
              if (iMultiBand > 0 )
               {
                  strcat(sBands,"32");
                  ++iMultiBand;
                  break;
               }
               else
               {
                  strcpy(sBands,"32");
                  ++iMultiBand;
                  break;
               }
           }



           default:
           {
              continue;
           }
         } /*switch*/
      } /*if*/

/*DEBUG: Where corrupt was put in string */     
      if(iModisFlag > 0 && iMultiBand == 38)
      {
         strcpy(sBands,"all");
         return;
      } 

      mask <<=1;
   }  /*for*/

}
