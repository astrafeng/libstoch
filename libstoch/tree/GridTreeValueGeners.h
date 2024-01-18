// Copyright (C) 2019 EDF
// All Rights Reserved
// This code is published under the GNU Lesser General Public License (GNU LGPL)
#ifndef  GRIDTREEVALUEGENERS_H
#define  GRIDTREEVALUEGENERS_H
#include <Eigen/Dense>
#include "geners/GenericIO.hh"
#include "libstoch/tree/GridTreeValue.h"
#include "libstoch/core/grids/SpaceGridGeners.h"
#include "libstoch/core/grids/RegularSpaceGridGeners.h"
#include "libstoch/core/grids/GeneralSpaceGridGeners.h"
#include "libstoch/core/grids/SparseSpaceGridNoBoundGeners.h"
#include "libstoch/core/grids/SparseSpaceGridBoundGeners.h"
#include "libstoch/core/grids/SparseInterpolatorSpectralGeners.h"
#include "libstoch/core/grids/InterpolatorSpectralGeners.h"
#include "libstoch/core/grids/LinearInterpolatorSpectralGeners.h"
#include "libstoch/core/grids/LegendreInterpolatorSpectralGeners.h"


/** \file GridTreeValueGeners.h
 * \brief Define non intrusive serialization with random access
*  \author Xavier Warin
 */

/// specialize the ClassIdSpecialization template
/// so that a ClassId object can be associated with the class we want to
/// serialize.  The second argument is the version number.
///@{
gs_specialize_class_id(libstoch::GridTreeValue, 1)
/// an external class
gs_declare_type_external(libstoch::GridTreeValue)
///@}

namespace gs
{
//
/// \brief  This is how the specialization of GenericWriter should look like
//
template <class Stream, class State >
struct GenericWriter < Stream, State, libstoch::GridTreeValue,
           Int2Type<IOTraits<int>::ISEXTERNAL> >
{
    inline static bool process(const libstoch::GridTreeValue  &p_state, Stream &p_os,
                               State *, const bool p_processClassId)
    {
        // If necessary, serialize the class id
        static const ClassId current(ClassId::makeId<libstoch::GridTreeValue >());
        const bool status = p_processClassId ? ClassId::makeId<libstoch::GridTreeValue >().write(p_os) : true;
        // Serialize object data if the class id was successfully
        // written out
        if (status)
        {
            std::shared_ptr< libstoch::SpaceGrid > ptrGrid = p_state.getGrid();
            bool bSharedPtr = (ptrGrid ? true : false);
            write_pod(p_os, bSharedPtr);
            if (bSharedPtr)
            {
                write_item(p_os, ptrGrid);
                write_item(p_os, p_state.getInterpolators());
            }
        }
        // Return "true" on success, "false" on failure
        return status && !p_os.fail();
    }
};

/// \brief  And this is the specialization of GenericReader
//
template <class Stream, class State  >
struct GenericReader < Stream, State, libstoch::GridTreeValue, Int2Type<IOTraits<int>::ISEXTERNAL> >
{
    inline static bool readIntoPtr(libstoch::GridTreeValue  *&ptr, Stream &p_is,
                                   State *, const bool p_processClassId)
    {

        if (p_processClassId)
        {
            static const ClassId current(ClassId::makeId<libstoch::GridTreeValue>());
            ClassId id(p_is, 1);
            current.ensureSameName(id);
        }

        // Deserialize object data.
        bool bSharedPtr ;
        read_pod(p_is, &bSharedPtr);
        std::shared_ptr<libstoch::SpaceGrid > pgridShared;
        CPP11_auto_ptr<std::vector< std::shared_ptr<libstoch::InterpolatorSpectral> > > pinterp;
        if (bSharedPtr)
        {
            CPP11_auto_ptr<libstoch::SpaceGrid> pgrid = read_item<libstoch::SpaceGrid>(p_is);
            pgridShared = std::move(pgrid);
            pinterp = read_item< std::vector< std::shared_ptr<libstoch::InterpolatorSpectral> > >(p_is);
            /// now affect grids to interpolator
            for (size_t i = 0 ; i < pinterp->size(); ++i)
                (*pinterp)[i]->setGrid(& *pgridShared);
        }

        if (p_is.fail())
            // Return "false" on failure
            return false;
        //Build the object from the stored data
        if (ptr)
        {
            if (bSharedPtr)
                *ptr = libstoch::GridTreeValue(pgridShared,  *pinterp);
            else
                *ptr = libstoch::GridTreeValue();
        }
        else
        {
            if (bSharedPtr)
                ptr = new  libstoch::GridTreeValue(pgridShared,  *pinterp);
            else
                ptr = new  libstoch::GridTreeValue();
        }
        return true;
    }

    inline static bool process(libstoch::GridTreeValue &s, Stream &is,
                               State *st, const bool p_processClassId)
    {
        // Simply convert reading by reference into reading by pointer
        libstoch::GridTreeValue *ps = &s;
        return readIntoPtr(ps, is, st, p_processClassId);
    }
};
}



#endif/*  GRIDANDREGRESSEDVALUEGENERS_H */
