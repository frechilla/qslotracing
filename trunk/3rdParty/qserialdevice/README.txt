###########
# WINDOWS #
###########
1) Download latest version of qserialdevice from http://qt-apps.org/content/show.php?content=112039
  1a) Note: You can also use version 0.4.0 uploaded here
2) Uncompress the file somewhere in your filesystem (path/to/qserialdevice)
3) Open the project with QT Creator 
  The .pri file is located under path/to/qserialdevice/qserialdevice-qserialdevice/src/src.pro)
4) In both debug and release mode build all (selecting in the menu "build->build all")
5) In this project's working directory (typically something like 
   path/to/qserialdevice/qserialdevice-qserialdevice/src-build-desktop)
  The debug library is located in (WORKING-DIR)/build/debug/libqserialdeviced.a
  The release library is located in (WORKING-DIR)/build/release/libqserialdevice.a
6) Copy both libraries into path/to/QSlotRacing/3rdParty/qserialdevice/i386-win32/lib
7) Get into the new lib directory (path/to/QSlotRacing/3rdParty/qserialdevice/i386-win32/lib) and compress all new libs
   before committing them into the repository


##########################
# The include/ directory #
##########################
Whatever system you compiled the qserialdevice library on, you should update the include directory in
path/to/QSlotRacing/3rdParty/qserialdevice/include with the newest header files. 

CD into path/to/qserialdevice/qserialdevice-qserialdevice/src. You must then copy all files found in 
the qserialdevice and qserialdeviceenumerator directories, plus the ones found in that directory itself
(path/to/qserialdevice/qserialdevice-qserialdevice/src)

At the time of writing this document they were the following files:
 - qserialdevice_global.h
 - qserialdevice/
     abstractserial.h
     abstractserial_p.h
     abstractserialengine.h
     abstractserialengine_p.h
     abstractserialnotifier.h
     nativeserialengine.h
     nativeserialengine_p.h
     nativeserialnotifier.h
     qcore_unix_p.h
     ttylocker.h
 - qserialdeviceenumerator/
     serialdeviceenumerator.h
     serialdeviceenumerator_p.h
