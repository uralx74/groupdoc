//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

#include "ThreadDataSet.h"

/**/
__fastcall TThreadDataSet::TThreadDataSet(bool CreateSuspended, TOraQuery* query, TDataSet* destinationDataSet,  TDataSetNotifyEvent NotifyEvent)
    : TThread(CreateSuspended)
{
    FreeOnTerminate = true;
    Suspended = true;


    _notifyEvent = NotifyEvent;

    _oraSession = new TOraSession(NULL);

    _dataSet = new TOraQuery(NULL);
    _dataSet->Assign(query);
    _dataSet->FetchAll = true;

    _oraSession->AssignConnect(query->Session);
    _dataSet->Session = _oraSession;

    _destinationDataSet = destinationDataSet;


    if ( !CreateSuspended )
    {
        Resume();
    } 


    //WParamResultMessage = 0;
    //LParamResultMessage = 0;
    /*AppPath = ExtractFilePath(Application->ExeName);

    SetThreadOpt(threadopt);
    _threadIndex++;

    randomize();
    _threadId = random(9999999999);*/
}

/**/
__fastcall TThreadDataSet::~TThreadDataSet()
{
    delete _dataSet;
    delete _oraSession;
}

/**/
void __fastcall TThreadDataSet::Execute()
{
    if (_dataSet != NULL)
    {
        _dataSet->Open();
        Synchronize(SyncQuery);
    }    
}

/**/
void __fastcall  TThreadDataSet::SyncQuery()
{
    try
    {
        if (_destinationDataSet != NULL)
        {
            CopyDataSet(_dataSet, _destinationDataSet);
        }
        if ( _notifyEvent != NULL )
        {
            _notifyEvent(_dataSet);
        }
    }
    catch(...)
    {
    }
}

/*  опирует данные из источника в приемник */
void __fastcall TThreadDataSet::CopyDataSet(TDataSet* sourceDs, TDataSet* destinationDs)
{
    destinationDs->DisableControls();
    destinationDs->FieldDefs->Assign(sourceDs->FieldDefs);
    destinationDs->Open();
    destinationDs->Assign(sourceDs);
    destinationDs->EnableControls();
    //ShowMessage(MainDataModule->VirtualTable1->RecordCount);
    //DBGridAltGeneral->DataSource->DataSet = MainDataModule->VirtualTable1;
    //MainDataModule->VirtualTable1->Filtered = true;
}



