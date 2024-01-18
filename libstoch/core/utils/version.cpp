// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#include <boost/version.hpp>
#include <iostream>
#include <Eigen/Core>
#include "libstoch/core/utils/version.h"

using namespace std ;

namespace libstoch
{
std::string getlibstochVersion()
{
    std::stringstream libstoch_version;

    libstoch_version << "libstoch version "
                  << libstoch_VERSION ;

    libstoch_version << ";  Boost version " << BOOST_VERSION / 100000 << "." << BOOST_VERSION / 100 % 1000 << "." << BOOST_VERSION % 100 << endl;

    libstoch_version << ";  Eigen version 3." << EIGEN_MAJOR_VERSION << "." << EIGEN_MINOR_VERSION ;

    return libstoch_version.str();
}
}
