//---------------------------------------------------------------------------

#ifndef FtpMountThreadH
#define FtpMountThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class FtpMount : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall FtpMount(bool CreateSuspended);
     __fastcall ~FtpMount(void);
     void  __fastcall Do(void);
};
//---------------------------------------------------------------------------
#endif
