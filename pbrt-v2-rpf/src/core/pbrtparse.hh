/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_BUILD_LINUX_STATS_CORE_PBRTPARSE_HH_INCLUDED
# define YY_YY_BUILD_LINUX_STATS_CORE_PBRTPARSE_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    STRING = 258,                  /* STRING  */
    ID = 259,                      /* ID  */
    NUM = 260,                     /* NUM  */
    LBRACK = 261,                  /* LBRACK  */
    RBRACK = 262,                  /* RBRACK  */
    ACCELERATOR = 263,             /* ACCELERATOR  */
    ACTIVETRANSFORM = 264,         /* ACTIVETRANSFORM  */
    ALL = 265,                     /* ALL  */
    AREALIGHTSOURCE = 266,         /* AREALIGHTSOURCE  */
    ATTRIBUTEBEGIN = 267,          /* ATTRIBUTEBEGIN  */
    ATTRIBUTEEND = 268,            /* ATTRIBUTEEND  */
    CAMERA = 269,                  /* CAMERA  */
    CONCATTRANSFORM = 270,         /* CONCATTRANSFORM  */
    COORDINATESYSTEM = 271,        /* COORDINATESYSTEM  */
    COORDSYSTRANSFORM = 272,       /* COORDSYSTRANSFORM  */
    ENDTIME = 273,                 /* ENDTIME  */
    FILM = 274,                    /* FILM  */
    IDENTITY = 275,                /* IDENTITY  */
    INCLUDE = 276,                 /* INCLUDE  */
    LIGHTSOURCE = 277,             /* LIGHTSOURCE  */
    LOOKAT = 278,                  /* LOOKAT  */
    MAKENAMEDMATERIAL = 279,       /* MAKENAMEDMATERIAL  */
    MATERIAL = 280,                /* MATERIAL  */
    NAMEDMATERIAL = 281,           /* NAMEDMATERIAL  */
    OBJECTBEGIN = 282,             /* OBJECTBEGIN  */
    OBJECTEND = 283,               /* OBJECTEND  */
    OBJECTINSTANCE = 284,          /* OBJECTINSTANCE  */
    PIXELFILTER = 285,             /* PIXELFILTER  */
    RENDERER = 286,                /* RENDERER  */
    REVERSEORIENTATION = 287,      /* REVERSEORIENTATION  */
    ROTATE = 288,                  /* ROTATE  */
    SAMPLER = 289,                 /* SAMPLER  */
    SCALE = 290,                   /* SCALE  */
    SHAPE = 291,                   /* SHAPE  */
    STARTTIME = 292,               /* STARTTIME  */
    SURFACEINTEGRATOR = 293,       /* SURFACEINTEGRATOR  */
    TEXTURE = 294,                 /* TEXTURE  */
    TRANSFORMBEGIN = 295,          /* TRANSFORMBEGIN  */
    TRANSFORMEND = 296,            /* TRANSFORMEND  */
    TRANSFORMTIMES = 297,          /* TRANSFORMTIMES  */
    TRANSFORM = 298,               /* TRANSFORM  */
    TRANSLATE = 299,               /* TRANSLATE  */
    VOLUME = 300,                  /* VOLUME  */
    VOLUMEINTEGRATOR = 301,        /* VOLUMEINTEGRATOR  */
    WORLDBEGIN = 302,              /* WORLDBEGIN  */
    WORLDEND = 303,                /* WORLDEND  */
    HIGH_PRECEDENCE = 304          /* HIGH_PRECEDENCE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 157 "build/linux-stats/core/pbrtparse.yy"

char string[1024];
float num;
ParamArray *ribarray;

#line 119 "build/linux-stats/core/pbrtparse.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_BUILD_LINUX_STATS_CORE_PBRTPARSE_HH_INCLUDED  */
