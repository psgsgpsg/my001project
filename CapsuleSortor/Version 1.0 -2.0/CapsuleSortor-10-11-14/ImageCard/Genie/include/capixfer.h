//
// Coreco API - Transfer module functions.
//

#ifndef _CAPIXFER_H_
#define _CAPIXFER_H_

#if !defined(__MKTYPLIB__) && !defined(__midl)
#include <capidef.h>
#include <capiacqd.h>
#endif

// Capability definitions
// Transfer base capability value
#define CORXFER_CAP_FIRST			0
#define CORXFER_CAP( n, size)	    ((((n) + CORXFER_CAP_FIRST) << 16) | (size))

// Query Event capability
#define CORXFER_CAP_EVENT_TYPE					CORXFER_CAP(1, 4)
	#define CORXFER_VAL_EVENT_TYPE_START_OF_FIELD	0x00010000
	#define CORXFER_VAL_EVENT_TYPE_START_OF_ODD		0x00020000
	#define CORXFER_VAL_EVENT_TYPE_START_OF_EVEN		0x00040000
	#define CORXFER_VAL_EVENT_TYPE_START_OF_FRAME	0x00080000
	#define CORXFER_VAL_EVENT_TYPE_END_OF_FIELD		0x00100000
	#define CORXFER_VAL_EVENT_TYPE_END_OF_ODD			0x00200000
	#define CORXFER_VAL_EVENT_TYPE_END_OF_EVEN		0x00400000
	#define CORXFER_VAL_EVENT_TYPE_END_OF_FRAME		0x00800000
   #define CORXFER_VAL_EVENT_TYPE_END_OF_LINE      0x01000000
   #define CORXFER_VAL_EVENT_TYPE_END_OF_NLINES    0x02000000
	#define CORXFER_VAL_EVENT_TYPE_END_OF_TRANSFER	0x04000000
   #define CORXFER_VAL_EVENT_TYPE_LINE_UNDERRUN    0x08000000
   #define CORXFER_VAL_EVENT_TYPE_FIELD_UNDERRUN   0x10000000

	#define CORXFER_VAL_EVENT_TYPE_MASK					0xFFFF0000

// Query cropping capability
#define CORXFER_CAP_CROP_HORZ					CORXFER_CAP(2, 4)
#define CORXFER_CAP_CROP_LEFT_MIN			CORXFER_CAP(3, 4)
#define CORXFER_CAP_CROP_LEFT_MAX			CORXFER_CAP(4, 4)
#define CORXFER_CAP_CROP_LEFT_MULT			CORXFER_CAP(5, 4)
#define CORXFER_CAP_CROP_VERT					CORXFER_CAP(6, 4)
#define CORXFER_CAP_CROP_TOP_MIN				CORXFER_CAP(7, 4)
#define CORXFER_CAP_CROP_TOP_MAX				CORXFER_CAP(8, 4)
#define CORXFER_CAP_CROP_TOP_MULT			CORXFER_CAP(9, 4)
#define CORXFER_CAP_CROP_WIDTH_MIN			CORXFER_CAP(10, 4)
#define CORXFER_CAP_CROP_WIDTH_MAX			CORXFER_CAP(11, 4)
#define CORXFER_CAP_CROP_WIDTH_MULT			CORXFER_CAP(12, 4)
#define CORXFER_CAP_CROP_HEIGHT_MIN			CORXFER_CAP(13, 4)
#define CORXFER_CAP_CROP_HEIGHT_MAX			CORXFER_CAP(14, 4)
#define CORXFER_CAP_CROP_HEIGHT_MULT		CORXFER_CAP(15, 4)

// Query scaling capabilities
#define CORXFER_CAP_SCALE_HORZ_METHOD	 	CORXFER_CAP(16, 4)
#define CORXFER_CAP_SCALE_HORZ_MIN 			CORXFER_CAP(17, 4)
#define CORXFER_CAP_SCALE_HORZ_MAX			CORXFER_CAP(18, 4)
#define CORXFER_CAP_SCALE_HORZ_MULT			CORXFER_CAP(19, 4)
#define CORXFER_CAP_SCALE_HORZ_MIN_FACTOR	CORXFER_CAP(20, 4)
#define CORXFER_CAP_SCALE_HORZ_MAX_FACTOR	CORXFER_CAP(21, 4)
#define CORXFER_CAP_SCALE_VERT_METHOD		CORXFER_CAP(22, 4)
#define CORXFER_CAP_SCALE_VERT_MIN			CORXFER_CAP(23, 4)
#define CORXFER_CAP_SCALE_VERT_MAX			CORXFER_CAP(24, 4)
#define CORXFER_CAP_SCALE_VERT_MULT			CORXFER_CAP(25, 4)
#define CORXFER_CAP_SCALE_VERT_MIN_FACTOR	CORXFER_CAP(26, 4)
#define CORXFER_CAP_SCALE_VERT_MAX_FACTOR	CORXFER_CAP(27, 4)

#define CORXFER_CAP_COUNTER_STAMP_EVENT_TYPE	CORXFER_CAP(28, 4)

// Get size of transfers (ie. limitation of 4Mb for example)
#define CORXFER_CAP_MAX_XFER_SIZE			CORXFER_CAP(29, 4)

#define CORXFER_CAP_SCALE_HORZ      	 	CORXFER_CAP(30, 4)
#define CORXFER_CAP_SCALE_VERT      	 	CORXFER_CAP(31, 4)

#define CORXFER_CAP_FLIP      	 	      CORXFER_CAP(32, 4)

#define CORXFER_CAP_NB_INT_BUFFERS 	      CORXFER_CAP(33, 4)
	#define CORXFER_VAL_NB_INT_BUFFERS_NONE		0 // not available
	#define CORXFER_VAL_NB_INT_BUFFERS_MANUAL	1 // create by application
	#define CORXFER_VAL_NB_INT_BUFFERS_AUTO		2 // automaticaly created

#define CORXFER_CAP_EVENT_COUNT_SOURCE 	      CORXFER_CAP(34, 4)
	#define CORXFER_VAL_EVENT_COUNT_SOURCE_NONE		0 // not available
	#define CORXFER_VAL_EVENT_COUNT_SOURCE_DST		1 // dst event is counted
	#define CORXFER_VAL_EVENT_COUNT_SOURCE_SRC		2 // src event is counted

#define CORXFER_CAP_MAX_FRAME_COUNT  	      CORXFER_CAP(35, 4)

#define CORXFER_CAP_RELATIVE_BUFFER_INDEX          CORXFER_CAP(36, 4)
   #define CORXFER_VAL_RELATIVE_BUFFER_INDEX_NONE        0
   #define CORXFER_VAL_RELATIVE_BUFFER_INDEX_SUPPORTED   1

#define CORXFER_CAP_COUNTER_STAMP_AVAILABLE	      CORXFER_CAP(37, 4)
#define CORXFER_CAP_COUNTER_STAMP_TIME_BASE	      CORXFER_CAP(38, 4)
	#define CORXFER_VAL_TIME_BASE_US						0x00000001
	#define CORXFER_VAL_TIME_BASE_MS						0x00000002
	#define CORXFER_VAL_TIME_BASE_LINE					0x00000004	//line valid or HS
	#define CORXFER_VAL_TIME_BASE_LINE_TRIGGER		0x00000008  //External line trigger or shaft encoder pulse (after drop or/and multiply)
	#define CORXFER_VAL_TIME_BASE_FRAME					0x00000010	//frame valid or VS
	#define CORXFER_VAL_TIME_BASE_EXT_FRAME_TRIGGER	0x00000020	//ext frame trigger

#define CORXFER_CAP_COUNTER_STAMP_MAX			      CORXFER_CAP(39, 4)
#define CORXFER_CAP_CYCLE_MODE					      CORXFER_CAP(40, 4)
#define CORXFER_CAP_FLATFIELD 					      CORXFER_CAP(41, 4)
   #define CORXFER_VAL_FLATFIELD_NOT_SUPPORTED     0
   #define CORXFER_VAL_FLATFIELD_SUPPORTED         1

#define CORXFER_CAP_NB_INPUT_FRAMES                CORXFER_PRM(42, 4)
   #define CORXFER_VAL_NB_INPUT_FRAMES_NONE       0x00000000
   #define CORXFER_VAL_NB_INPUT_FRAMES_SUPPORTED  0x00000001
    
#define CORXFER_CAP_NB_OUTPUT_FRAMES         CORXFER_PRM(43, 4)
   #define CORXFER_VAL_NB_OUTPUT_FRAMES_NONE         0x00000000
   #define CORXFER_VAL_NB_OUTPUT_FRAMES_SUPPORTED     0x00000001

#define CORXFER_CAP_FLATFIELD_CYCLE_MODE           CORXFER_CAP(44, 4)
   #define CORXFER_VAL_FLATFIELD_CYCLE_MODE_NOT_SUPPORTED   0     //not supported
   #define CORXFER_VAL_FLATFIELD_CYCLE_MODE_OFF             1     //only 1 flatfield is used, see CORACQ_PRM_FLATFIELD_NUMBER
   #define CORXFER_VAL_FLATFIELD_CYCLE_MODE_AUTOMATIC       2     //cycle throught all flatfields created, prm CORXFER_PRM_FLATFIELD_NUMBER is then read-only
   #define CORXFER_VAL_FLATFIELD_CYCLE_MODE_MANUAL          4     //use the flatfield number specified with prm CORXFER_PRM_FLATFIELD_NUMBER

#define CORXFER_CAP_PROCESSING_MODE						CORXFER_CAP(45, 4)
   #define CORXFER_VAL_PROCESSING_MODE_NOT_SUPPORTED			0     //not supported
   #define CORXFER_VAL_PROCESSING_MODE_1							1		//user defined
   #define CORXFER_VAL_PROCESSING_MODE_2							2
   #define CORXFER_VAL_PROCESSING_MODE_3							4

/*------------------------------------------------------------------------*/
// Parameter definitions
// Transfer base parameter value
#define CORXFER_PRM_FIRST        0
#define CORXFER_PRM( n, size)	((((n) + CORXFER_PRM_FIRST) << 16) | (size))

// Get transfer status
#define CORXFER_PRM_STATUS				CORXFER_PRM( 1, 4)
// Stopped
#define CORXFER_VAL_STATUS_STOPPED		0x00000000
// In progress	
#define CORXFER_VAL_STATUS_ACTIVE		0x00000001
// Pending
#define CORXFER_VAL_STATUS_PENDING		0x00000002
// Stopped (possibly in the middle)
#define CORXFER_VAL_STATUS_ABORTED		0x00000003
	
// Get/Set cropping window
#define CORXFER_PRM_CROP_LEFT			CORXFER_PRM( 2, 4)
#define CORXFER_PRM_CROP_TOP			CORXFER_PRM( 3, 4)
#define CORXFER_PRM_CROP_WIDTH		CORXFER_PRM( 4, 4)
#define CORXFER_PRM_CROP_HEIGHT		CORXFER_PRM( 5, 4)

// Get/Set scaling window
#define CORXFER_PRM_SCALE_HORZ		CORXFER_PRM( 6, 4)
#define CORXFER_PRM_SCALE_VERT		CORXFER_PRM( 7, 4)

// Get/Set scaling method
#define CORXFER_PRM_SCALE_HORZ_METHOD 	CORXFER_PRM( 8, 4)
#define CORXFER_PRM_SCALE_VERT_METHOD 	CORXFER_PRM( 9, 4)
	#define CORXFER_VAL_SCALE_DISABLE			0x00000001
	#define CORXFER_VAL_SCALE_SIMPLE				0x00000002
	#define CORXFER_VAL_SCALE_INTERPOLATION	0x00000003
	#define CORXFER_VAL_SCALE_POW2				0x00000004

#define CORXFER_VAL_SCALE_FACTOR					0x00010000

// Get registered event
#define CORXFER_PRM_EVENT_TYPE				CORXFER_PRM( 10, 4)
#define CORXFER_PRM_EVENT_COUNT			   CORXFER_PRM( 11, 4)

// Get/Set start mode transfer
#define CORXFER_PRM_START_MODE		CORXFER_PRM( 12, 4)
	#define CORXFER_VAL_START_MODE_ASYNCHRONOUS   		0x00000000
	#define CORXFER_VAL_START_MODE_SYNCHRONOUS			0x00000001
	#define CORXFER_VAL_START_MODE_HALF_ASYNCHRONOUS	0x00000002
	#define CORXFER_VAL_START_MODE_SEQUENTIAL				0x00000003

#define CORXFER_PRM_TIMEOUT			CORXFER_PRM( 13, 4)

// Get/Set cycle mode transfer
#define CORXFER_PRM_CYCLE_MODE		CORXFER_PRM( 14, 4)
	#define CORXFER_VAL_CYCLE_MODE_ASYNCHRONOUS   				0x00000000
	#define CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS					0x00000001
	#define CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS_WITH_TRASH		0x00000002
	#define CORXFER_VAL_CYCLE_MODE_OFF							0x00000003
	#define CORXFER_VAL_CYCLE_MODE_NEXT_EMPTY					0x00000004
	#define CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS_NEXT_EMPTY_WITH_TRASH		0x00000005
	#define CORXFER_VAL_CYCLE_MODE_LAST							CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS_NEXT_EMPTY_WITH_TRASH
	// Cycle mode mask
	#define CORXFER_VAL_CYCLE_MODE_MASK							0x0000FFFF
	// BitField Xfer containt
	#define CORXFER_VAL_CYCLE_MODE_EXT_SIGNAL					0x00010000

	// For backward compatibility
	#define CORXFER_VAL_CYCLE_MODE_SYNCHRONUOUS_WITH_TRASH		CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS_WITH_TRASH
	#define CORXFER_VAL_CYCLE_MODE_SYNCH_WITH_TRASH_BUFFER		CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS_WITH_TRASH
	#define CORXFER_VAL_CYCLE_MODE_NEXT_BUFFER_WITH_TRASH		CORXFER_VAL_CYCLE_MODE_SYNCHRONOUS_NEXT_EMPTY_WITH_TRASH

// use this macro to check if a cycle mode is supported
// cap is the capability of the transfer level
// cycle mode is a CORXFER_VAL_CYCLE_MODE_XXXX value
#define CORXFER_IS_CYCLE_MODE_SUPPORTED(cap,cycleMode) (((cap)&(1 << ((cycleMode) & 31))) != 0)

#define CORXFER_PRM_EVENT_SERVER    CORXFER_PRM( 15, sizeof( CORSERVER))
#define CORXFER_PRM_EVENT_CALLBACK  CORXFER_PRM( 16, sizeof( PCORCALLBACK))
#define CORXFER_PRM_EVENT_CONTEXT   CORXFER_PRM( 17, sizeof( void *))

#define CORXFER_PRM_FLIP					CORXFER_PRM( 18, 4)
#define CORXFER_PRM_NB_INT_BUFFERS		CORXFER_PRM( 19, 4)
#define CORXFER_PRM_EVENT_COUNT_SOURCE	CORXFER_PRM( 20, 4)

#define CORXFER_PRM_RELATIVE_BUFFER_INDEX    CORXFER_PRM(21, 4)
#define CORXFER_PRM_COUNTER_STAMP_TIME_BASE  CORXFER_PRM(22, 4)
#define CORXFER_PRM_FLATFIELD_NUMBER			CORXFER_PRM(23, 4)

#define CORXFER_PRM_NB_INPUT_FRAMES          CORXFER_PRM(24, 4)
#define CORXFER_PRM_NB_OUTPUT_FRAMES         CORXFER_PRM(25, 4)

#define CORXFER_PRM_FLATFIELD_CYCLE_MODE     CORXFER_PRM(26, 4)
#define CORXFER_PRM_PROCESSING_MODE				CORXFER_PRM(27, 4)

/*------------------------------------------------------------------------*/
#define CORXFER_VAL_FRAME_INTERLACED		CORACQ_VAL_FRAME_INTERLACED
#define CORXFER_VAL_FRAME_NON_INTERLACED	CORACQ_VAL_FRAME_NON_INTERLACED

#define CORXFER_VAL_FIELD_ORDER_ODD_EVEN	 CORACQ_VAL_FIELD_ORDER_ODD_EVEN
#define CORXFER_VAL_FIELD_ORDER_EVEN_ODD	 CORACQ_VAL_FIELD_ORDER_EVEN_ODD
#define CORXFER_VAL_FIELD_ORDER_ANY_ORDER	 CORACQ_VAL_FIELD_ORDER_NEXT_FIELD
#define CORXFER_VAL_FIELD_ORDER_NEXT_FIELD CORACQ_VAL_FIELD_ORDER_NEXT_FIELD

#define CORXFER_VAL_CHANNELS_ORDER_NORMAL   CORACQ_VAL_CHANNELS_ORDER_NORMAL 
#define CORXFER_VAL_CHANNELS_ORDER_REVERSE  CORACQ_VAL_CHANNELS_ORDER_REVERSE
#define CORXFER_VAL_CHANNELS_ORDER_DETECT   CORACQ_VAL_CHANNELS_ORDER_DETECT 

#define CORXFER_VAL_FLIP_OFF              CORACQ_VAL_FLIP_OFF
#define CORXFER_VAL_FLIP_HORZ             CORACQ_VAL_FLIP_HORZ
#define CORXFER_VAL_FLIP_VERT             CORACQ_VAL_FLIP_VERT

// Transfer Port Index Mask
#define CORXFER_PORT_INDEX_MASK(portIndex)      (portIndex&0xFFFF)

// Transfer module definitions
// To start continuous transfer
#define CORXFER_CONTINUOUS 	( (UINT32) -1)

#if !defined(__MKTYPLIB__) && !defined(__midl)

#include "cpackd.h"

// Transfer description structure
typedef struct
{
	UINT32 frame;
	UINT32 fieldOrder;
	UINT32 widthByte;
	UINT32 height;
	UINT32 incByte;
} CORXFER_DESC, *PCORXFER_DESC;

#include "cunpackd.h"

#ifdef __cplusplus
extern "C" {
#endif

//*******************************************************************************************
//Application programming interface
// Reset a transfer module
CORSTATUS CORAPIFUNC CorXferResetModule(CORSERVER hServer);

// Create a transfer object
CORSTATUS CORAPIFUNC CorXferNew(CORSERVER hServer, CORHANDLE hSrc, CORHANDLE hDst, PCORXFER_DESC pDesc, CORXFER *hXfer);
CORSTATUS CORAPIFUNC CorXferNewEx(CORSERVER hServer, CORHANDLE hSrc, UINT32 srcPort, CORHANDLE hDst, UINT32 dstPort, PCORXFER_DESC pDesc, CORXFER *hXfer);

// Free a transfer object and its ressources
CORSTATUS CORAPIFUNC CorXferFree(CORXFER hXfer);

// Append item to transfer list
CORSTATUS CORAPIFUNC CorXferAppend(CORXFER hXfer, CORHANDLE hSrc, CORHANDLE hDst, PCORXFER_DESC pDesc);
CORSTATUS CORAPIFUNC CorXferAppendEx(CORXFER hXfer, CORHANDLE hSrc, UINT32 srcPort, CORHANDLE hDst, UINT32 dstPort, PCORXFER_DESC pDesc);

// Select item in transfer list
CORSTATUS CORAPIFUNC CorXferSelect(CORXFER hXfer, CORHANDLE hSrc, CORHANDLE hDst, UINT32 index);
CORSTATUS CORAPIFUNC CorXferSelectEx(CORXFER hXfer, CORHANDLE hSrc, UINT32 srcPort, CORHANDLE hDst, UINT32 dstPort, UINT32 index);

// Build transfer description list and lock ressources
CORSTATUS CORAPIFUNC CorXferConnect(CORXFER hXfer);
// Free ressources
CORSTATUS CORAPIFUNC CorXferDisconnect(CORXFER hXfer);

// Reset transfer object
CORSTATUS CORAPIFUNC CorXferReset(CORXFER hXfer);

// Query about transfer capabilities
CORSTATUS CORAPIFUNC CorXferGetCap(CORXFER hXfer, UINT32 cap, void *value);

// Get/set transfer parameters
CORSTATUS CORAPIFUNC CorXferGetPrm(CORXFER hXfer, UINT32 prm, void *value);
CORSTATUS CORAPIFUNC CorXferSetPrm(CORXFER hXfer, UINT32 prm, UINT32 value);
CORSTATUS CORAPIFUNC CorXferSetPrmEx(CORXFER hXfer, UINT32 prm, const void *value);

// Start transfer
// 0, ULONG_MAX-1		   : transfer 'count' frames
// CORXFER_CONTINUOUS ( -1): transfer in continuous mode
CORSTATUS CORAPIFUNC CorXferStart(CORXFER hXfer, UINT32 count);
// Stop transfer synchronously
CORSTATUS CORAPIFUNC CorXferStop(CORXFER hXfer);
// Stop transfer asynchronously
CORSTATUS CORAPIFUNC CorXferAbort(CORXFER hXfer);
// Wait until transfer finish or until timeout expiration
CORSTATUS CORAPIFUNC CorXferWait(CORXFER hXfer, UINT32 timeout);

// Register callback
CORSTATUS CORAPIFUNC CorXferRegisterCallback( CORXFER hXfer, UINT32 eventType, PCORCALLBACK callback, void *context);

// Unregister callback
CORSTATUS CORAPIFUNC CorXferUnregisterCallback( CORXFER hXfer, PCORCALLBACK callback);

#if !COR_WIN64
CORSTATUS CORAPIFUNC CorXferLinkCounterStamp(CORXFER hXfer, CORCOUNTER hCounter, UINT32 eventType);
CORSTATUS CORAPIFUNC CorXferUnlinkCounterStamp(CORXFER hXfer, CORCOUNTER hCounter);
#endif   // !COR_WIN64

#ifdef __cplusplus
}
#endif

#endif // !(defined(__MKTYPLIB__) && !defined(__midl))

#endif   // _CAPIXFER_H_
