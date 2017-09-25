
/**********************************************************************
 * my_crypt.c
 * **********************************************************************
 *
 * my_crypt - Clone of the UNIX crypt (1) utility.
 *
 * The latest version of this program should be available at:
 *
 *   http://saa.dyndns.org/stewart
 *
 * Copyright ©2001, 2003, 2009, Stewart Adcock <stewart--AT--linux-domain.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *                     
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *                                                          
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 *  WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * **********************************************************************
 *
 *  synopsis:     Dodgy clone of the dodgy UNIX crypt utility.
 *
 *  compilation:  try typing something like this with the GNU compiler:
 *                gcc my_crypt.c -o crypt -lcrypt -Wall -O2
 *
 *  usage:        crypt [-s] passkey < file > encrypted_file
 *                crypt [-s] passkey < encrypted_file > file
 *
 *  Warning: This _IS_ _NOT_ designed to be cryptographically secure.
 *           It was solely written as a means to decrypt files which
 *           were encrypted using the legacy UNIX crypt utility.
 *
 * Last Updated:
 *
 *   01 June 2009 SAA  Re-released under the terms of the 2-clause BSD license.
 *   28 May 2003  SAA  Fixed a bug - thanks to Dr A J Bartlett for pointing this out.
 *   11 June 2001 SAA  Clone of UNIX crypt utility.
 *
 * *********************************************************************/

#define _XOPEN_SOURCE   /* For crypt() */
#define _ISO99_SOURCE   /* For int32_t */

#include <stdio.h>
#include <stdint.h>     /* Comment out this line for MIPSpro compiler on IRIX, and possibly others. */
#include <stdlib.h>
#include <unistd.h>

#define NUM_ROTORS      256
#define MASK            0377

char    t1[NUM_ROTORS];
char    t2[NUM_ROTORS];
char    t3[NUM_ROTORS];
char    deck[NUM_ROTORS];

char *init(char *key) {
  int           ic, i, j, temp;
  unsigned int  random;
  int32_t       seed=123;
  char          *password;

  password = (char *)crypt(key, key);

  for (i=0; i<13; i++) seed = seed*password[i] + i;

  for(i=0;i<NUM_ROTORS;i++) {
    t1[i] = i;
    deck[i] = i;
    t2[i] = 0;
    t3[i] = 0;
  }

  for(i=0;i<NUM_ROTORS;i++) {
    seed = 5*seed + password[i%13];
    random = seed % 65521;
    j = NUM_ROTORS - i;
    ic = (random&MASK) % j;
    j--;
    random >>= 8;
    temp = t1[ic];
    t1[ic] = t1[j];
    t1[j] = temp;
    if (t3[j]==0) {
      ic = (random&MASK) % j;
      while (t3[ic]!=0) ic = (ic+1) % j;
      t3[ic] = j;
      t3[j] = ic;
    }
  }

  for(i=0;i<NUM_ROTORS;i++) t2[t1[i]&MASK] = i;

  return password;
}


void shuffle(char *deck, char *password) {
  int                   ic, i, j, temp;
  static int32_t        seed = 123;

  for(i=0;i<NUM_ROTORS;i++) {
    seed = 5*seed + password[i%13];
    j = NUM_ROTORS - i;
    ic = ((seed%65521)&MASK)%j;
    j--;
    temp = deck[j];
    deck[j] = deck[ic];
    deck[ic] = temp;
  }

  return;
}


int main(int argc, char **argv) {
  int   i;
  int   n1=0, n2=0, nr1=0, nr2=0;
  int   soption = 0;
  char  *password;

  /* Check for the '-s' switch, the only supported option. */
  if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 's') {
    argc--;
    argv++;
    soption = 1;
  }

  password = init(argv[1]);

  if (soption) {
    while((i=getchar())>=0) {
      nr1 = deck[n1]&MASK;
      nr2 = deck[nr1]&MASK;
      i = t2[(t3[(t1[(i+nr1)&MASK]+nr2)&MASK]-nr2)&MASK]-nr1;
      putchar(i);
      n1++;
      if (n1==NUM_ROTORS) {
        n1 = 0;
        n2++;
        if(n2==NUM_ROTORS) n2 = 0;
        shuffle(deck, password);
      }
    }
  } else {
    while((i=getchar())>=0) {
      nr1 = n1;
      i = t2[(t3[(t1[(i+nr1)&MASK]+nr2)&MASK]-nr2)&MASK]-nr1;
      putchar(i);
      n1++;
      if (n1==NUM_ROTORS) {
        n1 = 0;
        n2++;
        if(n2==NUM_ROTORS) n2 = 0;
        nr2 = n2;
      }
    }
  }

  exit(0);
}
