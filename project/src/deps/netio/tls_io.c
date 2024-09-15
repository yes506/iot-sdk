/** 
 *  Copyright (c) 2018 KT Corp. All rights reserved.
 *
 *  This is a proprietary software of KT corp, and you may not use this file except in
 *  compliance with license agreement with KT corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of KT corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 *
 */

#ifdef HAVE_OPENSSL
#include "tls_openssl.c_imp"
/* HAVE_OPENSSL */

#elif HAVE_MBEDTLS
#include "tls_mbedtls.c_imp"
/* HAVE_MBEDTLS */

#endif
