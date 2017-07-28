#Include "Fivewin.ch"

FUNCTION tAdsPopupBrowse( aValue, oGet, bInit, nColumn, f_nWidth, f_nHeight)

   local oDlg, oBrw
   local bGetValid, uOrgl
   local aPoint := { oGet:nTop + oGet:nHeight, oGet:nLeft }

   DEFAULT nColumn := 1, f_nWidth := 500, f_nHeight := 180

   if oGet:Cargo == nil

      bGetValid   = oGet:bValid
      oGet:bValid = nil
      uOrgl       = oGet:VarGet()

      aPoint = ClientToScreen( oGet:oWnd:hWnd, aPoint )

      DEFINE DIALOG oDlg OF oGet:oWnd STYLE WS_POPUP ;
         SIZE f_nWidth, f_nHeight

      ACTIVATE DIALOG oDlg NOWAIT ;
         ON INIT oDlg:SetPos( aPoint[ 1 ], aPoint[ 2 ] )

      @ 0, 0 XBROWSE oBrw DATASOURCE aValue AUTOCOLS AUTOSORT ;
         SIZE oDlg:nWidth, oDlg:nHeight OF oDlg PIXEL NOBORDER

      oBrw:AutoFit()
      oBrw:lFastDraw := .t.
      oBrw:l2007      := .F.

      if bInit != nil
         Eval( bInit, oBrw )
      endif

      oBrw:CreateFromCode()

      oBrw:PostMsg( WM_SETFOCUS )
      oBrw:bKeyDown = { | nKey | If( nKey == VK_RETURN, ( oGet:bValid := bGetValid, oDlg:End() ), ) }
      oBrw:bChange = { || oGet:VarPut( oBrw:aCols[ nColumn ]:Value ), oGet:Refresh() }
      oBrw:bLButtonUp = { | nRow, nCol | If( nRow > oBrw:nHeaderHeight,;
                          ( Eval( oBrw:bChange ), oGet:bValid := bGetValid, oDlg:End(),;
                            oGet:oWnd:GoNextCtrl( oGet:hWnd ),;
                            oGet:Cargo := nil ),) }

      if oGet:oWnd:IsKindOf( "TDialog" )
         oGet:bLostFocus = { || If( GetFocus() != oDlg:hWnd, ( oGet:Cargo := nil, oGet:bValid := bGetValid, oDlg:End() ),) }
         oGet:oWnd:bGotFocus = { || oGet:Cargo := nil, oGet:bValid := bGetValid, oDlg:End() }
      endif

      oGet:oWnd:bMoved = { || oGet:Cargo := nil, oGet:bValid := bGetValid, oDlg:End() }

      oBrw:Seek( oGet:GetText() )

      oGet:Cargo = oDlg

      if ! oGet:oWnd:IsKindOf( "TXBROWSE" )
         oGet:bKeyDown = { | nKey | If( nKey == VK_DOWN, oBrw:GoDown(),),;
                                    If( nKey == VK_UP, oBrw:GoUp(),),;
                                    If( nKey == VK_DELETE .or. nKey == VK_ESCAPE, ( oGet:VarPut( uOrgl), oGet:Refresh(), oGet:bValid := bGetValid, oDlg:End()),),;
                                    If( nKey == VK_RETURN, ( oGet:bValid := bGetValid, oDlg:End()),), 0 }
      endif

      oGet:oWnd:bLClicked = { || oGet:bValid := bGetValid, oDlg:End(), oGet:Cargo := nil }
      oGet:oWnd:bMouseWheel = { || oDlg:SetFocus() }
   else
      oGet:Cargo:End()
      oGet:Cargo = nil
   endif

return nil

//----------------------------------------------------------------------------//
