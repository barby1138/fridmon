/*================================================================
** Copyright 2000, Clark Cooper
** All rights reserved.
**
** This is free software. You are permitted to copy, distribute, or modify
** it under the terms of the MIT/X license (contained in the COPYING file
** with this distribution.)
*/

#ifndef WXCONFIG_H
#define WXCONFIG_H

#include <string.h>

#define XML_NS 1
#define XML_DTD 1
#define XML_CONTEXT_BYTES 1024

/* We have big endian platform */
#define BYTEORDER 4321

/* VxWorks has memmove() available. */
#define HAVE_MEMMOVE

#endif /* ndef WINCONFIG_H */
