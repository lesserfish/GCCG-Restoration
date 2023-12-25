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

#include <SDL2/SDL_render.h>
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
    /// Pointer to the window.
    static SDL_Window* window;
    /// Pointer to the renderer
    static SDL_Renderer *renderer;
    /// Index of the driver being used.
    static int driver_index;
    /// Pointer to current output texture.
    static SDL_Texture *output;
    /// Number of the current texture.
    static int output_surface;
    /// Current set of allocated textures. NULL denotes free postion. Surface 0 is the screen.
    static vector<SDL_Texture*> textures;
    /// Structure of loaded cardimages: cardimage[card number][size][angle] is 0 if not loaded.
    static map<int,map<int,map<int,SDL_Texture*> > > cardimage;
    /// Vector for image store in display format.
    static vector<SDL_Texture*> image_collection;
    /// Vector for image store in original format.
    static vector<SDL_Texture*> image_collection_original;
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
        Uint8   *bits;

        bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*(surface->format->BytesPerPixel);

        switch(surface->format->BytesPerPixel)
        {
            case 2:
                *((Uint16 *)(bits)) = (Uint16)pixel;
                break;

            case 3:
                Uint8 r, g, b;

                r = (pixel>>surface->format->Rshift)&0xFF;
                g = (pixel>>surface->format->Gshift)&0xFF;
                b = (pixel>>surface->format->Bshift)&0xFF;
                *((bits)+surface->format->Rshift/8) = r; 
                *((bits)+surface->format->Gshift/8) = g;
                *((bits)+surface->format->Bshift/8) = b;
                break;

            case 4:
                *((Uint32 *)(bits)) = pixel;
                break;

            default:
                throw Error::IO("SetPixel(SDL_Surface*,int,int,Uint32)","Unsupported color depth "+ToString(surface->format->BytesPerPixel));
        }
    }

    static Uint32 GetPixel(SDL_Surface* surface,int x,int y)
    {
        Uint8   *bits;

        SDL_PixelFormat *fmt=surface->format;

        bits = ((Uint8 *)surface->pixels)+y*surface->pitch+x*(fmt->BytesPerPixel);

        switch(surface->format->BytesPerPixel)
        {
            case 2:
                return *((Uint16 *)(bits));

            case 3:

                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)			  
                    return ((*bits)<<16) | ((*(bits+1))<<8) | (*(bits+2));
                else
                    return ((*bits)) | ((*(bits+1))<<8) | (*(bits+2)<<16);

            case 4:
                return *((Uint32 *)(bits));

            default:
                throw Error::IO("GetPixel(SDL_Surface*,int,int)","Unsupported color depth "+ToString(surface->format->BytesPerPixel));
        }

        return 0;
    }
    static int GetEvent(SDL_Event& event)
    {
        if(SDL_PollEvent(&event)==0)
            return 0;

        if(event.type==SDL_KEYUP)
        {
            if(event.key.keysym.sym == SDLK_RCTRL || event.key.keysym.sym == SDLK_LCTRL)
                control=false;
            else if(event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT)
                shift=false;
            else if(event.key.keysym.sym == SDLK_RALT || event.key.keysym.sym == SDLK_LALT)
                alt=false;
        }
        else if(event.type==SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_RCTRL || event.key.keysym.sym == SDLK_LCTRL)
                control=true;
            else if(event.key.keysym.sym == SDLK_RSHIFT || event.key.keysym.sym == SDLK_LSHIFT)
                shift=true;
            else if(event.key.keysym.sym == SDLK_RALT || event.key.keysym.sym == SDLK_LALT)
                alt=true;
        }
        else if (event.type==SDL_MOUSEBUTTONDOWN)
        {
            if(event.button.button==SDL_BUTTON_LEFT)
                mouse1=true;
            else if(event.button.button==SDL_BUTTON_RIGHT)
                mouse2=true;
            else if(event.button.button==SDL_BUTTON_MIDDLE)
                mouse3=true;
        }
        else if (event.type==SDL_MOUSEBUTTONUP)
        {
            if(event.button.button==SDL_BUTTON_LEFT)
                mouse1=false;
            else if(event.button.button==SDL_BUTTON_RIGHT)
                mouse2=false;
            else if(event.button.button==SDL_BUTTON_MIDDLE)
                mouse3=false;
        }
        else if(event.type==SDL_MOUSEMOTION)
        {
            SDL_Event peep;
            int count=0;
            while(count < 50 && 
                    SDL_PeepEvents(&peep,
                        1,
                        SDL_GETEVENT,
                        SDL_MOUSEMOTION,
                        SDL_MOUSEMOTION) ==1)
            {
                event.motion.x=peep.motion.x;
                event.motion.y=peep.motion.y;
                event.motion.xrel+=peep.motion.xrel;
                event.motion.yrel+=peep.motion.yrel;
                count++;
            }
        }

        return 1;    }

    static string CommandModifier()
    {
        if(shift && control && alt)
            return "shift control alt ";
        else if(shift && control)
            return "shift control ";
        else if(control && alt)
            return "control alt ";
        else if(shift && alt)
            return "shift alt ";
        else if(shift)
            return "shift ";
        else if(alt)
            return "alt ";
        else if(control)
            return "control ";
        else
            return "";
    }

    static unsigned char fix_SDL_Keypad(SDL_Keycode key)
    {
        if((key >= SDLK_KP_0 && key <= SDLK_KP_9))
        {
            return '0' + key - SDLK_KP_0;
        }
        if(key == SDLK_KP_PERIOD){
            return '.';
        }
        if(key == SDLK_KP_DIVIDE){
            return '/';
        }
        return 0;
    }

    //
    //  SDL DRIVER CORE
    //  ===============
    //

    Driver::Driver(int screenwidth,int screenheight,bool _fullscreen,int physwidth,int physheight)
    {

		std::cout << "Driver\n";
        std::cout << "1\n";
        nographics=false;
        fullscreen=_fullscreen;

        scrw=screenwidth;
        scrh=screenheight;
        physw=physwidth ? physwidth : scrw;
        physh=physheight ? physheight : scrh;
        needscale=(physw != scrw || physh != scrh);

        cards=Database::cards.Cards();
        std::cout << "2\n";

        if(nosounds)
        {
            if(SDL_InitSubSystem(SDL_INIT_VIDEO))
                throw Error::IO("SDL_InitSubSystem failed",SDL_GetError());
        }
        else
        {
            if(SDL_InitSubSystem(SDL_INIT_VIDEO|SDL_INIT_AUDIO))
                throw Error::IO("SDL_InitSubSystem failed",SDL_GetError());

            if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096)<0)
            {
                nosounds=true;
                cerr << "Mix_OpenAudio failed: " << SDL_GetError();
            }
        }
        std::cout << "3\n";


        string caption="Gccg v"VERSION" ";
        caption+=Database::game.Gamedir();

        Uint32 flags=(fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_OPENGL;
        window = SDL_CreateWindow(caption.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                physw, physh,
                flags);
        if(!window){
            throw Error::IO("Driver::Driver(bool)",SDL_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


        if(!renderer){
            throw Error::IO("Driver::Driver(bool)",SDL_GetError());
        }

        std::cout << "4\n";
        SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(renderer, &rendererInfo);

        std::cout << "5\n";
        // TODO: Get proper driver_index
        driver_index = 0; //rendererInfo.num;

        std::cout << "6\n";
        main_icon = IMG_Load(CCG_DATADIR"/graphics/icon32.jpg");
        SDL_SetWindowIcon(window, main_icon); // TODO: Setting Window ICON gives a SEGFAULT

        std::cout << "7\n";
        output = SDL_CreateTexture(renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                physw,
                physh);
        output_surface=0;

        textures.resize(100);
        textures[0] = output;

        std::cout << "8\n";
        TTF_Init();

        control=false;
        shift=false;
        alt=false;
        mouse1=false;
        mouse2=false;
        mouse3=false;

        std::cout << "9\n";

        string drivername(SDL_GetVideoDriver(driver_index));
        SDL_DisplayMode info;
        if(SDL_GetCurrentDisplayMode(driver_index, &info) < 0)
        {
            throw Error::IO("Driver::Driver(bool)",SDL_GetError());
        }

        cout << Localization::Message("Graphics initialized:") << endl;
        cout << Localization::Message("  Driver = %s", drivername) << endl;
        cout << Localization::Message("  Fullscreen = %s", SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN ? "Yes" : "No") << endl;
        cout << Localization::Message("  Physical resolution = %s", ToString(info.w)+"x"+ToString(info.h)) << endl;
        cout << Localization::Message("  Refresh rate = %s", ToString(info.refresh_rate) + " Hz") << endl;

#ifdef WIN32
        // PERF_IMPROVEMENT: Avoid SDL_Image to load/unload shared libs on every image
        // This also seems to fix a double load of the same file.
        void* jpeglib = SDL_LoadObject("jpeg.dll");	
        void* pnglib = SDL_LoadObject("libpng12-0.dll");
#endif

    }

    Driver::~Driver()
    {

		std::cout << "~Driver\n";
#ifdef WIN32
        SDL_UnloadObject(pnglib);
        SDL_UnloadObject(jpeglib);
#endif

        SDL_FreeSurface(main_icon);

        map<int,map<int,map<int,SDL_Texture*> > >::iterator i;
        map<int,map<int,SDL_Texture*> >::iterator j;
        map<int,SDL_Texture*>::iterator k;

        for(i=cardimage.begin(); i!=cardimage.end(); i++)
            for(j=(*i).second.begin(); j!=(*i).second.end(); j++)
                for(k=(*j).second.begin(); k!=(*j).second.end(); k++)
                    SDL_DestroyTexture((*k).second);

        for(size_t n=0; n<textures.size(); n++)
            if(textures[n])
                SDL_DestroyTexture(textures[n]);

        TTF_Quit();

        // TODO: free all loaded sounds
        // Mix_FreeChunk(chunk);

        Mix_CloseAudio();

    }

    int Driver::AllocateSurface(int w,int h)
    {
        if(w <= 0 || h <= 0)
            throw Error::Invalid("Driver::AllocateSurface","invalid size "+ToString(w)+"x"+ToString(h));

        int i=1;
        while((size_t)i<textures.size() && textures[i])
            i++;
        if((size_t)i==textures.size())
            textures.resize(textures.size()+100);

        SDL_Texture* t = SDL_CreateTexture(renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                w,
                h);

        if(t==0)
            throw Error::Memory("Driver::AllocateSurface(int,int): " + string(SDL_GetError()));


        SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND); 
        textures[i]=t;

        return i;
    }

    int Driver::SelectSurface(int num)
    {

        std::cout << "SelectSurface\n";
        if(num==-1)
            throw Error::Invalid("Driver::SelectSurface","Surface not yet allocated");

        if(num < 0 || (size_t)num >= textures.size())
            throw Error::Invalid("Driver::SelectSurface","Invalid surface number "+ToString(num));

        if(textures[num]==0)
            throw Error::Invalid("Driver::SelectSurface","No such surface "+ToString(num));

        output=textures[num];
        int ret=output_surface;
        output_surface=num;

        return ret;
    }

    void Driver::FreeSurface(int num)
    {

        std::cout << "FreeSurface\n";
        if(num < 1 || (size_t)num >= textures.size())
            throw Error::Invalid("Driver::FreeSurface","Invalid surface number "+ToString(num));

        if(textures[num]==0)
            throw Error::Invalid("Driver::FreeSurface","No such surface "+ToString(num));

        SDL_DestroyTexture(textures[num]);
        textures[num]=0;
    }

    void Driver::DrawSurface(int x,int y,int num)
    {

        std::cout << "DrawSurface\n";
        if(num < 0 || (size_t)num >= textures.size())
            throw Error::Invalid("Driver::DrawSurface","Invalid surface number "+ToString(num));
        if(textures[num]==0)
            throw Error::Invalid("Driver::DrawSurface","No such surface "+ToString(num));

        SDL_Rect dst;

        int textureWidth, textureHeight;
        SDL_QueryTexture(textures[num], NULL, NULL, &textureWidth, &textureHeight);


        dst.x=x;
        dst.y=y;
        dst.w=textureWidth;
        dst.h=textureHeight;

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, textures[num], 0, &dst);

    }
    int Driver::SurfaceWidth(int num) const
    {
        if(num < 0 || (size_t)num >= textures.size())
            throw Error::Invalid("Driver::SurfaceWidth","Invalid surface number "+ToString(num));
        if(textures[num]==0)
            throw Error::Invalid("Driver::SurfaceWidth","No such surface "+ToString(num));

        int textureWidth, textureHeight;
        SDL_QueryTexture(textures[num], NULL, NULL, &textureWidth, &textureHeight);

        return textureWidth;
    }

    int Driver::SurfaceHeight(int num) const
    {
        if(num < 0 || (size_t)num >= textures.size())
            throw Error::Invalid("Driver::SurfaceWidth","Invalid surface number "+ToString(num));
        if(textures[num]==0)
            throw Error::Invalid("Driver::SurfaceWidth","No such surface "+ToString(num));

        int textureWidth, textureHeight;
        SDL_QueryTexture(textures[num], NULL, NULL, &textureWidth, &textureHeight);

        return textureHeight;    
    }

    void Driver::Fullscreen(bool mode)
    {

        std::cout << "Fullscreen\n";
        if(mode!=fullscreen)
        {
            fullscreen=mode;
            Uint32 flags = mode ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
            SDL_SetWindowFullscreen(window,flags);
        }
    }

    void Driver::Beep()
    {

        std::cout << "Beep\n";
        string beep_buffer="\007";
        cout << beep_buffer << flush;

    }

    void Driver::Blink(int enabled)
    {

        std::cout << "Blink\n";
        // TODO: Implement this
    }

    void Driver::UpdateScreen(int x0,int y0,int w,int h)
    {

        std::cout << "UpdateScreen\n";
        UpdateScreen();
    }

    void Driver::UpdateScreen()
    {

        std::cout << "UpdateScreen\n";
        SDL_RenderPresent(renderer);
    }

    void Driver::DrawCardImage(int imagenumber,int x,int y,int size,int angle,int alpha)
    {

        std::cout << "DrawCardImage\n";
        LoadIfUnloaded(imagenumber,size,angle);

        SDL_Rect dest;

        dest.x=x;
        dest.y=y;

        // blit graphic across.  <srcrect> is NULL: blit entire object
        SDL_RenderCopy(renderer, cardimage[imagenumber][size][angle], NULL, &dest);
    }

    int Driver::CardWidth(int imagenumber,int size,int angle)
    {

        std::cout << "CardWidth\n";
        LoadIfUnloaded(imagenumber,size,angle);
        int textureWidth, textureHeight;
        SDL_QueryTexture(cardimage[imagenumber][size][angle], NULL, NULL, &textureWidth, &textureHeight);

        return textureWidth;    
    }

    int Driver::CardHeight(int imagenumber,int size,int angle)
    {

        std::cout << "CardHeight\n";
        LoadIfUnloaded(imagenumber,size,angle);
        int textureWidth, textureHeight;
        SDL_QueryTexture(cardimage[imagenumber][size][angle], NULL, NULL, &textureWidth, &textureHeight);

        return textureHeight;    
    }

    void Driver::WaitKeyPress()
    {

        std::cout << "WaitKeyPress\n";
        SDL_Event event;

        while(1)
        {
            SDL_PollEvent(&event);
            if(event.type==SDL_KEYDOWN)
                break;
        }

        while(1)
        {
            SDL_PollEvent(&event);
            if(event.type==SDL_KEYUP)
                break;
        }

    }

    void Driver::ClearArea(int x0,int y0,int w,int h)
    {

        std::cout << "ClearArea\n";
        SDL_RenderClear(renderer);
    }

    void Driver::SetClipping(int surf,int x0,int y0,int w,int h)
    {

        std::cout << "SetClipping\n";
        // I don't think this exists for textures
    }

    void Driver::ClippingOff(int surf)
    {

        std::cout << "ClippingOff\n";
        // I don't think this exists for textures
    }

    void Driver::DrawFilledBox(int x0,int y0,int w,int h,Color c)
    {

        std::cout << "DrawFilledBox\n";
        if(w==0 || h==0)
            return;

        if(w < 1)
            w=1;
        if(h < 1)
            h=1;

        if(!c.invisible)
        {
            SDL_Rect dst;

            dst.x=x0;
            dst.y=y0;
            dst.w=w;
            dst.h=h;

            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(renderer, &dst);
        }

    }

    void Driver::DrawTriangleUp(int x0,int y0,int h,Color c)
    {

        std::cout << "DrawTriangleUp\n";
        if(!c.invisible)
        {
            y0+=h-1;
            for(int i=0; i<h; i++)
            {
                DrawFilledBox(x0,y0,(h-i)*2,1,c);
                x0++;
                y0--;
            }
        }

    }

    void Driver::DrawTriangleDown(int x0,int y0,int h,Color c)
    {

        std::cout << "DrawTriangleDown\n";
        if(!c.invisible)
        {
            for(int i=0; i<h; i++)
            {
                DrawFilledBox(x0,y0,(h-i)*2,1,c);
                x0++;
                y0++;
            }
        }
    }

    void Driver::DrawBox(int x0,int y0,int w,int h,Color c)
    {

        std::cout << "DrawBox\n";
        if(!c.invisible)
        {
            DrawFilledBox(x0,y0,w,1,c);
            DrawFilledBox(x0,y0+h-1,w,1,c);
            DrawFilledBox(x0,y0,1,h,c);
            DrawFilledBox(x0+w-1,y0,1,h,c);
        }

    }

    void Driver::HideMouse()
    {

        std::cout << "HideMouse\n";
        SDL_ShowCursor(SDL_DISABLE);
    }

    void Driver::ShowMouse()
    {

        std::cout << "ShowMouse\n";
        SDL_ShowCursor(SDL_ENABLE);
    }

    Command Driver::WaitCommand(int delay)
    {

        std::cout << "WaitCommand\n";
        static int state=0; // 0 - normal, 1 - dragging, 2 - mouse clicked (waiting drag or release)
        static string dragtype; // Description of the dragging mode (ctrl,left etc.).
        static int dragx,dragy; // Mouseposition when dragging begun
        static Uint32 clicktime_l=0; // Time when mouse pressed button down.
        static Uint32 clicktime_m=0;
        static Uint32 clicktime_r=0;

        // Get the event if any.
        Command ret;
        SDL_Event event;

        SDL_Delay(delay);

        event.type = SDL_USEREVENT;

        if(!GetEvent(event) && state!=2)
            return ret;

        // Handle application input focus
        if(event.type == SDL_WINDOWEVENT)
        {
            if(event.window.event == SDL_WINDOWEVENT_ENTER)
            {
                ret.command = "input gained";
                return ret;
            }
            if(event.window.event == SDL_WINDOWEVENT_LEAVE)
            {
                ret.command = "input lost";
                return ret;
            }
        }

        // Handle screen refresh.
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_EXPOSED)
        {
            ret.command="redraw";
            return ret;
        }

        // Handle quit event.
        if(event.type==SDL_QUIT)
        {
            ret.command="quit";
            return ret;
        }

        // Handle key press
        if(state==0 && event.type==SDL_KEYDOWN)
        {
            SDL_GetMouseState(&ret.x,&ret.y);

            ret.command=CommandModifier()+"key";
            ret.argument=SDL_GetKeyName(event.key.keysym.sym);
            ret.key = event.text.text[0];
            if( ret.key == 0 ) ret.key = fix_SDL_Keypad(event.key.keysym.sym);

            return ret;
        }

        // Handle key release
        if(state==0 && event.type==SDL_KEYUP)
        {
            SDL_GetMouseState(&ret.x,&ret.y);

            ret.command=CommandModifier()+"key up";
            ret.argument=SDL_GetKeyName(event.key.keysym.sym);
            ret.key = event.text.text[0];
            if( ret.key == 0 ) ret.key = fix_SDL_Keypad(event.key.keysym.sym);

            return ret;
        }

        // Handle mouse wheel
        if(event.type==SDL_MOUSEBUTTONUP && event.button.button==SDL_BUTTON_MIDDLE)
        {
            ret.x=event.button.x;
            ret.y=event.button.y;
            ret.command="wheel up";
            return ret;
        }
        else if(event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_MIDDLE)
        {
            ret.x=event.button.x;
            ret.y=event.button.y;
            ret.command="wheel down";
            return ret;
        }

        // Mouse state 1: dragging
        if(state==1)
        {
            if(event.type==SDL_MOUSEBUTTONUP)
            {
                ret.x=event.button.x;
                ret.y=event.button.y;
                state=0;
                ret.command=dragtype+" drag end";

                return ret;
            }
            else if(event.type==SDL_MOUSEMOTION)
            {
                ret.x=event.motion.x;
                ret.y=event.motion.y;
                ret.command=dragtype+" drag";

                return ret;
            }

            ret.command="";
            return ret;
        }

        // Mouse state 2: button is just pressed down
        static string click_command;
        if(state==2)
        {
            bool motion = false;
            int press_time=SDL_GetTicks();

            if(clicktime_l)
                press_time-=clicktime_l;
            else if(clicktime_r)
                press_time-=clicktime_r;
            else if(clicktime_m)
                press_time-=clicktime_m;

            if(event.type==SDL_MOUSEMOTION)
            {
                static int radius=0;

                if(radius==0)
                {
                    radius=physw/140;
                    radius*=physw/140;                            
                }

                int x,y;
                x=event.motion.x;
                y=event.motion.y;

                motion=(x-dragx)*(x-dragx) + (y-dragy)*(y-dragy) > radius;
            }

            if(event.type==SDL_MOUSEBUTTONUP)
            {
                ret.x=event.button.x;
                ret.y=event.button.y;
                ret.command=click_command+" click";
                state=0;
                return ret;
            }

            if(press_time > 400 || motion)
            {
                dragtype=click_command;
                ret.x=dragx;
                ret.y=dragy;
                ret.command=click_command+" drag begin";
                state=1;
                return ret;
            }

            ret.command="";
            return ret;
        }

        // Mouse state 0: buttons are not down.
        if (event.type==SDL_MOUSEBUTTONDOWN)
        {
            if(mouse1)
                clicktime_l=SDL_GetTicks();
            else
                clicktime_l=0;
            if(mouse2)
                clicktime_r=SDL_GetTicks();
            else
                clicktime_r=0;
            if(mouse3)
                clicktime_m=SDL_GetTicks();
            else
                clicktime_m=0;

            if(event.button.button==SDL_BUTTON_LEFT)
                click_command=CommandModifier()+"left";
            else if(event.button.button==SDL_BUTTON_RIGHT)
                click_command=CommandModifier()+"right";
            else if(event.button.button==SDL_BUTTON_MIDDLE)
                click_command=CommandModifier()+"middle";

            state=2;
            dragx=event.button.x;
            dragy=event.button.y;
        }

        // Mouse state 0: moving
        if(state==0 && event.type==SDL_MOUSEMOTION)
        {
            ret.command=CommandModifier()+"move";
            ret.x=event.motion.x;
            ret.y=event.motion.y;
            return ret;
        }

        ret.command="";
        return ret;

    }

    // Load cached image surface if available.
    static SDL_Surface* CacheLoad(const string& original_file,int imagenumber,int size,int angle)
    {
        if(nocache)
            return 0;

#ifdef WIN32
        string cachefile=getenv("TEMP");
        cachefile+="/gccg";
#else
        string cachefile="/tmp/gccg.";
        cachefile+=getenv("USER");
#endif
        cachefile+="/";
        cachefile+=Database::game.Gamedir();
        cachefile+="/";
        cachefile+=Database::cards.SetDirectory(Database::cards.Set(imagenumber));
        cachefile+="/";
        if(Localization::GetLanguage()!="en")
        {
            cachefile+=Localization::GetLanguage();
            cachefile+="/";
        }
        cachefile+=Database::cards.ImageFile(imagenumber);
        cachefile+="_";
        cachefile+=ToString(size);
        cachefile+="_";
        cachefile+=ToString(angle);
        cachefile+=".bmp";

        struct stat original,cached;
        if(stat(cachefile.c_str(),&cached))
            return 0;
        if(stat(original_file.c_str(),&original))
            return 0;
        if(cached.st_mtime < original.st_mtime)
        {
            unlink(cachefile.c_str());
            return 0;
        }

        return SDL_LoadBMP(cachefile.c_str());
    }

    // Create directory safely.
    static bool CreateDir(const string& dir)
    {
        struct stat dirfile;

#ifdef WIN32
        // PERF_IMPROVEMENT/TODO: use directory exists instead ? 
        if( stat(dir.c_str(), &dirfile)==0 )
        {
            if( dirfile.st_mode & S_IFDIR ) return true;
            if(unlink(dir.c_str())!=0) return false;
        }

        _mkdir(dir.c_str());
#else
        if(lstat(dir.c_str(),&dirfile)==0)
        {
            if(S_ISDIR(dirfile.st_mode))
                return true;
            if(unlink(dir.c_str())!=0)
                return false;
        }

        mkdir(dir.c_str(),0755);
#endif

        return true;
    }

    // Try to save cached image.
    static void CacheSave(SDL_Surface* surface,int imagenumber,int size,int angle)
    {
        if(nocache)
            return;

#ifdef WIN32
        string cachefile=getenv("TEMP");
        cachefile+="/gccg";
#else
        string cachefile="/tmp/gccg.";
        cachefile+=getenv("USER");
#endif
        // PERF_IMPROVEMENT/TODO: avoid attempting creation of existing directories
        if(!CreateDir(cachefile))
            return;
        cachefile+="/";
        cachefile+=Database::game.Gamedir();
        if(!CreateDir(cachefile))
            return;
        cachefile+="/";
        cachefile+=Database::cards.SetDirectory(Database::cards.Set(imagenumber));
        if(!CreateDir(cachefile))
            return;
        cachefile+="/";
        if(Localization::GetLanguage()!="en")
        {
            cachefile+=Localization::GetLanguage();
            if(!CreateDir(cachefile))
                return;
            cachefile+="/";
        }
        cachefile+=Database::cards.ImageFile(imagenumber);
        cachefile+="_";
        cachefile+=ToString(size);
        cachefile+="_";
        cachefile+=ToString(angle);
        cachefile+=".bmp";

#ifndef WIN32
        struct stat file;
        if(lstat(cachefile.c_str(),&file)==0)
        {
            if(unlink(cachefile.c_str())!=0)
                return;
        }
#endif

        if(SDL_SaveBMP(surface,cachefile.c_str())!=0)
            unlink(cachefile.c_str());
    }

    int Driver::LoadCardSound(int imagenumber)
    {

        std::cout << "LoadCardSound\n";
        if(nosounds) return 0;

        // Check arguments and if card is loaded already.		
        if(imagenumber < 0 || imagenumber >= Database::cards.Cards())
            return 0;

        if(cardsound[imagenumber] != 0)
            return cardsound[imagenumber];

        int snd=-1;
        int pos;

        string file=CCG_DATADIR;
        file+="/sounds/";
        file+=Database::game.Gamedir();
        file+="/";
        file+=Database::cards.SetDirectory(Database::cards.Set(imagenumber));
        file+="/";
        file+=Database::cards.ImageFile(imagenumber);
        file=Localization::File(file);
        if((pos=file.rfind(".")) >= 0)
        {
            file.replace(pos, file.length()-pos, ".ogg");
            snd=LoadSound(file.c_str());
        }

        if(snd < 0 )
        {
            string file=CCG_DATADIR;
            file+="/../";
            file+=ToLower(Database::game.Gamedir());
            file+="/sounds/";
            file+=Database::game.Gamedir();
            file+="/";
            file+=Database::cards.SetDirectory(Database::cards.Set(imagenumber));
            file+="/";
            file+=Database::cards.ImageFile(imagenumber);
            file=Localization::File(file);
            if((pos=file.rfind(".")) >= 0)
            {
                file.replace(pos, file.length()-pos, ".ogg");
                snd=LoadSound(file.c_str());
            }
        }

        cardsound[imagenumber]=snd;
        return snd;
    }

    void Driver::LoadIfUnloaded(int imagenumber,int size,int angle)
    {

        std::cout << "LoadIfUnloaded\n";
        bool self_generated=false;
        static list<int> image_load_order;

        // Check arguments and if card is loaded already.

        if(imagenumber < 0 || imagenumber >= Database::cards.Cards())
            throw Error::Range("Driver::LoadIfUnloaded()","Invalid imagenumber "+ToString(imagenumber));
        if(size < 1)
            throw Error::Range("Driver::LoadIfUnloaded()","Invalid size");
        if(angle < 0 || angle >= 360)
            throw Error::Range("Driver::LoadIfUnloaded()","Invalid angle");

        if(cardimage[imagenumber][size][angle])
            return;

        // Main loop of image loader.

try_again:

        SDL_Surface* newcard=0;

        string file=CCG_DATADIR;
        file+="/graphics/";
        file+=Database::game.Gamedir();
        file+="/";
        file+=Database::cards.SetDirectory(Database::cards.Set(imagenumber));
        file+="/";
        file+=Database::cards.ImageFile(imagenumber);
        file=Localization::File(file);

        // Load from cache or from original image file or generate it.
        if(!nographics || imagenumber==0)
        {
            newcard=CacheLoad(file,imagenumber,size,angle);
            if(newcard)
                goto scale_and_return;
            newcard=IMG_Load(file.c_str());
        }

        if(!nographics && !newcard)
        {
            string file=CCG_DATADIR;
            file+="/../";
            file+=ToLower(Database::game.Gamedir());
            file+="/graphics/";
            file+=Database::game.Gamedir();
            file+="/";
            file+=Database::cards.SetDirectory(Database::cards.Set(imagenumber));
            file+="/";
            file+=Database::cards.ImageFile(imagenumber);
            file=Localization::File(file);
            newcard=IMG_Load(file.c_str());
        }

        if(!newcard && imagenumber==0)
        {
            cerr << "Warning: cannot load " << file << endl;
            file=CCG_DATADIR;
            file+="/graphics/unknown_card.png";
            newcard=IMG_Load(file.c_str());
        }

        if(!newcard)
        {
            if(imagenumber==0)
                throw Error::IO("Driver::LoadIfUnloaded(int,int,int)","Unable to load 0-image file '"+file+"'");

            newcard=CreateOwnCard(imagenumber);

            self_generated=true;
        }

        if(!newcard)
            goto out_of_memory;

        // Now stretch it to the correct size and angle.

        // Use the information to set up parameters for creating a surface
        SDL_Surface* tmp;

        if(angle==0 && size!=100)
        {
            // Circumvent buggy SDL_rotozoom and check if there is memory.
            tmp=SDL_CreateRGBSurface(0, physw, physh, 32, 0, 0, 0, 0);
            if(!tmp)
                goto out_of_memory;
            SDL_FreeSurface(tmp);

            tmp=zoomSurface(newcard,double(size)/100.0,double(size)/100.0,SMOOTHING_OFF);
            SDL_FreeSurface(newcard);
            newcard=tmp;

            if(!newcard)
                goto out_of_memory;
        }
        else if(angle)
        {
            // Circumvent buggy SDL_rotozoom and check if there is memory.
            tmp=SDL_CreateRGBSurface(0, physw, physh, 32, 0, 0, 0, 0);
            if(!tmp)
                goto out_of_memory;
            SDL_FreeSurface(tmp);

            int a=angle;
            if(a)
                a=360-angle;
            if(a%90)
                tmp=rotozoomSurface(newcard,double(a),double(size)/100.0,SMOOTHING_ON);
            else
                //multiples of 90 degrees allow straightforward translation
                //so no smoothing necessary
                tmp=rotozoomSurface(newcard,double(a),double(size)/100.0,SMOOTHING_OFF);
            SDL_FreeSurface(newcard);
            newcard=tmp;

            if(!newcard)
                goto out_of_memory;
        }
        // otherwise size and angle are both unchanged, so do nothing

        // Save it to the cache.

        if(!self_generated)
            CacheSave(newcard,imagenumber,size,angle);

        goto scale_and_return;

        // What to do when out of memory.

out_of_memory:

        if(newcard)
            SDL_FreeSurface(newcard);

        if(image_load_order.size()==0)
            throw Error::Memory("Driver::LoadIfUnloaded(int,int,int)");

        for(int k=20; k; k--)
        {
            if(image_load_order.size())
            {
                int img=image_load_order.front();
                image_load_order.pop_front();

                if(img > 0)
                {
                    map<int,map<int,SDL_Texture*> >::iterator i;
                    for(i=cardimage[img].begin(); i!=cardimage[img].end(); i++)
                    {
                        int sz=i->first;
                        map<int,SDL_Texture*>::iterator j;
                        for(j=(*i).second.begin(); j!=(*i).second.end();j++)
                        {
                            int an=j->first;
                            SDL_DestroyTexture(cardimage[img][sz][an]);
                            cardimage[img][sz][an]=0;
                        }
                    }
                }
            }
        }

        goto try_again;

scale_and_return:

        // Scale if needed.
        if(needscale)
        {
            // Circumvent buggy SDL_rotozoom and check if there is memory.

            tmp=SDL_CreateRGBSurface(0, physw, physh, 32, 0, 0, 0, 0);
            if(!tmp)
                goto out_of_memory;
            SDL_FreeSurface(tmp);

            tmp=zoomSurface(newcard,double(physw)/double(scrw),double(physh)/double(scrh),SMOOTHING_ON);
            SDL_FreeSurface(newcard);
            newcard=tmp;
        }

        if(!newcard)
            goto out_of_memory;

        cardimage[imagenumber][size][angle]=SDL_CreateTextureFromSurface(renderer, newcard);
        SDL_FreeSurface(newcard);

        newcard=0;

        if(!cardimage[imagenumber][size][angle])
            goto out_of_memory;

        image_load_order.push_back(imagenumber);
    }


    int Driver::LoadImage(const string& filename,Color c)
    {
        SDL_Surface* img=IMG_Load(filename.c_str());

        SDL_Surface* imgscaled;
        if(!img)
            throw Error::IO("Driver::LoadImage(const string&,int,int,int)",string("Unable to load file '"+filename+"': ")+SDL_GetError());
        if(!c.invisible)
            SDL_SetColorKey(img,SDL_TRUE,SDL_MapRGB(img->format,c.r,c.g,c.b));

        imgscaled=img;
        if(needscale)
        {
            imgscaled=zoomSurface(img,double(physw)/double(scrw),double(physh)/double(scrh),SMOOTHING_ON);
            if(!imgscaled)
                throw Error::Memory("Driver::LoadImage(const string&,Color)");
            if(!c.invisible)
                SDL_SetColorKey(imgscaled,SDL_TRUE,SDL_MapRGB(imgscaled->format,c.r,c.g,c.b));
            SDL_FreeSurface(img);
        }

        SDL_Texture *txtscaled = SDL_CreateTextureFromSurface(renderer, imgscaled);
        SDL_Texture *txt= SDL_CreateTextureFromSurface(renderer, imgscaled);
        image_collection_original.push_back(txtscaled);

        image_collection.push_back(txt);

        return image_collection.size()-1;

    }

    void Driver::ScaleImage(int img,int neww,int newh)
    {

        std::cout << "ScaleImage\n";
    }

    void Driver::DrawImage(int image_number,int x,int y,int scl,int alpha,Color colorkey)
    {

        std::cout << "DrawImage\n";
    }

    int Driver::ImageWidth(int image_number)
    {

        std::cout << "ImageWidth\n";
        return 0;
    }

    int Driver::ImageHeight(int image_number)
    {

        std::cout << "ImageHeight\n";
        return 0;
    }

    void Driver::GetRGB(int image_number,int x,int y,int &r,int &g,int &b)
    {

        std::cout << "GetRGB\n";
    }

    int Driver::ValidImage(int number)
    {

        std::cout << "ValidImage\n";
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

        std::cout << "DrawTextToSurface\n";
    }

    int Driver::LoadFont(const string& primary,const string& secondary)
    {

        std::cout << "LoadFont\n";
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

        std::cout << "DrawText\n";
    }

    void Driver::DrawTextShadow(int fontnumber,int pointsize,int x0,int y0,const string& text,Color color,Color bgcolor)
    {

        std::cout << "DrawTextShadow\n";
    }

    /// Calculate width of the text.
    int Driver::GetTextWidth(string text,TextStyle style)
    {

        std::cout << "GetTextWidth\n";
        return 0;
    }

    /// Calculate height of the text.
    int Driver::GetTextHeight(string text,TextStyle style)
    {

        std::cout << "GetTextHeight\n";
        return 0;
    }

    int Driver::GetTextHeight(string text,TextStyle style,int width)
    {

        std::cout << "GetTextHeight\n";
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

        std::cout << "RenderTextFitVertical\n";
    }

    void Driver::RenderTextFitHorizontal(const string& text,TextStyle& style,int x0,int y0,int width,int height)
    {

        std::cout << "RenderTextFitHorizontal\n";
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

    static void DrawBoxToCard(SDL_Texture* ret,int imagenumber,const char* boxname)
    {
    }

    static void DrawTextToCard(SDL_Texture* ret,int imagenumber,const char* textname)
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
        std::cout << "LoadSound\n";
        return 0;
    }

    void Driver::PlaySound(int sound_number)
    {
        std::cout << "PlaySound\n";
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
