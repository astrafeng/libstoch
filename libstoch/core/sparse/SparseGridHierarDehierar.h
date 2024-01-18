// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SPARSEGRIDHIERARDEHIERAR_H
#define  SPARSEGRIDHIERARDEHIERAR_H
#include  <Eigen/Dense>
#include "libstoch/core/sparse/sparseGridTypes.h"

/** \file SparseGridHierarDehierar.h
 * \brief Base class for Hierarchization and Dehierarchization
 * \author Xavier Warin
 */

namespace libstoch
{

/// \class HierarDehierar SparseGridHierarDehierar.h
///  Abstract class for Hierarchization and Dehierarchization
class HierarDehierar
{
public :

    /// \brief Default constructor
    HierarDehierar() {}

    /// \brief Default destructor
    virtual ~HierarDehierar() {}

    /// \brief Get root point
    /// \param p_levelRoot     root level
    /// \param p_positionRoot  root position
    virtual void  get_root(Eigen::ArrayXc &p_levelRoot, Eigen::ArrayXui   &p_positionRoot)  = 0;


};
}
#endif /*  SPARSEGRIDHIERARDEHIERAR_H */
