/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 32 "core/pbrtparse.yy"

#include "api.h"
#include "pbrt.h"
#include "paramset.h"
#include <stdarg.h>

#ifdef PBRT_IS_WINDOWS
#pragma warning(disable:4065)
#pragma warning(disable:4996)
#pragma warning(disable:4018)
#endif // PBRT_IS_WINDOWS

extern int yylex();
extern void include_push(char *filename);
int line_num = 0;
string current_file;

#define YYMAXDEPTH 100000000

void yyerror(const char *str) {
    Error("Parsing error: %s", str);
    exit(1);
}



struct ParamArray {
    ParamArray() {
        isString = false;
        element_size = allocated = nelems = 0;
        array = NULL;
    }
    bool isString;
    int element_size;
    int allocated;
    int nelems;
    void *array;
};



struct ParamListItem {
    ParamListItem(const char *t, ParamArray *array) {
        arg = array->array;
        name = t;
        size = array->nelems;
        isString = array->isString;
        array->allocated = 0;
        array->nelems = 0;
        array->array = NULL;
    }
    const char *name;
    void *arg;
    int size;
    bool isString;
};



static vector<ParamListItem> cur_paramlist;

static ParamArray *cur_array = NULL;

static void AddArrayElement(void *elem) {
    if (cur_array->nelems >= cur_array->allocated) {
        cur_array->allocated = 2*cur_array->allocated + 1;
        cur_array->array = realloc(cur_array->array,
            cur_array->allocated*cur_array->element_size);
    }
    char *next = ((char *)cur_array->array) + cur_array->nelems * cur_array->element_size;
    Assert(cur_array->element_size == 4 || cur_array->element_size == 8);
    if (cur_array->element_size == 4)
        *((uint32_t *)next) = *((uint32_t *)elem);
    else
        *((uint64_t *)next) = *((uint64_t *)elem);
    cur_array->nelems++;
}



static void ArrayFree(ParamArray *ra) {
    if (ra->isString && ra->array)
        for (int i = 0; i < ra->nelems; ++i) free(((char **)ra->array)[i]);
    free(ra->array);
    delete ra;
}



static void FreeArgs() {
    for (uint32_t i = 0; i < cur_paramlist.size(); ++i)
        free((char *)cur_paramlist[i].arg);
    cur_paramlist.erase(cur_paramlist.begin(), cur_paramlist.end());
}



static bool VerifyArrayLength(ParamArray *arr, int required,
    const char *command) {
    if (arr->nelems != required) {
        Error("\"%s\" requires a %d element array! (%d found)",
                    command, required, arr->nelems);
        return false;
    }
    return true;
}


enum { PARAM_TYPE_INT, PARAM_TYPE_BOOL, PARAM_TYPE_FLOAT, PARAM_TYPE_POINT,
    PARAM_TYPE_VECTOR, PARAM_TYPE_NORMAL, PARAM_TYPE_RGB, PARAM_TYPE_XYZ,
    PARAM_TYPE_BLACKBODY, PARAM_TYPE_SPECTRUM,
    PARAM_TYPE_STRING, PARAM_TYPE_TEXTURE };
static const char *paramTypeToName(int type);
static void InitParamSet(ParamSet &ps, SpectrumType);
static bool lookupType(const char *name, int *type, string &sname);
#define YYPRINT(file, type, value)  { \
    if ((type) == ID || (type) == STRING) \
        fprintf ((file), " %s", (value).string); \
    else if ((type) == NUM) \
        fprintf ((file), " %f", (value).num); \
}



#line 196 "core/pbrtparse.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "pbrtparse.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_STRING = 3,                     /* STRING  */
  YYSYMBOL_ID = 4,                         /* ID  */
  YYSYMBOL_NUM = 5,                        /* NUM  */
  YYSYMBOL_LBRACK = 6,                     /* LBRACK  */
  YYSYMBOL_RBRACK = 7,                     /* RBRACK  */
  YYSYMBOL_ACCELERATOR = 8,                /* ACCELERATOR  */
  YYSYMBOL_ACTIVETRANSFORM = 9,            /* ACTIVETRANSFORM  */
  YYSYMBOL_ALL = 10,                       /* ALL  */
  YYSYMBOL_AREALIGHTSOURCE = 11,           /* AREALIGHTSOURCE  */
  YYSYMBOL_ATTRIBUTEBEGIN = 12,            /* ATTRIBUTEBEGIN  */
  YYSYMBOL_ATTRIBUTEEND = 13,              /* ATTRIBUTEEND  */
  YYSYMBOL_CAMERA = 14,                    /* CAMERA  */
  YYSYMBOL_CONCATTRANSFORM = 15,           /* CONCATTRANSFORM  */
  YYSYMBOL_COORDINATESYSTEM = 16,          /* COORDINATESYSTEM  */
  YYSYMBOL_COORDSYSTRANSFORM = 17,         /* COORDSYSTRANSFORM  */
  YYSYMBOL_ENDTIME = 18,                   /* ENDTIME  */
  YYSYMBOL_FILM = 19,                      /* FILM  */
  YYSYMBOL_IDENTITY = 20,                  /* IDENTITY  */
  YYSYMBOL_INCLUDE = 21,                   /* INCLUDE  */
  YYSYMBOL_LIGHTSOURCE = 22,               /* LIGHTSOURCE  */
  YYSYMBOL_LOOKAT = 23,                    /* LOOKAT  */
  YYSYMBOL_MAKENAMEDMATERIAL = 24,         /* MAKENAMEDMATERIAL  */
  YYSYMBOL_MATERIAL = 25,                  /* MATERIAL  */
  YYSYMBOL_NAMEDMATERIAL = 26,             /* NAMEDMATERIAL  */
  YYSYMBOL_OBJECTBEGIN = 27,               /* OBJECTBEGIN  */
  YYSYMBOL_OBJECTEND = 28,                 /* OBJECTEND  */
  YYSYMBOL_OBJECTINSTANCE = 29,            /* OBJECTINSTANCE  */
  YYSYMBOL_PIXELFILTER = 30,               /* PIXELFILTER  */
  YYSYMBOL_RENDERER = 31,                  /* RENDERER  */
  YYSYMBOL_REVERSEORIENTATION = 32,        /* REVERSEORIENTATION  */
  YYSYMBOL_ROTATE = 33,                    /* ROTATE  */
  YYSYMBOL_SAMPLER = 34,                   /* SAMPLER  */
  YYSYMBOL_SCALE = 35,                     /* SCALE  */
  YYSYMBOL_SHAPE = 36,                     /* SHAPE  */
  YYSYMBOL_STARTTIME = 37,                 /* STARTTIME  */
  YYSYMBOL_SURFACEINTEGRATOR = 38,         /* SURFACEINTEGRATOR  */
  YYSYMBOL_TEXTURE = 39,                   /* TEXTURE  */
  YYSYMBOL_TRANSFORMBEGIN = 40,            /* TRANSFORMBEGIN  */
  YYSYMBOL_TRANSFORMEND = 41,              /* TRANSFORMEND  */
  YYSYMBOL_TRANSFORMTIMES = 42,            /* TRANSFORMTIMES  */
  YYSYMBOL_TRANSFORM = 43,                 /* TRANSFORM  */
  YYSYMBOL_TRANSLATE = 44,                 /* TRANSLATE  */
  YYSYMBOL_VOLUME = 45,                    /* VOLUME  */
  YYSYMBOL_VOLUMEINTEGRATOR = 46,          /* VOLUMEINTEGRATOR  */
  YYSYMBOL_WORLDBEGIN = 47,                /* WORLDBEGIN  */
  YYSYMBOL_WORLDEND = 48,                  /* WORLDEND  */
  YYSYMBOL_HIGH_PRECEDENCE = 49,           /* HIGH_PRECEDENCE  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_start = 51,                     /* start  */
  YYSYMBOL_array_init = 52,                /* array_init  */
  YYSYMBOL_string_array_init = 53,         /* string_array_init  */
  YYSYMBOL_num_array_init = 54,            /* num_array_init  */
  YYSYMBOL_array = 55,                     /* array  */
  YYSYMBOL_string_array = 56,              /* string_array  */
  YYSYMBOL_single_element_string_array = 57, /* single_element_string_array  */
  YYSYMBOL_string_list = 58,               /* string_list  */
  YYSYMBOL_string_list_entry = 59,         /* string_list_entry  */
  YYSYMBOL_num_array = 60,                 /* num_array  */
  YYSYMBOL_single_element_num_array = 61,  /* single_element_num_array  */
  YYSYMBOL_num_list = 62,                  /* num_list  */
  YYSYMBOL_num_list_entry = 63,            /* num_list_entry  */
  YYSYMBOL_paramlist = 64,                 /* paramlist  */
  YYSYMBOL_paramlist_init = 65,            /* paramlist_init  */
  YYSYMBOL_paramlist_contents = 66,        /* paramlist_contents  */
  YYSYMBOL_paramlist_entry = 67,           /* paramlist_entry  */
  YYSYMBOL_pbrt_stmt_list = 68,            /* pbrt_stmt_list  */
  YYSYMBOL_pbrt_stmt = 69                  /* pbrt_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  136

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   304


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   180,   180,   186,   194,   202,   210,   216,   223,   230,
     238,   244,   249,   255,   263,   270,   278,   284,   289,   295,
     303,   309,   322,   328,   333,   341,   346,   352,   361,   367,
     373,   379,   388,   394,   400,   409,   417,   423,   429,   438,
     444,   450,   459,   465,   474,   483,   489,   495,   501,   507,
     516,   525,   531,   537,   546,   552,   561,   570,   579,   585,
     591,   597,   605,   611,   620,   629,   635
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "STRING", "ID", "NUM",
  "LBRACK", "RBRACK", "ACCELERATOR", "ACTIVETRANSFORM", "ALL",
  "AREALIGHTSOURCE", "ATTRIBUTEBEGIN", "ATTRIBUTEEND", "CAMERA",
  "CONCATTRANSFORM", "COORDINATESYSTEM", "COORDSYSTRANSFORM", "ENDTIME",
  "FILM", "IDENTITY", "INCLUDE", "LIGHTSOURCE", "LOOKAT",
  "MAKENAMEDMATERIAL", "MATERIAL", "NAMEDMATERIAL", "OBJECTBEGIN",
  "OBJECTEND", "OBJECTINSTANCE", "PIXELFILTER", "RENDERER",
  "REVERSEORIENTATION", "ROTATE", "SAMPLER", "SCALE", "SHAPE", "STARTTIME",
  "SURFACEINTEGRATOR", "TEXTURE", "TRANSFORMBEGIN", "TRANSFORMEND",
  "TRANSFORMTIMES", "TRANSFORM", "TRANSLATE", "VOLUME", "VOLUMEINTEGRATOR",
  "WORLDBEGIN", "WORLDEND", "HIGH_PRECEDENCE", "$accept", "start",
  "array_init", "string_array_init", "num_array_init", "array",
  "string_array", "single_element_string_array", "string_list",
  "string_list_entry", "num_array", "single_element_num_array", "num_list",
  "num_list_entry", "paramlist", "paramlist_init", "paramlist_contents",
  "paramlist_entry", "pbrt_stmt_list", "pbrt_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-119)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-6)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      58,     1,    -4,     4,  -119,  -119,     8,  -119,    16,    18,
      21,  -119,    22,    24,    25,    28,    29,    31,    32,  -119,
      33,    34,    35,  -119,    36,    37,    38,    39,    41,    43,
    -119,  -119,    42,  -119,    44,    45,    47,  -119,  -119,    51,
      58,  -119,  -119,  -119,  -119,  -119,  -119,  -119,    46,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,    48,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,    49,  -119,    50,  -119,  -119,    53,
      52,  -119,    54,  -119,  -119,  -119,  -119,  -119,    55,  -119,
    -119,  -119,    56,  -119,  -119,  -119,    57,  -119,  -119,  -119,
    -119,    59,  -119,    71,  -119,  -119,    60,  -119,    90,  -119,
    -119,  -119,  -119,    55,   100,  -119,  -119,   103,   104,  -119,
    -119,  -119,    10,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
     105,  -119,  -119,   106,   109,  -119,   108,   107,  -119,  -119,
     110,  -119,  -119,   111,   112,  -119
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,    32,    33,     0,     3,     0,     0,
       0,    39,     0,     0,     0,     0,     0,     0,     0,    47,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
      58,    59,     0,     3,     0,     0,     0,    65,    66,     0,
       2,    26,    21,    28,    29,    30,    21,    21,     5,    35,
      15,    36,    37,    21,    40,    21,     0,    21,    21,    45,
      46,    48,    21,    21,     0,    21,     0,    21,    21,     0,
       0,    61,     0,    21,    21,     1,    25,    27,    23,    31,
      34,     5,     0,    16,    38,    41,     0,    43,    44,    49,
      50,     0,    53,     0,    55,    56,     0,    60,     0,    64,
      63,     3,    20,    23,     5,    18,    19,     0,     0,    54,
      21,    62,     4,    24,     6,     9,     7,    22,    14,    17,
       0,    52,    57,     4,     0,    10,     0,     4,    12,    13,
       0,     8,    11,     0,     0,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -119,  -119,   -62,  -119,  -119,  -119,  -119,  -119,  -119,  -118,
     -33,  -119,  -119,   -78,   -45,  -119,   -43,  -119,  -119,    78
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    39,    48,   124,    82,   113,   114,   115,   127,   125,
      49,    50,   104,    83,    77,    78,   102,   103,    40,    41
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      71,    79,    80,   105,    42,   128,    43,    46,    84,   132,
      85,    47,    87,    88,    44,    -5,   123,    89,    90,    51,
      92,    52,    94,    95,    53,    54,   119,    55,    99,   100,
      56,    57,    58,    45,    59,    60,    61,    62,    63,   112,
      65,    64,    67,    66,    68,   105,    69,    70,    73,    72,
      74,    75,    81,    86,    91,    93,    96,    97,   101,    98,
     117,   106,   107,   110,   108,   122,     1,     2,   116,     3,
       4,     5,     6,     7,     8,     9,   109,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,   111,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,   118,   120,   121,
     126,    -5,   129,   130,   131,   133,   134,   135,    76
};

static const yytype_int8 yycheck[] =
{
      33,    46,    47,    81,     3,   123,    10,     3,    53,   127,
      55,     3,    57,    58,    18,     5,     6,    62,    63,     3,
      65,     3,    67,    68,     3,     3,   104,     3,    73,    74,
       5,     3,     3,    37,     3,     3,     3,     3,     3,   101,
       3,     5,     3,     5,     3,   123,     3,     5,     3,     5,
       3,     0,     6,     5,     5,     5,     3,     5,     3,     5,
     103,     5,     5,     3,     5,   110,     8,     9,   101,    11,
      12,    13,    14,    15,    16,    17,     5,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,     5,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,     7,     5,     5,
       5,     5,     3,     5,     7,     5,     5,     5,    40
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     8,     9,    11,    12,    13,    14,    15,    16,    17,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    51,
      68,    69,     3,    10,    18,    37,     3,     3,    52,    60,
      61,     3,     3,     3,     3,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     5,     3,     5,     3,     3,     3,
       5,    60,     5,     3,     3,     0,    69,    64,    65,    64,
      64,     6,    54,    63,    64,    64,     5,    64,    64,    64,
      64,     5,    64,     5,    64,    64,     3,     5,     5,    64,
      64,     3,    66,    67,    62,    63,     5,     5,     5,     5,
       3,     5,    52,    55,    56,    57,    60,    66,     7,    63,
       5,     5,    64,     6,    53,    59,     5,    58,    59,     3,
       5,     7,    59,     5,     5,     5
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    52,    53,    54,    55,    55,    56,    56,
      57,    58,    58,    59,    60,    60,    61,    62,    62,    63,
      64,    65,    66,    66,    67,    68,    68,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     0,     0,     1,     1,     4,     1,
       2,     2,     1,     2,     4,     1,     2,     2,     1,     2,
       2,     0,     2,     0,     2,     2,     1,     3,     2,     2,
       2,     3,     1,     1,     3,     2,     2,     2,     3,     1,
       2,     3,    10,     3,     3,     2,     2,     1,     2,     3,
       3,     1,     5,     3,     4,     3,     3,     5,     1,     1,
       3,     2,     4,     3,     3,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: pbrt_stmt_list  */
#line 181 "core/pbrtparse.yy"
{
}
#line 1339 "core/pbrtparse.cpp"
    break;

  case 3: /* array_init: %empty  */
#line 187 "core/pbrtparse.yy"
{
    if (cur_array) Severe("MUH");
    cur_array = new ParamArray;
}
#line 1348 "core/pbrtparse.cpp"
    break;

  case 4: /* string_array_init: %empty  */
#line 195 "core/pbrtparse.yy"
{
    cur_array->element_size = sizeof(const char *);
    cur_array->isString = true;
}
#line 1357 "core/pbrtparse.cpp"
    break;

  case 5: /* num_array_init: %empty  */
#line 203 "core/pbrtparse.yy"
{
    cur_array->element_size = sizeof(float);
    cur_array->isString = false;
}
#line 1366 "core/pbrtparse.cpp"
    break;

  case 6: /* array: string_array  */
#line 211 "core/pbrtparse.yy"
{
    (yyval.ribarray) = (yyvsp[0].ribarray);
}
#line 1374 "core/pbrtparse.cpp"
    break;

  case 7: /* array: num_array  */
#line 217 "core/pbrtparse.yy"
{
    (yyval.ribarray) = (yyvsp[0].ribarray);
}
#line 1382 "core/pbrtparse.cpp"
    break;

  case 8: /* string_array: array_init LBRACK string_list RBRACK  */
#line 224 "core/pbrtparse.yy"
{
    (yyval.ribarray) = cur_array;
    cur_array = NULL;
}
#line 1391 "core/pbrtparse.cpp"
    break;

  case 9: /* string_array: single_element_string_array  */
#line 231 "core/pbrtparse.yy"
{
    (yyval.ribarray) = cur_array;
    cur_array = NULL;
}
#line 1400 "core/pbrtparse.cpp"
    break;

  case 10: /* single_element_string_array: array_init string_list_entry  */
#line 239 "core/pbrtparse.yy"
{
}
#line 1407 "core/pbrtparse.cpp"
    break;

  case 11: /* string_list: string_list string_list_entry  */
#line 245 "core/pbrtparse.yy"
{
}
#line 1414 "core/pbrtparse.cpp"
    break;

  case 12: /* string_list: string_list_entry  */
#line 250 "core/pbrtparse.yy"
{
}
#line 1421 "core/pbrtparse.cpp"
    break;

  case 13: /* string_list_entry: string_array_init STRING  */
#line 256 "core/pbrtparse.yy"
{
    char *to_add = strdup((yyvsp[0].string));
    AddArrayElement(&to_add);
}
#line 1430 "core/pbrtparse.cpp"
    break;

  case 14: /* num_array: array_init LBRACK num_list RBRACK  */
#line 264 "core/pbrtparse.yy"
{
    (yyval.ribarray) = cur_array;
    cur_array = NULL;
}
#line 1439 "core/pbrtparse.cpp"
    break;

  case 15: /* num_array: single_element_num_array  */
#line 271 "core/pbrtparse.yy"
{
    (yyval.ribarray) = cur_array;
    cur_array = NULL;
}
#line 1448 "core/pbrtparse.cpp"
    break;

  case 16: /* single_element_num_array: array_init num_list_entry  */
#line 279 "core/pbrtparse.yy"
{
}
#line 1455 "core/pbrtparse.cpp"
    break;

  case 17: /* num_list: num_list num_list_entry  */
#line 285 "core/pbrtparse.yy"
{
}
#line 1462 "core/pbrtparse.cpp"
    break;

  case 18: /* num_list: num_list_entry  */
#line 290 "core/pbrtparse.yy"
{
}
#line 1469 "core/pbrtparse.cpp"
    break;

  case 19: /* num_list_entry: num_array_init NUM  */
#line 296 "core/pbrtparse.yy"
{
    float to_add = (yyvsp[0].num);
    AddArrayElement(&to_add);
}
#line 1478 "core/pbrtparse.cpp"
    break;

  case 20: /* paramlist: paramlist_init paramlist_contents  */
#line 304 "core/pbrtparse.yy"
{
}
#line 1485 "core/pbrtparse.cpp"
    break;

  case 21: /* paramlist_init: %empty  */
#line 310 "core/pbrtparse.yy"
{
    for (uint32_t i = 0; i < cur_paramlist.size(); ++i) {
        if (cur_paramlist[i].isString) {
            for (uint32_t j = 0; j < (uint32_t)cur_paramlist[i].size; ++j)
                free(((char **)cur_paramlist[i].arg)[j]);
        }
    }
    cur_paramlist.erase(cur_paramlist.begin(), cur_paramlist.end());
}
#line 1499 "core/pbrtparse.cpp"
    break;

  case 22: /* paramlist_contents: paramlist_entry paramlist_contents  */
#line 323 "core/pbrtparse.yy"
{
}
#line 1506 "core/pbrtparse.cpp"
    break;

  case 23: /* paramlist_contents: %empty  */
#line 328 "core/pbrtparse.yy"
{
}
#line 1513 "core/pbrtparse.cpp"
    break;

  case 24: /* paramlist_entry: STRING array  */
#line 334 "core/pbrtparse.yy"
{
    cur_paramlist.push_back(ParamListItem((yyvsp[-1].string), (yyvsp[0].ribarray)));
    ArrayFree((yyvsp[0].ribarray));
}
#line 1522 "core/pbrtparse.cpp"
    break;

  case 25: /* pbrt_stmt_list: pbrt_stmt_list pbrt_stmt  */
#line 342 "core/pbrtparse.yy"
{
}
#line 1529 "core/pbrtparse.cpp"
    break;

  case 26: /* pbrt_stmt_list: pbrt_stmt  */
#line 347 "core/pbrtparse.yy"
{
}
#line 1536 "core/pbrtparse.cpp"
    break;

  case 27: /* pbrt_stmt: ACCELERATOR STRING paramlist  */
#line 353 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtAccelerator((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1547 "core/pbrtparse.cpp"
    break;

  case 28: /* pbrt_stmt: ACTIVETRANSFORM ALL  */
#line 362 "core/pbrtparse.yy"
{
    pbrtActiveTransformAll();
}
#line 1555 "core/pbrtparse.cpp"
    break;

  case 29: /* pbrt_stmt: ACTIVETRANSFORM ENDTIME  */
#line 368 "core/pbrtparse.yy"
{
    pbrtActiveTransformEndTime();
}
#line 1563 "core/pbrtparse.cpp"
    break;

  case 30: /* pbrt_stmt: ACTIVETRANSFORM STARTTIME  */
#line 374 "core/pbrtparse.yy"
{
    pbrtActiveTransformStartTime();
}
#line 1571 "core/pbrtparse.cpp"
    break;

  case 31: /* pbrt_stmt: AREALIGHTSOURCE STRING paramlist  */
#line 380 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_ILLUMINANT);
    pbrtAreaLightSource((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1582 "core/pbrtparse.cpp"
    break;

  case 32: /* pbrt_stmt: ATTRIBUTEBEGIN  */
#line 389 "core/pbrtparse.yy"
{
    pbrtAttributeBegin();
}
#line 1590 "core/pbrtparse.cpp"
    break;

  case 33: /* pbrt_stmt: ATTRIBUTEEND  */
#line 395 "core/pbrtparse.yy"
{
    pbrtAttributeEnd();
}
#line 1598 "core/pbrtparse.cpp"
    break;

  case 34: /* pbrt_stmt: CAMERA STRING paramlist  */
#line 401 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtCamera((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1609 "core/pbrtparse.cpp"
    break;

  case 35: /* pbrt_stmt: CONCATTRANSFORM num_array  */
#line 410 "core/pbrtparse.yy"
{
    if (VerifyArrayLength((yyvsp[0].ribarray), 16, "ConcatTransform"))
        pbrtConcatTransform((float *) (yyvsp[0].ribarray)->array);
    ArrayFree((yyvsp[0].ribarray));
}
#line 1619 "core/pbrtparse.cpp"
    break;

  case 36: /* pbrt_stmt: COORDINATESYSTEM STRING  */
#line 418 "core/pbrtparse.yy"
{
    pbrtCoordinateSystem((yyvsp[0].string));
}
#line 1627 "core/pbrtparse.cpp"
    break;

  case 37: /* pbrt_stmt: COORDSYSTRANSFORM STRING  */
#line 424 "core/pbrtparse.yy"
{
    pbrtCoordSysTransform((yyvsp[0].string));
}
#line 1635 "core/pbrtparse.cpp"
    break;

  case 38: /* pbrt_stmt: FILM STRING paramlist  */
#line 430 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtFilm((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1646 "core/pbrtparse.cpp"
    break;

  case 39: /* pbrt_stmt: IDENTITY  */
#line 439 "core/pbrtparse.yy"
{
    pbrtIdentity();
}
#line 1654 "core/pbrtparse.cpp"
    break;

  case 40: /* pbrt_stmt: INCLUDE STRING  */
#line 445 "core/pbrtparse.yy"
{
  include_push((yyvsp[0].string));
}
#line 1662 "core/pbrtparse.cpp"
    break;

  case 41: /* pbrt_stmt: LIGHTSOURCE STRING paramlist  */
#line 451 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_ILLUMINANT);
    pbrtLightSource((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1673 "core/pbrtparse.cpp"
    break;

  case 42: /* pbrt_stmt: LOOKAT NUM NUM NUM NUM NUM NUM NUM NUM NUM  */
#line 460 "core/pbrtparse.yy"
{
    pbrtLookAt((yyvsp[-8].num), (yyvsp[-7].num), (yyvsp[-6].num), (yyvsp[-5].num), (yyvsp[-4].num), (yyvsp[-3].num), (yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
}
#line 1681 "core/pbrtparse.cpp"
    break;

  case 43: /* pbrt_stmt: MAKENAMEDMATERIAL STRING paramlist  */
#line 466 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtMakeNamedMaterial((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1692 "core/pbrtparse.cpp"
    break;

  case 44: /* pbrt_stmt: MATERIAL STRING paramlist  */
#line 475 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtMaterial((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1703 "core/pbrtparse.cpp"
    break;

  case 45: /* pbrt_stmt: NAMEDMATERIAL STRING  */
#line 484 "core/pbrtparse.yy"
{
    pbrtNamedMaterial((yyvsp[0].string));
}
#line 1711 "core/pbrtparse.cpp"
    break;

  case 46: /* pbrt_stmt: OBJECTBEGIN STRING  */
#line 490 "core/pbrtparse.yy"
{
    pbrtObjectBegin((yyvsp[0].string));
}
#line 1719 "core/pbrtparse.cpp"
    break;

  case 47: /* pbrt_stmt: OBJECTEND  */
#line 496 "core/pbrtparse.yy"
{
    pbrtObjectEnd();
}
#line 1727 "core/pbrtparse.cpp"
    break;

  case 48: /* pbrt_stmt: OBJECTINSTANCE STRING  */
#line 502 "core/pbrtparse.yy"
{
    pbrtObjectInstance((yyvsp[0].string));
}
#line 1735 "core/pbrtparse.cpp"
    break;

  case 49: /* pbrt_stmt: PIXELFILTER STRING paramlist  */
#line 508 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtPixelFilter((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1746 "core/pbrtparse.cpp"
    break;

  case 50: /* pbrt_stmt: RENDERER STRING paramlist  */
#line 517 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtRenderer((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1757 "core/pbrtparse.cpp"
    break;

  case 51: /* pbrt_stmt: REVERSEORIENTATION  */
#line 526 "core/pbrtparse.yy"
{
    pbrtReverseOrientation();
}
#line 1765 "core/pbrtparse.cpp"
    break;

  case 52: /* pbrt_stmt: ROTATE NUM NUM NUM NUM  */
#line 532 "core/pbrtparse.yy"
{
    pbrtRotate((yyvsp[-3].num), (yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
}
#line 1773 "core/pbrtparse.cpp"
    break;

  case 53: /* pbrt_stmt: SAMPLER STRING paramlist  */
#line 538 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtSampler((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1784 "core/pbrtparse.cpp"
    break;

  case 54: /* pbrt_stmt: SCALE NUM NUM NUM  */
#line 547 "core/pbrtparse.yy"
{
    pbrtScale((yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
}
#line 1792 "core/pbrtparse.cpp"
    break;

  case 55: /* pbrt_stmt: SHAPE STRING paramlist  */
#line 553 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtShape((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1803 "core/pbrtparse.cpp"
    break;

  case 56: /* pbrt_stmt: SURFACEINTEGRATOR STRING paramlist  */
#line 562 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtSurfaceIntegrator((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1814 "core/pbrtparse.cpp"
    break;

  case 57: /* pbrt_stmt: TEXTURE STRING STRING STRING paramlist  */
#line 571 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtTexture((yyvsp[-3].string), (yyvsp[-2].string), (yyvsp[-1].string), params);
    FreeArgs();
}
#line 1825 "core/pbrtparse.cpp"
    break;

  case 58: /* pbrt_stmt: TRANSFORMBEGIN  */
#line 580 "core/pbrtparse.yy"
{
    pbrtTransformBegin();
}
#line 1833 "core/pbrtparse.cpp"
    break;

  case 59: /* pbrt_stmt: TRANSFORMEND  */
#line 586 "core/pbrtparse.yy"
{
    pbrtTransformEnd();
}
#line 1841 "core/pbrtparse.cpp"
    break;

  case 60: /* pbrt_stmt: TRANSFORMTIMES NUM NUM  */
#line 592 "core/pbrtparse.yy"
{
    pbrtTransformTimes((yyvsp[-1].num), (yyvsp[0].num));
}
#line 1849 "core/pbrtparse.cpp"
    break;

  case 61: /* pbrt_stmt: TRANSFORM num_array  */
#line 598 "core/pbrtparse.yy"
{
    if (VerifyArrayLength( (yyvsp[0].ribarray), 16, "Transform" ))
        pbrtTransform( (float *) (yyvsp[0].ribarray)->array );
    ArrayFree((yyvsp[0].ribarray));
}
#line 1859 "core/pbrtparse.cpp"
    break;

  case 62: /* pbrt_stmt: TRANSLATE NUM NUM NUM  */
#line 606 "core/pbrtparse.yy"
{
    pbrtTranslate((yyvsp[-2].num), (yyvsp[-1].num), (yyvsp[0].num));
}
#line 1867 "core/pbrtparse.cpp"
    break;

  case 63: /* pbrt_stmt: VOLUMEINTEGRATOR STRING paramlist  */
#line 612 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtVolumeIntegrator((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1878 "core/pbrtparse.cpp"
    break;

  case 64: /* pbrt_stmt: VOLUME STRING paramlist  */
#line 621 "core/pbrtparse.yy"
{
    ParamSet params;
    InitParamSet(params, SPECTRUM_REFLECTANCE);
    pbrtVolume((yyvsp[-1].string), params);
    FreeArgs();
}
#line 1889 "core/pbrtparse.cpp"
    break;

  case 65: /* pbrt_stmt: WORLDBEGIN  */
#line 630 "core/pbrtparse.yy"
{
    pbrtWorldBegin();
}
#line 1897 "core/pbrtparse.cpp"
    break;

  case 66: /* pbrt_stmt: WORLDEND  */
#line 636 "core/pbrtparse.yy"
{
    pbrtWorldEnd();
}
#line 1905 "core/pbrtparse.cpp"
    break;


#line 1909 "core/pbrtparse.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 641 "core/pbrtparse.yy"

static const char *paramTypeToName(int type) {
    switch (type) {
    case PARAM_TYPE_INT: return "int";
    case PARAM_TYPE_BOOL: return "bool";
    case PARAM_TYPE_FLOAT: return "float";
    case PARAM_TYPE_POINT: return "point";
    case PARAM_TYPE_VECTOR: return "vector";
    case PARAM_TYPE_NORMAL: return "normal";
    case PARAM_TYPE_RGB: return "rgb/color";
    case PARAM_TYPE_XYZ: return "xyz";
    case PARAM_TYPE_BLACKBODY: return "blackbody";
    case PARAM_TYPE_SPECTRUM: return "spectrum";
    case PARAM_TYPE_STRING: return "string";
    case PARAM_TYPE_TEXTURE: return "texture";
    default: Severe("Error in paramTypeToName"); return NULL;
    }
}


static void InitParamSet(ParamSet &ps, SpectrumType type) {
    ps.Clear();
    for (uint32_t i = 0; i < cur_paramlist.size(); ++i) {
        int type;
        string name;
        if (lookupType(cur_paramlist[i].name, &type, name)) {
            if (type == PARAM_TYPE_TEXTURE || type == PARAM_TYPE_STRING ||
                type == PARAM_TYPE_BOOL) {
                if (!cur_paramlist[i].isString) {
                    Error("Expected string parameter value for parameter \"%s\" with type \"%s\". Ignoring.",
                          name.c_str(), paramTypeToName(type));
                    continue;
                }
            }
            else if (type != PARAM_TYPE_SPECTRUM) { /* spectrum can be either... */
                if (cur_paramlist[i].isString) {
                    Error("Expected numeric parameter value for parameter \"%s\" with type \"%s\".  Ignoring.",
                          name.c_str(), paramTypeToName(type));
                    continue;
                }
            }
            void *data = cur_paramlist[i].arg;
            int nItems = cur_paramlist[i].size;
            if (type == PARAM_TYPE_INT) {
                // parser doesn't handle ints, so convert from floats here....
                int nAlloc = nItems;
                int *idata = new int[nAlloc];
                float *fdata = (float *)cur_paramlist[i].arg;
                for (int j = 0; j < nAlloc; ++j)
                    idata[j] = int(fdata[j]);
                ps.AddInt(name, idata, nItems);
                delete[] idata;
            }
            else if (type == PARAM_TYPE_BOOL) {
                // strings -> bools
                int nAlloc = cur_paramlist[i].size;
                bool *bdata = new bool[nAlloc];
                for (int j = 0; j < nAlloc; ++j) {
                    string s(((const char **)data)[j]);
                    if (s == "true") bdata[j] = true;
                    else if (s == "false") bdata[j] = false;
                    else {
                        Warning("Value \"%s\" unknown for boolean parameter \"%s\"."
                            "Using \"false\".", s.c_str(), cur_paramlist[i].name);
                        bdata[j] = false;
                    }
                }
                ps.AddBool(name, bdata, nItems);
                delete[] bdata;
            }
            else if (type == PARAM_TYPE_FLOAT) {
                ps.AddFloat(name, (float *)data, nItems);
            } else if (type == PARAM_TYPE_POINT) {
                if ((nItems % 3) != 0)
                    Warning("Excess values given with point parameter \"%s\". "
                            "Ignoring last %d of them", cur_paramlist[i].name, nItems % 3);
                ps.AddPoint(name, (Point *)data, nItems / 3);
            } else if (type == PARAM_TYPE_VECTOR) {
                if ((nItems % 3) != 0)
                    Warning("Excess values given with vector parameter \"%s\". "
                            "Ignoring last %d of them", cur_paramlist[i].name, nItems % 3);
                ps.AddVector(name, (Vector *)data, nItems / 3);
            } else if (type == PARAM_TYPE_NORMAL) {
                if ((nItems % 3) != 0)
                    Warning("Excess values given with normal parameter \"%s\". "
                            "Ignoring last %d of them", cur_paramlist[i].name, nItems % 3);
                ps.AddNormal(name, (Normal *)data, nItems / 3);
            } else if (type == PARAM_TYPE_RGB) {
                if ((nItems % 3) != 0)
                    Warning("Excess RGB values given with parameter \"%s\". "
                            "Ignoring last %d of them", cur_paramlist[i].name, nItems % 3);
                ps.AddRGBSpectrum(name, (float *)data, nItems);
            } else if (type == PARAM_TYPE_XYZ) {
                if ((nItems % 3) != 0)
                    Warning("Excess XYZ values given with parameter \"%s\". "
                            "Ignoring last %d of them", cur_paramlist[i].name, nItems % 3);
                ps.AddXYZSpectrum(name, (float *)data, nItems);
            } else if (type == PARAM_TYPE_BLACKBODY) {
                if ((nItems % 2) != 0)
                    Warning("Excess value given with blackbody parameter \"%s\". "
                            "Ignoring extra one.", cur_paramlist[i].name);
                ps.AddBlackbodySpectrum(name, (float *)data, nItems);
            } else if (type == PARAM_TYPE_SPECTRUM) {
                if (cur_paramlist[i].isString) {
                    ps.AddSampledSpectrumFiles(name, (const char **)data, nItems);
                }
                else {
                    if ((nItems % 2) != 0)
                        Warning("Non-even number of values given with sampled spectrum "
                                "parameter \"%s\". Ignoring extra.", cur_paramlist[i].name);
                    ps.AddSampledSpectrum(name, (float *)data, nItems);
                }
            } else if (type == PARAM_TYPE_STRING) {
                string *strings = new string[nItems];
                for (int j = 0; j < nItems; ++j)
                    strings[j] = string(((const char **)data)[j]);
                ps.AddString(name, strings, nItems);
                delete[] strings;
            }
            else if (type == PARAM_TYPE_TEXTURE) {
                if (nItems == 1) {
                    string val(*((const char **)data));
                    ps.AddTexture(name, val);
                }
                else
                    Error("Only one string allowed for \"texture\" parameter \"%s\"",
                        name.c_str());
            }
        }
        else
            Warning("Type of parameter \"%s\" is unknown",
                cur_paramlist[i].name);
    }
}


static bool lookupType(const char *name, int *type, string &sname) {
    Assert(name != NULL);
    *type = 0;
    const char *strp = name;
    while (*strp && isspace(*strp))
        ++strp;
    if (!*strp) {
        Error("Parameter \"%s\" doesn't have a type declaration?!", name);
        return false;
    }
#define TRY_DECODING_TYPE(name, mask) \
        if (strncmp(name, strp, strlen(name)) == 0) { \
            *type = mask; strp += strlen(name); \
        }
         TRY_DECODING_TYPE("float",     PARAM_TYPE_FLOAT)
    else TRY_DECODING_TYPE("integer",   PARAM_TYPE_INT)
    else TRY_DECODING_TYPE("bool",      PARAM_TYPE_BOOL)
    else TRY_DECODING_TYPE("point",     PARAM_TYPE_POINT)
    else TRY_DECODING_TYPE("vector",    PARAM_TYPE_VECTOR)
    else TRY_DECODING_TYPE("normal",    PARAM_TYPE_NORMAL)
    else TRY_DECODING_TYPE("string",    PARAM_TYPE_STRING)
    else TRY_DECODING_TYPE("texture",   PARAM_TYPE_TEXTURE)
    else TRY_DECODING_TYPE("color",     PARAM_TYPE_RGB)
    else TRY_DECODING_TYPE("rgb",       PARAM_TYPE_RGB)
    else TRY_DECODING_TYPE("xyz",       PARAM_TYPE_XYZ)
    else TRY_DECODING_TYPE("blackbody", PARAM_TYPE_BLACKBODY)
    else TRY_DECODING_TYPE("spectrum",  PARAM_TYPE_SPECTRUM)
    else {
        Error("Unable to decode type for name \"%s\"", name);
        return false;
    }
    while (*strp && isspace(*strp))
        ++strp;
    sname = string(strp);
    return true;
}


