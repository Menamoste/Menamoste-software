#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <SDL2/SDL.h>

// ============================================================================
// COLORS (for SDL_SetRenderDrawColor usage)
// ============================================================================

#define TOOLBAR_BG_R              40
#define TOOLBAR_BG_G              42
#define TOOLBAR_BG_B              48
#define TOOLBAR_BG_A              255

#define SEPARATOR_R               70
#define SEPARATOR_G               73
#define SEPARATOR_B               82
#define SEPARATOR_A               255

#define RIBBON_BG_R               50
#define RIBBON_BG_G               52
#define RIBBON_BG_B               58
#define RIBBON_BG_A               255

#define TABS_AREA_BG_R            45
#define TABS_AREA_BG_G            47
#define TABS_AREA_BG_B            53
#define TABS_AREA_BG_A            255

#define TAB_BG_R                  55
#define TAB_BG_G                  57
#define TAB_BG_B                  63
#define TAB_BG_A                  255

#define CANVAS_BG_R               60
#define CANVAS_BG_G               62
#define CANVAS_BG_B               68
#define CANVAS_BG_A               255

#define ACTIVE_TOOL_R             255
#define ACTIVE_TOOL_G             195
#define ACTIVE_TOOL_B             50
#define ACTIVE_TOOL_A             255

#define TEXT_COLOR_R              200
#define TEXT_COLOR_G              200
#define TEXT_COLOR_B              200
#define TEXT_COLOR_A              255

#define TEXT_COLOR_LIGHT_R        180
#define TEXT_COLOR_LIGHT_G        180
#define TEXT_COLOR_LIGHT_B        180
#define TEXT_COLOR_LIGHT_A        255

#define COLOR_SELECT_R            255
#define COLOR_SELECT_G            255
#define COLOR_SELECT_B            255
#define COLOR_SELECT_A            255

// ============================================================================
// TOOLBAR DIMENSIONS
// ============================================================================

#define TOOLBAR_HEIGHT            330
#define TOOLBAR_Y_START           0
#define TOOLBAR_SEPARATOR_Y_TOP   279
#define TOOLBAR_SEPARATOR_Y_BOTTOM 280

// ============================================================================
// TOOL BUTTONS
// ============================================================================

#define TOOL_ICON_SIZE            96
#define TOOL_COUNT                8
#define TOOL_BUTTON_Y             167
#define TOOL_BUTTON_START_X       120
#define TOOL_BUTTON_SPACING       180

// Tool label positioning
#define TOOL_LABEL_OFFSET_Y       15
#define TOOL_LABEL_FONT_SIZE      14

// ============================================================================
// COLOR PALETTE
// ============================================================================

#define COLOR_PALETTE_X           (1920 - 225)
#define COLOR_PALETTE_GRID_ROWS   3
#define COLOR_PALETTE_GRID_COLS   3
#define COLOR_SWATCH_SIZE         60
#define COLOR_SWATCH_SPACING      70
#define COLOR_COUNT               9

// ============================================================================
// FILENAME WIDGET (IMAGE TABS)
// ============================================================================

#define FILENAME_WIDGET_Y_START   25
#define FILENAME_WIDGET_HEIGHT    85
#define FILENAME_WIDGET_WIDTH     250
#define FILENAME_WIDGET_X         10
#define FILENAME_WIDGET_Y_OFFSET  37
#define FILENAME_WIDGET_Y_TEXT    55
#define FILENAME_WIDGET_FONT_SIZE 22

// ============================================================================
// RIBBON (TOP BAR)
// ============================================================================

#define RIBBON_Y_START            0
#define RIBBON_HEIGHT             20
#define RIBBON_FONT_SIZE          16
#define RIBBON_TEXT_X             10
#define RIBBON_TEXT_Y             7

// ============================================================================
// SEPARATOR (Between tool groups)
// ============================================================================

#define SEPARATOR_Y_TOP           140
#define SEPARATOR_Y_BOTTOM        320
// Separator X calculated as: 120 + 180 * 3 + 96 + (180 - 96) / 2

// ============================================================================
// CANVAS & IMAGE
// ============================================================================

#define CANVAS_Y_START            330
#define CANVAS_BORDER_DISTANCE    4
#define CANVAS_BORDER_WIDTH       2

// ============================================================================
// STATUS BAR
// ============================================================================

#define STATUS_BAR_HEIGHT         35
#define STATUS_BAR_FONT_SIZE      18
#define STATUS_BAR_TEXT_X_LEFT    15
#define STATUS_BAR_ZOOM_X         (1920 - 70)
#define STATUS_BAR_SEPARATOR_Y_TOP (1080 - 36)
#define STATUS_BAR_SEPARATOR_Y_BOT (1080 - 35)

// ============================================================================
// BORDERS & OUTLINES
// ============================================================================

#define BUTTON_BORDER_DISTANCE    3
#define BUTTON_BORDER_WIDTH       1
#define ACTIVE_BORDER_WIDTH       2

#endif
