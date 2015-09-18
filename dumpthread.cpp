// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dumpthread.h"
#include "vdskapi.h"
#include "functions.h"
#include <stdio.h>
#include "main.h"

#pragma package(smart_init)
extern TForm1 *Form1;
extern bool ramdisk;
extern unsigned long curdisksize, disksize;
extern LPVOID memory;
extern char savefile[128];

// ---------------------------------------------------------------------------

// Important: Methods and properties of objects in VCL can only be
// used in a method called using Synchronize, for example:
//
// Synchronize(&UpdateCaption);
//
// where UpdateCaption could look like:
//
// void __fastcall dumpthread::UpdateCaption()
// {
// Form1->Caption = "Updated in a thread";
// }
// ---------------------------------------------------------------------------

__fastcall dumpthread::dumpthread(bool CreateSuspended):TThread(CreateSuspended)
{
}
// ---------------------------------------------------------------------------


void __fastcall dumpthread::Execute()
{
    NameThreadForDebugging("dumpthread");
    // ---- Place thread code here ----
    Form1->current->Text = "создание образа ...";

    deb("сохраняем в файл %s ...\r\n \r\n", savefile);
    Application->ProcessMessages();
    char *b = new char[curdisksize];

    if (!ramdisk)
        nread((unsigned long)b, 0, curdisksize, 0);
    else
        memcpy(b, memory, curdisksize);

    unlink(savefile);
    dump(b, curdisksize, savefile);

    int crc = checksum((unsigned short*)b, curdisksize);
    char temp[129];
    sprintf(temp, " * Образ сохранён, CRC=0x%04X\r\n \r\n", crc);
    deb(temp);
    Form1->current->Text = temp;
    delete[]b;
    Form1->Button4->Enabled = true;
    Application->ProcessMessages();

    if (!ramdisk)
        Form1->Button5Click(NULL);
}
// ---------------------------------------------------------------------------
