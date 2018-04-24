////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções referentes as propriedades SYSTEM do Dicionario de dados                               //
// Qualquer alteração ou modificação encaminhe para Giovany Vecchi                                //
// Giovany Vecchi 21/01/2014 giovanyvecchi@yahoo.com.br / giovanyvecchi@gmail.com                 //
////////////////////////////////////////////////////////////////////////////////////////////////////

#Include "FiveWin.ch"

FUNCTION TAds_SystemTablesNames(f_nConnection)
  Local aTables := {}, hConnectionFocus := AdsConnection()
  Local oDs_TablesNames

  Default f_nConnection := tAds_GetConnectionDefault()
 
  oDs_TablesNames := tAds():DsNew(1,f_nConnection)
  oDs_TablesNames:cQrySql := "SELECT {static} * FROM System.tables"
  oDs_TablesNames:DsExecute(30)
  
  //(oDs_TablesNames:cAlias)->(xBrowse())

  Do While !oDs_TablesNames:Eof()
    aadd(aTables,oDs_TablesNames:VarGetAlltrim("NAME"))
    oDs_TablesNames:Skip()
  EndDo
  oDs_TablesNames:End()

RETURN aTables
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_SystemTableInfo(f_nConnection,f_cTableName)
  Local oDs_Qry, aTableInfo := Array(12)

  Default f_nConnection := tAds_GetConnectionDefault()
  
  oDs_Qry := tAds():DsNew(1,f_nConnection)
  oDs_Qry:cQrySql := "SELECT {static} * FROM System.Tables Where NAME = '"+f_cTableName+"'"
  oDs_Qry:DsExecute(1)
  oDs_Qry:GoTop()

  //(oDs_Qry:cAlias)->(xBrowse())
  //xBrowse(oDs_Qry:aStruct)
  aTableInfo[01] := oDs_Qry:VarGetRtrim("NAME")                   // 01-Nome da Tabela
  aTableInfo[02] := oDs_Qry:VarGetRtrim("TABLE_RELATIVE_PATH")    // 02-Path relacionado ao arquivo Fisico 
  aTableInfo[03] := oDs_Qry:VarGet("TABLE_TYPE")                  // 03-Tipo da tabela / 1-DBFNTX /2-DBFCDX /3-ADT 
  aTableInfo[04] := oDs_Qry:VarGet("TABLE_AUTO_CREATE")           // 04-Criar automaticamente ao abrir 
  aTableInfo[05] := oDs_Qry:VarGetRtrim("TABLE_PRIMARY_KEY")      // 05-Tag primario  
  aTableInfo[06] := oDs_Qry:VarGetRtrim("TABLE_DEFAULT_INDEX")    // 06-Chave de indice por falta  
  aTableInfo[07] := oDs_Qry:VarGet("TABLE_ENCRYPTION")            // 07-Se a Tabela esta Encriptada  
  aTableInfo[08] := oDs_Qry:VarGet("TABLE_PERMISSION_LEVEL")      // 08- 1-Allow Hidden Field Filters /2-Prevent Hidden Field Filters /3-SQL Access Only
  aTableInfo[09] := oDs_Qry:VarGet("TABLE_MEMO_BLOCK_SIZE")       // 09-Tamanho do bloco em Bytes para campos Memo ou Binarios 
  aTableInfo[10] := oDs_Qry:VarGetRtrim("TABLE_VALIDATION_EXPR")  // 10-Define uma nova expressão de validação nível recorde para a tabela
  aTableInfo[11] := oDs_Qry:VarGetRtrim("TABLE_VALIDATION_MSG")   // 11-mensagem de erro que será devolvido quando um registro da tabela está sendo modificado e ultrapassou o limite de Validação   
  aTableInfo[12] := oDs_Qry:VarGetRtrim("COMMENT")                // 12-Comentario do Campo 

  oDs_Qry:End()
  
Return aTableInfo 
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_SystemIndexes(f_nConnection,f_cTableName)
  Local oDs_Qry, nArrayPos := 0, aIndexInfo := {}

  Default f_nConnection := tAds_GetConnectionDefault()
  
  oDs_Qry := tAds():DsNew(1,f_nConnection)
  oDs_Qry:cQrySql := "SELECT {static} * FROM System.Indexes Where PARENT = '"+f_cTableName+"'"
  oDs_Qry:DsExecute(10)
  oDs_Qry:GoTop()

  //(oDs_Qry:cAlias)->(xBrowse())
  //xBrowse(oDs_Qry:aStruct)

  Do While !oDs_Qry:Eof()  
    aadd(aIndexInfo,Array(14))
    nArrayPos := Len(aIndexInfo)
    aIndexInfo[nArrayPos,01] := oDs_Qry:VarGetRtrim("NAME")                 // 01-Nome da Chave do Indice
    aIndexInfo[nArrayPos,02] := oDs_Qry:VarGetRtrim("PARENT")               // 02-Tabela correspondente ao indice 
    aIndexInfo[nArrayPos,03] := oDs_Qry:VarGetRtrim("INDEX_FILE_NAME")      // 03-Nome do arquivo de Indice correspondente 
    aIndexInfo[nArrayPos,04] := oDs_Qry:VarGetRtrim("INDEX_EXPRESSION")     // 04-Campo correspondente a chave 
    aIndexInfo[nArrayPos,05] := oDs_Qry:VarGetRtrim("INDEX_CONDITION")      // 05-Condição da chave  
    aIndexInfo[nArrayPos,06] := oDs_Qry:VarGet("INDEX_OPTIONS")             // 06-Opção da chave /Default 2-ADS_COMPOUND  
    aIndexInfo[nArrayPos,07] := oDs_Qry:VarGet("INDEX_KEY_LENGTH")          // 07-Comprimento da chave  
    aIndexInfo[nArrayPos,08] := oDs_Qry:VarGet("INDEX_FTS_MIN_LENGTH")      // 08-Comprimento minimo para busca em indice FTS 
    aIndexInfo[nArrayPos,09] := oDs_Qry:VarGetRtrim("INDEX_FTS_DELIMITERS") // 09-Caracteres delimitadores em indices FTS 
    aIndexInfo[nArrayPos,10] := oDs_Qry:VarGetRtrim("INDEX_FTS_NOISE")      // 10-Caracteres ruidos em indices FTS 
    aIndexInfo[nArrayPos,11] := oDs_Qry:VarGetRtrim("INDEX_FTS_DROP_CHARS") // 11-Caracteres de separação   
    aIndexInfo[nArrayPos,12] := oDs_Qry:VarGetRtrim("INDEX_FTS_CONDITIONAL_CHARS") // 12-Caracteres de cortes descartaveis 
    aIndexInfo[nArrayPos,13] := oDs_Qry:VarGetRtrim("COMMENT")              // 13-Comentario do indice 
    aIndexInfo[nArrayPos,14] := oDs_Qry:VarGetRtrim("INDEX_COLLATION")      // 14-Colação dos caracteres 

    oDs_Qry:Skip()
  EndDo
  
  oDs_Qry:End()
  
Return aIndexInfo 

///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_SystemServerDate(f_nConnection)
  Local dServer := Date()
  Local oDs_Qry, aResult := ""

  Default f_nConnection := tAds_GetConnectionDefault()
  
  oDs_Qry := tAds():DsNew(1,f_nConnection)
  oDs_Qry:cQrySql := "SELECT CurDate() as DataAtual FROM System.IOTA"
  oDs_Qry:DsExecute(1)

  dServer := oDs_Qry:VarGet("DataAtual")
  
  oDs_Qry:End()

RETURN dServer
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_SystemServerTime(f_nConnection)
  Local cServerTime := Time()
  Local oDs_Qry, aResult := ""

  Default f_nConnection := tAds_GetConnectionDefault()
  
  oDs_Qry := tAds():DsNew(1,f_nConnection)
  oDs_Qry:cQrySql := "SELECT CurTime() as TimeAtual FROM System.IOTA"
  oDs_Qry:DsExecute(1)
  
  cServerTime := SubStr(cValToStr(oDs_Qry:VarGet("TimeAtual")),1,8)
 
  oDs_Qry:End()

RETURN cServerTime
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_SystemColumnsTable(f_nConnection,f_cTableName)
  Local oDs_Qry, aColumns := {}, aFieldsInfo := TAds_StructInfo()
  Local nArrayPos := 0, iFor := 0

  ///hb_default(f_nConnection,tAds_GetConnectionDefault())
  Default f_nConnection := tAds_GetConnectionDefault()
  
  oDs_Qry := tAds():DsNew(1,f_nConnection)
  oDs_Qry:cQrySql := "SELECT {static} * FROM System.Columns Where PARENT = '"+f_cTableName+"'"
  oDs_Qry:DsExecute(0)
  oDs_Qry:GoTop()
  
  Do While !oDs_Qry:Eof()

    aadd(aColumns,Array(20))
    nArrayPos := Len(aColumns)
    aColumns[nArrayPos,01] := oDs_Qry:VarGet("FIELD_NUM")             // 01-Posição do Registro
    aColumns[nArrayPos,02] := oDs_Qry:VarGetRtrim("NAME")             // 02-Descrição do Registro
    aColumns[nArrayPos,03] := oDs_Qry:VarGet("FIELD_TYPE")            // 03-Tipo do Registro Tabela Adt
    aColumns[nArrayPos,04] := aFieldsInfo[oDs_Qry:VarGet("FIELD_TYPE")] // 04-Descrição do tipo do Registro
    aColumns[nArrayPos,05] := oDs_Qry:VarGet("FIELD_LENGTH")          // 05-Tamanho do Registro
    aColumns[nArrayPos,06] := oDs_Qry:VarGet("FIELD_DECIMAL")         // 06-Casas Decimais
    aColumns[nArrayPos,07] := oDs_Qry:VarGet("FIELD_MIN_VALUE")       // 07-Valor Minimo estipulado
    aColumns[nArrayPos,08] := oDs_Qry:VarGet("FIELD_MAX_VALUE")       // 08-Valor Maximo estipulado
    aColumns[nArrayPos,09] := oDs_Qry:VarGet("FIELD_CAN_BE_NULL")     // 09-Se é valido quando Null
    aColumns[nArrayPos,10] := oDs_Qry:VarGet("FIELD_DEFAULT_VALUE")   // 10-Valor por falta
    aColumns[nArrayPos,11] := oDs_Qry:VarGet("FIELD_VALIDATION_MSG")  // 11-Mensagem quando ocorrer erros
    aColumns[nArrayPos,12] := oDs_Qry:VarGet("COMMENT")               // 12-Comentario do campo
    aColumns[nArrayPos,13] := oDs_Qry:VarGet("USER_DEFINED_PROP")     // 13-
    aColumns[nArrayPos,14] := oDs_Qry:VarGet("FIELD_OPTIONS")         // 14-

    oDs_Qry:Skip()
      
  EndDo
  
  oDs_Qry:End()

RETURN aColumns
///////////////////////////////////////////////////////////////////////////////
//// List Functions in Data Dictionary (complete coluns)
FUNCTION TAds_SystemFunctions(f_nConnection) // > array Functions
  Local aFunctions := {}, nArrayPos := 0
  Local oDs_Qry

  Default f_nConnection := tAds_GetConnectionDefault()
  
  oDs_Qry := tAds():DsNew(1,f_nConnection)
  oDs_Qry:cQrySql := "SELECT {static} * FROM System.Functions ;"
  oDs_Qry:DsExecute(10)
  
  Do While !oDs_Qry:Eof()

    aadd(aFunctions,Array(7))
    nArrayPos := Len(aFunctions)
    aFunctions[nArrayPos,01]  := oDs_Qry:VarGetAlltrim("Name")
    aFunctions[nArrayPos,02]  := oDs_Qry:VarGetAlltrim("Package")
    aFunctions[nArrayPos,03]  := oDs_Qry:VarGetAlltrim("Return Type")
    aFunctions[nArrayPos,04]  := oDs_Qry:VarGetAlltrim("Input Parameters")
    aFunctions[nArrayPos,05]  := oDs_Qry:VarGetAlltrim("Implementation")
    aFunctions[nArrayPos,06]  := oDs_Qry:VarGetAlltrim("Comment")
    aFunctions[nArrayPos,07]  := oDs_Qry:VarGet("User_Defined_Prop")

    oDs_Qry:Skip()
      
  EndDo
 
  oDs_Qry:End()

RETURN aFunctions
///////////////////////////////////////////////////////////////////////////////
//// List Functions names in Data Dictionary (names only)
FUNCTION TAds_SystemFunctionsNames(f_nConnection) // > array Functions Names
  Local aFunctionsComplete := {}, aFunctionsNames := {}
  Local iFor := 0

  Default f_nConnection := tAds_GetConnectionDefault()
  
  aFunctionsComplete  := TAds_SystemFunctions(f_nConnection) 
  
  For iFor := 1 To Len(aFunctionsComplete)
    aadd(aFunctionsNames,aFunctionsComplete[iFor,1])
  Next

RETURN aFunctionsNames
