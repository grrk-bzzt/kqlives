/*
   KQ is Copyright (C) 2002 by Josh Bolduc

   This file is part of KQ... a freeware RPG.

   KQ is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   KQ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KQ; see the file COPYING.  If not, write to
   the Free Software Foundation,
       675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "setup.h"
#include "entity.h"
#include "kq.h"
#include "draw.h"
#include "menu.h"
#include "sgame.h"
#include "mpcx.h"
#include "allegro.h"
#include "jgmod.h"
#include "res.h"
#include "progress.h"
#include "itemdefs.h"
#include "itemmenu.h"
#include "intrface.h"
#include "masmenu.h"

/*
   globals - and there's plenty of them!
*/
char curmap[16];
char icon_sets[6][16] =
   { "LAND_PCX", "NEWTOWN_PCX", "CASTLE_PCX", "INCAVE_PCX", "VILLAGE_PCX",
   "MOUNT_PCX"
};
int right, left, up, down, besc, balt, bctrl, benter;
int kright, kleft, kup, kdown, kesc, kenter, kalt, kctrl, jbalt, jbctrl,
   jbenter, jbesc;
int vx, vy, mx, my, steps = 0, lastm[PSIZE];
BITMAP *double_buffer, *map_icons[MAX_TILES];
BITMAP *back, *tc, *tc2, *bub[8], *b_shield, *b_shell, *b_repulse, *b_mp;
BITMAP *cframes[NUM_FIGHTERS][MAXCFRAMES],
   *tcframes[NUM_FIGHTERS][MAXCFRAMES], *frames[MAXCHRS][MAXFRAMES];
BITMAP *eframes[MAXE][MAXEFRAMES], *pgb[9], *sfonts[5], *bord[8];
BITMAP *menuptr, *mptr, *sptr, *stspics, *sicons, *bptr, *missbmp, *noway,
   *upptr, *dnptr;
BITMAP *shadow[MAX_SHADOWS];
unsigned short *map_seg, *b_seg, *f_seg;
unsigned char *progress, *z_seg, *s_seg, *o_seg, *treasure;
BITMAP *kfonts, *portrait[MAXCHRS];
s_map g_map;
s_entity g_ent[MAX_ENT + PSIZE];
s_anim tanim[6][MAX_ANIM] = {
   {{2, 5, 25}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{158, 159, 50}, {160, 163, 25}, {176, 179, 25}, {257, 258, 50},
    {262, 263, 25}},
   {{57, 58, 50}, {62, 63, 25}, {205, 206, 50}, {250, 253, 25}, {0, 0, 0}},
   {{30, 35, 30}, {176, 179, 25}, {323, 328, 40}, {380, 385, 40}, {0, 0, 0}},
   {{38, 39, 25}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
   {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
};
s_anim adata[MAX_ANIM];
int noe = 0, pidx[PSIZE], numchrs = 0, gp = 0, xofs, yofs, gsvol =
   250, gmvol = 250;
unsigned char autoparty = 0, autofollow = 1, alldead = 0, is_sound =
   1, deadeffect = 0, vfollow = 1, use_sstone = 0;
unsigned char kq_version = 91, hold_fade = 0, cansave = 0, skip_intro =
   0, wait_retrace = 0, windowed = 0, stretch_view = 0;
unsigned short tilex[MAX_TILES], adelay[MAX_ANIM];
char *strbuf;
s_player party[MAXCHRS];
s_player players[MAXCHRS] = {
   {"Sensar", 0, 70, 1, 100, 40, 40, 0, 0,
    {800, 500, 700, 300, 300, 4000, 9000, 9000, 0, 7500, 0, 100, 0}, {0, 0, 0,
								      0, 0, 0,
								      0, 0, 0,
								      0, 0, 0,
								      0, 0, 0,
								      0}},
   {"Sarina", 0, 70, 1, 100, 36, 36, 4, 4,
    {600, 600, 600, 400, 400, 6000, 9000, 9000, 0, 6000, 0, 100, 0}, {0, 0, 0,
								      5, 0, 0,
								      0, 0, 0,
								      0, 0, 0,
								      0, 0, 0,
								      0}},
   {"Corin", 0, 70, 1, 100, 25, 25, 15, 15,
    {400, 400, 500, 800, 400, 4500, 12000, 9000, 0, 5000, 0, 100, 0}, {0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0}},
   {"Ajathar", 0, 70, 1, 100, 30, 30, 8, 8,
    {600, 500, 500, 400, 800, 5000, 9000, 11000, 0, 5000, 0, 100, 0}, {0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0}},
   {"Casandra", 0, 70, 1, 100, 24, 24, 16, 16,
    {300, 600, 400, 800, 600, 5500, 10000, 10000, 0, 5000, 0, 100, 0}, {0, 0,
									0, 0,
									0, 0,
									0, 0,
									0, 0,
									8, 0,
									0, 0,
									8,
									0}},
   {"Temmin", 0, 70, 1, 100, 35, 35, 5, 5,
    {700, 500, 800, 300, 500, 5500, 8000, 10000, 0, 6000, 0, 100, 0}, {0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0}},
   {"Ayla", 0, 70, 1, 100, 32, 32, 8, 8,
    {500, 800, 500, 600, 400, 7000, 10000, 8000, 0, 5000, 0, 100, 0}, {0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0,
								       0, 0}},
   {"Noslom", 0, 70, 1, 65, 22, 22, 18, 18,
    {300, 600, 300, 700, 700, 5500, 10000, 10000, 0, 5000, 0, 100, 0}, {0, 0,
									0, 0,
									0, 0,
									0, 0,
									0, 0,
									0, 0,
									0, 0,
									0, 0}}
};
unsigned short lup[MAXCHRS][20] = {
   {10, 70, 9, 2, 190, 90, 150, 60, 70, 15, 20, 20, 50, 50, 0, 10, 0},
   {10, 70, 8, 4, 120, 120, 120, 90, 80, 10, 50, 45, 25, 45, 0, 25, 0},
   {10, 70, 6, 6, 90, 80, 100, 190, 80, 15, 100, 45, 10, 10, 0, 15, 0},
   {10, 70, 8, 5, 110, 90, 80, 80, 190, 15, 20, 100, 20, 15, 0, 10, 0},
   {10, 70, 8, 6, 50, 130, 50, 180, 120, 20, 100, 60, 0, 5, 0, 20, 0},
   {10, 70, 8, 3, 140, 90, 160, 50, 100, 15, 20, 70, 35, 35, 0, 15, 0},
   {10, 70, 7, 5, 110, 170, 90, 120, 70, 25, 50, 20, 25, 25, 0, 30, 0},
   {10, 70, 8, 6, 50, 100, 50, 160, 160, 10, 90, 90, 5, 0, 0, 20, 0}
};
s_fighter fighter[NUM_FIGHTERS];
s_fighter tempa, tempd;
int noi, shin[12], dct = 0;
char sname[39], ctext[39];
volatile int timer = 0, ksec = 0, kmin = 0, khr = 0, timer_count = 0;
COLOR_MAP cmap;
unsigned char can_run = 1, display_desc = 0;
unsigned char draw_background = 1, draw_middle = 1, draw_foreground =
   1, draw_shadow = 1;
unsigned short g_inv[MAX_INV][2];
int view_x1, view_y1, view_x2, view_y2, view_on = 0, in_combat = 0;
int frate, mfrate = 0, show_frate = 0, use_joy = 1, cheat_loaded = 0;

int cheat = 0;
int warx = 0, wary = 0;

/*
   New interrupt handler set to keep game time.
*/
void my_counter (void)
{
   timer++;
   if (timer >= 100)
     {
	timer = 0;
	ksec++;
	if (ksec >= 60)
	  {
	     ksec = 0;
	     kmin++;
	  }
	if (kmin >= 60)
	  {
	     kmin = 0;
	     khr++;
	  }
	mfrate = frate;
	frate = 0;
     }
   timer_count++;
}

END_OF_FUNCTION (my_counter);

/*
   Updates all of the game controls according to user input.
*/
void readcontrols (void)
{
   balt = 0;
   besc = 0;
   balt = 0;
   bctrl = 0;
   benter = 0;
   up = 0;
   down = 0;
   left = 0;
   right = 0;
   if (key[kup])
      up = 1;
   if (key[kdown])
      down = 1;
   if (key[kleft])
      left = 1;
   if (key[kright])
      right = 1;
   if (key[kesc])
      besc = 1;
   if (key[kalt])
      balt = 1;
   if (key[kctrl])
      bctrl = 1;
   if (key[kenter])
      benter = 1;
   if (key[KEY_ALT] && key[KEY_X])
      program_death ("X-ALT pressed... exiting.");
   if (key[KEY_F12])
     {
	data_dump ();
	while (key[KEY_F12])
	  {
	  }
     }
   if (use_joy > 0)
     {
	if (poll_joystick () == 0)
	  {
	     left = joy[use_joy - 1].stick[0].axis[0].d1;
	     right = joy[use_joy - 1].stick[0].axis[0].d2;
	     up = joy[use_joy - 1].stick[0].axis[1].d1;
	     down = joy[use_joy - 1].stick[0].axis[1].d2;
/*
      balt = joy[use_joy-1].button[0].b;
      bctrl = joy[use_joy-1].button[1].b;
      benter = joy[use_joy-1].button[2].b;
      besc = joy[use_joy-1].button[3].b;
*/
	  }
     }
}

void data_dump (void)
{
   FILE *ff;
   int a;
   PALETTE pal;
   ff = fopen ("treasure.log", "w");
   if (!ff)
      program_death ("could not open treasure.log!");
   for (a = 0; a < 200; a++)
      fprintf (ff, "%d = %d\n", a, treasure[a]);
   fclose (ff);
   ff = fopen ("progress.log", "w");
   if (!ff)
      program_death ("could not open treasure.log!");
   for (a = 0; a < 200; a++)
      fprintf (ff, "%d = %d\n", a, progress[a]);
   fclose (ff);
   get_palette (pal);
   save_bitmap ("snap.pcx", double_buffer, pal);
}

/*
   This is used to determine what part of the map is
   visible on the screen.  Usually, the party can walk around
   in the center of the screen a bit without causing it to
   scroll.  The centre parameter is mostly used for warps and
   such, so that the players start in the centre of the screen.
*/
void calc_viewport (int centre)
{
   int sx, sy, bl, br, bu, bd, zx, zy;

   if (vfollow && numchrs > 0)
     {
	zx = g_ent[0].x;
	zy = g_ent[0].y;
     }
   else
     {
	zx = vx;
	zy = vy;
     }
/*
  if (centre)
  {
*/
   bl = 152;
   br = 152;
   bu = 112;
   bd = 112;
/*  }
  else
  {
    bl = 64;
    br = 240;
    bu = 64;
    bd = 164;
  }
*/
   sx = zx - vx;
   sy = zy - vy;
   if (sx < bl)
     {
	vx = zx - bl;
	if (vx < 0)
	   vx = 0;
     }
   if (sy < bu)
     {
	vy = zy - bu;
	if (vy < 0)
	   vy = 0;
     }
   if (sx > br)
     {
	vx = zx - br;
	if (vx > mx)
	   vx = mx;
     }
   if (sy > bd)
     {
	vy = zy - bd;
	if (vy > my)
	   vy = my;
     }
   if (vx > mx)
      vx = mx;
   if (vy > my)
      vy = my;
}

/*
   This loads a new map and performs all of the functions
   that accompany the loading of a new map.
*/
void change_map (char *map_name, int msx, int msy, int mvx, int mvy)
{
   int i, o;
   PACKFILE *pf;
   DATAFILE *pb;
   BITMAP *pcxb;
   unsigned char cc[4];

   if (hold_fade == 0)
      fade_out (4);
   sprintf (strbuf, "%s%s.map", MAP_DIR, map_name);
   pf = pack_fopen (strbuf, F_READ_PACKED);
   if (!pf)
     {
	clear_bitmap (screen);
	clear_bitmap (double_buffer);
	if (hold_fade == 0)
	   fade_in (pal, 16);
	g_map.xsize = -1;
	sprintf (strbuf, "Could not load %s!", map_name);
	program_death (strbuf);
     }
   pack_fread (&g_map, sizeof (s_map), pf);
   pack_fread (&g_ent[PSIZE], sizeof (s_entity) * 50, pf);
   free (map_seg);
   map_seg = (unsigned short *) malloc (g_map.xsize * g_map.ysize * 2);
   free (b_seg);
   b_seg = (unsigned short *) malloc (g_map.xsize * g_map.ysize * 2);
   free (f_seg);
   f_seg = (unsigned short *) malloc (g_map.xsize * g_map.ysize * 2);
   free (z_seg);
   z_seg = (unsigned char *) malloc (g_map.xsize * g_map.ysize);
   free (s_seg);
   s_seg = (unsigned char *) malloc (g_map.xsize * g_map.ysize);
   free (o_seg);
   o_seg = (unsigned char *) malloc (g_map.xsize * g_map.ysize);
   pack_fread (map_seg, (g_map.xsize * g_map.ysize * 2), pf);
   pack_fread (b_seg, (g_map.xsize * g_map.ysize * 2), pf);
   pack_fread (f_seg, (g_map.xsize * g_map.ysize * 2), pf);
   pack_fread (z_seg, (g_map.xsize * g_map.ysize), pf);
   pack_fread (s_seg, (g_map.xsize * g_map.ysize), pf);
   pack_fread (o_seg, (g_map.xsize * g_map.ysize), pf);
   pack_fclose (pf);
   for (i = 0; i < g_map.xsize * g_map.ysize; i++)
     {
	if (map_seg[i] > 0)
	   cc[0] = 1;
	if (b_seg[i] > 0)
	   cc[1] = 1;
	if (f_seg[i] > 0)
	   cc[2] = 1;
	if (s_seg[i] > 0)
	   cc[3] = 1;
     }
   draw_background = cc[0];
   draw_middle = cc[1];
   draw_foreground = cc[2];
   draw_shadow = cc[3];
   for (i = 0; i < numchrs; i++)
     {
	if (msx == 0 && msy == 0)
	   place_ent (i, g_map.stx, g_map.sty);
	else
	   place_ent (i, msx, msy);
	lastm[i] = 0;
	g_ent[i].speed = 4;
	g_ent[i].obsmode = 1;
	g_ent[i].moving = 0;
     }
   for (i = 0; i < MAX_ENT; i++)
     {
	if (g_ent[i].chrx == 38 && g_ent[i].active == 1)
	  {
	     g_ent[i].eid = ID_ENEMY;
	     g_ent[i].speed = rand () % 4 + 1;
	     g_ent[i].obsmode = 1;
	     g_ent[i].moving = 0;
	     g_ent[i].movemode = 3;
	     g_ent[i].chasing = 0;
	     g_ent[i].extra = 50 + rand () % 50;
	     g_ent[i].delay = rand () % 25 + 25;
	  }
     }
   pb = load_datafile_object (PCX_DATAFILE, icon_sets[g_map.tileset]);
   pcxb = (BITMAP *) pb->dat;
   for (o = 0; o < pcxb->h / 16; o++)
      for (i = 0; i < pcxb->w / 16; i++)
	 blit ((BITMAP *) pb->dat, map_icons[o * (pcxb->w / 16) + i], i * 16,
	       o * 16, 0, 0, 16, 16);
   unload_datafile_object (pb);
   for (o = 0; o < MAX_ANIM; o++)
      adelay[o] = 0;
   play_song (g_map.song_file, 0);
   mx = g_map.xsize * 16 - 304;
   my = g_map.ysize * 16 - 224;
   if (mvx == 0 && mvy == 0)
     {
	vx = g_map.stx * 16;
	vy = g_map.sty * 16;
     }
   else
     {
	vx = mvx * 16;
	vy = mvy * 16;
     }
   calc_viewport (1);
   for (i = 0; i < MAX_TILES; i++)
      tilex[i] = i;
   for (i = 0; i < MAX_ANIM; i++)
      adata[i] = tanim[g_map.tileset][i];
   noe = 0;
   for (i = 0; i < numchrs; i++)
      g_ent[i].active = 1;
   count_entities ();
   for (i = 0; i < MAX_ENT; i++)
      g_ent[i].delayctr = 0;
   strcpy (curmap, map_name);
   set_view (0, 0, 0, 0, 0);
   if (strlen (g_map.map_desc) > 1)
      display_desc = 1;
   else
      display_desc = 0;
   do_luakill ();
   do_luainit (map_name);
   do_autoexec ();
   if (hold_fade == 0 && numchrs > 0)
     {
	drawmap ();
	blit2screen (xofs, yofs);
	fade_in (pal, 4);
     }
   use_sstone = g_map.use_sstone;
   cansave = g_map.can_save;
   timer_count = 0;
   timer = 0;
   do_postexec ();
   timer_count = 0;
   timer = 0;
}

/*
   This routine is called after every final step onto
   a new tile (not after warps or such things).  It
   just checks if the zone value for this co-ordinate is
   not zero and then it calls the event handler.  However,
   there is a member of the map structure called zero_zone
   that let's you call the event handler on 0 zones if you
   wish.
*/
void zone_check (void)
{
   unsigned short stc, zx, zy;
   zx = g_ent[0].x / 16;
   zy = g_ent[0].y / 16;

   if (progress[P_REPULSE] > 0)
     {
	if (g_map.map_no == MAP_MAIN)
	   progress[P_REPULSE]--;
	else
	  {
	     if (progress[P_REPULSE] > 1)
		progress[P_REPULSE] -= 2;
	     else
		progress[P_REPULSE] = 0;
	  }
	if (progress[P_REPULSE] < 1)
	   message ("Repulse has worn off!", 255, 0, xofs, yofs);
     }
   stc = z_seg[zy * g_map.xsize + zx];
   if (g_map.zero_zone != 0)
      do_zone (stc);
   else
     {
	if (stc > 0)
	   do_zone (stc);
     }
}

/*
   Fade out... change co-ordinates... fade in.
   The wtx/wty co-ordinates indicate where to put the player.
   The wvx/wvy co-ordinates indicate where to put the camera.
*/
void warp (int wtx, int wty, int fspeed)
{
   int i, f;

   if (hold_fade == 0)
      fade_out (fspeed);
   if (numchrs == 0)
      f = 1;
   else
      f = numchrs;
   for (i = 0; i < f; i++)
     {
	place_ent (i, wtx, wty);
	g_ent[i].moving = 0;
	g_ent[i].movcnt = 0;
	g_ent[i].framectr = 0;
	lastm[i] = 0;
     }
   vx = wtx * 16;
   vy = wty * 16;
   calc_viewport (1);
   drawmap ();
   blit2screen (xofs, yofs);
   if (hold_fade == 0)
      fade_in (pal, fspeed);
   timer_count = 0;
}

/*
   This updates tile indexes for animation threads.
*/
void check_animation (void)
{
   int i, j;

   for (i = 0; i < MAX_ANIM; i++)
     {
	if (adata[i].start != 0)
	  {
	     if (adata[i].delay && adata[i].delay < adelay[i])
	       {
		  adelay[i] = 0;
		  for (j = adata[i].start; j <= adata[i].end; j++)
		     if (tilex[j] < adata[i].end)
			tilex[j]++;
		     else
			tilex[j] = adata[i].start;
	       }
	     adelay[i]++;
	  }
     }
}

/*
   This function is called when the player presses the 'alt' key.
   Things that can be activated are entities and zones that are
   obstructed.
*/
void activate (void)
{
   int zx, zy, lx = 0, ly = 0, p, q, cf = 0, tf, mb;

   unpress ();
   switch (g_ent[0].facing)
     {
     case 0:
	ly = 1;
	cf = 1;
	break;
     case 1:
	ly = -1;
	cf = 0;
	break;
     case 2:
	lx = -1;
	cf = 3;
	break;
     case 3:
	lx = 1;
	cf = 2;
	break;
     }
   zx = g_ent[0].x / 16;
   zy = g_ent[0].y / 16;
   lx += zx;
   ly += zy;
   p = map_seg[ly * g_map.xsize + lx];
   q = z_seg[ly * g_map.xsize + lx];
   if (o_seg[ly * g_map.xsize + lx] != 0 && q > 0)
      do_zone (q);
   p = entityat (lx, ly, 0);
   if (p >= PSIZE)
     {
	tf = g_ent[p - 1].facing;
	if (g_ent[p - 1].facehero == 0)
	   g_ent[p - 1].facing = cf;
	drawmap ();
	blit2screen (xofs, yofs);
	mb = g_map.map_no;
	zx = abs (g_ent[p - 1].x - g_ent[0].x);
	zy = abs (g_ent[p - 1].y - g_ent[0].y);
	if ((zx <= 16 && zy <= 3) || (zx <= 3 && zy <= 16))
	   do_entity (p - 1);
	if (g_ent[p - 1].movemode == 0 && g_map.map_no == mb)
	   g_ent[p - 1].facing = tf;
     }
}

/*
   This is used to wait and make sure that the user has
   released a key before moving on.
*/
void unpress (void)
{
   int cc = 0;

   timer_count = 0;
   while (cc < 8)
     {
	cc = 0;
	readcontrols ();
	if (up == 0)
	   cc++;
	if (down == 0)
	   cc++;
	if (right == 0)
	   cc++;
	if (left == 0)
	   cc++;
	if (balt == 0)
	   cc++;
	if (bctrl == 0)
	   cc++;
	if (benter == 0)
	   cc++;
	if (besc == 0)
	   cc++;
	if (timer_count > 19)
	   cc = 8;
     }
   timer_count = 0;
}

/*
   Simply wait for the 'alt' key to be pressed.
*/
void wait_enter (void)
{
   int stop = 0;

   unpress ();
   while (!stop)
     {
	readcontrols ();
	if (balt)
	  {
	     unpress ();
	     stop = 1;
	  }
     }
   timer_count = 0;
}

/*
   This is for logging events within the program.  Very
   useful for debugging and tracing.
*/
void klog (char *msg)
{
   FILE *ff;

   ff = fopen ("game.log", "a");
   if (!ff)
      program_death ("could not open log!");
   fprintf (ff, "%s\n", msg);
   fclose (ff);
}

/*
   Set up allegro, set up variables, load stuff, blah...
*/
void startup (void)
{
   int p, i;
   time_t t;
   DATAFILE *pcxb;

   allegro_init ();
/*
   buffers to allocate
*/
   strbuf = (char *) malloc (256);
   map_seg = (unsigned short *) malloc (560);
   b_seg = (unsigned short *) malloc (560);
   f_seg = (unsigned short *) malloc (560);
   z_seg = (unsigned char *) malloc (280);
   s_seg = (unsigned char *) malloc (280);
   o_seg = (unsigned char *) malloc (280);
   progress = (unsigned char *) malloc (2000);
   treasure = (unsigned char *) malloc (1000);
   allocate_stuff ();
   install_keyboard ();
   install_timer ();
   parse_setup ();
   sound_init ();
   set_graphics_mode ();
   if (use_joy == 1)
      install_joystick (JOY_TYPE_AUTODETECT);
   if (num_joysticks == 0)
      use_joy = 0;
   else
     {
/*
    sprintf(strbuf,"%d joysticks detected.",num_joysticks);
    klog(strbuf);
    for (i=0;i<num_joysticks;i++)
    {
      sprintf(strbuf,"joystick %d: %d sticks, %d buttons",i,joy[i].num_sticks,joy[i].num_buttons);
      klog(strbuf);
    }
*/
	use_joy = 0;
	if (poll_joystick () == 0)
	  {
	     for (i = num_joysticks - 1; i >= 0; i--)
		if (joy[i].num_buttons >= 4)
		   use_joy = i + 1;
	  }
	if (use_joy == 0)
	  {
	     klog
		("Only joysticks/gamepads with at least 4 buttons can be used.");
	     remove_joystick ();
	  }
     }
   srand ((unsigned) time (&t));
   pcxb = load_datafile_object (PCX_DATAFILE, "MISC_PCX");
   if (!pcxb)
      program_death ("Could not load misc.pcx!");
   blit ((BITMAP *) pcxb->dat, menuptr, 24, 0, 0, 0, 16, 8);
   blit ((BITMAP *) pcxb->dat, sptr, 0, 0, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, mptr, 8, 0, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, upptr, 0, 8, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, dnptr, 8, 8, 0, 0, 8, 8);
   blit ((BITMAP *) pcxb->dat, bptr, 24, 8, 0, 0, 16, 8);
   blit ((BITMAP *) pcxb->dat, noway, 64, 16, 0, 0, 16, 16);
   blit ((BITMAP *) pcxb->dat, missbmp, 0, 16, 0, 0, 20, 6);
   blit ((BITMAP *) pcxb->dat, b_shield, 0, 80, 0, 0, 48, 48);
   blit ((BITMAP *) pcxb->dat, b_shell, 48, 80, 0, 0, 48, 48);
   blit ((BITMAP *) pcxb->dat, b_repulse, 0, 64, 0, 0, 16, 16);
   blit ((BITMAP *) pcxb->dat, b_mp, 0, 24, 0, 0, 10, 8);
   blit ((BITMAP *) pcxb->dat, sfonts[0], 0, 128, 0, 0, 60, 8);
   for (i = 0; i < 8; i++)
     {
	for (p = 0; p < 60; p++)
	  {
	     if (sfonts[0]->line[i][p] == 15)
	       {
		  sfonts[1]->line[i][p] = 22;
		  sfonts[2]->line[i][p] = 105;
		  sfonts[3]->line[i][p] = 39;
		  sfonts[4]->line[i][p] = 8;
	       }
	     else
	       {
		  sfonts[1]->line[i][p] = sfonts[0]->line[i][p];
		  sfonts[2]->line[i][p] = sfonts[0]->line[i][p];
		  sfonts[3]->line[i][p] = sfonts[0]->line[i][p];
		  sfonts[4]->line[i][p] = sfonts[0]->line[i][p];
	       }
	  }
     }
   for (p = 0; p < 27; p++)
      blit ((BITMAP *) pcxb->dat, stspics, p * 8 + 40, 0, 0, p * 8, 8, 8);
   for (p = 0; p < 40; p++)
      blit ((BITMAP *) pcxb->dat, sicons, p * 8, 32, 0, p * 8, 8, 8);
   for (p = 0; p < 40; p++)
      blit ((BITMAP *) pcxb->dat, sicons, p * 8, 40, 0, p * 8 + 320, 8, 8);
   for (p = 0; p < MAX_SHADOWS; p++)
      blit ((BITMAP *) pcxb->dat, shadow[p], p * 16, 160, 0, 0, 16, 16);
   for (p = 0; p < 8; p++)
      blit ((BITMAP *) pcxb->dat, bub[p], p * 16, 144, 0, 0, 16, 16);
   for (p = 0; p < 3; p++)
     {
	blit ((BITMAP *) pcxb->dat, bord[p], p * 8 + 96, 64, 0, 0, 8, 8);
	blit ((BITMAP *) pcxb->dat, bord[5 + p], p * 8 + 96, 84, 0, 0, 8, 8);
     }
   blit ((BITMAP *) pcxb->dat, bord[3], 96, 72, 0, 0, 8, 12);
   blit ((BITMAP *) pcxb->dat, bord[4], 112, 72, 0, 0, 8, 12);
   for (i = 0; i < 9; i++)
      blit ((BITMAP *) pcxb->dat, pgb[i], i * 16, 48, 0, 0, 9, 9);
   unload_datafile_object (pcxb);
   pcxb = load_datafile_object (PCX_DATAFILE, "KQFACES_PCX");
   if (!pcxb)
      program_death ("Could not load kqfaces.pcx!");
   for (p = 0; p < 4; p++)
     {
	blit ((BITMAP *) pcxb->dat, portrait[p], 0, p * 40, 0, 0, 40, 40);
	blit ((BITMAP *) pcxb->dat, portrait[p + 4], 40, p * 40, 0, 0, 40,
	      40);
     }
   unload_datafile_object (pcxb);
   load_data ();
   init_players ();
   LOCK_VARIABLE (timer);
   LOCK_VARIABLE (timer_count);
   LOCK_VARIABLE (ksec);
   LOCK_VARIABLE (kmin);
   LOCK_VARIABLE (khr);
   LOCK_FUNCTION (my_counter);
   install_int (my_counter, 10);
   create_trans_table (&cmap, pal, 128, 128, 128, NULL);
   color_map = &cmap;
   load_sgstats ();
}

/*
   Real descriptive huh?  This loads and sets up the
   fonts and entity frames and calls the functions to
   loads items and spells.
*/
void load_data (void)
{
   int p, q;
   DATAFILE *pb;

   pb = load_datafile_object (PCX_DATAFILE, "ALLFONTS_PCX");
   blit ((BITMAP *) pb->dat, kfonts, 0, 0, 0, 0, 744, 60);
   unload_datafile_object (pb);
   pb = load_datafile_object (PCX_DATAFILE, "ENTITIES_PCX");
   for (q = 0; q < MAXE; q++)
      for (p = 0; p < MAXEFRAMES; p++)
	 blit ((BITMAP *) pb->dat, eframes[q][p], p * 16, q * 16, 0, 0, 16,
	       16);
   unload_datafile_object (pb);
}

/*
   A separate function to create all global bitmaps needed in the game.
*/
void allocate_stuff (void)
{
   int i, p;

   kfonts = alloc_bmp (744, 60, "kfonts");
   for (i = 0; i < 5; i++)
      sfonts[i] = alloc_bmp (60, 8, "sfonts[i]");
   menuptr = alloc_bmp (16, 8, "menuptr");
   sptr = alloc_bmp (8, 8, "sptr");
   mptr = alloc_bmp (8, 8, "mptr");
   stspics = alloc_bmp (8, 216, "stspics");
   sicons = alloc_bmp (8, 640, "sicons");
   bptr = alloc_bmp (16, 8, "bptr");
   upptr = alloc_bmp (8, 8, "upptr");
   dnptr = alloc_bmp (8, 8, "dnptr");
   noway = alloc_bmp (16, 16, "noway");
   missbmp = alloc_bmp (20, 6, "missbmp");
   for (i = 0; i < 9; i++)
      pgb[i] = alloc_bmp (9, 9, "pgb[x]");
   tc = alloc_bmp (16, 16, "tc");
   tc2 = alloc_bmp (16, 16, "tc2");
   b_shield = alloc_bmp (48, 48, "b_shield");
   b_shell = alloc_bmp (48, 48, "b_shell");
   b_repulse = alloc_bmp (16, 166, "b_repulse");
   b_mp = alloc_bmp (10, 8, "b_mp");
   for (p = 0; p < MAXE; p++)
     {
	for (i = 0; i < MAXEFRAMES; i++)
	   eframes[p][i] = alloc_bmp (16, 16, "eframes[x][x]");
     }
   for (i = 0; i < MAXCHRS; i++)
     {
	for (p = 0; p < MAXFRAMES; p++)
	   frames[i][p] = alloc_bmp (16, 16, "frames[x][x]");
     }
   for (p = 0; p < MAXCFRAMES; p++)
     {
	for (i = 0; i < NUM_FIGHTERS; i++)
	  {
	     cframes[i][p] = alloc_bmp (32, 32, "cframes[x][x]");
	     tcframes[i][p] = alloc_bmp (32, 32, "tcframes[x][x]");
	  }
     }
   double_buffer = alloc_bmp (352, 280, "double_buffer");
   back = alloc_bmp (352, 280, "back");
   for (p = 0; p < MAX_SHADOWS; p++)
      shadow[p] = alloc_bmp (16, 16, "shadow[x]");
   for (p = 0; p < 8; p++)
      bub[p] = alloc_bmp (16, 16, "bub[x]");
   for (p = 0; p < 8; p++)
      bord[p] = alloc_bmp (8, 8, "bord[x]");
   for (p = 0; p < 8; p++)
      portrait[p] = alloc_bmp (40, 40, "portrait[x]");
   for (p = 0; p < MAX_TILES; p++)
      map_icons[p] = alloc_bmp (16, 16, "map_icons[x]");
}

BITMAP *alloc_bmp (int bx, int by, char *bname)
{
   BITMAP *tmp;

   tmp = create_bitmap (bx, by);
   if (!tmp)
     {
	sprintf (strbuf, "Could not allocate %s!.", bname);
	program_death (strbuf);
     }
   return tmp;
}

/*
   This frees memory and such things.
*/
void deallocate_stuff (void)
{
   int i, p;

   destroy_bitmap (kfonts);
   for (i = 0; i < 5; i++)
      destroy_bitmap (sfonts[i]);
   destroy_bitmap (menuptr);
   destroy_bitmap (sptr);
   destroy_bitmap (mptr);
   destroy_bitmap (upptr);
   destroy_bitmap (dnptr);
   destroy_bitmap (stspics);
   destroy_bitmap (sicons);
   destroy_bitmap (bptr);
   destroy_bitmap (noway);
   destroy_bitmap (missbmp);
   for (i = 0; i < 9; i++)
      destroy_bitmap (pgb[i]);
   destroy_bitmap (tc);
   destroy_bitmap (tc2);
   destroy_bitmap (b_shield);
   destroy_bitmap (b_shell);
   destroy_bitmap (b_repulse);
   destroy_bitmap (b_mp);
   for (p = 0; p < MAXE; p++)
      for (i = 0; i < MAXEFRAMES; i++)
	 destroy_bitmap (eframes[p][i]);
   for (i = 0; i < MAXFRAMES; i++)
      for (p = 0; p < MAXCHRS; p++)
	 destroy_bitmap (frames[p][i]);
   for (i = 0; i < MAXCFRAMES; i++)
     {
	for (p = 0; p < NUM_FIGHTERS; p++)
	  {
	     destroy_bitmap (cframes[p][i]);
	     destroy_bitmap (tcframes[p][i]);
	  }
     }
   destroy_bitmap (double_buffer);
   destroy_bitmap (back);
   for (p = 0; p < MAX_SHADOWS; p++)
      destroy_bitmap (shadow[p]);
   for (p = 0; p < 8; p++)
      destroy_bitmap (bub[p]);
   for (p = 0; p < 8; p++)
      destroy_bitmap (bord[p]);
   for (p = 0; p < MAXCHRS; p++)
      destroy_bitmap (portrait[p]);
   for (p = 0; p < MAX_TILES; p++)
      destroy_bitmap (map_icons[p]);
   if (map_seg)
      free (map_seg);
   if (b_seg)
      free (b_seg);
   if (f_seg)
      free (f_seg);
   if (z_seg)
      free (z_seg);
   if (s_seg)
      free (s_seg);
   if (o_seg)
      free (o_seg);
   if (progress)
      free (progress);
   if (treasure)
      free (treasure);
   if (strbuf)
      free (strbuf);
   if (is_sound)
     {
	if (is_mod_playing ())
	   stop_mod ();
	if (gsong)
	   destroy_mod (gsong);
	free_samples ();
     }
}

/*
   Set up the player characters and load data specific
   to them.
*/
void init_players (void)
{
   DATAFILE *pb;
   int i, j;

   for (j = 0; j < MAXCHRS; j++)
     {
	for (i = 0; i < 24; i++)
	   party[j].sts[i] = 0;
	for (i = 0; i < 6; i++)
	   party[j].eqp[i] = 0;
	for (i = 0; i < 60; i++)
	   party[j].spells[i] = 0;
	learn_new_spells (j);
     }
   pb = load_datafile_object (PCX_DATAFILE, "USCHRS_PCX");
   if (!pb)
      program_death ("Could not load chr graphics!");
   set_palette (pal);
   for (i = 0; i < MAXCHRS; i++)
      for (j = 0; j < MAXFRAMES; j++)
	 blit ((BITMAP *) pb->dat, frames[i][j], j * 16, i * 16, 0, 0, 16,
	       16);
   unload_datafile_object (pb);
}

/*
   Why not just use rest() you ask?  Well, this function
   kills time, but it also processes entities.  This function
   is basically used to run entity scripts and for automatic
   party movement.
*/
void kwait (int dtime)
{
   int cnt = 0;

   autoparty = 1;
   timer_count = 0;
   while (cnt < dtime)
     {
	while (timer_count > 0)
	  {
	     timer_count--;
	     cnt++;
	     process_entities ();
	     check_animation ();
	  }
	drawmap ();
	blit2screen (xofs, yofs);
	if (key[KEY_W] && key[KEY_ALT])
	  {
	     sprintf (strbuf,
		      "kwait(); cnt = %d, dtime = %d, timer_count = %d", cnt,
		      dtime, timer_count);
	     klog (strbuf);
	     break;
	  }
	if (key[KEY_X] && key[KEY_ALT])
	  {
	     sprintf (strbuf,
		      "kwait(); cnt = %d, dtime = %d, timer_count = %d", cnt,
		      dtime, timer_count);
	     program_death (strbuf);
	  }
     }
   timer_count = 0;
   autoparty = 0;
}

/*
   This does like wait() and processes entities...
   however, this function waits for particular entities
   to finish scripted movement rather than waiting for
   a specific amount of time to pass.
*/
void wait_for_entity (int est, int efi)
{
   int ewatch, ecnt = 0, wait_ent[50], a;

   if (efi < est)
      return;
   ewatch = efi - est + 1;
   for (a = est; a < est + ewatch; a++)
     {
	if (g_ent[a].active == 1 && g_ent[a].movemode == 2)
	  {
	     wait_ent[a] = 1;
	     ecnt++;
	  }
	else
	   wait_ent[a] = 0;
     }
   autoparty = 1;
   timer_count = 0;
   while (ecnt > 0)
     {
	while (timer_count > 0)
	  {
	     timer_count--;
	     process_entities ();
	     check_animation ();
	     for (a = est; a < est + ewatch; a++)
	       {
		  if (wait_ent[a] == 1)
		    {
		       if (g_ent[a].movemode == 0)
			 {
			    wait_ent[a] = 0;
			    ecnt--;
			 }
		    }
	       }
	  }
	drawmap ();
	blit2screen (xofs, yofs);
	if (key[KEY_W] && key[KEY_ALT])
	   break;
	if (key[KEY_X] && key[KEY_ALT])
	   program_death (strbuf);
     }
   timer_count = 0;
   autoparty = 0;
}

/*
   Kill the program and spit out a message.
*/
void program_death (char *message)
{
   klog (message);
   deallocate_stuff ();
   exit (-1);
}

int in_party (int pn)
{
   int a;

   for (a = 0; a < numchrs; a++)
      if (pidx[a] == pn)
	 return a + 1;
   return 0;
}

/*
   Well, this one is pretty obvious.
*/
int main (void)
{
   int stop = 0;

   startup ();
   if (start_menu (0) == 1)
      change_map ("starting", 0, 0, 0, 0);
   timer_count = 0;
   timer = 0;
   while (!stop)
     {
	while (timer_count > 0)
	  {
	     timer_count--;
	     process_entities ();
	     check_animation ();
	  }
	drawmap ();
	frate++;
	blit2screen (xofs, yofs);
	if (key[kesc])
	   stop = system_menu ();
	if (alldead)
	  {
	     clear (screen);
	     fade_in (pal, 16);
	     if (start_menu (0) == 1)
		change_map ("starting", 0, 0, 0, 0);
	     alldead = 0;
	     timer_count = 0;
	     timer = 0;
	  }
     }
   deallocate_stuff ();
   return 0;
}

END_OF_MAIN ();
