// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SPARSEINTERPOLATORSPECTRALGENERS_H
#define SPARSEINTERPOLATORSPECTRALGENERS_H
#include <geners/AbsReaderWriter.hh>
#include "libstoch/core/grids/SparseInterpolatorSpectral.h"
#include "libstoch/core/grids/InterpolatorSpectralGeners.h"


/** \file SparseInterpolatorSpectralGeners.h
 * \brief Define non intrusive  serialization with random acces
*  \author Xavier Warin
 */

// Concrete reader/writer for class SparseInterpolatorSpectral
// Note publication of SparseInterpolatorSpectral as "wrapped_type".
struct SparseInterpolatorSpectralGeners: public gs::AbsReaderWriter<libstoch::InterpolatorSpectral>
{
    typedef libstoch::InterpolatorSpectral wrapped_base;
    typedef libstoch::SparseInterpolatorSpectral wrapped_type;

    // Methods that have to be overridden from the base
    bool write(std::ostream &, const wrapped_base &, bool p_dumpId) const override;
    wrapped_type *read(const gs::ClassId &p_id, std::istream &p_in) const override;

    // The class id for SparseInterpolatorSpectral  will be needed both in the "read" and "write"
    // methods. Because of this, we will just return it from one static
    // function.
    static const gs::ClassId &wrappedClassId();
};

gs_specialize_class_id(libstoch::SparseInterpolatorSpectral, 1)
gs_declare_type_external(libstoch::SparseInterpolatorSpectral)
gs_associate_serialization_factory(libstoch::SparseInterpolatorSpectral, StaticSerializationFactoryForInterpolatorSpectral)

#endif
