/* Copyright (C) 2003, 2004 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, if you include this header file into source
   files compiled by GCC, this header file does not by itself cause
   the resulting executable to be covered by the GNU General Public
   License.  This exception does not however invalidate any other
   reasons why the executable file might be covered by the GNU General
   Public License.  */

/* Implemented from the specification included in the Intel C++ Compiler
   User Guide and Reference, version 8.0.  */

#ifndef _EMMINTRIN_H_INCLUDED
#define _EMMINTRIN_H_INCLUDED

#ifdef __SSE2__
#include <xmmintrin.h>

/* SSE2 */
typedef double __v2df __attribute__ ((mode (V2DF)));
typedef int __v2di __attribute__ ((mode (V2DI)));
typedef int __v4si __attribute__ ((mode (V4SI)));
typedef int __v8hi __attribute__ ((mode (V8HI)));
typedef int __v16qi __attribute__ ((mode (V16QI)));

/* Create a selector for use with the SHUFPD instruction.  */
#define _MM_SHUFFLE2(fp1,fp0) \
 (((fp1) << 1) | (fp0))

#define __m128i __v2di
#define __m128d __v2df

/* Create a vector with element 0 as *P and the rest zero.  */
static __inline __m128d
_mm_load_sd (double const *__P)
;

/* Create a vector with all two elements equal to *P.  */
static __inline __m128d
_mm_load1_pd (double const *__P)
;

static __inline __m128d
_mm_load_pd1 (double const *__P)
;

/* Load two DPFP values from P.  The address must be 16-byte aligned.  */
static __inline __m128d
_mm_load_pd (double const *__P)
;

/* Load two DPFP values from P.  The address need not be 16-byte aligned.  */
static __inline __m128d
_mm_loadu_pd (double const *__P)
;

/* Load two DPFP values in reverse order.  The address must be aligned.  */
static __inline __m128d
_mm_loadr_pd (double const *__P)
;

/* Create a vector with element 0 as F and the rest zero.  */
static __inline __m128d
_mm_set_sd (double __F)
;

/* Create a vector with all two elements equal to F.  */
static __inline __m128d
_mm_set1_pd (double __F)
;

static __inline __m128d
_mm_set_pd1 (double __F)
;

/* Create the vector [Z Y].  */
static __inline __m128d
_mm_set_pd (double __Z, double __Y)
;

/* Create the vector [Y Z].  */
static __inline __m128d
_mm_setr_pd (double __Z, double __Y)
;

/* Create a vector of zeros.  */
static __inline __m128d
_mm_setzero_pd (void)
;

/* Stores the lower DPFP value.  */
static __inline void
_mm_store_sd (double *__P, __m128d __A)
;

/* Store the lower DPFP value across two words.  */
static __inline void
_mm_store1_pd (double *__P, __m128d __A)
;

static __inline void
_mm_store_pd1 (double *__P, __m128d __A)
;

/* Store two DPFP values.  The address must be 16-byte aligned.  */
static __inline void
_mm_store_pd (double *__P, __m128d __A)
;

/* Store two DPFP values.  The address need not be 16-byte aligned.  */
static __inline void
_mm_storeu_pd (double *__P, __m128d __A)
;

/* Store two DPFP values in reverse order.  The address must be aligned.  */
static __inline void
_mm_storer_pd (double *__P, __m128d __A)
;

/* Sets the low DPFP value of A from the low value of B.  */
static __inline __m128d
_mm_move_sd (__m128d __A, __m128d __B)
;


static __inline __m128d
_mm_add_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_add_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_sub_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_sub_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_mul_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_mul_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_div_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_div_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_sqrt_pd (__m128d __A)
;

/* Return pair {sqrt (A[0), B[1]}.  */
static __inline __m128d
_mm_sqrt_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_min_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_min_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_max_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_max_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_and_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_andnot_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_or_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_xor_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpeq_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmplt_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmple_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpgt_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpge_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpneq_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpnlt_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpnle_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpngt_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpnge_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpord_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpunord_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpeq_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmplt_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmple_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpgt_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpge_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpneq_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpnlt_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpnle_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpngt_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpnge_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpord_sd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_cmpunord_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_comieq_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_comilt_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_comile_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_comigt_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_comige_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_comineq_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_ucomieq_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_ucomilt_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_ucomile_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_ucomigt_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_ucomige_sd (__m128d __A, __m128d __B)
;

static __inline int
_mm_ucomineq_sd (__m128d __A, __m128d __B)
;

/* Create a vector with element 0 as *P and the rest zero.  */

static __inline __m128i
_mm_load_si128 (__m128i const *__P)
;

static __inline __m128i
_mm_loadu_si128 (__m128i const *__P)
;

static __inline __m128i
_mm_loadl_epi64 (__m128i const *__P)
;

static __inline void
_mm_store_si128 (__m128i *__P, __m128i __B)
;

static __inline void
_mm_storeu_si128 (__m128i *__P, __m128i __B)
;

static __inline void
_mm_storel_epi64 (__m128i *__P, __m128i __B)
;

static __inline __m64
_mm_movepi64_pi64 (__m128i __B)
;

static __inline __m128i
_mm_move_epi64 (__m128i __A)
;

/* Create a vector of zeros.  */
static __inline __m128i
_mm_setzero_si128 (void)
;

static __inline __m128i
_mm_set_epi64 (__m64 __A,  __m64 __B)
;

/* Create the vector [Z Y X W].  */
static __inline __m128i
_mm_set_epi32 (int __Z, int __Y, int __X, int __W)
;

#ifdef __x86_64__
/* Create the vector [Z Y].  */
static __inline __m128i
_mm_set_epi64x (long long __Z, long long __Y)
;
#endif

/* Create the vector [S T U V Z Y X W].  */
static __inline __m128i
_mm_set_epi16 (short __Z, short __Y, short __X, short __W,
               short __V, short __U, short __T, short __S)
;

/* Create the vector [S T U V Z Y X W].  */
static __inline __m128i
_mm_set_epi8 (char __Z, char __Y, char __X, char __W,
              char __V, char __U, char __T, char __S,
              char __Z1, char __Y1, char __X1, char __W1,
              char __V1, char __U1, char __T1, char __S1)
;

static __inline __m128i
_mm_set1_epi64 (__m64 __A)
;

static __inline __m128i
_mm_set1_epi32 (int __A)
;

#ifdef __x86_64__
static __inline __m128i
_mm_set1_epi64x (long long __A)
;
#endif

static __inline __m128i
_mm_set1_epi16 (short __A)
;

static __inline __m128i
_mm_set1_epi8 (char __A)
;

static __inline __m128i
_mm_setr_epi64 (__m64 __A,  __m64 __B)
;

/* Create the vector [Z Y X W].  */
static __inline __m128i
_mm_setr_epi32 (int __W, int __X, int __Y, int __Z)
;
/* Create the vector [S T U V Z Y X W].  */
static __inline __m128i
_mm_setr_epi16 (short __S, short __T, short __U, short __V,
                short __W, short __X, short __Y, short __Z)
;

/* Create the vector [S T U V Z Y X W].  */
static __inline __m128i
_mm_setr_epi8 (char __S1, char __T1, char __U1, char __V1,
               char __W1, char __X1, char __Y1, char __Z1,
               char __S, char __T, char __U, char __V,
               char __W, char __X, char __Y, char __Z)
;

static __inline __m128d
_mm_cvtepi32_pd (__m128i __A)
;

static __inline __m128
_mm_cvtepi32_ps (__m128i __A)
;

static __inline __m128i
_mm_cvtpd_epi32 (__m128d __A)
;

static __inline __m64
_mm_cvtpd_pi32 (__m128d __A)
;

static __inline __m128
_mm_cvtpd_ps (__m128d __A)
;

static __inline __m128i
_mm_cvttpd_epi32 (__m128d __A)
;

static __inline __m64
_mm_cvttpd_pi32 (__m128d __A)
;

static __inline __m128d
_mm_cvtpi32_pd (__m64 __A)
;

static __inline __m128i
_mm_cvtps_epi32 (__m128 __A)
;

static __inline __m128i
_mm_cvttps_epi32 (__m128 __A)
;

static __inline __m128d
_mm_cvtps_pd (__m128 __A)
;

static __inline int
_mm_cvtsd_si32 (__m128d __A)
;

#ifdef __x86_64__
static __inline long long
_mm_cvtsd_si64x (__m128d __A)
;
#endif

static __inline int
_mm_cvttsd_si32 (__m128d __A)
;

#ifdef __x86_64__
static __inline long long
_mm_cvttsd_si64x (__m128d __A)
;
#endif

static __inline __m128
_mm_cvtsd_ss (__m128 __A, __m128d __B)
;

static __inline __m128d
_mm_cvtsi32_sd (__m128d __A, int __B)
;

#ifdef __x86_64__
static __inline __m128d
_mm_cvtsi64x_sd (__m128d __A, long long __B)
;
#endif

static __inline __m128d
_mm_cvtss_sd (__m128d __A, __m128 __B)
;

#define _mm_shuffle_pd(__A, __B, __C) ((__m128d)__builtin_ia32_shufpd ((__v2df)__A, (__v2df)__B, (__C)))

static __inline __m128d
_mm_unpackhi_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_unpacklo_pd (__m128d __A, __m128d __B)
;

static __inline __m128d
_mm_loadh_pd (__m128d __A, double const *__B)
;

static __inline void
_mm_storeh_pd (double *__A, __m128d __B)
;

static __inline __m128d
_mm_loadl_pd (__m128d __A, double const *__B)
;

static __inline void
_mm_storel_pd (double *__A, __m128d __B)
;

static __inline int
_mm_movemask_pd (__m128d __A)
;

static __inline __m128i
_mm_packs_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_packs_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_packus_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpackhi_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpackhi_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpackhi_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpackhi_epi64 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpacklo_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpacklo_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpacklo_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_unpacklo_epi64 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_add_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_add_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_add_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_add_epi64 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_adds_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_adds_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_adds_epu8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_adds_epu16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sub_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sub_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sub_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sub_epi64 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_subs_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_subs_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_subs_epu8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_subs_epu16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_madd_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_mulhi_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_mullo_epi16 (__m128i __A, __m128i __B)
;

static __inline __m64
_mm_mul_su32 (__m64 __A, __m64 __B)
;

static __inline __m128i
_mm_mul_epu32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sll_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sll_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sll_epi64 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sra_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sra_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_srl_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_srl_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_srl_epi64 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_slli_epi16 (__m128i __A, int __B)
;

static __inline __m128i
_mm_slli_epi32 (__m128i __A, int __B)
;

static __inline __m128i
_mm_slli_epi64 (__m128i __A, int __B)
;

static __inline __m128i
_mm_srai_epi16 (__m128i __A, int __B)
;

static __inline __m128i
_mm_srai_epi32 (__m128i __A, int __B)
;

#if 0
static __m128i __attribute__((__always_inline__))
_mm_srli_si128 (__m128i __A, const int __B)
;

static __m128i __attribute__((__always_inline__))
_mm_srli_si128 (__m128i __A, const int __B)
;
#endif
#define _mm_srli_si128(__A, __B) ((__m128i)__builtin_ia32_psrldqi128 (__A, __B))
#define _mm_slli_si128(__A, __B) ((__m128i)__builtin_ia32_pslldqi128 (__A, __B))

static __inline __m128i
_mm_srli_epi16 (__m128i __A, int __B)
;

static __inline __m128i
_mm_srli_epi32 (__m128i __A, int __B)
;

static __inline __m128i
_mm_srli_epi64 (__m128i __A, int __B)
;

static __inline __m128i
_mm_and_si128 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_andnot_si128 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_or_si128 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_xor_si128 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmpeq_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmpeq_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmpeq_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmplt_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmplt_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmplt_epi32 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmpgt_epi8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmpgt_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_cmpgt_epi32 (__m128i __A, __m128i __B)
;

#define _mm_extract_epi16(__A, __B) __builtin_ia32_pextrw128 ((__v8hi)__A, __B)

#define _mm_insert_epi16(__A, __B, __C) ((__m128i)__builtin_ia32_pinsrw128 ((__v8hi)__A, __B, __C))

static __inline __m128i
_mm_max_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_max_epu8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_min_epi16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_min_epu8 (__m128i __A, __m128i __B)
;

static __inline int
_mm_movemask_epi8 (__m128i __A)
;

static __inline __m128i
_mm_mulhi_epu16 (__m128i __A, __m128i __B)
;

#define _mm_shufflehi_epi16(__A, __B) ((__m128i)__builtin_ia32_pshufhw ((__v8hi)__A, __B))
#define _mm_shufflelo_epi16(__A, __B) ((__m128i)__builtin_ia32_pshuflw ((__v8hi)__A, __B))
#define _mm_shuffle_epi32(__A, __B) ((__m128i)__builtin_ia32_pshufd ((__v4si)__A, __B))

static __inline void
_mm_maskmoveu_si128 (__m128i __A, __m128i __B, char *__C)
;

static __inline __m128i
_mm_avg_epu8 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_avg_epu16 (__m128i __A, __m128i __B)
;

static __inline __m128i
_mm_sad_epu8 (__m128i __A, __m128i __B)
;

static __inline void
_mm_stream_si32 (int *__A, int __B)
;

static __inline void
_mm_stream_si128 (__m128i *__A, __m128i __B)
;

static __inline void
_mm_stream_pd (double *__A, __m128d __B)
;

static __inline __m128i
_mm_movpi64_epi64 (__m64 __A)
;

static __inline void
_mm_clflush (void const *__A)
;

static __inline void
_mm_lfence (void)
;

static __inline void
_mm_mfence (void)
;

static __inline __m128i
_mm_cvtsi32_si128 (int __A)
;

#ifdef __x86_64__
static __inline __m128i
_mm_cvtsi64x_si128 (long long __A)
;
#endif

static __inline int
_mm_cvtsi128_si32 (__m128i __A)
;

#ifdef __x86_64__
static __inline long long
_mm_cvtsi128_si64x (__m128i __A)
;
#endif

#endif /* __SSE2__  */

#endif /* _EMMINTRIN_H_INCLUDED */
