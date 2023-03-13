
/*--------------------------------------------------------------------*/
/*                                                                    */
/*              VCG : Visualization of Compiler Graphs                */
/*              --------------------------------------                */
/*                                                                    */
/*   file:         globals.h                                          */
/*   version:      1.00.00                                            */
/*   creation:     1.4.1993                                           */
/*   author:       I. Lemke  (...-Version 0.99.99)                    */
/*                 G. Sander (Version 1.00.00-...)                    */
/*                 Universitaet des Saarlandes, 66041 Saarbruecken    */
/*                 ESPRIT Project #5399 Compare                       */
/*   description:  Global definitions and setup                       */
/*   status:       in work                                            */
/*                                                                    */
/*--------------------------------------------------------------------*/


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


/* $Log: globals.h.tpl,v $
# Revision 3.9  1995/02/08  19:42:35  sander
# We need OWN_QUITSORT, since otherwisem we are too incompatible.
#
# Revision 3.8  1995/02/08  11:11:14  sander
# Distribution version 1.3.
#
# Revision 3.7  1994/11/23  14:50:47  sander
# #include <limits.h> added to get MAX_INT and MIN_INT.
#
# Revision 3.6  1994/08/08  16:01:47  sander
# Attributes xraster, xlraster, yraster added.
#
# Revision 3.5  1994/08/02  15:36:12  sander
# CHECKNODE option added to allow tracing of properties
# of one single node. For debugging only.
#
# Revision 3.4  1994/06/07  14:09:59  sander
# Splines implemented.
# HP-UX, Linux, AIX, Sun-Os, IRIX compatibility tested.
# The tool is now ready to be distributed.
#
# Revision 3.3  1994/05/16  08:56:03  sander
# shape attribute (boxes, rhombs, ellipses, triangles) added.
#
# Revision 3.2  1994/03/02  11:48:54  sander
# Layoutalgoritms mindepthslow, maxdepthslow, minindegree, ... mandegree
# added.
# Anchors and nearedges are not anymore allowed to be intermixed.
# Escapes in strings are now allowed.
#
# Revision 3.1  1994/03/01  10:59:55  sander
# Copyright and Gnu Licence message added.
# Problem with "nearedges: no" and "selfloops" solved.
#
# Revision 1.2  1994/01/21  19:43:42  sander
# New started with release 1.2 from the scratch.
# Old and useless comments removed.
#
 *
 */

/*   This file contains the setup of the tool for the C compiler.
 *   It is included into every other C file.
 *   Please adapt corresponding to your configuration.
 */

#ifndef GLOBALS_H
#define	GLOBALS_H


#pragma warning(disable:4005)

/*   The Gnu CC sometimes does not have a standard library.
 *   In this case, the include files might be wrong.
 *   To get fewer messages on -Wall, define NO_STDINCLUDES
 *   with Gnu CC.
 */

#undef NO_STDINCLUDES


/*   User Signals available or not ?
 *   Unix provides signals SIGUSR1 and SIGUSR2 (30, 31), which are used
 *   to control animations from external. If signal() or free signals are
 *   not available (e.g. on Non-Unix systems), then undefine USR_SIGNAL.
 */

//#define USR_SIGNAL
#undef  USR_SIGNAL 


/* Good quicksort available or not ?
 * If there exist a good randomized quicksort `qsort' in the library,
 * please use it. But if the quicksort in the library is slow, or not
 * randomized, define OWN_QUICKSORT. We often have to sort presorted
 * arrays.
 * Note: If OWN_QUICKSORT is defined, not all calls of `qsort' are
 * exchanged, but only the critical ones.
 */

/* #undef OWN_QUICKSORT  */
#define OWN_QUICKSORT




/*   Required alignment for structs (power of 2). IALIGN is the appropriate
 *   bit mask to generate the alignment. Because we use floats in structs,
 *   mostly an alignment of 8 is appropriate.
 */

#ifndef ALIGN
/* #define ALIGN  8 */
#define ALIGN  8
#define IALIGN (ALIGN-1)
#endif


/*   Memory block size for memory allocation in bytes. The allocation
 *   mechanism allocates blocks of this size, and dynamically increases
 *   the number of blocks, if more memory is necessary.
 *   Do not set the blocksize too small. For instance the tool does not
 *   work with blockssize less than 1 KB. Propose for good performance:
 *   256 KB minimal.
 */

/* Blocks of 1 Megabyte */

/* #define MEMBLOCKSIZE 1048576 */
#define MEMBLOCKSIZE 1048576


/*   Debugging On or Off ?
 *   We have three kinds of debugging:
 *      a) Behaviour tracing: a lot of messages nearly on every
 *         function entry are produced if _DEBUG is switched on.
 *      b) Assertion checking: only if an assertion fails, an
 *         appropriate message is produced.
 *      c) Node tracing: This is basically the same as the behaviour
 *	   tracing. At each function entry, the status of a node
 *	   will be printed. This happens if CHECKNODE is defined.
 *	   See step0 for the selection of the node to be traced.
 *   Behaviour or node tracing is only needed in very serious cases.
 *   The macros "debuggingmessage" can be redefined to do some special
 *   tests on every entry.
 *   Assertion checking can always be done, because it is silent
 *   as long as no error occurs. But it consumes runtime.
 *   This all can also be defined in the head of a module before
 *   #include "globals.h", to debug only this module.
 */

/* for internal debugging */
#undef _DEBUG
#undef CHECKNODE
/* for assertion checking at runtime */
#define CHECK_ASSERTIONS


/*
 *  If you prefer the system assert.h, then define ASSERT_AVAIL.
 */

#define ASSERT_AVAIL   
//#undef  ASSERT_AVAIL

/*  Check and printout the timing of phases.
 *  If CHECK_TIMING is defined, time measurement is done at some
 *  critical points. This is only for me, to fine tune the
 *  preformance. (GS)
 */

#undef CHECK_TIMING

/*---------------------------- End of Changes ------------------------*/

/* Maximal and minimal positive integer */

#include <limits.h>


#ifndef MAXINT
#ifdef INT_MAX
#define MAXINT  INT_MAX
#else
#define MAXINT  32767
#endif
#endif

#ifndef MININT
#ifdef INT_MIN
#define MININT  INT_MIN
#else
#define MININT  -32767
#endif
#endif

#ifndef MAXLONG
#ifdef LONG_MAX
#define MAXLONG LONG_MAX
#else
#define MAXLONG 32767
#endif
#endif

#ifndef MINLONG
#ifdef LONG_MIN
#define MINLONG LONG_MIN
#else
#define MINLONG -32767
#endif
#endif

#ifndef NULL
#define NULL	0
#endif

/* Debugging messages */

#ifdef _DEBUG
#define debugmessage(a,b) {FPRINTF(stderr,"Debug: %s %s\n",a,b);}
#else
#define debugmessage(a,b) 
#endif

#ifdef CHECKNODE
#define _DEBUG
#undef debugmessage
#define debugmessage(a,b) { \
	FPRINTF(stderr,"Debug: %s %s &",a,b); \
	if (debug_checknode)  \
		FPRINTF(stderr,"Checknode: |%s|\n",NTITLE(debug_checknode)); \
	FPRINTF(stderr,"\n"); \
}
#endif

/* Assertions */

#ifndef CHECK_ASSERTIONS
#define NDEBUG
#endif
#ifdef _DEBUG
#undef NDEBUG
#endif

#ifdef ASSERT_AVAIL
#include <assert.h>
#else
# ifndef NDEBUG
# define assert(ex)  {if (!(ex)) {\
   FPRINTF(stderr,"Assertion failed: file \"%s\", line %d\n",\
         __FILE__, __LINE__); \
   FPRINTF(stderr,"This is a bug! Please contact sander@cs.uni-sb.de !\n");\
   exit(1);}}
# else
# define assert(ex)
# endif
# endif


/* To make lint happy */

#define PRINTF  printf
#define FPRINTF fprintf
#define SPRINTF sprintf
#define FCLOSE  fclose
#define FFLUSH  fflush
#define FREE    free

#define gstoint(x) ((int)(x))


#endif /* GLOBALS_H */

