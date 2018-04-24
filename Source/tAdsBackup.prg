////////////////////////////////////////////////////////////////////////////////////////////////////
// Fun��es para Backup                                                                            //
// Qualquer altera��o ou modifica��o encaminhe para Giovany Vecchi                                //
// Giovany Vecchi 06/03/2018 giovanyvecchi@gmail.com                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////
// Help in:
// http://devzone.advantagedatabase.com/dz/webhelp/Advantage10.1/master_sp_backupdatabase.htm?zoom_highlightsub=sp_backupDatabase
// http://devzone.advantagedatabase.com/dz/webhelp/Advantage10.1/devguide_creating_a_backup_using_system_stored_procedures.htm
// http://devzone.advantagedatabase.com/dz/webhelp/Advantage10.1/master_backup_and_restore_options.htm

/*
  Portugues:
  Ambos as fun��es tAds_BackupDataBase e tAds_RestoreDatabase para escolher o destino do backup ou restore
  deve-se colocar a pasta onde o servidor enxerga, e n�o o terminal. A copia � gerada pelo servidor remoto
  e as pastas validas s�o as que o servidor possa acessar.
  Quando fazer uma nova copia n�o utilize o parametro f_cOptionBackup, ser� criado uma copia completa.
  Ap�s criar a copia completa, use o parametro f_cOptionBackup como 'PrepareDiff' apenas uma vez, isto
  at� voce incluir novos campos, indices, tabelas etc, e enquando n�o haver altera��es na estrutura das 
  tabelas os no dicionario de dados use a op��o 'diff' para fazer uma copia diferencial, que sera apenas
  atualizado inclus�es e altera��es e quando houver mudan�as na estrutura execute 'PrepareDiff' novamente.
  -----------------------------------------------------------------------------------------------------------
  Espanhol
  Ambos funciones tAds_BackupDataBase y tAds_RestoreDatabase para elegir el destino de copia de seguridad o restore
  se debe colocar la carpeta donde el servidor ve, y no el terminal. La copia es generada por el servidor remoto
  y las carpetas validas son las que el servidor puede tener acceso.
  Al hacer una nueva copia no utilice el par�metro f_cOptionBackup, se crear� una copia completa.
  Despu�s de crear la copia completa, utilice el par�metro f_cOptionBackup como 'PrepareDiff' s�lo una vez, esto
  hasta que incluya nuevos campos, �ndices, tablas, etc., y mientras no haya cambios en la estructura de las estructuras
  tablas en el diccionario de datos utilice la opci�n 'diff' para hacer una copia diferencial, que ser� s�lo
  actualizaciones actualizadas y cambios y cuando haya cambios en la estructura ejecute 'PrepareDiff' de nuevo.
  -----------------------------------------------------------------------------------------------------------
  English
  Both the tAds_BackupDataBase and tAds_RestoreDatabase functions to choose the backup or restore destination
  you should put the folder where the server sees, not the terminal. The copy is generated by the remote server
  and the valid folders are the ones the server can access.
  When making a new copy does not use the f_cOptionBackup parameter, a complete copy will be created.
  After creating the complete copy, use the f_cOptionBackup parameter as 'PrepareDiff' only once, this
  until you include new fields, indexes, tables etc, and when there are no changes in the structure of the
  tables in the data dictionary use the 'diff' option to make a differential copy, which will only be
  updated inclusions and changes and when there are changes in the structure run 'PrepareDiff' again.
*/

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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_BackupDataBase(f_nConnection,f_cDestinationPath,f_cOptionBackup)
  Local oDs_Qry, lExecute := .F.
  
  Default f_nConnection   := tAds_GetConnectionDefault()
  Default f_cOptionBackup := "" 
          // Include        > Include one or more tables. When using Include, follow it with an equals sign 
          //                  followed by a comma-separated list of tables you want to include. 
          // Exclude        > Exclude one or more tables. When using Exclude, follow it with an equals sign 
          //                  followed by a comma-separated list of tables you want to exclude.
          // DontOverwrite  > Use DontOverwrite to prevent backup from replacing an existing table during a
          //                  restore operation. If you include DontOverwrite and a table being restored
          //                  already exists at the restored destination, the result set will include a warning.
          // MetaOnly       > Use MetaOnly if you want to make a backup of the data dictionary files without
          //                  actually backing up any of the database's tables.
          // PrepareDiff    > Use PrepareDiff to prepare a database for a differential backup. PrepareDiff
          //                  needs to be executed only once, prior to initiating a differential backup.
          // Diff           > Use Diff to initiate a differential backup. Before you create your first
          //                  differential backup, you must first execute sp_BackupDatabase 
          //                  (or sp_BackupFreeTables) with the PrepareDiff option. Differential backups
          //                   are only supported with ADT tables.

  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_BackupDatabase('"+;
                      f_cDestinationPath+"', '"+;
                      f_cOptionBackup+"')"
  lExecute := oDs_Qry:DsExecute()
  oDs_Qry:End()

RETURN lExecute  
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
FUNCTION tAds_RestoreDataBase(f_cSourcePath,f_cDestinationPath,f_cSourcePassword)
  Local oDs_Qry, lExecute := .F.
  
  // Note: f_cSourcePassword is password adssys user
  
  oDs_Qry := tAds():DsNew(2,f_nConnection)
  oDs_Qry:cQrySql := "EXECUTE PROCEDURE sp_RestoreDatabase('"+;
                      f_cSourcePath+"', '"+;
                      f_cDestinationPath+"', '"+;
                      f_cSourcePassword+"')"
  lExecute := oDs_Qry:DsExecute()
  oDs_Qry:End()

RETURN lExecute  
