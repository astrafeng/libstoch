// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#include <fstream>
#include <memory>
#include <functional>
#ifdef USE_MPI
#include <boost/mpi.hpp>
#endif
#include <boost/lexical_cast.hpp>
#include <Eigen/Dense>
#include "geners/BinaryFileArchive.hh"
#include "libstoch/regression/BaseRegression.h"
#include "libstoch/core/grids/FullGrid.h"
#include "libstoch/dp/FinalStepDP.h"
#include "libstoch/dp/TransitionStepRegressionDP.h"
#include "libstoch/dp/OptimizerDPBase.h"

using namespace std;


double  DynamicProgrammingByRegression(const shared_ptr<libstoch::FullGrid> &p_grid,
                                       const shared_ptr<libstoch::OptimizerDPBase > &p_optimize,
                                       const shared_ptr<libstoch::BaseRegression> &p_regressor,
                                       const function<double(const int &, const Eigen::ArrayXd &, const Eigen::ArrayXd &)>  &p_funcFinalValue,
                                       const Eigen::ArrayXd &p_pointStock,
                                       const int &p_initialRegime,
                                       const string   &p_fileToDump
#ifdef USE_MPI
                                       , const boost::mpi::communicator &p_world
#endif

                                      )
{
    // from the optimizer get back the simulator
    shared_ptr< libstoch::SimulatorDPBase> simulator = p_optimize->getSimulator();
    // final values
    vector< shared_ptr< Eigen::ArrayXXd > >  valuesNext = libstoch::FinalStepDP(p_grid, p_optimize->getNbRegime())(p_funcFinalValue, simulator->getParticles().array());
    shared_ptr<gs::BinaryFileArchive> ar = make_shared<gs::BinaryFileArchive>(p_fileToDump.c_str(), "w");
    // name for object in archive
    string nameAr = "Continuation";
    // iterate on time steps
    for (int iStep = 0; iStep < simulator->getNbStep(); ++iStep)
    {
        Eigen::ArrayXXd asset = simulator->stepBackwardAndGetParticles();
        // conditional expectation operator
        p_regressor->updateSimulations(((iStep == (simulator->getNbStep() - 1)) ? true : false), asset);
        // transition object
        libstoch::TransitionStepRegressionDP transStep(p_grid, p_grid, p_optimize
#ifdef USE_MPI
                , p_world
#endif
                                                   );

        pair< vector< shared_ptr< Eigen::ArrayXXd > >, vector< shared_ptr< Eigen::ArrayXXd > > > valuesAndControl = transStep.oneStep(valuesNext, p_regressor);
        // dump continuation values
        transStep.dumpContinuationValues(ar, nameAr, iStep, valuesNext, valuesAndControl.second, p_regressor);
        valuesNext = valuesAndControl.first;
    }
    // interpolate at the initial stock point and initial regime
    return (p_grid->createInterpolator(p_pointStock)->applyVec(*valuesNext[p_initialRegime])).mean();
}
