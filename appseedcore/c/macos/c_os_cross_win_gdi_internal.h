#pragma once



#include <X11/Xlib.h>
#include <X11/Xutil.h>



typedef Window HWND;

class CLASS_DECL_____ device_context
{
public:
    
    Display *   m_display;
    Drawable    m_d;
    GC          m_gc;
    HWND        m_hwnd;
    
    
    device_context()
    {
        m_display   = NULL;
        m_d         = 0;
        m_gc        = NULL;
        m_hwnd      = NULL;
    }
    
};

