//---------------------------------------------------------------------------

#ifndef SelectFaPackFormUnitH
#define SelectFaPackFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBGridAlt.h"
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "MainDataModuleUnit.h"
#include "FaTypes.h"
#include "OtdelenComboBoxFrameUnit.h"
#include <DBCtrls.hpp>

//---------------------------------------------------------------------------
class TSelectFaPackForm : public TForm
{
__published:	// IDE-managed Components
    TDBGridAlt *faListGrid;
    TButton *Button1;
    TButton *Button2;
    TGroupBox *GroupBox1;
    TLabel *Label13;
    TComboBox *FaPackIdComboBox;
    TButton *Button3;
    TComboBox *FaPackTypeCdComboBox;
    TLabel *Label1;
    TGroupBox *GroupBox2;
    TLabel *Label4;
    TButton *Button4;
    TEdit *Edit1;
    TGroupBox *GroupBox3;
    TLabel *Label5;
    TButton *Button5;
    TDBLookupComboBox *OtdelenComboBox2;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall faListGridChangeCheck(TObject *Sender);
    void __fastcall FilterComboBoxChange(TObject *Sender);
    void __fastcall OtdelenComboBox2Click(TObject *Sender);
private:	// User declarations
    String _faPackId;
    //String _faPackType;
    //FaPack _faPack;
    //TDataSetFilter _filter;   // Фильтр

public:		// User declarations
    __fastcall TSelectFaPackForm(TComponent* Owner);
    bool __fastcall execute();
    FaPack getFaPack();
    String __fastcall getFaPackId();
    //FaTypes::PackTypeId __fastcall getFaPackTypeId();
    //void  __fastcall setCurOtdelen(const String& otdelen);
    //FaTypes::PackTypeId _fastcall getFaPackType();

};
//---------------------------------------------------------------------------
extern PACKAGE TSelectFaPackForm *SelectFaPackForm;
//---------------------------------------------------------------------------
#endif
