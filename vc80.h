

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Fri Oct 06 22:34:06 2006
 */
/* Compiler settings for vc80.idl:
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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __vc80_h__
#define __vc80_h__

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


#ifndef __IItunesServiceCallback_FWD_DEFINED__
#define __IItunesServiceCallback_FWD_DEFINED__
typedef interface IItunesServiceCallback IItunesServiceCallback;
#endif 	/* __IItunesServiceCallback_FWD_DEFINED__ */


#ifndef __IItunesDevicePrefs_FWD_DEFINED__
#define __IItunesDevicePrefs_FWD_DEFINED__
typedef interface IItunesDevicePrefs IItunesDevicePrefs;
#endif 	/* __IItunesDevicePrefs_FWD_DEFINED__ */


#ifndef __IItunesDevices_FWD_DEFINED__
#define __IItunesDevices_FWD_DEFINED__
typedef interface IItunesDevices IItunesDevices;
#endif 	/* __IItunesDevices_FWD_DEFINED__ */


#ifndef __IItunesHelper_FWD_DEFINED__
#define __IItunesHelper_FWD_DEFINED__
typedef interface IItunesHelper IItunesHelper;
#endif 	/* __IItunesHelper_FWD_DEFINED__ */


#ifndef __IItunesService_FWD_DEFINED__
#define __IItunesService_FWD_DEFINED__
typedef interface IItunesService IItunesService;
#endif 	/* __IItunesService_FWD_DEFINED__ */


#ifndef ___IiPodUpdaterEvents_FWD_DEFINED__
#define ___IiPodUpdaterEvents_FWD_DEFINED__
typedef interface _IiPodUpdaterEvents _IiPodUpdaterEvents;
#endif 	/* ___IiPodUpdaterEvents_FWD_DEFINED__ */


#ifndef ___IiPodManagerEvents_FWD_DEFINED__
#define ___IiPodManagerEvents_FWD_DEFINED__
typedef interface _IiPodManagerEvents _IiPodManagerEvents;
#endif 	/* ___IiPodManagerEvents_FWD_DEFINED__ */


/* header files for imported files */
#include "docobj.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_vc80_0000 */
/* [local] */ 











typedef long IPODAPPID;

typedef long HIPOD;


typedef /* [public] */ enum __MIDL___MIDL_itf_iPodService_0000_0001 EFormatState;

typedef long HDB;

typedef long EFileCopyStatus;

typedef long EDBNotification;

typedef long EDeviceStatus;

typedef long EUnlockNotification;


typedef /* [public][public] */ enum __MIDL___MIDL_itf_iPodService_0000_0002 _EDeviceStatus;


enum __MIDL___MIDL_itf_iPodService_0000_0001
    {	kFormatUnknown	= 0,
	kFormatFat32	= 1,
	kFormatMac	= 2,
	kFormatFat32NoFirmware	= 3,
	kFormatMacNoFirmware	= 4
    } ;

enum EIPodClass
    {	kIPodClassNone	= 0,
	kIPodClassV1	= 1,
	kIPodClassV2	= 2,
	kIPodClassV3	= 4,
	kIPodClassALL	= 65535
    } ;


extern RPC_IF_HANDLE __MIDL_itf_vc80_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vc80_0000_v0_0_s_ifspec;

#ifndef __IiPodManager_INTERFACE_DEFINED__
#define __IiPodManager_INTERFACE_DEFINED__

/* interface IiPodManager */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IiPodManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("31e3fbaf-c200-4776-9dbf-45226765cf4d")
    IiPodManager : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterApp( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ BSTR appName,
            /* [in] */ BSTR appPath,
            /* [in] */ BSTR cmdLine) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterApp( 
            /* [in] */ IPODAPPID appID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetRegisteredApps( 
            /* [retval][out] */ SAFEARRAY * *pArrayAppID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAppNameFromID( 
            /* [in] */ IPODAPPID appID,
            /* [retval][out] */ BSTR *pAppName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAPILockingApp( 
            /* [retval][out] */ IPODAPPID *pAppID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [in] */ IPODAPPID appID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Logout( 
            /* [in] */ IPODAPPID appID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE EnableSync( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DisableSync( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BeginTransfer( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE EndTransfer( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LockiPod( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnlockiPod( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetiPodLockingApp( 
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ IPODAPPID *pAppID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindFirstiPod( 
            /* [in] */ IPODAPPID appID,
            /* [retval][out] */ HIPOD *piPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindNextiPod( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD pCuriPodHandle,
            /* [retval][out] */ HIPOD *piPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Mount( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unmount( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsMounted( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *bIsMounted) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetiPodFreeSpace( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ CURRENCY *pSpace) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDiskSize( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ CURRENCY *pSize) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetiPodDriveLetter( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ int format,
            /* [retval][out] */ BSTR *driveLetter) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LaunchUpdater( 
            /* [in] */ IPODAPPID callerAppID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVersionInfo( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [out] */ BSTR *firmwareVersion,
            /* [retval][out] */ BSTR *pVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAPIVersion( 
            /* [retval][out] */ long *pVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsFormatted( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ EFormatState *IsFormatted) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetVoiceNotesFolderName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pFolderName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDeviceUniqueID( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pUniqueID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE BeginTransferAsync( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE EndTransferAsync( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFirmwareVersion( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ unsigned short *firmwareVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCheckpointData( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ SAFEARRAY * *checkpointData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSupportedIPodClasses( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ enum EIPodClass supportedIPodClasses) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSupportedIPodClasses( 
            /* [in] */ IPODAPPID appID,
            /* [retval][out] */ enum EIPodClass *supportedIPodClasses) = 0;
        
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterApp )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ BSTR appName,
            /* [in] */ BSTR appPath,
            /* [in] */ BSTR cmdLine);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterApp )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetRegisteredApps )( 
            IiPodManager * This,
            /* [retval][out] */ SAFEARRAY * *pArrayAppID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAppNameFromID )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [retval][out] */ BSTR *pAppName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAPILockingApp )( 
            IiPodManager * This,
            /* [retval][out] */ IPODAPPID *pAppID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Login )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Logout )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *EnableSync )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DisableSync )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BeginTransfer )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *EndTransfer )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LockiPod )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnlockiPod )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetiPodLockingApp )( 
            IiPodManager * This,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ IPODAPPID *pAppID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindFirstiPod )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [retval][out] */ HIPOD *piPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindNextiPod )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD pCuriPodHandle,
            /* [retval][out] */ HIPOD *piPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Mount )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unmount )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsMounted )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *bIsMounted);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetiPodFreeSpace )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ CURRENCY *pSpace);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDiskSize )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ CURRENCY *pSize);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetiPodDriveLetter )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ int format,
            /* [retval][out] */ BSTR *driveLetter);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LaunchUpdater )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID callerAppID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVersionInfo )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [out] */ BSTR *firmwareVersion,
            /* [retval][out] */ BSTR *pVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAPIVersion )( 
            IiPodManager * This,
            /* [retval][out] */ long *pVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsFormatted )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ EFormatState *IsFormatted);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetVoiceNotesFolderName )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pFolderName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDeviceUniqueID )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pUniqueID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *BeginTransferAsync )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *EndTransferAsync )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFirmwareVersion )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ unsigned short *firmwareVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCheckpointData )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ SAFEARRAY * *checkpointData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSupportedIPodClasses )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ enum EIPodClass supportedIPodClasses);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSupportedIPodClasses )( 
            IiPodManager * This,
            /* [in] */ IPODAPPID appID,
            /* [retval][out] */ enum EIPodClass *supportedIPodClasses);
        
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


#define IiPodManager_RegisterApp(This,appID,appName,appPath,cmdLine)	\
    (This)->lpVtbl -> RegisterApp(This,appID,appName,appPath,cmdLine)

#define IiPodManager_UnregisterApp(This,appID)	\
    (This)->lpVtbl -> UnregisterApp(This,appID)

#define IiPodManager_GetRegisteredApps(This,pArrayAppID)	\
    (This)->lpVtbl -> GetRegisteredApps(This,pArrayAppID)

#define IiPodManager_GetAppNameFromID(This,appID,pAppName)	\
    (This)->lpVtbl -> GetAppNameFromID(This,appID,pAppName)

#define IiPodManager_GetAPILockingApp(This,pAppID)	\
    (This)->lpVtbl -> GetAPILockingApp(This,pAppID)

#define IiPodManager_Login(This,appID)	\
    (This)->lpVtbl -> Login(This,appID)

#define IiPodManager_Logout(This,appID)	\
    (This)->lpVtbl -> Logout(This,appID)

#define IiPodManager_EnableSync(This,appID,iPodHandle)	\
    (This)->lpVtbl -> EnableSync(This,appID,iPodHandle)

#define IiPodManager_DisableSync(This,appID,iPodHandle)	\
    (This)->lpVtbl -> DisableSync(This,appID,iPodHandle)

#define IiPodManager_BeginTransfer(This,appID,iPodHandle)	\
    (This)->lpVtbl -> BeginTransfer(This,appID,iPodHandle)

#define IiPodManager_EndTransfer(This,appID,iPodHandle)	\
    (This)->lpVtbl -> EndTransfer(This,appID,iPodHandle)

#define IiPodManager_LockiPod(This,appID,iPodHandle)	\
    (This)->lpVtbl -> LockiPod(This,appID,iPodHandle)

#define IiPodManager_UnlockiPod(This,appID,iPodHandle)	\
    (This)->lpVtbl -> UnlockiPod(This,appID,iPodHandle)

#define IiPodManager_GetiPodLockingApp(This,iPodHandle,pAppID)	\
    (This)->lpVtbl -> GetiPodLockingApp(This,iPodHandle,pAppID)

#define IiPodManager_FindFirstiPod(This,appID,piPodHandle)	\
    (This)->lpVtbl -> FindFirstiPod(This,appID,piPodHandle)

#define IiPodManager_FindNextiPod(This,appID,pCuriPodHandle,piPodHandle)	\
    (This)->lpVtbl -> FindNextiPod(This,appID,pCuriPodHandle,piPodHandle)

#define IiPodManager_Mount(This,appID,iPodHandle)	\
    (This)->lpVtbl -> Mount(This,appID,iPodHandle)

#define IiPodManager_Unmount(This,appID,iPodHandle)	\
    (This)->lpVtbl -> Unmount(This,appID,iPodHandle)

#define IiPodManager_IsMounted(This,appID,iPodHandle,bIsMounted)	\
    (This)->lpVtbl -> IsMounted(This,appID,iPodHandle,bIsMounted)

#define IiPodManager_GetiPodFreeSpace(This,appID,iPodHandle,pSpace)	\
    (This)->lpVtbl -> GetiPodFreeSpace(This,appID,iPodHandle,pSpace)

#define IiPodManager_GetDiskSize(This,appID,iPodHandle,pSize)	\
    (This)->lpVtbl -> GetDiskSize(This,appID,iPodHandle,pSize)

#define IiPodManager_GetiPodDriveLetter(This,appID,iPodHandle,format,driveLetter)	\
    (This)->lpVtbl -> GetiPodDriveLetter(This,appID,iPodHandle,format,driveLetter)

#define IiPodManager_LaunchUpdater(This,callerAppID)	\
    (This)->lpVtbl -> LaunchUpdater(This,callerAppID)

#define IiPodManager_GetVersionInfo(This,appID,iPodHandle,firmwareVersion,pVersion)	\
    (This)->lpVtbl -> GetVersionInfo(This,appID,iPodHandle,firmwareVersion,pVersion)

#define IiPodManager_GetAPIVersion(This,pVersion)	\
    (This)->lpVtbl -> GetAPIVersion(This,pVersion)

#define IiPodManager_IsFormatted(This,appID,iPodHandle,IsFormatted)	\
    (This)->lpVtbl -> IsFormatted(This,appID,iPodHandle,IsFormatted)

#define IiPodManager_GetVoiceNotesFolderName(This,appID,iPodHandle,pFolderName)	\
    (This)->lpVtbl -> GetVoiceNotesFolderName(This,appID,iPodHandle,pFolderName)

#define IiPodManager_GetDeviceUniqueID(This,appID,iPodHandle,pUniqueID)	\
    (This)->lpVtbl -> GetDeviceUniqueID(This,appID,iPodHandle,pUniqueID)

#define IiPodManager_BeginTransferAsync(This,appID,iPodHandle)	\
    (This)->lpVtbl -> BeginTransferAsync(This,appID,iPodHandle)

#define IiPodManager_EndTransferAsync(This,appID,iPodHandle)	\
    (This)->lpVtbl -> EndTransferAsync(This,appID,iPodHandle)

#define IiPodManager_GetFirmwareVersion(This,appID,iPodHandle,firmwareVersion)	\
    (This)->lpVtbl -> GetFirmwareVersion(This,appID,iPodHandle,firmwareVersion)

#define IiPodManager_GetCheckpointData(This,appID,iPodHandle,checkpointData)	\
    (This)->lpVtbl -> GetCheckpointData(This,appID,iPodHandle,checkpointData)

#define IiPodManager_SetSupportedIPodClasses(This,appID,supportedIPodClasses)	\
    (This)->lpVtbl -> SetSupportedIPodClasses(This,appID,supportedIPodClasses)

#define IiPodManager_GetSupportedIPodClasses(This,appID,supportedIPodClasses)	\
    (This)->lpVtbl -> GetSupportedIPodClasses(This,appID,supportedIPodClasses)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_RegisterApp_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ BSTR appName,
    /* [in] */ BSTR appPath,
    /* [in] */ BSTR cmdLine);


void __RPC_STUB IiPodManager_RegisterApp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_UnregisterApp_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID);


void __RPC_STUB IiPodManager_UnregisterApp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetRegisteredApps_Proxy( 
    IiPodManager * This,
    /* [retval][out] */ SAFEARRAY * *pArrayAppID);


void __RPC_STUB IiPodManager_GetRegisteredApps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetAppNameFromID_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [retval][out] */ BSTR *pAppName);


void __RPC_STUB IiPodManager_GetAppNameFromID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetAPILockingApp_Proxy( 
    IiPodManager * This,
    /* [retval][out] */ IPODAPPID *pAppID);


void __RPC_STUB IiPodManager_GetAPILockingApp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_Login_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID);


void __RPC_STUB IiPodManager_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_Logout_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID);


void __RPC_STUB IiPodManager_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_EnableSync_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_EnableSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_DisableSync_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_DisableSync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_BeginTransfer_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_BeginTransfer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_EndTransfer_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_EndTransfer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_LockiPod_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_LockiPod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_UnlockiPod_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_UnlockiPod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetiPodLockingApp_Proxy( 
    IiPodManager * This,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ IPODAPPID *pAppID);


void __RPC_STUB IiPodManager_GetiPodLockingApp_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_FindFirstiPod_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [retval][out] */ HIPOD *piPodHandle);


void __RPC_STUB IiPodManager_FindFirstiPod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_FindNextiPod_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD pCuriPodHandle,
    /* [retval][out] */ HIPOD *piPodHandle);


void __RPC_STUB IiPodManager_FindNextiPod_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_Mount_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_Mount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_Unmount_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_Unmount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_IsMounted_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ long *bIsMounted);


void __RPC_STUB IiPodManager_IsMounted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetiPodFreeSpace_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ CURRENCY *pSpace);


void __RPC_STUB IiPodManager_GetiPodFreeSpace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetDiskSize_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ CURRENCY *pSize);


void __RPC_STUB IiPodManager_GetDiskSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetiPodDriveLetter_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ int format,
    /* [retval][out] */ BSTR *driveLetter);


void __RPC_STUB IiPodManager_GetiPodDriveLetter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_LaunchUpdater_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID callerAppID);


void __RPC_STUB IiPodManager_LaunchUpdater_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetVersionInfo_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [out] */ BSTR *firmwareVersion,
    /* [retval][out] */ BSTR *pVersion);


void __RPC_STUB IiPodManager_GetVersionInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetAPIVersion_Proxy( 
    IiPodManager * This,
    /* [retval][out] */ long *pVersion);


void __RPC_STUB IiPodManager_GetAPIVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_IsFormatted_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ EFormatState *IsFormatted);


void __RPC_STUB IiPodManager_IsFormatted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetVoiceNotesFolderName_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ BSTR *pFolderName);


void __RPC_STUB IiPodManager_GetVoiceNotesFolderName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetDeviceUniqueID_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ BSTR *pUniqueID);


void __RPC_STUB IiPodManager_GetDeviceUniqueID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_BeginTransferAsync_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_BeginTransferAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_EndTransferAsync_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodManager_EndTransferAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetFirmwareVersion_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ unsigned short *firmwareVersion);


void __RPC_STUB IiPodManager_GetFirmwareVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetCheckpointData_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ SAFEARRAY * *checkpointData);


void __RPC_STUB IiPodManager_GetCheckpointData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_SetSupportedIPodClasses_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ enum EIPodClass supportedIPodClasses);


void __RPC_STUB IiPodManager_SetSupportedIPodClasses_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodManager_GetSupportedIPodClasses_Proxy( 
    IiPodManager * This,
    /* [in] */ IPODAPPID appID,
    /* [retval][out] */ enum EIPodClass *supportedIPodClasses);


void __RPC_STUB IiPodManager_GetSupportedIPodClasses_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IiPodManager_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vc80_0273 */
/* [local] */ 


enum EAutomaticUpdateMode
    {	kUpdateUnknown	= 0,
	kUpdateAll	= 1,
	kUpdateSelectedPlaylists	= 2
    } ;

enum EiPodState
    {	kUnknowniPod	= 0,
	kNewiPod	= 1,
	kConfigured	= 4
    } ;


extern RPC_IF_HANDLE __MIDL_itf_vc80_0273_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vc80_0273_v0_0_s_ifspec;

#ifndef __IiPodPreferences_INTERFACE_DEFINED__
#define __IiPodPreferences_INTERFACE_DEFINED__

/* interface IiPodPreferences */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IiPodPreferences;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3be5a207-40f6-4f25-bf37-27b898660b67")
    IiPodPreferences : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DeviceName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ BSTR pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_OwnerName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_OwnerName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ BSTR pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeMachineName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HomeMachineName( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ BSTR pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FireWireDiskUse( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FireWireDiskUse( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LaunchApplication( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LaunchApplication( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeApplicationID( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ IPODAPPID *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HomeApplicationID( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ IPODAPPID pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_AutomaticUpdate( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_AutomaticUpdate( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_AutomaticUpdateMode( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ enum EAutomaticUpdateMode *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_AutomaticUpdateMode( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ enum EAutomaticUpdateMode pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeUserID( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ CURRENCY *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HomeUserID( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ CURRENCY pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Configured( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Configured( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetSessionState( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ enum EiPodState sessionState,
            /* [in] */ IPODAPPID sessionHomeAppID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SetHomeMachine( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsItHomeMachine( 
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pRet) = 0;
        
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
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceName )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DeviceName )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ BSTR pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_OwnerName )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_OwnerName )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ BSTR pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeMachineName )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomeMachineName )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ BSTR pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FireWireDiskUse )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FireWireDiskUse )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LaunchApplication )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LaunchApplication )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeApplicationID )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ IPODAPPID *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomeApplicationID )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ IPODAPPID pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutomaticUpdate )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutomaticUpdate )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutomaticUpdateMode )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ enum EAutomaticUpdateMode *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutomaticUpdateMode )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ enum EAutomaticUpdateMode pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeUserID )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ CURRENCY *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomeUserID )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ CURRENCY pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Configured )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Configured )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ long pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetSessionState )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [in] */ enum EiPodState sessionState,
            /* [in] */ IPODAPPID sessionHomeAppID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SetHomeMachine )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsItHomeMachine )( 
            IiPodPreferences * This,
            /* [in] */ IPODAPPID appID,
            /* [in] */ HIPOD iPodHandle,
            /* [retval][out] */ long *pRet);
        
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


#define IiPodPreferences_get_DeviceName(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_DeviceName(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_DeviceName(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_DeviceName(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_OwnerName(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_OwnerName(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_OwnerName(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_OwnerName(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_HomeMachineName(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_HomeMachineName(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_HomeMachineName(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_HomeMachineName(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_FireWireDiskUse(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_FireWireDiskUse(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_FireWireDiskUse(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_FireWireDiskUse(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_LaunchApplication(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_LaunchApplication(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_LaunchApplication(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_LaunchApplication(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_HomeApplicationID(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_HomeApplicationID(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_HomeApplicationID(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_HomeApplicationID(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_AutomaticUpdate(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_AutomaticUpdate(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_AutomaticUpdate(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_AutomaticUpdate(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_AutomaticUpdateMode(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_AutomaticUpdateMode(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_AutomaticUpdateMode(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_AutomaticUpdateMode(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_HomeUserID(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_HomeUserID(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_HomeUserID(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_HomeUserID(This,appID,iPodHandle,pVal)

#define IiPodPreferences_get_Configured(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> get_Configured(This,appID,iPodHandle,pVal)

#define IiPodPreferences_put_Configured(This,appID,iPodHandle,pVal)	\
    (This)->lpVtbl -> put_Configured(This,appID,iPodHandle,pVal)

#define IiPodPreferences_SetSessionState(This,appID,iPodHandle,sessionState,sessionHomeAppID)	\
    (This)->lpVtbl -> SetSessionState(This,appID,iPodHandle,sessionState,sessionHomeAppID)

#define IiPodPreferences_SetHomeMachine(This,appID,iPodHandle)	\
    (This)->lpVtbl -> SetHomeMachine(This,appID,iPodHandle)

#define IiPodPreferences_IsItHomeMachine(This,appID,iPodHandle,pRet)	\
    (This)->lpVtbl -> IsItHomeMachine(This,appID,iPodHandle,pRet)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_DeviceName_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IiPodPreferences_get_DeviceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_DeviceName_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ BSTR pVal);


void __RPC_STUB IiPodPreferences_put_DeviceName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_OwnerName_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IiPodPreferences_get_OwnerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_OwnerName_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ BSTR pVal);


void __RPC_STUB IiPodPreferences_put_OwnerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_HomeMachineName_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IiPodPreferences_get_HomeMachineName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_HomeMachineName_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ BSTR pVal);


void __RPC_STUB IiPodPreferences_put_HomeMachineName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_FireWireDiskUse_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IiPodPreferences_get_FireWireDiskUse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_FireWireDiskUse_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ long pVal);


void __RPC_STUB IiPodPreferences_put_FireWireDiskUse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_LaunchApplication_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IiPodPreferences_get_LaunchApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_LaunchApplication_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ long pVal);


void __RPC_STUB IiPodPreferences_put_LaunchApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_HomeApplicationID_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ IPODAPPID *pVal);


void __RPC_STUB IiPodPreferences_get_HomeApplicationID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_HomeApplicationID_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ IPODAPPID pVal);


void __RPC_STUB IiPodPreferences_put_HomeApplicationID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_AutomaticUpdate_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IiPodPreferences_get_AutomaticUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_AutomaticUpdate_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ long pVal);


void __RPC_STUB IiPodPreferences_put_AutomaticUpdate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_AutomaticUpdateMode_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ enum EAutomaticUpdateMode *pVal);


void __RPC_STUB IiPodPreferences_get_AutomaticUpdateMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_AutomaticUpdateMode_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ enum EAutomaticUpdateMode pVal);


void __RPC_STUB IiPodPreferences_put_AutomaticUpdateMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_HomeUserID_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ CURRENCY *pVal);


void __RPC_STUB IiPodPreferences_get_HomeUserID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_HomeUserID_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ CURRENCY pVal);


void __RPC_STUB IiPodPreferences_put_HomeUserID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_get_Configured_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IiPodPreferences_get_Configured_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_put_Configured_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ long pVal);


void __RPC_STUB IiPodPreferences_put_Configured_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_SetSessionState_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [in] */ enum EiPodState sessionState,
    /* [in] */ IPODAPPID sessionHomeAppID);


void __RPC_STUB IiPodPreferences_SetSessionState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_SetHomeMachine_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle);


void __RPC_STUB IiPodPreferences_SetHomeMachine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodPreferences_IsItHomeMachine_Proxy( 
    IiPodPreferences * This,
    /* [in] */ IPODAPPID appID,
    /* [in] */ HIPOD iPodHandle,
    /* [retval][out] */ long *pRet);


void __RPC_STUB IiPodPreferences_IsItHomeMachine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IiPodPreferences_INTERFACE_DEFINED__ */


#ifndef __IiPodUpdater_INTERFACE_DEFINED__
#define __IiPodUpdater_INTERFACE_DEFINED__

/* interface IiPodUpdater */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IiPodUpdater;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("42a63984-1154-4995-a75e-2e92f75ed902")
    IiPodUpdater : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LockAPI( 
            /* [in] */ IPODAPPID updaterID,
            /* [retval][out] */ IPODAPPID *pLockingAppID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnlockAPI( 
            /* [in] */ IPODAPPID updaterID) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_InitUpdater( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_CloseUpdater( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_IsConnected( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Mount( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Open( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_LockIPOD( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_EraseSector( 
            /* [in] */ unsigned long nSec) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_formatFAT32( 
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_WriteMBR( 
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_UnlockIPOD( 
            /* [in] */ long fDismount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Close( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Unmount( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Disconnect( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_GetPowerState( 
            /* [in] */ unsigned long *pdwState) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Write( 
            /* [in] */ unsigned long start,
            /* [in] */ unsigned long cnt,
            /* [in] */ VARIANT *pVar) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_GetiPodCount( 
            /* [in] */ unsigned long *pdwCount) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_GetProgressStatus( 
            /* [in] */ unsigned long *pdwDone,
            /* [in] */ unsigned long *pdwTotal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_GetDriveLetter( 
            /* [in] */ unsigned long *pchar) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_NotifyUpdateComplete( 
            /* [in] */ HIPOD HIPOD) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_iPodGetHWVersion( 
            /* [in] */ unsigned long *pVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_Execute( 
            /* [in] */ unsigned long param,
            /* [in] */ VARIANT *pvar1,
            /* [in] */ VARIANT *pvar2,
            /* [in] */ VARIANT *pvar3,
            /* [in] */ VARIANT *pvar4,
            /* [in] */ VARIANT *pvar5,
            /* [in] */ VARIANT *pvar6) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_formatFAT32Ex( 
            /* [in] */ unsigned long dwFlags,
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart,
            /* [in] */ unsigned long dwFirmwareSectors) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_WriteMBREx( 
            /* [in] */ unsigned long dwFlags,
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart,
            /* [in] */ unsigned long dwFirmwareSectors) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE U_GetLastErrorMsg( 
            /* [retval][out] */ BSTR *bstrMsg) = 0;
        
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LockAPI )( 
            IiPodUpdater * This,
            /* [in] */ IPODAPPID updaterID,
            /* [retval][out] */ IPODAPPID *pLockingAppID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnlockAPI )( 
            IiPodUpdater * This,
            /* [in] */ IPODAPPID updaterID);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_InitUpdater )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_CloseUpdater )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_IsConnected )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Mount )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Open )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_LockIPOD )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_EraseSector )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long nSec);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_formatFAT32 )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_WriteMBR )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_UnlockIPOD )( 
            IiPodUpdater * This,
            /* [in] */ long fDismount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Close )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Unmount )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Disconnect )( 
            IiPodUpdater * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_GetPowerState )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long *pdwState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Write )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long start,
            /* [in] */ unsigned long cnt,
            /* [in] */ VARIANT *pVar);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_GetiPodCount )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long *pdwCount);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_GetProgressStatus )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long *pdwDone,
            /* [in] */ unsigned long *pdwTotal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_GetDriveLetter )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long *pchar);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_NotifyUpdateComplete )( 
            IiPodUpdater * This,
            /* [in] */ HIPOD HIPOD);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_iPodGetHWVersion )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long *pVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_Execute )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long param,
            /* [in] */ VARIANT *pvar1,
            /* [in] */ VARIANT *pvar2,
            /* [in] */ VARIANT *pvar3,
            /* [in] */ VARIANT *pvar4,
            /* [in] */ VARIANT *pvar5,
            /* [in] */ VARIANT *pvar6);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_formatFAT32Ex )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long dwFlags,
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart,
            /* [in] */ unsigned long dwFirmwareSectors);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_WriteMBREx )( 
            IiPodUpdater * This,
            /* [in] */ unsigned long dwFlags,
            /* [in] */ unsigned long dwType,
            /* [in] */ unsigned long dwStart,
            /* [in] */ unsigned long dwFirmwareSectors);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *U_GetLastErrorMsg )( 
            IiPodUpdater * This,
            /* [retval][out] */ BSTR *bstrMsg);
        
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


#define IiPodUpdater_LockAPI(This,updaterID,pLockingAppID)	\
    (This)->lpVtbl -> LockAPI(This,updaterID,pLockingAppID)

#define IiPodUpdater_UnlockAPI(This,updaterID)	\
    (This)->lpVtbl -> UnlockAPI(This,updaterID)

#define IiPodUpdater_U_InitUpdater(This)	\
    (This)->lpVtbl -> U_InitUpdater(This)

#define IiPodUpdater_U_CloseUpdater(This)	\
    (This)->lpVtbl -> U_CloseUpdater(This)

#define IiPodUpdater_U_IsConnected(This)	\
    (This)->lpVtbl -> U_IsConnected(This)

#define IiPodUpdater_U_Mount(This)	\
    (This)->lpVtbl -> U_Mount(This)

#define IiPodUpdater_U_Open(This)	\
    (This)->lpVtbl -> U_Open(This)

#define IiPodUpdater_U_LockIPOD(This)	\
    (This)->lpVtbl -> U_LockIPOD(This)

#define IiPodUpdater_U_EraseSector(This,nSec)	\
    (This)->lpVtbl -> U_EraseSector(This,nSec)

#define IiPodUpdater_U_formatFAT32(This,dwType,dwStart)	\
    (This)->lpVtbl -> U_formatFAT32(This,dwType,dwStart)

#define IiPodUpdater_U_WriteMBR(This,dwType,dwStart)	\
    (This)->lpVtbl -> U_WriteMBR(This,dwType,dwStart)

#define IiPodUpdater_U_UnlockIPOD(This,fDismount)	\
    (This)->lpVtbl -> U_UnlockIPOD(This,fDismount)

#define IiPodUpdater_U_Close(This)	\
    (This)->lpVtbl -> U_Close(This)

#define IiPodUpdater_U_Unmount(This)	\
    (This)->lpVtbl -> U_Unmount(This)

#define IiPodUpdater_U_Disconnect(This)	\
    (This)->lpVtbl -> U_Disconnect(This)

#define IiPodUpdater_U_GetPowerState(This,pdwState)	\
    (This)->lpVtbl -> U_GetPowerState(This,pdwState)

#define IiPodUpdater_U_Write(This,start,cnt,pVar)	\
    (This)->lpVtbl -> U_Write(This,start,cnt,pVar)

#define IiPodUpdater_U_GetiPodCount(This,pdwCount)	\
    (This)->lpVtbl -> U_GetiPodCount(This,pdwCount)

#define IiPodUpdater_U_GetProgressStatus(This,pdwDone,pdwTotal)	\
    (This)->lpVtbl -> U_GetProgressStatus(This,pdwDone,pdwTotal)

#define IiPodUpdater_U_GetDriveLetter(This,pchar)	\
    (This)->lpVtbl -> U_GetDriveLetter(This,pchar)

#define IiPodUpdater_U_NotifyUpdateComplete(This,HIPOD)	\
    (This)->lpVtbl -> U_NotifyUpdateComplete(This,HIPOD)

#define IiPodUpdater_U_iPodGetHWVersion(This,pVersion)	\
    (This)->lpVtbl -> U_iPodGetHWVersion(This,pVersion)

#define IiPodUpdater_U_Execute(This,param,pvar1,pvar2,pvar3,pvar4,pvar5,pvar6)	\
    (This)->lpVtbl -> U_Execute(This,param,pvar1,pvar2,pvar3,pvar4,pvar5,pvar6)

#define IiPodUpdater_U_formatFAT32Ex(This,dwFlags,dwType,dwStart,dwFirmwareSectors)	\
    (This)->lpVtbl -> U_formatFAT32Ex(This,dwFlags,dwType,dwStart,dwFirmwareSectors)

#define IiPodUpdater_U_WriteMBREx(This,dwFlags,dwType,dwStart,dwFirmwareSectors)	\
    (This)->lpVtbl -> U_WriteMBREx(This,dwFlags,dwType,dwStart,dwFirmwareSectors)

#define IiPodUpdater_U_GetLastErrorMsg(This,bstrMsg)	\
    (This)->lpVtbl -> U_GetLastErrorMsg(This,bstrMsg)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_LockAPI_Proxy( 
    IiPodUpdater * This,
    /* [in] */ IPODAPPID updaterID,
    /* [retval][out] */ IPODAPPID *pLockingAppID);


void __RPC_STUB IiPodUpdater_LockAPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_UnlockAPI_Proxy( 
    IiPodUpdater * This,
    /* [in] */ IPODAPPID updaterID);


void __RPC_STUB IiPodUpdater_UnlockAPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_InitUpdater_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_InitUpdater_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_CloseUpdater_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_CloseUpdater_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_IsConnected_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_IsConnected_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Mount_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_Mount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Open_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_Open_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_LockIPOD_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_LockIPOD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_EraseSector_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long nSec);


void __RPC_STUB IiPodUpdater_U_EraseSector_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_formatFAT32_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long dwType,
    /* [in] */ unsigned long dwStart);


void __RPC_STUB IiPodUpdater_U_formatFAT32_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_WriteMBR_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long dwType,
    /* [in] */ unsigned long dwStart);


void __RPC_STUB IiPodUpdater_U_WriteMBR_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_UnlockIPOD_Proxy( 
    IiPodUpdater * This,
    /* [in] */ long fDismount);


void __RPC_STUB IiPodUpdater_U_UnlockIPOD_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Close_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Unmount_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_Unmount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Disconnect_Proxy( 
    IiPodUpdater * This);


void __RPC_STUB IiPodUpdater_U_Disconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_GetPowerState_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long *pdwState);


void __RPC_STUB IiPodUpdater_U_GetPowerState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Write_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long start,
    /* [in] */ unsigned long cnt,
    /* [in] */ VARIANT *pVar);


void __RPC_STUB IiPodUpdater_U_Write_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_GetiPodCount_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long *pdwCount);


void __RPC_STUB IiPodUpdater_U_GetiPodCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_GetProgressStatus_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long *pdwDone,
    /* [in] */ unsigned long *pdwTotal);


void __RPC_STUB IiPodUpdater_U_GetProgressStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_GetDriveLetter_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long *pchar);


void __RPC_STUB IiPodUpdater_U_GetDriveLetter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_NotifyUpdateComplete_Proxy( 
    IiPodUpdater * This,
    /* [in] */ HIPOD HIPOD);


void __RPC_STUB IiPodUpdater_U_NotifyUpdateComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_iPodGetHWVersion_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long *pVersion);


void __RPC_STUB IiPodUpdater_U_iPodGetHWVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_Execute_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long param,
    /* [in] */ VARIANT *pvar1,
    /* [in] */ VARIANT *pvar2,
    /* [in] */ VARIANT *pvar3,
    /* [in] */ VARIANT *pvar4,
    /* [in] */ VARIANT *pvar5,
    /* [in] */ VARIANT *pvar6);


void __RPC_STUB IiPodUpdater_U_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_formatFAT32Ex_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long dwFlags,
    /* [in] */ unsigned long dwType,
    /* [in] */ unsigned long dwStart,
    /* [in] */ unsigned long dwFirmwareSectors);


void __RPC_STUB IiPodUpdater_U_formatFAT32Ex_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_WriteMBREx_Proxy( 
    IiPodUpdater * This,
    /* [in] */ unsigned long dwFlags,
    /* [in] */ unsigned long dwType,
    /* [in] */ unsigned long dwStart,
    /* [in] */ unsigned long dwFirmwareSectors);


void __RPC_STUB IiPodUpdater_U_WriteMBREx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUpdater_U_GetLastErrorMsg_Proxy( 
    IiPodUpdater * This,
    /* [retval][out] */ BSTR *bstrMsg);


void __RPC_STUB IiPodUpdater_U_GetLastErrorMsg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IiPodUpdater_INTERFACE_DEFINED__ */


#ifndef __IiPodUser_INTERFACE_DEFINED__
#define __IiPodUser_INTERFACE_DEFINED__

/* interface IiPodUser */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IiPodUser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65e1d175-369d-412f-bbbf-a5e042c220e7")
    IiPodUser : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterUser( 
            /* [in] */ IUnknown *pUnk,
            /* [in] */ long procID,
            /* [retval][out] */ unsigned long *pUserCookie) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterUser( 
            /* [in] */ unsigned long userCookie) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnDisplayiPodManagerUIEvent( 
            /* [in] */ long UIHandle,
            /* [in] */ long SessionHomeApp,
            /* [in] */ long sessionState,
            /* [in] */ long UIStatus) = 0;
        
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
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterUser )( 
            IiPodUser * This,
            /* [in] */ IUnknown *pUnk,
            /* [in] */ long procID,
            /* [retval][out] */ unsigned long *pUserCookie);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterUser )( 
            IiPodUser * This,
            /* [in] */ unsigned long userCookie);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OnDisplayiPodManagerUIEvent )( 
            IiPodUser * This,
            /* [in] */ long UIHandle,
            /* [in] */ long SessionHomeApp,
            /* [in] */ long sessionState,
            /* [in] */ long UIStatus);
        
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


#define IiPodUser_RegisterUser(This,pUnk,procID,pUserCookie)	\
    (This)->lpVtbl -> RegisterUser(This,pUnk,procID,pUserCookie)

#define IiPodUser_UnregisterUser(This,userCookie)	\
    (This)->lpVtbl -> UnregisterUser(This,userCookie)

#define IiPodUser_OnDisplayiPodManagerUIEvent(This,UIHandle,SessionHomeApp,sessionState,UIStatus)	\
    (This)->lpVtbl -> OnDisplayiPodManagerUIEvent(This,UIHandle,SessionHomeApp,sessionState,UIStatus)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUser_RegisterUser_Proxy( 
    IiPodUser * This,
    /* [in] */ IUnknown *pUnk,
    /* [in] */ long procID,
    /* [retval][out] */ unsigned long *pUserCookie);


void __RPC_STUB IiPodUser_RegisterUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUser_UnregisterUser_Proxy( 
    IiPodUser * This,
    /* [in] */ unsigned long userCookie);


void __RPC_STUB IiPodUser_UnregisterUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IiPodUser_OnDisplayiPodManagerUIEvent_Proxy( 
    IiPodUser * This,
    /* [in] */ long UIHandle,
    /* [in] */ long SessionHomeApp,
    /* [in] */ long sessionState,
    /* [in] */ long UIStatus);


void __RPC_STUB IiPodUser_OnDisplayiPodManagerUIEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IiPodUser_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vc80_0276 */
/* [local] */ 


enum EItunesDeviceClass
    {	kItunesClassNone	= 0,
	kItunesClassV1	= 1,
	kItunesClassV2	= 2,
	kItunesClassALL	= 65535
    } ;


extern RPC_IF_HANDLE __MIDL_itf_vc80_0276_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vc80_0276_v0_0_s_ifspec;

#ifndef __IItunesServiceCallback_INTERFACE_DEFINED__
#define __IItunesServiceCallback_INTERFACE_DEFINED__

/* interface IItunesServiceCallback */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IItunesServiceCallback;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c337768e-d57c-4fc3-bed8-08fa9e892c6b")
    IItunesServiceCallback : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnDeviceStateChanged( 
            /* [in] */ long hDevice,
            /* [in] */ enum EItunesDeviceClass devClass,
            /* [in] */ _EDeviceStatus lNewState) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE OnRequestClientCookie( 
            /* [out] */ unsigned long *pdwCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IItunesServiceCallbackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IItunesServiceCallback * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IItunesServiceCallback * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IItunesServiceCallback * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IItunesServiceCallback * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IItunesServiceCallback * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IItunesServiceCallback * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IItunesServiceCallback * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OnDeviceStateChanged )( 
            IItunesServiceCallback * This,
            /* [in] */ long hDevice,
            /* [in] */ enum EItunesDeviceClass devClass,
            /* [in] */ _EDeviceStatus lNewState);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *OnRequestClientCookie )( 
            IItunesServiceCallback * This,
            /* [out] */ unsigned long *pdwCookie);
        
        END_INTERFACE
    } IItunesServiceCallbackVtbl;

    interface IItunesServiceCallback
    {
        CONST_VTBL struct IItunesServiceCallbackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IItunesServiceCallback_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IItunesServiceCallback_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IItunesServiceCallback_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IItunesServiceCallback_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IItunesServiceCallback_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IItunesServiceCallback_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IItunesServiceCallback_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IItunesServiceCallback_OnDeviceStateChanged(This,hDevice,devClass,lNewState)	\
    (This)->lpVtbl -> OnDeviceStateChanged(This,hDevice,devClass,lNewState)

#define IItunesServiceCallback_OnRequestClientCookie(This,pdwCookie)	\
    (This)->lpVtbl -> OnRequestClientCookie(This,pdwCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IItunesServiceCallback_OnDeviceStateChanged_Proxy( 
    IItunesServiceCallback * This,
    /* [in] */ long hDevice,
    /* [in] */ enum EItunesDeviceClass devClass,
    /* [in] */ _EDeviceStatus lNewState);


void __RPC_STUB IItunesServiceCallback_OnDeviceStateChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesServiceCallback_OnRequestClientCookie_Proxy( 
    IItunesServiceCallback * This,
    /* [out] */ unsigned long *pdwCookie);


void __RPC_STUB IItunesServiceCallback_OnRequestClientCookie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItunesServiceCallback_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_vc80_0277 */
/* [local] */ 


enum __MIDL___MIDL_itf_iPodService_0000_0002
    {	kDeviceStatusUnknown	= 0,
	kDeviceStatusConnected	= 1,
	kDeviceStatusMounted	= 2,
	kDeviceStatusUnmounted	= 3,
	kDeviceStatusDisconnected	= 4,
	kDeviceStatusMountFailed	= 5
    } ;


extern RPC_IF_HANDLE __MIDL_itf_vc80_0277_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_vc80_0277_v0_0_s_ifspec;

#ifndef __IItunesDevicePrefs_INTERFACE_DEFINED__
#define __IItunesDevicePrefs_INTERFACE_DEFINED__

/* interface IItunesDevicePrefs */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IItunesDevicePrefs;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6d3c0324-6a5a-47fe-97f9-ac9a1dc69cc4")
    IItunesDevicePrefs : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DeviceHandle( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Configured( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Configured( 
            /* [in] */ VARIANT_BOOL pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FireWireDiskUse( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FireWireDiskUse( 
            /* [in] */ VARIANT_BOOL pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HomeAppAutoPlay( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HomeAppAutoPlay( 
            /* [in] */ VARIANT_BOOL pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IItunesDevicePrefsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IItunesDevicePrefs * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IItunesDevicePrefs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IItunesDevicePrefs * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IItunesDevicePrefs * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IItunesDevicePrefs * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IItunesDevicePrefs * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IItunesDevicePrefs * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DeviceHandle )( 
            IItunesDevicePrefs * This,
            /* [retval][out] */ long *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Configured )( 
            IItunesDevicePrefs * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Configured )( 
            IItunesDevicePrefs * This,
            /* [in] */ VARIANT_BOOL pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FireWireDiskUse )( 
            IItunesDevicePrefs * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FireWireDiskUse )( 
            IItunesDevicePrefs * This,
            /* [in] */ VARIANT_BOOL pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HomeAppAutoPlay )( 
            IItunesDevicePrefs * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_HomeAppAutoPlay )( 
            IItunesDevicePrefs * This,
            /* [in] */ VARIANT_BOOL pVal);
        
        END_INTERFACE
    } IItunesDevicePrefsVtbl;

    interface IItunesDevicePrefs
    {
        CONST_VTBL struct IItunesDevicePrefsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IItunesDevicePrefs_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IItunesDevicePrefs_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IItunesDevicePrefs_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IItunesDevicePrefs_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IItunesDevicePrefs_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IItunesDevicePrefs_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IItunesDevicePrefs_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IItunesDevicePrefs_get_DeviceHandle(This,pVal)	\
    (This)->lpVtbl -> get_DeviceHandle(This,pVal)

#define IItunesDevicePrefs_get_Configured(This,pVal)	\
    (This)->lpVtbl -> get_Configured(This,pVal)

#define IItunesDevicePrefs_put_Configured(This,pVal)	\
    (This)->lpVtbl -> put_Configured(This,pVal)

#define IItunesDevicePrefs_get_FireWireDiskUse(This,pVal)	\
    (This)->lpVtbl -> get_FireWireDiskUse(This,pVal)

#define IItunesDevicePrefs_put_FireWireDiskUse(This,pVal)	\
    (This)->lpVtbl -> put_FireWireDiskUse(This,pVal)

#define IItunesDevicePrefs_get_HomeAppAutoPlay(This,pVal)	\
    (This)->lpVtbl -> get_HomeAppAutoPlay(This,pVal)

#define IItunesDevicePrefs_put_HomeAppAutoPlay(This,pVal)	\
    (This)->lpVtbl -> put_HomeAppAutoPlay(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_get_DeviceHandle_Proxy( 
    IItunesDevicePrefs * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IItunesDevicePrefs_get_DeviceHandle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_get_Configured_Proxy( 
    IItunesDevicePrefs * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IItunesDevicePrefs_get_Configured_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_put_Configured_Proxy( 
    IItunesDevicePrefs * This,
    /* [in] */ VARIANT_BOOL pVal);


void __RPC_STUB IItunesDevicePrefs_put_Configured_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_get_FireWireDiskUse_Proxy( 
    IItunesDevicePrefs * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IItunesDevicePrefs_get_FireWireDiskUse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_put_FireWireDiskUse_Proxy( 
    IItunesDevicePrefs * This,
    /* [in] */ VARIANT_BOOL pVal);


void __RPC_STUB IItunesDevicePrefs_put_FireWireDiskUse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_get_HomeAppAutoPlay_Proxy( 
    IItunesDevicePrefs * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IItunesDevicePrefs_get_HomeAppAutoPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IItunesDevicePrefs_put_HomeAppAutoPlay_Proxy( 
    IItunesDevicePrefs * This,
    /* [in] */ VARIANT_BOOL pVal);


void __RPC_STUB IItunesDevicePrefs_put_HomeAppAutoPlay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItunesDevicePrefs_INTERFACE_DEFINED__ */


#ifndef __IItunesDevices_INTERFACE_DEFINED__
#define __IItunesDevices_INTERFACE_DEFINED__

/* interface IItunesDevices */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IItunesDevices;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("be2617aa-4914-45c0-925b-1f3f1df8007d")
    IItunesDevices : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindFirstDevice( 
            /* [out] */ long *pDevHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FindNextDevice( 
            /* [out] */ long *pDevHandle) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RetainDevice( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ReleaseDevice( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RetainDeviceAsync( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ReleaseDeviceAsync( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDeviceClass( 
            /* [in] */ long hDevice,
            /* [out] */ enum EItunesDeviceClass *pDevClass) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSerialNumber( 
            /* [in] */ long hDevice,
            /* [out] */ BSTR *pBstrSerial) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFirmwareVersion( 
            /* [in] */ long hDevice,
            /* [out] */ unsigned short *pwVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDriveFormat( 
            /* [in] */ long hDevice,
            /* [retval][out] */ EFormatState *pDriveFormat) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetDriveLetter( 
            /* [in] */ long hDevice,
            /* [out] */ BSTR *pBstrDrive) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetAutoPlayDisableForDriveLetter( 
            /* [in] */ BSTR bstrDrive,
            /* [retval][out] */ VARIANT_BOOL *pvbDisableAutoPlay) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetPreferences( 
            /* [in] */ long hDevice,
            /* [out] */ IItunesDevicePrefs **ppDevPrefs) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetCheckpointData( 
            /* [in] */ long hDevice,
            /* [out] */ SAFEARRAY * *ppCheckpointData) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Mount( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Unmount( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE MountAsync( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnmountAsync( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE IsMounted( 
            /* [in] */ long hDevice,
            /* [retval][out] */ VARIANT_BOOL *pvbMounted) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetHWVersion( 
            /* [in] */ long hDevice,
            /* [retval][out] */ long *lVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetSysVersion( 
            /* [in] */ long hDevice,
            /* [retval][out] */ BSTR *pBstrVersion) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE LockAPI( 
            /* [in] */ long hDevice) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnlockAPI( 
            /* [in] */ long hDevice) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IItunesDevicesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IItunesDevices * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IItunesDevices * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IItunesDevices * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IItunesDevices * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IItunesDevices * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IItunesDevices * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IItunesDevices * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindFirstDevice )( 
            IItunesDevices * This,
            /* [out] */ long *pDevHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FindNextDevice )( 
            IItunesDevices * This,
            /* [out] */ long *pDevHandle);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RetainDevice )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ReleaseDevice )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RetainDeviceAsync )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ReleaseDeviceAsync )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDeviceClass )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [out] */ enum EItunesDeviceClass *pDevClass);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSerialNumber )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [out] */ BSTR *pBstrSerial);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFirmwareVersion )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [out] */ unsigned short *pwVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDriveFormat )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [retval][out] */ EFormatState *pDriveFormat);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetDriveLetter )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [out] */ BSTR *pBstrDrive);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetAutoPlayDisableForDriveLetter )( 
            IItunesDevices * This,
            /* [in] */ BSTR bstrDrive,
            /* [retval][out] */ VARIANT_BOOL *pvbDisableAutoPlay);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetPreferences )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [out] */ IItunesDevicePrefs **ppDevPrefs);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetCheckpointData )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [out] */ SAFEARRAY * *ppCheckpointData);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Mount )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Unmount )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *MountAsync )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnmountAsync )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *IsMounted )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [retval][out] */ VARIANT_BOOL *pvbMounted);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetHWVersion )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [retval][out] */ long *lVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetSysVersion )( 
            IItunesDevices * This,
            /* [in] */ long hDevice,
            /* [retval][out] */ BSTR *pBstrVersion);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *LockAPI )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnlockAPI )( 
            IItunesDevices * This,
            /* [in] */ long hDevice);
        
        END_INTERFACE
    } IItunesDevicesVtbl;

    interface IItunesDevices
    {
        CONST_VTBL struct IItunesDevicesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IItunesDevices_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IItunesDevices_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IItunesDevices_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IItunesDevices_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IItunesDevices_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IItunesDevices_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IItunesDevices_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IItunesDevices_FindFirstDevice(This,pDevHandle)	\
    (This)->lpVtbl -> FindFirstDevice(This,pDevHandle)

#define IItunesDevices_FindNextDevice(This,pDevHandle)	\
    (This)->lpVtbl -> FindNextDevice(This,pDevHandle)

#define IItunesDevices_RetainDevice(This,hDevice)	\
    (This)->lpVtbl -> RetainDevice(This,hDevice)

#define IItunesDevices_ReleaseDevice(This,hDevice)	\
    (This)->lpVtbl -> ReleaseDevice(This,hDevice)

#define IItunesDevices_RetainDeviceAsync(This,hDevice)	\
    (This)->lpVtbl -> RetainDeviceAsync(This,hDevice)

#define IItunesDevices_ReleaseDeviceAsync(This,hDevice)	\
    (This)->lpVtbl -> ReleaseDeviceAsync(This,hDevice)

#define IItunesDevices_GetDeviceClass(This,hDevice,pDevClass)	\
    (This)->lpVtbl -> GetDeviceClass(This,hDevice,pDevClass)

#define IItunesDevices_GetSerialNumber(This,hDevice,pBstrSerial)	\
    (This)->lpVtbl -> GetSerialNumber(This,hDevice,pBstrSerial)

#define IItunesDevices_GetFirmwareVersion(This,hDevice,pwVersion)	\
    (This)->lpVtbl -> GetFirmwareVersion(This,hDevice,pwVersion)

#define IItunesDevices_GetDriveFormat(This,hDevice,pDriveFormat)	\
    (This)->lpVtbl -> GetDriveFormat(This,hDevice,pDriveFormat)

#define IItunesDevices_GetDriveLetter(This,hDevice,pBstrDrive)	\
    (This)->lpVtbl -> GetDriveLetter(This,hDevice,pBstrDrive)

#define IItunesDevices_GetAutoPlayDisableForDriveLetter(This,bstrDrive,pvbDisableAutoPlay)	\
    (This)->lpVtbl -> GetAutoPlayDisableForDriveLetter(This,bstrDrive,pvbDisableAutoPlay)

#define IItunesDevices_GetPreferences(This,hDevice,ppDevPrefs)	\
    (This)->lpVtbl -> GetPreferences(This,hDevice,ppDevPrefs)

#define IItunesDevices_GetCheckpointData(This,hDevice,ppCheckpointData)	\
    (This)->lpVtbl -> GetCheckpointData(This,hDevice,ppCheckpointData)

#define IItunesDevices_Mount(This,hDevice)	\
    (This)->lpVtbl -> Mount(This,hDevice)

#define IItunesDevices_Unmount(This,hDevice)	\
    (This)->lpVtbl -> Unmount(This,hDevice)

#define IItunesDevices_MountAsync(This,hDevice)	\
    (This)->lpVtbl -> MountAsync(This,hDevice)

#define IItunesDevices_UnmountAsync(This,hDevice)	\
    (This)->lpVtbl -> UnmountAsync(This,hDevice)

#define IItunesDevices_IsMounted(This,hDevice,pvbMounted)	\
    (This)->lpVtbl -> IsMounted(This,hDevice,pvbMounted)

#define IItunesDevices_GetHWVersion(This,hDevice,lVersion)	\
    (This)->lpVtbl -> GetHWVersion(This,hDevice,lVersion)

#define IItunesDevices_GetSysVersion(This,hDevice,pBstrVersion)	\
    (This)->lpVtbl -> GetSysVersion(This,hDevice,pBstrVersion)

#define IItunesDevices_LockAPI(This,hDevice)	\
    (This)->lpVtbl -> LockAPI(This,hDevice)

#define IItunesDevices_UnlockAPI(This,hDevice)	\
    (This)->lpVtbl -> UnlockAPI(This,hDevice)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_FindFirstDevice_Proxy( 
    IItunesDevices * This,
    /* [out] */ long *pDevHandle);


void __RPC_STUB IItunesDevices_FindFirstDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_FindNextDevice_Proxy( 
    IItunesDevices * This,
    /* [out] */ long *pDevHandle);


void __RPC_STUB IItunesDevices_FindNextDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_RetainDevice_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_RetainDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_ReleaseDevice_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_ReleaseDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_RetainDeviceAsync_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_RetainDeviceAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_ReleaseDeviceAsync_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_ReleaseDeviceAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetDeviceClass_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [out] */ enum EItunesDeviceClass *pDevClass);


void __RPC_STUB IItunesDevices_GetDeviceClass_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetSerialNumber_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [out] */ BSTR *pBstrSerial);


void __RPC_STUB IItunesDevices_GetSerialNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetFirmwareVersion_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [out] */ unsigned short *pwVersion);


void __RPC_STUB IItunesDevices_GetFirmwareVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetDriveFormat_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [retval][out] */ EFormatState *pDriveFormat);


void __RPC_STUB IItunesDevices_GetDriveFormat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetDriveLetter_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [out] */ BSTR *pBstrDrive);


void __RPC_STUB IItunesDevices_GetDriveLetter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetAutoPlayDisableForDriveLetter_Proxy( 
    IItunesDevices * This,
    /* [in] */ BSTR bstrDrive,
    /* [retval][out] */ VARIANT_BOOL *pvbDisableAutoPlay);


void __RPC_STUB IItunesDevices_GetAutoPlayDisableForDriveLetter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetPreferences_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [out] */ IItunesDevicePrefs **ppDevPrefs);


void __RPC_STUB IItunesDevices_GetPreferences_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetCheckpointData_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [out] */ SAFEARRAY * *ppCheckpointData);


void __RPC_STUB IItunesDevices_GetCheckpointData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_Mount_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_Mount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_Unmount_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_Unmount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_MountAsync_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_MountAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_UnmountAsync_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_UnmountAsync_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_IsMounted_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [retval][out] */ VARIANT_BOOL *pvbMounted);


void __RPC_STUB IItunesDevices_IsMounted_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetHWVersion_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [retval][out] */ long *lVersion);


void __RPC_STUB IItunesDevices_GetHWVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_GetSysVersion_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice,
    /* [retval][out] */ BSTR *pBstrVersion);


void __RPC_STUB IItunesDevices_GetSysVersion_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_LockAPI_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_LockAPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesDevices_UnlockAPI_Proxy( 
    IItunesDevices * This,
    /* [in] */ long hDevice);


void __RPC_STUB IItunesDevices_UnlockAPI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItunesDevices_INTERFACE_DEFINED__ */


#ifndef __IItunesHelper_INTERFACE_DEFINED__
#define __IItunesHelper_INTERFACE_DEFINED__

/* interface IItunesHelper */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IItunesHelper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("0f6ffb0d-ddd3-407a-8349-7ec794720c60")
    IItunesHelper : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE InformDeviceNeedReformat( 
            /* [in] */ long hDevice,
            /* [in] */ enum EItunesDeviceClass devClass,
            /* [in] */ BSTR bstrUpdaterPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RequestExtendedFunction( 
            /* [in] */ BSTR bstrExtendedFunction) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IItunesHelperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IItunesHelper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IItunesHelper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IItunesHelper * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IItunesHelper * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IItunesHelper * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IItunesHelper * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IItunesHelper * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *InformDeviceNeedReformat )( 
            IItunesHelper * This,
            /* [in] */ long hDevice,
            /* [in] */ enum EItunesDeviceClass devClass,
            /* [in] */ BSTR bstrUpdaterPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RequestExtendedFunction )( 
            IItunesHelper * This,
            /* [in] */ BSTR bstrExtendedFunction);
        
        END_INTERFACE
    } IItunesHelperVtbl;

    interface IItunesHelper
    {
        CONST_VTBL struct IItunesHelperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IItunesHelper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IItunesHelper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IItunesHelper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IItunesHelper_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IItunesHelper_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IItunesHelper_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IItunesHelper_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IItunesHelper_InformDeviceNeedReformat(This,hDevice,devClass,bstrUpdaterPath)	\
    (This)->lpVtbl -> InformDeviceNeedReformat(This,hDevice,devClass,bstrUpdaterPath)

#define IItunesHelper_RequestExtendedFunction(This,bstrExtendedFunction)	\
    (This)->lpVtbl -> RequestExtendedFunction(This,bstrExtendedFunction)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IItunesHelper_InformDeviceNeedReformat_Proxy( 
    IItunesHelper * This,
    /* [in] */ long hDevice,
    /* [in] */ enum EItunesDeviceClass devClass,
    /* [in] */ BSTR bstrUpdaterPath);


void __RPC_STUB IItunesHelper_InformDeviceNeedReformat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesHelper_RequestExtendedFunction_Proxy( 
    IItunesHelper * This,
    /* [in] */ BSTR bstrExtendedFunction);


void __RPC_STUB IItunesHelper_RequestExtendedFunction_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItunesHelper_INTERFACE_DEFINED__ */


#ifndef __IItunesService_INTERFACE_DEFINED__
#define __IItunesService_INTERFACE_DEFINED__

/* interface IItunesService */
/* [object][dual][uuid] */ 


EXTERN_C const IID IID_IItunesService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("06d8e935-bf1a-4bb8-99c7-11a2d0abd238")
    IItunesService : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Login( 
            /* [in] */ long lAppID,
            /* [in] */ BSTR bstrAppName,
            /* [in] */ IItunesServiceCallback *pCallbackIntf,
            /* [out] */ unsigned long *pdwCookie) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Logout( 
            /* [in] */ unsigned long dwClientCookie) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetItunesDevices( 
            /* [in] */ unsigned long dwClientCookie,
            /* [out] */ IItunesDevices **ppDevicesIntf) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE RegisterHelper( 
            /* [in] */ IItunesHelper *pHelper) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE UnregisterHelper( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IItunesServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IItunesService * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IItunesService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IItunesService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IItunesService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IItunesService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IItunesService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IItunesService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Login )( 
            IItunesService * This,
            /* [in] */ long lAppID,
            /* [in] */ BSTR bstrAppName,
            /* [in] */ IItunesServiceCallback *pCallbackIntf,
            /* [out] */ unsigned long *pdwCookie);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Logout )( 
            IItunesService * This,
            /* [in] */ unsigned long dwClientCookie);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetItunesDevices )( 
            IItunesService * This,
            /* [in] */ unsigned long dwClientCookie,
            /* [out] */ IItunesDevices **ppDevicesIntf);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *RegisterHelper )( 
            IItunesService * This,
            /* [in] */ IItunesHelper *pHelper);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterHelper )( 
            IItunesService * This);
        
        END_INTERFACE
    } IItunesServiceVtbl;

    interface IItunesService
    {
        CONST_VTBL struct IItunesServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IItunesService_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IItunesService_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IItunesService_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IItunesService_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IItunesService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IItunesService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IItunesService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IItunesService_Login(This,lAppID,bstrAppName,pCallbackIntf,pdwCookie)	\
    (This)->lpVtbl -> Login(This,lAppID,bstrAppName,pCallbackIntf,pdwCookie)

#define IItunesService_Logout(This,dwClientCookie)	\
    (This)->lpVtbl -> Logout(This,dwClientCookie)

#define IItunesService_GetItunesDevices(This,dwClientCookie,ppDevicesIntf)	\
    (This)->lpVtbl -> GetItunesDevices(This,dwClientCookie,ppDevicesIntf)

#define IItunesService_RegisterHelper(This,pHelper)	\
    (This)->lpVtbl -> RegisterHelper(This,pHelper)

#define IItunesService_UnregisterHelper(This)	\
    (This)->lpVtbl -> UnregisterHelper(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IItunesService_Login_Proxy( 
    IItunesService * This,
    /* [in] */ long lAppID,
    /* [in] */ BSTR bstrAppName,
    /* [in] */ IItunesServiceCallback *pCallbackIntf,
    /* [out] */ unsigned long *pdwCookie);


void __RPC_STUB IItunesService_Login_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesService_Logout_Proxy( 
    IItunesService * This,
    /* [in] */ unsigned long dwClientCookie);


void __RPC_STUB IItunesService_Logout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesService_GetItunesDevices_Proxy( 
    IItunesService * This,
    /* [in] */ unsigned long dwClientCookie,
    /* [out] */ IItunesDevices **ppDevicesIntf);


void __RPC_STUB IItunesService_GetItunesDevices_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesService_RegisterHelper_Proxy( 
    IItunesService * This,
    /* [in] */ IItunesHelper *pHelper);


void __RPC_STUB IItunesService_RegisterHelper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IItunesService_UnregisterHelper_Proxy( 
    IItunesService * This);


void __RPC_STUB IItunesService_UnregisterHelper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IItunesService_INTERFACE_DEFINED__ */



#ifndef __iPodEventReceiver_LIBRARY_DEFINED__
#define __iPodEventReceiver_LIBRARY_DEFINED__

/* library iPodEventReceiver */
/* [uuid][version] */ 


EXTERN_C const IID LIBID_iPodEventReceiver;

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

#endif /* __iPodEventReceiver_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


