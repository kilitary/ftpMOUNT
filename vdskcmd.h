/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2000-2010 NT KERNEL RESOURCES, All Rights Reserved.     */
/* http://www.ntkernel.com                                               */
/*                                                                       */
/* Module Name:  vdskcmd.h                                               */
/*                                                                       */
/* Abstract: This is the include file that defines Virtual Disk driver   */
/*           interface.                                                  */
/*                                                                       */
/* Environment:                                                          */
/*                                                                       */
/*   User mode, Virtual Disk SDK                                         */
/*                                                                       */
/* Revision History:                                                     */
/*                                                                       */
/*************************************************************************/
#ifndef __VDSKCMD_H_
#define __VDSKCMD_H_

#ifdef _WINDOWS
#include <winioctl.h>   // Compiling Win32 Applications Or DLL's
#endif // _WINDOWS

#include "macros.h"

#pragma warning(disable:4200) //nonstandard extension used : zero-sized array in struct/union

// current vdsk driver version.
#define NTVDSK_DRIVER_VERSION		0x00000001

//driver name
#define VDSK_DRIVER_NAMEW L"Vdsk"
#define VDSK_DRIVER_NAMEA  "Vdsk"

#define VDSK_VOLUME_DEVICE_NAMEW VDSK_DRIVER_NAMEW L"Volume"
#define VDSK_VOLUME_DEVICE_NAMEA VDSK_DRIVER_NAMEW "Volume"

//Control device
#define NT_VDSK_DEVICE_NAME_W   L"\\Device\\" VDSK_DRIVER_NAMEW
#define NT_VDSK_DEVICE_NAME_A   "\\Device\\" VDSK_DRIVER_NAMEA

#define DOS_VDSK_DEVICE_NAME_W  L"\\DosDevices\\" VDSK_DRIVER_NAMEW
#define DOS_VDSK_DEVICE_NAME_A  "\\DosDevices\\" VDSK_DRIVER_NAMEA

#define WIN32_VDSK_DEVICE_NAME_W  L"\\\\.\\" VDSK_DRIVER_NAMEW
#define WIN32_VDSK_DEVICE_NAME_A  "\\\\.\\" VDSK_DRIVER_NAMEA

//Volume device: \\Device\\VdskVolume1
#define NT_VDSK_VOLUME_NAME_W   L"\\Device\\" VDSK_VOLUME_DEVICE_NAMEW
#define NT_VDSK_VOLUME_NAME_A   "\\Device\\" VDSK_VOLUME_DEVICE_NAMEA

//
// define notification event codes
//
#define VDSK_DISK_EVENT_NONE     0 // no events
#define VDSK_DISK_EVENT_DEADLOCK 1 // disk detected user-app hang

//
// Storage properties
//

#define VDSK_VENDOR_ID		"NTKERNEL"
#define VDSK_VENDOR_ID_SIZE sizeof(VDSK_VENDOR_ID)

#define VDSK_PRODUCT_ID    "VDSK1-0"
#define VDSK_PRODUCT_ID_SIZE sizeof(VDSK_PRODUCT_ID)

#define VDSK_PRODUCT_REV   "VDSK10"
#define VDSK_PRODUCT_REV_SIZE   sizeof(VDSK_PRODUCT_REV)

#define VDSK_SERIAL_NUMBER "87654321"
#define VDSK_SERIAL_NUMBER_SIZE sizeof(VDSK_SERIAL_NUMBER)

#define VDSK_VOLUME_LINK L"\\??\\Volume{XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}"
#define VDSK_VOLUME_LINK_SIZE sizeof(VDSK_VOLUME_LINK)

//values for disk I/O processing timeout by user app
#define VDSK_DISK_IO_TIMEOUT     SECONDS(30)
// the maximum allowed disk i/o timeout value
#define VDSK_DISK_IO_TIMEOUT_MAX SECONDS(120)

#define FILE_DEVICE_VDSK       0x8800
#define IOCTL_VDSK_BASE        0x800

#ifndef FUNCTION_FROM_CTL_CODE
	#define FUNCTION_FROM_CTL_CODE(ctlCode) (((ULONG)((ctlCode) & 0x00003fff)) >> 2)
#endif //FUNCTION_FROM_CTL_CODE

//
// virtual disk flags
//
#define VDSK_DISK_READ_ONLY       0x00000001
#define VDSK_DISK_REMOVABLE       0x00000002
#define VDSK_DISK_CDROM           0x00000004
#define VDSK_DISK_FLOPPY          0x00000008
#define VDSK_DISK_MEDIA_IN_DEVICE 0x00000010
#define VDSK_DISK_VALID_MASK      0x0000001F
//
// IOCTL_VDSK_CREATE_DISK
//
typedef struct _VDSK_DISK_PARAMETERS
{
	// device flags
	union{
		struct{
			// device is read only
			ULONG ReadOnly : 1;

			//media is removable
			ULONG Removable : 1;

			// device is cd-rom
			ULONG IsCDRom : 1;

			//device is floppy disk
			ULONG IsFloppy : 1;

			//media in device after device creation
			ULONG IsMediaInDevice : 1;
		};
		ULONG Flags;
	};

	// Drive Letter (if used, otherwise zero).
	WCHAR           DriveLetter;

	//
	// Drive geometry
	//

	// Drive size in bytes 
 	LARGE_INTEGER DiskSize;

	//Indicates the number of tracks in a cylinder.
	ULONG TracksPerCylinder;

	//Indicates the number of sectors in each track. 
	ULONG SectorsPerTrack;

	//Indicates the number of bytes in a disk sector.
	ULONG BytesPerSector;

	//
	// disk I/O timeout
	// specifies the absolute time, in units of 100 nanoseconds, 
	// for which the disk IO should be handled. If the disk IO 
	// was not handled in specified timeframe, driver reports deadlock 
	// (it stores VDSK_DISK_EVENT_DEADLOCK in LastDiskNotificationEvent and sets
	// DiskNotificationEvent provided by user-mode)
	// Then virtual disk goes offline:
	// - no media in device
	// - all io operations completes with status 'device is not ready'
	LARGE_INTEGER DiskIoTimeout;

	//
	// user provided handle to event
	// that disk use to notify user-space
	// about disk events:
	// 1. deadlock occurred
	// 2. 
	//
	// Note: this parameter is ignored for IOCTL_VDSK_SET_DISK
	union{
		HANDLE hDiskNotificationEvent;
		UINT32 Handle32; //for wow64
		UINT64 Handle64;
	};

} VDSK_DISK_PARAMETERS, *PVDSK_DISK_PARAMETERS;

// the result of disk device creation operation
typedef struct _VDSK_DISK_CREATE_RESULT
{
	// Device handle
	LONG DeviceID;	

}VDSK_DISK_CREATE_RESULT,*PVDSK_DISK_CREATE_RESULT;

//
// IOCTL_VDSK_START_DISK
//
typedef struct _VDSK_DISK_START_PARAMETERS
{
	// FALSE, if drive has no media
	// TRUE, otherwise
	LONG MediaInDevice;

}VDSK_DISK_START_PARAMETERS,*PVDSK_DISK_START_PARAMETERS;

// disk 'open' request parameters
typedef struct _VDSK_DISK_OPEN_PARAMETERS
{
	// Device handle, returned on device creation
	LONG DeviceID;

}VDSK_DISK_OPEN_PARAMETERS,*PVDSK_DISK_OPEN_PARAMETERS;

//disk information structure
typedef struct _VDSK_DISK_INFORMATION
{
	// device is read only
	LONG ReadOnly : 1;

	//media is removable
	LONG Removable : 1;

	// device is cd-rom
	LONG IsCDRom : 1;

	//device is floppy disk
	LONG IsFloppy : 1;

	//media in device after device creation
	LONG IsMediaInDevice : 1;

	//device deletion is in progress
	LONG		DeletePending : 1;

	// disk io is disabled,
	// we terminate disk io thread and cancel all pending requests
	LONG		IsOffline : 1;

	//volume link within mount mgr
	LONG		IsVolumeLinkCreated : 1;

	// the number of mount mgr links/ mount points
	LONG		LinksCount;

	//////////////////////////////////////////////////////////////////////////
	// disk interface

	// Specifies the number of the disk that contains this extent. 
	// This is the same disk number that is used to construct the name of the disk 
	// (for example, PhysicalDriveX or HarddiskX, where X is the disk number). 
	// for IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS
	ULONG DiskNumber;

	//Specifies one of the system-defined FILE_DEVICE_XXX 
	// FILE_DEVICE_DISK, FILE_DEVICE_CD_ROM, FILE_DEVICE_VIRTUAL_DISK, 
	DEVICE_TYPE DeviceType;

	//
	// Drive geometry
	//

	// Drive size in bytes 
	LARGE_INTEGER DiskSize;

	//Indicates the number of tracks in a cylinder.
	ULONG TracksPerCylinder;

	//Indicates the number of sectors in each track. 
	ULONG SectorsPerTrack;

	//Indicates the number of bytes in a disk sector.
	ULONG BytesPerSector;

	//////////////////////////////////////////////////////////////////////////
	//

	// last disk notification event
	LONG		LastDiskNotificationEvent; //VDSK_DISK_EVENT_DEADLOCK

	//////////////////////////////////////////////////////////////////////////
	// Disk statistics

	// number of read operations
	LARGE_INTEGER IoReadCount;

	//the number of bytes read
	LARGE_INTEGER IoReadBytes;

	// number of write operations
	LARGE_INTEGER IoWriteCount;

	// number of write bytes
	LARGE_INTEGER IoWriteBytes;

	// number of failed read operations
	LARGE_INTEGER IoReadFailed;

	// number of failed write operations
	LARGE_INTEGER IoWriteFailed;

	// device volume link in mount mgr
	WCHAR VolumeLinkName[VDSK_VOLUME_LINK_SIZE/sizeof(WCHAR)];

}VDSK_DISK_INFORMATION,*PVDSK_DISK_INFORMATION;

//
// Provides information about all mounted disks
//
typedef struct _VDSK_ALL_INFORMATION
{
	// number of disks
	ULONG Number;

	// array of disk information structures
	VDSK_DISK_INFORMATION DiskInformation[0];
	
}VDSK_ALL_INFORMATION,*PVDSK_ALL_INFORMATION;

//
// disk IO operation identifiers
//
typedef enum _VDSK_IO_OP
{
	IO_OP_NONE,  // nothing
	IO_OP_READ,  // disk IO read
	IO_OP_WRITE, // disk IO write

	IO_OP_FORMAT_TRACKS, // format tracks requests (for floppy only), optional
	IO_OP_NOTIFY_EJECT   // media eject request, optional

}VDSK_IO_OP,*PVDSK_IO_OP;

//
// user operation status codes
//

// operation completed successfully
#define IO_OP_STATUS_SUCCESS           0L

// operation failed
#define IO_OP_STATUS_UNSUCCESSFUL         1L

// operation is not implemented 
// use default processing handler
// valid for IO_OP_FORMAT_TRACKS, IO_OP_NOTIFY_EJECT
#define IO_OP_STATUS_NOT_IMPLEMENTED   2L

typedef struct _VDSK_IO_OPEARTION
{
	// user io operation
	VDSK_IO_OP Operation;

	union{
		// IO_OP_READ
		struct {
			// specifies if it's paging io operation
			// user app MUST NOT use any file IO operations diring this request handling
			LONG PagingIo;

			// Length in bytes of the data to be read. 
			// If the read operation is successful, 
			// the number of bytes read is returned in the BytesProcessed member 
			ULONG Length;

			// LARGE_INTEGER variable that specifies the starting byte 
			// offset within the file of the data to be read. 
			LARGE_INTEGER ByteOffset;
		}Read;

		// IO_OP_WRITE
		struct {
			// specifies if it's paging io operation
			// user app MUST NOT use any file IO operations diring this request handling
			LONG PagingIo;

			// Length in bytes of the data to be written. 
			// If the read operation is successful, 
			// the number of bytes written is returned in the BytesProcessed member 
			ULONG Length;

			// LARGE_INTEGER variable that specifies the starting byte 
			// offset within the file of the data to be written. 
			LARGE_INTEGER ByteOffset;
		}Write;

		//IO_OP_FORMAT_TRACKS
		struct {
			// Indicates format information, such as the disk size and the number 
			// of bytes per sector. For a list of the values that can be assigned 
			// to this member, see MEDIA_TYPE in WDK.
			MEDIA_TYPE MediaType;

			//Indicates the number of the cylinder where the formatting should begin.
			ULONG StartCylinderNumber;

			//Indicates the number of the cylinder where the formatting should end. 
			ULONG EndCylinderNumber;

			//Indicates the number of the head where the formatting should begin. 
			ULONG StartHeadNumber;

			//Indicates the number of the head where the formatting should end. 
			ULONG EndHeadNumber;

		}Format;		
	};

	// operation sequence number
	LONG OpeartionID;

	//operation result
	LONG Status; //IO_OP_STATUS_XXX

	//actual bytes r/w
	ULONG BytesProcessed;

	// operation data mapped to user application
	union{
		CHAR* DataBuffer;
		UINT32 DataBuffer32; //for wow64
		UINT64 DataBuffer64;
	};

#ifndef _VDSK_MAP_USER_MEMORY
	ULONG DataBufferSize;
#endif //_VDSK_MAP_USER_MEMORY

}VDSK_IO_OPEARTION,*PVDSK_IO_OPEARTION;

//Main device I/O controls
#define _VDSK_CTRL_CODE(_FUNC) \
	CTL_CODE(FILE_DEVICE_VDSK, IOCTL_VDSK_BASE + (_FUNC), METHOD_BUFFERED, FILE_ANY_ACCESS )

//
// driver version
//
#define IOCTL_VDSK_VERSION  _VDSK_CTRL_CODE(1)

//
// create new virtual disk associated with the current thread
//
#define IOCTL_VDSK_CREATE_DISK  _VDSK_CTRL_CODE(2)

//
// create new virtual disk associated with the current thread
//
#define IOCTL_VDSK_START_DISK  _VDSK_CTRL_CODE(3)

//
// open virtual disk by id
//
#define IOCTL_VDSK_OPEN_DISK  _VDSK_CTRL_CODE(4)

//
// unmount virtual disk drive
//
#define IOCTL_VDSK_DELETE_DISK _VDSK_CTRL_CODE(5)

//
// virtual disk drive ejection control
//
#define IOCTL_VDSK_INJECT_DISK _VDSK_CTRL_CODE(6)

//
// query virtual disk information
//
#define IOCTL_VDSK_QUERY_DISK _VDSK_CTRL_CODE(7)

//
// set virtual disk information
//
#define IOCTL_VDSK_SET_DISK _VDSK_CTRL_CODE(8)

//
// query all virtual disks information
//
#define IOCTL_VDSK_QUERY_INFORMATION _VDSK_CTRL_CODE(9)

//
// disk IO operation
// on call it provides results of the previous operation
// on return it contains the parameters of the next operation
//
#define IOCTL_VDSK_DISK_IO_OP _VDSK_CTRL_CODE(10)

#endif //__VDSKCMD_H_