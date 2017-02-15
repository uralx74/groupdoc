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
    SHEET_TYPE_APPROVE,//,

    SHEET_TYPE_STOP,
    SHEET_TYPE_PACK_STOP_LIST,  // Список реестров на отключение
    SHEET_TYPE_STOP_PACK,
    SHEET_TYPE_STOP_REFUSE_PACK,
    SHEET_TYPE_RELOAD
    //PACK_APPROVE_LIST
} TSheetType;


/* Класс для хранения расширеной обертки над TTabSheet */
class TSheetEx
{
private:
    std::vector<TAction*> _actions; // Список доступных действий

public:
    TSheetEx(TTabSheet* _sheet, bool _accessible);
    TTabSheet* sheet;   // Указатель на вкладку
    bool accessible;    // Признак доступности пользователю

    //void addAction(TAction* action);
    // std::vector<Action*> filters; // Список доступных фильтров
    //void assignMode(PackMode packModeId);
    void assignSheet(TTabSheet* sheet, bool accessible = false);
    void addAction(TAction* action, bool accessible = false);
    void showActions();

    TSheetType packModeId;    // Спорный вопрос, хранить здесь или нет!!!!!!!!!! Наверное да!
};

/* Конструктор */
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

/**/
/*void TSheetEx::assignMode(TSheetType packModeId)
{
    this->packModeId = packModeId;
}
  */
/* Добавляет действие в список доступных дейсвия для листа*/
void TSheetEx::addAction(TAction* action, bool accessible)
{
    _actions.push_back(action);
}

/* Отображает доступные действия */
void TSheetEx::showActions()
{
    for (std::vector<TAction*>::iterator action = _actions.begin(); action != _actions.end(); action++)
    {
        (*action)->Visible = true;
    }
}



/* Класс для хранения элемента(раздела),
   представляющего режим работы программы */
class TModeItem
{
private:
    void hideAllActions();
    std::vector<TAction*> _actions; // Список всех действий (для того чтобы можно было скрыть все действия перед отображением листа)

public:
    TModeItem::TModeItem();
    String caption;     // Наименование режима
    //String modeType;    // Возможно что равно pack_type_cd
    TMode mode;
    std::vector<TSheetEx> sheets;   // Список вкладок(подразделов)
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

/* Добавляет действие в общий список */
void TModeItem::addAction(TAction* action)
{
    _actions.push_back(action);
}


TSheetEx* TModeItem::addSheet(TTabSheet* _sheet, bool _accessible)
{
    sheets.push_back(TSheetEx(_sheet, _accessible));
    return &sheets.back();
}

/* Отображает доступные вкладки */
void TModeItem::showSheets()
{
    // Отображаем только вкладки активного режима и доступные текущему пользователю по его роли
    for (std::vector<TSheetEx>::iterator sheet = sheets.begin(); sheet != sheets.end(); sheet++)
    {
        sheet->sheet->TabVisible = sheet->accessible;
    }
}

TSheetEx* TModeItem::getSheetByType(TSheetType type)
{
    for(std::vector<TSheetEx>::iterator sheet = sheets.begin(); sheet != sheets.end(); sheet++)
    {
        if (sheet->packModeId == type)
        {
            return sheet;
        }
    }

}

/* Прячет все действия в Action листах */
void TModeItem::hideAllActions()
{
    for (std::vector<TAction*>::iterator action = _actions.begin(); action != _actions.end(); action++)
    {
        (*action)->Visible = false;
    }
}

/* Отображаем лист */
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

        // Это или убрать или обьединить с последующим циклом
        /*if ( actions.size() > 0 )
        {
            TActionList* actionList;
            actionList = (TActionList*)((TAction*)(*currentSheet->actions.begin()))->GetParentComponent();

            for (int i = 0; i < actionList->ActionCount; i++)
            {
                ((TAction*)actionList->Actions[i])->Visible = false;
            }

        }*/
        // Отображаем только доступные действия
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

/**/
void TModeItem::showSheet(TSheetType type)
{
    TSheetEx* sheetEx = getSheetByType(type);
    showSheet(sheetEx);
}


void TModeItem::showSheet(int index)
{
    // Тут доделать
    // если вкладка недоступна
    showSheet(&sheets[index]);
}

// Тип списка режимов работы программы
typedef std::vector<TModeItem> TModeList;


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
    TButton *Button6;
    TPageControl *PackPageControl;
    TTabSheet *DebtorsTabSheet;
    TTabSheet *PackManualTabSheet;
    TTabSheet *StopListTabSheet;
    TBitBtn *ShowDocumentsMenuButton;
    TPopupMenu *DocumentsPopupMenu;
    TMenuItem *N4;
    TCheckBox *CcDttmExistsCheckBox;
    TGroupBox *GroupBox3;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TComboBox *CityComboBox;
    TComboBox *OpAreaDescrFilterComboBox;
    TLabel *Label23;
    TBitBtn *BitBtn6;
    TMenuItem *N5;
    TTabSheet *ApprovalListTabSheet;
    TDBGridAlt *DBGridAltManual;
    TDBGridAlt *DBGridAltGeneral;
    TDBGridAlt *ApproveListGrid;
    TCheckBox *ccDttmIsApprovedCheckBox;
    TMenuItem *DocumentStopMenuItem;
    TMenuItem *N7;
    TMenuItem *N8;
    TDBGridAlt *StopListGrid;
    TDBLookupComboBox *OtdelenComboBox;
    TActionList *ActionList1;
    TAction *printDocumentFaNoticesAction;
    TAction *printDocumentFaNoticesListAction;
    TAction *printDocumentStopAction;
    TAction *printDocumentStopActionRefuseAction;
    TAction *printDocumentStopListAction;
    TAction *createFaPackAction;
    TAction *approveFaPackCcDttmAction;
    TBitBtn *ShowActionsMenuButton;
    TPopupMenu *ActionsPopupMenu;
    TMenuItem *MenuItem1;
    TMenuItem *MenuItem2;
    TDBGrid *DBGrid1;
    TAction *checkAllAction;
    TAction *checkNoneAction;
    TAction *checkWithoutCcAction;
    TAction *checkWithCcLess3MonthAction;
    TEditAlt *SaldoFilterEdit;
    TMenuItem *N6;
    TTabSheet *PackStopTabSheet;
    TTabSheet *PackRefuseStopTabSheet;
    TTabSheet *TabSheet3;
    TTabSheet *PackReloadTabSheet;
    TComboBox *SelectModeComboBox;
    TAction *createFaPackStopAction;
    TMenuItem *N9;
    TDBGridAlt *StopPackGrid;
    TLabel *Label24;
    TEdit *Edit1;
    TActionList *ActionList2;
    TAction *Action11;
    TAction *Action12;
    TAction *Action13;
    TAction *Action14;
    TBitBtn *ShowSelectAcctMenuButton;
    TCheckBox *SelectAllCheckBox;
    TPanel *SelectAcctPopupMenuPanel;
    TTabSheet *PackStopListTabSheet;
    TDBGridAlt *PackStopListGrid;
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
    void __fastcall ActionList1Update(TBasicAction *Action, bool &Handled);
    void __fastcall printDocumentStopActionExecute(TObject *Sender);
    void __fastcall SelectModeComboBoxChange(TObject *Sender);
    void __fastcall checkNoneActionExecute(TObject *Sender);
    void __fastcall createFaPackStopActionExecute(TObject *Sender);
    void __fastcall PackStopTabSheetShow(TObject *Sender);
    void __fastcall PackRefuseStopTabSheetShow(TObject *Sender);
    void __fastcall SelectAllCheckBoxClick(TObject *Sender);
    void __fastcall PackStopListTabSheetShow(TObject *Sender);
private:	// User declarations
    //void __fastcall TFieldActivityForm::selectMode(int mode);
    void __fastcall OnQueryAfterExecute(TObject *Sender);
    void __fastcall setMode(int index);
    //void __fastcall HideAllActions();   // Прячет все действия
    void __fastcall changeSheet(TSheetType type);

    //typedef pair<String, int> TModeItem;    // Для хранения элемента списка
    //typedef std::vector<TModeItem> TModeList;   // Для хранения списка


    TModeList _modeList;    // Список доступных режимов работы со списками (например: Работа с уведомления или Работа с заявками на ограничение)
    //_TModeList

    TModeItem* _currentMode;

    void __fastcall showDebtorList(const String& acctOtdelen);
    void __fastcall showFaPack(const String& faPackId);
    void __fastcall showStopList(const String& acctOtdelen);
    void __fastcall showApprovalList(const String& acctOtdelen);


    void __fastcall showFaList();
    void __fastcall showPackStopList(/*const String& acctOtdelen*/); // Показать список реестров на ограничение
    void __fastcall showFaPostList();
    void __fastcall showFaInspectorList();

    void __fastcall resetFilterControls();

    //void __fastcall setCurOtdelen(const String& otdelen, bool resetPack = true);
    void __fastcall setCurPackMode();
    //void __fastcall setCurPackId(const String& faPackId);
    void __fastcall refreshControls();
    void __fastcall showPopupSubMenu(TWinControl *control, TPopupMenu* menu);   // Отображает Popup-меню в позиции по отношению к позиции control
    void __fastcall refreshParametersControls();
    void __fastcall refreshFilterControls();
    void __fastcall refreshActionsStates();


    //void __fastcall setFilter(const String& filterName, const String& filterValue);
    ColorList _colorList;


    TDBGridAlt* _currentDbGrid;

    TDataSetFilter* _currentFilter;
    //TDataSetFilter _filterPackGeneral;   // Фильтр
    //TDataSetFilter _filterPackManual;   // Фильтр
    //TDataSetFilter _filterApproveList;   // Фильтр

    //TSheetType _curPackMode;  // Текущий режим
    //String _curPackId;      // Текущая пачка
    //String _curOtdelen;     // Текущий участок

    bool _packPageControlChangedSelf;
    /**/
    //bool _packPageControlChangedSelf;




    void __fastcall OnThreadEnd(TObject *Sender);
    void __fastcall OnThreadBegin(TObject *Sender);

    TThreadDataSet* tds;


public:		// User declarations
    __fastcall TFieldActivityForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFieldActivityForm *FieldActivityForm;
//---------------------------------------------------------------------------
#endif
