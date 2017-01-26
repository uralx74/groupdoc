//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditCcFormUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditCcForm *EditCcForm;
//---------------------------------------------------------------------------
__fastcall TEditCcForm::TEditCcForm(TComponent* Owner)
    : TForm(Owner)
{
    ccDttmDefaultValue = Now();
}
//---------------------------------------------------------------------------
void __fastcall TEditCcForm::Button3Click(TObject *Sender)
{
    String curFaId = MainDataModule->getFaPackQuery->FieldByName("fa_id")->AsString;
    String curAcctId = MainDataModule->getFaPackQuery->FieldByName("acct_id")->AsString;
    ccDttm = CcDttmDateTimePicker->Date;
    ccStatusFlg = (int)CcStatusFlgComboBox->KeyValue;    // V_INT // OleAuto.h
    ccTypeCd = (int)CcTypeCdComboBox->KeyValue;
    descrValue = DescrEdit->Text;
    callerValue = CallerEdit->Text;

    //TCcTypeCd::Type ccTypeCd;
    //TCcStatusFlg::Type ccStatusFlg;

    ccId = MainDataModule->addCc(
        _ds,
        ccDttm,      // дата контакта
        curAcctId,       // лицевой
        descrValue,          // описание
        curFaId,                 //
        callerValue,      // вызывающий
        ccTypeCd,          // тип контакта
        ccStatusFlg,    // статус
        TCcSourceTypeCd::FA // тип источника
    );

    SaveDefaultValues();

//    _modified = true;
}

/* —охран€ем значени€ по умолчанию */
void __fastcall TEditCcForm::SaveDefaultValues()
{
    if (LockCcDttmButton->Down)
    {
        ccDttmDefaultValue = ccDttm;
    }
    if (LockCcTypeCdButton->Down)
    {
        ccTypeCdDefaultValue = ccTypeCd;
    }
    if (LockCcStatusFlgButton->Down)
    {
        ccStatusFlgDefaultValue = ccStatusFlg;
    }
    if (LockDescrButton->Down)
    {
        descrDefaultValue = descrValue;
    }

    //callerDefaultValue;
    //descrDefaultValue;
}

/* ¬осстанавливаем значени€ на форме */
void __fastcall TEditCcForm::RestoreDefaultValues()
{
    if ( LockCcDttmButton->Down )
    {
            CcDttmDateTimePicker->Date = ccDttmDefaultValue;
    }
    else
    {
        if ( _ds->FieldByName("cc_dttm")->IsNull )
        {
            CcDttmDateTimePicker->Date = Now();
        }
        else
        {
            CcDttmDateTimePicker->DateTime = _ds->FieldByName("cc_dttm")->Value;
        }
    }


    if ( LockCcTypeCdButton->Down )
    {
        CcTypeCdComboBox->KeyValue = ccTypeCd;
    }
    else
    {
        CcTypeCdComboBox->KeyValue = TCcTypeCd::MANUAL;
    }

    if ( LockCcStatusFlgButton->Down )
    {
        CcStatusFlgComboBox->KeyValue = ccStatusFlgDefaultValue;
    }
    else
    {
        CcStatusFlgComboBox->KeyValue = TCcStatusFlg::SUCCESS;
    }

    if ( LockCallerButton->Down )
    {
        CallerEdit->Text = callerDefaultValue;
    }
    else
    {
        CallerEdit->Text = "";
    }

    if ( LockDescrButton->Down )
    {
        DescrEdit->Text = descrDefaultValue;
    }
    else
    {
        DescrEdit->Text = "";
    }
}

void __fastcall TEditCcForm::ResetDefaultValues()
{
}

/**/
bool TEditCcForm::Execute(TDataSet* ds)
{
    _ds = ds;
    RestoreDefaultValues();

    AcctIdLabel->Caption = ds->FieldByName("acct_id")->Value;
    FioLabel->Caption = ds->FieldByName("fio")->Value;


    switch (ShowModal())
    {
    case mrOk:
    {
        //ShowMessage("ќк");
        return true;
    }
    case mrCancel:
    {
        //ShowMessage("Canceled");
        return false;
    }
    }
}

//---------------------------------------------------------------------------
void __fastcall TEditCcForm::Button1Click(TObject *Sender)
{
    //_modified = true;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditCcForm::FormCreate(TObject *Sender)
{
    FormSpecialModule->ButtonImageList->GetBitmap(0, LockCcDttmButton->Glyph);
    FormSpecialModule->ButtonImageList->GetBitmap(0, LockCcTypeCdButton->Glyph);
    FormSpecialModule->ButtonImageList->GetBitmap(0, LockCcStatusFlgButton->Glyph);
    FormSpecialModule->ButtonImageList->GetBitmap(0, LockCallerButton->Glyph);
    FormSpecialModule->ButtonImageList->GetBitmap(0, LockDescrButton->Glyph);

}
//---------------------------------------------------------------------------

