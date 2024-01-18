// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef VERSION_H
#define VERSION_H
#define libstoch_VERSION "5.11"
#include <string>

/** \file version.h
 * \brief Defines libstoch version
 * \author Xavier Warin
 */

namespace libstoch
{
/// \brief get back library version
std::string getlibstochVersion();
}
#endif
