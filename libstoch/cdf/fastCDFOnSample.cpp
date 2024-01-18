// Copyright (C) 2020 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#include <Eigen/Dense>
#include <libstoch/cdf/nDDominanceAlone.h>

using namespace Eigen ;

namespace libstoch
{

ArrayXd fastCDFOnSample(const Eigen::ArrayXXd &p_x, const Eigen::ArrayXd &p_y)
{

    ArrayXd cdfDivid(p_y.size());

    // dominance excluding current point
    nDDominanceAlone(p_x, p_y, cdfDivid);
    cdfDivid += p_y;
    return cdfDivid / p_y.size();
}
}

