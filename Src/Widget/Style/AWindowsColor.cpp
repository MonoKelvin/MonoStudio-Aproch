/****************************************************************************
 * @file    AAWindowsColor.cpp
 * @date    2023-07-01 
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 *
 * This source file is part of Aproch.
 * Copyright (C) 2020 by MonoKelvin. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#include "stdafx.h"
#include "AWindowsColor.h"

/*!
\enum AWindowsColor::GlobalColor
Enumerator AWindowsColor::GlobalColor describes the set of predefined windows color used in Windows Metro UI. It contains a paltettes for Windows Phone or Classic Windows.
\inmodule QtitanNavigation
\value DefaultColor \raw HTML
<div style="border: 2px solid gray; background-color: white; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Sapphire \raw HTML
<div style="border: 2px solid gray; background-color: #002050; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_KleinBlue \raw HTML
<div style="border: 2px solid gray; background-color: #001CA5; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Cardinal \raw HTML
<div style="border: 2px solid gray; background-color: #C0172B; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Chocolate \raw HTML
<div style="border: 2px solid gray; background-color: #CD7108; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_RawUmber \raw HTML
<div style="border: 2px solid gray; background-color: #794300; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkCerulean \raw HTML
<div style="border: 2px solid gray; background-color: #005088; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_GreenHouse \raw HTML
<div style="border: 2px solid gray; background-color: #3F7239; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_VidaLoca \raw HTML
<div style="border: 2px solid gray; background-color: #578837; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_NavyBlue \raw HTML
<div style="border: 2px solid gray; background-color: #0072C6; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Matterhorn \raw HTML
<div style="border: 2px solid gray; background-color: #555555; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Watercourse \raw HTML
<div style="border: 2px solid gray; background-color: #006551; width: 100px; height: 20px;">&nbsp;</div>
\endraw

\value MetroUI_LightGreen \raw HTML
<div style="border: 2px solid gray; background-color: #99b433; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Green \raw HTML
<div style="border: 2px solid gray; background-color: #00a300; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkGreen \raw HTML
<div style="border: 2px solid gray; background-color: #1e7145; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Magenta \raw HTML
<div style="border: 2px solid gray; background-color: #ff0097; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_LightPurple \raw HTML
<div style="border: 2px solid gray; background-color: #9f00a7; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Purple \raw HTML
<div style="border: 2px solid gray; background-color: #7e3878; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkPurple \raw HTML
<div style="border: 2px solid gray; background-color: #603cba; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Darken \raw HTML
<div style="border: 2px solid gray; background-color: #1d1d1d; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Teal \raw HTML
<div style="border: 2px solid gray; background-color: #00aba9; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_LightBlue \raw HTML
<div style="border: 2px solid gray; background-color: #eff4ff; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Blue \raw HTML
<div style="border: 2px solid gray; background-color: #2d89ef; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkBlue \raw HTML
<div style="border: 2px solid gray; background-color: #2b5797; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Yellow \raw HTML
<div style="border: 2px solid gray; background-color: #ffc40d; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Orange \raw HTML
<div style="border: 2px solid gray; background-color: #e3a21a; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkOrange \raw HTML
<div style="border: 2px solid gray; background-color: #da532c; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Red \raw HTML
<div style="border: 2px solid gray; background-color: #ee1111; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkRed \raw HTML
<div style="border: 2px solid gray; background-color: #b91d47; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_White \raw HTML
<div style="border: 2px solid gray; background-color: #ffffff; width: 100px; height: 20px;">&nbsp;</div>
\endraw

\value MetroUI_Tile_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #008299; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Tile_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #00A0B1; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Blue_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #2672EC; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Blue_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #2E8DEF; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Purple_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #8C0095; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Purple_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #A700AE; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkPurple_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #5133AB; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_DarkPurple_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #643EBF; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Red_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #AC193D; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Red_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #BF1E4B; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Orange_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #D24726; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Orange_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #DC572E; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Green_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #008A00; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_Green_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #00A600; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_SkyBlue_Gradient1 \raw HTML
<div style="border: 2px solid gray; background-color: #094AB2; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value MetroUI_SkyBlue_Gradient2 \raw HTML
<div style="border: 2px solid gray; background-color: #A5BC4; width: 100px; height: 20px;">&nbsp;</div>
\endraw

\value Phone_Lime \raw HTML
<div style="border: 2px solid gray; background-color: #A4C400; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Green \raw HTML
<div style="border: 2px solid gray; background-color: #60A917; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Emerald \raw HTML
<div style="border: 2px solid gray; background-color: #008A00; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Teal \raw HTML
<div style="border: 2px solid gray; background-color: #00ABA9; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Cyan \raw HTML
<div style="border: 2px solid gray; background-color: #1BA1E2; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Cobalt \raw HTML
<div style="border: 2px solid gray; background-color: #0050EF; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Indigo \raw HTML
<div style="border: 2px solid gray; background-color: #6A00FF; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Violet \raw HTML
<div style="border: 2px solid gray; background-color: #AA00FF; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Pink \raw HTML
<div style="border: 2px solid gray; background-color: #F472D0; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Magenta \raw HTML
<div style="border: 2px solid gray; background-color: #D80073; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Crimson \raw HTML
<div style="border: 2px solid gray; background-color: #A20025; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Red \raw HTML
<div style="border: 2px solid gray; background-color: #E51400; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Orange \raw HTML
<div style="border: 2px solid gray; background-color: #FA6800; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Amber \raw HTML
<div style="border: 2px solid gray; background-color: #F0A30A; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Yellow \raw HTML
<div style="border: 2px solid gray; background-color: #E3C800; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Brown \raw HTML
<div style="border: 2px solid gray; background-color: #825A2C; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Olive \raw HTML
<div style="border: 2px solid gray; background-color: #6D8764; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Steel \raw HTML
<div style="border: 2px solid gray; background-color: #647687; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Mauve \raw HTML
<div style="border: 2px solid gray; background-color: #76608A; width: 100px; height: 20px;">&nbsp;</div>
\endraw
\value Phone_Taupe \raw HTML
<div style="border: 2px solid gray; background-color: #87794E; width: 100px; height: 20px;">&nbsp;</div>
\endraw

*/

APROCH_NAMESPACE_BEGIN

static const unsigned int windowsColorHexs[] = {
    0xFF000001,
    0x00002050, //MetroUI_Sapphire
    0x00001CA5,
    0x00C0172B,
    0x00CD7108,
    0x00794300,
    0x00005088,
    0x003F7239,
    0x00578837,
    0x000072C6,
    0x00555555,
    0x00006551,

    0x0099b433, //MetroUI_LightGreen
    0x0000a300,
    0x001e7145,
    0x00ff0097,
    0x009f00a7,
    0x007e3878,
    0x00603cba,
    0x001d1d1d,
    0x0000aba9,
    0x00eff4ff,
    0x002d89ef,
    0x002b5797,
    0x00ffc40d,
    0x00e3a21a,
    0x00da532c,
    0x00ee1111,
    0x00b91d47,
    0x00ffffff,

    0x00008299, //Tile_Gradient1
    0x0000A0B1,
    0x002672EC,
    0x002E8DEF,
    0x008C0095,
    0x00A700AE,
    0x005133AB,
    0x00643EBF,
    0x00AC193D,
    0x00BF1E4B,
    0x00D24726,
    0x00DC572E,
    0x00008A00,
    0x0000A600,
    0x00094AB2,
    0x000A5BC4,

    0x00A4C400, //Phone_Lime
    0x0060A917,
    0x00008A00,
    0x0000ABA9,
    0x001BA1E2,
    0x000050EF,
    0x006A00FF,
    0x00AA00FF,
    0x00F472D0,
    0x00D80073,
    0x00A20025,
    0x00E51400,
    0x00FA6800,
    0x00F0A30A,
    0x00E3C800,
    0x00825A2C,
    0x006D8764,
    0x00647687,
    0x0076608A,
    0x0087794E,
};

QRgb getWindowsRgb(AWindowsColor::GlobalColor color)
{
    return QRgb(windowsColorHexs[color]);
}

AWindowsColor::AWindowsColor(AWindowsColor::GlobalColor color)
    : QColor(getWindowsRgb(color))
{
}

APROCH_NAMESPACE_END