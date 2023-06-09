/* stdbool.h standard header */
#ifndef _STDBOOL
#define _STDBOOL
#ifndef _YVALS
 #include <yvals.h>
#endif /* _YVALS */

_C_STD_BEGIN
 #define __bool_true_false_are_defined	1

 #ifndef __cplusplus
		/* TYPES */

 #if 199901L <= __STDC_VERSION__

 #if defined(__GNUC__) && __GNUC__ < 3
typedef unsigned int _Bool;
 #endif /* defined(__GNUC__) && __GNUC__ < 3 */

 #else /* 199901L <= __STDC_VERSION__ */

 #if !defined(__GNUC__) || __GNUC__ < 3
typedef unsigned int _Bool;
 #endif /* !defined(__GNUC__) || __GNUC__ < 3 */

 #endif /* 199901L <= __STDC_VERSION__ */

		/* MACROS */
 #define bool	_Bool
 #define false	0
 #define true	1
 #endif /* __cplusplus */
_C_STD_END
#endif /* _STDBOOL */

/*
 * Copyright (c) 1992-2010 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.30:0009 */
