/*
   Gccg - Generic collectible card game.
   Copyright (C) 2001,2002,2003,2004 Tommi Ronkainen

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program, in the file license.txt. If not, write
   to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
   */

#if !defined(WIN32) && !defined(__WIN32__)
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#include <sys/stat.h>

#if defined(_MSC_VER)
# include "compat.h"
# include <direct.h>
#endif

// #include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_loadso.h>
#include <SDL2/SDL_mixer.h>
#include "SDL_rotozoom.h"

#include <string>
#include <vector>
#include <map>
#include "driver.h"
#include "error.h"
#include "carddata.h"
#include "tools.h"
#include "version.h"
#include "localization.h"

using namespace Database;

#define AlphaToSDL(c) (int)((c.a)*(SDL_ALPHA_OPAQUE-SDL_ALPHA_TRANSPARENT)/100)+SDL_ALPHA_TRANSPARENT

namespace Driver
{
    /// Do we want free fonts even if other fonts available.
    bool use_free_fonts=true;
    /// Pointer to the physical screen.
    static SDL_Surface* screen;
    /// Pointer to current output surface.
    static SDL_Surface* output=0;
    /// Number of the current surface.
    static int output_surface=0;
    /// Current set of allocated surfaces. NULL denotes free postion. Surface 0 is the screen.
    static vector<SDL_Surface*> surface;
    /// Structure of loaded cardimages: cardimage[card number][size][angle] is 0 if not loaded.
    static map<int,map<int,map<int,SDL_Surface*> > > cardimage;
    /// Vector for image store in display format.
    static vector<SDL_Surface*> image_collection;
    /// Vector for image store in original format.
    static vector<SDL_Surface*> image_collection_original;
    /// Pointer to the main window icon
    SDL_Surface* main_icon=0;

    /// Structure of loaded card sounds: cardsound[card number] is 0 if not loaded.
    static map<int,int> cardsound;
    /// Vector for sounds.
    static vector<Mix_Chunk*> sound_collection;
    /// enable/disable sounds.
    bool nosounds=false;

    /// Font files.
    static vector<string> fontfile;
    /// Fonts.
    static vector<map<int,TTF_Font*> > font;
    /// Left mouse button down.
    static bool mouse1;
    /// Right mouse button down.
    static bool mouse2;
    /// Middle mouse button down.
    static bool mouse3;
    /// Do we have a control pressed?
    static bool control;
    /// Do we have a shift pressed?
    static bool shift;
    /// Do we have a alt pressed?
    static bool alt;

    /// Create own card using xml-description.
    static SDL_Surface* CreateOwnCard(int imagenumber);

    /// lock dynamic libraries
#ifdef WIN32
    static void *jpeglib, *pnglib;
#endif

    // STATIC SUPPORT FUNCTIONS
    // ========================

    static void SetPixel(SDL_Surface* surface,int x,int y,Uint32 pixel)
    {
    }

    static Uint32 GetPixel(SDL_Surface* surface,int x,int y)
    {
        return 0;
    }

    static int GetEvent(SDL_Event& event)
    {
        return 1;
    }

    static string CommandModifier()
    {
        return "";
    }

    static unsigned char fix_SDL_Keypad(SDL_Keycode key)
    {
        return 0;
    }

    //
    //  SDL DRIVER CORE
    //  ===============
    //

    Driver::Driver(int screenwidth,int screenheight,bool _fullscreen,int physwidth,int physheight)
    {
    }

    Driver::~Driver()
    {
    }

    int Driver::AllocateSurface(int w,int h)
    {
        return 0;
    }

    int Driver::SelectSurface(int num)
    {
        return 0;
    }

    void Driver::FreeSurface(int num)
    {
    }

    void Driver::DrawSurface(int x,int y,int num)
    {
    }
    int Driver::SurfaceWidth(int num) const
    {
        return 0;
    }
	
    int Driver::SurfaceHeight(int num) const
    {
        return 0;
    }

    void Driver::Fullscreen(bool mode)
    {
    }

    void Driver::Beep()
    {
    }

    void Driver::Blink(int enabled)
    {
    }

    void Driver::UpdateScreen(int x0,int y0,int w,int h)
    {
    }

    void Driver::UpdateScreen()
    {
    }

    void Driver::DrawCardImage(int imagenumber,int x,int y,int size,int angle,int alpha)
    {
    }

    int Driver::CardWidth(int imagenumber,int size,int angle)
    {
        return 0;
    }

    int Driver::CardHeight(int imagenumber,int size,int angle)
    {
        return 0;
    }

    void Driver::WaitKeyPress()
    {
    }

    void Driver::ClearArea(int x0,int y0,int w,int h)
    {
    }

    void Driver::SetClipping(int surf,int x0,int y0,int w,int h)
    {
    }

    void Driver::ClippingOff(int surf)
    {
    }

    void Driver::DrawFilledBox(int x0,int y0,int w,int h,Color c)
    {
    }

    void Driver::DrawTriangleUp(int x0,int y0,int h,Color c)
    {
    }

    void Driver::DrawTriangleDown(int x0,int y0,int h,Color c)
    {
    }

    void Driver::DrawBox(int x0,int y0,int w,int h,Color c)
    {
    }

    void Driver::HideMouse()
    {
    }

    void Driver::ShowMouse()
    {
    }

    Command Driver::WaitCommand(int delay)
    {
        Command ret;
    }

    // Load cached image surface if available.
    static SDL_Surface* CacheLoad(const string& original_file,int imagenumber,int size,int angle)
    {
        return NULL;
    }

    // Create directory safely.
    static bool CreateDir(const string& dir)
    {
        return true;
    }

    // Try to save cached image.
    static void CacheSave(SDL_Surface* surface,int imagenumber,int size,int angle)
    {
    }

    int Driver::LoadCardSound(int imagenumber)
    {
        return 0;
    }

    void Driver::LoadIfUnloaded(int imagenumber,int size,int angle)
    {
    }


    int Driver::LoadImage(const string& filename,Color c)
    {
        return 0;
    }

    void Driver::ScaleImage(int img,int neww,int newh)
    {
    }

    void Driver::DrawImage(int image_number,int x,int y,int scl,int alpha,Color colorkey)
    {
    }

    int Driver::ImageWidth(int image_number)
    {
        return 0;
    }

    int Driver::ImageHeight(int image_number)
    {
        return 0;
    }

    void Driver::GetRGB(int image_number,int x,int y,int &r,int &g,int &b)
    {
    }

    int Driver::ValidImage(int number)
    {
        return 1;
    }

    void Driver::CheckFont(int fontnumber,int pointsize) const
    {
    }

    // TEXT RENDERING
    // ==============

    /// Render text to surface.
    void Driver::DrawTextToSurface(int fontnumber,int pointsize,int x0,int y0,const string& _text,Color color,Color bgcolor,bool addshadow)
    {
    }

    int Driver::LoadFont(const string& primary,const string& secondary)
    {
        return 0;
    }

    int Driver::TextLineSkip(int fontnumber,int pointsize) const
    {
        return 0;
    }

    int Driver::TextHeight(int fontnumber,int pointsize,const string& text) const
    {
        return 0;
    }

    int Driver::TextWidth(int fontnumber,int pointsize,const string& text) const
    {
        return 0;
    }

    void Driver::DrawText(int fontnumber,int pointsize,int x0,int y0,const string& text,Color color,Color bgcolor)
    {
    }

    void Driver::DrawTextShadow(int fontnumber,int pointsize,int x0,int y0,const string& text,Color color,Color bgcolor)
    {
    }

    /// Calculate width of the text.
    int Driver::GetTextWidth(string text,TextStyle style)
    {
        return 0;
    }

    /// Calculate height of the text.
    int Driver::GetTextHeight(string text,TextStyle style)
    {
        return 0;
    }

    int Driver::GetTextHeight(string text,TextStyle style,int width)
    {
        return 0;
    }

    void Driver::RenderTextLine(
            const string& text, TextStyle style, int& x0, int& y0, int width,
            bool move_upward, bool fit_text, bool cut_text
            ) {
    }

    void Driver::RenderTextLineR(
            const string& text, TextStyle style, int& x0, int& y0, int width,
            bool move_upward, bool fit_text, bool cut_text
            ) {
    }

    void Driver::RenderTextFitVertical(const string& text,TextStyle& style,int x0,int y0,int width,int height)
    {
    }

    void Driver::RenderTextFitHorizontal(const string& text,TextStyle& style,int x0,int y0,int width,int height)
    {
    }

    // CARD CREATION
    // =============

    static string GameOption(int image,const string& option)
    {
        return "";
    }

    static Color GameColor(int image,const string& option,Color def)
    {
        return Color(0,0,0);
    }

    static Uint32 GameSDLColor(int image,const string& option,int r_def,int g_def,int b_def)
    {
        return 0;
    }

    static int GameInt(int image,const string& option,int def)
    {
        return 0;
    }

    static SDL_Rect GamePos(int image,const string& option,int x_def,int y_def,int w_def, int h_def)
    {
        SDL_Rect ret;
        return ret;
    }

    static void GetTextStyle(int image,const string& option,TextStyle& style,int font=4,int size=12,Color color=Color(0,0,0),int align=0)
    {
    }

    // Replace "[attr]" notations with the corresponding values of the attributes.
    static string GetAttrText(const string& src,int imagenumber)
    {
        return "";
    }

    static void DrawBoxToCard(SDL_Surface* ret,int imagenumber,const char* boxname)
    {
    }

    static void DrawTextToCard(SDL_Surface* ret,int imagenumber,const char* textname)
    {
    }

    static string ImageText(int imagenumber)
    {
        return "";
    }

    static SDL_Surface* CreateOwnCard(int imagenumber)
    {
        return NULL;
    }

    int Driver::LoadSound(const string& filename)
    {
        return 0;
    }

    void Driver::PlaySound(int sound_number)
    {
    }
}

// MAIN PROGRAM

int gccg_main(int argc,const char** argv);

#ifdef WIN32
extern "C" int SDL_main(int argc,char** argv)
{
    return gccg_main(argc,(const char**)argv);
}
#else
extern "C" int main(int argc,char** argv)
{
    return gccg_main(argc,(const char**)argv);
}
#endif
