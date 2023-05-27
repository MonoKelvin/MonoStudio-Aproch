/****************************************************************************
 * @file    AprochAPI.h
 * @date    2021-1-9
 * @author  MonoKelvin
 * @email   15007083506@qq.com
 * @github  https://github.com/MonoKelvin
 * @brief
 * 
 *      ___   _____   _____    _____   _____   _   _  
 *     /   | |  _  \ |  _  \  /  _  \ /  ___| | | | | 
 *    / /| | | |_| | | |_| |  | | | | | |     | |_| | 
 *   / / | | |  ___/ |  _  /  | | | | | |     |  _  | 
 *  / /  | | | |     | | \ \  | |_| | | |___  | | | | 
 * /_/   |_| |_|     |_|  \_\ \_____/ \_____| |_| |_| 
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

#define APROCH_NAMESPACE

#include "AprochGlobal.h"

// service
#include "Common/IService.h"
#include "Common/IKeyExpressionService.h"
#include "Common/IAppConfigService.h"
#include "Common/IDocumentService.h"
#include "Common/IDataBaseService.h"

// common
#include "Common/IMainWindow.h"
#include "Common/IFileSaveLoader.h"
#include "Common/IJsonSerializable.h"
#include "Common/IStringSerializable.h"
#include "Common/IDataSerializable.h"
#include "Common/IDataReader.h"
#include "Common/IDataWriter.h"
#include "Common/IDataConverter.h"
#include "Common/ITextTranslatable.h"
#include "Common/IPlugin.h"
#include "Common/IContextObject.h"

#include "Common/AException.h"
#include "Common/APrototype.h"
#include "Common/AMathToolkit.h"
#include "Common/AFileToolkit.h"
#include "Common/AStringToolkit.h"
#include "Common/AIdentity.h"
#include "Common/ANumberValidator.h"
#include "Common/AIconFont.h"
#include "Common/ADoubleValidator.h"
#include "Common/ADataConverter.h"
#include "Common/ACategory.h"
#include "Common/AReflection.h"
#include "Common/AContextContainter.h"

// application
#include "AAction.h"
#include "AActionManager.h"
#include "ACommand.h"
#include "AApplicationContext.h"
#include "ACommandManager.h"
#include "ACursorManager.h"
#include "AAbstractKeyExpression.h"
#include "AKeyExpressionManager.h"
#include "AAndKeyExpression.h"
#include "AOrKeyExpression.h"
#include "ADocFile.h"
#include "AKeyExpressionService.h"
#include "AAppConfigService.h"
#include "ADocumentService.h"
#include "APrivateAppConfigService.h"
#include "ASqlDataBase.h"
#include "APluginManager.h"

// widgets
#include "Widget/Style/AStyleDefines.h"
#include "Widget/Style/AUIStyleObject.h"
#include "Widget/Style/AWindowsStyleManager.h"
#include "Widget/ABorderlessWidget.h"
#include "Widget/AFlowLayout.h"
#include "Widget/AWindow.h"
#include "Widget/ACaptionBar.h"
#include "Widget/AToolBar.h"
#include "Widget/ANavigationBar.h"
#include "Widget/AAbstractColorPicker.h"
#include "Widget/AWheelColorPicker.h"
#include "Widget/AColorPickerIndicator.h"
#include "Widget/AWheelColorPickerWidget.h"
#include "Widget/AColorSliderControl.h"
#include "Widget/AVerticalLabel.h"
#include "Widget/APromptWidget.h"
#include "Widget/AAvatar.h"

#include "Data/header.inc"

#ifdef APROCH_WINUI
#include "WinUI/include.inc"
#endif // APROCH_WINUI
