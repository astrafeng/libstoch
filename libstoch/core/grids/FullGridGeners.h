// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef FULLGRIDGENERS_H
#define FULLGRIDGENERS_H
#include "libstoch/core/grids/FullGrid.h"
#include <geners/AbsReaderWriter.hh>
#include <geners/associate_serialization_factory.hh>

/** \file FullGridGeners.h
 * \brief Define non intrusive  serialization with random acces
*  \author Xavier Warin
 */

/// \¢lass
///  I/O factory for classes derived from .
// Note publication of the base class and absence of public constructors.
class SerializationFactoryForFullGrid : public gs::DefaultReaderWriter<libstoch::FullGrid>
{
    typedef DefaultReaderWriter<libstoch::FullGrid> Base;
    friend class gs::StaticReaderWriter<SerializationFactoryForFullGrid>;
    SerializationFactoryForFullGrid();
};

// SerializationFactoryForFullGrid wrapped into a singleton
typedef gs::StaticReaderWriter<SerializationFactoryForFullGrid> StaticSerializationFactoryForFullGrid;

gs_specialize_class_id(libstoch::FullGrid, 1)
gs_declare_type_external(libstoch::FullGrid)
gs_associate_serialization_factory(libstoch::FullGrid, StaticSerializationFactoryForFullGrid)

#endif  /* FULLGRIDGENERS_H */
