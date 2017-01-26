//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#define DEBUG

USEFORM("MainFormUnit.cpp", MainForm);
USEFORM("FieldActivityFormUnit.cpp", FieldActivityForm);
USEFORM("MainDataModuleUnit.cpp", MainDataModule); /* TDataModule: File Type */
USEFORM("GridFrameUnit.cpp", GridFrame); /* TFrame: File Type */
USEFORM("SelectFaPackFormUnit.cpp", SelectFaPackForm);
USEFORM("EditCcFormUnit.cpp", EditCcForm);
USEFORM("FormSpecialModuleUnit.cpp", FormSpecialModule); /* TDataModule: File Type */
USEFORM("DocumentDataModuleUnit.cpp", DocumentDataModule); /* TDataModule: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->CreateForm(__classid(TMainDataModule), &MainDataModule);
         Application->CreateForm(__classid(TFormSpecialModule), &FormSpecialModule);
         Application->CreateForm(__classid(TDocumentDataModule), &DocumentDataModule);
         Application->CreateForm(__classid(TFieldActivityForm), &FieldActivityForm);
         Application->CreateForm(__classid(TSelectFaPackForm), &SelectFaPackForm);
         Application->CreateForm(__classid(TEditCcForm), &EditCcForm);
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
