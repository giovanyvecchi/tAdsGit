////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções para gerar classes das tabelas existentes no Dicionario de dados                                                              //
// Qualquer alteração ou modificação encaminhe para Giovany Vecchi                                //
// Giovany Vecchi 01/03/2014 giovanyvecchi@yahoo.com.br / giovanyvecchi@gmail.com                 //
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

/* Options for creating indexes - can be ORed together */
#define ADS_ASCENDING                 0x00000000
#define ADS_UNIQUE                    0x00000001
#define ADS_COMPOUND                  0x00000002
#define ADS_CUSTOM                    0x00000004
#define ADS_DESCENDING                0x00000008
#define ADS_USER_DEFINED              0x00000010
/* Options specifically for FTS indexes 0020 - 0200 */
#define ADS_FTS_INDEX                 0x00000020      // This is implied for AdsCreateFTSIndex
#define ADS_FTS_FIXED                 0x00000040      // Do not maintain the index with record updates
#define ADS_FTS_CASE_SENSITIVE        0x00000080      // Make the index case sensitive
#define ADS_FTS_KEEP_SCORE            0x00000100      // Track word counts in the index for faster SCORE()
#define ADS_FTS_PROTECT_NUMBERS       0x00000200      // Don't break numbers on commas and periods

#define ADS_NOT_AUTO_OPEN             0x00000400     // Don't make this an auto open index in data dictionary
#define ADS_CANDIDATE                 0x00000800     // true unique CDX index (equivalent to ADS_UNIQUE for ADIs)
#define ADS_BINARY_INDEX              0x00001000     // logical index with a bitmap for data


FUNCTION TAds_CreateClassFromDatabase(f_nConnection,f_cTableName,f_cDirectory)
	Local cTxtClass := "", cClassName := "", cPrgName := "", iFor := 0, iForChar := 0
	Local oConnectionObj, nConnectionHandle := 0, aTableInfo := {}, aColumsDb := {}, aIndicesDb := {}
  Local nSpace := 0, cTmp := "", cTmpChar := "", nTmpPosReg := 0
  Local oClipTmp
  
	f_cTableName := AllTrim(f_cTableName)
	
	oConnectionObj := tAds_GetConnectionObj(f_nConnection)
  AdsConnection(oConnectionObj:hConnectHandle)
	
	IF !ADSCHECKEXISTENCE(f_cTableName)
	   MsgStop("Table "+f_cTableName+" not Found.","TAds_CreateClassFromDatabase")
	   RETURN ""
	ENDIF

	If Hb_IsNil(f_cDirectory)
		f_cDirectory := ALLTRIM(HB_CURDRIVE()+":\"+CURDIR()+"\")
	EndIf 

  aTableInfo  := TAds_SystemTableInfo(f_nConnection, f_cTableName)
  aColumsDb   := TAds_SystemColumnsTable(f_nConnection, f_cTableName)

	cClassName := "DB_"+f_cTableName
	cPrgName := f_cDirectory+"DataBase_"+f_cTableName+".prg"
	
	cTxtClass := "//Classe gerada por TAds "+dToc(Date())+" - "+Time()+CRLF
	cTxtClass += "#Include 'Fivewin.ch'"+CRLF
	cTxtClass += "#Include 'TAds.ch'"+CRLF+CRLF
	cTxtClass += "CLASS "+cClassName+" from TAds"+CRLF+CRLF

	cTxtClass += "  Data cTableName     Init '"+f_cTableName+"'"+CRLF
	cTxtClass += "  Data nCache"+CRLF
	cTxtClass += "  Data lConnected     Init .F."+CRLF+CRLF

	cTxtClass += "  METHOD OpenRdd(f_nConexao,;    // Numero da Conexão - Default tAds_GetConnectionDefault()"+CRLF
	cTxtClass += "              f_nCache);         // Numero de registros em Cache - Default nCacheAds()"+CRLF
	cTxtClass += "              Constructor "+CRLF+CRLF
	
	cTxtClass += "  METHOD End()"+CRLF+CRLF
	
	cTxtClass +="ENDCLASS" +CRLF
	cTxtClass += "//-----------------------------------------------------------------------------"+CRLF
	cTxtClass += "METHOD OpenRdd(f_nConexao,f_nCache) Class "+cClassName+CRLF+CRLF

	cTxtClass += "  Default f_nCache := nCacheAds()"+CRLF+CRLF
  
	cTxtClass += "  ::nCache        := f_nCache"+CRLF
  cTxtClass += "  ::nTpCallRdd    := 2"+CRLF 
	cTxtClass += "  ::NewRdd(::cTableName,f_nConexao,::nCache)"+CRLF
	cTxtClass += "  If ::nOpenStatus == 0 "+CRLF
	cTxtClass += "    ::lConnected := .T."+CRLF
	cTxtClass += "  EndIf"+CRLF+CRLF

	cTxtClass += "RETURN Self"+CRLF 
	cTxtClass += "//-----------------------------------------------------------------------------"+CRLF
	cTxtClass += "METHOD End() Class "+cClassName +CRLF+CRLF

	cTxtClass += "   If ::lConnected"+CRLF
	cTxtClass += "     ::Super:End()"+CRLF
	cTxtClass += "   EndIf"+CRLF+CRLF

	cTxtClass += "   Self := Nil"+CRLF+CRLF

	cTxtClass += "RETURN Nil"+CRLF
	cTxtClass += "//-----------------------------------------------------------------------------"+CRLF

	cTxtClass := STRTRAN(cTxtClass,"'",CHR(34))

	MemoWrit(cPrgName,cTxtClass)
	
	///WinExec("Notepad.exe "+cPrgName)
	
Return cPrgName
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CreateAllClassFromDataDictionary(f_nConnection,f_cDirectory)
  Local aTablesInDictionary := {}, iFor := 0
  
  aTablesInDictionary := TAds_SystemTablesNames(f_nConnection)
  
  ///xbrowse(aTablesInDictionary)
  
  For iFor := 1 To Len(aTablesInDictionary)
    TAds_CreateClassFromDatabase(f_nConnection,aTablesInDictionary[iFor],f_cDirectory)
  Next

RETURN NIL
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CreatePropertiesFromDatabase(f_nConnection,f_cTableName,f_cDirectory)
	Local cTxtClass := "", cClassName := "", cPrgName := "", iFor := 0, iForChar := 0
	Local oConnectionObj, nConnectionHandle := 0, aTableInfo := {}, aColumsDb := {}, aIndicesDb := {}
  Local nSpace := 0, cTmp := "", cTmpChar := "", nTmpPosReg := 0
  Local oClipTmp
  
	f_cTableName := AllTrim(f_cTableName)
	
	oConnectionObj := tAds_GetConnectionObj(f_nConnection)
  AdsConnection(oConnectionObj:hConnectHandle)
	
	IF !ADSCHECKEXISTENCE(f_cTableName)
	   MsgStop("Table "+f_cTableName+" not Found.","TAds_CreateClassFromDatabase")
	   RETURN ""
	ENDIF

	If Hb_IsNil(f_cDirectory)
		f_cDirectory := ALLTRIM(HB_CURDRIVE()+":\"+CURDIR()+"\")
	EndIf 

  aTableInfo  := TAds_SystemTableInfo(f_nConnection, f_cTableName)
  aColumsDb   := TAds_SystemColumnsTable(f_nConnection, f_cTableName)

	cClassName := "SCH_"+f_cTableName
	cPrgName := f_cDirectory+"Schema_"+f_cTableName+".prg"
	
	cTxtClass := "//Classe gerada por TAds "+dToc(Date())+" - "+Time()+CRLF
	cTxtClass += "#Include 'Fivewin.ch'"+CRLF
	cTxtClass += "#Include 'TAds.ch'"+CRLF+CRLF
	cTxtClass += "CLASS "+cClassName+" from TAds"+CRLF+CRLF

	cTxtClass += "  Data cTableName     Init '"+f_cTableName+"'"+CRLF
	cTxtClass += "  Data aTableInfo     Init {}"+CRLF
	cTxtClass += "  Data aColumnsInfo   Init {}"+CRLF
	cTxtClass += "  Data aIndexInfo     Init {}"+CRLF+CRLF

	cTxtClass += "  METHOD New() Constructor " +CRLF+CRLF
	cTxtClass += "  METHOD End()"+CRLF+CRLF
	
	cTxtClass +="ENDCLASS" +CRLF
	cTxtClass += "//-----------------------------------------------------------------------------"+CRLF
	cTxtClass += "METHOD End() Class "+cClassName +CRLF+CRLF

	cTxtClass += "   Self := Nil"+CRLF+CRLF

	cTxtClass += "RETURN Nil"+CRLF
	cTxtClass += "//-----------------------------------------------------------------------------"+CRLF
	cTxtClass += "METHOD New() Class "+cClassName+CRLF+CRLF

	cTxtClass += "  Local nPosReg := 0, nPosIndex := 0"+CRLF+CRLF

  cTxtClass += "  ///////////////////////// Propriedades da Tabela"+CRLF 
  cTxtClass += "  ::aTableInfo := Array(12)"+CRLF

  cTxtClass += "  ::aTableInfo[01] := '"+aTableInfo[01]+"'  // 01-Nome da Tabela"+CRLF
  cTxtClass += "  ::aTableInfo[02] := '"+aTableInfo[02]+"'  // 02-Path relacionado ao arquivo Fisico"+CRLF 
  cTxtClass += "  ::aTableInfo[03] := "+AllTrim(Str(aTableInfo[03]))+"  // 03-Tipo da tabela / 1-DBFNTX /2-DBFCDX /3-ADT"+CRLF 
  cTxtClass += "  ::aTableInfo[04] := "+If(aTableInfo[04],".T.",".F.")+"  // 04-Criar automaticamente ao abrir "+CRLF
  cTxtClass += "  ::aTableInfo[05] := "+If(Empty(aTableInfo[05]),"NIL","'"+aTableInfo[05]+"'")+"  // 05-Tag primario  "+CRLF
  cTxtClass += "  ::aTableInfo[06] := "+If(Empty(aTableInfo[06]),"NIL","'"+aTableInfo[06]+"'")+"  // 06-Chave de indice por falta"+CRLF  
  cTxtClass += "  ::aTableInfo[07] := "+If(aTableInfo[07],".T.",".F.")+"  // 07-Se a Tabela esta Encriptada  "+CRLF
  cTxtClass += "  ::aTableInfo[08] := "+AllTrim(Str(aTableInfo[08]))+"  // 08- 1-Allow Hidden Field Filters /2-Prevent Hidden Field Filters /3-SQL Access Only"+CRLF
  cTxtClass += "  ::aTableInfo[09] := "+AllTrim(Str(aTableInfo[09]))+"  // 09-Tamanho do bloco em Bytes para campos Memo ou Binarios "+CRLF
  cTxtClass += "  ::aTableInfo[10] := "+If(Empty(aTableInfo[10]),"NIL","'"+aTableInfo[10]+"'")+"  // 10-Define uma nova expressão de validação nível recorde para a tabela"+CRLF
  cTxtClass += "  ::aTableInfo[11] := "+If(Empty(aTableInfo[11]),"NIL","'"+aTableInfo[11]+"'")+"  // 11-mensagem de erro que será devolvido quando um registro da tabela está sendo modificado e ultrapassou o limite de Validação"+CRLF   
  cTxtClass += "  ::aTableInfo[12] := "+If(Empty(aTableInfo[12]),"NIL","'"+aTableInfo[12]+"'")+"  // 12-Comentario do Campo "+CRLF+CRLF


  cTxtClass += "  ///////////////////////// Informações dos Campos"+CRLF 
  cTxtClass += "  ::aColumnsInfo := Array("+StrZero(Len(aColumsDb),2)+")"

  For iFor := 1 to Len(aColumsDb)

    cTxtClass += CRLF
    cTxtClass += "  nPosReg += 1"+CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg] := Array(14)"+CRLF+CRLF 
    cTxtClass += "  ///////////////////////// Field: "+aColumsDb[iFor,2] + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,01] := nPosReg  // 01-Posição do Registro" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,02] := '"+aColumsDb[iFor,2]+"'   // 02-Descrição do Registro" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,03] := "+AllTrim(Str(aColumsDb[iFor,3]))+"   // 03-Tipo do Registro Tabela Adt" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,04] := '"+aColumsDb[iFor,4]+"'   //  04-Descrição do tipo do Registro" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,05] := "+AllTrim(Str(aColumsDb[iFor,5]))+"   // 05-Tamanho do Registro" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,06] := "+Alltrim(Str(aColumsDb[iFor,6]))+"   // 06-Casas Decimais" + CRLF
    If !Empty(aColumsDb[iFor,7])
      cTmp := Alltrim(aColumsDb[iFor,7])
    Else
      cTmp := "NIL"
    EndIf
    cTxtClass += "  ::aColumnsInfo[nPosReg,07] := "+cTmp+"   // 07-Valor Minimo estipulado" + CRLF
    If !Empty(aColumsDb[iFor,8])
      cTmp := AllTrim(aColumsDb[iFor,8])
    Else
      cTmp := "NIL"
    EndIf
    cTxtClass += "  ::aColumnsInfo[nPosReg,08] := "+cTmp+"   // 08-Valor Maximo estipulado" + CRLF
    If aColumsDb[iFor,9]
      cTmp := ".T."
    Else
      cTmp := ".F."
    EndIf
    cTxtClass += "  ::aColumnsInfo[nPosReg,09] := "+cTmp+"   //  09-Se é valido quando Null" + CRLF
    If !Empty(aColumsDb[iFor,10])
      cTmp := AllTrim(aColumsDb[iFor,10])
      If aColumsDb[iFor,03] == 1 // Logical
        If cTmp == "TRUE"
          cTmp := ".T."
        Else 
          cTmp := ".F."
        EndIf
      ElseIf aColumsDb[iFor,03] == 3 // Date 
        cTmp := "cTod('"+cTmp+"')"
      ElseIf aColumsDb[iFor,03] == 4 // String
        cTmp := "'"+cTmp+"'"   
      ElseIf aColumsDb[iFor,03] == 20 // CIString
        cTmp := "'"+cTmp+"'"   
      EndIf
    Else
      cTmp := "NIL"
    EndIf
    cTxtClass += "  ::aColumnsInfo[nPosReg,10] := "+cTmp+"   //  10-Valor por falta" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,11] := '"+Alltrim(aColumsDb[iFor,11])+"'   //  11-Mensagem quando ocorrer erros" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,12] := '"+Alltrim(aColumsDb[iFor,12])+"'   //  12-Comentario do campo" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,13] := '"+aColumsDb[iFor,13]+"'   //  13-" + CRLF
    cTxtClass += "  ::aColumnsInfo[nPosReg,14] := "+AllTrim(Str(aColumsDb[iFor,14]))+"   // 14-" + CRLF +CRLF

  Next 
  
	aIndicesDb := TAds_SystemIndexes(f_nConnection, f_cTableName)

  cTxtClass += "  ///////////////////////////////////////////////////////"+CRLF 
  cTxtClass += "  ///////////////////////// Informações dos Indices"+CRLF 
  cTxtClass += "  ::aIndexInfo := Array("+StrZero(Len(aIndicesDb),2)+")"
	
  For iFor := 1 to Len(aIndicesDb)
	  cTxtClass += CRLF
    cTxtClass += "  nPosIndex += 1"+CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex] := Array(14)"+CRLF+CRLF 
    cTxtClass += "  ///////////////////////// Index "+aIndicesDb[iFor,1] + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,01] := '"+AllTrim(aIndicesDb[iFor,01])+"'  // 01-Nome da Chave do Indice" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,02] := '"+AllTrim(aIndicesDb[iFor,02])+"'  // 02-Tabela correspondente ao indice" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,03] := '"+AllTrim(aIndicesDb[iFor,03])+"'  // 03-Nome do arquivo de Indice correspondente" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,04] := '"+AllTrim(aIndicesDb[iFor,04])+"'  // 04-Campo ou expressao da chave correspondente" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,05] := '"+AllTrim(aIndicesDb[iFor,05])+"'  // 05-Condição da chave" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,06] := "+AllTrim(Str(aIndicesDb[iFor,06]))+"  // 06-Opção da chave / Default 2-ADS_COMPOUND" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,07] := "+AllTrim(Str(aIndicesDb[iFor,07]))+"  // 07-Comprimento da chave" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,08] := "+AllTrim(Str(aIndicesDb[iFor,08]))+"  // 08-Comprimento minimo para busca em indice FTS" + CRLF
    cTmpChar := ""
    cTmp := AllTrim(aIndicesDb[iFor,09])
    If !Empty(cTmp)
      For iForChar := 1 To Len(cTmp)
        If iForChar > 1
          cTmpChar += "+"
        EndIf
        cTmpChar += "Chr("+AllTrim(Str(ASC(SubStr(cTmp,iForChar,1))))+")"
      Next
    Else
      cTmpChar := "NIL"
    EndIf
    cTxtClass += "  ::aIndexInfo[nPosIndex,09] := "+cTmpChar+"  // 09-Caracteres delimitadores em indices FTS" + CRLF

    cTxtClass += "  ::aIndexInfo[nPosIndex,10] := NIL  // 10-Caracteres ruidos em indices FTS" + CRLF

    cTmpChar := ""
    cTmp := AllTrim(aIndicesDb[iFor,11])
    If !Empty(cTmp)
      For iForChar := 1 To Len(cTmp)
        If iForChar > 1
          cTmpChar += "+"
        EndIf
        cTmpChar += "Chr("+AllTrim(Str(ASC(SubStr(cTmp,iForChar,1))))+")"
      Next
    Else
      cTmpChar := "NIL"
    EndIf
    cTxtClass += "  ::aIndexInfo[nPosIndex,11] := "+cTmpChar+"  // 11-Caracteres de separação" + CRLF

    cTmpChar := ""
    cTmp := AllTrim(aIndicesDb[iFor,12])
    If !Empty(cTmp)
      For iForChar := 1 To Len(cTmp)
        If iForChar > 1
          cTmpChar += "+"
        EndIf
        cTmpChar += "Chr("+AllTrim(Str(ASC(SubStr(cTmp,iForChar,1))))+")"
      Next
    Else
      cTmpChar := "NIL"
    EndIf
    cTxtClass += "  ::aIndexInfo[nPosIndex,12] := "+cTmpChar+"  // 12-Caracteres de cortes descartaveis" + CRLF

    cTxtClass += "  ::aIndexInfo[nPosIndex,13] := '"+AllTrim(aIndicesDb[iFor,13])+"'  // 13-Comentario do indice" + CRLF
    cTxtClass += "  ::aIndexInfo[nPosIndex,14] := '"+AllTrim(aIndicesDb[iFor,14])+"'  // 14-Colação dos caracteres" + CRLF
	
	Next
	
	cTxtClass += CRLF

	cTxtClass += "RETURN Self"+CRLF 
	cTxtClass += "//-----------------------------------------------------------------------------"+CRLF

	cTxtClass := STRTRAN(cTxtClass,"'",CHR(34))

	MemoWrit(cPrgName,cTxtClass)
	
	///WinExec("Notepad.exe "+cPrgName)
	
Return cPrgName
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CreateAllPropertiesFromDataDictionary(f_nConnection,f_cDirectory)
  Local aTablesInDictionary := {}, iFor := 0
  
  aTablesInDictionary := TAds_SystemTablesNames(f_nConnection)
  
  ///xbrowse(aTablesInDictionary)
  
  For iFor := 1 To Len(aTablesInDictionary)
    TAds_CreatePropertiesFromDatabase(f_nConnection,aTablesInDictionary[iFor],f_cDirectory)
  Next

RETURN NIL

