/*
  Основной управляющий модуль программы.

  Примечание:
  На текущий момент в данном модуле также хранится набор всех основных используемых
  запросов. В дальнейшем необходимо перенести в отдельный модуль.

  Использование.
  1. Для выполнения запросов в фоновом режиме

  Важно:
  1. Запросы
    getOtdelenListQuery
    getFaPackInfo
    являются стратегически важными. В них хранится текущее состояние модели.

*/

// Режим отладки
#define DEBUG
//#undef NDEBUG

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
#pragma link "OraSmart"
#pragma resource "*.dfm"
TMainDataModule *MainDataModule;


void __fastcall TMainDataModule::CopyDataSet(TDataSet* sourceDs, TDataSet* destinationDs)
{
    destinationDs->FieldDefs->Assign(sourceDs->FieldDefs);
    destinationDs->Open();
    destinationDs->Assign(sourceDs);
    //ShowMessage(MainDataModule->VirtualTable1->RecordCount);
    //DBGridAltGeneral->DataSource->DataSet = MainDataModule->VirtualTable1;
    //MainDataModule->VirtualTable1->Filtered = true;
}


/*
  Описание объектов:
  getConfigQuery - запрос для получения конфигурационных данных для текущего пользователя


  getCcStatusFlgListQuery - Типы статусов контактов
  getCcTypeCdQuery - Типы контактов

  getDebtors
  getFaPackQuery
  getApprovalListQuery
  getStopListQuery
  selectFaPackQuery

*/


// Авторизация пользователя в программе
//void __fastcall TMainDataModule::Auth()
//{
    //TLoginForm* LoginForm = new TLoginForm(Application);
    //bool loggedon = LoginForm->Execute(EsaleSession);

    /*_username = UpperCase(LoginForm->getUsername());
    AddSystemVariable("username", _username);
    delete LoginForm;*/
   /* return loggedon;*/
//}

// Открывает запрос или обновляет (параметры)
void TMainDataModule::openOrRefresh(TOraQuery* query)
{
    //query->Close();
   /* if (query->State == dsOpening)
    {
        Sleep(500);
    }*/
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
        _descrList = NULL;
        delete _valueList;
        _valueList = NULL;
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
    : TDataModule(Owner)
    //_currentFaPackId("")
{
    // Список для комбобокса
    //otdelenList.setSourceQuery(getOtdelenList);

    getDebtorsDataSource->DataSet = getDebtorsRam;

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
    /* Авторизация пользователя */
    //#ifdef DEBUG
    EsaleSession->Username = "";
    EsaleSession->Password = "";
    //#endif

    TLoginForm* loginForm = new TLoginForm(this, EsaleSession, false);
    bool auth = loginForm->execute();



    userRole << TUserRole::UNDEFINED;
    if ( auth )
    {
        // Если соединение установлено, то определяем роль
        if (loginForm->checkRole("TASK_SWEETY_APPROVER_ROLE"))
        {
            userRole << TUserRole::APPROVER;
            userRole >> TUserRole::UNDEFINED;
        }
        if ( loginForm->checkRole("TASK_SWEETY_OPERATOR_ROLE") )
        {
            userRole << TUserRole::OPERATOR;
            userRole >> TUserRole::UNDEFINED;
        }
        if ( loginForm->checkRole("TASK_SWEETY_ADMINISTRATOR_ROLE") )
        {
            userRole << TUserRole::ADMINISTRATOR;
            userRole >> TUserRole::UNDEFINED;
        }

    }
    else  // Выход если не произошло авторизации
    {
        delete loginForm;
        Application->Terminate();
        Application->ProcessMessages();
        return;
    }

    delete loginForm;

    if ( userRole.Contains(TUserRole::UNDEFINED) )   
    {   // Если роль не установлена то выходим из программы
        Application->Terminate();
        Application->ProcessMessages();
        MessageBoxStop("Вы не имеете полномочий для работы в этой программе.\nПрограмма будет закрыта.");
        return;
    }



    /*  */
    String appPath = ExtractFilePath(Application->ExeName);

    // Это брать из базы данных
    TDate beginDt = IncMonth(Now(), -3);
    TDate endDt = Now();


    /* Соединяемся с базой данных */
    //connectEsale();

    /* Получаем список участков */
    getOtdelenListQuery->ParamByName("app_path")->AsString = appPath;
    getOtdelenListQuery->Open();

    
    if ( getOtdelenListQuery->RecordCount == 0 )
    { // Если для пользователя нет доступных участков
        Application->Terminate();
        Application->ProcessMessages();
        MessageBoxStop("Вы не имеете доступных участков.\nПрограмма будет закрыта.");
        return;
    }


    /* Задаем текущее отделение */
    setAcctOtdelen(getOtdelenListQuery->FieldByName("ACCT_OTDELEN")->AsString);


    /* Создание фильтров */
    // Фильтр для работы с выделенными строками
    getCheckedFilter->add("checked", "check_data = :param"); // Отмеченные
    getCheckedFilter->add("group", "grp_data = :param"); // С группировкой (например, для формирования заявок на отключение
                                                    // сгруппированных по сетевым в отдельные файлы)
    //getCheckedFilter->setValue("checked", "", " ");

    /* Фильтр для списка должников */
    //DBGridAltGeneral->Filtered = true;
    getDebtorsFilter->DisableControls();
    getDebtorsFilter->add("AcctIdComboBox", "acct_id like '%:param%'");
    getDebtorsFilter->add("AddressComboBox", "address like '%:param%'");
    getDebtorsFilter->add("CityComboBox", "city like '%:param%'");
    getDebtorsFilter->add("FioComboBox", "fio like '%:param%'");
    getDebtorsFilter->add("PackIdFilterComboBox", "fa_pack_id like '%:param%'");
    getDebtorsFilter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
    getDebtorsFilter->add("SaldoFilterEdit", "saldo_uch > :param");
    getDebtorsFilter->add("OpAreaDescrFilterComboBox", "op_area_descr like '%:param%'");
    getDebtorsFilter->add("cc_dttm", "(:param)");

    // Фильтры для пометок (мб перенести в отдельный фильтр?)
    getDebtorsFilter->add("cc_dttm_is", "cc_dttm is :param");
    getDebtorsFilter->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");

    // Фильтр для контролов
    getDebtorsFilter->add("vcl_ctrl", "");
    getDebtorsFilter->setValue("vcl_ctrl", "begin_dt", beginDt);
    getDebtorsFilter->setValue("vcl_ctrl", "end_dt", endDt);

    getDebtorsFilter->EnableControls();



    /* Фильтр для списка уведомлений */
    //DBGridAltManual->Filtered = true;
    getFaPackFilter->DisableControls();
    getFaPackFilter->add("AcctIdComboBox", "acct_id like '%:param%'");
    getFaPackFilter->add("AddressComboBox", "address like '%:param%'");
    getFaPackFilter->add("FioComboBox", "fio like '%:param%'");
    getFaPackFilter->add("PackIdFilterComboBox", "fa_pack_id like '%:param%'");
    getFaPackFilter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
    getFaPackFilter->add("SaldoFilterEdit", "saldo_uch > :param");
    getFaPackFilter->add("CityComboBox", "city like '%:param%'");
    getFaPackFilter->add("OpAreaDescrFilterComboBox", "op_area_descr like '%:param%'");
    getFaPackFilter->add("cc_dttm", "(:param)");

    // Фильтры для пометок
    getFaPackFilter->add("cc_dttm_is", "cc_dttm is :param");
    getFaPackFilter->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");
    getFaPackFilter->EnableControls();

    // Фильтр для контролов
    getFaPackFilter->add("vcl_ctrl", "");
    getFaPackFilter->setValue("vcl_ctrl", "begin_dt", beginDt);
    getFaPackFilter->setValue("vcl_ctrl", "end_dt", endDt);



    /* Список для утверждения */
    //ApproveListGrid->Filtered = true;
    getApprovalListFilter->DisableControls();
    getApprovalListFilter->add("AcctIdComboBox", "acct_id like '%:param%'");
    getApprovalListFilter->add("AddressComboBox", "address like '%:param%'");
    getApprovalListFilter->add("FioComboBox", "fio like '%:param%'");
    getApprovalListFilter->add("PackIdFilterComboBox", "fa_pack_id like '%:param%'");
    getApprovalListFilter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
    getApprovalListFilter->add("SaldoFilterEdit", "saldo_uch > :param");
    getApprovalListFilter->add("CityComboBox", "city like '%:param%'");
    getApprovalListFilter->add("cc_dttm", "(:param)");
    getApprovalListFilter->add("ccDttmIsApprovedCheckBox", "approval_dttm is not null :param");

    // Фильтры для пометок
    getApprovalListFilter->add("cc_dttm_is", "cc_dttm is :param");
    getApprovalListFilter->add("cc_dttm_more", "(cc_dttm < ':param' or cc_dttm is null)");
    getApprovalListFilter->EnableControls();


    // Вспомогательный фильтр для определения состояния элементов управления
    getApprovalListFilter->add("vcl_ctrl", "");
    getApprovalListFilter->setValue("vcl_ctrl", "begin_dt", beginDt);
    getApprovalListFilter->setValue("vcl_ctrl", "end_dt", endDt);

    //ccDttmSubFilter->add("cc_dttm", "( :cc_dttm_is or (cc_dttm >= ':begin_cc_dttm' and cc_dttm <= ':end_cc_dttm'))");

    /* Фильтр для реестра на ограничение */
    //DBGridAltManual->Filtered = true;
    //getFaPackFilter->DisableControls();
    getFaPackStopFilter->add("AcctIdComboBox", "acct_id like '%:param%'");
    getFaPackStopFilter->add("AddressComboBox", "address like '%:param%'");
    getFaPackStopFilter->add("FioComboBox", "fio like '%:param%'");







    //getOtdelenList->Open();
    //TOtdelenComboBoxFrame::add("тестирование","ddd");
    //TOtdelenComboBoxFrame::addFromQuery(getOtdelenList);
    //getOtdelenList->Close();


    /*faTypesList.add("Все", "0");
    faTypesList.add("Реестр контролеру", "1");
    faTypesList.add("Реестр по почте", "2");
    faTypesList.add("Реестр на отключение", "3");*/

    /*faTypesAndDebtorList.add("Должники", "0");
    faTypesAndDebtorList.add("Реестр контролеру", "1");
    faTypesAndDebtorList.add("Реестр по почте", "2");
    faTypesAndDebtorList.add("Реестр на отключение", "3");*/


    getCcStatusFlgListQuery->Open();
    getCcTypeCdQuery->Open();

    // Настройки программы
    getConfigQuery->ParamByName("app_path")->AsString = appPath;
    getConfigQuery->ParamByName("username")->AsString = EsaleSession->Username;
    getConfigQuery->Open();

    // Список отделений и прочая общая информация
    getOtdelenListQuery->ParamByName("app_path")->AsString = appPath;

    //getOtdelenListQuery->ParamByName("username")->AsString = EsaleSession->Username;

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



    _acctOtdelen = getOtdelenListQuery->FieldByName("acct_otdelen");
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


/* Создает несколько реестров
   используя группировку
   Используется, например, для создания заявок на ограничение в разные сетевые компании
*/
String __fastcall TMainDataModule::createPackMulti(TDataSetFilter* filter, TFaPackTypeCd faPackTypeCd, const String& acctOtdelen)
{
    int grp = 1;
    getCheckedFilter->setValue("checked", "param", "1");
    getCheckedFilter->setValue("group", "param", IntToStr(grp++)); //
    while ( !getFaPackStopQuery->Eof )
    {
        createPackNotice(faPackTypeCd);
        //setPackCharDttm("ST-P-DT", getConfigQuery->FieldByName("sysdate")->Value)
        getCheckedFilter->setValue("group", "param", IntToStr(grp++)); //
    }
}



/* Создать реестр
*/
String __fastcall TMainDataModule::createPackStop()
{
    // Цикл по выделенным строкам
    // из каждой строки по очереди берутся параметры город, улица, контрлер
    // и подставляются в запрос
    // Полученные данные выводятся в шаблон Excel,
    // затем действия повторяются для следующей строки из списка с параметрами
    //_currentDbGrid->DataSource->DataSet->DisableControls();
    TDataSetFilter* filter = getStopListFilter;
    filter->LockDataSetPos();

    getCheckedFilter->setValue("checked", "param", "1");
    //String k = getCheckedFilter->getFilterString();
    getCheckedFilter->DataSet = filter->DataSet;


    if (!filter->DataSet->Eof)  // Если есть отмеченные записи
    {
        // Задаем параметры пакета
        AddFaPackProc->ParamByName("p_fa_pack_type_cd")->Value = FPT_STOP;
        AddFaPackProc->ParamByName("p_acct_otdelen")->Value = _acctOtdelen->Value;

        int grp_data_max = getCheckedFilter->DataSet->FieldByName("grp_data_max")->AsInteger;
        for (int i = 1; i <= grp_data_max; i++ )
        {
            getCheckedFilter->setValue("group", "param", IntToStr(i));   // Фильтруем по группе (по индексу обслуживающей организации)
            if (filter->DataSet->Eof)
            {
                continue;
            }
            // Создаем новый пакет
            AddFaPackProc->ExecProc();
            String faPackId = MainDataModule->AddFaPackProc->ParamByName("result")->AsString;

            // Добавляем в новый пакет выделенные лицевые счета
            while ( !filter->DataSet->Eof )
            {
                AddFaToPackStopProc->ParamByName("p_acct_id")->AsString = filter->DataSet->FieldByName("acct_id")->AsString;
                AddFaToPackStopProc->ParamByName("p_fa_pack_id")->AsString = faPackId;
                AddFaToPackStopProc->ExecProc();
                filter->DataSet->Next();
            }
            MainDataModule->EsaleSession->Commit();
        }

        getCheckedFilter->DataSet = NULL;
        filter->UnlockDataSetPos();

        // Возвращаем ID нового пакета
        return AddFaToPackStopProc->ParamByName("p_fa_pack_id")->AsString;
    }
    else
    {
        return "";
    }
}

/* Создать реестр уведомлений
*/
String __fastcall TMainDataModule::createPackNotice(TFaPackTypeCd faPackTypeCd)
{
    // Цикл по выделенным строкам
    // из каждой строки по очереди берутся параметры город, улица, контрлер
    // и подставляются в запрос
    // Полученные данные выводятся в шаблон Excel,
    // затем действия повторяются для следующей строки из списка с параметрами
    //_currentDbGrid->DataSource->DataSet->DisableControls();

    //TDataSetFilter* filter = getDebtorsFilter;

    _currentFilter->LockDataSetPos();

    getCheckedFilter->setValue("checked", "param", "1");
    //String k = getCheckedFilter->getFilterString();
    getCheckedFilter->DataSet = _currentFilter->DataSet;

    if (!_currentFilter->DataSet->Eof)  // Если есть отмеченные записи
    {
        // Создаем новый пакет
        AddFaPackProc->ParamByName("p_fa_pack_type_cd")->Value = faPackTypeCd;
        AddFaPackProc->ParamByName("p_acct_otdelen")->Value = _acctOtdelen->Value;
        AddFaPackProc->ExecProc();
        String faPackId = AddFaPackProc->ParamByName("result")->AsString;

        // Цикл
        while ( !_currentFilter->DataSet->Eof )
        {
            AddFaProc->ParamByName("p_acct_id")->AsString = _currentFilter->DataSet->FieldByName("acct_id")->AsString;
            AddFaProc->ParamByName("p_fa_pack_id")->AsString = faPackId;
            AddFaProc->ExecProc();
            _currentFilter->DataSet->Next();

            // Здесь сделать вызов логирующей функции
            Application->ProcessMessages();
        }
        MainDataModule->EsaleSession->Commit();

        getCheckedFilter->DataSet = NULL;
        _currentFilter->UnlockDataSetPos();

        return faPackId;
    }
    else
    {
        return "";
    }
}

/**/
void __fastcall TMainDataModule::closeDebtorsQuery()
{
    // Чтобы при открытии "Всех должников" после создания нового реестра произошло обновление
    getDebtors->ParamByName("acct_otdelen")->Value = "";
    getDebtorsRam->Close();
}

//void __fastcall TMainDataModule::setFilter(const String& filterName, const String& filterValue)
void __fastcall TMainDataModule::setFilterParamValue(TDataSetFilter* filter, const String& filterName, const String& paramName, Variant paramValue)
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
    if ( !dataSet->Active )
    {
        return;
    }
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
void __fastcall TMainDataModule::getApprovalList()
{
    if (getApprovalListQuery->ParamByName("acct_otdelen")->Value != _acctOtdelen->Value)
    {
        getApprovalListQuery->ParamByName("acct_otdelen")->Value = _acctOtdelen->Value;
        openOrRefresh(getApprovalListQuery);
    }
}

/* Отображает список на введение ограничения
   по выбранному участку */
void __fastcall TMainDataModule::getStopList()
{
    if (getStopListQuery->ParamByName("acct_otdelen")->Value != _acctOtdelen->Value)
    {
        getStopListQuery->ParamByName("acct_otdelen")->Value = _acctOtdelen->Value;
        openOrRefresh(getStopListQuery);
    }
}

/* Отображает список реестров на ограничение
   по выбранному участку */
void __fastcall TMainDataModule::getPackStopList()
{
    if (getPackStopListQuery->ParamByName("acct_otdelen")->Value != _acctOtdelen->Value)
    {
        getPackStopListQuery->ParamByName("acct_otdelen")->Value = _acctOtdelen->Value;
        openOrRefresh(getPackStopListQuery);
    }
}


/* Отображает список реестров
   по выбранному участку */
void __fastcall TMainDataModule::getFaPackList(String faPackTypeCd)
{
    /* Это условие возможно нужно убрать,
    так как не обрабатывается ситуация, если случилось создание нового реестра */
//    if (selectFaPackQuery->ParamByName("acct_otdelen")->Value != _acctOtdelen->Value ||
//      selectFaPackQuery->ParamByName("fa_pack_type_cd")->Value != faPackTypeCd)
//    {
        selectFaPackQuery->ParamByName("acct_otdelen")->Value = _acctOtdelen->Value;
        selectFaPackQuery->ParamByName("fa_pack_type_cd")->Value = faPackTypeCd;
        openOrRefresh(selectFaPackQuery);
//    }
    selectFaPackQuery->First();
}

/* Отображает реестр
   по выбранному ID реестра */
void __fastcall TMainDataModule::getFaPack(const String& faPackId/*, String& acctOtdelen*/)
{
    if (getFaPackQuery->ParamByName("p_fa_pack_id")->Value != faPackId)
    {
        getFaPackQuery->ParamByName("p_fa_pack_id")->AsString = faPackId;
        openOrRefresh(MainDataModule->getFaPackQuery);
        //acctOtdelen = getFaPackQuery->FieldByName("acct_otdelen")->AsString;
    }
}


/* При завершении потока */
void __fastcall TMainDataModule::OnShowDebtorsThreadEnd(TDataSet* ds)
{
    if ( _afterOpenDataSet != NULL )
    {
        _afterOpenDataSet(this);
    }

    _threadDataSet = NULL;
}

/* Отображает список должников
   по выбранному участку */
void __fastcall TMainDataModule::getDebtorList()
{
    if (_threadDataSet != NULL)
    {
        return;
    }

    _currentFilter = getDebtorsFilter;
    //TThreadDataSet(false, MainDataModule->getDebtors, &OnShowDebtorsThreadEnd);

    if (getDebtors->ParamByName("acct_otdelen")->Value != _acctOtdelen->Value)
    {
        if (_beforeOpenDataSet != NULL)
        {
            _beforeOpenDataSet(this);
        }
        getDebtors->ParamByName("acct_otdelen")->Value = _acctOtdelen->Value;
        _threadDataSet = new TThreadDataSet(false, getDebtors, getDebtorsRam, &OnShowDebtorsThreadEnd);

        //openOrRefresh(MainDataModule->getDebtors);
    }
}

/* Отображает список на почту
   по выбранному участку */
void __fastcall TMainDataModule::getPostList()
{
    if (_threadDataSet != NULL)
    {
        return;
    }

    _currentFilter  = getPostListFilter;
    //TThreadDataSet(false, MainDataModule->getDebtors, &OnShowDebtorsThreadEnd);

    if (getPostListQuery->ParamByName("acct_otdelen")->Value != _acctOtdelen->Value)
    {
        getPostListQuery->ParamByName("acct_otdelen")->Value = _acctOtdelen->Value;
        openOrRefresh(getPostListQuery);

       /* if (_beforeOpenDataSet != NULL)
        {
            _beforeOpenDataSet(this);
        }
        _threadDataSet = new TThreadDataSet(false, getPostListQuery, getPostListRam, &OnShowDebtorsThreadEnd);
        */
        //openOrRefresh(MainDataModule->getDebtors);
    }

}

/* Задает текущее отделение
   updateOnly = true то меняется только значение _currentAcctOtdelen
*/
void __fastcall TMainDataModule::setAcctOtdelen(const String& acctOtdelen, bool updateOnly)
{
    static String oldAcctOtdelen = "";
    if (oldAcctOtdelen != acctOtdelen)
    {
        oldAcctOtdelen = acctOtdelen;

        // Сделать открытие запроса getOtdelenListQuery с параметром acctOtdelen
        // или переход на соответствующую запись
        // в случае DBGridComboBox ничего не делать

        if (updateOnly == false)
        {
            getFaPackQuery->Close();
            getFaPackInfo->Close();
            getApprovalListQuery->Close();
            getPackStopListQuery->Close();
            getFaPackStopQuery->Close();
            getStopListQuery->Close();
        }
    }
}

void __fastcall TMainDataModule::setFaPack(const String& faPackId)
{
    /* Запрос для хранения текущей информации по пакету */
    static String oldFaPackId = "";

    // Здесь возможно сделать проверку на наличие уже открытого запроса getFaPackInfo
    //if (oldFaPackId )

    getFaPackInfo->ParamByName("fa_pack_id")->AsString = faPackId;
    openOrRefresh(getFaPackInfo);

    setAcctOtdelen(getFaPackInfo->FieldByName("acct_otdelen")->AsString, true);
}


/* Задает текущее отделение по faPackId
   updateOnly = true то меняется только значение _currentAcctOtdelen
*/

/*String __fastcall TMainDataModule::getAcctOtdelenByFaPackId(const String& faPackId)
{
    getFaPackInfo->ParamByName("fa_pack_id")->Value = faPackId;
    openOrRefresh(getFaPackInfo);
    return getFaPackInfo->FieldByName("acct_otdelen")->AsString;
} */


/* Задает текущий пакет
*/
void __fastcall TMainDataModule::setFaPackId_Notice(const String& faPackId)
{

// здесь сделать или код открытия или использовать функцию
//  getFaPack может быть переименовать в openFaPack.....
    if (faPackId == "" ||
        (getFaPackInfo->Active &&  faPackId == getFaPackInfo->FieldByName("fa_pack_id")->AsString))
    {
        return;
    }

    _currentFilter  = getFaPackFilter;

    /* Запрос для списка*/
    getFaPackQuery->ParamByName("fa_pack_id")->AsString = faPackId;
    openOrRefresh(MainDataModule->getFaPackQuery);

    /* Запрос для хранения текущей информации по пакету */
    setFaPack(faPackId);
}

/* Задает текущий пакет
*/
void __fastcall TMainDataModule::setFaPackId_Stop(const String& faPackId)
{
    if (faPackId == "" ||
        (getFaPackInfo->Active &&  faPackId == getFaPackInfo->FieldByName("fa_pack_id")->AsString))
    {
        return;
    }

    _currentFilter  = getPackStopListFilter;

    /* Запрос для списка*/
    getFaPackStopQuery->ParamByName("fa_pack_id")->AsString = faPackId;
    openOrRefresh(getFaPackStopQuery);

    /* Запрос для хранения текущей информации по пакету */
    setFaPack(faPackId);
}

/*
*/
String __fastcall TMainDataModule::getAcctOtdelen()
{
    return getOtdelenListQuery->FieldByName("acct_otdelen")->AsString;
    //return _currentAcctOtdelen;
}

/**/
String __fastcall TMainDataModule::getFaPackId_Notice()
{
    return getFaPackQuery->FieldByName("fa_pack_id")->AsString;
}

/**/
String __fastcall TMainDataModule::getFaPackId_Stop()
{
    return getFaPackStopQuery->FieldByName("fa_pack_id")->AsString;
}

/* Задает функции, вызываемые при начале и по окончанию длительных по времени запросов и процедур
   Служат для синхронизации с графическим интерфейсом
*/
void __fastcall TMainDataModule::setOpenDataSetEvents(TNotifyEvent beforeOpenDataSet, TNotifyEvent afterOpenDataSet)
{
    _beforeOpenDataSet = beforeOpenDataSet;
    _afterOpenDataSet = afterOpenDataSet;
}

/* Событие при изменение фильтра
   Служат для синхронизации с графическим интерфейсом
*/
void __fastcall TMainDataModule::setOnChangeFilterMethod(TNotifyEvent onChangeFilterMethod)
{
    _onChangeFilterMethod = onChangeFilterMethod;
}

/* При изменении фильтра
   Служит для синхронизации с графическим интерфейсом
*/
void __fastcall TMainDataModule::OnFilterChange(
      TDataSetFilter *Sender, AnsiString filterName)
{
    if (_onChangeFilterMethod != NULL)
    {
        _onChangeFilterMethod(this);
    }
}
//---------------------------------------------------------------------------













