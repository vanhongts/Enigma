/*
 * Copyright (C) 2002,2003,2004,2005 Daniel Heck
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "ecl_video.hh"
#include "ecl_error.hh"
#include "ecl_sdl.hh"

#include "SDL_image.h"
#include "SDL_syswm.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"
#include "IMG_SavePNG.h"

#include <cassert>
#include <memory>
#include <cstdio>

using namespace std;
using namespace ecl;

Texture ecl::dummyTexture = {0, 0, 0, false};

void ecl::CreateTexture(SDL_Surface *s, Texture *tex) {
    bool alpha = false;
    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
        
    GLenum err = glGetError();
    if (err != 0)
        fprintf(stderr, "Could not create texture.\n");
        
    if (s->format->BitsPerPixel == 32 &&
            s->format->Bshift == 0 && s->format->Gshift == 8 &&
            s->format->Rshift == 16 && s->format->Ashift == 24) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, 
                GL_BGRA, GL_UNSIGNED_BYTE, s->pixels);
        alpha = (s->flags & SDL_SRCALPHA) != 0;
    } else {
        SDL_PixelFormat fmt;
        fmt.palette = NULL;
        fmt.BitsPerPixel = 32;
        fmt.BytesPerPixel = 4;
        fmt.Rloss = fmt.Gloss = fmt.Bloss = fmt.Aloss = 0;
        fmt.Rmask = 0xff; fmt.Gmask = 0xff00; fmt.Bmask = 0xff0000; fmt.Amask = 0xff000000;
        fmt.Rshift = 0; fmt.Gshift = 8; fmt.Bshift = 16; fmt.Ashift = 24;
        fmt.colorkey = 0;
        fmt.alpha = 0;

        SDL_Surface *tmp = SDL_ConvertSurface(s, &fmt, SDL_SWSURFACE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, 
                GL_RGBA, GL_UNSIGNED_BYTE, tmp->pixels);
        alpha = (tmp->flags & SDL_SRCALPHA) != 0;
        SDL_FreeSurface(tmp);
    }
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    tex->id = texid;
    tex->width = s->w;
    tex->height = s->h;
    tex->alpha = alpha;
}

void ecl::blit(const Texture &tex, int x, int y) {
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    if (tex.alpha) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    } else
        glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3f(x, y, 0);
    glTexCoord2f(1,0); glVertex3f(x + tex.width, y, 0);
    glTexCoord2f(1,1); glVertex3f(x + tex.width, y + tex.height, 0);
    glTexCoord2f(0,1); glVertex3f(x, y + tex.height, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void ecl::blit(const Texture &tex, int x, int y, const Rect &srcRect) {
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    if (tex.alpha) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    } else
        glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    float xf = 1.f / tex.width, yf = 1.f / tex.height;
    float x0 = srcRect.x * xf, x1 = (srcRect.x + srcRect.w) * xf;
    float y0 = srcRect.y * yf, y1 = (srcRect.y + srcRect.h) * yf;
    glBegin(GL_QUADS);
    glTexCoord2f(x0, y0); glVertex3f(x, y, 0);
    glTexCoord2f(x1,y0); glVertex3f(x + srcRect.w, y, 0);
    glTexCoord2f(x1,y1); glVertex3f(x + srcRect.w, y + srcRect.h, 0);
    glTexCoord2f(x0,y1); glVertex3f(x, y + srcRect.h, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void ecl::drawBox(const Rect &r) {
    glBegin(GL_QUADS);
    glVertex3f(r.x, r.y, 0);
    glVertex3f(r.x + r.w, r.y, 0);
    glVertex3f(r.x + r.w, r.y + r.h, 0);
    glVertex3f(r.x, r.y + r.h, 0);
    glEnd();
}


/* -------------------- Graphics primitives -------------------- */

void ecl::line (const GC &gc, int x1, int y1, int x2, int y2) {
    gc.drawable->line (gc, x1, y1, x2, y2);
}


/* -------------------- Clipping helper routines -------------------- */
namespace
{
    inline bool NOCLIP(const GS &gs) {
        return gs.flags & GS_NOCLIP;
    }

    bool clip_hline (const GS &gs, int &x, int &y, int &w) {
        const Rect &cliprect = gs.cliprect;

        if (y < cliprect.y || y >= cliprect.y + cliprect.h)
            return false;

        int d = x - cliprect.x;
        if (d < 0) {
            w += d;
            x = cliprect.x;
        }
        d = (cliprect.x+cliprect.w) - (x+w);
        if (d < 0)
            w += d;
        return w > 0;
    }

    bool clip_vline (const GS &gs, int &x, int &y, int &h) {
        const Rect &cliprect = gs.cliprect;

        if (x < cliprect.x || x >= cliprect.x + cliprect.w)
            return false;

        int d = y - cliprect.y;
        if (d < 0) {
            h += d;
            y = cliprect.y;
        }
        d = cliprect.y+cliprect.h - (y+h);
        if (d < 0)
            h += d;
        return h > 0;
    }

    bool clip_blit (Rect cliprect, int &x, int &y, Rect &r) {
        cliprect.x += r.x-x;
        cliprect.y += r.y-y;
        cliprect.intersect (r);

        if (cliprect.w > 0 && cliprect.h > 0) {
            x += cliprect.x-r.x;
            y += cliprect.y-r.y;
            r = cliprect;
            return true;
        }
        return false;
    }

    bool clip_rect (const GS &gs, Rect &r) {
        r.intersect (gs.cliprect);
        return r.w > 0 && r.h > 0;
    }

    inline bool clip_pixel (const GS &gs, int x, int y) {
        return gs.cliprect.contains(x,y);
    }
}


/* -------------------- Drawable implementation -------------------- */

/* `Xlib.h' also defines a type named `Drawable' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

void ecl::Drawable::set_pixels (const GS &gs, int n, const int* x, const int* y)
{
    const int *xp = x, *yp = y;
    for (int i=n; i; --i)
	set_pixel(gs, *xp++, *yp++);
}

void ecl::Drawable::hline (const GS &gs, int x, int y, int w)
{
    for (int i=w; i; --i)
	set_pixel (gs, x++, y);
}

void ecl::Drawable::vline (const GS &gs, int x, int y, int h)
{
    for (int i=h; i; --i)
	set_pixel (gs, x, y++);
}

void ecl::Drawable::box (const GS &gs, int x, int y, int w, int h)
{
    for (int i=h; i; --i)
	hline (gs, x, y--, w);
}

void ecl::Drawable::line(const GS &, int /*x1*/, int /*y1*/, int /*x2*/, int /*y2*/)
{
}


namespace
{

/* ---------- Generic Surface implementation ---------- */
    template <class PIXELT>
    class TSurface : virtual public Surface {
    public:
        TSurface (SDL_Surface *s=0) : Surface(s) {}

        PIXELT *pixel_pointer(int x, int y) {
            return static_cast<PIXELT*>(Surface::pixel_pointer(x,y));
        }

        /* ---------- Drawable interface ---------- */

        PackedColor get_pixel (int x, int y) {
            return *pixel_pointer(x,y);
        }

        void set_pixel (const GS &gs, int x, int y) {
            if (NOCLIP(gs) || clip_pixel(gs, x, y)) {
                *pixel_pointer(x, y) = gs.pcolor;
            }
        }

        void set_pixels(const GS &gs, int n, const int* xlist, const int* ylist, Uint32 color) {
            const int *xp = xlist, *yp = ylist;
            if (NOCLIP(gs)) {
                for (int i=n; i > 0; --i) {
                    int x = *xp++, y = *yp++;
                    *pixel_pointer(x,y) = gs.pcolor;
                }
            } else {
                for (int i=n; i > 0; --i) {
                    int x = *xp++, y = *yp++;
                    if (clip_pixel (gs, x, y))
                        *pixel_pointer(x,y) = gs.pcolor;
                }
            }
        }

        void hline (const GS &gs, int x, int y, int w) {
            if (NOCLIP(gs) || clip_hline(gs, x, y, w)) {
                PIXELT* dst = pixel_pointer(x, y);
                for (; w > 0; --w)
                    *dst++ = gs.pcolor;
            }
        }

        void vline (const GS &gs, int x, int y, int h) {
            if (NOCLIP(gs) || clip_vline(gs, x, y, h)) {
                PIXELT* dst = pixel_pointer(x, y);
                int offset = pitch() / bypp();
                for (; h > 0; --h)
                {
                    *dst = gs.pcolor;
                    dst += offset;
                }
            }
        }

    };

    typedef TSurface<Uint8> Surface8;
    typedef TSurface<Uint16> Surface16;
    typedef TSurface<Uint32> Surface32;

/* ---------- Surface24 ---------- */

    class Surface24 : virtual public Surface {
    public:
        Surface24 (SDL_Surface *s=0) : Surface(s) {}

        void set_pixel (const GS &gs, int x, int y) {
            if (NOCLIP(gs) || clip_pixel(gs, x, y)) {
                Uint8* p = static_cast<Uint8*> (pixel_pointer(x, y));
                SDL_GetRGB(gs.pcolor, m_surface->format, p,p+1,p+2);
            }
        }

        Uint32 get_pixel (int x, int y) {
//             if (NOCLIP(gs) || clip_pixel(gs, x, y)) {
            Uint8* p = static_cast<Uint8*> (pixel_pointer(x, y));
            return SDL_MapRGB(m_surface->format, p[0],p[1],p[2]);
//             }
        }
    };
}


/* -------------------- Surface -------------------- */

Surface::Surface (SDL_Surface* sfc)
: m_surface(sfc)
{
}

Surface::~Surface() {
    SDL_FreeSurface(m_surface);
}

void Surface::lock() {
    if (SDL_MUSTLOCK (m_surface))
        SDL_LockSurface (m_surface);
}

void Surface::unlock() {
    if (SDL_MUSTLOCK (m_surface))
        SDL_UnlockSurface (m_surface);
}

PackedColor
Surface::map_color(int r, int g, int b)
{
    return SDL_MapRGB(m_surface->format, r, g, b);
}

PackedColor
Surface::map_color(int r, int g, int b, int a)
{
    return SDL_MapRGBA(m_surface->format, r, g, b, a);
}


void Surface::box (const GS &gs, int x, int y, int w, int h) {
    Rect r (x, y, w, h);
    if (NOCLIP(gs) || clip_rect (gs, r)) {
        SDL_Rect dr;
        sdl::copy_rect (dr, r);
        SDL_FillRect (m_surface, &dr, gs.pcolor);
    }
}

void Surface::line (const GS &gs, int x1, int y1, int x2, int y2) {
    SDL_Rect s;
    sdl::copy_rect(s, gs.cliprect);
    SDL_SetClipRect (m_surface, &s);

    Uint8 r, g, b, a;
    SDL_GetRGBA (gs.pcolor, m_surface->format, &r, &g, &b, &a);

    if (has_flags(gs.flags, GS_ANTIALIAS))
        aalineRGBA (m_surface, x1, y1, x2, y2, r, g, b, a);
    else
        lineRGBA (m_surface, x1, y1, x2, y2, r, g, b, a);

    SDL_SetClipRect (m_surface, 0);
}

void Surface::blit (const GS &gs, int x, int y, const Surface* s, const Rect &r_) {
    Rect r(r_);
    if (NOCLIP(gs) || clip_blit (gs.cliprect, x, y, r)) {
        SDL_Rect r1;
        SDL_Rect r2;
        sdl::copy_rect (r1, r);
        r2.x = x; r2.y = y;
        SDL_BlitSurface (s->m_surface, &r1, m_surface, &r2);
    }
}

void Surface::blit (const GS &gs, int x, int y, const Surface* src) {
    assert (src != 0);
    blit (gs, x, y, src, src->size());
}

void Surface::set_color_key (int r, int g, int b) {
    Uint32 color = map_color(r,g,b);
    SDL_SetColorKey(get_surface(), SDL_SRCCOLORKEY | SDL_RLEACCEL, color);
}

void Surface::set_alpha(int a) {
    SDL_SetAlpha(get_surface(), SDL_SRCALPHA, a);
}

Surface * Surface::zoom(int w, int h) 
{
    SDL_Surface *s_new;
    s_new = zoomSurface( get_surface(), (double)w/width(), (double)h/height(), true);
    return Surface::make_surface (s_new);
}

Surface *
Surface::make_surface (SDL_Surface *sdls)
{
    if (sdls == 0) return 0;
    switch (sdls->format->BitsPerPixel) {
    case 8:  return new Surface8 (sdls); break;
    case 16: return new Surface16(sdls); break;
    case 32: return new Surface32(sdls); break;
    case 24: return new Surface24(sdls); break;
    default:
        fprintf(stderr, "Invalid bit depth in surface.\n");
        return 0; // throw XVideo("Invalid bit depth in surface.");
    }
}


/* -------------------- Screen -------------------- */

/* `Xlib.h' also defines a type named `Screen' so we have to specify
   the namespace explicitly and cannot simply use a using-declaration. */

// ecl::Screen::Screen (Surface *s)
// : m_surface(s), m_sdlsurface(s->get_surface())
// {
// }

// ecl::Screen::Screen (SDL_Surface *s)
// : m_surface (Surface::make_surface(s)), m_sdlsurface(s)
// {
// }

// Rect ecl::Screen::size() const {
//     return Rect(0, 0, width(), height());
// }

// int ecl::Screen::width() const {
//     return m_sdlsurface->w;
// }

// int ecl::Screen::height() const {
//     return m_sdlsurface->h;
// }


/* -------------------- Functions -------------------- */

/* Convert the surface to the native surface format.  A pointer to a
   new surface is returned; the old one must be deleted by hand if it
   is no longer needed.  */
Surface*
ecl::DisplayFormat(Surface* s)
{
    if (SDL_Surface* s2 = SDL_DisplayFormat(s->get_surface()))
        return Surface::make_surface(s2);
    return 0;
}


Surface *
ecl::Duplicate(const Surface *s)
{
    if (s==0) return 0;
    SDL_Surface *sdls = s->get_surface();
    SDL_Surface *copy = SDL_ConvertSurface(sdls, sdls->format, sdls->flags);

    if (sdls->format->palette != 0)
        SDL_SetColors(copy, sdls->format->palette->colors, 0,
                      sdls->format->palette->ncolors);

    return Surface::make_surface(copy);
}

void ecl::SavePNG (const ecl::Surface *s, const std::string &filename)
{
    IMG_SavePNG(s->get_surface(), filename.c_str());
}


void ecl::TintRect(Surface *s, Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    std::auto_ptr<Surface> copy(Grab(s, rect));
    if (copy.get()) {
        copy->set_alpha(a);

        GC gc(s);
        set_color(gc, r,g,b);
// TODO: OPENGL
//        box(gc, rect);
        blit(gc, rect.x, rect.y, copy.get());
    }
}


/*
 * (SDL_ConvertSurface with a few changes)
 */

static SDL_Surface *CropSurface (SDL_Surface *surface,
                                 SDL_Rect rect,
                                 SDL_PixelFormat *format, 
                                 Uint32 flags)
{
    SDL_Surface *convert;
    Uint32 colorkey = 0;
    Uint8 alpha = 0;
    Uint32 surface_flags;
    SDL_Rect bounds;

    /* Check for empty destination palette! (results in empty image) */
    if ( format->palette != NULL ) {
        int i;
        for ( i=0; i<format->palette->ncolors; ++i ) {
            if ( (format->palette->colors[i].r != 0) ||
                 (format->palette->colors[i].g != 0) ||
                 (format->palette->colors[i].b != 0) )
                break;
        }
        if ( i == format->palette->ncolors ) {
            SDL_SetError("Empty destination palette");
            return(NULL);
        }
    }

    /* Create a new surface with the desired format */
    convert = SDL_CreateRGBSurface(flags,
                                   rect.w, rect.h, format->BitsPerPixel,
                                   format->Rmask, format->Gmask, format->Bmask, format->Amask);
    if ( convert == NULL ) {
        return(NULL);
    }

    /* Copy the palette if any */
    if ( format->palette && convert->format->palette ) {
        memcpy(convert->format->palette->colors,
               format->palette->colors,
               format->palette->ncolors*sizeof(SDL_Color));
        convert->format->palette->ncolors = format->palette->ncolors;
    }

    /* Save the original surface color key and alpha */
    surface_flags = surface->flags;
    if ( (surface_flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY ) {
        /* Convert colourkeyed surfaces to RGBA if requested */
        if((flags & SDL_SRCCOLORKEY) != SDL_SRCCOLORKEY
           && format->Amask) {
            surface_flags &= ~SDL_SRCCOLORKEY;
        } else {
            colorkey = surface->format->colorkey;
            SDL_SetColorKey(surface, 0, 0);
        }
    }
    if ( (surface_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
        alpha = surface->format->alpha;
        SDL_SetAlpha(surface, 0, 0);
    }

    /* Copy over the image data */
    bounds.x = 0;
    bounds.y = 0;
    bounds.w = rect.w;
    bounds.h = rect.h;
    SDL_LowerBlit(surface, &rect, convert, &bounds);

    /* Clean up the original surface, and update converted surface */
//     if ( convert != NULL ) {
//         SDL_SetClipRect(convert, &surface->clip_rect);
//     }

    if ( (surface_flags & SDL_SRCCOLORKEY) == SDL_SRCCOLORKEY ) {
        Uint32 cflags = surface_flags&(SDL_SRCCOLORKEY|SDL_RLEACCELOK);
        if ( convert != NULL ) {
            Uint8 keyR, keyG, keyB;

            SDL_GetRGB(colorkey,surface->format,&keyR,&keyG,&keyB);
            SDL_SetColorKey(convert, cflags|(flags&SDL_RLEACCELOK),
                            SDL_MapRGB(convert->format, keyR, keyG, keyB));
        }
        SDL_SetColorKey(surface, cflags, colorkey);
    }
    if ( (surface_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
        Uint32 aflags = surface_flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
        if ( convert != NULL ) {
            SDL_SetAlpha(convert, aflags|(flags&SDL_RLEACCELOK),
                         alpha);
        }
        SDL_SetAlpha(surface, aflags, alpha);
    }

    /* We're ready to go! */
    return(convert);
}

Surface * ecl::Grab (const Surface *s, Rect &r) {
    if (s==0)
        return 0;
    SDL_Surface *sdls = s->get_surface();

    int x = 0;
    int y = 0;

    clip_blit (s->size(), x, y, r);

    SDL_Rect rect;
    sdl::copy_rect (rect, r);
    SDL_Surface *copy = CropSurface (sdls, rect, sdls->format, sdls->flags);
    return Surface::make_surface (copy);
}

// LoadImage is already defined as a macro in some Windows header file
#undef LoadImage


Surface* ecl::LoadImage (const char* filename)
{
    SDL_Surface *s = IMG_Load(filename);
    return s ? Surface::make_surface(s) : NULL;
}

Surface* ecl::LoadImage(SDL_RWops *src, int freesrc) {
    SDL_Surface *s = IMG_Load_RW(src, freesrc);
    return s ? Surface::make_surface(s) : NULL;
}

Surface * ecl::MakeSurface(int w, int h, int bipp, const RGBA_Mask &mask)
{
    SDL_Surface* sfc;
    sfc = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, bipp,
                               mask.r, mask.g, mask.g, mask.a);
    if (sfc == 0)
        return 0; //throw XVideo(string("Couldn't create surface: ") + SDL_GetError());
    return Surface::make_surface (sfc);
}


/* Create a surface from image data that is already somewhere in
   memory.  */
Surface * ecl::MakeSurface(void *data, int w, int h, int bipp, int pitch,
                          const RGBA_Mask &mask)
{
    SDL_Surface* sfc;
    sfc = SDL_CreateRGBSurfaceFrom(data, w, h, bipp,
                                   pitch, mask.r, mask.g, mask.b, mask.a);
    if (sfc == 0)
        return 0; //throw XVideo(string("Couldn't create surface:") + SDL_GetError());
    return Surface::make_surface (sfc);
}

Surface *ecl::MakeSurfaceLike (int w, int h, Surface *surface) 
{
    if (surface == 0)
        return 0;
    SDL_Surface *sdls = surface->get_surface();

    return MakeSurface (w, h, sdls->format->BitsPerPixel,
                        RGBA_Mask(sdls->format->Rmask,
                                  sdls->format->Gmask,
                                  sdls->format->Bmask,
                                  sdls->format->Amask));
}

/*! Resample a region inside a surface to a new size.  Returns a
  new 32 bit RGBA image containing the scaled image. */
Surface *ecl::Resample (Surface *s, Rect rect, int neww, int newh,
                       ResampleFilter /*filter*/)
{
    SDL_Surface *sdls = SDL_CreateRGBSurface(SDL_SWSURFACE, rect.w, rect.h, 32,
                                             0, 0, 0, 0);
    SDL_Rect r;
    sdl::copy_rect (r, rect);
    SDL_BlitSurface (s->get_surface(), &r, sdls, 0);

    SDL_Surface *s_new;
    s_new  = zoomSurface (sdls,
                          (double)neww/rect.w, 
                          (double)newh/rect.h, true);

    SDL_FreeSurface (sdls);
    return Surface::make_surface (s_new);
}

