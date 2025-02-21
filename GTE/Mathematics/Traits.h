// Addon to GeometricTools
// Enables compile-time access of Real type for templated shape classes

namespace gte
{
    // Primary template left undefined.
    template <typename T>
    struct RealTraits;

    // This specialization matches any class template of the form:
    // template<int, typename> class SomeShape;
    template <template<int32_t, typename> class ShapeType, int32_t N, typename Real>
    struct RealTraits<ShapeType<N, Real>>
    {
        using type = Real;
    };
}