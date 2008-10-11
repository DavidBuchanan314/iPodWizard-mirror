

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Fri Jul 21 13:55:09 2006
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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_iPodEventReceiver,0xbad4f4dd,0xbad6,0x3f14,0xb9,0x5b,0x11,0xd9,0xf6,0x15,0xba,0x9a);


MIDL_DEFINE_GUID(IID, DIID_IiPodManager,0x31e3fbaf,0xc200,0x4776,0x9d,0xbf,0x45,0x22,0x67,0x65,0xcf,0x4d);


MIDL_DEFINE_GUID(IID, DIID_IiPodPreferences,0x3be5a207,0x40f6,0x4f25,0xbf,0x37,0x27,0xb8,0x98,0x66,0x0b,0x67);


MIDL_DEFINE_GUID(IID, DIID_IiPodUpdater,0x42a63984,0x1154,0x4995,0xa7,0x5e,0x2e,0x92,0xf7,0x5e,0xd9,0x02);


MIDL_DEFINE_GUID(IID, DIID_IiPodUser,0x65e1d175,0x369d,0x412f,0xbb,0xbf,0xa5,0xe0,0x42,0xc2,0x20,0xe7);


MIDL_DEFINE_GUID(IID, DIID__IiPodManagerEvents,0x4d56b972,0x8259,0x4833,0x9a,0xb2,0x54,0x39,0x62,0xf5,0xcf,0x5b);


MIDL_DEFINE_GUID(IID, DIID__IiPodUpdaterEvents,0x7a1aad4c,0xa417,0x439c,0x93,0x18,0xd6,0xbd,0xc6,0x3f,0xa4,0x6a);


MIDL_DEFINE_GUID(CLSID, CLSID___Impl__IiPodManagerEvents,0x18FC7734,0xB4AC,0x3461,0x8C,0x00,0x21,0x06,0x7F,0xEF,0xD4,0xD4);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

