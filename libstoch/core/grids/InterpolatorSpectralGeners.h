// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef INTERPOLATORSPECTRALGENERS_H
#define INTERPOLATORSPECTRALGENERS_H
#include "libstoch/core/grids/InterpolatorSpectral.h"
#include <geners/AbsReaderWriter.hh>
#include <geners/associate_serialization_factory.hh>

/** \file InterpolatorSpectralGeners.h
 * \brief Base class mapping with geners to archive Spectral Interpolator
 * \author Xavier Warin
 */

///  I/O factory for classes derived from .
// Note publication of the base class and absence of public constructors.
class SerializationFactoryForInterpolatorSpectral : public gs::DefaultReaderWriter<libstoch::InterpolatorSpectral>
{
    typedef DefaultReaderWriter<libstoch::InterpolatorSpectral> Base;
    friend class gs::StaticReaderWriter<SerializationFactoryForInterpolatorSpectral>;
    SerializationFactoryForInterpolatorSpectral();
};

// SerializationFactoryForInterpolatorSpectral wrapped into a singleton
typedef gs::StaticReaderWriter<SerializationFactoryForInterpolatorSpectral> StaticSerializationFactoryForInterpolatorSpectral;

gs_specialize_class_id(libstoch::InterpolatorSpectral, 1)
gs_declare_type_external(libstoch::InterpolatorSpectral)
gs_associate_serialization_factory(libstoch::InterpolatorSpectral, StaticSerializationFactoryForInterpolatorSpectral)

#endif
