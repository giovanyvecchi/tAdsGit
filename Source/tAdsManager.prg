////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções para operações fisicas nas tabelas e dicionario de dados                               //
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

#DEFINE DF_CURSOR_ON    1
#DEFINE DF_CURSOR_OFF   2
#define ADS_ASCENDING                 0x00000000
#define ADS_UNIQUE                    0x00000001
#define ADS_COMPOUND                  0x00000002
#define ADS_CUSTOM                    0x00000004
#define ADS_DESCENDING                0x00000008
#define ADS_USER_DEFINED              0x00000010

/// Retorno de DbStruct de Harbour
#define HB_FT_NONE            0
#define HB_FT_STRING          1     /* "C" */
#define HB_FT_LOGICAL         2     /* "L" */
#define HB_FT_DATE            3     /* "D" */
#define HB_FT_LONG            4     /* "N" */
#define HB_FT_FLOAT           5     /* "F" */
#define HB_FT_INTEGER         6     /* "I" */
#define HB_FT_DOUBLE          7     /* "B" */
#define HB_FT_TIME            8     /* "T" */
#define HB_FT_TIMESTAMP       9     /* "@" */
#define HB_FT_MODTIME         10    /* "=" */
#define HB_FT_ROWVER          11    /* "^" */
#define HB_FT_AUTOINC         12    /* "+" */
#define HB_FT_CURRENCY        13    /* "Y" */
#define HB_FT_CURDOUBLE       14    /* "Z" */
#define HB_FT_VARLENGTH       15    /* "Q" */
#define HB_FT_MEMO            16    /* "M" */
#define HB_FT_ANY             17    /* "V" */
#define HB_FT_IMAGE           18    /* "P" */
#define HB_FT_BLOB            19    /* "W" */
#define HB_FT_OLE             20    /* "G" */

STATIC st_cAliasCopia, st_aEstruturaCopia, st_lExeTableErase

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_BeginTransaction(f_nConnection)
  Local hHandleAction, lOk := .F.
  
  Default f_nConnection := tAds_GetConnectionDefault()

  hHandleAction := tAds_GetConnectionHandle(f_nConnection)
  
  lOk := AdsBeginTransaction(hHandleAction)

RETURN lOk  
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_CommitTransaction(f_nConnection)
  Local hHandleAction, lOk := .F.
  
  Default f_nConnection := tAds_GetConnectionDefault()

  hHandleAction := tAds_GetConnectionHandle(f_nConnection)
  
  lOk := AdsCommitTransaction(hHandleAction)

RETURN lOk  
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_RollBackTransaction(f_nConnection)
  Local hHandleAction, lOk := .F.
  
  Default f_nConnection := tAds_GetConnectionDefault()

  hHandleAction := tAds_GetConnectionHandle(f_nConnection)
  
  lOk := AdsRollback(hHandleAction)

RETURN lOk  

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_StructInfo()
  Local aStructInfo := {}

  aadd(aStructInfo,"Logical")     // 01-Logical / DBF and ADT / 1 Byte
                                  //   1-byte logical (boolean) field. Recognized values for True are
                                  //   ‘1’, ‘T’, ‘t’, ‘Y’, and ‘y’.

  aadd(aStructInfo,"Numeric")     // 02-Numeric / DBF and ADT / 2 to 32 (Decimal: 0 to Length-2)
                                  //   Fixed-length (exact ASCII representation) numeric. One byte is reserved
                                  //   for the sign of the numeric value. If the decimal value is not zero,
                                  //   one additional byte is used for the decimal point.

  aadd(aStructInfo,"Date")        // 03-Date / DBF and ADT / 4 byte
                                  //   4-byte integer containing a Julian date.
  
  aadd(aStructInfo,"Char")      // 04-String or Character / DBF and ADT / 1 to 65530 Bytes
                                  //   Fixed-length character field that is stored entirely in the table.

  aadd(aStructInfo,"Memo")        // 05-Memo / DBF and ADT / 9 Bytes
                                  //   Variable-length memo field containing character data. The size of each
                                  //   field is limited to 4 GB. The memo data is actually stored in a 
                                  //   separate file, called a memo file, to reduce table bloat.

  aadd(aStructInfo,"Blob")      // 06-Binary / DBF and ADT / 9 Bytes
                                  //   Variable-length memo field containing binary data. The size of each field
                                  //   is limited to 4 GB. The binary data is actually stored in a separate file,
                                  //   called a memo file, to reduce table bloat.

  aadd(aStructInfo,"Image")       // 07-Image / DBF and ADT / 9 Bytes
                                  //   Variable-length memo field containing binary image data. The size of each
                                  //   field is limited to 4 GB. The binary image data is actually stored in a
                                  //   separate file, called a memo file, to reduce table bloat.
 
  aadd(aStructInfo,"Varchar")     // 08-Varchar / DBF_VFP and ADT / 1 to 65000 Bytes
                                  //   This field type allows variable length character data to be stored up to
                                  //   the maximum field length, which is specified when the table is created.
                                  //   It is similar to a character field except that the exact same data will be
                                  //   returned when it is read without extra blank padding on the end. If you 
                                  //   are creating this field using the Advantage Client Engine API directly 
                                  //   (e.g., AdsCreateTable), you must specify the type as "VarCharFox" to avoid
                                  //   legacy compatibility issues with an older obsolete varchar field type.
 
  aadd(aStructInfo,"Compactdate") // 09-Compactdate / DBF only / 4 Bytes

  aadd(aStructInfo,"Double")      // 10-Double / DBF and ADT / 8 Bytes
                                  //   8-byte IEEE floating point value in the range 1.7E +/-308 (15 digits of
                                  //   precision). The decimal value affects the use of the field in expressions.
                                  //   It does not affect the precision of the stored data. If the length is given,
                                  //   it will be ignored. For example, "salary, double, 10, 2" and "salary,
                                  //   double, 2" produce the same field.

  aadd(aStructInfo,"Integer")     // 11-Integer / DBF and ADT / 4 Bytes
                                  //   4-byte long integer values from -2,147,483,647 to 2,147,483,647. 

  aadd(aStructInfo,"Short")    // 12-ShortInt / ADT only / 2 Bytes
                                  //   2-byte short integer value from -32,767 to 32,767.
 
  aadd(aStructInfo,"Time")        // 13-Time / ADT only / 2 Bytes 
                                  //   4-byte integer internally stored as the number of milliseconds since midnight.
                                  
  aadd(aStructInfo,"TimeStamp")   // 14-TimeStamp / ADT only / 8 Bytes
                                  //   8-byte value where the high order 4 bytes are an integer containing a Julian
                                  //   date, and the low order 4 bytes are internally stored as the number of 
                                  //   milliseconds since midnight. If using the Advantage CA-Visual Objects RDDs,
                                  //   this is a string type.

  aadd(aStructInfo,"AutoInc")     // 15-AutoInc / ADT only / 4 Bytes 
                                  //   4-byte read-only positive integer value from 0 to 4,294,967,296 that is 
                                  //   unique for each record in the table.

  aadd(aStructInfo,"Raw")         // 16-Raw / ADT only / 1 to 65530 Bytes
                                  //   Fixed-length, data-typeless raw data field. If using the Advantage CA-Visual
                                  //   Objects RDDs, Advantage Client Engine APIs must be used to set and retrieve
                                  //   the raw data.

  aadd(aStructInfo,"CurDouble")   // 17-CurDouble / ADT only / 8 Bytes 
                                  //   Currency data stored internally as an 8-byte IEEE floating-point value in 
                                  //   the range 1.7E +/-308 (15 digits of precision). The decimal value affects
                                  //   the use of the field in expressions. It does not affect the precision of the
                                  //   stored data. If the length is given, it will be ignored. For example, 
                                  //   "salary, CurDouble, 10, 2" and "salary, CurDouble, 2" produce the same field.

  aadd(aStructInfo,"Money")       // 18-Money / ADT only / 8 Bytes
                                  //   Currency data stored internally as a 64-bit integer, with 4 implied decimal
                                  //   digits from -922,337,203,685,477.5807 to +922,337,203,685,477.5807. 
                                  //   The Money data type will not lose precision.

  aadd(aStructInfo,"LongLong")    // 19-LongLong / ADT only
  
  aadd(aStructInfo,"CIChar")    // 20-CIString / ADT only / 1 to 65530 Bytes
                                  //   Case insensitive fixed-length character field that is stored entirely in
                                  //   the table.

  aadd(aStructInfo,"RowVersion")  // 21-RowVersion / ADT only / 8 Bytes
                                  //   An 8-byte unsigned integer unique for each record in the table that is
                                  //   automatically incremented each time a record is updated.

  aadd(aStructInfo,"ModTime")     // 22-ModTime / ADT only / 8 Byte
                                  //   8-byte value where the high order 4 bytes are an integer containing a Julian
                                  //   date, and the low order 4 bytes are internally stored as the number of
                                  //   milliseconds since midnight. If using the Advantage CA-Visual Objects RDDs,
                                  //   this is a string type. The value of this field is automatically updated with
                                  //   the current date and time each time a record is updated.

  aadd(aStructInfo,"VarCharFox")  // 23-VarCharFox / DBF and ADT / 1 to 65000 Bytes
                                  //   This field type allows variable length character data to be stored up to
                                  //   the maximum field length, which is specified when the table is created.
                                  //   It is similar to a character field except that the exact same data will be
                                  //   returned when it is read without extra blank padding on the end. If you 
                                  //   are creating this field using the Advantage Client Engine API directly 
                                  //   (e.g., AdsCreateTable), you must specify the type as "VarCharFox" to avoid
                                  //   legacy compatibility issues with an older obsolete varchar field type.

  AADD(aStructInfo,"VarBinaryFox") // 24-VarBinaryFox / DBF and ADT / 1 to 65000 Bytes
                                  //   Variable length binary data. The maximum length of data that can be stored
                                  //   in the field is specified when the table is created. This is similar to
                                  //   the Raw field type except that the true length of the data is stored 
                                  //   internally in the record.

  aadd(aStructInfo,"SystemField") // 25-SystemField / Internal use only

  aadd(aStructInfo,"nChar")       // 26-NChar / DBF and ADT / 1 to 32500
                                  //   Fixed length Unicode character field that is stored entirely in the table.
                                  //   The length specified for the field is the number of UTF16 code units or
                                  //   characters. The internal storage uses UTF16 encoding so the number of 
                                  //   bytes occupied by the field in each record is 2 times the specified length.

  aadd(aStructInfo,"NVarChar")    // 27-NVarChar / DBF and ADT / 1 to 32500
                                  //   Variable length Unicode character data. The field is stored entirely in
                                  //   the table. The maximum length of the data that can be stored in the field
                                  //   is specified when the table is created. The internal storage uses UTF16
                                  //   encoding so the number of bytes occupied by the field in each record is 2
                                  //   times the specified length plus 2 bytes for the length. 

  aadd(aStructInfo,"NMemo")       // 28-NMemo / DBF and ADT / 9 Bytes
                                  //   Variable length Unicode memo field. The maximum length of data that can be
                                  //   stored in the field is 4GB. Since UTF16 is used as the internal storage,
                                  //   the number of UTF16 code units is limited to 2G. The data is stored in a
                                  //   separate file, called a memo file. 


RETURN aStructInfo
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Copia a estrutura padrão da Tabela e acrecenta outras opções para ser usado com TAds_CreateTableFromCode
// f_cAlias > Alias da tabela aberta com USE ou DsNew
FUNCTION TAds_StructCreate(f_cAlias)
  Local iFor := 0, aStructTadsRet := {}

  aStructTadsRet := (f_cAlias)->(DbStruct())

  For iFor := 1 to Len(aStructTadsRet)
    aadd(aStructTadsRet[iFor],0)    // 5 - Opc Index / 0-None 1-COMPOUND 2-UNIQUE 3-ASCENDING 4-DESCENDING 5-CUSTOM
    aadd(aStructTadsRet[iFor],Nil)  // 6 - Comentario do Campo
    aadd(aStructTadsRet[iFor],Nil)  // 7 - Valor default para o Campo
  Next

RETURN aStructTadsRet
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_StructToScript(f_aStructPointer)
  Local cTmp := "", nTmp := 0, cStructRetorno := ""

  cStructRetorno := f_aStructPointer[1]
  
  If f_aStructPointer[2] == "C"
    cTmp := AllTrim(Str(Int(f_aStructPointer[3])))
    cStructRetorno += " Char( " + cTmp + " )"
  Elseif f_aStructPointer[2] == "D"  
    cStructRetorno += " Date"
  Elseif f_aStructPointer[2] == "L"  
    cStructRetorno += " Logical"
  Elseif f_aStructPointer[2] == "M"  
    cStructRetorno += " Memo"
  Elseif f_aStructPointer[2] == "N"  
    cTmp := TAds_Str(f_aStructPointer[3]+1)
    cStructRetorno += " Numeric( " + cTmp 
    nTmp := f_aStructPointer[4]
    If nTmp == 1
      nTmp := 2
    Endif  
    cTmp := AllTrim(Str(Int(nTmp)))
    cStructRetorno += " , " + cTmp + ") "
  Elseif f_aStructPointer[2] == "Q"  
    cTmp := AllTrim(Str(Int(f_aStructPointer[3])))
    cStructRetorno += " VarChar( " + cTmp + " )"
  Elseif f_aStructPointer[2] == "I"  
    cStructRetorno += " Integer"
  Elseif f_aStructPointer[2] == "B"  
    cStructRetorno += " Double"
  Elseif f_aStructPointer[2] == "W"  
    cStructRetorno += " Blob"
  Elseif f_aStructPointer[2] == "+"  
    cStructRetorno += " AutoInc"
  Elseif f_aStructPointer[2] == "@"  
    cStructRetorno += " TimeStamp"
  ElseIf Upper(f_aStructPointer[2]) == "CHAR"
    cTmp := AllTrim(Str(Int(f_aStructPointer[3])))
    cStructRetorno += " Char( " + cTmp + " )"
  ElseIf Upper(f_aStructPointer[2]) == "CICHAR"
    cTmp := AllTrim(Str(Int(f_aStructPointer[3])))
    cStructRetorno += " Char( " + cTmp + " )"
  ElseIf Upper(f_aStructPointer[2]) == "CICHARACTER"
    cTmp := AllTrim(Str(Int(f_aStructPointer[3])))
    cStructRetorno += " Char( " + cTmp + " )"
  Elseif Upper(f_aStructPointer[2]) == "CISTRING"  
    cTmp := AllTrim(Str(Int(f_aStructPointer[3])))
    cStructRetorno += " CIChar( " + cTmp + " )"
  Elseif Upper(f_aStructPointer[2]) == "NUMERIC"  
    cTmp := TAds_Str(f_aStructPointer[3]+1)
    cStructRetorno += " Numeric( " + cTmp 
    nTmp := f_aStructPointer[4]
    If nTmp == 1
      nTmp := 2
    Endif  
    cTmp := AllTrim(Str(Int(nTmp)))
    cStructRetorno += " , " + cTmp + ") "
  Elseif Upper(f_aStructPointer[2]) == "SHORTINT"  
    cStructRetorno += " Short"
  Elseif Upper(f_aStructPointer[2]) == "SHORTINTEGER"  
    cStructRetorno += " Short"

  Else
    cStructRetorno += " "+f_aStructPointer[2]
  endif  

RETURN cStructRetorno
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CreateDataDictionary(f_cFileAdd,f_cComment)
  Local lCreate := .F.
  
  Default f_cComment := "New Data Dictionary"
  
  lCreate := AdsDdCreate(f_cFileAdd,0,f_cComment)

RETURN lCreate
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_TableExist(f_nConnection,f_cTableName)
  Local lExist := .F., oConexao

  Default f_nConnection := tAds_GetConnectionDefault()
  
  oConexao := tAds_GetConnectionObj(f_nConnection)
  
  lExist := AdsCheckExistence(f_cTableName,oConexao:hConnectHandle)
  
  If !lExist .and. f_nConnection == 121
    fErase(tAds_GetPathTemp()+f_cTableName+".adt")
    fErase(tAds_GetPathTemp()+f_cTableName+".adi")
    fErase(tAds_GetPathTemp()+f_cTableName+".adm")
  EndIf

RETURN lExist
///////////////////////////////////////////////////////////////////////////////
// TAds_TableErase - Erase Table
// f_nConnection  > Number Connection in tAdsConnection
// f_cTableName   > Name of Table
FUNCTION TAds_TableErase(f_nConnection,f_cTableName)
  Local oDs_Qry, lExecute := .F., iFor := 0

  Default f_nConnection := tAds_GetConnectionDefault()

  If HB_ISNIL(st_lExeTableErase)
    st_lExeTableErase := .F.
  EndIf

  For iFor := 1 to 50
    if st_lExeTableErase
      hb_idleSleep(.02)
    Else
      Exit
    EndIf
  Next

  st_lExeTableErase := .T.
  
  ///? "TAds_TableExist", f_cTableName
  
  If !TAds_TableExist(f_nConnection,f_cTableName)
    TADS_LOGFILE("tAdsError.log",{"TAds_TableErase - TAds_TableExist() / Incorrect erase / Table not exist: "+;
                             f_cTableName +" Con: "+Alltrim(Str(f_nConnection)), ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
    st_lExeTableErase := .F.
    Return .T.
  EndIf

  ////? HB_TADS_LOGFILE()
    
  If f_nConnection == 121 // Temporary Table local
    
    For iFor := 1 to 50
      AdsConnection(tAds_GetConnectionHandle(121))
      lExecute := AdsDDRemoveTable(f_cTableName,.T.)
      If lExecute
        Exit 
      Else
        TADS_LOGFILE("tAdsError.log",{"iFor = "+Alltrim(Str(iFor))+" Con: "+AllTrim(Str(121)),;
                                 "TAds_TableErase - AdsDDRemoveTable / Not Remove table: "+f_cTableName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
        hb_idleSleep(.1)
      EndIf
    Next
    AdsConnection(tAds_GetConnectionDefault())
  Else
    
    oDs_Qry := tAds():DsNew(2,f_nConnection)
    oDs_Qry:cQrySql := "DROP TABLE "+f_cTableName+" FROM DATABASE ;"
    lExecute := oDs_Qry:DsExecute()
    oDs_Qry:End()

  EndIf
  
  If f_nConnection == 121 // Temporary Table local
    For iFor := 1 to 80 
      fErase(tAds_GetPathTemp()+f_cTableName+".adt")
      fErase(tAds_GetPathTemp()+f_cTableName+".adi")
      fErase(tAds_GetPathTemp()+f_cTableName+".adm")
      If !File(tAds_GetPathTemp()+f_cTableName+".adt")
        Exit
      Else
        hb_idleSleep(.1)
      EndIf
    Next
    If File(tAds_GetPathTemp()+f_cTableName+".adt")
      TADS_LOGFILE("tAdsError.log",{"TAds_TableErase() - Not fErase "+f_cTableName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
    EndIf
  EndIf
  
  st_lExeTableErase := .F.
  //If !lExecute
  //  ? "TAds_TableErase retornando lExecute .F.",f_cTableName
  //EndIf
  
RETURN lExecute 
///////////////////////////////////////////////////////////////////////////////
// TAds_TableReindex - Reindex Table
// f_nConnection  > Number Connection in tAdsConnection
// f_cTableName   > Name of Table
// f_nPageSize    > Page size / 512 to 8192 / 0-Default Page Size set
FUNCTION TAds_ReindexTable(f_nConnection,f_cTableName,f_nPageSize)
  Local oDs_Qry, oDb_Table, lExecute := .F., nAdsVersion := Val(AdsVersion())

  Default f_nConnection := tAds_GetConnectionDefault(), f_nPageSize := 0 // 0-Default Page size (Será atribuido conforme a necessidade)
  
  IF AT(".ADT",UPPER(f_cTableName)) == 0 .and. nAdsVersion >= 10
    ///f_cTableName += ".ADT"
  EndIf
    
  If nAdsVersion >= 10
    oDs_Qry := tAds():DsNew(2,f_nConnection)
    oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_Reindex( '"+f_cTableName+"',"+Alltrim(Str(f_nPageSize))+" )"
    lExecute := oDs_Qry:DsExecute()
  Else
    oDb_Table := TAds():NewRdd(f_cTableName,f_nConnection,0,.T.)
    lExecute := (oDb_Table:cAlias)->(AdsReindex())
    oDb_Table:End()
  EndIf
  
RETURN lExecute 
///////////////////////////////////////////////////////////////////////////////
// TAds_ReindexAllTables - Reindex All Tables in Data Dictionary
// f_nConnection  > Number Connection in tAdsConnection
FUNCTION TAds_ReindexAllTables(f_nConnection,f_aTablesIgnore)
  Local aTablesInDictionary := {}, iFor := 0
  
  Default f_aTablesIgnore := {}
  
  aTablesInDictionary := TAds_SystemTablesNames(f_nConnection)
  
  For Ifor := 1 To Len(aTablesInDictionary)
    If ascan(f_aTablesIgnore,aTablesInDictionary[iFor]) == 0
      //? aTablesInDictionary[iFor]
      TAds_ReindexTable(f_nConnection,aTablesInDictionary[iFor])
    EndIf
  Next

RETURN Nil 
///////////////////////////////////////////////////////////////////////////////
// Criar nova tabela no Dicionario de dados usando estrutura padrão 
// f_nConnection > Number Connection in tAdsConnection
// f_cTableName > Nome da nova tabela
// f_aStruct > Estrutura do arquivo estilo dbstruct com outras opções
//      f_aStruct[1] > Nome/descrição do Campo
//      f_aStruct[2] > Tipo do Campo - Ver TAds_StructInfo() 
//      f_aStruct[3] > Lengh/Comprimento do Campo
//      f_aStruct[4] > Decimal do Campo
//      f_aStruct[5] > Opc Index / 0-None 1-COMPOUND 2-UNIQUE 3-ASCENDING 4-DESCENDING 5-CUSTOM
//      f_aStruct[6] > Comentario do Campo
//      f_aStruct[7] > Valor default para o Campo
// f_cDescription > Descrição da tabela  
// f_aPropertys > Propriedades da nova tabela
//      f_aPropertys[1] > Memo block file - Default 8
//      f_aPropertys[2] > Triggers Disabled - Default False
//      f_aPropertys[3] > Table Caching - 0-None 1-Reads 2-Writes / Default 1-Reads
//      f_aPropertys[4] > Table Trans Free - Default False
FUNCTION TAds_CreateTableFromCode(f_nConnection,f_cTableName,f_aStruct,f_cDescription,f_aPropertys)
  Local oDs_Qry, cQry := "", hConnectionHandle
  Local iFor := 0, cFields := "", cTmp := "", cTpIndex := "", lOkRet := .F.
  Local nAdsVersion := Val(AdsVersion())
  
  Default f_nConnection := tAds_GetConnectionDefault()
  Default f_cDescription := "", f_aPropertys := {8,.T.,1,.F.}

  hConnectionHandle := tAds_GetConnectionHandle(f_nConnection)

  For iFor := 1 to Len(f_aStruct)
    cFields += TAds_StructToScript(f_aStruct[iFor])
    If iFor != Len(f_aStruct)
      cFields += ", "
    EndIf
  Next

  cQry := "CREATE TABLE _FILE_NAME_ ( _FIELDS_ ) IN DATABASE ; "+CRLF
  cQry := StrTran(cQry,"_FILE_NAME_",f_cTableName)
  cQry := StrTran(cQry,"_FIELDS_",cFields) 

  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := cQry
  if f_nConnection == 121 // Temp /DADOSTMP Error because conection Local vs Remote
    For iFor := 1 to 40

      If !File(tAds_GetPathTemp()+f_cTableName+".adt")
        AdsConnection(tAds_GetConnectionHandle(121))
        AdsDDRemoveTable(f_cTableName,.T.)
        fErase(tAds_GetPathTemp()+f_cTableName+".adt")
        fErase(tAds_GetPathTemp()+f_cTableName+".adi")
        fErase(tAds_GetPathTemp()+f_cTableName+".adm")
      EndIf
      
      If !File(tAds_GetPathTemp()+f_cTableName+".adt")
        Exit
      Else
        TADS_LOGFILE("tAdsError.log",{"iFor = "+AllTrim(Str(iFor))+" Con: "+AllTrim(Str(121)),;
                                 "TAds_CreateTableFromCode() - fErase Error table "+f_cTableName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
        hb_idleSleep(.3)
      EndIf
    Next
    AdsConnection(tAds_GetConnectionDefault())

    For iFor := 1 to 20
      IF TAds_TableExist(121,f_cTableName)
        TAds_TableErase(121,f_cTableName)
      ENDIF  

      lOkRet := oDs_Qry:DsExecute(,.F.)
      If lOkRet
        Exit
      Else
        ///? "Create error in Dictionary Temp",f_cTableName
        TADS_LOGFILE("tAdsError.log",{"iFor = "+AllTrim(Str(iFor))+" Con: "+AllTrim(Str(121)),;
                                 "TAds_CreateTableFromCode() - Create Error "+f_cTableName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
        hb_idleSleep(.3)
      EndIf
    Next
  else
    lOkRet := oDs_Qry:DsExecute()
  EndIf
  
  If !lOkRet
    TADS_LOGFILE("tAdsError.log",{"Con: "+AllTrim(Str(f_nConnection)),;
                             "TAds_CreateTableFromCode() - Create Error "+f_cTableName, ProcName(1)+" ("+AllTrim(Str(ProcLine(1)))+")"})
    Return .F.
  EndIf
  
  cQry := ""

  If !Empty(f_cDescription)
    cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
            "'Comment',"+; 
            "'"+f_cDescription+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF
  EndIf

  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Table_Auto_Create',"+; 
          "'True', 'APPEND_FAIL', '"+f_cTableName+"fail');" +CRLF

  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Table_Permission_Level',"+; 
          "'2', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF

  //cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
  //        "'Table_Memo_Block_Size',"+; 
  //        "'"+Str(Int(f_aPropertys[1]))+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF

  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Triggers_Disabled',"+; 
          "'"+If(f_aPropertys[2],"True","False")+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF

  If nAdsVersion >= 10 
    /* cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
         "'Table_Caching',"+; 
         "'"+Str(Int(f_aPropertys[3]))+"', 'APPEND_FAIL',  '"+f_cTableName+"fail');"+CRLF

    cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
            "'Table_Trans_Free',"+; 
            "'"+If(f_aPropertys[4],"True","False")+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF
   */
  EndIf
  
  For iFor := 1 to Len(f_aStruct)
    If Len(f_aStruct[iFor]) > 6
      cTmp := cValToStr(f_aStruct[iFor,7])
      //? cTmp
      If !Hb_IsNil(f_aStruct[iFor,7])
        If Upper(cTmp) == ".T."
          cTmp := "True"
        ElseIf Upper(cTmp) == ".F."
          cTmp := "False"
        EndIf  
        cQry += "EXECUTE PROCEDURE sp_ModifyFieldProperty( '"+f_cTableName+"',"+; 
                "'"+f_aStruct[iFor,1]+"', 'Field_Default_Value', "+;
                "'"+ cTmp + "', 'APPEND_FAIL', '"+f_cTableName+"fail' );"+CRLF 
      EndIf
    EndIf
  Next

  For iFor := 1 to Len(f_aStruct)
    If Len(f_aStruct[iFor]) > 5
      cTmp := f_aStruct[iFor,6]
      If !Empty(cTmp)
        cQry += "EXECUTE PROCEDURE sp_ModifyFieldProperty( '"+f_cTableName+"',"+; 
                "'"+f_aStruct[iFor,1]+"', 'Comment',"+; 
                "'"+ cTmp + "', 'APPEND_FAIL', '"+f_cTableName+"fail' );"+CRLF 
      EndIf
    EndIf
  Next

  oDs_Qry := tAds():DsNew(DF_CURSOR_OFF,f_nConnection)
  oDs_Qry:cQrySql := cQry
  lOkRet := oDs_Qry:DsExecute()
  
  If !lOkRet
    ///AdsRollback(hConnectionHandle)
    Return .F.
  EndIf
  
  cQry := ""

  For iFor := 1 to Len(f_aStruct)
    If Len(f_aStruct[iFor]) > 4
      If f_aStruct[iFor,5] > 0
        If f_aStruct[iFor,5] == 1
          cTpIndex := "2"
        elseif f_aStruct[iFor,5] == 2
          cTpIndex := "1"
        elseif f_aStruct[iFor,5] == 3
          cTpIndex := "0"
        elseif f_aStruct[iFor,5] == 4
          cTpIndex := "8"
        elseif f_aStruct[iFor,5] == 5
          cTpIndex := "4"
        EndIf  
        cQry += "EXECUTE PROCEDURE sp_CreateIndex( "+;
                "'"+f_cTableName+"',"+;
                "'"+f_cTableName+".adi',"+;
                "'"+UPPER(f_aStruct[iFor,1])+"',"+;
                "'"+f_aStruct[iFor,1]+"',"+;
                "'',"+;
                cTpIndex+","+;
                "512); "+CRLF
      EndIf
    EndIf
  Next

  If !Empty(cQry)
    oDs_Qry := tAds():DsNew(DF_CURSOR_OFF,f_nConnection)
    oDs_Qry:cQrySql := cQry
    lOkRet := oDs_Qry:DsExecute()
  Else
    lOkRet := .T.
  EndIf  
  ///AdsBeginTransaction(hConnectionHandle)

  If !lOkRet
    //AdsRollback(hConnectionHandle)
    Return .F.
  Else
    //AdsCommitTransaction(hConnectionHandle)
    Return .T.
  EndIf

RETURN .T.
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Criar nova tabela no Dicionario de dados de Properties() das Classes Geradas por TAds_CreateClassFromDatabase() 
// f_nConnection > Number Connection in tAdsConnection
// f_oClassTable > Objeto da classe de chamada  / Ex: oDb_class := DB_TABLE():Properties()
// f_cTableName > Nome da tabela a Gerar - Default aTableInfo[01]
FUNCTION TAds_CreateTableFromClass(f_nConnection,f_oClassTable,f_cTableName)
  Local oDs_Qry, hConnectionHandle, cIndexFile := ""
  Local aTableInfo, aColumnsInfo, aIndexInfo, aColumnTmp := {}
  Local cQry := "", iFor := 0, cTmp := "", nBlockMemoSize := 8, lOtherName := .F., lOkRet := .F.
  Local cFieldsType := "", nFieldLengh := 0, nFieldDecimals := 0, aTmpFieldStruct := {}

  hConnectionHandle := tAds_GetConnectionHandle(f_nConnection)

  aTableInfo    := f_oClassTable:aTableInfo
  aColumnsInfo  := f_oClassTable:aColumnsInfo
  aIndexInfo    := f_oClassTable:aIndexInfo
  
  If Hb_IsNil(f_cTableName)
    f_cTableName  := aTableInfo[01] 
    cIndexFile    := aTableInfo[01]+".adi"
  EndIf

  If aTableInfo[01] != f_cTableName
    lOtherName    := .T.
  EndIf

  cQry := "CREATE TABLE _FILE_NAME_ ( _FIELDS_ ) IN DATABASE; "+CRLF
  cQry := StrTran(cQry,"_FILE_NAME_",f_cTableName)

  For iFor := 1 to len(aColumnsInfo)
    aColumnTmp := aColumnsInfo[iFor]
    aTmpFieldStruct := {aColumnTmp[02],;    
                          aColumnTmp[04],;    
                          aColumnTmp[05],;    
                          aColumnTmp[06]}    

    cFieldsType += TAds_StructToScript(aTmpFieldStruct)
    If iFor != Len(aColumnsInfo)
      cFieldsType += ", "
    EndIf
  Next

  cQry := StrTran(cQry,"_FIELDS_",cFieldsType) 

  If aTableInfo[7]  
    ///cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
    ///        "'Table_Encryption',"+; 
    ///        If(aTableInfo[07],"'True'","'False'")+", 'APPEND_FAIL', '"+f_cTableName+"fail');" +CRLF
    cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
            "'Table_Encryption',"+; 
            "'True'"+", 'APPEND_FAIL', '"+f_cTableName+"fail');" +CRLF

  EndIf
  
  If !Hb_IsNil(aTableInfo[12])
    cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
            "'Comment',"+; 
            "'"+aTableInfo[12]+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF
  EndIf

  If !Hb_IsNil(aTableInfo[06])
    cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
         "'Table_Default_Index',"+; 
         "'"+aTableInfo[06]+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF
  EndIf
  
  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Table_Auto_Create',"+; 
          If(aTableInfo[04],"'True'","'False'")+", 'APPEND_FAIL', '"+f_cTableName+"fail');" +CRLF

  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Table_Permission_Level',"+; 
          "'"+Alltrim(Str(aTableInfo[08]))+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF

  nBlockMemoSize := aTableInfo[09]
  If nBlockMemoSize < 8
    nBlockMemoSize := 8
  EndIf
  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Table_Memo_Block_Size',"+; 
          "'"+AllTrim(Str(Int(nBlockMemoSize)))+"', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF

  cQry += "EXECUTE PROCEDURE sp_ModifyTableProperty( '"+f_cTableName+"',"+; 
          "'Triggers_Disabled',"+; 
          "'False', 'APPEND_FAIL', '"+f_cTableName+"fail');"+CRLF

  For iFor := 1 to Len(aColumnsInfo)
    If !Hb_IsNil(aColumnsInfo[iFor,10])
      cTmp := cValToStr(aColumnsInfo[iFor,10])
      If Upper(cTmp) != "NIL"
        If Upper(cTmp) == ".T."
          cTmp := "True"
        ElseIf Upper(cTmp) == ".F."
          cTmp := "False"
        EndIf  
        cQry += "EXECUTE PROCEDURE sp_ModifyFieldProperty ( '"+f_cTableName+"',"+; 
                "'"+aColumnsInfo[iFor,02]+"', 'Field_Default_Value', "+;
                "'"+ cTmp + "', 'APPEND_FAIL', '"+f_cTableName+"fail' );"+CRLF 
      EndIf
    EndIf
  Next

  For iFor := 1 to Len(aColumnsInfo)
    cTmp := aColumnsInfo[iFor,12]
    If !Empty(cTmp)
      cQry += "EXECUTE PROCEDURE sp_ModifyFieldProperty ( '"+f_cTableName+"',"+; 
              "'"+aColumnsInfo[iFor,02]+"', 'Comment',"+; 
              "'"+ cTmp + "', 'APPEND_FAIL', '"+f_cTableName+"fail' );"+CRLF 
    EndIf
  Next

  For iFor := 1 to Len(aIndexInfo)
    If !lOtherName
      f_cTableName  := aIndexInfo[iFor,02]
      cIndexFile    := aIndexInfo[iFor,03]
    EndIf
    cQry += "EXECUTE PROCEDURE sp_CreateIndex( "+;
            "'"+f_cTableName+"',"+;
            "'"+cIndexFile+"',"+;
            "'"+aIndexInfo[iFor,01]+"',"+;
            "'"+aIndexInfo[iFor,04]+"',"+;
            "'"+aIndexInfo[iFor,05]+"',"+;
            AllTrim(Str(aIndexInfo[iFor,06]))+","+;
            "512); "+CRLF
  Next

  AdsBeginTransaction(hConnectionHandle)

  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := cQry
  lOkRet := oDs_Qry:DsExecute()
  
  If !lOkRet
    AdsRollback(hConnectionHandle)
  Else
    AdsCommitTransaction(hConnectionHandle)
  EndIf
  
RETURN lOkRet
///////////////////////////////////////////////////////////////////////////////
// Criar nova tabela no Dicionario de dados usando estrutura padrão 
// f_nConnection > Number Connection in tAdsConnection
// f_cTableName > Nome da nova tabela
// f_aStruct > Estrutura do arquivo estilo dbstruct com outras opções
//      f_aStruct[1] > Nome/descrição do Campo
//      f_aStruct[2] > Tipo do Campo - Ver TAds_StructInfo() 
//      f_aStruct[3] > Lengh/Comprimento do Campo
FUNCTION TAds_CreateIndex(f_nConnection,f_cTableName,f_cTagIndex,f_cTagExpr)
  Local oDs_Qry, cQry := "", hConnectionHandle
  Local iFor := 0, cFields := "", cTmp := "", cTpIndex := "2", lOkRet := .F.
  Local nAdsVersion := Val(AdsVersion())
  
  Default f_nConnection := tAds_GetConnectionDefault()

  hConnectionHandle := tAds_GetConnectionHandle(f_nConnection)

  cQry := ""

  cQry += "EXECUTE PROCEDURE sp_CreateIndex( "+;
          "'"+f_cTableName+"',"+;
          "'"+f_cTableName+".adi',"+;
          "'"+UPPER(f_cTagIndex)+"',"+;
          "'"+f_cTagExpr+"',"+;
          "'',"+;
          cTpIndex+","+;
          "512); "+CRLF

  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := cQry
  lOkRet := oDs_Qry:DsExecute()

  If !lOkRet
    Return .F.
  Else
    Return .T.
  EndIf

RETURN .T.
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Copiar um registro de uma tabela para outra tabela 
// f_oFrom_ObjTable > Source Table object tAds
// f_oTo_ObjTable   > Destiny Table object tAds
// lAppend          > Insert new register if true (True default) 
FUNCTION tAds_CopyFieldContents(f_oFrom_ObjTable,f_oTo_ObjTable,lAppend)
	Local icFor := 0, aStructFrom, aStructTo  
	Local uTmp  := Nil, nTmpFieldPos := 0
	
  Default lAppend := .T.
  
  aStructFrom   := f_oFrom_ObjTable:aStructTads
  aStructTo     := f_oTo_ObjTable:aStructTads
  
  IF lAppend
    f_oTo_ObjTable:Append()
  ELSE
    f_oTo_ObjTable:rLock()
  ENDIF

  FOR icFor := 1 to len(aStructFrom)
    nTmpFieldPos := Ascan(aStructTo,{|cb_aField|cb_aField[1] == aStructFrom[icFor,1]})  
    ///? nTmpFieldPos, aStructTo[nTmpFieldPos,1], aStructFrom[icFor,1]
    IF nTmpFieldPos > 0
      If aStructTo[nTmpFieldPos,2] == "AutoIncrement" .or. aStructTo[nTmpFieldPos,2] == "+"
        Loop
      EndIf 
       f_oTo_ObjTable:VarPut(aStructTo[nTmpFieldPos,1],f_oFrom_ObjTable:VarGet(aStructFrom[icFor,1]))
    ENDIF
  NEXT

RETURN NIL