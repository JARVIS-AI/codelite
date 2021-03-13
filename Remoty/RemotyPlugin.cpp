//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2021 Eran Ifrah
// file name            : RemotyPlugin.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "NewFileSystemWorkspaceDialog.h"
#include "RemoteWorkspace.hpp"
#include "RemotyPlugin.hpp"
#include "asyncprocess.h"
#include "clFileSystemWorkspace.hpp"
#include "clFileSystemWorkspaceConfig.hpp"
#include "clWorkspaceManager.h"
#include "environmentconfig.h"
#include "event_notifier.h"
#include "file_logger.h"
#include "globals.h"
#include <wx/msgdlg.h>

static RemotyPlugin* thePlugin = NULL;

// Define the plugin entry point
CL_PLUGIN_API IPlugin* CreatePlugin(IManager* manager)
{
    if(thePlugin == 0) {
        thePlugin = new RemotyPlugin(manager);
    }
    return thePlugin;
}

CL_PLUGIN_API PluginInfo* GetPluginInfo()
{
    static PluginInfo info;
    info.SetAuthor(wxT("Eran Ifrah"));
    info.SetName("Remoty");
    info.SetDescription(_("Remote work over SSH"));
    info.SetVersion("v1.0");
    return &info;
}

CL_PLUGIN_API int GetPluginInterfaceVersion() { return PLUGIN_INTERFACE_VERSION; }

RemotyPlugin::RemotyPlugin(IManager* manager)
    : IPlugin(manager)
{
    m_longName = _("Remote work over SSH");
    m_shortName = "Remoty";
    EventNotifier::Get()->Bind(wxEVT_CONTEXT_MENU_FOLDER, &RemotyPlugin::OnFolderContextMenu, this);
    EventNotifier::Get()->Bind(wxEVT_CMD_CREATE_NEW_WORKSPACE, &RemotyPlugin::OnNewWorkspace, this);
    clWorkspaceManager::Get().RegisterWorkspace(new RemoteWorkspace(true));
    m_workspace = new RemoteWorkspace();
}

RemotyPlugin::~RemotyPlugin() {}

void RemotyPlugin::CreateToolBar(clToolBar* toolbar) { wxUnusedVar(toolbar); }

void RemotyPlugin::CreatePluginMenu(wxMenu* pluginsMenu) { wxUnusedVar(pluginsMenu); }

void RemotyPlugin::HookPopupMenu(wxMenu* menu, MenuType type)
{
    wxUnusedVar(menu);
    wxUnusedVar(type);
}

void RemotyPlugin::UnPlug()
{
    wxDELETE(m_workspace);
    EventNotifier::Get()->Unbind(wxEVT_CONTEXT_MENU_FOLDER, &RemotyPlugin::OnFolderContextMenu, this);
    EventNotifier::Get()->Unbind(wxEVT_CMD_CREATE_NEW_WORKSPACE, &RemotyPlugin::OnNewWorkspace, this);
}

void RemotyPlugin::OnFolderContextMenu(clContextMenuEvent& event) { event.Skip(); }

void RemotyPlugin::OnNewWorkspace(clCommandEvent& e)
{
    e.Skip();
    if(e.GetString() == WORKSPACE_TYPE_NAME) {
        e.Skip(false);
    }
}