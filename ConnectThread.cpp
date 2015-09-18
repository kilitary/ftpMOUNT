// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ConnectThread.h"
#include "CacheThread.h"
#include "vdskapi.h"
#include "main.h"
#include <vector>
#include "functions.h"
#pragma package(smart_init)
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
extern double disksize;
extern LPVOID memory;
extern HANDLE hdisk;
#pragma package(smart_init)
// ---------------------------------------------------------------------------

// Important: Methods and properties of objects in VCL can only be
// used in a method called using Synchronize, for example:
//
// Synchronize(&UpdateCaption);
//
// where UpdateCaption could look like:
//
// void __fastcall ConnectThread::UpdateCaption()
// {
// Form1->Caption = "Updated in a thread";
// }
// ---------------------------------------------------------------------------

__fastcall ConnectThread::ConnectThread(bool CreateSuspended):TThread(CreateSuspended)
{
}

// ---------------------------------------------------------------------------
void __fastcall ConnectThread::Execute()
{
    NameThreadForDebugging("ConnectThread");
    // ---- Place thread code here ----

    LARGE_INTEGER xyu;
    // deb("������������ ...");
    xyu.QuadPart = 0;
    // paintdisk(2, xyu, 10, false);
    // paintdisk(4, xyu, 10, false);
    // Form1->deblog->Clear();
    if (!disksize)
        disksize = 100*1024*1024*1024;
    // deb("disksize = %.0f (%.0f MB)", disksize, disksize/1024/1024);
    extern SOCKET ConnectSocket;

    extern bool ramdisk;
    shutdown(ConnectSocket, SD_BOTH);
    closesocket(ConnectSocket);
    ConnectSocket = NULL;
    if (!ramdisk&&!nconn())
    {
        deb("������ ���������� � ��������.");
        return;
    }



//    if (!memory && disksize)
//    {
//        memory = (LPVOID)new char[disksize];
//        if (!memory)
//        {
//            deb("failed to alloc %d bytes of memory for disk", disksize);
//            return;
//        }
//        deb(" ��������  %.0f ���� (%.0f MB)  ��� ����� @ 0x%08p",disksize,
//            disksize/1024/1024, memory);
//    }
    
    // DWORD dwId;
    // CreateThread(NULL, 0, makefatdisk, NULL, NULL, &dwId);
    //
    // MakeFAT *x;
    // x = new MakeFAT(false);
    // prefetch();

    Form1->Button1->Enabled = false;
    // ShellExecute(NULL, "open", "y:\\", NULL, NULL, SW_SHOW);

    Form1->makefatdisk(NULL);
}
// ---------------------------------------------------------------------------
