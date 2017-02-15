//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "WaitFormUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWaitForm *WaitForm;
//---------------------------------------------------------------------------
__fastcall TWaitForm::TWaitForm(TComponent* Owner)
    //: TForm( (TComponent*)(NULL))
    : TForm(Owner)
{
    _parentForm = (TForm*)Owner;
    SetMessage("");
    SetStatus("");
}

/**/
void __fastcall TWaitForm::Timer1Timer(TObject *Sender)
{
    static int FrameIndex = 0;
    if (++FrameIndex >= ImageList1->Count)
    {
        FrameIndex=0;
    }

    Image1->Canvas->Lock();

    Image1->Canvas->FillRect(Image1->ClientRect); // Очищаем от старой картинки
    ImageList1->GetBitmap(FrameIndex, Image1->Picture->Bitmap);
    this->Refresh();

}

/**/
void __fastcall TWaitForm::FormCreate(TObject *Sender)
{
    this->DoubleBuffered = true;
}

/**/
void __fastcall TWaitForm::FormShow(TObject *Sender)
{
    Timer1->Enabled = true;
}

/**/
void __fastcall TWaitForm::FormHide(TObject *Sender)
{
    Timer1->Enabled = false;

}

/**/
void __fastcall TWaitForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    CanClose = _canClose;
}

/**/
void __fastcall TWaitForm::FormActivate(TObject *Sender)
{
    /*((TForm*)Sender)->Enabled = false;
    Show();*/
}

/**/
void __fastcall TWaitForm::FormDeactivate(TObject *Sender)
{
    /*((TForm*)Sender)->Enabled = true;
    Hide();*/
}

/**/
void __fastcall TWaitForm::SpeedButton1Click(TObject *Sender)
{
    SendMessage(Application->MainForm->Handle, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

/**/
void __fastcall TWaitForm::Execute()
{
    //((TForm*)this->Owner)->Enabled = false;
    _parentForm->Enabled = false;
    _canClose = false;
    Show();
}

/**/
void __fastcall TWaitForm::Close()
{
    _parentForm->Enabled = true;
    _canClose = true;
    TForm::Close();
}

/**/
void __fastcall TWaitForm::SetStatus(const String& text)
{
    StatusLabel->Caption = text;
}

/**/
void __fastcall TWaitForm::SetMessage(const String& text)
{
    MessageLabel->Caption = text;
}

