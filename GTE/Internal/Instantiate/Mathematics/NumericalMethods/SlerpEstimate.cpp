#include <Mathematics/SlerpEstimate.h>

namespace gte
{
    template<> template<> Quaternion<float> SLERP<float>::Estimate<1>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<2>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<3>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<4>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<5>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<6>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<7>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<8>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<9>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<10>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<11>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<12>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<13>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<14>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<15>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::Estimate<16>(float, Quaternion<float> const&, Quaternion<float>const&);

    template<> template<> Quaternion<float> SLERP<float>::EstimateR<1>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<2>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<3>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<4>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<5>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<6>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<7>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<8>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<9>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<10>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<11>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<12>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<13>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<14>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<15>(float, Quaternion<float> const&, Quaternion<float>const&);
    template<> template<> Quaternion<float> SLERP<float>::EstimateR<16>(float, Quaternion<float> const&, Quaternion<float>const&);

    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<1>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<2>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<3>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<4>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<5>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<6>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<7>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<8>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<9>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<10>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<11>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<12>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<13>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<14>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<15>(float, Quaternion<float> const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRP<16>(float, Quaternion<float> const&, Quaternion<float>const&, float);

    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<1>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<2>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<3>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<4>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<5>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<6>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<7>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<8>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<9>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<10>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<11>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<12>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<13>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<14>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<15>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);
    template<> template<> Quaternion<float> SLERP<float>::EstimateRPH<16>(float, Quaternion<float> const&, Quaternion<float>const&, Quaternion<float>const&, float);


    template<> template<> Quaternion<double> SLERP<double>::Estimate<1>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<2>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<3>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<4>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<5>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<6>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<7>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<8>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<9>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<10>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<11>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<12>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<13>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<14>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<15>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::Estimate<16>(double, Quaternion<double> const&, Quaternion<double>const&);

    template<> template<> Quaternion<double> SLERP<double>::EstimateR<1>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<2>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<3>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<4>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<5>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<6>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<7>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<8>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<9>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<10>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<11>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<12>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<13>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<14>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<15>(double, Quaternion<double> const&, Quaternion<double>const&);
    template<> template<> Quaternion<double> SLERP<double>::EstimateR<16>(double, Quaternion<double> const&, Quaternion<double>const&);

    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<1>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<2>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<3>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<4>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<5>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<6>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<7>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<8>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<9>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<10>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<11>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<12>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<13>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<14>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<15>(double, Quaternion<double> const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRP<16>(double, Quaternion<double> const&, Quaternion<double>const&, double);

    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<1>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<2>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<3>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<4>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<5>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<6>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<7>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<8>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<9>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<10>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<11>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<12>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<13>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<14>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<15>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
    template<> template<> Quaternion<double> SLERP<double>::EstimateRPH<16>(double, Quaternion<double> const&, Quaternion<double>const&, Quaternion<double>const&, double);
}