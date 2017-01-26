//---------------------------------------------------------------------------
/*
   Модуль для формирования документов
*/

#ifndef DocumentDataModuleUnitH
#define DocumentDataModuleUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DataSetFilter.h"
#include "DocumentWriter.h"
#include "MainDataModuleUnit.h"
#include <DB.hpp>
#include <DBClient.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Ora.hpp"
#include "OraSmart.hpp"
#include <Provider.hpp>

//---------------------------------------------------------------------------
class TDocumentDataModule : public TDataModule
{
__published:	// IDE-managed Components
    TClientDataSet *ClientDataSet1;
    TOraTable *mergeDataSet;
    TDataSetProvider *mergeDataSetProvider;
private:	// User declarations
    TDocumentWriter* documentWriter;
    void __fastcall BeginPrint(TDataSetFilter* mergeFields);
    void __fastcall EndPrint(TDataSetFilter* mergeFields);


public:		// User declarations
    __fastcall TDocumentDataModule(TComponent* Owner);
    void __fastcall getDocumentFaNoticesList(TDataSetFilter *mergeFields);
    void __fastcall getDocumentFaNotices(TDataSetFilter* mergeFields);
    //void __fastcall getDocumentFaNotices(TDataSet *mergeFields, TDataSet* formFields);


};
//---------------------------------------------------------------------------
extern PACKAGE TDocumentDataModule *DocumentDataModule;
//---------------------------------------------------------------------------
#endif
