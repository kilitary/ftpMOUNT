//---------------------------------------------------------------------------

#ifndef CacheThreadH
#define CacheThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class CacheThread : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall CacheThread(bool CreateSuspended);
    void __fastcall Do(void);
};
//---------------------------------------------------------------------------
#endif
