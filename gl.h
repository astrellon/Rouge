#ifdef _WIN32
#	include <Windows.h>

#	include <gl/GL.h>
#	include <gl/GLU.h>

#else
/* Xlib.h is the default header that is included and has the core functionallity */
#	include <X11/Xlib.h>                                                              
/* Xatom.h includes functionallity for creating new protocol messages */           
#	include <X11/Xatom.h>                                                             
/* keysym.h contains keysymbols which we use to resolv what keys that are being pressed */
#	include <X11/keysym.h>                                                                   
 
/* the XF86 Video Mode extension allows us to change the displaymode of the server
 * this allows us to set the display to fullscreen and also read videomodes and   
 * other information.                                                             
 */                                                                               
#	include <X11/extensions/xf86vmode.h>                                             
 
/* gl.h we need OpenGL :-) */
#	include <GL/gl.h>           
/* this file is needed for X11 applications if we want to use hardware rendering */
#	include <GL/glx.h>                                                                
#	include <GL/glu.h>   
#endif
