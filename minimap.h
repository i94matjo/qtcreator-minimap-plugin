/*
  Minimap QtCreator plugin.

  This library is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not see
  http://www.gnu.org/licenses/lgpl-2.1.html.

  Copyright (c) 2017, emJay Software Consulting AB, See AUTHORS for details.
*/

#pragma once

#include "minimap_global.h"

#include <extensionsystem/iplugin.h>

class QStyle;

namespace Core
{
class IEditor;
}

namespace Minimap
{
namespace Internal
{

class Minimap : public ExtensionSystem::IPlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE
                         "Minimap.json")

public:
   Minimap();

   ~Minimap();

   bool initialize(const QStringList& arguments, QString* errorString);
   void extensionsInitialized();
   ShutdownFlag aboutToShutdown();

private:
   void editorCreated(Core::IEditor* editor, const QString& fileName);
};
}
}
