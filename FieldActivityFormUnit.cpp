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
    : TForm(Owner),
    _curPackMode(PACK_MODE_UNDEFINED)
{

    // Список для утверждения
    {
        //ApproveListGrid->Filtered = true;
        MainDataModule->getApprovalListFilter->DisableControls();
        MainDataModule->getApprovalListFilter->add(AcctIdComboBox->Name, "acct_id like '%:param%'");
        MainDataModule->getApprovalListFilter->add(AddressComboBox->Name, "address like '%:param%'");
        MainDataModule->getApprovalListFilter->add(FioComboBox->Name, "fio like '%:param%'");
        MainDataModule->getApprovalListFilter->add(PackIdFilterComboBox->Name, "fa_pack_id like '%:param%'");
        MainDataModule->getApprovalListFilter->add(ServiceCompanyFilterComboBox->Name, "service_org like '%:param%'");
        MainDataModule->getApprovalListFilter->add(SaldoFilterEdit->Name, "saldo_uch > :param");
        MainDataModule->getApprovalListFilter->add(CityComboBox->Name, "city like '%:param%'");

        //_filter.add("cc_dttm", "cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm'");
        TDataSetFilterItem* filterItem = MainDataModule->getApprovalListFilter->add("cc_dttm", "((cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm') :or_cc_dttm_is_null)");
        filterItem->addParameter(":begin_cc_dttm");
        filterItem->addParameter(":end_cc_dttm");
        filterItem->addParameter(":or_cc_dttm_is_null");

        // Фильтры для пометок
        MainDataModule->getApprovalListFilter->add("cc_dttm_is", "cc_dttm is :param");
        MainDataModule->getApprovalListFilter->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");
        MainDataModule->getApprovalListFilter->EnableControls();
    }


    //MainDataModule->otdelenList.assignTo(OtdelenComboBox);

    _colorList.clear();
    _colorList.addColor(static_cast<TColor>(RGB(90,225,255)));  // blue
    _colorList.addColor(static_cast<TColor>(RGB(255,180,50)));  // orange
    _colorList.addColor(static_cast<TColor>(RGB(255,130,170))); // rose
    _colorList.addColor(static_cast<TColor>(RGB(120,230,90)));  // green

    //_colorList.addColor(static_cast<TColor>(RGB(180,255,20)));
    //_colorList.addColor(static_cast<TColor>(RGB(255,100,0))); // red
    //_colorList.addColor(static_cast<TColor>(RGB(255,255,0)));
    //_colorList.addColor(static_cast<TColor>(RGB(255,255,255)));

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
void __fastcall TFieldActivityForm::FormShow(TObject *Sender)
{
    this->WindowState = wsMaximized;
    this->UpdateWindowState();

    // Текущее отделение
    showDebtorList(MainDataModule->getAcctOtdelen());
}


/* Отображает список должников
   Возможно следует удалить эту функцию
*/
void __fastcall TFieldActivityForm::showDebtorList(const String& acctOtdelen)
{
    MainDataModule->getDebtorList(acctOtdelen);
    MainDataModule->setAcctOtdelen(acctOtdelen);

    //setCurOtdelen(acctOtdelen);

    refreshParametersControls();

    setCurPackMode(PACK_GENERAL);
}

/* Отображает реестр по ID
*/
void __fastcall TFieldActivityForm::showFaPack(const String& faPackId)
{
    /*if (faPackId == "")
    {
        return;
    }*/
    String acctOtdelen;
    MainDataModule->getFaPack(faPackId, acctOtdelen);

    if ( acctOtdelen != "")
    {
        MainDataModule->setFaPackId(faPackId, acctOtdelen);
    }

    setCurPackMode(PACK_MANUAL);
    //_currentFilter->clearAllValues();


    //if (_curPackMode != PACK_MANUAL /*|| _curPackId != faPackId*/)
    //{
    //    setCurPackMode(PACK_MANUAL);
    //    _currentFilter->clearAllValues();
    //}
}

void __fastcall TFieldActivityForm::showStopList(const String& acctOtdelen)
{
    MainDataModule->getStopList(acctOtdelen);
    //setCurOtdelen(acctOtdelen);
    setCurPackMode(PACK_STOP);
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
    setCurPackMode(PACK_APPROVE_LIST);
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
void __fastcall TFieldActivityForm::setCurPackMode(PackMode packMode)
{
    _curPackMode = packMode;
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

    bool bPackGeneral = _curPackMode == PACK_GENERAL;
    bool bPackManual = _curPackMode == PACK_MANUAL;
    bool bPackStop = _curPackMode == PACK_STOP;
    bool bPackApprove = _curPackMode == PACK_APPROVE_LIST;


    printDocumentFaNoticesAction->Enabled = bPackManual;
    printDocumentFaNoticesListAction->Enabled = bPackManual;
    printDocumentStopAction->Enabled = bPackStop;
    printDocumentStopListAction->Enabled = bPackStop;
    printDocumentStopActionRefuseAction->Enabled = bPackStop;

    approveFaPackCcDttmAction->Enabled = bPackApprove;
    createFaPackAction->Enabled = bPackGeneral;


    switch(_curPackMode)
    {
    case PACK_GENERAL:
        {
            ParamPackIdEdit->Text = "";
            ComboBox5->ItemIndex = 0;

            if (!DebtorsTabSheet->Visible)
            {
                DebtorsTabSheet->Show();
            }
            //int k2 = PackPageControl->ActivePageIndex;
            //ModeDebtorsButton->Down = true;
            //if (_packPageControlChangedSelf)
            //{
                //PageControl1->ActivePageIndex = DebtorsTabSheet->TabIndex;
            //}
            _currentFilter = MainDataModule->getDebtorsFilter;//&_filterPackGeneral;
            _currentDbGrid = DBGridAltGeneral;

            break;
        }
    case PACK_MANUAL:
        {
            ParamPackIdEdit->Text = MainDataModule->getFaPackId();//_curPackId;

            if (!PackManualTabSheet->Visible)
            {
                PackManualTabSheet->Show();
            }
            //PageControl1->ActivePageIndex = PackManualTabSheet->TabIndex;
            //_currentFilter = &_filterPackManual;

            _currentFilter = MainDataModule->getFaPackFilter;
            _currentDbGrid = DBGridAltManual;
            break;
        }
    case PACK_STOP:
        {
            if ( !StopListTabSheet->Visible )
            {
                StopListTabSheet->Show();
            }
            //ParamPackIdEdit->Text = _curPackId;

            _currentFilter = MainDataModule->getStopListFilter;
            _currentDbGrid = StopListDBGrid;

            break;
        }
     case PACK_APPROVE_LIST:
        {
            ParamPackIdEdit->Text = "";

            if ( !ApprovalListTabSheet->Visible )
            {
                ApprovalListTabSheet->Show();
            }
            _currentFilter = MainDataModule->getApprovalListFilter;
            _currentDbGrid = ApproveListGrid;

        }
    }

    //String t1 = MainDataModule->getAcctOtdelen();
    refreshFilterControls();
}


/* 
*/
void __fastcall TFieldActivityForm::refreshControls()
{
    if (_currentDbGrid != NULL)
    {
        bool checkedItems = _currentDbGrid->recordCountChecked > 0;
        bool countItems = _currentDbGrid->recordCount > 0;

    //Image3->Visible = _curPackMode == PACK_BLOCK;
    //Image2->Visible = Image3->Visible || _curPackMode == PACK_POST;
    //Image1->Visible = Image2->Visible || _curPackMode == PACK_MANUAL;
    //BitBtn1->Visible = checkedItems;
        GroupBox6->Enabled = checkedItems;
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

/*
*/
void __fastcall TFieldActivityForm::FilterCcDttmChange(TObject *Sender)
{
    String s1 = DateToStr(DateTimePicker1->Date);
    String s2 = DateToStr(DateTimePicker2->Date);
    String s3 = CcDttmIsNullCheckBox->Checked? "or cc_dttm is null" : " ";

    MainDataModule->setFilterParamValue(_currentFilter, "cc_dttm", "begin_cc_dttm", s1);
    MainDataModule->setFilterParamValue(_currentFilter, "cc_dttm", "begend_cc_dttmin_cc_dttm", s2);
    MainDataModule->setFilterParamValue(_currentFilter, "cc_dttm", "or_cc_dttm_is_null", s3);

    //MainDataModule->refreshFilter();
    //_currentFilter->setValue("cc_dttm", ":begin_cc_dttm", s1);
    //String ss1 = _currentFilter->getFilterString();
    //_currentFilter->setValue("cc_dttm", ":end_cc_dttm", s2);
    //String ss2 = _currentFilter->getFilterString();
    //_currentFilter->setValue("cc_dttm", ":or_cc_dttm_is_null", s3);
    //String ss3 = _currentFilter->getFilterString();
    //_currentDbGrid->refreshFilter();
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
}

//---------------------------------------------------------------------------
// Сбрасывает значения фильтров
void __fastcall TFieldActivityForm::resetFilterControls()
{

    FilterControlerComboBox->Text = "";
    FilterCityComboBox->Text = "";
    FilterUlitsaComboBox->Text = "";
    FilterPrioritetComboBox->Text = "";

    DBGridAlt1->Filtered = false;
    DBGridAlt1->Filter->Clear();
}
*/

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
    refreshControls();
}

/*
*/
void __fastcall TFieldActivityForm::DBGridAltGeneralChangeFilter(TObject *Sender)
{
    DBGridAltGeneralChangeCheck(Sender);
}

/*
*/
void __fastcall TFieldActivityForm::ParamPackIdEditClick(TObject *Sender)
{
    if ( SelectFaPackForm->execute() )
    {
        //FaPack faPack = SelectFaPackForm->getFaPack();
        //setCurPackId( SelectFaPackForm->getFaPackId() );
        //setCurPackMode( SelectFaPackForm->getFaPackTypeId() );
        showFaPack( SelectFaPackForm->getFaPackId() );
        refreshParametersControls();
    }

    //refreshControls();
}

/*
*/
void __fastcall TFieldActivityForm::Button6Click(TObject *Sender)
{
    bool b1 = MainDataModule->getFaPackQuery->Active;
    bool b11 = MainDataModule->getFaPackQuery->Filtered;

    //showFaPack("0000000818");

    //bool ll = MainDataModule->getOtdelenListDataSource->DataSet;

    //bool ff =  MainDataModule->getOtdelenList->Active;
    //int kk = MainDataModule->getOtdelenList->RecordCount;
    //DocumentDataModule->getDocumentFaNotices(_currentFilter->DataSet, MainDataModule->getOtdelenListQuery);
    //DocumentDataModule->getDocumentFaNotices(_currentFilter);


    //datetimepicker1.Kind := dtkDate;
    //DBGridAlt1->DataSource->DataSet->Filter = "acct_id = \"7474750000\"";
    //DBGridAlt1->DataSource->DataSet->Filter = "cre_dttm < '10.10.2016'";
    //DBGridAlt1->DataSource->DataSet->Filter = "cre_dttm is null";
    //DBGridAlt1->DataSource->DataSet->Filtered = true;

    /*DBGridAlt1->edit->Left = 0;
    DBGridAlt1->edit->Top = 0;
    DBGridAlt1->Visible = true;*/
    //DBGridAlt1->edit->Owner = this;

}

/*
*/
void __fastcall TFieldActivityForm::ParamPackIdEditKeyPress(
      TObject *Sender, char &Key)
{
    Key = '\0';
    ParamPackIdEditClick(Sender);
}

/**/
void __fastcall TFieldActivityForm::DBGridAltGeneralEnter(TObject *Sender)
{
    //ShowMessage("aaa");    
}

/* Отображает окно создания/редактирования контакта */
void __fastcall TFieldActivityForm::DBGridAltManualCellClick(TColumn *Column)
{
    if (Column->FieldName == "CC_DTTM")
    {
        EditCcForm->Execute(_currentDbGrid->DataSource->DataSet);
    }
}

/* Снять все пометки с отфильтрованных */
void __fastcall TFieldActivityForm::N2Click(TObject *Sender)
{
    _currentDbGrid->setCheckFiltered(false);
}
/**/
void __fastcall TFieldActivityForm::showMenuSelect(TButton *button, TPopupMenu* menu)
{
    TPoint p = ClientToScreen(TPoint(button->Left, button->Top + button->Height));
    menu->Popup(p.x, p.y);
}

/**/
void __fastcall TFieldActivityForm::ShowSelectAcctMenuButtonKeyPress(
      TObject *Sender, char &Key)
{
    showMenuSelect(dynamic_cast<TButton*>(Sender), SelectAcctPopupMenu);
}

/**/
void __fastcall TFieldActivityForm::ShowSelectAcctMenuButtonMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    showMenuSelect(dynamic_cast<TButton*>(Sender), SelectAcctPopupMenu);
}

/**/
void __fastcall TFieldActivityForm::ShowActionsMenuButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    showMenuSelect(dynamic_cast<TButton*>(Sender), ActionsMenu);
}/**/
void __fastcall TFieldActivityForm::PackPageControlDrawTab(
      TCustomTabControl *Control, int TabIndex, const TRect &Rect,
      bool Active)
{

    //TPageControl *pageControl = static_cast <TTabControl *> (Control);
    TCanvas *canvas = Control->Canvas;
    // Текст, который будем выводить на вкладке

    AnsiString tabCaption = ((TPageControl*)Control)->Pages[TabIndex]->Caption;

    // РИСОВАНИЕ
    canvas->Lock();    // Блокирум канвас перед рисованием
    //canvas->TextRect(Rect, Rect.Left+3, Rect.Top+3, tabCaption);
    canvas->Brush->Color = _colorList.getColorByIndex(TabIndex);
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
    showFaPack(MainDataModule->getFaPackId());
}

/**/
void __fastcall TFieldActivityForm::StopListTabSheetShow(TObject *Sender)
{
    //setCurPackMode(PACK_STOP);
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


void __fastcall TFieldActivityForm::refreshFilterControls()
{
    AcctIdComboBox->Text = _currentFilter->getValue(AcctIdComboBox->Name, "param");
    CityComboBox->Text = _currentFilter->getValue(CityComboBox->Name, "param");
    AddressComboBox->Text = _currentFilter->getValue(AddressComboBox->Name, "param");
    FioComboBox->Text = _currentFilter->getValue(FioComboBox->Name, "param");
    PackIdFilterComboBox->Text = _currentFilter->getValue(PackIdFilterComboBox->Name, "param");
    ServiceCompanyFilterComboBox->Text = _currentFilter->getValue(ServiceCompanyFilterComboBox->Name, "param");
    SaldoFilterEdit->Text = _currentFilter->getValue(SaldoFilterEdit->Name, "param");
}

/**/
void __fastcall TFieldActivityForm::OtdelenComboBoxClick(TObject *Sender)
{
    showDebtorList(OtdelenComboBox->KeyValue);
}

//void __fastcall TFieldActivityForm::Se

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
//---------------------------------------------------------------------------

void __fastcall TFieldActivityForm::printDocumentFaNoticesListActionExecute(TObject *Sender)
{
    DocumentDataModule->getDocumentFaNoticesList(_currentFilter);

}
//---------------------------------------------------------------------------


/**/
void __fastcall TFieldActivityForm::createFaPackActionExecute(TObject *Sender)
{
    String faPackId = MainDataModule->createPack(_currentFilter, TPackTypeCd::PACK_MANUAL, MainDataModule->getAcctOtdelen());
    showFaPack(faPackId);

}
//---------------------------------------------------------------------------

void __fastcall TFieldActivityForm::approveFaPackCcDttmActionExecute(TObject *Sender)
{
    if (MessageBoxQuestion("Отмечено " + IntToStr(ApproveListGrid->recordCountChecked) + " записей."
        "\nУже утвержденные контакты будут пропущены."
        "\nПродолжить?") != IDNO )
    {
        MainDataModule->setCcApprovalDttmSelected();
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TFieldActivityForm::BitBtn2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //
    showMenuSelect(dynamic_cast<TButton*>(Sender), ActionsPopupMenu);

}
//---------------------------------------------------------------------------

     //_currentDbGrid->setCheckAll(false);


    //MainDataModule->getFaPackQuery->Refresh();

    // Установка даты контакта
    /*MainDataModule->setFaCcDttm->ParamByName("p_fa_id")->Value = curFaId;
    MainDataModule->setFaCcDttm->ParamByName("p_cc_dttm")->Value = Edit1->Text;
    MainDataModule->setFaCcDttm->Execute();
    MainDataModule->getFaPackQuery->Refresh();  */

/* Выделить с датой контакта более 3 месяцев*/
void __fastcall TFieldActivityForm::checkWithCcLess3MonthActionExecute(
      TObject *Sender)
{
    MainDataModule->selectCcDttmMoreThanThree(_currentFilter);
}
//---------------------------------------------------------------------------

/* Выделить без даты контакта*/
void __fastcall TFieldActivityForm::checkWithoutCcActionExecute(
      TObject *Sender)
{
    MainDataModule->selectCcDttmIsNull(_currentFilter);

}
//---------------------------------------------------------------------------

/* Пометить отфильтрованные */
void __fastcall TFieldActivityForm::checkAllActionExecute(TObject *Sender)
{
    _currentDbGrid->setCheckFiltered(true);

}
//---------------------------------------------------------------------------





