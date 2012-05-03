#if !defined( __DEFINES_H_ )
#define __DEFINES_H_

#include "ParserTable.h"

typedef				__int8	int8;
typedef	unsigned	__int8	uint8;

typedef				__int16	int16;
typedef	unsigned	__int16	uint16;

typedef				int		int32;
typedef unsigned	int		uint32;

typedef				float	float32;
typedef				double	float64;	

#define GET_PREFIX(_ind_)	\
	ms_ppszValuePrefix[_ind_]

#define DECLARE_PREFIX_TABLE()	\
	static const char *ms_ppszValuePrefix[]

#define IMPLEMENT_PREFIX_TABLE(_class_)	\
	const char * _class_::ms_ppszValuePrefix[] = \
	{ \
		"string:\"", \
		"int:", \
		"dword:", \
		"float:", \
		"vector3:", \
		"vector4:", \
		"rgbacolor:", \
		"hex:", \
		"" \
	};

#define GET_POSTFIX(_ind_)	\
	ms_ppszValuePostfix[_ind_]

#define DECLARE_POSTFIX_TABLE()	\
	static const char *ms_ppszValuePostfix[]

#define IMPLEMENT_POSTFIX_TABLE(_class_)	\
	const char * _class_::ms_ppszValuePostfix[] = \
	{ \
		"\"", \
		"", \
		"", \
		"", \
		"", \
		"", \
		"", \
		"", \
	};


#endif	//	__DEFINES_H_