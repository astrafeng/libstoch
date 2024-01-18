// Copyright (C) 2019 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SIMULATESTEPTREEBASE_H
#define SIMULATESTEPTREEBASE_H
#include <vector>
#include <Eigen/Dense>
#include "libstoch/tree/StateTreeStocks.h"

/** SimulateStepTreeBase.h
 *  Base class for one time step of simulation with trees
 * \author Xavier Warin
 */

namespace libstoch
{
/// \class SimulateStepTreeBase SimulateStepTreeBase.h
/// One time  step  in simulation
class SimulateStepTreeBase
{

public :
    /// \brief Define one step arbitraging between possible commands
    /// \param p_statevector    Vector of states (regime, stock descriptor, uncertainty node number)
    /// \param p_phiInOut       actual contract value modified at current time step by applying an optimal command (size number of functions by number of simulations) in regression methods and (size number of functions by number of nodes at next date) for trees
    virtual void oneStep(std::vector<StateTreeStocks > &p_statevector, Eigen::ArrayXXd  &p_phiInOut) const = 0;
};

}


#endif /* SIMULATESTEPTREEBASE_H */
