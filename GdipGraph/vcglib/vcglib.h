#pragma once


// 如果定义了C++编译器，那么声明为C链接方式
#ifdef __cplusplus
extern "C" {
#endif



#ifndef VCGLIB_H 
#define VCGLIB_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


	typedef void(__cdecl* LINE_CB)(int x1, int y1, int x2, int y2, int color, void* painter);
	typedef void(__cdecl* RECTANGLE_CB)(long x, long y, int w, int h, int color, void* painter);
	typedef void(__cdecl* POLYGON_CB)(POINT* hp, int j, int color, void* painter);


	//void save_input_file_contents(FILE* fp);
	//void error(const char* format, ...);
	void display_part(void);
	void setScreenSpecifics(void);
	void gs_exit(int x);
	int  f_is_writable(char* fname);
	int vcg_main(int argc, char* argv[]);
	void Fatal_error(char* x, char* y);
	void parse_part(void);
	void visualize_part(void);
	void relayout(void);
	void draw_graph(LINE_CB _line_cb, RECTANGLE_CB _rectangle_cb, POLYGON_CB _polygon_cb, void* _painter);
	void m_reload(void);
	void m_validate_fe(int code);
	void display_complete_graph(void);
	void gs_line(int fx, int fy, int tx, int ty, int color);
	void gs_rectangle(long x, long y, int w, int h, int color);
	int set_drawing_rectangle(int width, int height);


    
    
	/*
	* step4.c
	*/
	extern void statistics(void);
    extern int st_nr_vis_nodes, st_nr_vis_edges, nr_crossings; 


	/*
	* option.c
	* extern const unsigned char redmap[];
	* extern const unsigned char greenmap[];
	* extern const unsigned char bluemap[];
	* extern char* G_title;
	* extern int G_color;
	* extern int G_displayel;
	* extern int G_stretch, G_shrink;
	* extern long V_xmin, V_ymin;
	*/
	#include "options.h"

	/*
	* fisheye.c
	* 
	* extern void set_fe_scaling(int stretch, int shrink);
	* extern void change_fe_scaling(int stretch, int shrink);
	* extern int incr_fe_focus(long dx, long dy);
	* extern int normal_fe_focus(void);
	* extern long fe_scaling;
	* extern long gfocus_x, gfocus_y;
	* extern int fisheye_view;
	*/
	#include "fisheye.h"

	/*
	* draw.c
	* 
	* extern void draw_main(); 
	*/
	#include "draw.h"


	#include "steps.h"

#endif /* VCGLIB_H  */

#ifdef __cplusplus
}
#endif