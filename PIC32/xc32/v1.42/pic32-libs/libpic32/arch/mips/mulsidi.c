#ident "lib/c/arch/mips/mulsidi.c: Copyright (c) MIPS Technologies, Inc. All rights reserved."

/*
 * Unpublished work (c) MIPS Technologies, Inc.  All rights reserved.
 * Unpublished rights reserved under the copyright laws of the United
 * States of America and other countries.
 * 
 * This code is confidential and proprietary to MIPS Technologies,
 * Inc. ("MIPS Technologies") and may be disclosed only as permitted in
 * writing by MIPS Technologies. Any copying, reproducing, modifying,
 * use or disclosure of this code (in whole or in part) that is not
 * expressly permitted in writing by MIPS Technologies is strictly
 * prohibited. At a minimum, this code is protected under trade secret,
 * unfair competition, and copyright laws. Violations thereof may result
 * in criminal penalties and fines.
 * 
 * MIPS Technologies reserves the right to change this code to improve
 * function, design or otherwise. MIPS Technologies does not assume any
 * liability arising out of the application or use of this code, or of
 * any error or omission in such code.  Any warranties, whether express,
 * statutory, implied or otherwise, including but not limited to the
 * implied warranties of merchantability or fitness for a particular
 * purpose, are excluded.  Except as expressly provided in any written
 * license agreement from MIPS Technologies, the furnishing of this
 * code does not give recipient any license to any intellectual property
 * rights, including any patent rights, that cover this code.
 * 
 * This code shall not be exported, reexported, transferred, or released,
 * directly or indirectly, in violation of the law of any country or
 * international law, regulation, treaty, Executive Order, statute,
 * amendments or supplements thereto.  Should a conflict arise regarding
 * the export, reexport, transfer, or release of this code, the laws of
 * the United States of America shall be the governing law.
 * 
 * This code may only be disclosed to the United States government
 * ("Government"), or to Government users, with prior written consent
 * from MIPS Technologies.  This code constitutes one or more of the
 * following: commercial computer software, commercial computer software
 * documentation or other commercial items.  If the user of this code,
 * or any related documentation of any kind, including related technical
 * data or manuals, is an agency, department, or other entity of the
 * Government, the use, duplication, reproduction, release, modification,
 * disclosure, or transfer of this code, or any related documentation
 * of any kind, is restricted in accordance with Federal Acquisition
 * Regulation 12.212 for civilian agencies and Defense Federal Acquisition
 * Regulation Supplement 227.7202 for military agencies.  The use of this
 * code by the Government is further restricted in accordance with the
 * terms of the license agreement(s) and/or applicable contract terms
 * and conditions covering this code from MIPS Technologies.
 * 
 * 
 */

/* 
 * mulsidi.c: 32bit x 32bit -> 64bit multiplication
 */


extern const unsigned char __mul4x4tab[256];


unsigned long long
__umulsidi3 (unsigned int a, unsigned int b)
{	
   unsigned long long ret = 0;
   int i, j;

   for (i = 0; a != 0; i += 4)
     {
       unsigned a_part = a & 0x0f;
       a >>= 4;
       if (a_part != 0) {
	   unsigned int b_tmp;
	   a_part <<= 4;
	   for (j = i, b_tmp = b; b_tmp != 0 && j < 64; j += 4, b_tmp >>= 4)
	       ret += (unsigned long long)__mul4x4tab[a_part + (b_tmp & 0x0f)] << j;
       }
    }

   return ret;
}


unsigned long long
__mulsidi3 (int a, int b)
{	
   unsigned long long ret;
   int sign = 0;

   if (a < 0) {
       a = -a;
       sign = 1;
   }
   if (b < 0) {
       b = -b;
       sign ^= 1;
   }
       
   ret = __umulsidi3 (a, b);
   return sign ? -ret : ret;
}
