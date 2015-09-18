// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "statthread.h"
#include "vdskapi.h"
#include "main.h"

#include <stdio.h>
#include <vector>        #include <winsock2.h> #include <math.h>
#include <windows.h>
#include "main.h"
#include "functions.h"

#include <winsock2.h>

#include "formatthread.h"
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "dumpthread.h"
#include "cachethread.h"
#include "MakeFATThread.h"
#include "ConnectThread.h"
using namespace std;
#pragma package(smart_init)
extern char *ftphost;
extern HANDLE hdisk;
extern TForm1 *Form1;
DWORD heapsize;
extern vector<FATFILE>files;
extern int ftpport;
extern ULONG totcachsize;
extern vector<CACHE>cache;
extern TCriticalSection *ccs;
extern double avg, avg2;
extern unsigned long lastdelay;
extern unsigned long ftpgetfileoffset, ftpgetfilelen;
extern char ftpgetfilename[128];
extern unsigned long timeoutst;
// ---------------------------------------------------------------------------
int unsaved = 0;            extern int debnewmsgs,ftpnewmsgs;
extern char ip[128];
extern int ftplport;
extern int ftpstate;
extern unsigned int *diskcolor;
extern char ftpcmdhist[128], ftprlhist[128];
extern unsigned long lastpasvread;
extern TCriticalSection *cchs;
extern double ibytesin, ibytes_sttime, ibytes_st, ibps, ilastlen;
extern double obytesin, obytes_sttime, obytes_st, obps, olastlen;
extern char delaystr[128];
extern bool ramdisk;
extern unsigned long curdisksize;
extern LPVOID memory;
extern char lastftpcmd[1024];
extern char lastftprep[1024];
char s[111];
extern TCriticalSection *ftpcs;
DWORD rtms = 0;
char xyu[1111] = "unknown";
int prevftpstate = 0;
DWORD dms = 0;
// dms = GetTickCount()-dms;
char what[1024];
extern int ftpcs_entered;
extern char ftpcs_lockedat[1024];

__fastcall statthread::statthread(bool CreateSuspended):TThread(CreateSuspended)
{
}

void __fastcall statthread::xxyu(void)
{

    char str[1024];
    bool cu = false;
    extern SOCKET FtpSocket;
    if (!FtpSocket)
        ftpstate = -2;
    try
    {
        cu = ccs->TryEnter();
    }
    catch(Exception &e)
    {
        char str[222];
        deunicode(e.Message.c_str(), str, sizeof(str));
        deb("exception ccs->enter: %s", str);
    }

    if (cu)
    {
        unsaved = 0;
        for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
            if ((*it).unsaved)
                unsaved++;

        try
        {
            ccs->Leave();
        }
        catch(Exception &e)
        {
            char str[222];
            deunicode(e.Message.c_str(), str, sizeof(str));
            deb("exception ccs->Leave: %s", str);
        }
    }

    sprintf(str, "%lu ���� (%d M�)", (ULONG)totcachsize, (ULONG)totcachsize/1024/1024);
    Form1->cached->Text = str;

    if (cchs->TryEnter())
    {
        cchs->Leave();
        Form1->net->Text = " ��������";
        Form1->ptr->Text = "<��������>";
        Form1->ptr->Font->Color = clGray;
        // Application->ProcessMessages();
    }
    else
        Form1->net->Text = " ���� / ��������";
    // Application->ProcessMessages();

    sprintf(str, "�������: %3lu,  %d ������ ", (ULONG)cache.size(), unsaved);
    if (cu)
        Form1->cacheimg->Visible = false;
    else
        Form1->cacheimg->Visible = true;

    Form1->cacheblocks->Text = str;
    // Application->ProcessMessages();

    if ((GetTickCount() - obytes_sttime) >= 500)
    {
        obps = (obytesin-obytes_st-olastlen) * 8 / (0.5*1000);
        obytes_sttime = GetTickCount();
        obytes_st = obytesin;
        olastlen = 0;
    }

    if ((GetTickCount() - ibytes_sttime) >= 500)
    {
        ibps = (ibytesin-ibytes_st-ilastlen) * 8 / (0.5*1000);
        ibytes_sttime = GetTickCount();
        ibytes_st = ibytesin;
        ilastlen = 0;
    }
    if (!ibps && !obps)
        Form1->spd->Font->Color = clGray;
    else
        if (ibps<1000&&obps<1000)
            Form1->spd->Font->Color = clBlue;
        else
            if (ibps>1000||obps>1000)
                Form1->spd->Font->Color = clPurple;
            else
                Form1->spd->Font->Color = clBlack;

    sprintf(str, "��/��� %4.1f / %4.1f ����/�", ibps, obps);
    Form1->spd->Text = str;

    // if(!ramdisk)
    // Form1->delays->Caption = delaystr;
    // Application->ProcessMessages();
    // if (memory && curdisksize)
    // {
    char str3[111];

    unsigned long h2eapsize;
    h2eapsize=0;

    // for (vector<FATFILE>::iterator it = files.begin();it!=files.end();it++)
    // if ((*it).memory)
    // {
    // //  deb("mem %p - %d", (*it).memory, (*it).memsize);
    // h2eapsize += (DWORD)HeapSize(GetProcessHeap(), 0, (*it).memory);
    // }
    PROCESS_HEAP_ENTRY phe;
    memset(&phe, 0, sizeof(phe));
    unsigned long blks = 0;   HeapLock(GetProcessHeap());
    while (HeapWalk(GetProcessHeap(), &phe))
    {
        if (!phe.wFlags&PROCESS_HEAP_ENTRY_BUSY)
            continue;

        h2eapsize += (unsigned long)phe.cbData + (unsigned long)phe.cbOverhead;
        blks++;
    }
    HeapUnlock(GetProcessHeap());
    heapsize = (DWORD)h2eapsize;
    extern double totalmb;
    snprintf(str3, sizeof(str3), "���� %.2f MB / %.2f RAM (%lu blks)",
        (double)((double)totalmb/1024.0/1024.0),

        (double)((double)h2eapsize/1024.0/1024.0),  blks);
    Form1->ramsize->Text = str3;

    if (GetTickCount()-rtms>200)
    {
        //if (!Form1->ftppb->Canvas->LockCount)
   // {
        //Form1->deblog->Scroll(0,20*(debnewmsgs+1));
        //debnewmsgs=0;
       // Form1->ftplog->Scroll(0,20*(ftpnewmsgs+1));
        //ftpnewmsgs=0;
        CloseHandle(Form1->ftppb->Canvas->Font->Handle);
        Form1->ftppb->Canvas->Font->Handle = getfont("lucida console", 8, 10, 6);
    //}
        Form1->imgptr->ShowHint = true;
        Form1->imgptr->Hint = Form1->imgptr->Caption;

        Form1->headptr->Caption = "";

        rtms = GetTickCount();
        Form1->imgpaging->Visible = false;

        if (ftpcs->TryEnter())
        {
            Form1->Image4->Visible = false;
            ftpcs->Leave();

            static bool curcharttitle = false;

            if (curcharttitle = !curcharttitle)
                Form1->knotts->Title->Text->Text = str3;
            else
                Form1->knotts->Title->Text->Text = "";

            if (Form1->pg2->ActivePageIndex == 2)
                Form1->knotts->Repaint();

            deunicode(Form1->ftpent->Text.c_str(), s, sizeof(s));
            if (strstr(s, "<"))
            {
                Form1->ftpent->Clear();
            }
        }
        else
        {
            if (Form1->Image4->Visible && ftpstate != -1)
                Form1->Image4->Visible = true;
            Form1->Image4->ShowHint = true;
            // Form1->Image4->Hint=ftpcs->
        }
        // Form1->ftplog->Repaint();
        // Form1->ftplog->Update();
        // Form1->Update();
        // Form1->Repaint();
        //
        // Application->ProcessMessages();

    }

    if (ftpcs)
    {

        switch(ftpstate)
        {
            case 4:
            // ftpdeb("  passive transfer %s:%d",ip,ftpport);
            sprintf(what, "DATA c��������� %s:%d %lu", ip, ftpport, lastpasvread);
            // Form1->imgpsvread->Picture->Bitmap->Monochrome = true;
            Form1->imgpsvread->Visible = true;
            // Application->ProcessMessages();
            break;
            case 0:
            sprintf(what, "����     %5lu", (unsigned long)ilastlen);
            break;
            case 1:
            sprintf(what, "��������  %5lu", (unsigned long)olastlen);
            break;
            case -1
            :strcpy(what, "��������");
            Form1->imgpsvread->Visible = false;
            break;
            case 33:
            strcpy(what, ""); // "enter ftpcs");
            break;
            case 2:
            sprintf(what, "��������� ������: %s:%d", ip, ftpport);
            // Form1->imgpsvread->Picture->Bitmap->Monochrome = false;
            Form1->imgpsvread->Visible = true;
            // Application->ProcessMessages();
            break;
            case -2
            :strcpy(what, "���������� �����������");

            timeoutst = 0;
            break;
            case -5
            :strcpy(what, "�� ���������");
            break;
            default:
            sprintf(what, "�3: %d", ftpstate);
            break;
        }

        if (prevftpstate!=ftpstate)
        {
            prevftpstate = ftpstate;
            dms = GetTickCount();
        }

        if (ftpcs->TryEnter())
        {
            ftpcs->Leave();
            // if(ftpstate!=-1)
            // Form1->Image4->Visible=true;
            // else
            // if (ftpstate != 33)
            Form1->Image4->Visible = false;
            // Form1->ftpe->Text = "";

            Form1->ftpimg->Visible = false;
            Form1->Image2->Visible = false;
            Form1->Image1->Visible = false;
            // Form1->Image3->Visible = false;
        }
        else
        {

            // Form1->Image2->Visible = false;
            // Form1->Image1->Visible = false;
            // Form1->Image3->Visible = false;

            if (ftpstate == 0)
                Form1->Image1->Visible = true;
            else
                if (ftpstate == 1)
                    Form1->Image2->Visible = true;
                else
                    if (ftpstate == 2)
                        Form1->imgpsvread->Visible = true;

            Form1->ftpimg->Visible = true;
            char lf[1024];
            AnsiString a1(lastftpcmd), a2(lastftprep);
            a1 = a1.Trim();
            a2 = a2.Trim();
            sprintf(lf, "%-20s:%-20s", a1.c_str(), a2.c_str());
            Form1->ftpe->Text = lf;

            // if (ftpstate == -1)
            if (GetTickCount()-dms>=1000)
                Form1->Image4->Visible = true;
            char u[111];
            sprintf(u, "channel locked for %.2f secs in %s",
                (double)((double)GetTickCount()-(double)dms)/(double)1000, ftpcs_lockedat);
            Form1->Image4->Hint = u;
            Form1->Image4->ShowHint = true;
            // Form1->imgpsvread->Visible= (ftpstate==2) ? true:false;
        }
        extern char ftpcs_enteringat[111];
        if (Form1->Image4->Visible)
            sprintf(what, "state:%2d in: %s, locks:%3d ", ftpstate, ftpcmdhist, ftpcs_entered);

        sprintf(xyu, "%-15s %5.2f ��� ", what,
            (double)((double)(GetTickCount()-(double)dms)/(double)1000.0));

        // if (Form1->Image4->Visible)
        // Form1->OnPaint(NULL);
        // Form1->ftplog->Repaint();

    }
    else
    {
        deb("no ftpcs!");
    }

    if (Form1->pg2->ActivePageIndex !=2)
        return;

    Form1->ftppb->Canvas->Brush->Color = clInfoBk;
    Form1->ftppb->Canvas->Brush->Style = bsSolid;
    Form1->ftppb->Canvas->Pen->Color = clRed;
    Form1->ftppb->Canvas->Pen->Style = psSolid;
    Form1->ftppb->Canvas->Pen->Width = 1;
    Form1->ftppb->Canvas->FillRect(TRect(1, 1, Form1->ftppb->Width-1,
            Form1->ftppb->Top+Form1->ftppb->Height));
    Form1->ftppb->Canvas->Rectangle(2, 1, Form1->ftppb->Width,
        Form1->ftppb->Top+Form1->ftppb->Height-2);
    //

    Form1->ftppb->Canvas->Font->Color = RGB(227, 15, 84);

    if (ftpstate == -1)
        Form1->ftppb->Canvas->Font->Color = clDkGray;
    else
        if (ftpstate == 0)
            Form1->ftppb->Canvas->Font->Color = clBlue;
        else
            if (ftpstate == 1)
                Form1->ftppb->Canvas->Font->Color = clGreen;

    if (Form1->Image4->Visible)
        Form1->ftppb->Canvas->Font->Color = clRed;

    extern char ftpcs_enteringat[111];

    Form1->ftppb->Canvas->TextOut(10, Form1->ftppb->Top+4, xyu);
    Form1->ftppb->Canvas->TextOut(10,
        Form1->ftppb->Top+Form1->ftppb->Height- (Form1->ftppb->Canvas->TextHeight
            (ftpcs_enteringat)), ftpcs_enteringat);

    char txt[1024];

    double z;
    if (lastdelay)
    {
        z = (double)lastdelay/1000.0;
        sprintf(txt, "�������� %.2f ���", z);
        Form1->ftppb->Canvas->TextOutA(Form1->ftppb->Width-Form1->ftppb->Canvas->TextWidth(txt)-10,
            Form1->ftppb->Top+Form1->ftppb->Canvas->TextHeight(txt)+4, txt);
    }
    if (ftpstate == 0)
    {

        if (strlen(ftpgetfilename)>1)
            Form1->ftppb->Canvas->TextOutA(10+Form1->ftppb->Left+Form1->ftppb->Canvas->TextWidth
            (xyu)+10, Form1->ftppb->Top+4, ftpgetfilename);
        Form1->ftppb->Canvas->Font->Color = clGreen;
        Form1->ftppb->Canvas->Font->Size = 7;
        Form1->ftppb->Canvas->TextOut(10, Form1->ftppb->Top+Form1->ftppb->Canvas->TextHeight(xyu)
            +9, lastftpcmd);

        Form1->ftppb->Canvas->Font->Color = RGB(126, 169, 130);
        Form1->ftppb->Canvas->Font->Size = 8;
        Form1->ftppb->Canvas->TextOutA(Form1->ftppb->Width-Form1->ftppb->Canvas->TextWidth
            (ftprlhist)-5,
            Form1->ftppb->Top+Form1->ftppb->Height-Form1->ftppb->Canvas->TextHeight(ftprlhist)-5,
            ftprlhist);

    }
    else
        if (ftpstate == 1)
        {
            Form1->ftppb->Canvas->Font->Color = RGB(126, 169, 130);
            Form1->ftppb->Canvas->Font->Size = 8;
            Form1->ftppb->Canvas->TextOutA(Form1->ftppb->Width-Form1->ftppb->Canvas->TextWidth
                (ftpcmdhist)-5,
                Form1->ftppb->Top+Form1->ftppb->Height-Form1->ftppb->Canvas->TextHeight
                (ftpcmdhist) -5, ftpcmdhist);
        }

    Form1->Image1->Hint = ftpcmdhist;
    if (!hdisk)
        return;
    try
    {
        VDSK_DISK_INFORMATION di;
        VdskQueryDisk(hdisk, &di);
        char str[111];
        sprintf(str, "%I64d", di.IoReadCount.QuadPart);
        Form1->reads->Text = str;
        sprintf(str, "%I64d", di.IoReadBytes.QuadPart);
        Form1->readb->Text = str;
        sprintf(str, "%I64d", di.IoWriteCount.QuadPart);
        Form1->writes->Text = str;
        sprintf(str, "%I64d", di.IoReadFailed.QuadPart);
        Form1->rfails->Text = str;
        sprintf(str, "%I64d", di.IoWriteFailed.QuadPart);
        Form1->wfails->Text = str;

        // Application->ProcessMessages();

        char w[1212];
        sprintf(w, "Last ftp command: %s", lastftpcmd);
        Form1->Image1->Hint = w;
    }
    // Application->ProcessMessages();
    catch(Exception &e)
    {
        char msg[128];
        deunicode(e.Message.c_str(), msg, sizeof(msg));
        deb("exception whilte stats thread: %s", msg);
        deb("ccs @ 0x%08p", ccs);
    }  Application->ProcessMessages();
    // Form1->Repaint();
}

void __fastcall statthread::Do(void)
{

//    if (Form1->pg2->HandleAllocated() && !Form1->pg2->Canvas->LockCount &&
//    Form1->pg2->ActivePageIndex==2)
//    // Synchronize(xxyu);
//        xxyu();

}

// ---------------------------------------------------------------------------
void __fastcall statthread::Execute()
{
    NameThreadForDebugging("statthread");
    Application->HintPause = 0;
    Application->HintShortPause = 0;
    Application->HintHidePause = 10000000000;
    // ---- Place thread code here ----

    // Form1->ftppb->Canvas->Font->Free();

    Sleep(2000);
    // deb("statsthread");
    while (1)
    {

        // Application->ActivateHint(TPoint(50,50));
        Sleep(90);

        // if (Form1->pg2->HandleAllocated() && !Form1->pg2->Canvas->LockCount)
        extern statthread *st;
       // st->RemoveQueuedEvents(xxyu);
       Synchronize(xxyu);
        //  Do();

    }
}
// ---------------------------------------------------------------------------
