// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "formatthread.h"
#include "vdskapi.h"
#include "main.h"
#include "functions.h"
#pragma package(smart_init)
#include <stdio.h>
extern HANDLE hdisk;
extern TForm1 *Form1;
extern LARGE_INTEGER sizee;
extern unsigned long clstsize;
extern TCriticalSection *cchs;
extern int *diskcolor;
extern double disksize;
extern LPVOID memory;

__fastcall formatthread::formatthread(bool CreateSuspended):TThread(CreateSuspended)
{
}

// ---------------------------------------------------------------------------
void __fastcall formatthread::Execute()
{
    NameThreadForDebugging("formatthread");
    // ---- Place thread code here ----

    Sleep(1000);
    // Synchronize(Do);
    Do();
}

void __fastcall formatthread::Do(void)
{
    // if (!cchs->TryEnter())
    // {
    // deb("���� �����");
    // return;
    // }

    extern int totreads, totwrites;
    extern int numpkt;
    int forr = 0, forw = totwrites;
    forr = totreads;
    forw = 0;
    numpkt = 0;
    // VdskSetDisk(hdisk, sizee, 2, 32, 512, false, true, true);
    if (!diskcolor)
        diskcolor = new int[Form1->pb1->Width];
    // for (DWORD i = 0;i<Form1->pb1->Width-1;i++)
    // diskcolor[i] = (int)clGray;
    // deb("��������� ���� ...");

    //VdskUnmountDisk(hdisk, 'y');
     //deb("���� ��������.");
  char str[233];
    DWORD ms = GetTickCount();
    for (DWORD i = 0;i<Form1->pb1->Width-1;i++)
        diskcolor[i] = (int)clWhite;
    if (!disksize)
        deb("format no disksize");
        extern LONGLONG curdisksize;
  //  memset(memory, 0x0, curdisksize);
    deb("������ �������������� (clstsize=%d,disksize=%.0f)...", clstsize, disksize);
    // Application->ProcessMessages();
    // Form1->deblog->Visible = false;
    int ret = VdskFormatDiskA(hdisk, VDSK_FS_FAT32, "t1", false, (unsigned long)0, OnFormat, NULL);

    if (!ret)
    {
        int err = GetLastError();

        if (err == ERROR_NOT_INITIALIZED)
        {
            deb("!!!!!!!!!! ERROR_NOT_INITIALIZED           ");
            VdskUnmountDisk(hdisk, 'y');
            VdskDeleteDisk(hdisk);
            VdskFreeLibrary();
        }
        else
        {
            deb("=============\r\n !!! VdskFormatDisk: ������ ��� ��������������\r\n " "   %s",
                fmterr(err));
        }
        VdskInitializeLibrary();
        // ret = VdskFormatDisk(hdisk, VDSK_FS_FAT32, "temp1", false, clstsize, OnFormat, NULL);
        // if (!ret)
        // {  //   cchs->Leave();
        // deb("fail restore");
        // return;
        // }
        sprintf(str, "FORMAT FAILED");
        Form1->current->Text = str;
    }
    else
    {
        ms = GetTickCount()-ms;
        deb("============= ���� �������������� �� %.2f ���.\r\n  ������: %d �������: %d",
            (double)ms/1000.0, totreads-forr, totwrites-forw);
        sprintf(str, "FORMAT DONE");
        Form1->current->Text = str;
    }
    // MessageBox(NULL, "format done", NULL, MB_OK);

    if (!VdskMountDisk(hdisk, 'y', true))
        deb("������ ��� ����������� ����� %s", fmterr());

    // VdskStartDisk(hdisk, true);
    // Form1->deblog->Visible = true;


    Application->ProcessMessages();
    Form1->OnPaint(0);
    Form1->Button3->Enabled = true;
}
// ---------------------------------------------------------------------------
