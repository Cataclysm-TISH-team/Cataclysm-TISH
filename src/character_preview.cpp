#include "character_preview.h"

#if defined(TILES)

#include "avatar.h"
#include "cached_options.h"
#include "cata_tiles.h"
#include "color.h"
#include "cursesport.h"
#include "output.h"
#include "sdltiles.h"
#include "translations.h"

namespace
{
constexpr int min_zoom = 32;
constexpr int max_zoom = 256;
// Smaller than this, the window cannot hold its title and a sprite
constexpr int min_preview_cols = 21;
constexpr int min_preview_lines = 8;

bool tiles_available()
{
    return use_tiles && tilecontext && tilecontext->is_valid();
}

// Size of one terminal cell, in pixels
int termx_to_pixel_value()
{
    return projected_window_width() / TERMX;
}

int termy_to_pixel_value()
{
    return projected_window_height() / TERMY;
}
} // namespace

character_preview_window::~character_preview_window()
{
    restore_scale();
}

void character_preview_window::init( avatar *player, const int nlines_, const int ncols_,
                                     const point &begin_ )
{
    u = player;
    begin = begin_;
    nlines = nlines_;
    ncols = ncols_;
    active = u != nullptr && tiles_available() && ncols >= min_preview_cols &&
             nlines >= min_preview_lines;
    if( !active ) {
        w_preview = catacurses::window();
        restore_scale();
        return;
    }

    if( previous_scale == 0 ) {
        previous_scale = tilecontext->get_draw_scale();
    }
    tilecontext->set_draw_scale( zoom );
    w_preview = catacurses::newwin( nlines, ncols, begin );
    termx_pixels = termx_to_pixel_value();
    termy_pixels = termy_to_pixel_value();
}

point character_preview_window::calc_character_pos() const
{
    // Centers the sprite in the window
    const int t_width = tilecontext->get_tile_width();
    const int t_height = tilecontext->get_tile_height();
    return point( begin.x * termx_pixels + ncols * termx_pixels / 2 - t_width / 2,
                  begin.y * termy_pixels + nlines * termy_pixels / 2 - t_height / 2 );
}

void character_preview_window::zoom_in()
{
    if( !active ) {
        return;
    }
    zoom = zoom * 2 > max_zoom ? min_zoom : zoom * 2;
    tilecontext->set_draw_scale( zoom );
}

void character_preview_window::zoom_out()
{
    if( !active ) {
        return;
    }
    zoom = zoom / 2 < min_zoom ? max_zoom : zoom / 2;
    tilecontext->set_draw_scale( zoom );
}

void character_preview_window::display() const
{
    if( !active ) {
        return;
    }
    werase( w_preview );
    draw_border( w_preview, BORDER_COLOR, _( "CHARACTER PREVIEW" ), BORDER_COLOR );
    wnoutrefresh( w_preview );

    tilecontext->display_character( *u, calc_character_pos() );
}

void character_preview_window::restore_scale()
{
    if( previous_scale > 0 && tiles_available() ) {
        tilecontext->set_draw_scale( previous_scale );
    }
    previous_scale = 0;
}

#else // defined(TILES)

character_preview_window::~character_preview_window() = default;
void character_preview_window::init( avatar *, const int, const int, const point & ) {}
void character_preview_window::zoom_in() {}
void character_preview_window::zoom_out() {}
void character_preview_window::display() const {}

#endif // defined(TILES)
