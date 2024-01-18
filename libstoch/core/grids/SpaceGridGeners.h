// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SPACEGRIDGENERS_H
#define SPACEGRIDGENERS_H
#include "libstoch/core/grids/SpaceGrid.h"
#include <geners/AbsReaderWriter.hh>
#include <geners/associate_serialization_factory.hh>

/** \file BaseRegressionGeners.h
 * \brief Base class mapping with geners to archive BaseRegression pointer type
 * \author  Xavier Warin
 */

/// \¢lass
///  I/O factory for classes derived from .
// Note publication of the base class and absence of public constructors.
class SerializationFactoryForSpaceGrid : public gs::DefaultReaderWriter<libstoch::SpaceGrid>
{
    typedef DefaultReaderWriter<libstoch::SpaceGrid> Base;
    friend class gs::StaticReaderWriter<SerializationFactoryForSpaceGrid>;
    SerializationFactoryForSpaceGrid();
};

// SerializationFactoryForSpaceGrid wrapped into a singleton
typedef gs::StaticReaderWriter<SerializationFactoryForSpaceGrid> StaticSerializationFactoryForSpaceGrid;

gs_specialize_class_id(libstoch::SpaceGrid, 1)
gs_declare_type_external(libstoch::SpaceGrid)
gs_associate_serialization_factory(libstoch::SpaceGrid, StaticSerializationFactoryForSpaceGrid)

#endif
