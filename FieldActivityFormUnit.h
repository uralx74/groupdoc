//---------------------------------------------------------------------------

#ifndef FieldActivityFormUnitH
#define FieldActivityFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "..\AltCtrl\DBGridAlt.h"
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "MainDataModuleUnit.h"
#include "DocumentDataModuleUnit.h"
#include "SelectFaPackFormUnit.h"
#include <Buttons.hpp>
#include <jpeg.hpp>
#include "OtdelenComboBoxFrameUnit.h"
#include "..\util\hack_ctrl.h"
#include "..\util\odacutils.h"
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include "..\util\ColorList.h"
#include "EditAlt.h"
#include <DBCtrls.hpp>
#include "MemDS.hpp"
#include "VirtualTable.hpp"
#include <DB.hpp>
#include "EditCcFormUnit.h"
#include "..\util\Messages.h"
#include "DataSetFilter.h"
#include <ActnList.hpp>

enum PackMode {
    PACK_MODE_UNDEFINED = 0,
    PACK_GENERAL,
    PACK_MANUAL,
    PACK_POST,
    PACK_STOP,
    PACK_MANUAL_POST,
    PACK_APPROVE_LIST
};

//---------------------------------------------------------------------------
class TFieldActivityForm : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox5;
    TBitBtn *ResetFiltersButton;
    TComboBox *PackIdFilterComboBox;
    TComboBox *FioComboBox;
    TComboBox *AddressComboBox;
    TComboBox *AcctIdComboBox;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label13;
    TGroupBox *GroupBox6;
    TBitBtn *BitBtn1;
    TComboBox *ComboBox5;
    TLabel *Label19;
    TComboBox *ServiceCompanyFilterComboBox;
    TGroupBox *GroupBox7;
    TLabel *Label2;
    TEdit *ParamPackIdEdit;
    TLabel *Label6;
    TGroupBox *GroupBox1;
    TLabel *Label18;
    TLabel *Label17;
    TLabel *Label16;
    TLabel *Label15;
    TLabel *Label14;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TDateTimePicker *DateTimePicker1;
    TDateTimePicker *DateTimePicker2;
    TCheckBox *CcDttmIsNullCheckBox;
    TLabel *Label1;
    TLabel *Label7;
    TPopupMenu *SelectAcctPopupMenu;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N31;
    TBitBtn *ShowSelectAcctMenuButton;
    TButton *Button6;
    TPageControl *PackPageControl;
    TTabSheet *DebtorsTabSheet;
    TTabSheet *PackManualTabSheet;
    TTabSheet *StopListTabSheet;
    TBitBtn *ShowActionsMenuButton;
    TPopupMenu *ActionsMenu;
    TMenuItem *N4;
    TCheckBox *CheckBox1;
    TGroupBox *GroupBox3;
    TLabel *Label20;
    TLabel *Label21;
    TEditAlt *SaldoFilterEdit;
    TLabel *Label22;
    TComboBox *CityComboBox;
    TComboBox *ComboBox1;
    TLabel *Label23;
    TBitBtn *BitBtn6;
    TMenuItem *N5;
    TTabSheet *ApprovalListTabSheet;
    TDBGridAlt *DBGridAltManual;
    TDBGridAlt *DBGridAltGeneral;
    TDBGridAlt *ApproveListGrid;
    TCheckBox *CheckBox2;
    TMenuItem *DocumentStopMenuItem;
    TMenuItem *N7;
    TMenuItem *N8;
    TDBGridAlt *StopListDBGrid;
    TDBLookupComboBox *OtdelenComboBox;
    TActionList *ActionList1;
    TAction *printDocumentFaNoticesAction;
    TAction *printDocumentFaNoticesListAction;
    TAction *printDocumentStopAction;
    TAction *printDocumentStopActionRefuseAction;
    TAction *printDocumentStopListAction;
    TAction *createFaPackAction;
    TAction *approveFaPackCcDttmAction;
    TBitBtn *BitBtn2;
    TPopupMenu *ActionsPopupMenu;
    TMenuItem *MenuItem1;
    TMenuItem *MenuItem2;
    TDBGrid *DBGrid1;
    TAction *checkAllAction;
    TAction *checkNoneAction;
    TAction *checkWithoutCcAction;
    TAction *checkWithCcLess3MonthAction;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FilterControlChange(TObject *Sender);
    void __fastcall DBGridAltGeneralChangeCheck(TObject *Sender);
    void __fastcall DBGridAltGeneralChangeFilter(TObject *Sender);
    void __fastcall ParamPackIdEditClick(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall OtdelenBoxChange(TObject *Sender);
    void __fastcall ParamPackIdEditKeyPress(TObject *Sender, char &Key);
    void __fastcall FilterCcDttmChange(TObject *Sender);
    void __fastcall DBGridAltGeneralEnter(TObject *Sender);
    void __fastcall DBGridAltManualCellClick(TColumn *Column);
    void __fastcall N2Click(TObject *Sender);
    void __fastcall ShowSelectAcctMenuButtonKeyPress(TObject *Sender,
          char &Key);
    void __fastcall ShowSelectAcctMenuButtonMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall PackPageControlDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active);
    void __fastcall SaldoFilterEditChange(TObject *Sender);
    void __fastcall DebtorsTabSheetShow(TObject *Sender);
    void __fastcall PackManualTabSheetShow(TObject *Sender);
    void __fastcall ApprovalListTabSheetShow(TObject *Sender);
    void __fastcall ShowActionsMenuButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ResetFiltersButtonClick(TObject *Sender);
    void __fastcall StopListTabSheetShow(TObject *Sender);
    void __fastcall OtdelenComboBoxClick(TObject *Sender);
    void __fastcall printDocumentFaNoticesActionExecute(TObject *Sender);
    void __fastcall printDocumentFaNoticesListActionExecute(TObject *Sender);
    void __fastcall createFaPackActionExecute(TObject *Sender);
    void __fastcall approveFaPackCcDttmActionExecute(TObject *Sender);
    void __fastcall BitBtn2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall checkWithCcLess3MonthActionExecute(TObject *Sender);
    void __fastcall checkWithoutCcActionExecute(TObject *Sender);
    void __fastcall checkAllActionExecute(TObject *Sender);
private:	// User declarations
    void __fastcall showDebtorList(const String& acctOtdelen);
    void __fastcall showFaPack(const String& faPackId);
    void __fastcall showStopList(const String& acctOtdelen);
    void __fastcall showApprovalList(const String& acctOtdelen);


    void __fastcall showFaList();
    void __fastcall showFaPostList();
    void __fastcall showFaInspectorList();

    void __fastcall resetFilterControls();

    //void __fastcall setCurOtdelen(const String& otdelen, bool resetPack = true);
    void __fastcall setCurPackMode(PackMode packMode);
    //void __fastcall setCurPackId(const String& faPackId);
    void __fastcall refreshControls();
    void __fastcall showMenuSelect(TButton *button, TPopupMenu* menu);
    void __fastcall refreshParametersControls();
    void __fastcall refreshFilterControls();


    //void __fastcall setFilter(const String& filterName, const String& filterValue);
    ColorList _colorList;


    TDBGridAlt* _currentDbGrid;

    TDataSetFilter* _currentFilter;
    //TDataSetFilter _filterPackGeneral;   // Фильтр
    //TDataSetFilter _filterPackManual;   // Фильтр
    //TDataSetFilter _filterApproveList;   // Фильтр

    PackMode _curPackMode;  // Текущий режим
    //String _curPackId;      // Текущая пачка
    //String _curOtdelen;     // Текущий участок

    bool _packPageControlChangedSelf;
    /**/
    //bool _packPageControlChangedSelf;

public:		// User declarations
    __fastcall TFieldActivityForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFieldActivityForm *FieldActivityForm;
//---------------------------------------------------------------------------
#endif
