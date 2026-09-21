/* mp4freetag -- write iTunes-style freeform metadata atoms into MP4/M4B files
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * usage: mp4freetag CODE MEAN NAME VALUE FILE
 *
 * example:
 *   mp4freetag '----' com.apple.iTunes SERIES "Harry Potter" book.m4b
 *   mp4freetag '----' com.apple.iTunes SERIES-PART "2" book.m4b
 *
 * Writes an iTunes-style freeform atom (----) with the given mean/name
 * and a UTF-8 value into the ilst box, using the generic iTMF API of
 * libmp4v2. Existing chapters and other tags are preserved.
 */

#include "util/impl.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

int main( int argc, char** argv )
{
    if( argc < 6 ) {
        fprintf( stderr, "usage: %s CODE MEAN NAME VALUE FILE\n", argv[0] );
        return 2;
    }

    MP4FileHandle h = MP4Modify( argv[5], 0 );
    if( !h ) {
        fprintf( stderr, "cannot open %s\n", argv[5] );
        return 1;
    }

    MP4ItmfItem* item = MP4ItmfItemAlloc( argv[1], 1 );
    if( !item ) {
        fprintf( stderr, "alloc failed\n" );
        MP4Close( h, 0 );
        return 1;
    }
    item->mean = strdup( argv[2] );
    item->name = strdup( argv[3] );
    item->dataList.elements[0].typeSetIdentifier = 0;
    item->dataList.elements[0].typeCode = MP4_ITMF_BT_UTF8;
    item->dataList.elements[0].locale = 0;
    item->dataList.elements[0].valueSize = (uint32_t)strlen( argv[4] );
    item->dataList.elements[0].value = (uint8_t*)strdup( argv[4] );

    bool ok = MP4ItmfAddItem( h, item );
    MP4ItmfItemFree( item );
    if( !ok ) {
        fprintf( stderr, "MP4ItmfAddItem failed\n" );
        MP4Close( h, 0 );
        return 1;
    }

    MP4Close( h, 0 );
    printf( "ok\n" );
    return 0;
}
