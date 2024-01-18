// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef REGULARLEGENDREGRIDGENERS_H
#define REGULARLEGENDREGRIDGENERS_H
#include <geners/AbsReaderWriter.hh>
#include "libstoch/core/grids/RegularLegendreGrid.h"
#include "libstoch/core/grids/SpaceGridGeners.h"

/** \file RegularLegendreGridGeners.h
 * \brief Define non intrusive  serialization with random access
*  \author Xavier Warin
 */

// Concrete reader/writer for class RegularLegendreGrid
// Note publication of RegularLegendreGrid as "wrapped_type".
struct RegularLegendreGridGeners: public gs::AbsReaderWriter<libstoch::SpaceGrid>
{
    typedef libstoch::SpaceGrid wrapped_base;
    typedef libstoch::RegularLegendreGrid wrapped_type;

    // Methods that have to be overridden from the base
    bool write(std::ostream &, const wrapped_base &, bool p_dumpId) const override;
    wrapped_type *read(const gs::ClassId &p_id, std::istream &p_in) const override;

    // The class id for RegularLegendreGrid  will be needed both in the "read" and "write"
    // methods. Because of this, we will just return it from one static
    // function.
    static const gs::ClassId &wrappedClassId();
};

gs_specialize_class_id(libstoch::RegularLegendreGrid, 1)
gs_declare_type_external(libstoch::RegularLegendreGrid)
gs_associate_serialization_factory(libstoch::RegularLegendreGrid, StaticSerializationFactoryForSpaceGrid)

#endif  /* REGULARLEGENDREGRIDGENERS_H */
