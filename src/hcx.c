#include <X11/Xlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define LMB 1
#define RMB 3

#define ALT Mod1Mask
#define WIN Mod4Mask

int main(void){
  Display* dpy;
  XWindowAttributes attr;
  XButtonEvent start;
  XEvent ev;

  if(!(dpy = XOpenDisplay(0x0))) return 1;

  // Grab:
  // All keysms
  // Win+Button1
  // Win+Button3, which is RMB

  for(int j = 8; j != 256; j++){
    KeyCode k = j;

    // All can be grabbed via Win modifier
    XGrabKey(dpy, XKeysymToKeycode(dpy, XKeycodeToKeysym(dpy, k, NULL)), AnyModifier,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
  }
  // XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), WIN, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
  XGrabButton(dpy, LMB, WIN, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
  XGrabButton(dpy, RMB, WIN, DefaultRootWindow(dpy), True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

  start.subwindow = None;
  for(;;){
    // Poll through each event
    XNextEvent(dpy, &ev);

    // Raise window
    if(ev.type == KeyPress && ev.xkey.subwindow != None){
      unsigned int keycode = ev.xkey.keycode;
      printf("Keycode is %d\n", keycode);
      
      XRaiseWindow(dpy, ev.xkey.subwindow);
    } else if(ev.type == KeyPress) printf("Keycode is %d\n", ev.xkey.keycode);

    if(ev.type == ButtonPress && ev.xbutton.subwindow != None){
      XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
      start = ev.xbutton;
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
