
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "app_window.h"


//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
{
	// Init random number generator
	srand(time(NULL));

	// Init freeglut library:
	glutInit ( &argc,argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Now create the window of your application:
	AppWindow* w = new AppWindow ( "CSE165 Support Code", 300, 300, 640, 640 );

	// Finally start the main loop:
	w->run ();
}
