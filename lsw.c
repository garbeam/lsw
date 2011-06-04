/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

static void getname(Window win, char *buf, size_t size);
static void lsw(Window win);

static Atom netwmname;
static Bool lflag = False;
static Display *dpy;

int
main(int argc, char *argv[]) {
	int i;

	if(!(dpy = XOpenDisplay(NULL))) {
		fputs("lsw: cannot open display\n", stderr);
		exit(EXIT_FAILURE);
	}
	netwmname = XInternAtom(dpy, "_NET_WM_NAME", False);

	for(i = 1; i < argc; i++)
		if(!strcmp(argv[i], "-v")) {
			puts("lsw-"VERSION", © 2006-2011 lsw engineers, see LICENSE for details");
			exit(EXIT_SUCCESS);
		}
		else if(!strcmp(argv[i], "-l"))
			lflag = True;
		else
			break;

	if(i == argc)
		lsw(DefaultRootWindow(dpy));
	else while(i < argc)
		lsw(strtol(argv[i++], NULL, 0));

	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}

void
lsw(Window win) {
	char buf[BUFSIZ];
	unsigned int i, n;
	Window *wins, dw;
	XWindowAttributes wa;

	if(!XQueryTree(dpy, win, &dw, &dw, &wins, &n))
		return;
	for(i = 0; i < n; i++)
		if(XGetWindowAttributes(dpy, win, &wa) && !wa.override_redirect) {
			getname(wins[i], buf, sizeof buf);
			if(lflag)
				printf("0x%lx %s\n", wins[i], buf);
			else if(*buf)
				puts(buf);
		}
	XFree(wins);
}

void
getname(Window win, char *buf, size_t size) {
	char **list = NULL;
	int n;
	XTextProperty prop;

	buf[0] = '\0';
	if(!XGetTextProperty(dpy, win, &prop, netwmname) || prop.nitems == 0)
		if(!XGetWMName(dpy, win, &prop) || prop.nitems == 0)
			return;

	if(prop.encoding == XA_STRING)
		strncpy(buf, (char *)prop.value, size);
	else if(!XmbTextPropertyToTextList(dpy, &prop, &list, &n) && n > 0) {
		strncpy(buf, list[0], size);
		XFreeStringList(list);
	}
	XFree(prop.value);
}
