#include "libstoch/core/grids/InterpolatorSpectralGeners.h"
// add include for all derived classes
#include "libstoch/core/grids/LinearInterpolatorSpectralGeners.h"
#include "libstoch/core/grids/LegendreInterpolatorSpectralGeners.h"
#include "libstoch/core/grids/SparseInterpolatorSpectralGeners.h"

// Register all wrappers
SerializationFactoryForInterpolatorSpectral::SerializationFactoryForInterpolatorSpectral()
{
    this->registerWrapper<LinearInterpolatorSpectralGeners>();
    this->registerWrapper<LegendreInterpolatorSpectralGeners>();
    this->registerWrapper<SparseInterpolatorSpectralGeners>();
}
