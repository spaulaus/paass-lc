/** Program which finds the decay constant for an active pixie channel
 *
 * David Miller, May 2010
 */

#include <iostream>

#include <cstdlib>

#include "PixieInterface.h"
#include "pixie16app_export.h"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  int modNum, chanNum;
  
  if (argc != 3) {
    cout << "usage: " << argv[0] << " <module> <channel>" << endl;
  }

  modNum  = atoi(argv[1]);
  chanNum = atoi(argv[2]);

  PixieInterface pif("pixie.cfg");

  pif.GetSlots();
  pif.Init();
  pif.Boot(PixieInterface::DownloadParameters |
	   PixieInterface::ProgramFPGA |
	   PixieInterface::SetDAC, true);

  char parName[] = "TAU";
  double tau;

  int errno = Pixie16TauFinder(modNum, chanNum, &tau);
  cout << "TAU: " << tau << endl;
  cout << "Errno: " << errno << endl;

  return EXIT_SUCCESS;
}
