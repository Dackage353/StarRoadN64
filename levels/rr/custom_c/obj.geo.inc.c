extern Gfx DL_ssl_geo_000630_0x700fce0[];
const GeoLayout geo_star_road_sr_roomba[]= {
    GEO_CULLING_RADIUS(4000),
    GEO_OPEN_NODE(),
    GEO_DISPLAY_LIST(LAYER_OPAQUE,DL_ssl_geo_000630_0x700fce0),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

extern Gfx DL_ssl_geo_000794_0x7022cf8[];
const GeoLayout geo_star_road_sr_rotating[]= {
    GEO_CULLING_RADIUS(700),
    GEO_OPEN_NODE(),
    GEO_DISPLAY_LIST(LAYER_OPAQUE,DL_ssl_geo_000794_0x7022cf8),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

extern Gfx DL_ssl_geo_000764_0x70228a8[];
extern Gfx DL_ssl_geo_000764_0x70229e8[];
const GeoLayout geo_star_road_sr_pushing[]= {
    GEO_CULLING_RADIUS(550),
    GEO_OPEN_NODE(),
    GEO_SHADOW(50,180,0),
    GEO_OPEN_NODE(),
    GEO_DISPLAY_LIST(LAYER_OPAQUE,DL_ssl_geo_000764_0x70228a8),
    GEO_DISPLAY_LIST(LAYER_ALPHA,DL_ssl_geo_000764_0x70229e8),
    GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};

extern Gfx DL_ssl_geo_000734_0x70220a8[];
extern Gfx DL_ssl_geo_000734_0x70221e8[];
const GeoLayout geo_star_road_sr_squishing[]= {
    GEO_CULLING_RADIUS(700),
    GEO_OPEN_NODE(),
    GEO_SHADOW(11,180,550),
    GEO_OPEN_NODE(),
    GEO_DISPLAY_LIST(LAYER_OPAQUE,DL_ssl_geo_000734_0x70220a8),
    GEO_DISPLAY_LIST(LAYER_ALPHA,DL_ssl_geo_000734_0x70221e8),
    GEO_CLOSE_NODE(),
    GEO_CLOSE_NODE(),
    GEO_END(),
};
