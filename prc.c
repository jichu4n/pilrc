/*
 * @(#)prc.c
 *
 * Copyright 
 *           
 * All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;  either version 2, or (at your option)
 * any version.
 *
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT  ANY  WARRANTY;   without  even   the  implied  warranty  of 
 * MERCHANTABILITY  or FITNESS FOR A  PARTICULAR  PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You  should have  received a  copy of the GNU General Public License
 * along with this program;  if not,  please write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Revisions:
 * ==========
 *
 *     3-May-2001  Laurent Dutour
 */

#ifdef WINGUI
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include "pilrc.h"
#include "util.h"
#include "prc.h"

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned long UInt32;
typedef UInt32 LocalID;

#define dmDBNameLength	32
#define dmHdrAttrResDB  0x0001                   // ressource database

/************************************************************
 * Structure of a Resource entry
 *************************************************************/
typedef struct
{
  UInt32 type;                                   // resource type
  UInt16 id;                                     // resource id
  LocalID localChunkID;                          // resource data chunk ID
}
RsrcEntryType;
typedef RsrcEntryType *RsrcEntryPtr;

// Attributes field
#define	dmRsrcAttrUnused		0x0000   // to be defined...

/************************************************************
 * Structure of a record list extension. This is used if all
 *  the database record/resource entries of a database can't fit into
 *  the database header.
 *************************************************************/
typedef struct
{
  LocalID nextRecordListID;                      // local chunkID of next list
  UInt16 numRecords;                             // number of records in this list
  UInt16 firstEntry;                             // array of Record/Rsrc entries 
  // starts here
}
RecordListType;
typedef RecordListType *RecordListPtr;

/************************************************************
 * Structure of a Database Header
 *************************************************************/
typedef struct
{
  UInt8 name[dmDBNameLength];                    // name of database
  UInt16 attributes;                             // database attributes
  UInt16 version;                                // version of database

  UInt32 creationDate;                           // creation date of database
  UInt32 modificationDate;                       // latest modification date
  UInt32 lastBackupDate;                         // latest backup date
  UInt32 modificationNumber;                     // modification number of
  // database

  LocalID appInfoID;                             // application specific info
  LocalID sortInfoID;                            // app specific sorting info

  UInt32 type;                                   // database type
  UInt32 creator;                                // database creator 

  UInt32 uniqueIDSeed;                           // used to generate unique IDs.
  // Note that only the low order
  // 3 bytes of this is used (in
  // RecordEntryType.uniqueID).
  // We are keeping 4 bytes for 
  // alignment purposes.

  RecordListType recordList;                     // first record list
}
DatabaseHdrType;

typedef DatabaseHdrType *DatabaseHdrPtr;

#ifndef _MAX_PATH
#define _MAX_PATH 256
#endif

extern BOOL vfLE32;
static char tempName[_MAX_PATH];
static char hdrName[_MAX_PATH];
static char prcName[_MAX_PATH];
static int nbres;
static FILE *tmpF;
static FILE *hdrF;
static int crt_offset;
static RsrcEntryType crt_entry;

//------------------------------------------------------------------------
// transliteration big indian/low indian
//------------------------------------------------------------------------
#define SwapW(x) ((unsigned short)(((x>>8)&0xff)|(x<<8)))
#define SwapL(x) ((unsigned long)((SwapW(x))<<16)|(SwapW(x>>16)&0xffff))

//------------------------------------------------------------------------
// Open a temporary file 
// all the resources will be written in this file
//------------------------------------------------------------------------
FILE *
PrcOpenFile(char *outFilenameP)
{
  // verify and store the name
  if (outFilenameP == NULL)
    Error("Need a name to create a prc file");
  strcpy(prcName, outFilenameP);

  // initialize nb of resources and current offset
  nbres = 0;
  crt_offset = 0;

  // Open a temporary file to store the resources
  //---------------------------------------------
  // change the last char of the file name
  strcpy(tempName, outFilenameP);
  tempName[strlen(tempName) - 1] = '~';

  // Open the file for output
  tmpF = fopen(tempName, "wb");
  if (tmpF == NULL)
    Error3("Unable to open:", tempName, strerror(errno));
  // keep the file opened

  // Open the temporary header file
  //--------------------------------
  strcpy(hdrName, outFilenameP);
  hdrName[strlen(hdrName) - 1] = '$';
  hdrF = fopen(hdrName, "wb");
  if (hdrF == NULL)
    Error3("Unable to open:", hdrName, strerror(errno));

  return hdrF;
}

//------------------------------------------------------------------------
// Copy, Close and Delete the temporary file
//------------------------------------------------------------------------
void
PrcCloseFile(FILE * pF)
{
  FILE *prcF;
  int cch;
  UInt16 word = 0;
  DatabaseHdrType hdr;
  UInt32 zero = 0;
  RsrcEntryType entry;
  unsigned char chr;
  UInt32 theTime, dTime;

  // close the temporary files (was opened 'wb')
  fclose(pF);
  fclose(tmpF);

  // now build the output file
  //--------------------------
  prcF = fopen(prcName, "wb");
  if (prcF == NULL)
    Error3("Unable to open:", prcName, strerror(errno));
  memset(&hdr, 0, sizeof(hdr));

  // get current time (since 1/1/1970) and convert to PalmOS time (1/1/1904)
  theTime = time(0);
  dTime = (UInt32) (66L * (365.25252 * 24 * 60 * 60));
  theTime += dTime;

  strncpy(hdr.name, vfPrcName, sizeof(hdr.name) - 1);
  if (vfLE32)
  {
    hdr.attributes = dmHdrAttrResDB;
    hdr.type = vfPrcType;
    hdr.creator = vfPrcCreator;
    hdr.creationDate = theTime;
    hdr.modificationDate = theTime;
  }
  else
  {
    hdr.attributes = SwapW(dmHdrAttrResDB);
    hdr.type = SwapL(vfPrcType);
    hdr.creator = SwapL(vfPrcCreator);
    hdr.creationDate = SwapL(theTime);
    hdr.modificationDate = SwapL(theTime);
  }

  if (fwrite(&hdr, sizeof(hdr) - sizeof(RecordListType), 1, prcF) != 1)
    Error3("Error writing to output file: ", prcName, strerror(errno));

  // nextRecordListID
  if (fwrite(&zero, sizeof(UInt32), 1, prcF) != 1)
    Error3("Error writing to output file: ", prcName, strerror(errno));

  // numRecords
  word = nbres;
  if (!vfLE32)
    word = SwapW(word);
  if (fwrite(&word, sizeof(word), 1, prcF) != 1)
    Error3("Error writing to output file: ", prcName, strerror(errno));

  // add the content of the temp header file
  //----------------------------------------
  tmpF = fopen(hdrName, "rb");
  if (tmpF == NULL)
    Error2("Unable to re-open temporary file ", hdrName);

  cch = fread(&entry, sizeof(entry), 1, tmpF);
  while (cch != 0)
  {

    // compute the real offset
    entry.localChunkID +=
      sizeof(DatabaseHdrType) + nbres * sizeof(RsrcEntryType);

    // update the informations, according to vfLE32
    if (!vfLE32)
    {
      //entry.type                    = SwapL (entry.type);
      entry.id = SwapW(entry.id);
      entry.localChunkID = SwapL(entry.localChunkID);
    }

    fwrite(&entry, sizeof(entry), 1, prcF);
    cch = fread(&entry, sizeof(entry), 1, tmpF);
  }
  fclose(tmpF);

  // Pad word in the output file
  //-----------------------------
  word = 0;
  if (fwrite(&word, sizeof(word), 1, prcF) != 1)
    Error3("Error writing output file ", prcName, strerror(errno));

  // add the content of the temp resource file
  //------------------------------------------
  tmpF = fopen(tempName, "rb");
  if (tmpF == NULL)
    Error3("Unable to re-open temporary file ", tempName, strerror(errno));

  // copy char by char because we need the exact size
  cch = fread(&chr, 1, 1, tmpF);
  while (cch != 0)
  {
    fwrite(&chr, 1, 1, prcF);
    cch = fread(&chr, 1, 1, tmpF);
  }
  fclose(tmpF);

  // close the output file
  fclose(prcF);

  // delete temporary file
  remove(tempName);
  remove(hdrName);
}

//--------------------------------------------------------------------------
// Open the resource file
//--------------------------------------------------------------------------
FILE *
PrcOpenResFile(char *szBase,
               int id)
{
  //one more !
  nbres++;

  // remember
  memset(&crt_entry, 0, sizeof(crt_entry));

  // we don't know actually the real offset of each resource in the file
  // also, we store only the offset in the temporary file
  crt_entry.type = *(unsigned long *)szBase;
  crt_entry.id = id;
  crt_entry.localChunkID = crt_offset;

  // the file is opened, return the pointer
  return tmpF;
}

//--------------------------------------------------------------------------
// Close the resource file
//--------------------------------------------------------------------------
void
PrcCloseResFile(FILE * vfhOut,
                int crt_out)
{
  // Write out the entry 
  if (fwrite(&crt_entry, sizeof(crt_entry), 1, hdrF) != 1)
    Error3("Error writing output file ", hdrName, strerror(errno));

  // remember the current offset
  crt_offset += crt_out;

  // do nothing, don't close the file !
  return;
}
