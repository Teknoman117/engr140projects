FREEGLUTAPP
-----------

This is a demo project to start implementing OpenGL applications
using freeglut. 

Linux:
 - Type make to compile the application
 - The makefile will compile all .cpp files in the folder
 - Edit the makefile to change the name of the executable

Windows:
 - Use the visual studio 10 solution in the visualc10 folder
 - Make sure visualc 2010 is installed 
 - To open the project just double click the .sln file
 
OS X:
 - Use the glutapp.xcodeproj file to open the project in XCode
 - It has been tested with XCode 6 on OS X Yosemite 10.10
 - Change the Build Settings if necessary


Notes:
 - The provided freeglut library is only used in Windows; 
   in Linux and OS X the compiler will seek for the header
   files and libraries in the default development folders.
   
 - On OS X, you can compile in the command line, so you don't
   have to use XCode. Simply follow the Linux instructions. 
   XCode should still be installed so that you have the OpenGL
   and GLUT libraries. To get started with XCode, download it
   from the App Store. Open it and let it set itself up. Then
   in the terminal type: xcode-select -install
   This will install the command line compiler tools on your Mac