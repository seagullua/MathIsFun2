#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"
#include <Windows.h>
USING_NS_CC;


void setDensity(float v);

void GetDesktopResolution(int& horizontal, int& vertical)
{
    RECT desktop;
    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();
    // Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);
    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)
    horizontal = desktop.right;
    vertical = desktop.bottom;
}


enum Mode
{
    //IPhones
    IPhone3GS, //480x320
    IPhone4GS, //960x640
    IPhone5, //1136x640
    //IPad
    IPad2, //1024x768
    IPad4, //2048x1536
    //Android phones
    Galaxy_Ace2, //62% 800x480
    Galaxy_Mini2, //14% 480x320
    HTC_One_S, //6% 960x540
    Motorola_Droid, //5% 854x480
    Galaxy_Y, //5% 320x240
    //Android tablets
    Galaxy_Tab2_7, //74% 1024x600
    Galaxy_Tab2_10, //22% 1280x800
    Google_Nexus_10, //2560x1600

    Kindle_Fire, //1280x720px,
    Sony_Xperia_Z, //1920x1080

    Microsoft_Surface_10, //1366x768
    HD_720p //1280x720

};
/*int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{*/
int CALLBACK WinMain(
        _In_  HINSTANCE,
        _In_  HINSTANCE,
        _In_  LPSTR,
        _In_  int
        )
{
    //UNREFERENCED_PARAMETER(hPrevInstance);
    //UNREFERENCED_PARAMETER(lpCmdLine);
#ifdef JUNIOR
    //assert(false);
#endif

    const float DENSITY_ldpi = 0.75f;
    const float DENSITY_mdpi = 1;
    const float DENSITY_hdpi = 1.5f;
    const float DENSITY_xhdpi = 2.0f;
    const float DENSITY_xxhdpi = 3.0f;

    Mode open_in_mode = Galaxy_Tab2_10;

    int swidth = 0;
    int sheight = 0;
    GetDesktopResolution(swidth, sheight);

    swidth -= 50;
    sheight -= 100;

    const float max_width = swidth;
    const float max_height = sheight;
    bool zoom = true;

    float width = 0;
    float height = 0;
    float bottom_spacing = 0;
    float density = DENSITY_mdpi;


    switch(open_in_mode)
    {
    case IPhone3GS:
        width = 480;
        height = 320;
        break;
    case IPhone4GS:
        width = 960;
        height = 640;
        break;
    case IPhone5:
        width = 1136;
        height = 640;
        break;
    case IPad2:
        width = 1024;
        height = 768;
        break;
    case IPad4:
        width = 2048;
        height = 1536;
        break;
    case Galaxy_Ace2:
        width = 800;
        height = 480;

        break;
    case Galaxy_Mini2:
        width = 480;
        height = 320;
        break;
    case HTC_One_S:
        width = 960;
        height = 540;
        density = DENSITY_hdpi;
        break;
    case Motorola_Droid:
        width = 854;
        height = 480;
        density = DENSITY_hdpi;
        break;
    case Galaxy_Y:
        width = 320;
        height = 240;
        density = DENSITY_ldpi;
        break;
    case Galaxy_Tab2_7:
        width = 1024;
        height = 600;
        bottom_spacing = 48;
        density = DENSITY_hdpi;
        break;
    case Galaxy_Tab2_10:
        width = 1280;
        height = 800;
        bottom_spacing = 48;
        break;
    case Google_Nexus_10:
        width = 2560;
        height = 1600;
        bottom_spacing = 112;
        density = DENSITY_xhdpi;
        break;
    case Sony_Xperia_Z:
        width = 1920;
        height = 1080;
        density = DENSITY_xxhdpi;
        break;
    case Microsoft_Surface_10:
        width = 1366;
        height = 768;
        break;
    case HD_720p:
        width = 1280;
        height = 720;
        break;
    case Kindle_Fire:
        width = 1280;
        height = 720;
        density = DENSITY_hdpi;
        break;
    }




    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    height-=bottom_spacing;
    eglView->setFrameSize(width, height);

    float zoom_factor = 1;
    if(zoom)
    {
        float needed_zoom_factor = MIN(max_width/width, max_height/height);
        zoom_factor = MIN(zoom_factor, needed_zoom_factor);
    }
    setDensity(density);
    eglView->setFrameZoomFactor(zoom_factor);
    return CCApplication::sharedApplication()->run();
}


