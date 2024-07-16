/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Audiokinetic Wwise generated include file. Do not edit.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WWISE_IDS_H__
#define __WWISE_IDS_H__

#include <AK/SoundEngine/Common/AkTypes.h>

namespace AK
{
    namespace EVENTS
    {
        static const AkUniqueID MODULAR_PLAY = 2974398838U;
        static const AkUniqueID MODULAR_STOP = 3746512648U;
        static const AkUniqueID PERCUSSION_PLAY = 2717275287U;
        static const AkUniqueID PERCUSSION_STOP = 1405717701U;
    } // namespace EVENTS

    namespace STATES
    {
        namespace MODULAR
        {
            static const AkUniqueID GROUP = 2165403525U;

            namespace STATE
            {
                static const AkUniqueID EG_NOW = 3205567180U;
                static const AkUniqueID NONE = 748895195U;
            } // namespace STATE
        } // namespace MODULAR

        namespace PERCUSSION
        {
            static const AkUniqueID GROUP = 3850444438U;

            namespace STATE
            {
                static const AkUniqueID ACTION = 209137191U;
                static const AkUniqueID CHILL = 4294400669U;
                static const AkUniqueID NONE = 748895195U;
                static const AkUniqueID OMINOUS = 1107723959U;
            } // namespace STATE
        } // namespace PERCUSSION

    } // namespace STATES

    namespace GAME_PARAMETERS
    {
        static const AkUniqueID DENSITY_FA_SSGRAIN = 2715217995U;
        static const AkUniqueID IMMERSION_FA_SSGRAIN = 2481728872U;
        static const AkUniqueID INSTRUMENT_FA_SSGRAIN = 2317409760U;
        static const AkUniqueID PERCUSSION_LOWPASS = 659738430U;
        static const AkUniqueID PROXIMITY_FA_SSGRAIN = 1791284502U;
        static const AkUniqueID RPM_FA_SSGRAIN = 1656280998U;
        static const AkUniqueID SIMULATION_FA_SSGRAIN = 2428833394U;
    } // namespace GAME_PARAMETERS

    namespace BANKS
    {
        static const AkUniqueID INIT = 1355168291U;
        static const AkUniqueID MODULAR = 2165403525U;
        static const AkUniqueID PERCUSSION = 3850444438U;
    } // namespace BANKS

    namespace BUSSES
    {
        static const AkUniqueID ENVIRONMENTAL = 1973600711U;
        static const AkUniqueID MASTER_AUDIO_BUS = 3803692087U;
        static const AkUniqueID MODULAR = 2165403525U;
        static const AkUniqueID MUSIC = 3991942870U;
        static const AkUniqueID NON_ENVIRONMENTAL = 2101962433U;
        static const AkUniqueID PERCUSSION = 3850444438U;
    } // namespace BUSSES

    namespace AUDIO_DEVICES
    {
        static const AkUniqueID NO_OUTPUT = 2317455096U;
        static const AkUniqueID SYSTEM = 3859886410U;
    } // namespace AUDIO_DEVICES

}// namespace AK

#endif // __WWISE_IDS_H__
