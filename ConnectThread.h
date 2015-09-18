//---------------------------------------------------------------------------

#ifndef ConnectThreadH
#define ConnectThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class ConnectThread : public TThread
{
protected:
    void __fastcall Execute();
public:
    __fastcall ConnectThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
