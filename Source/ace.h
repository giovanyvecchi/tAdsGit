// Copyright (c) 2001-2005 Extended Systems, Inc.
// Portions Copyright (c) 2005-2008, iAnywhere Solutions, Inc.
// All rights reserved. All unpublished rights reserved.
//
// This source code can be used, modified, or copied by the licensee as long as
// the modifications (or the new binary resulting from a copy or modification of
// this source code) are used with Extended Systems' products. The source code
// is not redistributable as source code, but is redistributable as compiled
// and linked binary code. If the source code is used, modified, or copied by
// the licensee, Extended Systems Inc. reserves the right to receive from the
// licensee, upon request, at no cost to Extended Systems Inc., the modifications.
//
// Extended Systems Inc. does not warrant that the operation of this software
// will meet your requirements or that the operation of the software will be
// uninterrupted, be error free, or that defects in software will be corrected.
// This software is provided "AS IS" without warranty of any kind. The entire
// risk as to the quality and performance of this software is with the purchaser.
// If this software proves defective or inadequate, purchaser assumes the entire
// cost of servicing or repair. No oral or written information or advice given
// by an Extended Systems Inc. representative shall create a warranty or in any
// way increase the scope of this warranty.

/*******************************************************************************
* Source File  : ace.h
* Description  : This is the main header file for the Advantage Client
*                Engine.  It contains the type definitions, constants,
*                and prototypes for the APIs
*******************************************************************************/

#ifndef __ACE_INCLUDED__
#define __ACE_INCLUDED__


#if defined( ADS_LINUX )
   #include "unixutils.h"

   #define ADS_PATH_DELIMITER    '/'

   #ifdef ASANLM
      #define delay(x) AdsSleep(x)
   #endif
#endif

#if defined( unix )
   #ifndef ADS_LINUX
      #define ADS_LINUX
   #endif
#endif

#if defined( ADS_LINUX ) && defined( ACE )
   /* This makes the callback functions compile in linux */
   #define WINAPI   /* nothing */
   #define FARPROC  void*
   #define HWND     void*
#endif  // ADS_LINUX && ACE


#ifdef ADS_LINUX
   #pragma pack( 1 )
#else
   #pragma pack( push, 1 )
#endif

#if defined( WIN32 ) && !defined( SNAPDUMP )
   #define ADS_WIN32
#endif

/* This forces a warning for single equals in if statements */
#ifdef ADS_WIN32
   // 16-bit compiler doesn't seem to like this
   #pragma warning( error : 4706 )

   #define ADS_PATH_DELIMITER    '\\'
#endif

/* added to get access() prototype */
#if defined( ADS_LINUX ) && !defined( NATIVE_SQL )
   #include <unistd.h>
#endif

/* data type definitions */
#ifndef RDDUNVRS_H
   #if defined( ADS_LINUX )
      /*
       * long on Linux has a variable size depending on the underlying
       * architecture.
       */
      typedef unsigned int   UNSIGNED32;
      typedef int            SIGNED32;
   #else
      typedef unsigned long  UNSIGNED32;
      typedef long           SIGNED32;
   #endif

   typedef unsigned short UNSIGNED16;
   typedef short          SIGNED16;
   typedef unsigned char  UNSIGNED8;
   typedef char           SIGNED8;

#endif


#ifndef ADSHANDLE_DEFINED
   #define ADSHANDLE_DEFINED
   #if defined( x64 )
      #if defined( ADS_LINUX )
         // Long is 64 bits in GCC.
         typedef unsigned long  ADSHANDLE;
      #else
         typedef unsigned __int64  ADSHANDLE;
      #endif
   #else
      typedef unsigned long  ADSHANDLE;
   #endif
#endif

   typedef double         DOUBLE;

#ifdef ADS_WIN32
   typedef ULONGLONG        UNSIGNED64;
   typedef LONGLONG         SIGNED64;
#elif defined( NLM ) && !defined( ADS_LINUX )
   #ifndef ADS_64INTS
      #define ADS_64INTS
         typedef __int64          SIGNED64;
         typedef unsigned __int64 UNSIGNED64;
   #endif
#elif defined( ADS_LINUX ) || defined( __linux__ )
   /* <sys/types.h> is required for loff_t (64bit ints) */
   #include <sys/types.h>

   /* use a define here to solve 64bit int typedef problems, see wincompat.h & rddunvrs.h */
   #ifndef ADS_64INTS
   #define ADS_64INTS
   typedef loff_t           UNSIGNED64;
   typedef loff_t           SIGNED64;
   #endif
#endif

#define VOID   void
#define EXTERN extern
#define STATIC static


#if defined( ASANT ) || defined( ADS_NT ) || defined( ADS_WIN9X ) || ( defined( x64 ) && !defined( ADS_LINUX ) )
   #define ENTRYPOINT WINAPI
#elif defined( ASANLM ) || defined( ADS_LINUX ) || defined( NLM )
   #define ENTRYPOINT
#elif defined( ADS_WIN32 ) && !defined( __BORLANDC__ )
   #define ENTRYPOINT _declspec( dllexport ) WINAPI
#else
   #define ENTRYPOINT _export WINAPI
#endif


/* Logical constants */
#define ADS_FALSE                0
#define ADS_TRUE                 1


/* This is for parameters to routines that accept a default setting */
#define ADS_DEFAULT              0

/* character set types */
#define ADS_ANSI                       1
#define ADS_OEM                        2
#define CZECH_VFP_CI_AS_1250           3
#define GENERAL_VFP_CI_AS_1250         4
#define HUNGARY_VFP_CI_AS_1250         5
#define MACHINE_VFP_BIN_1250           6
#define POLISH_VFP_CI_AS_1250          7
#define SLOVAK_VFP_CI_AS_1250          8
#define MACHINE_VFP_BIN_1251           9
#define RUSSIAN_VFP_CI_AS_1251         10
#define DUTCH_VFP_CI_AS_1252           11
#define GENERAL_VFP_CI_AS_1252         12
#define GERMAN_VFP_CI_AS_1252          13
#define ICELAND_VFP_CI_AS_1252         14
#define MACHINE_VFP_BIN_1252           15
#define NORDAN_VFP_CI_AS_1252          16
#define SPANISH_VFP_CI_AS_1252         17
#define SWEFIN_VFP_CI_AS_1252          18
#define UNIQWT_VFP_CS_AS_1252          19
#define GREEK_VFP_CI_AS_1253           20
#define MACHINE_VFP_BIN_1253           21
#define GENERAL_VFP_CI_AS_1254         22
#define MACHINE_VFP_BIN_1254           23
#define TURKISH_VFP_CI_AS_1254         24
#define DUTCH_VFP_CI_AS_437            25
#define GENERAL_VFP_CI_AS_437          26
#define GERMAN_VFP_CI_AS_437           27
#define ICELAND_VFP_CI_AS_437          28
#define MACHINE_VFP_BIN_437            29
#define NORDAN_VFP_CI_AS_437           30
#define SPANISH_VFP_CI_AS_437          31
#define SWEFIN_VFP_CI_AS_437           32
#define UNIQWT_VFP_CS_AS_437           33
#define GENERAL_VFP_CI_AS_620          34
#define MACHINE_VFP_BIN_620            35
#define POLISH_VFP_CI_AS_620           36
#define GREEK_VFP_CI_AS_737            37
#define MACHINE_VFP_BIN_737            38
#define DUTCH_VFP_CI_AS_850            39
#define GENERAL_VFP_CI_AS_850          40
#define ICELAND_VFP_CI_AS_850          41
#define MACHINE_VFP_BIN_850            42
#define NORDAN_VFP_CI_AS_850           43
#define SPANISH_VFP_CI_AS_850          44
#define SWEFIN_VFP_CI_AS_850           45
#define UNIQWT_VFP_CS_AS_850           46
#define CZECH_VFP_CI_AS_852            47
#define GENERAL_VFP_CI_AS_852          48
#define HUNGARY_VFP_CI_AS_852          49
#define MACHINE_VFP_BIN_852            50
#define POLISH_VFP_CI_AS_852           51
#define SLOVAK_VFP_CI_AS_852           52
#define GENERAL_VFP_CI_AS_857          53
#define MACHINE_VFP_BIN_857            54
#define TURKISH_VFP_CI_AS_857          55
#define GENERAL_VFP_CI_AS_861          56
#define ICELAND_VFP_CI_AS_861          57
#define MACHINE_VFP_BIN_861            58
#define GENERAL_VFP_CI_AS_865          59
#define MACHINE_VFP_BIN_865            60
#define NORDAN_VFP_CI_AS_865           61
#define SWEFIN_VFP_CI_AS_865           62
#define MACHINE_VFP_BIN_866            63
#define RUSSIAN_VFP_CI_AS_866          64
#define CZECH_VFP_CI_AS_895            65
#define GENERAL_VFP_CI_AS_895          66
#define MACHINE_VFP_BIN_895            67
#define SLOVAK_VFP_CI_AS_895           68
#define DANISH_ADS_CS_AS_1252          69
#define DUTCH_ADS_CS_AS_1252           70
#define ENGL_AMER_ADS_CS_AS_1252       71
#define ENGL_CAN_ADS_CS_AS_1252        72
#define ENGL_UK_ADS_CS_AS_1252         73
#define FINNISH_ADS_CS_AS_1252         74
#define FRENCH_ADS_CS_AS_1252          75
#define FRENCH_CAN_ADS_CS_AS_1252      76
#define GERMAN_ADS_CS_AS_1252          77
#define ICELANDIC_ADS_CS_AS_1252       78
#define ITALIAN_ADS_CS_AS_1252         79
#define NORWEGIAN_ADS_CS_AS_1252       80
#define PORTUGUESE_ADS_CS_AS_1252      81
#define SPANISH_ADS_CS_AS_1252         82
#define SPAN_MOD_ADS_CS_AS_1252        83
#define SWEDISH_ADS_CS_AS_1252         84
#define RUSSIAN_ADS_CS_AS_1251         85
#define ASCII_ADS_CS_AS_1252           86
#define TURKISH_ADS_CS_AS_1254         87
#define POLISH_ADS_CS_AS_1250          88
#define BALTIC_ADS_CS_AS_1257          89
#define UKRAINIAN_ADS_CS_AS_1251       90
#define DUDEN_DE_ADS_CS_AS_1252        91
#define USA_ADS_CS_AS_437              92
#define DANISH_ADS_CS_AS_865           93
#define DUTCH_ADS_CS_AS_850            94
#define FINNISH_ADS_CS_AS_865          95
#define FRENCH_ADS_CS_AS_863           96
#define GERMAN_ADS_CS_AS_850           97
#define GREEK437_ADS_CS_AS_437         98
#define GREEK851_ADS_CS_AS_851         99
#define ICELD850_ADS_CS_AS_850         100
#define ICELD861_ADS_CS_AS_861         101
#define ITALIAN_ADS_CS_AS_850          102
#define NORWEGN_ADS_CS_AS_865          103
#define PORTUGUE_ADS_CS_AS_860         104
#define SPANISH_ADS_CS_AS_852          105
#define SWEDISH_ADS_CS_AS_865          106
#define MAZOVIA_ADS_CS_AS_852          107
#define PC_LATIN_ADS_CS_AS_852         108
#define ISOLATIN_ADS_CS_AS_850         109
#define RUSSIAN_ADS_CS_AS_866          110
#define NTXCZ852_ADS_CS_AS_852         111
#define NTXCZ895_ADS_CS_AS_895         112
#define NTXSL852_ADS_CS_AS_852         113
#define NTXSL895_ADS_CS_AS_895         114
#define NTXHU852_ADS_CS_AS_852         115
#define NTXPL852_ADS_CS_AS_852         116
#define TURKISH_ADS_CS_AS_857          117
#define BOSNIAN_ADS_CS_AS_775          118

#define ADS_MAX_CHAR_SETS              118


/* rights checking options */
#define ADS_CHECKRIGHTS          1
#define ADS_IGNORERIGHTS         2

/* options for connecting to Advantage servers - can be ORed together */
#define ADS_INC_USERCOUNT           0x00000001
#define ADS_STORED_PROC_CONN        0x00000002
#define ADS_COMPRESS_ALWAYS         0x00000004
#define ADS_COMPRESS_NEVER          0x00000008
#define ADS_COMPRESS_INTERNET       0x0000000C
#define ADS_REPLICATION_CONNECTION  0x00000010
#define ADS_UDP_IP_CONNECTION       0x00000020
#define ADS_IPX_CONNECTION          0x00000040
#define ADS_TCP_IP_CONNECTION       0x00000080
#define ADS_TCP_IP_V6_CONNECTION    0x00000100
#define ADS_NOTIFICATION_CONNECTION 0x00000200


/* options for opening/create tables - can be ORed together */
#define ADS_EXCLUSIVE                     0x00000001
#define ADS_READONLY                      0x00000002
#define ADS_SHARED                        0x00000004
#define ADS_CLIPPER_MEMOS                 0x00000008
#define ADS_TABLE_PERM_READ               0x00000010
#define ADS_TABLE_PERM_UPDATE             0x00000020
#define ADS_TABLE_PERM_INSERT             0x00000040
#define ADS_TABLE_PERM_DELETE             0x00000080
#define ADS_REINDEX_ON_COLLATION_MISMATCH 0x00000100
#define ADS_IGNORE_COLLATION_MISMATCH     0x00000200
#define ADS_FREE_TABLE                    0x00001000  // Mutually exclusive with ADS_DICTIONARY_BOUND_TABLE
#define ADS_TEMP_TABLE                    0x00002000  // Mutually exclusive with ADS_DICTIONARY_BOUND_TABLE
#define ADS_DICTIONARY_BOUND_TABLE        0x00004000  // Mutually exclusive with ADS_FREE_TABLE or ADS_TEMP_TABLE
/* When adding entry in here, make sure the corresponding
 * entry is added in aceunpub.h and ensure that there is no
 * conflict.
 */





/* Options for creating indexes - can be ORed together */
#define ADS_ASCENDING            0x00000000
#define ADS_UNIQUE               0x00000001
#define ADS_COMPOUND             0x00000002
#define ADS_CUSTOM               0x00000004
#define ADS_DESCENDING           0x00000008
#define ADS_USER_DEFINED         0x00000010
// 020 - 200 FTS index options below
#define ADS_NOT_AUTO_OPEN        0x00000400     // Don't make this an auto open index in data dictionary
#define ADS_CANDIDATE            0x00000800     // true unique CDX index (equivalent to ADS_UNIQUE for ADIs)

/* Options specifically for FTS indexes */
#define ADS_FTS_INDEX            0x00000020      // This is implied for AdsCreateFTSIndex
#define ADS_FTS_FIXED            0x00000040      // Do not maintain the index with record updates
#define ADS_FTS_CASE_SENSITIVE   0x00000080      // Make the index case sensitive
#define ADS_FTS_KEEP_SCORE       0x00000100      // Track word counts in the index for faster SCORE()
#define ADS_FTS_PROTECT_NUMBERS  0x00000200      // Don't break numbers on commas and periods



/* Options for returning string values */
#define ADS_NONE                 0x0000
#define ADS_LTRIM                0x0001
#define ADS_RTRIM                0x0002
#define ADS_TRIM                 0x0003

/* this is for passing null terminated strings */
#define ADS_NTS    ( ( UNSIGNED16 ) -1 )

/* locking compatibility */
#define ADS_COMPATIBLE_LOCKING   0
#define ADS_PROPRIETARY_LOCKING  1

/* settings for seeks */
#define ADS_SOFTSEEK             0x0001
#define ADS_HARDSEEK             0x0002
#define ADS_SEEKGT               0x0004

/* data types for seeks (and scopes) */
#define ADS_RAWKEY               1   /* no conversion performed on given data */
#define ADS_STRINGKEY            2   /* data given as a string */
#define ADS_DOUBLEKEY            4   /* data is a pointer to 8 byte double */


/* For retrieving scope settings */
#define ADS_TOP                  1
#define ADS_BOTTOM               2

/* for calls that can optionally use filters */
#define ADS_RESPECTFILTERS       0x0001
#define ADS_IGNOREFILTERS        0x0002
#define ADS_RESPECTSCOPES        0x0003
/*
 * This value is only used with GetRecordCount:  It can be ORed in with the
 * ignore filter value to force a read from the table header to get the most
 * current record count.
 */
#define ADS_REFRESHCOUNT         0x0004

/* Server type constants */
#define ADS_LOCAL_SERVER         0x0001
#define ADS_REMOTE_SERVER        0x0002
#define ADS_AIS_SERVER           0x0004

/* ACE Handle types */
#define ADS_CONNECTION            1
#define ADS_TABLE                 2
#define ADS_INDEX_ORDER           3
#define ADS_STATEMENT             4
#define ADS_CURSOR                5
#define ADS_DATABASE_CONNECTION   6
/* #define ADS_SYS_ADMIN_CONNECTION  7   obsolete */
#define ADS_FTS_INDEX_ORDER       8


/* ACE Cursor ReadOnly settings */
#define ADS_CURSOR_READONLY      1
#define ADS_CURSOR_READWRITE     2

/* ACE Cursor Constrain settings */
#define ADS_CONSTRAIN            1
#define ADS_NO_CONSTRAIN         2

/* Select Field Read settings */
#define ADS_READ_ALL_COLUMNS     1
#define ADS_READ_SELECT_COLUMNS  2

/* Data dictionary new contraint property validation options */
#define ADS_NO_VALIDATE           0  /* Do not validate records against the new constraint */
#define ADS_VALIDATE_NO_SAVE      1  /* Delete record not meeting the constraint from the table, no save */
#define ADS_VALIDATE_WRITE_FAIL   2  /* Validate the records against the new constraint and overwrite
                                      * the fail table with records not meeting the constraint. */
#define ADS_VALIDATE_APPEND_FAIL  3  /* Validate the records against the new constraint and append
                                      * the failed records into the fail table */
#define ADS_VALIDATE_RETURN_ERROR 4  /* Validate the records against the new constraint and return
                                      * error if there is any record not meeting the constraint */

/* Possible result values from AdsCompareBookmarks. */
#define ADS_CMP_LESS    -1
#define ADS_CMP_EQUAL    0
#define ADS_CMP_GREATER  1

/* Property values for the AdsGetConnectionProperty API */
#define ADS_CONNECTIONPROP_USERNAME    0
#define ADS_CONNECTIONPROP_PASSWORD    1

/* Options for the AdsGetRecordCRC API */
#define ADS_CRC_LOCALLY          1
#define ADS_CRC_IGNOREMEMOPAGES  2

/* Options for notification events */
#define ADS_EVENT_ASYNC          1

/* Options for the AdsCancelUpdate90 API */
#define ADS_PRESERVE_ERR      0x0001

/* Success return code */
#define AE_SUCCESS                      0

/* Error codes */
#define AE_ALLOCATION_FAILED            5001
#define AE_COMM_MISMATCH                5002
#define AE_DATA_TOO_LONG                5003
#define AE_FILE_NOT_FOUND               5004
#define AE_INSUFFICIENT_BUFFER          5005
#define AE_INVALID_BOOKMARK             5006
#define AE_INVALID_CALLBACK             5007
#define AE_INVALID_CENTURY              5008
#define AE_INVALID_DATEFORMAT           5009
#define AE_INVALID_DECIMALS             5010
#define AE_INVALID_EXPRESSION           5011
#define AE_INVALID_FIELDDEF             5012
#define AE_INVALID_FILTER_OPTION        5013
#define AE_INVALID_INDEX_HANDLE         5014
#define AE_INVALID_INDEX_NAME           5015
#define AE_INVALID_INDEX_ORDER_NAME     5016
#define AE_INVALID_INDEX_TYPE           5017
#define AE_INVALID_HANDLE               5018
#define AE_INVALID_OPTION               5019
#define AE_INVALID_PATH                 5020
#define AE_INVALID_POINTER              5021
#define AE_INVALID_RECORD_NUMBER        5022
#define AE_INVALID_TABLE_HANDLE         5023
#define AE_INVALID_CONNECTION_HANDLE    5024
#define AE_INVALID_TABLETYPE            5025
#define AE_INVALID_WORKAREA             5026
#define AE_INVALID_CHARSETTYPE          5027
#define AE_INVALID_LOCKTYPE             5028
#define AE_INVALID_RIGHTSOPTION         5029
#define AE_INVALID_FIELDNUMBER          5030
#define AE_INVALID_KEY_LENGTH           5031
#define AE_INVALID_FIELDNAME            5032
#define AE_NO_DRIVE_CONNECTION          5033
#define AE_FILE_NOT_ON_SERVER           5034
#define AE_LOCK_FAILED                  5035
#define AE_NO_CONNECTION                5036
#define AE_NO_FILTER                    5037
#define AE_NO_SCOPE                     5038
#define AE_NO_TABLE                     5039
#define AE_NO_WORKAREA                  5040
#define AE_NOT_FOUND                    5041
#define AE_NOT_IMPLEMENTED              5042
#define AE_MAX_THREADS_EXCEEDED         5043
#define AE_START_THREAD_FAIL            5044
#define AE_TOO_MANY_INDEXES             5045
#define AE_TOO_MANY_TAGS                5046
#define AE_TRANS_OUT_OF_SEQUENCE        5047
#define AE_UNKNOWN_ERRCODE              5048
#define AE_UNSUPPORTED_LANGUAGE         5049
#define AE_NAME_TOO_LONG                5050
#define AE_DUPLICATE_ALIAS              5051
#define AE_TABLE_CLOSED_IN_TRANSACTION  5053
#define AE_PERMISSION_DENIED            5054
#define AE_STRING_NOT_FOUND             5055
#define AE_UNKNOWN_CHAR_SET             5056
#define AE_INVALID_OEM_CHAR_FILE        5057
#define AE_INVALID_MEMO_BLOCK_SIZE      5058
#define AE_NO_FILE_FOUND                5059
#define AE_NO_INF_LOCK                  5060
#define AE_INF_FILE_ERROR               5061
#define AE_RECORD_NOT_LOCKED            5062
#define AE_ILLEGAL_COMMAND_DURING_TRANS 5063
#define AE_TABLE_NOT_SHARED             5064
#define AE_INDEX_ALREADY_OPEN           5065
#define AE_INVALID_FIELD_TYPE           5066
#define AE_TABLE_NOT_EXCLUSIVE          5067
#define AE_NO_CURRENT_RECORD            5068
#define AE_PRECISION_LOST               5069
#define AE_INVALID_DATA_TYPE            5070
#define AE_DATA_TRUNCATED               5071
#define AE_TABLE_READONLY               5072
#define AE_INVALID_RECORD_LENGTH        5073
#define AE_NO_ERROR_MESSAGE             5074
#define AE_INDEX_SHARED                 5075
#define AE_INDEX_EXISTS                 5076
#define AE_CYCLIC_RELATION              5077
#define AE_INVALID_RELATION             5078
#define AE_INVALID_DAY                  5079
#define AE_INVALID_MONTH                5080
#define AE_CORRUPT_TABLE                5081
#define AE_INVALID_BINARY_OFFSET        5082
#define AE_BINARY_FILE_ERROR            5083
#define AE_INVALID_DELETED_BYTE_VALUE   5084
#define AE_NO_PENDING_UPDATE            5085
#define AE_PENDING_UPDATE               5086
#define AE_TABLE_NOT_LOCKED             5087
#define AE_CORRUPT_INDEX                5088
#define AE_AUTOOPEN_INDEX               5089
#define AE_SAME_TABLE                   5090
#define AE_INVALID_IMAGE                5091
#define AE_COLLATION_SEQUENCE_MISMATCH  5092
#define AE_INVALID_INDEX_ORDER          5093
#define AE_TABLE_CACHED                 5094
#define AE_INVALID_DATE                 5095
#define AE_ENCRYPTION_NOT_ENABLED       5096
#define AE_INVALID_PASSWORD             5097
#define AE_TABLE_ENCRYPTED              5098
#define AE_SERVER_MISMATCH              5099
#define AE_INVALID_USERNAME             5100
#define AE_INVALID_VALUE                5101
#define AE_INVALID_CONTINUE             5102
#define AE_UNRECOGNIZED_VERSION         5103
#define AE_RECORD_ENCRYPTED             5104
#define AE_UNRECOGNIZED_ENCRYPTION      5105
#define AE_INVALID_SQLSTATEMENT_HANDLE  5106
#define AE_INVALID_SQLCURSOR_HANDLE     5107
#define AE_NOT_PREPARED                 5108
#define AE_CURSOR_NOT_CLOSED            5109
#define AE_INVALID_SQL_PARAM_NUMBER     5110
#define AE_INVALID_SQL_PARAM_NAME       5111
#define AE_INVALID_COLUMN_NUMBER        5112
#define AE_INVALID_COLUMN_NAME          5113
#define AE_INVALID_READONLY_OPTION      5114
#define AE_IS_CURSOR_HANDLE             5115
#define AE_INDEX_EXPR_NOT_FOUND         5116
#define AE_NOT_DML                      5117
#define AE_INVALID_CONSTRAIN_TYPE       5118
#define AE_INVALID_CURSORHANDLE         5119
#define AE_OBSOLETE_FUNCTION            5120
#define AE_TADSDATASET_GENERAL          5121
#define AE_UDF_OVERWROTE_BUFFER         5122
#define AE_INDEX_UDF_NOT_SET            5123
#define AE_CONCURRENT_PROBLEM           5124
#define AE_INVALID_DICTIONARY_HANDLE    5125
#define AE_INVALID_PROPERTY_ID          5126
#define AE_INVALID_PROPERTY             5127
#define AE_DICTIONARY_ALREADY_EXISTS    5128
#define AE_INVALID_FIND_HANDLE          5129
#define AE_DD_REQUEST_NOT_COMPLETED     5130
#define AE_INVALID_OBJECT_ID            5131
#define AE_INVALID_OBJECT_NAME          5132
#define AE_INVALID_PROPERTY_LENGTH      5133
#define AE_INVALID_KEY_OPTIONS          5134
#define AE_CONSTRAINT_VALIDATION_ERROR  5135
#define AE_INVALID_OBJECT_TYPE          5136
#define AE_NO_OBJECT_FOUND              5137
#define AE_PROPERTY_NOT_SET             5138
#define AE_NO_PRIMARY_KEY_EXISTS        5139
#define AE_LOCAL_CONN_DISABLED          5140
#define AE_RI_RESTRICT                  5141
#define AE_RI_CASCADE                   5142
#define AE_RI_FAILED                    5143
#define AE_RI_CORRUPTED                 5144
#define AE_RI_UNDO_FAILED               5145
#define AE_RI_RULE_EXISTS               5146
#define AE_COLUMN_CANNOT_BE_NULL        5147
#define AE_MIN_CONSTRAINT_VIOLATION     5148
#define AE_MAX_CONSTRAINT_VIOLATION     5149
#define AE_RECORD_CONSTRAINT_VIOLATION  5150
#define AE_CANNOT_DELETE_TEMP_INDEX     5151
#define AE_RESTRUCTURE_FAILED           5152
#define AE_INVALID_STATEMENT            5153
#define AE_STORED_PROCEDURE_FAILED      5154
#define AE_INVALID_DICTIONARY_FILE      5155
#define AE_NOT_MEMBER_OF_GROUP          5156
#define AE_ALREADY_MEMBER_OF_GROUP      5157
#define AE_INVALID_OBJECT_RIGHT         5158
#define AE_INVALID_OBJECT_PERMISSION    5158    /* Note that this is same as above. The word
                                                 * permission is more commonly used.
                                                 */
#define AE_CANNOT_OPEN_DATABASE_TABLE   5159
#define AE_INVALID_CONSTRAINT           5160
#define AE_NOT_ADMINISTRATOR            5161
#define AE_NO_TABLE_ENCRYPTION_PASSWORD 5162
#define AE_TABLE_NOT_ENCRYPTED          5163
#define AE_INVALID_ENCRYPTION_VERSION   5164
#define AE_NO_STORED_PROC_EXEC_RIGHTS   5165
#define AE_DD_UNSUPPORTED_DEPLOYMENT    5166
#define AE_INFO_AUTO_CREATION_OCCURRED  5168
#define AE_INFO_COPY_MADE_BY_CLIENT     5169
#define AE_DATABASE_REQUIRES_NEW_SERVER 5170
#define AE_COLUMN_PERMISSION_DENIED     5171
#define AE_DATABASE_REQUIRES_NEW_CLIENT 5172
#define AE_INVALID_LINK_NUMBER          5173
#define AE_LINK_ACTIVATION_FAILED       5174
#define AE_INDEX_COLLATION_MISMATCH     5175
#define AE_ILLEGAL_USER_OPERATION       5176
#define AE_TRIGGER_FAILED               5177
#define AE_NO_ASA_FUNCTION_FOUND        5178
#define AE_VALUE_OVERFLOW               5179
#define AE_UNRECOGNIZED_FTS_VERSION     5180
#define AE_TRIG_CREATION_FAILED         5181
#define AE_MEMTABLE_SIZE_EXCEEDED       5182
#define AE_OUTDATED_CLIENT_VERSION      5183
#define AE_FREE_TABLE                   5184
#define AE_LOCAL_CONN_RESTRICTED        5185
#define AE_OLD_RECORD                   5186
#define AE_QUERY_NOT_ACTIVE             5187
#define AE_KEY_EXCEEDS_PAGE_SIZE        5188
#define AE_TABLE_FOUND                  5189
#define AE_TABLE_NOT_FOUND              5190
#define AE_LOCK_OBJECT                  5191
#define AE_INVALID_REPLICATION_IDENT    5192
#define AE_ILLEGAL_COMMAND_DURING_BACKUP 5193
#define AE_NO_MEMO_FILE                 5194
#define AE_SUBSCRIPTION_QUEUE_NOT_EMPTY 5195
#define AE_UNABLE_TO_DISABLE_TRIGGERS   5196
#define AE_UNABLE_TO_ENABLE_TRIGGERS    5197
#define AE_BACKUP                       5198
#define AE_FREETABLEFAILED              5199
#define AE_BLURRY_SNAPSHOT              5200
#define AE_INVALID_VERTICAL_FILTER      5201
#define AE_INVALID_USE_OF_HANDLE_IN_AEP 5202
#define AE_COLLATION_NOT_RECOGNIZED     5203
#define AE_INVALID_COLLATION            5204
#define AE_NOT_VFP_NULLABLE_FIELD       5205
#define AE_NOT_VFP_VARIABLE_FIELD       5206
#define AE_ILLEGAL_EVENT_COMMAND        5207
#define AE_KEY_CANNOT_BE_NULL           5208
#define AE_COLLATIONS_DO_NOT_MATCH      5209
#define AE_INVALID_APPID                5210

/* Supported file types */
#define ADS_DATABASE_TABLE       ADS_DEFAULT
#define ADS_NTX                  1
#define ADS_CDX                  2
#define ADS_ADT                  3
#define ADS_VFP                  4


/* for retrieving file names of tables */
#define ADS_BASENAME             1
#define ADS_BASENAMEANDEXT       2
#define ADS_FULLPATHNAME         3
#define ADS_DATADICTIONARY_NAME  4
#define ADS_TABLE_OPEN_NAME      5


/* Advantage Optimized Filter (AOF) optimization levels */
#define ADS_OPTIMIZED_FULL       1
#define ADS_OPTIMIZED_PART       2
#define ADS_OPTIMIZED_NONE       3

/* Advantage Optimized Filter (AOF) options */
#define ADS_DYNAMIC_AOF          0x00000000  /* default */
#define ADS_RESOLVE_IMMEDIATE    0x00000001
#define ADS_RESOLVE_DYNAMIC      0x00000002
#define ADS_KEYSET_AOF           0x00000004
#define ADS_FIXED_AOF            0x00000008
#define ADS_KEEP_AOF_PLAN        0x00000010

/* Advantage Optimized Filter (AOF) customization options */
#define ADS_AOF_ADD_RECORD       1
#define ADS_AOF_REMOVE_RECORD    2
#define ADS_AOF_TOGGLE_RECORD    3

/* Stored procedure or trigger type */
#define ADS_STORED_PROC          0x00000001
#define ADS_COMSTORED_PROC       0x00000002  /* means we know for sure this is a com
                                              * aep. Before 7.1 we couldn't distinguish. */
#define ADS_SCRIPT_PROC          0x00000004  /* Stored procedure written in SQL script */


/* some maximum values used by the client */
/* NOTE:  constants meant for string length exclude space for null terminator */
#define ADS_MAX_DATEMASK         12
#define ADS_MAX_ERROR_LEN        600
#define ADS_MAX_INDEX_EXPR_LEN   510   /* this is only accurate for index expressions */
#define ADS_MAX_KEY_LENGTH       4082  /* maximum key value length.  This is the max key length
                                        * of ADI indexes.  Max CDX key length is 240.  Max
                                        * NTX key length is 256 */
#define ADS_MAX_FIELD_NAME       128
#define ADS_MAX_DBF_FIELD_NAME   10    /* maximum length of field name in a DBF */
#define ADS_MAX_INDEXES          15    /* physical index files, NOT index orders */
#define ADS_MAX_PATH             260
#define ADS_MAX_TABLE_NAME       255   /* long file name */
#define ADS_MAX_TAG_NAME         128
#define ADS_MAX_TAGS             256   /* maximum for CDX/ADI file */
#define ADS_MAX_OBJECT_NAME      200   /* maximum length of DD object name */
#define ADS_MAX_TABLE_AND_PATH   ADS_MAX_TABLE_NAME + ADS_MAX_PATH

/*
 * Valid range of page sizes for ADI indexes.  The default page size is 512
 * bytes.  Before using another page size, please read the section titled
 * "Index Page Size" in the Advantage Client Engine help file (ace.hlp)
 */
#define ADS_MIN_ADI_PAGESIZE     512
#define ADS_MAX_ADI_PAGESIZE     8192


/* data types */
#define ADS_TYPE_UNKNOWN         0
#define ADS_LOGICAL              1     /* 1 byte logical value */
#define ADS_NUMERIC              2     /* DBF character style numeric */
#define ADS_DATE                 3     /* Date field.  With ADS_NTX, ADS_CDX, and
                                        * ADS_VFP< this is an 8 byte field of the form
                                        * CCYYMMDD.  With ADS_ADT, it is a 4 byte
                                        * Julian date. */
#define ADS_STRING               4     /* Character data */
#define ADS_MEMO                 5     /* Variable length character data */

/* the following are extended data types */
#define ADS_BINARY               6     /* BLOB - any data */
#define ADS_IMAGE                7     /* BLOB - bitmap */
#define ADS_VARCHAR              8     /* variable length character field */
#define ADS_COMPACTDATE          9     /* DBF date represented with 3 bytes */
#define ADS_DOUBLE               10    /* IEEE 8 byte floating point */
#define ADS_INTEGER              11    /* IEEE 4 byte signed long integer */

/* the following are supported with the ADT format */
#define ADS_SHORTINT             12    /* IEEE 2 byte signed short integer */
#define ADS_TIME                 13    /* 4 byte long integer representing
                                        * milliseconds since midnight */
#define ADS_TIMESTAMP            14    /* 8 bytes.  High order 4 bytes are a
                                        * long integer representing Julian date.
                                        * Low order 4 bytes are a long integer
                                        * representing milliseconds since
                                        * midnight */
#define ADS_AUTOINC              15    /* 4 byte auto-increment value */
#define ADS_RAW                  16    /* Untranslated data */
#define ADS_CURDOUBLE            17    /* IEEE 8 byte floating point currency */
#define ADS_MONEY                18    /* 8 byte, 4 implied decimal Currency Field */
#define ADS_LONGLONG             19    /* 8 byte integer */
#define ADS_CISTRING             20    /* CaSe INSensiTIVE character data */
#define ADS_ROWVERSION           21    /* 8 byte integer, incremented for every update, unique to entire table */
#define ADS_MODTIME              22    /* 8 byte timestamp, updated when record is updated */
#define ADS_VARCHAR_FOX          23    /* Visual FoxPro varchar field */
#define ADS_VARBINARY_FOX        24    /* Visual FoxPro varbinary field */
#define ADS_SYSTEM_FIELD         25    /* For internal usage */

/*
 * supported User Defined Function types to be used with AdsRegisterUDF
 */
#define ADS_INDEX_UDF            1

/*
 * Constant for AdsMgGetConfigInfo
 */
#define ADS_MAX_CFG_PATH         256

/*
 * Constants for AdsMgGetServerType
 * Note ADS_MGMT_NETWARE_SERVER remains for backwards compatibility only
 */
#define ADS_MGMT_NETWARE_SERVER           1
#define ADS_MGMT_NETWARE4_OR_OLDER_SERVER 1
#define ADS_MGMT_NT_SERVER                2
#define ADS_MGMT_LOCAL_SERVER             3
#define ADS_MGMT_WIN9X_SERVER             4
#define ADS_MGMT_NETWARE5_OR_NEWER_SERVER 5
#define ADS_MGMT_LINUX_SERVER             6
#define ADS_MGMT_NT_SERVER_64_BIT         7
#define ADS_MGMT_LINUX_SERVER_64_BIT      8

/*
 * Constants for AdsMgGetLockOwner
 */
#define ADS_MGMT_NO_LOCK         1
#define ADS_MGMT_RECORD_LOCK     2
#define ADS_MGMT_FILE_LOCK       3

/*
 * Constants for MgGetInstallInfo
 */
#define ADS_REG_OWNER_LEN        36
#define ADS_REVISION_LEN         16
#define ADS_INST_DATE_LEN        16
#define ADS_OEM_CHAR_NAME_LEN    16
#define ADS_ANSI_CHAR_NAME_LEN   16
#define ADS_SERIAL_NUM_LEN       16

/*
 * Constants for MgGetOpenTables
 */
#define ADS_MGMT_MAX_PATH              260
#define ADS_MGMT_PROPRIETARY_LOCKING   1
#define ADS_MGMT_CDX_LOCKING           2
#define ADS_MGMT_NTX_LOCKING           3
#define ADS_MGMT_ADT_LOCKING           4
#define ADS_MGMT_COMIX_LOCKING         5

#define ADS_MAX_USER_NAME        50

#define ADS_MAX_ADDRESS_SIZE     30
#define ADS_MAX_MGMT_APPID_SIZE  70

/*
 * Management API structures
 */
typedef struct
   {
   double      dPercentCheckSums;   /* % of pkts with checksum failures */
   UNSIGNED32  ulTotalPackets;      /* Total packets received           */
   UNSIGNED32  ulRcvPktOutOfSeq;    /* Receive packets out of sequence  */
   UNSIGNED32  ulNotLoggedIn;       /* Packet owner not logged in       */
   UNSIGNED32  ulRcvReqOutOfSeq;    /* Receive requests out of sequence */
   UNSIGNED32  ulCheckSumFailures;  /* Checksum failures                */
   UNSIGNED32  ulDisconnectedUsers; /* Server initiated disconnects     */
   UNSIGNED32  ulPartialConnects;   /* Removed partial connections      */
   UNSIGNED32  ulInvalidPackets;    /* Rcvd invalid packets (NT only)   */
   UNSIGNED32  ulRecvFromErrors;    /* RecvFrom failed (NT only)        */
   UNSIGNED32  ulSendToErrors;      /* SendTo failed (NT only)          */
   } ADS_MGMT_COMM_STATS;

typedef struct
   {
   UNSIGNED32  ulNumConnections;          /* number connections            */
   UNSIGNED32  ulNumWorkAreas;            /* number work areas             */
   UNSIGNED32  ulNumTables;               /* number tables                 */
   UNSIGNED32  ulNumIndexes;              /* number indexes                */
   UNSIGNED32  ulNumLocks;                /* number locks                  */
   UNSIGNED32  ulUserBufferSize;          /* user buffer                   */
   UNSIGNED32  ulStatDumpInterval;        /* statistics dump interval      */
   UNSIGNED32  ulErrorLogMax;             /* max size of error log         */
   UNSIGNED32  ulNumTPSHeaderElems;       /* number TPS header elems       */
   UNSIGNED32  ulNumTPSVisibilityElems;   /* number TPS vis elems          */
   UNSIGNED32  ulNumTPSMemoTransElems;    /* number TPS memo elems         */
   UNSIGNED16  usNumReceiveECBs;          /* number rcv ECBs (NLM only)    */
   UNSIGNED16  usNumSendECBs;             /* number send ECBs (NLM only)   */
   UNSIGNED16  usNumBurstPackets;         /* number packets per burst      */
   UNSIGNED16  usNumWorkerThreads;        /* number worker threads         */
   UNSIGNED32  ulSortBuffSize;            /* index sort buffer size        */
   UNSIGNED8   aucErrorLog[ADS_MAX_CFG_PATH];    /* error log path         */
   UNSIGNED8   aucSemaphore[ADS_MAX_CFG_PATH];   /* semaphore file path    */
   UNSIGNED8   aucTransaction[ADS_MAX_CFG_PATH]; /* TPS log file path      */

   UNSIGNED8   ucReserved3;               /* reserved                      */
   UNSIGNED8   ucReserved4;               /* reserved                      */
   UNSIGNED16  usSendIPPort;              /* NT Service IP send port #     */
   UNSIGNED16  usReceiveIPPort;           /* NT Service IP rcv port #      */
   UNSIGNED8   ucUseIPProtocol;           /* Win9x only. Which protocol to use */
   UNSIGNED8   ucFlushEveryUpdate;        /* Win9x specific                */

   UNSIGNED32  ulGhostTimeout;            /* Diconnection time for partial connections */
   UNSIGNED32  ulFlushFrequency;          /* For local server only         */

   UNSIGNED32  ulKeepAliveTimeOut;     /* When not using semaophore files. In milliseconds */
   UNSIGNED8   ucDisplayNWLoginNames;  /* Display connections using user names. */
   UNSIGNED8   ucUseSemaphoreFiles;    /* Whether or not to use semaphore files */
   UNSIGNED8   ucUseDynamicAOFs;
   UNSIGNED8   ucUseInternet;          /* 0 if an Internet port is not specified. */

   UNSIGNED16  usInternetPort;         /* Internet Port */
   UNSIGNED16  usMaxConnFailures;      /* Maximum Internet connection failures allowed. */
   UNSIGNED32  ulInternetKeepAlive;    /* In Milliseconds */

   UNSIGNED16  usCompressionLevel;     /* Compression option at server.  ADS_COMPRESS_NEVER,
                                        * ADS_COMPRESS_INTERNET, or ADS_COMPRESS_ALWAYS */
   UNSIGNED32  ulNumQueries;           /* number of queries */
   UNSIGNED16  usReserved5;            /* reserved */

   } ADS_MGMT_CONFIG_PARAMS;

typedef struct
   {
   UNSIGNED32  ulTotalConfigMem;          /* Total mem taken by cfg params */
   UNSIGNED32  ulConnectionMem;           /* memory taken by connections   */
   UNSIGNED32  ulWorkAreaMem;             /* memory taken by work areas    */
   UNSIGNED32  ulTableMem;                /* memory taken by tables        */
   UNSIGNED32  ulIndexMem;                /* memory taken by indexes       */
   UNSIGNED32  ulLockMem;                 /* memory taken by locks         */
   UNSIGNED32  ulUserBufferMem;           /* memory taken by user buffer   */
   UNSIGNED32  ulTPSHeaderElemMem;        /* memory taken by TPS hdr elems */
   UNSIGNED32  ulTPSVisibilityElemMem;    /* memory taken by TPS vis elems */
   UNSIGNED32  ulTPSMemoTransElemMem;     /* mem taken by TPS memo elems   */
   UNSIGNED32  ulReceiveEcbMem;           /* mem taken by rcv ECBs (NLM)   */
   UNSIGNED32  ulSendEcbMem;              /* mem taken by send ECBs (NLM)  */
   UNSIGNED32  ulWorkerThreadMem;         /* mem taken by worker threads   */
   UNSIGNED32  ulQueryMem;                /* mem taken by queries          */
   } ADS_MGMT_CONFIG_MEMORY;

typedef struct
   {
   UNSIGNED32  ulUserOption;                            /* User option purchased*/
   UNSIGNED8   aucRegisteredOwner[ADS_REG_OWNER_LEN];   /* Registered owner     */
   UNSIGNED8   aucVersionStr[ADS_REVISION_LEN];         /* Advantage version    */
   UNSIGNED8   aucInstallDate[ADS_INST_DATE_LEN];       /* Install date string  */
   UNSIGNED8   aucOemCharName[ADS_OEM_CHAR_NAME_LEN];   /* OEM char language    */
   UNSIGNED8   aucAnsiCharName[ADS_ANSI_CHAR_NAME_LEN]; /* ANSI char language   */
   UNSIGNED8   aucEvalExpireDate[ADS_INST_DATE_LEN];    /* Eval expiration date */
   UNSIGNED8   aucSerialNumber[ADS_SERIAL_NUM_LEN];     /* Serial number string */
   } ADS_MGMT_INSTALL_INFO;

typedef struct
   {
   UNSIGNED16  usDays;        /* Number of days server has been up    */
   UNSIGNED16  usHours;       /* Number of hours server has been up   */
   UNSIGNED16  usMinutes;     /* Number of minutes server has been up */
   UNSIGNED16  usSeconds;     /* Number of seconds server has been up */
   } ADS_MGMT_UPTIME_INFO;

typedef struct
   {
   UNSIGNED32  ulInUse;       /* Number of items in use        */
   UNSIGNED32  ulMaxUsed;     /* Max number of items ever used */
   UNSIGNED32  ulRejected;    /* Number of items rejected      */
   } ADS_MGMT_USAGE_INFO;

typedef struct
   {
   UNSIGNED32           ulOperations;     /* Number operations since started */
   UNSIGNED32           ulLoggedErrors;   /* Number logged errors            */
   ADS_MGMT_UPTIME_INFO stUpTime;         /* Length of time ADS has been up  */
   ADS_MGMT_USAGE_INFO  stUsers;          /* Users in use, max, rejected     */
   ADS_MGMT_USAGE_INFO  stConnections;    /* Conns in use, max, rejected     */
   ADS_MGMT_USAGE_INFO  stWorkAreas;      /* WAs in use, max, rejected       */
   ADS_MGMT_USAGE_INFO  stTables;         /* Tables in use, max, rejected    */
   ADS_MGMT_USAGE_INFO  stIndexes;        /* Indexes in use, max, rejected   */
   ADS_MGMT_USAGE_INFO  stLocks;          /* Locks in use, max, rejected     */
   ADS_MGMT_USAGE_INFO  stTpsHeaderElems; /* TPS header elems in use, max    */
   ADS_MGMT_USAGE_INFO  stTpsVisElems;    /* TPS vis elems in use, max       */
   ADS_MGMT_USAGE_INFO  stTpsMemoElems;   /* TPS memo elems in use, max      */
   ADS_MGMT_USAGE_INFO  stWorkerThreads;  /* Worker threads in use, max      */
   ADS_MGMT_USAGE_INFO  stQueries;        /* Queries in use, max, rejected   */
   } ADS_MGMT_ACTIVITY_INFO;

typedef struct
   {
   UNSIGNED8  aucUserName[ADS_MAX_USER_NAME]; /* Name of connected user    */
   UNSIGNED16 usConnNumber;                   /* NetWare conn # (NLM only) */
   UNSIGNED8  aucAuthUserName[ADS_MAX_USER_NAME]; /* Dictionary user name  */
   UNSIGNED8  aucAddress[ADS_MAX_ADDRESS_SIZE]; /* Network address of user */
   UNSIGNED8  aucOSUserLoginName[ADS_MAX_USER_NAME]; /* OS user login name */
   UNSIGNED8  aucTSAddress[ADS_MAX_ADDRESS_SIZE]; /* Terminal Services client IP Address */
   UNSIGNED8  aucApplicationID[ADS_MAX_MGMT_APPID_SIZE]; /* application id */
   } ADS_MGMT_USER_INFO;

typedef struct
   {
   UNSIGNED8  aucTableName[ADS_MGMT_MAX_PATH]; /* Fully qualified table name */
   UNSIGNED16 usLockType;                      /* Advantage locking mode     */
   } ADS_MGMT_TABLE_INFO;

typedef struct
   {
   UNSIGNED8  aucIndexName[ADS_MGMT_MAX_PATH]; /* Fully qualified table name */
   } ADS_MGMT_INDEX_INFO;

typedef struct
   {
   UNSIGNED32 ulRecordNumber;       /* Record number that is locked */
   } ADS_MGMT_RECORD_INFO;

typedef struct
   {
   UNSIGNED32 ulThreadNumber;                 /* Thread Number               */
   UNSIGNED16 usOpCode;                       /* Operation in progress       */
   UNSIGNED8  aucUserName[ADS_MAX_USER_NAME]; /* Name of user                */
   UNSIGNED16 usConnNumber;                   /* NetWare conn num (NLM only) */
   UNSIGNED16 usReserved1;                    /* Reserved                    */
   UNSIGNED8  aucOSUserLoginName[ADS_MAX_USER_NAME]; /* OS user login name   */
   } ADS_MGMT_THREAD_ACTIVITY;

/*
 * Data dictionary properties related constants and structure
 */

typedef struct _ADD_FIELD_DESC_
   {
   UNSIGNED16  usFieldType;
   UNSIGNED16  usFieldLength;
   UNSIGNED16  usFieldDecimal;
   } ADD_FIELD_DESC;


#define ADS_DD_PROPERTY_NOT_AVAIL   0xFFFF
#define ADS_DD_MAX_PROPERTY_LEN     0xFFFE
#define ADS_DD_MAX_OBJECT_NAME_LEN  200

#define ADS_DD_UNKNOWN_OBJECT            0
#define ADS_DD_TABLE_OBJECT              1
#define ADS_DD_RELATION_OBJECT           2
#define ADS_DD_INDEX_FILE_OBJECT         3
#define ADS_DD_FIELD_OBJECT              4
#define ADS_DD_COLUMN_OBJECT             4
#define ADS_DD_INDEX_OBJECT              5
#define ADS_DD_VIEW_OBJECT               6
#define ADS_DD_VIEW_OR_TABLE_OBJECT      7  /* Used in AdsFindFirst/NextTable */
#define ADS_DD_USER_OBJECT               8
#define ADS_DD_USER_GROUP_OBJECT         9
#define ADS_DD_PROCEDURE_OBJECT          10
#define ADS_DD_DATABASE_OBJECT           11
#define ADS_DD_LINK_OBJECT               12
#define ADS_DD_TABLE_VIEW_OR_LINK_OBJECT 13  /* Used in v6.2 AdsFindFirst/NextTable */
#define ADS_DD_TRIGGER_OBJECT            14
#define ADS_DD_PUBLICATION_OBJECT        15
#define ADS_DD_ARTICLE_OBJECT            16  /* the things (tables) that get published */
#define ADS_DD_SUBSCRIPTION_OBJECT       17  /* indicates where a publication goes */
#define ADS_DD_FUNCTION_OBJECT           18  /* User defined function */
#define ADS_DD_PACKAGE_OBJECT            19  /* function and stored procedure packages */
#define ADS_DD_QUALIFIED_TRIGGER_OBJ     20  /* Used in AdsDDFindFirst/NextObject */


/* Common properties numbers < 100 */
#define ADS_DD_COMMENT           1
#define ADS_DD_VERSION           2
#define ADS_DD_USER_DEFINED_PROP 3
#define ADS_DD_OBJECT_NAME       4
#define ADS_DD_TRIGGERS_DISABLED 5
#define ADS_DD_OBJECT_ID         6
#define ADS_DD_OPTIONS           7


/* Database properties between 100 and 199 */
#define ADS_DD_DEFAULT_TABLE_PATH            100
#define ADS_DD_ADMIN_PASSWORD                101
#define ADS_DD_TEMP_TABLE_PATH               102
#define ADS_DD_LOG_IN_REQUIRED               103
#define ADS_DD_VERIFY_ACCESS_RIGHTS          104
#define ADS_DD_ENCRYPT_TABLE_PASSWORD        105
#define ADS_DD_ENCRYPT_NEW_TABLE             106
#define ADS_DD_ENABLE_INTERNET               107
#define ADS_DD_INTERNET_SECURITY_LEVEL       108
#define ADS_DD_MAX_FAILED_ATTEMPTS           109
#define ADS_DD_ALLOW_ADSSYS_NET_ACCESS       110
#define ADS_DD_VERSION_MAJOR                 111  /* properties for customer dd version */
#define ADS_DD_VERSION_MINOR                 112
#define ADS_DD_LOGINS_DISABLED               113
#define ADS_DD_LOGINS_DISABLED_ERRSTR        114
#define ADS_DD_FTS_DELIMITERS                115
#define ADS_DD_FTS_NOISE                     116
#define ADS_DD_FTS_DROP_CHARS                117
#define ADS_DD_FTS_CONDITIONAL_CHARS         118
#define ADS_DD_ENCRYPTED                     119
#define ADS_DD_ENCRYPT_INDEXES               120
#define ADS_DD_QUERY_LOG_TABLE               121
#define ADS_DD_ENCRYPT_COMMUNICATION         122
#define ADS_DD_DEFAULT_TABLE_RELATIVE_PATH   123
#define ADS_DD_TEMP_TABLE_RELATIVE_PATH      124
#define ADS_DD_DISABLE_DLL_CACHING           125

/* Table properties between 200 and 299 */
#define ADS_DD_TABLE_VALIDATION_EXPR   200
#define ADS_DD_TABLE_VALIDATION_MSG    201
#define ADS_DD_TABLE_PRIMARY_KEY       202
#define ADS_DD_TABLE_AUTO_CREATE       203
#define ADS_DD_TABLE_TYPE              204
#define ADS_DD_TABLE_PATH              205
#define ADS_DD_TABLE_FIELD_COUNT       206
#define ADS_DD_TABLE_RI_GRAPH          207
#define ADS_DD_TABLE_OBJ_ID            208
#define ADS_DD_TABLE_RI_XY             209
#define ADS_DD_TABLE_IS_RI_PARENT      210
#define ADS_DD_TABLE_RELATIVE_PATH     211
#define ADS_DD_TABLE_CHAR_TYPE         212
#define ADS_DD_TABLE_DEFAULT_INDEX     213
#define ADS_DD_TABLE_ENCRYPTION        214
#define ADS_DD_TABLE_MEMO_BLOCK_SIZE   215
#define ADS_DD_TABLE_PERMISSION_LEVEL  216
#define ADS_DD_TABLE_TRIGGER_TYPES     217
#define ADS_DD_TABLE_TRIGGER_OPTIONS   218


/* Bit values for the ADS_DD_FIELD_OPTIONS property */
#define ADS_DD_FIELD_OPT_VFP_BINARY    0x00000001   /* field has NOCPTRANS option */
#define ADS_DD_FIELD_OPT_VFP_NULLABLE  0x00000002   /* field can be physicall set to NULL */

/* Field properties between 300 - 399 */
#define ADS_DD_FIELD_DEFAULT_VALUE     300
#define ADS_DD_FIELD_CAN_NULL          301
#define ADS_DD_FIELD_MIN_VALUE         302
#define ADS_DD_FIELD_MAX_VALUE         303
#define ADS_DD_FIELD_VALIDATION_MSG    304
#define ADS_DD_FIELD_DEFINITION        305
#define ADS_DD_FIELD_TYPE              306
#define ADS_DD_FIELD_LENGTH            307
#define ADS_DD_FIELD_DECIMAL           308
#define ADS_DD_FIELD_NUM               309
#define ADS_DD_FIELD_OPTIONS           310

/* Index tag properties between 400 - 499 */
#define ADS_DD_INDEX_FILE_NAME         400
#define ADS_DD_INDEX_EXPRESSION        401
#define ADS_DD_INDEX_CONDITION         402
#define ADS_DD_INDEX_OPTIONS           403
#define ADS_DD_INDEX_KEY_LENGTH        404
#define ADS_DD_INDEX_KEY_TYPE          405
#define ADS_DD_INDEX_FTS_MIN_LENGTH    406
#define ADS_DD_INDEX_FTS_DELIMITERS    407
#define ADS_DD_INDEX_FTS_NOISE         408
#define ADS_DD_INDEX_FTS_DROP_CHARS    409
#define ADS_DD_INDEX_FTS_CONDITIONAL_CHARS 410
#define ADS_DD_INDEX_COLLATION         411

/* RI properties between 500-599 */
#define ADS_DD_RI_PARENT_GRAPH         500
#define ADS_DD_RI_PRIMARY_TABLE        501
#define ADS_DD_RI_PRIMARY_INDEX        502
#define ADS_DD_RI_FOREIGN_TABLE        503
#define ADS_DD_RI_FOREIGN_INDEX        504
#define ADS_DD_RI_UPDATERULE           505
#define ADS_DD_RI_DELETERULE           506
#define ADS_DD_RI_NO_PKEY_ERROR        507
#define ADS_DD_RI_CASCADE_ERROR        508

/* User properties between 600-699 */
#define ADS_DD_USER_GROUP_NAME         600

/* View properties between 700-749 */
#define ADS_DD_VIEW_STMT               700
#define ADS_DD_VIEW_STMT_LEN           701
#define ADS_DD_VIEW_TRIGGER_TYPES      702
#define ADS_DD_VIEW_TRIGGER_OPTIONS    703

/* Stored procedure properties 800-899 */
#define ADS_DD_PROC_INPUT              800
#define ADS_DD_PROC_OUTPUT             801
#define ADS_DD_PROC_DLL_NAME           802
#define ADS_DD_PROC_DLL_FUNCTION_NAME  803
#define ADS_DD_PROC_INVOKE_OPTION      804
#define ADS_DD_PROC_SCRIPT             805

/* Index file properties 900-999 */
#define ADS_DD_INDEX_FILE_PATH          900
#define ADS_DD_INDEX_FILE_PAGESIZE      901
#define ADS_DD_INDEX_FILE_RELATIVE_PATH 902
#define ADS_DD_INDEX_FILE_TYPE          903

/*
 * Object rights properties 1001 - 1099 .  They can be used
 * with either user or user group objects.
 */
#define ADS_DD_TABLES_RIGHTS           1001
#define ADS_DD_VIEWS_RIGHTS            1002
#define ADS_DD_PROCS_RIGHTS            1003
#define ADS_DD_OBJECTS_RIGHTS          1004
#define ADS_DD_FREE_TABLES_RIGHTS      1005

/* User Properties 1101 - 1199 */
#define ADS_DD_USER_PASSWORD           1101
#define ADS_DD_USER_GROUP_MEMBERSHIP   1102
#define ADS_DD_USER_BAD_LOGINS         1103

/* User group Properties 1201 - 1299 */
/* None at this moment. */

/* Link properties 1301 - 1399 */
#define ADS_DD_LINK_PATH               1300
#define ADS_DD_LINK_OPTIONS            1301
#define ADS_DD_LINK_USERNAME           1302
#define ADS_DD_LINK_RELATIVE_PATH      1303

/* Trigger properties 1400 - 1499 */
#define ADS_DD_TRIG_TABLEID            1400
#define ADS_DD_TRIG_EVENT_TYPE         1401
#define ADS_DD_TRIG_TRIGGER_TYPE       1402
#define ADS_DD_TRIG_CONTAINER_TYPE     1403
#define ADS_DD_TRIG_CONTAINER          1404
#define ADS_DD_TRIG_FUNCTION_NAME      1405
#define ADS_DD_TRIG_PRIORITY           1406
#define ADS_DD_TRIG_OPTIONS            1407
#define ADS_DD_TRIG_TABLENAME          1408

/* Publication properties 1500 - 1599 */
#define ADS_DD_PUBLICATION_OPTIONS     1500

/* Publication article properties 1600 - 1699 */
#define ADS_DD_ARTICLE_FILTER             1600     // horizontal filter (optional)
#define ADS_DD_ARTICLE_ID_COLUMNS         1601     // columns that identify the target row
#define ADS_DD_ARTICLE_ID_COLUMN_NUMBERS  1602     // array of the field numbers
#define ADS_DD_ARTICLE_FILTER_SHORT       1603     // short version of the expression
#define ADS_DD_ARTICLE_INCLUDE_COLUMNS    1604     // Vertical filter (inclusion list)
#define ADS_DD_ARTICLE_EXCLUDE_COLUMNS    1605     // Vertical filter (exclusion list)
#define ADS_DD_ARTICLE_INC_COLUMN_NUMBERS 1606     // Retrieve column nums to replicate
#define ADS_DD_ARTICLE_INSERT_MERGE       1607     // Use SQL MERGE with INSERTs
#define ADS_DD_ARTICLE_UPDATE_MERGE       1608     // Use SQL MERGE with UPDATEs

/* Subscription article properties 1700 - 1799 */
#define ADS_DD_SUBSCR_PUBLICATION_NAME    1700    // Name of the publication (for reading)
#define ADS_DD_SUBSCR_TARGET              1701    // full path of target database
#define ADS_DD_SUBSCR_USERNAME            1702    // user name to use to connect to target
#define ADS_DD_SUBSCR_PASSWORD            1703    // password for connecting
#define ADS_DD_SUBSCR_FORWARD             1704    // boolean flag:  forward updates that came from a replication?
#define ADS_DD_SUBSCR_ENABLED             1705    // boolean flag:  Replication enabled on this subscription?
#define ADS_DD_SUBSCR_QUEUE_NAME          1706    // replication queue
#define ADS_DD_SUBSCR_OPTIONS             1707    // for future use
#define ADS_DD_SUBSCR_QUEUE_NAME_RELATIVE 1708    // replication queue relative to the DD
#define ADS_DD_SUBSCR_PAUSED              1709    // boolean flag:  Replication paused on this subscription?

/* AdsMgKillUser90 Constants */
#define ADS_PROPERTY_UNSPECIFIED   0x0000
#define ADS_DONT_KILL_APPID        0x0001

#define ADS_DD_LEVEL_0  0
#define ADS_DD_LEVEL_1  1
#define ADS_DD_LEVEL_2  2

/* Referential Integrity (RI) update and delete rules */
#define ADS_DD_RI_CASCADE       1
#define ADS_DD_RI_RESTRICT      2
#define ADS_DD_RI_SETNULL       3
#define ADS_DD_RI_SETDEFAULT    4

/* Default Field Value Options */
#define ADS_DD_DFV_UNKNOWN         1
#define ADS_DD_DFV_NONE            2
#define ADS_DD_DFV_VALUES_STORED   3

/* Supported permissions in the data dictionary */
#define ADS_PERMISSION_NONE            0x00000000
#define ADS_PERMISSION_READ            0x00000001
#define ADS_PERMISSION_UPDATE          0x00000002
#define ADS_PERMISSION_EXECUTE         0x00000004
#define ADS_PERMISSION_INHERIT         0x00000008
#define ADS_PERMISSION_INSERT          0x00000010
#define ADS_PERMISSION_DELETE          0x00000020
#define ADS_PERMISSION_LINK_ACCESS     0x00000040
#define ADS_PERMISSION_CREATE          0x00000080
#define ADS_PERMISSION_ALTER           0x00000100
#define ADS_PERMISSION_DROP            0x00000200
#define ADS_PERMISSION_WITH_GRANT      0x80000000
#define ADS_PERMISSION_ALL_WITH_GRANT  0x8FFFFFFF
#define ADS_PERMISSION_ALL             0xFFFFFFFF

/*
 * special code that can be used as the input to specify
 * which special permission to retrieve.
 */
#define ADS_GET_PERMISSIONS_WITH_GRANT          0x8000FFFF
#define ADS_GET_PERMISSIONS_CREATE              0xFFFF0080
#define ADS_GET_PERMISSIONS_CREATE_WITH_GRANT   0x8FFFFF8F


/* Link DD options */
#define ADS_LINK_GLOBAL             0x00000001
#define ADS_LINK_AUTH_ACTIVE_USER   0x00000002
#define ADS_LINK_PATH_IS_STATIC     0x00000004

/* Trigger event types */
#define ADS_TRIGEVENT_INSERT             1
#define ADS_TRIGEVENT_UPDATE             2
#define ADS_TRIGEVENT_DELETE             3

/* Trigger types */
#define ADS_TRIGTYPE_BEFORE         0x00000001
#define ADS_TRIGTYPE_INSTEADOF      0x00000002
#define ADS_TRIGTYPE_AFTER          0x00000004
#define ADS_TRIGTYPE_CONFLICTON     0x00000008

/* Trigger container types */
#define ADS_TRIG_WIN32DLL           1
#define ADS_TRIG_COM                2
#define ADS_TRIG_SCRIPT             3

/*
 * Trigger options, if changed or adding more please inspect code
 * in RemoveTriggerFromDictionary
 */
#define ADS_TRIGOPTIONS_NO_VALUES             0x00000000
#define ADS_TRIGOPTIONS_WANT_VALUES           0x00000001
#define ADS_TRIGOPTIONS_WANT_MEMOS_AND_BLOBS  0x00000002
#define ADS_TRIGOPTIONS_DEFAULT               0x00000003  /* default is to include vals and memos */
#define ADS_TRIGOPTIONS_NO_TRANSACTION        0x00000004  /* don't use implicit transactions */

/*
 * Table permission verification levels.
 * level 1 is all columns searchable, even those without permission.
 * level 2 is default. Permission to the column is required to search or filter on a column.
 * level 3 is most restricted. Only static SQL cursor is allowed.
 */
#define ADS_DD_TABLE_PERMISSION_LEVEL_1   1
#define ADS_DD_TABLE_PERMISSION_LEVEL_2   2
#define ADS_DD_TABLE_PERMISSION_LEVEL_3   3

/* AdsDDRenameObject options */
#define ADS_KEEP_TABLE_FILE_NAME        0x00000001


/* AdsDDCreateArticle options */
#define ADS_IDENTIFY_BY_PRIMARY         0x00000001
#define ADS_IDENTIFY_BY_ALL             0x00000002


/* AdsDDCreateSubscription options */
#define ADS_SUBSCR_QUEUE_IS_STATIC      0x00000001
#define ADS_SUBSCR_AIS_TARGET           0x00000002   // use AIS to connect to target
#define ADS_SUBSCR_IGNORE_FAILED_REP    0x00000004   // Delete failed replication updates from the queue
#define ADS_SUBSCR_LOG_FAILED_REP_DATA  0x00000008   // if set, show data of failed replication updates in
                                                     // the error log.


/* stored procedure functions must be of this type */
#ifdef WINAPI
typedef UNSIGNED32 (WINAPI *STORED_PROCEDURE_PTR)
(
   UNSIGNED32  ulConnectionID, // (I) value used to associate a user/connection
                               //     and can be used to track the state
   UNSIGNED8   *pucUserName,   // (I) the user name who invoked this procedure
   UNSIGNED8   *pucPassword,   // (I) the user's password in encrypted form
   UNSIGNED8   *pucProcName,   // (I) the stored procedure name
   UNSIGNED32  ulRecNum,       // (I) reserved for triggers
   UNSIGNED8   *pucTable1,     // (I) table one.  For Stored Proc this table
                               //     contains all input parameters.  For
                               //     triggers, it contains the original field
                               //     values if the trigger is an OnUpdate or
                               //     OnDelete
   UNSIGNED8   *pucTable2      // (I) table two.  For Stored Proc this table
                               //     is empty and the users function will
                               //     optionally add rows to it as output.
                               //     For triggers, it contains the new field
                               //     values if the trigger is an OnUpdate or
                               //     OnInsert
);


typedef UNSIGNED32 (WINAPI *STARTUP_PROCEDURE_PTR)
(
   UNSIGNED32  ulConnectionID, // (I) value used to associate a user/connection
                               //     and can be used to track the state
   UNSIGNED8   *pucUserName,   // (I) the user name who invoked this procedure
   UNSIGNED8   *pucPassword   // (I) the user's password in encrypted form
);


typedef UNSIGNED32 (WINAPI *SHUTDOWN_PROCEDURE_PTR)
(
   UNSIGNED32  ulConnectionID, // (I) value used to associate a user/connection
                               //     and can be used to track the state
   UNSIGNED8   *pucUserName,   // (I) the user name who invoked this procedure
   UNSIGNED8   *pucPassword    // (I) the user's password in encrypted form
);


typedef UNSIGNED32 (WINAPI *STORED_PROCEDURE2_PTR)
(
   UNSIGNED32  ulConnectionID,        // (I) value used to associate a user/connection
                                      //     and can be used to track the state
   ADSHANDLE   hConnection,           // (I) active connection to be used by the procedure
   UNSIGNED32  *pulNumRowsAffected    // (O) the number of rows affected
);


typedef UNSIGNED32 (WINAPI *STARTUP_PROCEDURE2_PTR)
(
   UNSIGNED32  ulConnectionID, // (I) value used to associate a user/connection
                               //     and can be used to track the state
   ADSHANDLE   hConnection     // (I) active connection to be used by the procedure
);


typedef UNSIGNED32 (WINAPI *SHUTDOWN_PROCEDURE2_PTR)
(
   UNSIGNED32  ulConnectionID, // (I) value used to associate a user/connection
                               //     and can be used to track the state
   ADSHANDLE   hConnection     // (I) active connection to be used by the procedure
);


typedef UNSIGNED32 (WINAPI *TRIGGER_FUNCTION_PTR)
(
   UNSIGNED32  ulConnectionID,  // (I) Unique ID identifying the user causing this trig
   ADSHANDLE   hConnection,     // (I) Active ACE connection handle user can perform
                                //     operations on
   UNSIGNED8   *pucTriggerName, // (I) Name of trigger in the dictionary
   UNSIGNED8   *pucTableName,   // (I) Name of the base table that caused the trigger
   UNSIGNED32  ulEventType,     // (I) Flag with event type (insert, update, etc.)
   UNSIGNED32  ulTriggerType,   // (I) Flag with trigger type (before, after, etc.)
   UNSIGNED32  ulRecNo          // (I) Record number of the record being modified
);

typedef UNSIGNED32 (WINAPI *GET_INTERFACE_VERSION_PTR)();

#endif // WINAPI

/*
 * This macro allows a numeric field value to be passed into functions
 * that expect field names.  If the user prefers to use column number,
 * then calls like this can be made:
 * ulRet = AdsGetDouble( hTable, ADSFIELD( 1 ), &dVal );
 * Where the first column is a numeric value to retrieve.
 */
#if defined( x64 )
   #if defined( ADS_LINUX )
      #define ADSFIELD(x)   ((unsigned char*)(long)( x ))
   #else
      #define ADSFIELD(x)   ((unsigned char*)(__int64)(x))
   #endif
#else
   #define ADSFIELD(x)   ((unsigned char*)(int)( x ))
#endif


#ifdef __cplusplus
   extern "C"
   {
#endif

#if defined( NLM ) || defined( ADS_LINUX )
   #if !defined( SNAPDUMP )      // Snapdump is now built as a WIN32 app so WINAPI is defined
      #define WINAPI   /* nothing */
   #endif
#endif

UNSIGNED32 ENTRYPOINT AdsAddCustomKey( ADSHANDLE hIndex );
typedef UNSIGNED32 (WINAPI *ADSADDCUSTOMKEY_PTR)( ADSHANDLE hIndex );


UNSIGNED32 ENTRYPOINT AdsAppendRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSAPPENDRECORD_PTR)( ADSHANDLE hTable );


UNSIGNED32 ENTRYPOINT AdsApplicationExit();
typedef UNSIGNED32 (WINAPI *ADSAPPLICATIONEXIT_PTR)();


UNSIGNED32 ENTRYPOINT AdsAtBOF( ADSHANDLE    hTable,
                                UNSIGNED16   *pbBof );
typedef UNSIGNED32 (WINAPI *ADSATBOF_PTR)( ADSHANDLE    hTable,
                                UNSIGNED16   *pbBof );

UNSIGNED32 ENTRYPOINT AdsAtEOF( ADSHANDLE    hTable,
                                UNSIGNED16   *pbEof );
typedef UNSIGNED32 (WINAPI *ADSATEOF_PTR)( ADSHANDLE    hTable,
                                UNSIGNED16   *pbEof );

UNSIGNED32 ENTRYPOINT AdsBeginTransaction( ADSHANDLE hConnect );
typedef UNSIGNED32 (WINAPI *ADSBEGINTRANSACTION_PTR)( ADSHANDLE hConnect );

UNSIGNED32 ENTRYPOINT AdsBinaryToFile( ADSHANDLE   hTable,
                                       UNSIGNED8   *pucFldName,
                                       UNSIGNED8   *pucFileName );
typedef UNSIGNED32 (WINAPI *ADSBINARYTOFILE_PTR)( ADSHANDLE   hTable,
                                       UNSIGNED8   *pucFldName,
                                       UNSIGNED8   *pucFileName );

UNSIGNED32 ENTRYPOINT AdsCacheOpenCursors( UNSIGNED16 usOpen );
typedef UNSIGNED32 (WINAPI *ADSCACHEOPENCURSORS_PTR)( UNSIGNED16 usOpen );

UNSIGNED32 ENTRYPOINT AdsCacheOpenTables( UNSIGNED16 usOpen );
typedef UNSIGNED32 (WINAPI *ADSCACHEOPENTABLES_PTR)( UNSIGNED16 usOpen );

UNSIGNED32 ENTRYPOINT AdsCacheRecords( ADSHANDLE hTable,
                                       UNSIGNED16 usNumRecords );
typedef UNSIGNED32 (WINAPI *ADSCACHERECORDS_PTR)( ADSHANDLE hTable,
                                       UNSIGNED16 usNumRecords );

UNSIGNED32 ENTRYPOINT AdsCancelUpdate( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSCANCELUPDATE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsCancelUpdate90( ADSHANDLE hTable, UNSIGNED32 ulOptions );
typedef UNSIGNED32 (WINAPI *ADSCANCELUPDATE90_PTR)( ADSHANDLE hTable, UNSIGNED32 ulOptions );

UNSIGNED32 ENTRYPOINT AdsCheckExistence( ADSHANDLE    hConnect,
                                         UNSIGNED8    *pucFileName,
                                         UNSIGNED16   *pusOnDisk );
typedef UNSIGNED32 (WINAPI *ADSCHECKEXISTENCE_PTR)( ADSHANDLE    hConnect,
                                         UNSIGNED8    *pucFileName,
                                         UNSIGNED16   *pusOnDisk );

UNSIGNED32 ENTRYPOINT AdsClearAllScopes( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSCLEARALLSCOPES_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsClearDefault( void );
typedef UNSIGNED32 (WINAPI *ADSCLEARDEFAULT_PTR)( void );

UNSIGNED32 ENTRYPOINT AdsClearFilter( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSCLEARFILTER_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsClearRelation( ADSHANDLE hTableParent );
typedef UNSIGNED32 (WINAPI *ADSCLEARRELATION_PTR)( ADSHANDLE hTableParent );

UNSIGNED32 ENTRYPOINT AdsClearScope( ADSHANDLE  hIndex,
                                     UNSIGNED16 usScopeOption );
typedef UNSIGNED32 (WINAPI *ADSCLEARSCOPE_PTR)( ADSHANDLE  hIndex,
                                     UNSIGNED16 usScopeOption );

UNSIGNED32 ENTRYPOINT AdsCloneTable( ADSHANDLE  hTable,
                                     ADSHANDLE  *phClone );
typedef UNSIGNED32 (WINAPI *ADSCLONETABLE_PTR)( ADSHANDLE  hTable,
                                     ADSHANDLE  *phClone );

UNSIGNED32 ENTRYPOINT AdsCloseAllIndexes( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSCLOSEALLINDEXES_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsCloseAllTables( void );
typedef UNSIGNED32 (WINAPI *ADSCLOSEALLTABLES_PTR)( void );

UNSIGNED32 ENTRYPOINT AdsCloseIndex( ADSHANDLE hIndex );
typedef UNSIGNED32 (WINAPI *ADSCLOSEINDEX_PTR)( ADSHANDLE hIndex );

UNSIGNED32 ENTRYPOINT AdsCloseTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSCLOSETABLE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsCloseCachedTables( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSCLOSECACHEDTABLES_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsCommitTransaction( ADSHANDLE hConnect );
typedef UNSIGNED32 (WINAPI *ADSCOMMITTRANSACTION_PTR)( ADSHANDLE hConnect );

UNSIGNED32 ENTRYPOINT AdsConnect( UNSIGNED8  *pucServerName,
                                  ADSHANDLE  *phConnect );
typedef UNSIGNED32 (WINAPI *ADSCONNECT_PTR)( UNSIGNED8  *pucServerName,
                                  ADSHANDLE  *phConnect );

UNSIGNED32 ENTRYPOINT AdsConnect26( UNSIGNED8  *pucServerName,
                                    UNSIGNED16 usServerTypes,
                                    ADSHANDLE  *phConnect );
typedef UNSIGNED32 (WINAPI *ADSCONNECT26_PTR)( UNSIGNED8  *pucServerName,
                                    UNSIGNED16 usServerTypes,
                                    ADSHANDLE  *phConnect );

UNSIGNED32 ENTRYPOINT AdsConnect60( UNSIGNED8  *pucServerPath,
                                    UNSIGNED16 usServerTypes,
                                    UNSIGNED8  *pucUserName,
                                    UNSIGNED8  *pucPassword,
                                    UNSIGNED32 ulOptions,
                                    ADSHANDLE  *phConnect );
typedef UNSIGNED32 (WINAPI *ADSCONNECT60_PTR)( UNSIGNED8  *pucServerPath,
                                    UNSIGNED16 usServerTypes,
                                    UNSIGNED8  *pucUserName,
                                    UNSIGNED8  *pucPassword,
                                    UNSIGNED32 ulOptions,
                                    ADSHANDLE  *phConnect );

UNSIGNED32 ENTRYPOINT AdsIsConnectionAlive(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       *pbConnectionIsAlive );
typedef UNSIGNED32 (WINAPI *ADSISCONNECTIONALIVE_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       *pbConnectionIsAlive );

UNSIGNED32 ENTRYPOINT AdsContinue( ADSHANDLE    hTable,
                                   UNSIGNED16   *pbFound );
typedef UNSIGNED32 (WINAPI *ADSCONTINUE_PTR)( ADSHANDLE    hTable,
                                   UNSIGNED16   *pbFound );

UNSIGNED32 ENTRYPOINT AdsConvertTable( ADSHANDLE   hObj,
                                       UNSIGNED16  usFilterOption,
                                       UNSIGNED8   *pucFile,
                                       UNSIGNED16  usTableType );
typedef UNSIGNED32 (WINAPI *ADSCONVERTTABLE_PTR)( ADSHANDLE   hObj,
                                       UNSIGNED16  usFilterOption,
                                       UNSIGNED8   *pucFile,
                                       UNSIGNED16  usTableType );

UNSIGNED32 ENTRYPOINT AdsCopyTable( ADSHANDLE   hObj,
                                    UNSIGNED16  usFilterOption,
                                    UNSIGNED8   *pucFile );
typedef UNSIGNED32 (WINAPI *ADSCOPYTABLE_PTR)( ADSHANDLE   hObj,
                                    UNSIGNED16  usFilterOption,
                                    UNSIGNED8   *pucFile );

UNSIGNED32 ENTRYPOINT AdsCopyTableContents( ADSHANDLE    hObjFrom,
                                            ADSHANDLE    hTableTo,
                                            UNSIGNED16   usFilterOption );
typedef UNSIGNED32 (WINAPI *ADSCOPYTABLECONTENTS_PTR)( ADSHANDLE    hObjFrom,
                                            ADSHANDLE    hTableTo,
                                            UNSIGNED16   usFilterOption );

UNSIGNED32 ENTRYPOINT AdsCopyTableStructure( ADSHANDLE   hTable,
                                             UNSIGNED8   *pucFile );
typedef UNSIGNED32 (WINAPI *ADSCOPYTABLESTRUCTURE_PTR)( ADSHANDLE   hTable,
                                             UNSIGNED8   *pucFile );

UNSIGNED32 ENTRYPOINT AdsCreateIndex( ADSHANDLE    hObj,
                                      UNSIGNED8    *pucFileName,
                                      UNSIGNED8    *pucTag,
                                      UNSIGNED8    *pucExpr,
                                      UNSIGNED8    *pucCondition,
                                      UNSIGNED8    *pucWhile,
                                      UNSIGNED32   ulOptions,
                                      ADSHANDLE    *phIndex );
typedef UNSIGNED32 (WINAPI *ADSCREATEINDEX_PTR)( ADSHANDLE    hObj,
                                      UNSIGNED8    *pucFileName,
                                      UNSIGNED8    *pucTag,
                                      UNSIGNED8    *pucExpr,
                                      UNSIGNED8    *pucCondition,
                                      UNSIGNED8    *pucWhile,
                                      UNSIGNED32   ulOptions,
                                      ADSHANDLE    *phIndex );

UNSIGNED32 ENTRYPOINT AdsCreateIndex61( ADSHANDLE    hObj,
                                        UNSIGNED8    *pucFileName,
                                        UNSIGNED8    *pucTag,
                                        UNSIGNED8    *pucExpr,
                                        UNSIGNED8    *pucCondition,
                                        UNSIGNED8    *pucWhile,
                                        UNSIGNED32   ulOptions,
                                        UNSIGNED32   ulPageSize,
                                        ADSHANDLE    *phIndex );
typedef UNSIGNED32 (WINAPI *ADSCREATEINDEX61_PTR)( ADSHANDLE    hObj,
                                        UNSIGNED8    *pucFileName,
                                        UNSIGNED8    *pucTag,
                                        UNSIGNED8    *pucExpr,
                                        UNSIGNED8    *pucCondition,
                                        UNSIGNED8    *pucWhile,
                                        UNSIGNED32   ulOptions,
                                        UNSIGNED32   ulPageSize,
                                        ADSHANDLE    *phIndex );

UNSIGNED32 ENTRYPOINT AdsCreateIndex90( ADSHANDLE    hObj,
                                        UNSIGNED8    *pucFileName,
                                        UNSIGNED8    *pucTag,
                                        UNSIGNED8    *pucExpr,
                                        UNSIGNED8    *pucCondition,
                                        UNSIGNED8    *pucWhile,
                                        UNSIGNED32   ulOptions,
                                        UNSIGNED32   ulPageSize,
                                        UNSIGNED8    *pucCollation,
                                        ADSHANDLE    *phIndex );
typedef UNSIGNED32 (WINAPI *ADSCREATEINDEX90_PTR)( ADSHANDLE    hObj,
                                        UNSIGNED8    *pucFileName,
                                        UNSIGNED8    *pucTag,
                                        UNSIGNED8    *pucExpr,
                                        UNSIGNED8    *pucCondition,
                                        UNSIGNED8    *pucWhile,
                                        UNSIGNED32   ulOptions,
                                        UNSIGNED32   ulPageSize,
                                        UNSIGNED8    *pucCollation,
                                        ADSHANDLE    *phIndex );

UNSIGNED32 ENTRYPOINT AdsCreateFTSIndex( ADSHANDLE   hTable,
                                         UNSIGNED8   *pucFileName,
                                         UNSIGNED8   *pucTag,
                                         UNSIGNED8   *pucField,
                                         UNSIGNED32  ulPageSize,
                                         UNSIGNED32  ulMinWordLen,
                                         UNSIGNED32  ulMaxWordLen,
                                         UNSIGNED16  usUseDefaultDelim,
                                         UNSIGNED8   *pucDelimiters,
                                         UNSIGNED16  usUseDefaultNoise,
                                         UNSIGNED8   *pucNoiseWords,
                                         UNSIGNED16  usUseDefaultDrop,
                                         UNSIGNED8   *pucDropChars,
                                         UNSIGNED16  usUseDefaultConditionals,
                                         UNSIGNED8   *pucConditionalChars,
                                         UNSIGNED8   *pucCollation,
                                         UNSIGNED8   *pucReserved1,
                                         UNSIGNED32  ulOptions );
typedef UNSIGNED32 (WINAPI *ADSCREATEFTSINDEX_PTR)( ADSHANDLE   hTable,
                                         UNSIGNED8   *pucFileName,
                                         UNSIGNED8   *pucTag,
                                         UNSIGNED8   *pucField,
                                         UNSIGNED32  ulPageSize,
                                         UNSIGNED32  ulMinWordLen,
                                         UNSIGNED32  ulMaxWordLen,
                                         UNSIGNED16  usUseDefaultDelim,
                                         UNSIGNED8   *pucDelimiters,
                                         UNSIGNED16  usUseDefaultNoise,
                                         UNSIGNED8   *pucNoiseWords,
                                         UNSIGNED16  usUseDefaultDrop,
                                         UNSIGNED8   *pucDropChars,
                                         UNSIGNED16  usUseDefaultConditionals,
                                         UNSIGNED8   *pucConditionalChars,
                                         UNSIGNED8   *pucReserved1,
                                         UNSIGNED8   *pucReserved2,
                                         UNSIGNED32  ulOptions );

UNSIGNED32 ENTRYPOINT AdsCreateTable( ADSHANDLE    hConnection,
                                      UNSIGNED8    *pucName,
                                      UNSIGNED8    *pucAlias,
                                      UNSIGNED16   usTableType,
                                      UNSIGNED16   usCharType,
                                      UNSIGNED16   usLockType,
                                      UNSIGNED16   usCheckRights,
                                      UNSIGNED16   usMemoSize,
                                      UNSIGNED8    *pucFields,
                                      ADSHANDLE    *phTable );
typedef UNSIGNED32 (WINAPI *ADSCREATETABLE_PTR)( ADSHANDLE    hConnection,
                                      UNSIGNED8    *pucName,
                                      UNSIGNED8    *pucAlias,
                                      UNSIGNED16   usTableType,
                                      UNSIGNED16   usCharType,
                                      UNSIGNED16   usLockType,
                                      UNSIGNED16   usCheckRights,
                                      UNSIGNED16   usMemoSize,
                                      UNSIGNED8    *pucFields,
                                      ADSHANDLE    *phTable );

UNSIGNED32 ENTRYPOINT AdsCreateTable71( ADSHANDLE    hConnection,
                                        UNSIGNED8    *pucName,
                                        UNSIGNED8    *pucDBObjName,
                                        UNSIGNED16   usTableType,
                                        UNSIGNED16   usCharType,
                                        UNSIGNED16   usLockType,
                                        UNSIGNED16   usCheckRights,
                                        UNSIGNED16   usMemoSize,
                                        UNSIGNED8    *pucFields,
                                        UNSIGNED32   ulOptions,
                                        ADSHANDLE    *phTable );
typedef UNSIGNED32 (WINAPI *ADSCREATETABLE71_PTR)( ADSHANDLE    hConnection,
                                      UNSIGNED8    *pucName,
                                      UNSIGNED8    *pucDBObjName,
                                      UNSIGNED16   usTableType,
                                      UNSIGNED16   usCharType,
                                      UNSIGNED16   usLockType,
                                      UNSIGNED16   usCheckRights,
                                      UNSIGNED16   usMemoSize,
                                      UNSIGNED8    *pucFields,
                                      UNSIGNED32   ulOptions,
                                      ADSHANDLE    *phTable );

UNSIGNED32 ENTRYPOINT AdsCreateTable90( ADSHANDLE    hConnection,
                                        UNSIGNED8    *pucName,
                                        UNSIGNED8    *pucDBObjName,
                                        UNSIGNED16   usTableType,
                                        UNSIGNED16   usCharType,
                                        UNSIGNED16   usLockType,
                                        UNSIGNED16   usCheckRights,
                                        UNSIGNED16   usMemoSize,
                                        UNSIGNED8    *pucFields,
                                        UNSIGNED32   ulOptions,
                                        UNSIGNED8    *pucCollation,
                                        ADSHANDLE    *phTable );
typedef UNSIGNED32 (WINAPI *ADSCREATETABLE90_PTR)( ADSHANDLE    hConnection,
                                      UNSIGNED8    *pucName,
                                      UNSIGNED8    *pucDBObjName,
                                      UNSIGNED16   usTableType,
                                      UNSIGNED16   usCharType,
                                      UNSIGNED16   usLockType,
                                      UNSIGNED16   usCheckRights,
                                      UNSIGNED16   usMemoSize,
                                      UNSIGNED8    *pucFields,
                                      UNSIGNED32   ulOptions,
                                      UNSIGNED8    *pucCollation,
                                      ADSHANDLE    *phTable );

UNSIGNED32 ENTRYPOINT AdsDDCreate( UNSIGNED8  *pucDictionaryPath,
                                   UNSIGNED16 usEncrypt,
                                   UNSIGNED8  *pucDescription,
                                   ADSHANDLE  *phDictionary );
typedef UNSIGNED32 (WINAPI *ADSDDCREATE_PTR)( UNSIGNED8  *pucDictionaryPath,
                                   UNSIGNED16 usEncrypt,
                                   UNSIGNED8  *pucDescription,
                                   ADSHANDLE  *phDictionary );

UNSIGNED32 ENTRYPOINT AdsDDCreateRefIntegrity( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucRIName,
                                               UNSIGNED8  *pucFailTable,
                                               UNSIGNED8  *pucParentTableName,
                                               UNSIGNED8  *pucParentTagName,
                                               UNSIGNED8  *pucChildTableName,
                                               UNSIGNED8  *pucChildTagName,
                                               UNSIGNED16 usUpdateRule,
                                               UNSIGNED16 usDeleteRule );
typedef UNSIGNED32 (WINAPI *ADSDDCREATEREFINTEGRITY_PTR)( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucRIName,
                                               UNSIGNED8  *pucFailTable,
                                               UNSIGNED8  *pucParentTableName,
                                               UNSIGNED8  *pucParentTagName,
                                               UNSIGNED8  *pucChildTableName,
                                               UNSIGNED8  *pucChildTagName,
                                               UNSIGNED16 usUpdateRule,
                                               UNSIGNED16 usDeleteRule );

UNSIGNED32 ENTRYPOINT AdsDDCreateRefIntegrity62( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucRIName,
                                               UNSIGNED8  *pucFailTable,
                                               UNSIGNED8  *pucParentTableName,
                                               UNSIGNED8  *pucParentTagName,
                                               UNSIGNED8  *pucChildTableName,
                                               UNSIGNED8  *pucChildTagName,
                                               UNSIGNED16 usUpdateRule,
                                               UNSIGNED16 usDeleteRule,
                                               UNSIGNED8  *pucNoPrimaryError,
                                               UNSIGNED8  *pucCascadeError );
typedef UNSIGNED32 (WINAPI *ADSDDCREATEREFINTEGRITY62_PTR)( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucRIName,
                                               UNSIGNED8  *pucFailTable,
                                               UNSIGNED8  *pucParentTableName,
                                               UNSIGNED8  *pucParentTagName,
                                               UNSIGNED8  *pucChildTableName,
                                               UNSIGNED8  *pucChildTagName,
                                               UNSIGNED16 usUpdateRule,
                                               UNSIGNED16 usDeleteRule,
                                               UNSIGNED8  *pucNoPrimaryError,
                                               UNSIGNED8  *pucCascadeError );

UNSIGNED32 ENTRYPOINT AdsDDRemoveRefIntegrity( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucRIName );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVEREFINTEGRITY_PTR)( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucRIName );

UNSIGNED32 ENTRYPOINT AdsDDGetDatabaseProperty( ADSHANDLE  hObject,
                                                UNSIGNED16 usPropertyID,
                                                VOID       *pvProperty,
                                                UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETDATABASEPROPERTY_PTR)( ADSHANDLE  hObject,
                                                UNSIGNED16 usPropertyID,
                                                VOID       *pvProperty,
                                                UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetFieldProperty( ADSHANDLE  hObject,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED8  *pucFieldName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETFIELDPROPERTY_PTR)( ADSHANDLE  hObject,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED8  *pucFieldName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetIndexFileProperty( ADSHANDLE  hObject,
                                                 UNSIGNED8  *pucTableName,
                                                 UNSIGNED8  *pucIndexFileName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETINDEXFILEPROPERTY_PTR)( ADSHANDLE  hObject,
                                                 UNSIGNED8  *pucTableName,
                                                 UNSIGNED8  *pucIndexFileName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetIndexProperty( ADSHANDLE  hObject,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED8  *pucIndexName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETINDEXPROPERTY_PTR)( ADSHANDLE  hObject,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED8  *pucIndexName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetLinkProperty( ADSHANDLE  hConnect,
                                            UNSIGNED8  *pucLinkName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETLINKPROPERTY_PTR)( ADSHANDLE  hConnect,
                                            UNSIGNED8  *pucLinkName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetTableProperty( ADSHANDLE  hObject,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETTABLEPROPERTY_PTR)( ADSHANDLE  hObject,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetUserGroupProperty( ADSHANDLE  hObject,
                                                 UNSIGNED8  *pucUserGroupName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETUSERGROUPPROPERTY_PTR)( ADSHANDLE  hObject,
                                                 UNSIGNED8  *pucUserGroupName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetUserProperty( ADSHANDLE  hObject,
                                            UNSIGNED8  *pucUserName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETUSERPROPERTY_PTR)( ADSHANDLE  hObject,
                                            UNSIGNED8  *pucUserName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetViewProperty( ADSHANDLE  hObject,
                                            UNSIGNED8  *pucViewName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETVIEWPROPERTY_PTR)( ADSHANDLE  hObject,
                                            UNSIGNED8  *pucViewName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetTriggerProperty( ADSHANDLE  hObject,
                                               UNSIGNED8  *pucTriggerName,
                                               UNSIGNED16 usPropertyID,
                                               VOID       *pvProperty,
                                               UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETTRIGGERPROPERTY_PTR)( ADSHANDLE  hObject,
                                               UNSIGNED8  *pucTriggerName,
                                               UNSIGNED16 usPropertyID,
                                               VOID       *pvProperty,
                                               UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetProcedureProperty( ADSHANDLE  hObject,
                                                 UNSIGNED8  *pucProcName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETPROCEDUREPROPERTY_PTR)( ADSHANDLE  hObject,
                                                 UNSIGNED8  *pucProcName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetRefIntegrityProperty( ADSHANDLE  hObject,
                                                    UNSIGNED8  *pucRIName,
                                                    UNSIGNED16 usPropertyID,
                                                    UNSIGNED8  *pucProperty,
                                                    UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETREFINTEGRITYPROPERTY_PTR)( ADSHANDLE  hObject,
                                                    UNSIGNED8  *pucRIName,
                                                    UNSIGNED16 usPropertyID,
                                                    UNSIGNED8  *pucProperty,
                                                    UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDGetPermissions( ADSHANDLE  hDBConn,
                                           UNSIGNED8  *pucGrantee,
                                           UNSIGNED16 usObjectType,
                                           UNSIGNED8  *pucObjectName,
                                           UNSIGNED8  *pucParentName,
                                           UNSIGNED16 usGetInherited,
                                           UNSIGNED32 *pulPermissions );
typedef UNSIGNED32 (WINAPI *ADSDDGETPERMISSIONS_PTR)( ADSHANDLE  hDBConn,
                                           UNSIGNED8  *pucGrantee,
                                           UNSIGNED16 usObjectType,
                                           UNSIGNED8  *pucObjectName,
                                           UNSIGNED8  *pucParentName,
                                           UNSIGNED16 usGetInherited,
                                           UNSIGNED32 *pulPermissions );

UNSIGNED32 ENTRYPOINT AdsDDGrantPermission( ADSHANDLE  hAdminConn,
                                            UNSIGNED16 usObjectType,
                                            UNSIGNED8  *pucObjectName,
                                            UNSIGNED8  *pucParentName,
                                            UNSIGNED8  *pucGrantee,
                                            UNSIGNED32 ulPermissions );
typedef UNSIGNED32 (WINAPI *ADSDDGRANTPERMISSION_PTR)( ADSHANDLE  hAdminConn,
                                            UNSIGNED16 usObjectType,
                                            UNSIGNED8  *pucObjectName,
                                            UNSIGNED8  *pucParentName,
                                            UNSIGNED8  *pucGrantee,
                                            UNSIGNED32 ulPermissions );

UNSIGNED32 ENTRYPOINT AdsDDRevokePermission( ADSHANDLE  hAdminConn,
                                             UNSIGNED16 usObjectType,
                                             UNSIGNED8  *pucObjectName,
                                             UNSIGNED8  *pucParentName,
                                             UNSIGNED8  *pucGrantee,
                                             UNSIGNED32 ulPermissions );
typedef UNSIGNED32 (WINAPI *ADSDDREVOKEPERMISSION_PTR)( ADSHANDLE  hAdminConn,
                                             UNSIGNED16 usObjectType,
                                             UNSIGNED8  *pucObjectName,
                                             UNSIGNED8  *pucParentName,
                                             UNSIGNED8  *pucGrantee,
                                             UNSIGNED32 ulPermissions );

UNSIGNED32 ENTRYPOINT AdsDDSetDatabaseProperty( ADSHANDLE  hDictionary,
                                        UNSIGNED16 usPropertyID,
                                        VOID       *pvProperty,
                                        UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETDATABASEPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                        UNSIGNED16 usPropertyID,
                                        VOID       *pvProperty,
                                        UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetFieldProperty( ADSHANDLE  hDictionary,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED8  *pucFieldName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 usPropertyLen,
                                             UNSIGNED16 usValidateOption,
                                             UNSIGNED8  *pucFailTable );
typedef UNSIGNED32 (WINAPI *ADSDDSETFIELDPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED8  *pucFieldName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 usPropertyLen,
                                             UNSIGNED16 usValidateOption,
                                             UNSIGNED8  *pucFailTable );

UNSIGNED32 ENTRYPOINT AdsDDSetProcedureProperty( ADSHANDLE  hDictionary,
                                                 UNSIGNED8  *pucProcedureName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETPROCEDUREPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                                 UNSIGNED8  *pucProcedureName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetTableProperty( ADSHANDLE  hDictionary,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 usPropertyLen,
                                             UNSIGNED16 usValidateOption,
                                             UNSIGNED8  *pucFailTable );
typedef UNSIGNED32 (WINAPI *ADSDDSETTABLEPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                             UNSIGNED8  *pucTableName,
                                             UNSIGNED16 usPropertyID,
                                             VOID       *pvProperty,
                                             UNSIGNED16 usPropertyLen,
                                             UNSIGNED16 usValidateOption,
                                             UNSIGNED8  *pucFailTable );

UNSIGNED32 ENTRYPOINT AdsDDSetUserGroupProperty( ADSHANDLE  hDictionary,
                                                 UNSIGNED8  *pucUserGroupName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETUSERGROUPPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                                 UNSIGNED8  *pucUserGroupName,
                                                 UNSIGNED16 usPropertyID,
                                                 VOID       *pvProperty,
                                                 UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetUserProperty( ADSHANDLE  hDictionary,
                                            UNSIGNED8  *pucUserName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETUSERPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                            UNSIGNED8  *pucUserName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetViewProperty( ADSHANDLE  hDictionary,
                                            UNSIGNED8  *pucViewName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETVIEWPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                            UNSIGNED8  *pucViewName,
                                            UNSIGNED16 usPropertyID,
                                            VOID       *pvProperty,
                                            UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetObjectAccessRights( ADSHANDLE  hDictionary,
                                                  UNSIGNED8  *pucObjectName,
                                                  UNSIGNED8  *pucAccessorName,
                                                  UNSIGNED8  *pucAllowedAccess );
typedef UNSIGNED32 (WINAPI *ADSDDSETOBJECTACCESSRIGHTS_PTR)( ADSHANDLE  hDictionary,
                                                  UNSIGNED8  *pucObjectName,
                                                  UNSIGNED8  *pucAccessorName,
                                                  UNSIGNED8  *pucAllowedAccess );

UNSIGNED32 ENTRYPOINT AdsDDAddProcedure( ADSHANDLE  hDictionary,
                                         UNSIGNED8  *pucName,
                                         UNSIGNED8  *pucContainer,
                                         UNSIGNED8  *pucProcName,
                                         UNSIGNED32 ulInvokeOption,
                                         UNSIGNED8  *pucInParams,
                                         UNSIGNED8  *pucOutParams,
                                         UNSIGNED8  *pucComments );
typedef UNSIGNED32 (WINAPI *ADSDDADDPROCEDURE_PTR)( ADSHANDLE  hDictionary,
                                         UNSIGNED8  *pucName,
                                         UNSIGNED8  *pucDLL,
                                         UNSIGNED8  *pucProcName,
                                         UNSIGNED32 ulInvokeOption,
                                         UNSIGNED8  *pucInParams,
                                         UNSIGNED8  *pucOutParams,
                                         UNSIGNED8  *pucComments );


UNSIGNED32 ENTRYPOINT AdsDDAddTable( ADSHANDLE    hDictionary,
                                     UNSIGNED8    *pucTableName,
                                     UNSIGNED8    *pucTablePath,
                                     UNSIGNED16   usTableType,
                                     UNSIGNED16   usCharType,
                                     UNSIGNED8    *pucIndexFiles,
                                     UNSIGNED8    *pucComments );
typedef UNSIGNED32 (WINAPI *ADSDDADDTABLE_PTR)( ADSHANDLE    hDictionary,
                                     UNSIGNED8    *pucTableName,
                                     UNSIGNED8    *pucTablePath,
                                     UNSIGNED16   usTableType,
                                     UNSIGNED16   usCharType,
                                     UNSIGNED8    *pucIndexFiles,
                                     UNSIGNED8    *pucComments );

UNSIGNED32 ENTRYPOINT AdsDDAddTable90( ADSHANDLE    hDictionary,
                                       UNSIGNED8    *pucTableName,
                                       UNSIGNED8    *pucTablePath,
                                       UNSIGNED16   usTableType,
                                       UNSIGNED16   usCharType,
                                       UNSIGNED8    *pucIndexFiles,
                                       UNSIGNED8    *pucComments,
                                       UNSIGNED8    *pucCollation );
typedef UNSIGNED32 (WINAPI *ADSDDADDTABLE90_PTR)( ADSHANDLE    hDictionary,
                                       UNSIGNED8    *pucTableName,
                                       UNSIGNED8    *pucTablePath,
                                       UNSIGNED16   usTableType,
                                       UNSIGNED16   usCharType,
                                       UNSIGNED8    *pucIndexFiles,
                                       UNSIGNED8    *pucComments,
                                       UNSIGNED8    *pucCollation );

UNSIGNED32 ENTRYPOINT AdsDDAddView( ADSHANDLE      hDictionary,
                                    UNSIGNED8      *pucName,
                                    UNSIGNED8      *pucComments,
                                    UNSIGNED8      *pucSQL );
typedef UNSIGNED32 (WINAPI *ADSDDADDVIEW_PTR)( ADSHANDLE      hDictionary,
                                    UNSIGNED8      *pucName,
                                    UNSIGNED8      *pucComments,
                                    UNSIGNED8      *pucSQL );

UNSIGNED32 ENTRYPOINT AdsDDCreateTrigger( ADSHANDLE      hDictionary,
                                          UNSIGNED8      *pucName,
                                          UNSIGNED8      *pucTableName,
                                          UNSIGNED32     ulTriggerType,
                                          UNSIGNED32     ulEventTypes,
                                          UNSIGNED32     ulContainerType,
                                          UNSIGNED8      *pucContainer,
                                          UNSIGNED8      *pucFunctionName,
                                          UNSIGNED32     ulPriority,
                                          UNSIGNED8      *pucComments,
                                          UNSIGNED32     ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDCREATETRIGGER_PTR)( ADSHANDLE      hDictionary,
                                          UNSIGNED8      *pucName,
                                          UNSIGNED8      *pucTableName,
                                          UNSIGNED32     ulTriggerType,
                                          UNSIGNED32     ulEventTypes,
                                          UNSIGNED32     ulContainerType,
                                          UNSIGNED8      *pucContainer,
                                          UNSIGNED8      *pucFunctionName,
                                          UNSIGNED32     ulPriority,
                                          UNSIGNED8      *pucComments,
                                          UNSIGNED32     ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDRemoveTrigger( ADSHANDLE      hDictionary,
                                          UNSIGNED8      *pucName );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVETRIGGER_PTR)( ADSHANDLE      hDictionary,
                                          UNSIGNED8      *pucName );

UNSIGNED32 ENTRYPOINT AdsDDAddIndexFile( ADSHANDLE    hDictionary,
                                         UNSIGNED8    *pucTableName,
                                         UNSIGNED8    *pucIndexFilePath,
                                         UNSIGNED8    *pucComment );
typedef UNSIGNED32 (WINAPI *ADSDDADDINDEXFILE_PTR)( ADSHANDLE    hDictionary,
                                         UNSIGNED8    *pucTableName,
                                         UNSIGNED8    *pucIndexFilePath,
                                         UNSIGNED8    *pucComment );

UNSIGNED32 ENTRYPOINT AdsDDCreateUser( ADSHANDLE    hDictionary,
                                       UNSIGNED8    *pucGroupName,
                                       UNSIGNED8    *pucUserName,
                                       UNSIGNED8    *pucPassword,
                                       UNSIGNED8    *pucDescription );
typedef UNSIGNED32 (WINAPI *ADSDDCREATEUSER_PTR)( ADSHANDLE    hDictionary,
                                       UNSIGNED8    *pucGroupName,
                                       UNSIGNED8    *pucUserName,
                                       UNSIGNED8    *pucPassword,
                                       UNSIGNED8    *pucDescription );

UNSIGNED32 ENTRYPOINT AdsDDAddUserToGroup( ADSHANDLE    hDictionary,
                                           UNSIGNED8    *pucGroupName,
                                           UNSIGNED8    *pucUserName );
typedef UNSIGNED32 (WINAPI *ADSDDADDUSERTOGROUP_PTR)( ADSHANDLE    hDictionary,
                                           UNSIGNED8    *pucGroupName,
                                           UNSIGNED8    *pucUserName );

UNSIGNED32 ENTRYPOINT AdsDDRemoveUserFromGroup( ADSHANDLE    hDictionary,
                                                UNSIGNED8    *pucGroupName,
                                                UNSIGNED8    *pucUserName );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVEUSERFROMGROUP_PTR)( ADSHANDLE    hDictionary,
                                                UNSIGNED8    *pucGroupName,
                                                UNSIGNED8    *pucUserName );


UNSIGNED32 ENTRYPOINT AdsDDDeleteUser( ADSHANDLE    hDictionary,
                                       UNSIGNED8    *pucUserName );
typedef UNSIGNED32 (WINAPI *ADSDDDELETEUSER_PTR)( ADSHANDLE    hDictionary,
                                       UNSIGNED8    *pucUserName );


UNSIGNED32 ENTRYPOINT AdsDDCreateUserGroup( ADSHANDLE    hDictionary,
                                            UNSIGNED8    *pucGroupName,
                                            UNSIGNED8    *pucDescription );
typedef UNSIGNED32 (WINAPI *ADSDDCREATEUSERGROUP_PTR)( ADSHANDLE    hDictionary,
                                            UNSIGNED8    *pucGroupName,
                                            UNSIGNED8    *pucDescription );

UNSIGNED32 ENTRYPOINT AdsDDDeleteUserGroup( ADSHANDLE    hDictionary,
                                            UNSIGNED8    *pucGroupName );
typedef UNSIGNED32 (WINAPI *ADSDDDELETEUSERGROUP_PTR)( ADSHANDLE    hDictionary,
                                            UNSIGNED8    *pucGroupName );

UNSIGNED32 ENTRYPOINT AdsDDDeleteIndex( ADSHANDLE    hDictionary,
                                        UNSIGNED8    *pucTableName,
                                        UNSIGNED8    *pucIndexName );
typedef UNSIGNED32 (WINAPI *ADSDDDELETEINDEX_PTR)( ADSHANDLE    hDictionary,
                                        UNSIGNED8    *pucTableName,
                                        UNSIGNED8    *pucIndexName );

UNSIGNED32 ENTRYPOINT AdsDDRemoveIndexFile( ADSHANDLE    hDictionary,
                                            UNSIGNED8    *pucTableName,
                                            UNSIGNED8    *pucIndexFileName,
                                            UNSIGNED16   usDeleteFile );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVEINDEXFILE_PTR)( ADSHANDLE    hDictionary,
                                            UNSIGNED8    *pucTableName,
                                            UNSIGNED8    *pucIndexFileName,
                                            UNSIGNED16   usDeleteFile );

UNSIGNED32 ENTRYPOINT AdsDDRemoveProcedure( ADSHANDLE  hDictionary,
                                            UNSIGNED8  *pucName );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVEPROCEDURE_PTR)( ADSHANDLE  hDictionary,
                                            UNSIGNED8  *pucName );


UNSIGNED32 ENTRYPOINT AdsDDRemoveTable( ADSHANDLE    hObject,
                                        UNSIGNED8    *pucTableName,
                                        UNSIGNED16   usDeleteFiles );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVETABLE_PTR)( ADSHANDLE    hObject,
                                        UNSIGNED8    *pucTableName,
                                        UNSIGNED16   usDeleteFiles );

UNSIGNED32 ENTRYPOINT AdsDDRemoveView( ADSHANDLE   hDictionary,
                                       UNSIGNED8   *pucName );
typedef UNSIGNED32 (WINAPI *ADSDDREMOVEVIEW_PTR)( ADSHANDLE   hDictionary,
                                       UNSIGNED8   *pucName );

UNSIGNED32 ENTRYPOINT AdsDDRenameObject( ADSHANDLE    hDictionary,
                                         UNSIGNED8    *pucObjectName,
                                         UNSIGNED8    *pucNewObjectName,
                                         UNSIGNED16   usObjectType,
                                         UNSIGNED32   ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDRENAMEOBJECT_PTR)( ADSHANDLE    hDictionary,
                                                    UNSIGNED8    *pucObjectName,
                                                    UNSIGNED8    *pucNewObjectName,
                                                    UNSIGNED16   usObjectType,
                                                    UNSIGNED32   ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDMoveObjectFile( ADSHANDLE    hDictionary,
                                           UNSIGNED16   usObjectType,
                                           UNSIGNED8    *pucObjectName,
                                           UNSIGNED8    *pucNewPath,
                                           UNSIGNED8    *pucIndexFiles,
                                           UNSIGNED8    *pucParent,
                                           UNSIGNED32   ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDMOVEOBJECTFILE_PTR)( ADSHANDLE    hDictionary,
                                                      UNSIGNED16   usObjectType,
                                                      UNSIGNED8    *pucObjectName,
                                                      UNSIGNED8    *pucNewPath,
                                                      UNSIGNED8    *pucIndexFiles,
                                                      UNSIGNED8    *pucParent,
                                                      UNSIGNED32   ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDFindFirstObject( ADSHANDLE  hObject,
                                            UNSIGNED16 usFindObjectType,
                                            UNSIGNED8  *pucParentName,
                                            UNSIGNED8  *pucObjectName,
                                            UNSIGNED16 *pusObjectNameLen,
                                            ADSHANDLE  *phFindHandle );
typedef UNSIGNED32 (WINAPI *ADSDDFINDFIRSTOBJECT_PTR)( ADSHANDLE  hObject,
                                            UNSIGNED16 usFindObjectType,
                                            UNSIGNED8  *pucParentName,
                                            UNSIGNED8  *pucObjectName,
                                            UNSIGNED16 *pusObjectNameLen,
                                            ADSHANDLE  *phFindHandle );

UNSIGNED32 ENTRYPOINT AdsDDFindNextObject( ADSHANDLE  hObject,
                                           ADSHANDLE  hFindHandle,
                                           UNSIGNED8  *pucObjectName,
                                           UNSIGNED16 *pusObjectNameLen );
typedef UNSIGNED32 (WINAPI *ADSDDFINDNEXTOBJECT_PTR)( ADSHANDLE  hObject,
                                           ADSHANDLE  hFindHandle,
                                           UNSIGNED8  *pucObjectName,
                                           UNSIGNED16 *pusObjectNameLen );

UNSIGNED32 ENTRYPOINT AdsDDFindClose( ADSHANDLE hObject, ADSHANDLE hFindHandle );
typedef UNSIGNED32 (WINAPI *ADSDDFINDCLOSE_PTR)( ADSHANDLE hObject, ADSHANDLE hFindHandle );

UNSIGNED32 ENTRYPOINT AdsDDCreateLink( ADSHANDLE  hDBConn,
                                       UNSIGNED8  *pucLinkAlias,
                                       UNSIGNED8  *pucLinkedDDPath,
                                       UNSIGNED8  *pucUserName,
                                       UNSIGNED8  *pucPassword,
                                       UNSIGNED32 ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDCREATELINK_PTR)( ADSHANDLE  hDBConn,
                                       UNSIGNED8  *pucLinkAlias,
                                       UNSIGNED8  *pucLinkedDDPath,
                                       UNSIGNED8  *pucUserName,
                                       UNSIGNED8  *pucPassword,
                                       UNSIGNED32 ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDModifyLink( ADSHANDLE  hDBConn,
                                       UNSIGNED8  *pucLinkAlias,
                                       UNSIGNED8  *pucLinkedDDPath,
                                       UNSIGNED8  *pucUserName,
                                       UNSIGNED8  *pucPassword,
                                       UNSIGNED32 ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDMODIFYLINK_PTR)( ADSHANDLE  hDBConn,
                                       UNSIGNED8  *pucLinkAlias,
                                       UNSIGNED8  *pucLinkedDDPath,
                                       UNSIGNED8  *pucUserName,
                                       UNSIGNED8  *pucPassword,
                                       UNSIGNED32 ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDDropLink( ADSHANDLE  hDBConn,
                                     UNSIGNED8  *pucLinkedDD,
                                     UNSIGNED16 usDropGlobal );
typedef UNSIGNED32 (WINAPI *ADSDDDROPLINK_PTR)( ADSHANDLE  hDBConn,
                                     UNSIGNED8  *pucLinkedDD,
                                     UNSIGNED16 usDropGlobal );

UNSIGNED32 ENTRYPOINT AdsDDCreatePublication( ADSHANDLE      hDictionary,
                                              UNSIGNED8      *pucPublicationName,
                                              UNSIGNED8      *pucComments,
                                              UNSIGNED32     ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDCREATEPUBLICATION_PTR)( ADSHANDLE      hDictionary,
                                              UNSIGNED8      *pucPublicationName,
                                              UNSIGNED8      *pucComments,
                                              UNSIGNED32     ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDGetPublicationProperty( ADSHANDLE  hObject,
                                                   UNSIGNED8  *pucPublicationName,
                                                   UNSIGNED16 usPropertyID,
                                                   VOID       *pvProperty,
                                                   UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETPUBLICATIONPROPERTY_PTR)( ADSHANDLE  hObject,
                                                   UNSIGNED8  *pucPublicationName,
                                                   UNSIGNED16 usPropertyID,
                                                   VOID       *pvProperty,
                                                   UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetPublicationProperty( ADSHANDLE  hDictionary,
                                                   UNSIGNED8  *pucPublicationName,
                                                   UNSIGNED16 usPropertyID,
                                                   VOID       *pvProperty,
                                                   UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETPUBLICATIONPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                                   UNSIGNED8  *pucPublicationName,
                                                   UNSIGNED16 usPropertyID,
                                                   VOID       *pvProperty,
                                                   UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDDeletePublication( ADSHANDLE    hDictionary,
                                              UNSIGNED8    *pucPublicationName );
typedef UNSIGNED32 (WINAPI *ADSDDDELETEPUBLICATION_PTR)( ADSHANDLE    hDictionary,
                                              UNSIGNED8    *pucPublicationName );

UNSIGNED32 ENTRYPOINT AdsDDCreateArticle( ADSHANDLE      hDictionary,
                                          UNSIGNED8      *pucPublicationName,
                                          UNSIGNED8      *pucObjectName,
                                          UNSIGNED8      *pucRowIdentColumns,
                                          UNSIGNED8      *pucFilter,
                                          UNSIGNED32     ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDCREATEARTICLE_PTR)( ADSHANDLE      hDictionary,
                                          UNSIGNED8      *pucPublicationName,
                                          UNSIGNED8      *pucObjectName,
                                          UNSIGNED8      *pucRowIdentColumns,
                                          UNSIGNED8      *pucFilter,
                                          UNSIGNED32     ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDGetArticleProperty( ADSHANDLE  hObject,
                                               UNSIGNED8  *pucPublicationName,
                                               UNSIGNED8  *pucObjectName,
                                               UNSIGNED16 usPropertyID,
                                               VOID       *pvProperty,
                                               UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETARTICLEPROPERTY_PTR)( ADSHANDLE  hObject,
                                               UNSIGNED8  *pucPublicationName,
                                               UNSIGNED8  *pucObjectName,
                                               UNSIGNED16 usPropertyID,
                                               VOID       *pvProperty,
                                               UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetArticleProperty( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucPublicationName,
                                               UNSIGNED8  *pucObjectName,
                                               UNSIGNED16 usPropertyID,
                                               VOID       *pvProperty,
                                               UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETARTICLEPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                               UNSIGNED8  *pucPublicationName,
                                               UNSIGNED8  *pucObjectName,
                                               UNSIGNED16 usPropertyID,
                                               VOID       *pvProperty,
                                               UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDDeleteArticle( ADSHANDLE    hDictionary,
                                          UNSIGNED8    *pucPublicationName,
                                          UNSIGNED8    *pucObjectName );
typedef UNSIGNED32 (WINAPI *ADSDDDELETEARTICLE_PTR)( ADSHANDLE    hDictionary,
                                          UNSIGNED8    *pucPublicationName,
                                          UNSIGNED8    *pucObjectName );

UNSIGNED32 ENTRYPOINT AdsDDCreateSubscription( ADSHANDLE      hDictionary,
                                               UNSIGNED8      *pucSubscriptionName,
                                               UNSIGNED8      *pucPublicationName,
                                               UNSIGNED8      *pucTarget,
                                               UNSIGNED8      *pucUser,
                                               UNSIGNED8      *pucPassword,
                                               UNSIGNED8      *pucReplicationQueue,
                                               UNSIGNED16     usForward,
                                               UNSIGNED8      *pucComments,
                                               UNSIGNED32     ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDCREATESUBSCRIPTION_PTR)( ADSHANDLE      hDictionary,
                                               UNSIGNED8      *pucSubscriptionName,
                                               UNSIGNED8      *pucPublicationName,
                                               UNSIGNED8      *pucTarget,
                                               UNSIGNED8      *pucUser,
                                               UNSIGNED8      *pucPassword,
                                               UNSIGNED8      *pucReplicationQueue,
                                               UNSIGNED16     usForward,
                                               UNSIGNED8      *pucComments,
                                               UNSIGNED32     ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDGetSubscriptionProperty( ADSHANDLE  hObject,
                                                    UNSIGNED8  *pucSubscriptionName,
                                                    UNSIGNED16 usPropertyID,
                                                    VOID       *pvProperty,
                                                    UNSIGNED16 *pusPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDGETSUBSCRIPTIONPROPERTY_PTR)( ADSHANDLE  hObject,
                                                    UNSIGNED8  *pucSubscriptionName,
                                                    UNSIGNED16 usPropertyID,
                                                    VOID       *pvProperty,
                                                    UNSIGNED16 *pusPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDSetSubscriptionProperty( ADSHANDLE  hDictionary,
                                                    UNSIGNED8  *pucSubscriptionName,
                                                    UNSIGNED16 usPropertyID,
                                                    VOID       *pvProperty,
                                                    UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETSUBSCRIPTIONPROPERTY_PTR)( ADSHANDLE  hDictionary,
                                                    UNSIGNED8  *pucSubscriptionName,
                                                    UNSIGNED16 usPropertyID,
                                                    VOID       *pvProperty,
                                                    UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsDDDeleteSubscription( ADSHANDLE    hDictionary,
                                               UNSIGNED8    *pucSubscriptionName );
typedef UNSIGNED32 (WINAPI *ADSDDDELETESUBSCRIPTION_PTR)( ADSHANDLE    hDictionary,
                                               UNSIGNED8    *pucSubscriptionName );


UNSIGNED32 ENTRYPOINT AdsDecryptRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSDECRYPTRECORD_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsDecryptTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSDECRYPTTABLE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsDeleteCustomKey( ADSHANDLE hIndex );
typedef UNSIGNED32 (WINAPI *ADSDELETECUSTOMKEY_PTR)( ADSHANDLE hIndex );

UNSIGNED32 ENTRYPOINT AdsDeleteIndex( ADSHANDLE hIndex );
typedef UNSIGNED32 (WINAPI *ADSDELETEINDEX_PTR)( ADSHANDLE hIndex );

UNSIGNED32 ENTRYPOINT AdsDeleteRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSDELETERECORD_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsGetKeyColumn( ADSHANDLE  hCursor,
                                       UNSIGNED8  *pucKeyColumn,
                                       UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETKEYCOLUMN_PTR)( ADSHANDLE  hCursor,
                                                  UNSIGNED8  *pucKeyColumn,
                                                  UNSIGNED16 *pusLen);

UNSIGNED32 ENTRYPOINT AdsDisableEncryption( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSDISABLEENCRYPTION_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsDisableLocalConnections( void );
typedef UNSIGNED32 (WINAPI *ADSDISABLELOCALCONNECTIONS_PTR)( void );

UNSIGNED32 ENTRYPOINT AdsDisconnect( ADSHANDLE hConnect );
typedef UNSIGNED32 (WINAPI *ADSDISCONNECT_PTR)( ADSHANDLE hConnect );

UNSIGNED32 ENTRYPOINT AdsEnableEncryption( ADSHANDLE hTable,
                                           UNSIGNED8 *pucPassword );
typedef UNSIGNED32 (WINAPI *ADSENABLEENCRYPTION_PTR)( ADSHANDLE hTable,
                                           UNSIGNED8 *pucPassword );

UNSIGNED32 ENTRYPOINT AdsEncryptRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSENCRYPTRECORD_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsEncryptTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSENCRYPTTABLE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsEvalLogicalExpr(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pbResult );
typedef UNSIGNED32 (WINAPI *ADSEVALLOGICALEXPR_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pbResult );

UNSIGNED32 ENTRYPOINT AdsEvalNumericExpr(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              DOUBLE           *pdResult );
typedef UNSIGNED32 (WINAPI *ADSEVALNUMERICEXPR_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              DOUBLE           *pdResult );

UNSIGNED32 ENTRYPOINT AdsEvalStringExpr(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED8        *pucResult,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSEVALSTRINGEXPR_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED8        *pucResult,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsEvalTestExpr(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pusType );
typedef UNSIGNED32 (WINAPI *ADSEVALTESTEXPR_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pusType );

UNSIGNED32 ENTRYPOINT AdsExtractKey(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucKey,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSEXTRACTKEY_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucKey,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsFailedTransactionRecovery( UNSIGNED8 *pucServer );
typedef UNSIGNED32 (WINAPI *ADSFAILEDTRANSACTIONRECOVERY_PTR)( UNSIGNED8 *pucServer );

UNSIGNED32 ENTRYPOINT AdsFileToBinary(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       usBinaryType,
                              UNSIGNED8        *pucFileName );
typedef UNSIGNED32 (WINAPI *ADSFILETOBINARY_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       usBinaryType,
                              UNSIGNED8        *pucFileName );

UNSIGNED32 ENTRYPOINT AdsFindConnection(
                              UNSIGNED8        *pucServerName,
                              ADSHANDLE        *phConnect );
typedef UNSIGNED32 (WINAPI *ADSFINDCONNECTION_PTR)(
                              UNSIGNED8        *pucServerName,
                              ADSHANDLE        *phConnect );

UNSIGNED32 ENTRYPOINT AdsFindConnection25(
                              UNSIGNED8        *pucFullPath,
                              ADSHANDLE        *phConnect );
typedef UNSIGNED32 (WINAPI *ADSFINDCONNECTION25_PTR)(
                              UNSIGNED8        *pucFullPath,
                              ADSHANDLE        *phConnect );

UNSIGNED32 ENTRYPOINT AdsFindClose( ADSHANDLE hConnect, ADSHANDLE lHandle );
typedef UNSIGNED32 (WINAPI *ADSFINDCLOSE_PTR)( ADSHANDLE hConnect, ADSHANDLE lHandle );

UNSIGNED32 ENTRYPOINT AdsFindFirstTable( ADSHANDLE  hConnect,
                                         UNSIGNED8  *pucFileMask,
                                         UNSIGNED8  *pucFirstFile,
                                         UNSIGNED16 *pusFileLen,
                                         ADSHANDLE  *plHandle );
typedef UNSIGNED32 (WINAPI *ADSFINDFIRSTTABLE_PTR)( ADSHANDLE  hConnect,
                                         UNSIGNED8  *pucFileMask,
                                         UNSIGNED8  *pucFirstFile,
                                         UNSIGNED16 *pusFileLen,
                                         ADSHANDLE  *plHandle );

UNSIGNED32 ENTRYPOINT AdsFindNextTable( ADSHANDLE hConnect,
                                        ADSHANDLE lHandle,
                                        UNSIGNED8 *pucFileName,
                                        UNSIGNED16 *pusFileLen );
typedef UNSIGNED32 (WINAPI *ADSFINDNEXTTABLE_PTR)( ADSHANDLE hConnect,
                                        ADSHANDLE lHandle,
                                        UNSIGNED8 *pucFileName,
                                        UNSIGNED16 *pusFileLen );

UNSIGNED32 ENTRYPOINT AdsFindFirstTable62( ADSHANDLE  hConnect,
                                           UNSIGNED8  *pucFileMask,
                                           UNSIGNED8  *pucFirstDD,
                                           UNSIGNED16 *pusDDLen,
                                           UNSIGNED8  *pucFirstFile,
                                           UNSIGNED16 *pusFileLen,
                                           ADSHANDLE  *plHandle );
typedef UNSIGNED32 (WINAPI *ADSFINDFIRSTTABLE62_PTR)( ADSHANDLE  hConnect,
                                           UNSIGNED8  *pucFileMask,
                                           UNSIGNED8  *pucFirstDD,
                                           UNSIGNED16 *pusDDLen,
                                           UNSIGNED8  *pucFirstFile,
                                           UNSIGNED16 *pusFileLen,
                                           ADSHANDLE  *plHandle );

UNSIGNED32 ENTRYPOINT AdsFindNextTable62( ADSHANDLE hConnect,
                                          ADSHANDLE lHandle,
                                          UNSIGNED8 *pucDDName,
                                          UNSIGNED16 *pusDDLen,
                                          UNSIGNED8 *pucFileName,
                                          UNSIGNED16 *pusFileLen );
typedef UNSIGNED32 (WINAPI *ADSFINDNEXTTABLE62_PTR)( ADSHANDLE hConnect,
                                          ADSHANDLE lHandle,
                                          UNSIGNED8 *pucDDName,
                                          UNSIGNED16 *pusDDLen,
                                          UNSIGNED8 *pucFileName,
                                          UNSIGNED16 *pusFileLen );

UNSIGNED32 ENTRYPOINT AdsGetAllIndexes(
                              ADSHANDLE        hTable,
                              ADSHANDLE        ahIndex[],
                              UNSIGNED16       *pusArrayLen );
typedef UNSIGNED32 (WINAPI *ADSGETALLINDEXES_PTR)(
                              ADSHANDLE        hTable,
                              ADSHANDLE        ahIndex[],
                              UNSIGNED16       *pusArrayLen );

UNSIGNED32 ENTRYPOINT AdsGetFTSIndexes(
                              ADSHANDLE        hTable,
                              ADSHANDLE        ahIndex[],
                              UNSIGNED16       *pusArrayLen );
typedef UNSIGNED32 (WINAPI *ADSGETFTSINDEXES_PTR)(
                              ADSHANDLE        hTable,
                              ADSHANDLE        ahIndex[],
                              UNSIGNED16       *pusArrayLen );

UNSIGNED32 ENTRYPOINT AdsGetFTSIndexInfo(
                              ADSHANDLE   hIndex,
                              UNSIGNED8   *pucOutput,
                              UNSIGNED32  *pulBufLen,
                              UNSIGNED8   **ppucField,
                              UNSIGNED32  *pulMinWordLen,
                              UNSIGNED32  *pulMaxWordLen,
                              UNSIGNED8   **ppucDelimiters,
                              UNSIGNED8   **ppucNoiseWords,
                              UNSIGNED8   **ppucDropChars,
                              UNSIGNED8   **ppucConditionalChars,
                              UNSIGNED8   **ppucReserved1,
                              UNSIGNED8   **ppucReserved2,
                              UNSIGNED32  *pulOptions );

typedef UNSIGNED32 (WINAPI *ADSGETFTSINDEXINFO_PTR)(
                              ADSHANDLE   hIndex,
                              UNSIGNED8   *pucOutput,
                              UNSIGNED32  *pulBufLen,
                              UNSIGNED8   **ppucField,
                              UNSIGNED32  *pulMinWordLen,
                              UNSIGNED32  *pulMaxWordLen,
                              UNSIGNED8   **ppucDelimiters,
                              UNSIGNED8   **ppucNoiseWords,
                              UNSIGNED8   **ppucDropChars,
                              UNSIGNED8   **ppucConditionalChars,
                              UNSIGNED8   **ppucReserved1,
                              UNSIGNED8   **ppucReserved2,
                              UNSIGNED32  *pulOptions );

UNSIGNED32 ENTRYPOINT AdsGetAllLocks(
                              ADSHANDLE        hTable,
                              UNSIGNED32       aulLocks[],
                              UNSIGNED16       *pusArrayLen );
typedef UNSIGNED32 (WINAPI *ADSGETALLLOCKS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       aulLocks[],
                              UNSIGNED16       *pusArrayLen );

UNSIGNED32 ENTRYPOINT AdsGetAllTables(
                              ADSHANDLE        ahTable[],
                              UNSIGNED16       *pusArrayLen );
typedef UNSIGNED32 (WINAPI *ADSGETALLTABLES_PTR)(
                              ADSHANDLE        ahTable[],
                              UNSIGNED16       *pusArrayLen );

UNSIGNED32 ENTRYPOINT AdsGetBinary(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       ulOffset,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       *pulLen );
typedef UNSIGNED32 (WINAPI *ADSGETBINARY_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       ulOffset,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       *pulLen );

UNSIGNED32 ENTRYPOINT AdsGetBinaryLength(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETBINARYLENGTH_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsGetBookmark(
                              ADSHANDLE        hTable,
                              ADSHANDLE        *phBookmark );
typedef UNSIGNED32 (WINAPI *ADSGETBOOKMARK_PTR)(
                              ADSHANDLE        hTable,
                              ADSHANDLE        *phBookmark );

UNSIGNED32 ENTRYPOINT AdsGetBookmark60(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucBookmark,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETBOOKMARK60_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucBookmark,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsGetBookmarkLength(
                              ADSHANDLE        hObj,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETBOOKMARKLENGTH_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsCompareBookmarks(
                              UNSIGNED8        *pucBookmark1,
                              UNSIGNED8        *pucBookmark2,
                              SIGNED32         *plResult );
typedef UNSIGNED32 (WINAPI *ADSCOMPAREBOOKMARKS_PTR)(
                              UNSIGNED8        *pucBookmark1,
                              UNSIGNED8        *pucBookmark2,
                              SIGNED32         *plResult );

UNSIGNED32 ENTRYPOINT AdsGetCollationLang(
                              UNSIGNED8  *pucLang,
                              UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETCOLLATIONLANG_PTR)(
                              UNSIGNED8  *pucLang,
                              UNSIGNED16 *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetCollation(
                              ADSHANDLE  hConnect,
                              UNSIGNED8  *pucCollation,
                              UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETCOLLATION_PTR)(
                              ADSHANDLE  hConnect,
                              UNSIGNED8  *pucCollation,
                              UNSIGNED16 *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetConnectionType(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       *pusConnectType );
typedef UNSIGNED32 (WINAPI *ADSGETCONNECTIONTYPE_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       *pusConnectType );

UNSIGNED32 ENTRYPOINT AdsGetConnectionPath(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucConnectionPath,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETCONNECTIONPATH_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucConnectionPath,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetConnectionProperty(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       usPropertyID,
                              VOID             *pvProperty,
                              UNSIGNED32       *pulPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSGETCONNECTIONPROPERTY_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       usPropertyID,
                              VOID             *pvProperty,
                              UNSIGNED32       *pulPropertyLen );

UNSIGNED32 ENTRYPOINT AdsGetDate(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETDATE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetDateFormat( UNSIGNED8  *pucFormat,
                                        UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETDATEFORMAT_PTR)( UNSIGNED8   *pucFormat,
                                                   UNSIGNED16  *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetDateFormat60( ADSHANDLE  hConnect,
                                          UNSIGNED8  *pucFormat,
                                          UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETDATEFORMAT60_PTR)( ADSHANDLE  hConnect,
                                                     UNSIGNED8  *pucFormat,
                                                     UNSIGNED16 *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetDecimals( UNSIGNED16 *pusDecimals );
typedef UNSIGNED32 (WINAPI *ADSGETDECIMALS_PTR)( UNSIGNED16 *pusDecimals );

UNSIGNED32 ENTRYPOINT AdsGetDefault(
                              UNSIGNED8        *pucDefault,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETDEFAULT_PTR)(
                              UNSIGNED8        *pucDefault,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetDeleted( UNSIGNED16 *pbUseDeleted );
typedef UNSIGNED32 (WINAPI *ADSGETDELETED_PTR)( UNSIGNED16 *pbUseDeleted );

UNSIGNED32 ENTRYPOINT AdsGetDouble(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              DOUBLE           *pdValue );
typedef UNSIGNED32 (WINAPI *ADSGETDOUBLE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              DOUBLE           *pdValue );

UNSIGNED32 ENTRYPOINT AdsGetEpoch( UNSIGNED16 *pusCentury );
typedef UNSIGNED32 (WINAPI *ADSGETEPOCH_PTR)( UNSIGNED16 *pusCentury );

UNSIGNED32 ENTRYPOINT AdsGetErrorString(
                              UNSIGNED32       ulErrCode,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusBufLen );
typedef UNSIGNED32 (WINAPI *ADSGETERRORSTRING_PTR)(
                              UNSIGNED32       ulErrCode,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusBufLen );

UNSIGNED32 ENTRYPOINT AdsGetExact( UNSIGNED16 *pbExact );
typedef UNSIGNED32 (WINAPI *ADSGETEXACT_PTR)( UNSIGNED16 *pbExact );

UNSIGNED32 ENTRYPOINT AdsGetExact22(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pbExact );
typedef UNSIGNED32 (WINAPI *ADSGETEXACT22_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pbExact );

UNSIGNED32 ENTRYPOINT AdsGetField(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       *pulLen,
                              UNSIGNED16       usOption );
typedef UNSIGNED32 (WINAPI *ADSGETFIELD_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       *pulLen,
                              UNSIGNED16       usOption );

UNSIGNED32 ENTRYPOINT AdsGetFieldDecimals(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusDecimals );
typedef UNSIGNED32 (WINAPI *ADSGETFIELDDECIMALS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusDecimals );

UNSIGNED32 ENTRYPOINT AdsGetFieldLength(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETFIELDLENGTH_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsGetFieldName(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usFld,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusBufLen );
typedef UNSIGNED32 (WINAPI *ADSGETFIELDNAME_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usFld,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusBufLen );

UNSIGNED32 ENTRYPOINT AdsGetFieldNum(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusNum );
typedef UNSIGNED32 (WINAPI *ADSGETFIELDNUM_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusNum );

UNSIGNED32 ENTRYPOINT AdsGetFieldOffset(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulOffset );
typedef UNSIGNED32 (WINAPI *ADSGETFIELDOFFSET_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulOffset );

UNSIGNED32 ENTRYPOINT AdsGetFieldType(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusType );
typedef UNSIGNED32 (WINAPI *ADSGETFIELDTYPE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusType );

UNSIGNED32 ENTRYPOINT AdsGetFilter(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFilter,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETFILTER_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFilter,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetHandleLong(
                              ADSHANDLE        hObj,
                              UNSIGNED32       *pulVal );
typedef UNSIGNED32 (WINAPI *ADSGETHANDLELONG_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED32       *pulVal );

UNSIGNED32 ENTRYPOINT AdsGetHandleType(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pusType );
typedef UNSIGNED32 (WINAPI *ADSGETHANDLETYPE_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pusType );

UNSIGNED32 ENTRYPOINT AdsGetIndexCondition(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXCONDITION_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetIndexExpr(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXEXPR_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetIndexFilename(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usOption,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXFILENAME_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usOption,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetIndexHandle(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucIndexOrder,
                              ADSHANDLE        *phIndex );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXHANDLE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucIndexOrder,
                              ADSHANDLE        *phIndex );

UNSIGNED32 ENTRYPOINT AdsGetIndexHandleByOrder(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usOrderNum,
                              ADSHANDLE        *phIndex );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXHANDLEBYORDER_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usOrderNum,
                              ADSHANDLE        *phIndex );


UNSIGNED32 ENTRYPOINT AdsGetIndexHandleByExpr(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED32       ulDescending,
                              ADSHANDLE        *phIndex );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXHANDLEBYEXPR_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED32       ulDescending,
                              ADSHANDLE        *phIndex );


UNSIGNED32 ENTRYPOINT AdsGetIndexName(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXNAME_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetIndexOrderByHandle( ADSHANDLE   hIndex,
                                                UNSIGNED16  *pusIndexOrder );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXORDERBYHANDLE_PTR)( ADSHANDLE   hIndex,
                                                UNSIGNED16  *pusIndexOrder );

UNSIGNED32 ENTRYPOINT AdsGetJulian(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED32         *plDate );
typedef UNSIGNED32 (WINAPI *ADSGETJULIAN_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED32         *plDate );

UNSIGNED32 ENTRYPOINT AdsGetKeyCount(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulCount );
typedef UNSIGNED32 (WINAPI *ADSGETKEYCOUNT_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulCount );

UNSIGNED32 ENTRYPOINT AdsGetKeyNum(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulKey );
typedef UNSIGNED32 (WINAPI *ADSGETKEYNUM_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulKey );

UNSIGNED32 ENTRYPOINT AdsGetKeyLength(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pusKeyLength );
typedef UNSIGNED32 (WINAPI *ADSGETKEYLENGTH_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pusKeyLength );

UNSIGNED32 ENTRYPOINT AdsGetKeyType(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *usKeyType );
typedef UNSIGNED32 (WINAPI *ADSGETKEYTYPE_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *usKeyType );

UNSIGNED32 ENTRYPOINT AdsGetLastError(
                              UNSIGNED32       *pulErrCode,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusBufLen );
typedef UNSIGNED32 (WINAPI *ADSGETLASTERROR_PTR)(
                              UNSIGNED32       *pulErrCode,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusBufLen );

UNSIGNED32 ENTRYPOINT AdsGetLastTableUpdate(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucDate,
                              UNSIGNED16       *pusDateLen );
typedef UNSIGNED32 (WINAPI *ADSGETLASTTABLEUPDATE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucDate,
                              UNSIGNED16       *pusDateLen );

UNSIGNED32 ENTRYPOINT AdsGetLogical(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pbValue );
typedef UNSIGNED32 (WINAPI *ADSGETLOGICAL_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pbValue );

UNSIGNED32 ENTRYPOINT AdsGetLong(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED32         *plValue );
typedef UNSIGNED32 (WINAPI *ADSGETLONG_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED32         *plValue );

UNSIGNED32 ENTRYPOINT AdsGetLongLong(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED64         *pqValue );
typedef UNSIGNED32 (WINAPI *ADSGETLONGLONG_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED64         *pqValue );

UNSIGNED32 ENTRYPOINT AdsGetMemoBlockSize(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusBlockSize );
typedef UNSIGNED32 (WINAPI *ADSGETMEMOBLOCKSIZE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusBlockSize );

UNSIGNED32 ENTRYPOINT AdsGetMemoLength(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETMEMOLENGTH_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsGetMemoDataType(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusType );
typedef UNSIGNED32 (WINAPI *ADSGETMEMODATATYPE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pusType );

UNSIGNED32 ENTRYPOINT AdsGetMilliseconds(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED32         *plTime );
typedef UNSIGNED32 (WINAPI *ADSGETMILLISECONDS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED32         *plTime );

UNSIGNED32 ENTRYPOINT AdsGetMoney(
                              ADSHANDLE  hTbl,
                              UNSIGNED8  *pucFldName,
                              SIGNED64   *pqValue );
typedef UNSIGNED32 (WINAPI *ADSGETMONEY_PTR)(
                              ADSHANDLE  hTbl,
                              UNSIGNED8  *pucFldName,
                              SIGNED64   *pqValue );

UNSIGNED32 ENTRYPOINT AdsGetActiveLinkInfo(
                              ADSHANDLE         hDBConn,
                              UNSIGNED16        usLinkNum,
                              UNSIGNED8         *pucLinkInfo,
                              UNSIGNED16        *pusBufferLen );
typedef UNSIGNED32 (WINAPI *ADSGETACTIVELINKINFO_PTR)(
                              ADSHANDLE         hDBConn,
                              UNSIGNED16        usLinkNum,
                              UNSIGNED8         *pucLinkInfo,
                              UNSIGNED16        *pusBufferLen );

UNSIGNED32 ENTRYPOINT AdsGetNumActiveLinks(
                              ADSHANDLE         hDBConn,
                              UNSIGNED16        *pusNumLinks );
typedef UNSIGNED32 (WINAPI *ADSGETNUMACTIVELINKS_PTR)(
                              ADSHANDLE         hDBConn,
                              UNSIGNED16        *pusNumLinks );

UNSIGNED32 ENTRYPOINT AdsGetNumFields(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusCount );
typedef UNSIGNED32 (WINAPI *ADSGETNUMFIELDS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusCount );

UNSIGNED32 ENTRYPOINT AdsGetNumIndexes(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusNum );
typedef UNSIGNED32 (WINAPI *ADSGETNUMINDEXES_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusNum );

UNSIGNED32 ENTRYPOINT AdsGetNumFTSIndexes(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusNum );
typedef UNSIGNED32 (WINAPI *ADSGETNUMFTSINDEXES_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusNum );

UNSIGNED32 ENTRYPOINT AdsGetNumLocks(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusNum );
typedef UNSIGNED32 (WINAPI *ADSGETNUMLOCKS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusNum );

UNSIGNED32 ENTRYPOINT AdsGetNumOpenTables( UNSIGNED16 *pusNum );
typedef UNSIGNED32 (WINAPI *ADSGETNUMOPENTABLES_PTR)( UNSIGNED16 *pusNum );

UNSIGNED32 ENTRYPOINT AdsGetRecord(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucRec,
                              UNSIGNED32       *pulLen );
typedef UNSIGNED32 (WINAPI *ADSGETRECORD_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucRec,
                              UNSIGNED32       *pulLen );

UNSIGNED32 ENTRYPOINT AdsGetRecordCount(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulCount );
typedef UNSIGNED32 (WINAPI *ADSGETRECORDCOUNT_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulCount );

UNSIGNED32 ENTRYPOINT AdsGetRecordNum(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulRec );
typedef UNSIGNED32 (WINAPI *ADSGETRECORDNUM_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usFilterOption,
                              UNSIGNED32       *pulRec );

UNSIGNED32 ENTRYPOINT AdsGetRecordLength(
                              ADSHANDLE        hTable,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETRECORDLENGTH_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsGetRecordCRC(
                              ADSHANDLE        hTable,
                              UNSIGNED32       *pulCRC,
                              UNSIGNED32       ulOptions );
typedef UNSIGNED32 (WINAPI *ADSGETRECORDCRC_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       *pulCRC,
                              UNSIGNED32       ulOptions );

UNSIGNED32 ENTRYPOINT AdsGetRelKeyPos(
                              ADSHANDLE        hIndex,
                              DOUBLE           *pdPos );
typedef UNSIGNED32 (WINAPI *ADSGETRELKEYPOS_PTR)(
                              ADSHANDLE        hIndex,
                              DOUBLE           *pdPos );

UNSIGNED32 ENTRYPOINT AdsGetScope(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usScopeOption,
                              UNSIGNED8        *pucScope,
                              UNSIGNED16       *pusBufLen );
typedef UNSIGNED32 (WINAPI *ADSGETSCOPE_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       usScopeOption,
                              UNSIGNED8        *pucScope,
                              UNSIGNED16       *pusBufLen );

UNSIGNED32 ENTRYPOINT AdsGetSearchPath(
                              UNSIGNED8        *pucPath,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETSEARCHPATH_PTR)(
                              UNSIGNED8        *pucPath,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetServerName(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETSERVERNAME_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetServerTime(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucDateBuf,
                              UNSIGNED16       *pusDateBufLen,
                              SIGNED32         *plTime,
                              UNSIGNED8        *pucTimeBuf,
                              UNSIGNED16       *pusTimeBufLen );
typedef UNSIGNED32 (WINAPI *ADSGETSERVERTIME_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucDateBuf,
                              UNSIGNED16       *pusDateBufLen,
                              SIGNED32         *plTime,
                              UNSIGNED8        *pucTimeBuf,
                              UNSIGNED16       *pusTimeBufLen );

UNSIGNED32 ENTRYPOINT AdsGetShort(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED16         *psValue );
typedef UNSIGNED32 (WINAPI *ADSGETSHORT_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              SIGNED16         *psValue );

UNSIGNED32 ENTRYPOINT AdsGetString(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       *pulLen,
                              UNSIGNED16       usOption );
typedef UNSIGNED32 (WINAPI *ADSGETSTRING_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       *pulLen,
                              UNSIGNED16       usOption );

UNSIGNED32 ENTRYPOINT AdsGetTableAlias(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucAlias,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETTABLEALIAS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucAlias,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetTableCharType(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusCharType );
typedef UNSIGNED32 (WINAPI *ADSGETTABLECHARTYPE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusCharType );

UNSIGNED32 ENTRYPOINT AdsGetTableConnection(
                              ADSHANDLE        hTable,
                              ADSHANDLE        *phConnect );
typedef UNSIGNED32 (WINAPI *ADSGETTABLECONNECTION_PTR)(
                              ADSHANDLE        hTable,
                              ADSHANDLE        *phConnect );

UNSIGNED32 ENTRYPOINT AdsGetTableFilename(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usOption,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETTABLEFILENAME_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       usOption,
                              UNSIGNED8        *pucName,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetTableHandle(
                              UNSIGNED8        *pucName,
                              ADSHANDLE        *phTable );
typedef UNSIGNED32 (WINAPI *ADSGETTABLEHANDLE_PTR)(
                              UNSIGNED8        *pucName,
                              ADSHANDLE        *phTable );

UNSIGNED32 ENTRYPOINT AdsGetTableHandle25(
                              ADSHANDLE   hConnect,
                              UNSIGNED8   *pucName,
                              ADSHANDLE   *phTable );
typedef UNSIGNED32 (WINAPI *ADSGETTABLEHANDLE25_PTR)(
                              ADSHANDLE   hConnect,
                              UNSIGNED8   *pucName,
                              ADSHANDLE   *phTable );

UNSIGNED32 ENTRYPOINT AdsGetTableLockType(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusLockType );
typedef UNSIGNED32 (WINAPI *ADSGETTABLELOCKTYPE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusLockType );

UNSIGNED32 ENTRYPOINT AdsGetTableMemoSize(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusMemoSize );
typedef UNSIGNED32 (WINAPI *ADSGETTABLEMEMOSIZE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusMemoSize );

UNSIGNED32 ENTRYPOINT AdsGetTableOpenOptions(
                              ADSHANDLE        hTable,
                              UNSIGNED32       *pulOptions );
typedef UNSIGNED32 (WINAPI *ADSGETTABLEOPENOPTIONS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       *pulOptions );

UNSIGNED32 ENTRYPOINT AdsGetTableRights(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusRights );
typedef UNSIGNED32 (WINAPI *ADSGETTABLERIGHTS_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusRights );

UNSIGNED32 ENTRYPOINT AdsGetTableType(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusType );
typedef UNSIGNED32 (WINAPI *ADSGETTABLETYPE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pusType );

UNSIGNED32 ENTRYPOINT AdsGetTime(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETTIME_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetVersion(
                              UNSIGNED32       *pulMajor,
                              UNSIGNED32       *pulMinor,
                              UNSIGNED8        *pucLetter,
                              UNSIGNED8        *pucDesc,
                              UNSIGNED16       *pusDescLen );
typedef UNSIGNED32 (WINAPI *ADSGETVERSION_PTR)(
                              UNSIGNED32       *pulMajor,
                              UNSIGNED32       *pulMinor,
                              UNSIGNED8        *pucLetter,
                              UNSIGNED8        *pucDesc,
                              UNSIGNED16       *pusDescLen );

UNSIGNED32 ENTRYPOINT AdsGotoBookmark(
                              ADSHANDLE        hTable,
                              ADSHANDLE        hBookmark );
typedef UNSIGNED32 (WINAPI *ADSGOTOBOOKMARK_PTR)(
                              ADSHANDLE        hTable,
                              ADSHANDLE        hBookmark );

UNSIGNED32 ENTRYPOINT AdsGotoBookmark60(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucBookmark );
typedef UNSIGNED32 (WINAPI *ADSGOTOBOOKMARK60_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucBookmark );

UNSIGNED32 ENTRYPOINT AdsGotoBottom( ADSHANDLE hObj );
typedef UNSIGNED32 (WINAPI *ADSGOTOBOTTOM_PTR)( ADSHANDLE hObj );

UNSIGNED32 ENTRYPOINT AdsGotoRecord(
                              ADSHANDLE        hTable,
                              UNSIGNED32       ulRec );
typedef UNSIGNED32 (WINAPI *ADSGOTORECORD_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       ulRec );

UNSIGNED32 ENTRYPOINT AdsGotoTop( ADSHANDLE hObj );
typedef UNSIGNED32 (WINAPI *ADSGOTOTOP_PTR)( ADSHANDLE hObj );

UNSIGNED32 ENTRYPOINT AdsImageToClipboard( ADSHANDLE hTable,
                                           UNSIGNED8 *pucFldName );
typedef UNSIGNED32 (WINAPI *ADSIMAGETOCLIPBOARD_PTR)( ADSHANDLE hTable,
                                           UNSIGNED8 *pucFldName );

UNSIGNED32 ENTRYPOINT AdsInTransaction(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       *pbInTrans );
typedef UNSIGNED32 (WINAPI *ADSINTRANSACTION_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED16       *pbInTrans );

UNSIGNED32 ENTRYPOINT AdsIsEmpty(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pbEmpty );
typedef UNSIGNED32 (WINAPI *ADSISEMPTY_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       *pbEmpty );

UNSIGNED32 ENTRYPOINT AdsIsExprValid(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pbValid );
typedef UNSIGNED32 (WINAPI *ADSISEXPRVALID_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       *pbValid );

UNSIGNED32 ENTRYPOINT AdsIsFound(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pbFound );
typedef UNSIGNED32 (WINAPI *ADSISFOUND_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pbFound );

UNSIGNED32 ENTRYPOINT AdsIsIndexCompound(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbCompound );
typedef UNSIGNED32 (WINAPI *ADSISINDEXCOMPOUND_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbCompound );

UNSIGNED32 ENTRYPOINT AdsIsIndexCandidate(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbCandidate );
typedef UNSIGNED32 (WINAPI *ADSISINDEXCANDIDATE_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbCandidate );

UNSIGNED32 ENTRYPOINT AdsIsIndexNullable(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbNullable );
typedef UNSIGNED32 (WINAPI *ADSISINDEXNULLABLE_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbNullable );

UNSIGNED32 ENTRYPOINT AdsIsIndexCustom(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbCustom );
typedef UNSIGNED32 (WINAPI *ADSISINDEXCUSTOM_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbCustom );

UNSIGNED32 ENTRYPOINT AdsIsIndexDescending(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbDescending );
typedef UNSIGNED32 (WINAPI *ADSISINDEXDESCENDING_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbDescending );

UNSIGNED32 ENTRYPOINT AdsIsIndexPrimaryKey(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbPrimaryKey );
typedef UNSIGNED32 (WINAPI *ADSISINDEXPRIMARYKEY_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbPrimaryKey );

UNSIGNED32 ENTRYPOINT AdsIsIndexFTS(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbFTS );
typedef UNSIGNED32 (WINAPI *ADSISINDEXFTS_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbFTS );

UNSIGNED32 ENTRYPOINT AdsIsIndexUnique(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbUnique );
typedef UNSIGNED32 (WINAPI *ADSISINDEXUNIQUE_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED16       *pbUnique );

UNSIGNED32 ENTRYPOINT AdsIsRecordDeleted(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbDeleted );
typedef UNSIGNED32 (WINAPI *ADSISRECORDDELETED_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbDeleted );

UNSIGNED32 ENTRYPOINT AdsIsRecordEncrypted(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbEncrypted );
typedef UNSIGNED32 (WINAPI *ADSISRECORDENCRYPTED_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbEncrypted );

UNSIGNED32 ENTRYPOINT AdsIsRecordLocked(
                              ADSHANDLE        hTable,
                              UNSIGNED32       ulRec,
                              UNSIGNED16       *pbLocked );
typedef UNSIGNED32 (WINAPI *ADSISRECORDLOCKED_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       ulRec,
                              UNSIGNED16       *pbLocked );

UNSIGNED32 ENTRYPOINT AdsIsRecordVisible(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pbVisible );
typedef UNSIGNED32 (WINAPI *ADSISRECORDVISIBLE_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED16       *pbVisible );

UNSIGNED32 ENTRYPOINT AdsIsServerLoaded(
                              UNSIGNED8        *pucServer,
                              UNSIGNED16       *pbLoaded );
typedef UNSIGNED32 (WINAPI *ADSISSERVERLOADED_PTR)(
                              UNSIGNED8        *pucServer,
                              UNSIGNED16       *pbLoaded );

UNSIGNED32 ENTRYPOINT AdsIsTableEncrypted(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbEncrypted );
typedef UNSIGNED32 (WINAPI *ADSISTABLEENCRYPTED_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbEncrypted );

UNSIGNED32 ENTRYPOINT AdsIsTableLocked(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbLocked );
typedef UNSIGNED32 (WINAPI *ADSISTABLELOCKED_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED16       *pbLocked );

UNSIGNED32 ENTRYPOINT AdsLocate(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       bForward,
                              UNSIGNED16       *pbFound );
typedef UNSIGNED32 (WINAPI *ADSLOCATE_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucExpr,
                              UNSIGNED16       bForward,
                              UNSIGNED16       *pbFound );

UNSIGNED32 ENTRYPOINT AdsLockRecord(
                              ADSHANDLE        hTable,
                              UNSIGNED32       ulRec );
typedef UNSIGNED32 (WINAPI *ADSLOCKRECORD_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED32       ulRec );

UNSIGNED32 ENTRYPOINT AdsLockTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSLOCKTABLE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsLookupKey( ADSHANDLE  hIndex,
                                    UNSIGNED8 *pucKey,
                                    UNSIGNED16 usKeyLen,
                                    UNSIGNED16 usDataType,
                                    UNSIGNED16 *pbFound );
typedef UNSIGNED32 (WINAPI *ADSLOOKUPKEY_PTR)( ADSHANDLE  hIndex,
                                    UNSIGNED8 *pucKey,
                                    UNSIGNED16 usKeyLen,
                                    UNSIGNED16 usDataType,
                                    UNSIGNED16 *pbFound );

UNSIGNED32 ENTRYPOINT AdsMgConnect( UNSIGNED8   *pucServerName,
                                    UNSIGNED8   *pucUserName,
                                    UNSIGNED8   *pucPassword,
                                    ADSHANDLE   *phMgmtHandle );
typedef UNSIGNED32 (WINAPI *ADSMGCONNECT_PTR)( UNSIGNED8   *pucServerName,
                                    UNSIGNED8   *pucUserName,
                                    UNSIGNED8   *pucPassword,
                                    ADSHANDLE   *phMgmtHandle );

UNSIGNED32 ENTRYPOINT AdsMgDisconnect( ADSHANDLE   hMgmtHandle );
typedef UNSIGNED32 (WINAPI *ADSMGDISCONNECT_PTR)( ADSHANDLE   hMgmtHandle );


UNSIGNED32 ENTRYPOINT AdsMgGetCommStats( ADSHANDLE           hMgmtHandle,
                                         ADS_MGMT_COMM_STATS *pstCommStats,
                                         UNSIGNED16          *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETCOMMSTATS_PTR)( ADSHANDLE           hMgmtHandle,
                                         ADS_MGMT_COMM_STATS *pstCommStats,
                                         UNSIGNED16          *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgResetCommStats( ADSHANDLE hMgmtHandle );
typedef UNSIGNED32 (WINAPI *ADSMGRESETCOMMSTATS_PTR)( ADSHANDLE hMgmtHandle );

UNSIGNED32 ENTRYPOINT AdsMgDumpInternalTables( ADSHANDLE  hMgmtHandle );
typedef UNSIGNED32 (WINAPI *ADSMGDUMPINTERNALTABLES_PTR)( ADSHANDLE  hMgmtHandle );

UNSIGNED32 ENTRYPOINT AdsMgGetConfigInfo(
                           ADSHANDLE               hMgmtHandle,
                           ADS_MGMT_CONFIG_PARAMS  *pstConfigValues,
                           UNSIGNED16              *pusConfigValuesStructSize,
                           ADS_MGMT_CONFIG_MEMORY  *pstConfigMemory,
                           UNSIGNED16              *pusConfigMemoryStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETCONFIGINFO_PTR)(
                           ADSHANDLE               hMgmtHandle,
                           ADS_MGMT_CONFIG_PARAMS  *pstConfigValues,
                           UNSIGNED16              *pusConfigValuesStructSize,
                           ADS_MGMT_CONFIG_MEMORY  *pstConfigMemory,
                           UNSIGNED16              *pusConfigMemoryStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetInstallInfo(
                                 ADSHANDLE               hMgmtHandle,
                                 ADS_MGMT_INSTALL_INFO   *pstInstallInfo,
                                 UNSIGNED16              *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETINSTALLINFO_PTR)(
                                 ADSHANDLE               hMgmtHandle,
                                 ADS_MGMT_INSTALL_INFO   *pstInstallInfo,
                                 UNSIGNED16              *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetActivityInfo(
                                 ADSHANDLE               hMgmtHandle,
                                 ADS_MGMT_ACTIVITY_INFO  *pstActivityInfo,
                                 UNSIGNED16              *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETACTIVITYINFO_PTR)(
                                 ADSHANDLE               hMgmtHandle,
                                 ADS_MGMT_ACTIVITY_INFO  *pstActivityInfo,
                                 UNSIGNED16              *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetUserNames( ADSHANDLE           hMgmtHandle,
                                         UNSIGNED8           *pucFileName,
                                         ADS_MGMT_USER_INFO  astUserInfo[],
                                         UNSIGNED16          *pusArrayLen,
                                         UNSIGNED16          *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETUSERNAMES_PTR)( ADSHANDLE           hMgmtHandle,
                                         UNSIGNED8           *pucFileName,
                                         ADS_MGMT_USER_INFO  astUserInfo[],
                                         UNSIGNED16          *pusArrayLen,
                                         UNSIGNED16          *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetOpenTables(
                                 ADSHANDLE           hMgmtHandle,
                                 UNSIGNED8           *pucUserName,
                                 UNSIGNED16          usConnNumber,
                                 ADS_MGMT_TABLE_INFO astOpenTableInfo[],
                                 UNSIGNED16          *pusArrayLen,
                                 UNSIGNED16          *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETOPENTABLES_PTR)(
                                 ADSHANDLE           hMgmtHandle,
                                 UNSIGNED8           *pucUserName,
                                 UNSIGNED16          usConnNumber,
                                 ADS_MGMT_TABLE_INFO astOpenTableInfo[],
                                 UNSIGNED16          *pusArrayLen,
                                 UNSIGNED16          *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetOpenIndexes(
                                 ADSHANDLE           hMgmtHandle,
                                 UNSIGNED8           *pucTableName,
                                 UNSIGNED8           *pucUserName,
                                 UNSIGNED16          usConnNumber,
                                 ADS_MGMT_INDEX_INFO astOpenIndexInfo[],
                                 UNSIGNED16          *pusArrayLen,
                                 UNSIGNED16          *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETOPENINDEXES_PTR)(
                                 ADSHANDLE           hMgmtHandle,
                                 UNSIGNED8           *pucTableName,
                                 UNSIGNED8           *pucUserName,
                                 UNSIGNED16          usConnNumber,
                                 ADS_MGMT_INDEX_INFO astOpenIndexInfo[],
                                 UNSIGNED16          *pusArrayLen,
                                 UNSIGNED16          *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetLocks( ADSHANDLE            hMgmtHandle,
                                     UNSIGNED8            *pucTableName,
                                     UNSIGNED8            *pucUserName,
                                     UNSIGNED16           usConnNumber,
                                     ADS_MGMT_RECORD_INFO astRecordInfo[],
                                     UNSIGNED16           *pusArrayLen,
                                     UNSIGNED16           *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETLOCKS_PTR)( ADSHANDLE            hMgmtHandle,
                                     UNSIGNED8            *pucTableName,
                                     UNSIGNED8            *pucUserName,
                                     UNSIGNED16           usConnNumber,
                                     ADS_MGMT_RECORD_INFO astRecordInfo[],
                                     UNSIGNED16           *pusArrayLen,
                                     UNSIGNED16           *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetServerType( ADSHANDLE   hMgmtHandle,
                                          UNSIGNED16  *pusServerType );
typedef UNSIGNED32 (WINAPI *ADSMGGETSERVERTYPE_PTR)( ADSHANDLE   hMgmtHandle,
                                          UNSIGNED16  *pusServerType );

UNSIGNED32 ENTRYPOINT AdsMgKillUser( ADSHANDLE  hMgmtHandle,
                                     UNSIGNED8  *pucUserName,
                                     UNSIGNED16 usConnNumber );
typedef UNSIGNED32 (WINAPI *ADSMGKILLUSER_PTR)( ADSHANDLE  hMgmtHandle,
                                     UNSIGNED8  *pucUserName,
                                     UNSIGNED16 usConnNumber );

UNSIGNED32 ENTRYPOINT AdsMgGetWorkerThreadActivity(
                        ADSHANDLE                  hMgmtHandle,
                        ADS_MGMT_THREAD_ACTIVITY   astWorkerThreadActivity[],
                        UNSIGNED16                 *pusArrayLen,
                        UNSIGNED16                 *pusStructSize );
typedef UNSIGNED32 (WINAPI *ADSMGGETWORKERTHREADACTIVITY_PTR)(
                        ADSHANDLE                  hMgmtHandle,
                        ADS_MGMT_THREAD_ACTIVITY   astWorkerThreadActivity[],
                        UNSIGNED16                 *pusArrayLen,
                        UNSIGNED16                 *pusStructSize );

UNSIGNED32 ENTRYPOINT AdsMgGetLockOwner( ADSHANDLE          hMgmtHandle,
                                         UNSIGNED8          *pucTableName,
                                         UNSIGNED32         ulRecordNumber,
                                         ADS_MGMT_USER_INFO *pstUserInfo,
                                         UNSIGNED16         *pusStructSize,
                                         UNSIGNED16         *pusLockType );
typedef UNSIGNED32 (WINAPI *ADSMGGETLOCKOWNER_PTR)( ADSHANDLE          hMgmtHandle,
                                         UNSIGNED8          *pucTableName,
                                         UNSIGNED32         ulRecordNumber,
                                         ADS_MGMT_USER_INFO *pstUserInfo,
                                         UNSIGNED16         *pusStructSize,
                                         UNSIGNED16         *pusLockType );

UNSIGNED32 ENTRYPOINT AdsNullTerminateStrings( UNSIGNED16 bNullTerminate );
typedef UNSIGNED32 (WINAPI *ADSNULLTERMINATESTRINGS_PTR)( UNSIGNED16 bNullTerminate );

UNSIGNED32 ENTRYPOINT AdsOpenIndex(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucName,
                              ADSHANDLE        ahIndex[],
                              UNSIGNED16       *pusArrayLen );
typedef UNSIGNED32 (WINAPI *ADSOPENINDEX_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucName,
                              ADSHANDLE        ahIndex[],
                              UNSIGNED16       *pusArrayLen );

UNSIGNED32 ENTRYPOINT AdsOpenTable(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucName,
                              UNSIGNED8        *pucAlias,
                              UNSIGNED16       usTableType,
                              UNSIGNED16       usCharType,
                              UNSIGNED16       usLockType,
                              UNSIGNED16       usCheckRights,
                              UNSIGNED32       ulOptions,
                              ADSHANDLE        *phTable );
typedef UNSIGNED32 (WINAPI *ADSOPENTABLE_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucName,
                              UNSIGNED8        *pucAlias,
                              UNSIGNED16       usTableType,
                              UNSIGNED16       usCharType,
                              UNSIGNED16       usLockType,
                              UNSIGNED16       usCheckRights,
                              UNSIGNED32       ulOptions,
                              ADSHANDLE        *phTable );

UNSIGNED32 ENTRYPOINT AdsOpenTable90(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucName,
                              UNSIGNED8        *pucAlias,
                              UNSIGNED16       usTableType,
                              UNSIGNED16       usCharType,
                              UNSIGNED16       usLockType,
                              UNSIGNED16       usCheckRights,
                              UNSIGNED32       ulOptions,
                              UNSIGNED8        *pucCollation,
                              ADSHANDLE        *phTable );
typedef UNSIGNED32 (WINAPI *ADSOPENTABLE90_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucName,
                              UNSIGNED8        *pucAlias,
                              UNSIGNED16       usTableType,
                              UNSIGNED16       usCharType,
                              UNSIGNED16       usLockType,
                              UNSIGNED16       usCheckRights,
                              UNSIGNED32       ulOptions,
                              UNSIGNED8        *pucCollation,
                              ADSHANDLE        *phTable );

UNSIGNED32 ENTRYPOINT AdsPackTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSPACKTABLE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsRecallRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSRECALLRECORD_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsRecallAllRecords( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSRECALLALLRECORDS_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsRefreshRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSREFRESHRECORD_PTR)( ADSHANDLE hTable );


#if !( defined( ASANLM ) || defined( ASANT ) || defined( NLM ) || defined( ADS_NT ) || defined( ADS_WIN9X ) || ( defined( ADS_LINUX ) && !defined( ACE ) ) )
   UNSIGNED32 ENTRYPOINT AdsClearProgressCallback( void );
typedef UNSIGNED32 (WINAPI *ADSCLEARPROGRESSCALLBACK_PTR)( void );

   UNSIGNED32 ENTRYPOINT AdsRegisterProgressCallback(
         UNSIGNED32 (WINAPI *lpfnCallback)( UNSIGNED16 usPercent ) );
typedef UNSIGNED32 (WINAPI *ADSREGISTERPROGRESSCALLBACK_PTR)(
         UNSIGNED32 (WINAPI *lpfnCallback)( UNSIGNED16 usPercent ) );

   UNSIGNED32 ENTRYPOINT AdsRegisterCallbackFunction(
         UNSIGNED32 (WINAPI *lpfnCallback)( UNSIGNED16 usPercent, UNSIGNED32 ulCallbackID ),
         UNSIGNED32 ulCallbackID );
typedef UNSIGNED32 (WINAPI *ADSREGISTERCALLBACKFUNCTION_PTR)(
         UNSIGNED32 (WINAPI *lpfnCallback)( UNSIGNED16 usPercent, UNSIGNED32 ulCallbackID ),
         UNSIGNED32 ulCallbackID );

   UNSIGNED32 ENTRYPOINT AdsClearCallbackFunction( void );
typedef UNSIGNED32 (WINAPI *ADSCLEARCALLBACKFUNCTION_PTR)( void );

#endif

UNSIGNED32 ENTRYPOINT AdsReindex( ADSHANDLE hObject );
typedef UNSIGNED32 (WINAPI *ADSREINDEX_PTR)( ADSHANDLE hObject );

UNSIGNED32 ENTRYPOINT AdsReindex61( ADSHANDLE  hObject,
                                    UNSIGNED32 ulPageSize );
typedef UNSIGNED32 (WINAPI *ADSREINDEX61_PTR)( ADSHANDLE  hObject,
                                    UNSIGNED32 ulPageSize );

UNSIGNED32 ENTRYPOINT AdsReindexFTS( ADSHANDLE  hObject,
                                    UNSIGNED32 ulPageSize );
typedef UNSIGNED32 (WINAPI *ADSREINDEXFTS_PTR)( ADSHANDLE  hObject,
                                    UNSIGNED32 ulPageSize );

UNSIGNED32 ENTRYPOINT AdsResetConnection( ADSHANDLE hConnect );
typedef UNSIGNED32 (WINAPI *ADSRESETCONNECTION_PTR)( ADSHANDLE hConnect );

UNSIGNED32 ENTRYPOINT AdsRollbackTransaction( ADSHANDLE hConnect );
typedef UNSIGNED32 (WINAPI *ADSROLLBACKTRANSACTION_PTR)( ADSHANDLE hConnect );

UNSIGNED32 ENTRYPOINT AdsSeek(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucKey,
                              UNSIGNED16       usKeyLen,
                              UNSIGNED16       usDataType,
                              UNSIGNED16       usSeekType,
                              UNSIGNED16       *pbFound );
typedef UNSIGNED32 (WINAPI *ADSSEEK_PTR)(
                              ADSHANDLE        hIndex,
                              UNSIGNED8        *pucKey,
                              UNSIGNED16       usKeyLen,
                              UNSIGNED16       usDataType,
                              UNSIGNED16       usSeekType,
                              UNSIGNED16       *pbFound );

UNSIGNED32 ENTRYPOINT AdsSeekLast(
                                  ADSHANDLE        hIndex,
                                  UNSIGNED8        *pucKey,
                                  UNSIGNED16       usKeyLen,
                                  UNSIGNED16       usDataType,
                                  UNSIGNED16       *pbFound );
typedef UNSIGNED32 (WINAPI *ADSSEEKLAST_PTR)(
                                  ADSHANDLE        hIndex,
                                  UNSIGNED8        *pucKey,
                                  UNSIGNED16       usKeyLen,
                                  UNSIGNED16       usDataType,
                                  UNSIGNED16       *pbFound );

UNSIGNED32 ENTRYPOINT AdsSetBinary(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       usBinaryType,
                              UNSIGNED32       ulTotalLength,
                              UNSIGNED32       ulOffset,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       ulLen );
typedef UNSIGNED32 (WINAPI *ADSSETBINARY_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED16       usBinaryType,
                              UNSIGNED32       ulTotalLength,
                              UNSIGNED32       ulOffset,
                              UNSIGNED8        *pucBuf,
                              UNSIGNED32       ulLen );

UNSIGNED32 ENTRYPOINT AdsSetCollationLang( UNSIGNED8 *pucLang );
typedef UNSIGNED32 (WINAPI *ADSSETCOLLATIONLANG_PTR)( UNSIGNED8 *pucLang );

UNSIGNED32 ENTRYPOINT AdsSetCollation(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucCollation );
typedef UNSIGNED32 (WINAPI *ADSSETCOLLATION_PTR)(
                              ADSHANDLE        hConnect,
                              UNSIGNED8        *pucCollation );

UNSIGNED32 ENTRYPOINT AdsSetDate(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucValue,
                              UNSIGNED16       usLen );
typedef UNSIGNED32 (WINAPI *ADSSETDATE_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED8        *pucValue,
                              UNSIGNED16       usLen );

UNSIGNED32 ENTRYPOINT AdsSetDateFormat( UNSIGNED8 *pucFormat );
typedef UNSIGNED32 (WINAPI *ADSSETDATEFORMAT_PTR)( UNSIGNED8 *pucFormat );

UNSIGNED32 ENTRYPOINT AdsSetDateFormat60( ADSHANDLE hConnect,
                                          UNSIGNED8 *pucFormat );
typedef UNSIGNED32 (WINAPI *ADSSETDATEFORMAT60_PTR)( ADSHANDLE hConnect,
                                                     UNSIGNED8 *pucFormat );

UNSIGNED32 ENTRYPOINT AdsSetDecimals( UNSIGNED16 usDecimals );
typedef UNSIGNED32 (WINAPI *ADSSETDECIMALS_PTR)( UNSIGNED16 usDecimals );

UNSIGNED32 ENTRYPOINT AdsSetDefault( UNSIGNED8 *pucDefault );
typedef UNSIGNED32 (WINAPI *ADSSETDEFAULT_PTR)( UNSIGNED8 *pucDefault );

UNSIGNED32 ENTRYPOINT AdsShowDeleted( UNSIGNED16 bShowDeleted );
typedef UNSIGNED32 (WINAPI *ADSSHOWDELETED_PTR)( UNSIGNED16 bShowDeleted );

UNSIGNED32 ENTRYPOINT AdsSetDouble(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucFldName,
                              DOUBLE           dValue );
typedef UNSIGNED32 (WINAPI *ADSSETDOUBLE_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucFldName,
                              DOUBLE           dValue );

UNSIGNED32 ENTRYPOINT AdsSetEmpty(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucFldName );
typedef UNSIGNED32 (WINAPI *ADSSETEMPTY_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED8        *pucFldName );

UNSIGNED32 ENTRYPOINT AdsSetEpoch( UNSIGNED16 usCentury );
typedef UNSIGNED32 (WINAPI *ADSSETEPOCH_PTR)( UNSIGNED16 usCentury );

UNSIGNED32 ENTRYPOINT AdsSetExact( UNSIGNED16 bExact );
typedef UNSIGNED32 (WINAPI *ADSSETEXACT_PTR)( UNSIGNED16 bExact );

UNSIGNED32 ENTRYPOINT AdsSetExact22(
                              ADSHANDLE        hObj,
                              UNSIGNED16       bExact );
typedef UNSIGNED32 (WINAPI *ADSSETEXACT22_PTR)(
                              ADSHANDLE        hObj,
                              UNSIGNED16       bExact );

UNSIGNED32 ENTRYPOINT AdsSetField(
                                 ADSHANDLE        hObj,
                                 UNSIGNED8        *pucFldName,
                                 UNSIGNED8        *pucBuf,
                                 UNSIGNED32       ulLen );
typedef UNSIGNED32 (WINAPI *ADSSETFIELD_PTR)(
                                 ADSHANDLE        hObj,
                                 UNSIGNED8        *pucFldName,
                                 UNSIGNED8        *pucBuf,
                                 UNSIGNED32       ulLen );

UNSIGNED32 ENTRYPOINT AdsSetFilter(
                                  ADSHANDLE        hTable,
                                  UNSIGNED8        *pucFilter );
typedef UNSIGNED32 (WINAPI *ADSSETFILTER_PTR)(
                                  ADSHANDLE        hTable,
                                  UNSIGNED8        *pucFilter );

UNSIGNED32 ENTRYPOINT AdsSetHandleLong(
                                      ADSHANDLE        hObj,
                                      UNSIGNED32       ulVal );
typedef UNSIGNED32 (WINAPI *ADSSETHANDLELONG_PTR)(
                                      ADSHANDLE        hObj,
                                      UNSIGNED32       ulVal );

UNSIGNED32 ENTRYPOINT AdsSetJulian(
                                  ADSHANDLE        hObj,
                                  UNSIGNED8        *pucFldName,
                                  SIGNED32         lDate );
typedef UNSIGNED32 (WINAPI *ADSSETJULIAN_PTR)(
                                  ADSHANDLE        hObj,
                                  UNSIGNED8        *pucFldName,
                                  SIGNED32         lDate );

UNSIGNED32 ENTRYPOINT AdsSetLogical(
                                   ADSHANDLE        hObj,
                                   UNSIGNED8        *pucFldName,
                                   UNSIGNED16       bValue );
typedef UNSIGNED32 (WINAPI *ADSSETLOGICAL_PTR)(
                                   ADSHANDLE        hObj,
                                   UNSIGNED8        *pucFldName,
                                   UNSIGNED16       bValue );

UNSIGNED32 ENTRYPOINT AdsSetLong(
                                ADSHANDLE        hObj,
                                UNSIGNED8        *pucFldName,
                                SIGNED32         lValue );
typedef UNSIGNED32 (WINAPI *ADSSETLONG_PTR)(
                                ADSHANDLE        hObj,
                                UNSIGNED8        *pucFldName,
                                SIGNED32         lValue );

UNSIGNED32 ENTRYPOINT AdsSetLongLong(
                                ADSHANDLE        hObj,
                                UNSIGNED8        *pucFldName,
                                SIGNED64         qValue );
typedef UNSIGNED32 (WINAPI *ADSSETLONGLONG_PTR)(
                                ADSHANDLE        hObj,
                                UNSIGNED8        *pucFldName,
                                SIGNED64         qValue );

UNSIGNED32 ENTRYPOINT AdsSetMilliseconds(
                                        ADSHANDLE        hObj,
                                        UNSIGNED8        *pucFldName,
                                        SIGNED32         lTime );
typedef UNSIGNED32 (WINAPI *ADSSETMILLISECONDS_PTR)(
                                        ADSHANDLE        hObj,
                                        UNSIGNED8        *pucFldName,
                                        SIGNED32         lTime );

UNSIGNED32 ENTRYPOINT AdsSetMoney(
                                ADSHANDLE  hObj,
                                UNSIGNED8  *pucFldName,
                                SIGNED64   qValue );
typedef UNSIGNED32 (WINAPI *ADSSETMONEY_PTR)(
                                ADSHANDLE  hObj,
                                UNSIGNED8  *pucFldName,
                                SIGNED64   qValue );

UNSIGNED32 ENTRYPOINT AdsSetRecord(
                                  ADSHANDLE        hObj,
                                  UNSIGNED8        *pucRec,
                                  UNSIGNED32       ulLen );
typedef UNSIGNED32 (WINAPI *ADSSETRECORD_PTR)(
                                  ADSHANDLE        hObj,
                                  UNSIGNED8        *pucRec,
                                  UNSIGNED32       ulLen );

UNSIGNED32 ENTRYPOINT AdsSetRelation(
                                    ADSHANDLE        hTableParent,
                                    ADSHANDLE        hIndexChild,
                                    UNSIGNED8        *pucExpr );
typedef UNSIGNED32 (WINAPI *ADSSETRELATION_PTR)(
                                    ADSHANDLE        hTableParent,
                                    ADSHANDLE        hIndexChild,
                                    UNSIGNED8        *pucExpr );

UNSIGNED32 ENTRYPOINT AdsSetRelKeyPos(
                                     ADSHANDLE        hIndex,
                                     DOUBLE           dPos );
typedef UNSIGNED32 (WINAPI *ADSSETRELKEYPOS_PTR)(
                                     ADSHANDLE        hIndex,
                                     DOUBLE           dPos );

UNSIGNED32 ENTRYPOINT AdsSetScope(
                                 ADSHANDLE        hIndex,
                                 UNSIGNED16       usScopeOption,
                                 UNSIGNED8        *pucScope,
                                 UNSIGNED16       usScopeLen,
                                 UNSIGNED16       usDataType );
typedef UNSIGNED32 (WINAPI *ADSSETSCOPE_PTR)(
                                 ADSHANDLE        hIndex,
                                 UNSIGNED16       usScopeOption,
                                 UNSIGNED8        *pucScope,
                                 UNSIGNED16       usScopeLen,
                                 UNSIGNED16       usDataType );

UNSIGNED32 ENTRYPOINT AdsSetScopedRelation(
                                          ADSHANDLE        hTableParent,
                                          ADSHANDLE        hIndexChild,
                                          UNSIGNED8        *pucExpr );
typedef UNSIGNED32 (WINAPI *ADSSETSCOPEDRELATION_PTR)(
                                          ADSHANDLE        hTableParent,
                                          ADSHANDLE        hIndexChild,
                                          UNSIGNED8        *pucExpr );

UNSIGNED32 ENTRYPOINT AdsSetSearchPath( UNSIGNED8 *pucPath );
typedef UNSIGNED32 (WINAPI *ADSSETSEARCHPATH_PTR)( UNSIGNED8 *pucPath );

UNSIGNED32 ENTRYPOINT AdsSetServerType( UNSIGNED16 usServerOptions );
typedef UNSIGNED32 (WINAPI *ADSSETSERVERTYPE_PTR)( UNSIGNED16 usServerOptions );

UNSIGNED32 ENTRYPOINT AdsSetShort(
                                 ADSHANDLE        hObj,
                                 UNSIGNED8        *pucFldName,
                                 SIGNED16         sValue );
typedef UNSIGNED32 (WINAPI *ADSSETSHORT_PTR)(
                                 ADSHANDLE        hObj,
                                 UNSIGNED8        *pucFldName,
                                 SIGNED16         sValue );

UNSIGNED32 ENTRYPOINT AdsSetString( ADSHANDLE        hObj,
                                    UNSIGNED8        *pucFldName,
                                    UNSIGNED8        *pucBuf,
                                    UNSIGNED32       ulLen );
typedef UNSIGNED32 (WINAPI *ADSSETSTRING_PTR)( ADSHANDLE        hObj,
                                    UNSIGNED8        *pucFldName,
                                    UNSIGNED8        *pucBuf,
                                    UNSIGNED32       ulLen );

UNSIGNED32 ENTRYPOINT AdsSetTime( ADSHANDLE        hObj,
                                  UNSIGNED8        *pucFldName,
                                  UNSIGNED8        *pucValue,
                                  UNSIGNED16       usLen );
typedef UNSIGNED32 (WINAPI *ADSSETTIME_PTR)( ADSHANDLE        hObj,
                                  UNSIGNED8        *pucFldName,
                                  UNSIGNED8        *pucValue,
                                  UNSIGNED16       usLen );

UNSIGNED32 ENTRYPOINT AdsShowError( UNSIGNED8 *pucTitle );
typedef UNSIGNED32 (WINAPI *ADSSHOWERROR_PTR)( UNSIGNED8 *pucTitle );

UNSIGNED32 ENTRYPOINT AdsSkip( ADSHANDLE        hObj,
                               SIGNED32         lRecs );
typedef UNSIGNED32 (WINAPI *ADSSKIP_PTR)( ADSHANDLE        hObj,
                               SIGNED32         lRecs );

UNSIGNED32 ENTRYPOINT AdsSkipUnique( ADSHANDLE        hIndex,
                                     SIGNED32         lRecs );
typedef UNSIGNED32 (WINAPI *ADSSKIPUNIQUE_PTR)( ADSHANDLE        hIndex,
                                                SIGNED32         lRecs );

UNSIGNED32 ENTRYPOINT AdsThreadExit( void );
typedef UNSIGNED32 (WINAPI *ADSTHREADEXIT_PTR)( void );

UNSIGNED32 ENTRYPOINT AdsUnlockRecord( ADSHANDLE        hTable,
                                       UNSIGNED32       ulRec );
typedef UNSIGNED32 (WINAPI *ADSUNLOCKRECORD_PTR)( ADSHANDLE        hTable,
                                       UNSIGNED32       ulRec );

UNSIGNED32 ENTRYPOINT AdsUnlockTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSUNLOCKTABLE_PTR)( ADSHANDLE hTable );

/* AdsVerifyPassword is obsolete; retained for backward compatibility.
 * Use AdsIsEncryptionEnabled instead.
 */
UNSIGNED32 ENTRYPOINT AdsVerifyPassword( ADSHANDLE       hTable,
                                         UNSIGNED16      *pusEnabled );
typedef UNSIGNED32 (WINAPI *ADSVERIFYPASSWORD_PTR)( ADSHANDLE       hTable,
                                         UNSIGNED16      *pusEnabled );

UNSIGNED32 ENTRYPOINT AdsIsEncryptionEnabled( ADSHANDLE  hTable,
                                              UNSIGNED16 *pusEnabled );
typedef UNSIGNED32 (WINAPI *ADSISENCRYPTIONENABLED_PTR)( ADSHANDLE  hTable,
                                              UNSIGNED16 *pusEnabled );

UNSIGNED32 ENTRYPOINT AdsWriteAllRecords( void );
typedef UNSIGNED32 (WINAPI *ADSWRITEALLRECORDS_PTR)( void );

UNSIGNED32 ENTRYPOINT AdsWriteRecord( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSWRITERECORD_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsZapTable( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSZAPTABLE_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsSetAOF( ADSHANDLE        hTable,
                                 UNSIGNED8        *pucFilter,
                                 UNSIGNED16       usOptions );
typedef UNSIGNED32 (WINAPI *ADSSETAOF_PTR)( ADSHANDLE        hTable,
                                 UNSIGNED8        *pucFilter,
                                 UNSIGNED16       usOptions );

UNSIGNED32 ENTRYPOINT AdsEvalAOF( ADSHANDLE        hTable,
                                  UNSIGNED8        *pucFilter,
                                  UNSIGNED16       *pusOptLevel );
typedef UNSIGNED32 (WINAPI *ADSEVALAOF_PTR)( ADSHANDLE        hTable,
                                  UNSIGNED8        *pucFilter,
                                  UNSIGNED16       *pusOptLevel );

UNSIGNED32 ENTRYPOINT AdsClearAOF( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSCLEARAOF_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsRefreshAOF( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSREFRESHAOF_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsGetAOF( ADSHANDLE        hTable,
                                 UNSIGNED8        *pucFilter,
                                 UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETAOF_PTR)( ADSHANDLE        hTable,
                                 UNSIGNED8        *pucFilter,
                                 UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetAOFOptLevel( ADSHANDLE        hTable,
                                         UNSIGNED16       *pusOptLevel,
                                         UNSIGNED8        *pucNonOpt,
                                         UNSIGNED16       *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETAOFOPTLEVEL_PTR)( ADSHANDLE        hTable,
                                         UNSIGNED16       *pusOptLevel,
                                         UNSIGNED8        *pucNonOpt,
                                         UNSIGNED16       *pusLen );

UNSIGNED32 ENTRYPOINT AdsIsRecordInAOF( ADSHANDLE  hTable,
                                        UNSIGNED32 ulRecordNum,
                                        UNSIGNED16 *pusIsInAOF );
typedef UNSIGNED32 (WINAPI *ADSISRECORDINAOF_PTR)( ADSHANDLE  hTable,
                                        UNSIGNED32 ulRecordNum,
                                        UNSIGNED16 *pusIsInAOF );

UNSIGNED32 ENTRYPOINT AdsCustomizeAOF( ADSHANDLE  hTable,
                                       UNSIGNED32 ulNumRecords,
                                       UNSIGNED32 *pulRecords,
                                       UNSIGNED16 usOption );
typedef UNSIGNED32 (WINAPI *ADSCUSTOMIZEAOF_PTR)( ADSHANDLE  hTable,
                                       UNSIGNED32 ulNumRecords,
                                       UNSIGNED32 *pulRecords,
                                       UNSIGNED16 usOption );

UNSIGNED32 ENTRYPOINT AdsInitRawKey( ADSHANDLE hIndex );
typedef UNSIGNED32 (WINAPI *ADSINITRAWKEY_PTR)( ADSHANDLE hIndex );

UNSIGNED32 ENTRYPOINT AdsBuildRawKey( ADSHANDLE        hIndex,
                                      UNSIGNED8        *pucKey,
                                      UNSIGNED16       *pusKeyLen );
typedef UNSIGNED32 (WINAPI *ADSBUILDRAWKEY_PTR)( ADSHANDLE        hIndex,
                                      UNSIGNED8        *pucKey,
                                      UNSIGNED16       *pusKeyLen );

UNSIGNED32 ENTRYPOINT AdsCreateSQLStatement( ADSHANDLE hConnect,
                                             ADSHANDLE *phStatement );
typedef UNSIGNED32 (WINAPI *ADSCREATESQLSTATEMENT_PTR)( ADSHANDLE hConnect,
                                             ADSHANDLE *phStatement );

UNSIGNED32 ENTRYPOINT AdsPrepareSQL( ADSHANDLE hStatement,
                                     UNSIGNED8 *pucSQL );
typedef UNSIGNED32 (WINAPI *ADSPREPARESQL_PTR)( ADSHANDLE hStatement,
                                     UNSIGNED8 *pucSQL );

UNSIGNED32 ENTRYPOINT AdsExecuteSQL( ADSHANDLE hStatement,
                                     ADSHANDLE *phCursor );
typedef UNSIGNED32 (WINAPI *ADSEXECUTESQL_PTR)( ADSHANDLE hStatement,
                                     ADSHANDLE *phCursor );

UNSIGNED32 ENTRYPOINT AdsExecuteSQLDirect( ADSHANDLE hStatement,
                                           UNSIGNED8 *pucSQL,
                                           ADSHANDLE *phCursor );
typedef UNSIGNED32 (WINAPI *ADSEXECUTESQLDIRECT_PTR)( ADSHANDLE hStatement,
                                           UNSIGNED8 *pucSQL,
                                           ADSHANDLE *phCursor );

UNSIGNED32 ENTRYPOINT AdsCloseSQLStatement( ADSHANDLE hStatement );
typedef UNSIGNED32 (WINAPI *ADSCLOSESQLSTATEMENT_PTR)( ADSHANDLE hStatement );

UNSIGNED32 ENTRYPOINT AdsStmtSetTableRights( ADSHANDLE  hStatement,
                                             UNSIGNED16 usCheckRights );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLERIGHTS_PTR)( ADSHANDLE  hStatement,
                                             UNSIGNED16 usCheckRights );

UNSIGNED32 ENTRYPOINT AdsStmtSetTableReadOnly( ADSHANDLE  hStatement,
                                               UNSIGNED16 usReadOnly );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLEREADONLY_PTR)( ADSHANDLE  hStatement,
                                               UNSIGNED16 usReadOnly );

UNSIGNED32 ENTRYPOINT AdsStmtSetTableLockType( ADSHANDLE  hStatement,
                                               UNSIGNED16 usLockType );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLELOCKTYPE_PTR)( ADSHANDLE  hStatement,
                                               UNSIGNED16 usLockType );

UNSIGNED32 ENTRYPOINT AdsStmtSetTableCharType( ADSHANDLE  hStatement,
                                               UNSIGNED16 usCharType );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLECHARTYPE_PTR)( ADSHANDLE  hStatement,
                                               UNSIGNED16 usCharType );

UNSIGNED32 ENTRYPOINT AdsStmtSetTableType( ADSHANDLE  hStatement,
                                           UNSIGNED16 usTableType );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLETYPE_PTR)( ADSHANDLE  hStatement,
                                           UNSIGNED16 usTableType );

UNSIGNED32 ENTRYPOINT AdsStmtSetTableCollation( ADSHANDLE  hStatement,
                                                UNSIGNED8  *pucCollation );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLECOLLATION_PTR)( ADSHANDLE  hStatement,
                                                UNSIGNED8  *pucCollation );

UNSIGNED32 ENTRYPOINT AdsStmtConstrainUpdates( ADSHANDLE  hStatement,
                                               UNSIGNED16 usConstrain );
typedef UNSIGNED32 (WINAPI *ADSSTMTCONSTRAINUPDATES_PTR)( ADSHANDLE  hStatement,
                                               UNSIGNED16 usConstrain );

UNSIGNED32 ENTRYPOINT AdsStmtEnableEncryption( ADSHANDLE  hStatement,
                                               UNSIGNED8  *pucPassword );
typedef UNSIGNED32 (WINAPI *ADSSTMTENABLEENCRYPTION_PTR)( ADSHANDLE  hStatement,
                                               UNSIGNED8  *pucPassword );

UNSIGNED32 ENTRYPOINT AdsStmtDisableEncryption( ADSHANDLE  hStatement );
typedef UNSIGNED32 (WINAPI *ADSSTMTDISABLEENCRYPTION_PTR)( ADSHANDLE  hStatement );

UNSIGNED32 ENTRYPOINT AdsStmtSetTablePassword( ADSHANDLE hStatement,
                                               UNSIGNED8 *pucTableName,
                                               UNSIGNED8 *pucPassword );
typedef UNSIGNED32 (WINAPI *ADSSTMTSETTABLEPASSWORD_PTR)( ADSHANDLE hStatement,
                                               UNSIGNED8 *pucTableName,
                                               UNSIGNED8 *pucPassword );

UNSIGNED32 ENTRYPOINT AdsStmtClearTablePasswords( ADSHANDLE  hStatement );
typedef UNSIGNED32 (WINAPI *ADSSTMTCLEARTABLEPASSWORDS_PTR)( ADSHANDLE  hStatement );

UNSIGNED32 ENTRYPOINT AdsStmtReadAllColumns( ADSHANDLE  hStatement,
                                             UNSIGNED16 usReadColumns );
typedef UNSIGNED32 (WINAPI *ADSSTMTREADALLCOLUMNS_PTR)( ADSHANDLE  hStatement,
                                             UNSIGNED16 usReadColumns );

UNSIGNED32 ENTRYPOINT AdsClearSQLParams( ADSHANDLE  hStatement );
typedef UNSIGNED32 (WINAPI *ADSCLEARSQLPARAMS_PTR)( ADSHANDLE  hStatement );

UNSIGNED32 ENTRYPOINT AdsSetTimeStamp( ADSHANDLE        hObj,
                                       UNSIGNED8        *pucFldName,
                                       UNSIGNED8        *pucBuf,
                                       UNSIGNED32       ulLen );
typedef UNSIGNED32 (WINAPI *ADSSETTIMESTAMP_PTR)( ADSHANDLE        hObj,
                                       UNSIGNED8        *pucFldName,
                                       UNSIGNED8        *pucBuf,
                                       UNSIGNED32       ulLen );


#if !( defined( ASANLM ) || defined( ASANT ) || defined( NLM ) || defined( ADS_NT ) || defined( ADS_WIN9X ) || ( defined( ADS_LINUX ) && !defined( ACE ) ) )
   UNSIGNED32 ENTRYPOINT AdsClearSQLAbortFunc( void );
typedef UNSIGNED32 (WINAPI *ADSCLEARSQLABORTFUNC_PTR)( void );

   UNSIGNED32 ENTRYPOINT AdsRegisterSQLAbortFunc( UNSIGNED32 (WINAPI *lpfnCallback)(void) );
typedef UNSIGNED32 (WINAPI *ADSREGISTERSQLABORTFUNC_PTR)( UNSIGNED32 (WINAPI *lpfnCallback)(void) );

   UNSIGNED32 ENTRYPOINT AdsRegisterUDF( ADSHANDLE  hObj,
                                         UNSIGNED16 usType,
                                         UNSIGNED32 (WINAPI *lpfnUDF)(void) );
typedef UNSIGNED32 (WINAPI *ADSREGISTERUDF_PTR)( ADSHANDLE  hObj,
                                         UNSIGNED16 usType,
                                         UNSIGNED32 (WINAPI *lpfnUDF)(void) );
#endif

UNSIGNED32 ENTRYPOINT AdsGetNumParams( ADSHANDLE hStatement, UNSIGNED16 *pusNumParams );
typedef UNSIGNED32 (WINAPI *ADSGETNUMPARAMS_PTR)( ADSHANDLE hStatement, UNSIGNED16 *pusNumParams );

UNSIGNED32 ENTRYPOINT AdsGetLastAutoinc( ADSHANDLE hObj, UNSIGNED32 *pulAutoIncVal );
typedef UNSIGNED32 (WINAPI *ADSGETLASTAUTOINC_PTR)( ADSHANDLE hObj, UNSIGNED32 *pulAutoIncVal );

UNSIGNED32 ENTRYPOINT AdsIsIndexUserDefined( ADSHANDLE hIndex,
                                             UNSIGNED16 *pbUserDefined );
typedef UNSIGNED32 (WINAPI *ADSISINDEXUSERDEFINED_PTR)( ADSHANDLE hIndex,
                                             UNSIGNED16 *pbUserDefined );

UNSIGNED32 ENTRYPOINT AdsRestructureTable( ADSHANDLE    hObj,
                                           UNSIGNED8    *pucName,
                                           UNSIGNED8    *pucPassword,
                                           UNSIGNED16   usTableType,
                                           UNSIGNED16   usCharType,
                                           UNSIGNED16   usLockType,
                                           UNSIGNED16   usCheckRights,
                                           UNSIGNED8    *pucAddFields,
                                           UNSIGNED8    *pucDeleteFields,
                                           UNSIGNED8    *pucChangeFields );
typedef UNSIGNED32 (WINAPI *ADSRESTRUCTURETABLE_PTR)( ADSHANDLE    hObj,
                                           UNSIGNED8    *pucName,
                                           UNSIGNED8    *pucPassword,
                                           UNSIGNED16   usTableType,
                                           UNSIGNED16   usCharType,
                                           UNSIGNED16   usLockType,
                                           UNSIGNED16   usCheckRights,
                                           UNSIGNED8    *pucAddFields,
                                           UNSIGNED8    *pucDeleteFields,
                                           UNSIGNED8    *pucChangeFields );

UNSIGNED32 ENTRYPOINT AdsRestructureTable90( ADSHANDLE    hObj,
                                             UNSIGNED8    *pucName,
                                             UNSIGNED8    *pucPassword,
                                             UNSIGNED16   usTableType,
                                             UNSIGNED16   usCharType,
                                             UNSIGNED16   usLockType,
                                             UNSIGNED16   usCheckRights,
                                             UNSIGNED8    *pucAddFields,
                                             UNSIGNED8    *pucDeleteFields,
                                             UNSIGNED8    *pucChangeFields,
                                             UNSIGNED8    *pucCollation );
typedef UNSIGNED32 (WINAPI *ADSRESTRUCTURETABLE90_PTR)( ADSHANDLE    hObj,
                                             UNSIGNED8    *pucName,
                                             UNSIGNED8    *pucPassword,
                                             UNSIGNED16   usTableType,
                                             UNSIGNED16   usCharType,
                                             UNSIGNED16   usLockType,
                                             UNSIGNED16   usCheckRights,
                                             UNSIGNED8    *pucAddFields,
                                             UNSIGNED8    *pucDeleteFields,
                                             UNSIGNED8    *pucChangeFields,
                                             UNSIGNED8    *pucCollation );

UNSIGNED32 ENTRYPOINT AdsGetSQLStatementHandle( ADSHANDLE  hCursor,
                                                ADSHANDLE  *phStmt );
typedef UNSIGNED32 (WINAPI *ADSGETSQLSTATEMENTHANDLE_PTR)( ADSHANDLE  hCursor,
                                                ADSHANDLE  *phStmt );

UNSIGNED32 ENTRYPOINT AdsGetSQLStatement( ADSHANDLE  hStmt,
                                          UNSIGNED8  *pucSQL,
                                          UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETSQLSTATEMENT_PTR)( ADSHANDLE  hStmt,
                                          UNSIGNED8  *pucSQL,
                                          UNSIGNED16 *pusLen );

UNSIGNED32 ENTRYPOINT AdsFlushFileBuffers( ADSHANDLE hTable );
typedef UNSIGNED32 (WINAPI *ADSFLUSHFILEBUFFERS_PTR)( ADSHANDLE hTable );

UNSIGNED32 ENTRYPOINT AdsDDDeployDatabase( UNSIGNED8 *pucDestination,
                                           UNSIGNED8 *pucDestinationPassword,
                                           UNSIGNED8 *pucSource,
                                           UNSIGNED8 *pucSourcePassword,
                                           UNSIGNED16 usServerTypes,
                                           UNSIGNED16 usValidateOption,
                                           UNSIGNED16 usBackupFiles,
                                           UNSIGNED32 ulOptions );
typedef UNSIGNED32 (WINAPI *ADSDDDEPLOYDATABASE_PTR)( UNSIGNED8 *pucDestination,
                                           UNSIGNED8 *pucDestinationPassword,
                                           UNSIGNED8 *pucSource,
                                           UNSIGNED8 *pucSourcePassword,
                                           UNSIGNED16 usServerTypes,
                                           UNSIGNED16 usValidateOption,
                                           UNSIGNED16 usBackupFiles,
                                           UNSIGNED32 ulOptions );

UNSIGNED32 ENTRYPOINT AdsVerifySQL( ADSHANDLE hStatement,
                                    UNSIGNED8 *pucSQL );
typedef UNSIGNED32 (WINAPI *ADSVERIFYSQL_PTR)( ADSHANDLE hStatement,
                                    UNSIGNED8 *pucSQL );

UNSIGNED32 ENTRYPOINT AdsDisableUniqueEnforcement( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSDISABLEUNIQUEENFORCEMENT_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsEnableUniqueEnforcement( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSENABLEUNIQUEENFORCEMENT_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsDisableRI( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSDISABLERI_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsEnableRI( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSENABLERI_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsDisableAutoIncEnforcement( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSDISABLEAUTOINCENFORCEMENT_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsEnableAutoIncEnforcement( ADSHANDLE hConnection );
typedef UNSIGNED32 (WINAPI *ADSENABLEAUTOINCENFORCEMENT_PTR)( ADSHANDLE hConnection );

UNSIGNED32 ENTRYPOINT AdsRollbackTransaction80( ADSHANDLE   hConnect,
                                                UNSIGNED8   *pucSavepoint,
                                                UNSIGNED32  ulOptions );
typedef UNSIGNED32 (WINAPI *ADSROLLBACKTRANSACTION80_PTR)( ADSHANDLE   hConnect,
                                                           UNSIGNED8   *pucSavepoint,
                                                           UNSIGNED32  ulOptions );


UNSIGNED32 ENTRYPOINT AdsCreateSavepoint( ADSHANDLE   hConnect,
                                          UNSIGNED8   *pucSavepoint,
                                          UNSIGNED32  ulOptions );
typedef UNSIGNED32 (WINAPI *ADSCREATESAVEPOINT_PTR)( ADSHANDLE   hConnect,
                                                     UNSIGNED8   *pucSavepoint,
                                                     UNSIGNED32  ulOptions );

UNSIGNED32 ENTRYPOINT AdsDDFreeTable( UNSIGNED8 *pucTableName, UNSIGNED8 *pucPassword );
typedef UNSIGNED32 (WINAPI *ADSDDFREETABLE_PTR)( UNSIGNED8 *pucTableName, UNSIGNED8 *pucPassword );

UNSIGNED32 ENTRYPOINT AdsDDSetIndexProperty( ADSHANDLE   hAdminConn,
                                             UNSIGNED8   *pucTableName,
                                             UNSIGNED8   *pucIndexName,
                                             UNSIGNED16  usPropertyID,
                                             VOID        *pvProperty,
                                             UNSIGNED16  usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSDDSETINDEXPROPERTY_PTR)( ADSHANDLE  hAdminConn,
                                                        UNSIGNED8  *pucTableName,
                                                        UNSIGNED8  *pucIndexName,
                                                        UNSIGNED16 usPropertyID,
                                                        VOID       *pvProperty,
                                                        UNSIGNED16 usPropertyLen );

UNSIGNED32 ENTRYPOINT AdsIsFieldBinary( ADSHANDLE  hTable,
                                        UNSIGNED8  *pucFldName,
                                        UNSIGNED16 *pbBinary );
typedef UNSIGNED32 (WINAPI *ADSISFIELDBINARY_PTR)( ADSHANDLE   hTable,
                                                   UNSIGNED8   *pucFldName,
                                                   UNSIGNED16  *pbBinary );

UNSIGNED32 ENTRYPOINT AdsIsNull( ADSHANDLE   hTable,
                                 UNSIGNED8   *pucFldName,
                                 UNSIGNED16  *pbNull );
typedef UNSIGNED32 (WINAPI *ADSISNULL_PTR)( ADSHANDLE   hTable,
                                            UNSIGNED8   *pucFldName,
                                            UNSIGNED16  *pbNull );

UNSIGNED32 ENTRYPOINT AdsIsNullable( ADSHANDLE  hTable,
                                     UNSIGNED8  *pucFldName,
                                     UNSIGNED16 *pbNullable );
typedef UNSIGNED32 (WINAPI *ADSISNULLABLE_PTR)( ADSHANDLE  hTable,
                                                UNSIGNED8  *pucFldName,
                                                UNSIGNED16 *pbNullable );

UNSIGNED32 ENTRYPOINT AdsSetNull( ADSHANDLE   hTable,
                                  UNSIGNED8   *pucFldName );
typedef UNSIGNED32 (WINAPI *ADSSETNULL_PTR)( ADSHANDLE   hTable,
                                             UNSIGNED8   *pucFldName );

UNSIGNED32 ENTRYPOINT AdsGetTableCollation( ADSHANDLE  hTbl,
                                            UNSIGNED8  *pucCollation,
                                            UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETTABLECOLLATION_PTR)( ADSHANDLE  hTbl,
                                                       UNSIGNED8  *pucCollation,
                                                       UNSIGNED16 *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetIndexCollation( ADSHANDLE  hIndex,
                                            UNSIGNED8  *pucCollation,
                                            UNSIGNED16 *pusLen );
typedef UNSIGNED32 (WINAPI *ADSGETINDEXCOLLATION_PTR)( ADSHANDLE  hIndex,
                                                       UNSIGNED8  *pucCollation,
                                                       UNSIGNED16 *pusLen );

UNSIGNED32 ENTRYPOINT AdsGetDataLength(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       ulOptions,
                              UNSIGNED32       *pulLength );
typedef UNSIGNED32 (WINAPI *ADSGETDATALENGTH_PTR)(
                              ADSHANDLE        hTable,
                              UNSIGNED8        *pucFldName,
                              UNSIGNED32       ulOptions,
                              UNSIGNED32       *pulLength );

UNSIGNED32 ENTRYPOINT AdsSetIndexDirection( ADSHANDLE        hIndex,
                                           UNSIGNED16       usReverseDirection );

typedef UNSIGNED32 (WINAPI *ADSSETINDEXDIRECTION_PTR)( ADSHANDLE        hIndex,
                                                       UNSIGNED16       usReverseDirection );

UNSIGNED32 ENTRYPOINT AdsMgKillUser90( ADSHANDLE  hMgmtHandle,
                                       UNSIGNED8  *pucUserName,
                                       UNSIGNED16 usConnNumber,
                                       UNSIGNED16 usPropertyID,
                                       VOID       *pvProperty,
                                       UNSIGNED16 usPropertyLen );
typedef UNSIGNED32 (WINAPI *ADSMGKILLUSER90_PTR)( ADSHANDLE  hMgmtHandle,
                                                  UNSIGNED8  *pucUserName,
                                                  UNSIGNED16 usConnNumber,
                                                  UNSIGNED16 usPropertyID,
                                                  VOID       *pvProperty,
                                                  UNSIGNED16 usPropertyLen );


#ifdef __cplusplus
   }  /* extern "C" */
#endif


#ifdef ADS_LINUX
   #pragma pack()
#else
   #pragma pack( pop )
#endif

#endif  /* !__ACE_INCLUDED__ */



/*
 * $Header: //H/BIGDOG/ACE/h:/bigdog/ace/rcs.d//ace.h,v 1.478.1.2 2008/10/16 15:35:25 lancesc Exp $
 */

