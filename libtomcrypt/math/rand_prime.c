/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@gmail.com, http://libtom.org
 */
#include "tomcrypt.h"
#include <linux/slab.h>

/**
  @file rand_prime.c
  Generate a random prime, Tom St Denis
*/  

#define USE_BBS 1

int rand_prime(mp_int *N, long len)
{
   int            err, res, type;
   unsigned char *buf;

   LTC_ARGCHK(N != NULL);

   /* get type */
   if (len < 0) {
      type = USE_BBS;
      len = -len;
   } else {
      type = 0;
   }

   /* allow sizes between 2 and 512 bytes for a prime size */
   if (len < 2 || len > 512) { 
      return CRYPT_INVALID_PRIME_SIZE;
   }

   /* allocate buffer to work with */
   buf = XCALLOC(1, len);
   if (buf == NULL) {
       return CRYPT_MEM;
   }

   do {
      /* generate value */
      get_random_bytes( buf, len);

      /* munge bits */
      buf[0]     |= 0x80 | 0x40;
      buf[len-1] |= 0x01 | ((type & USE_BBS) ? 0x02 : 0x00);

      /* load value */
      if ((err = mp_read_unsigned_bin(N, buf, len)) != CRYPT_OK) {
         XFREE(buf);
         return err;
      }

      /* test */
      if ((err = mp_prime_is_prime(N, 8, &res)) != CRYPT_OK) {
         XFREE(buf);
         return err;
      }
   } while (res == LTC_MP_NO);

#ifdef LTC_CLEAN_STACK
   zeromem(buf, len);
#endif

   XFREE(buf);
   return CRYPT_OK;
}
      


/* $Source: /cvs/libtom/libtomcrypt/src/math/rand_prime.c,v $ */
/* $Revision: 1.7 $ */
/* $Date: 2006/12/28 01:27:23 $ */
