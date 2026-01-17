extern Gfx DL_dorrie_geo_0x600ce28[];
const GeoLayout star_road_wc_blade[]= {
GEO_CULLING_RADIUS(5000),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_ALPHA, DL_dorrie_geo_0x600ce28),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
