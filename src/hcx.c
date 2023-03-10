#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include <stdio.h>

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define ALT Mod1Mask
#define WIN Mod4Mask

static void spawn(char **com) {
	if (fork() == 0) {
		setsid();
		if (fork() != 0) {
			_exit(0);
		}
		execvp((char*)com[0], (char**)com);
		_exit(0);
	}
	wait(NULL);
}

int main(void){
	Display* dpy;
	XWindowAttributes attr;
	XButtonEvent start;
	XEvent ev;

	if(!(dpy = XOpenDisplay(0x0))) return 1;

	// Grab ONLY:
	// Win+F1 to Win+F3
	// Win+Button1
	// Win+Button3, which is RMB

	// F1-F3 written better
	for(int j = 67; j != 70; j++){
		KeyCode k = j;

		// All can be grabbed via Win modifier
		// KeySym s = XKeycodeToKeysym(dpy, k, 0);
		KeySym s = XkbKeycodeToKeysym(dpy, k, 0, 0);
		XGrabKey(dpy, XKeysymToKeycode(dpy, s), AnyModifier,
		DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
	}

	XGrabButton(dpy, LMB, WIN, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
	XGrabButton(dpy, RMB, WIN, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

	start.subwindow = None;
	for(;;){
		// Poll through each event
		XNextEvent(dpy, &ev);

		// Raise window
		if(ev.type == KeyPress && ev.xkey.subwindow != None){
			printf("%d\n", ev.xkey.keycode);
			unsigned int keycode = ev.xkey.keycode;
			switch(keycode){
				case 67: // F1
					// Raise window only in this case
					XRaiseWindow(dpy, ev.xkey.subwindow);
					break;

				case 68: // F2
					spawn(termcmd);
					break;	

				case 69: // F3
					spawn(menucmd);
					break;
			}
		}


		if(ev.type == ButtonPress && ev.xbutton.subwindow != None){
			XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
			start = ev.xbutton;
		} else {
			// We are on the root window
			if(ev.type == ButtonPress &&ev.xbutton.window == DefaultRootWindow(dpy)){
				int button = ev.xbutton.button;
				if(button == menu_btn){
					printf("Opening menucmd that is \"%s\"\n", menucmd[0]);
					// open dmenu or whatever the menu cmd is
					spawn(menucmd);
				} else {
					printf("unhandled button: %d\n", button);
				}
			}
		}
		
		// This event only fires when we are clicking and dragging at the same time.
		// We also have to check if we are on the current window
		if(ev.type == MotionNotify && start.subwindow != None){
			int xdiff = ev.xbutton.x_root - start.x_root;
			int ydiff = ev.xbutton.y_root - start.y_root;
			XMoveResizeWindow(dpy, start.subwindow,
				attr.x + (start.button==1 ? xdiff : 0),
				attr.y + (start.button==1 ? ydiff : 0),
				MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
				MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
		}

		if(ev.type == ButtonRelease) start.subwindow = None;
	}
}
