/********************************************************************/
/*	trace_paw.cpp						    */
/*		last updated: 10/02/09 DTM	     	       	    */
/*			       					    */
/********************************************************************/
#include <cmath>
#include <cstdio>
#include <cstdlib>

// pixie includes
#include "PixieInterface.h"

// cernlib includes
#include "cfortran.h"
#include "hbook.h" 

const size_t PAWC_SIZE = 900000;

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct {
    float h[PAWC_SIZE];
  } PAWC_DEF;
  
#define PAWC COMMON_BLOCK(PAWC,pawc)
  COMMON_BLOCK_DEF(PAWC_DEF, PAWC);

  PAWC_DEF pawc_;
#ifdef __cplusplus
}
#endif

int main(int argc, char *argv[])
{
  const size_t TRACE_LENGTH = 8000;
  int ModNum;
  unsigned int ChanNum;
  
  unsigned short Trace[TRACE_LENGTH];
  
  unsigned long trace_aver;
  unsigned long trace_sig;
  float xmin=0,xmax=TRACE_LENGTH;
  unsigned int nhis;
  
  HLIMIT(PAWC_SIZE);

  if (argc !=  3) {
    printf("usage: %s <module> <channel>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
   
  ModNum = atoi(argv[1]);
  ChanNum = atoi(argv[2]);
  
  PixieInterface pif("pixie.cfg");
  pif.GetSlots();
  pif.Init();
           
  usleep(200);
  pif.Boot(PixieInterface::DownloadParameters |
	   PixieInterface::ProgramFPGA |
	   PixieInterface::SetDAC, true);
  
  char traceName[] = "test trace";
  char traceFile[] = "trace.dat";

  if (ModNum<0) {
    for(ModNum = 0; ModNum < pif.GetNumberCards(); ModNum ++) {
      if (!pif.AcquireTraces(ModNum))
	continue;
      for(ChanNum = 0; ChanNum < pif.GetNumberChannels(); ChanNum ++) {
	 nhis=100*(ModNum+1)+ChanNum;
	 HBOOK1(nhis,traceName,TRACE_LENGTH,xmin,xmax,0);
	 if (!pif.ReadSglChanTrace(Trace, TRACE_LENGTH, ModNum, ChanNum))
	   continue;

	 trace_aver=0;
	 trace_sig=0;
	 for(size_t chan = 0; chan < TRACE_LENGTH; chan++) {
	   trace_aver += Trace[chan];
	   HF1(nhis,chan,Trace[chan]);
	 }
	 trace_aver /= TRACE_LENGTH;

	 for(size_t chan = 0; chan < TRACE_LENGTH ; chan++) {
	   trace_sig += (trace_aver-Trace[chan])*(trace_aver-Trace[chan]);
	 }

	 trace_sig=(long)sqrt(trace_sig / TRACE_LENGTH);

	 printf("Trace ---- MOD/CHAN %u/%u AVER |-  %lu  -| SIG %lu \n",ModNum,ChanNum,trace_aver,trace_sig);
         
       } // loop over chans      
     } // loop over mods
   } else {
     nhis=100*(ModNum+1)+ChanNum;
     HBOOK1(nhis,traceName,TRACE_LENGTH,xmin,xmax,0);
     usleep(200);
     if (pif.AcquireTraces(ModNum)) {
       if (pif.ReadSglChanTrace(Trace, TRACE_LENGTH, ModNum, ChanNum)) {
	 usleep(200);
	
	 trace_aver=0;
	 trace_sig=0;
	 for(size_t chan = 0; chan < TRACE_LENGTH ; chan++) {
	   trace_aver += Trace[chan];
	   HF1(nhis,chan,Trace[chan]);
	 }
	 trace_aver /= TRACE_LENGTH;
	 
	 for(size_t chan = 0; chan < TRACE_LENGTH ; chan++) {
	   trace_sig += (trace_aver-Trace[chan])*(trace_aver-Trace[chan]);
	 }     
	 trace_sig = (long)sqrt(trace_sig /TRACE_LENGTH);

	 printf("Trace ---- MOD/CHAN %u/%u AVER |-  %lu  -| SIG %lu \n",ModNum,ChanNum,trace_aver,trace_sig);
       }
     }
  } // if ModNum > 0
  
  char fileOptions[] = "N";

  HRPUT (0, traceFile, fileOptions);
  
  return EXIT_SUCCESS;
}
