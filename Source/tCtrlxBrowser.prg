///#Include "xHbCls.ch"
#Include "Fivewin.ch"
#Include "xBrowse.ch"

// Filter Aofs option
#Define ADS_DYNAMIC_AOF           00
#Define ADS_RESOLVE_IMMEDIATE     01
#Define ADS_RESOLVE_DYNAMIC       02
#Define ADS_KEYSET_AOF            04
#Define ADS_FIXED_AOF             08
#define ADS_KEEP_AOF_PLAN         10

//Mode OrdKeyCount
#define ADS_RESPECTFILTERS                1
#define ADS_IGNOREFILTERS                 2
#define ADS_RESPECTSCOPES                 3
#define ADS_REFRESHCOUNT                  4

Class tCtrlxBrw ///From TXbrowse

  Data oXBrwCtrl, oDbDs_DataBaseAds
  Data bStrData
  Data oGetDsFilter, nTpCampoFilter, cCampoFilter, lLastRecZero
  Data bChangeBrw
  Data nTpKeyCount, nTpKeyNo, nTpFilterAof
  Data aOrdersCreation
  
  Method New(f_oXBrowse,f_oDbDs_DataBaseAds) Constructor
  Method End()

  Method SetTAds()
  Method ClearSetTAds()
  
  Method tCtrlxBrw_SetOrder(f_cSortOrder)
  Method tCtrlxBrw_Skip(f_nKip)
  
  Method tCtrlxBrw_Seek(f_oBjCol,f_uVar)
  Method tCtrlxBrw_EditFilterRdd()
  Method tCtrlxBrw_EditFilterSql()
  Method tCtrlxBrw_EditChange()
  Method tCtrlxBrw_EditClearChar(f_cTxt, f_nTipo)


Endclass
//-----------------------------------------------------------------------------
Method New(f_oXBrowse,f_oDbDs_DataBaseAds,f_aGetFilter) Class tCtrlxBrw
  Local oSelf

  // f_aGetFilter[1] > oBjeto Get
  // f_aGetFilter[2] > Tipo Filtro 1=SetAof contains / 2=Sql select Contains
  // f_aGetFilter[3] > Nome do campo a pesquisar
   
  ///Override Method Skip In Class Txbrowse With oSelf:tCtrlxBrw_Skip

  ::lLastRecZero  := .F.
  ::nTpKeyCount   := ADS_RESPECTFILTERS
  ::nTpKeyNo      := ADS_RESPECTFILTERS
  ::nTpFilterAof  := ADS_RESOLVE_IMMEDIATE

  ::oDbDs_DataBaseAds     := f_oDbDs_DataBaseAds
  ::oXBrwCtrl             := f_oXBrowse
  
  ::oXBrwCtrl:cAlias      := ::oDbDs_DataBaseAds:cAlias
  
  If !Hb_IsNil(f_aGetFilter)

    ::oGetDsFilter    := f_aGetFilter[1]
    ::nTpCampoFilter  := f_aGetFilter[2]
    ::cCampoFilter    := f_aGetFilter[3]
    
    If f_aGetFilter[2] == 2 // Sql Contains
      ::oGetDsFilter:bChange := {||::tCtrlxBrw_EditFilterSql()}
    
    ElseIf f_aGetFilter[2] == 1 // Rdd set aof

      ::oGetDsFilter:bChange := {||::tCtrlxBrw_EditFilterRdd()}
      
    EndIf

    ::bChangeBrw := ::oXBrwCtrl:bChange
    ::oXBrwCtrl:bChange    := {||::tCtrlxBrw_EditChange()}
    ::oXBrwCtrl:bGotFocus  := {||::tCtrlxBrw_EditChange()}
    
  EndIf  

  ::aOrdersCreation   := {}
  ::oXBrwCtrl:bClrSel := {|| { CLR_BLACK, CLR_HGRAY} } // Giovany
  ::SetTAds()
  ::oDbDs_DataBaseAds:Refresh()

Return Self
//-----------------------------------------------------------------------------
Method End() Class tCtrlxBrw

  Self := Nil

  ///? "saiu"

Return Nil
//-----------------------------------------------------------------------------
Method SetTAds() Class tCtrlxBrw
  Local nTpGetRelPos := 1, cTmpQry := "", nTmp := 0

  ::oDbDs_DataBaseAds:lBufferOnSkip := .T.
  
  If ::oDbDs_DataBaseAds:nOpenType == 3 // Open via Dataset
    cTmpQry := Upper(::oDbDs_DataBaseAds:cQrySql)
    If At("{STATIC}",cTmpQry) == 0 
      nTpGetRelPos := 2
    Else
      nTpGetRelPos := 1 
    EndIf
  Else
    nTpGetRelPos := 1
  EndIf

  If nTpGetRelPos == 2 // Open via Sql Dataset cursor vivo 

    ::oXBrwCtrl:bKeyNo    := {| n, _Self | If( n == nil, ;
                                   nCalcRelPos((::oDbDs_DataBaseAds:GetRelKeyPos() * _Self:nLen) /10,;
                                                ::oXBrwCtrl,;
                                                ::oDbDs_DataBaseAds) , ;
                                   ::oDbDs_DataBaseAds:SetRelKeyPos( Round(n / _Self:nLen,3) ) ) }

  Else // Via Rdd ou Static Cursor

    ::oXBrwCtrl:bKeyNo    := {| n, _Self | If( n == nil, ;
                                 nCalcRelPos((::oDbDs_DataBaseAds:GetRelKeyPos() * _Self:nLen)  ,;
                                              ::oXBrwCtrl,;
                                              ::oDbDs_DataBaseAds ) , ;
                                 ::oDbDs_DataBaseAds:SetRelKeyPos( Int(Round((n+1) / _Self:nLen,3)))) }
  EndIf

  ::oXBrwCtrl:bBookMark := {| n | iif( n == nil,;
                               ::oDbDs_DataBaseAds:Recno() , ;
                               ::oDbDs_DataBaseAds:GoTo( n ) ) }

  ::oXBrwCtrl:bKeyCount := {|| ::oDbDs_DataBaseAds:KeyCount(::nTpKeyCount)  } 
  
  ::oXBrwCtrl:bBof      := { ||  ::oDbDs_DataBaseAds:Bof() }
  ::oXBrwCtrl:bEof      := { ||  ::oDbDs_DataBaseAds:Eof() }
  ::oXBrwCtrl:bSkip     := { |n| Iif( n == Nil, n := 1, ), ::tCtrlxBrw_Skip( n )} 
  ::oXBrwCtrl:bGoTop    := { ||  ::oDbDs_DataBaseAds:GoTop() }
  ::oXBrwCtrl:bGoBottom := { ||  ::oDbDs_DataBaseAds:GoBottom() }


  ::oDbDs_DataBaseAds:OnSkip()

  For nTmp := 1 to Len(::oXBrwCtrl:aCols)
    If !Empty(::oXBrwCtrl:aCols[nTmp]:cSortOrder)
      aadd(::aOrdersCreation,{nTmp,::oXBrwCtrl:aCols[nTmp]:cSortOrder})
      ::oXBrwCtrl:aCols[nTmp]:bLClickHeader := {|nRow,nCol,nFlag,ooBj|::tCtrlxBrw_SetOrder(ooBj)}
    EndIf
  Next
 
Return Nil
//-----------------------------------------------------------------------------
Method ClearSetTAds() Class tCtrlxBrw 

  ::oXBrwCtrl:bKeyNo    := {||0}
  ::oXBrwCtrl:bBookMark := {||0}
  ::oXBrwCtrl:bKeyCount := {||0}
  ::oXBrwCtrl:bBof      := {||0}
  ::oXBrwCtrl:bEof      := {||0}
  ::oXBrwCtrl:bSkip     := {||0} 
  ::oXBrwCtrl:bGoTop    := {||0}
  ::oXBrwCtrl:bGoBottom := {||0}
  
Return Nil
//-----------------------------------------------------------------------------
Method tCtrlxBrw_SetOrder(f_cObjCol) Class tCtrlxBrw
  Local iForOrder := 0, nOrderPos := 0, lc_lSeekBar := .F.
  
  For iForOrder := 1 To Len(::aOrdersCreation)
    If f_cObjCol:nCreationOrder == ::aOrdersCreation[iForOrder,1]
      nOrderPos := iForOrder
      Exit
    EndIf
  Next
  
  lc_lSeekBar := ::oXBrwCtrl:lSeekBar
  
  If nOrderPos == 0
    Return Nil
  EndIf
  
  If !Empty(::aOrdersCreation[nOrderPos,2])
    
    If ::oDbDs_DataBaseAds:lDsStaticCursors
      ///AEval( ::oXBrwCtrl:aCols, { |o| o:cSortOrder := ""  } )
    EndIf
    If ::oXBrwCtrl:lSeekBar
      //::oXBrwCtrl:RefreshHeaders(.T.)  
      //::oXBrwCtrl:Refresh()
      ///AEval( ::oXBrwCtrl:aCols, { |o| o:CheckBarGet( .F. ) } )
      ///AEval( ::oXBrwCtrl:aCols, { |o| o:ShowSeek( .F. ) } )
    EndIf

    f_cObjCol:cSortOrder := ::aOrdersCreation[nOrderPos,2]
    ///f_cObjCol:cDataType := ValType(::oDbDs_DataBaseAds:VarGet(f_cObjCol:cSortOrder))
    ::oDbDs_DataBaseAds:SetOrder(::aOrdersCreation[nOrderPos,2])
    ///? "tCtrlxBrw_SetOrder > ::oDbDs_DataBaseAds:cOrderFocus " + ::oDbDs_DataBaseAds:cOrderFocus
    If ::oXBrwCtrl:lSeekBar
      //AEval( ::oXBrwCtrl:aCols, { |o| o:CheckBarGet( .T. ) } )
      //AEval( ::oXBrwCtrl:aCols, { |o| o:ShowSeek( .T. ) } )
    EndIf
    ::oXBrwCtrl:lSeekBar := lc_lSeekBar

    //::oXBrwCtrl:Adjust()
    f_cObjCol:SetOrder()

  EndIf
  ///xBrowse(f_cObjCol)
  //::oDbDs_DataBaseAds:SetOrder(f_cSortOrder)
  //::oXBrwCtrl:aCols[nTmp]:cSortOrder}
  
Return Nil  

//-----------------------------------------------------------------------------
Method tCtrlxBrw_Skip(f_nSkip) Class tCtrlxBrw
  Local nSkipper := 0
  
  Default f_nSkip := 1 

  nSkipper := (::oDbDs_DataBaseAds:cAlias)->(__DbSkipper(f_nSkip))
 
  ::oDbDs_DataBaseAds:OnSkip()
  
Return nSkipper  
//-----------------------------------------------------------------------------
Method tCtrlxBrw_Seek(f_oBjCol,f_uVar,f_lTpSeek) Class tCtrlxBrw
LOCAL nRecMarca := ::oDbDs_DataBaseAds:Recno(), cTpData := "", lc_cOrderIn

  lc_cOrderIn := f_oBjCol:cSortOrder
  
  cTpData := VALTYPE(f_uVar)

  IF cTpData == "N"
     f_lTpSeek := .F.
  ELSEIF cTpData == "D"
     f_lTpSeek := .F.
  ELSEIF cTpData == "C"
     f_lTpSeek := .T.
  ELSEIF cTpData == "U"
     Return Nil
  ENDIF
  
  If ::oDbDs_DataBaseAds:lDsStaticCursors
    ::tCtrlxBrw_SetOrder(f_oBjCol)

    //f_oBjCol:HeaderLButtonUp(1,1,0) ///PaintHeader( 2, nil, ::oXBrwCtrl:nHeaderHeight - 3, .t. ) ////HeaderLButtonDown( 1, 1, 0 )
    //? f_oBjCol:cSortOrder
    //::oDbDs_DataBaseAds:SetOrder(f_oBjCol:cSortOrder)
    //::oXBrwCtrl:RefreshHeaders()
    //f_oBjCol:HeaderLButtonUp(1,1,0) ///PaintHeader( 2, nil, ::oXBrwCtrl:nHeaderHeight - 3, .t. ) ////HeaderLButtonDown( 1, 1, 0 )
    f_oBjCol:SetOrder()
  Else
    ::oDbDs_DataBaseAds:SetOrder(f_oBjCol:cSortOrder)
    f_oBjCol:SetOrder()
  EndIf    
    
    
  If Empty(f_oBjCol:cSortOrder)
    f_oBjCol:cSortOrder := lc_cOrderIn
  EndIf  
  If Empty(::oDbDs_DataBaseAds:cOrderFocus)
    ///::oDbDs_DataBaseAds:Locate(f_oBjCol:cSortOrder,f_uVar)
    //? "sdcsds"
    ::oDbDs_DataBaseAds:Seek(f_uVar,f_oBjCol:cSortOrder,f_lTpSeek)
  Else  
    ::oDbDs_DataBaseAds:Seek(f_uVar,f_oBjCol:cSortOrder,f_lTpSeek)
  EndIf
  
  IF ::oDbDs_DataBaseAds:EOF()
  
     ::oDbDs_DataBaseAds:GoTo(nRecMarca)
     
     MSGSTOP("A ocorrencia solicitada não foi encontrada.",;
             "Procedimento abortado...")
  
  ENDIF

  ::oXBrwCtrl:nRowSel := 1
  ///::oXBrwCtrl:Skip(-1)
  ::oXBrwCtrl:Refresh(.T.)
   
  IF ::oXBrwCtrl:bChange != nil
     EVAL(::oXBrwCtrl:bChange)
  ENDIF

RETURN NIL
//-----------------------------------------------------------------------------
Method tCtrlxBrw_EditFilterRdd() Class tCtrlxBrw
  Local cTxtDigitacao := "", aTxtDigitacao := {} ,cAof := ""
  Local aCampos := {}
  Local iForTxt := 0,  iForCampo := 0, cAnd := "", cOr := "", iFor := 0
  Local nRowCounts := 0, nRowFocus := 0, nSeekRecFound := 0, nCountSkip := 0
  
  cTxtDigitacao := Alltrim(::oGetDsFilter:cText)
  
  If Len(cTxtDigitacao) < 4
    cTxtDigitacao := ""
  Else
    ::lLastRecZero := .F.
  EndIf
  
  aTxtDigitacao := Hb_aTokens(cTxtDigitacao)

  aCampos := Hb_aTokens(::cCampoFilter,",")
  
  For iForCampo := 1 To Len(aCampos)
    cAof += cOr +"("
    For iForTxt := 1 to Len(aTxtDigitacao)
      cAof += (cAnd + Chr(39)+aTxtDigitacao[iForTxt]+Chr(39)+" $"+aCampos[iForCampo])
      cAnd := " .and. " 
    Next
    cOr     := " .or. "
    cAnd    := ""
    cAof += ")"
  Next
    
  If ::lLastRecZero
    Return Nil
  EndIf

  If GetKeyState(VK_SHIFT)
    //msginfo(cAof)
  EndIf
   
  ::oDbDs_DataBaseAds:Filter(cAof,,::nTpFilterAof)
  ::oDbDs_DataBaseAds:GoTop()
  nRowCounts := ::oXBrwCtrl:nLen 

  If ::oDbDs_DataBaseAds:nOpenType == 1 // Via Rdd

    If !::oDbDs_DataBaseAds:Seek(cTxtDigitacao)
      ::oDbDs_DataBaseAds:GoTop()
      nRowFocus := 1 
    Else 
      nSeekRecFound := ::oDbDs_DataBaseAds:Recno()
      nRowFocus := Int(::oXBrwCtrl:nDataRows / 2)
      For Ifor := 1 to nRowFocus
        nCountSkip ++
        ::oDbDs_DataBaseAds:Skip(-1)
        If ::oDbDs_DataBaseAds:Bof()
          Exit
        EndIf
      Next
  
      ::oDbDs_DataBaseAds:GoTo(nSeekRecFound)
      
      If nCountSkip < nRowFocus
        nRowFocus := nCountSkip      
      EndIf
      
    EndIf

  EndIf    

  ::oXBrwCtrl:nRowSel := nRowFocus
  
  ::oXBrwCtrl:Refresh()
  
  If !Hb_IsNil(::bChangeBrw)
    ///? "aqui"
    Eval(::bChangeBrw)
  EndIf


  If Len(cTxtDigitacao) < 3
    ::lLastRecZero := .T.
  EndIf

Return Nil
//-----------------------------------------------------------------------------
Method tCtrlxBrw_EditFilterSql() Class tCtrlxBrw
  Local cTxtDigitacao := ""

  //::oGetDsFilter:Save()
  cTxtDigitacao := Alltrim(::tCtrlxBrw_EditClearChar(::oGetDsFilter:cText))

  If Len(cTxtDigitacao) < 5
    cTxtDigitacao := ""
  Else
    ::lLastRecZero := .F.
  EndIf
  
  If ::lLastRecZero
    Return Nil
  EndIf
  ///? cTxtDigitacao

  ::oDbDs_DataBaseAds:aVarsSql := {{"_TxtDigitacao_",cTxtDigitacao},;
                                   {"_Campo_",::cCampoFilter}}
  
  ::oDbDs_DataBaseAds:DsExecute()
  ::oXBrwCtrl:Refresh(.T.)

  If Len(cTxtDigitacao) < 5
    ::lLastRecZero := .T.
  EndIf

Return Nil
//-----------------------------------------------------------------------------
Method tCtrlxBrw_EditChange() Class tCtrlxBrw
  Local cFirstFielf := "", aFields := {} 
  
  If ::oDbDs_DataBaseAds:LastRec() == 0
    Return Nil
  EndIf
  
  If AT(",",::cCampoFilter) > 0
    aFields := Hb_aTokens(::cCampoFilter,",")
    cFirstFielf := Alltrim(aFields[1])
    //::oGetDsFilter:VarPut(::oDbDs_DataBaseAds:VarGet(cFirstFielf))
  Else
    cFirstFielf := Alltrim(::cCampoFilter)
    ::oGetDsFilter:VarPut(::oDbDs_DataBaseAds:VarGet(cFirstFielf))
    ::oGetDsFilter:Refresh()
  EndIf
  //::oGetDsFilter:VarPut(::oDbDs_DataBaseAds:VarGet(::cCampoFilter))
  
  If !Hb_IsNil(::bChangeBrw)
    ///? "aqui"
    Eval(::bChangeBrw)
  EndIf
  
Return Nil

//-----------------------------------------------------------------------------
Method tCtrlxBrw_EditClearChar(f_cTxt) Class tCtrlxBrw
  Local cRetTxt := f_cTxt
  Local iFor := 0, cAnd := ""
  Local aCharDeletes := { chr(34),"-",".","+","=","&","'","´"}
  Local aPalavras := {}
  
  cRetTxt := _CHAR_CLEAN(cRetTxt,{ " AND ",;
                                      " NEAR ",;
                                      " ANY "})

  For iFor := 1 To Len(aCharDeletes)
    cRetTxt := StrTran(cRetTxt,aCharDeletes[iFor]," ")
  Next

  aPalavras := Hb_aTokens(cRetTxt)

  cRetTxt := ""
  For iFor := 1 To Len(aPalavras)
    If Len(aPalavras[iFor]) > 3
      cRetTxt += (cAnd + Chr(34)+aPalavras[iFor]+Chr(34)) //+ " "
      //cRetTxt += (cAnd + "'" + aPalavras[iFor]+"'") //+ " "
      cAnd := " and "
    Endif
  Next

Return cRetTxt
///////////////////////////////////////////////////////////////////////////////
FUNCTION nCalcRelPos(f_nVal,f_oXBrw, f_oDbDs)
  Local nLenRegs := f_oXBrw:nLen, nRowPos := f_oXBrw:nRowSel
  Local nRetorno := 0
  
  If !Hb_isNil(f_oDbDs)
    
    If !Empty(f_oDbDs:GetFilter())
      f_nVal := (f_oDbDs:GetRelKeyPos() * nLenRegs) /10
    EndIf

  EndIf
    
  nRetorno := Round(f_nVal,3) ///Int(f_nVal) ///Round(f_nVal,3)
  
  If f_nVal > 80
    //f_nVal ++
  EndIf
  
  If nRetorno == 0
    nRetorno := 1
  EndIf

  If nRetorno < nRowPos
    nRetorno := nRowPos
  EndIf  

  nRetorno := Int(nRetorno)

RETURN nRetorno
///////////////////////////////////////////////////////////////////////////////
FUNCTION _CHAR_CLEAN(f_cTxt,f_aCaracteresRetira)
  Local cTxtRetorno := f_cTxt, iFor := 0
  
  For iFor := 1 To Len(f_aCaracteresRetira)
     cTxtRetorno := StrTran(cTxtRetorno,f_aCaracteresRetira[iFor],"")
  Next
   
RETURN cTxtRetorno
