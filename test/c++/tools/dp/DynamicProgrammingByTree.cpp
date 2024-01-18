// Copyright (C) 2019 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#include <fstream>
#include <memory>
#include <functional>
#include <array>
#ifdef USE_MPI
#include <boost/mpi.hpp>
#endif
#include <boost/lexical_cast.hpp>
#include <Eigen/Dense>
#include "geners/BinaryFileArchive.hh"
#include "libstoch/tree/Tree.h"
#include "libstoch/core/grids/FullGrid.h"
#include "libstoch/dp/FinalStepDP.h"
#include "libstoch/dp/TransitionStepTreeDP.h"
#include "libstoch/dp/OptimizerDPTreeBase.h"
#include "libstoch/dp/SimulatorDPBaseTree.h"

using namespace std;


double  DynamicProgrammingByTree(const shared_ptr<libstoch::FullGrid> &p_grid,
                                 const shared_ptr<libstoch::OptimizerDPTreeBase > &p_optimize,
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
    shared_ptr< libstoch::SimulatorDPBaseTree> simulator = p_optimize->getSimulator();
    // final values
    vector< shared_ptr< Eigen::ArrayXXd > >  valuesNext = libstoch::FinalStepDP(p_grid, p_optimize->getNbRegime())(p_funcFinalValue, simulator->getNodes());
    shared_ptr<gs::BinaryFileArchive> ar = make_shared<gs::BinaryFileArchive>(p_fileToDump.c_str(), "w");
    // name for object in archive
    string nameAr = "Continuation";
    // iterate on time steps
    for (int iStep = 0; iStep < simulator->getNbStep(); ++iStep)
    {
        simulator->stepBackward();
        // probabilities
        std::vector<double>  proba = simulator->getProba();
        // get connection between nodes
        std::vector< std::vector<std::array<int, 2>  > >  connected = simulator->getConnected();
        // conditional expectation operator
        shared_ptr<libstoch::Tree> tree = std::make_shared<libstoch::Tree>(proba, connected);
        // transition object
        libstoch::TransitionStepTreeDP transStep(p_grid, p_grid, p_optimize
#ifdef USE_MPI
                                              , p_world
#endif

                                             );
        pair< vector< shared_ptr< Eigen::ArrayXXd > >, vector< shared_ptr< Eigen::ArrayXXd > > > valuesAndControl = transStep.oneStep(valuesNext, tree);
        // dump continuation values
        transStep.dumpContinuationValues(ar, nameAr, iStep, valuesNext, valuesAndControl.second, tree);
        valuesNext = valuesAndControl.first;

    }
    // interpolate at the initial stock point and initial regime
    return (p_grid->createInterpolator(p_pointStock)->applyVec(*valuesNext[p_initialRegime])).mean();
}
