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

#ifndef GUCEF_CORE_CTSHAREDPTR_H
#define GUCEF_CORE_CTSHAREDPTR_H
 
/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <assert.h>

#ifndef GUCEF_CORE_CTBASICSHAREDPTR_H
#include "CTBasicSharedPtr.h"
#define GUCEF_CORE_CTBASICSHAREDPTR_H
#endif /* GUCEF_CORE_CTBASICSHAREDPTR_H ? */

#ifndef GUCEF_CORE_CTDYNAMICDESTRUCTOR_H
#include "CTDynamicDestructor.h"
#define GUCEF_CORE_CTDYNAMICDESTRUCTOR_H
#endif /* GUCEF_CORE_CTDYNAMICDESTRUCTOR_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF { 
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TEMPLATES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/**
 *  Templated implementation of a shared pointer.
 *
 *  A shared pointer adds some safety to pointers that get passed
 *  around where objects keep a reference to object instances independent
 *  of each other. In such situations it can become unclear who is responsible 
 *  for destroying the referenced object instance.
 *  The shared pointer will reference count the pointer and ensure that even 
 *  though the original owner has no knowledge of other classes referencing the 
 *  object it can safely manage object destruction as desired.
 *
 *  Note that the default behavior of this template is to delete the referenced
 *  object instance when the reference count hits zero. This raises some 
 *  "out-of-scope-memory-management" concerns. While this can be the intended 
 *  behavior, it is advised to alter this behavior to provide an in-scope mechanism
 *  ( ie delegating destruction responsibilities to the original creator of the object ).
 *
 *      Method 1:
 *          inherit from this template and redirect instance destruction
 *          This is achieved by overriding the destructor.
 *
 *      Method 2:
 *          Make the owner of the referenced object inherit from CTSharedObjectDestructor
 *          for the given type. Pass the owner in when creating the pointer object.
 *          This will result in in-scope memory management.
 *
 *  Note that this shared pointer implementation is by no means threadsafe and should 
 *  never be used across thread-boundries.
 */
template< typename T >
class CTSharedPtr : public CTBasicSharedPtr< T >
{
    public:   

    typedef CTBasicSharedPtr< T >::TDestructor TDestructor;
        
    /**
     *  Constructor that allows you to create an 'uninitialized' pointer.
     *  You will have to assign a valid shared pointer at a later time 
     *  to initialize the shared pointer.
     *
     *  Note that if you attempt to use the shared pointer before it
     *  is initialized an exception will be thrown
     *
     */
    CTSharedPtr( void );
    
    CTSharedPtr( const int NULLvalue );
    
    CTSharedPtr( T* ptr                               ,
                 TDestructor* objectDestructor = NULL );
 
    CTSharedPtr( T& ptr                               ,
                 TDestructor* objectDestructor = NULL );

    /**
     *  Conversion constructor making it possible to pass
     *  a CTBasicSharedPtr to a function which requires a
     *  CTSharedPtr thanks to an implicit cast using this constructor.
     */
    explicit CTSharedPtr( const CTBasicSharedPtr< T >& src );    

    // inlined copy constructor, has to be inlined in class definition for now due to VC6 limitations
    template< class Derived >
    CTSharedPtr( const CTSharedPtr< Derived >& src )
        : CTBasicSharedPtr< T >( reinterpret_cast< const CTSharedPtr& >( src ) )
    {GUCEF_TRACE;
       
        // regarding the initializer list:
        //   We use reinterpret_cast to make use of the automatic same-class-friend-relationship
        //   which allows us to access the data members of the given src.

        // Cast performed for safety reasons:
        // Allows us to catch unlawful casts compile time.
        // If you get an error here then 'Derived' is not actually in the same 
        // inheritance chain as 'T'
        Derived* testPtrDerived = NULL;
        T* testPtrT = static_cast< Derived* >( testPtrDerived );
        testPtrT = NULL;
    }
    
    CTSharedPtr( const CTSharedPtr& src );

    virtual ~CTSharedPtr();    

    // implemented inline as a workaround for VC6 issues
    template< class Derived >
    CTSharedPtr& operator=( const CTSharedPtr< Derived >& src )
    {GUCEF_TRACE;
    
        if ( &reinterpret_cast< const CTSharedPtr& >( src ) != this )
        {
            // Cast performed for safety reasons:
            // Allows us to catch unlawful casts compile time.
            // If you get an error here then 'Derived' is not actually in the same 
            // inheritance chain as 'T'
            Derived* testPtrDerived( NULL );
            T* testPtrT( static_cast< Derived* >( testPtrDerived ) );
            testPtrT = NULL;
            
            CTBasicSharedPtr< T >::operator=( reinterpret_cast< const CTSharedPtr& >( src ) );
        }
        return *this;
    }

    CTSharedPtr& operator=( const CTSharedPtr& src );

    bool operator==( const void* other ) const;
    
    bool operator==( const CTSharedPtr& other ) const;
 
    bool operator!=( const void* other ) const;
    
    bool operator!=( const CTSharedPtr& other ) const;
    
    bool operator<( const CTSharedPtr& other ) const;
 
    /**
     *  operator that implements '(*mySharedPtr)'
     *
     *  @throws ENotInitialized if the pointer is not initialized 
     */
    inline T& operator*( void );
    
    /**
     *  operator that implements '(*mySharedPtr)'
     *
     *  @throws ENotInitialized if the pointer is not initialized 
     */
    inline const T& operator*( void ) const;

    /**
     *  operator that emulates 'myPointer->'
     *
     *  @throws ENotInitialized if the pointer is not initialized
     */
    inline T* operator->( void );

    /**
     *  operator that emulates 'myPointer->'
     *
     *  @throws ENotInitialized if the pointer is not initialized
     */
    inline const T* operator->( void ) const;
    
    // implemented inline as a workaround for VC6 issues
    // The dummy param is a VC6 hack for templated member functions
    template< class Derived >
    CTSharedPtr< Derived >
    StaticCast( bool dummy = true )
    {
        // The actual compile time check is the line of code below.
        // You should get a casting error if 'Derived' is not a type that is
        // derived from 'T'. After we made this check we can just reinterpret
        // this object to be able to give out more pointer references as 'Derived'
        // Note that we have to use the 'derivedClassPtr' variable otherwise the compiler
        // will optimize out the code which is why the code is structured this way. 
        Derived* derivedClassPtr = static_cast< Derived* >( GetPointer() );
        if ( 0 != derivedClassPtr )
        {
            return reinterpret_cast< CTSharedPtr< Derived >& >( *this );
        }
        return CTSharedPtr< Derived >();
    }
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( void )
        : CTBasicSharedPtr< T >()
{GUCEF_TRACE;
    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( const int NULLvalue )
    : CTBasicSharedPtr< T >()
{GUCEF_TRACE;

    // Note that if this constructor is used an assignment is required at
    // a later time to initialize the shared pointer
    assert( NULLvalue == NULL );    
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( T* ptr                                     ,
                               TDestructor* objectDestructor /* = NULL */ )
        : CTBasicSharedPtr< T >()
{GUCEF_TRACE;
    
    if ( NULL != objectDestructor )
    {
        Initialize( ptr, objectDestructor );
    }
    else
    {
        Initialize( ptr, new CTDynamicDestructor< T >( true ) );
    }
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( T& ptr                                     ,
                               TDestructor* objectDestructor /* = NULL */ )
        : CTBasicSharedPtr< T >() ,
          m_localDestructor()    
{GUCEF_TRACE;

    if ( NULL != objectDestructor )
    {
        Initialize( &ptr, objectDestructor );
    }
    else
    {
        Initialize( &ptr, new CTDynamicDestructor< T >( true ) );
    }
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( const CTSharedPtr< T >& src )
        : CTBasicSharedPtr< T >( src )
{GUCEF_TRACE;
       
}

/*-------------------------------------------------------------------------*/

template< typename T >
CTSharedPtr< T >::CTSharedPtr( const CTBasicSharedPtr< T >& src )
    : CTBasicSharedPtr< T >( src )
{GUCEF_TRACE;
    
}

/*-------------------------------------------------------------------------*/
   
template< typename T >
CTSharedPtr< T >::~CTSharedPtr()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/
  
template< typename T >
CTSharedPtr< T >& 
CTSharedPtr< T >::operator=( const CTSharedPtr< T >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CTBasicSharedPtr< T >::operator=( src );
    }
    return *this;
}

/*-------------------------------------------------------------------------*/
/*
template< class Derived >
CTSharedPtr< T >&
CTSharedPtr< T >::operator=( const CTSharedPtr< Derived >& src )
{GUCEF_TRACE;

    if ( this != &src )
    {
        CTSharedPtr< T > basePtr( static_cast );
        
        CTBasicSharedPtr< T >::operator=( src );
    }
    return *this;    
}
*/

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTSharedPtr< T >::operator<( const CTSharedPtr< T >& other ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >& >( *this ) < static_cast< const CTBasicSharedPtr< T >& >( other );
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTSharedPtr< T >::operator==( const CTSharedPtr< T >& other ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >& >( *this ) == static_cast< const CTBasicSharedPtr< T >& >( other );
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTSharedPtr< T >::operator==( const void* other ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >& >( *this ) == other;
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
operator==( const void* ptr, const CTSharedPtr< T >& other )
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >& >( other ) == ptr;
}

/*-------------------------------------------------------------------------*/

/*
 *  workaround for comparison to NULL, since NULL is a integer by default 
 */
template< typename T >
inline bool 
operator==( const int ptr, const CTSharedPtr< T >& other )
{GUCEF_TRACE;

    return static_cast< const T* >( NULL ) == static_cast< const CTBasicSharedPtr< T >& >( other );
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline bool 
CTSharedPtr< T >::operator!=( const CTSharedPtr< T >& other ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >& >( *this ) != static_cast< const CTBasicSharedPtr< T >& >( other );    
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
CTSharedPtr< T >::operator!=( const void* other ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >& >( *this ) != other;    
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline bool 
operator!=( const void* ptr, const CTSharedPtr< T >& other )
{GUCEF_TRACE;

    return other != ptr;
}

/*-------------------------------------------------------------------------*/

/*
 *  workaround for comparison to NULL, since NULL is a integer by default 
 */
template< typename T >
inline bool 
operator!=( const int ptr, const CTSharedPtr< T >& other )
{GUCEF_TRACE;

    return ptr != static_cast< const CTBasicSharedPtr< T >& >( other);
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline T& 
CTSharedPtr< T >::operator*( void )
{GUCEF_TRACE;

    return static_cast< CTBasicSharedPtr< T >* >( this )->operator*();
}

/*-------------------------------------------------------------------------*/

template< typename T >
inline const T& 
CTSharedPtr< T >::operator*( void ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >* >( this )->operator*();
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline T*
CTSharedPtr< T >::operator->( void )
{GUCEF_TRACE;

    return static_cast< CTBasicSharedPtr< T >* >( this )->operator->();
}

/*-------------------------------------------------------------------------*/
    
template< typename T >
inline const T* 
CTSharedPtr< T >::operator->( void ) const
{GUCEF_TRACE;

    return static_cast< const CTBasicSharedPtr< T >* >( this )->operator->();  
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 24-01-2007 :
        - Dinand: Fixed and cleaned up the object destruction mechanics
          Instead of using a local object to delete the shared object we now use
          a self-deleting destructor delegator. This allows us to copy the destructor
          pointer safely in all scenario's since it's guaranteed to be kept alive as
          long as the reference count does not hit zero. This could actually cause 
          nasty crashes in the previous implementation because the local destructor
          mechanism broke down when a CTSharedPtr got converted into a CTBasicSharedPtr
          which could result in an invalid destructor object being invoked.
          This fix has the desirable side-effect of simplifying the code.
- 14-12-2005 :
        - Dinand: Moved code into this new base class from CTSharedPtr to allow
          some level of shared pointer usage without actually requiring the type
          to be defined. ie you can create a shared pointer with a forward declaration.

-----------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CTSHAREDPTR_H ? */
