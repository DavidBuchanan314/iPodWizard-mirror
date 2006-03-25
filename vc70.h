

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sat Mar 25 17:48:34 2006
 */
/* Compiler settings for vc70.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __vc70_h__
#define __vc70_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IiPodManager_FWD_DEFINED__
#define __IiPodManager_FWD_DEFINED__
typedef interface IiPodManager IiPodManager;
#endif 	/* __IiPodManager_FWD_DEFINED__ */


#ifndef __IiPodPreferences_FWD_DEFINED__
#define __IiPodPreferences_FWD_DEFINED__
typedef interface IiPodPreferences IiPodPreferences;
#endif 	/* __IiPodPreferences_FWD_DEFINED__ */


#ifndef __IiPodUpdater_FWD_DEFINED__
#define __IiPodUpdater_FWD_DEFINED__
typedef interface IiPodUpdater IiPodUpdater;
#endif 	/* __IiPodUpdater_FWD_DEFINED__ */


#ifndef __IiPodUser_FWD_DEFINED__
#define __IiPodUser_FWD_DEFINED__
typedef interface IiPodUser IiPodUser;
#endif 	/* __IiPodUser_FWD_DEFINED__ */


#ifndef ___IiPodManagerEvents_FWD_DEFINED__
#define ___IiPodManagerEvents_FWD_DEFINED__
typedef interface _IiPodManagerEvents _IiPodManagerEvents;
#endif 	/* ___IiPodManagerEvents_FWD_DEFINED__ */


#ifndef ___IiPodUpdaterEvents_FWD_DEFINED__
#define ___IiPodUpdaterEvents_FWD_DEFINED__
typedef interface _IiPodUpdaterEvents _IiPodUpdaterEvents;
#endif 	/* ___IiPodUpdaterEvents_FWD_DEFINED__ */


#ifndef ____Impl__IiPodManagerEvents_FWD_DEFINED__
#define ____Impl__IiPodManagerEvents_FWD_DEFINED__

#ifdef __cplusplus
typedef class __Impl__IiPodManagerEvents __Impl__IiPodManagerEvents;
#else
typedef struct __Impl__IiPodManagerEvents __Impl__IiPodManagerEvents;
#endif /* __cplusplus */

#endif 	/* ____Impl__IiPodManagerEvents_FWD_DEFINED__ */


/* header files for imported files */
#include "docobj.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_vc70_0000 */
/* [local] */ 

typedef long IPODAPPID;

typedef long HIPOD;

typedef long HDB;

typedef long HTRACK;

typedef long HPLAYLIST;

typedef long HPLAYLISTITEM;

typedef long EDeviceStatus;

typedef long EDBNotification;

typedef long EUnlockNotification;

typedef long EFileCopyStatus;








enum EFormatState
    {	kFormatUnknown	= 0,
	kFormatFat32	= 1,
	kFormatMac	= 2,
	kFormatFat32NoFirmware	= 3,
	kFormatMacNoFirmware	= 4
    } ;

enum _EDeviceStatus
    {	kDeviceStatusUnknown	= 0,
	kDeviceStatusConnected	= 1,
	kDeviceStatusMounted	= 2,
	kDeviceStatusUnmounted	= 3,
	kDeviceStatusDisconnected	= 4
    } ;

enum _EDBNotification
    {	kDBNotifyUnknown	= 0,
	kDBNotifyDBChange	= 1,
	kDBNotifyTrackChanged	= 2,
	kDBNotifyPlaylistChanged	= 3,
	kDBNotifyPlaylistItemChanged	= 4
    } ;

enum _EUnlockNotification
    {	kAPIUnlocked	= 0,
	kiPodUnlocked	= 1,
	kiPodServiceShutdown	= 2
    } ;

enum _EFileCopyStatus
    {	kFileCopyInQueue	= 0,
	kFileCopyInProgress	= 1,
	kFileCopyComplete	= 2,
	kFileCopyCompleteDuplicate	= 3,
	kFileCopyCancel	= 4,
	kFileCopyError	= 5
    } ;

enum EiPodState
    {	kUnknowniPod	= 0,
	kNewiPod	= 1,
	kConfigured	= 4
    } ;

enum EAutomaticUpdateMode
    {	kUpdateUnknown	= 0,
	kUpdateAll	= 1,
	kUpdateSelectedPlaylists	= 2
    } ;

enum EiPodDBFilter
    {	kDBFilterOn	= 0,
	kDBFilterOff	= -1
    } ;

enum ECopyThreadStatus
    {	kCopyThreadStatusUnknown	= -1,
	kCopyThreadStatusWaiting	= 0,
	kCopyThreadStatusPumping	= 1
    } ;

enum EIPodClass
    {	kIPodClassNone	= 0,
	kIPodClassV1	= 1,
	kIPodClassV2	= 2,
	kIPodClassV3	= 4
    } ;

enum EiPodErrors
    {	kErrCopyThreadBusy	= -2147164159,
	kErriPodNotMounted	= -2147164059,
	kErrAppNotRegistered	= -2147164058,
	kErrAppNotLoggedIn	= -2147164057,
	kErrAPILock	= -2147164056,
	kErriPodLock	= -2147164055,
	kErriPodHandle	= -2147164054,
	kErriPodNotLocked	= -2147164053,
	kErrWrongServiceUser	= -2147164052,
	kErrAddTrackRejected	= -2147163959,
	kErrMountUnmount	= -2147163957
    } ;

enum EFormatFlags
    {	kFormatFlagsNone	= 0,
	kFormatFlagsOptimizeFATAlignment	= 1,
	kFormatFlagsGrowFirmwarePartition	= 2
    } ;

enum ETrackOrders
    {	kTrackOrderInvalid	= 0,
	kTrackOrderPhysical	= 1,
	kTrackOrderShuffle	= 2,
	kTrackOrderTrackID	= 3,
	kTrackOrderName	= 5,
	kTrackOrderAlbum	= 6,
	kTrackOrderArtist	= 7,
	kTrackOrderBitRate	= 8,
	kTrackOrderGenre	= 9,
	kTrackOrderKind	= 10,
	kTrackOrderDate	= 11,
	kTrackOrderTrackNumber	= 12,
	kTrackOrderTotalSize	= 13,
	kTrackOrderTotalTime	= 14,
	kTrackOrderYear	= 15,
	kTrackOrderSampleRate	= 16,
	kTrackOrderCategory	= 17,
	kTrackOrderLocation	= 18,
	kTrackOrderComment	= 20,
	kTrackOrderDateAdded	= 21,
	kTrackOrderComposer	= 23,
	kTrackOrderNormVolume	= 25,
	kTrackOrderUserPlayCount	= 26,
	kTrackOrderPlayDate	= 27,
	kTrackOrderDiscNumber	= 28,
	kTrackOrderUserRating	= 29
    } ;
struct TrackInformation
    {
    HTRACK HTRACK;
    long trackID;
    BSTR trackName;
    BSTR artist;
    BSTR album;
    BSTR genre;
    BSTR kind;
    BSTR comments;
    BSTR composer;
    BSTR category;
    long totalTimeInMS;
    long startTimeInMS;
    long stopTimeInMS;
    long bookmarkTimeInMS;
    long date;
    long sizeInBytes;
    long sampleRate;
    short bitRate;
    short year;
    short volumeAdjustment;
    short userRating;
    short audioFormat;
    short trackNumber;
    short maxTrackNumber;
    short discNumber;
    short maxDiscNumber;
    long userPlayCount;
    long recentPlayCount;
    long playDate;
    } ;
struct AddTrackStatus
    {
    enum ECopyThreadStatus status;
    long nTracksInQueue;
    long nTracksInQueueFromThisApp;
    unsigned long dwIntThreadBuffSize;
    CURRENCY liBytesToCopy;
    CURRENCY liEstDeviceSize;
    } ;


extern RPC_IF_HANDLE __MIDL_itf_vc70_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vc70_0000_v0_0_s_ifspec;


#ifndef __iPodEventReceiver_LIBRARY_DEFINED__
#define __iPodEventReceiver_LIBRARY_DEFINED__

/* library iPodEventReceiver */
/* [uuid][version] */ 


EXTERN_C const IID LIBID_iPodEventReceiver;

#ifndef __IiPodManager_DISPINTERFACE_DEFINED__
#define __IiPodManager_DISPINTERFACE_DEFINED__

/* dispinterface IiPodManager */
/* [uuid] */ 


EXTERN_C const IID DIID_IiPodManager;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("31e3fbaf-c200-4776-9dbf-45226765cf4d")
    IiPodManager : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IiPodManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IiPodManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IiPodManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IiPodManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IiPodManager * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IiPodManager * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IiPodManager * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IiPodManager * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IiPodManagerVtbl;

    interface IiPodManager
    {
        CONST_VTBL struct IiPodManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IiPodManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IiPodManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IiPodManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IiPodManager_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IiPodManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IiPodManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IiPodManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IiPodManager_DISPINTERFACE_DEFINED__ */


#ifndef __IiPodPreferences_DISPINTERFACE_DEFINED__
#define __IiPodPreferences_DISPINTERFACE_DEFINED__

/* dispinterface IiPodPreferences */
/* [uuid] */ 


EXTERN_C const IID DIID_IiPodPreferences;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3be5a207-40f6-4f25-bf37-27b898660b67")
    IiPodPreferences : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IiPodPreferencesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IiPodPreferences * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IiPodPreferences * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IiPodPreferences * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IiPodPreferences * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IiPodPreferences * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IiPodPreferences * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IiPodPreferences * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IiPodPreferencesVtbl;

    interface IiPodPreferences
    {
        CONST_VTBL struct IiPodPreferencesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IiPodPreferences_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IiPodPreferences_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IiPodPreferences_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IiPodPreferences_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IiPodPreferences_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IiPodPreferences_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IiPodPreferences_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IiPodPreferences_DISPINTERFACE_DEFINED__ */


#ifndef __IiPodUpdater_DISPINTERFACE_DEFINED__
#define __IiPodUpdater_DISPINTERFACE_DEFINED__

/* dispinterface IiPodUpdater */
/* [uuid] */ 


EXTERN_C const IID DIID_IiPodUpdater;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("42a63984-1154-4995-a75e-2e92f75ed902")
    IiPodUpdater : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IiPodUpdaterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IiPodUpdater * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IiPodUpdater * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IiPodUpdater * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IiPodUpdater * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IiPodUpdater * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IiPodUpdater * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IiPodUpdater * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IiPodUpdaterVtbl;

    interface IiPodUpdater
    {
        CONST_VTBL struct IiPodUpdaterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IiPodUpdater_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IiPodUpdater_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IiPodUpdater_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IiPodUpdater_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IiPodUpdater_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IiPodUpdater_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IiPodUpdater_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IiPodUpdater_DISPINTERFACE_DEFINED__ */


#ifndef __IiPodUser_DISPINTERFACE_DEFINED__
#define __IiPodUser_DISPINTERFACE_DEFINED__

/* dispinterface IiPodUser */
/* [uuid] */ 


EXTERN_C const IID DIID_IiPodUser;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("65e1d175-369d-412f-bbbf-a5e042c220e7")
    IiPodUser : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IiPodUserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IiPodUser * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IiPodUser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IiPodUser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IiPodUser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IiPodUser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IiPodUser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IiPodUser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IiPodUserVtbl;

    interface IiPodUser
    {
        CONST_VTBL struct IiPodUserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IiPodUser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IiPodUser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IiPodUser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IiPodUser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IiPodUser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IiPodUser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IiPodUser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IiPodUser_DISPINTERFACE_DEFINED__ */


#ifndef ___IiPodManagerEvents_DISPINTERFACE_DEFINED__
#define ___IiPodManagerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IiPodManagerEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IiPodManagerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4d56b972-8259-4833-9ab2-543962f5cf5b")
    _IiPodManagerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IiPodManagerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IiPodManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IiPodManagerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IiPodManagerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IiPodManagerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IiPodManagerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IiPodManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IiPodManagerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IiPodManagerEventsVtbl;

    interface _IiPodManagerEvents
    {
        CONST_VTBL struct _IiPodManagerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IiPodManagerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IiPodManagerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IiPodManagerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IiPodManagerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IiPodManagerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IiPodManagerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IiPodManagerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IiPodManagerEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IiPodUpdaterEvents_DISPINTERFACE_DEFINED__
#define ___IiPodUpdaterEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IiPodUpdaterEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IiPodUpdaterEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7a1aad4c-a417-439c-9318-d6bdc63fa46a")
    _IiPodUpdaterEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IiPodUpdaterEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IiPodUpdaterEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IiPodUpdaterEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IiPodUpdaterEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IiPodUpdaterEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IiPodUpdaterEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IiPodUpdaterEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IiPodUpdaterEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IiPodUpdaterEventsVtbl;

    interface _IiPodUpdaterEvents
    {
        CONST_VTBL struct _IiPodUpdaterEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IiPodUpdaterEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IiPodUpdaterEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IiPodUpdaterEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IiPodUpdaterEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IiPodUpdaterEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IiPodUpdaterEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IiPodUpdaterEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IiPodUpdaterEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID___Impl__IiPodManagerEvents;

#ifdef __cplusplus

class DECLSPEC_UUID("18FC7734-B4AC-3461-8C00-21067FEFD4D4")
__Impl__IiPodManagerEvents;
#endif
#endif /* __iPodEventReceiver_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


