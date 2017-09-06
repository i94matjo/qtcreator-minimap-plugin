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

#include "minimapsettings.h"
#include "minimapconstants.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/displaysettings.h>
#include <texteditor/texteditoroptionspage.h>
#include <texteditor/texteditorsettings.h>
#include <utils/qtcassert.h>
#include <utils/settingsutils.h>

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPointer>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include <limits>

namespace Minimap
{
namespace Internal
{
namespace
{
const char minimapPostFix[] = "Minimap";
const char enabledKey[] = "Enabled";
const char widthKey[] = "Width";
const char lineCountThresholdKey[] = "LineCountThresHold";
const char alphaKey[] = "Alpha";

MinimapSettings* m_instance = 0;
}

class MinimapSettingsPage : public TextEditor::TextEditorOptionsPage
{
public:
   MinimapSettingsPage(QObject* parent)
   : TextEditor::TextEditorOptionsPage(parent)
   , m_widget(0)
   {
      setId(Constants::MINIMAP_SETTINGS);
      setDisplayName(tr("Minimap"));
      connect(TextEditor::TextEditorSettings::instance(),
              &TextEditor::TextEditorSettings::displaySettingsChanged, this,
              &MinimapSettingsPage::displaySettingsChanged);
      m_textWrapping =
         TextEditor::TextEditorSettings::displaySettings().m_textWrapping;
   }

   ~MinimapSettingsPage()
   {
   }

   QWidget* widget()
   {
      if (!m_widget)
      {
         m_widget = new QWidget;
         QVBoxLayout* layout = new QVBoxLayout;
         QGroupBox* groupBox = new QGroupBox(m_widget);
         groupBox->setTitle(tr("Minimap"));
         layout->addWidget(groupBox);
         QFormLayout* form = new QFormLayout;
         m_enabled = new QCheckBox(groupBox);
         m_enabled->setToolTip(tr("Check to enable Minimap scrollbar"));
         m_enabled->setChecked(m_instance->m_enabled);
         form->addRow(tr("Enabled:"), m_enabled);
         m_width = new QSpinBox;
         m_width->setMinimum(1);
         m_width->setMaximum(std::numeric_limits<int>::max());
         m_width->setToolTip(tr("The width of the Minimap"));
         m_width->setValue(m_instance->m_width);
         form->addRow(tr("Width:"), m_width);
         m_lineCountThresHold = new QSpinBox;
         m_lineCountThresHold->setMinimum(1);
         m_lineCountThresHold->setMaximum(std::numeric_limits<int>::max());
         m_lineCountThresHold->setToolTip(tr(
            "Line count threshold where no Minimap scrollbar is to be used"));
         m_lineCountThresHold->setValue(m_instance->m_lineCountThreshold);
         form->addRow(tr("Line Count Threshold:"), m_lineCountThresHold);
         m_alpha = new QSpinBox;
         m_alpha->setMinimum(0);
         m_alpha->setMaximum(255);
         m_alpha->setToolTip(tr("The alpha value of the scrollbar slider"));
         m_alpha->setValue(m_instance->m_alpha);
         form->addRow(tr("Scrollbar slider alpha value:"), m_alpha);
         groupBox->setLayout(form);
         m_widget->setLayout(layout);
         m_widget->setEnabled(!m_textWrapping);
         m_widget->setToolTip(
            m_textWrapping ?
               tr("Disable text wrapping to enable Minimap scrollbar") :
               QString());
      }
      return m_widget;
   }

   void apply()
   {
      if (!m_widget)
      {
         return;
      }
      bool save(false);
      if (m_enabled->isChecked() != MinimapSettings::enabled())
      {
         m_instance->setEnabled(m_enabled->isChecked());
         save = true;
      }
      if (m_width->value() != MinimapSettings::width())
      {
         m_instance->setWidth(m_width->value());
         save = true;
      }
      if (m_lineCountThresHold->value()
          != MinimapSettings::lineCountThreshold())
      {
         m_instance->setLineCountThreshold(m_lineCountThresHold->value());
         save = true;
      }
      if (m_alpha->value()
          != MinimapSettings::alpha())
      {
         m_instance->setAlpha(m_alpha->value());
         save = true;
      }
      QSettings* s = Core::ICore::settings();
      if (save)
      {
         Utils::toSettings(minimapPostFix, QLatin1String("text"), s,
                           m_instance);
      }
   }

   void finish()
   {
      delete m_widget;
   }

private:
   void displaySettingsChanged(const TextEditor::DisplaySettings& settings)
   {
      m_textWrapping = settings.m_textWrapping;
      if (m_widget)
      {
         m_widget->setEnabled(!m_textWrapping);
         m_widget->setToolTip(
            m_textWrapping ?
               tr("Disable text wrapping to enable Minimap scrollbar") :
               QString());
      }
   }

   QPointer<QWidget> m_widget;
   QCheckBox* m_enabled;
   QSpinBox* m_width;
   QSpinBox* m_lineCountThresHold;
   QSpinBox* m_alpha;
   bool m_textWrapping;
};

MinimapSettings::MinimapSettings(QObject* parent)
: QObject(parent)
, m_enabled(true)
, m_width(Constants::MINIMAP_WIDTH_DEFAULT)
, m_lineCountThreshold(Constants::MINIMAP_MAX_LINE_COUNT_DEFAULT)
, m_alpha(Constants::MINIMAP_ALPHA_DEFAULT)
{
   QTC_ASSERT(!m_instance, return );
   m_instance = this;
   const QSettings* s = Core::ICore::settings();
   Utils::fromSettings(minimapPostFix, QLatin1String("text"), s, m_instance);
   m_settingsPage = new MinimapSettingsPage(this);
   ExtensionSystem::PluginManager::addObject(m_settingsPage);
}

MinimapSettings::~MinimapSettings()
{
   ExtensionSystem::PluginManager::removeObject(m_settingsPage);
   m_instance = 0;
}

MinimapSettings* MinimapSettings::instance()
{
   return m_instance;
}

void MinimapSettings::toMap(const QString& prefix, QVariantMap* map) const
{
   map->insert(prefix + QLatin1String(enabledKey), m_enabled);
   map->insert(prefix + QLatin1String(widthKey), m_width);
   map->insert(prefix + QLatin1String(lineCountThresholdKey),
               m_lineCountThreshold);
   map->insert(prefix + QLatin1String(alphaKey), m_alpha);
}

void MinimapSettings::fromMap(const QString& prefix, const QVariantMap& map)
{
   m_enabled =
      map.value(prefix + QLatin1String(enabledKey), m_enabled).toBool();
   m_width = map.value(prefix + QLatin1String(widthKey), m_width).toInt();
   m_lineCountThreshold =
      map.value(prefix + QLatin1String(lineCountThresholdKey),
                m_lineCountThreshold)
         .toInt();
   m_alpha = map.value(prefix + QLatin1String(alphaKey), m_alpha).toInt();
}

bool MinimapSettings::enabled()
{
   return m_instance->m_enabled;
}

int MinimapSettings::width()
{
   return m_instance->m_width;
}

int MinimapSettings::lineCountThreshold()
{
   return m_instance->m_lineCountThreshold;
}

int MinimapSettings::alpha()
{
   return m_instance->m_alpha;
}

void MinimapSettings::setEnabled(bool enabled)
{
   if (m_enabled != enabled)
   {
      m_enabled = enabled;
      emit enabledChanged(enabled);
   }
}

void MinimapSettings::setWidth(int width)
{
   if (m_width != width)
   {
      m_width = width;
      emit widthChanged(width);
   }
}

void MinimapSettings::setLineCountThreshold(int lineCountThreshold)
{
   if (m_lineCountThreshold != lineCountThreshold)
   {
      m_lineCountThreshold = lineCountThreshold;
      emit lineCountThresholdChanged(lineCountThreshold);
   }
}

void MinimapSettings::setAlpha(int alpha)
{
   if (m_alpha != alpha)
   {
      m_alpha = alpha;
      emit alphaChanged(alpha);
   }
}
}
}
