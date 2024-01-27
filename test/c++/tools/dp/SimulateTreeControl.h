
#ifndef SIMULATEREGTREECONTROL_H
#define SIMULATEREGTREECONTROL_H
#include <Eigen/Dense>
#include <functional>
#include <memory>
#ifdef USE_MPI
#include <boost/mpi.hpp>
#endif
#include "geners/BinaryFileArchive.hh"
#include "libstoch/tree/StateTreeStocks.h"
#include "libstoch/core/grids/SpaceGrid.h"
#include "libstoch/dp/SimulateStepTreeControl.h"
#include "libstoch/dp/OptimizerDPTreeBase.h"
#include "libstoch/dp/SimulatorDPBaseTree.h"


/** \file SimulateTree.h
 *  \brief Defines a simple program showing how to use simulation : here the optimal control calculated in optimization are used
 *        Tree method
 *        A simple grid  is used
 *  \author Xavier Warin
 */

/// \brief Simulate the optimal strategy , threaded version, with optimal control
/// \param p_grid                   grid used for  deterministic state (stocks for example)
/// \param p_optimize               optimizer defining the optimization between two time steps
/// \param p_funcFinalValue         function defining the final value
/// \param p_pointStock             initial point stock
/// \param p_initialRegime          regime at initial date
/// \param p_fileToDump             name of the file used to dump continuation values in optimization
/// \param p_world                  MPI communicator
double SimulateTreeControl(const std::shared_ptr<libstoch::SpaceGrid> &p_grid,
                           const std::shared_ptr<libstoch::OptimizerDPTreeBase > &p_optimize,
                           const std::function<double(const int &, const Eigen::ArrayXd &, const Eigen::ArrayXd &)>   &p_funcFinalValue,
                           const Eigen::ArrayXd &p_pointStock,
                           const int &p_initialRegime,
                           const std::string   &p_fileToDump
#ifdef USE_MPI
                           , const boost::mpi::communicator &p_world
#endif
                          )
{
    // from the optimizer get back the simulator
    std::shared_ptr< libstoch::SimulatorDPBaseTree> simulator = p_optimize->getSimulator();
    int nbStep = simulator->getNbStep();
    std::vector< libstoch::StateTreeStocks> states;
    states.reserve(simulator->getNbSimul());
    for (int is = 0; is < simulator->getNbSimul(); ++is)
        states.push_back(libstoch::StateTreeStocks(p_initialRegime, p_pointStock, 0));
    gs::BinaryFileArchive ar(p_fileToDump.c_str(), "r");
    // name for continuation object in archive
    std::string nameAr = "Continuation";
    // cost function
    Eigen::ArrayXXd costFunction = Eigen::ArrayXXd::Zero(p_optimize->getSimuFuncSize(), simulator->getNbSimul());
    // iterate on time steps
    for (int istep = 0; istep < nbStep; ++istep)
    {
        libstoch::SimulateStepTreeControl(ar, nbStep - 1 - istep, nameAr, p_grid, p_optimize
#ifdef USE_MPI
                                       , p_world
#endif
                                      ).oneStep(states, costFunction);

        // new date
        simulator->stepForward();
        for (int is = 0; is < simulator->getNbSimul(); ++is)
            states[is].setStochasticRealization(simulator->getNodeAssociatedToSim(is));

    }
    // final : accept to exercise if not already done entirely (here suppose one function to follow)
    for (int is = 0; is < simulator->getNbSimul(); ++is)
        costFunction(0, is) += p_funcFinalValue(states[is].getRegime(), states[is].getPtStock(), simulator->getValueAssociatedToNode(states[is].getStochasticRealization()));
    return costFunction.mean();
}
#endif /* SIMULATETREECONTROL_H */
