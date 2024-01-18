// Copyright (C) 2019 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SIMULATESTEPTREE_H
#define SIMULATESTEPTREE_H
#ifdef OMP
#include <omp.h>
#endif
#ifdef USE_MPI
#include <boost/mpi.hpp>
#endif
#include <boost/lexical_cast.hpp>
#include "geners/BinaryFileArchive.hh"
#include "geners/Reference.hh"
#include "libstoch/dp/SimulateStepTreeBase.h"
#include "libstoch/tree/StateTreeStocks.h"
#include "libstoch/core/grids/SpaceGrid.h"
#include "libstoch/dp/OptimizerDPTreeBase.h"
#include "libstoch/tree/ContinuationValueTree.h"
#include "libstoch/tree/GridTreeValueGeners.h"

/** \file SimulateStepTree.h
 *  \brief  In simulation part, permits to  use  continuation values stored in in optimization step and
 *           calculate optimal control for tree method
 *  \author Xavier Warin
 */
namespace libstoch
{

/// \class SimulateStepTree SimulateStepTree.h
/// One step in forward simulation using tree for uncertainty
class SimulateStepTree : public SimulateStepTreeBase
{
private :

    std::shared_ptr<SpaceGrid>  m_pGridFollowing ; ///< global grid at following time step
    std::shared_ptr<libstoch::OptimizerDPTreeBase >          m_pOptimize ; ///< optimizer solving the problem for one point and one step
    std::vector< GridTreeValue >  m_continuationObj ; ///< to store continuation value per regime  on the grid at following step
#ifdef USE_MPI
    boost::mpi::communicator  m_world; ///< Mpi communicator
#endif

public :

    /// \brief default
    SimulateStepTree() {}
    virtual ~SimulateStepTree() {}

    /// \brief Constructor
    /// \param p_ar               Archive where continuation values are stored
    /// \param p_iStep            Step number indentifier
    /// \param p_nameCont         Name use to store conuation valuation
    /// \param p_pGridFollowing   grid at following time step
    /// \param p_pOptimize        Optimize object defining the transition step
    /// \param p_world            MPI communicator
    SimulateStepTree(gs::BinaryFileArchive &p_ar,  const int &p_iStep,  const std::string &p_nameCont,
                     const   std::shared_ptr<SpaceGrid> &p_pGridFollowing, const  std::shared_ptr<libstoch::OptimizerDPTreeBase > &p_pOptimize
#ifdef USE_MPI
                     , const boost::mpi::communicator &p_world
#endif
                    );

    /// \brief Define one step arbitraging between possibhle commands
    /// \param p_statevector    Vector of states (regime, stock descritor, uncertainty)
    /// \param p_phiInOut       actual contract values modified at current time step by applying an optimal command (number of function by numver of simulations)
    void oneStep(std::vector<StateTreeStocks > &p_statevector, Eigen::ArrayXXd  &p_phiInOut) const ;

};
}
#endif /* SIMULATESTEPTREE_H */
