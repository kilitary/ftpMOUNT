/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2000-2010 NT KERNEL RESOURCES, All Rights Reserved.     */
/* http://www.ntkernel.com                                               */
/*                                                                       */
/* Module Name:  vdskapi.h                                               */
/*                                                                       */
/* Abstract: This is the include file that defines Virtual Disk SDK      */
/*           interface.                                                  */
/*                                                                       */
/* Environment:                                                          */
/*                                                                       */
/*   User mode, Virtual Disk SDK                                         */
/*                                                                       */
/* Revision History:                                                     */
/*                                                                       */
/*************************************************************************/

#define DEVICE_TYPE ULONG
typedef enum _MEDIA_TYPE {
    Unknown,                // Format is unknown
    F5_1Pt2_512,            // 5.25", 1.2MB,  512 bytes/sector
    F3_1Pt44_512,           // 3.5",  1.44MB, 512 bytes/sector
    F3_2Pt88_512,           // 3.5",  2.88MB, 512 bytes/sector
    F3_20Pt8_512,           // 3.5",  20.8MB, 512 bytes/sector
    F3_720_512,             // 3.5",  720KB,  512 bytes/sector
    F5_360_512,             // 5.25", 360KB,  512 bytes/sector
    F5_320_512,             // 5.25", 320KB,  512 bytes/sector
    F5_320_1024,            // 5.25", 320KB,  1024 bytes/sector
    F5_180_512,             // 5.25", 180KB,  512 bytes/sector
    F5_160_512,             // 5.25", 160KB,  512 bytes/sector
    RemovableMedia,         // Removable media other than floppy
    FixedMedia,             // Fixed hard disk media
    F3_120M_512,            // 3.5", 120M Floppy
    F3_640_512,             // 3.5" ,  640KB,  512 bytes/sector
    F5_640_512,             // 5.25",  640KB,  512 bytes/sector
    F5_720_512,             // 5.25",  720KB,  512 bytes/sector
    F3_1Pt2_512,            // 3.5" ,  1.2Mb,  512 bytes/sector
    F3_1Pt23_1024,          // 3.5" ,  1.23Mb, 1024 bytes/sector
    F5_1Pt23_1024,          // 5.25",  1.23MB, 1024 bytes/sector
    F3_128Mb_512,           // 3.5" MO 128Mb   512 bytes/sector
    F3_230Mb_512,           // 3.5" MO 230Mb   512 bytes/sector
    F8_256_128,             // 8",     256KB,  128 bytes/sector
    F3_200Mb_512,           // 3.5",   200M Floppy (HiFD)
    F3_240M_512,            // 3.5",   240Mb Floppy (HiFD)
    F3_32M_512              // 3.5",   32Mb Floppy
} MEDIA_TYPE, *PMEDIA_TYPE;

#ifndef __VDSKAPI_H_
#define __VDSKAPI_H_

#ifdef VDSKAPI_EXPORTS
	#define VDSK_API __declspec(dllexport)
#else
	#define VDSK_API __declspec(dllimport)
#endif

//
// vdskapi private error code that means that
// library was not initialized properly
//
#define ERROR_NOT_INITIALIZED 0xC0001001L

// virtual disk format flags
typedef enum _VDSK_FS
{
	VDSK_FS_FAT   = 0,
	VDSK_FS_FAT32 = 1,
	VDSK_FS_NTFS  = 2
}VDSK_FS,*PVDSK_FS;

#ifdef __cplusplus
extern "C" {
#endif

#include "vdskcmd.h"

//***********************************************************************************
// Name: PVDSK_DISK_ON_READ
//
// Routine Description:
// 
//   User defined disk read operation handler.
//
// Return Value:
// 
//     Operation status.
//
// Parameters:
//  hDisk     - Handle of the virtual disk drive.
//  Context   - Pointer to the user provided context value that was passed to 
//              VdskCreateVirtualDisk function
//  Buffer    - Pointer to a caller-allocated buffer that receives the data read from 
//	            the disk.
//  Length    - The size, in bytes, of the buffer pointed to by Buffer.
//  ByteOffset - specifies the starting byte offset within the disk of the data 
//	             to be read.
//  PagingIo  - Specifies if the read operation handles the paging IO.
//  BytesRead - A pointer to the variable that receives the number of bytes read.
// Note:
//  1.All files, access to which is done from callbacks, should be opened with 
//    FILE_FLAG_NO_BUFFERING flag. 
//  2. APCs (async io operations) should  not be performed. 
//  Failure to comply with rules could lead to system-wide deadlocks.
//  
//***********************************************************************************
typedef
DWORD
(__stdcall *PVDSK_DISK_ON_READ)(
	IN HANDLE hDisk,
	IN PVOID Context,
	IN PVOID Buffer,
	IN ULONG Length,
	IN LARGE_INTEGER ByteOffset,
	IN BOOL PagingIo,
	OUT PULONG BytesRead
	);

//***********************************************************************************
// Name: PVDSK_DISK_ON_WRITE
//
// Routine Description:
// 
//   User defined disk write operation handler.
//
// Return Value:
// 
//     Operation status.
//
// Parameters:
//  hDisk     - Handle of the virtual disk drive.
//  Context   - Pointer to the user provided context value that was passed to 
//              VdskCreateVirtualDisk function
//  Buffer    - A pointer to the buffer containing the data to be written.
//  Length    - The size, in bytes, of the buffer pointed to by Buffer.
//  ByteOffset - specifies the starting byte offset within the disk of the data 
//	             to be written.
//  PagingIo  - Specifies if the write operation handles the paging IO.
//  BytesWritten - A pointer to the variable that receives the number of bytes written. 
// Note:
//  1.All files, access to which is done from callbacks, should be opened with 
//    FILE_FLAG_NO_BUFFERING flag. 
//  2. APCs (async io operations) should  not be performed. 
//  Failure to comply with rules could lead to system-wide deadlocks.
//  
//***********************************************************************************
typedef
DWORD
(__stdcall *PVDSK_DISK_ON_WRITE)(
	IN HANDLE hDisk,
	IN PVOID Context,
	IN PVOID Buffer,
	IN ULONG Length,
	IN LARGE_INTEGER ByteOffset,
	IN BOOL PagingIo,
	OUT PULONG BytesWritten
	);

//***********************************************************************************
// Name: PVDSK_DISK_ON_FORMAT_TRACKS
//
// Routine Description:
// 
//   media format tracks operation callback.
//
// Return Value:
// 
//     Operation status.
//
// Parameters:
//  hDisk     - Handle of the virtual disk drive.
//  Context   - Pointer to the user provided context value that was passed to 
//              VdskCreateVirtualDisk function
//  MediaType - Indicates format information, such as the disk size and the 
//              number of bytes per sector. For a list of the values that can 
//              be assigned to this member, see MEDIA_TYPE. 
//  StartCylinderNumber - Indicates the number of the cylinder where the 
//                        formatting should begin. 
//  EndCylinderNumber   - Indicates the number of the cylinder where the formatting 
//                        should end. 
//  StartHeadNumber     - Indicates the number of the head where the formatting 
//                        should begin. 
//  EndHeadNumber       - Indicates the number of the head where the formatting 
//                        should end.
//  FormatEx            - TRUE, if system sent IOCTL_DISK_FORMAT_TRACKS_EX request.
//  SectorsPerTrack     - Indicates the number of sectors per track.
//
// Note:
//  Track format requests support is optional. It's floppy disk specific. You need to 
//  handle floppy media format requests. The default implementation does nothing and
//  returns status success to the operating system.
//***********************************************************************************
typedef
DWORD
(__stdcall *PVDSK_DISK_ON_FORMAT_TRACKS)(
	IN HANDLE hDisk,
	IN PVOID Context,
	IN MEDIA_TYPE MediaType,
	IN ULONG StartCylinderNumber,
	IN ULONG EndCylinderNumber,
	IN ULONG StartHeadNumber,
	IN ULONG EndHeadNumber
	);

//***********************************************************************************
// Name: PVDSK_DISK_ON_EJECT
//
// Routine Description:
// 
//   media eject operation callback
//
// Return Value:
// 
//     Operation status.
//
// Parameters:
//  hDisk     - Handle of the virtual disk drive.
//  Context   - Pointer to the user provided context value that was passed to 
//              VdskCreateVirtualDisk function
// Note:
//  Media eject support is optional. The default implementation marks that
//  there is no media in device and return status success to the operating system.
//***********************************************************************************
typedef
DWORD
(__stdcall *PVDSK_DISK_ON_EJECT)(
	IN HANDLE hDisk,
	IN PVOID Context	
	);

//***********************************************************************************
// Name: PVDSK_DISK_ON_FS_FORMAT
//
// Routine Description:
// 
//   filesystem format callback
//
// Return Value:
// 
//     Operation status.
//
// Parameters:
//  hDisk     - Handle of the virtual disk drive.
//  Progress  - formating progress in per cent, or zero
//              zero means 'one' step progress, reported by system.
//  Context   - Pointer to the user provided context value that was passed to 
//              VdskCreateVirtualDisk function
// Note:
//  Filesystem format support is optional. The default implementation returns TRUE
//  to continue the media formatting.
//***********************************************************************************
typedef
BOOL
(__stdcall *PVDSK_DISK_ON_FS_FORMAT)(
	IN HANDLE hDisk,
	IN DWORD Progress, // progress in per cent
	IN PVOID Context
	);

//***********************************************************************************
// Name: PVDSK_DISK_ON_NOTIFICATION
//
// Routine Description:
// 
//   vdskapi call this callback to notify user about disk events.
//
// Return Value:
// 
//    None.
//
// Parameters:
//  hDisk     - Handle of the virtual disk drive.
//  DiskEvent - Specifies disk event. The following values are defined:
//              VDSK_DISK_EVENT_DEADLOCK - disk io deadlock was detected.
//  Context   - Pointer to the user provided context value that was passed to 
//              VdskCreateVirtualDisk function
// Note:
//  Disk notification support is optional.
//  If deadlock occurred disk goes to 'offine' state, all the pending disk IO is 
//  automatically canceled, all new requests are rejected with status 'device is not
//  ready'. To restart the disk IO call VdskStartDisk function.
//***********************************************************************************
// disk notification event callback
typedef
VOID
(__stdcall *PVDSK_DISK_ON_NOTIFICATION)(
	IN HANDLE hDisk,
	IN LONG DiskEvent,
	IN PVOID Context
	);

VDSK_API
BOOL
__stdcall
	VdskInitializeLibrary(
		VOID
		);

VDSK_API
BOOL
__stdcall
	VdskFreeLibrary(
		VOID
		);

VDSK_API
DWORD
__stdcall
	VdskGetDriverVersion(
		VOID
		);

VDSK_API
HANDLE
__stdcall
	VdskCreateVirtualDisk(
		IN LARGE_INTEGER DiskSize,  //virtual disk size in bytes
		IN ULONG TracksPerCylinder, //Indicates the number of tracks in a cylinder.
		IN ULONG SectorsPerTrack,   //Indicates the number of sectors in each track. 
		IN ULONG BytesPerSector,    //Indicates the number of bytes in a disk sector.
		IN ULONG DeviceFlags,       // device flags
		IN int DiskIoTimeout OPTIONAL, // Disk IO operation timeout 
		IN PVDSK_DISK_ON_READ OnDiskRead,       // virtual disk read callback
		IN PVDSK_DISK_ON_WRITE OnDiskWrite,     // virtual disk write callback
		IN PVDSK_DISK_ON_FORMAT_TRACKS OnDiskFormatTracks OPTIONAL, // virtual media format callback
		IN PVDSK_DISK_ON_EJECT OnDiskEject OPTIONAL,                // virtual disk eject callback
		IN PVDSK_DISK_ON_NOTIFICATION OnDiskNotification OPTIONAL,
		IN PVOID Context OPTIONAL                                     // user provided context
		);

VDSK_API
BOOL
__stdcall
	VdskStartDisk(
		IN HANDLE hDisk,
		IN BOOL MediaInDevice
		);

VDSK_API
BOOL
__stdcall
	VdskDeleteDisk(
		IN HANDLE hDisk
		);

VDSK_API
BOOL
__stdcall
	VdskQueryDisk(
		IN HANDLE hDisk,
		IN OUT PVDSK_DISK_INFORMATION DiskInformation
		);

VDSK_API
BOOL
__stdcall
	VdskSetDisk(
		IN HANDLE hDisk,
		IN LARGE_INTEGER DiskSize,  //virtual disk size in bytes
		IN ULONG TracksPerCylinder, //Indicates the number of tracks in a cylinder.
		IN ULONG SectorsPerTrack,   //Indicates the number of sectors in each track. 
		IN ULONG BytesPerSector,    //Indicates the number of bytes in a disk sector.
		IN BOOL ReadOnly,
		IN BOOL Removable,
		IN BOOL IsMediaInDevice
		);

VDSK_API
BOOL
__stdcall
	VdskMountDisk(
		IN HANDLE hDisk,
		IN CHAR DriveLetter,
		IN BOOL MountGlobal
		);

VDSK_API
BOOL
__stdcall
	VdskMountDiskExW(
		IN HANDLE hDisk,
		IN LPWSTR DiskMountPoint
		);

VDSK_API
BOOL
__stdcall
	VdskMountDiskExA(
		IN HANDLE hDisk,
		IN LPSTR DiskMountPoint
		);


#ifdef UNICODE
#define VdskMountDiskEx  VdskMountDiskExW
#else
#define VdskMountDiskEx  VdskMountDiskExA
#endif // !UNICODE

VDSK_API
BOOL
__stdcall
	VdskUnmountDisk(
		IN HANDLE hDisk,
		IN CHAR DriveLetter
		);

VDSK_API
BOOL
__stdcall
	VdskUnmountDiskExW(
		IN HANDLE hDisk,
		IN LPWSTR DiskMountPoint
		);

VDSK_API
BOOL
__stdcall
	VdskUnmountDiskExA(
		IN HANDLE hDisk,
		IN LPSTR DiskMountPoint
		);

#ifdef UNICODE
#define VdskUnmountDiskEx  VdskUnmountDiskExW
#else
#define VdskUnmountDiskEx  VdskUnmountDiskExA
#endif // !UNICODE

VDSK_API
BOOL
__stdcall
	VdskFormatDiskW( 
		IN HANDLE hDisk,  
		IN VDSK_FS FsType, // fat, fat32 or ntfs
		IN LPWSTR DiskLabel, //OPTIONAL
		IN BOOL QuickFormat, //OPTIONAL
		IN DWORD ClusterSize,
		IN PVDSK_DISK_ON_FS_FORMAT FsFormatCallback,
		IN PVOID Context
		);

VDSK_API
BOOL
__stdcall
	VdskFormatDiskA( 
		IN HANDLE hDisk,  
		IN VDSK_FS FsType, // fat, fat32 or ntfs
		IN LPSTR DiskLabel, //OPTIONAL
		IN BOOL QuickFormat, //OPTIONAL
		IN DWORD ClusterSize,
		IN PVDSK_DISK_ON_FS_FORMAT FsFormatCallback,
		IN PVOID Context
		);

#ifdef UNICODE
#define VdskFormatDisk  VdskFormatDiskW
#else
#define VdskFormatDisk  VdskFormatDiskA
#endif // !UNICODE

VDSK_API
BOOL
__stdcall
	VdskEjectMedia( 
		IN HANDLE hDisk
		);

VDSK_API
BOOL
__stdcall
	VdskInstallDriverW(
		IN PWSTR DriverBinaryPath,
		IN BOOL DemandStart
		);

VDSK_API
BOOL
__stdcall
	VdskInstallDriverA(
		IN PSTR DriverBinaryPath,
		IN BOOL DemandStart
		);

#ifdef UNICODE
#define VdskInstallDriver  VdskInstallDriverW
#else
#define VdskInstallDriver  VdskInstallDriverA
#endif // !UNICODE

VDSK_API
BOOL
__stdcall
	VdskStartDriver(
		VOID
		);

VDSK_API
BOOL
__stdcall
	VdskStopDriver(
		VOID
		);

VDSK_API
BOOL
__stdcall
	VdskUninstallDriver(
		VOID
		);

VDSK_API
BOOL
__stdcall
	VdskIsDriverRunning(
		VOID
		);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class VDSK_API CVirtualDisk
{
protected:
	HANDLE m_hDevice;
	LARGE_INTEGER m_DiskSize;
	DWORD m_TracksPerCylinder;
	DWORD m_SectorsPerTrack;
	DWORD m_BytesPerSector;

	BOOL m_ReadOnly;
	BOOL m_Removable;
	BOOL m_IsCDRom;
	BOOL m_IsFloppy;
	BOOL m_IsMediaInDevice;

private:
	bool Update();

	//////////////////////////////////////////////////////////////////////////
	// internal callbacks handlers

	// Disk read event handler
	static DWORD __stdcall _OnDiskRead(
		IN HANDLE hDisk,
		IN PVOID Context,
		IN PVOID Buffer,
		IN ULONG Length,
		IN LARGE_INTEGER ByteOffset,
		IN BOOL PagingIo,
		OUT PULONG BytesRead
		);

	// Disk write event handler
	static DWORD __stdcall _OnDiskWrite(
		IN HANDLE hDisk,
		IN PVOID Context,
		IN PVOID Buffer,
		IN ULONG Length,
		IN LARGE_INTEGER ByteOffset,
		IN BOOL PagingIo,
		OUT PULONG BytesWritten
		);

	// Disk format tracks event handler
	static DWORD __stdcall _OnDiskFormatTracks(
		IN HANDLE hDisk,
		IN PVOID Context,
		IN MEDIA_TYPE MediaType,
		IN ULONG StartCylinderNumber,
		IN ULONG EndCylinderNumber,
		IN ULONG StartHeadNumber,
		IN ULONG EndHeadNumber
		);

	// Disk media eject event handler
	static DWORD __stdcall _OnDiskEject(
		IN HANDLE hDisk,
		IN PVOID Context
		);

	// Disk fs format callback
	static BOOL __stdcall _OnDiskFsFormatCallback(
		IN HANDLE hDisk,
		IN DWORD Progress, // progress in per cent
		IN PVOID Context
		);

	static void __stdcall 
		_OnDiskNotificationCallback(
			IN HANDLE hDisk,
			IN LONG DiskEvent,
			IN PVOID Context
			);

	//callbacks
protected:

	// disk read operation handler
	// this method is mandatory
	// you must implement it in your application
	virtual DWORD 
		OnDiskRead(
			IN PVOID Buffer,
			IN ULONG Length,
			IN LARGE_INTEGER ByteOffset,
			IN BOOL PagingIo,
			OUT PULONG BytesRead
			) = 0;

	// disk write operation handler
	// this method is mandatory
	// you must implement it in your application
	virtual DWORD 
		OnDiskWrite(
			IN PVOID Buffer,
			IN ULONG Length,
			IN LARGE_INTEGER ByteOffset,
			IN BOOL PagingIo,
			OUT PULONG BytesWritten
			) = 0;

	// disk format tracks operation handler
	//  Track format requests support is optional. It's floppy disk specific. You need to 
	//  handle floppy media format requests. The default implementation does nothing and
	//  returns status success to the operating system.
	virtual DWORD 
		OnDiskFormatTracks(
			IN MEDIA_TYPE MediaType,
			IN ULONG StartCylinderNumber,
			IN ULONG EndCylinderNumber,
			IN ULONG StartHeadNumber,
			IN ULONG EndHeadNumber
			);

	// disk media eject operation handler
	//  Media eject support is optional. The default implementation marks that
	//  there is no media in device and return status success to the operating system.
	virtual DWORD OnDiskEject();

	// disk filesystem format event handler
	// Note:
	//  Filesystem format support is optional. The default implementation returns TRUE
	//  to continue the media formatting.
	virtual BOOL OnDiskFsFormat(IN DWORD Progress);

	// disk notifications handler
	// Note:
	//  Disk notification support is optional.
	virtual void OnDiskNotification(IN LONG DiskEvent);

	// attributes
public:

	// virtual disk size in bytes.
	LARGE_INTEGER get_DiskSize();
	void set_DiskSize( LARGE_INTEGER DiskSize);

	/// the number of tracks in a cylinder.
	int get_TracksPerCylinder();
	void set_TracksPerCylinder(int TracksPerCylinder);

	// the number of sectors in each track.
	int get_SectorsPerTrack();
	void set_SectorsPerTrack( int SectorsPerTrack);

	// the number of bytes in a disk sector.
	int get_BytesPerSector();
	void set_BytesPerSector( int BytesPerSector);

	// device media is read-only
	bool get_ReadOnly();
	void set_ReadOnly( bool ReadOnly);

	// device media is removable
	bool get_Removable();
	void set_Removable( bool Removable);

	// device is CD-ROM
	bool get_IsCDRom();
	void set_IsCDRom( bool IsCDRom);

	// device is floppy drive
	bool get_IsFloppy();
	void set_IsFloppy( bool IsFloppy);

	// there is media in device
	bool get_IsMediaInDevice();
	void set_IsMediaInDevice( bool IsMediaInDevice);

	//methods
public:

	// constructor
	CVirtualDisk();

	// creates new virtual disk/cd or floppy drive.
	bool CreateDisk( int IoTimeout );
	bool CreateDisk();

	// Re-starts virtual disk drive if it's in 'offline' mode because of the IO errors.
	bool StartDisk(bool MediaInDevice);

	// Destroys virtual disk drive created with CreateDisk method.
	bool Delete();

	// Returns information about the virtual disk, if it was created with CreateDisk method.
	bool Query(PVDSK_DISK_INFORMATION DiskInformation);

	// Modifies set of disk parameters as one operation
	bool Set(
		IN LARGE_INTEGER DiskSize,  //virtual disk size in bytes
		IN ULONG TracksPerCylinder, //Indicates the number of tracks in a cylinder.
		IN ULONG SectorsPerTrack,   //Indicates the number of sectors in each track. 
		IN ULONG BytesPerSector,    //Indicates the number of bytes in a disk sector.
		IN BOOL ReadOnly,
		IN BOOL Removable,
		IN BOOL IsMediaInDevice
		);

	// Mounts virtual disk drive to the letter.
	bool Mount(char Letter);

	// Mounts virtual disk drive to the directory on the filesystem.
	bool Mount(LPWSTR strPath);

	// Unmounts virtual disk drive from the letter.
	bool Unmount(char Letter);

	// Unmounts the volume from the specified volume mount point.
	bool Unmount(LPWSTR strPath);

	// Ejects media from virtual disk drive.
	bool Eject();

	// format media with filesystem
	bool Format( 
		IN VDSK_FS FsType, // fat, fat32 or ntfs
		IN LPWSTR DiskLabel, //OPTIONAL
		IN BOOL QuickFormat, //OPTIONAL
		IN DWORD ClusterSize
		);

	// returns true if the virtual disk drive created with CreateDisk method.
	bool IsCreated();

	//////////////////////////////////////////////////////////////////////////
	// statics

	// Returns current driver version.
	static int GetDriverVersion();

	//   Installs Virtual Disk SDK core driver.
	// Parameters:
	//  DriverFileName - driver binary path on the file system. 
	//                   This parameters can be NULL. 
	//                   In this case driver's binary should be located at 
	//                   <SystemRoot>\system32\drivers</param>
	//  DemandStart    - TRUE if Virtual Disk SDK core driver will be started manually
	//                   FALSE if Virtual Disk SDK core driver will start while system starts.
	static bool InstallDriver(LPWSTR DriverFileName, bool DemandStart);

	// Starts Virtual Disk SDK core driver 
	static bool StartDriver();

	// Stops Virtual Disk SDK core driver
	static bool StopDriver();

	// Uninstalls Virtual Disk SDK core driver 
	static bool UninstallDriver();

	// Returns TRUE if driver is running.
	static bool IsDriverRunning();
};
#endif

#endif //__VDSKAPI_H_