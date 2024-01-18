// Copyright (C) 2019 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef OPTIMIZERBASEINTERP_H
#define OPTIMIZERBASEINTERP_H
#include <Eigen/Dense>
#include "libstoch/core/utils/StateWithStocks.h"
#include "libstoch/core/grids/SpaceGrid.h"
#include "libstoch/regression/BaseRegression.h"
#include "libstoch/regression/ContinuationValue.h"
#include "libstoch/regression/GridAndRegressedValue.h"
#include "libstoch/dp/OptimizerBase.h"
#include "libstoch/dp/SimulatorDPBase.h"

/** \file OptimizerBaseInterp.h
 *  \brief Define an abstract class for Dynamic Programming problems solved by Monte Carlo methods  with interpolation for Bellman values
 *     \author Xavier Warin
 */

namespace libstoch
{

/// \class OptimizerBaseInterp OptimizerBaseInterp.h
///  Base class for optimizer for Dynamic Programming  with interpolation for Bellman values  with and without regression methods
class OptimizerBaseInterp : public OptimizerBase
{


public :

    OptimizerBaseInterp() {}

    virtual ~OptimizerBaseInterp() {}


    /// \brief Defines a step in simulation using interpolation in controls
    /// \param p_grid          grid at arrival step after command
    /// \param p_control       defines the controls
    /// \param p_state         defines the state value (modified)
    /// \param p_phiInOut      defines the value function (modified): size number of functions to follow
    virtual void stepSimulateControl(const std::shared_ptr< libstoch::SpaceGrid>   &p_grid, const std::vector< libstoch::GridAndRegressedValue  > &p_control,
                                     libstoch::StateWithStocks &p_state,
                                     Eigen::Ref<Eigen::ArrayXd> p_phiInOut) const = 0 ;

    /// \brief get the simulator back
    virtual std::shared_ptr< libstoch::SimulatorDPBase > getSimulator() const = 0;


};
}
#endif /* OPTIMIZERBASEINTERP_H */
