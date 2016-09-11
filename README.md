#### CI Testing
* Tarvis-CI for OS X, iOS, Linux [![Build Status](https://travis-ci.org/kodi-adsp/adsp.template.svg?branch=master)](https://travis-ci.org/kodi-adsp/adsp.template)
* AppVeyor for Windows [![Build status](https://ci.appveyor.com/api/projects/status/p3pd175e1leas49a/branch/master?svg=true)](https://ci.appveyor.com/project/AchimTuran/adsp-template/branch/master)
* Code analyses for Linux [![Coverity Scan Build Status](https://scan.coverity.com/projects/5120/badge.svg)](https://scan.coverity.com/projects/5120)


ADSP Template Addon
==============
You can use this template to see how the ADSP Addon API works or to start a new ADSP Addon.
Preprocessing only copies input to output.
Master process implements a simple channel upmixer (e.g. stereo to 5.1) by copying the input channel to output channels.
Postprocessing copies input samples to output.


ADSP Addon API Documentation
==============
For more details see: http://wisler.bplaced.de/xbmc-adsp-addons/

## Build instructions for Kodi's binary add-ons

When building the addon you have to use the correct branch depending on which version of Kodi you're building against. 
For example, if you're building the `Jarvis` branch of Kodi you should checkout the `Jarvis` branch of this repository. 
Addon releases are also tagged regularly.

For further instructions replace `{addon-id}` and `{org}` with the repository- and the Github organisation-name.
E.g. `https://github.com/kodi-adsp/adsp.basic.git` results in `{addon-id}=adsp.basic` and `{org}=kodi-adsp`

With the following instructions the add-on files will be placed in `../../xbmc/addons`. Consequently this adds a system add-on, which can be used direclty if you build Kodi from source.

### Linux

1. `git clone https://github.com/xbmc/xbmc.git`
2. `git clone https://github.com/{org}/{addon-id}.git`
3. `cd {addon-id} && mkdir build && cd build`
4. `cmake -DADDONS_TO_BUILD={addon-id} -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../xbmc/addons -DPACKAGE_ZIP=1 ../../xbmc/project/cmake/addons`
5. `make`

### Windows

Use the visual studio command line or run `call "%VS120COMNTOOLS%..\..\VC\bin\vcvars32.bat"`

1. `git clone https://github.com/xbmc/xbmc.git`
2. `git clone https://github.com/{org}/{addon-id}.git`
3. `cd {addon-id} && mkdir build && cd build`
4. `cmake -G "NMake Makefiles" -DADDONS_TO_BUILD={addon-id} -DADDON_SRC_PREFIX=../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../xbmc/addons -DPACKAGE_ZIP=1 ../../xbmc/project/cmake/addons`
5. `cmake --build "%cd%" --target "{addon-id}"`


## Useful links

* [Kodi's PVR user support] (http://forum.kodi.tv/forumdisplay.php?fid=167)
* [General binary add-on development] (http://forum.kodi.tv/forumdisplay.php?fid=26)
* [Kodi Core development support] (http://forum.kodi.tv/forumdisplay.php?fid=93)
* [Kodi AudioDSP development support] (http://forum.kodi.tv/forumdisplay.php?fid=235)
* [Kodi Screensavers and Visualisations development support] (http://forum.kodi.tv/forumdisplay.php?fid=38)
* [Kodi PVR development support] (http://forum.kodi.tv/forumdisplay.php?fid=136)

-------------
#### License
[GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007](https://github.com/kodi-adsp/adsp.template/blob/master/LICENSE.md)

