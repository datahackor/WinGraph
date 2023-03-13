#pragma once

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <float.h>

#include <vcglib.h>




// Public declaration goes here

#ifdef __cplusplus
extern "C" {
#endif

//#ifdef LIBGRAPH_EXPORTS
//#define LIBGRAPH_API __declspec(dllexport)
//#else
//#define LIBGRAPH_API __declspec(dllimport)
//#endif

	void __cdecl save_input_file_contents(FILE* fp);
	void __cdecl error(const char* format, ...);
	char* __cdecl qtmpnam(char* answer);
	int __cdecl setDrawingRectangle(int width, int height);
	
	void __cdecl drawGraph(/*Graphics*/void* g);
	void  __cdecl moveFocus(int dx, int dy);
	void __cdecl updateZoomStatus(void);
	void __cdecl fishEyeMode(int code);
	void __cdecl changeFeScaling(int stretch, int shrink);
	void __cdecl setFeScaling(int stretch, int shrink);
	void __cdecl displayCompleteGraph(void);
	int __cdecl normalFeFocus(void);
	void __cdecl updateOriginStatus(void);
	void __cdecl relayOut(void);
	int __cdecl vcgMain(int argc, char* argv[]);
	void __cdecl doStatistics(void);
	void __cdecl setNodesValue(const char* _nodes);

#ifdef __cplusplus
}
#endif

#endif



