/*
    Copyright (C) 2015  Intel Corporation

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "xfstkdldrplugincloverviewplus.h"

XfstkDldrPluginCloverviewPlus::XfstkDldrPluginCloverviewPlus()
{
    this->PluginInfo.PlatformName = QString("CloverviewPlus");
    this->PluginInfo.PluginName = QString("CLV+ A0");
    this->PluginInfo.PluginTabOrder = 3;

    this->PluginInfo.PluginInterface = (XfstkDldrPluginInterface *)this;
    QObject::connect(&(this->UserTabInterface),SIGNAL(UserInterfaceChanged(UserInterfaceState*)),&(this->OptionsTabInterface),SLOT(UserInterfaceChanged(UserInterfaceState*)));
    QObject::connect(&(this->OptionsTabInterface),SIGNAL(OptionsInterfaceChanged(OptionsInterfaceState*)),&(this->UserTabInterface),SLOT(OptionsInterfaceChanged(OptionsInterfaceState*)));
}

XfstkDldrPluginInterface* XfstkDldrPluginCloverviewPlus::Create()
{
    return new XfstkDldrPluginCloverviewPlus();
}

QTabWidget *XfstkDldrPluginCloverviewPlus::GetUserTabInterface()
{
    return this->UserTabInterface.GetTabInterface();
}

QTabWidget *XfstkDldrPluginCloverviewPlus::GetOptionsTabInterface()
{
    return this->OptionsTabInterface.GetTabInterface();
}

bool XfstkDldrPluginCloverviewPlus::InitializeTabInterfaces()
{
    return true;
}

bool XfstkDldrPluginCloverviewPlus::SaveTabInterfaceSettings()
{
    this->UserTabInterface.SaveUserInterfaceStateToDisk(QString("Intel"),QString("CloverviewPlus"));
    return true;
}

bool XfstkDldrPluginCloverviewPlus::RestoreTabInterfaceSettings()
{
    this->UserTabInterface.LoadUserInterfaceStateFromDisk(QString("Intel"),QString("CloverviewPlus"));
    this->UserTabInterface.RestoreUserInterfaceFromCurrentState();
    return true;
}

XfstkDldrPluginInfo *XfstkDldrPluginCloverviewPlus::GetPluginInfo()
{
    return &(this->PluginInfo);
}

bool XfstkDldrPluginCloverviewPlus::Init()
{
    return true;
}
