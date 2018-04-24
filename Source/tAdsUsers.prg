////////////////////////////////////////////////////////////////////////////////////////////////////
// Funções Relacionadas as propriedades de restrições aos Usuarios e Tabelas                      //
// Qualquer alteração ou modificação encaminhe para Giovany Vecchi                                //
// Giovany Vecchi 03/02/2015 giovanyvecchi@yahoo.com.br / giovanyvecchi@gmail.com                 //
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

#define ADS_DD_USER_PASSWORD           1101
#define ADS_DD_USER_GROUP_MEMBERSHIP   1102
#define ADS_DD_USER_BAD_LOGINS         1103

///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CreateUser(f_nConnection,f_cUserName,f_cPassWord,f_cComment)
  Local oDs_Qry := "", lExecute := .F.
  
  Default f_cPassWord := "", f_cComment := ""
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_CreateUser(_cUserName_,_cNewPassWord_,_cComment_);"
  oDs_Qry:DsAddVar("_cUserName_",f_cUserName)
  oDs_Qry:DsAddVar("_cPassWord_",f_cPassWord)
  oDs_Qry:DsAddVar("_cComment_",f_cComment)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_ModifyUserProperty(f_nConnection,f_cUserName,f_cNewPassWord)
  Local oDs_Qry := "", lExecute := .F.
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_ModifyUserProperty(_cUserName_,'USER_PASSWORD',_cNewPassWord_);"
  oDs_Qry:DsAddVar("_cUserName_",f_cUserName)
  oDs_Qry:DsAddVar("_cNewPassWord_",f_cNewPassWord)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_DropUser(f_nConnection,f_cUserName)
  Local oDs_Qry := "", lExecute := .F.
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_DropUser(_cUserName_);"
  oDs_Qry:DsAddVar("_cUserName_",f_cUserName)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_CreateGroup(f_nConnection,f_cGroupName,f_cComment)
  Local oDs_Qry := "", lExecute := .F.
  
  Default f_cComment := ""
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_CreateGroup(_cGroupName_,_cComment_);"
  oDs_Qry:DsAddVar("_cGroupName_",f_cGroupName)
  oDs_Qry:DsAddVar("_cComment_",f_cComment)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_ModifyGroupProperty(f_nConnection,f_cGroupName,f_cDataBaseProperty,f_cComment)
  Local oDs_Qry := "", lExecute := .F.
  
  Default f_cDataBaseProperty := "", f_cComment := ""
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_ModifyGroupProperty(_cGroupName_,_cDataBaseProperty_,_cComment_);"
  oDs_Qry:DsAddVar("_cGroupName_",f_cGroupName)
  oDs_Qry:DsAddVar("_cDataBaseProperty_",f_cDataBaseProperty)
  oDs_Qry:DsAddVar("_cComment_",f_cComment)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_DropGroup(f_nConnection,f_cGroupName)
  Local oDs_Qry := "", lExecute := .F.
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_DropGroup(_cGroupName_);"
  oDs_Qry:DsAddVar("_cGroupName_",f_cGroupName)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_AddUserToGroup(f_nConnection,f_cUserName,f_cGroupName)
  Local oDs_Qry := "", lExecute := .F.
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_AddUserToGroup(_cUserName_,_cGroupName_);"
  oDs_Qry:DsAddVar("_cUserName_",f_cUserName)
  oDs_Qry:DsAddVar("_cGroupName_",f_cGroupName)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute
///////////////////////////////////////////////////////////////////////////////
FUNCTION TAds_RemoveUserFromGroup(f_nConnection,f_cUserName,f_cGroupName)
  Local oDs_Qry := "", lExecute := .F.
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_RemoveUserFromGroup(_cUserName_,_cGroupName_);"
  oDs_Qry:DsAddVar("_cUserName_",f_cUserName)
  oDs_Qry:DsAddVar("_cGroupName_",f_cGroupName)
  lExecute := oDs_Qry:DsExecute()

RETURN lExecute

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
