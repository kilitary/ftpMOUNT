// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <windows.h>
#include "main.h"
#include "functions.h"

#include "vdskapi.h"
#include <winsock2.h>
#include "statthread.h"
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
#pragma package(smart_init)
#pragma link "ExceptionLog"
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "ArrowCha"
#pragma link "GanttCh"
#pragma link "BubbleCh"
#pragma resource "*.dfm"

#pragma pack(1)

#include "FtpMountThread.h"
using namespace std;
extern DWORD heapsize;
unsigned long lastdelay = 0;
unsigned long timeoutst = 0;
unsigned long netfatfilesize;
unsigned long ftpgetfileoffset = 0, ftpgetfilelen = 0;
// ---------------------------------------------------------------------------
char temp8[1024];
bool unmounting = false;
TCriticalSection *ccs;
unsigned int nf = 0;
FtpMount *fm;
double avg = 1, avg2 = 2;
SOCKET tempsock;
double formatp = 0.0;
int dirlev = 0;
double delayperc = 0;
int ftpcs_entered = 0;
double dp;
DWORD tmsleft = 0;
double gt = 0, bt = 0;
LPVOID rootdirptr;
int ftpport = 0;
char ftphost[128] = "";
extern unsigned char fat32[];
LARGE_INTEGER sizee;
TCriticalSection *ecs;
int totreads = 0, totwrites = 0;
formatthread *ft = NULL;

LONGLONG curdisksize = 0;
LONGLONG fatsize;
DWORD fs;
TCriticalSection *cchs;
char ip[128];
int ftplport;
TEvent *cev;
LONGLONG totcachsize = 0;
DWORD fat1, fat2;
LPVOID pData;
TForm1 *Form1;
extern unsigned char fb[];
char ftpcs_enteringat[111];
unsigned long rntrys = 0;
LONGLONG rlastoffset = 0;
unsigned long wntrys = 0;
LONGLONG wlastoffset = 0;
HANDLE hdisk = NULL;
LPVOID memory = NULL;
SOCKET ConnectSocket = 0, FtpSocket = 0;
LONGLONG clstsize = 4096;
bool precache = false;

int ftpstate = -5;
unsigned long lastpasvread = 0;
bool showformathint;
int numpkt = 1;
bool loggedin = false;
DWORD fspace_ptr;
unsigned long start_cluster = 3;
unsigned long stc;
unsigned long totclusters;
unsigned long dataoffset;
char ftpcmdhist[128], ftprlhist[128];
char lastftpcmd[1024] = "";
char lastftprep[1024];
LPVOID dirptr;
int nfiles = 0;
LONGLONG maxdiskuse = 0;
unsigned long rootdirectory = 0;
int err;
bool nconn(void);

bool ramdisk = false;

LONGLONG disksize = 32*1024*1024*1024;
statthread *st = NULL;
char ftpgetfilename[128];
CacheThread *ct = NULL;
TCriticalSection *ftpcs;
double ibytesin = 0, ibytes_sttime = 0, ibytes_st = 0, ibps = 0, ilastlen;
double obytesin = 0, obytes_sttime = 0, obytes_st = 0, obps = 0, olastlen;
double maxquad = 0;
double totalmb = 0;
unsigned long pfat1, pfat2;
char serv[] = "192.168.1.107"; // "192.168.253.129"; // "69.197.152.44";
unsigned int *diskcolor = NULL;

vector<CACHE>cache;

PKT pkt;
char delaystr[128];
int pids[0xffff];
vector<FATFILE>files;
unsigned int *fat;
char fatfiles[32768] = "";
FAT32BOOT fat32boot;
char ftpcs_lockedat[1024] = "unknown!";

#define ftplock()   _ftplock(__FUNC__,__LINE__)
#define ftpunlock() _ftpunlock(__FUNC__,__LINE__)
#define ftprl(buf,tim, sign) _ftprl(__FUNC__,__LINE__, buf, (DWORD) tim, sign)
#define ftpcmd(cmd, inbuf, inbufsiz) _ftpcmd(__FUNC__,__LINE__,cmd,inbuf,inbufsiz)

void ftpblocking(bool block)
{
    unsigned long iMode;

    iMode = !block;
    // ftpcs->Enter();
    if (FtpSocket<=0)
    {
        deb("ftpblocking zero socket!");
        return;

    }
    ioctlsocket(FtpSocket, FIONBIO, (unsigned long*)&iMode);

    // ioctlsocket(FtpSocket, FIONREAD, (unsigned long*)&iMode);
    // ftpcs->Leave();
}

int _ftprl(char *func, int line, char *buf, DWORD maxms = 0, char *sign = 0)
{
    char *bp = buf;
    int nr = 0;
    int ss = 0;
    memset(buf, 0, 256);
    buf[0] = 1;
    int totr = 0;
    Application->ProcessMessages();
    // Form1->Update();
    // deb("ftprl() ent");
    // ftpdeb("sign %d", sign);
    sprintf(ftprlhist, "%15s->ftprl(maxms=%lu,sign=%s):%5d", func, (unsigned long)maxms,
        (int)sign>0?sign:"", line);

    ftpstate = 0;

    char blaaa[1111];
    if (!Form1->ftpent->Focused())
    {
        sprintf(blaaa, "computer: < READING >");
        Form1->ftpent->Text = blaaa;
    }

    // if (maxms)
    ftpblocking(false);

    DWORD tms = GetTickCount();

    while (true)
    {

        if (ss>0)
            bp++;

        ftplock();
        ftpblocking(false);
        ss = recv(FtpSocket, bp, 1, 0);
        ftpblocking(true);
        ftpunlock();

        if (ss>0 && !*bp && !sign)
            break;

        // ftpdeb("ss:%d %u", ss, GetTickCount()-tms);
        if (ss>0)
        {
            ibytesin+=ss;
            if (ss>1)
                ilastlen = (double)ss;
            totr+=ss;
        }

        if (ss<=0 && (int)sign == -1 && GetTickCount()-tms>=maxms)
        {
            break;
        }

        if (maxms>0&&(int)sign == 0&&strstr(buf, "\r\n")&&GetTickCount()-tms>=maxms/4&&ss<=0)
            break;

        if (maxms>0&&GetTickCount()-tms>=5000)
        {
            ftpdeb("   max rl timeout");
            break;
        }

        if (maxms == (DWORD) -1 && GetTickCount()-tms >= (totr?lastdelay?lastdelay*3:100:timeoutst))
        {
            break;
        }

        if (maxms>0&&GetTickCount()-tms>=maxms && !sign)
            break;

        if (((int)sign > 0 && strstr(buf, sign) && strstr(buf, "\r\n")&&ss<=0))
        {
            break;
        }

        if ((int)sign <= 0 && strstr(buf, "\r\n"))
            break;

        if (ss == 0 && !sign)
            break;
    }

    if (ss<0)
    {
        int err = WSAGetLastError();
        // deb("rln: ss recv : %d err:%d", ss, err);
    }
    // deb("r: %c (%x)",*bp,*bp);

    tms = GetTickCount()-tms;
    lastdelay = tms;
    // if (maxms)
    ftpblocking(true);

    ftpdeb("   rln(sign=%d,maxms=%lu) totr:%d in %u ms, *bp=%x", sign, (unsigned long)maxms, totr,
        tms, *bp);

    if (totr&&tms&&tms > timeoutst)
    {
        timeoutst = (DWORD)(floor((double)tms*1.9));
        // ftpdeb(" %% + packet delay %lu ms", timeoutst);
    }
    else
        if (totr&&tms&&tms < timeoutst)
        {
            timeoutst = (DWORD)(floor((double)tms*1.9));
            // ftpdeb(" %% - packet delay %lu ms", timeoutst);
        }

    if (timeoutst < 900)
    {
        timeoutst = 900;

    }

    char *p;

    if (totr && (p = strstr(buf, "227")))
    {
        char portaddr[128];
        sscanf(p, "%*s %*s %*s %*s %s", portaddr);
        unsigned long port1, port2;

        int ip1, ip2, ip3, ip4;
        sscanf(portaddr+1, "%d,%d,%d,%d,%d,%d", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
        sprintf(ftphost, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
        unsigned long port3 = port1*256+port2;
        ftpport = (int)port3;
        ftpdeb(" %% port addr %s [%s:%d (%lu,%lu)]", portaddr, ftphost, ftpport, port1, port2);
    }

    strncpy(lastftprep, buf, sizeof(lastftprep));
    // char buf2[5555];
    ///   deunicode((wchar_t*)buf,buf2,sizeof(buf2));
    if (Form1->debcheck->Checked)
        deb(buf);

    if (totr)
        ftpdeb(" > %s", buf);
    // deb("ftprl() ext");
    // Form1->ftpent->Text = "";
    ftpstate = -1;

    // Application->ProcessMessages();

    return totr;
}

int _ftpcmd(char *func, int line, char *cmd, char *obuf = 0, int len = 0)
{
    char buf[2048];
    ftpstate = 1;
    Application->ProcessMessages();
    sprintf(lastftpcmd, "%s", cmd);
    sprintf(ftpcmdhist, "%15s->ftpcmd(%s):%5d", func, cmd, line);
    if (Form1->debcheck->Checked)
        deb(" # %s", cmd);
    ftpdeb(" # %s", cmd);

    if (!FtpSocket)
        if (!ftpconn(true))
        {
            ftpstate = -1;
            return -1;
        }
    ftpstate = 1;
    ftplock();
    // unsigned long iMode = 1;
    // ioctlsocket(FtpSocket, FIONBIO, (unsigned long*) &iMode);
    // ioctlsocket(FtpSocket, FIONREAD, (unsigned long*)&iMode);
    ftpblocking(false);
    int canread;
    while ((canread = recv(FtpSocket, buf, 1111, MSG_PEEK))>0)
    {
        if (Form1->debcheck->Checked)
            deb("reading OOB %d bytes", canread);
        memset(buf, 0, 1111);
        recv(FtpSocket, buf, canread, 0);
        if (Form1->debcheck->Checked)
            ftpdeb("OOB: %s", buf);
        ftpdeb(buf);
    }
    ftpblocking(true);
    // iMode = 0;
    // ioctlsocket(FtpSocket, FIONBIO, (unsigned long*) &iMode);
    // ioctlsocket(FtpSocket, FIONREAD, (unsigned long*)&iMode);

    ftpblocking(true);
    if (!strstr(lastftpcmd, "\r"))
    {
        strcat(lastftpcmd, "\r");
    }
    if (!strstr(lastftpcmd, "\n"))
    {
        strcat(lastftpcmd, "\n");
    }

    int ss;
    int sd = send(FtpSocket, (char*)lastftpcmd, strlen(lastftpcmd), 0);
    if (len)
    {
        ss = recv(FtpSocket, (char*)obuf, sizeof(buf), 0);
        if (ss>0)
        {
            ibytesin+=ss;
            if (ss>1)
                ilastlen = (double)ss;
        }
        if (sd == -1 || !sd)
        {
            int err = WSAGetLastError();
            deb("ftpcmd recv: %s", fmterr(err));
            closesocket(FtpSocket);
            FtpSocket = 0;

        }
        else
        {
            char *p;
            if (p = strstr(obuf, "|||"))
            {
                sscanf(p, "|||%u", &ftpport);

            }
        }
    }
    ftpblocking(false);
    ftpunlock();
    char blaaa[1111];
    if (!Form1->ftpent->Focused())
    {
        sprintf(blaaa, "computer: %s", cmd);
        Form1->ftpent->Text = blaaa;
    }
    ftpstate = -1;
    if (sd>0)
    {
        olastlen = sd;
        obytesin+=sd;
    }
    else
    {
        deb("sd: %d", sd);
    }
    // deb("lv");
    return sd;
}

unsigned long getbytecluster(unsigned long byte)
{
    unsigned long clb;

    byte -= rootdirectory - 2*clstsize;

    byte /= clstsize;
    return byte;
}

unsigned long getclusterbyte(unsigned long ncl)
{
    unsigned long clb;
    if (!ncl)
        clb = rootdirectory;
    else
        clb = ((unsigned long)rootdirectory + (unsigned long)((unsigned long)ncl*(unsigned long)
            clstsize)) - 2*clstsize;
    // deb("cluster %lu %lu", ncl, clb);
    return clb;
}

void ftppsvblocking(bool block)
{
    unsigned long iMode;

    iMode = !block;
    ioctlsocket(tempsock, FIONBIO, (unsigned long*)&iMode);

    // ioctlsocket(tempsock, FIONREAD, (unsigned long*)&iMode);
}

void _ftplock(char *func, int line)
{
    sprintf(ftpcs_enteringat, "entering in %s():%3d", func, line);
    if (ftpcs_entered<0||ftpcs_entered>3)
        deb("_ftplock(%s,%d) ftpstate: %d ftpcs_entered: %d", func, line, ftpstate, ftpcs_entered);
    int ss = ftpstate;
    ftpstate = 33;
    ftpcs->Enter();
    if (line)
        sprintf(ftpcs_lockedat, "%s:%d", func, line);
    ftpcs_entered++;
    ftpstate = ss;
    ftpcs_enteringat[0] = 0;
}

void _ftpunlock(char *func, int line)
{
    if (ftpcs_entered<0||ftpcs_entered>3)
        deb("_ftpunlock(%s,%d) ftpstate: %d ftpcs_entered: %d", func, line, ftpstate,
        ftpcs_entered);
    int ss = ftpstate;
    ftpstate = 34;
    ftpcs->Leave();
    ftpstate = ss;
    ftpcs_entered--;
}

unsigned char lfn_checksum(const unsigned char *pFcbName)
{
    int i;
    unsigned char sum = 0;

    for (i = 11;i;i--)
        sum = ((sum & 1) << 7) + (sum >> 1) + *pFcbName++;
    return sum;
}

unsigned long getfreecluster(void)
{
    static unsigned long nextfree = 0;
    unsigned long i;
    static unsigned long lasti = 0;
    unsigned long fs;
    if (nextfree)
    {
        if (!fat[nextfree])
        {
            i = nextfree;

            nextfree = 0;
            return i;
        }
        nextfree = 0;

    }

    fs = fatsize/4;

    if (lasti>=fs)
        lasti = 0;

    for (i = lasti;i<fs;i++)
        if (!fat[i])
            break;

    if (!fat[i+1])
        nextfree = i+1;

    lasti = i;

    return i;
}

void markclused(unsigned long cl)
{
    fat[cl] = 0x0fffffff;
}

char *ftppwd(void)
{
    static char b[1024];
    ftpcmd("PWD", 0, 0);
    ftprl(b, timeoutst*2, 0);
    static char bb[128];
    sscanf(b, "%*d \"%[^\"]\"", bb);
    return bb;
}

int ftpcd(char *dir)
{
    char b[222];
    sprintf(b, "CWD %s", dir);
    ftpcmd(b, 0, 0);
    // ftprl(b, timeoutst, "50");
    return 0;
}

int fataddfiles(char *dirname, LPVOID memory, unsigned long stclst, char *buf)
{
    deb(" %% fataddfiles(%s,%p,%lu,%p)", dirname, memory, stclst, buf);
    char line[1024];
    char temp2[32768], temp3[32768];
    char tempbuf[32768];
    char filename[1024];
    char fn[1024];
    unsigned long allocatedbytes;
    unsigned long startclust;
    unsigned long startoffset;
    unsigned int offset;
    unsigned int clst;
    unsigned int ssize;
    unsigned int nclust;
    unsigned int entrcls;
    unsigned long cluster;
    int attr = 0;
    char fnm[1024];
    char *p;
    unsigned long freeclusters;
    char *nl;
    static int curdirlev = 0;
    unsigned int nb = 0;
    char xyu0[128];
    char *next_token3 = NULL, *next_token4 = NULL;
    LARGE_INTEGER off;
    unsigned int i = 1024;
    int nfiles;
    DIR dir;
    DWORD tms2;
    LPVOID dirptr, ndirptr;
    int nr = 0;
    unsigned long nf = 0;
    nl = strtok_s(buf, &i, "\r\n", &next_token3);
    static int numit = 0;
    char ftpdir[255] = "";
    nfiles = 0;
    // if (stclst>2)
    // stclst-=2;
    // deb("scanning:%d fat(%s)", curdirlev, dirname);
    int added = 0;
    strncpy(ftpdir, dirname, sizeof(ftpdir));
    unsigned long clstbyte;
    clstbyte = getclusterbyte(stclst);
    // ndirptr = (LPVOID)((unsigned long)memory+ ((unsigned long)getclusterbyte(stclst)));
    ndirptr = (LPVOID)((unsigned long)memory);
    char spcrs[1024];
    // memset(spcrs, 0, 1024);
    // for (int i = 0;i<curdirlev*4;i++)
    // spcrs[i] = 0x20;
    deb("@%d:%s=%p (%lu kb in %lu kb) cluster %lu", curdirlev, dirname, ndirptr,
        (unsigned long)((unsigned long)ndirptr-(unsigned long)memory)/1024,
        (unsigned long)curdisksize/1024, stclst);
    dirptr = ndirptr;
    nfiles = 0;
    nf = 0;
    fat[stclst] = 0x0fffffff;
    stc = stclst;
    numit++;
    UnicodeString st;

    if ((LONGLONG)totalmb >= maxdiskuse)
    {
        deb("reached maxdiskuse (%lu MB)", (unsigned long)maxdiskuse/1024/1024);
        return 0;
    }

    char sss[128];

    unsigned long maxssize;
    char txt2[11];
    deunicode(Form1->ftpmaxfilesize->Text.c_str(), txt2, sizeof(txt2));
    maxssize = atoi(txt2)*1024*1024;
    // deb("max file size %lu b", maxssize);
    while (nl)
    {
        // Application->ProcessMessages();
        // Sleep(1);
        if ((LONGLONG)curdisksize > (LONGLONG)maxdiskuse)
        {
            deb("stop fat fill, totalmb %lu reach", (unsigned long)maxdiskuse);
            return added;
        }
        char snf[128];
        // sprintf(snf, "%lu", (unsigned long)curdisksize/1024/1024);
        st = "";
        Form1->knotts->Series[1]->Add((double)floor(heapsize/1024/1024), st, clRed);
        // sprintf(snf, "%lu", (unsigned long)curdisksize/1024/1024);
        // st = "";
        Form1->knotts->Series[2]->Add((double)floor(totalmb/1024/1024), st, clGray);

        dirptr = (LPVOID)((unsigned long)memory+ ((unsigned long)(nr?sizeof(dir):0)*
                ((unsigned long)nr?nr:0)));

        if (IsBadWritePtr(dirptr, sizeof(dir)))
        {
            deb("bad write ptr dir %p (mem %p + %lu KB) [%lu kb in %lu kb]", dirptr, memory,
                (unsigned long)((unsigned long)dirptr-(unsigned long)memory)/1024,
                (unsigned long)((unsigned long)dirptr-(unsigned long)memory)/1024,
                HeapSize(GetProcessHeap(), 0, memory)/1024);
        }
        // if(nfiles++)
        attr = 0x20;
        tms2 = GetTickCount();
        nfiles++;

        DWORD size = 0;
        memset(&dir, 0, sizeof(dir));
        memset(filename, 0, sizeof(filename));
        memset(fn, 0, sizeof(fn));
        memset(fnm, 0, sizeof(fnm));
        dir.crdate = 0x11;
        dir.crtime = 0x11;
        dir.crtimesecs = 0x11;
        dir.laccessdate = 0x11;
        dir.lmoddate = 0x11;
        dir.lmodtime = 0x11;

        int ret;

        // strncpy(line, nl, sizeof(line));
        memset(line, 0, sizeof(line));
        int s;
        s = 0;
        int d;
        d = 0;
        for (int i = 0;i<strlen(nl);i++)
        {
            if (nl[s])
            {
                if (nl[s] == 0x20 && line[d-1] == 0x20)
                {
                    s++;
                    continue;
                }
                line[d] = nl[s];
                d++;
            }
            s++;
        }
        unsigned long fsize = 0;
        // deb("line #%d:[%s]", dirlev, line);
        unsigned long d1;
        char attrs[110];
        memset(attrs, 0, 11);
        ret = sscanf(line, "%10s %*lu %*s %*s %lu %s %*s %*s %s", &attrs, &fsize, &temp8, &fn);
        if (ret !=4)
        {
            ret = sscanf(line, "%10s %*lu %*s %*s %lu %s %*s %*d:%*d %s", &attrs, &fsize, &temp8,
                &fn);
            if (Form1->debcheck->Checked)
            {
                deb("fail to parse list %d: %s", ret, line);
                deb("attrs=[%s] d1=%lu fsize=%lu fn=[%s] temp8=[%s]", attrs, d1, fsize, fn, temp8);
            }

            if (ret !=4)
            {
                i = 1024;
                nl = strtok_s(NULL, &i, "\r\n", &next_token3);
                // if (nr)
                deb("skip !=5(%d) %s", ret, nl);
                continue;
            }
        }

        // deb("scanf done fn=[%s]", fn);
        AnsiString as(fn);

        as = as.Trim();
        strncpy(fn, as.c_str(), sizeof(fn));

        if (fn[0] == '.' || fn[1] == '.')
        {
            i = 1024;
            nl = strtok_s(NULL, &i, "\r\n", &next_token3);
            // deb("skip %s", fn);
            continue;
        }

        strncpy(filename, fn, sizeof(filename));
        // for(int o=0;o<strlen(filename);o++)
        // if(filename[o]==0x20)filename[o]=0;
        if (attrs[0] == 'd'||attrs[0] == 'l')
        {
            if (attrs[0] == 'l')
            {
                sscanf(line, "%10s %lu %*s %*s %lu %s %*s %*s %*s -> %*s", &attrs, &d1, &fsize,
                    &temp8, &fn);
                strcpy(filename, fn);

            }

            attr = 0x10;

        }

        i = 128;
        p = strtok_s(fn, &i, ".", &next_token4);
        if (p)
        {
            strncpy(fnm, p, sizeof(fnm));
            strncpy(dir.fname, strupr(fnm), 8);

            p = strtok_s(NULL, &i, ".", &next_token4);
            // deb("fn %s ext %s", fnm, p);
            if (p)
                strncpy(dir.ext, strupr(p), 3);
        }
        else
        {
            strncpy(dir.fname, fn, sizeof(dir.fname));
        }

        for (int u = 0;u<11;u++)
            if (!dir.fname[u])
                dir.fname[u] = 0x20;

        if (strlen(filename)>11)
        {
            dir.fname[6] = '~';
            dir.fname[7] = '1';
        }
        FATFILE f;
        // Form1->currentadd->Caption = dir.fname;
        stc = getfreecluster();

        dir.st_clust_l = LOWORD(stc);
        dir.st_clust_h = HIWORD(stc);

        if (attr == 0x10)
        {
            //

            markclused(stc);
            dir.attr = 0x10;
            // deb("memcpy (%p)",dirptr);
            memcpy((LPVOID)dirptr, &dir, sizeof(dir));
            nr++;
            char cmd[128];
            char obuf[2048];

            char snf[128];
            sprintf(snf, "#%d %s", nf, dir.fname);
            st = "";
            Form1->knotts->Series[0]->Add((double)floor(dir.size/1024/1024), st, clYellow);

            f.memory = HeapAlloc(GetProcessHeap(), 0, 32768);
            f.memsize = 32768;
            // deb(obuf);
            deb("  <dir> %s @ %p, cluster %lu, byte %lu", dir.fname, dirptr, stc,
                getclusterbyte(stc));
            char *tbuf;
            char fd[111];
            char curd[111];
            if (curdirlev < dirlev)
            {
                char ffn[1024];
                sprintf(ffn, "y:\\%s\\%s", dirname+1, filename);

                sprintf(sss, "%-25s (psv=%s:%-5d,timeoutst %5lu,lastdelay %5lu)", ffn, ip, ftpport,
                    timeoutst, lastdelay);
                Form1->knotts->Title->Text->Text = sss;
                strcpy(curd, ftppwd());
                // deb("recursive:%d ftp reading [%s] ...", dirlev, filename);
                // snprintf(cmd, sizeof(cmd), "CWD %s", filename);
                ftpcd(filename);
                // ftpcmd(cmd, obuf, 0);
                bool dirok;
                dirok = false;
                memset(obuf, 0, sizeof(obuf));
                while (ftprl(obuf, dirok?lastdelay?lastdelay:100:timeoutst, 0)>0)
                {
                    if (strstr(obuf, "250"))
                    {
                        dirok = true;
                        // break;
                    }
                    continue;
                }
                // deb("tt:");
                // memset(obuf, 0, sizeof(obuf));
                // while (!strstr(obuf, "250") && !strstr(obuf, "550"))
                // ftprl(obuf, 0, 0);

                // deb(obuf);

                if (dirok)
                {
                    curdirlev++;
                    sprintf(cmd, "PWD");
                    ftpcmd(cmd, obuf, 0);
                    ftprl(obuf, timeoutst, 0);
                    // memset(obuf, 0, sizeof(obuf));
                    // while (!strstr(obuf, "257"))
                    // ftprl(obuf, 0, 0);

                    // deb(obuf);
                    ftpcmd("PASV", obuf, 0);
                    ftprl(obuf, timeoutst, "227");
                    // while (!strstr(obuf, "229"))
                    // ftprl(obuf, 0, 0);
                    // deb(obuf);
                    sprintf(cmd, "LIST -l");
                    ftpcmd(cmd, obuf, 0);
                    // ftprl(obuf, timeoutst, (char*)0);

                    tbuf = new char[2*1024*1024];
                    memset(tbuf, 0, sizeof(tbuf));
                    int nr = ftppasvread(ftphost, ftpport, tbuf, 0, 2*1024*1024);

                    while (ftprl(obuf, lastdelay?lastdelay:timeoutst, (char*)0)>0)
                        if (strstr(obuf, "226"))
                            break;
                        else
                            continue;

                    // deb("fatfiles size: %d", strlen(tbuf), tbuf);
                    if (strlen(tbuf)<100)
                        deb("[%s]", tbuf);
                    // ndirptr = (char*)(unsigned long)memory+(unsigned long)getclusterbyte(stc);
                    // deb("ndirptr %p", ndirptr);

                    snprintf(fd, 111, "%s\\%s", dirname, filename);

                    added += fataddfiles(fd, f.memory, stc, tbuf);
                    // deb("new dir %s crc:%x",fd,checksum((unsigned short*)f.memory,4096));
                    delete[]tbuf;
                    // dirptr = (LPVOID)((unsigned long)dirptr+  (unsigned long)
                    // sizeof(dir));

                    memset(obuf, 0, sizeof(obuf));
                    // while (!strstr(obuf, "226"))
                    // ftprl(obuf);
                    // deb(obuf);
                    // sprintf(cmd, "CDUP");
                    // ftpcmd(cmd, 0, 0);
                    // ftprl(obuf, timeoutst, (char*)"250");
                    // ftprl(obuf);
                    // deb(obuf);
                    curdirlev--;

                }

                else
                {
                    ftpdeb(" ! skip filename %s", filename);

                    deb(" skip filename %s - %s in %s", filename, obuf, curd);
                    nl = strtok_s(NULL, &i, "\r\n", &next_token3);
                    dirptr = (LPVOID)((unsigned long)dirptr +(unsigned long)sizeof(LFNDIR));
                    continue;
                    // return added;
                }

                ftpcd(curd);

                dirok = false;
                while (ftprl((char*)ffn, dirok?lastdelay?lastdelay:100:timeoutst, 0)>0)
                {
                    if (strstr((char*)ffn, "250"))
                    {
                        dirok = true;
                        // break;
                    }
                    continue;
                }

                f.fetched = true;
            }
            else
            {
                // ftpdeb("max dir reaxched");
                f.fetched = false;
            }

            strcpy(f.fn, filename);
            f.start_offset = getclusterbyte(stc);
            f.end_offset = getclusterbyte(stc)+clstsize;
            f.firstcluster = stc;
            f.clusters = 1;
            f.accesses = 0;
            f.dir = true;
            strcpy(f.ftppath, ftpdir);
            f.size = 0;

            files.push_back(f);

            dirptr = (LPVOID)((unsigned long)dirptr +(unsigned long)sizeof(LFNDIR));
            nf++;
            i = 1024;

            nl = strtok_s(NULL, &i, "\r\n", &next_token3);

            continue;
        }
        nf++;
        // size = 512;
        dir.size = (unsigned int)fsize;

        // sprintf(snf, "%s", nf, dir.fname);
        st = "";
        Form1->knotts->Series[0]->Add((double)floor(dir.size/1024.0/1024.0), st,
            checksum((unsigned short*)dirname, strlen(dirname)));

        if (totalmb > maxquad)
            maxquad = totalmb;

        if (Form1->fatdeb->Checked)
            deb("F #%-3d %25s %8u ���� (%.2f MB) ", nfiles, filename, fsize,
            (double)fsize/1024/1024);

        // stc = cluster;

        // fat[stc] = 0x0fffffff;
        // stc = getfreecluster();

        // fat[stc] = 0x0fffffff;

        ssize = fsize;

        if (ssize > maxssize)
        {
            deb("skip %s too big ", filename);
            nl = strtok_s(NULL, &i, "\r\n", &next_token3);
            continue;
        }

        if ((LONGLONG)totalmb +(LONGLONG)fsize > (LONGLONG)maxdiskuse)
        {
            deb("max diskuse reached %lu MB", (unsigned long)maxdiskuse/1024/1024);
            return added;
        }

        totalmb += (double)fsize;

        markclused(stc);
        // stc = getfreecluster();
        clst = stc;

        nclust = 0;
        int clustersused;
        clustersused = 1;

        startoffset = 0;
        startclust = stc;
        offset = (clstsize*stc);
        if (Form1->fatdeb->Checked)
            deb(" offset +%lu (0x%08x) cluster %d ", (unsigned long)dataoffset+offset,
            (unsigned long)dataoffset+offset, stc);

        entrcls = (unsigned int)dir.size/clstsize;
        allocatedbytes = 0;
        sprintf(sss, "%s\%s, %.2f ��", dirname+1, filename, (double)((double)fsize/1024.0));
        Form1->knotts->Title->Text->Text = sss;
        while (ssize > 0)
        {
            unsigned long nclst;
            nclst = getfreecluster();
            fat[stc] = nclst;
            clustersused++;
            stc = nclst;
            markclused(stc);
            char str2[255];
            double pr;

            if (entrcls)
            {

                char sss[111];

                if ((GetTickCount()-tms2) >=700)
                {
                    pr = ((((double)allocatedbytes?(double)allocatedbytes:(double)1))/(double)
                        dir.size) * (double)100;
                    sprintf(sss, "������ FAT %.2f %% [%s\%s]", pr, dirname, filename);
                    Form1->current->Text = sss;
                    Form1->knotts->Title->Text->Text = sss;
                    deb(sss);
                    tms2 = GetTickCount();

                    Form1->headptr->Left = (int)avg;

                }

            }

            ssize = (unsigned int)ssize - ((unsigned int)clstsize < (unsigned int)ssize ?
                (unsigned int)clstsize:(unsigned int)ssize);

            if (!startoffset)
                startoffset = getclusterbyte(startclust); // (unsigned long)offset+(unsigned long)dataoffset-(unsigned long)32768;

            offset = (unsigned long)((unsigned long)clstsize*(unsigned long)stc);

            off.QuadPart = (LONGLONG)(unsigned long)offset+(unsigned long)dataoffset;
            paintdisk(9, off, clMedGray, true, 0);

            allocatedbytes+=clstsize;
        }

        char ffn[1024];
        sprintf(ffn, "y:\\%s\\%s", dirname+1, filename);
        sprintf(sss, "%-50s (%.2f M�, %6lu clstrs from %5lu)", ffn,
            (double)((double)fsize/1024.0/1024.0), clustersused, startclust);
        Form1->knotts->Title->Text->Text = sss;
        off.QuadPart = startoffset;
        paintdisk(9, off, clDkGray, true, 0);

        fat[stc] = 0x0fffffff;
        // stc++;
        freeclusters-=nclust;
        if (Form1->fatdeb->Checked)
            deb("   %u clusters (%d free clusters)", nclust, freeclusters);
        // deb("startoffset %lu",startoffset);

        strcpy(f.fn, filename);
        f.start_offset = startoffset;
        f.end_offset = getclusterbyte(startclust+nclust)+clstsize; // startoffset+fsize;
        f.firstcluster = startclust;

        strcpy(f.ftppath, ftpdir);
        f.accesses = 0;
        f.size = fsize;
        f.clusters = clustersused;
        f.fetched = false;
        files.push_back(f);

        netfatfilesize+=fsize;

        dir.attr = 0x20;
        LFNDIR lfn;
        memset(&lfn, 0xFF, sizeof(lfn));
        // memcpy(&lfn,&dir,sizeof(lfn));
        lfn.attr = 0x0f;
        lfn.seq = 0x41;
        lfn.entry_type = 0x00;
        lfn.zero = 0x00;

        wchar_t xyu2[128];
        memset(xyu2, 0xFF, sizeof(xyu2));
        // swprintf(xyu2, L"%s", strlwr(filename));
        // strlwr(filename);
        unicode(filename, xyu2, 111);
        // deb("str %s => %S", filename, xyu2);
        memcpy(lfn.fn5, xyu2, 10);

        memcpy(lfn.fn6, xyu2+5, 12);
        memcpy(lfn.fn7, xyu2+5+6, 4);
        // dirptr = (LPVOID)((unsigned long)dirptr +(unsigned long)sizeof(LFNDIR));
        lfn.cksum = lfn_checksum((unsigned char*)&dir);
        memcpy((LPVOID)(dirptr), &lfn, sizeof(lfn));
        nr++;

        // deb("memcpy (%p)",dirptr);

        dirptr = (LPVOID)((unsigned long)dirptr +(unsigned long)sizeof(LFNDIR));
        memcpy((LPVOID)dirptr, &dir, sizeof(dir));
        nr++;
        deb("      %11s %8lu cls:%lu clustersused:%lu", dir.fname, (unsigned long)dir.size, clst,
            clustersused);
        // stc+=clustersused;
        fspace_ptr += size;
        i = 1024;
        nl = strtok_s(NULL, &i, "\r\n", &next_token3);

        // dirptr = (LPVOID)((unsigned long)dirptr +(unsigned long)sizeof(LFNDIR));

    }
    added+=nf;
    memset(&dir, 0, sizeof(dir));
    dirptr = (LPVOID)((unsigned long)dirptr +(unsigned long)sizeof(LFNDIR));
    // dirptr = (LPVOID)((unsigned long)memory+(unsigned long)clstbyte+
    // ((unsigned long)(nr?sizeof(dir):0) * ((unsigned long)nr?nr-1:1)));
    // unsigned long byte;
    // byte = (unsigned long)((unsigned long)dirptr-(unsigned long)memory);
    // // deb("memcpy(%p) byte %lu/%lu", dirptr, byte, (unsigned long)curdisksize);
    // if (((unsigned long)byte) >= ((unsigned long)curdisksize))
    // {
    // deb("mem need %lu MB, ram only %lu MB", (unsigned long)byte/1024/1024,
    // (unsigned long)curdisksize/1024/1024);
    // return false;
    // }
    memcpy(dirptr, &dir, sizeof(dir));
    deb("fataddfiles(%s): crc(%p): %x", dirname, memory, checksum((unsigned short*)memory, 4096));
    // deb("     %s %d files", spcrs, added);
    numit--;
    return added;
}

bool nconn(void)
{
    unsigned long iMode = 0;
    static int checkdone = true;

    // if (ramdisk)
    // {
    // deb("nconn ram used");
    // return false;
    // }

    numpkt = 0;
    shutdown(ConnectSocket, SD_BOTH);
    closesocket(ConnectSocket);

    deb("���������� � �������� ...");
    // Form1->Canvas->Font->Color=clRed;
    // Form1->Canvas->Font->Size=12;
    // Form1->deblog->Canvas->TextOutA(Form1->Left+Form1->deblog->Width/2,Form1->deblog->Height/2,"���������� ...");
    DWORD ms = GetTickCount();
    // Application->ProcessMessages();
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        deb("Client: Error at socket(): %ld\n", WSAGetLastError());

        return false;
    }
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(serv);
    clientService.sin_port = htons(11000);

    // iMode=!iMode;
    ioctlsocket(ConnectSocket, FIONBIO, (unsigned long*)&iMode);
    // iMode=99999999;
    ioctlsocket(ConnectSocket, FIONREAD, (unsigned long*)&iMode);

    iMode = true;
    // setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, (unsigned char*)&iMode, 1);
    setsockopt(ConnectSocket, IPPROTO_TCP, SO_DEBUG, (unsigned char*)&iMode, 1);
    setsockopt(ConnectSocket, IPPROTO_TCP, SO_KEEPALIVE, (unsigned char*)&iMode, 1);

    setsockopt(ConnectSocket, IPPROTO_TCP, SO_OOBINLINE, (unsigned char*)&iMode, 1);
    iMode = 999999999999;
    setsockopt(ConnectSocket, IPPROTO_TCP, SO_RCVBUF, (unsigned char*)&iMode, 4);
    setsockopt(ConnectSocket, IPPROTO_TCP, SO_SNDBUF, (unsigned char*)&iMode, 4);

    deb("tcp.socket max recv() size = %u", (unsigned long)iMode);
    iMode = true;

    if (connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
    {
        deb("Client: ���������� ��������� � ��������\n");

        return false;
    }

    ms = GetTickCount()-ms;
    deb("    �� %d ����\n", ms);
    Application->ProcessMessages();
    if (!checkdone)
    {
        deb("performing check ...");
        unsigned int sizo = 112000;
        char *testbuf = new char[sizo];
        int a = 1;

        for (int i = 0;i<sizo;i++)
        {
            testbuf[i] = i;
        }

        USHORT *us = (USHORT*)testbuf;
        *us = checksum((USHORT*)testbuf, sizeof(testbuf));

        unsigned long dwt = 0;
        unsigned long sz;
        LARGE_INTEGER ofs;
        ofs.QuadPart = 0;
        sz = sizo;
        dump(testbuf, sizo, "initial");
        OnWrite(NULL, NULL, testbuf, sz, ofs, 0, &dwt);
        // dump(testbuf, sizeof(testbuf), "awrite");
        memset(testbuf, 0, sizo);
        OnRead(NULL, NULL, testbuf, sz, ofs, 0, &dwt);
        dump(testbuf, sizo, "readed");
        deb("check done");
        checkdone = true;
        delete[](char*)testbuf;
    }

    static int numconn = 0;
    memset(&pkt, 0, sizeof(pkt));
    pkt.cmd = 3;
    pkt.offset = numconn++;
    // pkt.size = size;
    pkt.sign = 'A';
    pkt.size = disksize;
    pkt.paged = 0;
    pkt.preverr = err;
    err = 0;
    DWORD dwSiz = 64;
    GetComputerNameEx(ComputerNameDnsHostname, pkt.compid, &dwSiz);
    // dwSiz = 128-strlen(pkt.compid);
    // pkt.compid[strlen(pkt.compid)] = '-';
    // GetComputerNameEx(ComputerNameNetBIOS, pkt.compid+strlen(pkt.compid), &dwSiz);
    // pkt.crc = (unsigned long)checksum((unsigned short*)buf, size);
    int ss = send(ConnectSocket, (char*) &pkt, sizeof(pkt), 0);

    if (ss!=sizeof(pkt))
    {
        err = WSAGetLastError();
        deb("nconn:failed to send PACKET! sent only %d bytes err %d", ss, err);
        closesocket(ConnectSocket);
        ConnectSocket = 0;
    }
    else
    {
        deb("sent helo packet %d", ss);
    }
    Application->ProcessMessages();
    return true;
}

unsigned long ftppasvread(char *host, int port, char *buf, int bufsiz, unsigned long len)
{

    ftpdeb("   ftppasvread(%s, %d, 0x%p, %d, %lu) - %x", host, port, buf, bufsiz, len, rand());
    memset(buf, 0, 4096);
    tempsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tempsock == INVALID_SOCKET)
    {
        deb("Client: Error at socket(): %ld\n", WSAGetLastError());
        ftpstate = -1;
        return false;
    } // deb("tcp.socket max recv() size = %u", (unsigned long)iMode);

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    // ftpdeb("dns: resolve %s ...", host);
    clientService.sin_addr.s_addr = resolve(host);

    if (clientService.sin_addr.s_addr == -1)
    {
        deb("resolve %s failed", host);
        closesocket(tempsock);
        tempsock = 0;
        return false;
    }
    // ftpdeb("dns: IP %s", inet_ntoa(clientService.sin_addr));
    clientService.sin_port = htons(port);

    // iMode=!iMode;
    ULONG iMode = true;
    bool im = true;
    // iMode = true;

    // setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, (unsigned char*)&iMode, 1);
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_DEBUG, (unsigned char*)&im, 1);
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_KEEPALIVE, (unsigned char*)&im, 1);
    //
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_OOBINLINE, (unsigned char*)&im, 1);
    // iMode = 999999999999;
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_RCVBUF, (unsigned char*)&iMode, 4);
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_SNDBUF, (unsigned char*)&iMode, 4);

    // if (Form1->netdebug->Checked)
    // ftpdeb("ftppasvread read from %s:%d ...", host, port);
    // Form1->Canvas->Font->Color=clRed;
    // Form1->Canvas->Font->Size=12;
    // Form1->deblog->Canvas->TextOutA(Form1->Left+Form1->deblog->Width/2,Form1->deblog->Height/2,"���������� ...");
    DWORD ms = GetTickCount();
    int ret = 0;

    ftppsvblocking(false);
    ftpstate = 4;
    Form1->imgpsvread->Visible = true;
    Form1->imgpsvread->Repaint();
    int err;
    while ((ret = connect(tempsock, (SOCKADDR*)&clientService, sizeof(clientService))) == -1)
    {
        err = WSAGetLastError();

        if (err = 10056)
            break;

        // if (GetTickCount() - ms >= 900)
        // ftpdeb("ftppasvread: ���������� ��������� � �������� %s:%d\n\r  %s (%d)", host, port,
        // fmterr(err), err);

        Sleep(100);

        if (GetTickCount() - ms >= timeoutst*7)
        {
            ftpstate = -1;
            deb(" ! too many connect fails");
            ftpdeb(" ! failed to pasv connect %s:%d", host, port);
            return false;
        }
    }
    err = WSAGetLastError();
    char tb[222];
    // while (ftprl(tb, timeoutst, (char*)0)>0)
    // continue;
    // int err = WSAGetLastError();
    if (err && err != 10035)
    {
        deb("f:%d ret: %d %s in %d ms", err, ret, fmterr(err), GetTickCount()-ms);
        ftpdeb(" ! failed to pasv connect %s:%d", host, port);
        return false;
    }
    ftpstate = 2;
    ftppsvblocking(false);
    ftprl(tb, timeoutst, 0);

    if (!strstr(tb, "150"))
    {
        ftpdeb("list -l failed: no 150 code");
        return false;
    }
    ms = GetTickCount()-ms;
    // if (ms)
    // {
    int lport;
    struct sockaddr_in sa;

    int nl = sizeof(sa);
    getsockname(tempsock, (struct sockaddr*)&sa, &nl);
    lport = ntohs(sa.sin_port);

    clientService.sin_addr.s_addr = resolve(host);
    // ftpdeb(" dns: A %s", inet_ntoa(clientService.sin_addr));

    ftpdeb("    �������� � %s:%d - %u ����\n", inet_ntoa(clientService.sin_addr), lport, ms);
    // }
    // Form1->Button14->Enabled = true;
    // Application->ProcessMessages();
    // ftprl(tb, timeoutst, 0);
    int ss = 0;

    char *ptr = buf;
    bool cont = true;
    int readleft = len ? len:bufsiz;
    if (!len && !bufsiz)
    {
        readleft = 16384;
        cont = true;
    }
    bool exact = false;

    if (len == bufsiz && len)
        exact = true;

    int readok = 0;
    DWORD tms = GetTickCount();
    int numr = 0;
    char tempbuf[8192];
    bool tempreading = false;

    // ftplock();

    // ftprl(buf);
    DWORD ltms = 0;
    ftppsvblocking(false);
    DWORD lastinms = 0;
    while (true)
    {

        if (!readleft)
            break;
        ftpstate = 4;
        DWORD tms2 = GetTickCount();
        ss = recv(tempsock, (char*)ptr, readleft, 0);
        if (ss>0)
            ltms = GetTickCount();
        ftpstate = -1;
        tms2 = GetTickCount()-tms2;
        if (ss>0)
            lastinms = tms2;

        if (!readok)
        {
            ///  if (ss>0)

            // while (ftprl(tb, 1, (char*)0)>0)
            // continue;
            // ftpdeb("ss %d",ss);
        }
        // if (Form1->netdebug->Checked)
        lastdelay = tms2;
        if (ss>0)
        {
            ftpdeb("    pasv read %d - %4lu ��", ss, GetTickCount()-tms);
            lastpasvread = ss;
        }

        // if ((GetTickCount()-tms)>=timeoutst*3&& (ss <= 0 &&numr++>=1500))
        // {
        // // // if (Form1->netdebug->Checked)
        // ftpdeb("   ftp break read numr: %d", numr);
        // break;
        // }

        if (ss>0)
            numr = 0;

        // if (!tempreading)
        // {
        if (ss>0)
        {
            ptr = ptr+ss;
            if (ss>1)
                ilastlen = (double)ss;
            ibytesin+=(double)ss;
        }

        if ((readleft-ss)>=0 && ss>0)
            readleft-=ss;

        if (ss>0)
            readok+=ss;
        // }

        if (exact && readok >= len)
        {

            ftpblocking(true);

            // while (ftprl(tb, timeoutst, (char*)0)>0)
            // continue;
            shutdown(tempsock, SD_BOTH);
            closesocket(tempsock);
            sprintf(tb, "%c%c%c%c%cABOR\r\nABOR\r\n", 0xff, 0xf4, 0xff, 0xfd, 0x06);
            // // rr = send(FtpSocket, tb, strlen(tb), 0);
            ftpcmd(tb, 0, 0);

            if (ftprl(tb, timeoutst*2, 0) < 0)
            {
                ftpdeb(" ! No ABOR reply, reset");
                shutdown(FtpSocket, SD_BOTH);
                closesocket(FtpSocket);
                FtpSocket = 0;
                ftpstate = -2;
                // ftpconn(true);
                break;

            }

            // send(FtpSocket, 0, 0, 0);
            ftpstate = -1;

            DWORD ta = GetTickCount();

        }
        if (ss == 0 && readok && lastinms && GetTickCount()-ltms>=lastinms)
            break;

        if (ss == 0 && GetTickCount()-ltms>=timeoutst)
            break;

        // if (cont && ss == 0)
        // break;

        if (ss >0 && ss < 1000)
            break;
        Application->ProcessMessages();
    }

    if (ss<0)
    {
        int err = WSAGetLastError();
        deb("ftppasvread ss: %d", ss, err);
    }

    shutdown(tempsock, SD_BOTH);
    closesocket(tempsock);

    // ftpblocking(true);
    // ftprl(tb,timeoutst,0);
    Form1->imgpsvread->Visible = false;
    // ftppsvblocking(true);
    // if (Form1->netdebug->Checked)
    tms = GetTickCount()-tms;
    ftpdeb("   ftppasvread() = read %d, numr: %lu ss %d, %d ��", readok, numr, ss, tms);

    // shutdown(tempsock, SD_BOTH);

    // ftpunlock();
    ftpstate = -1;
    return ss == -1? ss:readok;
}

bool ftpconn(bool quick = false)
{
    ftpstate = -2;
    loggedin = false;
    unsigned long iMode = 1;
    static int checkdone = true;

    // if (ramdisk)
    // {
    // deb("nconn ram used");
    // return false;
    // }

    Form1->knotts->Title->Text->Text = "���������� ...";
    shutdown(FtpSocket, SD_BOTH);
    closesocket(FtpSocket);

    char host[128];
    deunicode(Form1->ftpip->Text.c_str(), host, sizeof(host));
    strncpy(ftphost, host, sizeof(ftphost));

    // Application->ProcessMessages();
    FtpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        deb("Client: Error at socket(): %ld\n", WSAGetLastError());

        return false;
    } // deb("tcp.socket max recv() size = %u", (unsigned long)iMode);

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = resolve(host); // inet_addr(host);
    clientService.sin_port = htons(21);

    if (clientService.sin_addr.s_addr == 0)
    {
        deb("failed to resolve: %s", fmterr(WSAGetLastError()));
        ftpdeb("resolve for %s: %s", ftphost, fmterr(WSAGetLastError()));
        return false;
    }

    iMode = false;
    ioctlsocket(FtpSocket, FIONBIO, (unsigned long*)&iMode);
    // iMode=99999999;
    ioctlsocket(FtpSocket, FIONREAD, (unsigned long*)&iMode);

    setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, (unsigned char*)&iMode, 1);
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_DEBUG, (unsigned char*)&iMode, 1);
    setsockopt(FtpSocket, IPPROTO_TCP, SO_KEEPALIVE, (unsigned char*)&iMode, 1);
    int lport;
    struct sockaddr_in sa;
    setsockopt(FtpSocket, IPPROTO_TCP, SO_OOBINLINE, (unsigned char*)&iMode, 1);
    iMode = 999999999999;
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_RCVBUF, (unsigned char*)&iMode, 4);
    // setsockopt(FtpSocket, IPPROTO_TCP, SO_SNDBUF, (unsigned char*)&iMode, 4);
    int nl = sizeof(sa);
    getsockname(FtpSocket, (struct sockaddr*)&sa, &nl);
    strcpy(ip, inet_ntoa(sa.sin_addr));
    deb("���������� � �������� %s (%s) ...", host, ip);
    // Form1->Canvas->Font->Color=clRed;
    // Form1->Canvas->Font->Size=12;
    // Form1->deblog->Canvas->TextOutA(Form1->Left+Form1->deblog->Width/2,Form1->deblog->Height/2,"���������� ...");
    DWORD ms = GetTickCount();
    ftpblocking(true);
    Form1->Button14->Caption = "���������";
    // ftpdeb(" %% correct delay %lu", timeoutst);
    if (!Form1->pg2->Canvas->LockCount)
    {
        Form1->knotts->Visible = true;
        Form1->knotts->Repaint();
    }
    int ret = connect(FtpSocket, (SOCKADDR*)&clientService, sizeof(clientService));
    if (ret == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        deb("Client: ���������� ��������� � �������� %s\n\r  %s", host, fmterr(err));

        return false;
    }

    ms = GetTickCount()-ms;
    timeoutst = 5000;
    // if (ms <= 1000)
    // timeoutst = 1000;
    // else
    // timeoutst = (unsigned long)((double)timeoutst*(double)1.8);

    ftpdeb(" %% starting packet delay %lu msec", timeoutst);

    Form1->Button14->Enabled = true;

    lport = ntohs(sa.sin_port);
    strcpy(ip, inet_ntoa(sa.sin_addr));
    ftplport = lport;
    ftpdeb("  �������� %u ��, laddr=%s:%d", ms, inet_ntoa(sa.sin_addr), lport);

    char buf[65535];
    ftpstate = -1;
    if (ms)
        deb("    �� %d ����\n", ms);
    // Form1->Button14->Enabled = true;
    // Application->ProcessMessages();
    // ftpcmd("STaT", 0, 0);
    Form1->knotts->Title->Text->Text = "����������� �������� ...";
    while (ftprl(buf, timeoutst, (char*)0)<0)
        continue;
    Form1->knotts->Title->Text->Text = buf;
    // tms = (DWORD) ((double)tms*(double)1.5);
    // timeoutst = (DWORD)(double)tms*((double)totr/1200.0);
    unsigned long wtimeoutst = GetTickCount();

    int ss;

    char user[111] = "";
    char pass[111] = "";

    if (Form1->anonymous->Checked)
    {
        strcpy(user, "anonymous");
        char comp[255];
        unsigned long zize = sizeof(comp);
        GetComputerName(comp, &zize);
        snprintf(pass, sizeof(pass), "%s@%s", strlwr(comp), strlwr(ftphost));
    }
    else
    {
        deunicode(Form1->ftpuser->Text.c_str(), user, sizeof(user));
        deunicode(Form1->ftppass->Text.c_str(), pass, sizeof(pass));
    }
    Form1->knotts->Title->Text->Text = "����� ...";
    sprintf(buf, "USER %s", user);
    ftpcmd(buf, 0, 0);
    while (ftprl(buf, timeoutst, (char*)0)>0)
        continue;
    Form1->knotts->Title->Text->Text = "������ ...";
    sprintf(buf, "PASS %s", pass);
    // ftpdeb("pass [%s]",pass);
    ftpcmd(buf, 0, 0);
    // ftprl(buf, timeoutst*2, (char*)0);
    // ss = send(FtpSocket, (char*)buf, strlen(buf), 0);
    // ftprl(buf, 6500, (char*)0);
    while (ftprl(buf, timeoutst*2, (char*)0)>0)
        if (!strstr(buf, "230") && !loggedin)
        {
            Form1->knotts->Title->Text->Text = buf;
            deb("   ������ ��� ��������������");
            ftpdeb(" > ������ ��������������");
            closesocket(FtpSocket);
            FtpSocket = 0;

            return false;
        }
        else
        {
            loggedin = true;

        }
    // loggedin=true;
    if (!quick)
    {
        deb("�������� � ���");

        char *ftpcmds[] =
        {
            "SYST", "TYPE A", "PASV", NULL
        };

        char *p;
        for (int i = 0;ftpcmds[i];i++)
        {
            Form1->knotts->Title->Text->Text = ftpcmds[i]; // "������������� %s ...";
            deb("  # %s", ftpcmds[i]);
            sprintf(buf, "%s", ftpcmds[i]);
            ftpcmd(buf, 0, 0);
            ftprl(buf, timeoutst*2, 0);

            deb("  > %s", buf);

            // }
        }
        sprintf(buf, "LIST -l");
        ftpcmd(buf, 0, 0);
        Form1->knotts->Title->Text->Text = "������ ������ ������ ...";
        deb("������ ������ ������ ...");

        // continue;
        // ftprl(buf, timeoutst, (char*)0);
        ftppasvread(ftphost, ftpport, buf, 0, 4096);
        // deb("%s", buf);
        // char tb[1111];

        // continue;
        char *nt, *strtk = 0;
        unsigned int i1 = 99999;

        strcpy(fatfiles, buf);
        while (ftprl(buf, timeoutst, (char*)0)>0)
            continue;
        // ftprl(buf, timeoutst, (char*)0);
        // continue;

    }
    else
    {
        sprintf(buf, "TYPE I");

        ftpcmd(buf, 0, 0);
        ftprl(buf, timeoutst, (char*)0);

        deb("reconnected to ftp.");
    }
    deb("ftp list: %s", fatfiles);
    // ftplock();
    //
    // int canread;
    // ftpblocking(false);
    // while ((canread = recv(FtpSocket, buf, 1111, MSG_PEEK))>0)
    // {
    // // deb("reading OOB %d bytes", canread);
    // memset(buf, 0, 1111);
    // recv(FtpSocket, buf, canread, 0);
    // deb("OOB: %s", buf);
    // ftpdeb("OOB: %s", buf);
    // }
    // ftpblocking(true);
    //
    // ftpunlock();
    Application->ProcessMessages();
    return true;
}

unsigned long nread(unsigned long buf, unsigned long offset, unsigned long size, bool paged)
{
    // cev->SetEvent();
    // deb("nread enter cchs");
    cchs->Enter();
    // deb("nread entered cchs");

    if (!ConnectSocket)
        nconn();

    memset(&pkt, 0, sizeof(pkt));
    pkt.cmd = 0;
    pkt.offset = offset;
    pkt.size = size;
    pkt.sign = 'A';
    pkt.flags = 0;

    if (paged)
        pkt.flags |= 0x00000002;
    if (precache)
        pkt.flags |= 0x00000004;

    DWORD dwSiz = 64;
    GetComputerNameEx(ComputerNameDnsHostname, pkt.compid, &dwSiz);
    dwSiz = 128-strlen(pkt.compid);
    GetComputerNameEx(ComputerNameNetBIOS, pkt.compid+strlen(pkt.compid), &dwSiz);

    pkt.preverr = err;
    pkt.crc = (unsigned long)checksum((unsigned short*)buf, size);

    int ss = send(ConnectSocket, (char*) &pkt, sizeof(pkt), 0);

    if (ss!=sizeof(pkt))
    {
        err = WSAGetLastError();
        deb("failed to send PACKET! sent only %d bytes err %d", ss, err);
    }

    unsigned long readok = 0, readleft = size;
    unsigned long p = 0;
    double pr;
    int len;
    char str[128];

    int rds = 0;
    while (readleft)
    {
        rds++;
        p = (unsigned long)((unsigned long)buf+(unsigned long)readok);

        long rn = readleft<118000?readleft:118000;

        len = recv(ConnectSocket, (char*)p, rn, 0);
        if (len>0)
        {
            ibytesin+=len;
            if (ss>1)
                ilastlen = (double)len;

        }
        if (len!=readleft)
        {
            err = WSAGetLastError();

            pr = readok+len;
            pr = (pr/size)*100.0;
            deb("   ��������� %5d �� %-8lu %.2f%%", len, readleft, pr, err);
            if (err)
                deb(" err=%d:   %s", err, fmterr(err));

            Application->ProcessMessages();
            if (len<=0||err)
            {
                ConnectSocket = 0;
                cchs->Leave();
                // deb("nread leaved cchs");
                return readok;
            }
        }
        readleft -= len;
        readok += len;

        sprintf(str, "%6d #%-4d %3.2f%%", len, rds, pr);
        Form1->readb->Text = str;
        Application->ProcessMessages();
    }

    int opt = 1;
    setsockopt(ConnectSocket, SOL_SOCKET, SO_DONTLINGER, (char*) &opt, 1);
    opt = 65535;
    setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVBUF, (char*)&opt, sizeof(opt));
    setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDBUF, (char*)&opt, sizeof(opt));

    cchs->Leave();
    Form1->readb->Text = "-";
    Application->ProcessMessages();
    // deb("nread leaved cchs");
    return readok;
}

unsigned long nwrite(unsigned long buf, unsigned long offset, unsigned long size, bool paged,
    bool cachesave = false, int flags = 0, char *addon = NULL)
{
    cchs->Enter();
    // cev->SetEvent();
    if (!ConnectSocket)
        nconn();

    memset(&pkt, 0, sizeof(pkt));
    pkt.cmd = 1;
    pkt.offset = offset;
    pkt.size = size;
    pkt.sign = 'A';
    pkt.flags = 0;
    if (addon)
        strncpy(pkt.preverrstr, addon, sizeof(pkt.preverrstr));

    if (paged)
        pkt.flags |= 0x00000002;
    if (cachesave)
        pkt.flags |= 0x00000008;
    if (flags)
        pkt.flags |= flags;
    DWORD dwSiz = 64;
    GetComputerNameEx(ComputerNameDnsHostname, pkt.compid, &dwSiz);
    dwSiz = 128-strlen(pkt.compid);
    GetComputerNameEx(ComputerNameNetBIOS, pkt.compid+strlen(pkt.compid), &dwSiz);
    pkt.preverr = err;
    pkt.crc = (unsigned long)checksum((unsigned short*)buf, size);

    int ss = send(ConnectSocket, (char*) &pkt, sizeof(pkt), 0);
    if (ss!=sizeof(pkt))
    {
        err = WSAGetLastError();
        deb("failed to send PACKET! sent only %d bytes err %d", ss, err);
        closesocket(ConnectSocket);
        ConnectSocket = 0;
        nconn();
        cchs->Leave();
        return -1;
    }
    ULONG writeok = 0, writeleft = size;
    unsigned long p = 0;

    while (writeleft)
    {
        p = (unsigned long)((unsigned long)buf+(unsigned long)writeok);
        int len;
        len = send(ConnectSocket, (char*)p, writeleft, 0);
        if (len>0)
        {
            obytesin+=len;
            olastlen = len;

        }
        err = WSAGetLastError();
        if (len<=0||err)
        {
            // ConnectSocket=0;
            cchs->Leave();
            return 0;
        }
        if (len>writeleft)
        {
            nconn();
            cchs->Leave();
            return 0;
        }
        // nconn();

        if (len!=writeleft)
        {

            deb(" write just %d of %u,err=%d", len, writeleft, err);
            if (err)
                deb("   %s", fmterr(err));

        }
        writeleft-=len;
        writeok+=len;
        char str[128];
        sprintf(str, "%d", len);
        // Form1->
    }
    cchs->Leave();
    return writeok;
}

void paintdisk(int what, LARGE_INTEGER off, ULONG len, bool paging = false, unsigned short crc = 0)
{
    // if (!Form1->debcheck->Checked)
    // return;
    static DWORD tms = 0;
    static double si;
    static double si2;

    // Form1->imgptr->Left = Form1->ptr->Left;

    try
    {
        if ((double)off.QuadPart >= (double)maxquad)
        {
            maxquad = (double)(off.QuadPart);
            // Form1->OnPaint(NULL);
        }

        static double temp, temp2;
        temp = (double)off.QuadPart;
        temp2 = (double)Form1->pb1->Width;

        // if(temp > ((double)maxquad*0.7))
        // {
        // maxquad = temp*1.4;
        //
        // }
        if (!diskcolor)
        {

            diskcolor = new unsigned int[Form1->pb1->Width];

            for (UINT i = 0;i<Form1->pb1->Width;i++)
                diskcolor[i] = clWhite;

        }
        if (temp > 0 && maxquad > 0 & temp2 > 0)
            si = (temp/maxquad) * (temp2);

        if (what == 9&&diskcolor)
        {

            diskcolor[(unsigned int)si] = len;
        }

        tms = GetTickCount();

        // if (ramdisk)
        // return;
        char ss[128];

        if ((double)temp > ((double)maxquad))
        {
            maxquad = (double)temp;
            // Form1->Repaint();

        }
        si = (double)((double)temp/(double)(maxquad)) * ((double)temp2);

        avg = (double)si;

        char avgstr[111];
        sprintf(avgstr, "%-3.2f MB - %.2f", temp/1024/1024, avg);
        // Form1->secnum->Text = avgstr;
        // deb("avg %.2f", avg);
        if (Form1->debcheck->Checked)
        {
            Form1->headptr->Visible = false;
            Form1->headptr->Top = Form1->pb1->Top-13;
            if (avg>1.0&&avg<Form1->pb1->Width)
                Form1->headptr->Left = (int)avg;
            // deb("set avg to %.0f", avg);
            //
            Form1->headptr->Visible = true;
        }
        // Application->ProcessMessages();
        char *swhat = "<��������>";
        switch(what)
        {
            case 1:
            swhat = "������";
            Form1->ptr->Font->Color = clBlack; // clTeal;
            sprintf(ss, "%6s %5lu @ %I64d ", swhat, len, off.QuadPart);
            if (Form1->debcheck->Checked)
                Form1->ptr->Text = ss;
            break;
            case 0:
            swhat = "������";
            Form1->ptr->Font->Color = clBlack;
            sprintf(ss, "%6s %5lu @ %I64d ", swhat, len, off.QuadPart);
            if (Form1->debcheck->Checked)
                Form1->ptr->Text = ss;
            break;
            case 5:
            Form1->ptr->Font->Color = clGreen;
            swhat = "������ ����";
            sprintf(ss, "%6s %5lu @ %I64d ", swhat, len, off.QuadPart);
            if (Form1->debcheck->Checked)
                Form1->ptr->Text = ss;
            break;

            default:
            Form1->ptr->Font->Color = clLtGray;
            // ss[0] = 0;
            // sprintf(ss, "%5s", swhat);
            break;
        }

        // Form1->ptr->Font->Size = 9;

        if (paging)
            Form1->imgpaging->Visible = true; // Form1->ptr->Font->Color = clRed;
        else
            Form1->imgpaging->Visible = false; // Form1->ptr->Font->Color = clBlue;

        // Form1->ptr->Width = Form1->Canvas->TextWidth(ss)*2;

        switch(what)
        {
            case 0:

            for (int i = (int)avg;i<(int)avg+1;i++)
            {

                unsigned char r, g, b;
                unsigned int fourBytes;
                fourBytes = diskcolor[i];
                // caches 4 bytes at a time
                r = (fourBytes>>16);
                g = (fourBytes>>8);
                b = fourBytes;

                r-=85;
                b-=85;
                g-=85;

                // for (int u = 1;u<11;u++)
                diskcolor[i] = RGB(r, g, b);
                // Form1->pb1->Canvas->Pixels[i][1]-50-len;
                // Form1->pb1->Canvas->Pixels[i][u] = Form1->pb1->Canvas->Pixels[i][u]+50+len; // clRed;
            }
            break;
            case 5:
            case 1:

            for (int i = (int)avg;i<(int)avg+1;i++)
            {
                unsigned char r, g, b;
                unsigned int fourBytes;
                fourBytes = diskcolor[i];
                // caches 4 bytes at a time
                r = (fourBytes>>16);
                g = (fourBytes>>8);
                b = fourBytes;
                // r+=45;
                // b+=45;
                // g+=45;
                diskcolor[i] = RGB(r, g, b);
            }
            break;

            case 4:
            Form1->diskptr->Left = 0;

            // sprintf(ss, "<��������>");
            // Form1->ptr->Text = ss;
            Form1->ptr->Font->Color = clLtGray;
            // Form1->imgptr->Visible = false;
            break;

            case 9:
            // deb("avg %.2f avg2 %.2f ofs %I64d", avg, avg2,off.QuadPart);
            // for (double i = avg;i<avg+1;i++)
            // {

            diskcolor[(int)avg] = len;

            // }
            break;
        }

    }
    catch(...)
    {
        deb("exception while paintdisk");
    }
    if (!paging)
    {
        // Form1->diskptr->Repaint();
        // Form1->ptr->Repaint();
    }

    // Form1->Repaint();

}

bool in_cache(long offset, ULONG len)
{
    bool incache = false;

    // deb("in_cache enter ccs");
    Application->ProcessMessages();

    ccs->Enter();

    // deb("ok enter ccs");
    Application->ProcessMessages();

    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        if (offset == (*it).offset)
        {

            incache = true;
        }
    }

    ccs->Leave();
    // deb("leave ccs");
    Application->ProcessMessages();

    return incache;
}

long cache_read(long offset, LPVOID buf, ULONG len)
{
    // cev->SetEvent();
    ccs->Enter();
    long hits = 0;
    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        if ((*it).offset == offset)
        {
            if (len > (*it).len)
            {
                char str[1024];
                sprintf(str,
                    "  * ���: �� ������� ������ %d @ %d : \r\n  ����� %d > ���� %d, �����������",
                    len, offset, len, (*it).len);
                deb(str);
                // MessageBox(NULL, str, "check console. critical error.", MB_OK);
                HeapFree(GetProcessHeap(), 0, (*it).buf);
                totcachsize-=(*it).len;
                it = cache.erase(it);

                break;
            }

            memcpy(buf, (*it).buf, len);
            hits = (*it).hits++;
            break;
        }
    }
    ccs->Leave();
    return hits;
}

bool cache_write(long offset, LPVOID buf, ULONG len, int *hits, bool unsaved = true)
{
    bool found = false;
    bool notcached = true;
    int upds = 0;
    LARGE_INTEGER li;
    // cev->SetEvent();
    ccs->Enter();

    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        if ((*it).offset == offset)
        {
            found = true;
            if (len > (*it).len)
            {
                li.QuadPart = (*it).offset;
                paintdisk(5, li, (*it).len, false);
                char str[128];
                sprintf(str, "  * ���: ������� ���� %d @ %d: \r\n    ������ %5d, ����� %5d",
                    (*it).len, (*it).offset, (*it).len, len);

                deb(str);

                nwrite((unsigned long)(*it).buf, (*it).offset, (*it).len, 0, false, 0x00000010,
                    str);

                HeapFree(GetProcessHeap(), 0, (*it).buf);
                totcachsize-=(*it).len;
                totcachsize += len;
                (*it).buf = HeapAlloc(GetProcessHeap(), 0, len);
                (*it).len = len;
                paintdisk(4, li, 1, 0);
                // (*it).unsaved = false;

            }
            // deb("cache overwrite");
            memcpy((*it).buf, buf, len);
            //

            (*it).changed = GetTickCount();
            (*it).unsaved = unsaved;
            (*it).opid = numpkt;
            (*it).updates++;
            if (hits)
                *hits = (*it).hits;
            upds = (*it).updates++;
            break;
        }
    }

    if (!found)
    {
        CACHE ch;
        ch.offset = offset;
        ch.len = len;
        ch.buf = HeapAlloc(GetProcessHeap(), 0, len);
        memcpy(ch.buf, buf, len);

        totcachsize+=len;
        ch.changed = GetTickCount();
        ch.unsaved = unsaved;
        ch.opid = numpkt;
        ch.updates = 0;
        ch.hits = 0;
        upds = 0;
        cache.push_back(ch);
        // deb("   + ������������ %lu ����", len, offset);
        // upds=false;
    }
    ccs->Leave();
    return upds;
}

int nreadfile(char *fn, unsigned long start_offset, int len, char *buf)
{
    PKT pkt;
    memset(&pkt, 0, sizeof(pkt));
    pkt.sign = 'A';
    pkt.cmd = 7;
    strcpy(pkt.preverrstr, fn);
    pkt.offset = (unsigned long)start_offset;
    pkt.size = (unsigned long)len;

    cchs->Enter();

    if (!ConnectSocket)
    {
        deb("socket is not connected!");
        if (!nconn())
        {
            deb("failed to connect");
            cchs->Leave();
            return -1;
        }
    }
    LARGE_INTEGER xyu;
    int nrecv = -1;
    int nsend = send(ConnectSocket, (char*)&pkt, sizeof(pkt), 0);
    if (nsend < 0)
    {
        deb("send only %d bytes of file reqyest %s", nsend, fn);
        shutdown(ConnectSocket, SD_BOTH);
        closesocket(ConnectSocket);
        ConnectSocket = NULL;
        nconn();
        int nsend = send(ConnectSocket, (char*)&pkt, sizeof(pkt), 0);
        if (nsend != sizeof(PKT))
        {
            deb("failed to restore connection.");
            cchs->Leave();
            return nsend;
        }
    }
    else
    {

        obytesin+=nsend;
        olastlen = nsend;

        xyu.QuadPart = start_offset;
        paintdisk(1, xyu, len, false, 0);
    }

    nrecv = recv(ConnectSocket, buf, len, 0);
    if (nrecv < 0)
    {
        deb("recieved only %d bytes of file %s", nrecv, fn);
        shutdown(ConnectSocket, SD_BOTH);
        closesocket(ConnectSocket);
        ConnectSocket = NULL;
        // nconn();
    }
    else
    {

        ibytesin+=nrecv;
        if (nrecv>1)
            ilastlen = (double)nrecv;

    }
    paintdisk(4, xyu, len, false, 0);
    cchs->Leave();
    return nrecv;
}

unsigned long ftpgetfile(char *fname, unsigned long offset, char *buf, unsigned long len)
{
    char tb[512];
    int rr = 0;

    if (!FtpSocket)
        ftpconn(true);

    strcpy(ftpgetfilename, fname);
    ftpgetfileoffset = offset;
    ftpgetfilelen = len;
    Application->ProcessMessages();
    sprintf(tb, "read %s %lu @ %lu", fname, len, offset);
    Form1->ftpe->Text = tb;

    unsigned long iMode = 1;
    ioctlsocket(FtpSocket, FIONBIO, (unsigned long*) &iMode);
    ioctlsocket(FtpSocket, FIONREAD, (unsigned long*)&iMode);

    int canread;
    while ((canread = recv(FtpSocket, buf, len, MSG_PEEK))>0)
    {
        // deb("reading OOB %d bytes", canread);
        memset(buf, 0, len);
        recv(FtpSocket, buf, canread, 0);
        // deb("OOB: %s", buf);
    }
    iMode = false;
    ioctlsocket(FtpSocket, FIONBIO, (unsigned long*) &iMode);
    ioctlsocket(FtpSocket, FIONREAD, (unsigned long*)&iMode);

    sprintf(tb, "EPSV");
    // rr = send(FtpSocket, tb, strlen(tb), 0);
    ftpcmd(tb, 0, 0);
    memset(tb, 0, sizeof(tb));
    DWORD tmepsv = GetTickCount();
    // while (!ftprl(tb, 1500, "|||"))
    // {
    //
    // // deb(".");
    //
    // if (GetTickCount()-tmepsv>=2000)
    // {
    // ftpdeb(" ! RST: ��� �� �������� �� %s", lastftpcmd);
    // shutdown(FtpSocket, SD_BOTH);
    // closesocket(FtpSocket);
    // FtpSocket = 0;
    // ftpconn(true);
    // ftpstate = -1;
    // strcpy(ftpgetfilename, "");
    // return -1;
    // }
    // }
    ftprl(tb, 3000, "229");

    if (char*p = strstr(tb, "|||"))
    {
        sscanf(p, "|||%u", &ftpport);

    }
    else
    {
        ftpdeb(" ! no reply EPSV!");
        ftpstate = -1;
        return -1;
    }
    // }

    if (offset)
    {

        sprintf(tb, "REST %lu", offset);
        // rr = send(FtpSocket, tb, strlen(tb), 0);
        ftpcmd(tb, 0, 0);
        tb[0] = 0;
        memset(tb, 0, sizeof(tb));
        while (!ftprl(tb, 3000, "350"))
        {
            continue;
        }

    }
    DWORD tms = GetTickCount();
    sprintf(tb, "RETR %s", fname);
    ftpcmd(tb, 0, 0);
    ftprl(tb, timeoutst, 0);

    unsigned long rrr = ftppasvread(ftphost, ftpport, buf, len, len);

    memset(tb, 0, sizeof(tb));
    // ftpblocking(false);
    ftprl(tb, timeoutst, 0);
    // ftpblocking(true);

    sprintf(tb, "free");
    Form1->ftpe->Text = tb;

    return rrr;
}

DWORD __stdcall OnRead(IN HANDLE hDisk, IN PVOID Context, IN PVOID Buffer, IN ULONG Length,
    IN LARGE_INTEGER ByteOffset, IN BOOL PagingIo, OUT PULONG BytesRead)
{
    bool cached = false;
    char str[128];
    char sdf[1111];
    if (unmounting)
        return IO_OP_STATUS_UNSUCCESSFUL;
    ecs->Enter();

    sprintf(sdf, "OnRead(%8p, %5lu, @ %I64d, PagingIo: %d)", Buffer, Length, ByteOffset.QuadPart,
        PagingIo);
    OutputDebugString(sdf);
    Application->ProcessMessages();

    if (Form1->debcheck->Checked)
        deb(sdf);
    char qqq[222];
    char trys[111];

    if (ByteOffset.QuadPart!=rlastoffset)
        rntrys = 0;
    else
        rntrys++;
    rlastoffset = ByteOffset.QuadPart;
    trys[0] = 0;

    // for (unsigned long i = 0;i<((floor(ntrys/20)*10));i++)
    // {
    // strncat(trys, ".", 50);
    // }
    sprintf(qqq, "r#<unknown> sector:%3lu repeat:%lu",
        ((unsigned long)ByteOffset.QuadPart/(unsigned long)fat32boot.bytes_per_sector),
        rntrys);
    if (Form1->unkdeb->Checked)
        Form1->imgptr->Caption = qqq;
    str[0] = qqq[0];
    str[1] = 0;
    Form1->headptr->Caption = str;
    DWORD tms = GetTickCount();
    LARGE_INTEGER bka;
    bka = ByteOffset;
    paintdisk(1, bka, Length, PagingIo, 0);
    // deb("OnRead thr deb() p(1, %I64d, %u, %d", bka.QuadPart, Length, PagingIo);

    // if (!memory||!disksize)
    // {
    // char ss[128];
    // sprintf(ss, "fail! memory=%x disksize=%u", memory, disksize);
    //
    // OutputDebugString(ss);
    //
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // }
    char ccc[111];
    if ((ByteOffset.QuadPart >= pfat1 &&(unsigned long)ByteOffset.QuadPart+(unsigned long)Length <=
            (unsigned long)pfat2+(unsigned long)fatsize))
    {

        if (Form1->fatdeb->Checked)
            deb("   read of FAT len %d ofs %d crc %x", (int)Length, (int)ByteOffset.QuadPart,
            checksum((unsigned short*)((long)memory+ByteOffset.QuadPart), Length));

        sprintf(ccc, "r#FAT clstr:%3lu sect: %lu", 0,
            (unsigned long)ByteOffset.QuadPart/fat32boot.bytes_per_sector);
        if (Form1->fatdeb->Checked)
            if (Form1->fatdeb->Checked)
                Form1->imgptr->Caption = ccc;

    }
    if (ByteOffset.QuadPart >= 1024 && ByteOffset.QuadPart <= pfat1)
    {
        if (Form1->debcheck->Checked)
            deb("   read of reserved sector len %d ofs %d", (int)Length, (int)ByteOffset.QuadPart);
        sprintf(ccc, "r#RESERVED sector %lu repeat:%lu",
            (unsigned long)(ByteOffset.QuadPart/(LONGLONG)fat32boot.bytes_per_sector), rntrys);
        Form1->imgptr->Caption = ccc;
    }

    if (ByteOffset.QuadPart >= rootdirectory && ByteOffset.QuadPart <= rootdirectory+files.size()
        *2*sizeof(DIR))
    {
        if (Form1->debcheck->Checked)
            deb("    read of root directory len %d ofs %d", (int)Length, (int)ByteOffset.QuadPart);
        Form1->imgptr->Caption = "r#DIR";
    }

    // if (ByteOffset.QuadPart > curdisksize || (ByteOffset.QuadPart+Length) > curdisksize)
    // {
    // unsigned long newdisksize;
    // newdisksize = (ByteOffset.QuadPart+Length)*2;
    // // deb("need to resize disk to %u MB", newdisksize/1024/1024);
    //
    // memory = HeapReAlloc(GetProcessHeap(), 0, memory, newdisksize);
    // if (!memory)
    // {
    // deb("failed to realloc: %s", fmterr());
    // ecs->Leave();
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // }
    // deb("Reallocated mem to %lu MB @ %p", newdisksize/1024/1024, memory);
    // curdisksize = newdisksize;
    // }
    //
    for (vector<FATFILE>::iterator it = files.begin();it!=files.end();it++)
    {
        if (!(*it).start_offset)
            continue;

        unsigned long ptr;
        unsigned long flen = 0;
        unsigned long stofs, enofs;

        if (ByteOffset.QuadPart >= (LONGLONG)(*it).start_offset && ByteOffset.QuadPart+Length <=
            (LONGLONG)(*it).end_offset)
        {
            unsigned long clst;
            clst = getbytecluster((unsigned long)ByteOffset.QuadPart);
            // sprintf(sdf, "OnRead(%8p, %5lu, @ %I64d (clst %lu), PagingIo: %d)", Buffer, Length,
            // ByteOffset.QuadPart, clst, PagingIo);
            // deb(sdf);

            ptr = (*it).size - ((*it).end_offset - ByteOffset.QuadPart);
            ptr /= clstsize;
            stofs = (unsigned long)((unsigned long)ByteOffset.QuadPart - (*it).start_offset);
            enofs = (unsigned long)((unsigned long)ByteOffset.QuadPart - (*it)
                .start_offset +Length);
            deb("READ %s/%-15s %6lu %8lu - %8lu\r\n [stc:%lu@%lu - %lu@%lu] (siz:%lu) ",
                (*it).ftppath, (*it).fn, (int)Length, (unsigned long)stofs, (unsigned long)enofs,
                (*it).firstcluster, getclusterbyte((*it).firstcluster),
                (*it).firstcluster+(*it).clusters-1,
                getclusterbyte((*it).firstcluster+(*it).clusters-1), (unsigned long)(*it).size);

            // memcpy(Buffer, (LPVOID)((unsigned long)memory+ByteOffset.QuadPart), Length);
            // *BytesRead = (unsigned long)Length;
            // return IO_OP_STATUS_SUCCESS;

            char fs[1293];
            sprintf(fs, "w#%s:%d", (*it).fn, ptr);
            Form1->imgptr->Caption = fs;
            (*it).accesses++;

            // if (Length > (*it).size)
            flen = Length;

            if (!(*it).fetched)
            {
                if ((*it).dir)
                {
                    char *buf2;
                    buf2 = new char[1*1024*1024];

                    char curdir[1024];
                    strncpy(curdir, ftppwd(), sizeof(curdir));
                    if (strlen((*it).ftppath)>2)
                    {
                        ftpcd((*it).ftppath);
                        ftprl(fs, timeoutst, 0);
                    }
                    ftpcd((*it).fn);
                    ftprl(fs, timeoutst, 0);

                    Form1->imgptr->Caption = (*it).fn;

                    char buv[1024];

                    ftpcmd("PASV", 0, 0);
                    ftprl(buv, timeoutst, "227");

                    ftpcmd("LIST -l", 0, 0);
                    // ftprl(buv, timeoutst, 0);

                    if (!buf2)
                    {
                        deb("memory failed");
                        return false;
                    }
                    // deb("fn %s", (*it).fn);
                    // deb("fc %lu", (*it).firstcluster);
                    // deb("buf2 %p", buf2);
                    memset(buf2, 0, 1*1024*1024);

                    ftppasvread(ftphost, ftpport, buf2, 0, 1*1024*1024);
                    ftprl(buv, timeoutst, 0);
                    // deb("fataddfiles(%s,%lu,%p)...", (*it).fn, (*it).firstcluster, buf2);
                    // ftprl(buv, timeoutst, 0);

                    fataddfiles((*it).fn, (*it).memory, (*it).firstcluster, buf2);

                    // deb("eject");
                    // VdskEjectMedia(hdisk);
                    // LARGE_INTEGER size;
                    // size.QuadPart=(LONGLONG)disksize;
                    // deb("setdisk");
                    // VdskSetDisk(hdisk, size, 2, 32, 0, VDSK_DISK_REMOVABLE|VDSK_DISK_MEDIA_IN_DEVICE,
                    // true, true);
                    // it=files.begin();

                    ftpcd(curdir);
                    ftprl(buf2, timeoutst, 0);
                    delete[]buf2;
                }
                else
                {
                    char curdir[1024];
                    strncpy(curdir, ftppwd(), sizeof(curdir));
                    ftpcd((*it).ftppath);
                    ftprl(fs, timeoutst, 0);
                    Form1->imgptr->Caption = (*it).fn;

                    unsigned long nrd = ftpgetfile((*it).fn, (unsigned long)stofs, (char*)Buffer,
                        flen);

                    ftpcd("..");
                }
            }
            ecs->Leave();

            // memcpy(Buffer, (LPVOID)((unsigned long)memory+(unsigned long)ByteOffset.QuadPart),
            // Length);
            if ((*it).memory)
            {
                deb("memcpy (%p,%p,%lu)", Buffer, (*it).memory, (unsigned long)Length);
                memcpy(Buffer, (*it).memory, Length);
                deb("done");
            }
            else
            {
                deb("no memory for %s", (*it).fn);
            }

            *BytesRead = (unsigned long)Length;
            return IO_OP_STATUS_SUCCESS;
        }
    }
    ecs->Leave();

    if (ByteOffset.QuadPart+Length>=curdisksize)
    {
        deb("unk read!");
        return -1;
    }

    memcpy(Buffer, (LPVOID)((unsigned long)memory+ByteOffset.QuadPart), Length);
    *BytesRead = (unsigned long)Length;
    // deb("done read OK of %u bytes ", Length);
    return IO_OP_STATUS_SUCCESS;
    //
    // totreads++;
    // // ramdisk = Form1->ramonly->Checked;
    //
    // if (ramdisk)
    // {
    //
    // // OutputDebugString("paintdisk/memcpy");
    // LARGE_INTEGER bka;
    // bka.QuadPart = ByteOffset.QuadPart;
    //
    // if (Length>=disksize)
    // {
    // deb("onread too big len");
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // }
    // *BytesRead = Length;
    // memcpy(Buffer, (char*)memory+ByteOffset.QuadPart, Length);
    // // OutputDebugString(" done pnt/cpy");
    //
    // // Form1->debcheck->Checked = false;
    // // }
    // // else
    // // if (dp < 5.0 ||(!tms && !Form1->debcheck->Checked && !tmsleft--))
    // // {
    // // tmsleft = 15;
    // // // Form1->debcheck->Checked = true;
    // // }
    // tms = GetTickCount()-tms;
    // if (Form1->debcheck->Checked)
    // deb("#%3d READ  %5lu @ %-8I64d CRC=0x%04X [%4d ��]", numpkt++, Length,
    // ByteOffset.QuadPart, checksum((unsigned short*)Buffer, Length), tms);
    // paintdisk(4, ByteOffset, 1, 0);
    // return IO_OP_STATUS_SUCCESS;
    //
    // }
    //
    // if (Form1->Button1->Enabled)
    // {
    // deb("read unsuccess");
    // // Application->ProcessMessages();
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // }
    //
    // numpkt++;
    // // cev->SetEvent();
    //
    // // deb("checking cache ...");
    // // Application->ProcessMessages();
    // cached = in_cache(ByteOffset.QuadPart, Length);
    // // deb("cached %d", cached);
    // // Application->ProcessMessages();
    // if (cached)
    // {
    // long hits = cache_read(ByteOffset.QuadPart, Buffer, Length);
    // tms = GetTickCount()-tms;
    // deb("#%3d READ  %5lu @ %-8I64d CRC=0x%04X ��� %d [%d ��]", numpkt, Length,
    // ByteOffset.QuadPart, checksum((unsigned short*)Buffer, Length), hits, tms);
    //
    // sprintf(str, "<��������>", 0, 1);
    // Form1->current->Text = str;
    // tms = GetTickCount()-tms;
    //
    // delayperc += tms;
    // if (tms < 50)
    // gt++;
    // if (tms > 50)
    // bt++;
    //
    // dp = (gt>bt?bt/gt:gt/bt)*100.0;
    // // if (dp >= 15.0 &&  Form1->debcheck->Checked)
    // // {
    // // tmsleft=15;
    //
    // sprintf(delaystr, "�������� %.2f %% (gt %.2f bt %.2f)", dp, gt, bt);
    //
    // if (dp>50.0)
    // Form1->delays->Color = clWhite;
    // else
    // Form1->delays->Color = clLtGray;
    // paintdisk(4, ByteOffset, 1, 0);
    //
    // if (hits == -1)
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // *BytesRead = Length; // ��� �����������!
    // return IO_OP_STATUS_SUCCESS;
    // }
    // // sprintf(str, "#%3d READ  %5lu @ %-8I64d CRC=0x%04X", numpkt, Length, ByteOffset.QuadPart,
    // // checksum((unsigned short*)Buffer, Length));
    // // OutputDebugString(str);
    // // Form1->current->Text = str;
    // // deb(str);
    // // deb("paintdisk enter");
    // // Application->ProcessMessages();
    // paintdisk(1, ByteOffset, Length, PagingIo);
    //
    // // deb("paintdisk leave");
    // // Application->ProcessMessages();
    //
    // // LONGLONG p = (LONGLONG)memory + (LONGLONG)ByteOffset.QuadPart;
    // // memcpy(Buffer, (LPVOID)p, Length);
    //
    // // deb("nread enter");
    // long len = nread((unsigned long)Buffer, (unsigned long)ByteOffset.QuadPart, Length, PagingIo);
    // // deb("nread=%d", len);
    // if (len != Length)
    // {
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // }
    //
    // // deb(" -- crc %x", checksum((unsigned short*)Buffer, Length));
    // sprintf(str, "<��������>", 0, 1);
    // Form1->current->Text = str;
    // // Application->ProcessMessages();
    //
    // paintdisk(4, ByteOffset, 1, 0);
    // *BytesRead = Length; // ��� �����������!
    // tms = GetTickCount()-tms;
    //
    // delayperc += tms;
    // if (tms < 50)
    // gt++;
    // if (tms > 50)
    // bt++;
    //
    // dp = (gt>bt?bt/gt:gt/bt)*100.0;
    // // if (dp >= 15.0 &&  Form1->debcheck->Checked)
    // // {
    // // tmsleft=15;
    //
    // sprintf(delaystr, "�������� %.2f %% (gt %.2f bt %.2f)", dp, gt, bt);
    //
    // if (dp>50.0)
    // Form1->delays->Color = clWhite;
    // else
    // Form1->delays->Color = clLtGray;
    // int hits = 0;
    // int upds = cache_write(ByteOffset.QuadPart, Buffer, Length, &hits, false);
    //
    // char strc[128];
    // if (!upds)
    // sprintf(strc, "������������", upds);
    // else
    // sprintf(strc, "%d ���.", hits);
    // tms = GetTickCount()-tms;
    // deb("#%3d READ  %5lu @ %-8I64d CRC=0x%04X %s [%4d ��]", numpkt++, Length, ByteOffset.QuadPart,
    // checksum((unsigned short*)Buffer, Length), strc, tms);
    //
    // return IO_OP_STATUS_SUCCESS;
}

DWORD __stdcall OnWrite(IN HANDLE hDisk, IN PVOID Context, IN PVOID Buffer, IN ULONG Length,
    IN LARGE_INTEGER ByteOffset, IN BOOL PagingIo, OUT PULONG BytesWritten)
{
    DWORD tms = GetTickCount();
    char str[128];
    char qqq[222];
    if (unmounting)
    {

        return IO_OP_STATUS_UNSUCCESSFUL;
    }
    ecs->Enter();
    try
    {

        sprintf(str, "WRITE %-5u @ %-8I64d", Length, ByteOffset.QuadPart);
        // if (Form1->debcheck->Checked)
        OutputDebugString(str);
        char trys[111];

        if (ByteOffset.QuadPart > curdisksize || (ByteOffset.QuadPart+Length) > curdisksize)
        {
            unsigned long newdisksize;
            newdisksize = (ByteOffset.QuadPart+Length)*2;
            // deb("need to resize disk to %u MB", newdisksize/1024/1024);

            memory = HeapReAlloc(GetProcessHeap(), 0, memory, newdisksize);
            if (!memory)
            {
                deb("failed to realloc: %s", fmterr());
                ecs->Leave();
                return IO_OP_STATUS_UNSUCCESSFUL;
            }
            deb("Reallocated mem to %u @ %p", newdisksize, memory);
            curdisksize = newdisksize;
        }

        if (ByteOffset.QuadPart!=wlastoffset)
            wntrys = 0;
        else
            wntrys++;
        wlastoffset = ByteOffset.QuadPart;
        // trys[0] = 0;
        // for (unsigned long i = 0;i<((floor(ntrys/20)*10));i++)
        // {
        // strncat(trys, ".", 50);
        // }

        sprintf(qqq, "w#unknown sector:%3lu repeat:%lu",
            (unsigned long)ByteOffset.QuadPart/(LONGLONG)fat32boot.bytes_per_sector, wntrys);
        if (Form1->unkdeb->Checked)
            Form1->imgptr->Caption = qqq;
        str[0] = qqq[0];
        str[1] = 0;
        Form1->headptr->Caption = str;

        paintdisk(0, ByteOffset, Length, PagingIo, 0);

        // if (!memory)
        // {
        // OutputDebugString("fail! no memory");
        //
        // return IO_OP_STATUS_UNSUCCESSFUL;
        // }
        //
        if (ByteOffset.QuadPart >= 4096 && ByteOffset.QuadPart <= pfat1)
        {
            if (Form1->debcheck->Checked)
                deb("   WRITE of reserved sector len %d ofs %d", (int)Length,
                (int)ByteOffset.QuadPart);
            printf(qqq, "w#RESERVED sector %lu",
                (unsigned long)ByteOffset.QuadPart/fat32boot.bytes_per_sector);
            Form1->imgptr->Caption = qqq;
        }
        if (ByteOffset.QuadPart >= rootdirectory && ByteOffset.QuadPart < rootdirectory+ files.size
            () *2*sizeof(DIR))
        {
            if (Form1->debcheck->Checked)
                deb("   WRITE of root directory len %d ofs %d", (int)Length,
                (int)ByteOffset.QuadPart);
            char ccc[111];
            sprintf(ccc, "w#DIR files:%3lu freespace:%.2f MB", files.size(),
                (double)((double)(disksize-totalmb)/1024/1024));
            // (unsigned long)ByteOffset.QuadPart/fat32boot.bytes_per_sector);
            Form1->imgptr->Caption = ccc;
        }

        if ((ByteOffset.QuadPart >= pfat1 && ByteOffset.QuadPart <= rootdirectory))
        {

            if (Form1->fatdeb->Checked)
                deb("   WRITE of FAT len %d ofs %d crc %x", (int)Length, (int)ByteOffset.QuadPart,
                checksum((unsigned short*)((long)memory+ByteOffset.QuadPart), Length));
            char ccc[111];
            sprintf(ccc, "w#FAT clstr:%3lu sect: %lu",
                (unsigned long)(ByteOffset.QuadPart/fat32boot.bytes_per_sector)/clstsize,
                (unsigned long)ByteOffset.QuadPart/fat32boot.bytes_per_sector);
            if (Form1->fatdeb->Checked)
                Form1->imgptr->Caption = ccc;
        }

        for (vector<FATFILE>::iterator it = files.begin();it!=files.end();it++)
        {
            if (ByteOffset.QuadPart >= (*it).start_offset && ByteOffset.QuadPart < (*it).end_offset)
            {
                unsigned int ptr;
                ptr = (*it).size - ((*it).end_offset - ByteOffset.QuadPart);
                ptr /= clstsize;
                (*it).accesses++;
                unsigned long stofs, enofs;
                stofs = (unsigned long)ByteOffset.QuadPart - (*it).start_offset;
                enofs = (unsigned long)ByteOffset.QuadPart - (*it).start_offset +Length;
                if (Form1->netdebug->Checked)
                    deb("WRITE %5d cluster %4u of file %-15s : %8d - %-8d", (int)Length, ptr,
                    (*it).fn, stofs, enofs);
                char fs[129];
                sprintf(fs, "w#%s:%3d", (*it).fn, (ByteOffset.QuadPart/fat32boot.bytes_per_sector)
                    /clstsize);
                Form1->imgptr->Caption = fs;
                // memset((LPVOID)((unsigned long)memory+ByteOffset.QuadPart), 0, Length);
                memcpy((LPVOID)((unsigned long)memory+ByteOffset.QuadPart), Buffer, Length);
                *BytesWritten = Length;
                ecs->Leave();
                return IO_OP_STATUS_SUCCESS;
            }
        }
    }
    catch(...)
    {
        deb("exception while OnWrite");
    }
    ecs->Leave();
    memcpy((LPVOID)((unsigned long)memory+ByteOffset.QuadPart), Buffer, Length);
    *BytesWritten = (unsigned long)Length;
    Application->ProcessMessages();
    // deb("done Write OK of %u bytes ", Length);
    return IO_OP_STATUS_SUCCESS;

    totwrites++;

    if (ramdisk)
    {
        //
        // Application->ProcessMessages();

        memcpy((char*)memory+ByteOffset.QuadPart, Buffer, Length);
        // OutputDebugString("done memcpy");
        tms = GetTickCount()-tms;
        delayperc += tms;

        if (tms < 50)
            gt++;
        if (tms >= 50)
            bt++;

        // dp = (gt>bt?bt/gt:gt/bt)*100.0;
        // // if (dp >= 15.0)
        //
        // if (dp>50.0)
        // Form1->delays->Color = clWhite;
        //
        // else
        // Form1->delays->Color = clLtGray;
        // tmsleft=15;
        char str[234];
        // sprintf(str, "�������� %.2f %% (gt %.2f bt %.2f)", dp, gt, bt);
        // Form1->delays->Caption = str;
        // Form1->debcheck->Checked = false;
        // Form1->debcheck->Checked = false;

        // else
        // if (!tms)
        // Form1->debcheck->Checked = true;

        sprintf(str, "#%3d WRITE %5u @ %-8I64d CRC=0x%04X [%4d ��]", numpkt++, Length,
            ByteOffset.QuadPart, checksum((unsigned short*)Buffer, Length), tms);
        OutputDebugString(str);
        if (Form1->debcheck->Checked)
            deb(str);
        paintdisk(4, ByteOffset, 1, 0);
        *BytesWritten = Length;
        return IO_OP_STATUS_SUCCESS;
    }
    else
    {
        deb("non ramdisk!");
    }

    Form1->current->Text = str;
    // deb(str);
    Application->ProcessMessages();

    if (Form1->Button1->Enabled)
    {
        deb("write unsuccess");
        return IO_OP_STATUS_UNSUCCESSFUL;
    }

    //
    // cev->SetEvent();

    deb("#%3d WRITE %5u @ %-8I64d CRC=0x%04X", numpkt++, Length, ByteOffset.QuadPart,
        checksum((unsigned short*)Buffer, Length));

    Length = cache_write(ByteOffset.QuadPart, Buffer, Length, NULL, true);
    *BytesWritten = Length;
    paintdisk(4, ByteOffset, 1, 0);
    return IO_OP_STATUS_SUCCESS;

    // paintdisk(0, ByteOffset, Length, PagingIo);
    // // LONGLONG p = (LONGLONG)memory + (LONGLONG)ByteOffset.QuadPart;
    // // memcpy((LPVOID)p, Buffer, Length);
    //
    // int len = nwrite((unsigned long)Buffer, (unsigned long)ByteOffset.LowPart, Length, PagingIo);
    // if (len == -1)
    // {
    // return IO_OP_STATUS_UNSUCCESSFUL;
    // }
    // *BytesWritten = Length; // ��� �����������!
    //
    // sprintf(str, "<idle>", 0, 1);
    // Form1->current->Text = str;
    // paintdisk(4, ByteOffset, 1, 0);
    // return IO_OP_STATUS_SUCCESS;
}

VOID __stdcall OnEvent(IN HANDLE hDisk, IN LONG DiskEvent, IN PVOID Context)
{
    shutdown(ConnectSocket, SD_BOTH);
    closesocket(ConnectSocket);

    ConnectSocket = 0;

    VdskUnmountDisk(hdisk, 'y');
    VdskDeleteDisk(hdisk);

    MessageBox(Application->MainFormHandle, "����������� ������ �����. ����������.", "������",
        MB_OK);

    // VdskUnmountDisk(hdisk,'y');
    // VdskMountDisk(hdisk,'y',true);
    deb(" !!!!!!!!!!!!! ����������� ������ �����: %d, ���������� ...", DiskEvent);

    // Application->ProcessMessages();
    LARGE_INTEGER size;
    size.QuadPart = disksize;
    hdisk = VdskCreateVirtualDisk((LARGE_INTEGER)size, 2, 32, 512,
        VDSK_DISK_REMOVABLE|VDSK_DISK_MEDIA_IN_DEVICE, 11111, OnRead, OnWrite, NULL, NULL,
        OnEvent, NULL);
    if (hdisk == INVALID_HANDLE_VALUE)
    {
        deb("VdskCreateVirtualDisk: %s", fmterr());
        deb(" -- STOP -- Critical error.");
        return;
    }

    VdskMountDisk(hdisk, 'y', true);
    if (!VdskStartDisk(hdisk, true))
        MessageBox(NULL, DiskEvent?"VDSK_DISK_EVENT_DEADLOCK":"VDSK_DISK_EVENT_NONE", "EVENT",
        MB_OK);
    // else
    deb("      -- ������");
}

BOOL __stdcall OnFormat(IN HANDLE hDisk, IN DWORD Progress, IN PVOID Context)
{
    // deb("OnFormat %x: %d", hDisk, Progress);
    // return IO_OP_STATUS_SUCCESS;

    // deb("progress %d",Progress);
    // if ( ((int)xx % 2) != 0)
    if (Progress > 0)
    {
        // formatp = p;
        char str[333];
        if (Progress >= 100)
            sprintf(str, "");
        else
            sprintf(str, "������������� %d %%", Progress);

        // deb(str);
        Form1->Canvas->Font->Size = 10;
        Form1->Canvas->Font->Color = clCream;
        Form1->Canvas->Brush->Style = bsClear;
        Form1->Canvas->Brush->Color = clSilver;
        Form1->Canvas->TextOutA(Form1->ptr->Left, Form1->ptr->Top-(Form1->Canvas->TextHeight(str)),
            str);
        Form1->current->Text = str;
        // Form1->OnPaint(0);

        Application->ProcessMessages();
    }

    return IO_OP_STATUS_SUCCESS;
}

void __fastcall TForm1::onidle(TObject *Sender, bool &Done)
{
    OnPaint(NULL);
}

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent *Owner):TForm(Owner)
{                                                  ftplog->Perform(WM_SETREDRAW, 0, 0);
    deblog->Perform(WM_SETREDRAW, 0, 0);
    ecs = new TCriticalSection();
    diskhint->Font->Handle = getfont("lucida console", 6, 7, 5);
    Label13->Font->Handle = getfont("lucida console", 6, 7, 5);
    pg1->ActivePageIndex = 4;
    deblog->Font->Handle = getfont("lucida console", 6, 7, 5);
    Application->OnIdle = onidle;
    // if (VdskStopDriver())
    // deb("stopped driver");
    LARGE_INTEGER size;
    ccs = new TCriticalSection();
    // deb("ccs @ 0x%08p", ccs);
    unmounting = false;
    memset(&size, 0, sizeof(size));
    size.QuadPart = (LONGLONG)32*1024*1024*1024;
    disksize = (LONGLONG)size.QuadPart;
    ftpcs = new TCriticalSection();
    memset(pids, 0, sizeof(pids));
    char str[233];
    // disksize = atoi(deunicode(Edit1->Text.c_str(), str, sizeof(str)))*1024*1024;

    // deb("short=%d int=%d UINT=%d long=%d float=%d double=%d longlong=%d", sizeof(short),
    // sizeof(int), sizeof(UINT), sizeof(long), sizeof(float), sizeof(double), sizeof(LONGLONG));
    kill_processes("ftpmount");
    ramdisk = ramonly->Checked;
    // Initialize Winsock
    // deb("sizeof(PKT) = %d", sizeof(PKT));

    cchs = new TCriticalSection();
    cev = new TEvent(false);
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
        deb("Client: Error at WSAStartup().\n");

    // VdskStartDriver();

    // DWORD dwId;
    // CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)nconn, NULL, NULL, &dwId);
    if (!ccs->TryEnter())
    {
        deb("strange err, cant enter ccs");
    }
    else
    {
        ccs->Leave();
    }
    cache.clear();
    files.clear();
    // maxquad = disksize/2.0;
    st = new statthread(false);
    st->Priority = tpHighest;

    deblog->Column[0]->Width = deblog->Width-28;
    ftplog->Column[0]->Width = ftplog->Width-28;
    Button1->Enabled = true;
    // ct = new CacheThread(false);

    if (diskcolor)
        for (DWORD i = 0;i<Form1->pb1->Width-1;i++)
            diskcolor[i] = clWhite;
    sizee.QuadPart = disksize;
    // memcpy(&fat32boot, fb, sizeof(FAT32BOOT));
    maxquad = 1*1024*1024;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{

    try
    {
        // Button2Click(NULL);
        kill_processes("ftpmount");
        // VdskUnmountDisk(hdisk, 'y');
        // VdskDeleteDisk(hdisk);
        VdskFreeLibrary();

        // ccs->Enter();

        cache.clear();

        if (st)
            st->Terminate();
        if (ct)
            ct->Terminate();

        // delete[](LPVOID)memory;
        HeapFree(GetProcessHeap(), 0, memory);
    }
    catch(...)
    {
        deb("exception while FormClose");
    }

    deb("disk freed");

    try
    {

        // ExitProcess(0);
        // Application->Terminate();
    }
    catch(...)
    {
        deb("exit catch error!");
    }

}

void prefetch(void)
{
    char temp[2048];
    DWORD dwnull;
    long offset = 0;
    precache = true;
    deb("starting disk prefetch ...");
    for (int i = 0;i<10;i++)
    {
        LARGE_INTEGER li;
        li.QuadPart = i*2048;
        deb("precaching fat %I64d...", li.QuadPart);
        OnRead(NULL, NULL, temp, (unsigned long)2048, li, false, &dwnull);
        Application->ProcessMessages();
    }
    deb("done precache");
    precache = false;
}

DWORD WINAPI makefatdisk(LPVOID p)
{
    MakeFAT *mf = new MakeFAT(false);
}
#include "CreateDisk.h"
CreateDisk *cr;

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    srand(time(NULL));
    VdskStartDriver();
    // deb("������� �������");
    if (!VdskInitializeLibrary())
    {
        deb("VdskInitializeLibrary: %s", fmterr());
        Application->ProcessMessages();
        Button8->Enabled = false;
        return;
    }
    DWORD ver = VdskGetDriverVersion();
    deb("vdskapi.dll ver %u", ver);
    // try
    // {
    Form1->pb1->Canvas->Rectangle(0, 0, Form1->pb1->Width, Form1->pb1->Height);
    ramonly->Enabled = false;

    char sztemp[111];

    if (hdisk)
        VdskDeleteDisk(hdisk);
    // Application->ProcessMessages();
    // disksize = (double)atoi(deunicode(Edit1->Text.c_str(), sztemp, sizeof(sztemp)))*1024*1024;

    // maxquad = Form1->pb1->Width;
    Button3->Enabled = true;
    Button2->Enabled = true;

    Button5->Enabled = true;
    Button4->Enabled = true;

    // Form1->makefatdisk(NULL);
    Button1->Enabled = false;
    cr = new CreateDisk(false);

    return;

    // ConnectThread *ct;
    // ct = new ConnectThread(false);
    //
    // ct->WaitFor();
    //
    // memcpy(memory, fb, sizeof(FB));
    //
    // if (ConnectSocket)
    // cr = new CreateDisk(false);
    // }
    // catch(...)
    // {
    // deb("except inside button1click");
    // }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    gt = 0;
    bt = 0;
    dp = 0;
    unmounting = true;
    shutdown(ConnectSocket, 2);
    closesocket(ConnectSocket);
    shutdown(FtpSocket, 2);
    closesocket(FtpSocket);
    ramonly->Enabled = true;
    LARGE_INTEGER ds;
    ds.QuadPart = disksize;
    // VdskSetDisk(hdisk,  ds,2,32,512,true,true,false);
    // deb("��������� ���� ...");
    // VdskEjectMedia(hdisk);
    if (ft)
        ft->Terminate();
    deb("unmount ...");

    VdskUnmountDisk(hdisk, 'y');
    deb("delete ...");
    VdskDeleteDisk(hdisk);
    hdisk = 0;

    showformathint = 0;
    numpkt = 0;
    Form1->pb1->Canvas->Rectangle(0, 0, Form1->pb1->Width, Form1->pb1->Height);

    // MessageBox(NULL, "unmount done", NULL, MB_OK);
    Button2->Enabled = false;
    Button3->Enabled = false;
    Button1->Enabled = true;
    ccs->Enter();
    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        HeapFree(GetProcessHeap(), 0, (*it).buf);
    }
    ccs->Leave();
    cache.clear();
    totcachsize = 0;

    try
    {
        // delete[]memory;
        // memory = NULL;

    }
    catch(...)
    {
        deb("except while delete[] memory");
    }
    Button4->Enabled = false;
    Button5->Enabled = false;
    Button6->Enabled = false;
    diskptr->Visible = false;
    deb("���� ��������.");
    // OnPaint(0);
    ptr->Text = "<�� ���������>";
    Form1->OnPaint(NULL);
    Application->ProcessMessages();
    // OnPaint(0);

    hdisk = NULL;
}
// ---------------------------------------------------------------------------
char savefile[128];

void __fastcall TForm1::Button4Click(TObject *Sender)
{
    strcpy(savefile, "iso333.dmp");
    Button4->Enabled = false;
    dumpthread *d = new dumpthread(false);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
    char str[233];
    sprintf(str, "FORMAT %3.2f %%", formatp/3.0);
    Form1->headptr->Left = formatp;
    Form1->current->Text = str;
    // Application->ProcessMessages();
    showformathint = false;
    // if (diskcolor)
    // for (DWORD i = 0;i<Form1->pb1->Width-1;i++)
    // diskcolor[i] = clWhite;
    deb("����������� ���� ...\r\n \r\n");
    // Application->ProcessMessages();
    // if (!ConnectSocket && !ramdisk)
    // nconn();

    pkt.cmd = 2;
    pkt.offset = 0;
    pkt.size = 0;
    pkt.sign = 'A';
    pkt.paged = 0;
    pkt.preverr = 0;
    pkt.crc = 0;

    // int ss = send(ConnectSocket, (char*) &pkt, sizeof(pkt), 0);

    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        HeapFree(GetProcessHeap(), 0, (*it).buf);
    }
    cache.clear();
    totcachsize = 0;

    formatthread *f = new formatthread(false);
    Button3->Enabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
    if (ramdisk)
    {
        deb("   �RC RAM ����� = 0x%04X memory @ %p", checksum((unsigned short*)memory,
                curdisksize), memory);

        // memcpy(&f, memory, sizeof(f));
        deb("   CRC fat32boot record = 0x%04X", checksum((unsigned short*)memory,
                sizeof(fat32boot)));
        unsigned long pfat;
        pfat = (DWORD)(fat32boot.reserved_sectors * fat32boot.bytes_per_sector) +
            (fat32boot.fat_copys*fat32boot.sectors_per_fat*fat32boot.bytes_per_sector);

        deb("   CRC Root Dir @ offset %lu %p = 0x%04X", pfat, (unsigned long)memory+pfat,
            checksum((unsigned short*)memory+pfat, files.size()*2*sizeof(DIR)));
        unsigned long pfat1, pfat2;
        pfat1 = (fat32boot.reserved_sectors * fat32boot.bytes_per_sector)+sizeof(FSINFO)+sizeof(FB);
        pfat2 = (fat32boot.reserved_sectors * fat32boot.bytes_per_sector) +
            ((fat32boot.sectors_per_fat*fat32boot.bytes_per_sector))+sizeof(FSINFO)+sizeof(FB);
        pfat1-=1024;
        pfat2-=1024;
        unsigned long fatsize = (fat32boot.sectors_per_fat) *fat32boot.bytes_per_sector;
        deb("       fat1 %d fat2 %d fatsize %d (%d mb)", pfat1, pfat2, fatsize, fatsize/1024/1024);

        deb("   CRC Fat 1 @ %p = 0x%04X", ((unsigned)memory+pfat1),
            checksum((unsigned short*)memory+pfat1, fatsize));

        deb("   CRC Fat 2 @ %p = 0x%04X", ((unsigned)memory+pfat2),
            checksum((unsigned short*)memory+pfat2, fatsize));

        return;
    }
    VdskSetDisk(hdisk, sizee, 2, 32, 512, false, true, true);
    memset(&pkt, 0, sizeof(pkt));
    pkt.cmd = 5;
    pkt.sign = 'A';
    deb(" * ����������� �RC �� ������� ...");
    Application->ProcessMessages();
    DWORD ms = GetTickCount();
    cchs->Enter();
    send(ConnectSocket, (const char*) &pkt, sizeof(pkt), 0);
    unsigned short crc;
    Sleep(400);
    int ret = recv(ConnectSocket, (char*)&crc, sizeof(crc), 0);
    cchs->Leave();
    ms = GetTickCount()-ms;
    deb("   �RC ����� = 0x%04X, %d ����\r\n \r\n", crc, ms);
    char xyu[128];
    sprintf(xyu, "CRC=0x%04X", crc);
    Button5->Caption = xyu;
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button6Click(TObject *Sender)
{
    VDSK_DISK_INFORMATION di;
    VdskQueryDisk(hdisk, &di);
    deb("mediaindevice: %d vollinkcreated: %d \r\ndisknumber: %d devicetype: %d\r\ndisksize: %I64d (%d MB)",
        di.IsMediaInDevice, di.IsVolumeLinkCreated, di.DiskNumber, di.DeviceType,
        di.DiskSize.QuadPart, di.DiskSize.QuadPart/1024/1024);
    deb("readb: %I64d writeb: %I64d \r\nreadfail: %I64d writefail: %I64d", di.IoReadCount.QuadPart,
        di.IoWriteCount.QuadPart, di.IoReadFailed.QuadPart, di.IoWriteFailed.QuadPart);
    deb("cluster size: %d", clstsize);

    memset(&pkt, 0, sizeof(pkt));
    pkt.cmd = 8;
    pkt.sign = 'A';
    deb("��������� ���������� ...");
    Application->ProcessMessages();
    cchs->Enter();
    send(ConnectSocket, (const char*) &pkt, sizeof(pkt), 0);
    char temp0[16384];
    int ret = recv(ConnectSocket, (char*)temp0, sizeof(temp0), 0);
    cchs->Leave();
    deb("recvd %d: \r\n%s", ret, temp0);
    dump(temp0, ret, "stats");
    // MessageBox(Application->MainFormHandle, temp0, "statistics", MB_OK);

}

DWORD WINAPI openDisk(LPVOID p)
{
    deb("��������� ���� ...");
    Application->ProcessMessages();
    ShellExecute(NULL, "explore", "Y:\\", NULL, NULL, SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::pb1Click(TObject *Sender)
{
    static int runum = 0;

    // if (!runum++)
    // Form1->debcheck2->Checked = false;
    DWORD dwId;
    CreateThread(NULL, 0, openDisk, NULL, 0, &dwId);

    // VDSK_DISK_INFORMATION di;
    // memset(&di, 0, sizeof(di));
    // VdskQueryDisk(hdisk, &di);
    // deb("readonly: %d removable: %d iscd: %d isflop: %d\r\n"
    // "ismediaindev: %d deletepend: %d isoffl: %d isvollinkcr: %d", di.ReadOnly, di.Removable,
    // di.IsCDRom, di.IsFloppy, di.IsMediaInDevice, di.DeletePending, di.IsOffline,
    // di.IsVolumeLinkCreated);
    // deb("links: %u diskn: %u devtype: %u disksize: %I64d GB tracks: %u\r\n"
    // "sectors: %u bytepersec: %u lastnotify: %u", di.LinksCount, di.DiskNumber,
    // di.DiskSize.QuadPart/1024/1024/1024, di.TracksPerCylinder, di.SectorsPerTrack,
    // di.BytesPerSector, di.LastDiskNotificationEvent);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::pb1MouseEnter(TObject *Sender)
{
    Cursor = crHelp;

    unsigned long dof;
    dof = dataoffset+(clstsize*2);
    char sss[12212];
    sprintf(sss, "FAT 1 @ %lu\r\nFAT 2 @ %lu\r\nRoot @ %lu\r\nData @ %lu", fat1, fat2,
        rootdirectory, dof);
    pb1->Hint = sss;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::pb1MouseLeave(TObject *Sender)
{
    Cursor = crDefault;
    deblog->Repaint();
}

bool sortcachebyoffset(CACHE c1, CACHE c2)
{
    return c1.offset<c2.offset;
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::cacheblocksDblClick(TObject *Sender)
{
    FILE *f;
    unlink("cache-dir.txt");
    f = fopen("cache-dir.txt", "w");

    int num = 0;
    ccs->Enter();
    fprintf(f, "   * ��� - %d �������, %u ���� (%u ��):\r\n", cache.size(), totcachsize,
        totcachsize/1024/1024);

    fprintf(f,
        "  �   ������  ��������     �����. ���.   �����   ����\r\n-----------------------------------------------------------\r\n");
    sort(cache.begin(), cache.end(), sortcachebyoffset);
    for (vector<CACHE>::iterator it = cache.begin();it!=cache.end();it++)
    {
        char str[1024];
        sprintf(str, "#%3d %6u   %8u    %4d    %3d    %3d    %3d\r\n", num++, (*it).len,
            (*it).offset, (*it).updates, (*it).hits, (GetTickCount()-(*it).changed)/1000,
            (*it).opid);

        fprintf(f, str);
    }
    fclose(f);
    ccs->Leave();
    // deb("  * ���: ������ ��������.");
    ShellExecute(NULL, "open", "notepad.exe", " cache-dir.txt", NULL, SW_SHOWNORMAL);
}
// ---------------------------------------------------------------------------

void cdump(char *b, int size, char *tag = NULL)
{
    FILE *x;
    char fn[128];
    sprintf(fn, "dump-%s.c", tag?tag:"unnamed");
    x = fopen(fn, "w");
    fprintf(x, "// %d bytes\r", size);
    int nl = 0;
    for (int i = 0;i<size;i++)
    {
        if (nl++>6)
        {
            fprintf(x, "\r");
            nl = 0;
        }

        char str[128];
        sprintf(str, "0x%02x, ", (unsigned char)b[i]);
        fwrite(str, 1, strlen(str), x);
    }
    fclose(x);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::deblogDblClick(TObject *Sender)
{
    deblog->Clear();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormPaint(TObject *Sender)
{
    static DWORD tms = 0;
    char ss[128];
    try
    {

        if ((GetTickCount()-tms)>20)
        {
            Canvas->Brush->Style = bsClear;
            Canvas->Rectangle(TRect(Form1->pb1->Left, Form1->pb1->Top, Form1->pb1->Width,
                    Form1->pb1->Top+Form1->pb1->Height));
            if (diskcolor)
                for (int i = 1;i<Form1->pb1->Width-1;i++)
                    for (int u = 1;u<Form1->pb1->Height-1;u++)
                        Form1->pb1->Canvas->Pixels[(int)i][u] = (TColor)diskcolor[(int)i];
            sprintf(ss, "%d ���� � %.1f M� �� %.1f ��", files.size(),
                (double)((double)curdisksize/1024.0/1024.0),
                (double)((double)disksize/1024.0/1024.0/1024.0));
            //
            Label13->Caption = ss;
            sprintf(ss, "������������ %.2f MB �� %.2f MB", (double)maxquad/1024/1024,
                (double)disksize/1024/1024);
            // Canvas->Font->Color=clGray;
            maxquadl->Caption = ss;

            // Canvas->Font->Size = 8;
            // Canvas->Font->Name = "tahoma";
            // // Canvas->Font->Style = Canvas->Font->Style << fsBold;
            // Canvas->Font->Color = clBlack; Canvas->Brush->Style = bsClear;
            // diskcap->Caption.sprintf(L"%-15s", ss);
            // diskcap->Invalidate();
            diskcap->Width = 0;
            // diskcap->Repaint();

            // Canvas->Brush->Color = RGB(0xd4, 0xd8, 0xc8);

            // Form1->Label13->Canvas->TextOutA(0, // Form1->pb1->Width-Canvas->TextWidth(ss),
            // 0, ss);
            tms = GetTickCount();
        }
    }
    catch(...)
    {
        deb("Excpt while paint()");
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::deblogMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
    int X, int Y)
{
    static bool xyu = false;
    if (Button == mbRight)
    {
        xyu = !xyu;
        deblog->Font->Name = xyu?"consolas":"tahoma";
    }
}

void __fastcall TForm1::ramonlyExit(TObject *Sender)
{
    ramdisk = !ramdisk;

    if (ramdisk)
        ramonly->Checked = true;
    else
        ramonly->Checked = false;
}

DWORD make_dword(WORD lo, WORD hi)
{
    return((DWORD)hi << 16) | lo;
}

void readfatdir(unsigned int clst, LPVOID dirptr, char *dn)
{
    DIR dir;
    int nf = 0;
    static int numit = 0;
    LPVOID memory;
    memory = dirptr;

    if (!dirptr)
        return;

    wchar_t wfn[44];
    unsigned int stcl = 0;

    deb("readfatdir(%u, %p, %s) crc:%x", clst, dirptr, dn, checksum((unsigned short*)dirptr, 4096));
    if (clst>2)
        clst-=2;
    // dirptr = (LPVOID)(unsigned long)((unsigned long)memory+(unsigned long)dataoffset+
    // ((unsigned long)clstsize*clst));

    memcpy(&dir, dirptr, sizeof(dir));
    numit++;
    char spcrs[1024];
    memset(spcrs, 0, 1024);
    for (int i = 0;i<numit*3;i++)
        spcrs[i] = 0x20;
    // deb("%sreading dir %s cluster %lu", spcrs, dn, clst);
    int nr = 0;
    char lfn[128];
    LFNDIR lfndir;
    for (int i = 0;dir.fname[0];i++)
    {
        char ff[13];
        char type[111];
        char strclusters[1024] = "";
        // stcl = ((dir.st_clust_h<<16) | (dir.st_clust_l&0xff));

        // stcl+=fat32boot.reserved_sectors-2+2*fat32boot.sectors_per_fat*2;
        unsigned long stcl = 0;
        if (ff[0] == 0xe5)
        {
            // memcpy(&dir, ((char*)memory)+rootdirectory+((i+1)*sizeof(dir)), sizeof(dir));
            dirptr = (LPVOID)((unsigned long)dirptr+(unsigned long)sizeof(dir));
            memcpy(&dir, dirptr, sizeof(dir));
            nr++;
            continue;
        }

        if (dir.attr == 0x10)
            strcpy(type, "D");
        else
            if (dir.attr == 0x08)
                strcpy(type, "V");
            else
                strcpy(type, "F");

        if (dir.attr == 0x0f) // long file name
        {

            // deb("sizeof dir=%d lfndir=%d",sizeof(dir),sizeof(lfndir));
            memcpy(&lfndir, &dir, sizeof(lfndir));

            if (lfndir.zero||lfndir.attr!=0x0f)
                deb("LFNDIR ZERO NOT ZERO lfndir.attr!=0x0f");

            memset(lfn, 0, 128);
            wchar_t wfn[1024];
            memset(wfn, 0, sizeof(wfn));

            memcpy(wfn, &lfndir.fn5, 10);
            memcpy(wfn+10, &lfndir.fn7, 4);
            memcpy(wfn+10+4, &lfndir.fn6, 12);

            // wfn[25] = 0x0;
            deunicode(wfn, lfn, sizeof(lfn));
            // sprintf(lfn, "%S", wfn);

        }
        else
            if (dir.attr)
            {

                snprintf(ff, 13, "%s%c%s", dir.fname, strlen(dir.ext)>1 ? '.':' ',
                    dir.ext[0] ? dir.ext:"");
                // strncpy(ff, dir.fname, 11);

                if (dir.attr!=0x20&&dir.attr!=0x0f&&dir.attr!=8&&dir.attr!=0x10)
                    deb(" attr=%x ", dir.attr);

                int clusters;

                if (dir.size > fat32boot.bytes_per_sector)
                    clusters = dir.size /
                        (fat32boot.sectors_per_cluster*fat32boot.bytes_per_sector);
                else
                    clusters = 1;
                clusters = dir.size / (fat32boot.bytes_per_sector* fat32boot.sectors_per_cluster);
                // ? dir.size/ (fat32boot.bytes_per_sector*fat32boot.sectors_per_cluster):1;
                // if(dir.attr
                stcl = make_dword(dir.st_clust_l, dir.st_clust_h);
                unsigned long ncl;
                ncl = stcl;
                unsigned long curcl;
                unsigned long offset;
                curcl = 0;
                LARGE_INTEGER off;
                offset = dataoffset + (clstsize * ncl) - 32768;

                if (dir.size)
                {

                    strclusters[0] = 0;

                    off.QuadPart = offset;
                    paintdisk(9, off, clGray, 0, 0);

                    try
                    {
                        while (curcl++<=5)
                        {

                            if (!fat[ncl] || fat[ncl] == 0xffffff0f || fat[ncl] == 0xffffffff ||fat
                                [ncl] == 0xfffffff0 || fat[ncl] == 0x0fffffff)
                                break;

                            char str2[128];

                            // (int)pData+offset);
                            sprintf(str2, " %u ", ncl);
                            strcat(strclusters, str2);
                            ncl = fat[ncl];

                        }
                    }
                    catch(...)
                    {
                        deb("exception while traversing fat");
                    }
                    ff[12] = 0;
                    if (!clusters)
                        strclusters[0] = 0;

                }
                nf++;
                unsigned long byte;
                byte = getclusterbyte(make_dword(dir.st_clust_l, dir.st_clust_h));
                // deb("%s%-1s #%3d %-12s (%03.2f MB, %5d sectors) %3d FAT Clusters: %4s", spcrs,
                // type, nf++, ff, (double)dir.size/1024/1024,
                // (unsigned)dir.size/fat32boot.bytes_per_sector, clusters, strclusters);
                deb("%s %s %10s %-8lu clst:%-8lu (byte %lu)", spcrs, type, ff,
                    (unsigned long)dir.size, make_dword(dir.st_clust_l, dir.st_clust_h), byte);
                deb("%s  attr=%x res=%x sth=%x stl=%x siz=%x (%u KB)", spcrs, dir.attr,
                    dir.reserved, dir.st_clust_h, dir.st_clust_l, dir.size, dir.size/1024);
                // if (byte >= (unsigned long)curdisksize)
                // {
                // dirptr = (LPVOID)((unsigned long)dirptr+(unsigned long)sizeof(dir));
                // deb("cluster points out of memory");
                // memcpy(&dir, dirptr, sizeof(dir));
                // nr++;
                // continue;
                // }
                if (lfn[0])
                {
                    // deb("         %-20s (seq %x entry_type: %x cksum: %x zero:%x)", lfn,
                    // lfndir.seq, lfndir.entry_type, lfndir.cksum, lfndir.zero);

                    lfn[0] = 0;
                }
                // deb("       cluster %lu @ %lu (0x%08X @ memory[0x%08X])", stcl, offset, offset,
                // offset+(unsigned long)pData);
                int c = 0;
                char *l;
                l = (char*)pData;

                // try
                // {
                // if (offset<curdisksize)
                // c = l[offset];
                // if (isalpha(c))
                // {
                // char tt[128];
                // snprintf(tt, 32, "%s", (unsigned long)l+offset);
                // deb("      %s", tt);
                // }
                // }
                // catch(...)
                // {
                // deb("offset exception byte %lu, offset %p", offset, (unsigned long)l+offset);
                // }

                if (dir.attr == 0x10)
                {
                    unsigned long dclst = make_dword(dir.st_clust_l, dir.st_clust_h);
                    if (dclst == clst)
                    {
                        deb("self-lock: directory to self [%s]", dn);
                        dirptr = (LPVOID)((unsigned long)dirptr+(unsigned long)sizeof(dir));
                        memcpy(&dir, dirptr, sizeof(dir));
                        nr++;
                        continue;
                    }
                    LPVOID mem = 0;
                    for (vector<FATFILE>::iterator it = files.begin();it!=files.end();it++)
                        if ((*it).firstcluster-3 == dclst)
                            mem = (*it).memory;
                    // else deb("%s %lu != %lu",(*it).fn, (*it).firstcluster,dclst);

                    readfatdir(dclst, mem, dir.fname);
                }

            }
            else
            {
                deb("volume record");
            }

        dirptr = (LPVOID)((unsigned long)dirptr+(unsigned long)sizeof(dir));
        memcpy(&dir, dirptr, sizeof(dir));
        nr++;

    }
    numit--;
    char s[128];
    sprintf(s, "%s %d", spcrs, nf);
    // dirptr = (LPVOID)(unsigned long)((unsigned long)memory+(unsigned long)dataoffset+
    // ((unsigned long)clstsize*clst));
    char ddn[128];
    snprintf(ddn, 13, "dir-%4s", dn);
    dump((char*)memory, sizeof(dir)*(nr+1), ddn);
    deb(s);

    Form1->diskhint->Caption = s;
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button9Click(TObject *Sender)
{
    FILE *x;
    LPVOID disk = memory;
    debcheck2->Checked = true;
    try
    {
        if (!memory || IsBadReadPtr(memory, curdisksize))
        {
            deb("NO MEMORY || Bad read ptr %x - %x", memory, (long)memory+(long)disksize);
            return;
        }

    }
    catch(...)
    {
        deb("exception while cpying memory to disk [read fat]");
    }

    deb("memory @ 0x%08p, disk: ������������ %lu MB �� %I64d (%I64d GB)", memory,
        (unsigned long)curdisksize/1024/1024, disksize, disksize/1024/1024/1024);
    ////////////////////////////////////////////////////////////////////////////////////

    PART part;
    FB fb;
    char *p;

    memcpy(&fb, memory, sizeof(fb));

    p = (char*)&fb.sign;
    // deb("sign(55AA): 0x%2x (%x %x)", fb.sign, (unsigned char)*(p), (unsigned char)*(p+1));
    if (fb.sign!=0xaa55)
    {
        deb("��������� FAT �����������, copy %d", 4096);
        return;

    }

    memcpy(&fat32boot, memory, sizeof(fat32boot));
    if (fat32boot.sign_0x29 != 0x29)
    {
        deb("   --- FAIL\r\n   no sign - sign0x29 = %02x\r\n", fat32boot.sign_0x29);
        return;
    }

    deb("oem: %s bytes_per_sector=%d sectors_per_cluster=%d", fat32boot.oem,
        fat32boot.bytes_per_sector, fat32boot.sectors_per_cluster);
    deb("reserved_sectors=%d (%d bytes) fat_copys=%d max_root_ent=%d", fat32boot.reserved_sectors,
        fat32boot.reserved_sectors*fat32boot.bytes_per_sector, fat32boot.fat_copys,
        fat32boot.max_root_ent);
    deb("media=%x sectors_per_track=%u heads=%u hidden_sectors=%u", fat32boot.media,
        fat32boot.sectors_per_track, fat32boot.heads, fat32boot.hidden_sectors);
    deb("sectors=%u \r\nsectors_per_fat=%lu\r\n"
        "%lu clusters, %lu sectors, size %.2f GB flags=%x version=%x", (unsigned)fat32boot.sectors,
        (unsigned long)fat32boot.sectors_per_fat, (unsigned long)fatsize/4,
        (unsigned long)((disksize / (LONGLONG)fat32boot.bytes_per_sector)),
        (double)((LONGLONG)((fatsize/4)*(LONGLONG)clstsize)/1024/1024/1024), fat32boot.flags,
        fat32boot.version);
    deb("start_root_cluster=%d file_info_sector=%d backup_boot_sector=%d",
        fat32boot.start_root_cluster, fat32boot.file_info_sector, fat32boot.backup_boot_sector);
    deb("logical_drive_num=%d sign_0x29=%x serial=%x", fat32boot.logical_drive_num,
        fat32boot.sign_0x29, fat32boot.serial_num);
    deb("volume_name=%s fat32_name=%s", fat32boot.volume_name, fat32boot.fat32_name);

    int ptr = 0;

    // deb("fat32boot @ %d", 0);
    ptr+=sizeof(FB);
    deb("fsinfo @ %d", ptr);
    ptr+=sizeof(FSINFO)+fat32boot.reserved_sectors*fat32boot.bytes_per_sector;
    fatsize = (fat32boot.sectors_per_fat)*fat32boot.bytes_per_sector;

    deb("������ FAT = %d (%d MB)", fatsize, fatsize/1024/1024);

    int fat_correct = 1024;
    fat1 = ptr-fat_correct;
    deb("fat1 @ %d", fat1);

    ptr+=fat32boot.sectors_per_fat*fat32boot.bytes_per_sector;
    deb("fat2 @ %d", ptr-fat_correct);
    fat2 = ptr;
    DIR dir;
    // FirstRootDirSecNum = BPB_ResvdSecCnt + (BPB_NumFATs * BPB_FATSz16);
    ptr = fat32boot.reserved_sectors * fat32boot.bytes_per_sector +
        (fat32boot.fat_copys*fat32boot.sectors_per_fat*fat32boot.bytes_per_sector);
    // deb("root dir @ %d", ptr);
    rootdirectory = ptr;
    fs = ptr;
    clstsize = fat32boot.sectors_per_cluster*fat32boot.bytes_per_sector;
    deb("clustersize: %lu", clstsize);
    deb("root @ %d (0x%x) in %.2f%% of disk", fs, fs, (float)(fs/disksize)*100.0);
    // deb("need for fs %u MB", ptr/1024/1024);
    // cdump((char*)disk+fs, fat32boot.sectors_per_fat*fat32boot.bytes_per_sector*fat32boot.fat_copys,
    // "fat1.2");
    LARGE_INTEGER off;
    off.QuadPart = fat1;
    paintdisk(9, off, clRed, 0, 0);
    off.QuadPart = fat2;
    paintdisk(9, off, clRed, 0, 0);
    off.QuadPart = rootdirectory;
    paintdisk(9, off, clGreen, 0, 0);

    fatsize = fat32boot.sectors_per_fat*fat32boot.bytes_per_sector;

    // dataoffset = (fatsize*2) + (fat32boot.reserved_sectors*fat32boot.bytes_per_sector) -
    // (16 * fat32boot.bytes_per_sector) +16384;
    // FirstDataSector = BPB_ResvdSecCnt + (BPB_NumFATs * FATSz)
    dataoffset = fat32boot.reserved_sectors*fat32boot.bytes_per_sector +
        (fat32boot.fat_copys*fat32boot.sectors_per_fat*fat32boot.bytes_per_sector);
    pData = (LPVOID)((unsigned int)memory + dataoffset);

    deb("data @ 0x%08p (byte %lu) memory: 0x%08p - 0x%08p (%lu MB) pData 0x%08p", dataoffset,
        dataoffset, memory, HeapSize(GetProcessHeap(), 0, memory)+(SIZE_T)memory,
        HeapSize(GetProcessHeap(), 0, memory)/1024/1024, pData);
    // if (maxquad < (dataoffset+1*1024*1024))
    // maxquad = dataoffset+(1*1024*1024);

    int nf = 0;
    LFNDIR lfndir;
    char lfn[128];
    wchar_t wfn[44];
    unsigned int stcl = 0;
    memset(lfn, 0, sizeof(lfn));
    memset(wfn, 0, sizeof(wfn));

    if (fat)
        HeapFree(GetProcessHeap(), 0, fat);

    fat = (unsigned int*)HeapAlloc(GetProcessHeap(), 0, fatsize);

    // memcpy((LPVOID)fat, (LPVOID)((DWORD)memory+fat1), fatsize);
    deb("load fat from %u", (fat1));
    memset(&lfndir, 0, sizeof(lfndir));

    if (sizeof(lfndir)!=sizeof(dir))
    {
        deb("lfndir != dir");
        return;
    }
    readfatdir(0, rootdirptr, "root");
    Form1->OnPaint(NULL);
    // Application->ProcessMessages();
    deb("done fat ");
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::makefatdisk(TObject *Sender)
{

    char temp2[16374], temp3[16374];
    char tempbuf[16374];
    maxquad = 0;
    LPVOID dirptr;
    files.clear();

    if (!disksize)
    {
        disksize = (LONGLONG)32*1024*1024*1024;
    }

    if (!curdisksize||!memory)
    {

        curdisksize = 18*1024*1024;

        if (memory)
            HeapFree(GetProcessHeap(), 0, memory);

        memory = HeapAlloc(GetProcessHeap(), 0, curdisksize);

        deb("disksize = %.2f GB (%lu), memsize = %lu (%.2f MB) memory @ %p",
            (double)((double)disksize/1024.0/1024.0/1024.0), (unsigned long)disksize,
            (unsigned long)curdisksize, (double)((double)curdisksize/1024.0/1024.0),
            memory);

    }

    char slev[128];
    deunicode(Form1->dirlevel->Text.c_str(), slev, sizeof(slev));
    dirlev = atoi(slev);
    deb("dirlevel %d", dirlev);

    if (!diskcolor)
        diskcolor = (unsigned int*)HeapAlloc(GetProcessHeap(), 0, Form1->pb1->Width*4);
    for (int i = 0;i<Form1->pb1->Width;i++)
        diskcolor[i] = clWhite;
    Form1->Repaint();
    // Application->ProcessMessages();
    // if (FtpSocket)
    // {
    memcpy(memory, fb, 512);
    // memset((LPVOID)((unsigned)memory +sizeof(FAT32BOOT)), (unsigned char)rand()%255,
    // 512-sizeof(FAT32BOOT)-2);
    memcpy(&fat32boot, memory, sizeof(fat32boot));
    fat32boot.bytes_per_sector = 512;
    fat32boot.sectors_per_cluster = 32;
    fat32boot.reserved_sectors = 38;
    fat32boot.sectors = 67108863;
    fat32boot.sectors_per_fat = 16377;
    fat32boot.fat_copys = 2;
    memcpy(memory, &fat32boot, sizeof(fat32boot));

    // }

    // PKT pkt2;
    //
    // memset(&pkt2, 0, sizeof(pkt2));
    // pkt2.sign = 'A';
    // pkt2.cmd = 6;

    // cchs->Enter();
    //
    // deb(" ��������� FAT ������ \r\n ");
    // send(ConnectSocket, (const char*)&pkt2, sizeof(pkt2), 0);
    //
    // temp2 = new char[4096];
    // temp3 = new char[4096];
    //
    // memset(temp2, 0, 4096);
    //
    // // deb(" ��������� �������(%d)... \r \n ", sizeof(pkt2));
    // // Application->ProcessMessages();
    // int rec = recv(ConnectSocket, (char*)&pkt2, sizeof(pkt2), 0);
    // if (rec<=0)
    // {
    // deb("recv: %d", rec);
    // deb("������ ����");
    // return;
    // }
    //
    // int len = recv(ConnectSocket, temp2, pkt2.size, 0);
    //
    // cchs->Leave();
    //
    // if (len<=0)
    // {
    // deb(" ������ ������� !");
    // delete[]temp2;
    // delete[]temp3;
    //
    // Form1->Button4->Enabled = true;
    // Form1->Button3->Enabled = true;
    // Form1->Button5->Enabled = true;
    // Form1->Button6->Enabled = true;
    // Form1->Button2->Enabled = true;
    // return;
    // }

    // FILE *t;
    // t = fopen("e:\\files.txt", "rt");
    // if (!t)
    // {
    // deb("no files.txt");
    // return;
    // }
    // fseek(t, 0, SEEK_END);
    // int size = ftell(t);
    // fseek(t, 0, SEEK_SET);
    // fread(temp2, 1, size, t);
    // fclose(t);

    char *nl;
    unsigned int nb = 0, i = 0;
    char xyu0[128];
    char *next_token1 = NULL, *next_token2 = NULL;

    nl = temp3;
    if (!strlen(fatfiles))
    {
        strcpy(fatfiles,
            "345354 temp1.txt\r\n823673 text.doc\r\n345345 file.c\r\n345345 project.zip");
    }
    else
    {
        strncpy(temp3, fatfiles, sizeof(temp3)); // trlen(fatfiles));
    }

    // while (nl[i++])
    // if (nl[i] == 0xa)
    // nb++;

    // deb(" FAT:��������� %d �������: \r\n%s", nb, temp3);
    DWORD pfat = 0; // 0xab020;
    fatsize = fat32boot.sectors_per_fat*fat32boot.bytes_per_sector;
    // deb("������ FAT = %lu (%lu MB � %d ��������)", fatsize, fatsize/1024/1024,
    // fat32boot.sectors_per_fat);
    // pfat = ((DWORD)fs);
    deb("bytes_per_sector=%d sectors_per_cluster=%d\r\n", fat32boot.bytes_per_sector,
        fat32boot.sectors_per_cluster);
    deb("reserved_sectors=%d (%d bytes) fat_copys=%d ", fat32boot.reserved_sectors,
        fat32boot.reserved_sectors*fat32boot.bytes_per_sector, fat32boot.fat_copys);
    deb("sectors_per_fat=%d (FAT: %d MB)", fat32boot.sectors_per_fat, fatsize/1024/1024);

    if (!fat)
        fat = new unsigned int[fatsize];

    if (fatsize > curdisksize)
    {
        unsigned long ns;
        ns = fatsize*4;

        memory = HeapReAlloc(GetProcessHeap(), 0, memory, ns);
        deb("���  %d �� @ %p", ns/1024/1024, memory);
        curdisksize = (LONGLONG)ns;
        maxquad = (double)curdisksize;
    }

    memset(fat, 0x00, fatsize);

    nf = 0;

    pfat = (unsigned long)(fat32boot.reserved_sectors * fat32boot.bytes_per_sector) +
        (fat32boot.fat_copys*fat32boot.sectors_per_fat*fat32boot.bytes_per_sector);
    // deb("fat end offset %lu", pfat);
    // deb("FAT ��������� �������� %lu ���� (%.2f MB)", (unsigned long)pfat,
    // (double)((double)pfat/1024.0/1024.0));

    rootdirectory = pfat;

    DIR dir;

    fspace_ptr = (DWORD)memory+ (DWORD)pfat;

    pfat1 = (fat32boot.reserved_sectors * fat32boot.bytes_per_sector)+sizeof(FSINFO)+sizeof(FB);
    pfat2 = (fat32boot.reserved_sectors * fat32boot.bytes_per_sector) +
        ((fat32boot.sectors_per_fat*fat32boot.bytes_per_sector))+sizeof(FSINFO)+sizeof(FB);
    pfat1-=1024;

    pfat2-=1024;

    LARGE_INTEGER off;
    off.QuadPart = pfat1;
    paintdisk(9, off, clRed, true, 0);
    off.QuadPart = pfat2;
    paintdisk(9, off, clRed, true, 0);

    clstsize = fat32boot.sectors_per_cluster*fat32boot.bytes_per_sector;
    totclusters = ((LONGLONG)disksize) / (LONGLONG)clstsize;
    deb("������� = %lu ����, ����� ���� = %lu (%.2f GB)\r\��������� = %lu", clstsize,
        (unsigned long)disksize, (double)((double)disksize/1024.0/1024.0/1024.0), totclusters);
    LPVOID pData;

    dataoffset = (fatsize*2) + (fat32boot.reserved_sectors*fat32boot.bytes_per_sector);
    off.QuadPart = dataoffset;
    paintdisk(9, off, clGreen, true, 0);
    pData = (LPVOID)((unsigned int)memory + dataoffset);
    deb("1 ������� @ %p (dataoffset %d, 0x%4X)", pData, dataoffset, dataoffset);

    dirptr = (LPVOID)((unsigned long)memory + (unsigned long)rootdirectory);
    deb("rootdirectory @ memory:0x%08p (offset %lu - 0x%08X) ... ", dirptr, pfat, pfat);

    deb("pfat2 size need %d MB", ((unsigned long)pfat2+fatsize)/1024/1024);
    // if (pfat2+fatsize>curdisksize)
    // {
    // unsigned long ns;
    // ns = pfat2+(2*1024*1024);
    // deb("������ ��� ��� 2 ������� %d ��", ns/1024/1024);
    // memory = HeapReAlloc(GetProcessHeap(), 0, memory, ns);
    // curdisksize = ns;
    //
    // }

    char filename[255];
    char fn[255];
    unsigned long allocatedbytes;
    unsigned long startclust;
    unsigned long startoffset;
    unsigned int offset;
    unsigned int clst;
    unsigned int ssize;
    unsigned int nclust;
    unsigned int entrcls;
    unsigned long cluster;
    unsigned long fsize = 0;
    int attr = 0;
    char fnm[128];
    char *p;
    unsigned long freeclusters;

    freeclusters = totclusters;
    totalmb = dataoffset;
    deb("total B = %.0f (%.1f MB )", totalmb, totalmb/1024/1024);
    maxquad = (double)curdisksize;
    deb("maxquad=%.2f MB, disksize=%.2f MB", (double)maxquad/1024.0/1024.0,
        (double)((double)disksize/1024.0/1024.0));

    DWORD tms2;

    // Form1->Repaint();

    netfatfilesize = 0;

    cluster = start_cluster;

    stc = start_cluster;
    fat[0] = 0x0ffffff8;
    fat[1] = 0xffffffff;
    fat[2] = 0x0fffffff;
    fat[3] = 0x0fffffff;
    fat[4] = 0x0fffffff;

    // for(unsigned long u=0;u<14000;u++)
    // if(!fat[u])
    // fat[u]=0x0fffffff;

    LPVOID ndirptr;
    ndirptr = (LPVOID)dirptr;
    deb("fat write rootdir begin @ %p (%lu kb)", dirptr,
        ((unsigned long)dirptr-(unsigned long)memory)/1024);
    char db[1024];
    strcpy(db, "");
    FATFILE f;
    memset(&f, 0, sizeof(f));
    f.memory = HeapAlloc(GetProcessHeap(), 0, 32768);
    f.memsize = 32768;
    f.start_offset = rootdirectory;
    f.end_offset = rootdirectory+32768;
    f.fetched = true;
    f.dir = true;
    strcpy(f.fn, "root");
    files.push_back(f);

    rootdirptr = (LPVOID)f.memory;
    int added = fataddfiles(db, f.memory, 0, temp3);

    deb("fat write rootdir done");

    dirptr = (LPVOID)((unsigned long)f.memory +(sizeof(LFNDIR)*(((added)/2)+sizeof(DIR))));

    memset(&dir, 0, sizeof(dir));
    dir.attr = 8;
    strcpy(dir.fname, "FtpDisk");
    deb("    write volume dir label '%s' @ %p", dir.fname, dirptr);
    memcpy((LPVOID)dirptr, &dir, sizeof(dir));

    if (!files.size())
    {
        deb("no netfat, ftpfiles: %s", fatfiles);
    }
    // deb("  ------------ totalMB %.0f (%.1f MB) [%.1f MB FAT32]", totalmb, totalmb/1024/1024,
    // (double)dataoffset/1024/1024);

    deb("crc fat32boot %d @ %p = %x", sizeof(fat32boot), memory,
        (USHORT)checksum((unsigned short*)memory, sizeof(fat32boot)));
    // deb("pfat1 %d pfat2 %d", pfat1, pfat2);
    deb("crc fat[]            %x (%d)", checksum((unsigned short*)fat, fatsize), fatsize);
    memcpy((LPVOID)((DWORD)memory+pfat1), fat, fatsize);

    deb("crc fat 1 @ %p %x (%d)", ((unsigned long)memory+pfat1),
        checksum((unsigned short*)((unsigned long)memory+pfat1), fatsize), fatsize);

    memcpy((LPVOID)((DWORD)memory+pfat2), fat, fatsize);
    deb("crc fat 2 @ %p %x (%d)", ((unsigned long)memory+pfat2),
        checksum((unsigned short*)((unsigned long)memory+pfat2), fatsize), fatsize);

    deb("NetFAT: %d, vol: 1, FAT: %d. rootdirsize: %lu ����", files.size(), added,
        (unsigned long)(added*2+1)*sizeof(DIR));
    // deb("�������� fat1 = > %d fat2 = > %d (%d bytes) memory @ %p", pfat1, pfat2,
    // fat32boot.sectors_per_fat*fat32boot.bytes_per_sector, memory);
    off.QuadPart = pfat;
    paintdisk(9, off, clLime, true, 0);
    // delete[]fat;
    deb("done FAT");
    try
    {
        // delete[]temp2;
        // delete[]temp3;
        // }
        Form1->Button4->Enabled = true;
        Form1->Button3->Enabled = true;
        Form1->Button5->Enabled = true;
        Form1->Button6->Enabled = true;
        Form1->Button2->Enabled = true;

        LARGE_INTEGER sizee;
        sizee.QuadPart = disksize;

        // maxquad = totalmb*1.7;

        Form1->current->Text = "FAT ���� ������";

        // Form1->Repaint();
    }
    catch(...)
    {
        deb("exception while makefatdisk");
    }

    // Application->ProcessMessages();
    // VdskSetDisk(hdisk, sizee, 2, 32, 512, false, true, true);
    // Button9Click(NULL);
    return;
}
char loadfile[128] = " ";

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button8Click(TObject *Sender)
{
    strcpy(loadfile, "0dump.dmp");
    MakeFAT *mf = new MakeFAT(false);

    // mf->WaitFor();
    // Form1->makefatdisk(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button10Click(TObject *Sender)
{
    maxquad = 0;
    strcpy(loadfile, "e:\\32gb2.dmp");
    if (!diskcolor)
        diskcolor = (unsigned int*)HeapAlloc(GetProcessHeap(), 0, Form1->pb1->Width*4);
    for (int i = 0;i<Form1->pb1->Width;i++)
        diskcolor[i] = clWhite;
    memset(memory, 0, (unsigned int)curdisksize);
    MakeFAT *mf = new MakeFAT(false);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button11Click(TObject *Sender)
{
    strcpy(savefile, "0dump.dmp");
    Button4->Enabled = false;
    dumpthread *d = new dumpthread(false);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::deblogCustomDrawSubItem(TCustomListView *Sender, TListItem *Item,
    int SubItem, TCustomDrawState State, bool &DefaultDraw)
{
    int pid;

    char cap[1212];

    deunicode(Item->Caption.c_str(), cap, sizeof(cap));

    CloseHandle(deblog->Canvas->Font->Handle);
    deblog->Canvas->Font->Handle = getfont("lucida console", 6, 7, 5);

    // getfont("lucida console", 7, 9, 0);

    if (cap[6] == '#')
    {
        deblog->Canvas->Font->Color = clBlue;

    }
    else
        if (cap[6] == '>')
            deblog->Canvas->Font->Color = clWebSeaGreen;
        else
            if (cap[6] == '!')
                deblog->Canvas->Font->Color = clRed;
            else
                if (cap[6] == '%')
                    deblog->Canvas->Font->Color = clOlive;
                else
                    deblog->Canvas->Font->Color = clBlack;

    deblog->Canvas->TextOutA(5, Item->Top, Item->Caption);

    // deb("pid %d DefaultDraw %d Sender %x", pid, DefaultDraw, Sender->GetHashCode());
    // DefaultDraw = false;
    char xx[128];
    // sprintf(xx, "pid %d", pid);

    // OutputDebugString(cap);
    // for (int x = Item->Left-3;x<Item->Left+1;x++)
    // for (int y = Item->Top+3;y<Item->Top+10;y++)
    // deblog->Canvas->Pixels[x][y] = RGB(dp, dp, 0xaa);

}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button12Click(TObject *Sender)
{
    FAT32BOOT f;
    memcpy(&f, memory, sizeof(f));
    deb("   CRC Fat 32 Boot record = 0x%04X", checksum((unsigned short*)memory, sizeof(f)));

    cdump((char*)memory, sizeof(FB), "FB.c");
    DWORD pfat;
    pfat = (DWORD)f.reserved_sectors * f.bytes_per_sector +
        (f.fat_copys*f.sectors_per_fat*f.bytes_per_sector);

    deb("   CRC Root Dir = 0x%04X", checksum((unsigned short*)memory+pfat,
            files.size()*2*sizeof(DIR)));
    DWORD pfat1, pfat2;
    pfat1 = (f.reserved_sectors * f.bytes_per_sector)+sizeof(FSINFO)+sizeof(FB);
    pfat2 = (f.reserved_sectors * f.bytes_per_sector) + ((f.sectors_per_fat*f.bytes_per_sector))
        +sizeof(FSINFO)+sizeof(FB);
    pfat1-=1024;
    pfat2-=1024;
    long fatsize = (f.sectors_per_fat-2)*f.bytes_per_sector;
    deb("pfat1 %d pfat2 %d", pfat1, pfat2);
    dump((char*)memory+pfat1, fatsize, "fat1.dmp");
    checksum((unsigned short*)memory+pfat1, fatsize);
    dump((char*)memory+pfat2, fatsize, "fat2.dmp");
    checksum((unsigned short*)memory+pfat2, fatsize);

    checksum((unsigned short*)memory, sizeof(FB));
    dump((unsigned char*)memory, sizeof(FB), "fat32boot.dmp");

    deb("root entries: %d, %d bytes", files.size(), files.size()*2*sizeof(DIR));
    dump((unsigned char*)memory+pfat2+(f.bytes_per_sector*f.sectors_per_fat),
        files.size()*2*sizeof(DIR)+1024, "rootdir.dmp");
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button13Click(TObject *Sender)
{
    if (files.size())
        deb("   \r\n \r\n ======== Network FAT Table (%d recs): ========", files.size());

    int s = 0;
    for (vector<FATFILE>::iterator it = files.begin();it!=files.end();it++)
    {
        deb("#%3d %7lu" " %10lu - %-10lu %-14s mem: (%p,crc:%x) clstr: %lu", ++s, (*it).size,
            (*it).start_offset, (*it).end_offset, (*it).fn, (*it).memory,
            checksum((unsigned short*)(*it).memory, 4096), (*it).firstcluster);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button14Click(TObject *Sender)
{

    // Form1->knotts->ClearChart();
    Form1->knotts->Series[0]->Clear();
    Form1->knotts->Series[1]->Clear();
    Form1->knotts->Series[2]->Clear();
    maxquad = 0;
    char txt2[111], *endptr;
    deunicode(Form1->ftpmaxds->Text.c_str(), txt2, sizeof(txt2));
    maxdiskuse = (LONGLONG)(atoi(txt2));
    maxdiskuse *= 1024L*1024L;
    // maxdiskuse = (unsigned long)strtoul(txt2,&endptr,10)*1024*1024;
    deb("MaxDiskUse %.2f GB (%I64d bytes) ", (double)maxdiskuse/1024.0/1024.0/1024.0,
        (LONGLONG)maxdiskuse);
    if ((memory && curdisksize) || FtpSocket)
    {
        unmounting = true;

        Sleep(300);

        if (fm)
        {
            fm->Terminate();
            fm = 0;

        }

        shutdown(FtpSocket, SD_BOTH);
        closesocket(FtpSocket);
        FtpSocket = 0;

        Button2Click(NULL);

        try
        {
            HeapFree(GetProcessHeap(), 0, memory);
            memory = 0;
        }
        catch(...)
        {
            deb("exception while freeing all in disconnect ftp");
        }
        curdisksize = 0;
        disksize = 0;

        if (!diskcolor)
            diskcolor = (unsigned int*)HeapAlloc(GetProcessHeap(), 0, Form1->pb1->Width*4);
        for (int i = 0;i<Form1->pb1->Width;i++)
            diskcolor[i] = clWhite;

        cache.clear();

        files.clear();
        fatfiles[0] = 0;
        Form1->Button14->Caption = "����������";

        Form1->Button14->Enabled = true;
        // Application->ProcessMessages();

        return;
    }

    disksize = (LONGLONG)32*1024*1024*1024;

    unmounting = false;
    fm = new FtpMount(false);
    fm->Priority = tpLowest;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::deblogKeyPress(TObject *Sender, wchar_t &Key)
{
    char sdf[11];
    wchar_t xx[11];
    static int width = 6;
    static int height = 8;
    static int q = 6;
    // wcscpy(xx,(wchar_t*)Key);
    memset(xx, 0, sizeof(xx));
    xx[0] = Key;
    deunicode((wchar_t*)xx, sdf, 11);
    deb("key %s (%c) 0x%X", sdf, sdf[0], sdf[0]);
    if (sdf[0] == 'w')
    {

        height++;

    }
    else
        if (sdf[0] == 's')
        {
            height--;
        }
        else
            if (sdf[0] == 'd')
            {
                width++;
            }
            else
                if (sdf[0] == 'a')
                {
                    width--;
                }
                else
                    if (sdf[0] == 'q')
                    {
                        if (++q>=7)
                            q = 0;
                        deb("quality %d", q);
                    }

    CloseHandle(deblog->Font->Handle);
    deblog->Font->Handle = getfont("lucida console", width, height, q);
}

void __fastcall TForm1::Button15Click(TObject *Sender)
{
    unsigned long siz;
    if (!curdisksize)
        return;
    siz = (unsigned long)HeapSize(GetProcessHeap(), 0, memory);
    deb("memory %lu MB @ %p", siz/1024/1024, memory);

    char srchstr[128];
    memset(srchstr, 0, sizeof(srchstr));
    deunicode(find->Text.c_str(), srchstr, sizeof(srchstr));
    deb("searching for %s ...", srchstr);
    int strsz = strlen(srchstr);
    unsigned long prevpos = 0;
    unsigned long founds = 0;
    Form1->Repaint();
    for (unsigned long mb = 0;mb<siz;mb++)
    {
        char c;
        char *cp;
        cp = (char*)((unsigned long)memory+mb);
        c = *cp;
        if (c == srchstr[0])
            if (strncmp(srchstr, cp, strsz) == 0)
            {
                char s[32];
                memset(s, 0, sizeof(s));
                memcpy(s, cp, 32);
                // for(int i=0;i<32;i++)
                // if(!isalpha((int)s[i]))
                // s[i]='.';
                // s[32]=0;
                unsigned long clust, sect, ofs;
                ofs = (unsigned long)cp - (unsigned long)memory;

                sect = ofs/fat32boot.bytes_per_sector;
                clust = ((dataoffset-ofs)/fat32boot.bytes_per_sector)
                    /fat32boot.sectors_per_cluster;
                deb("found @ %9lu (0x%08p) + %-9lu sect %lu clst %lu : %s", ofs, cp,
                    prevpos?ofs-prevpos:0, sect, clust, s);
                double ofsd;
                ofsd = (double)((double)ofs/(double)maxquad)*(double)Form1->pb1->Width;
                // deb(" %lu/%lu*%lu=%.3f",(unsigned long)ofs,(unsigned long)maxquad,
                // (unsigned long)Form1->pb1->Width,(double)ofsd);
                Form1->Canvas->Font->Size = 8;
                Form1->Canvas->Font->Color = clGray;
                Form1->Canvas->TextOutA(ofsd, Form1->pb1->Top-11, srchstr);
                cp+=strsz;
                prevpos = ofs;
                mb+=strsz;
                // Application->ProcessMessages();
                founds++;
            }
    }
    deb("done search: %lu.", founds);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::pb1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
    // POINT pt;
    // GetCursorPos(&pt);

    if (!curdisksize)
        return;

    int cp;
    cp = X-pb1->BoundsRect.left;
    if (cp<=0)
        return;

    char str[4096];

    double nsec, ncl, b;
    b = (double)curdisksize/((double)Form1->pb1->Width/(double)cp);
    nsec = b/(double)fat32boot.bytes_per_sector;
    ncl = (b - (double)dataoffset)/clstsize;
    sprintf(str, "sector    %-8.0f\r\ncluster   %-8.0f\r\nbyte      %-9.0f (%.0f MB)", nsec, ncl,
        b, (double)b/1024.0/1024.0);
    // diskhint->Caption=str;
    deblog->Canvas->Brush->Color = clWhite;
    deblog->Canvas->Brush->Style = bsSolid;
    deblog->Canvas->FillRect(TRect(0, 0, deblog->Width, deblog->Height/2));
    deblog->Canvas->Rectangle(5, 21, deblog->Width-5, (deblog->Height/2)-5);
    RECT rect;
    rect.left = 10;
    rect.top = 22;
    rect.right = deblog->Left+deblog->Width;
    rect.bottom = deblog->Top+deblog->Height;

    deblog->Canvas->Font->Handle = getfont("consolas", 6, 9, 6);
    DrawText(deblog->Canvas->Handle, str, strlen(str), &rect, DT_WORDBREAK);
    CloseHandle(deblog->Canvas->Font->Handle);
    rect.top = 80;

    bool file = false;
    for (vector<FATFILE>::iterator it = files.begin();it!=files.end();it++)
    {
        if (!(*it).start_offset)
            continue;

        if ((unsigned long)b>= (*it).start_offset&& (unsigned long)b <= (*it).end_offset)
        {

            char st[1024];
            double diff;
            diff = b-(*it).start_offset;
            sprintf(st, "%-20s %8lu/%8u", (*it).fn, (unsigned long)diff, (*it).size);
            DrawText(deblog->Canvas->Handle, (char*)st, strlen((char*)st), &rect, DT_WORDBREAK);
            diskhint->Caption = st;
            file = true;
        }

    }
    if (!file)
        diskhint->Caption = "<free>";

    if (b<fat2)
        diskhint->Caption = "FAT 1";
    else
        if (b>fat2&&b<rootdirectory)
            diskhint->Caption = "FAT 2";
    rect.top = 100;
    LPVOID dirptr;
    dirptr = (LPVOID)((unsigned long)memory+(unsigned long)b);
    try
    {
        if ((unsigned long)b<(unsigned long)curdisksize)
            DrawText(deblog->Canvas->Handle, (char*)dirptr, strlen((char*)dirptr), &rect,
            DT_WORDBREAK);
        Form1->OnPaint(NULL);
    }
    catch(...)
    {
        deb("exception while drawing dirptr string");
    }
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::EurekaLog1ExceptionNotify(TEurekaExceptionRecord &EurekaExceptionRecord,
    bool &Handled)
{
    char err[255];
    deunicode(EurekaExceptionRecord.CallStack->ToString().c_str(), err, sizeof(err));
    deb("exception: %s", err);
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::ftplogCustomDrawItem(TCustomListView *Sender, TListItem *Item,
    TCustomDrawState State, bool &DefaultDraw)
{
    // if (Item->Top <= 76)
    // {
    // ftplog->Canvas->FillRect(TRect(Item->Left, ftplog->Top, Item->Left+ftplog->Width, ftplog->Top+ftplog->Height));
    // return;
    // }
    char cap[1212];

    deunicode(Item->Caption.c_str(), cap, sizeof(cap));

    CloseHandle(ftplog->Canvas->Font->Handle);
    ftplog->Canvas->Font->Handle = getfont("lucida console", 6, 7, 5);

    // getfont("lucida console", 7, 9, 0);

    if (cap[6] == '#')
    {
        ftplog->Canvas->Font->Color = clBlue;

    }
    else
        if (cap[6] == '>')
            ftplog->Canvas->Font->Color = clWebSeaGreen;
        else
            if (cap[6] == '!')
                ftplog->Canvas->Font->Color = clRed;
            else
                if (cap[6] == '%')
                    ftplog->Canvas->Font->Color = clOlive;
                else
                    ftplog->Canvas->Font->Color = clMedGray;

    ftplog->Canvas->TextOutA(5, Item->Top, Item->Caption);
    // ftplog->Canvas->Unlock() ;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::ftpentKeyPress(TObject *Sender, wchar_t &Key)
{
    char sdf[11];
    wchar_t xx[11];
    static int width = 6;
    static int height = 8;
    static int q = 6;
    // wcscpy(xx,(wchar_t*)Key);
    memset(xx, 0, sizeof(xx));
    xx[0] = Key;
    deunicode((wchar_t*)xx, sdf, 11);
    // deb("key %s (%c) 0x%X", sdf, sdf[0], sdf[0]);

    char cmd[1111];
    deunicode(ftpent->Text.c_str(), cmd, sizeof(cmd));

    if (sdf[0] == '`')
    {
        shutdown(FtpSocket, SD_BOTH);
        closesocket(FtpSocket);
        shutdown(tempsock, SD_BOTH);
        closesocket(tempsock);
        FtpSocket = 0;
        tempsock = 0;
        Form1->ftpent->Clear();
        deb("ftp rest");
        ftpdeb(" ! ���������� ���������");
        ftpconn();
        return;
    }

    if (sdf[0] == 13)
    {
        if (!ftpcs->TryEnter())
        {
            ftpdeb("��� �����");
            return;
        }
        ftpcs->Leave();

        Form1->ftpent->Clear();

        // ftpblocking(false);
        if (strlen(cmd))
        {
            ftpcmd(cmd, 0, 0);
        }

        while (ftprl(cmd, (DWORD)-1, (char*)0)>0)
            continue;
        // ftpblocking(true);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ftpentEnter(TObject *Sender)
{
    char str[1288];
    deunicode(ftpent->Text.c_str(), str, sizeof(str));
    if (strstr(str, "computer"))
    {
        ftpent->Clear();
    }

}

// ---------------------------------------------------------------------------
void __fastcall TForm1::ftplogClick(TObject *Sender)
{
    ftpent->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::ftplogDblClick(TObject *Sender)
{
    ftplog->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::ftplogMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
    int X, int Y)
{
    if (Button = mbRight)
        ftplog->Visible = !ftplog->Visible;
}

// ---------------------------------------------------------------------------
void __fastcall TForm1::Button16Click(TObject *Sender)
{

    PROCESS_HEAP_ENTRY phe;
    memset(&phe, 0, sizeof(phe));
    unsigned long blks = 0;
    HeapLock(GetProcessHeap());
    unsigned long h2eapsize = 0;
    while (HeapWalk(GetProcessHeap(), &phe))
    {
        if (!phe.wFlags&PROCESS_HEAP_ENTRY_BUSY)
            continue;

        h2eapsize += (LONGLONG)phe.cbData + (unsigned long)phe.cbOverhead;
        blks++;
        deb("block #%4lu @ %8p - %7lu bytes (total %lu MB)", blks, phe.lpData, phe.cbData,
            (unsigned long)(h2eapsize/(unsigned long)1024L/(unsigned long)1024L));
    }
    HeapUnlock(GetProcessHeap());

    deb("heap total %lu MB", (unsigned long)(h2eapsize/(unsigned long)1024L/(unsigned long)1024L));
}
// ---------------------------------------------------------------------------
