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
#include "formlogin.h"
#include <ActnMan.hpp>
#include <set>
#include "WaitFormUnit.h"

enum PackMode {
    PACK_MODE_UNDEFINED = 0,
    PACK_GENERAL,
    PACK_MANUAL,
    PACK_POST,
    PACK_STOP,
    PACK_MANUAL_POST,
    PACK_APPROVE_LIST
};



typedef enum  {
    MODE_UNDEFINED = 0,
    MODE_NOTICES,
    MODE_STOP
} TMode;

typedef enum   {
    SHEET_TYPE_UNDEFINED = 0,
    SHEET_TYPE_DEBTORS,         // general
    SHEET_TYPE_NOTICES_PACK,    //PACK_MANUAL,
    SHEET_TYPE_APPROVE,

    SHEET_TYPE_POST_LIST,
    //SHEET_TYPE_POST_PACK,

    SHEET_TYPE_STOP,
    SHEET_TYPE_PACK_STOP_LIST,  // ������ �������� �� ����������
    SHEET_TYPE_STOP_PACK,
    SHEET_TYPE_STOP_REFUSE_PACK,
    SHEET_TYPE_RELOAD
    //PACK_APPROVE_LIST
} TSheetType;


/* ����� ��� �������� ���������� ������� ��� TTabSheet */
class TSheetEx
{
private:
    std::vector<TAction*> _actions; // ������ ��������� ��������

public:
    TSheetEx(TTabSheet* _sheet, bool _accessible);
    TTabSheet* sheet;   // ��������� �� �������
    bool accessible;    // ������� ����������� ������������

    //void addAction(TAction* action);
    // std::vector<Action*> filters; // ������ ��������� ��������

    /* actions */
    void assignSheet(TTabSheet* sheet, bool accessible = false);
    void addAction(TAction* action, bool accessible = false);
    void showActions();

    TSheetType packModeId;    // ������� ������, ������� ����� ��� ���!!!!!!!!!! �������� ��!
};

/* ����������� */
TSheetEx::TSheetEx(TTabSheet* _sheet, bool _accessible):
    sheet(_sheet), accessible(_accessible)
{
}

/**/
void TSheetEx::assignSheet(TTabSheet* sheet, bool accessible)
{
    this->sheet = sheet;
    this->accessible = accessible;
}

/* ��������� �������� � ������ ��������� ������� ��� �����*/
void TSheetEx::addAction(TAction* action, bool accessible)
{
    _actions.push_back(action);
}

/* ���������� ��������� �������� */
void TSheetEx::showActions()
{
    for (std::vector<TAction*>::iterator action = _actions.begin(); action != _actions.end(); action++)
    {
        (*action)->Visible = true;
    }
}

/* ����� ��� �������� ��������(�������),
   ��������������� ����� ������ ��������� */
class TModeItem
{
private:
    void hideAllActions();
    std::vector<TAction*> _actions; // ������ ���� �������� (��� ���� ����� ����� ���� ������ ��� �������� ����� ������������ �����)

public:
    TModeItem::TModeItem();
    String caption;     // ������������ ������
    //String modeType;    // �������� ��� ����� pack_type_cd
    TMode mode;
    std::vector<TSheetEx> sheets;   // ������ �������(�����������)
    void showSheets();

    TSheetEx* addSheet(TTabSheet* _sheet, bool _accessible = true);

    void showSheet(TSheetEx* sheetEx);
    void showSheet(int index);
    TSheetEx* currentSheet;

    TSheetEx* getSheetByType(TSheetType type);
    void showSheet(TSheetType type);

    void addAction(TAction* action);

};
TModeItem::TModeItem()
{
    //currentSheet = sheets.begin()+1;
}

/* ��������� �������� � ����� ������ */
void TModeItem::addAction(TAction* action)
{
    _actions.push_back(action);
}

/* ��������� ��������� */
TSheetEx* TModeItem::addSheet(TTabSheet* _sheet, bool _accessible)
{
    sheets.push_back(TSheetEx(_sheet, _accessible));
    return &sheets.back();
}

/* ���������� ��������� ������� */
void TModeItem::showSheets()
{
    // ���������� ������ ������� ��������� ������ � ��������� �������� ������������ �� ��� ����
    for (std::vector<TSheetEx>::iterator sheet = sheets.begin(); sheet != sheets.end(); sheet++)
    {
        sheet->sheet->TabVisible = sheet->accessible;
    }
}

/* �������� �������� �� ���� */
TSheetEx* TModeItem::getSheetByType(TSheetType type)
{
    for(std::vector<TSheetEx>::iterator sheet = sheets.begin(); sheet != sheets.end(); sheet++)
    {
        if (sheet->packModeId == type)
        {
            return sheet;
        }
    }
    return SHEET_TYPE_UNDEFINED;
}

/* ������ ��� �������� � Action ������ */
void TModeItem::hideAllActions()
{
    for (std::vector<TAction*>::iterator action = _actions.begin(); action != _actions.end(); action++)
    {
        (*action)->Visible = false;
    }
}

/* ���������� ���� */
void TModeItem::showSheet(TSheetEx* sheetEx)
{
    if ( sheetEx->accessible )
    {
        hideAllActions();

        currentSheet = sheetEx;
        if ( !currentSheet->sheet->Visible )
        {
            currentSheet->sheet->Show();
        }

        // ��� ��� ������ ��� ���������� � ����������� ������
        /*if ( actions.size() > 0 )
        {
            TActionList* actionList;
            actionList = (TActionList*)((TAction*)(*currentSheet->actions.begin()))->GetParentComponent();

            for (int i = 0; i < actionList->ActionCount; i++)
            {
                ((TAction*)actionList->Actions[i])->Visible = false;
            }

        }*/
        // ���������� ������ ��������� ��������
        currentSheet->showActions();
        /*for (std::vector<TAction*>::iterator action = currentSheet->_actions.begin(); action != currentSheet->_actions.end(); action++)
        {
            (*action)->Visible = true;
        }*/
    }
    else
    {
        throw Exception("The tab is not accessible." );
    }
}

/* ���������� �������� */
void TModeItem::showSheet(TSheetType type)
{
    TSheetEx* sheetEx = getSheetByType(type);
    showSheet(sheetEx);
}

/* ���������� �������� �� ������� */
void TModeItem::showSheet(int index)
{
    // ��� ��������
    // ���� ������� ����������
    showSheet(&sheets[index]);
}

// ��� ������ ������� ������ ���������
typedef std::vector<TModeItem> TModeList;


//---------------------------------------------------------------------------
class TFieldActivityForm : public TForm
{
__published:	// IDE-managed Components
    TPopupMenu *DocumentsPopupMenu;
    TMenuItem *N4;
    TGroupBox *GroupBox3;
    TLabel *Label20;
    TMenuItem *N5;
    TMenuItem *DocumentStopMenuItem;
    TMenuItem *N7;
    TMenuItem *N8;
    TActionList *ActionList1;
    TAction *printDocumentFaNoticesAction;
    TAction *printDocumentFaNoticesListAction;
    TAction *printDocumentStopAction;
    TAction *printDocumentStopActionRefuseAction;
    TAction *printDocumentStopListAction;
    TAction *createFaPackAction;
    TAction *approveFaPackCcDttmAction;
    TPopupMenu *ActionsPopupMenu;
    TMenuItem *MenuItem1;
    TMenuItem *MenuItem2;
    TAction *checkAllAction;
    TAction *checkNoneAction;
    TAction *checkWithoutCcAction;
    TAction *checkWithCcLess3MonthAction;
    TMenuItem *N6;
    TAction *createFaPackStopAction;
    TMenuItem *N9;
    TActionList *ActionList2;
    TAction *Action11;
    TAction *Action12;
    TAction *Action13;
    TAction *Action14;
    TPanel *Panel1;
    TGroupBox *GroupBox1;
    TLabel *Label16;
    TLabel *Label15;
    TLabel *Label14;
    TLabel *SelectedStatLabel;
    TLabel *TotalStatLabel;
    TLabel *FilteredStatLabel;
    TGroupBox *GroupBox5;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label13;
    TLabel *Label19;
    TLabel *Label1;
    TLabel *Label7;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TBitBtn *ResetFiltersButton;
    TComboBox *PackIdFilterComboBox;
    TComboBox *AddressComboBox;
    TComboBox *ServiceCompanyFilterComboBox;
    TDateTimePicker *DateTimePicker1;
    TDateTimePicker *DateTimePicker2;
    TCheckBox *CcDttmIsNullCheckBox;
    TCheckBox *CcDttmExistsCheckBox;
    TComboBox *CityComboBox;
    TComboBox *OpAreaDescrFilterComboBox;
    TCheckBox *ccDttmIsApprovedCheckBox;
    TEditAlt *SaldoFilterEdit;
    TEditAlt *AcctIdComboBox;
    TEdit *FioComboBox;
    TGroupBox *GroupBox7;
    TLabel *Label2;
    TLabel *Label6;
    TEdit *ParamPackIdEdit;
    TDBLookupComboBox *OtdelenComboBox;
    TComboBox *SelectModeComboBox;
    TPanel *Panel2;
    TPanel *SelectAcctPopupMenuPanel;
    TBitBtn *ShowSelectAcctMenuButton;
    TCheckBox *SelectAllCheckBox;
    TBitBtn *ShowActionsMenuButton;
    TBitBtn *ShowDocumentsMenuButton;
    TButton *Button6;
    TPageControl *PackPageControl;
    TTabSheet *DebtorsTabSheet;
    TDBGridAlt *DBGridAltGeneral;
    TTabSheet *PackManualTabSheet;
    TDBGridAlt *DBGridAltManual;
    TTabSheet *ApprovalListTabSheet;
    TDBGridAlt *ApproveListGrid;
    TTabSheet *StopListTabSheet;
    TDBGridAlt *StopListGrid;
    TTabSheet *PackStopListTabSheet;
    TDBGridAlt *PackStopListGrid;
    TTabSheet *PackStopTabSheet;
    TDBGridAlt *StopPackGrid;
    TTabSheet *PackRefuseStopTabSheet;
    TTabSheet *TabSheet3;
    TTabSheet *PackReloadTabSheet;
    TPopupMenu *SelectAcctPopupMenu;
    TMenuItem *N1;
    TMenuItem *N2;
    TMenuItem *N3;
    TMenuItem *N31;
    TTabSheet *PostListTabSheet;
    TDBGridAlt *PostListGrid;
    TAction *createFaPackPostAction;
    TMenuItem *N10;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FilterControlChange(TObject *Sender);
    void __fastcall DBGridAltGeneralChangeCheck(TObject *Sender);
    void __fastcall DBGridAltGeneralChangeFilter(TObject *Sender);
    void __fastcall ParamPackIdEditClick(TObject *Sender);
    void __fastcall Button6Click(TObject *Sender);
    void __fastcall OtdelenBoxChange(TObject *Sender);
    void __fastcall ParamPackIdEditKeyPress(TObject *Sender, char &Key);
    void __fastcall FilterCcDttmChange(TObject *Sender);
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
    void __fastcall ShowDocumentsMenuButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ResetFiltersButtonClick(TObject *Sender);
    void __fastcall StopListTabSheetShow(TObject *Sender);
    void __fastcall OtdelenComboBoxClick(TObject *Sender);
    void __fastcall printDocumentFaNoticesActionExecute(TObject *Sender);
    void __fastcall printDocumentFaNoticesListActionExecute(TObject *Sender);
    void __fastcall createFaPackActionExecute(TObject *Sender);
    void __fastcall approveFaPackCcDttmActionExecute(TObject *Sender);
    void __fastcall ShowActionsMenuButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall checkWithCcLess3MonthActionExecute(TObject *Sender);
    void __fastcall checkWithoutCcActionExecute(TObject *Sender);
    void __fastcall checkAllActionExecute(TObject *Sender);
    void __fastcall ccDttmIsApprovedCheckBoxClick(TObject *Sender);
    void __fastcall DBGridAltManualCellClick(TColumn *Column);
    void __fastcall printDocumentStopActionExecute(TObject *Sender);
    void __fastcall SelectModeComboBoxChange(TObject *Sender);
    void __fastcall checkNoneActionExecute(TObject *Sender);
    void __fastcall createFaPackStopActionExecute(TObject *Sender);
    void __fastcall PackStopTabSheetShow(TObject *Sender);
    void __fastcall PackRefuseStopTabSheetShow(TObject *Sender);
    void __fastcall SelectAllCheckBoxClick(TObject *Sender);
    void __fastcall PackStopListTabSheetShow(TObject *Sender);
    void __fastcall checkAllActionUpdate(TObject *Sender);
    void __fastcall createFaPackNoticeActionUpdate(TObject *Sender);
    void __fastcall PostListTabSheetShow(TObject *Sender);
    void __fastcall createFaPackPostActionExecute(TObject *Sender);
private:	// User declarations
    //void __fastcall TFieldActivityForm::selectMode(int mode);
    void __fastcall OnQueryAfterExecute(TObject *Sender);
    void __fastcall setMode(int index);
    void __fastcall changeSheet(TSheetType type);


    TModeList _modeList;    // ������ ��������� ������� ������ �� �������� (��������: ������ � ����������� ��� ������ � �������� �� �����������)
    TModeItem* _currentMode;

    void __fastcall showDebtorList();
    void __fastcall showPostList();
    void __fastcall showFaPack(const String& faPackId);
    void __fastcall showStopList();
    void __fastcall showApprovalList();


    void __fastcall showFaList();
    void __fastcall showPackStopList(/*const String& acctOtdelen*/); // �������� ������ �������� �� �����������
    void __fastcall showFaPostList();
    void __fastcall showFaInspectorList();

    void __fastcall resetFilterControls();

    void __fastcall setCurPackMode();
    void __fastcall refreshCheckedCount();
    void __fastcall showPopupSubMenu(TWinControl *control, TPopupMenu* menu);   // ���������� Popup-���� � ������� �� ��������� � ������� control
    void __fastcall refreshParametersControls();
    void __fastcall refreshFilterControls();
    void __fastcall refreshActionsStates();


    ColorList _colorList;
    TDBGridAlt* _currentDbGrid;
    TDataSetFilter* _currentFilter;


    bool _packPageControlChangedSelf;
    void __fastcall OnThreadEnd(TObject *Sender);
    void __fastcall OnThreadBegin(TObject *Sender);
    void __fastcall OnChangeFilter(TObject *Sender);


    TThreadDataSet* tds;


    int _checkedCount;
    int _recordCount;

    //void __fastcall setCurPackId(const String& faPackId);
    //void __fastcall setCurOtdelen(const String& otdelen, bool resetPack = true);
    //void __fastcall setFilter(const String& filterName, const String& filterValue);
    //bool _packPageControlChangedSelf;

public:		// User declarations
    __fastcall TFieldActivityForm(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TFieldActivityForm *FieldActivityForm;
//---------------------------------------------------------------------------
#endif
