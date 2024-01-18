// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SIMULATEREGREGRESSIONVARYINGGRIDSCONTROLDIST_H
#define SIMULATEREGREGRESSIONVARYINGGRIDSCONTROLDIST_H
#include <functional>
#include <memory>
#include <Eigen/Dense>
#include <boost/mpi.hpp>
#include "geners/BinaryFileArchive.hh"
#include "libstoch/core/utils/comparisonUtils.h"
#include "libstoch/core/grids/FullGrid.h"
#include "libstoch/core/utils/StateWithStocks.h"
#include "libstoch/dp/SimulateStepRegressionControlDist.h"
#include "libstoch/dp/OptimizerDPBase.h"
#include "libstoch/dp/SimulatorDPBase.h"


/** \file SimulateRegressionVaryingGridsControlDist.h
 *  \brief Defines a simple program showing how to use simulation
 *        A simple grid  is used
 *  \author Xavier Warin
 */


/// \brief Simulate the optimal strategy , mpi version
/// \param p_timeChangeGrid    date for changing grids
/// \param p_grids             grids depending on time
/// \param p_optimize          Optimiser defining the problem to solve
/// \param p_funcFinalValue    function defining the final value
/// \param p_pointStock        initial point stock
/// \param p_initialRegime     regime at initial date
/// \param p_fileToDump        name associated to dumped bellman values
/// \param p_bOneFile          do we store continuation values  in only one file
/// \param p_world                  MPI communicator
double SimulateRegressionVaryingGridsControlDist(const std::vector<double>    &p_timeChangeGrid,
        const std::vector<std::shared_ptr<libstoch::FullGrid> >   &p_grids,
        const std::shared_ptr<libstoch::OptimizerDPBase > &p_optimize,
        const std::function<double(const int &, const Eigen::ArrayXd &, const Eigen::ArrayXd &)>  &p_funcFinalValue,
        const Eigen::ArrayXd &p_pointStock,
        const int &p_initialRegime,
        const std::string   &p_fileToDump,
        const bool &p_bOneFile,
        const boost::mpi::communicator &p_world)
{
    // from the optimizer get back the simulation
    std::shared_ptr< libstoch::SimulatorDPBase> simulator = p_optimize->getSimulator();
    int nbStep = simulator->getNbStep();
    std::vector< libstoch::StateWithStocks> states;
    states.reserve(simulator->getNbSimul());
    for (int is = 0; is < simulator->getNbSimul(); ++is)
        states.push_back(libstoch::StateWithStocks(p_initialRegime, p_pointStock, Eigen::ArrayXd::Zero(simulator->getDimension())));
    std::string toDump = p_fileToDump ;
    // test if one file generated
    if (!p_bOneFile)
        toDump +=  "_" + boost::lexical_cast<std::string>(p_world.rank());
    gs::BinaryFileArchive ar(toDump.c_str(), "r");
    // name for continuation object in archive
    std::string nameAr = "Continuation";
    // cost function
    Eigen::ArrayXXd costFunction = Eigen::ArrayXXd::Zero(p_optimize->getSimuFuncSize(), simulator->getNbSimul());
    for (int istep = 0; istep < nbStep; ++istep)
    {
        // get time step
        double nextTime = simulator->getCurrentStep() + simulator->getStep() ;
        int iTime = p_timeChangeGrid.size() - 1;
        while (libstoch::isStrictlyLesser(nextTime, p_timeChangeGrid[iTime]))
            iTime--;       // conditional expectation operator
        // current time
        double timeCur = simulator->getCurrentStep();
        int iTimeCurrent = iTime;
        while (libstoch::isStrictlyLesser(timeCur, p_timeChangeGrid[iTimeCurrent]))
            iTimeCurrent--;
        libstoch::SimulateStepRegressionControlDist(ar, nbStep - 1 - istep, nameAr, p_grids[iTimeCurrent], p_grids[iTime], p_optimize, p_bOneFile, p_world).oneStep(states, costFunction);
        // new stochastic state
        Eigen::ArrayXXd particles =  simulator->stepForwardAndGetParticles();
        for (int is = 0; is < simulator->getNbSimul(); ++is)
            states[is].setStochasticRealization(particles.col(is));

    }
    // final : accept to exercise if not already done entirely
    for (int is = 0; is < simulator->getNbSimul(); ++is)
        costFunction(0, is) += p_funcFinalValue(states[is].getRegime(), states[is].getPtStock(), states[is].getStochasticRealization()) * simulator->getActu();

    return costFunction.mean();
}

#endif /* SIMULATEREGRESSIONVARYINGGRIDSCONTROLDIST_H */
