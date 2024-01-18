// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifdef USE_MPI
#ifndef DYNAMICPROGRAMMINGPORTFOLIODIST_H
#define DYNAMICPROGRAMMINGPORTFOLIODIST_H
#include <fstream>
#include <memory>
#include <functional>
#include <boost/mpi.hpp>
#include <Eigen/Dense>
#include "libstoch/core/grids/SpaceGrid.h"
#include "libstoch/core/grids/FullGrid.h"
#include "test/c++/tools/dp/OptimizePortfolioDP.h"

/* \file DynamicProgrammingPortfolio.h
 * \brief Defines a simple  programm  showing how to optimize a portfolio with Monte Carlo (according to a given criterium in expectation)  distributing calculations
 *        A simple grid  is used
 *        The optimization is rather generic.
 * \author Xavier Warin
 */

/// \brief Principal function to optimize  the portfolio : distributed version where data are split onj processor
///        MPI is need permitting speed up and size up for the application
/// \param p_grid                  grid used for portfolio discretization
/// \param p_optimize              optimizer defining the optimization problem for the portfolio
/// \param p_nbMesh                number of mesh in each direction (here only the asset value is discretized)
/// \param p_funcFinalValue        function defining the final value (the payoff associated to  the portfolio)
/// \param p_initialPortfolio      initial portfolio value
/// \param p_fileToDump            file to dump optimal command
/// \param p_bOneFile              true if
/// \param p_world                 MPI communicator
///
double  DynamicProgrammingPortfolioDist(const std::shared_ptr<libstoch::FullGrid> &p_grid,
                                        const std::shared_ptr<OptimizePortfolioDP> &p_optimize,
                                        const Eigen::ArrayXi &p_nbMesh,
                                        const std::function<double(const int &, const Eigen::ArrayXd &, const Eigen::ArrayXd &)>  &p_funcFinalValue,
                                        const Eigen::ArrayXd &p_initialPortfolio,
                                        const std::string   &p_fileToDump,
                                        const bool &p_bOneFile,
                                        const boost::mpi::communicator &p_world);
#endif
#endif
