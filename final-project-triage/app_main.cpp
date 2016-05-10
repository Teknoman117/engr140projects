# include <destructo-base/stdafx.h>
# include "app_window.h"

//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
 {
   // Init freeglut library:
   glutInit ( &argc, argv );
#if defined __APPLE__
   glutInitDisplayMode ( GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
#else
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitContextProfile ( GLUT_CORE_PROFILE );
#endif

   // Now create the window of your application:
   AppWindow* w = new AppWindow ( "OOP Project", 300, 300, 640, 480 );

   // Finally start the main loop:
   w->run ();
}
