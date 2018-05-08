# Laughing Conqueror - Pixie Acquisition and Analysis Software Suite (PAASS)
<!-- Author: S. Paulauskas -->
## Build Status
Branch | Build status
---------|-------------:
Master | [![Build Status](https://travis-ci.org/spaulaus/paass-lc.svg?branch=master)](https://travis-ci.org/spaulaus/paass-lc)
Test | [![Build Status](https://travis-ci.org/spaulaus/paass-lc.svg?branch=test)](https://travis-ci.org/spaulaus/paass-lc)
Dev | [![Build Status](https://travis-ci.org/spaulaus/paass-lc.svg?branch=dev)](https://travis-ci.org/spaulaus/paass-lc)

## Disclaimer
<b>Neither I nor the software are UTK affiliated.</b>

## Introduction
I forked this software from [pixie16/paass](https://github.com/pixie16/paass). I was a developer on that project from 2009 - 2016. I gave the fork a new name to differentiate from the original software. PAASS - Laughing Conqueror or PAASS-LC combines my name with another developer's. 
 
Our development goals are
 
1. A modern scientific software package.
2. Acquisition and analysis test coverage to ensure accurate results. 
3. Provide an extensible framework to run experiments.

Software updates focus on 

1. Testing components
2. Performance enhancements
3. Documentation
4. UX

I made the decision not to focus on analysis of a single data set. I want to provide a software package that users can extend beyond a single experiment.

The software provides acquisition and analytis frameworks for the 
[XIA, LLC  DGF Pixie-16 modules](http://www.xia.com/DGF_Pixie-16.html). We write data in PLD or LDF formats. The software can interface with PACMAN for run control. This provides a stable and well debugged DAQ system. For more information visit the [wiki](https://github.com/spaulaus/paass-laughing-conqueror/wiki). For an overview of the main components you can read [here](https://www.projectscience.tech/_posts/2018/04/01/PAASS-LC)

## Dependencies
### For all components 
* ncurses
* gcc v4.4.6+
* cmake v2.8.8+
* [ROOT](https://root.cern.ch)
 * gsl v2.x 

### Acquisition
* [PLX](http://support.xia.com/default.asp?W372)
* [XIA API](http://support.xia.com/default.asp?W372)

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