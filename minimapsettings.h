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

#include <QObject>

namespace Minimap
{
namespace Internal
{
class MinimapSettingsPage;

class MinimapSettings : public QObject
{
   Q_OBJECT
public:
   explicit MinimapSettings(QObject* parent);
   ~MinimapSettings();

   void toMap(const QString& prefix, QVariantMap* map) const;
   void fromMap(const QString& prefix, const QVariantMap& map);

   static MinimapSettings* instance();

   static bool enabled();
   static int width();
   static int lineCountThreshold();
   static int alpha();

signals:
   void enabledChanged(bool);
   void widthChanged(int);
   void lineCountThresholdChanged(int);
   void alphaChanged(int);

private:
   friend class MinimapSettingsPage;

   void setEnabled(bool enabled);
   void setWidth(int width);
   void setLineCountThreshold(int lineCountThreshold);
   void setAlpha(int alpha);

   bool m_enabled;
   int m_width;
   int m_lineCountThreshold;
   int m_alpha;
   MinimapSettingsPage* m_settingsPage;
};
}
}
