/*****************************************************************************
* Copyright 2015-2020 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "common/dirtool.h"

#include "common/constants.h"
#include "gui/dialog.h"
#include "gui/helphandler.h"
#include "options/optiondata.h"
#include "settings/settings.h"

#include <QDir>
#include <QDebug>
#include <QUrl>

const static QChar SEP(QDir::separator());

DirTool::DirTool(QWidget *parent, const QString& base, const QString& appName, const QString& settingsKeyPrefix)
  : parentWidget(parent), documentsDir(base), settingsPrefix(settingsKeyPrefix)
{
  documentsDir = base;
  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  applicationDir = tr("%1 Files").arg(appName);

  // #ifndef QT_NO_DEBUG
  // applicationDir += " - DEBUG";
  // #endif

  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  flightPlanDir = tr("Flight Plans");

  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  perfDir = tr("Aircraft Performance");

  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  layoutDir = tr("Window Layouts");

  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  airspaceDir = tr("User Airspaces");

  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  globeDir = tr("GLOBE Elevation Data");

  /*: Default folder name which might be created initially by program.
   * Change only once. Otherwise program might suggest the user to create the new folders again */
  airportsDir = tr("Airports");
}

void DirTool::run()
{
  QString message;

  message.append(tr("<p style='white-space:pre'>"
                      "<b>Little Navmap can create a recommended directory structure "
                        "to store all your files<br/>in your documents folder.</b></p>"
                        "<p>The following folders will be created:</p>"
                          "<p><b>%1</b><br/>"
                          "Top level directory for all files of Little Navmap "
                          "containing the following sub-directories:</p>").
                 arg(QDir::toNativeSeparators(documentsDir + SEP + applicationDir)));

  // Show list of folders and comment =========
  message.append(tr("<ul>"));
  message.append(tr("<li><b>%1</b><br/>"
                    "For flight plans in Little Navmap's own format <code>.lnmpln</code></li>").
                 arg(flightPlanDir));
  message.append(tr("<li><b>%1</b><br/>"
                    "Directory for aircraft performance files (<code>.lnmperf</code>)</li>").
                 arg(perfDir));
  message.append(tr("<li><b>%1</b><br/>"
                    "For saved window layouts (<code>.lnmlayout</code>)</li>").
                 arg(layoutDir));
  message.append(tr("<li><b>%1</b><br/>"
                    "A place to store PDF, text, image or other files that are linked in the aiport information").
                 arg(airportsDir));
  message.append(tr("<li><b>%1</b><br/>"
                    "User defined airspaces in OpenAir format</li>").
                 arg(airspaceDir));
  message.append(tr("<li><b>%1</b><br/>"
                    "A place for the GLOBE data used by the flight plan elevation profile</li>").
                 arg(globeDir));
  message.append(tr("</li></ul>"));

  message.append(tr("<p>This step is optional.</p>"));

  QUrl url = atools::gui::HelpHandler::getHelpUrlWeb(lnm::helpOnlineInstallDirUrl, lnm::helpLanguageOnline());
  message.append(tr("<p><a href=\"%1\">Click here for more information in the Little Navmap online manual</a></p>").arg(url.toString()));

  message.append(tr("<p>Should Little Navmap create these directories now?</p>"));

  int result = atools::gui::Dialog(parentWidget).showQuestionMsgBox(settingsPrefix, message,
                                                                    QObject::tr("Do not &show this dialog again."),
                                                                    QMessageBox::Yes | QMessageBox::No,
                                                                    QMessageBox::No, QMessageBox::No);

  if(result == QMessageBox::Yes)
  {
    qDebug() << Q_FUNC_INFO;
    createAllDirs();

    if(!errors.isEmpty())
      QMessageBox::warning(parentWidget, QCoreApplication::applicationName(),
                           tr("Errors creating directory structure:\n%1").arg(errors.join("\n")));
    else
      updateOptions();
  }
}

bool DirTool::runIfMissing()
{
  bool hasDirs = hasAllDirs();

  qDebug() << Q_FUNC_INFO << "hasDirs" << hasDirs;

  if(!hasDirs)
    run();

  return hasDirs;
}

QString DirTool::getApplicationDir() const
{
  return QDir::toNativeSeparators(QFileInfo(documentsDir + QDir::separator() + applicationDir).absoluteFilePath());
}

bool DirTool::createAllDirs()
{
  errors.clear();

  mkdirBase();
  mkdir(flightPlanDir);
  mkdir(perfDir);
  mkdir(layoutDir);
  mkdir(airspaceDir);
  mkdir(airportsDir);
  mkdir(globeDir);

  return errors.isEmpty();
}

void DirTool::mkdir(const QString& dir)
{
  QFileInfo fi(documentsDir + SEP + applicationDir + SEP + dir);
  if(!fi.exists())
  {
    if(!QDir(documentsDir + SEP + applicationDir).mkdir(dir))
      errors.append(tr("Cannot create directory \"%1\"").arg(fi.filePath()));
  }
  else
  {
    if(fi.isFile())
      errors.append(tr("Cannot create directory \"%1\". File with same name already exists.").
                    arg(QDir::toNativeSeparators(fi.filePath())));
  }
}

void DirTool::mkdirBase()
{
  QFileInfo fi(documentsDir + SEP + applicationDir);
  if(!fi.exists())
  {
    if(!QDir(documentsDir).mkdir(applicationDir))
      errors.append(tr("Cannot create directory \"%1\"").arg(QDir::toNativeSeparators(fi.filePath())));
  }
  else
  {
    if(fi.isFile())
      errors.append(tr("Cannot create directory \"%1\". File with same name already exists.").arg(fi.filePath()));
  }
}

bool DirTool::hasDir(const QString& dir)
{
  QFileInfo fi(d(dir));
  return fi.exists() && fi.isDir();
}

QString DirTool::d(const QString& dir)
{
  return documentsDir + SEP + applicationDir + SEP + dir;
}

bool DirTool::hasAllDirs()
{
  return hasDir(flightPlanDir) && hasDir(perfDir) && hasDir(layoutDir) && hasDir(airspaceDir) && hasDir(globeDir) && hasDir(airportsDir);
}

void DirTool::updateOptions()
{
  atools::settings::Settings& settings = atools::settings::Settings::instance();
  settings.setValue("Route/LnmPlnFileDialogDir", d(flightPlanDir));
  settings.setValue("AircraftPerformance/FileDialogDir", d(perfDir));
  settings.setValue("WindowLayout/FileDialogDir", d(layoutDir));
  settings.setValue("Database/AirspaceConfig_lineEditAirspacePath", d(airspaceDir));
  settings.setValue("OptionsDialog/CacheFileDialogGlobeFileDialogDir", d(globeDir));
  atools::settings::Settings::syncSettings();
}
