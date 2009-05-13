/*
 *  gucefGUI: GUCEF module providing a uniform interface towards GUI backends
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_GUI_CGUIDRIVER_H
#define GUCEF_GUI_CGUIDRIVER_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_COBSERVINGNOTIFIER_H
#include "CObservingNotifier.h"
#define GUCEF_CORE_COBSERVINGNOTIFIER_H
#endif /* GUCEF_CORE_COBSERVINGNOTIFIER_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_GUI_MACROS_H
#include "gucefGUI_macros.h"
#define GUCEF_GUI_MACROS_H
#endif /* GUCEF_GUI_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace GUI {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class CIGUIContext;

/*-------------------------------------------------------------------------*/

class GUCEF_GUI_EXPORT_CPP CGUIDriver : public CORE::CObservingNotifier ,
                                        public CORE::CIConfigurable
{    
    public:

    static const CORE::CEvent WidgetTypeRegisteredEvent;
    static const CORE::CEvent WidgetTypeUnregisteredEvent;    
    static const CORE::CEvent FormTypeRegisteredEvent;
    static const CORE::CEvent FormTypeUnregisteredEvent;

    public:
    
    typedef std::set< CString > TStringSet;
    typedef std::set< CIGUIContext* > TGUIContextSet;

    virtual ~CGUIDriver();
    
    virtual CIGUIContext* CreateGUIContext() = 0;
    
    virtual void DestroyGUIContext( CIGUIContext* context ) = 0;   
    
    virtual TGUIContextSet GetContextList( void ) = 0;
    
    virtual UInt32 GetContextCount( void ) = 0;
    
    virtual CString GetDriverName( void ) = 0;
    
    virtual TStringSet GetAvailableFormTypes( void ) = 0;
    
    virtual TStringSet GetAvailableWidgetTypes( void ) = 0;
    
    virtual const CString& GetClassTypeName( void ) const;
    
    protected:
    
    CGUIDriver( void );
    
    CGUIDriver( const CGUIDriver& src );
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace GUI */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
          
#endif /* GUCEF_GUI_CGUIDRIVER_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Dinand: Initial implementation

-----------------------------------------------------------------------------*/