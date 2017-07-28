////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe principal para manipular funções de RDDADS estilo oop (programação orientada ao objeto) //
// Qualquer alteração ou modificação encaminhe para Giovany Vecchi                                //
// Giovany Vecchi 18/12/2011 giovanyvecchi@yahoo.com.br / giovanyvecchi@gmail.com                 //
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

#ifdef __HARBOUR__
   #ifndef __XHARBOUR__
      #xtranslate DbSkipper => __DbSkipper
   #endif
#endif

#define HKEY_CLASSES_ROOT        2147483648
#define HKEY_CURRENT_USER        2147483649
#define HKEY_LOCAL_MACHINE       2147483650
#define HKEY_USERS               2147483651
#define HKEY_PERFORMANCE_DATA    2147483652
#define HKEY_CURRENT_CONFIG      2147483653
#define HKEY_DYN_DATA            2147483654

#Define ADS_DYNAMIC_AOF           00
#Define ADS_RESOLVE_IMMEDIATE     01
#Define ADS_RESOLVE_DYNAMIC       02
#Define ADS_KEYSET_AOF            04
#Define ADS_FIXED_AOF             08
#define ADS_KEEP_AOF_PLAN         10

#define ADS_RESPECTFILTERS                1
#define ADS_IGNOREFILTERS                 2
#define ADS_RESPECTSCOPES                 3
#define ADS_REFRESHCOUNT                  4


STATIC st_aCtrlAlias_Tabelas, st_aCtrlAlias_Monitor, St_SifraoMoeda
STATIC St_nRecnoNew
STATIC st_lProcessAutoAlias

FUNCTION tAds_Version() 

RETURN "1707"

CLASS TAds

  DATA aStructTads    AS ARRAY INIT {}
  DATA nTpCallRdd     AS NUMERIC INIT 1 // 1-Open newRdd of Direct line / 2-Open Newrdd of Class Table
  DATA nOpenType, nOpenStatus, cOpenStatus, nOpenAliasCtrl, nConnectionType
  DATA aBuffersField
  DATA cAlias, cTableAliasTmp, cTableName
  DATA lBof, lEof
  DATA cFilterMask, aVarsFormat
  DATA nFocusRecno, nRecnoLastInsert, nRecnoLastAppend, nRecnoVarPutSet, nRecnoDataBuffer
  DATA cOrderFocus, nKeyNoCount
  Data aFilterFix
  Data aArrayRecnos
  Data lBufferOnSkip      AS LOGIC INIT .F.
  Data lBufferRddToTemp   AS LOGIC INIT .F. 
  Data cFilterLast        INIT ""
  Data cAliasBufferRdd    
 
  METHOD NewRdd(f_cTableName,f_nConexao,f_nCache, f_lExclusive)

  METHOD End()
  
  Method DataLoad(f_lRefreshRecord)   // Carrega / Atualiza as variaveis DATA referente ao registro focalizado
  Method DataBlank()       // Carrega / AStualiza as variaveis DATA com valores em branco
  Method DataSave(f_aIgnoreFields) // Salva / grava o conteudo das variaveis DATA para o registro focalizado
                                   // f_aIgnoreFields > Vetor com os nomes dos campos que não serão atualizados na tabela 
  Method DataLoadToFR(f_oFastRep,f_cTituloGrupo)   // Carrega as variaveis Data e cria Grupo np objeto Fast Report

  Method FieldInfo(f_cCampo) // Retorna vetor com dados da estrutura do campo
  Method FieldPos(f_cCampo) // Retorna a Posição do campo na tabela
  Method FieldBlank(f_cCampo) // Retorna as caracteristica do campo em branco
  Method FieldSum(f_cCampo) // Soma a tabela referente ao campo 

  METHOD Lock(f_nRecno)
  METHOD RLock(f_nRecno)
  METHOD UnLock(f_nRecno)
  METHOD IsRecordLocked(f_nRecno)

  METHOD OnSkip(f_lData)
  METHOD Skip(f_nSkip)
  METHOD GoTop()
  METHOD GoBottom()
  METHOD GoTo(f_nRecno)
  METHOD Bof() INLINE (::cAlias)->(Bof())
  METHOD Eof() INLINE (::cAlias)->(Eof())
  METHOD LastRec() INLINE (::cAlias)->(LastRec())
  Method KeyCount(f_nModo)
  Method KeyNo()
  Method KeyGoTo(f_nKey) 

  
  //Method GetRelKeyNum() INLINE (::cAlias)->(AdsGetRelKeyNum())
  Method GetRelKeyPos() INLINE (::cAlias)->(AdsGetRelKeyPos())
  Method SetRelKeyPos(f_nNewPos) INLINE (::cAlias)->(AdsSetRelKeyPos(f_nNewPos))

  METHOD Append()
  METHOD Delete(f_lLock)

  METHOD Refresh()

  METHOD VarGet(f_cCampo)
  Method VarGetFieldNull(f_cCampo) // Retorna valor do campo vazio
  METHOD VarGetAlltrim(f_cCampo) // Le o campo tipo caracter e limpa os brancos de ambos os lados
  METHOD VarGetRtrim(f_cCampo) // Le o campo tipo caracter e limpa os brancos a direita
  METHOD VarGetStrZero(f_cCampo,f_nZeros) // Le o campo tipo numerico e preenche com zeros
  Method VarGetDivZero(f_cCampo) // Nunca retorna 0 Zero
  METHOD VarGetTransform(f_cCampo,f_cMask) // Le o campo tipo numerico e formata
  Method VarGetSubStr(f_cCampo,f_nCharStart, f_nCharCount) //Retorna uma parte do conteudo do campo caracter
  METHOD VarGetCleanChars(f_cCampo,f_aCharsClean) // Le o campo tipo caracter e apaga caracteres solicitados
  Method VarGetPadL(f_cCampo,nLength) // Le campo Numerico, Data ou Caracter e preenche com espacos a Esquerda
  Method VarGetPadR(f_cCampo,nLength) // Le campo Numerico, Data ou Caracter e preenche com espacos a Direita

  METHOD VarGetNumToChar(f_cCampo,f_nCasasDecimais,f_lFixCasasDecimais)
      // f_cCampo > Campo da tabela
      // f_nCasasDecimais >  Casas decimais a apresentar, Se f_lFixCasasDecimais for verdadeiro,
      //                mostrara zeros conforme o numero de casas decimais, senão
      //                sera suprido os zeros.
      //                Ex: 102.20 Varget("CAMPO",2,.T.) > Resulta 102,20
      //                Ex: 102.20 Varget("CAMPO",2,.F.) > Resulta 102,2
      // f_lFixCasasDecimais > Se é para fixar a quantidade da casas decimais / padrão .F.

  METHOD VarGetDateToChar(f_cCampo,f_nTpConvert)
      // f_cCampo > Campo da tabela
      // f_nTpConvert >    Tipo de conversão do campo data
      //              0-Converte para numeros ( 31/12/2010 ) padrão
      //              1-Converte em extenso simples (31 de Dezembro de 2010)
      //              2-Converte em extenso completo ( Sexta feira, 31 de Dezembro de 2010 )

  METHOD VarGetYear(f_cCampoDt,f_lStrReturn)
      // f_cCampoDt > Campo da tabela do Tipo Date
      // f_lStrReturn >  Tipo de retorno da Variavel ( Padrão .F.)
      //            .F. - Retorna Numerico Ex: 2012 / padrão
      //            .T. - Retorna String Ex: "2012"

  METHOD VarGetNumToMoeda(f_cCampo,f_nCasasDecimais,f_lNulo,f_nEspacoPreenche,f_cCharEspaco,f_cSifrao)
      // f_cCampo > Campo da tabela
      // f_nCasasDecimais >  Quantidade de casas decimais / Padrão 2
      //  f_lNulo >        Se o Valor for 0,00 retornar "" nulo
      //  f_nEspacoPreenche >  Preenche com espaços a frente para determinar sempre o mesmo tamanho
      //  f_cCharEspaco >    Caracter para preencher os espaços. / Padrão " "
      // f_cSifrao >       Sigla do sifrao da moeda. / Padrão R$

  METHOD VarGetNumToMoedaExtenso(f_cCampo,f_nTpConvert) 
      // f_cCampo > Campo da tabela
      // f_nTpConvert >    Tipo de conversão do campo numerico
      //              0-Converte em extenso simples (Cento e Dois Reais) PADRÃO
      //              1-Converte em extenso completo " R$ 102,00 (Cento e Dois Reais) "

  METHOD VarGetAtokens(f_cCampo,f_cCharSeparator)
   
  METHOD VarPut(f_cCampo,f_uVal,f_cOperador)
  METHOD VarPutDbfTmp(f_cCampo,f_cAliasDbf)

  METHOD Commit(f_lUnlock)

  METHOD SetOrder(f_uOrder)
  METHOD Seek(f_uOcorrencia,f_cTagOpcional,f_lAproximado,f_lReturnLastOrder)
  Method Locate(f_cCampo,f_uValor)

  METHOD Recno()
  Method aGetRecnos()

  METHOD Scope(f_cTagIndex,f_uTopCondicao,f_uEofCondicao)
  METHOD Filter(f_cFilterMask,f_aVarsFormat,f_nTpFiltro)
  METHOD FilterFts(f_cCampo,f_aStrings)
  METHOD AddFilterFix(f_cFilterMask,f_aVarsFormat)
  Method AddRecnosInFilter(f_aRecnos)
  METHOD FilterRefresh()
  METHOD RefreshFilter()
  METHOD CommandFormat(f_cStrCommand,f_aVarsFormat,f_nOpenType)
  METHOD GetFilter() Inline (::cAlias)->(AdsGetAof())
  METHOD ClearFilter(f_lFilterFixRemove)
  Method IsRecordInFilter(f_nRecno)

  METHOD Blob2File(f_cFileDestino,f_cFieldBlobName)
  METHOD Blob2ZipFiles(f_cDirDescomp,f_cFieldBlobName,f_bBlock,f_cPassWord)
  METHOD File2Blob(f_cFileOrigem,f_cFieldBlobName)
  METHOD Files2BlobZip(f_aFilesToCompress,f_cFieldBlobName,f_nLeverCompress,f_bBlock,f_lOverWrite,f_cPassWord)
  
  METHOD CreateIndex( f_cFileBagName, f_cTagName, f_cKeyName, f_bKey, f_lUnique)
  
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  /////// tDataSet

  Data oConexao, nQryTipo
  Data cQrySql, aVarsSql, cDsTxtAlias, cQrySqlLast
  Data nErrorSql, lDsErrorSave, lLogDsExecute
  Data lQrySelectStart
  Data lDsCursorsToTemp, lDsTempEraseOnClose
  Data lDsCursorsToArray, aDsCursorsData, aDsCursorsHeaders 
  Data aFieldsTempIndex
  Data cDsWhere                 Init ""
  Data cDsOrderBy               Init ""
  Data lDsStaticCursors         Init .F. // Se {STATIC} ou INNER JOIN identifica se a tabela é estatica

  METHOD DsNew( f_nQryTipo, f_nConexao ) 
      // f_nQryTipo   > Tipo da Query
      //                1-Quando há retorno de cursor ou handle EX: SELECT
      //                2-Sem retorno de cursors da tabela EX: INSERT, UPDATE, DELETE
      // f_nConexao   > Numero da Conexão - Default tAds_GetConnectionDefault() 
      
  METHOD DsExecute(f_nCache)
  METHOD DsError(f_nErrorSql,f_cErrorSql,f_cSqlScript,f_cErrorComplete)
  METHOD DsCursorsToArray()
  METHOD DsCursorsToTemp()
  METHOD DsAddVar(f_cTxtVarInQuery,f_uVarBlock)
  METHOD DsSetVar(f_cTxtVarInQuery,f_uVarBlock)
  ///METHOD DataSetFillRecno()

ENDCLASS
//-----------------------------------------------------------------------------
METHOD NewRdd(f_cTableName,f_nConexao, f_nCache, f_lExclusive) Class TAds
  LOCAL cComando := "", nIFor := 0, oSelf,;
      aTmpChar := {}, cTmpAliasName := "", nTmpAliasExistentePosition := 0, nTmpAliasNewPosition := 0
  Local aDatas := {}, aMethods := {}

  DEFAULT f_nConexao := tAds_GetConnectionDefault(), f_nCache := nCacheAds(), f_lExclusive := .F.  ///360

  ::oConexao := tAds_GetConnectionObj(f_nConexao)
  
  ///AdsConnection(::oConexao:hConnectHandle)

  St_nRecnoNew := 0

  If Hb_IsNil(st_lProcessAutoAlias)
    st_lProcessAutoAlias := .F.
  EndIf
  Do While st_lProcessAutoAlias
    hb_idleSleep(0.1)
  EndDo

  st_lProcessAutoAlias := .T.

  If Hb_IsNil(St_SifraoMoeda)
    St_SifraoMoeda := "R$"
  EndIf

  if Hb_IsNil(st_aCtrlAlias_Tabelas)
    st_aCtrlAlias_Tabelas  := {}
    st_aCtrlAlias_Monitor  := {}
  EndIf

  ::nOpenAliasCtrl := 0
  ::cTableName := f_cTableName

  For nIfor := 1 to 9999
    ///cTmpAliasName := SubStr(f_cTableName,1,8)+StrZero(nIFor,2)
    cTmpAliasName := "RDD"+StrZero(::nTpCallRdd,1)+"_"+StrZero(nIFor,4)
    nTmpAliasExistentePosition := aScan(st_aCtrlAlias_Tabelas,cTmpAliasName)
    If nTmpAliasExistentePosition == 0
      Exit
    EndIf
  Next

  cTmpAliasName := cTmpAliasName

  nTmpAliasNewPosition := aScan(st_aCtrlAlias_Tabelas,"_EMPTY_")

  If nTmpAliasNewPosition == 0
    aadd(st_aCtrlAlias_Tabelas,cTmpAliasName)
    aadd(st_aCtrlAlias_Monitor,cTmpAliasName+" - "+ProcName(::nTpCallRdd)+" ("+AllTrim(Str(ProcLine(::nTpCallRdd)))+")")
    nTmpAliasNewPosition := aScan(st_aCtrlAlias_Tabelas,cTmpAliasName)
  Else
    st_aCtrlAlias_Tabelas[nTmpAliasNewPosition]  := cTmpAliasName
    st_aCtrlAlias_Monitor[nTmpAliasNewPosition]  := (cTmpAliasName+" - "+ProcName(::nTpCallRdd)+" ("+AllTrim(Str(ProcLine(::nTpCallRdd)))+") - "+dToc(Date())+"/"+Time())
  EndIf

  ::nOpenAliasCtrl := nTmpAliasNewPosition

  st_lProcessAutoAlias := .F.

  ::nRecnoLastInsert   := 0
  ::nRecnoLastAppend   := 0
  ::nRecnoVarPutSet    := 0
  ::nRecnoDataBuffer   := 0

  ::nOpenType        := 1 // RDD
  ::nOpenStatus      := 0
  ::cOpenStatus      := "0 - Alias do recurso de dados aberto com sucesso"
  ::cAlias           := cTmpAliasName
  ::aFilterFix       := {}
  ::aBuffersField    := {}

  IF !ADSCHECKEXISTENCE(f_cTableName,::oConexao:hConnectHandle)
     MSGSTOP("Não foi encontrado a tabela "+f_cTableName+" "+;
             "na lista dos recursos do Data Dictionary para conexão."+CRLF+;
             "A inicialização será finalizada.",;
             "Erro, Ausência de Tabela...")
     ::nOpenStatus := 1
     ::cOpenStatus := "1 - Tabela não encontrada: "+f_cTableName
      LogFile("tAdsError.log",{"NewRdd() 1 - Tabela não encontrada: "+f_cTableName,;
              "Alias "+cTmpAliasName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
     RETURN Self
  ENDIF

 AdsConnection(::oConexao:hConnectHandle)

  If f_lExclusive .AND. f_nConexao == 121
    For nifor := 1 to 20
      USE (f_cTableName) ALIAS (cTmpAliasName) NEW EXCLUSIVE
      
      If NetErr()
        ::nOpenStatus := 3
        ::cOpenStatus := "3 - Erro acesso Exclusivo tabela: "+f_cTableName
        LogFile("tAdsError.log",{"NewRdd() 3 - Erro acesso Exclusivo tabela: "+f_cTableName,;
                "Alias "+cTmpAliasName+" - Ifor "+Alltrim(Str(niFor)), ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
        hb_idleSleep(.3)
        //Return Self
      Else
        ::nOpenStatus      := 0
        ::cOpenStatus      := "0 - Alias do recurso de dados aberto com sucesso"
        Exit
      EndIf
    Next

  ElseIf f_lExclusive .AND. f_nConexao < 121
    USE (f_cTableName) ALIAS (cTmpAliasName) NEW EXCLUSIVE
    If NetErr()
      ::nOpenStatus := 3
      ::cOpenStatus := "3 - Erro acesso Exclusivo tabela: "+f_cTableName
      LogFile("tAdsError.log",{"NewRdd() 3 - Erro acesso Exclusivo tabela: "+f_cTableName,;
              "Alias "+cTmpAliasName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
      Return Self
    EndIf

  Else
    USE (f_cTableName) ALIAS (cTmpAliasName) NEW SHARED
    //Select 0
    //AdsCreateSqlStatement(::cAlias,3,::oConexao:hConnectHandle)
    //(::cAlias)->(AdsPrepareSQL("Select {static} * From "+f_cTableName))
    //(::cAlias)->(AdsExecuteSQL())

    If NetErr()
      ::nOpenStatus := 2
      ::cOpenStatus := "2 - Erro abertura modo compartilhado tabela: "+f_cTableName
      LogFile("tAdsError.log",{"NewRdd() 2 - Erro abertura modo compartilhado tabela: "+f_cTableName,;
              "Alias "+cTmpAliasName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
      Return Self
    EndIf
  EndIf
  
  If f_nCache > 0
    (cTmpAliasName)->(AdsCacheRecords(f_nCache))
  EndIf

  ::OnSkip()
  ::cOrderFocus       := ""
  ::aStructTads       := TAds_StructCreate(::cAlias)
  ::nConnectionType   := (::cAlias)->(AdsGetTableConType())
  
  oSelf := Hb_QSelf()
  
  For nIFor := 1 To Len(::aStructTads)
    __objAddData(oSelf,::aStructTads[nIFor,1])
  Next

  ::DataLoad()

RETURN Self
//-----------------------------------------------------------------------------
METHOD End() Class TAds
  Local lEnd := .F.

  If ::nOpenType == 1 .and. ::oConexao:nConnection != 121 // Via Rdd USE
    If ::nOpenStatus == 0
      (::cAlias)->(DbCommit())
      (::cAlias)->(AdsFlushFileBuffers())
      (::cAlias)->(DbrUnlock())
    EndIf
  EndIf
  
  If ::nOpenStatus == 0
    If ::nOpenType == 1   // RDD
      lEnd := (::cAlias)->(DbCloseArea())
    ElseIf ::nOpenType == 2 // sql select
      (::cAlias)->(DbCloseArea()) /// Obsoleto-reservado para esquema futuro
    ElseIf ::nOpenType == 3 // Data Set Sql
      If ::nQryTipo == 1 // Select   
        If ::nErrorSql == 0
          lEnd := (::cAlias)->(DbCloseArea())
        EndIf
      EndIf
      If ::lDsCursorsToTemp .and. ::lDsTempEraseOnClose
        TAds_TableErase(121,::cTableAliasTmp)
      EndIf
    EndIf
  EndIf
  
  If ::nOpenAliasCtrl > 0
    st_aCtrlAlias_Tabelas[::nOpenAliasCtrl]  := "_EMPTY_"
    st_aCtrlAlias_Monitor[::nOpenAliasCtrl]  := "_EMPTY_"
  Else
    ? "Method End tAds.prg > cTableAliasTmp", ::cTableAliasTmp
  EndIf

  //hb_gcAll()
  ::nOpenStatus := -1

RETURN NIL
//-----------------------------------------------------------------------------
Method DataLoad(f_lRefreshRecord, f_lTrimCaracter) class TAds
  Local iFor := 0, aFieldsDatas := {}, oSelf := Hb_qSelf()
  Local nCountData := 0, aStructTmp := ::aStructTads, uTmpDataValue
  
  Default f_lRefreshRecord := .F., f_lTrimCaracter := .F. 
  
  aStructTmp := oSelf:aStructTads

  If f_lRefreshRecord 
    ::Refresh()
  EndIf
  
  For iFor := 1 To Len(aStructTmp)

    If __ObjHasData(oSelf,aStructTmp[iFor,1])
      nCountData ++
      ///oSelf:&(aStructTmp[iFor,1]) := (::cAlias)->(FieldGet(iFor))
      aadd(aFieldsDatas,{Nil,Nil})
      aFieldsDatas[ nCountData, HB_OO_DATA_SYMBOL ] := AllTrim(aStructTmp[iFor,1]) ///AllTrim(::aStructTads[iFor,1])
      If aStructTmp[iFor,2] == "W" // Binary
        uTmpDataValue := "Binary"
      Else
        uTmpDataValue :=  ((::cAlias)->(FieldGet((::cAlias)->(FieldPos(aStructTmp[iFor,1]))))) ////((::cAlias)->&(aStructTmp[iFor,1]))  /// (FieldGet(iFor))
      EndIf
      ///uTmpDataValue := ((::cAlias)->&(aStructTmp[iFor,1]))  /// (FieldGet(iFor))

      If ValType(uTmpDataValue) == "C"  
        uTmpDataValue := StrTran(uTmpDataValue,"'","´")
        If f_lTrimCaracter
          uTmpDataValue := AllTrim(uTmpDataValue)      
        EndIf
      EndIf
      aFieldsDatas[ nCountData, HB_OO_DATA_VALUE  ] := (uTmpDataValue) //(::cAlias)->(FieldGet(iFor))
      
    EndIf
  Next

  If Len(aFieldsDatas) > 0
    __ObjSetValueList( oSelf, aFieldsDatas )
  EndIf

  ::nRecnoDataBuffer   := ::Recno()
  
Return Nil
//-----------------------------------------------------------------------------
Method DataBlank() class TAds
  Local iFor := 0, aFieldsDatas := {}, oSelf := Hb_qSelf()
  Local nCountData := 0, aStructTmp := ::aStructTads
  
  For iFor := 1 To Len(aStructTmp)
    If __ObjHasData(oSelf,aStructTmp[iFor,1])
      nCountData ++
      aadd(aFieldsDatas,{Nil,Nil})
      aFieldsDatas[ nCountData, HB_OO_DATA_SYMBOL ] := ::aStructTads[iFor,1]
      aFieldsDatas[ nCountData, HB_OO_DATA_VALUE  ] := uValBlank((::cAlias)->(FieldGet(iFor)))
    EndIf
  Next

  If Len(aFieldsDatas) > 0
    __ObjSetValueList( oSelf, aFieldsDatas )
  EndIf

  ::nRecnoDataBuffer   := 0
  
Return Nil
//-----------------------------------------------------------------------------
Method DataSave(f_aIgnoreFields) class TAds
  Local aStructTmp := ::aStructTads, lFieldIgnore := .F., oSelf := Hb_qSelf()
  Local iFor := 0, iForIgnore := 0, cFieldsIgnore := "" 
  
  Default f_aIgnoreFields := {}

  aStructTmp := oSelf:aStructTads

  For iForIgnore := 1 to Len(f_aIgnoreFields)
    cFieldsIgnore += Upper(f_aIgnoreFields[iForIgnore]) + "-"
  Next

  For iFor := 1 To Len(aStructTmp)
    
    lFieldIgnore := .F.

    If At(Upper(aStructTmp[iFor,1])+"-",cFieldsIgnore) > 0
      lFieldIgnore := .T.
    EndIf
    
    If aStructTmp[iFor,2] == "AutoIncrement" .or. aStructTmp[iFor,2] == "+"
      lFieldIgnore := .T.
    EndIf
    
    If !lFieldIgnore
      
      If __ObjHasData(oSelf,aStructTmp[iFor,1])
      
        ::VarPut(aStructTmp[iFor,1],oSelf:&(aStructTmp[iFor,1]))
      
      EndIf    
   
    EndIf    

  Next 

Return Nil
//-----------------------------------------------------------------------------
Method DataLoadToFR(f_oFastRep,f_cTituloGrupo,f_lTrimChar,f_aIgnoreFields,f_cPrefixVar) Class TAds
  Local iFor := 0, aStructTmp := ::aStructTads,  oSelf := Hb_qSelf()
  Local cTmpChar := ""

  Default f_lTrimChar := .T., f_aIgnoreFields := {}, f_cPrefixVar := ""
  
  aStructTmp := oSelf:aStructTads
  ::DataLoad()
    
  For iFor := 1 To Len(aStructTmp)

    if aStructTmp[Ifor,1] == "LIVRO_APON"
      ///? oSelf:&(aStructTmp[iFor,1]), aStructTmp[Ifor,1]
    EndIf
    If Ascan(f_aIgnoreFields, aStructTmp[Ifor,1]) == 0

      If ValType(oSelf:&(aStructTmp[iFor,1])) == "C"
        If f_lTrimChar
          cTmpChar := Alltrim(StrTran(oSelf:&(aStructTmp[iFor,1]),"'","´"))
        Else
          cTmpChar := StrTran(oSelf:&(aStructTmp[iFor,1]),"'","´")
        EndIf
        f_oFastRep:AddVariable(f_cTituloGrupo,f_cPrefixVar+aStructTmp[Ifor,1],"'"+cTmpChar+"'")
        //f_oFastRep:AddVariable(f_cTituloGrupo,aStructTmp[Ifor,1],"'"+cTmpChar+"'")
      Else
        f_oFastRep:AddVariable(f_cTituloGrupo,f_cPrefixVar+aStructTmp[Ifor,1],oSelf:&(aStructTmp[iFor,1]))
        //f_oFastRep:AddVariable(f_cTituloGrupo,aStructTmp[Ifor,1],oSelf:&(aStructTmp[iFor,1]))
      EndIf

    EndIf
  Next

Return Nil
//-----------------------------------------------------------------------------
Method FieldInfo(f_cCampo) Class TAds
  Local aRetStruct := {"","C",1,0}, iFor := 0

  For Ifor := 1 To len(::aStructTads)
    If Upper(::aStructTads[iFor,1]) == Upper(f_cCampo)
      aRetStruct[1] := ::aStructTads[iFor,1]
      aRetStruct[2] := ::aStructTads[iFor,2]
      aRetStruct[3] := ::aStructTads[iFor,3]
      aRetStruct[4] := ::aStructTads[iFor,4]
    EndIf
  Next

Return aRetStruct
//-----------------------------------------------------------------------------
Method FieldPos(f_cCampo) Class TAds
  Local nPos := 0, iFor := 0

  For Ifor := 1 To len(::aStructTads)
    If Upper(::aStructTads[iFor,1]) == Upper(f_cCampo)
      nPos := iFor
      Exit
    EndIf
  Next

Return nPos
//-----------------------------------------------------------------------------
Method FieldBlank(f_cCampo) Class TAds
  Local uGet := Nil, nPosField := 0

  nPosField := (::cAlias)->(FieldPos(f_cCampo))

  uGet := (::cAlias)->(FieldGet(nPosField))

  If ValType(uGet) == "C"
    uGet := Space(::aStructTads[nPosField][3])
  Elseif ValType(uGet) == "N"
    uGet := 0
  Elseif ValType(uGet) == "I"
    uGet := 0
  Elseif ValType(uGet) == "D"
    uGet := cTod("  /  /    ")
  EndIf

Return uGet
//-----------------------------------------------------------------------------
Method FieldSum(f_cCampo) Class TAds
  Local nRecnoPos := ::Recno(), nCalcSum := 0

  ::Gotop()
  Do While !::Eof()
    nCalcSum += ::VarGet(f_cCampo)
    ::Skip()
  EndDo
  
  ::GoTo(nRecnoPos)

Return nCalcSum
//-----------------------------------------------------------------------------
METHOD OnSkip(f_lData) Class TAds

  Default f_lData := .T.
  
  ::lBoF        := (::cAlias)->(Bof())
  ::lEoF        := (::cAlias)->(Eof())
  ::nFocusRecno := (::cAlias)->(Recno())
  
  If ::lBufferOnSkip .and. f_lData
    if ::lEof
      ::DataBlank()
    Else
      ::DataLoad()
    EndIf
  EndIf
  
RETURN NIL
//-----------------------------------------------------------------------------
METHOD Lock(f_nRecno) Class TAds
  Local lLockRecno := .F., iForTentativa := 0

  Default f_nRecno := (::cAlias)->(Recno())

  If (::cAlias)->(Recno()) != f_nRecno
    (::cAlias)->(DbGoTo(f_nRecno))
    ::OnSkip()
  EndIf

  For iForTentativa := 1 To 5
    (::cAlias)->(AdsRefreshRecord())
    If (::cAlias)->(DbRLock())
      lLockRecno := .T.
      Exit
    Else
      SysWait(.2)
    EndIf
  Next

RETURN lLockRecno
//-----------------------------------------------------------------------------
METHOD RLock(f_nRecno) Class TAds
  Local lLockRecno := .F., iForTentativa := 0

  Default f_nRecno := (::cAlias)->(Recno())

  If (::cAlias)->(Recno()) != f_nRecno
    (::cAlias)->(DbGoTo(f_nRecno))
    ::OnSkip()
  EndIf

  For iForTentativa := 1 To 16
    (::cAlias)->(AdsRefreshRecord())
    If (::cAlias)->(DbRLock())
      lLockRecno := .T.
      Exit
    Else
      SysWait(.2)
    EndIf
  Next

RETURN lLockRecno
//-----------------------------------------------------------------------------
METHOD UnLock(f_nRecno) Class TAds
  Local lUnLockRecno := .F., iForTentativa := 0

  Default f_nRecno := (::cAlias)->(Recno())

  If (::cAlias)->(Recno()) != f_nRecno
    (::cAlias)->(DbGoTo(f_nRecno))
    ::OnSkip()
  EndIf

  (::cAlias)->(DbRUnLock())

  lUnLockRecno := .T.

RETURN lUnLockRecno
//-----------------------------------------------------------------------------
METHOD IsRecordLocked(f_nRecno) Class TAds
  Local lIsRecnoLocked := .F.

  Default f_nRecno := (::cAlias)->(Recno())

  If (::cAlias)->(Recno()) != f_nRecno
    (::cAlias)->(DbGoTo(f_nRecno))
    ::OnSkip()
  EndIf

  lIsRecnoLocked := (::cAlias)->(AdsIsRecordLocked())

RETURN lIsRecnoLocked
//-----------------------------------------------------------------------------
METHOD Skip(f_nSkip) Class TAds
  
  Default f_nSkip := 1 

  (::cAlias)->(DbSkip(f_nSkip))

  ::OnSkip()

RETURN ::Recno()
//-----------------------------------------------------------------------------
METHOD GoTop() Class TAds

  (::cAlias)->(DbGoTop())
  ::OnSkip()

RETURN NIL
//-----------------------------------------------------------------------------
METHOD GoBottom() Class TAds

  (::cAlias)->(DbGoBottom())
  ::OnSkip()

RETURN NIL
//-----------------------------------------------------------------------------
METHOD GoTo(f_nRecno) Class TAds
  Local lOkRegistro := .T.

  if f_nRecno == 0
    f_nRecno := 1
  EndIf

  (::cAlias)->(DbGoTo(f_nRecno))
  If ::Eof()
    (::cAlias)->(AdsRefreshRecord())
    (::cAlias)->(LastRec())
  EndIf
  If ::Recno() != f_nRecno
    lOkRegistro := .F.
  EndIf
  ::OnSkip()

RETURN lOkRegistro
//-----------------------------------------------------------------------------
METHOD Append() Class TAds
  Local lAppend := .F.

  ::nRecnoLastAppend := 0
  lAppend := (::cAlias)->(DbAppend())
  ::nRecnoLastAppend := ::Recno()
  ::OnSkip(.F.)
  St_nRecnoNew := ::nRecnoLastAppend

RETURN lAppend
//-----------------------------------------------------------------------------
METHOD Delete(f_lLock) Class TAds
  Local lDelete := .F.

  Default f_lLock := .F.

  If f_lLock
    If !::rLock()
      Return .F.
    EndIf
  EndIf

  lDelete := (::cAlias)->(DbDelete())
  ::Refresh()
  ::OnSkip()

RETURN lDelete
//-----------------------------------------------------------------------------
METHOD Refresh() Class TAds

  (::cAlias)->(AdsRefreshRecord())
  //::cOrderFocus := Upper((::cAlias)->(OrdName(0)))

RETURN Nil
//-----------------------------------------------------------------------------
Method KeyCount(f_nModo) Class tAds 
  Local nRecCount := 0

  Default f_nModo := ADS_RESPECTFILTERS  // 1 > ADS_RESPECTFILTERS - Respeita filtros e condições
                                         // 2 > ADS_IGNOREFILTERS - Retorna o indice inteiro
  
  nRecCount := (::cAlias)->(ADSKeyCount(,,f_nModo))

Return nRecCount
//-----------------------------------------------------------------------------
Method KeyNo(f_uOrder,f_nFilterOption) class tAds
  Local nKey := 0
  Local cOrderPar := ""

  Default f_nFilterOption := ADS_RESPECTFILTERS, ::nKeyNoCount := 1

  cOrderPar := If(Hb_isNil(f_uOrder),;
                                   ::cOrderFocus,;
                                   f_uOrder)
  
  ::nKeyNoCount ++
  //f_nFilterOption := 1
  
  If Empty(cOrderPar)
    nKey := (::cAlias)->(ADSKeyNo(,,f_nFilterOption))
  Else
    nKey := (::cAlias)->(ADSKeyNo(cOrderPar,,f_nFilterOption))
  EndIf
  
  ///wndMain():cTitle := "KeyNoCount "+m_str(::nKeyNoCount)
  
Return nKey
//-----------------------------------------------------------------------------
Method KeyGoTo(f_nKey) class tAds
  Local nKeyPos := 0
  Local cOrderPar := ::cOrderFocus

  If f_nKey < 1
    f_nKey := 1
  EndIf

  If Empty(cOrderPar)
    nKeyPos := (::cAlias)->(OrdKeyGoto( f_nKey ))
    //nKeyPos := ::Goto(f_nKey)
  Else
    nKeyPos := (::cAlias)->(OrdKeyGoto( f_nKey ))
  EndIf

Return nKeyPos
//-----------------------------------------------------------------------------
METHOD VarGet(f_cCampo,f_lRefresh) Class TAds
  Local uGet := Nil, nPosField := 0, iForField := 0, nRecnoAtual := 0

  Default f_lRefresh := .F.

  f_cCampo := Alltrim(f_cCampo)
  
  nPosField := ::FieldPos(f_cCampo)  
  If nPosField == 0
    ? "Method VarGet() - Field Not Found: "+::cTableName+"->"+f_cCampo, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"
    LogFile("tAdsError.log",{"Method VarGet() - Field Not Found: "+::cTableName+"->"+f_cCampo, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
    Return Nil
  EndIf

  uGet := (::cAlias)->(FieldGet(nPosField))

  If Hb_IsNil(uGet)
    uGet := (::cAlias)->&f_cCampo
  EndIf

RETURN uGet
//-----------------------------------------------------------------------------
Method VarGetFieldNull(f_cCampo) Class TAds
  Local uGet := Nil, nPosField := 0

  nPosField := (::cAlias)->(FieldPos(f_cCampo))

  uGet := (::cAlias)->(FieldGet(nPosField))

  If ValType(uGet) == "C"
    uGet := Space(::aStructTads[nPosField][3])
  Elseif ValType(uGet) == "N"
    uGet := 0
  Elseif ValType(uGet) == "D"
    uGet := cTod("  /  /    ")
  EndIf

Return uGet
//-----------------------------------------------------------------------------
METHOD VarGetAlltrim(f_cCampo) Class TAds
  Local uGet := Nil

  uGet := Alltrim(::VarGet(f_cCampo))

RETURN uGet
//-----------------------------------------------------------------------------
METHOD VarGetRtrim(f_cCampo) Class TAds
  Local uGet := Nil

  uGet := RTrim(::VarGet(f_cCampo))

RETURN uGet
//-----------------------------------------------------------------------------
METHOD VarGetDivZero(f_cCampo) Class TAds
  Local nGet := 0

  nGet := ::VarGet(f_cCampo)

  If nGet == 0
    nGet := 1
  EndIf

RETURN nGet
//-----------------------------------------------------------------------------
METHOD VarGetStrZero(f_cCampo,f_nZeros) Class TAds
  Local uGet := Nil

  Default f_nZeros := 9

  uGet := StrZero(::VarGet(f_cCampo),f_nZeros)

RETURN uGet
//-----------------------------------------------------------------------------
METHOD VarGetTransform(f_cCampo,f_cMask) Class TAds
  Local uGet := Nil

  Default f_cMask := "@!"

  uGet := Transform(::VarGet(f_cCampo),f_cMask)

RETURN uGet
//-----------------------------------------------------------------------------
Method VarGetSubStr(f_cCampo,f_nCharStart,f_nCharCount)
  Local cRet := ""

  Default f_nCharStart := 1

  cRet := SubStr(::VarGet(f_cCampo),f_nCharStart,f_nCharCount)

Return cRet
//-----------------------------------------------------------------------------
METHOD VarGetCleanChars(f_cCampo,f_aCharsClean) Class TAds
  Local cGet := ""

  Default f_aCharsClean := {" "}

  cGet := TAds_CleanChars(::VarGet(f_cCampo),f_aCharsClean)

RETURN cGet
//-----------------------------------------------------------------------------
Method VarGetPadL(f_cCampo,nLength,cFillChar) Class TAds
  Local cValType := "", uGet := ::VarGet(f_cCampo)

  Default cFillChar := " "
  
  cValType := ValType(uGet)

  If cValType == "N"
    uGet := Alltrim(Str(uGet))
  ElseIf cValType == "I"
    uGet := Alltrim(Str(uGet))
  ElseIf cValType == "D"
    uGet := cToD(uGet)
  EndIf

  uGet := PadL(Alltrim(uGet),nLength, cFillChar)

Return uGet
//-----------------------------------------------------------------------------
Method VarGetPadR(f_cCampo,nLength) Class TAds
  Local cValType := "", uGet := ::VarGet(f_cCampo)

  cValType := ValType(uGet)

  If cValType == "N"
    uGet := Alltrim(Str(uGet))
  ElseIf cValType == "I"
    uGet := Alltrim(Str(uGet))
  ElseIf cValType == "D"
    uGet := cToD(uGet)
  EndIf

  uGet := PadR(Alltrim(uGet),nLength)

Return uGet
//-----------------------------------------------------------------------------
METHOD VarGetNumToChar(f_cCampo,f_nCasasDecimais,f_lFixCasasDecimais) Class TAds
  Local cRetVal := ""

  Default f_nCasasDecimais := 2, f_lFixCasasDecimais := .F.

  cRetVal := TAds_Str(::VarGet(f_cCampo),0,!f_lFixCasasDecimais,f_nCasasDecimais,.T.)

RETURN cRetVal
//-----------------------------------------------------------------------------
METHOD VarGetDateToChar(f_cCampo,f_nTpConvert) Class TAds
  Local cRetVal := ""

  Default f_nTpConvert := 0

  If f_nTpConvert == 0
    cRetVal := DtoC(::VarGet(f_cCampo))
  Elseif f_nTpConvert == 1
    cRetVal := TAds_DataExtenso(::VarGet(f_cCampo),.F.)
  ElseIf f_nTpConvert == 2
    cRetVal := TAds_DataExtenso(::VarGet(f_cCampo),.T.)
  EndIf

RETURN cRetVal
//-----------------------------------------------------------------------------
METHOD VarGetYear(f_cCampoDt,f_lStrReturn)
  Local uRetVal

  If Hb_IsNil(f_lStrReturn)
    f_lStrReturn := .F.
  EndIf

  If f_lStrReturn
    uRetVal := TAds_Str(Year(::VarGet(f_cCampoDt)))
  Else
    uRetVal := Year(::VarGet(f_cCampoDt))
  EndIf

Return uRetVal
//-----------------------------------------------------------------------------
METHOD VarGetNumToMoeda(f_cCampo,f_nCasasDecimais,f_lNulo,f_nEspacoPreenche,f_cCharEspaco,f_cSifrao) class TAds
  Local cRetVal

  Default f_nCasasDecimais := 2, f_lNulo := .T., f_nEspacoPreenche := 0, f_cCharEspaco := " "

  cRetVal := TAds_Valor(::VarGet(f_cCampo),f_nCasasDecimais,f_lNulo,f_nEspacoPreenche,f_cCharEspaco,.T.)

RETURN cRetVal
//-----------------------------------------------------------------------------
METHOD VarGetNumToMoedaExtenso(f_cCampo,f_nTpConvert) Class TAds
  Local cRetVal := ""

  Default f_nTpConvert := 0

  If f_nTpConvert == 0
    cRetVal := TAds_ValExtenso(::VarGet(f_cCampo),.F.)
  ElseIf f_nTpConvert == 1
    cRetVal := TAds_ValExtenso(::VarGet(f_cCampo),.T.)
  EndIf

RETURN cRetVal
//-----------------------------------------------------------------------------
METHOD VarGetAtokens(f_cCampo,f_cCharSeparator) Class TAds
  Local aTokens := {}

  Default f_cCharSeparator := " "
  
  aTokens := Hb_aTokens(::VarGet(f_cCampo),f_cCharSeparator,.T.)

Return aTokens
//-----------------------------------------------------------------------------
METHOD VarPut(f_cCampo,f_uVal,f_cOperador,f_lConvertAp) Class TAds
  Local uField := ""
  Local aFieldInfo := ::FieldInfo(f_cCampo)

  Default f_cOperador := "=", f_lConvertAp := .T.

  f_cCampo := AllTrim(f_cCampo)
  
  If ValType(f_uVal) == "C" .and. aFieldInfo[2] == "C" .and. f_lConvertAp  
      f_uVal := StrTran(f_uVal,"'","´") // Muitos erros ocorrem em instruções sql por causa deste caracter
  EndIf

  If f_cOperador == "="
    (::cAlias)->&f_cCampo := (f_uVal)
  ElseIf f_cOperador == "+"
    ::Refresh()
    (::cAlias)->&f_cCampo += (f_uVal)
  ElseIf f_cOperador == "-"
    ::Refresh()
    (::cAlias)->&f_cCampo -= (f_uVal)
  Else
    (::cAlias)->&f_cCampo := (f_uVal)
  EndIf

RETURN ((::cAlias)->&f_cCampo)
//-----------------------------------------------------------------------------
METHOD VarPutDbfTmp(f_cFieldNameAdt,f_cAliasDbf) Class TAds  // oBsoleto
  Local cFieldNameDbf := ""

  ///cFieldNameDbf := TAds_FieldAdtToDbf(f_cFieldNameAdt)

  ::VarPut(f_cFieldNameAdt,(f_cAliasDbf)->&cFieldNameDbf)

RETURN Nil
//-----------------------------------------------------------------------------
METHOD Commit(f_lUnlock,f_lFlushBuffers) Class TAds

  Default f_lUnlock := .F., f_lFlushBuffers := .T.

  If f_lUnlock
    (::cAlias)->(DbrUnlock())
  EndIf

  (::cAlias)->(DbCommit())

  If f_lFlushBuffers ///::nConnectionType == 1 // Local Connection
    (::cAlias)->(AdsFlushFileBuffers())
  EndIf

  (::cAlias)->(DbCommit())
  ::Refresh()

RETURN Nil
//-----------------------------------------------------------------------------
METHOD SetOrder(f_uOrder,f_cFileBagName,f_lForce) Class TAds
  Local cOrder := "", lc_cOldOrder, lc_cOrdBagNameStatic
  Local cmsg := 0 //space(13)
  
  Default f_lForce := .F.
  
  If !::lDsStaticCursors
    lc_cOldOrder := ::cOrderFocus  //// Upper((::cAlias)->(OrdName(0)))  //::cOrderFocus

    If ValType(f_uOrder) == "C"
      f_uOrder  := Upper(f_uOrder)
      cOrder    := Upper(f_uOrder)
    Else
      //? "f_uOrder", f_uOrder
      cOrder    := Upper((::cAlias)->(OrdName(f_uOrder)))
      f_uOrder  := cOrder
    EndIf
  
    If lc_cOldOrder == cOrder
      Return nil
    EndIf
    //? lc_cOldOrder, cOrder
    
    (::cAlias)->(OrdSetFocus(cOrder,f_cFileBagName))
    ::cOrderFocus := Upper((::cAlias)->(OrdName(0)))
        
  ElseIf ::lDsStaticCursors // If lDsStaticCursors create new order and erase older
      //? ::cqrysql
    If AT("__ORDER_BY__",::cQrySql) > 0
      ::cDsOrderBy := "ORDER BY "+f_uOrder
      ::DsExecute(120)
    Else
    
      lc_cOldOrder := ::cOrderFocus //Upper((::cAlias)->(OrdName(0)))

      If ValType(f_uOrder) == "C"
        f_uOrder  := Upper(f_uOrder)
        cOrder    := Upper(f_uOrder)
      Else
        cOrder    := Upper((::cAlias)->(OrdName(f_uOrder)))
        f_uOrder  := cOrder
      EndIf
      
      If lc_cOldOrder == cOrder .and. !f_lForce
        Return nil
      EndIf

      If !Empty(lc_cOldOrder)
        (::cAlias)->(OrdDestroy(::cAlias,lc_cOldOrder))
        (::cAlias)->(ADSDELETECUSTOMKEY(lc_cOldOrder))
      EndIf
      CursorWait()
      ::CreateIndex( ::cAlias, f_uOrder, f_uOrder, ::cAlias+"->"+f_uOrder, .F.)
      (::cAlias)->(OrdListAdd(::cAlias,Upper(f_uOrder)))
      (::cAlias)->(ADSADDCUSTOMKEY(f_uOrder))
      (::cAlias)->(OrdSetFocus(f_uOrder,::cAlias))
      ::cOrderFocus := Upper((::cAlias)->(OrdName(0)))
      CursorArrow()
    EndIf
  EndIf

RETURN Nil
//-----------------------------------------------------------------------------
METHOD Seek(f_uOcorrencia,f_cTagOpcional,f_lAproximado,f_lLocateIfNotFound,f_lReturnLastOrder) Class TAds
  Local lFoundOcorrencia := .F., cLastTag := ::cOrderFocus

  Default f_lAproximado := .F., f_lReturnLastOrder := .F., f_lLocateIfNotFound := .F.

  If !Hb_IsNil(f_cTagOpcional)
    ::SetOrder(f_cTagOpcional)
  EndIf

  If ValType(f_uOcorrencia) != "C"
    f_lAproximado := .F.  
  EndIf
  
  lFoundOcorrencia := (::cAlias)->(DbSeek(f_uOcorrencia,f_lAproximado))

  If !lFoundOcorrencia .and. f_lLocateIfNotFound
    lFoundOcorrencia := ::Locate(f_cTagOpcional,f_uOcorrencia)
  EndIf
  
  if !lFoundOcorrencia .and. !::Eof()
    If !f_lAproximado
      If !Empty(::cOrderFocus)
        lFoundOcorrencia := (f_uOcorrencia == ::VarGet(::cOrderFocus))
      EndIf
    Else
      lFoundOcorrencia := .T.  
    EndIf
  EndIf

  If f_lReturnLastOrder 
    If !Empty(cLastTag)
      ::SetOrder(cLastTag)
    EndIf
  EndIf
  If lFoundOcorrencia
    ::Refresh()
  EndIf
  ::OnSkip()

RETURN lFoundOcorrencia
//-----------------------------------------------------------------------------
Method Locate(f_cCampo,f_uValor) Class TAds
  Local lFoundRegistro := .F., nRecAtual := ::Recno()

  ::GoTop()
  Do While !::Eof()
    If ::VarGet(f_cCampo) == f_uValor
      lFoundRegistro := .T.
      Exit
    EndIf
    ::Skip()
  EndDo

  If !lFoundRegistro
    ::GoTo(nRecAtual)
  Else
    ::OnSkip()
  EndIf

Return lFoundRegistro
//-----------------------------------------------------------------------------
METHOD Recno() Class TAds
  Local nRecno := 0

  If (::cAlias)->(Eof())
    Return 0
  EndIf

  nRecno := (::cAlias)->(Recno())

RETURN nRecno
//-----------------------------------------------------------------------------
METHOD aGetRecnos() Class TAds
  Local aRecnos := {}, nRecatual := ::Recno()

  ::GoTop()
  Do While !::Eof()
    AaDd(aRecnos,::Recno())
    ::Skip()
  Enddo

  If nRecAtual > 0
    ::GoTo(nRecatual)
  EndIf

Return aRecnos
//-----------------------------------------------------------------------------
METHOD Scope(f_cTagIndex,f_uTopCondicao,f_uEofCondicao) Class TAds

 default f_uEofCondicao := f_uTopCondicao 
 
 ::SetOrder(f_cTagIndex)
 (::cAlias)->(OrdScope(0,f_uTopCondicao))
 (::cAlias)->(OrdScope(1,f_uEofCondicao))
 ::GoTop()                                

Return Nil
//-----------------------------------------------------------------------------
METHOD Filter(f_cFilterMask,f_aVarsFormat,f_nTpFiltro) class TAds
  Local _nRecMarca:=(::cAlias)->(RECNO())
  Local _lFiltro:=.F., iForFilter := 0, cAnd := "", cTpAnd := " .and. "

  Default   f_nTpFiltro := 1,;
            f_cFilterMask := ""

  If ::nOpenType == 1 // RDD
    cTpAnd := " .and. "
  ElseIf ::nOpenType == 3 // SQL DataSet
    cTpAnd := " and "
  EndIf

  IF !Hb_IsNil(f_AVarsFormat)
     f_cFilterMask := ::CommandFormat(f_cFilterMask,f_aVarsFormat,::nOpenType)
  ENDIF

  If !Empty(f_cFilterMask)
    cAnd := cTpAnd
  EndIf

  For iForFilter := 1 to Len(::aFilterFix)
    f_cFilterMask += cAnd + ::aFilterFix[iForFilter]
    cAnd := cTpAnd
  Next

  If Empty(f_cFilterMask)
    (::cAlias)->(AdsClearAof())
    Return .F.
  EndIf

  IF Ascan({  ADS_DYNAMIC_AOF,;
              ADS_RESOLVE_IMMEDIATE,;
              ADS_RESOLVE_DYNAMIC,;
              ADS_KEYSET_AOF,;
              ADS_FIXED_AOF,;
              ADS_KEEP_AOF_PLAN,;
              ADS_RESOLVE_IMMEDIATE + ADS_KEYSET_AOF,;
              ADS_RESOLVE_DYNAMIC + ADS_KEYSET_AOF,;
              ADS_RESOLVE_IMMEDIATE + ADS_FIXED_AOF,;
              ADS_RESOLVE_DYNAMIC + ADS_FIXED_AOF},f_nTpFiltro) == 0
     f_nTpFiltro := ADS_RESOLVE_IMMEDIATE
  ENDIF

  IF LEN((::cAlias)->(AdsGetAof())) > 0
     (::cAlias)->(AdsClearAof())
     (::cAlias)->(DBGOTOP())
  ENDIF

  If GetKeyState(VK_SHIFT)
   //? f_cFilterMask //Teste para ver o filtro
  EndIf

  IF f_cFilterMask != Nil

     ////AdsIsExprValid (cFilter)
     
     _lFiltro := (::cAlias)->(AdsSetAof(f_cFilterMask,f_nTpFiltro))

     IF _lFiltro
        ::GoTop()
     ENDIF

  ENDIF

  ::OnSkip()
  ::KeyCount()

RETURN _lFiltro
//-----------------------------------------------------------------------------
METHOD FilterFts(f_cCampo,f_aStrings) class TAds
  Local _nRecMarca:=(::cAlias)->(RECNO())
  Local _lFiltro := .F., iForFilter := 0, cAnd := "( ", cStrings := "", cFilterFix := ""

  /// AND, OR, NOT, and NEAR

  cStrings := "CONTAINS("+f_cCampo+",'"
  For iForFilter := 1 To Len(f_aStrings)
    cStrings += f_aStrings[iForFilter,1]+" "+f_aStrings[iForFilter,2]+" "
  Next
  cStrings += "' )"

  For iForFilter := 1 to Len(::aFilterFix)
    cFilterFix += cAnd + ::aFilterFix[iForFilter]
    cAnd := " And "
  Next
  If !Empty(cFilterFix)
    cFilterFix += " ) And "
  EndIf

  IF LEN((::cAlias)->(AdsGetAof())) > 0
     (::cAlias)->(AdsClearAof())
     (::cAlias)->(DBGOTOP())
  ENDIF

   _lFiltro := (::cAlias)->(AdsSetAof(cFilterFix+cStrings,1))

   IF _lFiltro
      ::GoTop()
   ENDIF

Return _lFiltro
//-----------------------------------------------------------------------------
METHOD AddFilterFix(f_cFilterMask,f_aVarsFormat) class TAds
  Local _nRecMarca:=(::cAlias)->(RECNO())
  Local _lFiltro:=.F., nTpFiltro := 1

  Default   nTpFiltro := 1,; 
            f_cFilterMask := ""

  IF f_AVarsFormat != Nil
     f_cFilterMask := ::CommandFormat(f_cFilterMask,f_aVarsFormat,::nOpenType)
  ENDIF

  IF f_cFilterMask != Nil
     f_cFilterMask  :=  StrTran(f_cFilterMask,"[","CTOD("+CHR(39))
     f_cFilterMask  :=  StrTran(f_cFilterMask,"]",CHR(39)+")")
     aadd(::aFilterFix,f_cFilterMask)
  ENDIF

RETURN Len(::aFilterFix)
//-----------------------------------------------------------------------------
METHOD AddRecnosInFilter(f_aRecnos) class TAds
  Local lOk := .T., iFor := 0, cRecnos := ""

  If !Empty(::GetFilter())
      (::cAlias)->(ADSCustomizeAOF(f_aRecnos))
  Else
    lOk := .F.
  EndIf
  ::OnSkip()

Return lOk
//-----------------------------------------------------------------------------
METHOD FilterRefresh() class TAds

  (::cAlias)->(AdsRefreshAof())
  ::Refresh()
  ::OnSkip()

Return Nil
//-----------------------------------------------------------------------------
METHOD RefreshFilter() class TAds

  If !Empty((::cAlias)->(AdsGetAof()))
    (::cAlias)->(AdsRefreshAof())
    ::OnSkip()
  EndIf
  ::Refresh()

Return Nil
//-----------------------------------------------------------------------------
Method ClearFilter(f_lFilterFixRemove, f_lFocusRemains) Class TAds
  Local nRecFocus := ::Recno()
  
  Default f_lFilterFixRemove := .F., f_lFocusRemains := .F.


  (::cAlias)->(AdsClearAof())

  If f_lFilterFixRemove
    ::aFilterFix := {}
  EndIf

  If Len(::aFilterFix) > 0
    ::Filter()
    ::OnSkip()
  EndIf

  if f_lFocusRemains .and. nRecFocus > 0
    ::Goto(nRecFocus)
  EndIf

  ::Refresh()
  ::Skip(0)
  
Return Nil
//-----------------------------------------------------------------------------
Method IsRecordInFilter(f_nRecno) Class TAds
  Local lPresent := .F.

  Default f_nRecno := ::Recno()

  lPresent := (::cAlias)->(AdsIsRecordInAof(f_nRecno))

Return lPresent
//-----------------------------------------------------------------------------
METHOD CommandFormat(f_cStrCommand,f_aVarsFormat,f_nOpenType) class TAds
  Local _cRet := f_cStrCommand, _iFor := 0, _cTmpVar := "", _cTmpFormat := "", _cTmpData := ""

  Default f_nOpenType := ::nOpenType

  IF f_aVarsFormat == Nil
     Return _cRet
  ENDIF

  IF VALTYPE(f_aVarsFormat) != "A"
     Return _cRet
  ENDIF

  IF LEN(f_aVarsFormat) == 0
     Return _cRet
  ENDIF

  FOR _iFor := 1 TO LEN(f_aVarsFormat)
    _cTmpVar := f_aVarsFormat[_iFor,1]
    IF VALTYPE(f_aVarsFormat[_iFor,2]) == "C"

      _cTmpFormat := StrTran(f_aVarsFormat[_iFor,2],"'","´")
      _cTmpFormat := "'"+_cTmpFormat+"'"

    ELSEIF VALTYPE(f_aVarsFormat[_iFor,2]) == "D"

      If f_nOpenType == 3 // Query Open Table In SELECT
        _cTmpData := DTOS(f_aVarsFormat[_iFor,2])
        _cTmpData := (SubStr(_cTmpData,1,4)+"-"+SubStr(_cTmpData,5,2)+"-"+SubStr(_cTmpData,7,2))
        _cTmpFormat := "'"+_cTmpData+"'"
      ElseIf f_nOpenType == 2 .or. f_nOpenType == 1 // SetAof Table open SQL or RDD
        _cTmpFormat := "'"+DTOC(f_aVarsFormat[_iFor,2])+"'"
        _cTmpFormat := "CTOD("+_cTmpFormat+")"
      EndIf

    ELSEIF VALTYPE(f_aVarsFormat[_iFor,2]) == "N"

      _cTmpFormat := ALLTRIM(STR(f_aVarsFormat[_iFor,2]))

    ELSEIF VALTYPE(f_aVarsFormat[_iFor,2]) == "L"

      If f_nOpenType == 3 // Query em Select
        _cTmpFormat := IF(f_aVarsFormat[_iFor,2],"TRUE","FALSE")
      ElseIf f_nOpenType == 2 .OR. f_nOpenType == 1 // Se for para Rdd
        _cTmpFormat := IF(f_aVarsFormat[_iFor,2],".T.",".F.")
      EndIf

    ENDIF
    _cRet := STRTRAN(_cRet,_cTmpVar,_cTmpFormat)
  NEXT

RETURN _cRet
//-----------------------------------------------------------------------------
METHOD Blob2File(f_cFileDestino,f_cFieldBlobName) Class TAds

  (::cAlias)->(AdsBlob2File(f_cFileDestino,f_cFieldBlobName))

RETURN NIL
//-----------------------------------------------------------------------------
METHOD Blob2ZipFiles(f_cDirDescomp,f_cFieldBlobName,f_bBlock,f_cPassWord)
  Local aFilesInZip := {}, cFileDescomp := ".\Descomp.zip"

  ::Blob2File(cFileDescomp,f_cFieldBlobName)
  
  aFilesInZip := hb_GetFilesInZip(cFileDescomp)

  If !Hb_UnzipFile(cFileDescomp,f_bBlock,.T.,f_cPassWord,f_cDirDescomp)
    aFilesInZip := {}
  EndIf

Return aFilesInZip
//-----------------------------------------------------------------------------
METHOD File2Blob(f_cFileOrigem,f_cFieldBlobName) Class TAds

  (::cAlias)->(AdsFile2Blob(f_cFileOrigem,f_cFieldBlobName))

RETURN NIL
//-----------------------------------------------------------------------------
METHOD Files2BlobZip(f_aFilesToCompress,f_cFieldBlobName,f_nLeverCompress,f_bBlock,f_lOverWrite,f_cPassWord) Class TAds
  Local lProcessOk := .F.
  
  fErase("Compress.zip")

  lProcessOk := Hb_ZipFile("Compress.zip",f_aFilesToCompress,f_nLeverCompress,f_bBlock,f_lOverWrite,f_cPassWord)

  If lProcessOk
    ::File2Blob("Compress.zip",f_cFieldBlobName)
  EndIf

Return lProcessOk
//-----------------------------------------------------------------------------
METHOD CreateIndex( f_cFileBagName, f_cTagName, f_cKeyName, f_bKey, f_lUnique) 
  Local lCreate := .F.
  
  Default f_cFileBagName := ::cAlias, f_lUnique := .F.
  
  lCreate := ( ::cAlias )->( OrdCreate( f_cFileBagName, f_cTagName, f_cKeyName, f_bKey, f_lUnique ) )

Return lCreate
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
METHOD DsNew( f_nQryTipo, f_nConexao ) Class TAds
  LOCAL cComando := "", nIFor := 0
  LOCAL  cNewAlias := "", aTmpChar := {}, cTmpAliasName := "", nTmpAliasExistentePosition := 0, nTmpAliasNewPosition := 0

  Default f_nConexao := tAds_GetConnectionDefault(), f_nQryTipo := 0
  
  ::oConexao            := tAds_GetConnectionObj(f_nConexao)
  ::nQryTipo            := f_nQryTipo
  ::lDsCursorsToTemp    := .F.
  ::lDsCursorsToArray   := .F.
  ::aDsCursorsHeaders   := {}
  ::aDsCursorsData      := {}
  ::aFieldsTempIndex    := {}
  ::lDsTempEraseOnClose := .T.
  ::nErrorSql           := 0
  ::lDsErrorSave        := .T.
  ::lLogDsExecute       := .F.
  
  St_nRecnoNew := 0

  If Hb_IsNil(st_lProcessAutoAlias)
    st_lProcessAutoAlias := .F.
  EndIf
  Do While st_lProcessAutoAlias
    hb_idleSleep(0.1)
  EndDo
  st_lProcessAutoAlias := .T.

  If Hb_IsNil(St_SifraoMoeda)
    St_SifraoMoeda := "R$"
  EndIf

  if Hb_IsNil(st_aCtrlAlias_Tabelas)
    st_aCtrlAlias_Tabelas  := {}
    st_aCtrlAlias_Monitor  := {}
  EndIf

  Do While .T. 
    cTmpAliasName := "Sql_"+StrZero(nRandom(99991)+1,5)
    nTmpAliasExistentePosition := aScan(st_aCtrlAlias_Tabelas,cTmpAliasName)
    If nTmpAliasExistentePosition == 0
      Exit
    EndIf
  EndDo 

  ::cTableName      := "DataSet"
  ::cDsTxtAlias     := cTmpAliasName
  ::aVarsSql        := {}
  ::cQrySql         := ""
  ::cQrySqlLast     := ""
  ::nOpenAliasCtrl  := 0
  ::lQrySelectStart := .F.

  cNewAlias := cTmpAliasName

  nTmpAliasNewPosition := aScan(st_aCtrlAlias_Tabelas,"_EMPTY_")

  If nTmpAliasNewPosition == 0
    aadd(st_aCtrlAlias_Tabelas,cNewAlias)
    aadd(st_aCtrlAlias_Monitor,cNewAlias+" - "+ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")")
    nTmpAliasNewPosition := aScan(st_aCtrlAlias_Tabelas,cNewAlias)
  Else
    st_aCtrlAlias_Tabelas[nTmpAliasNewPosition] := cNewAlias
    st_aCtrlAlias_Monitor[nTmpAliasNewPosition] := (cNewAlias+" - "+ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")")
  EndIf

  ::nOpenAliasCtrl := nTmpAliasNewPosition

  st_lProcessAutoAlias := .F.

  ::nRecnoLastInsert  := 0
  ::nRecnoLastAppend  := 0
  ::nRecnoVarPutSet   := 0
  ::nRecnoDataBuffer  := 0
  

  ::nOpenType     := 3 // SQL Data Set
  ::nOpenStatus   := 0
  ::cOpenStatus   := "0 - Tabela aberta com sucesso"
  ::cAlias        := cNewAlias
  ::aFilterFix    := {}
  ::aBuffersField := {}

RETURN Self
//-----------------------------------------------------------------------------
METHOD DsExecute(f_nCache,f_lErrorDisplay) Class TAds
  Local cQueryTmp := "", iFor := 0, cTypeData := "C"
  Local aVarsFormatTmp := {}, _cTmpFormat := "", _cTmpData, uResult
  Local cTmp01, aErrorAds := {}, aErrorLog := {}, cErrorBuffer := "", cErrorFile := "", nErroLoop := 1
  Local lExeSucesso := .F.
  
  Default f_nCache := nCacheAds(), f_lErrorDisplay := .T.

  cQueryTmp       := ::cQrySql
  aVarsFormatTmp  := ::aVarsSql

  if !Empty(::cDsWhere)
    cQueryTmp := StrTran(cQueryTmp,"__WHERE__",::cDsWhere)
  Else
    cQueryTmp := StrTran(cQueryTmp,"__WHERE__","")
  EndIf  

  if !Empty(::cDsOrderBy)
    cQueryTmp := StrTran(cQueryTmp,"__ORDER_BY__",::cDsOrderBy)
  Else
    cQueryTmp := StrTran(cQueryTmp,"__ORDER_BY__","")
  EndIf  

  If ::nQryTipo == 0 // Verificar tipo da query
    If At(Upper(cQueryTmp),"SELECT") > 0 .and. At(Upper(cQueryTmp),"SELECT") < 10
      ::nQryTipo := 1 // Cursor Ativo
    Else
      ::nQryTipo := 2 // Query Insert / Update /create /Execute etc
    EndIf
  EndIf

  For iFor := 1 to Len(aVarsFormatTmp)

    cTypeData := ValType(aVarsFormatTmp[iFor,2])

    If cTypeData == "B"
      uResult   := Eval(aVarsFormatTmp[iFor,2])
      cTypeData := ValType(uResult)
    Else
      uResult := (aVarsFormatTmp[iFor,2])
    EndIf

    If cTypeData == "C" 

      uResult := StrTran(uResult,::cDsTxtAlias,::cAlias)
      _cTmpFormat := StrTran(uResult,"'","´")
      _cTmpFormat := "'"+_cTmpFormat+"'"

    ElseIf cTypeData == "D"

      _cTmpData := DTOS(uResult)
      _cTmpData := (SubStr(_cTmpData,1,4)+"-"+SubStr(_cTmpData,5,2)+"-"+SubStr(_cTmpData,7,2))
      _cTmpFormat := "'"+_cTmpData+"'"

    ElseIf cTypeData == "N"

      _cTmpFormat := ALLTRIM(STR(uResult))

    ElseIf cTypeData == "L"

      _cTmpFormat := IF(uResult,"TRUE","FALSE")

    ENDIF

    cQueryTmp := STRTRAN(cQueryTmp,aVarsFormatTmp[iFor,1],_cTmpFormat)

  Next

  If ::nQryTipo == 1 .and. ::lQrySelectStart  

    (::cAlias)->(DbCloseArea())
    
  EndIf

  ::cQrySqlLast := cQueryTmp
  
  Select 0
  AdsCreateSqlStatement(::cAlias,3,::oConexao:hConnectHandle)
  
  (::cAlias)->(AdsPrepareSQL(cQueryTmp))
  lExeSucesso := (::cAlias)->(AdsExecuteSQL())
  
  If !lExeSucesso
    ::nErrorSql := AdsGetLastError(@cErrorBuffer)
  EndIf

  If ::lLogDsExecute
    LogFile("SqlExec.txt",{st_aCtrlAlias_Monitor[::nOpenAliasCtrl],cQueryTmp,cErrorBuffer})
  EndIf
  
  IF ::nErrorSql <> 0
    aadd(aErrorLog,::nErrorSql)
    aadd(aErrorLog,cQueryTmp)
    aadd(aErrorLog,cErrorBuffer)    
    cErrorFile := cErrorBuffer +CRLF
    cErrorFile += Replicate("_",80)+CRLF
    cErrorFile += cQueryTmp+CRLF
    cErrorFile += Replicate("_",80)+CRLF

    while ( nErroLoop < 74 )
      if ! Empty(ProcName( nErroLoop ) )
        cErrorFile += "Called from: " + ProcFile( nErroLoop ) + " => " + Trim( ProcName( nErroLoop ) ) + ;
                        "(" + Alltrim(Str( ProcLine( nErroLoop ))) + ")" + CRLF 
      endif
      nErroLoop++
    end
    
    Hb_MemoWrit("tAdsSqlError.log",cErrorFile)
    
    If ::nQryTipo == 2 // Insert, Update, Execute  / no cursors handles
      (::cAlias)->(DbCloseArea())
    Endif
    
    If f_lErrorDisplay
      MsgInfo(cErrorFile,"tAds Sql Error "+AllTrim(Str(::nErrorSql)))
    EndIf
    
    If ::lDsErrorSave
      //::DsError(::nErrorSql,cErrorBuffer,cQueryTmp,cErrorFile)
    EndIf
    
    ::End()
    
    Return lExeSucesso 
    
  ENDIF

  If ::nQryTipo == 1 // Cursor ativo

    (::cAlias)->(AdsCacheRecords(f_nCache))

    If At("{STATIC}",Upper(cQueryTmp)) > 0 ;
       .or. At("INNER",Upper(cQueryTmp)) > 0 ;
       .or. At(" JOIN ",Upper(cQueryTmp)) > 0
      ::lDsStaticCursors := .T. // Static cursors detect
    EndIf

    If !::lQrySelectStart 
      
      ::cOrderFocus     := ""
      ::aStructTads     := TAds_StructCreate(::cAlias)
      ::nConnectionType := (::cAlias)->(AdsGetTableConType())
      ::aArrayRecnos    := {}
      ::lQrySelectStart := .T.

      //oSelf := Hb_QSelf()
      
      For IFor := 1 To Len(::aStructTads)
        __objAddData(Hb_QSelf(),::aStructTads[IFor,1])
      Next


    EndIf
    ::GoTop()
    //::Refresh()
    ::OnSkip()
    
    If ::lDsCursorsToArray .and. !::lDsCursorsToTemp
      ::DsCursorsToArray()
    EndIf

    If ::lDsCursorsToTemp 
      ::DsCursorsToTemp()
    EndIf

  ElseIf ::nQryTipo == 2 // Insert, Update, Execute  / no cursors handles

    ::End()

  EndIf

RETURN lExeSucesso
//-----------------------------------------------------------------------------
METHOD DsCursorsToArray() Class TAds
  Local iFor := 0, aTmpRegister := {}
  
  ::aDsCursorsHeaders := {}
  ::aDsCursorsData    := {}
  
  For Ifor := 1 To Len(::aStructTads)
    aadd(::aDsCursorsHeaders,::aStructTads[iFor,1])
  Next
 
  ::GoTop()
  Do While !::Eof()
    aTmpRegister := {}
    For Ifor := 1 To Len(::aStructTads)
      aadd(aTmpRegister,(::cAlias)->(FieldGet(iFor)))
    Next
    aadd(::aDsCursorsData,aTmpRegister)
    ::Skip()
  Enddo
  ::GoTop()
  
Return Nil
//-----------------------------------------------------------------------------
METHOD DsCursorsToTemp() Class TAds
  Local iFor := 0, iForField := 0, uGet := {}, aFieldStruct := {}
  Local cTmp := "", nTmp := 0
  
  For Ifor := 1 To Len(::aStructTads)
    aadd(::aDsCursorsHeaders,::aStructTads[iFor,1])
  Next

  For Ifor := 1 To Len(::aFieldsTempIndex)
    ::aFieldsTempIndex[iFor] := Upper(::aFieldsTempIndex[iFor])
  Next

  ::cTableAliasTmp := StrTran(::cAlias,"Sql","Tmp")

  For iFor := 1 To Len(::aStructTads)
    aFieldStruct := ::aStructTads[iFor]
    If aFieldStruct[2] == "AutoIncrement"
      ::aStructTads[iFor,2] := "N"
      ::aStructTads[iFor,3] := 10
      ::aStructTads[iFor,4] := 0
    EndIf
    cTmp := Upper(aFieldStruct[1])
    If Ascan(::aFieldsTempIndex, cTmp) > 0
      ::aStructTads[iFor,5] := 1
    EndIf  
  Next
  
  ::GoTop()
  
  If AdsCheckExistence(::cTableAliasTmp,tAds_GetConnectionHandle(121))
    TAds_TableErase(121,::cTableAliasTmp)
  EndIf
  
  TAds_CreateTableFromCode(121,::cTableAliasTmp,::aStructTads)

  Select 0
  AdsCreateSqlStatement(::cTableAliasTmp,3,121)
  
  AdsConnection(tAds_GetConnectionHandle(121))
  
  USE (::cTableAliasTmp) ALIAS (::cTableAliasTmp) NEW EXCLUSIVE
  
  ///(::cAlias)->(AdsCopyTableContents(::cTableAliasTmp))

  ::DsCursorsToArray() // Carregar cursores para ::aDsCursorsData
  
  For iFor := 1 To Len(::aDsCursorsData)
    (::cTableAliasTmp)->(DbAppend()) 
    For iForField := 1 To Len(::aStructTads)
      (::cTableAliasTmp)->(FieldPut(iForField,::aDsCursorsData[iFor,iForField]))  
    Next
  Next
  
  (::cTableAliasTmp)->(DbCloseArea())
  (::cAlias)->(DbCloseArea())
  
  USE (::cTableAliasTmp) ALIAS (::cAlias) NEW EXCLUSIVE

  AdsConnection(tAds_GetConnectionHandle(tAds_GetConnectionDefault()))
  
  ::lDsStaticCursors := .F.

Return Nil
//-----------------------------------------------------------------------------
METHOD DsAddVar(f_cTxtVarInQuery,f_uVarBlock) Class TAds

  aadd(::aVarsSql,{f_cTxtVarInQuery,f_uVarBlock})

Return Nil
//-----------------------------------------------------------------------------
METHOD DsSetVar(f_cTxtVarInQuery,f_uVarBlock) Class TAds
  Local iFor := 0
  
  For iFor := 1 To Len(::aVarsSql)
    If ::aVarsSql[iFor,1] == f_cTxtVarInQuery
      ::aVarsSql[iFor,2] := f_uVarBlock
      exit
    EndIf
  Next  
    
Return Nil
//-----------------------------------------------------------------------------
METHOD DsError(f_nErrorSql,f_cErrorSql,f_cSqlScript,f_cErrorComplete) Class TAds
  Local oDs_Insert
  Local oReg, cComputer := ""
  
  oReg := TReg32():New( HKEY_LOCAL_MACHINE,;
       "System\CurrentControlSet\Control\ComputerName\ComputerName" )
  cComputer:=oReg:Get("ComputerName")
  oReg:Close()

  oDs_Insert := tAds():DsNew(2,121)
  AdsBeginTransaction(tAds_GetConnectionHandle(121))

  oDs_Insert:DsAddVar("_Dt_Ocorrencia_",Date())
  oDs_Insert:DsAddVar("_Hr_Ocorrencia_",Time())
  oDs_Insert:DsAddVar("_cComputerName_",cComputer)
  oDs_Insert:DsAddVar("_cInfoProcLine_",st_aCtrlAlias_Monitor[::nOpenAliasCtrl])
  oDs_Insert:DsAddVar("_nErrorSql_",f_nErrorSql)
  oDs_Insert:DsAddVar("_cErrorSql_",f_cErrorSql)
  oDs_Insert:DsAddVar("_cSqlScript_",f_cSqlScript)
  oDs_Insert:DsAddVar("_cErrorComplete_",f_cErrorComplete)
  oDs_Insert:cQrySql := "Insert Into TADS_ERR "+;
                        "(Dt_Ocorrencia, Hr_Ocorrencia, cComputerName, cInfoProcLine, nErrorSql,"+;
                        "cErrorSql, cSqlScript, cErrorComplete) "+;
                        "VALUES "+;
                        "(_Dt_Ocorrencia_, _Hr_Ocorrencia_, _cComputerName_, _cInfoProcLine_, _nErrorSql_,"+;
                        "_cErrorSql_, _cSqlScript_, _cErrorComplete_);"
  oDs_Insert:lLogDsExecute  := .T.
  oDs_Insert:lDsErrorSave   := .F.
  oDs_Insert:DsExecute()

  AdsCommitTransaction(tAds_GetConnectionHandle(121))
  
Return Nil

//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_AliasesAutoOpened()
LOCAL nFor := 0, vText:="", cTmp := ""

  If hb_IsNil(st_aCtrlAlias_Monitor)
    Return ""
  EndIf

  FOR nFor:=1 to len(st_aCtrlAlias_Monitor)
    cTmp := st_aCtrlAlias_Monitor[nFor]
    If cTmp != "_EMPTY_"
      vText += (st_aCtrlAlias_Monitor[nFor]+CRLF)
    EndIf
  NEXT

  /*
  If !Empty(vText)
    xbrowse(st_aCtrlAlias_Monitor,"teste de teste",.T.)
    xbrowse(agetworkareas(),"teste de teste",.F.)
  EndIf
 */
 
Return vText
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
#pragma BEGINDUMP

#define HB_OS_WIN_32_USED

#include <windows.h>
#include "hbapi.h"
#include "hbapiitm.h"
#include "hbinit.h"
#include "hbvm.h"
#include "rddsys.ch"
#include "hbapilng.h"
#include "hbdate.h"
#include "hbapierr.h"
#include "rddads.h"

HB_FUNC( ADSFLUSHFILEBUFFERS )  //////  giovany
{
   ADSAREAP pArea;

   pArea = (ADSAREAP) hb_rddGetCurrentWorkAreaPointer();
   if( pArea )
      AdsFlushFileBuffers( pArea->hTable );
   else
      hb_errRT_DBCMD( EG_NOTABLE, 2001, NULL, "ADSFLUSHFILEBUFFERS" );
}

#pragma ENDDUMP

*/