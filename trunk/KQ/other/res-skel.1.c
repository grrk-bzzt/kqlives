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

#include "res.h"
#include "kq.h"
#include "allegro.h"


// *INDENT-OFF*
PALETTE pal = {
   {0, 0, 0, 0},     {0, 0, 0, 0},     {8, 8, 8, 0},     {12, 12, 12, 0},
   {16, 16, 16, 0},  {20, 20, 20, 0},  {24, 24, 24, 0},  {28, 28, 28, 0},
   {33, 33, 33, 0},  {38, 38, 38, 0},  {43, 43, 43, 0},  {47, 47, 47, 0},
   {51, 51, 51, 0},  {55, 55, 55, 0},  {59, 59, 59, 0},  {63, 63, 63, 0},

   {7, 0, 0, 0},     {15, 0, 0, 0},    {23, 0, 0, 0},    {31, 0, 0, 0},
   {39, 0, 0, 0},    {47, 0, 0, 0},    {55, 0, 0, 0},    {63, 0, 0, 0},
   {63, 7, 7, 0},    {63, 14, 14, 0},  {63, 21, 21, 0},  {63, 29, 29, 0},
   {63, 36, 36, 0},  {63, 44, 44, 0},  {63, 51, 51, 0},  {63, 59, 59, 0},

   {0, 7, 0, 0},     {0, 11, 0, 0},    {0, 15, 0, 0},    {0, 19, 0, 0},
   {0, 23, 0, 0},    {0, 27, 0, 0},    {0, 31, 0, 0},    {0, 39, 0, 0},
   {0, 47, 0, 0},    {0, 55, 0, 0},    {0, 63, 0, 0},    {22, 63, 22, 0},
   {37, 63, 37, 0},  {47, 63, 47, 0},  {53, 63, 53, 0},  {58, 63, 58, 0},

   {0, 0, 7, 0},     {0, 0, 15, 0},    {0, 0, 23, 0},    {0, 0, 31, 0},
   {0, 0, 39, 0},    {0, 0, 47, 0},    {0, 0, 55, 0},    {0, 0, 63, 0},
   {7, 7, 63, 0},    {15, 15, 63, 0},  {22, 22, 63, 0},  {30, 30, 63, 0},
   {37, 37, 63, 0},  {45, 45, 63, 0},  {52, 52, 63, 0},  {58, 58, 63, 0},

   {7, 0, 7, 0},     {15, 0, 15, 0},   {23, 0, 23, 0},   {31, 0, 31, 0},
   {39, 0, 39, 0},   {47, 0, 47, 0},   {55, 0, 55, 0},   {63, 0, 63, 0},
   {63, 7, 63, 0},   {63, 15, 63, 0},  {63, 22, 63, 0},  {63, 30, 63, 0},
   {63, 37, 63, 0},  {63, 45, 63, 0},  {63, 52, 63, 0},  {63, 58, 63, 0},

   {7, 1, 0, 0},     {15, 2, 0, 0},    {23, 4, 0, 0},    {31, 7, 0, 0},
   {39, 9, 0, 0},    {46, 12, 0, 0},   {54, 15, 0, 0},   {63, 18, 0, 0},
   {63, 21, 4, 0},   {63, 25, 8, 0},   {63, 29, 12, 0},  {63, 34, 16, 0},
   {63, 38, 22, 0},  {63, 44, 28, 0},  {63, 49, 34, 0},  {63, 55, 41, 200},

   {7, 7, 0, 200},   {11, 11, 0, 0},   {15, 15, 0, 0},   {19, 19, 0, 200},
   {23, 23, 0, 0},   {27, 27, 0, 0},   {31, 31, 0, 0},   {39, 39, 0, 0},
   {47, 47, 0, 0},   {54, 54, 0, 0},   {63, 63, 0, 0},   {63, 63, 23, 0},
   {63, 63, 35, 0},  {63, 63, 47, 0},  {63, 63, 53, 0},  {63, 63, 58, 0},

   {0, 7, 7, 0},     {0, 11, 11, 0},   {0, 15, 15, 0},   {0, 19, 19, 0},
   {0, 23, 23, 0},   {0, 27, 27, 0},   {0, 31, 31, 0},   {0, 39, 39, 0},
   {0, 47, 47, 0},   {0, 55, 55, 0},   {0, 63, 63, 0},   {16, 63, 63, 0},
   {32, 63, 63, 0},  {47, 63, 63, 0},  {51, 63, 63, 0},  {58, 63, 63, 0},

   {3, 1, 0, 0},     {7, 3, 0, 0},     {11, 5, 0, 0},    {15, 8, 0, 0},
   {19, 11, 0, 0},   {23, 14, 0, 0},   {27, 17, 0, 0},   {31, 20, 0, 105},
   {35, 24, 0, 93},  {39, 27, 0, 0},   {43, 32, 0, 0},   {47, 35, 0, 0},
   {51, 38, 0, 0},   {55, 42, 0, 0},   {59, 46, 0, 0},   {63, 55, 22, 0},

   {6, 3, 3, 0},     {12, 6, 6, 0},    {18, 9, 9, 0},    {24, 12, 12, 0},
   {30, 15, 15, 0},  {36, 18, 18, 0},  {42, 21, 21, 0},  {48, 24, 24, 0},
   {54, 27, 27, 0},  {60, 30, 30, 0},  {63, 33, 33, 0},  {63, 36, 36, 0},
   {63, 39, 39, 0},  {63, 43, 43, 0},  {63, 47, 47, 0},  {63, 51, 51, 0},

   {3, 6, 3, 0},     {6, 12, 6, 0},    {9, 18, 9, 0},    {12, 24, 12, 0},
   {15, 30, 15, 0},  {18, 36, 18, 0},  {21, 42, 21, 0},  {24, 48, 24, 0},
   {27, 54, 27, 0},  {30, 60, 30, 0},  {33, 63, 33, 0},  {37, 63, 37, 0},
   {41, 63, 41, 0},  {45, 63, 45, 0},  {49, 63, 49, 0},  {53, 63, 53, 0},

   {4, 2, 1, 0},     {8, 4, 2, 0},     {12, 6, 3, 0},    {16, 8, 4, 0},
   {20, 10, 5, 0},   {24, 16, 7, 0},   {28, 20, 10, 0},  {32, 24, 16, 0},
   {36, 28, 20, 0},  {40, 32, 25, 0},  {44, 36, 28, 0},  {48, 40, 32, 0},
   {52, 44, 36, 0},  {56, 48, 40, 0},  {60, 52, 44, 0},  {63, 56, 48, 0},

   {6, 3, 6, 0},     {12, 6, 12, 0},   {18, 9, 18, 0},   {24, 12, 24, 0},
   {30, 15, 30, 0},  {36, 18, 36, 0},  {42, 21, 42, 0},  {48, 24, 48, 0},
   {54, 27, 54, 0},  {60, 30, 60, 0},  {63, 34, 63, 0},  {63, 38, 63, 0},
   {63, 42, 63, 0},  {63, 46, 63, 0},  {63, 50, 63, 0},  {63, 54, 63, 87},

   {6, 3, 2, 79},    {12, 6, 3, 0},    {18, 9, 4, 0},    {24, 12, 5, 0},
   {30, 15, 6, 0},   {36, 18, 7, 0},   {42, 21, 8, 0},   {48, 24, 9, 0},
   {54, 27, 11, 0},  {60, 30, 13, 0},  {63, 33, 15, 0},  {63, 36, 17, 0},
   {63, 39, 19, 80}, {63, 42, 21, 71}, {63, 46, 24, 0},  {63, 50, 27, 0},

   {6, 6, 3, 0},     {12, 12, 6, 87},  {18, 18, 9, 0},   {24, 24, 12, 0},
   {30, 30, 15, 0},  {36, 36, 18, 0},  {42, 42, 21, 0},  {48, 48, 24, 0},
   {54, 54, 27, 0},  {60, 60, 30, 0},  {63, 63, 34, 0},  {63, 63, 38, 0},
   {63, 63, 42, 0},  {63, 63, 46, 200}, {63, 63, 50, 0}, {63, 63, 54, 200},

   {3, 6, 6, 0},     {6, 12, 12, 0},   {9, 18, 18, 0},   {12, 24, 24, 0},
   {15, 30, 30, 0},  {18, 36, 36, 0},  {21, 42, 42, 0},  {24, 48, 48, 0},
   {27, 54, 54, 0},  {30, 60, 60, 0},  {34, 63, 63, 0},  {38, 63, 63, 0},
   {42, 63, 63, 0},  {46, 63, 63, 0},  {50, 63, 63, 0},  {63, 63, 63, 0}
};



s_item items[NUM_ITEMS] = {
