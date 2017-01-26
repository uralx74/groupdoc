//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DocumentDataModuleUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DataSetFilter"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Ora"
#pragma link "OraSmart"
#pragma resource "*.dfm"
TDocumentDataModule *DocumentDataModule;
//---------------------------------------------------------------------------
__fastcall TDocumentDataModule::TDocumentDataModule(TComponent* Owner)
    : TDataModule(Owner)
{
    documentWriter = new TDocumentWriter();
}

/* Функция для подготовки к выводу данных */
void __fastcall TDocumentDataModule::BeginPrint(TDataSetFilter* mergeFields)
{
    mergeFields->LockDataSetPos();
    mergeFields->DisableControls();

    MainDataModule->getCheckedFilter->setValue("checked", "", " ");
    MainDataModule->getCheckedFilter->DataSet = mergeFields->DataSet;
}

/* Функция, вызываемая при завершении вывода данных */
void __fastcall TDocumentDataModule::EndPrint(TDataSetFilter* mergeFields)
{
    MainDataModule->getCheckedFilter->DataSet = NULL;

    mergeFields->EnableControls();
    mergeFields->UnlockDataSetPos();
}

/* */
//void __fastcall TDocumentDataModule::getDocumentFaNotices(TDataSet* mergeFields, TDataSet* formFields)
void __fastcall TDocumentDataModule::getDocumentFaNotices(TDataSetFilter* mergeFields)
{
    TWordExportParams wordExportParams;
    wordExportParams.templateFilename = "c:\\PROGRS\\current\\GroupDoc\\report\\template_document_notice.dotx";
    wordExportParams.resultFilename = "c:\\PROGRS\\current\\GroupDoc\\report\\document_notice_[:counter].doc";
    wordExportParams.pagePerDocument = 500;

    BeginPrint(mergeFields);

    if ( mergeFields->DataSet->RecordCount > 0)
    {
        MainDataModule->getFaPackInfo->ParamByName("fa_pack_id")->Value = mergeFields->DataSet->FieldByName("fa_pack_id")->Value;
        MainDataModule->openOrRefresh(MainDataModule->getFaPackInfo);
        documentWriter->ExportToWordTemplate(&wordExportParams, mergeFields->DataSet, MainDataModule->getFaPackInfo);
    }
    
    EndPrint(mergeFields);

}

/* */
void __fastcall TDocumentDataModule::getDocumentFaNoticesList(TDataSetFilter *mergeFields)
{
    TExcelExportParams excelExportParams;
    excelExportParams.templateFilename = "c:\\PROGRS\\current\\GroupDoc\\report\\template_document_notice.xltx";
    excelExportParams.resultFilename = "c:\\PROGRS\\current\\GroupDoc\\report\\document_notice.xlsx";
    excelExportParams.table_range_name = "range_body";


    BeginPrint(mergeFields);

    if ( mergeFields->DataSet->RecordCount > 0)
    {
        MainDataModule->getFaPackInfo->ParamByName("fa_pack_id")->Value = mergeFields->DataSet->FieldByName("fa_pack_id")->Value;
        MainDataModule->openOrRefresh(MainDataModule->getFaPackInfo);
        //documentWriter->ExportToExcelTemplate(&excelExportParams, filter->DataSet, MainDataModule->getOtdelenListQuery);
        documentWriter->ExportToExcelTemplate(&excelExportParams, mergeFields->DataSet, MainDataModule->getFaPackInfo);
    }
    EndPrint(mergeFields);

}




/*
    mergeDataSetProvider->DataSet = mergeFields->DataSet;
    ClientDataSet1->Data = mergeDataSetProvider->Data;

    //mergeDataSet->Assign() = mergeDataSetProvider->DataSet;

    int yyy1 = ClientDataSet1->RecordCount;
    int yyy2 = mergeDataSet->RecordCount;
*/
    //mergeFields->DataSet = TempProvider->DataSet;
    //ClientDataSet1->Data = mergeFields->;
    /*TDataSetProvider* TempProvider = new TDataSetProvider(this);
    TempProvider->DataSet = mergeFields->DataSet;

    ClientDataSet1->Data = TempProvider->Data;
    mergeDataSet = TempProvider->DataSet;
                                           */

    //int xxx = ClientDataSet1->RecordCount;

    /*ClientDataSet1->Data = TempProvider->Data;
    delete TempProvider;
                        */

    //TClientDataSet* mergeDataSet = new TClientDataSet(NULL);
    //mergeDataSet->Assign(mergeFields->DataSet);
    //mergeFields->DataSet->Assign(mergeDataSet);
    //mergeDataSet->Active;




