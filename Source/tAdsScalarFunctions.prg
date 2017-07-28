

/*

 oAdsQuery:cSql := "CREATE FUNCTION recno(" +;
      "s_rowid CHAR ( 18 ) ) " + CRLF + ;
      "RETURNS INTEGER " + CRLF + ;
      "DESCRIPTION 'Convert string row_id to isam recno() integer value' " + CRLF + ;
      "BEGIN " + CRLF + ;
      CRLF + ;
      " return ( position( substring( s_rowid, 13, 1 ) in " +CRLF+;
      "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/')-1)*1073741824 + "+;
      "(position(substring(s_rowid, 14, 1) in " + ;
      "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/')-1)*16777216 + "+;
      "(position(substring(s_rowid, 15, 1) in " + ;
      "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/')-1)*262144 + "+;
      "(position(substring(s_rowid, 16, 1) in "+ ;
      "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/')-1)*4096 + "+;
      "(position(substring(s_rowid, 17, 1) in "+;
      "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/')-1)*64 + "+;
      "(position(substring(s_rowid, 18, 1) in "+;
      "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/')-1); " + CRLF + ;
      "END; "
      
      
      */