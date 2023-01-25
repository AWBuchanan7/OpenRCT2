/*****************************************************************************
 * Copyright (c) 2014-2023 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../common.h"
#include "../world/Location.hpp"
#include "../world/ScenerySelection.h"
#include "TileElement.h"

#include <limits>
#include <string_view>

#define SCENERY_WITHER_AGE_THRESHOLD_1 0x28
#define SCENERY_WITHER_AGE_THRESHOLD_2 0x37

struct LargeSceneryText;

#pragma pack(push, 1)

struct SceneryEntryBase
{
    StringId name;
    uint32_t image;
};

struct LargeSceneryTile
{
    int16_t x_offset;
    int16_t y_offset;
    int16_t z_offset;
    uint8_t z_clearance;
    // CCCC WWWW 0SS0 0000
    uint16_t flags;
};

enum
{
    LARGE_SCENERY_TILE_FLAG_NO_SUPPORTS = 0x20,
    LARGE_SCENERY_TILE_FLAG_ALLOW_SUPPORTS_ABOVE = 0x40,
};

struct LargeSceneryTextGlyph
{
    uint8_t image_offset;
    uint8_t width;
    uint8_t height;
    uint8_t Pad3;
};

// TODO: Remove not required
struct RCTLargeSceneryText
{
    struct
    {
        int16_t x, y;
    } offset[2];                       // 0x0
    uint16_t max_width;                // 0x8
    uint16_t PadA;                     // 0xA
    uint8_t flags;                     // 0xC
    uint8_t num_images;                // 0xD
    LargeSceneryTextGlyph glyphs[256]; // 0xE
};

enum LARGE_SCENERY_TEXT_FLAGS
{
    LARGE_SCENERY_TEXT_FLAG_VERTICAL = (1 << 0), // 0x1
    LARGE_SCENERY_TEXT_FLAG_TWO_LINE = (1 << 1), // 0x2
};

struct LargeSceneryEntry : SceneryEntryBase
{
    CursorID tool_id;
    uint16_t flags;
    money32 price;
    money32 removal_price;
    LargeSceneryTile* tiles;
    ObjectEntryIndex scenery_tab_id;
    uint8_t scrolling_mode;
    LargeSceneryText* text;
    uint32_t text_image;
};

enum LARGE_SCENERY_FLAGS
{
    LARGE_SCENERY_FLAG_HAS_PRIMARY_COLOUR = (1 << 0),          // 0x1
    LARGE_SCENERY_FLAG_HAS_SECONDARY_COLOUR = (1 << 1),        // 0x2
    LARGE_SCENERY_FLAG_3D_TEXT = (1 << 2),                     // 0x4
    LARGE_SCENERY_FLAG_ANIMATED = (1 << 3),                    // 0x8
    LARGE_SCENERY_FLAG_PHOTOGENIC = (1 << 4),                  // 0x10
    LARGE_SCENERY_FLAG_IS_TREE = (1 << 5),                     // 0x20
    LARGE_SCENERY_FLAG_HAS_TERTIARY_COLOUR = (1 << 6),         // 0x40
    LARGE_SCENERY_FLAG_HIDE_PRIMARY_REMAP_BUTTON = (1 << 7),   // 0x80
    LARGE_SCENERY_FLAG_HIDE_SECONDARY_REMAP_BUTTON = (1 << 8), // 0x100
};

#pragma pack(pop)

struct LargeSceneryText
{
    CoordsXY offset[2];
    uint16_t max_width;
    uint8_t flags;
    uint16_t num_images;
    LargeSceneryTextGlyph glyphs[256];

    LargeSceneryText() = default;
    explicit LargeSceneryText(const RCTLargeSceneryText& original);
    const LargeSceneryTextGlyph* GetGlyph(char32_t codepoint) const;
    const LargeSceneryTextGlyph& GetGlyph(char32_t codepoint, char32_t defaultCodepoint) const;
    int32_t MeasureWidth(std::string_view text) const;
    int32_t MeasureHeight(std::string_view text) const;
};

struct SceneryGroupEntry
{
    StringId name;
    uint32_t image;
    std::vector<ScenerySelection> SceneryEntries;
    uint8_t priority;
    uint32_t entertainer_costumes;
};

enum
{
    SCENERY_TYPE_SMALL,
    SCENERY_TYPE_PATH_ITEM,
    SCENERY_TYPE_WALL,
    SCENERY_TYPE_LARGE,
    SCENERY_TYPE_BANNER,

    SCENERY_TYPE_COUNT,
};

enum
{
    SCENERY_GHOST_FLAG_0 = (1 << SCENERY_TYPE_SMALL),
    SCENERY_GHOST_FLAG_1 = (1 << SCENERY_TYPE_PATH_ITEM),
    SCENERY_GHOST_FLAG_2 = (1 << SCENERY_TYPE_WALL),
    SCENERY_GHOST_FLAG_3 = (1 << SCENERY_TYPE_LARGE),
    SCENERY_GHOST_FLAG_4 = (1 << SCENERY_TYPE_BANNER)
};

enum class ScatterToolDensity : uint8_t
{
    LowDensity,
    MediumDensity,
    HighDensity
};

extern uint8_t gSceneryQuadrant;

extern money32 gSceneryPlaceCost;
extern ScenerySelection gSceneryPlaceObject;
extern uint16_t gSceneryPlaceObjectEntryIndex;
extern int16_t gSceneryPlaceZ;
extern uint8_t gSceneryPlaceRotation;

extern uint8_t gSceneryGhostType;
extern CoordsXYZ gSceneryGhostPosition;
extern uint8_t gSceneryGhostWallRotation;

extern int16_t gSceneryShiftPressed;
extern int16_t gSceneryShiftPressX;
extern int16_t gSceneryShiftPressY;
extern int16_t gSceneryShiftPressZOffset;

extern int16_t gSceneryCtrlPressed;
extern int16_t gSceneryCtrlPressZ;

extern const CoordsXY SceneryQuadrantOffsets[];

extern money64 gClearSceneryCost;

void SceneryInit();
void SceneryUpdateTile(const CoordsXY& sceneryPos);
void ScenerySetDefaultPlacementConfiguration();
void SceneryRemoveGhostToolPlacement();

struct WallSceneryEntry;

WallSceneryEntry* GetWallEntry(ObjectEntryIndex entryIndex);
BannerSceneryEntry* GetBannerEntry(ObjectEntryIndex entryIndex);
PathBitEntry* GetFootpathItemEntry(ObjectEntryIndex entryIndex);
SceneryGroupEntry* GetSceneryGroupEntry(ObjectEntryIndex entryIndex);

int32_t WallEntryGetDoorSound(const WallSceneryEntry* wallEntry);

bool IsSceneryAvailableToBuild(const ScenerySelection& item);

bool IsSceneryItemRestricted(const ScenerySelection& item);
void ClearRestrictedScenery();
void RestrictAllMiscScenery();
void MarkAllUnrestrictedSceneryAsInvented();
std::vector<ScenerySelection>& GetRestrictedScenery();

ObjectType GetObjectTypeFromSceneryType(uint8_t type);
uint8_t GetSceneryTypeFromObjectType(ObjectType type);
