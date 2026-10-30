#pragma once
#ifndef CATA_SRC_CHARACTER_PREVIEW_H
#define CATA_SRC_CHARACTER_PREVIEW_H

#include "cursesdef.h"
#include "point.h"

class avatar;

/**
 * Window of the character creation screen that shows the sprite of the character being
 * created, drawn the way the tileset draws it in game (so traits that change the look of the
 * character are visible).
 *
 * Only does something in tiles builds: in curses builds every function is empty.
 * The preview is not shown if the window would be too small, or if tiles are turned off.
 */
class character_preview_window
{
    public:
        character_preview_window() = default;
        character_preview_window( const character_preview_window & ) = delete;
        character_preview_window &operator=( const character_preview_window & ) = delete;
        // Puts the zoom of the tileset back as it was before the preview changed it
        ~character_preview_window();

        /** Creates the window; can be called again after the screen was resized. */
        void init( avatar *player, int nlines, int ncols, const point &begin );
        void zoom_in();
        void zoom_out();
        /** Draws the border and the character. Call it after the other windows were refreshed. */
        void display() const;

    private:
        catacurses::window w_preview;
        avatar *u = nullptr;
        // Position and size of the window, in terminal cells
        point begin;
        int nlines = 0;
        int ncols = 0;
        // Size of one terminal cell, in pixels
        int termx_pixels = 0;
        int termy_pixels = 0;
        // Zoom of the preview, in the units of cata_tiles::set_draw_scale (16 is 1x)
        int zoom = 128;
        // Zoom of the tileset before the preview changed it
        int previous_scale = 0;
        bool active = false;

        point calc_character_pos() const;
        void restore_scale();
};

#endif // CATA_SRC_CHARACTER_PREVIEW_H
