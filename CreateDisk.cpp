// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CreateDisk.h"

#include "ConnectThread.h"
#include "CacheThread.h"
#include "vdskapi.h"
#include "main.h"
#include <vector>
#include "functions.h"

#include <algorithm>
#include <string.h>
#include <vector>
#include "dumpthread.h"
#include "cachethread.h"
#include "MakeFATThread.h"
#include <windows.h>
#include <Winsock2.h>
using namespace std;

extern ULONG totcachsize;
extern vector<CACHE>cache;
extern TCriticalSection *ccs;
extern TEvent *cev;
extern int unsaved;
extern TCriticalSection *cchs;
extern LONGLONG disksize;
extern LPVOID memory;
extern HANDLE hdisk;
extern bool unmounting;
extern LONGLONG clstsize;
extern SOCKET ConnectSocket;
extern LONGLONG curdisksize;
extern double maxquad;
#pragma package(smart_init)
// ---------------------------------------------------------------------------

// Important: Methods and properties of objects in VCL can only be
// used in a method called using Synchronize, for example:
//
// Synchronize(&UpdateCaption);
//
// where UpdateCaption could look like:
//
// void __fastcall CreateDisk::UpdateCaption()
// {
// Form1->Caption = "Updated in a thread";
// }
// ---------------------------------------------------------------------------

__fastcall CreateDisk::CreateDisk(bool CreateSuspended):TThread(CreateSuspended)
{
}

// ---------------------------------------------------------------------------
void __fastcall CreateDisk::Execute()
{
    NameThreadForDebugging("CreateDisk");
    // ---- Place thread code here ----
    LARGE_INTEGER size;
    unmounting = false;
    memset(&size, 0, sizeof(size));
    size.QuadPart = (LONGLONG)32*1024*1024*1024;
    disksize = (LONGLONG)size.QuadPart;

    // maxquad = (double)curdisksize;
    deb("creating disk %I64d/%lu..", size.QuadPart, (unsigned long)curdisksize);
    hdisk = VdskCreateVirtualDisk(size, 2, 32, 0, VDSK_DISK_REMOVABLE|VDSK_DISK_MEDIA_IN_DEVICE,
        11300, OnRead, OnWrite, NULL, NULL, OnEvent, NULL);
    if (hdisk == INVALID_HANDLE_VALUE)
    {
        deb("VdskCreateVirtualDisk: %s", fmterr());
        deb("\r\n.\r\n.\r\n.\r\n             -- STOP -- Critical error.");

        closesocket(ConnectSocket);

        return;
    }

    if (VdskStartDisk(hdisk, true))
    {
        // deb("started drive");
    }
    // if(!VdskSetDisk(hdisk,size, 2, 32, 8192, 1, 1, 1)) {
    // deb("failed VdskSetDisk(): %s",fmterr());
    // }

    bool ret = VdskMountDisk(hdisk, 'y', true);
    if (!ret)
    {
        deb("VdskMountDisk: %s", fmterr());
        // delete[]memory;
        HeapFree(GetProcessHeap(), 0, memory);
        memory = NULL;
        Form1->Button1->Enabled = true;
        Form1->Button2->Enabled = false;
        return;
    }
    else
    {
        extern char ftphost[128];

        deunicode(Form1->ftpip->Text.c_str(), ftphost, 128);
        deb("���� Y:\\ (%lu GB)  ��������� � %s.", (unsigned long)(size.QuadPart/1024/1024/1024),
            ftphost);
    }
    Form1->ptr->Text = "���������";
    Application->ProcessMessages();

    // VdskMountDiskEx(hdisk, "e:\y");
}
// ---------------------------------------------------------------------------
