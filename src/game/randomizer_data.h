#pragma once

#include "randomizer_data.h"
#include "randomizer.h"

u8 Randomizer_gRequiredStars[Randomizer_STAR_REQ_MAX];

u8 sRemainingWarpsStatic[23] = {4,5,7,8,9,10,11,12,13,14,15,17,18,19,20,22,23,24,27,28,29,31,36};
u8 sRemainingWarpsTemp[23];
u8 sB1WarpsStatic[9] = { LEVEL_BOB, LEVEL_WF, LEVEL_CCM, LEVEL_JRB, LEVEL_PSS, LEVEL_BITDW, LEVEL_BBH, LEVEL_TOTWC, LEVEL_SA };
u8 sB1WarpsTemp[9];
u8 sB2WarpsStatic[7] = { LEVEL_SSL, LEVEL_LLL, LEVEL_HMC, LEVEL_COTMC, LEVEL_DDD, LEVEL_BITFS, LEVEL_VCUTM };
u8 sB2WarpsTemp[7];
u8 sB3WarpsStatic[7] = { LEVEL_WDW, LEVEL_TTM, LEVEL_SL, LEVEL_THI, LEVEL_RR, LEVEL_TTC, LEVEL_WMOTR };
u8 sB3WarpsTemp[7];
u8 gWarpDestinationsStatic[37] = {0,0,0,0,4,5,0,7,8,9,10,11,12,13,14,15,0,17,18,19,20,0,22,23,24,0,0,27,28,29,0,31,0,0,0,0,36};
u8 gWarpDestinations[37];

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

// Use bhvStub for avoidance points for all objects.
// Use bhv<OBJECT> for points that only those OBJECTs should avoid.
struct Randomizer_AvoidancePoint mmmAvoidancePoints[] = {
    {bhvStub, {573, -2493, -1339}, 1500.0f, 3500.0f, Randomizer_AVOIDANCE_SAFETY_ALL} // Arph bottom
};

struct Randomizer_AvoidancePoint micrAvoidancePoints[] = {
    {bhvStub, {-898, -2910, -4833}, 2000.0f, 2900.0f, Randomizer_AVOIDANCE_SAFETY_ALL} // Snow pile in bottom
};


// {Xmin, Xmax, Ymin, Ymax, Zmin, Zmax, 0, 0, NULL}

// Main courses
struct Randomizer_AreaParams bobParams[] = {
    {-7356, 7474, -3498, 2350, -8000, 6700, 0, 0, NULL}
};
struct Randomizer_AreaParams wfParams[] = {
    {-6000, 8192, -2166, 3564, -7200, 7000, 0, 0, NULL}
};
struct Randomizer_AreaParams jrbParams[] = {
    {-8192, 8192, -2546, 3900, -7600, 8192, 0, 0, NULL}
};
struct Randomizer_AreaParams ccmParams[] = {
    {-7146, 7194, -5650, 5090, -6624, 7637, 0, 0, NULL}
};
struct Randomizer_AreaParams bbhParams[] = {
    {-8192, 7266, -1977, 3000, -7124, 6318, 0, 0, NULL}
};
struct Randomizer_AreaParams hmcParams[] = {
    {-7133, 8192, -7363, 3929, -8192, 8192, 0, 0, NULL}
};
struct Randomizer_AreaParams lllParams[] = {
    {-7603, 6600, -3979, 5183, -7900, 6342, 0, 0, NULL}
};
struct Randomizer_AreaParams sslParams[] = {
    {-5462, 6806, -5250, 3960, -6956, 5105, 0, 0, NULL}
};
struct Randomizer_AreaParams dddParams[] = {
    {-6236, 7014, -2460, 2640, -7920, 5893, 0, ARRAY_SIZE(mmmAvoidancePoints), mmmAvoidancePoints}
};
struct Randomizer_AreaParams slParams[] = {
    {-7284, 6957, -870, 4350, -7280, 5781, 0, ARRAY_SIZE(micrAvoidancePoints), micrAvoidancePoints}
};
struct Randomizer_AreaParams wdwParams[] = {
    {-5515, 7069, -4521, 4524, -6966, 6610, 0, 0, NULL}
};
struct Randomizer_AreaParams ttmParams[] = {
    {-7319, 7456, -1230, 5064, -8182, 7468, 0, 0, NULL}
};
struct Randomizer_AreaParams thiParams[] = {
    {-7839, 7380, -3530, 3104, -6877, 8013, 0, 0, NULL}
};
struct Randomizer_AreaParams ttcParams[] = {
    {-5834, 6167, -1524, 3785, -7578, 5612, 0, 0, NULL}
};
struct Randomizer_AreaParams rrParams[] = {
    {-7883, 7637, -6624, 11416, -7451, 7450, 0, 0, NULL}
};

// Secret courses
struct Randomizer_AreaParams pssParams[] = {
    {-6191, 6539, -2302, 4725, -6828, 6896, 0, 0, NULL}
};
struct Randomizer_AreaParams saParams[] = {
    {-6262, 6626, -6010, 4106, -6933, 5629, 0, 0, NULL}
};
struct Randomizer_AreaParams wmotrParams[] = {
    {-6694, 5039, -1385, 6180, -4827, 6362, 0, 0, NULL}
};
struct Randomizer_AreaParams totwcParams[] = {
    {-6070, 5392, -2348, 7470, -5409, 6683, 0, 0, NULL}
};
struct Randomizer_AreaParams cotmcParams[] = {
    {-6762, 5286, 4400, 10920, -3896, 5550, 0, 0, NULL}
};
struct Randomizer_AreaParams vcutmParams[] = {
    {-6065, 7353, -789, 1821, -3404, 2498, 0, 0, NULL}
};
struct Randomizer_AreaParams bitdwParams[] = {
    {-6693, 6223, -940, 2570, -7165, 6440, 0, 0, NULL}
};
struct Randomizer_AreaParams bitfsParams[] = {
    {-6378, 4236, -815, 1200, -5222, 5037, 0, 0, NULL}
};
struct Randomizer_AreaParams bitsParams[] = {
    {-6007, 7033, -2679, 4463, -6114, 7823, 0, 0, NULL}
};

// Castle areas
struct Randomizer_AreaParams cgParams[] = {
    {-7956, 8192, -1798, 6841, -7460, 7913, 0, 0, NULL}
};
struct Randomizer_AreaParams ccParams[] = {
    {-3878, 8192, -3986, 4380, -3423, 7664, 0, 0, NULL}
};
struct Randomizer_AreaParams icParams[] = {
    {-6292, 4951, -436, 1800, -7893, 8192, 0, 0, NULL}
};

Randomizer_AreaParamsArray *sLevelParams[] = {
    &bbhParams,
    &ccmParams,
    &icParams,
    &hmcParams,
    &sslParams,
    &bobParams,
    &slParams,
    &wdwParams,
    &jrbParams,
    &thiParams,
    &ttcParams,
    &rrParams,
    &cgParams,
    &bitdwParams,
    &vcutmParams,
    &bitfsParams,
    &saParams,
    &bitsParams,
    &lllParams,
    &dddParams,
    &wfParams,
    NULL,
    &ccParams,
    &pssParams,
    &cotmcParams,
    &totwcParams,
    NULL,
    &wmotrParams,
    NULL,
    NULL,
    NULL,
    NULL,
    &ttmParams
};
