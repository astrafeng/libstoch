// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SDDPACUT_H
#define SDDPACUT_H
#include <Eigen/Dense>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "libstoch/core/utils/stdSharedPtrSerialization.h"
#include "libstoch/core/utils/eigenSerialization.h"

/** \file SDDPACut.h
 * \brief Cut object
 * \author Xavier Warin
 */
namespace libstoch
{

/// \class SDDPACut SDDPACut.h
/// A cut
///  Give a cut
///  The cut is a 2 dimensional array
///    - first the number of coefficients of the cut ( value  and derivatives in all dimensions of the state)
///    - second is the  dimension in the decomposition of the cut on the conditional expectation basis for conditional cuts
///
class SDDPACut
{
private :
    std::shared_ptr<Eigen::ArrayXXd> m_cut ; ///<  the cut  (conditional cut ( dimension of the cut by the number of function basis for conditional expectation))

    friend class boost::serialization::access ;

    /// \brief boost serialization
    /// \param p_ar archive used for serialization
    /// \param p_version
    template< typename Archive>
    void serialize(Archive &p_ar, const unsigned int p_version)
    {
        p_ar &m_cut ;
    }


public :

    /// \brief Default constructor
    SDDPACut() {}

    /// \brief Constructor
    /// \param p_cut  cut
    SDDPACut(const std::shared_ptr<Eigen::ArrayXXd> &p_cut) : m_cut(p_cut) {}

    /// \brief get state size
    inline int getStateSize() const
    {
        return m_cut->rows() - 1 ;
    }

    /// \brief get cut
    std::shared_ptr<Eigen::ArrayXXd> getCut() const
    {
        return m_cut;
    }
};

}

#endif /* SDDPACUT_H */
