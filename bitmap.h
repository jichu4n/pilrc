/*
 * @(#)bitmap.h
 *
 * Copyright 1997-1999, Wes Cherry   (wesc@technosis.com)
 * Copyright      2000, Aaron Ardiri (ardiri@palmgear.com)
 *
 * This source file was generated as part of the PilRC application
 * that is  used by developers  of the Palm Computing Platform  to
 * translate  a resource script (.rcp)  into binary resource files 
 * required to generate Palm Computing Platform applications. 
 * 
 * The source code and  binaries are available free  of charge and 
 * are to be used on an "as-is" basis. The developers of PilRC are
 * to provide no warranty, either expressed or  implied due to the 
 * use of the software. 
 *
 * Revisions:
 * ----------
 *
 * 05-Mar-97  David Dawson     file created
 * 08-Mar-97  Wes Cherry       removed windows dependancies
 * 07-Feb-00  Aaron Ardiri     color bitmap resources integrated
 *
 * PilRC is open-source,  that means that as a  developer can take 
 * part in the development of the software. Any modifications (bug
 * fixes or improvements) should be sent to the current maintainer
 * of PilRC.  This will ensure the entire Palm Computing  Platform
 * development  community can obtain the  latest and greatest from 
 * PilRC.
 *
 * Credits:
 * --------
 * 
 * Andrew Bulhak   PBM conversion
 * Hoshi Takanori  XBM, PBITM and compression
 * Aaron Ardiri    color bitmap resource integration
 */

#ifndef __BITMAP_H__
#define __BITMAP_H__

/**
 * Dump a single Bitmap (Tbmp or tAIB) resource.
 * 
 * @param fileName   the source file name  
 * @param isIcon     an icon? 0 = bitmap, 1 = normal, 2 = small
 * @param compress   compression style?
 * @param bitmaptype the type of bitmap (B+W, Grey, Grey16 or Color)?
 * @param colortable does a color table need to be generated?
 * @param transparencyData anything we need to know about transparency
 * @param multibit   should this bitmap be prepared for multibit? 
 */
extern void DumpBitmap(char *fileName, 
                       int  isIcon, 
                       int  compress, 
                       int  bitmaptype, 
                       BOOL colortable,
                       int  *transparencyData,
                       BOOL multibit);

#endif
