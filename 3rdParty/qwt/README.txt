1) Download qwt from http://qwt.sourceforge.net/
2) Edit qwtconfig.pri and comment out "QWT_CONFIG += QwtDll" to build static libraries
3) qmake -set QMAKEFEATURES C:\path\to\qwt-6.0\qwt.prf
4) qmake -query
5) cd C:\path\to\qwt-6.0\
6) qmake qwt.pro
7) mingw32-make
8) mingw32-make install
9) In C:\Qwt-6.0.1-svn
    - Copy all contents of the directory "include" into path/to/QSlotRacing/3rdParty/qwt/i386-win32/include
    - Copy all contents of the directory "libs" into path/to/QSlotRacing/3rdParty/qwt/i386-win32/lib
    