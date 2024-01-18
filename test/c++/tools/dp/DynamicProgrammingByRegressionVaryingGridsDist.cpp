// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifdef USE_MPI
#include <fstream>
#include <memory>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <boost/mpi.hpp>
#include <Eigen/Dense>
#include "geners/BinaryFileArchive.hh"
#include "libstoch/core/utils/comparisonUtils.h"
#include "libstoch/core/grids/FullGrid.h"
#include "libstoch/regression/BaseRegression.h"
#include "libstoch/dp/FinalStepDPDist.h"
#include "libstoch/dp/TransitionStepRegressionDPDist.h"
#include "libstoch/core/parallelism/reconstructProc0Mpi.h"
#include "libstoch/dp/OptimizerDPBase.h"
#include "libstoch/dp/SimulatorDPBase.h"

using namespace std;

double  DynamicProgrammingByRegressionVaryingGridsDist(const vector<double>    &p_timeChangeGrid,
        const vector<shared_ptr<libstoch::FullGrid> >   &p_grids,
        const shared_ptr<libstoch::OptimizerDPBase > &p_optimize,
        shared_ptr<libstoch::BaseRegression> &p_regressor,
        const function<double(const int &, const Eigen::ArrayXd &, const Eigen::ArrayXd &)>   &p_funcFinalValue,
        const Eigen::ArrayXd &p_pointStock,
        const int &p_initialRegime,
        const string   &p_fileToDump,
        const bool &p_bOneFile,
        const boost::mpi::communicator &p_world)
{
    // from the optimizer get back the simulation
    shared_ptr< libstoch::SimulatorDPBase> simulator = p_optimize->getSimulator();
    // identify last grid
    double currentTime = simulator->getCurrentStep();
    int iTime = p_timeChangeGrid.size() - 1;
    while (libstoch::isStrictlyLesser(currentTime, p_timeChangeGrid[iTime]))
        iTime--;
    shared_ptr<libstoch::FullGrid>  gridCurrent = p_grids[iTime];
    // final values
    vector< shared_ptr< Eigen::ArrayXXd > >  valuesNext = libstoch::FinalStepDPDist(gridCurrent, p_optimize->getNbRegime(), p_optimize->getDimensionToSplit(), p_world)(p_funcFinalValue, simulator->getParticles().array());
    shared_ptr<libstoch::FullGrid> gridPrevious = gridCurrent;
    // dump
    string toDump = p_fileToDump ;
    // test if one file generated
    if (!p_bOneFile)
        toDump +=  "_" + boost::lexical_cast<string>(p_world.rank());
    shared_ptr<gs::BinaryFileArchive> ar;
    if ((!p_bOneFile) || (p_world.rank() == 0))
        ar = make_shared<gs::BinaryFileArchive>(toDump.c_str(), "w");
    // name for object in archive
    string nameAr = "Continuation";
    for (int iStep = 0; iStep < simulator->getNbStep(); ++iStep)
    {
        Eigen::ArrayXXd asset = simulator->stepBackwardAndGetParticles();
        // update grid
        currentTime = simulator->getCurrentStep();
        while (libstoch::isStrictlyLesser(currentTime, p_timeChangeGrid[iTime]))
            iTime--;       // conditional expectation operator
        gridCurrent = p_grids[iTime];
        // conditional expectation operator
        p_regressor->updateSimulations(((iStep == (simulator->getNbStep() - 1)) ? true : false), asset);
        // transition object
        libstoch::TransitionStepRegressionDPDist transStep(gridCurrent, gridPrevious, p_optimize, p_world);
        pair< vector< shared_ptr< Eigen::ArrayXXd > >, vector< shared_ptr< Eigen::ArrayXXd > > >  valuesAndControl =  transStep.oneStep(valuesNext, p_regressor);
        transStep.dumpContinuationValues(ar, nameAr, iStep, valuesNext, valuesAndControl.second, p_regressor, p_bOneFile);
        valuesNext = valuesAndControl.first;
        gridPrevious = gridCurrent;
    }
    // reconstruct a small grid for interpolation
    return libstoch::reconstructProc0Mpi(p_pointStock, gridPrevious, valuesNext[p_initialRegime], p_optimize->getDimensionToSplit(), p_world).mean();

}
#endif
