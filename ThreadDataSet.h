//---------------------------------------------------------------------------

#ifndef ThreadDataSetH
#define ThreadDataSetH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Ora.hpp"
#include "math.h"
#include <DB.hpp>
#include "MemDS.hpp"
#include "DBAccess.hpp"


//---------------------------------------------------------------------------
class TThreadDataSet : public TThread
{
private:

    TOraQuery* _dataSet;
    TOraSession* _oraSession;
    TDataSetNotifyEvent _notifyEvent;
    void __fastcall  SyncQuery();

    //TNotifyEvent FOnChangeCheck;
    //**TOnDataEvent fOnData;


public:
    //__fastcall ThreadSelect(bool CreateSuspended, THREADOPTIONS* threadopt, void (*f)(const String&, int));
    __fastcall TThreadDataSet(bool CreateSuspended, TOraQuery* query, TDataSet* destinationDataSet = NULL, TDataSetNotifyEvent NotifyEvent = NULL);
    __fastcall ~TThreadDataSet();
    void __fastcall Execute();
    
    //TOraSession* __fastcall CreateOraSession(TOraSession* TemplateOraSession);
    //void __fastcall SyncThreadChangeStatus();

private:
    HWND ParentFormHandle;
    TDataSet* _destinationDataSet;
    void __fastcall CopyDataSet(TDataSet* sourceDs, TDataSet* destinationDs);


};
//---------------------------------------------------------------------------

#endif
