// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SPARSEGRIDHIERARONEPOINTBOUND_H
#define SPARSEGRIDHIERARONEPOINTBOUND_H
#include "libstoch/core/sparse/SparseGridHierarOnePoint.h"

/** \file SparseGridHierarOnePointBound.h
 * \brief Base class to Hierarchize a single point
 * \author Xavier Warin
 */

namespace libstoch
{

/// \class SparseGridHierarOnePointBound SparseGridHierarOnePointBound.h
/// Abstract class for only only point Hierarchization with boundary points
class SparseGridHierarOnePointBound : SparseGridHierarOnePoint
{
public :

    /// \brief Default constructor
    SparseGridHierarOnePointBound() {}

    /// \brief Default destructor
    virtual ~SparseGridHierarOnePointBound() {}
};
}

#endif /* SPARSEGRIDHIERARONEPOINTBOUND_H */
