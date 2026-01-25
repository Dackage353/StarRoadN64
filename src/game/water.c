#include "water.h"

#include "game/game_init.h"
#include "engine/graph_node.h"
#include "engine/math_util.h"

#include <string.h>

#include "levels/castle_grounds/cg_water/header.h"
#include "levels/bob/boi_water/header.h"

struct WaterConfig
{
    Gfx* dl;
    Vtx* vtxBuffer;
    size_t vtxBufferCount;
};

static struct WaterConfig get_water_config(int param)
{
#define WATER_DECL(adl, avb) \
    (struct WaterConfig){ \
        .dl             = adl, \
        .vtxBuffer      = avb, \
        .vtxBufferCount = sizeof(avb) / sizeof(Vtx), \
    }

    switch (param)
    {
        case WATER_CASTLE_GROUNDS:
            return WATER_DECL(cg_water_Plane_001_mesh, cg_water_Plane_001_mesh_vtx_0);
        case WATER_BOI:
            return WATER_DECL(boi_water_node_001_mesh, boi_water_node_001_mesh_vtx_0);
        default:
            return (struct WaterConfig){0};
    }

#undef WATER_DECL
}

struct CloneResult {
    Gfx* dl;
    Vtx* vtxBuffer;
};

static Vtx* clone_vbuffer(const Vtx* _vtxBuffer, size_t vtxBufferCount)
{
    size_t vtxBufferSize = vtxBufferCount * sizeof(Vtx);
    Vtx* vtxBuffer = main_pool_alloc_aligned(0, vtxBufferSize, 16);
    memcpy(vtxBuffer, segmented_to_virtual(_vtxBuffer), vtxBufferSize);
    return vtxBuffer;
}

static struct CloneResult clone_dl(const Gfx* _dl, const Vtx* _vtxBuffer, size_t vtxBufferCount)
{
    Vtx* vtxBuffer = clone_vbuffer(_vtxBuffer, vtxBufferCount);

    const u8* start = (const u8*)segmented_to_virtual(_dl);
    const u8* dl = start;
    while (G_ENDDL != *dl)
    {
        dl += 8;
    }

    // I need very good alignment guarantees for gfx dls
    void* clonedDl = main_pool_alloc_aligned(0, dl - start + 8, 8);
    memcpy(clonedDl, start, dl - start + 8);

    dl = (const u8*)clonedDl;
    u32 physVtxBuffer = VIRTUAL_TO_PHYSICAL(vtxBuffer);
    while (G_ENDDL != *dl)
    {
        if (G_VTX == *dl)
        {
            u32* pvtxBuf = (u32*)(dl + 4);
            u32 offset = *pvtxBuf - (u32)(_vtxBuffer);
            *pvtxBuf = physVtxBuffer + offset;
        }

        dl += 8;
    }

    return (struct CloneResult){ (Gfx*) clonedDl, vtxBuffer };
}

static void rotate(Vtx* vtxBuffer, const Vtx* origVtxBuffer, size_t vtxBufferCount)
{
    int val = gGlobalTimer * 16;
    f32 s = sins(val);
    f32 c = coss(val);

    for (size_t i = 0; i < vtxBufferCount; i++)
    {
        Vtx* vtx = &vtxBuffer[i];
        const Vtx* origVtx = &origVtxBuffer[i];

        vtx->v.tc[0] = origVtx->v.tc[0] * c - origVtx->v.tc[1] * s;
        vtx->v.tc[1] = origVtx->v.tc[0] * s + origVtx->v.tc[1] * c;
    }
}

// Note that for this to work param must be between 0 and 127. Should plenty.
static u32 pack(void* ptr, u32 param)
{
    u32 val = (u32)ptr;
    return param | (val << 8);
}

static u32 unpack_param(u32 packed)
{
    return packed & 0xFF;
}

static void* unpack_ptr(u32 packed)
{
    return (void*)((packed >> 8) | (0x80000000U));
}

struct WaterContext
{
    Vtx* constVtxBuffer;

    Gfx* dl;
    Vtx* vtxBuffer;
};

Gfx *geo_render_water_dl(s32 callContext, struct GraphNode *node, UNUSED Mat4 mtx)
{
    struct GraphNodeGenerated *genNode = (struct GraphNodeGenerated *) node;
    int param = unpack_param(genNode->parameter);
    struct WaterConfig config = get_water_config(param);

    if (callContext == GEO_CONTEXT_AREA_LOAD) 
    {
        struct WaterContext* ctx = main_pool_alloc(sizeof(*ctx));
        struct CloneResult res = clone_dl(config.dl, config.vtxBuffer, config.vtxBufferCount);
        *ctx = (struct WaterContext){
            .constVtxBuffer = clone_vbuffer(config.vtxBuffer, config.vtxBufferCount),
            .dl             = (Gfx*) VIRTUAL_TO_PHYSICAL(res.dl),
            .vtxBuffer      = res.vtxBuffer,
        };
        genNode->parameter = pack(ctx, unpack_param(genNode->parameter));
        SET_GRAPH_NODE_LAYER(genNode->fnNode.node.flags, LAYER_TRANSPARENT);
    }

    if (callContext == GEO_CONTEXT_RENDER)
    {
        struct WaterContext* ctx = (struct WaterContext*)unpack_ptr(genNode->parameter);

        Vtx* vtxBuffer;
        Gfx* dl;
        if (gGlobalTimer & 1)
        {
            vtxBuffer = ctx->vtxBuffer;
            dl = ctx->dl;
        }
        else
        {
            vtxBuffer = segmented_to_virtual(config.vtxBuffer);
            dl = config.dl;
        }
        rotate(vtxBuffer, ctx->constVtxBuffer, config.vtxBufferCount);
        return dl;
    }
    return NULL;
}
