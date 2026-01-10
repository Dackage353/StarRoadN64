extern Gfx DL_hmc_geo_000530_0x7020188[];
const GeoLayout star_road_boi_rock_geo[]= {
    GEO_CULLING_RADIUS(1000),
    GEO_OPEN_NODE(),
    GEO_DISPLAY_LIST(4,DL_hmc_geo_000530_0x7020188),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

extern Gfx DL_hmc_geo_000570_0x7024048[];
const GeoLayout star_road_boi_platform_geo[]= {
    GEO_CULLING_RADIUS(100),
    GEO_OPEN_NODE(),
    GEO_DISPLAY_LIST(1,DL_hmc_geo_000570_0x7024048),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
