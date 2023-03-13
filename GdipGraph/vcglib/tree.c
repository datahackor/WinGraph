
/*
 *   Copyright (C) 1993--1995 by Georg Sander, Iris Lemke, and
 *                               the Compare Consortium
 *
 *  This program and documentation is free software;you can redistribute
 *  it under the terms of the  GNU General Public License as published by
 *  the  Free Software Foundation; either version 2  of the License,  or
 *  (at your option) any later version.
 *
 *  This  program  is  distributed  in  the hope that it will be useful,
 *  but  WITHOUT ANY WARRANTY; without  even  the  implied  warranty of
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
 *  GNU General Public License for more details.
 *
 *  You  should  have  received a copy of the GNU General Public License
 *  along  with  this  program; if  not,  write  to  the  Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  The software is available per anonymous ftp at ftp.cs.uni-sb.de.
 *  Contact  sander@cs.uni-sb.de  for additional information.
 */




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "alloc.h"
#include "main.h"
#include "options.h"
#include "folding.h"
#include "steps.h"
#include "timing.h"


static int gs_ide2009(void);
static int gs_ide2008(GNODE v);
static void gs_ide2000(void);
static void gs_ide2004(void);
static void gs_ide2015(void);
static int gs_ide2001(const GNODE* a, const GNODE* b);
static void gs_ide2013(void);
static void gs_ide2018(void);
static int gs_ide2005(GNODE v, int l);
static void gs_ide2002(GNODE conn, GNODE v);
static int gs_ide2011(GNODE v, GNODE conn);
static int gs_ide2010(GNODE v, int depth);
static void gs_ide2003(GNODE v, int diff);
static int gs_ide2012;
static GNODE* gs_ide2014 = NULL;
int spread_level = 1;
double tree_factor = 0.5;
static int gs_ide2016;
static int gs_ide2017;
int tree_main(void)
{
	start_time();

	assert((layer));
	if (tree_factor < 0) tree_factor = -tree_factor;
	if (tree_factor > 1.0) {
		gs_ide2017 = (int)(10.0 / tree_factor);
		gs_ide2016 = 10;
	}
	else {
		gs_ide2016 = (int)(tree_factor * 10.0);
		gs_ide2017 = 10;
	} if ((gs_ide2016 == 0) || (gs_ide2017 == 0)) {
		tree_factor = 0.5;
		gs_ide2016 = 1;
		gs_ide2017 = 2;
	}
	gs_wait_message('T');
	if (!gs_ide2009()) {
		stop_time("tree_main");
		return(0);
	}
	gs_wait_message('T');
	gs_ide2000();
	gs_ide2004();
	calc_all_node_sizes();
	alloc_levelshift();
	gs_ide2014 = (GNODE*)tpred_connection1;
	gs_ide2018();
	gs_ide2015();
	gs_wait_message('T');
	gs_ide2013();
	calc_all_ports(1);
	nr_crossings = 0;
	stop_time("tree_main");
	return(TREE_LAYOUT);
}
#define forward_connection1(c)  (((( c )->edge)  )&& ((( (( c )->edge)   )->end)  !=v))
#define forward_connection2(c)  (((( c )->edge2)  )&&((( (( c )->edge2)   )->end)  !=v))
#define backward_connection1(c) (((( c )->edge)  )&& ((( (( c )->edge)   )->end)  ==v))
#define backward_connection2(c) (((( c )->edge2)  )&&((( (( c )->edge2)   )->end)  ==v))
static int gs_ide2009(void)
{
	int i;
	int result;
	int fresh;
	GNODE v;
	GNLIST li;
	CONNECT c;

	v = nodelist;
	while (v) {
		v->markiert = 1;
		v = v->next;
	} v = labellist;
	while (v) {
		v->markiert = 1;
		v = v->next;
	} v = dummylist;
	while (v) {
		v->markiert = 1;
		v = v->next;
	}

	for (i = 0; i <= maxdepth + 1; i++) {
		li = ((layer[i]).succlist);
		while (li) {
			v = li->node;
			fresh = v->markiert;
			c = v->connection;
			if (c) {
				if (backward_connection1(c)) fresh = 0;
				if (backward_connection2(c)) fresh = 0;
			} result = 0;
			if (fresh) result = gs_ide2008(v);
			if (result) return(0);
			li = li->next;
		}
	} return(1);
}
static int gs_ide2008(GNODE v)
{
	int i;
	ADJEDGE a;
	CONNECT c;
	;
	if (!v->markiert) return(1);
	v->markiert = 0;
	i = 0;
	a = v->pred;
	while (a) {
		i++;
		a = a->next;
	} if (i > 1) return(1);
	i = 0;
	a = v->succ;
	while (a) {
		i++;
		if (gs_ide2008(a->kante->end)) return(1);
		a = a->next;
	} c = v->connection;
	if (c) {
		if (forward_connection1(c)) {
			if (gs_ide2008(c->edge->end)) return(1);
		} if (forward_connection2(c)) {
			if (gs_ide2008(c->edge2->end)) return(1);
		}
	} return(0);
}
static void gs_ide2000(void)
{
	int i, j;
	GNODE v;
	GNLIST li;
	ADJEDGE a;
	;
	gs_ide2012 = 0;
	for (i = 0; i <= maxdepth + 1; i++) {
		li = ((layer[i]).succlist);
		while (li) {
			v = li->node;
			j = 0;
			a = v->pred;
			while (a) {
				j++;
				a = a->next;
			} v->indegree = j;
			assert((j <= 1));
			j = 0;
			a = v->succ;
			while (a) {
				j++;
				a = a->next;
			} v->outdegree = j;
			if (j > gs_ide2012) gs_ide2012 = j;
			li = li->next;
		}
	} maxindeg = 1;
	maxoutdeg = gs_ide2012;
}
static void gs_ide2004(void)
{
	int i, k;
	GNLIST h1, h2;
	;
	max_nodes_per_layer = 0;
	for (i = 0; i <= maxdepth + 1; i++) {
		h1 = ((layer[i]).succlist);
		((layer[i]).predlist) = NULL;
		k = 0;
		while (h1) {
			k++;
			h2 = tmpnodelist_alloc();
			h2->next = ((layer[i]).predlist);
			((layer[i]).predlist) = h2;
			h2->node = h1->node;
			h1 = h1->next;
		} ((layer[i]).anz) = k;
		if (k > max_nodes_per_layer) max_nodes_per_layer = k;
	}
}
static void gs_ide2015(void)
{
	int i, k, max;
	GNLIST h1;
	;
	for (i = 0; i <= maxdepth + 1; i++) {
		h1 = ((layer[i]).succlist);
		k = 0;
		while (h1) {
			gs_ide2014[k++] = h1->node;
			h1 = h1->next;
		} max = k;
		if (k) qsort(gs_ide2014, k, sizeof(GNODE), (int (*) (const void*, const void*))gs_ide2001);
		h1 = ((layer[i]).succlist);
		k = 0;
		while (h1) {
			((gs_ide2014[k])->position) = k;
			h1->node = gs_ide2014[k++];
			h1 = h1->next;
		} assert((k == max));
		k--;
		h1 = ((layer[i]).predlist);
		while (h1) {
			h1->node = gs_ide2014[k--];
			h1 = h1->next;
		} assert((k == -1));
	}
}
static int gs_ide2001(const GNODE* a, const GNODE* b)
{
	if (((*a)->xloc) > ((*b)->xloc)) return(1);
	if (((*a)->xloc) < ((*b)->xloc)) return(-1);
	return(0);
}
static void gs_ide2013(void)
{
	int i;
	GNLIST h1;
	ADJEDGE a;
	;
	for (i = 0; i <= maxdepth + 1; i++) {
		if (i > 0) {
			h1 = ((layer[i - 1]).succlist);
			while (h1) {
				h1->node->tmpadj = h1->node->succ;
				h1 = h1->next;
			}
		} if (i < maxdepth + 1) {
			h1 = ((layer[i + 1]).succlist);
			while (h1) {
				h1->node->tmpadj = h1->node->pred;
				h1 = h1->next;
			}
		} h1 = ((layer[i]).succlist);
		while (h1) {
			a = h1->node->pred;
			while (a) {
				assert(a->kante->start->tmpadj);
				a->kante->start->tmpadj->kante = a->kante;
				a->kante->start->tmpadj = a->kante->start->tmpadj->next;
				a = a->next;
			} a = h1->node->succ;
			while (a) {
				assert(a->kante->end->tmpadj);
				a->kante->end->tmpadj->kante = a->kante;
				a->kante->end->tmpadj = a->kante->end->tmpadj->next;
				a = a->next;
			} h1 = h1->next;
		}
	} for (i = 0; i <= maxdepth + 1; i++) {
		h1 = ((layer[i]).succlist);
		while (h1) {
			h1->node->predleft = h1->node->predright = 0;
			a = h1->node->pred;
			if (a) {
				h1->node->predleft = a->kante;
				while (a->next) a = a->next;
				h1->node->predright = a->kante;
			} h1->node->succleft = h1->node->succright = 0;
			a = h1->node->succ;
			if (a) {
				h1->node->succleft = a->kante;
				while (a->next) a = a->next;
				h1->node->succright = a->kante;
			} h1 = h1->next;
		}
	}
} static int gs_ide2006;
#define xralign(a)  ((((a)+G_xraster-1)/G_xraster)*G_xraster)
#define xlalign(a)  ((((a)            )/G_xraster)*G_xraster)
#define dxralign(a) ((((a)+G_dxraster-1)/G_dxraster)*G_dxraster)
#define yralign(a)  ((((a)+G_yraster-1)/G_yraster)*G_yraster)
static void gs_ide2018(void)
{
	int actypos;
	int maxboxheight;
	int i, fresh;
	GNLIST li;
	GNODE v;
	CONNECT c;
	;
	gs_ide2006 = 0;
	if (G_yspace < 5) G_yspace = 5;
	if (G_xspace < 5) G_xspace = 5;
	if (G_dspace == 0) {
		if (G_spline) G_dspace = 4 * G_xspace / 5;
		else G_dspace = G_xspace / 2;
	} if (G_flat_factor < 1) G_flat_factor = 1;
	if (G_flat_factor > 100) G_flat_factor = 100;
	actypos = yralign(G_ybase);
	for (i = 0; i <= maxdepth + 1; i++) {
		((layer[i]).actx) = G_xbase;
		maxboxheight = 0;
		li = ((layer[i]).succlist);
		while (li) {
			li->node->yloc = actypos;
			if (maxboxheight < li->node->height) maxboxheight = li->node->height;
			li = li->next;
		}  if (G_yalign == 1) {
			li = ((layer[i]).succlist);
			while (li) {
				li->node->yloc += (maxboxheight - li->node->height) / 2;
				li = li->next;
			}
		}
		else if (G_yalign == 2) {
			li = ((layer[i]).succlist);
			while (li) {
				li->node->yloc += (maxboxheight - li->node->height);
				li = li->next;
			}
		} actypos += (maxboxheight + G_yspace);
		actypos = yralign(actypos);
	} v = nodelist;
	while (v) {
		v->markiert = 1;
		v = v->next;
	} v = labellist;
	while (v) {
		v->markiert = 1;
		v = v->next;
	} v = dummylist;
	while (v) {
		v->markiert = 1;
		v = v->next;
	} gs_ide2012 = 0;
	for (i = 0; i <= maxdepth + 1; i++) {
		li = ((layer[i]).succlist);
		while (li) {
			v = li->node;
			fresh = v->markiert;
			c = v->connection;
			if (c) {
				if (backward_connection1(c)) fresh = 0;
				if (backward_connection2(c)) fresh = 0;
			} if (fresh) {
				gs_wait_message('T');
				(void)gs_ide2005(v, G_xbase + v->width / 2);
			} li = li->next;
		}
	}
}
static int gs_ide2005(GNODE v, int leftest_pos)
{
	int xpos, minpos, maxpos, l, num, i;
	ADJEDGE a;
	int minhorder, maxhorder;
	GNODE w, actl, actr, actn;
	GNODE conn1, conn2, leftconn;
	CONNECT c;
	;
	if (leftest_pos < 0) leftest_pos = 0;
	if (!v) return(leftest_pos);
	xpos = leftest_pos;
	v->markiert = 0;
	if ((v->nhorder != -1) && (!gs_ide2006)) {
		gs_ide2006 = 1;
		if (!silent) {
			FPRINTF(stderr, "Note: On tree layout ");
			FPRINTF(stderr, "the attribute `horizontal_order' ");
			FPRINTF(stderr, "sorts only the child nodes\n");
			FPRINTF(stderr, "of a node locally, ");
			FPRINTF(stderr, "but not the whole layer.");
			FPRINTF(stderr, "\n");
		}
	} l = v->tiefe;
	conn1 = conn2 = 0;
	c = v->connection;
	if (c) {
		if (forward_connection1(c)) {
			conn1 = c->edge->end;
		} if (forward_connection2(c)) {
			conn2 = c->edge2->end;
		}
	} if (conn1 && conn2) {
		if (conn1->nhorder == -1) {
			if (conn2->nhorder < v->nhorder)  gs_ide2005(conn2, xpos - v->width / 2 - conn2->width / 2 - G_xspace);
			else gs_ide2005(conn1, xpos - v->width / 2 - conn1->width / 2 - G_xspace);
		}
		else if (conn1->nhorder == v->nhorder) {
			if (conn2->nhorder < v->nhorder)  gs_ide2005(conn2, xpos - v->width / 2 - conn2->width / 2 - G_xspace);
			else gs_ide2005(conn1, xpos - v->width / 2 - conn1->width / 2 - G_xspace);
		}
		else if (conn1->nhorder < v->nhorder) {
			gs_ide2005(conn1, xpos - v->width / 2 - conn1->width / 2 - G_xspace);
		}
		else {
			if (conn2->nhorder < conn1->nhorder)  gs_ide2005(conn2, xpos - v->width / 2 - conn2->width / 2 - G_xspace);
			else gs_ide2005(conn1, xpos - v->width / 2 - conn1->width / 2 - G_xspace);
		}
	}
	else if (conn1) {
		if (conn1->nhorder != -1) {
			if (conn1->nhorder < v->nhorder) {
				gs_ide2005(conn1, xpos - v->width / 2 - conn1->width / 2 - G_xspace);
			}
		}
	}
	else if (conn2) {
		if (conn2->nhorder != -1) {
			if (conn2->nhorder < v->nhorder) {
				gs_ide2005(conn2, xpos - v->width / 2 - conn2->width / 2 - G_xspace);
			}
		}
	} xpos = leftest_pos;
	if (xpos - v->width / 2 < ((layer[l]).actx))  xpos = ((layer[l]).actx) + v->width / 2;
	switch (v->outdegree) {
	case 0:  break;
	case 1: a = v->succ;
		assert((a));
		minpos = gs_ide2005(a->kante->end, xpos);
		xpos = minpos;
		break;
	default: a = v->succ;
		actl = NULL;
		minhorder = MAXINT;
		while (a) {
			w = a->kante->end;
			if (w->nhorder < minhorder) {
				minhorder = w->nhorder;
				actl = w;
			} a = a->next;
		} a = v->succ;
		actr = NULL;
		maxhorder = MININT;
		while (a) {
			w = a->kante->end;
			if ((w != actl) && (w->nhorder > maxhorder)) {
				maxhorder = w->nhorder;
				actr = w;
			} a = a->next;
		} assert((actl) && (actr) && (actl != actr));
		a = v->succ;
		num = 0;
		while (a) {
			num += a->kante->end->width;
			a = a->next;
		} num += ((v->outdegree - 1) * G_xspace);
		num -= actl->width / 2;
		num -= actr->width / 2;
		minpos = gs_ide2005(actl, xpos - (num * gs_ide2016) / gs_ide2017);
		a = v->succ;
		actn = NULL;
		minhorder = MAXINT;
		while (a) {
			w = a->kante->end;
			if ((w != actr) && w->markiert && (w->nhorder < minhorder)) {
				minhorder = w->nhorder;
				actn = w;
			} a = a->next;
		} i = 2;
		while (actn) {
			if (l > spread_level)  maxpos = gs_ide2005(actn, minpos + (2 * (i - 1) * (xpos - minpos)) / (v->outdegree - 1));
			else {
				if (i <= (v->outdegree + 1) / 2)  maxpos = gs_ide2005(actn, minpos + (2 * (i - 1) * (xpos - minpos)) / (v->outdegree - 1));
				else maxpos = gs_ide2005(actn, xpos);
			} if (l > spread_level) {
				xpos = minpos + (v->outdegree - 1) * (maxpos - minpos) / (2 * (i - 1));
				if (xpos < leftest_pos) xpos = leftest_pos;
			} i++;
			a = v->succ;
			actn = NULL;
			minhorder = MAXINT;
			while (a) {
				w = a->kante->end;
				if ((w != actr) && w->markiert && (w->nhorder < minhorder)) {
					minhorder = w->nhorder;
					actn = w;
				} a = a->next;
			}
		} if (l > spread_level)  maxpos = gs_ide2005(actr, minpos + (2 * (i - 1) * (xpos - minpos)) / (v->outdegree - 1));
		else {
			if (i <= (v->outdegree + 1) / 2)   maxpos = gs_ide2005(actr, minpos + (2 * (i - 1) * (xpos - minpos)) / (v->outdegree - 1));
			else maxpos = gs_ide2005(actr, xpos);
		} if (l > spread_level) {
			xpos = minpos + (v->outdegree - 1) * (maxpos - minpos) / (2 * (i - 1));
			if (xpos < leftest_pos) xpos = leftest_pos;
		} xpos = (minpos + maxpos) / 2;
		if (xpos - v->width / 2 < ((layer[l]).actx))  xpos = ((layer[l]).actx) + v->width / 2;
	} v->xloc = xpos - v->width / 2;
	if ((v->width == 0) && (v->height == 0)) v->xloc = dxralign(v->xloc + v->width / 2) - v->width / 2;
	else v->xloc = xralign(v->xloc + v->width / 2) - v->width / 2;
	((layer[l]).actx) = v->xloc + v->width + G_xspace;
	leftconn = 0;
	if (conn1 && (!conn1->markiert)) leftconn = conn1;
	if (conn2 && (!conn2->markiert)) leftconn = conn2;
	if (conn1 && conn1->markiert) (void)gs_ide2005(conn1, ((layer[l]).actx));
	if (conn2 && conn2->markiert) (void)gs_ide2005(conn2, ((layer[l]).actx));
	if (leftconn) gs_ide2002(leftconn, v);
	return(xpos);
}
#define TMINX(x) (( x ).cross)  
static void gs_ide2002(GNODE conn, GNODE v)
{
	int i, depth, diff;
	;
	gs_wait_message('T');
	assert((v->xloc > conn->xloc));
	for (i = 0; i <= maxdepth + 1; i++)  TMINX(layer[i]) = MAXINT;
	depth = gs_ide2011(v, conn);
	diff = gs_ide2010(conn, depth);
	if (diff - G_xspace > 0) gs_ide2003(conn, xlalign(diff - G_xspace));
}
static int gs_ide2011(GNODE v, GNODE conn)
{
	int maxlevel, l, h;
	ADJEDGE a;
	CONNECT c;
	;
	if (v == conn) return(0);
	maxlevel = l = v->tiefe;
	if (v->xloc < TMINX(layer[l])) TMINX(layer[l]) = v->xloc;
	a = v->succ;
	while (a) {
		h = gs_ide2011(a->kante->end, conn);
		if (h > maxlevel) maxlevel = h;
		a = a->next;
	} c = v->connection;
	if (c) {
		if (forward_connection1(c)) {
			h = gs_ide2011(c->edge->end, conn);
			if (h > maxlevel) maxlevel = h;
		} if (forward_connection2(c)) {
			h = gs_ide2011(c->edge2->end, conn);
			if (h > maxlevel) maxlevel = h;
		}
	} return(maxlevel);
}
static int gs_ide2010(GNODE v, int depth)
{
	int mindiff, l, h;
	ADJEDGE a;
	CONNECT c;
	;
	l = v->tiefe;
	if (l > depth) return(0);
	mindiff = TMINX(layer[l]) - v->xloc - v->width;
	a = v->succ;
	while (a) {
		h = gs_ide2010(a->kante->end, depth);
		if (h < mindiff) mindiff = h;
		a = a->next;
	} c = v->connection;
	if (c) {
		if (forward_connection1(c)) {
			h = gs_ide2010(c->edge->end, depth);
			if (h < mindiff) mindiff = h;
		} if (forward_connection2(c)) {
			h = gs_ide2010(c->edge2->end, depth);
			if (h < mindiff) mindiff = h;
		}
	} return(mindiff);
}
static void gs_ide2003(GNODE v, int diff)
{
	ADJEDGE a;
	CONNECT c;
	;
	v->xloc += diff;
	a = v->succ;
	while (a) {
		gs_ide2003(a->kante->end, diff);
		a = a->next;
	} c = v->connection;
	if (c) {
		if (forward_connection1(c)) {
			gs_ide2003(c->edge->end, diff);
		} if (forward_connection2(c)) {
			gs_ide2003(c->edge2->end, diff);
		}
	}
}
