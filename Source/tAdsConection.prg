////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções e classes para acesso e conexão ao Dicionario de dados de Advantage                    //
// Qualquer alteração ou modificação encaminhe para Giovany Vecchi                                //
// Giovany Vecchi 04/11/2013 giovanyvecchi@yahoo.com.br / giovanyvecchi@gmail.com                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

#IFNDEF __DOS__

   #include "fivewin.ch"

#ELSE

   #include "hbclass.ch"

  #DEFINE  CRLF CHR(13)+CHR(10)

  #xtranslate MsgInfo(<cMsn>) => Alert( <cMsn> )
  #xtranslate MsgStop(<cMsn>) => Alert( <cMsn> )

   #xcommand DEFAULT <uVar1> := <uVal1> ;
               [, <uVarN> := <uValN> ] => ;
                  If( <uVar1> == nil, <uVar1> := <uVal1>, ) ;;
                [ If( <uVarN> == nil, <uVarN> := <uValN>, ); ]

#ENDIF

#define ADS_INC_USERCOUNT           0x00000001
#define ADS_STORED_PROC_CONN        0x00000002
#define ADS_COMPRESS_ALWAYS         0x00000004
#define ADS_COMPRESS_NEVER          0x00000008
#define ADS_COMPRESS_INTERNET       0x0000000C
#define ADS_REPLICATION_CONNECTION  0x00000010
#define ADS_UDP_IP_CONNECTION       0x00000020
#define ADS_IPX_CONNECTION          0x00000040
#define ADS_TCP_IP_CONNECTION       0x00000080
#define ADS_TCP_IP_V6_CONNECTION    0x00000100
#define ADS_NOTIFICATION_CONNECTION 0x00000200
// Reserved                         0x00000400
// Reserved                         0x00000800
#define ADS_TLS_CONNECTION          0x00001000
#define ADS_CHECK_FREE_TABLE_ACCESS 0x00002000

Static st_nCacheAds
Static St_aConnection, St_nConnectionDefault
Static St_cDirTmp

FUNCTION TADS_START_CONFIG(f_cDirTmp,f_cPassAdsSys)
  Local oConnectionTmp, cDirExpr := "", lCreateTmp := .F.
  Local aStructTmp := {}
  
  Default f_cDirTmp := HB_CURDRIVE()+":\"+CURDIR()+"\DADOSTMP\"
  Default f_cPassAdsSys := ""

  St_cDirTmp := f_cDirTmp 
  
  REQUEST DBFCDX , DBFFPT, DBFDBT
  REQUEST ADS , ADSX, ADSADTX, ADSKeyno, ADSKeycount, AdsGetRelKeyPos,  AdsSetRelKeyPos
  
  hb_rddADSRegister()
  Set( _SET_OPTIMIZE, .T. ) 
  //Set( _SET_AUTORDER, .T. )
  
  //RddRegister( "ADSADTX", 1 )      // ADS for Harbour
  //RddSetDefault( "ADSADTX" )       // ADS for Harbour

  RddRegister( "ADS", 1 )      // ADS for Harbour
  RddSetDefault( "ADS" )       // ADS for Harbour
  AdsLocking( .T. )
  AdsRightsCheck( .T. )
  AdsTestRecLocks( .T. )
  ADSCACHEOPENTABLES( 10 ) // PADRÃO 0
  AdsCacheOpenCursors( 60 ) // PADRÃO 25
  AdsSetDateFormat( "DD/MM/YYYY" )
  AdsSetEpoch("01/01/1990")
  AdsSetFileType( 3 ) /// 1 NTX / 2 CDX / 3 ADT

  AdsSetCharType(1)
  
  #ifdef __XHARBOUR__
    SET(_SET_HARDCOMMIT,.T.)  
  #else
    SET(106,.T.) 
  #endif


  ///SET(43,.F.)

  lMkDir(f_cDirTmp)
  FERASE(f_cDirTmp+"TADS_ERR.ADI")
  If !File(f_cDirTmp+"TADS_TMP.ADD")
    cDirExpr := StrTran(f_cDirTmp,"\","\\")
    AdsSetServerType(1)
    TAds_CreateDataDictionary(cDirExpr+"TADS_TMP.ADD","Dicionario de dados temporario para TAds")
    lCreateTmp := .T.
  EndIf

  oConnectionTmp := tAdsConnection():New(121,.F.)
  oConnectionTmp:cDataDictionary  := f_cDirTmp+"TADS_TMP.ADD"
  oConnectionTmp:cSenhaConnect    := f_cPassAdsSys 
  oConnectionTmp:nTpConnect       := 1
  oConnectionTmp:tAdsConnect()

  If lCreateTmp

    if !Empty(f_cPassAdsSys)
      TAds_ModifyUserProperty(121,"ADSSYS",f_cPassAdsSys)
    EndIf

    aadd(aStructTmp,{"STATUS","Short",2,0,1,"Status para Controle Interno",0})
    aadd(aStructTmp,{"Dt_Ocorrencia","Date",8,0,1,"Data da Ocorrencia do erro",Nil})
    aadd(aStructTmp,{"Hr_Ocorrencia","C",10,0,0,"Horas da Ocorrencia do erro",Nil})
    aadd(aStructTmp,{"cComputerName","C",50,0,0,"Nome do Computador ",Nil})
    aadd(aStructTmp,{"cInfoProcLine","C",100,0,0,"Linha e procedimento da chamada de DsNew()",Nil})
    aadd(aStructTmp,{"nErrorSql","N",7,0,0,"Numero do Erro retornado por AdsGetLastError",Nil})
    aadd(aStructTmp,{"cErrorSql","Memo",8,0,0,"Descricao do Erro retornado por AdsGetLastError",Nil})
    aadd(aStructTmp,{"cSqlScript","Memo",8,0,0,"Script Sql aplicado",Nil})
    aadd(aStructTmp,{"cErrorComplete","Memo",8,0,0,"Descrição completa do erro",Nil})

    TAds_CreateTableFromCode(121,"TADS_ERR",aStructTmp,"Tabela para registros de erros de tAds DataSet")
    
  EndIf
  
RETURN NIL
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_GetPathTemp()

RETURN St_cDirTmp
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_GetConnectionDefault()

RETURN St_nConnectionDefault
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_GetConnectionObj(f_nConexao)

  Default f_nConexao := St_nConnectionDefault

RETURN St_aConnection[f_nConexao]
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_GetConnectionHandle(f_nConexao)

  Default f_nConexao := St_nConnectionDefault

RETURN St_aConnection[f_nConexao]:hConnectHandle
///////////////////////////////////////////////////////////////////////////////
Class tAdsConnection

  Data cDataDictionary    Init ""
  Data cUserLogin         Init "adssys"
  Data cSenhaConnect      Init ""
  Data nTpConnect         Init 7
  Data lConnected         Init .F.
  Data lDefault           Init .F.
  
  Data nConnection, hConnectHandle
  
  Method New() Constructor
  Method tAdsConnect
  Method tAdsCloseConnect()

EndClass
//-----------------------------------------------------------------------------
Method New(f_nConnection,f_lDefault) Class tAdsConnection

  Default f_nConnection := 1, f_lDefault := .F.
  
  ::nConnection   := f_nConnection
  ::lDefault      := f_lDefault

  If ::lDefault
    St_nConnectionDefault := f_nConnection
  EndIf
  
  If Hb_IsNil(St_aConnection)
    St_aConnection          := Array(121)
  EndIf

  If Hb_IsNil(St_nConnectionDefault)
    ::lDefault              := .T.
    St_nConnectionDefault   := f_nConnection
  EndIf

  St_aConnection[f_nConnection] := Self

Return Self
//-----------------------------------------------------------------------------
Method tAdsConnect() Class tAdsConnection
  lOCAL lConectou := .F.
  
  
  lConectou := AdsConnect60( ::cDataDictionary,;
                             ::nTpConnect,;
                             ::cUserLogin,;
                             ::cSenhaConnect,;
                             (ADS_COMPRESS_INTERNET+;
                              ADS_TCP_IP_CONNECTION),;
                             @::hConnectHandle)

                             ///(0x0000000C+0x00000010),;


  ::cSenhaConnect := "0192837465"
  
  If !lConectou
    Return .F.
  EndIf
 
  ADSCACHEOPENTABLES( 12 ) //64
  AdsCacheOpenCursors( 128 ) // 128
  
  ::lConnected    := .T.
  
Return .T.
//-----------------------------------------------------------------------------
Method tAdsCloseConnect() Class tAdsConnection
  Local hConnectionFocus := AdsConnection()
  Local hConnectionDefault := tAds_GetConnectionObj(St_nConnectionDefault):hConnectHandle
  
  ///? hConectionFocus, ::hConnectHandle, tAds_GetConnectionObj(1):hConnectHandle
  
  AdsConnection(::hConnectHandle)
  
  AdsCloseCachedTables(::hConnectHandle)

  //AdsCloseAllTables()
  If !AdsDisconnect(::hConnectHandle)
    AdsDisconnect(::hConnectHandle)
  EndIf
  hb_idleSleep(.5)
      
  If ::hConnectHandle <> hConnectionFocus
    AdsConnection(hConnectionFocus)
  Else
    If !::lDefault
      AdsConnection(hConnectionDefault)
    EndIf
  EndIf
  
  ::lConnected := .F.
  
Return NIL
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION nCacheAds()

  If HB_ISNIL(st_nCacheAds)
    st_nCacheAds := 100
  EndIf

Return st_nCacheAds

