//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FieldActivityFormUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridAlt"
#pragma link "OtdelenComboBoxFrameUnit"
#pragma link "EditAlt"
#pragma link "MemDS"
#pragma link "VirtualTable"
#pragma link "DataSetFilter"
#pragma resource "*.dfm"
TFieldActivityForm *FieldActivityForm;
//---------------------------------------------------------------------------
__fastcall TFieldActivityForm::TFieldActivityForm(TComponent* Owner)
    : TForm(Owner)/*,
    _curPackMode(PACK_MODE_UNDEFINED)  */
{
    Caption = "ДолжникиФ (pre aplpha) - " + MainDataModule->getConfigQuery->FieldByName("username")->AsString;
    //MainDataModule->otdelenList.assignTo(OtdelenComboBox);

    // Задаем функции для связи модуля данных с графическим интерфейсом
    MainDataModule->setOpenDataSetEvents(OnThreadBegin, OnThreadEnd);

    // Окно ожидания выполнения запроса
    WaitForm = new TWaitForm(this);
    WaitForm->SetMessage("\nЗАГРУЗКА ДАННЫХ...");

    _colorList.clear();
    _colorList.addColor(static_cast<TColor>(RGB(90,225,255)));  // blue
    _colorList.addColor(static_cast<TColor>(RGB(120,230,90)));  // green
    _colorList.addColor(static_cast<TColor>(RGB(245,220,00)));  // yellow
    _colorList.addColor(static_cast<TColor>(RGB(255,180,50)));  // orange
    _colorList.addColor(static_cast<TColor>(RGB(255,130,170))); // rose
    _colorList.addColor(static_cast<TColor>(RGB(255,100,0))); // red

    //_colorList.addColor(static_cast<TColor>(RGB(180,255,20)));
    //_colorList.addColor(static_cast<TColor>(RGB(255,255,0)));
    //_colorList.addColor(static_cast<TColor>(RGB(255,255,255)));

    /* Формирование списков доступа к вкладкам */

    // Список должников
    TUserRole::TRoleTypes AccessDebtorsTabSheet;
    AccessDebtorsTabSheet<<TUserRole::OPERATOR;
    AccessDebtorsTabSheet<<TUserRole::ADMINISTRATOR;

    // Реестр уведомлений
    TUserRole::TRoleTypes AccessPackManualTabSheet;
   AccessPackManualTabSheet<<TUserRole::OPERATOR;
    AccessPackManualTabSheet<<TUserRole::ADMINISTRATOR;

    // Список на ограничение
    TUserRole::TRoleTypes AccessStopListTabSheet;
    AccessStopListTabSheet<<TUserRole::OPERATOR;
    AccessStopListTabSheet<<TUserRole::ADMINISTRATOR;

    // Список на утверждение
    TUserRole::TRoleTypes AccessApprovalListTabSheet;
    AccessApprovalListTabSheet<<TUserRole::APPROVER;
    AccessApprovalListTabSheet<<TUserRole::ADMINISTRATOR;

    // Список реестров на ограничение
    TUserRole::TRoleTypes AccessPackStopListTabSheet;
    AccessPackStopListTabSheet<<TUserRole::OPERATOR;
    AccessPackStopListTabSheet<<TUserRole::ADMINISTRATOR;

    // Реестр на ограничение
    TUserRole::TRoleTypes AccessPackStopTabSheet;
    AccessPackStopTabSheet<<TUserRole::OPERATOR;
    AccessPackStopTabSheet<<TUserRole::ADMINISTRATOR;

    // Реестр на отзыв ограничения
    TUserRole::TRoleTypes AccessPackRefuseStopTabSheet;
    AccessPackRefuseStopTabSheet<<TUserRole::OPERATOR;
    AccessPackRefuseStopTabSheet<<TUserRole::ADMINISTRATOR;

    // Реестр на возобновление
    TUserRole::TRoleTypes AccessPackReloadTabSheet;
    AccessPackReloadTabSheet<<TUserRole::OPERATOR;
    AccessPackReloadTabSheet<<TUserRole::ADMINISTRATOR;

    //AccessPackReloadTabSheet<<TUserRole::APPROVER;
    //TUserRole::TRoleTypes AccessPackRefuseStopTabSheet;
    //AccessPackRefuseStopTabSheet<<TUserRole::OPERATOR;
    //AccessPackRefuseStopTabSheet<<TUserRole::ADMINISTRATOR;


    TSheetEx* sheetTemp;
    /* Присоединяем вкладки к списку режимов работы */
    TModeItem item;
    item.caption = "Уведомления контролеру";
    item.mode = MODE_NOTICES;

    // Добавляем действия в общий список (для возможности скрыть все действия)
    for (int i = 0; i < ActionList1->ActionCount; i++)
    {
        item.addAction((TAction*)ActionList1->Actions[i]);
    }
    /*item.addAction(checkAllAction);
    item.addAction(checkNoneAction);
    item.addAction(checkWithoutCcAction);
    item.addAction(checkWithCcLess3MonthAction);
    item.addAction(createFaPackAction);
    item.addAction(printDocumentFaNoticesAction);
    item.addAction(printDocumentFaNoticesListAction);
    item.addAction(printDocumentStopAction);
    item.addAction(printDocumentStopListAction);
    item.addAction(printDocumentStopActionRefuseAction);
    item.addAction(approveFaPackCcDttmAction);
    item.addAction(createFaPackStopAction);*/



    // Вкладка списка должников
    sheetTemp = item.addSheet(DebtorsTabSheet);
    sheetTemp->accessible = !(AccessDebtorsTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_DEBTORS;
    sheetTemp->addAction(createFaPackAction);
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);

    // Вкладка реестра должников
    sheetTemp = item.addSheet(PackManualTabSheet);
    sheetTemp->accessible = !(AccessPackManualTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_NOTICES_PACK;
    sheetTemp->addAction(printDocumentFaNoticesAction);
    sheetTemp->addAction(printDocumentFaNoticesListAction);
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);

    // Вкладка утверждения вручения уведомления
    sheetTemp = item.addSheet(ApprovalListTabSheet);
    sheetTemp->accessible = !(AccessApprovalListTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_APPROVE;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);
    sheetTemp->addAction(approveFaPackCcDttmAction);

    //item.sheets.push_back( TSheetEx(DebtorsTabSheet, !(AccessDebtorsTabSheet * MainDataModule->userRole).Empty()) );
    //item.sheets.push_back( TSheetEx(PackManualTabSheet, !(AccessPackManualTabSheet * MainDataModule->userRole).Empty()) );
    //item.sheets.push_back( TSheetEx(ApprovalListTabSheet, !(AccessApprovalListTabSheet * MainDataModule->userRole).Empty()) );

    TSheetEx sheet(DebtorsTabSheet, !(AccessDebtorsTabSheet * MainDataModule->userRole).Empty());



    _modeList.push_back(item);

    item.sheets.clear();
    item.caption = "Заявки на отключение";
    item.mode = MODE_STOP;

    // Добавляем действия в общий список (для возможности скрыть все действия)
    for (int i = 0; i < ActionList1->ActionCount; i++)
    {
        item.addAction((TAction*)ActionList1->Actions[i]);
    }

    // Список кандидатов на ограничение
    sheetTemp = item.addSheet(StopListTabSheet);
    sheetTemp->accessible = !(AccessStopListTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_STOP;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);
    sheetTemp->addAction(createFaPackStopAction);

    // Список реестров на ограничение
    sheetTemp = item.addSheet(PackStopListTabSheet);
    sheetTemp->accessible = !(AccessPackStopListTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_PACK_STOP_LIST;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);
    sheetTemp->addAction(printDocumentStopAction);
    sheetTemp->addAction(printDocumentStopListAction);

    // Список реестров на ограничение
    sheetTemp = item.addSheet(PackStopTabSheet);
    sheetTemp->accessible = !(AccessPackStopTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_STOP_PACK;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);

    // Список на
    sheetTemp = item.addSheet(PackRefuseStopTabSheet);
    sheetTemp->accessible = !(AccessPackRefuseStopTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_STOP_REFUSE_PACK;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);
    sheetTemp->addAction(printDocumentStopActionRefuseAction);


    //
    sheetTemp = item.addSheet(PackReloadTabSheet);
    sheetTemp->accessible = !(AccessPackReloadTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_RELOAD;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);



    /*item.sheets.push_back( TSheetEx(StopListTabSheet, !(AccessStopListTabSheet * MainDataModule->userRole).Empty()) );
    item.sheets.push_back( TSheetEx(PackStopTabSheet, !(AccessPackStopTabSheet * MainDataModule->userRole).Empty()) );
    item.sheets.push_back( TSheetEx(PackRefuseStopTabSheet, !(AccessPackRefuseStopTabSheet * MainDataModule->userRole).Empty()) );
    item.sheets.push_back( TSheetEx(PackReloadTabSheet, !(AccessPackReloadTabSheet * MainDataModule->userRole).Empty()) );
    */
    
    _modeList.push_back(item);


    // Заполняем контрол со списком режимов
    for (TModeList::iterator it = _modeList.begin(); it != _modeList.end(); it++ )
    {
        for (std::vector<TSheetEx>::iterator sheet=it->sheets.begin(); sheet != it->sheets.end(); sheet++)
        {
            if ( sheet->accessible )
            {
                SelectModeComboBox->Items->AddObject((*it).caption, (TObject*)it);
                break;
            }

        }
    }
    SelectModeComboBox->ItemIndex = 0;

    setMode(0);     // Активируем первый доступный режим работы

}

/* Выполняется при завешении процедуры получения данных */
void __fastcall TFieldActivityForm::OnThreadEnd(TObject *Sender)
{
    WaitForm->Close();
}

/* Выполняется в начале процедуры получения данных */
void __fastcall TFieldActivityForm::OnThreadBegin(TObject *Sender)
{
    WaitForm->Execute();
}

/* Отображение формы */
void __fastcall TFieldActivityForm::FormShow(TObject *Sender)
{
    this->WindowState = wsMaximized;
    this->UpdateWindowState();
}

/* Отображает список должников
   Возможно следует удалить эту функцию
*/
void __fastcall TFieldActivityForm::showDebtorList(const String& acctOtdelen)
{
    MainDataModule->getDebtorList(acctOtdelen);
    //MainDataModule->setAcctOtdelen(acctOtdelen);

    _currentMode->showSheet(SHEET_TYPE_DEBTORS);

    //setCurOtdelen(acctOtdelen);

    refreshParametersControls();

    setCurPackMode();
}

/* Меняем отображаемую страницу */
void __fastcall TFieldActivityForm::changeSheet(TSheetType type)
{
    _currentMode->showSheet(type);  // Отображаем требуемую страницу, одновременно с этим отображаем доступные действия
}

/* Смена режима работы программы */
void __fastcall TFieldActivityForm::SelectModeComboBoxChange(
      TObject *Sender)
{
    setMode(SelectModeComboBox->ItemIndex);
}

/* Отображает реестр по ID
*/
void __fastcall TFieldActivityForm::showFaPack(const String& faPackId)
{

    // Здесь нужно переделать
    // так как уже есть два реестра Manual и Stop

    switch (_currentMode->mode)
    {
    case MODE_NOTICES:
    {
        MainDataModule->setFaPackId_Notice(faPackId);
        _currentMode->showSheet(SHEET_TYPE_NOTICES_PACK);
        break;
    }
    case MODE_STOP:
    {
        MainDataModule->setFaPackId_Stop(faPackId);
        _currentMode->showSheet(SHEET_TYPE_STOP_PACK);
        break;
    }
    }


    //_currentMode->showSheet(1);
    setCurPackMode();



    /*if (faPackId == "")
    {
        return;
    }*/
    //String acctOtdelen;
    //String faPackTypeCd;
    //MainDataModule->getFaPack(faPackId, acctOtdelen, faPackTypeCd);

    //if ( acctOtdelen != "")
    //{
    //    MainDataModule->setFaPackId(acctOtdelen, faPackId, faPackTypeCd);
    //}


    //_currentFilter->clearAllValues();
    //if (_curPackMode != PACK_MANUAL /*|| _curPackId != faPackId*/)
    //{
    //    setCurPackMode(PACK_MANUAL);
    //    _currentFilter->clearAllValues();
    //}
}

/* Отобразить список абонентов-кандидатов на ограничение*/
void __fastcall TFieldActivityForm::showStopList(const String& acctOtdelen)
{
    MainDataModule->getStopList(acctOtdelen);
    _currentMode->showSheet(SHEET_TYPE_STOP);

    //
    //setCurOtdelen(acctOtdelen);
    setCurPackMode();
}

/* Отобразить список реестров на ограничение */
void __fastcall TFieldActivityForm::showPackStopList(/*const String& acctOtdelen*/)
{
    MainDataModule->getPackStopList(MainDataModule->getAcctOtdelen());
    _currentMode->showSheet(SHEET_TYPE_PACK_STOP_LIST);

    setCurPackMode();
}

/*
*/
void __fastcall TFieldActivityForm::showFaList()
{
    /*MainDataModule->GeneralFaList->Open();
    MainDataModule->GridFaDataSource->DataSet = MainDataModule->GeneralFaList;
    _currentDbGrid->refreshFilter();

    setCurPackMode(PACK_GENERAL); */
}

/*
*/
void __fastcall TFieldActivityForm::showApprovalList(const String& acctOtdelen)
{
    MainDataModule->getApprovalList(acctOtdelen);
    _currentMode->showSheet(SHEET_TYPE_APPROVE);
    setCurPackMode();
}

/*
*/
void __fastcall TFieldActivityForm::showFaInspectorList()
{
}



/* Задает текущее отделение
*/
/*void __fastcall TFieldActivityForm::setCurOtdelen(const String& otdelen, bool resetPack)
{
    MainDataModule->setAcctOtdelen(otdelen);
    //_curPackId = "";
}  */


/* Задает текущее Pack_id
*/
/*void __fastcall TFieldActivityForm::setCurPackId(const String& faPackId)
{
    //_curPackId = faPackId;
}  */


/* Задает текущий режим
*/
void __fastcall TFieldActivityForm::setCurPackMode()
{
    //_curPackMode = packMode;
    /*if (_curPackMode != packMode)
    {
        _curPackMode = packMode;
    }
    else
    {
        return;
    } */

    //refreshControls();

    //bool checkedItems = _currentDbGrid->recordCountChecked > 0;
    //bool countItems = _currentDbGrid->recordCount > 0;

    //Image3->Visible = _curPackMode == PACK_BLOCK;
    //Image2->Visible = Image3->Visible || _curPackMode == PACK_POST;
    //Image1->Visible = Image2->Visible || _curPackMode == PACK_MANUAL;

    //BitBtn1->Visible = checkedItems;
    //GroupBox6->Enabled = checkedItems;
    //switchEnabledGroupBox(GroupBox6);
    //BitBtn2->Visible = _curPackMode == PACK_MANUAL && checkedItems;
    //BitBtn3->Visible = _curPackMode == PACK_POST && checkedItems;

/*    bool bPackGeneral = _curPackMode == PACK_GENERAL;
    bool bPackManual = _curPackMode == PACK_MANUAL;
    bool bPackStop = _curPackMode == PACK_STOP;
    bool bPackApprove = _curPackMode == PACK_APPROVE_LIST;
*/

/*    printDocumentFaNoticesAction->Enabled = bPackManual;
    printDocumentFaNoticesListAction->Enabled = bPackManual;
    printDocumentStopAction->Enabled = bPackStop;
    printDocumentStopListAction->Enabled = bPackStop;
    printDocumentStopActionRefuseAction->Enabled = bPackStop;
*/
 /*   approveFaPackCcDttmAction->Enabled = bPackApprove;
    createFaPackAction->Enabled = bPackGeneral;
*/

/*    SHEET_TYPE_UNDEFINED = 0,
    SHEET_TYPE_DEBTORS,         // general
    SHEET_TYPE_NOTICES_PACK,    //PACK_MANUAL,
    SHEET_TYPE_APPROVE,//,

    SHEET_TYPE_STOP,
    SHEET_TYPE_STOP_PACK,
    SHEET_TYPE_STOP_REFUSE_PACK,
    SHEET_TYPE_RELOAD
*/
    //TActionList


    /*for (int i = 0; i < ActionList1->ActionCount; i++)
    {
        ActionList1->Actions[i]->Visible = false;
    } */

    switch(_currentMode->currentSheet->packModeId)
//    switch(_curPackMode)
    {
    //case PACK_GENERAL:
    case SHEET_TYPE_DEBTORS:
        {
            ParamPackIdEdit->Text = "";
            ComboBox5->ItemIndex = 0;

            /*if (!DebtorsTabSheet->Visible)
            {
                DebtorsTabSheet->Show();
            }*/

            _currentFilter = MainDataModule->getDebtorsFilter;//&_filterPackGeneral;
            _currentDbGrid = DBGridAltGeneral;

            ccDttmIsApprovedCheckBox->Visible = false;

            break;
        }
    case SHEET_TYPE_NOTICES_PACK:
        {
            ParamPackIdEdit->Text = MainDataModule->getFaPackId_Notice();//_curPackId;

            /*if (!PackManualTabSheet->Visible)
            {
                PackManualTabSheet->Show();
            } */
            //PageControl1->ActivePageIndex = PackManualTabSheet->TabIndex;
            //_currentFilter = &_filterPackManual;

            _currentFilter = MainDataModule->getFaPackFilter;
            _currentDbGrid = DBGridAltManual;

            ccDttmIsApprovedCheckBox->Visible = false;

            break;
        }
     case SHEET_TYPE_APPROVE:
        {
            ParamPackIdEdit->Text = "";

            /*if ( !ApprovalListTabSheet->Visible )
            {
                ApprovalListTabSheet->Show();
            } */
            _currentFilter = MainDataModule->getApprovalListFilter;
            _currentDbGrid = ApproveListGrid;

            ccDttmIsApprovedCheckBox->Visible = true;
            break;
        }

    case SHEET_TYPE_STOP:
        {
            ParamPackIdEdit->Text = ""; //_curPackId;
            /*if ( !StopListTabSheet->Visible )
            {
                StopListTabSheet->Show();
            }*/
            //ParamPackIdEdit->Text = _curPackId;

            _currentFilter = MainDataModule->getStopListFilter;
            _currentDbGrid = StopListGrid;

            ccDttmIsApprovedCheckBox->Visible = false;
            break;
        }
    case SHEET_TYPE_STOP_PACK:
        {
            ParamPackIdEdit->Text = MainDataModule->getFaPackId_Stop();//_curPackId;
            /*if ( !StopListTabSheet->Visible )
            {
                StopListTabSheet->Show();
            }*/
            //ParamPackIdEdit->Text = _curPackId;

            _currentFilter = MainDataModule->getFaPackStopFilter;
            _currentDbGrid = StopPackGrid;

            ccDttmIsApprovedCheckBox->Visible = false;
            break;
        }
    case SHEET_TYPE_PACK_STOP_LIST:
        {
            ParamPackIdEdit->Text = "";

            _currentFilter = MainDataModule->getPackStopListFilter;
            _currentDbGrid = PackStopListGrid;
            break;
        }
    }

    refreshFilterControls();
}


/* 
*/
void __fastcall TFieldActivityForm::refreshControls()
{
    if ( _currentDbGrid != NULL )
    {
        bool checkedItems = _currentDbGrid->recordCountChecked > 0;
        bool countItems = _currentDbGrid->recordCount > 0;

        //Image3->Visible = _curPackMode == PACK_BLOCK;
        //Image2->Visible = Image3->Visible || _curPackMode == PACK_POST;
        //Image1->Visible = Image2->Visible || _curPackMode == PACK_MANUAL;
        //BitBtn1->Visible = checkedItems;
        //GroupBox6->Enabled = checkedItems;
        switchEnabledGroupBox(GroupBox6);
    }

    //BitBtn2->Visible = _curPackMode == PACK_MANUAL && checkedItems;
    //BitBtn3->Visible = _curPackMode == PACK_POST && checkedItems;

/*    switch(_curPackMode)
    {
    case PACK_GENERAL:
        {
            ParamPackIdEdit->Text = "";
            ComboBox5->ItemIndex = 0;
            ModeDebtorsButton->Down = true;
            PageControl1->ActivePageIndex = DebtorsTabSheet->TabIndex;
            break;
        }
    case PACK_MANUAL:
        {
            ParamPackIdEdit->Text = _curPackId;
            ComboBox5->ItemIndex = 1;
            ModePackManualButton->Down = true;
            PageControl1->ActivePageIndex = PackManualTabSheet->TabIndex;
            break;
        }
    case PACK_POST:
        {
            ParamPackIdEdit->Text = _curPackId;
            ComboBox5->ItemIndex = 2;
            ModePackPostButton->Down = true;
            PageControl1->ActivePageIndex = PackPostTabSheet->TabIndex;
            break;
        }
    case PACK_BLOCK:
        {
            ParamPackIdEdit->Text = _curPackId;
            ModePackBlockButton->Down = true;
            break;
        }

    case PACK_MANUAL_POST:
        {
            ParamPackIdEdit->Text = _curPackId;
            break;
        }
    }  */
}

/*
*/
void __fastcall TFieldActivityForm::FilterControlChange(TObject *Sender)
{
    TComboBox* comboBox = static_cast<TComboBox*>(Sender);
    MainDataModule->setFilterParamValue(_currentFilter, comboBox->Name, "param", comboBox->Text);
}

/*
*/
void __fastcall TFieldActivityForm::SaldoFilterEditChange(TObject *Sender)
{
    TEdit* editBox = static_cast<TEdit*>(Sender);
    MainDataModule->setFilterParamValue(_currentFilter, editBox->Name, "param", editBox->Text);
}

/*
*/
/*void __fastcall TFieldActivityForm::setFilter(const String& filterName, const String& filterValue)
{
    _currentFilter->setValue(filterName, ":param", filterValue);
    //_currentDbGrid->refreshFilter();
} */

/* Сложный фильтр с датами контакта
*/
void __fastcall TFieldActivityForm::FilterCcDttmChange(TObject *Sender)
{
    String beginDttm = DateToStr(DateTimePicker1->Date);
    String endDttm = DateToStr(DateTimePicker2->Date);

    String ccDttmIs = "";
    String ccDttm = "";

    if ( CcDttmIsNullCheckBox->Checked )
    {
        ccDttmIs = "cc_dttm is null" ;
    }

    if ( CcDttmExistsCheckBox->Checked )
    {
        ccDttm = " ( cc_dttm >= '" + beginDttm + "' and cc_dttm <= '" + endDttm + "')";
    }

    String resultCcDttm = tasktools::MergeStr(ccDttmIs, ccDttm, " OR ");
    MainDataModule->setFilterParamValue(_currentFilter, "cc_dttm", "param", resultCcDttm);

    //String s = _currentFilter->getFilterString();


    //Variant d = DateTimePicker1->Date;
    // Сохраняем значения элементов управления
    MainDataModule->setFilterParamValue(_currentFilter, "vcl_ctrl", "begin_dt", DateTimePicker1->Date);
    MainDataModule->setFilterParamValue(_currentFilter, "vcl_ctrl", "end_dt", DateTimePicker2->Date);

    // Нужно удостовериться что элементы управления в финальном положении,
    // иначе могут возникнуть коллизии
    // в случае попытки установки значения CheckBox и сработки события OnClick
    MainDataModule->setFilterParamValue(_currentFilter, "vcl_ctrl", "cc_dttm_exists", CcDttmExistsCheckBox->Checked);
    MainDataModule->setFilterParamValue(_currentFilter, "vcl_ctrl", "cc_dttm_is_null", CcDttmIsNullCheckBox->Checked);
}

/**/
void __fastcall TFieldActivityForm::ccDttmIsApprovedCheckBoxClick(
      TObject *Sender)
{
    //
    TCheckBox* checkBox = static_cast<TCheckBox*>(Sender);
    MainDataModule->setFilterParamValue(_currentFilter, checkBox->Name, "param", checkBox->Checked? " " : "");
//    String sss = _currentFilter->getFilterString();

}



/*
//---------------------------------------------------------------------------
// Заполняем строку итогов
void __fastcall TZadanie_Form::FillTotalBar()
{
    DM->ListZadanie->DisableControls();
    DM->ListZadanie->First();
    int myArr[5] = {0,0,0,0,0};
    for (;!DM->ListZadanie->Eof;DM->ListZadanie->Next()) {
        myArr[0] += DM->ListZadanie->FieldByName("kol_all")->AsInteger;
        myArr[1] += DM->ListZadanie->FieldByName("kol1")->AsInteger;
        myArr[2] += DM->ListZadanie->FieldByName("kol2")->AsInteger;
        myArr[3] += DM->ListZadanie->FieldByName("kol3")->AsInteger;
        myArr[4] += DM->ListZadanie->FieldByName("kol4")->AsInteger;
    }
    for (int i = 0; i < 5; i++) {
        HeaderControl1->Sections->Items[i]->Text = IntToStr(myArr[i]);
    }
    DM->ListZadanie->First();
    DM->ListZadanie->EnableControls();
}*/

//---------------------------------------------------------------------------
//
void __fastcall TFieldActivityForm::DBGridAltGeneralChangeCheck(TObject *Sender)
{
//
/*    Label8->Caption = IntToStr(DBGridAlt1->recordCountChecked)  + "/" +IntToStr(DBGridAlt1->recordCount);
    Label9->Caption = FloatToStr(DBGridAlt1->getSum("saldo", false, false));
    Label10->Caption = FloatToStr(DBGridAlt1->getSum("saldo", false, true));
    Label11->Caption = FloatToStr(DBGridAlt1->getSum("saldo", true, false));
    Label12->Caption = FloatToStr(DBGridAlt1->getSum("saldo", true, true));
*/

    //int checkedCount = _currentDbGrid->getSum("", "CHECK_DATA = 1", false).first;
    

    refreshControls();
}

/*
*/
void __fastcall TFieldActivityForm::DBGridAltGeneralChangeFilter(TObject *Sender)
{
    DBGridAltGeneralChangeCheck(Sender);
}

/* Отображает форму для выбора реестра
*/
void __fastcall TFieldActivityForm::ParamPackIdEditClick(TObject *Sender)
{
    String faPackTypeCd = "";
    switch(_currentMode->mode)
    {
    case MODE_NOTICES:
    {
        faPackTypeCd = "20";
        break;
    }
    case MODE_STOP:
    {
        faPackTypeCd = "40";
        break;
    }
    }

    if ( SelectFaPackForm->execute(MainDataModule->getAcctOtdelen(), faPackTypeCd) )
    {
        //FaPack faPack = SelectFaPackForm->getFaPack();
        //setCurPackId( SelectFaPackForm->getFaPackId() );
        //setCurPackMode( SelectFaPackForm->getFaPackTypeId() );
        showFaPack( SelectFaPackForm->getFaPackId() );
        refreshParametersControls();
    }

    //refreshControls();
}


/* тестовая функция */
//void __fastcall TFieldActivityForm::OnShowDebtorsThreadEnd(TDataSet* ds)
//{
//    MainDataModule->CopyDataSet(ds, MainDataModule->getDebtorsRam);
//}

/* Прячет все действия в Action листах */
/*void __fastcall TFieldActivityForm::HideAllActions()
{
    for (int i = 0; i < ActionList1->ActionCount; i++)
    {
        ((TAction*)ActionList1->Actions[i])->Visible = false;
    }
}*/

/*
*/
void __fastcall TFieldActivityForm::Button6Click(TObject *Sender)
{
    //Application->CreateForm(__classid(TWaitForm), &WaitForm);
    // Form_Wait->Label3->Caption = "Выполнение запроса...";
//    WaitForm->Execute();
    //TThreadDataSet* tds =
//    TThreadDataSet(false, MainDataModule->getDebtors, &OnShowDebtorsThreadEnd);
}

/*
*/
void __fastcall TFieldActivityForm::ParamPackIdEditKeyPress(
      TObject *Sender, char &Key)
{
    Key = '\0';
    ParamPackIdEditClick(Sender);
}

/* Отображает Popup-меню в позиции по отношению к позиции control
   В будущем перенести функцию в отдельную библиотеку
*/
void __fastcall TFieldActivityForm::showPopupSubMenu(TWinControl *control, TPopupMenu* menu)
{
    TPoint p = ClientToScreen(TPoint(control->Left, control->Top + control->Height));
    menu->Popup(p.x, p.y);
}

/* Отображает меню выбора пунктов - нажатие клавиши (на клавиатуре)*/
void __fastcall TFieldActivityForm::ShowSelectAcctMenuButtonKeyPress(
      TObject *Sender, char &Key)
{
    showPopupSubMenu(dynamic_cast<TWinControl*>(SelectAcctPopupMenuPanel), SelectAcctPopupMenu);
}

/* Отображает меню выбора пунктов - нажатие кнопки (на форме) */
void __fastcall TFieldActivityForm::ShowSelectAcctMenuButtonMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    showPopupSubMenu(dynamic_cast<TWinControl*>(SelectAcctPopupMenuPanel), SelectAcctPopupMenu);
}

/* Отображает меню выбора документа - нажатие кнопки (на форме) */
void __fastcall TFieldActivityForm::ShowDocumentsMenuButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    showPopupSubMenu(dynamic_cast<TWinControl*>(Sender), DocumentsPopupMenu);
}

/* Отображает меню выбора действия - нажатие кнопки (на форме) */
void __fastcall TFieldActivityForm::ShowActionsMenuButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    showPopupSubMenu(dynamic_cast<TWinControl*>(Sender), ActionsPopupMenu);
}


/**/
int PageIndexFromTabIndex(TPageControl* pageControl, int tabIndex)
{
    /*int visiblePageIndex = 0;
    while (tabIndex > 0 )
    {
        if (pageControl->Pages[i]->Showing)
        {
            tabIndex--;
        }
    } */

    int visiblePageCount = 0;
    for (int i = 0; i <= pageControl->PageCount; i++)
    {
        if ( pageControl->Pages[i]->TabVisible )
        {
            visiblePageCount++;
        }
        if (visiblePageCount > tabIndex)
        {
            return i;
        }
    }
    return -1;
}

/**/
void __fastcall TFieldActivityForm::PackPageControlDrawTab(
      TCustomTabControl *Control, int TabIndex, const TRect &Rect,
      bool Active)
{
    /*if ( !((TPageControl*)Control)->Pages[TabIndex]->TabVisible )
    {
        return;
    }*/

    //TPageControl *pageControl = static_cast <TTabControl *> (Control);
    TCanvas *canvas = Control->Canvas;
    // Текст, который будем выводить на вкладке

    //int pageIndex = TabIndex;
    int pageIndex = PageIndexFromTabIndex(((TPageControl*)Control), TabIndex);

    //((TPageControl*)Control)->Pages->TabIndex
    //int n = ((TPageControl*)Control)->Pages->PageIndex;
    //int n = ((TPageControl*)Control)->Pages->TabIndex(TabIndex);
    //int t = ((TPageControl*)Control)->TabIndex;
    AnsiString tabCaption = ((TPageControl*)Control)->Pages[pageIndex]->Caption;

    // РИСОВАНИЕ
    canvas->Lock();    // Блокирум канвас перед рисованием
    //canvas->TextRect(Rect, Rect.Left+3, Rect.Top+3, tabCaption);
    canvas->Brush->Color = _colorList.getColorByIndex(pageIndex);
    canvas->FillRect(TRect(Rect.Left, Rect.Top + Rect.Height()-3, Rect.Left + Rect.Width(), Rect.Top + Rect.Height()));

    canvas->Brush->Color = clBtnFace;
    canvas->Font->Color = static_cast<TColor>(RGB(0,0,0));    // Цвет шрифта
    canvas->TextOutA(Rect.Left+3, Rect.Top+3, tabCaption);
    canvas->Unlock();
}

/* Должники */
void __fastcall TFieldActivityForm::DebtorsTabSheetShow(TObject *Sender)
{
    showDebtorList( MainDataModule->getAcctOtdelen() );
}

/* Переключили на вкладку Уведомления */
void __fastcall TFieldActivityForm::PackManualTabSheetShow(TObject *Sender)
{
    // возможно переделать на show...
    showFaPack(MainDataModule->getFaPackId_Notice());
}

/**/
void __fastcall TFieldActivityForm::StopListTabSheetShow(TObject *Sender)
{
    // возможно переделать на show...
    showStopList( MainDataModule->getAcctOtdelen() );
}

/* Переключили на вкладку Утверждения */
void __fastcall TFieldActivityForm::ApprovalListTabSheetShow(
      TObject *Sender)
{
    //setCurPackMode(PACK_APPROVE_LIST);
    // возможно переделать на show...
    showApprovalList( MainDataModule->getAcctOtdelen() );
}

/**/
void __fastcall TFieldActivityForm::ResetFiltersButtonClick(
      TObject *Sender)
{
    _currentFilter->clearAllValues();
    refreshFilterControls();
}

void __fastcall TFieldActivityForm::refreshParametersControls()
{
    OtdelenComboBox->KeyValue = MainDataModule->getAcctOtdelen();
}

void __fastcall TFieldActivityForm::refreshActionsStates()
{
    
}




void __fastcall TFieldActivityForm::refreshFilterControls()
{
    // Видимость
    AcctIdComboBox->Enabled = _currentFilter->isFilterExists(AcctIdComboBox->Name);
    CityComboBox->Enabled = _currentFilter->isFilterExists(CityComboBox->Name);
    AddressComboBox->Enabled = _currentFilter->isFilterExists(AddressComboBox->Name);
    FioComboBox->Enabled = _currentFilter->isFilterExists(FioComboBox->Name);
    PackIdFilterComboBox->Enabled = _currentFilter->isFilterExists(PackIdFilterComboBox->Name);
    ServiceCompanyFilterComboBox->Enabled = _currentFilter->isFilterExists(ServiceCompanyFilterComboBox->Name);
    SaldoFilterEdit->Enabled = _currentFilter->isFilterExists(SaldoFilterEdit->Name);
    DateTimePicker1->Enabled = _currentFilter->isFilterExists("cc_dttm_is");
    DateTimePicker2->Enabled = _currentFilter->isFilterExists("cc_dttm_is");
    CcDttmExistsCheckBox->Enabled = _currentFilter->isFilterExists("cc_dttm_is");
    CcDttmIsNullCheckBox->Enabled = _currentFilter->isFilterExists("cc_dttm_is");
    OpAreaDescrFilterComboBox->Enabled = _currentFilter->isFilterExists(OpAreaDescrFilterComboBox->Name);

    // Восстановление значений
    AcctIdComboBox->Text = _currentFilter->getValue(AcctIdComboBox->Name, "param");
    CityComboBox->Text = _currentFilter->getValue(CityComboBox->Name, "param");
    AddressComboBox->Text = _currentFilter->getValue(AddressComboBox->Name, "param");
    FioComboBox->Text = _currentFilter->getValue(FioComboBox->Name, "param");
    PackIdFilterComboBox->Text = _currentFilter->getValue(PackIdFilterComboBox->Name, "param");
    ServiceCompanyFilterComboBox->Text = _currentFilter->getValue(ServiceCompanyFilterComboBox->Name, "param");
    SaldoFilterEdit->Text = _currentFilter->getValue(SaldoFilterEdit->Name, "param");
    OpAreaDescrFilterComboBox->Text = _currentFilter->getValue(OpAreaDescrFilterComboBox->Name, "param");

    // Восстановление значений сложного фильтра с датами
    DateTimePicker1->Date = _currentFilter->getValue("vcl_ctrl", "begin_dt");
    DateTimePicker2->Date = _currentFilter->getValue("vcl_ctrl", "end_dt");
    CcDttmExistsCheckBox->OnClick = NULL;
    CcDttmIsNullCheckBox->OnClick = NULL;
    CcDttmExistsCheckBox->Checked = _currentFilter->getValue("vcl_ctrl", "cc_dttm_exists");
    CcDttmIsNullCheckBox->Checked = _currentFilter->getValue("vcl_ctrl", "cc_dttm_is_null");
    CcDttmExistsCheckBox->OnClick = FilterCcDttmChange;
    CcDttmIsNullCheckBox->OnClick = FilterCcDttmChange;

}

/* Реакция на выбор учатка из списка ComboBox */
void __fastcall TFieldActivityForm::OtdelenComboBoxClick(TObject *Sender)
{
    MainDataModule->setAcctOtdelen(OtdelenComboBox->KeyValue);  // Задаем текущий филиал (участок)
    switch(_currentMode->currentSheet->packModeId)
    {
    case SHEET_TYPE_DEBTORS:
    {
        showDebtorList(OtdelenComboBox->KeyValue);
        break;
    }
    case SHEET_TYPE_STOP:
    {
        showStopList(OtdelenComboBox->KeyValue);
        break;
    }
    case SHEET_TYPE_PACK_STOP_LIST:
    {
        showPackStopList();
        break;
    }
    default:
    {
        switch(_currentMode->mode)
        {
        case MODE_NOTICES:
        {
            showDebtorList(OtdelenComboBox->KeyValue);
            break;
        }

        case MODE_STOP:
        {
            showStopList(OtdelenComboBox->KeyValue);
            break;
        }
        }
    }
    }
}

/**/
void __fastcall TFieldActivityForm::OtdelenBoxChange(TObject *Sender)
{
    //MainDataModule->getAcctOtdelen()
    //_curOtdelen = MainDataModule->otdelenList.getValue(OtdelenComboBox);
    //refreshData();
}

/**/
void __fastcall TFieldActivityForm::printDocumentFaNoticesActionExecute(
      TObject *Sender)
{
    DocumentDataModule->getDocumentFaNotices(_currentFilter);
}


/**/
void __fastcall TFieldActivityForm::printDocumentFaNoticesListActionExecute(TObject *Sender)
{
    DocumentDataModule->getDocumentFaNoticesList(_currentFilter);

}


/**/
void __fastcall TFieldActivityForm::createFaPackActionExecute(TObject *Sender)
{
    String faPackId = MainDataModule->createPackNotice();
    //String faPackId = MainDataModule->createPack(_currentFilter, TPackTypeCd::PACK_MANUAL, MainDataModule->getAcctOtdelen());
    showFaPack(faPackId);
}

/**/
void __fastcall TFieldActivityForm::approveFaPackCcDttmActionExecute(TObject *Sender)
{
    if (MessageBoxQuestion("Отмечено " + IntToStr(ApproveListGrid->recordCountChecked) + " записей."
        "\nУже утвержденные контакты будут пропущены."
        "\nПродолжить?") != IDNO )
    {
        MainDataModule->setCcApprovalDttmSelected();
    }
    
}

/* Выделить с датой контакта более 3 месяцев*/
void __fastcall TFieldActivityForm::checkWithCcLess3MonthActionExecute(
      TObject *Sender)
{
    MainDataModule->selectCcDttmMoreThanThree(_currentFilter);
}

/* Выделить без даты контакта*/
void __fastcall TFieldActivityForm::checkWithoutCcActionExecute(
      TObject *Sender)
{
    MainDataModule->selectCcDttmIsNull(_currentFilter);

}

/* Пометить отфильтрованные */
void __fastcall TFieldActivityForm::checkAllActionExecute(TObject *Sender)
{
    _currentDbGrid->setCheckFiltered(true);
    SelectAllCheckBox->Checked = true;
}

/* Снять пометки */
void __fastcall TFieldActivityForm::checkNoneActionExecute(TObject *Sender)
{
    _currentDbGrid->setCheckFiltered(false);
    SelectAllCheckBox->Checked = false;
}


/* Отображает окно создания/редактирования контакта */
void __fastcall TFieldActivityForm::DBGridAltManualCellClick(
      TColumn *Column)
{
    if (Column->FieldName == "CC_DTTM")
    {
        EditCcForm->Execute(_currentDbGrid->DataSource->DataSet);
    }
}

/* Обновление при обращении */
void __fastcall TFieldActivityForm::ActionList1Update(TBasicAction *Action,
      bool &Handled)
{

    //((TAction*)ActionList1->Actions[0])->Visible = false;
    //((TAction*)Action)->Visible = false;
    //std::vector<TAction*>::iterator it;
   // it = find(_currentMode->currentSheet->actions.begin(), _currentMode->currentSheet->actions.end(), Action);

    /*if (it != _currentMode->currentSheet->actions.end())
    {
        Action->en

        //std::cout << "Element found in myvector: " << *it << '\n';
    }
    else
    {
        //std::cout << "Element not found in myvector\n";
    }*/




    //Временно закоментированно 2017-02-14


    if (Action->Tag != 1) // Для обработки Update 1 раз
    {
        return;
    }


    int checkedCount = _currentDbGrid->getSum("", "CHECK_DATA = 1", false).first;
    bool bChecked = checkedCount > 0;

    switch(_currentMode->currentSheet->packModeId)
    {
    case SHEET_TYPE_DEBTORS:
    {
        checkAllAction->Enabled = true;
        checkNoneAction->Enabled = true;
        checkWithoutCcAction->Enabled  = true;
        checkWithCcLess3MonthAction->Enabled = true;

        createFaPackAction->Enabled = bChecked;
        break;
    }
    case SHEET_TYPE_NOTICES_PACK:
    {
        checkAllAction->Enabled = true;
        checkNoneAction->Enabled = true;
        checkWithoutCcAction->Enabled  = true;
        checkWithCcLess3MonthAction->Enabled = true;

        printDocumentFaNoticesAction->Enabled = bChecked;
        printDocumentFaNoticesListAction->Enabled = bChecked;
        break;
    }
    case SHEET_TYPE_APPROVE:
    {
        checkAllAction->Enabled = true;
        checkNoneAction->Enabled = true;
        checkWithoutCcAction->Enabled  = true;
        checkWithCcLess3MonthAction->Enabled = true;

        approveFaPackCcDttmAction->Enabled = bChecked;
        break;
    }
        case SHEET_TYPE_STOP:
    {

        checkAllAction->Enabled = true;
        checkNoneAction->Enabled = true;
        checkWithoutCcAction->Enabled  = true;
        checkWithCcLess3MonthAction->Enabled = true;

        createFaPackStopAction->Enabled = true;

        break;
    }
    case SHEET_TYPE_PACK_STOP_LIST:
    {
        checkAllAction->Enabled = true;
        checkNoneAction->Enabled = true;
        checkWithoutCcAction->Enabled  = true;
        checkWithCcLess3MonthAction->Enabled = true;

        printDocumentStopAction->Enabled = bChecked;

        break;
    }
    case SHEET_TYPE_STOP_PACK:
    {

        checkAllAction->Enabled = true;
        checkNoneAction->Enabled = true;
        checkWithoutCcAction->Enabled  = true;
        checkWithCcLess3MonthAction->Enabled = true;

        printDocumentStopAction->Enabled = bChecked;

        break;
    }
    }

    //Handled = true;
}

/* Печать заявок на ограничение */
void __fastcall TFieldActivityForm::printDocumentStopActionExecute(
      TObject *Sender)
{
    DocumentDataModule->getDocumentStopService();
}

/* Задает текущий режим работы программы */
void __fastcall TFieldActivityForm::setMode(int index)
{

    PackPageControl->Visible = false; // В будущем попробовать с отключением OnChange

    // Сначала прячем вообще все вкладки
    for (int i = 0; i < PackPageControl->PageCount; i++ )
    {
        PackPageControl->Pages[i]->TabVisible = false;
    }

    // Отображаем только вкладки активного режима и доступные текущему пользователю по его роли
    _currentMode = (TModeItem*)SelectModeComboBox->Items->Objects[index];
    _currentMode->showSheets();
    _currentMode->showSheet(0);

    //_currentMode->currentSheet



    PackPageControl->Visible = true;
    //PackPageControl->Refresh();
    //Application->ProcessMessages();
}

/* Создать реестр заявок на ограничение*/
void __fastcall TFieldActivityForm::createFaPackStopActionExecute(
      TObject *Sender)
{
    String faPackId = MainDataModule->createPackStop();
    //String faPackId = MainDataModule->createPack(_currentFilter, TPackTypeCd::PACK_STOP, MainDataModule->getAcctOtdelen());
    //showFaPack(faPackId);
    showPackStopList();

}
//---------------------------------------------------------------------------


void __fastcall TFieldActivityForm::PackStopTabSheetShow(
      TObject *Sender)
{
    showFaPack(MainDataModule->getFaPackId_Stop());

}


void __fastcall TFieldActivityForm::PackRefuseStopTabSheetShow(
      TObject *Sender)
{
    //showFaPack(MainDataModule->getFaPackId_Stop());

}
//---------------------------------------------------------------------------

void __fastcall TFieldActivityForm::SelectAllCheckBoxClick(TObject *Sender)
{
    if (  ((TCheckBox*)Sender)->Checked )
    {
        checkAllAction->Execute();
    }
    else
    {
        checkNoneAction->Execute();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFieldActivityForm::PackStopListTabSheetShow(
      TObject *Sender)
{
    // возможно переделать на show...
    showPackStopList( /*MainDataModule->getAcctOtdelen()*/ );

}
//---------------------------------------------------------------------------

void __fastcall TFieldActivityForm::OnQueryAfterExecute(TObject *Sender)
{
    //WaitForm->Close();
}



