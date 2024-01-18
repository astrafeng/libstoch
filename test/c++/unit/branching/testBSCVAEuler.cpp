// Copyright (C) 2018 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#include <functional>
#include <boost/mpi.hpp>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>
#include <Eigen/Dense>
#include <trng/yarn4.hpp>
#include "libstoch/branching/ExpDist.h"
#include "libstoch/branching/GammaDist.h"
#include "libstoch/branching/solvePDEMC.h"


/** \file mainBSCVA.cpp
 * \brief solve the CVA problem in dimension 6 using Euler Scheme routine
 *
 *         Equation
 *         \f[
 *             (-\partial_t u-{\cal L} u)(t,x)  = beta*(std::max(u,0.)-u)
 *         \f]
 *   P-H Labordere test case for primal-dual bound with deep learning
 *
 * \author Xavier Warin
 */

using namespace std;
using  namespace Eigen;



#if defined   __linux
#include <fenv.h>
#define enable_abort_on_floating_point_exception() feenableexcept(FE_DIVBYZERO | FE_INVALID)
#endif


int main(int argc, char *argv[])
{

#if defined   __linux
    enable_abort_on_floating_point_exception();
#endif

    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;

    double mat = 1.; // maturity
    double volat = 0.2; // VOL
    double s0 = 1; // initial value
    double trend = - 0.5 * volat * volat;
    double beta  = 0.03;

    // g terminal function
    auto payOff([](const Matrix< double, 6, 1> &p_x)
    {
        Array< double, 6, 1> asset = p_x.array().exp();
        double ret  = 0;
        for (int id = 0; id < 6; ++id)
        {
            double retLoc = 1.;
            if (asset(id) > 1)
                retLoc -= 2 ;
            ret += retLoc;
        }
        return ret ;
    }
               );
    function< double (const Matrix<double, 6, 1>&)>   g(std::cref(payOff));

    // create function for non linearity
    auto  fLambda([ beta ](const double &, const Matrix<double, 6, 1> &, const double & p_y)
    {
        return  beta * (std::max(p_y, 0.) - p_y);
    });
    function< double (const double &, const Matrix<double, 6, 1>&, const double &) > fNonLin(std::cref(fLambda));

    // trend
    auto muFunc([trend](const double &, const Eigen::Matrix<double, 6, 1> &)
    {
        return Eigen::Matrix<double, 6, 1>::Constant(trend);
    });
    std::function<  Eigen::Matrix<double, 6, 1> (const double &, const Eigen::Matrix<double, 6, 1> &) >  mu(std::cref(muFunc));

    // volatility
    auto volFunc([volat](const double &, const Eigen::Matrix<double, 6, 1> &)
    {
        return volat * Eigen::Matrix<double, 6, 6>::Identity();
    });
    std::function<  Eigen::Matrix<double, 6, 6>(const double &, const Eigen::Matrix<double, 6, 1> &) >  vol(std::cref(volFunc));


    // initial point (log of asset value)
    Matrix<double,  6, 1> point = Matrix<double,  6, 1>::Constant(log(s0));


    // nest on numb switches
    Array2i  nbSwitch;
    nbSwitch << 1,  2 ;

    // intensity for switches
    double lambda = 0.1;

    // nest for particle number
    int nbpart = 5;

    for (int isw = 0; isw < nbSwitch.size(); ++isw)
    {

        if (world.rank() == 0)
        {
            cout << " SWITCH NB " <<  nbSwitch(isw) << endl ;
            cout << "****************" << endl ;
        }

        libstoch::ExpDist law(lambda);
        if (world.rank() == 0)
        {
            std::cout << " Lambd " << lambda << std::endl ;
            cout << " -----------------" << endl ;
        }

        // nest on particles
        for (int ipart = 0; ipart < nbpart; ++ipart)
        {
            std::vector<int > nbSim(nbSwitch(isw));
            nbSim[0] = 36000 * pow(2, ipart);
            nbSim[1] = 140 * pow(2, ipart);
            if (world.rank() == 0)
            {
                cout << " NBPART " << nbSim[0] <<  "  " << nbSim[1] << endl ;
            }

            // no Euler
            double stepEuler = 10;

            trng::yarn4 gen;

            boost::timer::cpu_timer tt;
            std::tuple< double,  double > val = libstoch::solvePDEMCEuler<6, libstoch::ExpDist, trng::yarn4>(mu, vol, fNonLin, point, 0., mat, law,  g, nbSim, stepEuler, gen);

            if (world.rank() == 0)
            {
                boost::chrono::duration<double> seconds = boost::chrono::nanoseconds(tt.elapsed().user);
                cout << " Value  PDE " << std::get<0>(val) <<   "  std " << std::get<1>(val) <<   " TIME " << seconds.count() <<   endl ;
            }
        }
    }
}
