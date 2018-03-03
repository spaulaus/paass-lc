# Laughing Conqueror - Pixie Acquisition and Analysis Software Suite (PAASS)
<!-- Author: S. Paulauskas -->
## Build Status
Branch | Build status
---------|-------------:
Master | [![Build Status](https://travis-ci.org/spaulaus/paass-laughing-conqueror.svg?branch=master)](https://travis-ci.org/spaulaus/paass-laughing-conqueror)
Test | [![Build Status](https://travis-ci.org/spaulaus/paass-laughing-conqueror.svg?branch=test)](https://travis-ci.org/spaulaus/paass-laughing-conqueror)
Dev | [![Build Status](https://travis-ci.org/spaulaus/paass-laughing-conqueror.svg?branch=dev)](https://travis-ci.org/spaulaus/paass-laughing-conqueror)

## Disclaimer
<b>Neither I nor the software are affiliated with the UTK in any way.</b>

I forked this software from the [Experimental Nuclear Structure Group](http://www.phys.utk.edu/expnuclear)
at the [University of Tennessee](https://utk.edu). I was an employee there for 7 years, and have since moved on to a new
job. While with the UTK group, I was project lead. I renamed the software to PAASS - Laughing Conqueror to 
differentiate it from the original UTK software. The name comes from the meanings of the two lead programmers.

## Introduction
Our development goals are
 
1. Create a scientific software package that adheres to modern programming practices. 
2. Create tests for the acquisition and analysis software to ensure accurate results. 
3. Provide an extensible framework that others can use to run experiments.

Software updates focus on 

1. Testing components
2. Performance enhancements
3. Documentation
4. UX

I made the decision not to focus on analysis for a single data set. I want to provide an easy to use software package
that can be extended beyond just a single group or digitizer. 

The software provides a framework to acquire and analyze data from the 
[XIA, LLC  DGF Pixie-16 modules](http://www.xia.com/DGF_Pixie-16.html). These data are written to disk in PLD or LDF 
formats. We provide an interface to use PACMAN as our run control software. This provides a stable and well debugged 
DAQ system. For more information visit the [wiki](https://github.com/spaulaus/paass-laughing-conqueror/wiki).

## A little history! 
Many post-docs, students, and PIs worked on this software over the years. I want to give credit where it's due. They 
documented their names in the software they wrote, and in the documentation files. If your name is missing or you have a
contribution, please contact the code maintainers.
### Data Acquisition Development
  * R. Grzywacz
  * D. T. Miller
  * C. Thornsberry
  * K. Smith
  * S. V. Paulauskas

### Data Analysis Development
  * R. Grzywacz
  * S. N. Liddick
  * D. T. Miller
  * S. V. Paulauskas
  * K. A. Miernik

The data acquisition software is heavily based on the work by D. Miller in 
[Pixie Suite](https://github.com/pixie16/PixieSuite/). K. Smith and C. R. Thornsberry forked Pixie Suite into 
[Pixie Suite 2](https://github.com/pixie16/PixieSuite2/). They introduced numerous bug fixes and the ncurses GUI. The 
main DAQ program, poll2, allows users to fully configure Pixie-16 modules and acquire data. 

The analysis software comes from [pixie_scan](https://github.com/pixie16/pixie_scan/). Pixie scan is the workhorse 
analysis package used by groups world wide. PAASS-LC blends the repositories to create a complete experimental 
package. DAQ and analysis rolled into a single suite. The man analysis program, utkscan, is modular and extensible
to accommodate any experiment. Users pick and choose detector types to build a custom experiment processor.

For more information about the original project you can view the read me at the 
[original project](https://github.com/pixie16/paass).   

## Dependencies
### For all components 
* ncurses
* gcc v4.4.6+
* cmake v2.8.8+
* [ROOT](https://root.cern.ch)

### Acquisition
* [PLX](http://support.xia.com/default.asp?W372)
* [XIA API](http://support.xia.com/default.asp?W372)

### Analysis
* gsl v2.x

### Optional (but recommended at this time)
* [UPAK](https://www.phy.ornl.gov/computing/daqsupport.html)

### Tested Systems
The software has been compiled and tested on the following setups: 

Platform | Kernel | Cmake | GCC
-------- | ------ |:-----:| ---
CentOS 6.3 | 2.6.32-279.19.1.el6.x86\_64 | 2.8.12.2 | 4.4.6
RHEL 6.6   | 2.6.32-504.23.4.el6.x86\_64 | 2.8.12.2 | 4.4.7
Bash on Ubuntu on Windows | 4.4.0-43-Microsoft | 3.5.1 | 5.4.0 20160609

## Licensing and Copyright
### Software developed by S. V. Paulauskas
I release all software developed for this project under the Creative Commons Attribution-ShareAlike 4.0 International 
License.   

Copyright (c) 2017-2018 S. V. Paulauskas. 

### Software developed at UTK
UTKs code is licensed under the GNU GPL v. 3.0.

Copyright (c) 2009 - 2017, University of Tennessee Board of Trustees All rights reserved.

This code was developed as part of the research efforts of the Experimental Nuclear Physics Group for use with the 
Pixie-16 electronics. The work was supported by the following grants:

DE-FG52-08NA28552

provided by the Department of Energy and the National Science Foundation.

The work is licensed under the GNU GPLv3.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of University of Tennessee nor the names of its contributors may be used to endorse or promote 
products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.