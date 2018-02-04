<!-- Author: S. Paulauskas -->
# Laughing Conqueror - Pixie Acquisition and Analysis Software Suite (PAASS)
## Build Status
Branch | Build status
---------|-------------:
Master | [![Build Status](https://travis-ci.org/spaulaus/paass-laughing-conqueror.svg?branch=master)](https://travis-ci.org/spaulaus/paass-laughing-conqueror)
Test | [![Build Status](https://travis-ci.org/spaulaus/paass-laughing-conqueror.svg?branch=test)](https://travis-ci.org/spaulaus/paass-laughing-conqueror)
Dev | [![Build Status](https://travis-ci.org/spaulaus/paass-laughing-conqueror.svg?branch=dev)](https://travis-ci.org/spaulaus/paass-laughing-conqueror)

## Brief history
I forked this project from the [Experimental Nuclear Structure Group](http://www.phys.utk.edu/expnuclear/) at the
[University of Tennessee at Knoxville](https://utk.edu). I have worked on this software since 2009. I became the lead
software developer in 2015. Since my departure from UT in 2017, I have continued to work on the software as a hobby. 
To be explicit, I no longer have any affiliation with UTK or the Experimental Nuclear Structure group. 

My development goals are to

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