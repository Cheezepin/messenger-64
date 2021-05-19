#include "src/game/envfx_snow.h"

const GeoLayout rope_dart_geo[] = {
	GEO_CULLING_RADIUS(32767),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rope_dart_root_mesh_layer_1),
		GEO_ASM(0, geo_rope_dart_scale),
		GEO_SCALE_BETTER(0x00, 65536, 65536, 65536),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, rope_dart_rope_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, rope_dart_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
