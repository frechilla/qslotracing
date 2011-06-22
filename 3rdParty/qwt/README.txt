###########
# WINDOWS #
###########
1) Download qwt from http://qwt.sourceforge.net/
2) Edit qwtconfig.pri and comment out "QWT_CONFIG += QwtDll" to build static libraries
3) Open up the Qt Command Prompt
4) qmake -set QMAKEFEATURES C:\path\to\qwt-6.0\qwt.prf
5) qmake -query
6) cd C:\path\to\qwt-6.0\
7) qmake qwt.pro
8) mingw32-make
9) mingw32-make install
10) In C:\Qwt-6.0.1-svn
    - Copy all contents of the directory "libs" into path/to/QSlotRacing/3rdParty/qwt/i386-win32/lib
11) Get into the new lib directory (path/to/QSlotRacing/3rdParty/qwt/i386-win32/lib) and compress all new libs
    before committing them into the repository

    
#############
# GNU/Linux #
#############
1) Download qwt from http://qwt.sourceforge.net/
2) Edit qwtconfig.pri and comment out "QWT_CONFIG += QwtDll" to build static libraries
    - Edit also the QWT_INSTALL_PREFIX to install the library in a well-known directory
3) Open up a console terminal
4) qmake -set QMAKEFEATURES /path/to/qwt-6.0/qwt.prf
5) qmake -query
6) cd /path/to/qwt-6.0/
7) qmake qwt.pro
8) make
9) make install
10) Get into the path used for QWT_INSTALL_PREFIX in step 2. In the lib/ directory there will 
   be 2 .a files (the static libraries). Run the following commands (on that lib/ directory)
    $ cp libqwt.a libqwtd.a
    $ cp libqwtmathml.a libqwtmathmld.a
    $ strip libqwt.a
    $ strip libqwtmathml.a
11) You will have then 4 different files, 2 release and 2 debug libraries. Copy them into the 3rdParty directory:
    $ cp QWT_INSTALL_PREFIX/lib/*.a path/to/QSlotRacing/3rdParty/qwt/i386-linux/lib
12) gzip the .a library files you just copied before committing them to the repository


##########################
# The include/ directory #
##########################
Whatever system you compiled and installed the qwt library on, the target directory will contain another 
directory called "include". Copy all contents (header files) of this directory into 
path/to/QSlotRacing/3rdParty/qwt/include

