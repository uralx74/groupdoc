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
    MainDataModule->setOnChangeFilterMethod(OnChangeFilter);

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
    TUserRole::TRoleTypes Access_DebtorsTabSheet;
    Access_DebtorsTabSheet<<TUserRole::OPERATOR;
    Access_DebtorsTabSheet<<TUserRole::ADMINISTRATOR;

    // Реестр уведомлений
    TUserRole::TRoleTypes Access_PackManualTabSheet;
    Access_PackManualTabSheet<<TUserRole::OPERATOR;
    Access_PackManualTabSheet<<TUserRole::ADMINISTRATOR;

    // Список на утверждение
    TUserRole::TRoleTypes Access_ApprovalListTabSheet;
    Access_ApprovalListTabSheet<<TUserRole::APPROVER;
    Access_ApprovalListTabSheet<<TUserRole::ADMINISTRATOR;

    // Список на почту
    TUserRole::TRoleTypes Access_PostListTabSheet;
    Access_PostListTabSheet<<TUserRole::OPERATOR;
    Access_PostListTabSheet<<TUserRole::ADMINISTRATOR;


    // Список на ограничение
    TUserRole::TRoleTypes AccessStopListTabSheet;
    AccessStopListTabSheet<<TUserRole::OPERATOR;
    AccessStopListTabSheet<<TUserRole::ADMINISTRATOR;

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
    sheetTemp->accessible = !(Access_DebtorsTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_DEBTORS;
    sheetTemp->addAction(createFaPackAction);
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);

    // Вкладка реестра должников
    sheetTemp = item.addSheet(PackManualTabSheet);
    sheetTemp->accessible = !(Access_PackManualTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_NOTICES_PACK;
    sheetTemp->addAction(printDocumentFaNoticesAction);
    sheetTemp->addAction(printDocumentFaNoticesListAction);
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);

    // Вкладка утверждения вручения уведомления
    sheetTemp = item.addSheet(ApprovalListTabSheet);
    sheetTemp->accessible = !(Access_ApprovalListTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_APPROVE;
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);
    sheetTemp->addAction(approveFaPackCcDttmAction);

    // Вкладка списка уведомлений на почту
    sheetTemp = item.addSheet(PostListTabSheet);
    sheetTemp->accessible = !(Access_PostListTabSheet * MainDataModule->userRole).Empty();
    sheetTemp->packModeId = SHEET_TYPE_POST_LIST;
    sheetTemp->addAction(createFaPackAction);
    sheetTemp->addAction(createFaPackPostAction);
    sheetTemp->addAction(checkAllAction);
    sheetTemp->addAction(checkNoneAction);
    sheetTemp->addAction(checkWithoutCcAction);
    sheetTemp->addAction(checkWithCcLess3MonthAction);

    //item.sheets.push_back( TSheetEx(DebtorsTabSheet, !(AccessDebtorsTabSheet * MainDataModule->userRole).Empty()) );
    //item.sheets.push_back( TSheetEx(PackManualTabSheet, !(AccessPackManualTabSheet * MainDataModule->userRole).Empty()) );
    //item.sheets.push_back( TSheetEx(ApprovalListTabSheet, !(AccessApprovalListTabSheet * MainDataModule->userRole).Empty()) );

    // Закомментировано 2017-02-22
    //TSheetEx sheet(DebtorsTabSheet, !(Access_DebtorsTabSheet * MainDataModule->userRole).Empty());



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

    refreshParametersControls();

    setMode(0);     // Активируем первый доступный режим работы
}

/* Выполняется при завешении процедуры получения данных */
void __fastcall TFieldActivityForm::OnThreadEnd(TObject *Sender)
{
    WaitForm->StopWait();
    refreshCheckedCount();
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
void __fastcall TFieldActivityForm::showDebtorList()
{
    MainDataModule->getDebtorList();
    _currentMode->showSheet(SHEET_TYPE_DEBTORS);
    setCurPackMode();
}

void __fastcall TFieldActivityForm::showPostList()
{
    MainDataModule->getPostList();
    _currentMode->showSheet(SHEET_TYPE_POST_LIST);
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

    refreshParametersControls();

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
void __fastcall TFieldActivityForm::showStopList()
{
    MainDataModule->getStopList();
    _currentMode->showSheet(SHEET_TYPE_STOP);

    //
    //setCurOtdelen(acctOtdelen);
    setCurPackMode();
}

/* Отобразить список реестров на ограничение */
void __fastcall TFieldActivityForm::showPackStopList(/*const String& acctOtdelen*/)
{
    MainDataModule->getPackStopList();
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
void __fastcall TFieldActivityForm::showApprovalList()
{
    MainDataModule->getApprovalList();
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
    switch(_currentMode->currentSheet->packModeId)
    {
    case SHEET_TYPE_DEBTORS:
    {
        ParamPackIdEdit->Text = "";

        _currentFilter = MainDataModule->getDebtorsFilter;//&_filterPackGeneral;
        _currentDbGrid = DBGridAltGeneral;

        ccDttmIsApprovedCheckBox->Visible = false;

        break;
    }
    case SHEET_TYPE_NOTICES_PACK:
    {
        ParamPackIdEdit->Text = MainDataModule->getFaPackId_Notice();//_curPackId;

        _currentFilter = MainDataModule->getFaPackFilter;
        _currentDbGrid = DBGridAltManual;

        ccDttmIsApprovedCheckBox->Visible = false;

        break;
    }
    case SHEET_TYPE_APPROVE:
    {
        ParamPackIdEdit->Text = "";

        _currentFilter = MainDataModule->getApprovalListFilter;
        _currentDbGrid = ApproveListGrid;

        ccDttmIsApprovedCheckBox->Visible = true;
        break;
    }
    case SHEET_TYPE_POST_LIST:
    {                        // Список на почту
        ParamPackIdEdit->Text = "";

        _currentFilter = MainDataModule->getPostListFilter;
        _currentDbGrid = PostListGrid;

        break;
    }
    case SHEET_TYPE_STOP:
    {
        ParamPackIdEdit->Text = "";

        _currentFilter = MainDataModule->getStopListFilter;
        _currentDbGrid = StopListGrid;

        //ccDttmIsApprovedCheckBox->Visible = false;
        break;
    }
    case SHEET_TYPE_STOP_PACK:
    {
        ParamPackIdEdit->Text = MainDataModule->getFaPackId_Stop();

        _currentFilter = MainDataModule->getFaPackStopFilter;
        _currentDbGrid = StopPackGrid;

        //ccDttmIsApprovedCheckBox->Visible = false;
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

    refreshFilterControls();    // Обновляем состояние элементов управления для фильтров
    refreshCheckedCount();  //
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
    //int checkedCount = _currentDbGrid->getSum("", "CHECK_DATA = 1", false).first;
    refreshCheckedCount();
}

/* Обновляет статистику по выделенным элементам в dbgrid */
void __fastcall TFieldActivityForm::refreshCheckedCount()
{
    if ( _currentDbGrid != NULL )
    {
        TSumResult checkedResult = _currentDbGrid->getSum("", "CHECK_DATA = 1", false);
        TSumResult recordResult = _currentDbGrid->getSum("", "", false);
        TSumResult recordFilteredResult = _currentDbGrid->getSum("", "", true);

        _checkedCount = checkedResult.first;
        _recordCount = recordResult.first;

        SelectedStatLabel->Caption = IntToStr(checkedResult.first);
        FilteredStatLabel->Caption = IntToStr(recordFilteredResult.first);
        TotalStatLabel->Caption = IntToStr(recordResult.first);


        // Состояние CheckBox для выделения записей
        TNotifyEvent old_OnClick = SelectAllCheckBox->OnClick;
        SelectAllCheckBox->OnClick = NULL;
        if (_recordCount > 0)
        {
            if (_checkedCount == _recordCount)
            {
                SelectAllCheckBox->State = cbChecked;
            }
            else if (_checkedCount > 0)
            {
                SelectAllCheckBox->State = cbGrayed;
            }
            else
            {
                SelectAllCheckBox->State = cbUnchecked;
            }
        }
        else
        {
            SelectAllCheckBox->State = cbUnchecked;
        }
        SelectAllCheckBox->OnClick = old_OnClick;


        /*Label8->Caption = IntToStr(checkedResult.first)  + "/" +IntToStr(recordResult.first);
        /*Label9->Caption = FloatToStr(DBGridAlt1->getSum("saldo", false, false));
        Label10->Caption = FloatToStr(DBGridAlt1->getSum("saldo", false, true));
        Label11->Caption = FloatToStr(DBGridAlt1->getSum("saldo", true, false));
        Label12->Caption = FloatToStr(DBGridAlt1->getSum("saldo", true, true));
        */
    }
}

/*
*/
void __fastcall TFieldActivityForm::DBGridAltGeneralChangeFilter(TObject *Sender)
{
    //DBGridAltGeneralChangeCheck(Sender);
    refreshCheckedCount();
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
        //refreshParametersControls();
    }

    //refreshControls();
}

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
    refreshCheckedCount();
    //Application->CreateForm(__classid(TWaitForm), &WaitForm);
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
    TPoint p = control->ClientOrigin;
    menu->Popup(p.x, p.y + control->Height);
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
    //TPageControl *pageControl = static_cast <TTabControl *> (Control);
    TCanvas *canvas = Control->Canvas;
    // Текст, который будем выводить на вкладке

    int pageIndex = PageIndexFromTabIndex(((TPageControl*)Control), TabIndex);

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
    showDebtorList();
}

void __fastcall TFieldActivityForm::PostListTabSheetShow(TObject *Sender)
{
//
    showPostList();
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
    showStopList();
}

/* Переключили на вкладку Утверждения */
void __fastcall TFieldActivityForm::ApprovalListTabSheetShow(
      TObject *Sender)
{
    //setCurPackMode(PACK_APPROVE_LIST);
    // возможно переделать на show...
    showApprovalList();
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

/**/
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
        showDebtorList();
        break;
    }
    case SHEET_TYPE_STOP:
    {
        showStopList();
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
            showDebtorList();
            break;
        }

        case MODE_STOP:
        {
            showStopList();
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

/* Создать реестр уведомлений контролеру */
void __fastcall TFieldActivityForm::createFaPackActionExecute(TObject *Sender)
{

    String faPackId = MainDataModule->createPackNotice(FPT_MANUAL);
    if (faPackId != "" )
    {
        WaitForm->Execute(2);
        //String faPackId = MainDataModule->createPack(_currentFilter, TPackTypeCd::PACK_MANUAL, MainDataModule->getAcctOtdelen());
        showFaPack(faPackId);
        MainDataModule->closeDebtorsQuery();
        //MessageBoxInf("Создан реестр с номером " + faPackId + ".\nНе забудьте ввести даты контактов.");
        WaitForm->StopWait();
    }
    else
    {
        MessageBoxStop("Не удалось создать реестр!\nОбратитесь к системному администратору.");
    }
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
//    SelectAllCheckBox->Checked = true;
}

/* Снять пометки */
void __fastcall TFieldActivityForm::checkNoneActionExecute(TObject *Sender)
{
    _currentDbGrid->setCheckFiltered(false);
//    SelectAllCheckBox->Checked = false;
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

//
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
/* Выделение элементов с помощью CheckBox */
void __fastcall TFieldActivityForm::SelectAllCheckBoxClick(TObject *Sender)
{
    switch( ((TCheckBox*)Sender)->State )
    {
    case cbChecked:
    case cbGrayed:
    {
        checkAllAction->Execute();
        break;
    }
    case cbUnchecked:
    {
        checkNoneAction->Execute();
        break;
    }
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

/**/
void __fastcall TFieldActivityForm::checkAllActionUpdate(TObject *Sender)
{
    static_cast<TAction*>(Sender)->Enabled = _recordCount > 0;
}

/**/
void __fastcall TFieldActivityForm::createFaPackNoticeActionUpdate(
      TObject *Sender)
{
    static_cast<TAction*>(Sender)->Enabled = _checkedCount > 0;
}
//---------------------------------------------------------------------------
/**/
void __fastcall TFieldActivityForm::OnChangeFilter(TObject *Sender)
{
    refreshCheckedCount();
}


/**/
void __fastcall TFieldActivityForm::createFaPackPostActionExecute(
      TObject *Sender)
{
    String faPackId = MainDataModule->createPackNotice(FPT_POST);
    if (faPackId != "" )
    {
        WaitForm->Execute(2);
        showFaPack(faPackId);
        MainDataModule->closeDebtorsQuery();
        //MessageBoxInf("Создан реестр с номером " + faPackId + ".\nНе забудьте ввести даты контактов.");
        WaitForm->StopWait();
    }
    else
    {
        MessageBoxStop("Не удалось создать реестр!\nОбратитесь к системному администратору.");
    }

}
//---------------------------------------------------------------------------

