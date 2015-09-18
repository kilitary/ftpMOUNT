//---------------------------------------------------------------------------

#ifndef dumpthreadH
#define dumpthreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class dumpthread : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall dumpthread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
