//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainDataModuleUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "Ora"
#pragma link "MemDS"
#pragma link "VirtualTable"
#pragma link "DataSetFilter"
#pragma resource "*.dfm"
TMainDataModule *MainDataModule;

// Открывает запрос или обновляет (параметры)
void TMainDataModule::openOrRefresh(TOraQuery* query)
{
    if (query->Active)
    {
        query->Refresh();
    }
    else
    {
        query->Open();
    }
}


DualList::DualList() :
    _used(false),
    _sourceQuery(NULL)

{
    _descrList = new TStringList();
    _valueList = new TStringList();
}

DualList::~DualList()
{
    if (_descrList != NULL)
    {
        delete _descrList;
        delete _valueList;
    }
}

void DualList::assignTo(TComboBox* comboBox)
{
    if (!_used && _sourceQuery != NULL)
    {
        addFromQuery(_sourceQuery);
    }

    comboBox->Clear();
    comboBox->Items->Assign(_descrList);
    comboBox->ItemIndex = 0;
}


void DualList::setUsed()
{
    _used = true;
}

void DualList::setSourceQuery(TOraQuery* query)
{
    _sourceQuery = query;
}

//---------------------------------------------------------------------------
// добавляет "наименование - значение" в список
void DualList::add(const String& descr, const String& value)
{
    setUsed();
    _valueList->Add(value);
    _descrList->Add(descr);
}

//---------------------------------------------------------------------------
// Добавляет список "наименование - значение" из запроса
void DualList::addFromQuery(TOraQuery* listQuery)
{
    _sourceQuery->Open();
    _sourceQuery->First();
    while ( !listQuery->Eof )
    {
        add(listQuery->FieldByName("descr")->AsString, listQuery->FieldByName("value")->AsString);
        listQuery->Next();
    }
    _sourceQuery->Close();

}

/*
*/
String DualList::getValue(int index)
{
    if (!_used && _sourceQuery != NULL)
    {
        addFromQuery(_sourceQuery);
    }
    //return _valueList->Values[this->OtdelenComboBox->ItemIndex];
    return _valueList->Strings[index];
}

/*
*/
String DualList::getValue(TComboBox* comboBox)
{
    return getValue(comboBox->ItemIndex);
}

/*
*/
__fastcall TMainDataModule::TMainDataModule(TComponent* Owner)
    : TDataModule(Owner),
    _currentFaPackId("")
{
    // Список для комбобокса
    //otdelenList.setSourceQuery(getOtdelenList);
}

/*
*/
void __fastcall TMainDataModule::connectEsale()
{
    MainDataModule->EsaleSession->Connect();
}
/*
*/
void __fastcall TMainDataModule::DataModuleCreate(TObject *Sender)
{
    String appPath = ExtractFilePath(Application->ExeName);

    /* Соединяемся с базой данных */
    connectEsale();

    /* Получаем список должников */
    getOtdelenListQuery->Open();

    /* Задаем текущее отделение */
    setAcctOtdelen(getOtdelenListQuery->FieldByName("ACCT_OTDELEN")->Value);


    /* Создание фильтров */
    // Фильтр для работы с выделенными строками
    getCheckedFilter->add("checked", "check_data = 1");

    // Фильтр для списка должников
    {
        //DBGridAltGeneral->Filtered = true;
        getDebtorsFilter->DisableControls();
        getDebtorsFilter->add("AcctIdComboBox", "acct_id like '%:param%'");
        getDebtorsFilter->add("AddressComboBox", "address like '%:param%'");
        getDebtorsFilter->add("CityComboBox", "city like '%:param%'");
        getDebtorsFilter->add("FioComboBox", "fio like '%:param%'");
        getDebtorsFilter->add("PackIdFilterComboBox", "fa_pack_id like '%:param%'");
        getDebtorsFilter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
        getDebtorsFilter->add("SaldoFilterEdit", "saldo_uch > :param");

        //_filter.add("cc_dttm", "cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm'");
        TDataSetFilterItem* filterItem = MainDataModule->getDebtorsFilter->add("cc_dttm", "((cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm') :or_cc_dttm_is_null)");
        filterItem->addParameter(":begin_cc_dttm");
        filterItem->addParameter(":end_cc_dttm");
        filterItem->addParameter(":or_cc_dttm_is_null");

        // Фильтры для пометок
        getDebtorsFilter->add("cc_dttm_is", "cc_dttm is :param");
        getDebtorsFilter->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");

        getDebtorsFilter->EnableControls();

        //_filter.add(DateTimePicker1->Name, "cc_dttm > ':begin_cc_dttm' and ':param1'");
        //TOtdelenComboBoxFrame::YS = "nt";
        //DBGridAlt2->DataSource = MainDataModule->GridFaPackDs;
        //DBGridAlt2->Filtered = true;
        //DBGridAlt2->assignFilter(&_filter);
    }


        // Фильтр для списка уведомлений
    {
        ///DBGridAltManual->Filtered = true;
        getFaPackFilter->DisableControls();
        getFaPackFilter->add("AcctIdComboBox", "acct_id like '%:param%'");
        getFaPackFilter->add("AddressComboBox", "address like '%:param%'");
        getFaPackFilter->add("FioComboBox", "fio like '%:param%'");
        getFaPackFilter->add("PackIdFilterComboBox", "fa_pack_id like '%:param%'");
        getFaPackFilter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
        getFaPackFilter->add("SaldoFilterEdit", "saldo_uch > :param");
        getFaPackFilter->add("CityComboBox", "city like '%:param%'");

        //_filter.add("cc_dttm", "cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm'");
        TDataSetFilterItem* filterItem = MainDataModule->getFaPackFilter->add("cc_dttm", "((cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm') :or_cc_dttm_is_null)");
        filterItem->addParameter(":begin_cc_dttm");
        filterItem->addParameter(":end_cc_dttm");
        filterItem->addParameter(":or_cc_dttm_is_null");

        // Фильтры для пометок
        getFaPackFilter->add("cc_dttm_is", "cc_dttm is :param");
        getFaPackFilter->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");
        getFaPackFilter->EnableControls();
    }









    //getOtdelenList->Open();
    //TOtdelenComboBoxFrame::add("тестирование","ddd");
    //TOtdelenComboBoxFrame::addFromQuery(getOtdelenList);
    //getOtdelenList->Close();


    faTypesList.add("Все", "0");
    faTypesList.add("Реестр контролеру", "1");
    faTypesList.add("Реестр по почте", "2");
    faTypesList.add("Реестр на отключение", "3");

    faTypesAndDebtorList.add("Должники", "0");
    faTypesAndDebtorList.add("Реестр контролеру", "1");
    faTypesAndDebtorList.add("Реестр по почте", "2");
    faTypesAndDebtorList.add("Реестр на отключение", "3");


    getCcStatusFlgListQuery->Open();
    getCcTypeCdQuery->Open();

    getOtdelenListQuery->ParamByName("app_path")->AsString = appPath;
    getOtdelenListQuery->Open();

    getFaPackInfo->ParamByName("app_path")->AsString = appPath;

    /* DataSet Powerfull Filters */
    //_filterForApprove;
    //_filterForApprove.add("1", "check_data = 1");

        // Список для утверждения
/*    {
        //ApproveListGrid->Filtered = true;
        //ApproveListGrid->assignFilter(&_filterApproveList);
        _filterApproveList->add(AcctIdComboBox->Name, "acct_id like '%:param%'");
        _filterApproveList->add(AddressComboBox->Name, "address like '%:param%'");
        _filterApproveList->add(FioComboBox->Name, "fio like '%:param%'");
        _filterApproveList->add(PackIdFilterComboBox->Name, "fa_pack_id like '%:param%'");
        _filterApproveList->add(ServiceCompanyFilterComboBox->Name, "service_org like '%:param%'");
        _filterApproveList->add(SaldoFilterEdit->Name, "saldo_uch > :param");
        _filterApproveList->add(CityComboBox->Name, "city like '%:param%'");

        //_filter.add("cc_dttm", "cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm'");
        TDBGridAltFilterItem* filterItem = _filterApproveList.add("cc_dttm", "((cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm') :or_cc_dttm_is_null)");
        filterItem->addParameter(":begin_cc_dttm");
        filterItem->addParameter(":end_cc_dttm");
        filterItem->addParameter(":or_cc_dttm_is_null");

        // Фильтры для пометок
        _filterApproveList->add("cc_dttm_is", "cc_dttm is :param");
        _filterApproveList->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");
    } */

    // фильтр для фильтрации выделенных
    getCheckedFilter->add("checked", "check_data = 1");
    //getCheckedFilter->setValue("checked", "", " ");



}

/*
*/
String TMainDataModule::addCc(TDataSet* ds, TDateTime ccDttm, const String& acct_id, const String& descr,
    const String& srcId, const String& caller,
    TCcTypeCd::Type ccTypeCd, TCcStatusFlg::Type ccStatusFlg, TCcSourceTypeCd::Type ccSourceTypeCd)
{
    addCcProc->ParamByName("p_cc_dttm")->Value = ccDttm;
    addCcProc->ParamByName("p_cc_type_cd")->Value = FormatFloat("00", ccTypeCd);//IntToStr(statusFlg);
    addCcProc->ParamByName("p_cc_status_flg")->Value = FormatFloat("00", ccStatusFlg);//IntToStr(statusFlg);
    addCcProc->ParamByName("p_src_type_cd")->Value = FormatFloat("00", ccSourceTypeCd);//IntToStr(statusFlg);
    addCcProc->ParamByName("p_acct_id")->Value = acct_id;
    addCcProc->ParamByName("p_descr")->Value = descr;
    addCcProc->ParamByName("p_src_id")->Value = srcId;
    addCcProc->ParamByName("p_caller")->Value = caller;

    addCcProc->ExecProc();


    // Для моментального отображения
    if ( !addCcProc->ParamByName("result")->IsNull )
    {
        ds->DisableControls();
        ds->Edit();
        ds->FieldByName("cc_dttm")->Value = ccDttm;
        //ds->FieldByName("cc_dttm")->RefreshLookupList();
        //ds->FieldByName("cc_dttm")->Value = ccDttm;
        ds->Post();
        ds->EnableControls();
    }


    return addCcProc->ParamByName("result")->AsString;
}

/*
*/
String TMainDataModule::setCcApprovalDttm(String ccId, bool cancelApproval)
{
    if (cancelApproval)
    {
        setCcApprovalDttmProc->ParamByName("p_approval_dttm")->Clear();
    }
    else
    {
        setCcApprovalDttmProc->ParamByName("p_approval_dttm")->Value = Now();
    }
    setCcApprovalDttmProc->ParamByName("p_cc_id")->Value = ccId;

    setCcApprovalDttmProc->ExecProc();
    //return setCcApprovalDttmProc->ParamByName("result")->AsString;
}

/*
*/
void TMainDataModule::setCcApprovalDttmSelected(bool cancelApproval)
{
    getApprovalListQuery->DisableControls();
    int N = getApprovalListQuery->RecNo;
    //getApprovalListQuery->Filtered = false;
    getApprovalListQuery->First();
    // снять все фильтры
    // возможно наложить фильтр соответственно cancelApproval
    // или анализировать в цикле

    while ( !getApprovalListQuery->Eof )
    {
        if (getApprovalListQuery->FieldByName("check_data")->Value == 1 &&
            !getApprovalListQuery->FieldByName("cc_id")->IsNull)
        {
            setCcApprovalDttm(getApprovalListQuery->FieldByName("cc_id")->Value);

            getApprovalListQuery->Edit();
            getApprovalListQuery->FieldByName("approval_dttm")->Value = setCcApprovalDttmProc->ParamByName("p_approval_dttm")->Value;
            getApprovalListQuery->Post();
        }
        getApprovalListQuery->Next();
    }

    //getApprovalListQuery->Filtered = true;
    getApprovalListQuery->RecNo = N;

    // вернуть фильтры
    getApprovalListQuery->EnableControls();
}

/* Создать реестр
*/
String __fastcall TMainDataModule::createPack(TDataSetFilter* filter, TPackTypeCd::Type packTypeCd, const String& acctOtdelen)
{
    // Создаем новый пакет
    MainDataModule->AddFaPackProc->ParamByName("p_fa_pack_type_cd")->Value = packTypeCd;
    MainDataModule->AddFaPackProc->ParamByName("p_acct_otdelen")->Value = acctOtdelen;
    MainDataModule->AddFaPackProc->ExecProc();
    String faPackId = MainDataModule->AddFaPackProc->ParamByName("result")->AsString;

    /*if (_currentDbGrid->recordCountChecked == 0)
    {
        return NULL;
    }*/


    // Цикл по выделенным строкам
    // из каждой строки по очереди берутся параметры город, улица, контрлер
    // и подставляются в запрос
    // Полученные данные выводятся в шаблон Excel,
    // затем действия повторяются для следующей строки из списка с параметрами
    //_currentDbGrid->DataSource->DataSet->DisableControls();
    filter->LockDataSetPos();


    getCheckedFilter->setValue("checked", "", " ");
    //String k = getCheckedFilter->getFilterString();
    getCheckedFilter->DataSet = filter->DataSet;


    // Цикл
    while ( !filter->DataSet->Eof )
    {
        AddFaProc->ParamByName("p_acct_id")->AsString = filter->DataSet->FieldByName("acct_id")->AsString;
        AddFaProc->ParamByName("p_fa_pack_id")->AsString = faPackId;
        AddFaProc->ExecProc();
        filter->DataSet->Next();
    }
    getCheckedFilter->DataSet = NULL;
    filter->UnlockDataSetPos();
    return faPackId;



    /*for (int i = 1; !filter->DataSet->Eof; i++, _currentDbGrid->DataSource->DataSet->Next())
    {
        //if ( !_currentDbGrid->isChecked() ) // Если строка отмечена, то добавляем параметры
        //{
        //    continue;
        //}

        AddFaProc->ParamByName("p_acct_id")->AsString = _currentDbGrid->DataSource->DataSet->FieldByName("acct_id")->AsString;
        AddFaProc->ParamByName("p_fa_pack_id")->AsString = faPackId;
        AddFaProc->ExecProc();
    } */






    //_currentDbGrid->Filtered = false;
    //_currentDbGrid->DataSource->DataSet->First();
    //dataSet->First();

    //filter->SetFilterExclusive("cheked");
    //filter->setFilterParamValue(_currentFilter, "cc_dttm", "begin_cc_dttm", " ");

    //getCheckedFilter->



    // 2017-01-16
    // Накладываем фильтр на дата сет + цикл

/*    for (int i = 1; !_currentDbGrid->DataSource->DataSet->Eof; i++, _currentDbGrid->DataSource->DataSet->Next())
    {
        if ( !_currentDbGrid->isChecked() ) // Если строка отмечена, то добавляем параметры
        {
            continue;
        }

        MainDataModule->AddFaProc->ParamByName("p_acct_id")->AsString = _currentDbGrid->DataSource->DataSet->FieldByName("acct_id")->AsString;
        MainDataModule->AddFaProc->ParamByName("p_fa_pack_id")->AsString = faPackId;
        MainDataModule->AddFaProc->ExecProc();
    }
    _currentDbGrid->Filtered = true;

    MainDataModule->EsaleSession->Commit();
    _currentDbGrid->DataSource->DataSet->EnableControls();
*/

}

//void __fastcall TMainDataModule::setFilter(const String& filterName, const String& filterValue)
void __fastcall TMainDataModule::setFilterParamValue(TDataSetFilter* filter, const String& filterName, const String& paramName, const String& paramValue)
{
    filter->setValue(filterName, paramName, paramValue);
    //_currentFilter->setValue(filterName, "param", filterValue);
    //_currentDbGrid->refreshFilter();
}

/**/
void __fastcall TMainDataModule::selectCcDttmIsNull(TDataSetFilter* filter)
{
    //filter->BeginUpdate();
    filter->LockDataSetPos();
    filter->DisableControls();

    filter->setValue("cc_dttm_is", "param", "null");
    setCheckAll(filter->DataSet, true);
    filter->setValue("cc_dttm_is", "param", "");

    filter->EnableControls();
    filter->UnlockDataSetPos();
    //filter->DataSet->EnableControls();
}

/**/
void __fastcall TMainDataModule::selectCcDttmMoreThanThree(TDataSetFilter* filter)
{
    filter->LockDataSetPos();
    filter->DisableControls();

    String dttm = DateToStr( IncMonth(Now(), -3) );
    filter->setValue("cc_dttm_more", "param", dttm);

    setCheckAll(filter->DataSet, true);
    filter->setValue("cc_dttm_more", "param", "");

    filter->DataSet->EnableControls();
    filter->EnableControls();
    filter->UnlockDataSetPos();
}



/*void __fastcall TMainDataModule::LockDataSetPos(TDataSet* dataSet)
{
    int N = dataSet->RecNo;
    dataSet->DisableControls();
}

void __fastcall TMainDataModule::UnlockDataSetPos(TDataSet* dataSet)
{
    dataSet->RecNo = N;
    dataSet->EnableControls();
}  */

//---------------------------------------------------------------------------
// Пометить все строки
void __fastcall TMainDataModule::setCheckAll(TDataSet* dataSet, bool value)
{
    //int N = dataSet->RecNo;
    //dataSet->DisableControls();

    dataSet->First();
    while( !dataSet->Eof )
    {
        dataSet->Edit();
        dataSet->FieldByName("CHECK_DATA")->Value = (value ? TCheckTypes::CT_CHECKED : TCheckTypes::CT_UNCHECKED);
        dataSet->Post();

        dataSet->Next();
    }

    //dataSet->RecNo = N;
    //dataSet->EnableControls();
}


/* Отображает список на утверждение
   по выбранному участку */
void __fastcall TMainDataModule::getApprovalList(const String& acctOtdelen)
{
    if (getApprovalListQuery->ParamByName("acct_otdelen")->Value != acctOtdelen)
    {
        getApprovalListQuery->ParamByName("acct_otdelen")->Value = acctOtdelen;
        openOrRefresh(getApprovalListQuery);
    }
}

/* Отображает список на введение ограничения
   по выбранному участку */
void __fastcall TMainDataModule::getStopList(const String& acctOtdelen)
{
    if (getStopListQuery->ParamByName("acct_otdelen")->Value != acctOtdelen)
    {
        getStopListQuery->ParamByName("acct_otdelen")->Value = acctOtdelen;
        openOrRefresh(getStopListQuery);
    }
}

/* Отображает список реестров
   по выбранному участку */
void __fastcall TMainDataModule::getFaPackList(String acctOtdelen)
{
    if (acctOtdelen == "")
    {
        acctOtdelen = _currentAcctOtdelen;
    }
    if (selectFaPackQuery->ParamByName("acct_otdelen")->Value != acctOtdelen)
    {
        selectFaPackQuery->ParamByName("acct_otdelen")->Value = acctOtdelen;
        openOrRefresh(selectFaPackQuery);
    }
    selectFaPackQuery->First();
}

/* Отображает реестр
   по выбранному ID реестра */
void __fastcall TMainDataModule::getFaPack(const String& faPackId, String& acctOtdelen)
{
    if (getFaPackQuery->ParamByName("p_fa_pack_id")->Value != faPackId)
    {
        getFaPackQuery->ParamByName("p_fa_pack_id")->AsString = faPackId;
        openOrRefresh(MainDataModule->getFaPackQuery);
        acctOtdelen = getFaPackQuery->FieldByName("acct_otdelen")->AsString;
    }
}

/* Отображает список должников
   по выбранному участку */
void __fastcall TMainDataModule::getDebtorList(const String& acctOtdelen)
{
    if (getDebtors->ParamByName("otdelen")->Value != acctOtdelen)
    {
        getDebtors->ParamByName("otdelen")->Value = acctOtdelen;
        openOrRefresh(MainDataModule->getDebtors);
    }
}

/*
void __fastcall TMainDataModule::getDebtorList()
{
    filter->LockDataSetPos();
    filter->DisableControls();

    DocumentDataModule->getDocumentFaNotices(getFaPackQuery, getOtdelenListQuery);

    //String dttm = DateToStr( IncMonth(Now(), -3) );
    //filter->setValue("cc_dttm_more", "param", dttm);
    //setCheckAll(filter->DataSet, true);
    //filter->setValue("cc_dttm_more", "param", "");


    /*getCheckedFilter->setValue("checked", "", " ");
    //String k = getCheckedFilter->getFilterString();
    getCheckedFilter->DataSet = filter->DataSet;
    getCheckedFilter->DataSet = NULL;*/


  /*

    filter->DataSet->EnableControls();
    filter->EnableControls();
    filter->UnlockDataSetPos();
}    */


/* Задает текущее отделение
   updateOnly = true то меняется только значение _currentAcctOtdelen
*/
void __fastcall TMainDataModule::setAcctOtdelen(const String& acctOtdelen, bool updateOnly)
{
    if (_currentAcctOtdelen != acctOtdelen)
    {
        _currentAcctOtdelen = acctOtdelen;

        if (updateOnly == false)
        {
            _currentFaPackId = "";
            getFaPackQuery->Close();
        }
    }
}

/* Задает текущий пакет
*/
void __fastcall TMainDataModule::setFaPackId(const String& faPackId, const String& acctOtdelen)
{
    _currentFaPackId = faPackId;
    if (acctOtdelen != "") // если отделение известно
    {
        setAcctOtdelen(acctOtdelen, true);
    }
    else
    {
        // При необходимости сделать запрос для получения значения acctOtdelen по faPackId
    }
}

String __fastcall TMainDataModule::getAcctOtdelen()
{
    return _currentAcctOtdelen;
}

String __fastcall TMainDataModule::getFaPackId()
{
    return _currentFaPackId;
}
