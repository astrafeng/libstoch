// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef CONSTANT_H
#define CONSTANT_H
/** \file constant.h
 *  \brief Define constants for program
 *  \author Xavier Warin
 */
namespace libstoch
{

static const double zero = 0. ; ///< Zero values
static const int  ulp   =  10;  ///< units in the last place for double comparison
static const double infty = 1.e30;
static const double small = 1e-3;
static const double tiny = 1e-9;

}
#endif /* CONSTANT_H */
