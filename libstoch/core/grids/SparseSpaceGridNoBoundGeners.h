// Copyright (C) 2016 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef SPARSESPACEGRIDNOBOUNDGENERS_H
#define SPARSESPACEGRIDNOBOUNDGENERS_H
#include <geners/AbsReaderWriter.hh>
#include "libstoch/core/grids/SparseSpaceGridNoBound.h"
#include "libstoch/core/grids/SpaceGridGeners.h"

/** \file SparseSpaceGridNoBoundGeners.h
 * \brief Define non intrusive  serialization with random access
*  \author Xavier Warin
 */

// Concrete reader/writer for class SparseSpaceGridNoBound
// Note publication of SparseSpaceGridNoBound as "wrapped_type".
struct SparseSpaceGridNoBoundGeners: public gs::AbsReaderWriter<libstoch::SpaceGrid>
{
    typedef libstoch::SpaceGrid wrapped_base;
    typedef libstoch::SparseSpaceGridNoBound wrapped_type;

    // Methods that have to be overridden from the base
    bool write(std::ostream &, const wrapped_base &, bool p_dumpId) const override;
    wrapped_type *read(const gs::ClassId &p_id, std::istream &p_in) const override;

    // The class id for SparseSpaceGridNoBound  will be needed both in the "read" and "write"
    // methods. Because of this, we will just return it from one static
    // function.
    static const gs::ClassId &wrappedClassId();
};

gs_specialize_class_id(libstoch::SparseSpaceGridNoBound, 1)
gs_declare_type_external(libstoch::SparseSpaceGridNoBound)
gs_associate_serialization_factory(libstoch::SparseSpaceGridNoBound, StaticSerializationFactoryForSpaceGrid)

#endif  /* SPARSESPACEGRIDNOBOUNDGENERS_H */
