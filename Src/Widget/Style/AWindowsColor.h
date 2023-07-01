/****************************************************************************
 * @file    AWindowsColor.h
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
#pragma once
#include <QColor>

 // 
 // The most of the following code is copied from Qtitan.
 // 
 // Qtitan Library by Developer Machines(Microsoft - Ribbon implementation for Qt.C++)
 // Copyright (c) 2009 - 2022 Developer Machines (https://www.devmachines.com) ALL RIGHTS RESERVED
 // 

APROCH_NAMESPACE_BEGIN

class APROCH_API AWindowsColor : public QColor
{
public:
    enum GlobalColor
    {
        DefaultColor,
        MetroUI_Sapphire,
        MetroUI_KleinBlue,
        MetroUI_Cardinal,
        MetroUI_Chocolate,
        MetroUI_RawUmber,
        MetroUI_DarkCerulean,
        MetroUI_GreenHouse,
        MetroUI_VidaLoca,
        MetroUI_NavyBlue,
        MetroUI_Matterhorn,
        MetroUI_Watercourse,

        MetroUI_LightGreen,
        MetroUI_Green,
        MetroUI_DarkGreen,
        MetroUI_Magenta,
        MetroUI_LightPurple,
        MetroUI_Purple,
        MetroUI_DarkPurple,
        MetroUI_Darken,
        MetroUI_Teal,
        MetroUI_LightBlue,
        MetroUI_Blue,
        MetroUI_DarkBlue,
        MetroUI_Yellow,
        MetroUI_Orange,
        MetroUI_DarkOrange,
        MetroUI_Red,
        MetroUI_DarkRed,
        MetroUI_White,

        MetroUI_Tile_Gradient1,
        MetroUI_Tile_Gradient2,
        MetroUI_Blue_Gradient1,
        MetroUI_Blue_Gradient2,
        MetroUI_Purple_Gradient1,
        MetroUI_Purple_Gradient2,
        MetroUI_DarkPurple_Gradient1,
        MetroUI_DarkPurple_Gradient2,
        MetroUI_Red_Gradient1,
        MetroUI_Red_Gradient2,
        MetroUI_Orange_Gradient1,
        MetroUI_Orange_Gradient2,
        MetroUI_Green_Gradient1,
        MetroUI_Green_Gradient2,
        MetroUI_SkyBlue_Gradient1,
        MetroUI_SkyBlue_Gradient2,

        Phone_Lime,
        Phone_Green,
        Phone_Emerald,
        Phone_Teal,
        Phone_Cyan,
        Phone_Cobalt,
        Phone_Indigo,
        Phone_Violet,
        Phone_Pink,
        Phone_Magenta,
        Phone_Crimson,
        Phone_Red,
        Phone_Orange,
        Phone_Amber,
        Phone_Yellow,
        Phone_Brown,
        Phone_Olive,
        Phone_Steel,
        Phone_Mauve,
        Phone_Taupe,
    };

public:
    AWindowsColor(GlobalColor color);
};

APROCH_NAMESPACE_END
