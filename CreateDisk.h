//---------------------------------------------------------------------------

#ifndef CreateDiskH
#define CreateDiskH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class CreateDisk : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall CreateDisk(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
