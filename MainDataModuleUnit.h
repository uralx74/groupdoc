//---------------------------------------------------------------------------
/*
   Модуль для работы с базой данных
*/
#ifndef MainDataModuleUnitH
#define MainDataModuleUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DBAccess.hpp"
#include "Ora.hpp"
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <DB.hpp>
#include "MemDS.hpp"
#include "OtdelenComboBoxFrameUnit.h"
#include "VirtualTable.hpp"
#include "DataSetFilter.h"
#include "formlogin.h"
#include "ThreadDataSet.h"
#include <DBClient.hpp>
#include "OraSmart.hpp"
#include <Provider.hpp>


namespace TCheckTypes
{
enum Type
{
    CT_UNDEFINED = 0,
    CT_CHECKED,
    CT_UNCHECKED
};
};

class DualList
{
private:
    TStringList* _descrList;
    TStringList* _valueList;
    TOraQuery* _sourceQuery;
    void setUsed();
    bool _used;

public:
    DualList();
    ~DualList();
    void add(const String& descr, const String& value);
    void addFromQuery(TOraQuery* listQuery);
    void setSourceQuery(TOraQuery* query);
    String getValue(int index);
    String getValue(TComboBox* comboBox);
    void assignTo(TComboBox* comboBox);
};


namespace TUserRole
{
enum TRoleType
{
    UNDEFINED = 10,
    OPERATOR = 20,
    APPROVER = 30,
    ADMINISTRATOR = 40
};

typedef Set<TRoleType, UNDEFINED, ADMINISTRATOR> TRoleTypes;
}



namespace TCcTypeCd
{
enum Type
{
    MANUAL = 10,
    LETTER = 20,
    PHONE_CALL = 30
};
}

namespace TCcStatusFlg
{
enum Type
{
    UNDEFINED = 10,
    SUCCESS = 20,
    FAILED = 30
};
}

namespace TCcSourceTypeCd
{
enum Type
{
    FA = 10,
    OTHER = 20
};
}

namespace TPackTypeCd
{
enum Type
{
    PACK_NULL = 10,
    PACK_MANUAL = 20,
    PACK_POST = 30,
    PACK_STOP = 40
};
}

//---------------------------------------------------------------------------
class TMainDataModule : public TDataModule
{
__published:	// IDE-managed Components
    TOraSession *EsaleSession;
    TOraSession *CcbSession;
    TOraQuery *GeneralFaList;
    TOraDataSource *getDebtorsDataSource;
    TOraQuery *GeneralDebtorList;
    TOraStoredProc *AddFaProc;
    TOraStoredProc *AddFaPackProc;
    TOraQuery *getFaPackQuery;
    TOraDataSource *selectFaPackDataSource;
    TOraQuery *selectFaPackQuery;
    TOraQuery *getOtdelenList_old;
    TOraQuery *getDebtors;
    TOraQuery *OraQuery2;
    TFloatField *OraQuery2ROWNUM;
    TStringField *OraQuery2ACCT_ID;
    TStringField *OraQuery2FIO;
    TStringField *OraQuery2CITY;
    TStringField *OraQuery2ADDRESS;
    TFloatField *OraQuery2SALDO_UCH;
    TFloatField *OraQuery2SALDO_M3;
    TDateTimeField *OraQuery2CC_DTTM;
    TStringField *OraQuery2FA_ID;
    TStringField *OraQuery2FA_PACK_ID;
    TDateTimeField *OraQuery2CRE_DTTM;
    TStringField *OraQuery2ACCT_OTDELEN;
    TStringField *OraQuery2SERVICE_ORG;
    TOraDataSource *getFaPackDataSource;
    TFloatField *getDebtorsROWNUM;
    TStringField *getDebtorsACCT_ID;
    TStringField *getDebtorsFIO;
    TStringField *getDebtorsCITY;
    TStringField *getDebtorsADDRESS;
    TFloatField *getDebtorsSALDO_UCH;
    TFloatField *getDebtorsSALDO_M3;
    TDateTimeField *getDebtorsCC_DTTM;
    TStringField *getDebtorsFA_ID;
    TStringField *getDebtorsFA_PACK_ID;
    TDateTimeField *getDebtorsCRE_DTTM;
    TStringField *getDebtorsACCT_OTDELEN;
    TStringField *getDebtorsSERVICE_ORG;
    TFloatField *getDebtorsCHECK_DATA;
    TOraStoredProc *addCcProc;
    TOraStoredProc *setCcStatusFlgProc;
    TOraQuery *getApprovalListQuery;
    TFloatField *FloatField1;
    TFloatField *FloatField2;
    TStringField *StringField1;
    TStringField *StringField2;
    TStringField *StringField3;
    TStringField *StringField4;
    TDateTimeField *DateTimeField1;
    TOraDataSource *getApprovalListDataSource;
    TOraStoredProc *setCcApprovalDttmProc;
    TStringField *getApprovalListQueryCC_ID;
    TStringField *getApprovalListQuerySRC_TYPE_CD;
    TDateTimeField *getApprovalListQueryAPPROVAL_DTTM;
    TStringField *getApprovalListQueryCITY;
    TStringField *getApprovalListQuerySERVICE_ORG;
    TDataSetFilter *getFaPackFilter;
    TDataSetFilter *selectFaPackFilter;
    TDataSetFilter *getApprovalListFilter;
    TDataSetFilter *getDebtorsFilter;
    TDataSetFilter *getCheckedFilter;
    TOraQuery *getCcStatusFlgListQuery;
    TOraDataSource *getCcStatusFlgListDataSource;
    TOraQuery *getCcTypeCdQuery;
    TOraDataSource *getCcTypeCdDataSource;
    TStringField *getCcStatusFlgListQueryDESCR;
    TStringField *getCcStatusFlgListQueryFIELD_VALUE;
    TStringField *getCcTypeCdQueryDESCR;
    TStringField *getCcTypeCdQueryFIELD_VALUE;
    TOraDataSource *getStopListDataSource;
    TOraQuery *getStopListQuery;
    TDataSetFilter *getStopListFilter;
    TFloatField *getStopListQueryROWNUM;
    TFloatField *getStopListQueryCHECK_DATA;
    TStringField *getStopListQueryACCT_ID;
    TStringField *getStopListQueryFIO;
    TStringField *getStopListQueryPOSTAL;
    TStringField *getStopListQueryCITY;
    TStringField *getStopListQueryADDRESS;
    TStringField *getStopListQueryACCT_OTDELEN;
    TFloatField *getStopListQuerySALDO_UCH;
    TDateTimeField *getStopListQueryAPPROVAL_DTTM;
    TDateTimeField *getStopListQueryCC_DTTM;
    TOraDataSource *getOtdelenListDataSource;
    TFloatField *getFaPackQueryROWNUM;
    TFloatField *getFaPackQueryCHECK_DATA;
    TStringField *getFaPackQueryFA_ID;
    TStringField *getFaPackQueryACCT_ID;
    TStringField *getFaPackQueryADDRESS;
    TStringField *getFaPackQueryCITY;
    TStringField *getFaPackQueryFIO;
    TDateTimeField *getFaPackQueryCC_DTTM;
    TStringField *getFaPackQuerySRC_TYPE_CD;
    TStringField *getFaPackQuerySERVICE_ORG;
    TStringField *getFaPackQueryPHONES;
    TFloatField *getFaPackQueryEND_REG_READING1;
    TFloatField *getFaPackQueryEND_REG_READING2;
    TFloatField *getFaPackQuerySALDO_UCH;
    TStringField *getFaPackQueryMTR_SERIAL_NBR;
    TStringField *getFaPackQueryFA_PACK_ID;
    TDateTimeField *getFaPackQueryCRE_DTTM;
    TOraQuery *OraQuery3_old;
    TFloatField *FloatField4;
    TStringField *StringField5;
    TStringField *StringField6;
    TStringField *StringField7;
    TStringField *StringField8;
    TFloatField *FloatField5;
    TFloatField *FloatField6;
    TDateTimeField *DateTimeField2;
    TStringField *StringField9;
    TStringField *StringField10;
    TDateTimeField *DateTimeField3;
    TStringField *StringField11;
    TStringField *StringField12;
    TFloatField *FloatField7;
    TOraQuery *getOtdelenListQuery;
    TFloatField *selectFaPackQueryROWNUM;
    TFloatField *selectFaPackQueryCHECK_DATA;
    TDateTimeField *selectFaPackQueryCRE_DTTM;
    TStringField *selectFaPackQueryACCT_OTDELEN;
    TStringField *selectFaPackQueryFA_PACK_ID;
    TStringField *selectFaPackQueryFA_PACK_TYPE_CD;
    TOraQuery *getApprovalListQuery_old;
    TFloatField *FloatField8;
    TFloatField *FloatField9;
    TStringField *StringField13;
    TStringField *StringField14;
    TStringField *StringField15;
    TStringField *StringField16;
    TFloatField *FloatField10;
    TDateTimeField *DateTimeField4;
    TStringField *StringField17;
    TStringField *StringField18;
    TDateTimeField *DateTimeField5;
    TStringField *StringField19;
    TStringField *StringField20;
    TFloatField *getApprovalListQuerySALDO_UCH;
    TStringField *getFaPackQueryACCT_OTDELEN;
    TStringField *getFaPackQueryPHONE;
    TStringField *getFaPackQueryPOSTAL;
    TOraQuery *getFaPackInfo;
    TFloatField *getFaPackInfoROWNUM;
    TDateTimeField *getFaPackInfoSYSDATE;
    TDateTimeField *getFaPackInfoCRE_DTTM;
    TStringField *getFaPackInfoACCT_OTDELEN;
    TStringField *getFaPackInfoFA_PACK_ID;
    TStringField *getFaPackInfoACCT_OTDELEN_DESCR;
    TStringField *getFaPackInfoADDRESS;
    TStringField *getFaPackInfoPOST;
    TStringField *getFaPackInfoPHONE;
    TStringField *getFaPackInfoNACHALNIK;
    TStringField *getFaPackInfoVISA;
    TStringField *getDebtorsOP_AREA_DESCR;
    TStringField *getFaPackQueryOP_AREA_DESCR;
    TOraQuery *getConfigQuery;
    TOraQuery *OraQuery33333333333333;
    TFloatField *FloatField3;
    TStringField *StringField27;
    TStringField *StringField28;
    TStringField *StringField29;
    TStringField *StringField30;
    TFloatField *FloatField11;
    TFloatField *FloatField12;
    TDateTimeField *DateTimeField6;
    TStringField *StringField31;
    TStringField *StringField32;
    TDateTimeField *DateTimeField7;
    TStringField *StringField33;
    TStringField *StringField34;
    TFloatField *FloatField13;
    TStringField *StringField35;
    TOraQuery *getFaPackStopQuery;
    TFloatField *FloatField14;
    TFloatField *FloatField15;
    TStringField *StringField36;
    TStringField *StringField37;
    TStringField *StringField38;
    TStringField *StringField39;
    TStringField *StringField40;
    TDateTimeField *DateTimeField8;
    TStringField *StringField41;
    TStringField *StringField43;
    TFloatField *FloatField16;
    TFloatField *FloatField17;
    TFloatField *FloatField18;
    TStringField *StringField44;
    TStringField *StringField45;
    TDateTimeField *DateTimeField9;
    TStringField *StringField46;
    TStringField *StringField47;
    TStringField *StringField48;
    TStringField *StringField49;
    TOraDataSource *getFaPackStopDataSource;
    TDataSetFilter *getFaPackStopFilter;
    TStringField *getFaPackQueryFA_PACK_TYPE_CD;
    TStringField *getFaPackStopQueryFA_PACK_TYPE_CD;
    TFloatField *getFaPackStopQueryGRP;
    TOraQuery *getFaPackStopInfoQuery;
    TStringField *getFaPackStopQueryPREM_TYPE_DESCR;
    TFloatField *getFaPackStopInfoQueryROWNUM;
    TDateTimeField *getFaPackStopInfoQuerySYSDATE;
    TDateTimeField *getFaPackStopInfoQueryCRE_DTTM;
    TStringField *getFaPackStopInfoQueryACCT_OTDELEN;
    TStringField *getFaPackStopInfoQueryFA_PACK_ID;
    TStringField *getOtdelenListQueryFNAME;
    TIntegerField *getOtdelenListQueryACCT_OTDELEN;
    TStringField *getOtdelenListQueryADDRESS;
    TStringField *getOtdelenListQueryNACHALNIK;
    TStringField *getOtdelenListQueryPHONE;
    TStringField *getOtdelenListQueryCCB_ACCT_CHAR_VAL;
    TStringField *getOtdelenListQueryVISA;
    TStringField *getOtdelenListQueryPOST;
    TOraQuery *getPackStopListQuery;
    TOraDataSource *getPackStopListDataSource;
    TDataSetFilter *getPackStopListFilter;
    TStringField *getPackStopListQueryFA_PACK_ID;
    TDateTimeField *getPackStopListQueryCRE_DTTM;
    TFloatField *getPackStopListQueryACCT_ID_CNT;
    TDateTimeField *getPackStopListQueryST_P_DT;
    TFloatField *getPackStopListQueryROWNUM;
    TFloatField *getPackStopListQueryCHECK_DATA;
    TStringField *getStopListQuerySERVICE_ORG;
    TStringField *getPackStopListQuerySPR_DESCR;
    TOraStoredProc *AddFaToPackStopProc;
    TDateTimeField *getFaPackStopQueryST_P_DT;
    TStringField *getFaPackStopQuerySPR_DESCR;
    TFloatField *getStopListQueryGRP_DATA;
    TFloatField *getStopListQueryGRP_DATA_MAX;
    TDateTimeField *getConfigQuerySYSDATE;
    TStringField *getConfigQueryAPP_PATH;
    TStringField *getConfigQueryVISA_PATH;
    TStringField *getConfigQueryREPORT_PATH;
    TStringField *getConfigQueryUSERNAME;
    TVirtualTable *getDebtorsRam;
    void __fastcall DataModuleCreate(TObject *Sender);
private:	// User declarations
    bool __fastcall Auth();
    String __fastcall createPackNotice(TDataSetFilter* filter, const String& acctOtdelen);
    String __fastcall createPackStop(TDataSetFilter* filter, const String& acctOtdelen);
    //TNotifyEvent FOnQueryAfterExecuteEvent;

    void __fastcall OnShowDebtorsThreadEnd(TDataSet* ds);
    TThreadDataSet* _threadDataSet; // Поток для открытия запросов

    /* Синхронизация с графическим интерфейсом */
    TNotifyEvent _afterOpenDataSet;
    TNotifyEvent _beforeOpenDataSet;


public:		// User declarations
    __fastcall TMainDataModule(TComponent* Owner);
    void __fastcall connectEsale();


    void __fastcall CopyDataSet(TDataSet* sourceDs, TDataSet* destinationDs);
    void openOrRefresh(TOraQuery* query);

    //void __fastcall setAfterOpenEvent(TNotifyEvent event);
    void __fastcall setOpenDataSetEvents(TNotifyEvent beforeOpenDataSet, TNotifyEvent afterOpenDataSet);



    TUserRole::TRoleTypes userRole; // Роль пользоваля
    String username;    // Имя пользователя

    //DualList otdelenList;
    //DualList faTypesList;
    //DualList faTypesAndDebtorList;

    String addCc(TDataSet* ds, TDateTime ccDttm, const String& acct_id, const String& descr,
        const String& srcId, const String& caller,
        TCcTypeCd::Type ccTypeCd,
        TCcStatusFlg::Type ccStatusFlg,
        TCcSourceTypeCd::Type ccSourceTypeCd);

    String setCcApprovalDttm(String ccId, bool cancelApproval = false);
    void setCcApprovalDttmSelected(bool cancelApproval = false);

    String __fastcall createPackNotice();
    String __fastcall createPackStop();

    String __fastcall createPackMulti(TDataSetFilter* filter, TPackTypeCd::Type packTypeCd, const String& acctOtdelen);
    //String __fastcall createPack(TPackTypeCd::Type packTypeCd);

    /* Connect with form */
    //void __fastcall setFilter(const String& filterName, const String& filterValue);
    void __fastcall setFilterParamValue(TDataSetFilter* filter, const String& filterName, const String& paramName, Variant paramValue);

    void __fastcall selectCcDttmIsNull(TDataSetFilter* filter);
    void __fastcall selectCcDttmMoreThanThree(TDataSetFilter* filter);
    void __fastcall setCheckAll(TDataSet* dataSet, bool value);

    /* Отображаемы списки */
    void __fastcall getDebtorList(const String& acctOtdelen);
    void __fastcall getApprovalList(const String& acctOtdelen);
    void __fastcall getStopList(const String& acctOtdelen);
    void __fastcall getPackStopList(const String& acctOtdelen);

    /* Вспомогательные запросы */
    //void __fastcall getFaPack(const String& faPackId, String& acctOtdelen);
    void __fastcall getFaPack(const String& faPackId);


    void __fastcall getFaPackList(String acctOtdelen, String faPackTypeCd);

    //void __fastcall clearAllFilters();

    /**/
    void __fastcall setAcctOtdelen(const String& acctOtdelen, bool updateOnly = false);
    String __fastcall getAcctOtdelen();

    void __fastcall setFaPackId_Notice(const String& faPackId);
    void __fastcall setFaPackId_Stop(const String& faPackId);
    String __fastcall getFaPackId_Notice();
    String __fastcall getFaPackId_Stop();

    String _currentFaPackTypeCd;      // Тип текущего реестр
    String _currentFaPackId;      // Текущий реестр
    String _currentAcctOtdelen;     // Текущий участок

};
//---------------------------------------------------------------------------
extern PACKAGE TMainDataModule *MainDataModule;
//---------------------------------------------------------------------------
#endif
