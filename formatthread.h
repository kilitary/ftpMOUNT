//---------------------------------------------------------------------------

#ifndef formatthreadH
#define formatthreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class formatthread : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall formatthread(bool CreateSuspended);
    void __fastcall Do(void);
};
//---------------------------------------------------------------------------
#endif
