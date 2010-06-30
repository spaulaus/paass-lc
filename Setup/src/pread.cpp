/********************************************************************/
/*	pread.cpp      					            */
/*		last updated: 09/30/09 DTM	     	       	    */
/*			       					    */
/********************************************************************/
#include <cstdio>
#include <cstdlib>

#include "PixieInterface.h"

int main(int argc, char *argv[])
{
  int ModNum, ChanNum;
  char *NAME;

  if (argc != 4) {
    printf("usage: %s <module> <channel> <parameter name>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  ModNum  = atoi(argv[1]);
  ChanNum = atoi(argv[2]);
  NAME = argv[3];

  PixieInterface pif("pixie.cfg");

  pif.GetSlots();
  pif.Init();
  pif.Boot(PixieInterface::DownloadParameters |
	   PixieInterface::ProgramFPGA |
	   PixieInterface::SetDAC, true);

  if (ModNum < 0) {
    // read from all modules, all channels
    for (ModNum = 0; ModNum < pif.GetNumberCards(); ModNum++) {
      for (ChanNum = 0; ChanNum < 16; ChanNum++) {
	pif.PrintSglChanPar(NAME, ModNum, ChanNum);
      }
    }
  } else if (ModNum >= 0) {
    if (ChanNum >= 0) {
      pif.PrintSglChanPar(NAME, ModNum, ChanNum);
    } else if (ChanNum < 0) {            
      for (ChanNum = 0; ChanNum < 16; ChanNum++) {
	pif.PrintSglChanPar(NAME, ModNum, ChanNum);
      } // ChanNum loop
    } // ChanNum +/-
  } // ModNum +/-

  return EXIT_SUCCESS;
}
