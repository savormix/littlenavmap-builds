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

#ifndef LNM_ROUTEEXPORTFLAGS_H
#define LNM_ROUTEEXPORTFLAGS_H

#include <QFlags>

namespace re {
enum RouteExportType
{
  UNKNOWN,
  VFP, /* VATSIM VFP format, XML - vPilot or SWIFT */
  IVAP, /* IVAO IVAP, INI */
  XIVAP /* IVAO X-IVAP, INI */
};

}

namespace rexp {

/* Enum for all export formats. Order in multiexport dialog list is defined by order in this enumeration.
 *  Do not change ids */
enum RouteExportFormatType : quint16
{
  /* Simulators ================ */
  NO_TYPE = 255,
  LNMPLN = 35, /* LNM own plan format */

  PLN = 0, /* FSX/P3D XML PLN format */
  PLNANNOTATED = 1, /* Save as above but with annotations for procedures as used by LNM before 2.4.5 */
  PLNMSFS = 31, /* Microsoft Flight Simulator 2020 */ // LAST VALUE
  FMS3 = 2, /* Old X-Plane FMS 3 */
  FMS11 = 3, /* New X-Plane FMS 11 */
  FLIGHTGEAR = 4, /* FlightGear XML */
  PROSIM = 5, /* ProSim */

  /* Garmin ================ */
  RXPGNS = 6, /* Reality XP GNS */
  RXPGNSUWP = 32, /* Reality XP GNS with user defined waypoints */
  RXPGTN = 7, /* Reality XP GTN */
  RXPGTNUWP = 33, /* Reality XP GTN with user defined waypoints */
  GFP = 8, /* Garmin GFP Format */
  GFPUWP = 34, /* Garmin GFP Format with user defined waypoints */

  /* Online ================ */
  VFP = 9, /* vPilot VATSIM */
  IVAP = 10, /* IVAP for IVAO */
  XIVAP = 11, /* X-IVAP for IVAO */

  /* FMC ================ */
  XFMC = 12, /* X-FMC */
  UFMC = 13, /* UFMC */

  /* Aircraft ================ */
  FLP = 14, /* Aerosoft Airbus FLP */
  FLPCRJ = 30, /* Aerosoft CRJ FLP */
  FLPCRJMSFS = 37, /* Aerosoft CRJ FLP for MSFS */
  TXT = 15, /* Rotate MD-80 and others */
  TXTJAR = 36, /* Jar Design */
  RTE = 16, /* PMDG RTE format */
  FPR = 17, /* Majestic Dash binary format */
  FPL = 18, /* IXEG 737 */
  CORTEIN = 19, /* Flight factor airbus */
  FLTPLAN = 20, /* iFly */
  BBS = 21, /* BlackBox Simulations Airbus */
  FEELTHEREFPL = 22, /* FeelThere or Wilco aircraft */
  LEVELDRTE = 23, /* Level-D */
  QWRTE = 24, /* QualityWings Aircraft RTE */
  MDR = 25, /* Leonardo Maddog X */
  TFDI = 26, /* TFDi Design 717 */
  FSLABS = 38, /* FSLabs A320 - FSX PLN */

  /* Other ================ */
  EFBR = 27, /* AivlaSoft EFB */
  GPX = 28, /* Garmin exchange format. Not a flight plan format.  */
  HTML = 29 /* Export as HTML page */

         // Next = 39
};

/* Flags for export format. Some flags are saved to the configuration,
 * some are used in temporary objects and some are hardcoded in configuration. */
enum RouteExportFormatFlag : quint16
{
  NONE = 0,
  FILE = 1 << 0, /* Append to file instead of saving to folder.
                  * path and default path contain filename instead of directory if this is the case. */
  SELECTED = 1 << 1, /* Selected for multiexport in dialog. Saved to configuration. */

  MANUAL = 1 << 2, /* Format is cloned and selected for manual export from file menu. Temporary flag. */
  FILEDIALOG = 1 << 3, /* Format is cloned and selected for manual export from file dialog.
                        *  Forces file dialog to be shown. Temporary flag. */

  PARKING = 1 << 4, /* Format allows to import parking start position */
  AIRPORTS = 1 << 5, /* Valid departure and destination airports are needed */
  CYCLE = 1 << 6, /* Format needs a valid AIRAC cycle */
  GARMIN_AS_WAYPOINTS = 1 << 7, /* Format to export Garmin as waypoints. */

  SAVED_FLAGS = SELECTED /* Saved to configuration. */
};

Q_DECLARE_FLAGS(RouteExportFormatFlags, RouteExportFormatFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(rexp::RouteExportFormatFlags);

} // namespace rexp

#endif // LNM_ROUTEEXPORTFLAGS_H
