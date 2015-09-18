// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CacheThread.h"
#include "main.h"
#include <vector>
#include "functions.h"
#pragma package(smart_init)
#include <algorithm>
using namespace std;

extern LONGLONG totcachsize;
extern vector<CACHE>cache;

extern TEvent *cev;
extern int unsaved;
extern TCriticalSection *cchs;
       extern TCriticalSection *ccs;
__fastcall CacheThread::CacheThread(bool CreateSuspended):TThread(CreateSuspended)
{
}

bool sortcachebyopid(CACHE c1, CACHE c2)
{
    return c1.opid < c2.opid;
}

void __fastcall CacheThread::Do(void)
{

    if(!ccs->TryEnter())
        return;

    sort(cache.begin(), cache.end(), sortcachebyopid);

    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        if ((*it).unsaved && ((GetTickCount() - (*it).changed) > 3000))
        {

            LARGE_INTEGER li;
            li.QuadPart = (*it).offset;

            paintdisk(5, li, (*it).len, 0, 0);

            double ms;
            ms = GetTickCount();

            unsaved = 0;
            for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
                if ((*it).unsaved)
                    unsaved++;

            char str[129];
            sprintf(str, "%4d ���.", unsaved-1);

            nwrite((ULONG)(*it).buf, (*it).offset, (*it).len, 0, true, 0, str);

            li.QuadPart = 0;
            paintdisk(4, li, 1, 0, 0);
            // HeapFree(GetProcessHeap(), 0, (*it).buf);

            ms = (GetTickCount() - ms) /1000.0;
            deb("  * ���: ��������� %5lu @ %8lu, opid: %d �� %.2f ���", (*it).len, (*it).offset,
                (*it).opid, ms);
            // deb("     ��������� �� %.2f ���", ms);
            // totcachsize-=(*it).len;
            // it = cache.erase(it);
            (*it).unsaved = false;
            (*it).changed = GetTickCount();
            // if(it == cache.end())
            // if (cev->WaitFor(0) == wrSignaled)
            // {
            // deb("���������� ���� �������� ���� ������� � �����");
            break;
            // }
        }
    }
    ccs->Leave();
}

// ---------------------------------------------------------------------------
void __fastcall CacheThread::Execute()
{
    NameThreadForDebugging("CacheThread");

    Sleep(1000);
    while (1)
    {
        // cev->ResetEvent();
        Sleep(1);

        // if (cev->WaitFor(0) == wrSignaled)
        // continue;

        // cev->ResetEvent();

        // Synchronize(Do);
        Do();
    }
}

// ---------------------------------------------------------------------------
