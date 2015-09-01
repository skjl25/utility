#include"viewer.h"

viewer::viewer()
{
	data_buff = NULL;
	ximg1 = NULL;
	ximg1_data = NULL;
	type = GRAY;
	width = 160;
	height = 120;
	DRAWING = false;
	box_r = 0;
	box_g = 0;
	box_b = 255;
	ulx = 0;
	uly = 0;
	lrx = 0;
	lry = 0;
	BOX_POINTS = 0;
	DRAW_BOX = false;

	xdisplay = XOpenDisplay("");       //Connect to the X server
	if(xdisplay == NULL){
		cout<<"Cannot connect to server!"<<endl;
		//    exit(EXIT_FAILURE);
	}

	rootwindow = DefaultRootWindow(xdisplay);
	xscreen = DefaultScreen(xdisplay);
	xvisual = DefaultVisual(xdisplay, xscreen);
	cmap = DefaultColormap(xdisplay, xscreen);
	depth = DefaultDepth(xdisplay, xscreen);

	black = BlackPixel(xdisplay, xscreen);
	white = WhitePixel(xdisplay, xscreen);

	sem_init(&mutex_event, 0, 1);
	sem_init(&mutex_draw, 0, 1);
	EVENT = false;

	window1 = XCreateWindow(xdisplay, rootwindow, 0, 0, width, height,
			0, depth, InputOutput, xvisual, 0, NULL);
	wgc = XCreateGC(xdisplay, window1, NULL, NULL);

	XSizeHints sizehints = {
			PMinSize | PMaxSize | PPosition | PSize | USSize,
			0, 0,                 //x, y
			width, height,        //width, height
			width, height,        //min_width, min_height
			width, height,        //max_width, max_height
			0, 0,                 //width and height increments, not set
			0, 0, 0, 0,           //aspect ration, not set
	};

	XSetStandardProperties(xdisplay, window1, "Viewer", NULL, NULL, NULL, 0, &sizehints);
	XSetWindowBorder(xdisplay, window1, black);
	XSetWindowBackground(xdisplay, window1, white);
	XSelectInput(xdisplay, window1, ExposureMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
	XMapRaised(xdisplay, window1);     			//Pop this window up on screen
}


viewer::~viewer()
{
	XFreeGC(xdisplay, wgc);
	XDestroyWindow(xdisplay, window1);
	//XCloseDisplay(xdisplay);
}


void viewer::move(int x, int y)
{
	XMoveWindow(xdisplay, window1, x, y);
}


//RGB to BGRA
void viewer::image(char* data, int w, int h, int t)
{
	int i, at1, at3, at4;
	int r, g, b;
	int size;

	sem_wait(&mutex_draw);
	DRAWING = true;

	if(data != NULL){	//Buffer data
		size = w * h;
		if(t == RGB) size *= 3;

		if(data_buff == NULL || type != t || width != w || height != h){
			if(data_buff != NULL) delete [] data_buff;
			data_buff = new char[size];
		}

		for(i=0; i<size; i++) data_buff[i] = data[i];
	}

	if(ximg1_data == NULL || type != t || width != w || height != h){
		type = t;
		width = w;
		height = h;
		if(ximg1_data != NULL) delete [] ximg1_data;
		ximg1_data = new char[width * height * 4];

		XSizeHints sizehints = {
				PMinSize | PMaxSize | PPosition | PSize | USSize,
				0, 0,                 //x, y
				width, height,        //width, height
				width, height,        //min_width, min_height
				width, height,        //max_width, max_height
				0, 0,                 //width and height increments, not set
				0, 0, 0, 0,           //aspect ration, not set
		};

		XSetStandardProperties(xdisplay, window1, "Viewer", NULL, NULL, NULL, 0, &sizehints);
		XResizeWindow(xdisplay, window1, width, height);
	}

	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			at4 = y*width*4 + x*4;

			if(type == GRAY){
				at1 = y*width + x;
				r = data_buff[at1];
				g = r;
				b = r;
			}else{
				at3 = y*width*3 + x*3;
				r = data_buff[at3+0];
				g = data_buff[at3+1];
				b = data_buff[at3+2];
			}

			if(DRAW_BOX && BOX_POINTS == 2){
				if(y==uly && x>=ulx && x<=lrx){ r=box_r; g=box_g; b=box_b; }	//Top line
				if(y==lry && x>=ulx && x<=lrx){ r=box_r; g=box_g; b=box_b; }	//Bottom line
				if(x==ulx && y>=uly && y<=lry){ r=box_r; g=box_g; b=box_b; }	//Left line
				if(x==lrx && y>=uly && y<=lry){ r=box_r; g=box_g; b=box_b; }	//Right line
			}

#ifdef EXCEED_
			ximg1_data[at4+0] = 0;
			ximg1_data[at4+1] = b;
			ximg1_data[at4+2] = g;
			ximg1_data[at4+3] = r;
#else
			ximg1_data[at4+0] = b;
			ximg1_data[at4+1] = g;
			ximg1_data[at4+2] = r;
			ximg1_data[at4+3] = 0;
#endif
		}
	}

	ximg1 = XCreateImage(xdisplay, xvisual, depth, ZPixmap, 0, ximg1_data, width, height, 32, 4*width);
	sem_post(&mutex_draw);

	//XClearArea(xdisplay, window1, 0, 0, 0, 0, true);	//Refresh: clear entire area
	XClearArea(xdisplay, window1, 0, 0, 1, 1, true);	//Refresh: clear one pixel (double buffer)
	XFlush(xdisplay);
}

void viewer::image(char* data, int w, int h, int t, char* viewerName)
{
	int i, at1, at3, at4;
	int r, g, b;
	int size;

	sem_wait(&mutex_draw);
	DRAWING = true;

	if(data != NULL){	//Buffer data
		size = w * h;
		if(t == RGB) size *= 3;

		if(data_buff == NULL || type != t || width != w || height != h){
			if(data_buff != NULL) delete [] data_buff;
			data_buff = new char[size];
		}

		for(i=0; i<size; i++) data_buff[i] = data[i];
	}

	if(ximg1_data == NULL || type != t || width != w || height != h){
		type = t;
		width = w;
		height = h;
		if(ximg1_data != NULL) delete [] ximg1_data;
		ximg1_data = new char[width * height * 4];

		XSizeHints sizehints = {
				PMinSize | PMaxSize | PPosition | PSize | USSize,
				0, 0,                 //x, y
				width, height,        //width, height
				width, height,        //min_width, min_height
				width, height,        //max_width, max_height
				0, 0,                 //width and height increments, not set
				0, 0, 0, 0,           //aspect ration, not set
		};

		XSetStandardProperties(xdisplay, window1, viewerName, NULL, NULL, NULL, 0, &sizehints);
		XResizeWindow(xdisplay, window1, width, height);
	}

	for(int y=0; y<height; y++){
		for(int x=0; x<width; x++){
			at4 = y*width*4 + x*4;

			if(type == GRAY){
				at1 = y*width + x;
				r = data_buff[at1];
				g = r;
				b = r;
			}else{
				at3 = y*width*3 + x*3;
				r = data_buff[at3+0];
				g = data_buff[at3+1];
				b = data_buff[at3+2];
			}

			if(DRAW_BOX && BOX_POINTS == 2){
				if(y==uly && x>=ulx && x<=lrx){ r=box_r; g=box_g; b=box_b; }	//Top line
				if(y==lry && x>=ulx && x<=lrx){ r=box_r; g=box_g; b=box_b; }	//Bottom line
				if(x==ulx && y>=uly && y<=lry){ r=box_r; g=box_g; b=box_b; }	//Left line
				if(x==lrx && y>=uly && y<=lry){ r=box_r; g=box_g; b=box_b; }	//Right line
			}

#ifdef EXCEED_
			ximg1_data[at4+0] = 0;
			ximg1_data[at4+1] = b;
			ximg1_data[at4+2] = g;
			ximg1_data[at4+3] = r;
#else
			ximg1_data[at4+0] = b;
			ximg1_data[at4+1] = g;
			ximg1_data[at4+2] = r;
			ximg1_data[at4+3] = 0;
#endif
		}
	}

	ximg1 = XCreateImage(xdisplay, xvisual, depth, ZPixmap, 0, ximg1_data, width, height, 32, 4*width);
	sem_post(&mutex_draw);

	//XClearArea(xdisplay, window1, 0, 0, 0, 0, true);	//Refresh: clear entire area
	XClearArea(xdisplay, window1, 0, 0, 1, 1, true);	//Refresh: clear one pixel (double buffer)
	XFlush(xdisplay);
}
void viewer::get_event()
{
	XNextEvent(xdisplay, &e);	//Wait for next event
	switch(e.type){
	case Expose:
		sem_wait(&mutex_draw);
		if(ximg1 != NULL){
			XPutImage(xdisplay, window1, wgc, ximg1, 0, 0, 0, 0, ximg1->width, ximg1->height);
		}
		DRAWING = false;
		sem_post(&mutex_draw);
		break;
	case ButtonPress:
		if(e.xbutton.window == window1){
			if(ximg1){
				if(e.xbutton.x >= 0 && e.xbutton.x < width && e.xbutton.y >= 0 && e.xbutton.y < height){
					sem_wait(&mutex_event);
					X = e.xbutton.x;
					Y = e.xbutton.y;
					BUTTON = e.xbutton.button;
					TYPE = PRESS;
					x1 = X;
					y1 = Y;
					BOX_POINTS = 1;
					EVENT = true;
					sem_post(&mutex_event);
				}
			}
		}
		break;
	case ButtonRelease:
		if(e.xbutton.window == window1){
			if(ximg1){
				if(e.xbutton.x >= 0 && e.xbutton.x < width && e.xbutton.y >= 0 && e.xbutton.y < height){
					sem_wait(&mutex_event);
					X = e.xbutton.x;
					Y = e.xbutton.y;
					BUTTON = e.xbutton.button;
					TYPE = RELEASE;

					if(DRAW_BOX){
						DRAW_BOX = false;
						BOX_POINTS = 0;
						image(NULL, width, height);
					}

					EVENT = true;
					sem_post(&mutex_event);
				}
			}
		}
		break;
	case MotionNotify:
		if(e.xbutton.window == window1){
			if(ximg1){
				if(e.xbutton.x >= 0 && e.xbutton.x < width && e.xbutton.y >= 0 && e.xbutton.y < height){
					sem_wait(&mutex_event);
					X = e.xbutton.x;
					Y = e.xbutton.y;
					//BUTTON = e.xbutton.button;	//FIX: invalid, just keep last value
					TYPE = DRAG;

					//Set box
					x2 = X;
					y2 = Y;

					if(x1 < x2){
						ulx = x1;
						lrx = x2;
					}else{
						ulx = x2;
						lrx = x1;
					}

					if(y1 < y2){
						uly = y1;
						lry = y2;
					}else{
						uly = y2;
						lry = y1;
					}

					BOX_POINTS = 2;
					if(DRAW_BOX) redraw();

					EVENT = true;
					sem_post(&mutex_event);
				}
			}
		}
		break;
	}

	XFlush(xdisplay);
}


void viewer::ginput(int& x, int& y)
{
	while(1){
		sem_wait(&mutex_event);
		if(EVENT){
			if(TYPE == PRESS){
				break;
			}else{
				EVENT = false;
			}
		}
		sem_post(&mutex_event);
	}

	x = X;
	y = Y;
	EVENT = false;
	sem_post(&mutex_event);
}


void viewer::ginput(int& x, int& y, int& button)
{
	while(1){
		sem_wait(&mutex_event);
		if(EVENT){
			if(TYPE == PRESS){
				break;
			}else{
				EVENT = false;
			}
		}
		sem_post(&mutex_event);
	}

	x = X;
	y = Y;
	button = BUTTON;
	EVENT = false;
	sem_post(&mutex_event);
}


void viewer::ginput(int& x, int& y, int& button, int& type)
{
	while(1){
		sem_wait(&mutex_event);
		if(EVENT) break;
		sem_post(&mutex_event);
	}

	x = X;
	y = Y;
	button = BUTTON;
	type = TYPE;
	EVENT = false;
	sem_post(&mutex_event);
}


void viewer::box_color(int r, int g, int b)
{
	box_r = r;
	box_g = g;
	box_b = b;
}


void viewer::draw_box()
{
	DRAW_BOX = true;
}


void viewer::get_box(int& u1, int& v1, int& u2, int& v2)
{
	u1 = ulx;
	v1 = uly;
	u2 = lrx;
	v2 = lry;
}


void viewer::raise()
{
	XRaiseWindow(xdisplay, window1);
	XFlush(xdisplay);
}


void viewer::redraw()
{
	//DRAWING is started in image() and ends in get_event(), a semaphore is not
	// necessary since a mistake is only inefficient.
	if(!DRAWING) image(NULL, width, height);
}


void thread_viewer(viewer& v)
{
	pthread_t t1;
	pthread_create(&t1, NULL, viewer_thread, (void*) &v);
}


void* viewer_thread(void* args)
		{
	viewer * v = (viewer*) args;
	while(1){
		v->get_event();
	}
		}
