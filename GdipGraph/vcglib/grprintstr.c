/* SCCS-info %W% %E% */

/*--------------------------------------------------------------------*/
/*								      */
/*		VCG : Visualization of Compiler Graphs		      */
/*		--------------------------------------		      */
/*								      */
/*   file:	   grprintstr.c					      */
/*   version:	   1.00.00					      */
/*   creation:	   31.1.95					      */
/*   author:	   G. Sander (Version 1.00.00-...)		      */
/*		   Universitaet des Saarlandes, 66041 Saarbruecken    */
/*		   ESPRIT Project #5399 Compare 		      */
/*   description:  Print bitmap into a file, String Character Rout.   */
/*   status:	   in work					      */
/*								      */
/*--------------------------------------------------------------------*/

//
//#ifndef lint
//static char *id_string="$Id: grprintstr.c,v 1.1 1995/02/08 11:11:14 sander Exp $";
//#endif


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
  * $Log: grprintstr.c,v $
  * Revision 1.1  1995/02/08  11:11:14  sander
  * Initial revision
  *
  */

  /************************************************************************
   *  This file allows to write the graph bitmap into a file. These routines
   *  are called, if the "export"-menu was selected. The supported formats
   *  are pbm-P4 format and ppm-P6 format.
   *  This is the string character library and is compatible to drawstr.c.
   ************************************************************************/



	/* Includes, Prototypes, Variables
	 * ===============================
	 * We must include globals.h before everything, otherwise we cannot
	 * define the prototypes.
	 * Even if globals.h is included in draw.c and drawlib.c again, this
	 * is correct, because of the include-security mechanism, that prevents
	 * second inclusion of a file from having ugly effects.
	 */

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void bm_line(int fx, int fy, int tx, int ty, int c);

//#include "drawstr.c"
#include "drawstr.h"
#include "grprint.h"

/*--------------------------------------------------------------------*/

