//
// Coreco standard library.
//

#ifndef _CORLIB_H_
#define _CORLIB_H_

#include <cordef.h>

// Macros that replace some C run-time string functions.
// By default, these macros resolve to the regular run-time functions.
// See COR_WIN32 below for redefinitions.
#define CorSnprintf  _snprintf
#define CorVsnprintf _vsnprintf
#define CorSscanf    sscanf
#define CorStrncpy   strncpy
#define CorStricmp   stricmp
#define CorStrncat   strncat


#if COR_NTKERNEL
	#define CorMalloc( nBytes)				ExAllocatePool( PagedPool, nBytes)
	#define CorFree( p)						ExFreePool(p)
	#define CorCalloc( nItem, nBytes)   CorMalloc( (nItem) * (nBytes))
	#define CorRealloc( p, nBytes)      Not implemented
   #define CorMemcpy(d,s,c)            CorManMemcpy( d, s, c, FALSE)
   #define CorMemcpyAlignSrc(d,s,c)    CorManMemcpy( d, s, c, TRUE)
#elif COR_95KERNEL
	#define CorMalloc( nBytes)				malloc( nBytes)
	#define CorFree(p)						free( p)
	#define CorCalloc( nItem, nBytes)   calloc( nItem, nBytes)
	#define CorRealloc( p, nBytes)      realloc( p, nBytes)
   #define CorMemcpy(d,s,c)            CorManMemcpy( d, s, c, FALSE)
   #define CorMemcpyAlignSrc(d,s,c)    CorManMemcpy( d, s, c, TRUE)
#elif COR_LINUX && defined(__KERNEL__)
	#include <GDrv_kernel.h>
	#define CorMalloc( nBytes)				vmalloc( nBytes)
	#define CorFree(p)						vfree( p)
	#define CorCalloc( nItem, nBytes)   CorMalloc( (nItem) * (nBytes))
	#define CorRealloc( p, nBytes)		Not implemented
   #define CorMemcpy(d,s,c)            CorManMemcpy( d, s, c, FALSE)
   #define CorMemcpyAlignSrc(d,s,c)    CorManMemcpy( d, s, c, TRUE)
#else
	#include <assert.h>

	#ifndef ASSERT
		#define ASSERT(a) assert((int)a)
	#endif

	#ifdef NOSTANDARD_ALLOC
		void *CorMalloc(UINT32 size);
		void CorFree(void *memblock);
	#else
      #include <stdlib.h>
		#if COR_WIN32
         #define CorMalloc(nBytes)        HeapAlloc(GetProcessHeap(), 0, nBytes)
         #define CorFree(p)               HeapFree(GetProcessHeap(), 0,  p)
         #define CorCalloc(nItem, nBytes) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (nItem) * (nBytes))
         #define CorRealloc(p, nBytes)    HeapRealloc(GetProcessHeap(), 0, p, nBytes)
		#else
			#define CorMalloc( nBytes)				malloc( nBytes)
			#define CorFree(p)						free( p)
			#define CorCalloc( nItem, nBytes)   calloc( nItem, nBytes)
			#define CorRealloc( p, nBytes)		realloc( p, nBytes)
		#endif
	#endif

	#if COR_WIN32
		#define CorSignalEvent(event)		SetEvent(event)
      #define CorMemcpy(d,s,c)         CorManMemcpy( d, s, c, FALSE)
      #define CorMemcpyAlignSrc(d,s,c) CorManMemcpy( d, s, c, TRUE)

      // With Visual Studio 2005, some regular string functions are marked as deprecated by the compiler.
      // In this case, the macros call the safer/more robust string functions under Win32 and Win64.
      #if defined(_MSC_VER) && _MSC_VER >= 1400
         #include <strsafe.h>

         #undef  CorSnprintf
         #define CorSnprintf                    StringCbPrintf

         #undef  CorVsnprintf
         #define CorVsnprintf                   StringCbVPrintf

         #undef  CorSscanf
         #define CorSscanf                      sscanf_s

         #undef  CorStrncpy
         #define CorStrncpy(dest, src, count)   StringCbCopy((dest), (count), (src))

         #undef  CorStricmp
         #define CorStricmp                     _stricmp

         #undef  CorStrncat
         #define CorStrncat(str1, str2, count)  StringCbCat((str1), (count), (str2))
      #endif
	#endif
	#if COR_C60
		#define CorSignalEvent(event)		CorManSetEvent(event)//_event_set((pointer)event,1)
      #define CorMemcpy(d,s,c)         memcpy( d, s, c, FALSE)
      #define CorMemcpyAlignSrc(d,s,c) memcpy( d, s, c)
	#endif
	#if COR_I960
		#define EVENT_MAGIC_NUMBER 0xCAFECAFE
		#define CorSignalEvent(event)    *(PUINT32)(event)= EVENT_MAGIC_NUMBER
      #define CorMemcpy(d,s,c)         memcpy( d, s, c)
      #define CorMemcpyAlignSrc(d,s,c) memcpy( d, s, c)
	#endif
	#if COR_IOP321
		#define EVENT_MAGIC_NUMBER 0xA55AA55A
		#define CorSignalEvent(event)    *(PUINT32)(event)= EVENT_MAGIC_NUMBER
      #define CorMemcpy(d,s,c)         memcpy( d, s, c)
      #define CorMemcpyAlignSrc(d,s,c) memcpy( d, s, c)
	#endif

#endif

#endif   // _CORLIB_H_
