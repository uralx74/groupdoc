//---------------------------------------------------------------------------

#ifndef EditCcFormUnitH
#define EditCcFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include "MainDataModuleUnit.h"
#include "FormSpecialModuleUnit.h"
#include <DBCtrls.hpp>
//---------------------------------------------------------------------------
class TEditCcForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox2;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TDateTimePicker *CcDttmDateTimePicker;
    TEdit *CallerEdit;
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *AcctIdLabel;
    TLabel *Label3;
    TLabel *FioLabel;
    TButton *Button1;
    TButton *Button2;
    TButton *Button3;
    TRichEdit *DescrEdit;
    TSpeedButton *LockCcDttmButton;
    TSpeedButton *LockCcTypeCdButton;
    TSpeedButton *LockCcStatusFlgButton;
    TSpeedButton *LockCallerButton;
    TSpeedButton *LockDescrButton;
    TDBLookupComboBox *CcTypeCdComboBox;
    TDBLookupComboBox *CcStatusFlgComboBox;
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    bool _canceled;
    //bool _modified;
    TDataSet* _ds;

    /* default values to each display form */
    void __fastcall SaveDefaultValues();
    void __fastcall RestoreDefaultValues();
    void __fastcall ResetDefaultValues();
    TCcTypeCd::Type ccTypeCdDefaultValue;
    TCcStatusFlg::Type ccStatusFlgDefaultValue;
    TDateTime ccDttmDefaultValue;
    String callerDefaultValue;
    String descrDefaultValue;


    /**/
    TDateTime ccDttm;
    TCcStatusFlg::Type ccStatusFlg;
    TCcTypeCd::Type ccTypeCd;
    String callerValue;
    String descrValue;


public:		// User declarations
    __fastcall TEditCcForm(TComponent* Owner);
    bool Execute(TDataSet* ds);

    String ccId;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditCcForm *EditCcForm;
//---------------------------------------------------------------------------
#endif
