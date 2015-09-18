//---------------------------------------------------------------------------

#ifndef MakeFATThreadH
#define MakeFATThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class MakeFAT : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall MakeFAT(bool CreateSuspended);
    void __fastcall Do(void);
};
//---------------------------------------------------------------------------
#endif
