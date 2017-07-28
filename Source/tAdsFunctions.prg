////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções complementares do projeto                                                              //
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

///////////////////////////////////////////////////////////////////////////////
// TAds_QueryFormat - Formata query conforme os tipos de variaveis para substituição
// f_cStrQuery    > Texto com solicitação de comando Query
// f_aVarsFormat  > Vetor com as variaveis textos e valores para substituir em f_cStrQuery
// ----------------------------------------------------------------------------
// Exemplo:
// cQuery := "Where SALARIO <= nSalario and DT_CONTRAT >= DtContrato and L_SUSPENSO == lSuspendido"
// aadd(aVarsFormat,{"nSalario",1200.00})
// aadd(aVarsFormat,{"DtContrato",cToD("01/01/2007")})
// aadd(aVarsFormat,{"lSuspendido",.F.})
// cResult := TAds_QueryFormat(cQuery,aVarsFormat) 
// ? cResult
// "Where SALARIO <= 1200.00 and DT_CONTRAT >= '2007-01-01' and L_SUSPENSO == FALSE"
FUNCTION TAds_QueryFormat(f_cStrQuery,f_aVarsFormat) 
	Local _cRet := f_cStrQuery, _iFor := 0, _cTmpVar := "", _cTmpFormat := "", _cTmpData := ""
	
	IF VALTYPE(f_aVarsFormat) != "A"
	   Return _cRet
	ENDIF

	IF LEN(f_aVarsFormat) == 0
	   Return _cRet
	ENDIF
	
	FOR _iFor := 1 TO LEN(f_aVarsFormat)
		_cTmpVar := f_aVarsFormat[_iFor,1]
		IF VALTYPE(f_aVarsFormat[_iFor,2]) == "C"
			_cTmpFormat := StrTran(f_aVarsFormat[_iFor,2],"'","´") //Error if the text has character (') Example: D'Ambrosio
			_cTmpFormat := "'"+_cTmpFormat+"'" 
		ELSEIF VALTYPE(f_aVarsFormat[_iFor,2]) == "D"
			_cTmpData := DTOS(f_aVarsFormat[_iFor,2])
			_cTmpData := (SubStr(_cTmpData,1,4)+"-"+SubStr(_cTmpData,5,2)+"-"+SubStr(_cTmpData,7,2))
			_cTmpFormat := "'"+_cTmpData+"'"
		ELSEIF VALTYPE(f_aVarsFormat[_iFor,2]) == "N"
			_cTmpFormat := ALLTRIM(STR(f_aVarsFormat[_iFor,2]))
		ELSEIF VALTYPE(f_aVarsFormat[_iFor,2]) == "L"
			_cTmpFormat := IF(f_aVarsFormat[_iFor,2],"TRUE","FALSE")
		ENDIF
		_cRet := STRTRAN(_cRet,_cTmpVar,_cTmpFormat)
	NEXT

RETURN _cRet
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_Str(nValor,nSpcFinal,lDelDecimal,f_nDecimal,f_lBritanico)
LOCAL cRet:=""
LOCAL cDecimals:=""

DEFAULT f_nDecimal := 0, lDelDecimal := .T., f_lBritanico := .T.

IF f_nDecimal == 0
   cRet := ALLTRIM(STR(nValor))
ELSE
   cRet:=ALLTRIM(STR(ROUND(nValor,f_nDecimal)))
ENDIF

IF nSpcFinal==Nil
   nSpcFinal := 0
ENDIF

IF lDelDecimal==Nil
   lDelDecimal:=.T.
ENDIF

IF AT(".",cRet) == 0
   lDelDecimal := .F.
ENDIF

IF lDelDecimal
   cDecimals:=SUBSTR(cRet,RAT(".",cRet))
   ///cDecimals:=Strtran(cDecimals,"0","")
   DO WHILE .T.
      IF SUBSTR(cDecimals,LEN(cDecimals),1) == "0"
         cDecimals := SUBSTR(cDecimals,1,LEN(cDecimals)-1)
      ELSE
         EXIT
      ENDIF
   ENDDO
   IF cDecimals=="."
      cDecimals:=""
   ENDIF
   cRet:=SUBSTR(cRet,1,RAT(".",cRet)-1)+cDecimals
ENDIF

IF nSpcFinal > 0
   cRet:=cRet+Space(nSpcFinal)
ENDIF

If f_lBritanico
	cRet := StrTran(cRet,".",",")
EndIf

RETURN cRet
///////////////////////////////////////////////////////////////////////////
FUNCTION TAds_DiaExtenso(Data,Num)
LOCAL RetData:=""

IF Num == Nil ; Num:=.f. ; ENDIF

IF DOW(data) == 1
   RetData:=IF(Num,"Domingo","Domingo")
ELSEIF DOW(data) == 2
   RetData:=IF(Num,"2ª Feira","Segunda Feira")
ELSEIF DOW(data) == 3
   RetData:=IF(Num,"3ª Feira","Terça Feira")
ELSEIF DOW(data) == 4
   RetData:=IF(Num,"4ª Feira","Quarta Feira")
ELSEIF DOW(data) == 5
   RetData:=IF(Num,"5ª Feira","Quinta Feira")
ELSEIF DOW(data) == 6
   RetData:=IF(Num,"6ª Feira","Sexta Feira")
ELSEIF DOW(data) == 7
   RetData:=IF(Num,"Sabado","Sabado")
ENDIF

RETURN RetData
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_MesExtenso(Data)
LOCAL RetMes := "",nMes := MONTH(Data)

IF nMes == 1
   RetMes:= "Janeiro"
ELSEIF nMes == 2
   RetMes:= "Fevereiro"
ELSEIF nMes == 3
   RetMes:= "Março"
ELSEIF nMes == 4
   RetMes:= "Abril"
ELSEIF nMes == 5
   RetMes:= "Maio"
ELSEIF nMes == 6
   RetMes:= "Junho"
ELSEIF nMes == 7
   RetMes:= "Julho"
ELSEIF nMes == 8
   RetMes:= "Agosto"
ELSEIF nMes == 9
   RetMes:= "Setembro"
ELSEIF nMes == 10
   RetMes:= "Outubro"
ELSEIF nMes == 11
   RetMes:= "Novembro"
ELSEIF nMes == 12
   RetMes:= "Dezembro"
ENDIF

RETURN RetMes
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_DataExtenso(dDate,f_lDia)
LOCAL cDate := ""

DEFAULT f_lDia := .T.

IF f_lDia
   cDate += TAds_DiaExtenso(dDate)+" "
ENDIF

cDate += TAds_Str(DAY(dDate))+" de "+;
         TAds_MesExtenso(dDate)+" de "+;
         TAds_Str(YEAR(dDate))

///vartype(ddate)

RETURN cDate
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_ValExtenso(f_nValor,lMoeda)
	local wext,bilhoes,milhoes,mil,real,centavo
	LOCAL vValExt:=0
	Local cExtenso := ""
if f_nValor < 0
    return "Erro! , Menor que Zero"
endif

IF lMoeda==Nil
   lMoeda:=.T.
ENDIF

///cExtenso := f_nValor
wext := strzero(f_nValor,15,2)
bilhoes  := subs(wext,01,3)
milhoes  := subs(wext,04,3)
mil      := subs(wext,07,3)
real     := subs(wext,10,3)
centavo  := subs(wext,14,2)

cExtenso := ''
if bilhoes # '000'
   cExtenso := TAds_SubExtenso(bilhoes)
   cExtenso := cExtenso +iif(bilhoes :='001',' Bilhão',' Bilhões')
endif
   if bilhoes # '000' .and. (VAL(milhoes)+VAL(mil)+VAL(real)) # 0
     cExtenso := cExtenso + ', '
     elseif bilhoes # '000' .and. (VAL(milhoes)+VAL(mil)+VAL(real)) == 0
     cExtenso := cExtenso + ' de'
   endif
   if bilhoes == '000'
      cExtenso = ''
   endif

if milhoes # '000'
   cExtenso := TAds_SubExtenso(milhoes,cExtenso)
   cExtenso := cExtenso + iif(milhoes == '001',' Milhão',' Milhões')
endif
   if milhoes # '000' .and. (VAL(mil)+VAL(real)) # 0
     cExtenso := cExtenso + ', '
     elseif milhoes # '000' .and. (VAL(mil)+VAL(real)) == 0
     cExtenso := cExtenso + ' de'
   endif
  if bilhoes == '000' .and. milhoes == '000'
      cExtenso := ''
   endif

if mil # '000'
   cExtenso := TAds_SubExtenso(mil,cExtenso)
   cExtenso := cExtenso + ' Mil'
endif
   if mil # '000' .AND. (VAL(BILHOES)+VAL(MILHOES))# 0
      cExtenso := cExtenso + IIF(REAL # '000' ,', ','')
      ELSEIF mil # '000' .AND. (VAL(BILHOES)+VAL(MILHOES))== 0 .AND. REAL # '000'
      cExtenso := cExtenso + IIF(SUBSTR(REAL,2,2)=='00',' e ',', ')
   ENDIF
  if bilhoes == '000' .and. milhoes == '000' .and. mil == '000'
      cExtenso := ''
   endif

if real # '000'
   cExtenso := TAds_SubExtenso(real,cExtenso)
   cExtenso := cExtenso
endif
if .not. empty(cExtenso)
   cExtenso := cExtenso + iif(val(wext) > 1.99,' Reais',' Real')
endif

  if (VAL(bilhoes)+VAL(milhoes)+VAL(mil)+VAL(real)) == 0
      cExtenso := ''
    elseif (VAL(bilhoes)+VAL(milhoes)+VAL(mil)+VAL(real)) # 0
      cExtenso := cExtenso + iif(centavo # '00',' e ','')
  endif

if centavo # '00'
   cExtenso := TAds_SubExtenso('0'+centavo,cExtenso)
   cExtenso := cExtenso + iif(centavo = '01',' centavo',' centavos')
endif
vvalext := cExtenso
IF !lMoeda
   vValExt:=(STRTRAN(vValExt,"Real",""))
   vValExt:=(STRTRAN(vValExt,"Reais",""))
   vValExt:=(STRTRAN(vValExt,"Centavo",""))
   vValExt:=(STRTRAN(vValExt,"Centavos",""))
ENDIF

RETURN alltrim(vvalext)

//////////////////////////////////////////////////////////////////
/////////// cExtenso := TAds_SubExtenso()    && Calcula o Extenso de 0...999
FUNCTION TAds_SubExtenso(SExt,f_cExtenso)
	local guni:=array(19), gdez:=array(9), gcen:=array(9)
	Local wsExt := ""
	guni[01]  = 'Um'
	guni[02]  = 'Dois'
	guni[03]  = 'Tres'
	guni[04]  = 'Quatro'
	guni[05]  = 'Cinco'
	guni[06]  = 'Seis'
	guni[07]  = 'Sete'
	guni[08]  = 'Oito'
	guni[09]  = 'Nove'
	guni[10]  = 'Dez'
	guni[11]  = 'Onze'
	guni[12]  = 'Doze'
	guni[13]  = 'Treze'
	guni[14]  = 'Catorze'
	guni[15]  = 'Quinze'
	guni[16]  = 'Dezesseis'
	guni[17]  = 'Dezesete'
	guni[18]  = 'Dezoito'
	guni[19]  = 'Dezenove'
	
	gdez[02]  = 'Vinte'
	gdez[03]  = 'Trinta'
	gdez[04]  = 'Quarenta'
	gdez[05]  = 'Cinquenta'
	gdez[06]  = 'Sessenta'
	gdez[07]  = 'Setenta'
	gdez[08]  = 'Oitenta'
	gdez[09]  = 'Noventa'
	
	gcen[01]  = 'Cento'
	gcen[02]  = 'Duzentos'
	gcen[03]  = 'Trezentos'
	gcen[04]  = 'Quatrocentos'
	gcen[05]  = 'Quinhentos'
	gcen[06]  = 'Seiscentos'
	gcen[07]  = 'Setecentos'
	gcen[08]  = 'Oitocentos'
	gcen[09]  = 'Novecentos'
	
	wSExt = ''
	////publ wsext
	if SExt = '100'
	   wSExt = 'CEM'
	 else
	   if subs(SExt,1,1) # '0'
	      wSExt := gcen[VAL(SUBSTR(SEXT,1,1))]
	   endif
	   if subs(SExt,2,2) # '00'
	      if subs(SExt,2,2) < '20'
	         wSExt := wSExt + iif(len(wSExt)#0,' e ','')
	         wSExt := wSExt + guni[VAL(SUBSTR(SEXT,2,2))]
	      else
	         wSExt := wSExt + iif(len(wSExt)#0,' e ','')
	         wSExt := wSExt + gdez[VAL(SUBSTR(SEXT,2,1))]
	         if subs(SExt,3,1) # '0'
	            wSExt := wSExt + iif(len(wSExt)#0,' e ','')
	            wSExt := wSExt + guni[VAL(SUBSTR(SEXT,3,1))]
	         endif
	      endif
	   endif
	endif
	f_cExtenso := f_cExtenso + wsext
	SExt := wSExt
	***vext := wsext

RETURN (f_cExtenso)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////// FUNCAO VALOR
/// Valor - Numero a transformar em caracter
/// nCasas - Numero de casas decimais / Padrao 2
/// SeBranco - Se o Valor for 0.00 retornar "" / branco nada
/// nEspaco - Calcula espacos a preencher para DOS Padrao 0
/// CharEspaco - Caracter para os Espacos / padrao " "
/// SeSifrao - Se mostra o sifrão da moeda R$
FUNCTION TAds_Valor(valor,nCasas,SeBranco,nEspaco,CharEspaco,SeSifrao)
	LOCAL numero:=""
	IF nCasas==NIL
	   nCasas:=2
	ENDIF
	IF SeBranco==NIL
	   Sebranco:=.T.
	ENDIF
	IF nEspaco==NIL
	   nEspaco:=0
	ENDIF
	IF CharEspaco==NIL
	   CharEspaco:=" "
	ENDIF
	IF SeSifrao==Nil
	   SeSifrao:=.T.
	ENDIF
	
	IF Valor==0.00 .AND. Sebranco
	   IF nEspaco==0
	      RETURN ("")
	   ELSE
	      RETURN REPLICATE(CharEspaco,nEspaco)
	   ENDIF
	ENDIF
	
	IF nCasas==2
	   Numero:=transform(valor,"@E 999,999,999.99")
	   numero:=strtran(numero," ","")
	ELSEIF nCasas==3
	   Numero:=transform(valor,"@E 999,999,999.999")
	   numero:=strtran(numero," ","")
	ELSEIF nCasas==4
	   Numero:=transform(valor,"@E 999,999,999.9999")
	   numero:=strtran(numero," ","")
	ELSEIF nCasas==5
	   Numero:=transform(valor,"@E 999,999,999.99999")
	   numero:=strtran(numero," ","")
	ENDIF
	
	IF SeSifrao
	   numero:="R$ "+numero
	ENDIF
	
	IF nEspaco > 0
	   Numero:= REPLICATE(CharEspaco,nEspaco-LEN(Numero)) + Numero
	ENDIF

RETURN (numero)
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_TxtToArray(f_cTxt,f_cCaracter, f_nPedacoMinimo)
	Local aTxt := {}, nContaPalavra := 0
	Local cPedaco := "", cLetra := ""
	Local cTxtAll := AllTrim(f_cTxt)

	Default f_cCaracter := " ", f_nPedacoMinimo := 3

	If At(f_cCaracter,cTxtall) == 0
	   Return {cTxtAll}
	Endif
	
	//cTxtAll += " ."
	Do While .T.
		nContaPalavra ++
		cPedaco := StrToken(cTxtAll,nContaPalavra,f_cCaracter)
		If EmpTy(cPedaco) 
			exit
		Else
			If Len(cPedaco) > f_nPedacoMinimo
				aadd(aTxt,cPedaco)
			EndIf
		EndIf 
	EndDo

RETURN aTxt
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CleanChars(f_cTxt,f_aCaracteresRetira)
	Local cTxtRetorno := f_cTxt, iFor := 0
	
	For iFor := 1 To Len(f_aCaracteresRetira)
   	cTxtRetorno := StrTran(cTxtRetorno,f_aCaracteresRetira[iFor],"")
   Next
   
RETURN cTxtRetorno
