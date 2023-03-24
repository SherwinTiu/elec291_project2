/* math.h standard header */
#ifndef _MATH
#define _MATH
 #include <xtgmath.h>
#ifndef _YMATH
 #include <ymath.h>
#endif /* _YMATH */


#define __MATHH_INLINE static inline

_C_STD_BEGIN
		/* CODES FOR is* FUNCTIONS */
#define _FP_LT	1
#define _FP_EQ	2
#define _FP_GT	4

		/* CODES FOR ilogb FUNCTIONS */

 #if _ILONG
  #define _FP_ILOGB0	(-0x7fffffff - _C2)
  #define _FP_ILOGBNAN	0x7fffffff

 #else /* _ILONG */
  #define _FP_ILOGB0	(-0x7fff - _C2)
  #define _FP_ILOGBNAN	0x7fff
 #endif /* _ILONG */

 #if _HAS_C9X
		/* TYPES */

 #if _FEVAL == 1
typedef double float_t;
typedef double double_t;

 #elif _FEVAL == 2
typedef long double float_t;
typedef long double double_t;

 #else /* _FEVAL */
typedef float float_t;
typedef double double_t;
 #endif /* _FEVAL */

		/* MACROS */

 #if 245 < __EDG_VERSION__ && !defined(__cplusplus)
  #define HUGE_VAL	((double)__INFINITY__)
  #define HUGE_VALF	__INFINITY__
  #define HUGE_VALL	((long double)__INFINITY__)
  #define INFINITY	__INFINITY__
  #define NAN		__NAN__

 #elif defined(__EDG__) || 0 < _MSC_VER
  #define HUGE_VAL	_CSTD _Inf._Double
  #define HUGE_VALF	_CSTD _FInf._Float
  #define HUGE_VALL	_CSTD _LInf._Long_double
  #define INFINITY	_CSTD _FInf._Float
  #define NAN		_CSTD _FNan._Float

 #else /* !defined(__EDG__) */
  #ifndef _HUGE_ENUF
   #define _HUGE_ENUF	1e+300	/* _HUGE_ENUF*_HUGE_ENUF must overflow */
  #endif /* _HUGE_ENUF */

  #define HUGE_VAL	((double)INFINITY)
  #define HUGE_VALF	((float)INFINITY)
  #define HUGE_VALL	((long double)INFINITY)
  #define INFINITY	((float)(_HUGE_ENUF * _HUGE_ENUF))
  #define NAN		((float)(INFINITY * 0.0F))
 #endif /* 245 < __EDG_VERSION__ */

#define FP_INFINITE		_INFCODE
#define FP_NAN			_NANCODE
#define FP_NORMAL		_FINITE
#define FP_SUBNORMAL	_DENORM
#define FP_ZERO			0

 #if _HAS_C9X_FAST_FMA
  #define FP_FAST_FMA	1
  #define FP_FAST_FMAF	1
  #define FP_FAST_FMAL	1
 #endif /* _HAS_C9X_FAST_FMA */

 #define FP_ILOGB0		_FP_ILOGB0
 #define FP_ILOGBNAN	_FP_ILOGBNAN

#define MATH_ERRNO			1
#define MATH_ERREXCEPT		2
#define math_errhandling	(MATH_ERRNO | MATH_ERREXCEPT)	/* do both */

_C_LIB_DECL
int _FFpcomp(float, float);
int _Fpcomp(double, double);
int _LFpcomp(long double, long double);

int _FDclass(float);
int _Dclass(double);
int _LDclass(long double);

int _FDsign(float);
int _Dsign(double);
int _LDsign(long double);
_END_C_LIB_DECL

 #if _HAS_GENERIC_TEMPLATES
extern "C++" {
		// TEMPLATE FUNCTION _FPCOMP
__MATHH_INLINE int _FPCOMP(float _Left, float _Right)
	{	// compare _Left and _Right
	return (_FFpcomp(_Left, _Right));
	}

__MATHH_INLINE int _FPCOMP(double _Left, double _Right)
	{	// compare _Left and _Right
	return (_Fpcomp(_Left, _Right));
	}

__MATHH_INLINE int _FPCOMP(long double _Left, long double _Right)
	{	// compare _Left and _Right
	return (_LFpcomp(_Left, _Right));
	}

template<class _T1, class _T2> __MATHH_INLINE
	int _FPCOMP(_T1 _Left, _T2 _Right)
	{	// compare _Left and _Right
	typedef typename _Combined_type<float,
		typename _Real_widened<
			typename _Real_type<_T1>::_Type,
			typename _Real_type<_T2>::_Type>::_Type>::_Type _Tw;
	return (_FPCOMP((_Tw)_Left, (_Tw)_Right));
	}

		// FUNCTION fpclassify
__MATHH_INLINE int fpclassify(float _Left)
	{	// classify argument
	return (_FDtest(&_Left));
	}

__MATHH_INLINE int fpclassify(double _Left)
	{	// classify argument
	return (_Dtest(&_Left));
	}

__MATHH_INLINE int fpclassify(long double _Left)
	{	// classify argument
	return (_LDtest(&_Left));
	}

		// FUNCTION signbit
__MATHH_INLINE bool signbit(float _Left)
	{	// test sign bit
	return (_FDsign(_Left) != 0);
	}

__MATHH_INLINE bool signbit(double _Left)
	{	// test sign bit
	return (_Dsign(_Left) != 0);
	}

__MATHH_INLINE bool signbit(long double _Left)
	{	// test sign bit
	return (_LDsign(_Left) != 0);
	}

template<class _Ty> __MATHH_INLINE
	bool isfinite(_Ty _Left)
	{	// test for finite
	return (fpclassify(_Left) <= 0);
	}

template<class _Ty> __MATHH_INLINE
	bool isinf(_Ty _Left)
	{	// test for infinite
	return (fpclassify(_Left) == FP_INFINITE);
	}

#ifdef __XC32
template<class _Ty> __MATHH_INLINE
	bool isinfl(_Ty _Left)
	{	// test for infinite
	return (fpclassify(_Left) == FP_INFINITE);
	}
#endif

template<class _Ty> __MATHH_INLINE
	bool isnan(_Ty _Left)
	{	// test for NaN
	return (fpclassify(_Left) == FP_NAN);
	}

template<class _Ty> __MATHH_INLINE
	bool isnormal(_Ty _Left)
	{	// test for normal
	return (fpclassify(_Left) == FP_NORMAL);
	}

template<class _Ty1, class _Ty2> __MATHH_INLINE
	bool isgreater(_Ty1 _Left, _Ty2 _Right)
	{	// test for _Left > _Right
	return ((_FPCOMP(_Left, _Right) & _FP_GT) != 0);
	}

template<class _Ty1, class _Ty2> __MATHH_INLINE
	bool isgreaterequal(_Ty1 _Left, _Ty2 _Right)
	{	// test for _Left >= _Right
	return ((_FPCOMP(_Left, _Right) & (_FP_EQ | _FP_GT)) != 0);
	}

template<class _Ty1, class _Ty2> __MATHH_INLINE
	bool isless(_Ty1 _Left, _Ty2 _Right)
	{	// test for _Left < _Right
	return ((_FPCOMP(_Left, _Right) & _FP_LT) != 0);
	}

template<class _Ty1, class _Ty2> __MATHH_INLINE
	bool islessequal(_Ty1 _Left, _Ty2 _Right)
	{	// test for _Left <= _Right
	return ((_FPCOMP(_Left, _Right) & (_FP_LT | _FP_EQ)) != 0);
	}

template<class _Ty1, class _Ty2> __MATHH_INLINE
	bool islessgreater(_Ty1 _Left, _Ty2 _Right)
	{	// test for _Left != _Right
	return ((_FPCOMP(_Left, _Right) & (_FP_LT | _FP_GT)) != 0);
	}

template<class _Ty1, class _Ty2> __MATHH_INLINE
	bool isunordered(_Ty1 _Left, _Ty2 _Right)
	{	// test for _Left unorderd w.r.t. _Right
	return (_FPCOMP(_Left, _Right) == 0);
	}

  #define fpclassify(x) 		(_CSTD fpclassify(x))
  #define signbit(x)			(_CSTD signbit(x))
  #define isfinite(x)			(_CSTD isfinite(x))
  #define isinf(x)				(_CSTD isinf(x))
#ifdef __XC32
  #define isinfl(x)				(_CSTD isinfl(x))
#endif
  #define isnan(x)				(_CSTD isnan(x))
  #define isnormal(x)			(_CSTD isnormal(x))
  #define isgreater(x, y)		(_CSTD isgreater(x, y))
  #define isgreaterequal(x, y)	(_CSTD isgreaterequal(x, y))
  #define isless(x, y)			(_CSTD isless(x, y))
  #define islessequal(x, y)		(_CSTD islessequal(x, y))
  #define islessgreater(x, y)	(_CSTD islessgreater(x, y))
  #define isunordered(x, y)		(_CSTD isunordered(x, y))
}	// extern "C++"

 #else /* _HAS_GENERIC_TEMPLATES */

  #if __EDG__
   #define _CARGI(x, fd, ff, fl) \
	__generic(x,,, fd, ff, fl,,,)(x)
   #define _CARG2I(x, y, fd, ff, fl) \
	__generic(x, y,, fd, ff, fl,,,)(x, y)

  #elif 2 <= __GNUC__
   #define _FLT_TYPE(expression)	_FLT_OR_DBL(__typeof__ (expression))

  #define _FLT_OR_DBL(mytype)	__typeof__ (*(0 \
	? (__typeof__ (0 ? (mytype *)0 \
		: (void *)!((mytype)0.5 && (mytype)0.5 - 1)))0 \
	: (__typeof__ (0 ? (double *)0 \
		: (void *)((mytype)0.5 && (mytype)0.5 - 1)))0))

   #define _CARGI(x, fd, ff, fl) \
	(__extension__({ \
	int _Ans; \
	if (sizeof (_FLT_TYPE(x)) == sizeof (double)) \
		_Ans = fd(x); \
	else if (sizeof (_FLT_TYPE(x)) == sizeof (float)) \
	_Ans = ff(x); \
	else \
		_Ans = fl(x); \
	_Ans; \
	}))
   #define _CARG2I(x, y, fd, ff, fl) \
	(__extension__({ \
	int _Ans; \
	if (sizeof (_FLT_TYPE((x) + (y))) == sizeof (double)) \
		_Ans = fd(x, y); \
	else if (sizeof (_FLT_TYPE((x) + (y))) == sizeof (float)) \
		_Ans = ff(x, y); \
	else \
		_Ans = fl(x, y); \
	_Ans; \
	}))

  #else /* compiler type */
   #define _ARG(x)	(sizeof ((x) + (float)0) == sizeof (float) ? 'f' \
	: sizeof ((x) + (double)0) == sizeof (double) ? 'd' \
	: 'l')
   #define _CARGI(x, fd, ff, fl)	\
	(_ARG(x) == 'f' ? ff((float)(x)) \
	: _ARG(x) == 'd' ? fd((double)(x)) \
	: fl((long double)(x)))
   #define _CARG2I(x, y, fd, ff, fl)	\
	(_ARG((x) + (y)) == 'f' ? ff((float)(x), (float)(y)) \
	: _ARG((x) + (y)) == 'd' ? fd((double)(x), (double)(y)) \
	: fl((long double)(x), (long double)(y)))
  #endif /* compiler type */

  #define _FPCOMP(x, y) \
	_CARG2I(x, y, _Fpcomp, _FFpcomp, _LFpcomp)
  #define fpclassify(x) \
	_CARGI(x, _Dclass, _FDclass, _LDclass)
  #define signbit(x) \
	_CARGI(x, _Dsign, _FDsign, _LDsign)

#define isfinite(x)	(fpclassify(x) <= 0)
#define isinf(x)	(fpclassify(x) == FP_INFINITE)
#define isnan(x)	(fpclassify(x) == FP_NAN)
#define isnormal(x)	(fpclassify(x) == FP_NORMAL)

#define isgreater(x, y)	((_FPCOMP(x, y) & _FP_GT) != 0)
#define isgreaterequal(x, y)	\
	((_FPCOMP(x, y) & (_FP_EQ | _FP_GT)) != 0)
#define isless(x, y)	((_FPCOMP(x, y) & _FP_LT) != 0)
#define islessequal(x, y)	((_FPCOMP(x, y) & (_FP_LT | _FP_EQ)) != 0)
#define islessgreater(x, y)	\
	((_FPCOMP(x, y) & (_FP_LT | _FP_GT)) != 0)
#define isunordered(x, y)	(_FPCOMP(x, y) == 0)
 #endif /* _HAS_GENERIC_TEMPLATES */

 #else /* _IS_C9X */
		/* MACROS */
 #define HUGE_VAL	_CSTD _Hugeval._Double
 #endif /* _IS_C9X */

_C_LIB_DECL

#if 0 /* Moved below to __MATHH_INLINE routing functions */
/* double declarations */
double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double ceil(double);
double exp(double);
double fabs(double);
double floor(double);
double fmod(double, double);
double frexp(double, int *);
double ldexp(double, int);
double modf(double, double *);
double pow(double, double);
double sqrt(double);
double tan(double);
double tanh(double);
#endif
 #if _HAS_C9X
#if 0 /* Moved below to __MATHH_INLINE routing functions */
double acosh(double);
double asinh(double);
double atanh(double);
double cbrt(double);
double copysign(double, double);
double erf(double);
double erfc(double);
double exp2(double);
double expm1(double);
double fdim(double, double);
double fma(double, double, double);
double fmax(double, double);
double fmin(double, double);
double hypot(double, double);
int ilogb(double);
double lgamma(double);
_Longlong llrint(double);
_Longlong llround(double);
double log1p(double);
double logb(double);
long lrint(double);
long lround(double);
double nan(const char *);
double nearbyint(double);
double nextafter(double, double);
double nexttoward(double, long double);
double remainder(double, double);
double remquo(double, double, int *);
double rint(double);
double round(double);
double scalbn(double, int);
double scalbln(double, long);
double tgamma(double);
double trunc(double);
#endif
 #endif /* _IS_C9X */

 #if _HAS_TR1_DECLARATIONS
 #if 0
double assoc_laguerre(unsigned, unsigned, double);
double assoc_legendre(unsigned, unsigned, double);
double beta(double, double);
double comp_ellint_1(double);
double comp_ellint_2(double);
double comp_ellint_3(double, double);
double cyl_bessel_i(double, double);
double cyl_bessel_j(double, double);
double cyl_bessel_k(double, double);
double cyl_neumann(double, double);
double ellint_1(double, double);
double ellint_2(double, double);
double ellint_3(double, double, double);
double expint(double);
double hermite(unsigned, double);
double laguerre(unsigned, double);
double legendre(unsigned, double);
double riemann_zeta(double);
double sph_bessel(unsigned, double);
double sph_legendre(unsigned, unsigned, double);
double sph_neumann(unsigned, double);

 #if _HAS_ADDED_FUNCTIONS
double besseli0(double);
double besseli1(double);
double besselj0(double);
double besselj1(double);
double besselk0(double);
double besselk1(double);
double bessely0(double);
double bessely1(double);

double j0(double);
double j1(double);
double jn(double);
double y0(double);
double y1(double);
double yn(double);
 #endif /* _HAS_ADDED_FUNCTIONS */
 #endif /* 0 */
 #endif /* _HAS_TR1_DECLARATIONS */

		/* float declarations */
float acosf(float);
float asinf(float);
float atanf(float);
float atan2f(float, float);
float ceilf(float);
float expf(float);
float fabsf(float);
float floorf(float);
float fmodf(float, float);
float frexpf(float, int *);
float ldexpf(float, int);
float modff(float, float *);
float powf(float, float);
float sqrtf(float);
float tanf(float);
float tanhf(float);

 #if _HAS_C9X
float acoshf(float);
float asinhf(float);
float atanhf(float);
float cbrtf(float);
float copysignf(float, float);
float erff(float);
float erfcf(float);
float expm1f(float);
float exp2f(float);
float fdimf(float, float);
float fmaf(float, float, float);
float fmaxf(float, float);
float fminf(float, float);
float hypotf(float, float);
int ilogbf(float);
float lgammaf(float);
_Longlong llrintf(float);
_Longlong llroundf(float);
float log1pf(float);
float logbf(float);
long lrintf(float);
long lroundf(float);
float nanf(const char *);
float nearbyintf(float);
float nextafterf(float, float);
float nexttowardf(float, long double);
float remainderf(float, float);
float remquof(float, float, int *);
float rintf(float);
float roundf(float);
float scalbnf(float, int);
float scalblnf(float, long);
float tgammaf(float);
float truncf(float);
 #endif /* _IS_C9X */

 #if _HAS_TR1_DECLARATIONS
float assoc_laguerref(unsigned, unsigned, float);
float assoc_legendref(unsigned, unsigned, float);
float betaf(float, float);
float comp_ellint_1f(float);
float comp_ellint_2f(float);
float comp_ellint_3f(float, float);
float cyl_bessel_if(float, float);
float cyl_bessel_jf(float, float);
float cyl_bessel_kf(float, float);
float cyl_neumannf(float, float);
float ellint_1f(float, float);
float ellint_2f(float, float);
float ellint_3f(float, float, float);
float expintf(float);
float hermitef(unsigned, float);
float laguerref(unsigned, float);
float legendref(unsigned, float);
float riemann_zetaf(float);
float sph_besself(unsigned, float);
float sph_legendref(unsigned, unsigned, float);
float sph_neumannf(unsigned, float);

 #if _HAS_ADDED_FUNCTIONS
float besseli0f(float);
float besseli1f(float);
float besselj0f(float);
float besselj1f(float);
float besselk0f(float);
float besselk1f(float);
float bessely0f(float);
float bessely1f(float);

double j0f(double);
double j1f(double);
double jnf(double);
double y0f(double);
double y1f(double);
double ynf(double);
 #endif /* _HAS_ADDED_FUNCTIONS */

 #endif /* _HAS_TR1_DECLARATIONS */

		/* long double declarations */
long double acosl(long double);
long double asinl(long double);
long double atanl(long double);
long double atan2l(long double, long double);
long double ceill(long double);
long double expl(long double);
long double fabsl(long double);
long double floorl(long double);
long double fmodl(long double, long double);
long double frexpl(long double, int *);
long double ldexpl(long double, int);
long double modfl(long double, long double *);
long double powl(long double, long double);
long double sqrtl(long double);
long double tanl(long double);
long double tanhl(long double);

 #if _HAS_C9X
long double acoshl(long double);
long double asinhl(long double);
long double atanhl(long double);
long double cbrtl(long double);
long double copysignl(long double, long double);
long double erfl(long double);
long double erfcl(long double);
long double exp2l(long double);
long double expm1l(long double);
long double fdiml(long double, long double);
long double fmal(long double, long double, long double);
long double fmaxl(long double, long double);
long double fminl(long double, long double);
long double hypotl(long double, long double);
int ilogbl(long double);
long double lgammal(long double);
_Longlong llrintl(long double);
_Longlong llroundl(long double);
long double log1pl(long double);
long double logbl(long double);
long lrintl(long double);
long lroundl(long double);
long double nanl(const char *);
long double nearbyintl(long double);
long double nextafterl(long double, long double);
long double nexttowardl(long double, long double);
long double remainderl(long double, long double);
long double remquol(long double, long double, int *);
long double rintl(long double);
long double roundl(long double);
long double scalbnl(long double, int);
long double scalblnl(long double, long);
long double tgammal(long double);
long double truncl(long double);
 #endif /* _IS_C9X */

 #if _HAS_TR1_DECLARATIONS
long double assoc_laguerrel(unsigned, unsigned, long double);
long double assoc_legendrel(unsigned, unsigned, long double);
long double betal(long double, long double);
long double comp_ellint_1l(long double);
long double comp_ellint_2l(long double);
long double comp_ellint_3l(long double, long double);
long double cyl_bessel_il(long double, long double);
long double cyl_bessel_jl(long double, long double);
long double cyl_bessel_kl(long double, long double);
long double cyl_neumannl(long double, long double);
long double ellint_1l(long double, long double);
long double ellint_2l(long double, long double);
long double ellint_3l(long double, long double, long double);
long double expintl(long double);
long double hermitel(unsigned, long double);
long double laguerrel(unsigned, long double);
long double legendrel(unsigned, long double);
long double riemann_zetal(long double);
long double sph_bessell(unsigned, long double);
long double sph_legendrel(unsigned, unsigned, long double);
long double sph_neumannl(unsigned, long double);

 #if _HAS_ADDED_FUNCTIONS
long double besseli0l(long double);
long double besseli1l(long double);
long double besselj0l(long double);
long double besselj1l(long double);
long double besselk0l(long double);
long double besselk1l(long double);
long double bessely0l(long double);
long double bessely1l(long double);

double j0l(double);
double j1l(double);
double jnl(double);
double y0l(double);
double y1l(double);
double ynl(double);
 #endif /* _HAS_ADDED_FUNCTIONS */

 #endif /* _HAS_TR1_DECLARATIONS */
_END_C_LIB_DECL

 #if defined(__cplusplus) && !defined(_NO_CPP_INLINES)
extern "C++" {

// double __MATHH_INLINES, FOR C++

__MATHH_INLINE double cos(double _Left)
	{	// return cosine
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LSin(_Left, 1));
#else
	return (_FSin((float)_Left, 1));
#endif
	}

__MATHH_INLINE double cosh(double _Left)
	{	// return hyperbolic cosine
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LCosh(_Left, 1.0));
#else
	return (_FCosh((float)_Left, 1.0F));
#endif
	}

__MATHH_INLINE double fmod(double _Left, double _Right)
	{	// return modulus
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fmodl(_Left, _Right));
#else
	return (fmodf(_Left, _Right));
#endif
	}


__MATHH_INLINE double log(double _Left)
	{	// return natural logarithm
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LLog(_Left, 0));
#else
	return (_FLog((float)_Left, 0));
#endif
	}

__MATHH_INLINE double log10(double _Left)
	{	// return base-10 logarithm
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LLog(_Left, 1));
#else
	return (_FLog((float)_Left, 1));
#endif
	}

__MATHH_INLINE double sin(double _Left)
	{	// return sine
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LSin(_Left, 0));
#else
	return (_FSin((float)_Left, 0));
#endif
	}

__MATHH_INLINE double sinh(double _Left)
	{	// return hyperbolic sine
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LSinh(_Left, 1.0));
#else
	return (_FSinh((float)_Left, 1.0F));
#endif
	}

 #if _HAS_C9X
__MATHH_INLINE double log2(double _Left)
	{	// return base-2 logarithm
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (_LLog(_Left, -1));
#else
	return (_FLog((float)_Left, -1));
#endif
	}
 #endif /* _IS_C9X */

#if 1
__MATHH_INLINE double abs(double _Left)	// OVERLOADS
	{	// return absolute value
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fabsl((long double)_Left));
#else
	return (fabsf((float)_Left));
#endif
	}
#endif

/* Overloads for -fshort-double / -fno-short-double */
/* acos */
__MATHH_INLINE  double acos( double _Left)
	{	// return arccosine
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (acosl(_Left));
#else
	return (acosf(_Left));
#endif
	}

/* asin */
__MATHH_INLINE  double asin(double _Left)
	{	// return arcsine
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (asinl(_Left));
#else
	return (asinf(_Left));
#endif
}

/* atan */
__MATHH_INLINE double atan(double _Left)
	{	// return arctangent
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (atanl(_Left));
#else
	return (atanf(_Left));
#endif
	}

/* atan2 */
__MATHH_INLINE double atan2(double _Left, double _Right)
	{	// return arctangent
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (atan2l(_Left, _Right));
#else
	return (atan2f(_Left, _Right));
#endif
	}

/* ceil */
__MATHH_INLINE double ceil(double _Left)
	{	// return ceiling
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ceill(_Left));
#else
	return (ceilf(_Left));
#endif
	}

/* exp */
__MATHH_INLINE double exp(double _Left)
	{	// return exponential
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (expl(_Left));
#else
	return (expf(_Left));
#endif
	}

/* fabs */
__MATHH_INLINE double fabs(double _Left)
	{	// return absolute value
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fabsl(_Left));
#else
	return (fabsf(_Left));
#endif
	}

/* floor */
__MATHH_INLINE double floor(double _Left)
	{	// return floor
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (floorl(_Left));
#else
	return (floorf(_Left));
#endif
	}

/* frexp */
__MATHH_INLINE double frexp(double _Left, int *_Right)
	{	// unpack exponent
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (frexpl(_Left, _Right));
#else
	return (frexpf(_Left, _Right));
#endif
	}

/* ldexp */
__MATHH_INLINE double ldexp(double _Left, int _Right)
	{	// pack exponent
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ldexpl(_Left, _Right));
#else
	return (ldexpf(_Left, _Right));
#endif
	}

/* modf */
__MATHH_INLINE double modf(double _Left, double *_Right)
	{	// unpack fraction
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	void *ptr = _Right;
	return (modfl(_Left, (long double*)ptr));
#else
	return (modff(_Left, (float*)_Right));
#endif
	}

/* pow */
__MATHH_INLINE double pow(double _Left, double _Right)
	{	// raise to power
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (powl(_Left, _Right));
#else
	return (powf(_Left, _Right));
#endif
	}

/* sqrt */
__MATHH_INLINE double sqrt(double _Left)
	{	// return square root
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (sqrtl(_Left));
#else
	return (sqrtf(_Left));
#endif
	}

/* tan */
__MATHH_INLINE double tan(double _Left)
	{	// return arctangent
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (tanl(_Left));
#else
	return (tanf(_Left));
#endif
	}

/* tanh */
__MATHH_INLINE double tanh(double _Left)
	{	// return hyperbolic tangent
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (tanhl(_Left));
#else
	return (tanhf(_Left));
#endif
	}

#if _HAS_C9X
__MATHH_INLINE double acosh(double _Left)
	{	// return acosh
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (acoshl(_Left));
#else
	return (acoshf(_Left));
#endif
    }

__MATHH_INLINE double asinh(double _Left)
	{	// return asinh
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (asinhl(_Left));
#else
	return (asinhf(_Left));
#endif
	}

__MATHH_INLINE double atanh(double _Left)
	{	// return atanh
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (atanhl(_Left));
#else
	return (atanhf(_Left));
#endif
	}

__MATHH_INLINE double cbrt(double _Left)
	{	// return cbrt
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (cbrtl(_Left));
#else
	return (cbrtf(_Left));
#endif
	}

__MATHH_INLINE double copysign(double _Left, double _Right)
	{	// return copysign
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (copysignl(_Left, _Right));
#else
	return (copysignf(_Left, _Right));
#endif
	}

__MATHH_INLINE double erf(double _Left)
	{	// return erf
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (erfl(_Left));
#else
	return (erff(_Left));
#endif
	}

__MATHH_INLINE double erfc(double _Left)
	{	// return erfc
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (erfcl(_Left));
#else
	return (erfcf(_Left));
#endif
	}

__MATHH_INLINE double exp2(double _Left)
	{	// return exp2
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (exp2l(_Left));
#else
	return (exp2f(_Left));
#endif
	}

__MATHH_INLINE double expm1(double _Left)
	{	// return expml
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (expm1l(_Left));
#else
	return (expm1f(_Left));
#endif
	}

__MATHH_INLINE double fdim(double _Left, double _Right)
	{	// return fdim
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fdiml(_Left, _Right));
#else
	return (fdimf(_Left, _Right));
#endif
	}

__MATHH_INLINE double fma(double _Left, double _Right,
	double _Addend)
	{	// return fma
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fmal(_Left, _Right, _Addend));
#else
	return (fmaf(_Left, _Right, _Addend));
#endif
	}

__MATHH_INLINE double fmax(double _Left, double _Right)
	{	// return fmax
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fmaxl(_Left, _Right));
#else
	return (fmaxf(_Left, _Right));
#endif
	}

__MATHH_INLINE double fmin(double _Left, double _Right)
	{	// return fmin
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (fminl(_Left, _Right));
#else
	return (fminf(_Left, _Right));
#endif
	}

__MATHH_INLINE double hypot(double _Left, double _Right)
	{	// return hypot
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (hypotl(_Left, _Right));
#else
	return (hypotf(_Left, _Right));
#endif
	}

__MATHH_INLINE int ilogb(double _Left)
	{	// return ilogb
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ilogbl(_Left));
#else
	return (ilogbf(_Left));
#endif
	}

__MATHH_INLINE double lgamma(double _Left)
	{	// return lgamma
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (lgammal(_Left));
#else
	return (lgammaf(_Left));
#endif
	}

__MATHH_INLINE _Longlong llrint(double _Left)
	{	// return llrint
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (llrintl(_Left));
#else
	return (llrintf(_Left));
#endif
	}

__MATHH_INLINE _Longlong llround(double _Left)
	{	// return llround
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (llroundl(_Left));
#else
	return (llroundf(_Left));
#endif
	}

__MATHH_INLINE double log1p(double _Left)
	{	// return loglp
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (log1pl(_Left));
#else
	return (log1pf(_Left));
#endif
	}

__MATHH_INLINE double logb(double _Left)
	{	// return logb
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (logbl(_Left));
#else
	return (logbf(_Left));
#endif
	}

__MATHH_INLINE long lrint(double _Left)
	{	// return lrint
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (lrintl(_Left));
#else
	return (lrintf(_Left));
#endif
	}

__MATHH_INLINE double nan(const char * _Left)
	{	// return lround
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (nanl(_Left));
#else
	return (nanf(_Left));
#endif
	}

__MATHH_INLINE long lround(double _Left)
	{	// return lround
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (lroundl(_Left));
#else
	return (lroundf(_Left));
#endif
	}

__MATHH_INLINE double nearbyint(double _Left)
	{	// return nearbyint
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (nearbyintl(_Left));
#else
	return (nearbyintf(_Left));
#endif
	}

__MATHH_INLINE double nextafter(double _Left, double _Right)
	{	// return nextafter
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (nextafterl(_Left, _Right));
#else
	return (nextafterf(_Left, _Right));
#endif
	}

__MATHH_INLINE double nexttoward(double _Left, double _Right)
	{	// return nexttoward
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (nexttowardl(_Left, _Right));
#else
	return (nexttowardf(_Left, _Right));
#endif
	}

__MATHH_INLINE double remainder(double _Left, double _Right)
	{	// return remainder
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (remainderl(_Left, _Right));
#else
	return (remainderf(_Left, _Right));
#endif
	}

__MATHH_INLINE double remquo(double _Left, double _Right,
	int *_Pval)
	{	// return remquo
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (remquol(_Left, _Right, _Pval));
#else
	return (remquof(_Left, _Right, _Pval));
#endif
	}

__MATHH_INLINE double rint(double _Left)
	{	// return rint
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (rintl(_Left));
#else
	return (rintf(_Left));
#endif
	}

__MATHH_INLINE double round(double _Left)
	{	// return round
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (roundl(_Left));
#else
	return (roundf(_Left));
#endif
	}

__MATHH_INLINE double scalbn(double _Left, int _Right)
	{	// return scalbn
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (scalbnl(_Left, _Right));
#else
	return (scalbnf(_Left, _Right));
#endif
	}

__MATHH_INLINE double scalbln(double _Left, long _Right)
	{	// return scalbln
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (scalblnl(_Left, _Right));
#else
	return (scalblnf(_Left, _Right));
#endif
	}

__MATHH_INLINE double tgamma(double _Left)
	{	// return tgamma
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (tgammal(_Left));
#else
	return (tgammaf(_Left));
#endif
	}

__MATHH_INLINE double trunc(double _Left)
	{	// return trunc
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (truncl(_Left));
#else
	return (truncf(_Left));
#endif
	}

#endif /* _IS_C9X */

#if _HAS_TR1_DECLARATIONS
__MATHH_INLINE double assoc_laguerre(unsigned _Nv, unsigned _Mv,
	double _Xv)
	{	// return assoc_laguerre
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (assoc_laguerrel(_Nv, _Mv, _Xv));
#else
	return (assoc_laguerref(_Nv, _Mv, _Xv));
#endif
	}

__MATHH_INLINE double assoc_legendre(unsigned _Nv, unsigned _Mv,
    double _Xv)
	{	// return assoc_legendre
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (assoc_legendrel(_Nv, _Mv, _Xv));
#else
	return (assoc_legendref(_Nv, _Mv, _Xv));
#endif
	}

__MATHH_INLINE double beta(double _Xv, double _Yv)
	{	// return beta
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (betal(_Xv, _Yv));
#else
	return (betaf(_Xv, _Yv));
#endif
	}

__MATHH_INLINE double comp_ellint_1(double _Kv)
	{	// return comp_ellint_1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (comp_ellint_1l(_Kv));
#else
	return (comp_ellint_1f(_Kv));
#endif
	}

__MATHH_INLINE double comp_ellint_2(double _Kv)
	{	// return comp_ellint_2
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (comp_ellint_2l(_Kv));
#else
	return (comp_ellint_2f(_Kv));
#endif
	}

__MATHH_INLINE double comp_ellint_3(double _Kv, double _Nu)
	{	// return comp_ellint_3
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (comp_ellint_3l(_Kv, _Nu));
#else
	return (comp_ellint_3f(_Kv, _Nu));
#endif
	}

__MATHH_INLINE double cyl_bessel_i(double _Nu, double _Xv)
	{	// return cyl_bessel_i
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (cyl_bessel_il(_Nu, _Xv));
#else
	return (cyl_bessel_if(_Nu, _Xv));
#endif
	}

__MATHH_INLINE double cyl_bessel_j(double _Nu, double _Xv)
	{	// return cyl_bessel_j
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (cyl_bessel_jl(_Nu, _Xv));
#else
	return (cyl_bessel_jf(_Nu, _Xv));
#endif
	}

__MATHH_INLINE double cyl_bessel_k(double _Nu, double _Xv)
	{	// return cyl_bessel_k
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (cyl_bessel_kl(_Nu, _Xv));
#else
	return (cyl_bessel_kf(_Nu, _Xv));
#endif
	}

__MATHH_INLINE double cyl_neumann(double _Nu, double _Xv)
	{	// return cyl_neumann
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (cyl_neumannl(_Nu, _Xv));
#else
	return (cyl_neumannf(_Nu, _Xv));
#endif
	}

__MATHH_INLINE double ellint_1(double _Kv, double _Phi)
	{	// return ellint_1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ellint_1l(_Kv, _Phi));
#else
	return (ellint_1f(_Kv, _Phi));
#endif
	}

__MATHH_INLINE double ellint_2(double _Kv, double _Phi)
	{	// return ellint_2
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ellint_2l(_Kv, _Phi));
#else
	return (ellint_2f(_Kv, _Phi));
#endif
	}

__MATHH_INLINE double ellint_3(double _Kv, double _Nu,
	double _Phi)
	{	// return ellint_3
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ellint_3l(_Kv, _Nu, _Phi));
#else
	return (ellint_3f(_Kv, _Nu, _Phi));
#endif
	}

__MATHH_INLINE double expint(double _Xv)
	{	// return expint
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (expintl(_Xv));
#else
	return (expintf(_Xv));
#endif
	}

__MATHH_INLINE double hermite(unsigned _Nv, double _Xv)
	{	// return hermite
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (hermitel(_Nv, _Xv));
#else
	return (hermitef(_Nv, _Xv));
#endif
	}

__MATHH_INLINE double laguerre(unsigned _Nv, double _Xv)
	{	// return laguerre
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (laguerrel(_Nv, _Xv));
#else
	return (laguerref(_Nv, _Xv));
#endif
	}

__MATHH_INLINE double legendre(unsigned _Lv, double _Xv)
	{	// return legendre
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (legendrel(_Lv, _Xv));
#else
	return (legendref(_Lv, _Xv));
#endif
	}

__MATHH_INLINE double riemann_zeta(double _Xv)
	{	// return riemann_zeta
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (riemann_zetal(_Xv));
#else
	return (riemann_zetaf(_Xv));
#endif
	}

__MATHH_INLINE double sph_bessel(unsigned _Nv, double _Xv)
	{	// return sph_bessel
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (sph_bessell(_Nv, _Xv));
#else
	return (sph_besself(_Nv, _Xv));
#endif
	}

__MATHH_INLINE double sph_legendre(unsigned _Lv, unsigned _Mv,
	double _Xv)
	{	// return sph_legendre
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (sph_legendrel(_Lv, _Mv, _Xv));
#else
	return (sph_legendref(_Lv, _Mv, _Xv));
#endif
	}

__MATHH_INLINE double sph_neumann(unsigned _Nv, double _Xv)
	{	// return sph_neumann
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (sph_neumannl(_Nv, _Xv));
#else
	return (sph_neumannf(_Nv, _Xv));
#endif
	}

#if _HAS_ADDED_FUNCTIONS
__MATHH_INLINE double besseli0(double _Xv)
	{	// return besseli0
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (besseli0l(_Xv));
#else
	return (besseli0f(_Xv));
#endif
	}

__MATHH_INLINE double besseli1(double _Xv)
	{	// return besseli1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (besseli1l(_Xv));
#else
	return (besseli1f(_Xv));
#endif
	}

__MATHH_INLINE double besselj0(double _Xv)
	{	// return besselj0
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (besselj0l(_Xv));
#else
	return (besselj0f(_Xv));
#endif
	}

__MATHH_INLINE double besselj1(double _Xv)
	{	// return besselj1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (besselj1l(_Xv));
#else
	return (besselj1l(_Xv));
#endif
	}

__MATHH_INLINE double besselk0(double _Xv)
	{	// return besselk0
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (besselk0l(_Xv));
#else
	return (besselk0f(_Xv));
#endif
	}

__MATHH_INLINE double besselk1(double _Xv)
	{	// return besselk1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (besselk1l(_Xv));
#else
	return (besselk1f(_Xv));
#endif
	}

__MATHH_INLINE double bessely0(double _Xv)
	{	// return bessely0
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (bessely0l(_Xv));
#else
	return (bessely0f(_Xv));
#endif
	}

__MATHH_INLINE double bessely1(double _Xv)
	{	// return bessely1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (bessely1l(_Xv));
#else
	return (bessely1f(_Xv));
#endif
    }

__MATHH_INLINE double j0(double _Xv)
	{	// return j0
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (j0l(_Xv));
#else
	return (j0f(_Xv));
#endif
	}

__MATHH_INLINE double j1(double _Xv)
	{	// return j1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (j1l(_Xv));
#else
	return (j1f(_Xv));
#endif
    }

__MATHH_INLINE double jn(double _Xv)
	{	// return jn
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (jnl(_Xv));
#else
	return (jnf(_Xv));
#endif
    }

__MATHH_INLINE double y0(double _Xv)
	{	// return y0
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (y0l(_Xv));
#else
	return (y0f(_Xv));
#endif
	}

__MATHH_INLINE double y1(double _Xv)
	{	// return y1
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (y1l(_Xv));
#else
	return (y1f(_Xv));
#endif
    }

__MATHH_INLINE double yn(double _Xv)
	{	// return yn
#if (__DBL_MANT_DIG__>__FLT_MANT_DIG__)
	return (ynl(_Xv));
#else
	return (ynf(_Xv));
#endif
    }

#endif /* _HAS_ADDED_FUNCTIONS */
#endif /* _HAS_TR1_DECLARATIONS */

// float __MATHH_INLINES, FOR C++

__MATHH_INLINE float cosf(float _Left)
	{	// return cosine
	return (_FSin(_Left, 1));
	}

__MATHH_INLINE float coshf(float _Left)
	{	// return hyperbolic cosine
	return (_FCosh(_Left, 1.0F));
	}

__MATHH_INLINE float logf(float _Left)
	{	// return natural logarithm
	return (_FLog(_Left, 0));
	}

__MATHH_INLINE float log10f(float _Left)
	{	// return base-10 logarithm
	return (_FLog(_Left, 1));
	}

__MATHH_INLINE float sinf(float _Left)
	{	// return sine
	return (_FSin(_Left, 0));
	}

__MATHH_INLINE float sinhf(float _Left)
	{	// return hyperbolic sine
	return (_FSinh(_Left, 1.0F));
	}

 #if _HAS_C9X
__MATHH_INLINE float log2f(float _Left)
	{	// return base-2 logarithm
	return (_FLog(_Left, -1));
	}
 #endif /* _IS_C9X */

__MATHH_INLINE float abs(float _Left)	// OVERLOADS
	{	// return absolute value
	return (fabsf(_Left));
	}

__MATHH_INLINE float acos(float _Left)
	{	// return arccosine
	return (acosf(_Left));
	}

__MATHH_INLINE float asin(float _Left)
	{	// return arcsine
	return (asinf(_Left));
	}

__MATHH_INLINE float atan(float _Left)
	{	// return arctangent
	return (atanf(_Left));
	}

__MATHH_INLINE float atan2(float _Left, float _Right)
	{	// return arctangent
	return (atan2f(_Left, _Right));
	}

__MATHH_INLINE float ceil(float _Left)
	{	// return ceiling
	return (ceilf(_Left));
	}

__MATHH_INLINE float cos(float _Left)
	{	// return cosine
	return (_FSin(_Left, 1));
	}

__MATHH_INLINE float cosh(float _Left)
	{	// return hyperbolic cosine
	return (_FCosh(_Left, 1.0F));
	}

__MATHH_INLINE float exp(float _Left)
	{	// return exponential
	return (expf(_Left));
	}

__MATHH_INLINE float fabs(float _Left)
	{	// return absolute value
	return (fabsf(_Left));
	}

__MATHH_INLINE float floor(float _Left)
	{	// return floor
	return (floorf(_Left));
	}

__MATHH_INLINE float fmod(float _Left, float _Right)
	{	// return modulus
	return (fmodf(_Left, _Right));
	}

__MATHH_INLINE float frexp(float _Left, int *_Right)
	{	// unpack exponent
	return (frexpf(_Left, _Right));
	}

__MATHH_INLINE float ldexp(float _Left, int _Right)
	{	// pack exponent
	return (ldexpf(_Left, _Right));
	}

__MATHH_INLINE float log(float _Left)
	{	// return natural logarithm
	return (_FLog(_Left, 0));
	}

__MATHH_INLINE float log10(float _Left)
	{	// return base-10 logarithm
	return (_FLog(_Left, 1));
	}

__MATHH_INLINE float modf(float _Left, float *_Right)
	{	// unpack fraction
	return (modff(_Left, _Right));
	}

__MATHH_INLINE float pow(float _Left, float _Right)
	{	// raise to power
	return (powf(_Left, _Right));
	}

__MATHH_INLINE float sin(float _Left)
	{	// return sine
	return (_FSin(_Left, 0));
	}

__MATHH_INLINE float sinh(float _Left)
	{	// return hyperbolic sine
	return (_FSinh(_Left, 1.0F));
	}

__MATHH_INLINE float sqrt(float _Left)
	{	// return square root
	return (sqrtf(_Left));
	}

__MATHH_INLINE float tan(float _Left)
	{	// return tangent
	return (tanf(_Left));
	}

__MATHH_INLINE float tanh(float _Left)
	{	// return hyperbolic tangent
	return (tanhf(_Left));
	}

 #if _HAS_C9X
__MATHH_INLINE float acosh(float _Left)
	{	// return hyperbolic arccosine
	return (acoshf(_Left));
	}

__MATHH_INLINE float asinh(float _Left)
	{	// return hyperbolic arcsine
	return (asinhf(_Left));
	}

__MATHH_INLINE float atanh(float _Left)
	{	// return hyperbolic arctangent
	return (atanhf(_Left));
	}

__MATHH_INLINE float cbrt(float _Left)
	{	// return cube root
	return (cbrtf(_Left));
	}

__MATHH_INLINE float copysign(float _Left, float _Right)
	{	// return copysign
	return (copysignf(_Left, _Right));
	}

__MATHH_INLINE float erf(float _Left)
	{	// return erf
 	return (erff(_Left)); }

__MATHH_INLINE float erfc(float _Left)
	{	// return erfc
	return (erfcf(_Left));
	}

__MATHH_INLINE float exp2(float _Left)
	{	// return exp2
	return (exp2f(_Left));
	}

__MATHH_INLINE float expm1(float _Left)
	{	// return expml
	return (expm1f(_Left));
	}

__MATHH_INLINE float fdim(float _Left, float _Right)
	{	// return fdim
	return (fdimf(_Left, _Right));
	}

__MATHH_INLINE float fma(float _Left, float _Right, float _Addend)
	{	// return fma
	return (fmaf(_Left, _Right, _Addend));
	}

__MATHH_INLINE float fmax(float _Left, float _Right)
	{	// return fmax
	return (fmaxf(_Left, _Right));
	}

__MATHH_INLINE float fmin(float _Left, float _Right)
	{	// return fmin
	return (fminf(_Left, _Right));
	}

__MATHH_INLINE float hypot(float _Left, float _Right)
	{	// return hypot
	return (hypotf(_Left, _Right));
	}

__MATHH_INLINE int ilogb(float _Left)
	{	// return ilogb
	return (ilogbf(_Left));
	}

__MATHH_INLINE float lgamma(float _Left)
	{	// return lgamma
	return (lgammaf(_Left));
	}

__MATHH_INLINE _Longlong llrint(float _Left)
	{	// return llrint
	return (llrintf(_Left));
	}

__MATHH_INLINE _Longlong llround(float _Left)
	{	// return llround
	return (llroundf(_Left));
	}

__MATHH_INLINE float log1p(float _Left)
	{	// return loglp
	return (log1pf(_Left));
	}

__MATHH_INLINE float log2(float _Left)
	{	// return log2
	return (_FLog(_Left, -1));
	}

__MATHH_INLINE float logb(float _Left)
	{	// return logb
	return (logbf(_Left));
	}

__MATHH_INLINE long lrint(float _Left)
	{	// return lrint
	return (lrintf(_Left));
	}

__MATHH_INLINE long lround(float _Left)
	{	// return lround
	return (lroundf(_Left));
	}

__MATHH_INLINE float nearbyint(float _Left)
	{	// return nearbyint
	return (nearbyintf(_Left));
	}

__MATHH_INLINE float nextafter(float _Left, float _Right)
	{	// return nextafter
	return (nextafterf(_Left, _Right));
	}

__MATHH_INLINE float nexttoward(float _Left, long double _Right)
	{	// return nexttoward
	return (nexttowardf(_Left, _Right));
	}

__MATHH_INLINE float remainder(float _Left, float _Right)
	{	// return remainder
	return (remainderf(_Left, _Right));
	}

__MATHH_INLINE float remquo(float _Left, float _Right, int *_Pval)
	{	// return remquo
	return (remquof(_Left, _Right, _Pval));
	}

__MATHH_INLINE float rint(float _Left)
	{	// return rint
	return (rintf(_Left));
	}

__MATHH_INLINE float round(float _Left)
	{	// return round
	return (roundf(_Left));
	}

__MATHH_INLINE float scalbn(float _Left, int _Right)
	{	// return scalbn
	return (scalbnf(_Left, _Right));
	}

__MATHH_INLINE float scalbln(float _Left, long _Right)
	{	// return scalbln
	return (scalblnf(_Left, _Right));
	}

__MATHH_INLINE float tgamma(float _Left)
	{	// return tgamma
	return (tgammaf(_Left));
	}

__MATHH_INLINE float trunc(float _Left)
	{	// return trunc
	return (truncf(_Left));
	}
 #endif /* _IS_C9X */

 #if _HAS_TR1_DECLARATIONS
__MATHH_INLINE float assoc_laguerre(unsigned _Nv, unsigned _Mv,
	float _Xv)
	{	// return assoc_laguerre
	return (assoc_laguerref(_Nv, _Mv, _Xv));
	}

__MATHH_INLINE float assoc_legendre(unsigned _Nv, unsigned _Mv,
	float _Xv)
	{	// return assoc_legendre
	return (assoc_legendref(_Nv, _Mv, _Xv));
	}

__MATHH_INLINE float beta(float _Xv, float _Yv)
	{	// return beta
	return (betaf(_Xv, _Yv));
	}

__MATHH_INLINE float comp_ellint_1(float _Kv)
	{	// return comp_ellint_1
	return (comp_ellint_1f(_Kv));
	}

__MATHH_INLINE float comp_ellint_2(float _Kv)
	{	// return comp_ellint_2
	return (comp_ellint_2f(_Kv));
	}

__MATHH_INLINE float comp_ellint_3(float _Kv, float _Nu)
	{	// return comp_ellint_3
	return (comp_ellint_3f(_Kv, _Nu));
	}

__MATHH_INLINE float cyl_bessel_i(float _Nu, float _Xv)
	{	// return cyl_bessel_i
	return (cyl_bessel_if(_Nu, _Xv));
	}

__MATHH_INLINE float cyl_bessel_j(float _Nu, float _Xv)
	{	// return cyl_bessel_j
	return (cyl_bessel_jf(_Nu, _Xv));
	}

__MATHH_INLINE float cyl_bessel_k(float _Nu, float _Xv)
	{	// return cyl_bessel_k
	return (cyl_bessel_kf(_Nu, _Xv));
	}

__MATHH_INLINE float cyl_neumann(float _Nu, float _Xv)
	{	// return cyl_neumann
	return (cyl_neumannf(_Nu, _Xv));
	}

__MATHH_INLINE float ellint_1(float _Kv, float _Phi)
	{	// return ellint_1
	return (ellint_1f(_Kv, _Phi));
	}

__MATHH_INLINE float ellint_2(float _Kv, float _Phi)
	{	// return ellint_2
	return (ellint_2f(_Kv, _Phi));
	}

__MATHH_INLINE float ellint_3(float _Kv, float _Nu, float _Phi)
	{	// return ellint_3
	return (ellint_3f(_Kv, _Nu, _Phi));
	}

__MATHH_INLINE float expint(float _Xv)
	{	// return expint
	return (expintf(_Xv));
	}

__MATHH_INLINE float hermite(unsigned _Nv, float _Xv)
	{	// return hermite
	return (hermitef(_Nv, _Xv));
	}

__MATHH_INLINE float laguerre(unsigned _Nv, float _Xv)
	{	// return laguerre
	return (laguerref(_Nv, _Xv));
	}

__MATHH_INLINE float legendre(unsigned _Lv, float _Xv)
	{	// return legendre
	return (legendref(_Lv, _Xv));
	}

__MATHH_INLINE float riemann_zeta(float _Xv)
	{	// return riemann_zeta
	return (riemann_zetaf(_Xv));
	}

__MATHH_INLINE float sph_bessel(unsigned _Nv, float _Xv)
	{	// return sph_bessel
	return (sph_besself(_Nv, _Xv));
	}

__MATHH_INLINE float sph_legendre(unsigned _Lv, unsigned _Mv, float _Xv)
	{	// return sph_legendre
	return (sph_legendref(_Lv, _Mv, _Xv));
	}

__MATHH_INLINE float sph_neumann(unsigned _Nv, float _Xv)
	{	// return sph_neumann
	return (sph_neumannf(_Nv, _Xv));
	}

 #if _HAS_ADDED_FUNCTIONS
__MATHH_INLINE float besseli0(float _Xv)
	{	// return besseli0
	return (besseli0f(_Xv));
	}

__MATHH_INLINE float besseli1(float _Xv)
	{	// return besseli1
	return (besseli1f(_Xv));
	}

__MATHH_INLINE float besselj0(float _Xv)
	{	// return besselj0
	return (besselj0f(_Xv));
	}

__MATHH_INLINE float besselj1(float _Xv)
	{	// return besselj1
	return (besselj1f(_Xv));
	}

__MATHH_INLINE float besselk0(float _Xv)
	{	// return besselk0
	return (besselk0f(_Xv));
	}

__MATHH_INLINE float besselk1(float _Xv)
	{	// return besselk1
	return (besselk1f(_Xv));
	}

__MATHH_INLINE float bessely0(float _Xv)
	{	// return bessely0
	return (bessely0f(_Xv));
	}

__MATHH_INLINE float bessely1(float _Xv)
	{	// return bessely1
	return (bessely1f(_Xv));
	}
 #endif /* _HAS_ADDED_FUNCTIONS */

 #endif /* _HAS_TR1_DECLARATIONS */

// long double __MATHH_INLINES, FOR C++

__MATHH_INLINE long double cosl(long double _Left)
	{	// return cosine
	return (_LSin(_Left, 1));
	}

__MATHH_INLINE long double coshl(long double _Left)
	{	// return hyperbolic cosine
	return (_LCosh(_Left, 1.0L));
	}

__MATHH_INLINE long double logl(long double _Left)
	{	// return natural logarithm
	return (_LLog(_Left, 0));
	}

__MATHH_INLINE long double log10l(long double _Left)
	{	// return base-10 logarithm
	return (_LLog(_Left, 1));
	}

__MATHH_INLINE long double sinl(long double _Left)
	{	// return sine
	return (_LSin(_Left, 0));
	}

__MATHH_INLINE long double sinhl(long double _Left)
	{	// return hyperbolic sine
	return (_LSinh(_Left, 1.0L));
	}

 #if _HAS_C9X
__MATHH_INLINE long double log2l(long double _Left)
	{	// return base-2 logarithm
	return (_LLog(_Left, -1));
	}
 #endif /* _IS_C9X */

__MATHH_INLINE long double abs(long double _Left)	// OVERLOADS
	{	// return absolute value
	return (fabsl(_Left));
	}

__MATHH_INLINE long double acos(long double _Left)
	{	// return arccosine
	return (acosl(_Left));
	}

__MATHH_INLINE long double asin(long double _Left)
	{	// return arcsine
	return (asinl(_Left));
	}

__MATHH_INLINE long double atan(long double _Left)
	{	// return arctangent
	return (atanl(_Left));
	}

__MATHH_INLINE long double atan2(long double _Left, long double _Right)
	{	// return arctangent
	return (atan2l(_Left, _Right));
	}

__MATHH_INLINE long double ceil(long double _Left)
	{	// return ceiling
	return (ceill(_Left));
	}

__MATHH_INLINE long double cos(long double _Left)
	{	// return cosine
	return (_LSin(_Left, 1));
	}

__MATHH_INLINE long double cosh(long double _Left)
	{	// return hyperbolic cosine
	return (_LCosh(_Left, 1.0L));
	}

__MATHH_INLINE long double exp(long double _Left)
	{	// return exponential
	return (expl(_Left));
	}

__MATHH_INLINE long double fabs(long double _Left)
	{	// return absolute value
	return (fabsl(_Left));
	}

__MATHH_INLINE long double floor(long double _Left)
	{	// return floor
	return (floorl(_Left));
	}

__MATHH_INLINE long double fmod(long double _Left, long double _Right)
	{	// return modulus
	return (fmodl(_Left, _Right));
	}

__MATHH_INLINE long double frexp(long double _Left, int *_Right)
	{	// unpack exponent
	return (frexpl(_Left, _Right));
	}

__MATHH_INLINE long double ldexp(long double _Left, int _Right)
	{	// pack exponent
	return (ldexpl(_Left, _Right));
	}

__MATHH_INLINE long double log(long double _Left)
	{	// return natural logarithm
	return (_LLog(_Left, 0));
	}

__MATHH_INLINE long double log10(long double _Left)
	{	// return base-10 logarithm
	return (_LLog(_Left, 1));
	}

__MATHH_INLINE long double modf(long double _Left, long double *_Right)
	{	// unpack fraction
	return (modfl(_Left, _Right));
	}

__MATHH_INLINE long double pow(long double _Left, long double _Right)
	{	// raise to power
	return (powl(_Left, _Right));
	}

__MATHH_INLINE long double sin(long double _Left)
	{	// return sine
	return (_LSin(_Left, 0));
	}

__MATHH_INLINE long double sinh(long double _Left)
	{	// return hyperbolic sine
	return (_LSinh(_Left, 1.0L));
	}

__MATHH_INLINE long double sqrt(long double _Left)
	{	// return square root
	return (sqrtl(_Left));
	}

__MATHH_INLINE long double tan(long double _Left)
	{	// return tangent
	return (tanl(_Left));
	}

__MATHH_INLINE long double tanh(long double _Left)
	{	// return hyperbolic tangent
	return (tanhl(_Left));
	}

 #if _HAS_C9X
__MATHH_INLINE long double acosh(long double _Left)
	{	// return acosh
	return (acoshl(_Left));
	}

__MATHH_INLINE long double asinh(long double _Left)
	{	// return asinh
	return (asinhl(_Left));
	}

__MATHH_INLINE long double atanh(long double _Left)
	{	// return atanh
	return (atanhl(_Left));
	}

__MATHH_INLINE long double cbrt(long double _Left)
	{	// return cbrt
	return (cbrtl(_Left));
	}

__MATHH_INLINE long double copysign(long double _Left, long double _Right)
	{	// return copysign
	return (copysignl(_Left, _Right));
	}

__MATHH_INLINE long double erf(long double _Left)
	{	// return erf
	return (erfl(_Left));
	}

__MATHH_INLINE long double erfc(long double _Left)
	{	// return erfc
	return (erfcl(_Left));
	}

__MATHH_INLINE long double exp2(long double _Left)
	{	// return exp2
	return (exp2l(_Left));
	}

__MATHH_INLINE long double expm1(long double _Left)
	{	// return expml
	return (expm1l(_Left));
	}

__MATHH_INLINE long double fdim(long double _Left, long double _Right)
	{	// return fdim
	return (fdiml(_Left, _Right));
	}

__MATHH_INLINE long double fma(long double _Left, long double _Right,
	long double _Addend)
	{	// return fma
	return (fmal(_Left, _Right, _Addend));
	}

__MATHH_INLINE long double fmax(long double _Left, long double _Right)
	{	// return fmax
	return (fmaxl(_Left, _Right));
	}

__MATHH_INLINE long double fmin(long double _Left, long double _Right)
	{	// return fmin
	return (fminl(_Left, _Right));
	}

__MATHH_INLINE long double hypot(long double _Left, long double _Right)
	{	// return hypot
	return (hypotl(_Left, _Right));
	}

__MATHH_INLINE int ilogb(long double _Left)
	{	// return ilogb
	return (ilogbl(_Left));
	}

__MATHH_INLINE long double lgamma(long double _Left)
	{	// return lgamma
	return (lgammal(_Left));
	}

__MATHH_INLINE _Longlong llrint(long double _Left)
	{	// return llrint
	return (llrintl(_Left));
	}

__MATHH_INLINE _Longlong llround(long double _Left)
	{	// return llround
	return (llroundl(_Left));
	}

__MATHH_INLINE long double log1p(long double _Left)
	{	// return loglp
	return (log1pl(_Left));
	}

__MATHH_INLINE long double log2(long double _Left)
	{	// return log2
	return (_LLog(_Left, -1));
	}

__MATHH_INLINE long double logb(long double _Left)
	{	// return logb
	return (logbl(_Left));
	}

__MATHH_INLINE long lrint(long double _Left)
	{	// return lrint
	return (lrintl(_Left));
	}

__MATHH_INLINE long lround(long double _Left)
	{	// return lround
	return (lroundl(_Left));
	}

__MATHH_INLINE long double nearbyint(long double _Left)
	{	// return nearbyint
	return (nearbyintl(_Left));
	}

__MATHH_INLINE long double nextafter(long double _Left, long double _Right)
	{	// return nextafter
	return (nextafterl(_Left, _Right));
	}

__MATHH_INLINE long double nexttoward(long double _Left, long double _Right)
	{	// return nexttoward
	return (nexttowardl(_Left, _Right));
	}

__MATHH_INLINE long double remainder(long double _Left, long double _Right)
	{	// return remainder
	return (remainderl(_Left, _Right));
	}

__MATHH_INLINE long double remquo(long double _Left, long double _Right,
	int *_Pval)
	{	// return remquo
	return (remquol(_Left, _Right, _Pval));
	}

__MATHH_INLINE long double rint(long double _Left)
	{	// return rint
	return (rintl(_Left));
	}

__MATHH_INLINE long double round(long double _Left)
	{	// return round
	return (roundl(_Left));
	}

__MATHH_INLINE long double scalbn(long double _Left, int _Right)
	{	// return scalbn
	return (scalbnl(_Left, _Right));
	}

__MATHH_INLINE long double scalbln(long double _Left, long _Right)
	{	// return scalbln
	return (scalblnl(_Left, _Right));
	}

__MATHH_INLINE long double tgamma(long double _Left)
	{	// return tgamma
	return (tgammal(_Left));
	}

__MATHH_INLINE long double trunc(long double _Left)
	{	// return trunc
	return (truncl(_Left));
	}
 #endif /* _IS_C9X */

 #if _HAS_TR1_DECLARATIONS
__MATHH_INLINE long double assoc_laguerre(unsigned _Nv, unsigned _Mv,
	long double _Xv)
	{	// return assoc_laguerre
	return (assoc_laguerrel(_Nv, _Mv, _Xv));
	}

__MATHH_INLINE long double assoc_legendre(unsigned _Nv, unsigned _Mv,
	long double _Xv)
	{	// return assoc_legendre
	return (assoc_legendrel(_Nv, _Mv, _Xv));
	}

__MATHH_INLINE long double beta(long double _Xv, long double _Yv)
	{	// return beta
	return (betal(_Xv, _Yv));
	}

__MATHH_INLINE long double comp_ellint_1(long double _Kv)
	{	// return comp_ellint_1
	return (comp_ellint_1l(_Kv));
	}

__MATHH_INLINE long double comp_ellint_2(long double _Kv)
	{	// return comp_ellint_2
	return (comp_ellint_2l(_Kv));
	}

__MATHH_INLINE long double comp_ellint_3(long double _Kv, long double _Nu)
	{	// return comp_ellint_3
	return (comp_ellint_3l(_Kv, _Nu));
	}

__MATHH_INLINE long double cyl_bessel_i(long double _Nu, long double _Xv)
	{	// return cyl_bessel_i
	return (cyl_bessel_il(_Nu, _Xv));
	}

__MATHH_INLINE long double cyl_bessel_j(long double _Nu, long double _Xv)
	{	// return cyl_bessel_j
	return (cyl_bessel_jl(_Nu, _Xv));
	}

__MATHH_INLINE long double cyl_bessel_k(long double _Nu, long double _Xv)
	{	// return cyl_bessel_k
	return (cyl_bessel_kl(_Nu, _Xv));
	}

__MATHH_INLINE long double cyl_neumann(long double _Nu, long double _Xv)
	{	// return cyl_neumann
	return (cyl_neumannl(_Nu, _Xv));
	}

__MATHH_INLINE long double ellint_1(long double _Kv, long double _Phi)
	{	// return ellint_1
	return (ellint_1l(_Kv, _Phi));
	}

__MATHH_INLINE long double ellint_2(long double _Kv, long double _Phi)
	{	// return ellint_2
	return (ellint_2l(_Kv, _Phi));
	}

__MATHH_INLINE long double ellint_3(long double _Kv, long double _Nu,
	long double _Phi)
	{	// return ellint_3
	return (ellint_3l(_Kv, _Nu, _Phi));
	}

__MATHH_INLINE long double expint(long double _Xv)
	{	// return expint
	return (expintl(_Xv));
	}

__MATHH_INLINE long double hermite(unsigned _Nv, long double _Xv)
	{	// return hermite
	return (hermitel(_Nv, _Xv));
	}

__MATHH_INLINE long double laguerre(unsigned _Nv, long double _Xv)
	{	// return laguerre
	return (laguerrel(_Nv, _Xv));
	}

__MATHH_INLINE long double legendre(unsigned _Lv, long double _Xv)
	{	// return legendre
	return (legendrel(_Lv, _Xv));
	}

__MATHH_INLINE long double riemann_zeta(long double _Xv)
	{	// return riemann_zeta
	return (riemann_zetal(_Xv));
	}

__MATHH_INLINE long double sph_bessel(unsigned _Nv, long double _Xv)
	{	// return sph_bessel
	return (sph_bessell(_Nv, _Xv));
	}

__MATHH_INLINE long double sph_legendre(unsigned _Lv, unsigned _Mv,
	long double _Xv)
	{	// return sph_legendre
	return (sph_legendrel(_Lv, _Mv, _Xv));
	}

__MATHH_INLINE long double sph_neumann(unsigned _Nv, long double _Xv)
	{	// return sph_neumann
	return (sph_neumannl(_Nv, _Xv));
	}

 #if _HAS_ADDED_FUNCTIONS
__MATHH_INLINE long double besseli0(long double _Xv)
	{	// return besseli0
	return (besseli0l(_Xv));
	}

__MATHH_INLINE long double besseli1(long double _Xv)
	{	// return besseli1
	return (besseli1l(_Xv));
	}

__MATHH_INLINE long double besselj0(long double _Xv)
	{	// return besselj0
	return (besselj0l(_Xv));
	}

__MATHH_INLINE long double besselj1(long double _Xv)
	{	// return besselj1
	return (besselj1l(_Xv));
	}

__MATHH_INLINE long double besselk0(long double _Xv)
	{	// return besselk0
	return (besselk0l(_Xv));
	}

__MATHH_INLINE long double besselk1(long double _Xv)
	{	// return besselk1
	return (besselk1l(_Xv));
	}

__MATHH_INLINE long double bessely0(long double _Xv)
	{	// return bessely0
	return (bessely0l(_Xv));
	}

__MATHH_INLINE long double bessely1(long double _Xv)
	{	// return bessely1
	return (bessely1l(_Xv));
	}
 #endif /* _HAS_ADDED_FUNCTIONS */

 #endif /* _HAS_TR1_DECLARATIONS */
}	// extern "C++"

 #else /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
_C_LIB_DECL
		/* double MACRO OVERRIDES, FOR C */
double cos(double);
double cosh(double);
double log(double);
double log10(double);
double sin(double);
double sinh(double);

 #define cos(x)		_Sin(x, 1)
 #define cosh(x)	_Cosh(x, 1.0)
 #define log(x)		_Log(x, 0)
 #define log10(x)	_Log(x, 1)
 #define sin(x)		_Sin(x, 0)
 #define sinh(x)	_Sinh(x, 1.0)

 #if _HAS_C9X
double log2(double);

 #define log2(x)	_Log(x, -1)
 #endif /* _IS_C9X */

		/* float MACRO OVERRIDES, FOR C */
float cosf(float);
float coshf(float);
float logf(float);
float log10f(float);
float sinf(float);
float sinhf(float);

 #define cosf(x)	_FSin(x, 1)
 #define coshf(x)	_FCosh(x, 1.0F)
 #define logf(x)	_FLog(x, 0)
 #define log10f(x)	_FLog(x, 1)
 #define sinf(x)	_FSin(x, 0)
 #define sinhf(x)	_FSinh(x, 1.0F)

 #if _HAS_C9X
float log2f(float);

 #define log2f(x)	_FLog(x, -1)
 #endif /* _IS_C9X */

		/* long double MACRO OVERRIDES, FOR C */
long double cosl(long double);
long double coshl(long double);
long double logl(long double);
long double log10l(long double);
long double sinl(long double);
long double sinhl(long double);

 #define cosl(x)	_LSin(x, 1)
 #define coshl(x)	_LCosh(x, 1.0L)
 #define logl(x)	_LLog(x, 0)
 #define log10l(x)	_LLog(x, 1)
 #define sinl(x)	_LSin(x, 0)
 #define sinhl(x)	_LSinh(x, 1.0L)

 #if _HAS_C9X
long double log2l(long double);

 #define log2l(x)	_LLog(x, -1)
 #endif /* _IS_C9X */

_END_C_LIB_DECL
 #endif /* defined(__cplusplus) && !defined(_NO_CPP_INLINES) */
_C_STD_END

		/* GENERIC TEMPLATES */

 #if _HAS_GENERIC_TEMPLATES
extern "C++" {
_C_STD_BEGIN
	_TGEN_RC(acos)	// real or complex generic overloads
	_TGEN_RC(acosh)
	_TGEN_RC(asin)
	_TGEN_RC(asinh)
	_TGEN_RC(atan)
	_TGEN_RC(atanh)
	_TGEN_RC(cos)
	_TGEN_RC(cosh)
	_TGEN_RC(exp)
	_TGEN_RC0(fabs)
	_TGEN_RC(log)
	_TGEN_RC(log10)
	_TGEN_RC2(pow)
	_TGEN_RC(sin)
	_TGEN_RC(sinh)
	_TGEN_RC(sqrt)
	_TGEN_RC(tan)
	_TGEN_RC(tanh)

	_TGEN_R2(atan2)	// real generic overloads
	_TGEN_R(cbrt)
	_TGEN_R(ceil)
	_TGEN_R2(copysign)
	_TGEN_R(erf)
	_TGEN_R(erfc)
	_TGEN_R(exp2)
	_TGEN_R(expm1)
	_TGEN_R2(fdim)
	_TGEN_R(floor)
	_TGEN_R3(fma)
	_TGEN_R2(fmax)
	_TGEN_R2(fmin)
	_TGEN_R2(fmod)
	_TGEN_RX(frexp, int *)
	_TGEN_R2(hypot)
	_TGEN_RI(ilogb, int)
	_TGEN_RX(ldexp, int)
	_TGEN_R(lgamma)
	_TGEN_RI(llrint, _Longlong)
	_TGEN_RI(llround, _Longlong)
	_TGEN_R(log1p)
	_TGEN_R(log2)
	_TGEN_R(logb)
	_TGEN_RI(lrint, long)
	_TGEN_RI(lround, long)
	_TGEN_R(nearbyint)
	_TGEN_R2(nextafter)
	_TGEN_RX(nexttoward, long double)
	_TGEN_R2(remainder)
	_TGEN_R2X(remquo, int *)
	_TGEN_R(rint)
	_TGEN_R(round)
	_TGEN_RX(scalbln, long)
	_TGEN_RX(scalbn, int)
	_TGEN_R(tgamma)
	_TGEN_R(trunc)
_C_STD_END
}	// extern "C++"
 #endif /* _HAS_GENERIC_TEMPLATES */

#undef __MATHH_INLINE

#endif /* _MATH */

 #if defined(_STD_USING)

  #if !defined(_NO_CPP_INLINES)
using _CSTD abs;
  #endif /* !defined(_NO_CPP_INLINES) */

using _CSTD acos; using _CSTD asin;
using _CSTD atan; using _CSTD atan2; using _CSTD ceil;
using _CSTD cos; using _CSTD cosh; using _CSTD exp;
using _CSTD fabs; using _CSTD floor; using _CSTD fmod;
using _CSTD frexp; using _CSTD ldexp; using _CSTD log;
using _CSTD log10; using _CSTD modf; using _CSTD pow;
using _CSTD sin; using _CSTD sinh; using _CSTD sqrt;
using _CSTD tan; using _CSTD tanh;

using _CSTD acosf; using _CSTD asinf;
using _CSTD atanf; using _CSTD atan2f; using _CSTD ceilf;
using _CSTD cosf; using _CSTD coshf; using _CSTD expf;
using _CSTD fabsf; using _CSTD floorf; using _CSTD fmodf;
using _CSTD frexpf; using _CSTD ldexpf; using _CSTD logf;
using _CSTD log10f; using _CSTD modff; using _CSTD powf;
using _CSTD sinf; using _CSTD sinhf; using _CSTD sqrtf;
using _CSTD tanf; using _CSTD tanhf;

using _CSTD acosl; using _CSTD asinl;
using _CSTD atanl; using _CSTD atan2l; using _CSTD ceill;
using _CSTD cosl; using _CSTD coshl; using _CSTD expl;
using _CSTD fabsl; using _CSTD floorl; using _CSTD fmodl;
using _CSTD frexpl; using _CSTD ldexpl; using _CSTD logl;
using _CSTD log10l; using _CSTD modfl; using _CSTD powl;
using _CSTD sinl; using _CSTD sinhl; using _CSTD sqrtl;
using _CSTD tanl; using _CSTD tanhl;

 #if _HAS_C9X

 #if _HAS_GENERIC_TEMPLATES
using _CSTD _FPCOMP; using _CSTD fpclassify;
using _CSTD signbit; using _CSTD isfinite; using _CSTD isinf;
using _CSTD isnan; using _CSTD isnormal;
using _CSTD isgreater; using _CSTD isgreaterequal;
using _CSTD isless; using _CSTD islessequal; using _CSTD islessgreater;
using _CSTD isunordered;
 #endif	/* _HAS_GENERIC_TEMPLATES */

using _CSTD float_t; using _CSTD double_t;

using _CSTD acosh; using _CSTD asinh; using _CSTD atanh;
using _CSTD cbrt; using _CSTD erf; using _CSTD erfc;
using _CSTD expm1; using _CSTD exp2;
using _CSTD hypot; using _CSTD ilogb; using _CSTD lgamma;
using _CSTD log1p; using _CSTD log2; using _CSTD logb;
using _CSTD llrint; using _CSTD lrint; using _CSTD nearbyint;
using _CSTD rint; using _CSTD llround; using _CSTD lround;
using _CSTD fdim; using _CSTD fma; using _CSTD fmax; using _CSTD fmin;
using _CSTD round; using _CSTD trunc;
using _CSTD remainder; using _CSTD remquo;
using _CSTD copysign; using _CSTD nan; using _CSTD nextafter;
using _CSTD scalbn; using _CSTD scalbln;
using _CSTD nexttoward; using _CSTD tgamma;

using _CSTD acoshf; using _CSTD asinhf; using _CSTD atanhf;
using _CSTD cbrtf; using _CSTD erff; using _CSTD erfcf;
using _CSTD expm1f; using _CSTD exp2f;
using _CSTD hypotf; using _CSTD ilogbf; using _CSTD lgammaf;
using _CSTD log1pf; using _CSTD log2f; using _CSTD logbf;
using _CSTD llrintf; using _CSTD lrintf; using _CSTD nearbyintf;
using _CSTD rintf; using _CSTD llroundf; using _CSTD lroundf;
using _CSTD fdimf; using _CSTD fmaf; using _CSTD fmaxf; using _CSTD fminf;
using _CSTD roundf; using _CSTD truncf;
using _CSTD remainderf; using _CSTD remquof;
using _CSTD copysignf; using _CSTD nanf;
using _CSTD nextafterf; using _CSTD scalbnf; using _CSTD scalblnf;
using _CSTD nexttowardf; using _CSTD tgammaf;

using _CSTD acoshl; using _CSTD asinhl; using _CSTD atanhl;
using _CSTD cbrtl; using _CSTD erfl; using _CSTD erfcl;
using _CSTD expm1l; using _CSTD exp2l;
using _CSTD hypotl; using _CSTD ilogbl; using _CSTD lgammal;
using _CSTD log1pl; using _CSTD log2l; using _CSTD logbl;
using _CSTD llrintl; using _CSTD lrintl; using _CSTD nearbyintl;
using _CSTD rintl; using _CSTD llroundl; using _CSTD lroundl;
using _CSTD fdiml; using _CSTD fmal; using _CSTD fmaxl; using _CSTD fminl;
using _CSTD roundl; using _CSTD truncl;
using _CSTD remainderl; using _CSTD remquol;
using _CSTD copysignl; using _CSTD nanl;
using _CSTD nextafterl; using _CSTD scalbnl; using _CSTD scalblnl;
using _CSTD nexttowardl; using _CSTD tgammal;
 #endif /* _IS_C9X */

 #if _HAS_TR1_DECLARATIONS
using _CSTD assoc_laguerre; using _CSTD assoc_legendre; using _CSTD beta;
using _CSTD comp_ellint_1; using _CSTD comp_ellint_2;
using _CSTD comp_ellint_3; using _CSTD cyl_bessel_i;
using _CSTD cyl_bessel_j; using _CSTD cyl_bessel_k;
using _CSTD cyl_neumann; using _CSTD ellint_1; using _CSTD ellint_2;
using _CSTD ellint_3; using _CSTD expint; using _CSTD hermite;
using _CSTD laguerre; using _CSTD legendre; using _CSTD riemann_zeta;
using _CSTD sph_bessel; using _CSTD sph_legendre; using _CSTD sph_neumann;

using _CSTD assoc_laguerref; using _CSTD assoc_legendref; using _CSTD betaf;
using _CSTD comp_ellint_1f; using _CSTD comp_ellint_2f;
using _CSTD comp_ellint_3f; using _CSTD cyl_bessel_if;
using _CSTD cyl_bessel_jf; using _CSTD cyl_bessel_kf;
using _CSTD cyl_neumannf; using _CSTD ellint_1f; using _CSTD ellint_2f;
using _CSTD ellint_3f; using _CSTD expintf; using _CSTD hermitef;
using _CSTD laguerref; using _CSTD legendref; using _CSTD riemann_zetaf;
using _CSTD sph_besself; using _CSTD sph_legendref; using _CSTD sph_neumannf;

using _CSTD assoc_laguerrel; using _CSTD assoc_legendrel; using _CSTD betal;
using _CSTD comp_ellint_1l; using _CSTD comp_ellint_2l;
using _CSTD comp_ellint_3l; using _CSTD cyl_bessel_il;
using _CSTD cyl_bessel_jl; using _CSTD cyl_bessel_kl;
using _CSTD cyl_neumannl; using _CSTD ellint_1l; using _CSTD ellint_2l;
using _CSTD ellint_3l; using _CSTD expintl; using _CSTD hermitel;
using _CSTD laguerrel; using _CSTD legendrel; using _CSTD riemann_zetal;
using _CSTD sph_bessell; using _CSTD sph_legendrel; using _CSTD sph_neumannl;

 #if _HAS_ADDED_FUNCTIONS
using _CSTD besseli0; using _CSTD besseli1;
using _CSTD besselj0; using _CSTD besselj1;
using _CSTD besselk0; using _CSTD besselk1;
using _CSTD bessely0; using _CSTD bessely1;
using _CSTD j0; using _CSTD j1; using _CSTD jn;
using _CSTD y0; using _CSTD y1; using _CSTD yn;

using _CSTD besseli0f; using _CSTD besseli1f;
using _CSTD besselj0f; using _CSTD besselj1f;
using _CSTD besselk0f; using _CSTD besselk1f;
using _CSTD bessely0f; using _CSTD bessely1f;
using _CSTD j0f; using _CSTD j1f; using _CSTD jnf;
using _CSTD y0f; using _CSTD y1f; using _CSTD ynf;

using _CSTD besseli0l; using _CSTD besseli1l;
using _CSTD besselj0l; using _CSTD besselj1l;
using _CSTD besselk0l; using _CSTD besselk1l;
using _CSTD bessely0l; using _CSTD bessely1l;
using _CSTD j0l; using _CSTD j1l; using _CSTD jnl;
using _CSTD y0l; using _CSTD y1l; using _CSTD ynl;
 #endif /* _HAS_ADDED_FUNCTIONS */

 #endif /* _HAS_TR1_DECLARATIONS */
 #endif /* defined(_STD_USING) */

/*
 * Copyright (c) 1992-2010 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.30:0009 */
