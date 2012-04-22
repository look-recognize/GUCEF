/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef GUCEF_CORE_CX11WINDOW_H
#define GUCEF_CORE_CX11WINDOW_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"  /* module config macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

#ifdef GUCEF_LINUX_BUILD

#include <X11/Xlib.h>

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Class with basic utilities for creating a X11 window
 */
class GUCEF_CORE_PUBLIC_CPP CX11Window : public CObservingNotifier
{
    public:

    static const CEvent WindowCloseEvent;
    static const CEvent WindowDestroyEvent;
    static const CEvent WindowActivationEvent;
    static const CEvent WindowResizeEvent;
    static const CEvent WindowPaintEvent;

    static void RegisterEvents( void );

    public:

    CX11Window();

    virtual ~CX11Window();

    void Close( void );

    virtual void SetText( const CString& text );

    virtual CString GetText( void ) const;

    void SetDisplay( ::Display* display );

    ::Display* GetDisplay( void ) const;

    void SetWindow( ::Window window );

    ::Window GetWindow( void ) const;

    void SetScreen( int screenNr );

    int GetScreen( void ) const;

    ::Window GetParentWindow( void ) const;

    virtual const CString& GetClassTypeName( void ) const;

    virtual bool WindowCreate( const Int32 xPosition     ,
                               const Int32 yPosition     ,
                               const UInt32 width        ,
                               const UInt32 height       ,
                               ::Window parentWindow = 0 );

    void WindowDestroy( void );

    bool Show( void );

    bool Hide( void );

    bool Maximize( void );

    bool Minimize( void );

    void Repaint( void );

    void SendToForegound( void );

    void GrabFocus( void );

    int GetWidthOfClientAreaBorder( void ) const;

    bool SetWindowAreaPosition( int x, int y );

    bool GetWindowAreaPosition( int& x, int& y ) const;

    bool GetClientAreaPosition( int& x, int& y ) const;

    bool GetWindowArea( int& x, int& y, int& width, int& height ) const;

    bool GetWindowAreaRelativeToParentClientArea( Float32& x      ,
                                                  Float32& y      ,
                                                  Float32& width  ,
                                                  Float32& height ) const;

    bool GetClientArea( int& x, int& y, int& width, int& height ) const;

    void WindowAreaResize( int x, int y, int width, int height, bool repaint = true );

    void WindowAreaResize( int width, int height, bool repaint = true );

    void ClientAreaResize( int x, int y, int width, int height, bool repaint = true );

    void ClientAreaResize( int width, int height, bool repaint = true );

    void WindowAreaResizeRelativeToParentClientArea( Float32 x           ,
                                                     Float32 y           ,
                                                     Float32 width       ,
                                                     Float32 height      ,
                                                     bool repaint = true );

    void ClientAreaResizeRelativeToParentClientArea( Float32 x           ,
                                                     Float32 y           ,
                                                     Float32 width       ,
                                                     Float32 height      ,
                                                     bool repaint = true );

    protected:

    virtual void OnNotify( CNotifier* notifier           ,
                           const CEvent& eventid         ,
                           CICloneable* eventdata = NULL );

    private:

    CX11Window( const CX11Window& src );            /**< cannot be copied */
    CX11Window& operator=( const CX11Window& src ); /**< cannot be copied */

    void OnX11Event( const ::XEvent& event );

    private:

    ::Display* m_display;
    int m_screenNr;
    ::Window m_window;
    ::Atom m_wmDeleteWindow;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_LINUX_BUILD ? */

#endif /* GUCEF_CORE_CX11WINDOW_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 22-04-2003 :
        - Initial implementation

---------------------------------------------------------------------------*/
