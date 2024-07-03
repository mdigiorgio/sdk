// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024

#include <nds.h>

// This is autogenerated from ball.png and ball.grit
#include "ball.h"

// There are 32 affine matrices per engine. You can assign any matrix to any
// sprite, and you can reuse the same matrix for multiple sprites.
//
// An affine matrix allows you to scale and rotate sprites. It's very easy for
// the scaled/rotated sprite to be bigger than the original bounds of the
// sprite. If this happens, the image will be cut. You can solve this to some
// extent by using the "double size" option. This will double the canvas of the
// sprite so that you can display scaled sprites.
//
// Note that the coordinates of the sprite need to be adjusted when using the
// "double size" flag.

int main(int argc, char *argv[])
{
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetPrimaryBanks(VRAM_A_MAIN_SPRITE, VRAM_B_LCD, VRAM_C_LCD,
                        VRAM_D_SUB_SPRITE);

    consoleDemoInit();

    oamInit(&oamMain, SpriteMapping_1D_32, false);
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    // Allocate space for the tiles and copy them there
    u16 *gfxMain = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
    dmaCopy(ballTiles, gfxMain, ballTilesLen);

    u16 *gfxSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
    dmaCopy(ballTiles, gfxSub, ballTilesLen);

    // Copy palette
    dmaCopy(ballPal, SPRITE_PALETTE, ballPalLen);
    dmaCopy(ballPal, SPRITE_PALETTE_SUB, ballPalLen);

    oamSet(&oamMain, 0,
           50, 50, // X, Y
           0, // Priority
           0, // Palette index
           SpriteSize_32x32, SpriteColorFormat_256Color, // Size, format
           gfxMain,  // Graphics offset
           0, // Affine matrix index
           true, // Double size
           false, // Hide
           false, false, // H flip, V flip
           false); // Mosaic

    oamSet(&oamMain, 1,
           150, 50, // X, Y
           0, // Priority
           0, // Palette index
           SpriteSize_32x32, SpriteColorFormat_256Color, // Size, format
           gfxMain,  // Graphics offset
           1, // Affine matrix index
           false, // Double size
           false, // Hide
           false, false, // H flip, V flip
           false); // Mosaic

    oamSet(&oamSub, 0,
           50, 100, // X, Y
           0, // Priority
           0, // Palette index
           SpriteSize_32x32, SpriteColorFormat_256Color, // Size, format
           gfxSub,  // Graphics offset
           0, // Affine matrix index
           true, // Double size
           false, // Hide
           false, false, // H flip, V flip
           false); // Mosaic

    oamSet(&oamSub, 1,
           150, 100, // X, Y
           0, // Priority
           0, // Palette index
           SpriteSize_32x32, SpriteColorFormat_256Color, // Size, format
           gfxSub,  // Graphics offset
           1, // Affine matrix index
           false, // Double size
           false, // Hide
           false, false, // H flip, V flip
           false); // Mosaic

    printf("PAD:   Rotate sprite\n");
    printf("A/B:   Scale sprite\n");
    printf("START: Exit to loader\n");

    int angle = 0;
    int scale = 1 << 8;

    while (1)
    {
        swiWaitForVBlank();

        oamRotateScale(&oamMain, 0, angle, scale, scale);
        oamRotateScale(&oamMain, 1, -angle, scale, scale);
        oamRotateScale(&oamSub, 0, angle, scale, scale);
        oamRotateScale(&oamSub, 1, -angle, scale, scale);

        oamUpdate(&oamMain);
        oamUpdate(&oamSub);

        scanKeys();

        u16 keys_held = keysHeld();

        if (keys_held & KEY_LEFT)
            angle += degreesToAngle(2);
        if (keys_held & KEY_RIGHT)
            angle -= degreesToAngle(2);

        if (keys_held & KEY_A)
            scale += 1 << 3;
        if (keys_held & KEY_B)
            scale -= 1 << 3;

        if (keys_held & KEY_START)
            break;
    }

    oamFreeGfx(&oamMain, gfxMain);
    oamFreeGfx(&oamSub, gfxSub);

    return 0;
}