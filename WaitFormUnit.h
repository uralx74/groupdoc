//---------------------------------------------------------------------------

#ifndef WaitFormUnitH
#define WaitFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TWaitForm : public TForm
{
__published:	// IDE-managed Components
    TImage *Image1;
    TLabel *MessageLabel;
    TBevel *Bevel1;
    TLabel *StatusLabel;
    TSpeedButton *SpeedButton1;
    TSpeedButton *CancelBtn;
    TImageList *ImageList1;
    TTimer *Timer1;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormHide(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall FormDeactivate(TObject *Sender);
    void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
    bool _canClose;
    TForm* _parentForm;

public:		// User declarations
    __fastcall TWaitForm(TComponent* Owner);
    void __fastcall Execute();
    void __fastcall Close();

    void __fastcall SetStatus(const String& text);
    void __fastcall SetMessage(const String& text);

};
//---------------------------------------------------------------------------
extern PACKAGE TWaitForm *WaitForm;
//---------------------------------------------------------------------------
#endif
