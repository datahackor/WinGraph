/* SCCS-info %W% %E% */

/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */ 
/*              --------------------------------------                */ 
/*                                                                    */
/*   file:         drawlib.h                                          */
/*   version:      1.00.00                                            */
/*   creation:     14.4.93                                            */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */ 
/*                 G. Sander (Version 1.00.00-...)                    */ 
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */ 
/*   description:  Library of drawing routines                        */ 
/*                 of edges                                           */
/*   status:       in work                                            */
/*                                                                    */
/*--------------------------------------------------------------------*/

/* $Id: drawlib.h,v 3.6 1995/02/08 11:11:14 sander Exp $ */

/*
 *   Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                               the Compare Consortium 
 *
 *  This program and documentation is free software; you can redistribute 
 *  it under the terms of the  GNU General Public License as published by
 *  the  Free Software Foundation;  either version 2  of the License,  or
 *  (at your option) any later version.
 *
 *  This  program  is  distributed  in  the hope that it will be useful,
 *  but  WITHOUT ANY WARRANTY;  without  even  the  implied  warranty of
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
 *  GNU General Public License for more details.
 *
 *  You  should  have  received a copy of the GNU General Public License
 *  along  with  this  program;  if  not,  write  to  the  Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  The software is available per anonymous ftp at ftp.cs.uni-sb.de.
 *  Contact  sander@cs.uni-sb.de  for additional information.
 */


/*
 * $Log: drawlib.h,v $
 * Revision 3.6  1995/02/08  11:11:14  sander
 * Distribution version 1.3.
 *
 * Revision 3.5  1994/12/23  18:12:45  sander
 * Manhatten layout added.
 * Option interface cleared.
 * infobox behaviour improved.
 * First version of fisheye (carthesian).
 * Options Noedge and nonode.
 * Titles in the node title box are now sorted.
 * Timelimit functionality improved.
 *
 * Revision 3.4  1994/06/07  14:09:59  sander
 * Splines implemented.
 * HP-UX, Linux, AIX, Sun-Os, IRIX compatibility tested.
 * The tool is now ready to be distributed.
 *
 * Revision 3.3  1994/05/16  08:56:03  sander
 * shape attribute (boxes, rhombs, ellipses, triangles) added.
 *
 * Revision 3.2  1994/05/05  12:03:00  sander
 * Follow edge action speedup by a special drawing routine.
 *
 * Revision 3.1  1994/03/01  10:59:55  sander
 * Copyright and Gnu Licence message added.
 * Problem with "nearedges: no" and "selfloops" solved.
 *
 * Revision 2.3  1994/01/21  19:33:46  sander
 * VCG Version tested on Silicon Graphics IRIX, IBM R6000 AIX and Sun 3/60.
 * Option handling improved. Option -grabinputfocus installed.
 * X11 Font selection scheme implemented. The user can now select a font
 * during installation.
 * Sun K&R C (a nonansi compiler) tested. Some portabitility problems solved.
 *
 * Revision 2.2  1994/01/03  15:29:06  sander
 * First complete X11 version.
 *
 */

#ifndef DRAWLIB_H
#define DRAWLIB_H

#ifdef __cplusplus
extern "C" {
#endif 

/*--------------------------------------------------------------------*/

/* See drawlib.c for explanation
 * -----------------------------
 */


/* Global Variables
 * ----------------
 */

extern int 	gs_actbackground;
extern int 	gs_stringw; 
extern int 	gs_stringh; 
extern int	gs_boxw;
extern int	gs_boxh;


/* Prototypes
 * ----------
 */

void 	gs_printstr(char *s,int c);
void 	gs_calcstringsize(char *s);
void 	gs_setto(int x,int y);
void 	gs_setshrink(int a,int b);
void 	gs_calctextboxsize(GNODE v);
void 	gs_textbox(GNODE v);
void 	gs_reverttextbox(GNODE v);
void 	gs_halfreverttextbox(GNODE v);
void 	gs_stringbox(GNODE v);
void 	gs_calcrhombsize(GNODE v);
void 	gs_rhomb(GNODE v);
void 	gs_revertrhomb(GNODE v);
void 	gs_halfrevertrhomb(GNODE v);
void 	gs_calctrianglesize(GNODE v);
void 	gs_triangle(GNODE v);
void 	gs_reverttriangle(GNODE v);
void 	gs_halfreverttriangle(GNODE v);
void 	gs_calcellipsesize(GNODE v);
void 	gs_ellipse(GNODE v);
void 	gs_revertellipse(GNODE v);
void 	gs_halfrevertellipse(GNODE v);
void 	gs_anchornode(GNODE v);
void	gs_solidarrow(GEDGE e);
void	gs_dashedarrow(GEDGE e);
void	gs_dottedarrow(GEDGE e);



/*--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif 

 
#endif /* DRAWLIB_H  */


