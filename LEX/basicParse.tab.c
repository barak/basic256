
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 19 "basicParse.y"



#ifdef __cplusplus
  extern "C" {
#endif
    
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "../ByteCodes.h"

    #define SYMTABLESIZE 2000
    #define IFTABLESIZE 1000

    extern int yylex();
    extern char *yytext;
    int yyerror(const char *);
    int errorcode;
    extern int column;
    extern int linenumber;

    char *byteCode = NULL;
    unsigned int byteOffset = 0;
	unsigned int lastLineOffset = 0;	// store the byte offset for the end of the last line - use in loops
    unsigned int oldByteOffset = 0;
    unsigned int listlen = 0;

    struct label 
    {
      char *name;
      int offset;
    };

    char *EMPTYSTR = "";
    char *symtable[SYMTABLESIZE];
    int labeltable[SYMTABLESIZE];
    int numsyms = 0;
    int numlabels = 0;
    unsigned int maxbyteoffset = 0;
	
	// array to hold stack of if statement branch locations
	// that need to have final jump location added to them
	unsigned int iftable[IFTABLESIZE];
	unsigned int numifs = 0;

    int
    basicParse(char *);

	void clearIfTable()
	{
		int j;
		for (j = 0; j < IFTABLESIZE; j++)
		{
			iftable[j] = -1;
		}
		numifs = 0;
    }

    void 
    clearLabelTable()
    {
      int j;
      for (j = 0; j < SYMTABLESIZE; j++)
	{
	  labeltable[j] = -1;
	}
	numlabels = 0;
    }

    void
    clearSymbolTable()
    {
      int j;
      if (numsyms == 0)
	{
	  for (j = 0; j < SYMTABLESIZE; j++)
	    {
	      symtable[j] = 0;
	    }
	}
      for (j = 0; j < numsyms; j++)
	{
	  if (symtable[j])
	    {
	      free(symtable[j]);
	    }
	  symtable[j] = 0;
	}
      numsyms = 0;
    }

    int 
    getSymbol(char *name)
    {
      int i;
      for (i = 0; i < numsyms; i++)
	{
	  if (symtable[i] && !strcmp(name, symtable[i]))
	    return i;
	}
      return -1;
    }

    int
    newSymbol(char *name) 
    {
      symtable[numsyms] = name;
      numsyms++;
      return numsyms - 1;
    }


    int
    newByteCode(unsigned int size) 
    {
      if (byteCode)
	{
	  free(byteCode);
	}
      maxbyteoffset = 1024;
      byteCode = malloc(maxbyteoffset);

      if (byteCode)
	{
	  memset(byteCode, 0, maxbyteoffset);
	  byteOffset = 0;
	  return 0;
	}
      
      return -1;
    }
    
    void 
    checkByteMem(unsigned int addedbytes)
    {
      if (byteOffset + addedbytes + 1 >= maxbyteoffset)
	{
	  maxbyteoffset += maxbyteoffset + addedbytes + 32;
	  byteCode = realloc(byteCode, maxbyteoffset);
	  memset(byteCode + byteOffset, 0, maxbyteoffset - byteOffset);
	}
    }

    void
    addOp(char op)
    {
      checkByteMem(sizeof(char));
      byteCode[byteOffset] = op;
      byteOffset++;
    }

    void
    addExtendedOp(char extgroup, char extop)
    {
		addOp(extgroup);
		addOp(extop);
    }
	
	
	unsigned int addInt(int data) {
	  // add an integer to the bytecode at the current location
	  // return starting location of the integer - so we can write to it later
	  int *temp;
      unsigned int holdOffset = byteOffset;
	  checkByteMem(sizeof(int));
      temp = (int *) (byteCode + byteOffset);
      byteOffset += sizeof(int);
	  return holdOffset;
	}
	
    void 
    addIntOp(char op, int data)
    {
      int *temp = NULL;
      checkByteMem(sizeof(char) + sizeof(int));
      byteCode[byteOffset] = op;
      byteOffset++;
      
      temp = (int *) (byteCode + byteOffset);
      *temp = data;
      byteOffset += sizeof(int);
    }

    void 
    addInt2Op(char op, int data1, int data2)
    {
      int *temp = NULL;
      checkByteMem(sizeof(char) + 2 * sizeof(int));
      byteCode[byteOffset] = op;
      byteOffset++;
      
      temp = (int *) (byteCode + byteOffset);
      temp[0] = data1;
      temp[1] = data2;
      byteOffset += 2 * sizeof(int);
    }

    void 
    addFloatOp(char op, double data)
    {
      double *temp = NULL;
      checkByteMem(sizeof(char) + sizeof(double));
      byteCode[byteOffset] = op;
      byteOffset++;
      
      temp = (double *) (byteCode + byteOffset);
      *temp = data;
      byteOffset += sizeof(double);
    }

    void 
    addStringOp(char op, char *data)
    {
      double *temp = NULL;
      int len = strlen(data) + 1;
      checkByteMem(sizeof(char) + len);
      byteCode[byteOffset] = op;
      byteOffset++;
      
      temp = (double *) (byteCode + byteOffset);
      strncpy((char *) byteCode + byteOffset, data, len);
      byteOffset += len;
    }


#ifdef __cplusplus
  }
#endif



/* Line 189 of yacc.c  */
#line 306 "basicParse.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PRINT = 258,
     INPUT = 259,
     KEY = 260,
     PIXEL = 261,
     RGB = 262,
     PLOT = 263,
     CIRCLE = 264,
     RECT = 265,
     POLY = 266,
     STAMP = 267,
     LINE = 268,
     FASTGRAPHICS = 269,
     GRAPHSIZE = 270,
     REFRESH = 271,
     CLS = 272,
     CLG = 273,
     IF = 274,
     THEN = 275,
     ELSE = 276,
     ENDIF = 277,
     WHILE = 278,
     ENDWHILE = 279,
     DO = 280,
     UNTIL = 281,
     FOR = 282,
     TO = 283,
     STEP = 284,
     NEXT = 285,
     OPEN = 286,
     READ = 287,
     WRITE = 288,
     CLOSE = 289,
     RESET = 290,
     GOTO = 291,
     GOSUB = 292,
     RETURN = 293,
     REM = 294,
     END = 295,
     SETCOLOR = 296,
     GTE = 297,
     LTE = 298,
     NE = 299,
     DIM = 300,
     REDIM = 301,
     NOP = 302,
     TOINT = 303,
     TOSTRING = 304,
     LENGTH = 305,
     MID = 306,
     LEFT = 307,
     RIGHT = 308,
     UPPER = 309,
     LOWER = 310,
     INSTR = 311,
     CEIL = 312,
     FLOOR = 313,
     RAND = 314,
     SIN = 315,
     COS = 316,
     TAN = 317,
     ASIN = 318,
     ACOS = 319,
     ATAN = 320,
     ABS = 321,
     PI = 322,
     DEGREES = 323,
     RADIANS = 324,
     LOG = 325,
     LOGTEN = 326,
     AND = 327,
     OR = 328,
     XOR = 329,
     NOT = 330,
     PAUSE = 331,
     SOUND = 332,
     ASC = 333,
     CHR = 334,
     TOFLOAT = 335,
     READLINE = 336,
     WRITELINE = 337,
     BOOLEOF = 338,
     MOD = 339,
     INTDIV = 340,
     YEAR = 341,
     MONTH = 342,
     DAY = 343,
     HOUR = 344,
     MINUTE = 345,
     SECOND = 346,
     TEXT = 347,
     FONT = 348,
     SAY = 349,
     SYSTEM = 350,
     VOLUME = 351,
     GRAPHWIDTH = 352,
     GRAPHHEIGHT = 353,
     GETSLICE = 354,
     PUTSLICE = 355,
     IMGLOAD = 356,
     SPRITEDIM = 357,
     SPRITELOAD = 358,
     SPRITESLICE = 359,
     SPRITEMOVE = 360,
     SPRITEHIDE = 361,
     SPRITESHOW = 362,
     SPRITEPLACE = 363,
     SPRITECOLLIDE = 364,
     SPRITEX = 365,
     SPRITEY = 366,
     SPRITEH = 367,
     SPRITEW = 368,
     SPRITEV = 369,
     WAVPLAY = 370,
     WAVSTOP = 371,
     WAVWAIT = 372,
     SIZE = 373,
     SEEK = 374,
     EXISTS = 375,
     BOOLTRUE = 376,
     BOOLFALSE = 377,
     MOUSEX = 378,
     MOUSEY = 379,
     MOUSEB = 380,
     CLICKCLEAR = 381,
     CLICKX = 382,
     CLICKY = 383,
     CLICKB = 384,
     GETCOLOR = 385,
     CLEAR = 386,
     BLACK = 387,
     WHITE = 388,
     RED = 389,
     DARKRED = 390,
     GREEN = 391,
     DARKGREEN = 392,
     BLUE = 393,
     DARKBLUE = 394,
     CYAN = 395,
     DARKCYAN = 396,
     PURPLE = 397,
     DARKPURPLE = 398,
     YELLOW = 399,
     DARKYELLOW = 400,
     ORANGE = 401,
     DARKORANGE = 402,
     GREY = 403,
     DARKGREY = 404,
     CHANGEDIR = 405,
     CURRENTDIR = 406,
     DECIMAL = 407,
     DBOPEN = 408,
     DBCLOSE = 409,
     DBEXECUTE = 410,
     DBOPENSET = 411,
     DBCLOSESET = 412,
     DBROW = 413,
     DBINT = 414,
     DBFLOAT = 415,
     DBSTRING = 416,
     ONERROR = 417,
     OFFERROR = 418,
     LASTERROR = 419,
     LASTERRORMESSAGE = 420,
     LASTERRORLINE = 421,
     LASTERROREXTRA = 422,
     LINENUM = 423,
     INTEGER = 424,
     FLOAT = 425,
     STRING = 426,
     VARIABLE = 427,
     STRINGVAR = 428,
     NEWVAR = 429,
     COLOR = 430,
     LABEL = 431,
     UMINUS = 432
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 283 "basicParse.y"

  int number;
  double floatnum;
  char *string;



/* Line 214 of yacc.c  */
#line 527 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 539 "basicParse.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  346
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   14011

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  197
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  85
/* YYNRULES -- Number of rules.  */
#define YYNRULES  436
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1025

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   432

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     186,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     188,   190,   182,   181,   189,   180,     2,   183,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   187,   193,
     177,   179,   178,   196,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   191,     2,   192,   185,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   194,     2,   195,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   184
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,    10,    13,    15,    17,    19,    21,
      23,    25,    27,    29,    31,    33,    35,    36,    40,    43,
      45,    47,    50,    52,    55,    57,    60,    62,    64,    67,
      69,    73,    75,    77,    79,    81,    83,    85,    87,    89,
      91,    93,    95,    97,    99,   101,   103,   105,   107,   109,
     111,   113,   115,   117,   119,   121,   123,   125,   127,   129,
     131,   133,   135,   137,   139,   141,   143,   145,   147,   149,
     151,   153,   155,   157,   159,   161,   163,   165,   167,   169,
     171,   173,   175,   177,   179,   181,   183,   185,   187,   189,
     191,   193,   197,   201,   209,   217,   221,   225,   233,   241,
     244,   246,   248,   250,   255,   262,   264,   266,   269,   276,
     285,   289,   296,   305,   309,   313,   317,   324,   333,   336,
     339,   342,   344,   347,   349,   356,   365,   368,   373,   376,
     381,   384,   391,   396,   401,   408,   417,   428,   435,   444,
     453,   464,   471,   480,   487,   496,   503,   512,   515,   520,
     523,   526,   531,   534,   537,   542,   545,   554,   565,   574,
     581,   590,   597,   608,   621,   632,   637,   640,   647,   652,
     657,   660,   667,   672,   677,   680,   687,   692,   694,   698,
     701,   703,   707,   710,   713,   720,   725,   729,   736,   740,
     747,   750,   756,   764,   767,   773,   781,   784,   789,   791,
     794,   799,   802,   806,   812,   816,   819,   824,   826,   830,
     832,   836,   843,   852,   861,   872,   879,   888,   897,   908,
     919,   932,   935,   940,   947,   958,   971,   978,   987,   994,
    1003,  1006,  1009,  1011,  1015,  1018,  1023,  1026,  1029,  1034,
    1036,  1040,  1043,  1048,  1051,  1056,  1058,  1062,  1066,  1070,
    1072,  1076,  1080,  1084,  1088,  1092,  1096,  1100,  1104,  1108,
    1111,  1115,  1119,  1123,  1126,  1130,  1134,  1138,  1142,  1146,
    1150,  1154,  1158,  1162,  1166,  1170,  1174,  1176,  1178,  1183,
    1188,  1194,  1200,  1206,  1212,  1217,  1224,  1226,  1231,  1236,
    1241,  1246,  1251,  1256,  1263,  1268,  1273,  1278,  1283,  1288,
    1293,  1298,  1303,  1308,  1313,  1318,  1323,  1328,  1330,  1334,
    1336,  1340,  1342,  1346,  1348,  1352,  1354,  1358,  1363,  1368,
    1370,  1374,  1376,  1380,  1382,  1386,  1388,  1392,  1394,  1398,
    1400,  1404,  1406,  1410,  1412,  1416,  1418,  1422,  1427,  1429,
    1433,  1435,  1439,  1441,  1445,  1447,  1451,  1453,  1457,  1459,
    1463,  1465,  1469,  1471,  1475,  1477,  1481,  1483,  1487,  1489,
    1493,  1495,  1499,  1501,  1505,  1507,  1511,  1513,  1517,  1519,
    1523,  1525,  1529,  1531,  1535,  1537,  1541,  1543,  1547,  1549,
    1553,  1555,  1559,  1561,  1565,  1567,  1571,  1573,  1577,  1579,
    1583,  1590,  1599,  1601,  1605,  1612,  1617,  1622,  1627,  1632,
    1637,  1641,  1646,  1651,  1653,  1657,  1659,  1663,  1665,  1669,
    1673,  1677,  1681,  1683,  1688,  1695,  1697,  1702,  1707,  1712,
    1717,  1726,  1733,  1740,  1751,  1753,  1757,  1762,  1764,  1768,
    1773,  1775,  1779,  1784,  1786,  1790,  1792
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     198,     0,    -1,   199,   186,    -1,   199,   186,   198,    -1,
     200,   201,    -1,   201,    -1,   176,    -1,   202,    -1,   203,
      -1,   204,    -1,   206,    -1,   207,    -1,   209,    -1,   210,
      -1,   211,    -1,   212,    -1,    -1,   222,    20,   212,    -1,
     222,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     205,    -1,    23,   279,    -1,    24,    -1,    40,    23,    -1,
     208,    -1,    25,    -1,    26,   279,    -1,   213,    -1,   212,
     187,   213,    -1,   229,    -1,   230,    -1,   231,    -1,   232,
      -1,   233,    -1,   255,    -1,   236,    -1,   238,    -1,   239,
      -1,   245,    -1,   246,    -1,   237,    -1,   225,    -1,   226,
      -1,   227,    -1,   228,    -1,   234,    -1,   253,    -1,   221,
      -1,   217,    -1,   220,    -1,   218,    -1,   219,    -1,   214,
      -1,   215,    -1,   216,    -1,   224,    -1,   223,    -1,   247,
      -1,   248,    -1,   249,    -1,   250,    -1,   251,    -1,   235,
      -1,   240,    -1,   241,    -1,   242,    -1,   243,    -1,   244,
      -1,   256,    -1,   257,    -1,   258,    -1,   259,    -1,   260,
      -1,   261,    -1,   262,    -1,   263,    -1,   264,    -1,   265,
      -1,   266,    -1,   267,    -1,   252,    -1,   268,    -1,   269,
      -1,   270,    -1,   271,    -1,   272,    -1,   273,    -1,   274,
      -1,   275,    -1,    45,   172,   279,    -1,    45,   173,   279,
      -1,    45,   172,   188,   279,   189,   279,   190,    -1,    45,
     173,   188,   279,   189,   279,   190,    -1,    46,   172,   279,
      -1,    46,   173,   279,    -1,    46,   172,   188,   279,   189,
     279,   190,    -1,    46,   173,   188,   279,   189,   279,   190,
      -1,    76,   279,    -1,    17,    -1,    18,    -1,    14,    -1,
      15,   279,   189,   279,    -1,    15,   188,   279,   189,   279,
     190,    -1,    16,    -1,    40,    -1,    19,   279,    -1,   173,
     191,   279,   192,   179,   281,    -1,   173,   191,   279,   189,
     279,   192,   179,   281,    -1,   173,   179,   276,    -1,   172,
     191,   279,   192,   179,   279,    -1,   172,   191,   279,   189,
     279,   192,   179,   279,    -1,   172,   179,   277,    -1,   172,
     179,   279,    -1,   173,   179,   281,    -1,    27,   172,   179,
     279,    28,   279,    -1,    27,   172,   179,   279,    28,   279,
      29,   279,    -1,    30,   172,    -1,    36,   172,    -1,    37,
     172,    -1,   163,    -1,   162,   172,    -1,    38,    -1,    41,
     279,   189,   279,   189,   279,    -1,    41,   188,   279,   189,
     279,   189,   279,   190,    -1,    41,   279,    -1,    41,   188,
     279,   190,    -1,    77,   172,    -1,    77,   188,   172,   190,
      -1,    77,   277,    -1,    77,   188,   279,   189,   279,   190,
      -1,    77,   279,   189,   279,    -1,     8,   279,   189,   279,
      -1,     8,   188,   279,   189,   279,   190,    -1,    13,   279,
     189,   279,   189,   279,   189,   279,    -1,    13,   188,   279,
     189,   279,   189,   279,   189,   279,   190,    -1,     9,   279,
     189,   279,   189,   279,    -1,     9,   188,   279,   189,   279,
     189,   279,   190,    -1,    10,   279,   189,   279,   189,   279,
     189,   279,    -1,    10,   188,   279,   189,   279,   189,   279,
     189,   279,   190,    -1,    92,   279,   189,   279,   189,   281,
      -1,    92,   188,   279,   189,   279,   189,   281,   190,    -1,
      92,   279,   189,   279,   189,   279,    -1,    92,   188,   279,
     189,   279,   189,   279,   190,    -1,    93,   281,   189,   279,
     189,   279,    -1,    93,   188,   281,   189,   279,   189,   279,
     190,    -1,    94,   281,    -1,    94,   188,   281,   190,    -1,
      94,   279,    -1,    95,   281,    -1,    95,   188,   281,   190,
      -1,    96,   279,    -1,    11,   172,    -1,    11,   188,   172,
     190,    -1,    11,   277,    -1,    12,   279,   189,   279,   189,
     279,   189,   172,    -1,    12,   188,   279,   189,   279,   189,
     279,   189,   172,   190,    -1,    12,   279,   189,   279,   189,
     279,   189,   277,    -1,    12,   279,   189,   279,   189,   172,
      -1,    12,   188,   279,   189,   279,   189,   172,   190,    -1,
      12,   279,   189,   279,   189,   277,    -1,    12,   279,   189,
     279,   189,   279,   189,   279,   189,   172,    -1,    12,   188,
     279,   189,   279,   189,   279,   189,   279,   189,   172,   190,
      -1,    12,   279,   189,   279,   189,   279,   189,   279,   189,
     277,    -1,    31,   188,   281,   190,    -1,    31,   281,    -1,
      31,   188,   279,   189,   281,   190,    -1,    31,   279,   189,
     281,    -1,    33,   188,   281,   190,    -1,    33,   281,    -1,
      33,   188,   279,   189,   281,   190,    -1,    33,   279,   189,
     281,    -1,    82,   188,   281,   190,    -1,    82,   281,    -1,
      82,   188,   279,   189,   281,   190,    -1,    82,   279,   189,
     281,    -1,    34,    -1,    34,   188,   190,    -1,    34,   279,
      -1,    35,    -1,    35,   188,   190,    -1,    35,   279,    -1,
     119,   279,    -1,   119,   188,   279,   189,   279,   190,    -1,
     119,   279,   189,   279,    -1,   254,   189,   173,    -1,   254,
     189,   173,   191,   279,   192,    -1,   254,   189,   172,    -1,
     254,   189,   172,   191,   279,   192,    -1,     4,   173,    -1,
       4,   173,   191,   279,   192,    -1,     4,   173,   191,   279,
     189,   279,   192,    -1,     4,   172,    -1,     4,   172,   191,
     279,   192,    -1,     4,   172,   191,   279,   189,   279,   192,
      -1,     4,   281,    -1,     4,   188,   281,   190,    -1,     3,
      -1,     3,   281,    -1,     3,   188,   281,   190,    -1,     3,
     279,    -1,     3,   281,   193,    -1,     3,   188,   281,   190,
     193,    -1,     3,   279,   193,    -1,   115,   281,    -1,   115,
     188,   281,   190,    -1,   116,    -1,   116,   188,   190,    -1,
     117,    -1,   117,   188,   190,    -1,   100,   279,   189,   279,
     189,   281,    -1,   100,   188,   279,   189,   279,   189,   281,
     190,    -1,   100,   279,   189,   279,   189,   281,   189,   279,
      -1,   100,   188,   279,   189,   279,   189,   281,   189,   279,
     190,    -1,   101,   279,   189,   279,   189,   281,    -1,   101,
     188,   279,   189,   279,   189,   281,   190,    -1,   101,   279,
     189,   279,   189,   279,   189,   281,    -1,   101,   188,   279,
     189,   279,   189,   279,   189,   281,   190,    -1,   101,   279,
     189,   279,   189,   279,   189,   279,   189,   281,    -1,   101,
     188,   279,   189,   279,   189,   279,   189,   279,   189,   281,
     190,    -1,   102,   279,    -1,   103,   279,   189,   281,    -1,
     103,   188,   279,   189,   281,   190,    -1,   104,   279,   189,
     279,   189,   279,   189,   279,   189,   279,    -1,   104,   188,
     279,   189,   279,   189,   279,   189,   279,   189,   279,   190,
      -1,   108,   279,   189,   279,   189,   279,    -1,   108,   188,
     279,   189,   279,   189,   279,   190,    -1,   105,   279,   189,
     279,   189,   279,    -1,   103,   188,   279,   189,   279,   189,
     279,   190,    -1,   106,   279,    -1,   107,   279,    -1,   126,
      -1,   126,   188,   190,    -1,   150,   281,    -1,   150,   188,
     281,   190,    -1,   152,   279,    -1,   153,   281,    -1,   153,
     188,   281,   190,    -1,   154,    -1,   154,   188,   190,    -1,
     155,   281,    -1,   155,   188,   281,   190,    -1,   156,   281,
      -1,   156,   188,   281,   190,    -1,   157,    -1,   157,   188,
     190,    -1,   194,   280,   195,    -1,   194,   278,   195,    -1,
     279,    -1,   279,   189,   278,    -1,   188,   279,   190,    -1,
     279,   181,   279,    -1,   279,   180,   279,    -1,   279,   182,
     279,    -1,   279,    84,   279,    -1,   279,    85,   279,    -1,
     279,   183,   279,    -1,   279,   185,   279,    -1,   180,   279,
      -1,   279,    72,   279,    -1,   279,    73,   279,    -1,   279,
      74,   279,    -1,    75,   279,    -1,   281,   179,   281,    -1,
     281,    44,   281,    -1,   281,   177,   281,    -1,   281,   178,
     281,    -1,   281,    42,   281,    -1,   281,    43,   281,    -1,
     279,   179,   279,    -1,   279,    44,   279,    -1,   279,   177,
     279,    -1,   279,   178,   279,    -1,   279,    42,   279,    -1,
     279,    43,   279,    -1,   170,    -1,   169,    -1,   172,   191,
     196,   192,    -1,   173,   191,   196,   192,    -1,   172,   191,
     196,   189,   192,    -1,   173,   191,   196,   189,   192,    -1,
     172,   191,   189,   196,   192,    -1,   173,   191,   189,   196,
     192,    -1,   172,   191,   279,   192,    -1,   172,   191,   279,
     189,   279,   192,    -1,   172,    -1,    48,   188,   279,   190,
      -1,    48,   188,   281,   190,    -1,    80,   188,   279,   190,
      -1,    80,   188,   281,   190,    -1,    50,   188,   281,   190,
      -1,    78,   188,   281,   190,    -1,    56,   188,   281,   189,
     281,   190,    -1,    57,   188,   279,   190,    -1,    58,   188,
     279,   190,    -1,    60,   188,   279,   190,    -1,    61,   188,
     279,   190,    -1,    62,   188,   279,   190,    -1,    63,   188,
     279,   190,    -1,    64,   188,   279,   190,    -1,    65,   188,
     279,   190,    -1,    68,   188,   279,   190,    -1,    69,   188,
     279,   190,    -1,    70,   188,   279,   190,    -1,    71,   188,
     279,   190,    -1,    66,   188,   279,   190,    -1,    59,    -1,
      59,   188,   190,    -1,    67,    -1,    67,   188,   190,    -1,
     121,    -1,   121,   188,   190,    -1,   122,    -1,   122,   188,
     190,    -1,    83,    -1,    83,   188,   190,    -1,    83,   188,
     279,   190,    -1,   120,   188,   281,   190,    -1,    86,    -1,
      86,   188,   190,    -1,    87,    -1,    87,   188,   190,    -1,
      88,    -1,    88,   188,   190,    -1,    89,    -1,    89,   188,
     190,    -1,    90,    -1,    90,   188,   190,    -1,    91,    -1,
      91,   188,   190,    -1,    97,    -1,    97,   188,   190,    -1,
      98,    -1,    98,   188,   190,    -1,   118,    -1,   118,   188,
     190,    -1,   118,   188,   279,   190,    -1,     5,    -1,     5,
     188,   190,    -1,   123,    -1,   123,   188,   190,    -1,   124,
      -1,   124,   188,   190,    -1,   125,    -1,   125,   188,   190,
      -1,   127,    -1,   127,   188,   190,    -1,   128,    -1,   128,
     188,   190,    -1,   129,    -1,   129,   188,   190,    -1,   131,
      -1,   131,   188,   190,    -1,   132,    -1,   132,   188,   190,
      -1,   133,    -1,   133,   188,   190,    -1,   134,    -1,   134,
     188,   190,    -1,   135,    -1,   135,   188,   190,    -1,   136,
      -1,   136,   188,   190,    -1,   137,    -1,   137,   188,   190,
      -1,   138,    -1,   138,   188,   190,    -1,   139,    -1,   139,
     188,   190,    -1,   140,    -1,   140,   188,   190,    -1,   141,
      -1,   141,   188,   190,    -1,   142,    -1,   142,   188,   190,
      -1,   143,    -1,   143,   188,   190,    -1,   144,    -1,   144,
     188,   190,    -1,   145,    -1,   145,   188,   190,    -1,   146,
      -1,   146,   188,   190,    -1,   147,    -1,   147,   188,   190,
      -1,   148,    -1,   148,   188,   190,    -1,   149,    -1,   149,
     188,   190,    -1,     6,   188,   279,   189,   279,   190,    -1,
       7,   188,   279,   189,   279,   189,   279,   190,    -1,   130,
      -1,   130,   188,   190,    -1,   109,   188,   279,   189,   279,
     190,    -1,   110,   188,   279,   190,    -1,   111,   188,   279,
     190,    -1,   112,   188,   279,   190,    -1,   113,   188,   279,
     190,    -1,   114,   188,   279,   190,    -1,   158,   188,   190,
      -1,   159,   188,   279,   190,    -1,   160,   188,   279,   190,
      -1,   164,    -1,   164,   188,   190,    -1,   166,    -1,   166,
     188,   190,    -1,   281,    -1,   281,   189,   280,    -1,   281,
     181,   281,    -1,   279,   181,   281,    -1,   281,   181,   279,
      -1,   171,    -1,   173,   191,   279,   192,    -1,   173,   191,
     279,   189,   279,   192,    -1,   173,    -1,    79,   188,   279,
     190,    -1,    49,   188,   279,   190,    -1,    54,   188,   281,
     190,    -1,    55,   188,   281,   190,    -1,    51,   188,   281,
     189,   279,   189,   279,   190,    -1,    52,   188,   281,   189,
     279,   190,    -1,    53,   188,   281,   189,   279,   190,    -1,
      99,   188,   279,   189,   279,   189,   279,   189,   279,   190,
      -1,    32,    -1,    32,   188,   190,    -1,    32,   188,   279,
     190,    -1,    81,    -1,    81,   188,   190,    -1,    81,   188,
     279,   190,    -1,   151,    -1,   151,   188,   190,    -1,   161,
     188,   279,   190,    -1,   165,    -1,   165,   188,   190,    -1,
     167,    -1,   167,   188,   190,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   316,   316,   317,   320,   321,   324,   326,   327,   328,
     329,   330,   337,   338,   345,   346,   347,   350,   365,   371,
     392,   393,   395,   410,   423,   424,   426,   444,   451,   462,
     462,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   527,   528,   529,   530,   533,   534,   535,   536,   539,
     542,   543,   546,   549,   550,   553,   556,   559,   573,   574,
     575,   578,   579,   580,   584,   587,   590,   595,   602,   605,
     608,   612,   616,   619,   622,   623,   624,   625,   628,   629,
     630,   631,   632,   635,   636,   639,   640,   644,   645,   648,
     649,   652,   653,   654,   655,   658,   659,   662,   663,   664,
     667,   668,   671,   674,   675,   676,   679,   680,   681,   682,
     683,   684,   685,   686,   687,   690,   691,   692,   693,   696,
     697,   698,   699,   702,   703,   704,   705,   708,   709,   710,
     713,   714,   715,   718,   719,   720,   723,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   735,   736,   739,   740,
     741,   742,   743,   744,   745,   748,   749,   752,   753,   756,
     757,   760,   761,   762,   763,   765,   766,   767,   768,   769,
     770,   773,   776,   777,   780,   781,   784,   785,   788,   789,
     792,   795,   798,   799,   802,   803,   806,   809,   810,   813,
     814,   817,   818,   821,   822,   825,   826,   829,   832,   835,
     836,   839,   840,   841,   842,   843,   844,   845,   846,   847,
     848,   849,   850,   851,   852,   853,   854,   855,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,   866,   867,
     868,   869,   870,   871,   872,   873,   874,   885,   886,   887,
     888,   889,   890,   891,   892,   893,   894,   895,   896,   897,
     898,   899,   900,   901,   902,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   912,   913,   914,   915,   916,   917,
     918,   919,   920,   921,   922,   923,   924,   925,   926,   927,
     928,   929,   930,   931,   932,   933,   934,   935,   936,   937,
     938,   939,   940,   941,   942,   943,   944,   945,   946,   947,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,   961,   962,   963,   964,   965,   966,   967,
     968,   969,   970,   971,   972,   973,   974,   975,   976,   977,
     978,   979,   980,   981,   982,   983,   984,   985,   986,   987,
     988,   989,   990,   991,   992,   993,   994,   995,   996,   997,
     998,   999,  1000,  1001,  1002,  1003,  1004,  1007,  1008,  1011,
    1012,  1013,  1014,  1015,  1016,  1017,  1028,  1029,  1030,  1031,
    1032,  1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,  1041,
    1042,  1043,  1044,  1045,  1046,  1047,  1048
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PRINT", "INPUT", "KEY", "PIXEL", "RGB",
  "PLOT", "CIRCLE", "RECT", "POLY", "STAMP", "LINE", "FASTGRAPHICS",
  "GRAPHSIZE", "REFRESH", "CLS", "CLG", "IF", "THEN", "ELSE", "ENDIF",
  "WHILE", "ENDWHILE", "DO", "UNTIL", "FOR", "TO", "STEP", "NEXT", "OPEN",
  "READ", "WRITE", "CLOSE", "RESET", "GOTO", "GOSUB", "RETURN", "REM",
  "END", "SETCOLOR", "GTE", "LTE", "NE", "DIM", "REDIM", "NOP", "TOINT",
  "TOSTRING", "LENGTH", "MID", "LEFT", "RIGHT", "UPPER", "LOWER", "INSTR",
  "CEIL", "FLOOR", "RAND", "SIN", "COS", "TAN", "ASIN", "ACOS", "ATAN",
  "ABS", "PI", "DEGREES", "RADIANS", "LOG", "LOGTEN", "AND", "OR", "XOR",
  "NOT", "PAUSE", "SOUND", "ASC", "CHR", "TOFLOAT", "READLINE",
  "WRITELINE", "BOOLEOF", "MOD", "INTDIV", "YEAR", "MONTH", "DAY", "HOUR",
  "MINUTE", "SECOND", "TEXT", "FONT", "SAY", "SYSTEM", "VOLUME",
  "GRAPHWIDTH", "GRAPHHEIGHT", "GETSLICE", "PUTSLICE", "IMGLOAD",
  "SPRITEDIM", "SPRITELOAD", "SPRITESLICE", "SPRITEMOVE", "SPRITEHIDE",
  "SPRITESHOW", "SPRITEPLACE", "SPRITECOLLIDE", "SPRITEX", "SPRITEY",
  "SPRITEH", "SPRITEW", "SPRITEV", "WAVPLAY", "WAVSTOP", "WAVWAIT", "SIZE",
  "SEEK", "EXISTS", "BOOLTRUE", "BOOLFALSE", "MOUSEX", "MOUSEY", "MOUSEB",
  "CLICKCLEAR", "CLICKX", "CLICKY", "CLICKB", "GETCOLOR", "CLEAR", "BLACK",
  "WHITE", "RED", "DARKRED", "GREEN", "DARKGREEN", "BLUE", "DARKBLUE",
  "CYAN", "DARKCYAN", "PURPLE", "DARKPURPLE", "YELLOW", "DARKYELLOW",
  "ORANGE", "DARKORANGE", "GREY", "DARKGREY", "CHANGEDIR", "CURRENTDIR",
  "DECIMAL", "DBOPEN", "DBCLOSE", "DBEXECUTE", "DBOPENSET", "DBCLOSESET",
  "DBROW", "DBINT", "DBFLOAT", "DBSTRING", "ONERROR", "OFFERROR",
  "LASTERROR", "LASTERRORMESSAGE", "LASTERRORLINE", "LASTERROREXTRA",
  "LINENUM", "INTEGER", "FLOAT", "STRING", "VARIABLE", "STRINGVAR",
  "NEWVAR", "COLOR", "LABEL", "'<'", "'>'", "'='", "'-'", "'+'", "'*'",
  "'/'", "UMINUS", "'^'", "'\\n'", "':'", "'('", "','", "')'", "'['",
  "']'", "';'", "'{'", "'}'", "'?'", "$accept", "program", "validline",
  "label", "validstatement", "compoundifstmt", "ifstmt", "elsestmt",
  "endifexpr", "endifstmt", "whilestmt", "endwhileexpr", "endwhilestmt",
  "dostmt", "untilstmt", "compoundstmt", "statement", "dimstmt",
  "redimstmt", "pausestmt", "clearstmt", "fastgraphicsstmt",
  "graphsizestmt", "refreshstmt", "endstmt", "ifexpr", "strarrayassign",
  "arrayassign", "numassign", "stringassign", "forstmt", "nextstmt",
  "gotostmt", "gosubstmt", "offerrorstmt", "onerrorstmt", "returnstmt",
  "colorstmt", "soundstmt", "plotstmt", "linestmt", "circlestmt",
  "rectstmt", "textstmt", "fontstmt", "saystmt", "systemstmt",
  "volumestmt", "polystmt", "stampstmt", "openstmt", "writestmt",
  "writelinestmt", "closestmt", "resetstmt", "seekstmt", "inputstmt",
  "inputexpr", "printstmt", "wavplaystmt", "wavstopstmt", "wavwaitstmt",
  "putslicestmt", "imgloadstmt", "spritedimstmt", "spriteloadstmt",
  "spriteslicestmt", "spriteplacestmt", "spritemovestmt", "spritehidestmt",
  "spriteshowstmt", "clickclearstmt", "changedirstmt", "decimalstmt",
  "dbopenstmt", "dbclosestmt", "dbexecutestmt", "dbopensetstmt",
  "dbclosesetstmt", "immediatestrlist", "immediatelist", "floatlist",
  "floatexpr", "stringlist", "stringexpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,    60,    62,    61,
      45,    43,    42,    47,   432,    94,    10,    58,    40,    44,
      41,    91,    93,    59,   123,   125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   197,   198,   198,   199,   199,   200,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   202,   203,   204,
     205,   205,   206,   207,   208,   208,   209,   210,   211,   212,
     212,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   214,   214,   214,   214,   215,   215,   215,   215,   216,
     217,   217,   218,   219,   219,   220,   221,   222,   223,   223,
     223,   224,   224,   224,   225,   226,   227,   227,   228,   229,
     230,   231,   232,   233,   234,   234,   234,   234,   235,   235,
     235,   235,   235,   236,   236,   237,   237,   238,   238,   239,
     239,   240,   240,   240,   240,   241,   241,   242,   242,   242,
     243,   243,   244,   245,   245,   245,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   247,   247,   247,   247,   248,
     248,   248,   248,   249,   249,   249,   249,   250,   250,   250,
     251,   251,   251,   252,   252,   252,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   254,   254,   255,   255,
     255,   255,   255,   255,   255,   256,   256,   257,   257,   258,
     258,   259,   259,   259,   259,   260,   260,   260,   260,   260,
     260,   261,   262,   262,   263,   263,   264,   264,   265,   265,
     266,   267,   268,   268,   269,   269,   270,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   275,   276,   277,   278,
     278,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   280,   280,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     2,     1,
       1,     2,     1,     2,     1,     2,     1,     1,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     7,     7,     3,     3,     7,     7,     2,
       1,     1,     1,     4,     6,     1,     1,     2,     6,     8,
       3,     6,     8,     3,     3,     3,     6,     8,     2,     2,
       2,     1,     2,     1,     6,     8,     2,     4,     2,     4,
       2,     6,     4,     4,     6,     8,    10,     6,     8,     8,
      10,     6,     8,     6,     8,     6,     8,     2,     4,     2,
       2,     4,     2,     2,     4,     2,     8,    10,     8,     6,
       8,     6,    10,    12,    10,     4,     2,     6,     4,     4,
       2,     6,     4,     4,     2,     6,     4,     1,     3,     2,
       1,     3,     2,     2,     6,     4,     3,     6,     3,     6,
       2,     5,     7,     2,     5,     7,     2,     4,     1,     2,
       4,     2,     3,     5,     3,     2,     4,     1,     3,     1,
       3,     6,     8,     8,    10,     6,     8,     8,    10,    10,
      12,     2,     4,     6,    10,    12,     6,     8,     6,     8,
       2,     2,     1,     3,     2,     4,     2,     2,     4,     1,
       3,     2,     4,     2,     4,     1,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     4,     4,
       5,     5,     5,     5,     4,     6,     1,     4,     4,     4,
       4,     4,     4,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     4,     4,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     4,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       6,     8,     1,     3,     6,     4,     4,     4,     4,     4,
       3,     4,     4,     1,     3,     1,     3,     1,     3,     3,
       3,     3,     1,     4,     6,     1,     4,     4,     4,     4,
       8,     6,     6,    10,     1,     3,     4,     1,     3,     4,
       1,     3,     4,     1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      16,   198,     0,     0,     0,     0,     0,     0,     0,   102,
       0,   105,   100,   101,     0,    19,    20,     0,    24,    27,
       0,     0,     0,     0,     0,   177,   180,     0,     0,   123,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   207,   209,     0,   232,     0,     0,     0,   239,
       0,     0,   245,     0,   121,     0,     0,     6,     0,     0,
      16,     5,     7,     8,     9,    22,    10,    11,    26,    12,
      13,    14,    15,    29,    54,    55,    56,    50,    52,    53,
      51,    49,     0,    58,    57,    43,    44,    45,    46,    31,
      32,    33,    34,    35,    47,    64,    37,    42,    38,    39,
      65,    66,    67,    68,    69,    40,    41,    59,    60,    61,
      62,    63,    82,    48,     0,    36,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    83,    84,
      85,    86,    87,    88,    89,    90,   338,     0,     0,   424,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,     0,     0,     0,     0,     0,     0,   309,
       0,     0,     0,     0,     0,     0,     0,     0,   427,   315,
     319,   321,   323,   325,   327,   329,   331,   333,     0,     0,
       0,     0,     0,     0,     0,   335,     0,   311,   313,   340,
     342,   344,   346,   348,   350,   392,   352,   354,   356,   358,
     360,   362,   364,   366,   368,   370,   372,   374,   376,   378,
     380,   382,   384,   386,   388,   430,     0,     0,     0,     0,
     403,   433,   405,   435,   277,   276,   412,   286,   415,     0,
       0,   201,   199,   286,   415,     0,     0,   196,     0,     0,
       0,     0,     0,     0,     0,   153,     0,     0,   155,     0,
       0,     0,     0,     0,     0,     0,   107,    23,    28,     0,
     118,     0,     0,   166,     0,     0,   170,     0,   179,     0,
     182,   119,   120,    21,    25,     0,   126,     0,     0,     0,
       0,    99,   286,     0,   130,     0,     0,     0,   174,     0,
       0,     0,     0,     0,   149,   147,     0,   150,   152,     0,
       0,     0,     0,   221,     0,     0,     0,     0,     0,   230,
     231,     0,     0,     0,   205,     0,     0,     0,   183,     0,
       0,   234,   236,     0,   237,     0,     0,   241,     0,   243,
       0,   122,     0,     0,     0,     0,     1,     2,     4,     0,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   204,     0,     0,     0,     0,     0,     0,     0,   202,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,   181,     0,     0,     0,
      91,     0,    92,     0,    95,     0,    96,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   208,   210,     0,     0,   233,     0,     0,   240,     0,
       0,   246,   113,   114,     0,     0,   110,   115,     0,     3,
     106,    30,    17,   188,   186,   339,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
     310,     0,     0,     0,     0,     0,     0,     0,     0,   428,
       0,   316,     0,   320,   322,   324,   326,   328,   330,   332,
     334,     0,     0,     0,     0,     0,     0,     0,   336,     0,
       0,   312,   314,   341,   343,   345,   347,   349,   351,   393,
     353,   355,   357,   359,   361,   363,   365,   367,   369,   371,
     373,   375,   377,   379,   381,   383,   385,   387,   389,   431,
     400,     0,     0,     0,   404,   434,   406,   436,     0,     0,
       0,     0,     0,     0,   251,   200,   274,   275,   271,   260,
     261,   262,   255,   256,   272,   273,   270,   253,   252,   410,
     254,   257,   258,   268,   269,   265,   266,   267,   264,   411,
     409,     0,     0,   197,     0,   133,     0,     0,     0,     0,
     154,   248,     0,     0,     0,     0,     0,     0,   103,     0,
       0,   165,   168,     0,   169,   172,     0,   251,     0,     0,
       0,     0,     0,   129,     0,   132,     0,   173,   176,     0,
       0,     0,     0,   148,   151,     0,     0,     0,     0,     0,
     222,     0,     0,     0,     0,     0,   206,     0,   185,   235,
     238,   242,   244,     0,     0,     0,   407,     0,     0,     0,
       0,     0,     0,   426,   287,   288,   417,   291,     0,     0,
       0,   418,   419,     0,   294,   295,   296,   297,   298,   299,
     300,   301,   306,   302,   303,   304,   305,   292,   416,   289,
     290,   429,   317,     0,     0,   395,   396,   397,   398,   399,
     337,   318,   401,   402,   432,     0,     0,   278,     0,   284,
       0,     0,   279,     0,   413,   203,     0,   284,     0,   413,
       0,     0,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   282,   280,     0,   283,   281,     0,
       0,     0,   134,     0,   137,     0,     0,     0,   286,   161,
       0,     0,     0,   104,   116,   167,   171,     0,   124,     0,
       0,     0,     0,   131,   175,     0,   143,   141,     0,   145,
       0,   211,     0,     0,   215,     0,   223,     0,     0,   228,
       0,   226,   184,     0,   111,   408,     0,   108,   189,   187,
     390,     0,     0,   421,   422,   293,     0,   394,   285,   414,
     285,   414,     0,     0,     0,   286,     0,     0,     0,     0,
       0,     0,    93,    94,    97,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   138,     0,   139,   160,     0,   286,   158,
       0,     0,   135,   117,   125,   144,   142,   146,     0,   212,
     213,     0,   216,     0,   217,   229,     0,     0,   227,   112,
     109,   391,   420,     0,     0,   286,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   140,   157,     0,   162,
     164,   136,   214,     0,   218,   219,     0,   224,   423,     0,
       0,     0,   163,   220,   225
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   546,
     258,   480,   246,   745,   250
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -272
static const yytype_int16 yypact[] =
{
    8647,  3174,  3321,  3468,  3615,  3762,  -157,  3909,  4056,  -272,
    4203,  -272,  -272,  -272,  4350,  -272,  -272,  4350,  -272,  -272,
    4350,  -155,  -151,  4497,  4644,  4791,  4938,  -143,  -133,  -272,
      -5,  5085,  -161,  -103,  4350,  1236,  5232,  5379,  5526,  5673,
    5820,  4350,  5967,  6114,  4350,  6261,  6408,  4350,  4350,  4350,
    6555,  6702,  -187,  -138,  6849,  -131,  6996,  4350,  7143,  -129,
    7290,  7437,  -117,   -99,  -272,  -171,  -125,  -272,    81,   -82,
    8818,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -104,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,    75,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,   -88,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,   -49,   -37,   -35,   -31,
     -28,   -24,   -12,    19,    24,    26,    46,    48,    52,    53,
      60,    61,    63,    66,    67,    69,    71,    73,    77,    83,
      84,    86,    87,    89,  4350,    91,    95,    97,   105,   106,
     108,   109,   111,   113,   115,   116,   118,   119,   121,   123,
     126,   128,   129,   130,   131,   133,   135,   136,   137,   139,
     140,   142,   143,   145,   146,   148,   150,   151,   152,   155,
     157,   158,   159,   160,   161,   162,   163,   169,   170,   171,
     172,   173,   174,   175,   178,   186,   191,   192,   197,   202,
     204,   216,   218,   219,  -272,  -272,  -272,   -60,   -19,  4350,
    4350,   953,    32,  -126,  -119,  4350, 13806,  1569,  4350, 11714,
    1569,  4350, 11736,  4350, 11781,  -272,    11,  4350,  -272,  4350,
   11801,  4350, 11838,  4350, 11858,  4350, 13806, 13806, 13806,    90,
    -272,  4350, 11903,  1569,  4350, 11925,  1569,  2292, 13806,  2439,
   13806,  -272,  -272,  -272,  -272,  4350, 11960,  7584,  7731,  7878,
    8025, 13806,  -109,  8172,  -272, 11982,  4350, 12027,  1569,  4350,
   12047,  4350,    54,  4350, 13806,  1569,  4350,  1569, 13806,  4350,
   12084,  4350, 12104, 13806,  4350, 12149,  4350, 12171, 12206, 13806,
   13806,  4350, 12228,  4350,  1569,   220,   221,  4350, 12273,   222,
    4350,  1569, 13806,  4350,  1569,   228,  4350,  1569,  4350,  1569,
     230,  -272,  1704,  4350,  1851,  4350,  -272,  1502,  -272,  8984,
    8984,   -85,   231,  4350,  4350,  2586,  4350,  4350,  4350,  4350,
    4350,  4350,  4350,  4350,  4350,  4350,  4350,   232,  4350,  4350,
    4350,  4350,  4350,  4350,  4350,   236,  4350,  4350,  4350,  4350,
      78,  4350,  4350,  4350,  2733,  2880,   237,   239,   240,   241,
     245,   246,   247,   248,  4350,  4350,  4350,  4350,  4350,  4350,
    4350,  3027,  4350,   251,   252,   253,   270,   271,   272,   273,
     274,   275,   276,   278,   281,   282,   284,   286,   288,   289,
     290,   291,   293,   295,   298,   299,   300,   304,   305,   306,
     309,   311,   313,  4350,  4350,  4350,   315,   316,   317,   319,
      57,   486,    78, 10014,   -16,  4350,  4350,  4350,  4350,  4350,
    4350,  4350,  4350,  4350,  4350,  4350,  4350,  4350,  4350,  4350,
    4350,  -272,  4350,  4350,  4350,  4350,  4350,  4350,  4350,  -272,
      57,   486,   238,  8702,  4350,  9320,  4350,  9358,  4350,   320,
      47, 12293,  9391,  4350,  9448,  4350,  9472,  4350,  4350,  9510,
     381,  4350,  9535,   544,  4350,  -272,  -272,  9562,  4350,  4350,
   13806,  4350, 13806,  4350, 13806,  4350, 13806,   -91,  9597,  4350,
    9624,   672,  4350,  9684,  4350,   658,  4350,   710,   812,  9711,
    4350,  9746,  4350,  9768,  4350,  9813,  4350,  4350,  9833,  4350,
     885,  -272,  -272,  9870,  4350,  -272,   930,   964,  -272,   986,
    1074,  -272,  -272, 13806,  1005,  4350,  -272,  1569,  1041,  -272,
    -272,  -272,  -104,   217,   321,  -272, 12330, 12350,  -272, 10059,
   10079,  1080, 10116,  1131,  2512,  2795,  2953,  1233,  1287,  3236,
   10136, 10181,  -272, 10203, 10238, 10260, 10305, 10325, 10362, 10382,
    -272, 10427, 10449, 10484, 10506,  1294, 10551, 10571,  1309,  -272,
   10608,  -272, 10628,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272, 12395, 12417, 10673, 10695, 10730, 10752, 10797,  -272, 10817,
    1384,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272, 10854, 10874, 10919,  -272,  -272,  -272,  -272,   213,  -134,
    1375,   318,   -98,  8855,  -272,   323,   206,   206,   206,  9030,
    8693, 13826,    78,    78,   206,   206,   206,   -33,   -33,  -272,
      78,    78,  -272,   330,   330,   330,   330,   330,   330,   -33,
    -272,  8873,  9039,  -272,  4350, 13806,  4350, 12452,  4350, 12474,
    -272,  -272,  4350,  4350, 12519,  4350, 12539,  4350, 13806,   839,
    4350,  -272,  1569,  4350,  -272,  1569,  4350,   -94, 12576,  9890,
    9935,  9957,  9992,  -272,  4350, 13806,  4350,  -272,  1569,  4350,
   12596,  4350, 12641,  -272,  -272,  4350, 12663,  4350, 12698,  4350,
    1569,  4350, 12720, 12765,  4350, 12785,  -272,  4350, 13806,  -272,
    -272,  -272,  -272,  4350,   338,   325,  3394,  4350,   343,  4350,
    4350,  4350,  4350,  -272,  -272,  -272,  -272,  -272,  4350,  4350,
    4350,  -272,  -272,  4350,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  4350,  4350,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,   331,   332,  -272,  4350,  -272,
     335,   337,  -272,  4350,  -272,  -272,  4350,   -84,  4350,   -57,
   10941, 12822,  4350, 12842,  4350,  -272, 12887,  1998, 12909,  4350,
   10976,  4350,  1391,  1409, 12944,  4350,  4350,  4350,  4350,  4350,
   10998,  1453, 12966,  4350, 13011,  4350, 13031,  4350, 13068,  4350,
   13088,  1619, 13133,  4350,  4350, 13155,  4350, 11043,  9076,  4350,
    -272,  4350,  9100,  4350,  9120,  9153, 11063, 13190, 13212, 11100,
   11120,  1629, 13257, 11165,  -272,  -272,  9190,  -272,  -272,  9244,
    9267,  9304,  -272,  4350, 13806,  4350, 13277,  8319,  -107,  -272,
   13314,  4350, 13334,  -272,   874,  -272,  -272,  4350, 13806, 11187,
   11222, 11244, 11289,  -272,  -272,  4350, 13806,  1569,  4350, 13806,
    4350,  3677,  4350, 13379,  1569,  4350,  -272,  4350, 13401, 13806,
    4350, 13806,  -272,   351, 13806,  -272,   352,  1569,  -272,  -272,
    -272,  4350,  4350,  -272,  -272,  -272,  4350,  -272,  -272,  -272,
     -53,   -45, 11309, 13436,  4350,   -32, 13458,  2145, 13503,  4350,
    4350, 11346,  -272,  -272,  -272,  -272, 11366,  1701, 11411,   224,
    4350, 13523,  1913,  4350, 11433, 13560,  4350, 11468,  4350,  4350,
   11490, 11535, 13580,  -272,  4350, 13806,  -272,  8466,  -101,  -272,
   13625,  4350, 13806, 13806,  -272,  -272,  -272,  -272,  4350,  -272,
   13806,  4350,  -272, 13647,  1569,  -272,  4350, 13682,  -272, 13806,
    1569,  -272,  -272,  4350, 11555,    29, 13704,  -162, 11592, 11612,
   13749,  2071,  4350, 13769,  4350, 11657,  -272,  -272,   360,  -272,
    -272,  -272,  -272,  4350,  -272,  1569,  4350, 13806,  -272,   373,
    2354, 11679,  -272,  -272,  -272
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -272,   223,  -272,  -272,   498,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,   229,   233,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
    -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,  -272,
     -30,  -114,    -1,  -271,   738
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -196
static const yytype_int16 yytable[] =
{
     241,   325,   249,   252,   254,   294,   260,   262,   342,   264,
    1009,   287,   288,   266,   283,   255,   267,   269,   284,   268,
     343,   270,   272,   275,   278,   280,   462,   463,   464,   281,
     286,   256,   257,   291,   295,   297,   300,   257,   304,   282,
     308,   310,   312,   313,   315,   317,   318,   319,   320,   322,
     326,   451,   452,   328,   344,   796,   332,   329,   797,   335,
    -193,  -193,   146,   147,   148,   470,   345,  -190,  -190,   289,
     290,   340,   471,   341,   462,   463,   464,  -128,  -128,  -159,
    -159,   346,   440,   349,   440,  -156,  -156,   553,   554,   149,
     440,   801,  -127,  -127,   802,   350,   462,   463,   464,   713,
     440,   351,  -194,  -194,   347,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,  -191,
    -191,   440,   174,  -195,  -195,   175,   176,   177,   178,   352,
     179,  -192,  -192,   180,   181,   182,   183,   184,   185,   458,
     459,   353,   460,   354,   186,   187,   188,   355,   966,   440,
     356,   465,   466,   467,   357,   468,   189,   190,   191,   192,
     193,   194,   441,   380,   655,   195,   358,   196,   197,   198,
     199,   200,   201,   479,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   359,   225,   465,
     466,   467,   360,   468,   361,   226,   227,   228,   229,  1007,
     440,   230,   231,   232,   233,   469,   234,   235,   236,   237,
     238,   465,   466,   467,   362,   468,   363,   239,   442,   443,
     364,   365,   691,   516,   443,   265,   648,   473,   366,   367,
     475,   368,   477,   649,   369,   370,   481,   371,   482,   372,
     484,   373,   486,   460,   443,   374,   462,   463,   464,   488,
     489,   375,   376,   492,   377,   378,   443,   379,   443,   381,
     462,   463,   464,   382,   497,   383,   500,   502,   504,   506,
     451,   452,   508,   384,   385,   510,   386,   387,   513,   388,
     443,   389,   443,   390,   391,   443,   392,   393,   519,   394,
     521,   395,   542,   523,   396,   525,   397,   398,   399,   400,
     528,   401,   443,   402,   403,   404,   533,   405,   406,   443,
     407,   408,   443,   409,   410,   443,   411,   443,   412,   413,
     414,   543,   544,   415,   548,   416,   417,   418,   419,   420,
     421,   422,   556,   557,   559,   560,   562,   423,   424,   425,
     426,   427,   428,   429,   570,   571,   430,   573,   574,   575,
     576,   577,   578,   579,   431,   581,   582,   583,   584,   432,
     433,   586,   587,   590,   592,   434,   456,   457,   458,   459,
     435,   460,   436,   601,   602,   603,   604,   605,   606,   607,
     609,   465,   466,   467,   437,   468,   438,   439,   749,   795,
     531,   532,   535,   978,   979,   465,   466,   467,   538,   468,
     541,   555,   572,   462,   463,   464,   580,   593,   683,   594,
     595,   596,   641,   642,   643,   597,   598,   599,   600,   650,
     653,   611,   612,   613,   656,   657,   658,   659,   660,   661,
     662,   663,   664,   665,   666,   667,   668,   670,   671,   672,
     614,   615,   616,   617,   618,   619,   620,   679,   621,   681,
     682,   622,   623,   685,   624,   687,   625,   689,   626,   627,
     628,   629,   694,   630,   696,   631,   698,   699,   632,   633,
     634,   146,   147,   148,   635,   636,   637,   708,   709,   638,
     710,   639,   711,   640,   712,   644,   645,   646,   715,   647,
     690,   468,   750,   720,   800,   722,   805,   849,   149,   726,
     850,   728,   853,   864,   865,   732,   733,   867,   735,   868,
     958,   959,  1019,   738,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   465,   466,
     467,   174,   468,  1022,   175,   176,   177,   178,   348,   179,
     549,   701,   180,   181,   182,   183,   184,   185,   815,   552,
     915,     0,   551,   186,   187,   188,   462,   463,   464,     0,
       0,     0,     0,     0,     0,   189,   190,   191,   192,   193,
     194,     0,     0,     0,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,     0,     0,
       0,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   237,   238,
       0,     0,     0,     0,     0,     0,   239,     0,     0,     0,
       0,     0,     0,     0,   265,   651,     0,     0,     0,     0,
       0,     0,   652,   810,     0,   811,     0,   813,     0,     0,
       0,   481,   816,     0,   818,     0,   820,     0,     0,     0,
     462,   463,   464,     0,     0,   824,     0,     0,     0,     0,
       0,     0,     0,   830,   462,   463,   464,     0,   832,     0,
     834,   465,   466,   467,   836,   468,   838,     0,   840,     0,
     842,     0,     0,   845,   704,     0,   847,     0,     0,   242,
     247,     0,   848,     0,     0,     0,   852,     0,   854,   855,
     856,   857,   462,   463,   464,     0,     0,   858,   859,   860,
       0,   273,   276,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,   302,   305,   307,     0,
       0,     0,   862,   863,     0,     0,     0,   879,     0,   324,
       0,     0,     0,     0,   331,     0,   334,   866,   337,   339,
       0,     0,   869,     0,     0,   870,     0,   871,     0,     0,
       0,   874,     0,   876,     0,     0,   880,     0,   882,     0,
     884,     0,     0,     0,   888,   889,   890,   891,   892,     0,
       0,     0,   896,     0,   899,   465,   466,   467,   903,   468,
       0,     0,   908,   909,     0,   911,     0,   721,   914,   465,
     466,   467,     0,   468,   462,   463,   464,     0,     0,     0,
       0,     0,   717,     0,     0,     0,     0,   821,     0,     0,
       0,     0,   932,     0,   933,     0,   936,     0,     0,     0,
     938,   445,   446,   447,     0,     0,   941,   465,   466,   467,
       0,   468,     0,     0,   946,     0,     0,   948,     0,     0,
     723,   951,     0,   940,   954,     0,   955,   969,     0,   957,
       0,   448,   449,   450,     0,     0,   445,   446,   447,     0,
     960,   961,     0,   451,   452,   962,     0,   462,   463,   464,
       0,     0,     0,   965,     0,     0,   970,     0,   972,   973,
       0,     0,     0,     0,     0,     0,   448,   449,   450,   980,
       0,     0,   983,     0,     0,   987,     0,   989,   451,   452,
       0,     0,     0,   994,     0,     0,   996,  1010,     0,     0,
     998,     0,   462,   463,   464,     0,     0,   999,   444,     0,
    1000,     0,     0,   472,     0,  1003,     0,     0,     0,   465,
     466,   467,  1005,   468,     0,   445,   446,   447,     0,     0,
       0,     0,   724,  1017,     0,     0,   462,   463,   464,   490,
       0,     0,   493,     0,     0,  1021,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   448,   449,   450,   462,   463,
     464,     0,     0,     0,   511,     0,     0,   451,   452,   515,
       0,   517,     0,     0,   518,     0,     0,   445,   446,   447,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
       0,   530,   465,   466,   467,     0,   468,     0,   536,     0,
       0,   537,     0,     0,   539,   736,   540,   448,   449,   450,
       0,     0,   547,   445,   446,   447,     0,     0,     0,   451,
     452,     0,     0,     0,   561,     0,   563,   564,   565,   566,
     567,   568,   569,     0,     0,     0,     0,   465,   466,   467,
       0,   468,     0,   448,   449,   450,   462,   463,   464,   585,
     739,   588,   462,   463,   464,   451,   452,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,     0,
     610,   465,   466,   467,     0,   468,   461,     0,     0,     0,
       0,     0,     0,     0,   740,     0,     0,     0,     0,     0,
       0,     0,     0,   465,   466,   467,     0,   468,     0,     0,
       0,     0,     0,   462,   463,   464,   741,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,     0,     0,     0,   743,   669,     0,   744,     0,     0,
     673,   674,   675,   676,   677,   678,   680,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,     0,     0,   702,
     747,     0,   705,   748,     0,     0,     0,     0,     0,     0,
       0,   146,   147,   148,     0,     0,     0,     0,     0,     0,
     718,   465,   466,   467,     0,   468,     0,   465,   466,   467,
       0,   468,   730,     0,   742,     0,     0,     0,   149,     0,
     755,     0,     0,     0,     0,   462,   463,   464,     0,     0,
       0,     0,     0,   746,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   465,   466,
     467,   174,   468,     0,   175,   176,   177,   178,     0,   179,
       0,   757,   180,   181,   182,   183,   184,   185,     0,   462,
     463,   464,     0,   186,   187,   188,   462,   463,   464,     0,
       0,     0,     0,     0,     0,   189,   190,   191,   192,   193,
     194,   462,   463,   464,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,     0,     0,
       0,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   292,   238,
     465,   466,   467,     0,   468,     0,   239,   445,   446,   447,
       0,     0,     0,   761,   293,     0,   462,   463,   464,     0,
     257,     0,     0,   462,   463,   464,     0,     0,   822,     0,
       0,   823,     0,     0,     0,     0,     0,   448,   449,   450,
       0,   462,   463,   464,   831,     0,     0,     0,     0,   451,
     452,     0,     0,     0,   465,   466,   467,   841,   468,     0,
       0,   465,   466,   467,     0,   468,     0,   762,     0,     0,
       0,     0,     0,     0,   777,     0,   465,   466,   467,     0,
     468,     0,     0,     0,     0,   462,   463,   464,     0,   780,
       0,   861,     0,     0,     0,     1,     2,     0,     0,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,     0,    15,    16,    17,    18,    19,    20,    21,
       0,     0,    22,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,     0,     0,    32,    33,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   465,   466,   467,   798,   468,     0,   799,   465,   466,
     467,   897,   468,     0,   791,   901,     0,   904,    34,    35,
       0,   885,     0,     0,    36,     0,   465,   466,   467,   746,
     468,   917,     0,     0,    37,    38,    39,    40,    41,   886,
       0,     0,    42,    43,    44,    45,    46,    47,    48,    49,
      50,   462,   463,   464,     0,     0,     0,    51,    52,    53,
       0,    54,     0,     0,     0,     0,     0,     0,    55,     0,
     465,   466,   467,   947,   468,     0,     0,     0,   949,     0,
     952,     0,     0,   894,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    57,    58,    59,    60,    61,    62,
       0,   462,   463,   464,    63,    64,     0,     0,     0,     0,
       0,   462,   463,   464,    65,    66,     0,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -16,     0,
       0,   984,     0,     0,     0,     0,     0,   990,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   146,
     147,   148,     0,     0,     0,     0,     0,     0,     0,  1001,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,     0,     0,     0,
    1015,     0,     0,   462,   463,   464,   465,   466,   467,     0,
     468,  1020,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,   178,     0,   179,     0,     0,
     180,   181,   182,   183,   184,   185,   465,   466,   467,     0,
     468,   186,   187,   188,     0,     0,   465,   466,   467,   906,
     468,     0,     0,   189,   190,   191,   192,   193,   194,   925,
       0,     0,   195,     0,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,     0,   225,   146,   147,   148,     0,
       0,     0,   226,   227,   228,   229,     0,     0,   230,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   465,   466,
     467,     0,   468,   149,   239,     0,     0,     0,     0,     0,
       0,   976,   265,     0,     0,     0,     0,     0,   257,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,   178,     0,   179,     0,     0,   180,   181,   182,
     183,   184,   185,     0,     0,     0,     0,     0,   186,   187,
     188,     0,     0,     0,     0,   462,   463,   464,     0,     0,
     189,   190,   191,   192,   193,   194,     0,     0,     0,   195,
       0,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,     0,   225,   146,   147,   148,     0,     0,     0,   226,
     227,   228,   229,     0,     0,   230,   231,   232,   233,     0,
     234,   235,   236,   237,   238,     0,     0,     0,     0,     0,
     149,   239,     0,     0,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,     0,   545,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,   178,
       0,   179,     0,     0,   180,   181,   182,   183,   184,   185,
     465,   466,   467,     0,   468,   186,   187,   188,     0,     0,
       0,     0,     0,   982,     0,     0,     0,   189,   190,   191,
     192,   193,   194,   462,   463,   464,   195,     0,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,     0,   225,
     146,   147,   148,     0,     0,     0,   226,   227,   228,   229,
       0,     0,   230,   231,   232,   233,     0,   234,   235,   236,
     878,   238,     0,     0,     0,     0,     0,   149,   239,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,     0,   257,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,   178,     0,   179,     0,
       0,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,   186,   187,   188,     0,     0,     0,   465,   466,
     467,     0,   468,     0,   189,   190,   191,   192,   193,   194,
       0,  1014,     0,   195,     0,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,     0,   225,   146,   147,   148,
       0,     0,     0,   226,   227,   228,   229,     0,     0,   230,
     231,   232,   233,     0,   234,   235,   236,   968,   238,     0,
       0,     0,     0,     0,   149,   239,     0,     0,     0,     0,
       0,     0,     0,   265,     0,     0,     0,     0,     0,   257,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,   178,     0,   179,     0,     0,   180,   181,
     182,   183,   184,   185,     0,     0,     0,     0,     0,   186,
     187,   188,     0,     0,     0,     0,   462,   463,   464,     0,
       0,   189,   190,   191,   192,   193,   194,     0,     0,     0,
     195,     0,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,     0,   225,   146,   147,   148,     0,     0,     0,
     226,   227,   228,   229,     0,     0,   230,   231,   232,   233,
       0,   234,   235,   236,   237,   238,     0,     0,     0,     0,
       0,   149,   239,     0,     0,     0,     0,     0,     0,     0,
     265,     0,   495,     0,     0,     0,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
     178,     0,   179,     0,     0,   180,   181,   182,   183,   184,
     185,   465,   466,   467,     0,   468,   186,   187,   188,     0,
       0,     0,     0,     0,  1023,     0,     0,     0,   189,   190,
     191,   192,   193,   194,   462,   463,   464,   195,     0,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,     0,
     225,   146,   147,   148,     0,     0,     0,   226,   227,   228,
     229,     0,     0,   230,   231,   232,   233,     0,   234,   235,
     236,   237,   238,     0,     0,     0,     0,     0,   149,   239,
       0,     0,     0,     0,     0,     0,     0,   265,     0,   496,
       0,     0,     0,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,   178,     0,   179,
       0,     0,   180,   181,   182,   183,   184,   185,     0,     0,
       0,     0,     0,   186,   187,   188,     0,     0,     0,   465,
     466,   467,     0,   468,     0,   189,   190,   191,   192,   193,
     194,   758,     0,     0,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,   146,   147,
     148,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   237,   238,
       0,     0,     0,     0,     0,   149,   239,     0,     0,     0,
       0,     0,     0,     0,   265,     0,   558,     0,     0,     0,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,   178,     0,   179,     0,     0,   180,
     181,   182,   183,   184,   185,     0,     0,     0,     0,     0,
     186,   187,   188,     0,     0,     0,     0,   462,   463,   464,
       0,     0,   189,   190,   191,   192,   193,   194,     0,     0,
       0,   195,     0,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,     0,   225,   146,   147,   148,     0,     0,
       0,   226,   227,   228,   229,     0,     0,   230,   231,   232,
     233,     0,   234,   235,   236,   237,   238,     0,     0,     0,
       0,     0,   149,   239,     0,     0,     0,     0,     0,     0,
       0,   265,     0,   589,     0,     0,     0,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,   178,     0,   179,     0,     0,   180,   181,   182,   183,
     184,   185,   465,   466,   467,     0,   468,   186,   187,   188,
       0,     0,     0,     0,   759,     0,     0,     0,     0,   189,
     190,   191,   192,   193,   194,   462,   463,   464,   195,     0,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
       0,   225,   146,   147,   148,     0,     0,     0,   226,   227,
     228,   229,     0,     0,   230,   231,   232,   233,     0,   234,
     235,   236,   237,   238,     0,     0,     0,     0,     0,   149,
     239,     0,     0,     0,     0,     0,     0,     0,   265,     0,
     591,     0,     0,     0,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,   178,     0,
     179,     0,     0,   180,   181,   182,   183,   184,   185,     0,
       0,     0,     0,     0,   186,   187,   188,     0,     0,     0,
     465,   466,   467,     0,   468,     0,   189,   190,   191,   192,
     193,   194,   760,     0,     0,   195,     0,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,     0,   225,   146,
     147,   148,     0,     0,     0,   226,   227,   228,   229,     0,
       0,   230,   231,   232,   233,     0,   234,   235,   236,   237,
     238,     0,     0,     0,     0,     0,   149,   239,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   608,     0,     0,
       0,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,   178,     0,   179,     0,     0,
     180,   181,   182,   183,   184,   185,     0,     0,     0,     0,
       0,   186,   187,   188,     0,     0,     0,     0,   462,   463,
     464,     0,     0,   189,   190,   191,   192,   193,   194,     0,
       0,     0,   195,     0,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,     0,   225,   146,   147,   148,     0,
       0,     0,   226,   227,   228,   229,     0,     0,   230,   231,
     232,   233,     0,   234,   235,   236,   237,   238,     0,     0,
       0,     0,     0,   149,   239,     0,     0,     0,     0,     0,
       0,     0,   240,     0,     0,     0,     0,     0,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,   178,     0,   179,     0,     0,   180,   181,   182,
     183,   184,   185,   465,   466,   467,     0,   468,   186,   187,
     188,     0,     0,     0,     0,   763,     0,     0,     0,     0,
     189,   190,   191,   192,   193,   194,   462,   463,   464,   195,
       0,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,     0,   225,   146,   147,   148,     0,     0,     0,   226,
     227,   228,   229,     0,     0,   230,   231,   232,   233,     0,
     234,   235,   236,   243,   244,     0,     0,     0,     0,     0,
     149,   239,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,   178,
       0,   179,     0,     0,   180,   181,   182,   183,   184,   185,
       0,     0,     0,     0,     0,   186,   187,   188,     0,     0,
       0,   465,   466,   467,     0,   468,     0,   189,   190,   191,
     192,   193,   194,   851,     0,     0,   195,     0,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,     0,   225,
     146,   147,   148,     0,     0,     0,   226,   227,   228,   229,
       0,     0,   230,   231,   232,   233,     0,   234,   235,   236,
     237,   238,     0,     0,     0,     0,     0,   149,   239,     0,
       0,     0,     0,     0,     0,     0,   248,     0,     0,     0,
       0,     0,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,   178,     0,   179,     0,
       0,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,   186,   187,   188,     0,     0,     0,     0,   462,
     463,   464,     0,     0,   189,   190,   191,   192,   193,   194,
       0,     0,     0,   195,     0,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,     0,   225,   146,   147,   148,
       0,     0,     0,   226,   227,   228,   229,     0,     0,   230,
     231,   232,   233,     0,   234,   235,   236,   237,   238,     0,
       0,     0,     0,     0,   149,   239,     0,     0,     0,     0,
       0,     0,     0,   251,     0,     0,     0,     0,     0,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,   178,     0,   179,     0,     0,   180,   181,
     182,   183,   184,   185,   465,   466,   467,     0,   468,   186,
     187,   188,     0,     0,     0,     0,   950,     0,     0,     0,
       0,   189,   190,   191,   192,   193,   194,     0,     0,     0,
     195,     0,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,     0,   225,   146,   147,   148,     0,     0,     0,
     226,   227,   228,   229,     0,     0,   230,   231,   232,   233,
       0,   234,   235,   236,   237,   238,     0,     0,     0,     0,
       0,   149,   239,     0,     0,     0,     0,     0,     0,     0,
     253,     0,     0,     0,     0,     0,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
     178,     0,   179,     0,     0,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,     0,   186,   187,   188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,   192,   193,   194,     0,     0,     0,   195,     0,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,     0,
     225,   146,   147,   148,     0,     0,     0,   226,   227,   228,
     229,     0,     0,   230,   231,   232,   233,     0,   234,   235,
     236,   237,   238,     0,     0,     0,     0,     0,   149,   239,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,     0,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,   178,     0,   179,
       0,     0,   180,   181,   182,   183,   184,   185,     0,     0,
       0,     0,     0,   186,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,   192,   193,
     194,     0,     0,     0,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,   146,   147,
     148,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   237,   238,
       0,     0,     0,     0,     0,   149,   239,     0,     0,     0,
       0,     0,     0,     0,   261,     0,     0,     0,     0,     0,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,   178,     0,   179,     0,     0,   180,
     181,   182,   183,   184,   185,     0,     0,     0,     0,     0,
     186,   187,   188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,   190,   191,   192,   193,   194,     0,     0,
       0,   195,     0,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,     0,   225,   146,   147,   148,     0,     0,
       0,   226,   227,   228,   229,     0,     0,   230,   231,   232,
     233,     0,   234,   235,   236,   237,   238,     0,     0,     0,
       0,     0,   149,   239,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,   178,     0,   179,     0,     0,   180,   181,   182,   183,
     184,   185,     0,     0,     0,     0,     0,   186,   187,   188,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
     190,   191,   192,   193,   194,     0,     0,     0,   195,     0,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
       0,   225,   146,   147,   148,     0,     0,     0,   226,   227,
     228,   229,     0,     0,   230,   231,   232,   233,     0,   234,
     235,   236,   237,   238,     0,     0,     0,     0,     0,   149,
     239,     0,     0,     0,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,   178,     0,
     179,     0,     0,   180,   181,   182,   183,   184,   185,     0,
       0,     0,     0,     0,   186,   187,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   190,   191,   192,
     193,   194,     0,     0,     0,   195,     0,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,     0,   225,   146,
     147,   148,     0,     0,     0,   226,   227,   228,   229,     0,
       0,   230,   231,   232,   233,     0,   234,   235,   236,   237,
     238,     0,     0,     0,     0,     0,   149,   239,     0,     0,
       0,     0,     0,     0,     0,   271,     0,     0,     0,     0,
       0,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,   178,     0,   179,     0,     0,
     180,   181,   182,   183,   184,   185,     0,     0,     0,     0,
       0,   186,   187,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,   192,   193,   194,     0,
       0,     0,   195,     0,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,     0,   225,   146,   147,   148,     0,
       0,     0,   226,   227,   228,   229,     0,     0,   230,   231,
     232,   233,     0,   234,   235,   236,   237,   238,     0,     0,
       0,     0,     0,   149,   239,     0,     0,     0,     0,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,   178,     0,   179,     0,     0,   180,   181,   182,
     183,   184,   185,     0,     0,     0,     0,     0,   186,   187,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,   192,   193,   194,     0,     0,     0,   195,
       0,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,     0,   225,   146,   147,   148,     0,     0,     0,   226,
     227,   228,   229,     0,     0,   230,   231,   232,   233,     0,
     234,   235,   236,   237,   238,     0,     0,     0,     0,     0,
     149,   239,     0,     0,     0,     0,     0,     0,     0,   277,
       0,     0,     0,     0,     0,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,   178,
       0,   179,     0,     0,   180,   181,   182,   183,   184,   185,
       0,     0,     0,     0,     0,   186,   187,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   190,   191,
     192,   193,   194,     0,     0,     0,   195,     0,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,     0,   225,
     146,   147,   148,     0,     0,     0,   226,   227,   228,   229,
       0,     0,   230,   231,   232,   233,     0,   234,   235,   236,
     237,   238,     0,     0,     0,     0,     0,   149,   239,     0,
       0,     0,     0,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,   178,     0,   179,     0,
       0,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,   186,   187,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,   192,   193,   194,
       0,     0,     0,   195,     0,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,     0,   225,   146,   147,   148,
       0,     0,     0,   226,   227,   228,   229,     0,     0,   230,
     231,   232,   233,     0,   234,   235,   236,   237,   238,     0,
       0,     0,     0,     0,   149,   239,     0,     0,     0,     0,
       0,     0,     0,   285,     0,     0,     0,     0,     0,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,   178,     0,   179,     0,     0,   180,   181,
     182,   183,   184,   185,     0,     0,     0,     0,     0,   186,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,   192,   193,   194,     0,     0,     0,
     195,     0,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,     0,   225,   146,   147,   148,     0,     0,     0,
     226,   227,   228,   229,     0,     0,   230,   231,   232,   233,
       0,   234,   235,   236,   237,   238,     0,     0,     0,     0,
       0,   149,   239,     0,     0,     0,     0,     0,     0,     0,
     296,     0,     0,     0,     0,     0,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
     178,     0,   179,     0,     0,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,     0,   186,   187,   188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,   192,   193,   194,     0,     0,     0,   195,     0,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,     0,
     225,   146,   147,   148,     0,     0,     0,   226,   227,   228,
     229,     0,     0,   230,   231,   232,   233,     0,   234,   235,
     236,   237,   238,     0,     0,     0,     0,     0,   149,   239,
       0,     0,     0,     0,     0,     0,     0,   299,     0,     0,
       0,     0,     0,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,   178,     0,   179,
       0,     0,   180,   181,   182,   183,   184,   185,     0,     0,
       0,     0,     0,   186,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,   192,   193,
     194,     0,     0,     0,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,   146,   147,
     148,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   237,   238,
       0,     0,     0,     0,     0,   149,   239,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,   178,     0,   179,     0,     0,   180,
     181,   182,   183,   184,   185,     0,     0,     0,     0,     0,
     186,   187,   188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,   190,   191,   192,   193,   194,     0,     0,
       0,   195,     0,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,     0,   225,   146,   147,   148,     0,     0,
       0,   226,   227,   228,   229,     0,     0,   230,   231,   232,
     233,     0,   234,   235,   236,   237,   238,     0,     0,     0,
       0,     0,   149,   239,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,     0,     0,     0,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,   178,     0,   179,     0,     0,   180,   181,   182,   183,
     184,   185,     0,     0,     0,     0,     0,   186,   187,   188,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
     190,   191,   192,   193,   194,     0,     0,     0,   195,     0,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
       0,   225,   146,   147,   148,     0,     0,     0,   226,   227,
     228,   229,     0,     0,   230,   231,   232,   233,     0,   234,
     235,   236,   237,   238,     0,     0,     0,     0,     0,   149,
     239,     0,     0,     0,     0,     0,     0,     0,   306,     0,
       0,     0,     0,     0,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,   178,     0,
     179,     0,     0,   180,   181,   182,   183,   184,   185,     0,
       0,     0,     0,     0,   186,   187,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   190,   191,   192,
     193,   194,     0,     0,     0,   195,     0,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,     0,   225,   146,
     147,   148,     0,     0,     0,   226,   227,   228,   229,     0,
       0,   230,   231,   232,   233,     0,   234,   235,   236,   237,
     238,     0,     0,     0,     0,     0,   149,   239,     0,     0,
       0,     0,     0,     0,     0,   309,     0,     0,     0,     0,
       0,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,   178,     0,   179,     0,     0,
     180,   181,   182,   183,   184,   185,     0,     0,     0,     0,
       0,   186,   187,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,   192,   193,   194,     0,
       0,     0,   195,     0,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,     0,   225,   146,   147,   148,     0,
       0,     0,   226,   227,   228,   229,     0,     0,   230,   231,
     232,   233,     0,   234,   235,   236,   237,   238,     0,     0,
       0,     0,     0,   149,   239,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,     0,     0,     0,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,   178,     0,   179,     0,     0,   180,   181,   182,
     183,   184,   185,     0,     0,     0,     0,     0,   186,   187,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,   192,   193,   194,     0,     0,     0,   195,
       0,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,     0,   225,   146,   147,   148,     0,     0,     0,   226,
     227,   228,   229,     0,     0,   230,   231,   232,   233,     0,
     234,   235,   236,   237,   238,     0,     0,     0,     0,     0,
     149,   239,     0,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,   178,
       0,   179,     0,     0,   180,   181,   182,   183,   184,   185,
       0,     0,     0,     0,     0,   186,   187,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   190,   191,
     192,   193,   194,     0,     0,     0,   195,     0,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,     0,   225,
     146,   147,   148,     0,     0,     0,   226,   227,   228,   229,
       0,     0,   230,   231,   232,   233,     0,   234,   235,   236,
     237,   238,     0,     0,     0,     0,     0,   149,   239,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,   178,     0,   179,     0,
       0,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,   186,   187,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,   192,   193,   194,
       0,     0,     0,   195,     0,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,     0,   225,   146,   147,   148,
       0,     0,     0,   226,   227,   228,   229,     0,     0,   230,
     231,   232,   233,     0,   234,   235,   236,   237,   238,     0,
       0,     0,     0,     0,   149,   239,     0,     0,     0,     0,
       0,     0,     0,   321,     0,     0,     0,     0,     0,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,   178,     0,   179,     0,     0,   180,   181,
     182,   183,   184,   185,     0,     0,     0,     0,     0,   186,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,   192,   193,   194,     0,     0,     0,
     195,     0,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,     0,   225,   146,   147,   148,     0,     0,     0,
     226,   227,   228,   229,     0,     0,   230,   231,   232,   233,
       0,   234,   235,   236,   237,   238,     0,     0,     0,     0,
       0,   149,   239,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,     0,     0,     0,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
     178,     0,   179,     0,     0,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,     0,   186,   187,   188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,   192,   193,   194,     0,     0,     0,   195,     0,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,     0,
     225,   146,   147,   148,     0,     0,     0,   226,   227,   228,
     229,     0,     0,   230,   231,   232,   233,     0,   234,   235,
     236,   237,   238,     0,     0,     0,     0,     0,   149,   239,
       0,     0,     0,     0,     0,     0,     0,   327,     0,     0,
       0,     0,     0,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,   178,     0,   179,
       0,     0,   180,   181,   182,   183,   184,   185,     0,     0,
       0,     0,     0,   186,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,   192,   193,
     194,     0,     0,     0,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,   146,   147,
     148,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   237,   238,
       0,     0,     0,     0,     0,   149,   239,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
       0,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,     0,     0,     0,   174,     0,
       0,   175,   176,   177,   178,     0,   179,     0,     0,   180,
     181,   182,   183,   184,   185,     0,     0,     0,     0,     0,
     186,   187,   188,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,   190,   191,   192,   193,   194,     0,     0,
       0,   195,     0,   196,   197,   198,   199,   200,   201,     0,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,     0,   225,   146,   147,   148,     0,     0,
       0,   226,   227,   228,   229,     0,     0,   230,   231,   232,
     233,     0,   234,   235,   236,   237,   238,     0,     0,     0,
       0,     0,   149,   239,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,     0,     0,     0,   174,     0,     0,   175,   176,
     177,   178,     0,   179,     0,     0,   180,   181,   182,   183,
     184,   185,     0,     0,     0,     0,     0,   186,   187,   188,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   189,
     190,   191,   192,   193,   194,     0,     0,     0,   195,     0,
     196,   197,   198,   199,   200,   201,     0,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
       0,   225,   146,   147,   148,     0,     0,     0,   226,   227,
     228,   229,     0,     0,   230,   231,   232,   233,     0,   234,
     235,   236,   237,   238,     0,     0,     0,     0,     0,   149,
     239,     0,     0,     0,     0,     0,     0,     0,   336,     0,
       0,     0,     0,     0,     0,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,     0,
       0,     0,   174,     0,     0,   175,   176,   177,   178,     0,
     179,     0,     0,   180,   181,   182,   183,   184,   185,     0,
       0,     0,     0,     0,   186,   187,   188,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   190,   191,   192,
     193,   194,     0,     0,     0,   195,     0,   196,   197,   198,
     199,   200,   201,     0,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,     0,   225,   146,
     147,   148,     0,     0,     0,   226,   227,   228,   229,     0,
       0,   230,   231,   232,   233,     0,   234,   235,   236,   237,
     238,     0,     0,     0,     0,     0,   149,   239,     0,     0,
       0,     0,     0,     0,     0,   338,     0,     0,     0,     0,
       0,     0,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,     0,     0,     0,   174,
       0,     0,   175,   176,   177,   178,     0,   179,     0,     0,
     180,   181,   182,   183,   184,   185,     0,     0,     0,     0,
       0,   186,   187,   188,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   189,   190,   191,   192,   193,   194,     0,
       0,     0,   195,     0,   196,   197,   198,   199,   200,   201,
       0,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,     0,   225,   146,   147,   148,     0,
       0,     0,   226,   227,   228,   229,     0,     0,   230,   231,
     232,   233,     0,   234,   235,   236,   237,   238,     0,     0,
       0,     0,     0,   149,   239,     0,     0,     0,     0,     0,
       0,     0,   499,     0,     0,     0,     0,     0,     0,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,     0,     0,     0,   174,     0,     0,   175,
     176,   177,   178,     0,   179,     0,     0,   180,   181,   182,
     183,   184,   185,     0,     0,     0,     0,     0,   186,   187,
     188,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   190,   191,   192,   193,   194,     0,     0,     0,   195,
       0,   196,   197,   198,   199,   200,   201,     0,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,     0,   225,   146,   147,   148,     0,     0,     0,   226,
     227,   228,   229,     0,     0,   230,   231,   232,   233,     0,
     234,   235,   236,   237,   238,     0,     0,     0,     0,     0,
     149,   239,     0,     0,     0,     0,     0,     0,     0,   501,
       0,     0,     0,     0,     0,     0,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
       0,     0,     0,   174,     0,     0,   175,   176,   177,   178,
       0,   179,     0,     0,   180,   181,   182,   183,   184,   185,
       0,     0,     0,     0,     0,   186,   187,   188,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   189,   190,   191,
     192,   193,   194,     0,     0,     0,   195,     0,   196,   197,
     198,   199,   200,   201,     0,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,     0,   225,
     146,   147,   148,     0,     0,     0,   226,   227,   228,   229,
       0,     0,   230,   231,   232,   233,     0,   234,   235,   236,
     237,   238,     0,     0,     0,     0,     0,   149,   239,     0,
       0,     0,     0,     0,     0,     0,   503,     0,     0,     0,
       0,     0,     0,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,     0,     0,     0,
     174,     0,     0,   175,   176,   177,   178,     0,   179,     0,
       0,   180,   181,   182,   183,   184,   185,     0,     0,     0,
       0,     0,   186,   187,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   190,   191,   192,   193,   194,
       0,     0,     0,   195,     0,   196,   197,   198,   199,   200,
     201,     0,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,     0,   225,   146,   147,   148,
       0,     0,     0,   226,   227,   228,   229,     0,     0,   230,
     231,   232,   233,     0,   234,   235,   236,   237,   238,     0,
       0,     0,     0,     0,   149,   239,     0,     0,     0,     0,
       0,     0,     0,   505,     0,     0,     0,     0,     0,     0,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,     0,     0,     0,   174,     0,     0,
     175,   176,   177,   178,     0,   179,     0,     0,   180,   181,
     182,   183,   184,   185,     0,     0,     0,     0,     0,   186,
     187,   188,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,   190,   191,   192,   193,   194,     0,     0,     0,
     195,     0,   196,   197,   198,   199,   200,   201,     0,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,     0,   225,   146,   147,   148,     0,     0,     0,
     226,   227,   228,   229,     0,     0,   230,   231,   232,   233,
       0,   234,   235,   236,   507,   238,     0,     0,     0,     0,
       0,   149,   239,     0,     0,     0,     0,     0,     0,     0,
     265,     0,     0,     0,     0,     0,     0,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,     0,     0,     0,   174,     0,     0,   175,   176,   177,
     178,     0,   179,     0,     0,   180,   181,   182,   183,   184,
     185,     0,     0,     0,     0,     0,   186,   187,   188,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   190,
     191,   192,   193,   194,     0,     0,     0,   195,     0,   196,
     197,   198,   199,   200,   201,     0,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,     0,
     225,   146,   147,   148,     0,     0,     0,   226,   227,   228,
     229,     0,     0,   230,   231,   232,   233,     0,   234,   235,
     236,   935,   238,     0,     0,     0,     0,     0,   149,   239,
       0,     0,     0,     0,     0,     0,     0,   265,     0,     0,
       0,     0,     0,     0,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,     0,     0,
       0,   174,     0,     0,   175,   176,   177,   178,     0,   179,
       0,     0,   180,   181,   182,   183,   184,   185,     0,     0,
       0,     0,     0,   186,   187,   188,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   189,   190,   191,   192,   193,
     194,     0,     0,     0,   195,     0,   196,   197,   198,   199,
     200,   201,     0,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,     0,   225,     0,     0,
       0,     0,     0,     0,   226,   227,   228,   229,     0,     0,
     230,   231,   232,   233,     0,   234,   235,   236,   995,   238,
       0,     0,     0,     0,     0,     0,   239,     0,     0,     0,
       1,     2,     0,     0,   265,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,    20,    21,     0,     0,    22,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
       0,     0,    32,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    35,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,   445,   446,   447,     0,    37,
      38,    39,    40,    41,   445,   446,   447,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,     0,     0,
       0,     0,    51,    52,    53,   448,    54,     0,     0,     0,
       0,     0,     0,    55,   448,   449,   450,   451,   452,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,    57,
      58,    59,    60,    61,    62,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,     1,     2,    67,     0,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,     0,    15,
      16,    17,    18,    19,    20,    21,     0,     0,    22,    23,
       0,    24,    25,    26,    27,    28,    29,     0,    30,    31,
       0,     0,     0,    32,    33,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   453,
     454,   455,   456,   457,   458,   459,     0,   460,     0,     0,
       0,   684,   654,     0,    34,    35,     0,   445,   446,   447,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,   445,   446,   447,    42,    43,
      44,    45,    46,    47,    48,    49,    50,   448,   449,   450,
       0,     0,     0,    51,    52,    53,     0,    54,     0,   451,
     452,     0,     0,     0,    55,   448,   449,   450,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
      57,    58,    59,    60,    61,    62,     0,     0,     0,     0,
      63,    64,     0,     0,     0,     0,     0,     1,     2,     0,
      65,    66,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,     0,     0,     0,     0,
       0,    21,     0,     0,    22,    23,     0,    24,    25,    26,
      27,    28,    29,     0,   550,    31,     0,     0,     0,    32,
      33,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,     0,     0,     0,   803,     0,     0,   804,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,     0,
      34,    35,   806,     0,     0,   807,    36,     0,     0,     0,
       0,     0,   445,   446,   447,     0,    37,    38,    39,    40,
      41,   445,   446,   447,    42,    43,    44,    45,    46,    47,
      48,    49,    50,     0,     0,     0,     0,     0,     0,    51,
      52,    53,     0,    54,     0,     0,     0,     0,     0,     0,
      55,   448,   449,   450,   451,   452,     0,     0,   445,   446,
     447,     0,     0,   451,   452,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,    57,    58,    59,    60,
      61,    62,   445,   446,   447,     0,    63,    64,   448,   449,
     450,     0,     0,     0,     0,     0,    65,    66,     0,     0,
     451,   452,   445,   446,   447,     0,     0,     0,     0,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,     0,     0,     0,
       0,     0,   448,   449,   450,   445,   446,   447,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   448,   449,   450,   808,     0,
       0,   809,   445,   446,   447,     0,     0,   451,   452,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   448,   449,   450,     0,     0,     0,   913,     0,
       0,     0,     0,     0,   451,   452,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,     0,
       0,     0,   916,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,     0,     0,     0,   445,
     446,   447,   918,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   448,
     449,   450,     0,     0,     0,   919,   445,   446,   447,     0,
       0,   451,   452,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   445,   446,   447,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   448,   449,   450,     0,
       0,     0,   928,     0,     0,     0,     0,     0,   451,   452,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
     445,   446,   447,     0,   451,   452,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     448,   449,   450,   445,   446,   447,   929,     0,     0,     0,
       0,     0,   451,   452,   453,   454,   455,   456,   457,   458,
     459,     0,   460,     0,     0,     0,     0,     0,     0,   930,
       0,     0,     0,   448,   449,   450,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,     0,     0,     0,   931,   453,   454,   455,
     456,   457,   458,   459,     0,   460,     0,     0,     0,   686,
     654,     0,     0,     0,   445,   446,   447,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   448,   449,   450,   688,   654,     0,
       0,     0,   445,   446,   447,     0,   451,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
     693,   654,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,     0,     0,     0,
       0,     0,     0,     0,   445,   446,   447,   448,   449,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   448,   449,   450,   695,   654,   445,
     446,   447,     0,     0,     0,     0,   451,   452,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,     0,     0,
       0,   697,   654,     0,     0,     0,   445,   446,   447,   448,
     449,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   448,   449,   450,   700,
     654,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,     0,     0,     0,   703,   654,   445,   446,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,     0,     0,
       0,   706,   707,   445,   446,   447,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   448,   449,   450,   714,   654,   445,   446,
     447,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,   716,   654,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,     0,     0,     0,     0,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
       0,     0,     0,   719,   654,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
     725,   654,     0,     0,     0,   448,   449,   450,     0,     0,
       0,     0,   445,   446,   447,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,   727,   654,     0,     0,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,   729,   654,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,   731,   654,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,   734,   654,     0,     0,     0,     0,     0,   448,
     449,   450,     0,     0,   445,   446,   447,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,   737,
     654,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,   826,
     654,     0,     0,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,   827,   654,     0,     0,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,   828,   654,     0,     0,
       0,   448,   449,   450,     0,     0,     0,     0,   445,   446,
     447,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,   829,   654,     0,     0,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,     0,   654,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,   445,   446,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   445,   446,   447,     0,   753,
       0,     0,     0,   448,   449,   450,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   451,   452,     0,     0,   754,
       0,     0,     0,     0,     0,   448,   449,   450,     0,     0,
     445,   446,   447,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,     0,   756,     0,     0,     0,
     448,   449,   450,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   451,   452,     0,     0,   764,     0,     0,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
       0,   765,     0,     0,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,     0,   766,     0,     0,     0,   448,   449,   450,
       0,     0,     0,     0,   445,   446,   447,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   445,   446,   447,     0,   767,     0,
       0,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,     0,
     768,     0,     0,     0,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,   445,
     446,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,     0,   769,     0,     0,     0,   448,
     449,   450,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   451,   452,     0,     0,   770,     0,     0,     0,     0,
       0,   448,   449,   450,     0,     0,   445,   446,   447,     0,
       0,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,     0,   771,     0,     0,     0,   448,   449,   450,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   451,   452,
       0,     0,   772,     0,     0,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,   445,   446,   447,     0,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   445,   446,   447,     0,   773,     0,     0,
       0,     0,     0,   448,   449,   450,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   451,   452,     0,     0,   774,
       0,     0,     0,   448,   449,   450,     0,     0,     0,     0,
     445,   446,   447,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,     0,   775,     0,     0,     0,     0,     0,
     448,   449,   450,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   451,   452,     0,     0,   776,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
       0,   778,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
       0,   779,     0,     0,     0,     0,     0,   448,   449,   450,
       0,     0,   445,   446,   447,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   445,   446,   447,     0,   781,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,     0,   782,     0,
       0,     0,     0,     0,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,   445,
     446,   447,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,     0,   785,     0,     0,     0,     0,     0,   448,
     449,   450,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   451,   452,     0,     0,   786,     0,     0,     0,   448,
     449,   450,     0,     0,     0,     0,   445,   446,   447,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,     0,
     787,     0,     0,     0,     0,     0,   448,   449,   450,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   451,   452,
       0,     0,   788,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   445,   446,   447,     0,   789,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,     0,   790,     0,     0,
       0,     0,     0,   448,   449,   450,     0,     0,   445,   446,
     447,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,     0,   792,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,     0,   793,     0,     0,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   445,   446,   447,     0,   794,
       0,     0,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
       0,   872,     0,     0,     0,   448,   449,   450,     0,     0,
       0,     0,   445,   446,   447,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,     0,   883,     0,     0,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,     0,   893,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,     0,   912,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,     0,   920,     0,     0,     0,     0,     0,   448,
     449,   450,     0,     0,   445,   446,   447,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,     0,
     923,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,     0,
     924,     0,     0,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,     0,   927,     0,     0,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,     0,   942,     0,     0,
       0,   448,   449,   450,     0,     0,     0,     0,   445,   446,
     447,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,     0,   943,     0,     0,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,     0,   944,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,   445,   446,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   445,   446,   447,     0,   945,
       0,     0,     0,   448,   449,   450,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   451,   452,     0,     0,   963,
       0,     0,     0,     0,     0,   448,   449,   450,     0,     0,
     445,   446,   447,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,     0,   974,     0,     0,     0,
     448,   449,   450,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   451,   452,     0,     0,   975,     0,     0,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
       0,   977,     0,     0,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,     0,   985,     0,     0,     0,   448,   449,   450,
       0,     0,     0,     0,   445,   446,   447,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   445,   446,   447,     0,   988,     0,
       0,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,     0,
     991,     0,     0,     0,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,   445,
     446,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,     0,   992,     0,     0,     0,   448,
     449,   450,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   451,   452,     0,     0,  1006,     0,     0,     0,     0,
       0,   448,   449,   450,     0,     0,   445,   446,   447,     0,
       0,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,     0,  1011,     0,     0,     0,   448,   449,   450,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   451,   452,
       0,     0,  1012,     0,     0,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,   445,   446,   447,     0,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   445,   446,   447,     0,  1018,     0,     0,
       0,     0,     0,   448,   449,   450,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   451,   452,     0,     0,  1024,
       0,     0,     0,   448,   449,   450,     0,     0,     0,     0,
     445,   446,   447,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,   474,     0,     0,     0,     0,     0,     0,
     448,   449,   450,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   451,   452,     0,   476,     0,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
     478,     0,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
     483,     0,     0,     0,     0,     0,     0,   448,   449,   450,
       0,     0,   445,   446,   447,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   445,   446,   447,   485,     0,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,   487,     0,     0,
       0,     0,     0,     0,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,   445,
     446,   447,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,   491,     0,     0,     0,     0,     0,     0,   448,
     449,   450,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   451,   452,     0,   494,     0,     0,     0,     0,   448,
     449,   450,     0,     0,     0,     0,   445,   446,   447,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,   498,
       0,     0,     0,     0,     0,     0,   448,   449,   450,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   451,   452,
       0,   509,     0,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   445,   446,   447,   512,     0,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,   514,     0,     0,     0,
       0,     0,     0,   448,   449,   450,     0,     0,   445,   446,
     447,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,   520,     0,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,   522,     0,     0,     0,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   445,   446,   447,   524,     0,
       0,     0,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
     526,     0,     0,     0,     0,   448,   449,   450,     0,     0,
       0,     0,   445,   446,   447,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,   527,     0,     0,     0,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,   529,     0,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,   534,     0,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,   692,     0,     0,     0,     0,     0,     0,   448,
     449,   450,     0,     0,   445,   446,   447,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,   751,
       0,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,   752,
       0,     0,     0,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,   783,     0,     0,     0,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,   784,     0,     0,     0,
       0,   448,   449,   450,     0,     0,     0,     0,   445,   446,
     447,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,   812,     0,     0,     0,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,   814,     0,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,   445,   446,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   445,   446,   447,   817,     0,
       0,     0,     0,   448,   449,   450,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   451,   452,     0,   819,     0,
       0,     0,     0,     0,     0,   448,   449,   450,     0,     0,
     445,   446,   447,     0,     0,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,   825,     0,     0,     0,     0,
     448,   449,   450,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   451,   452,     0,   833,     0,     0,     0,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
     835,     0,     0,     0,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,   837,     0,     0,     0,     0,   448,   449,   450,
       0,     0,     0,     0,   445,   446,   447,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   445,   446,   447,   839,     0,     0,
       0,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,   843,
       0,     0,     0,     0,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,   445,
     446,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,   844,     0,     0,     0,     0,   448,
     449,   450,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   451,   452,     0,   846,     0,     0,     0,     0,     0,
       0,   448,   449,   450,     0,     0,   445,   446,   447,     0,
       0,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,   873,     0,     0,     0,     0,   448,   449,   450,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   451,   452,
       0,   875,     0,     0,     0,     0,     0,     0,   448,   449,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,   445,   446,   447,     0,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   445,   446,   447,   877,     0,     0,     0,
       0,     0,     0,   448,   449,   450,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   451,   452,     0,   881,     0,
       0,     0,     0,   448,   449,   450,     0,     0,     0,     0,
     445,   446,   447,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,   887,     0,     0,     0,     0,     0,     0,
     448,   449,   450,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   451,   452,     0,   895,     0,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   445,   446,   447,
     898,     0,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
     900,     0,     0,     0,     0,     0,     0,   448,   449,   450,
       0,     0,   445,   446,   447,     0,     0,     0,     0,   451,
     452,     0,     0,     0,     0,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   445,   446,   447,   902,     0,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,   905,     0,     0,
       0,     0,     0,     0,   448,   449,   450,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,   445,
     446,   447,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,   907,     0,     0,     0,     0,     0,     0,   448,
     449,   450,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   451,   452,     0,   910,     0,     0,     0,     0,   448,
     449,   450,     0,     0,     0,     0,   445,   446,   447,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,   921,
       0,     0,     0,     0,     0,     0,   448,   449,   450,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   451,   452,
       0,   922,     0,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   445,   446,   447,   926,     0,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,   934,     0,     0,     0,
       0,     0,     0,   448,   449,   450,     0,     0,   445,   446,
     447,     0,     0,     0,     0,   451,   452,     0,     0,     0,
       0,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     445,   446,   447,   937,     0,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,   939,     0,     0,     0,     0,     0,     0,
     448,   449,   450,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   452,     0,   445,   446,   447,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,   445,   446,   447,   953,     0,
       0,     0,     0,     0,     0,   448,   449,   450,   453,   454,
     455,   456,   457,   458,   459,     0,   460,   451,   452,     0,
     956,     0,     0,     0,     0,   448,   449,   450,     0,     0,
       0,     0,   445,   446,   447,     0,     0,   451,   452,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,   445,   446,   447,   964,     0,     0,     0,     0,
       0,     0,   448,   449,   450,   453,   454,   455,   456,   457,
     458,   459,     0,   460,   451,   452,     0,   967,     0,     0,
       0,     0,   448,   449,   450,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   452,     0,   445,   446,   447,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   445,
     446,   447,   971,     0,     0,     0,     0,   448,   449,   450,
     453,   454,   455,   456,   457,   458,   459,     0,   460,   451,
     452,     0,   981,     0,     0,     0,     0,     0,     0,   448,
     449,   450,     0,     0,   445,   446,   447,     0,     0,     0,
       0,   451,   452,     0,     0,     0,     0,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   445,   446,   447,   986,
       0,     0,     0,     0,   448,   449,   450,   453,   454,   455,
     456,   457,   458,   459,     0,   460,   451,   452,     0,   993,
       0,     0,     0,     0,     0,     0,   448,   449,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
       0,   445,   446,   447,     0,     0,     0,     0,     0,     0,
       0,     0,   453,   454,   455,   456,   457,   458,   459,     0,
     460,   445,   446,   447,   997,     0,     0,     0,     0,     0,
       0,   448,   449,   450,   453,   454,   455,   456,   457,   458,
     459,     0,   460,   451,   452,     0,  1002,     0,     0,     0,
       0,   448,   449,   450,     0,     0,     0,     0,   445,   446,
     447,     0,     0,   451,   452,     0,     0,     0,     0,   453,
     454,   455,   456,   457,   458,   459,     0,   460,   445,   446,
     447,  1004,     0,     0,     0,     0,     0,     0,   448,   449,
     450,   453,   454,   455,   456,   457,   458,   459,     0,   460,
     451,   452,     0,  1008,     0,     0,     0,     0,   448,   449,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     451,   452,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,     0,     0,     0,  1013,     0,
       0,     0,     0,     0,     0,     0,   453,   454,   455,   456,
     457,   458,   459,     0,   460,     0,     0,     0,  1016,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   453,   454,   455,   456,   457,   458,   459,
       0,   460
};

static const yytype_int16 yycheck[] =
{
       1,   188,     3,     4,     5,    35,     7,     8,   179,    10,
     172,   172,   173,    14,    19,   172,    17,   172,    23,    20,
     191,   172,    23,    24,    25,    26,    42,    43,    44,   172,
      31,   188,   194,    34,    35,    36,    37,   194,    39,   172,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     188,    84,    85,    54,   179,   189,    57,   188,   192,   188,
     186,   187,     5,     6,     7,   191,   191,   186,   187,   172,
     173,   188,   191,   172,    42,    43,    44,   186,   187,   186,
     187,     0,   191,   187,   191,   186,   187,   172,   173,    32,
     191,   189,   186,   187,   192,    20,    42,    43,    44,   190,
     191,   189,   186,   187,   186,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,   186,
     187,   191,    75,   186,   187,    78,    79,    80,    81,   188,
      83,   186,   187,    86,    87,    88,    89,    90,    91,   182,
     183,   188,   185,   188,    97,    98,    99,   188,   190,   191,
     188,   177,   178,   179,   188,   181,   109,   110,   111,   112,
     113,   114,   191,   174,   190,   118,   188,   120,   121,   122,
     123,   124,   125,   172,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   188,   151,   177,
     178,   179,   188,   181,   188,   158,   159,   160,   161,   190,
     191,   164,   165,   166,   167,   193,   169,   170,   171,   172,
     173,   177,   178,   179,   188,   181,   188,   180,   239,   240,
     188,   188,   195,   189,   245,   188,   189,   248,   188,   188,
     251,   188,   253,   196,   188,   188,   257,   188,   259,   188,
     261,   188,   263,   185,   265,   188,    42,    43,    44,   179,
     271,   188,   188,   274,   188,   188,   277,   188,   279,   188,
      42,    43,    44,   188,   285,   188,   287,   288,   289,   290,
      84,    85,   293,   188,   188,   296,   188,   188,   299,   188,
     301,   188,   303,   188,   188,   306,   188,   188,   309,   188,
     311,   188,   342,   314,   188,   316,   188,   188,   188,   188,
     321,   188,   323,   188,   188,   188,   327,   188,   188,   330,
     188,   188,   333,   188,   188,   336,   188,   338,   188,   188,
     188,   342,   343,   188,   345,   188,   188,   188,   188,   188,
     188,   188,   353,   354,   355,   356,   357,   188,   188,   188,
     188,   188,   188,   188,   365,   366,   188,   368,   369,   370,
     371,   372,   373,   374,   188,   376,   377,   378,   379,   188,
     188,   382,   383,   384,   385,   188,   180,   181,   182,   183,
     188,   185,   188,   394,   395,   396,   397,   398,   399,   400,
     401,   177,   178,   179,   188,   181,   188,   188,   191,   196,
     190,   190,   190,   189,   190,   177,   178,   179,   190,   181,
     190,   190,   190,    42,    43,    44,   190,   190,   190,   190,
     190,   190,   433,   434,   435,   190,   190,   190,   190,   440,
     441,   190,   190,   190,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
     190,   190,   190,   190,   190,   190,   190,   468,   190,   470,
     471,   190,   190,   474,   190,   476,   190,   478,   190,   190,
     190,   190,   483,   190,   485,   190,   487,   488,   190,   190,
     190,     5,     6,     7,   190,   190,   190,   498,   499,   190,
     501,   190,   503,   190,   505,   190,   190,   190,   509,   190,
     190,   181,   191,   514,   196,   516,   193,   179,    32,   520,
     195,   522,   179,   192,   192,   526,   527,   192,   529,   192,
     179,   179,   172,   534,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,   177,   178,
     179,    75,   181,   190,    78,    79,    80,    81,    70,    83,
     347,   190,    86,    87,    88,    89,    90,    91,   692,   350,
     851,    -1,   349,    97,    98,    99,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,   189,    -1,    -1,    -1,    -1,
      -1,    -1,   196,   684,    -1,   686,    -1,   688,    -1,    -1,
      -1,   692,   693,    -1,   695,    -1,   697,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   706,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   714,    42,    43,    44,    -1,   719,    -1,
     721,   177,   178,   179,   725,   181,   727,    -1,   729,    -1,
     731,    -1,    -1,   734,   190,    -1,   737,    -1,    -1,     1,
       2,    -1,   743,    -1,    -1,    -1,   747,    -1,   749,   750,
     751,   752,    42,    43,    44,    -1,    -1,   758,   759,   760,
      -1,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    40,    -1,
      -1,    -1,   783,   784,    -1,    -1,    -1,   817,    -1,    51,
      -1,    -1,    -1,    -1,    56,    -1,    58,   798,    60,    61,
      -1,    -1,   803,    -1,    -1,   806,    -1,   808,    -1,    -1,
      -1,   812,    -1,   814,    -1,    -1,   817,    -1,   819,    -1,
     821,    -1,    -1,    -1,   825,   826,   827,   828,   829,    -1,
      -1,    -1,   833,    -1,   835,   177,   178,   179,   839,   181,
      -1,    -1,   843,   844,    -1,   846,    -1,   189,   849,   177,
     178,   179,    -1,   181,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,   190,    -1,    -1,    -1,    -1,    28,    -1,    -1,
      -1,    -1,   873,    -1,   875,    -1,   877,    -1,    -1,    -1,
     881,    42,    43,    44,    -1,    -1,   887,   177,   178,   179,
      -1,   181,    -1,    -1,   895,    -1,    -1,   898,    -1,    -1,
     190,   902,    -1,    29,   905,    -1,   907,   937,    -1,   910,
      -1,    72,    73,    74,    -1,    -1,    42,    43,    44,    -1,
     921,   922,    -1,    84,    85,   926,    -1,    42,    43,    44,
      -1,    -1,    -1,   934,    -1,    -1,   937,    -1,   939,   940,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   950,
      -1,    -1,   953,    -1,    -1,   956,    -1,   958,    84,    85,
      -1,    -1,    -1,   964,    -1,    -1,   967,   997,    -1,    -1,
     971,    -1,    42,    43,    44,    -1,    -1,   978,   240,    -1,
     981,    -1,    -1,   245,    -1,   986,    -1,    -1,    -1,   177,
     178,   179,   993,   181,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,   190,  1004,    -1,    -1,    42,    43,    44,   271,
      -1,    -1,   274,    -1,    -1,  1016,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    72,    73,    74,    42,    43,
      44,    -1,    -1,    -1,   296,    -1,    -1,    84,    85,   301,
      -1,   303,    -1,    -1,   306,    -1,    -1,    42,    43,    44,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      -1,   323,   177,   178,   179,    -1,   181,    -1,   330,    -1,
      -1,   333,    -1,    -1,   336,   190,   338,    72,    73,    74,
      -1,    -1,   344,    42,    43,    44,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,   356,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,    -1,    -1,    -1,   177,   178,   179,
      -1,   181,    -1,    72,    73,    74,    42,    43,    44,   381,
     190,   383,    42,    43,    44,    84,    85,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    -1,
     402,   177,   178,   179,    -1,   181,   193,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,   190,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    -1,    -1,    -1,   189,   457,    -1,   192,    -1,    -1,
     462,   463,   464,   465,   466,   467,   468,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    -1,    -1,   491,
     189,    -1,   494,   192,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,
     512,   177,   178,   179,    -1,   181,    -1,   177,   178,   179,
      -1,   181,   524,    -1,   190,    -1,    -1,    -1,    32,    -1,
     190,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,   545,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,   177,   178,
     179,    75,   181,    -1,    78,    79,    80,    81,    -1,    83,
      -1,   190,    86,    87,    88,    89,    90,    91,    -1,    42,
      43,    44,    -1,    97,    98,    99,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    42,    43,    44,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
     177,   178,   179,    -1,   181,    -1,   180,    42,    43,    44,
      -1,    -1,    -1,   190,   188,    -1,    42,    43,    44,    -1,
     194,    -1,    -1,    42,    43,    44,    -1,    -1,   700,    -1,
      -1,   703,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    42,    43,    44,   716,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,   177,   178,   179,   729,   181,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,   190,    -1,    -1,
      -1,    -1,    -1,    -1,   190,    -1,   177,   178,   179,    -1,
     181,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,   190,
      -1,   763,    -1,    -1,    -1,     3,     4,    -1,    -1,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      -1,    -1,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,   177,   178,   179,   189,   181,    -1,   192,   177,   178,
     179,   833,   181,    -1,   190,   837,    -1,   839,    76,    77,
      -1,   190,    -1,    -1,    82,    -1,   177,   178,   179,   851,
     181,   853,    -1,    -1,    92,    93,    94,    95,    96,   190,
      -1,    -1,   100,   101,   102,   103,   104,   105,   106,   107,
     108,    42,    43,    44,    -1,    -1,    -1,   115,   116,   117,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
     177,   178,   179,   895,   181,    -1,    -1,    -1,   900,    -1,
     902,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
      -1,    42,    43,    44,   162,   163,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,   172,   173,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,    -1,
      -1,   953,    -1,    -1,    -1,    -1,    -1,   959,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
       6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
    1002,    -1,    -1,    42,    43,    44,   177,   178,   179,    -1,
     181,  1013,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      86,    87,    88,    89,    90,    91,   177,   178,   179,    -1,
     181,    97,    98,    99,    -1,    -1,   177,   178,   179,   190,
     181,    -1,    -1,   109,   110,   111,   112,   113,   114,   190,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,     5,     6,     7,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,   169,   170,   171,   172,   173,   177,   178,
     179,    -1,   181,    32,   180,    -1,    -1,    -1,    -1,    -1,
      -1,   190,   188,    -1,    -1,    -1,    -1,    -1,   194,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,   118,
      -1,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,   194,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    86,    87,    88,    89,    90,    91,
     177,   178,   179,    -1,   181,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,   190,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,    42,    43,    44,   118,    -1,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
       5,     6,     7,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,   194,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,   177,   178,
     179,    -1,   181,    -1,   109,   110,   111,   112,   113,   114,
      -1,   190,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,     5,     6,     7,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,   194,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,    -1,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,   190,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,   177,   178,   179,    -1,   181,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,   190,    -1,    -1,    -1,   109,   110,
     111,   112,   113,   114,    42,    43,    44,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,     5,     6,     7,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,   190,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,   177,
     178,   179,    -1,   181,    -1,   109,   110,   111,   112,   113,
     114,   189,    -1,    -1,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,     5,     6,
       7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,   190,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,   118,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,     5,     6,     7,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,   190,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,   177,   178,   179,    -1,   181,    97,    98,    99,
      -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    42,    43,    44,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
     190,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
     177,   178,   179,    -1,   181,    -1,   109,   110,   111,   112,
     113,   114,   189,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,     5,
       6,     7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,   190,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,     5,     6,     7,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    86,    87,    88,
      89,    90,    91,   177,   178,   179,    -1,   181,    97,    98,
      99,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   114,    42,    43,    44,   118,
      -1,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,
      -1,   177,   178,   179,    -1,   181,    -1,   109,   110,   111,
     112,   113,   114,   189,    -1,    -1,   118,    -1,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
       5,     6,     7,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,     5,     6,     7,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    86,    87,
      88,    89,    90,    91,   177,   178,   179,    -1,   181,    97,
      98,    99,    -1,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,    -1,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,     5,     6,     7,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,     5,     6,
       7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,   118,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,     5,     6,     7,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,     5,
       6,     7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,     5,     6,     7,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,   118,
      -1,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
       5,     6,     7,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,     5,     6,     7,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,    -1,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,     5,     6,     7,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,     5,     6,
       7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,   118,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,     5,     6,     7,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,     5,
       6,     7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,     5,     6,     7,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,   118,
      -1,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
       5,     6,     7,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,     5,     6,     7,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,    -1,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,     5,     6,     7,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,     5,     6,
       7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,   118,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,     5,     6,     7,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,
      -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,   169,
     170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,
     180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,     5,
       6,     7,    -1,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,    -1,    75,
      -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,     5,     6,     7,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,   169,   170,   171,   172,   173,    -1,    -1,
      -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,
      79,    80,    81,    -1,    83,    -1,    -1,    86,    87,    88,
      89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     109,   110,   111,   112,   113,   114,    -1,    -1,    -1,   118,
      -1,   120,   121,   122,   123,   124,   125,    -1,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
      32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,
      -1,    83,    -1,    -1,    86,    87,    88,    89,    90,    91,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
       5,     6,     7,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,   169,   170,   171,
     172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,     5,     6,     7,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,    -1,
      -1,    -1,    -1,    -1,    32,   180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,    -1,
      78,    79,    80,    81,    -1,    83,    -1,    -1,    86,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   109,   110,   111,   112,   113,   114,    -1,    -1,    -1,
     118,    -1,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,     5,     6,     7,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,   169,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,    32,   180,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,     5,     6,     7,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,    -1,    -1,    -1,    -1,    -1,    32,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   188,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,    83,
      -1,    -1,    86,    87,    88,    89,    90,    91,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,
     114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,
     124,   125,    -1,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,   169,   170,   171,   172,   173,
      -1,    -1,    -1,    -1,    -1,    -1,   180,    -1,    -1,    -1,
       3,     4,    -1,    -1,   188,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    31,    -1,
      33,    34,    35,    36,    37,    38,    -1,    40,    41,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    92,
      93,    94,    95,    96,    42,    43,    44,   100,   101,   102,
     103,   104,   105,   106,   107,   108,    -1,    -1,    -1,    -1,
      -1,    -1,   115,   116,   117,    72,   119,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    72,    73,    74,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
     173,     3,     4,   176,    -1,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      22,    23,    24,    25,    26,    27,    -1,    -1,    30,    31,
      -1,    33,    34,    35,    36,    37,    38,    -1,    40,    41,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    -1,    -1,
      -1,   189,   190,    -1,    76,    77,    -1,    42,    43,    44,
      82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      92,    93,    94,    95,    96,    42,    43,    44,   100,   101,
     102,   103,   104,   105,   106,   107,   108,    72,    73,    74,
      -1,    -1,    -1,   115,   116,   117,    -1,   119,    -1,    84,
      85,    -1,    -1,    -1,   126,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,
     152,   153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,
     162,   163,    -1,    -1,    -1,    -1,    -1,     3,     4,    -1,
     172,   173,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    -1,    -1,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,
      46,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    -1,    -1,    -1,   189,    -1,    -1,   192,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    -1,
      76,    77,   189,    -1,    -1,   192,    82,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    92,    93,    94,    95,
      96,    42,    43,    44,   100,   101,   102,   103,   104,   105,
     106,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,   115,
     116,   117,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    72,    73,    74,    84,    85,    -1,    -1,    42,    43,
      44,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,    42,    43,    44,    -1,   162,   163,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,   172,   173,    -1,    -1,
      84,    85,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    72,    73,    74,   189,    -1,
      -1,   192,    42,    43,    44,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    72,    73,    74,    -1,    -1,    -1,   192,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,    -1,
      -1,    -1,   192,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    -1,    -1,    -1,    42,
      43,    44,   192,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    72,
      73,    74,    -1,    -1,    -1,   192,    42,    43,    44,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    72,    73,    74,    -1,
      -1,    -1,   192,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      72,    73,    74,    42,    43,    44,   192,    -1,    -1,    -1,
      -1,    -1,    84,    85,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,   192,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,    -1,    -1,    -1,   192,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    -1,    -1,    -1,   189,
     190,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    72,    73,    74,   189,   190,    -1,
      -1,    -1,    42,    43,    44,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
     189,   190,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    44,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    72,    73,    74,   189,   190,    42,
      43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    -1,    -1,
      -1,   189,   190,    -1,    -1,    -1,    42,    43,    44,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    72,    73,    74,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    -1,    -1,    -1,   189,   190,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    -1,    -1,
      -1,   189,   190,    42,    43,    44,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    72,    73,    74,   189,   190,    42,    43,
      44,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,   189,   190,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      -1,    -1,    -1,   189,   190,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
     189,   190,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,   189,   190,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,   189,   190,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,   189,   190,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,   189,   190,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,   189,
     190,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,   189,
     190,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,   189,   190,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,   189,   190,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,   189,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,    -1,   190,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    42,    43,    44,    -1,   190,
      -1,    -1,    -1,    72,    73,    74,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    84,    85,    -1,    -1,   190,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,    -1,   190,    -1,    -1,    -1,
      72,    73,    74,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    84,    85,    -1,    -1,   190,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,    -1,   190,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    42,    43,    44,    -1,   190,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,    -1,
     190,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,    -1,   190,    -1,    -1,    -1,    72,
      73,    74,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    84,    85,    -1,    -1,   190,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,    -1,   190,    -1,    -1,    -1,    72,    73,    74,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    84,    85,
      -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    42,    43,    44,    -1,   190,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    84,    85,    -1,    -1,   190,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,    -1,   190,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    84,    85,    -1,    -1,   190,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
      -1,   190,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    42,    43,    44,    -1,   190,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,    -1,   190,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,    -1,   190,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    84,    85,    -1,    -1,   190,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,    -1,
     190,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    84,    85,
      -1,    -1,   190,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    42,    43,    44,    -1,   190,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,    -1,   190,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,    -1,   190,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    42,    43,    44,    -1,   190,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
      -1,   190,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,    -1,   190,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,    -1,   190,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,    -1,   190,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,    -1,
     190,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,    -1,
     190,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,    -1,   190,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,    -1,   190,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,    -1,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,    -1,   190,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    42,    43,    44,    -1,   190,
      -1,    -1,    -1,    72,    73,    74,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    84,    85,    -1,    -1,   190,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,    -1,   190,    -1,    -1,    -1,
      72,    73,    74,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    84,    85,    -1,    -1,   190,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
      -1,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,    -1,   190,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    42,    43,    44,    -1,   190,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,    -1,
     190,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,    -1,   190,    -1,    -1,    -1,    72,
      73,    74,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    84,    85,    -1,    -1,   190,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,    -1,   190,    -1,    -1,    -1,    72,    73,    74,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    84,    85,
      -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    42,    43,    44,    -1,   190,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    84,    85,    -1,    -1,   190,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
     189,    -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    42,    43,    44,   189,    -1,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    84,    85,    -1,   189,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    84,    85,
      -1,   189,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,   189,    -1,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    42,    43,    44,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
     189,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,   189,    -1,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,   189,
      -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,   189,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    42,    43,    44,   189,    -1,
      -1,    -1,    -1,    72,    73,    74,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    84,    85,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,
      72,    73,    74,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,   189,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    42,    43,    44,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,   189,
      -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,    72,
      73,    74,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    84,    85,    -1,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,   189,    -1,    -1,    -1,    -1,    72,    73,    74,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    84,    85,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    84,    85,    -1,   189,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    42,    43,    44,
     189,    -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    42,    43,    44,   189,    -1,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    84,    85,    -1,   189,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    84,    85,
      -1,   189,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      42,    43,    44,   189,    -1,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    42,    43,    44,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,
     179,   180,   181,   182,   183,    -1,   185,    84,    85,    -1,
     189,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    42,
      43,    44,   189,    -1,    -1,    -1,    -1,    72,    73,    74,
     177,   178,   179,   180,   181,   182,   183,    -1,   185,    84,
      85,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    42,    43,    44,   189,
      -1,    -1,    -1,    -1,    72,    73,    74,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    84,    85,    -1,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   178,   179,   180,   181,   182,   183,    -1,
     185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   177,   178,   179,   180,   181,   182,
     183,    -1,   185,    84,    85,    -1,   189,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    42,    43,
      44,   189,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      84,    85,    -1,   189,    -1,    -1,    -1,    -1,    72,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,   178,   179,   180,
     181,   182,   183,    -1,   185,    -1,    -1,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   178,   179,   180,   181,   182,   183,
      -1,   185
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    21,    22,    23,    24,    25,
      26,    27,    30,    31,    33,    34,    35,    36,    37,    38,
      40,    41,    45,    46,    76,    77,    82,    92,    93,    94,
      95,    96,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   115,   116,   117,   119,   126,   150,   152,   153,   154,
     155,   156,   157,   162,   163,   172,   173,   176,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,     5,     6,     7,    32,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    75,    78,    79,    80,    81,    83,
      86,    87,    88,    89,    90,    91,    97,    98,    99,   109,
     110,   111,   112,   113,   114,   118,   120,   121,   122,   123,
     124,   125,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   151,   158,   159,   160,   161,
     164,   165,   166,   167,   169,   170,   171,   172,   173,   180,
     188,   279,   281,   172,   173,   188,   279,   281,   188,   279,
     281,   188,   279,   188,   279,   172,   188,   194,   277,   188,
     279,   188,   279,   188,   279,   188,   279,   279,   279,   172,
     172,   188,   279,   281,   188,   279,   281,   188,   279,   188,
     279,   172,   172,    19,    23,   188,   279,   172,   173,   172,
     173,   279,   172,   188,   277,   279,   188,   279,   281,   188,
     279,   188,   281,   188,   279,   281,   188,   281,   279,   188,
     279,   188,   279,   279,   188,   279,   188,   279,   279,   279,
     279,   188,   279,   188,   281,   188,   188,   188,   279,   188,
     188,   281,   279,   188,   281,   188,   188,   281,   188,   281,
     188,   172,   179,   191,   179,   191,     0,   186,   201,   187,
      20,   189,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     279,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     191,   191,   279,   279,   281,    42,    43,    44,    72,    73,
      74,    84,    85,   177,   178,   179,   180,   181,   182,   183,
     185,   193,    42,    43,    44,   177,   178,   179,   181,   193,
     191,   191,   281,   279,   189,   279,   189,   279,   189,   172,
     278,   279,   279,   189,   279,   189,   279,   189,   179,   279,
     281,   189,   279,   281,   189,   190,   190,   279,   189,   188,
     279,   188,   279,   188,   279,   188,   279,   172,   279,   189,
     279,   281,   189,   279,   189,   281,   189,   281,   281,   279,
     189,   279,   189,   279,   189,   279,   189,   189,   279,   189,
     281,   190,   190,   279,   189,   190,   281,   281,   190,   281,
     281,   190,   277,   279,   279,   194,   276,   281,   279,   198,
      40,   213,   212,   172,   173,   190,   279,   279,   190,   279,
     279,   281,   279,   281,   281,   281,   281,   281,   281,   281,
     279,   279,   190,   279,   279,   279,   279,   279,   279,   279,
     190,   279,   279,   279,   279,   281,   279,   279,   281,   190,
     279,   190,   279,   190,   190,   190,   190,   190,   190,   190,
     190,   279,   279,   279,   279,   279,   279,   279,   190,   279,
     281,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   279,   279,   279,   190,   190,   190,   190,   189,   196,
     279,   189,   196,   279,   190,   190,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   281,
     279,   279,   279,   281,   281,   281,   281,   281,   281,   279,
     281,   279,   279,   190,   189,   279,   189,   279,   189,   279,
     190,   195,   189,   189,   279,   189,   279,   189,   279,   279,
     189,   190,   281,   189,   190,   281,   189,   190,   279,   279,
     279,   279,   279,   190,   189,   279,   189,   190,   281,   189,
     279,   189,   279,   190,   190,   189,   279,   189,   279,   189,
     281,   189,   279,   279,   189,   279,   190,   189,   279,   190,
     190,   190,   190,   189,   192,   280,   281,   189,   192,   191,
     191,   189,   189,   190,   190,   190,   190,   190,   189,   189,
     189,   190,   190,   189,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   189,   189,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   196,   189,   192,   189,   192,
     196,   189,   192,   189,   192,   193,   189,   192,   189,   192,
     279,   279,   189,   279,   189,   278,   279,   189,   279,   189,
     279,    28,   281,   281,   279,   189,   189,   189,   189,   189,
     279,   281,   279,   189,   279,   189,   279,   189,   279,   189,
     279,   281,   279,   189,   189,   279,   189,   279,   279,   179,
     195,   189,   279,   179,   279,   279,   279,   279,   279,   279,
     279,   281,   279,   279,   192,   192,   279,   192,   192,   279,
     279,   279,   190,   189,   279,   189,   279,   189,   172,   277,
     279,   189,   279,   190,   279,   190,   190,   189,   279,   279,
     279,   279,   279,   190,   190,   189,   279,   281,   189,   279,
     189,   281,   189,   279,   281,   189,   190,   189,   279,   279,
     189,   279,   190,   192,   279,   280,   192,   281,   192,   192,
     190,   189,   189,   190,   190,   190,   189,   190,   192,   192,
     192,   192,   279,   279,   189,   172,   279,   189,   279,   189,
      29,   279,   190,   190,   190,   190,   279,   281,   279,   281,
     189,   279,   281,   189,   279,   279,   189,   279,   179,   179,
     279,   279,   279,   190,   189,   279,   190,   189,   172,   277,
     279,   189,   279,   279,   190,   190,   190,   190,   189,   190,
     279,   189,   190,   279,   281,   190,   189,   279,   190,   279,
     281,   190,   190,   189,   279,   172,   279,   189,   279,   279,
     279,   281,   189,   279,   189,   279,   190,   190,   189,   172,
     277,   190,   190,   189,   190,   281,   189,   279,   190,   172,
     281,   279,   190,   190,   190
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:

/* Line 1455 of yacc.c  */
#line 324 "basicParse.y"
    { labeltable[(yyvsp[(1) - (1)].number)] = byteOffset; lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 326 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 327 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 328 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 329 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 330 "basicParse.y"
    { 
			// push to iftable the byte location of the end of the last stmt (top of loop)
			iftable[numifs] = lastLineOffset;
			numifs++;
			lastLineOffset = byteOffset; 
			addIntOp(OP_CURRLINE, linenumber);
			;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 337 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 338 "basicParse.y"
    { 
			// push to iftable the byte location of the end of the last stmt (top of loop)
			iftable[numifs] = lastLineOffset;
			numifs++;
			lastLineOffset = byteOffset;
			addIntOp(OP_CURRLINE, linenumber);
			;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 345 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 346 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 347 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 351 "basicParse.y"
    { 
	  // if there is an if branch or jump on the iftable stack get where it is
	  // in the bytecode array and then put the current bytecode address there
	  // - so we can jump over code
	  if (numifs>0) 
	    { 
	      unsigned int *temp = NULL;
		  numifs--;
	      temp = (unsigned int *) (byteCode + iftable[numifs]);
	      *temp = byteOffset; 
	    } 
	;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 366 "basicParse.y"
    {
		// there is nothing to do with a multi line if (ifexp handles it)
	;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 372 "basicParse.y"
    { 
	    unsigned int elsegototemp = 0;
		// on else create a jump point to the endif
		addIntOp(OP_PUSHINT, 0);	// false - always jump before else to endif
		addOp(OP_BRANCH);
		elsegototemp = addInt(0);
		// resolve the false jump on the if to the current location
		if (numifs>0) 
			{ 
			    unsigned int *temp = NULL;
				numifs--;
				temp = (unsigned int *) (byteCode + iftable[numifs]);
				*temp = byteOffset; 
			} 
		// now add the elsegoto jump to the iftable
		iftable[numifs] = elsegototemp;
		numifs++;
	;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 396 "basicParse.y"
    { 
		// if there is an if branch or jump on the iftable stack get where it is
		// in the bytecode array and then put the current bytecode address there
		// - so we can jump over code
		if (numifs>0) 
			{ 
			    unsigned int *temp = NULL;
				numifs--;
				temp = (unsigned int *) (byteCode + iftable[numifs]);
				*temp = byteOffset; 
			} 
	;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 411 "basicParse.y"
    { 
		 // create temp 
	   //if true, don't branch. If false, go to next line do the loop.
	   addOp(OP_BRANCH);
	   // after branch add a placeholder for the final end of the loop
	   // it will be resolved in the endwhile statement, push the
	   // location of this location on the iftable
	   iftable[numifs] = addInt(0);
	   numifs++;
         ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 427 "basicParse.y"
    { 
		// there should be two bytecode locations.  the TOP is the
		// location to jump to at the top of the loopthe , TOP-1 is the location
		// the exit jump needs to be written back to jump point on WHILE
		if (numifs>1) 
			{ 
			    unsigned int *temp = NULL;
				addIntOp(OP_PUSHINT, 0);	// false - always jump back to the beginning
				addIntOp(OP_BRANCH, iftable[numifs-1]);
				// resolve the false jump on the while to the current location
				temp = (unsigned int *) (byteCode + iftable[numifs-2]);
				*temp = byteOffset; 
				numifs-=2;
			} 
	;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 445 "basicParse.y"
    { 
		 // need nothing done at top of a do
         ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 452 "basicParse.y"
    { 
		 // create temp 
	   //if If false, go to to the corresponding do.
		if (numifs>0) 
			{ 
				addIntOp(OP_BRANCH, iftable[numifs-1]);
				numifs--;
			} 
         ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 527 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 528 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 529 "basicParse.y"
    { addIntOp(OP_DIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 530 "basicParse.y"
    { addIntOp(OP_DIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 533 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 534 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 535 "basicParse.y"
    { addIntOp(OP_REDIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 536 "basicParse.y"
    { addIntOp(OP_REDIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 539 "basicParse.y"
    { addOp(OP_PAUSE); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 542 "basicParse.y"
    { addOp(OP_CLS); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 543 "basicParse.y"
    { addOp(OP_CLG); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 546 "basicParse.y"
    { addOp(OP_FASTGRAPHICS); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 549 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 550 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 553 "basicParse.y"
    { addOp(OP_REFRESH); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 556 "basicParse.y"
    { addOp(OP_END); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 560 "basicParse.y"
    { 
	   //if true, don't branch. If false, go to next line.
	   addOp(OP_BRANCH);
	   // after branch add a placeholder for the final end of the if
	   // it will be resolved in the if/else/endif statement, push the
	   // location of this location on the iftable
	   checkByteMem(sizeof(int));
	   iftable[numifs] = byteOffset;
	   numifs++;
	   byteOffset += sizeof(int);
         ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 573 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 574 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 575 "basicParse.y"
    { addInt2Op(OP_STRARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 578 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 579 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 580 "basicParse.y"
    { addInt2Op(OP_ARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 584 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 587 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 591 "basicParse.y"
    { 
	    addIntOp(OP_PUSHINT, 1); //step
	    addIntOp(OP_FOR, (yyvsp[(2) - (6)].number));
	  ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 596 "basicParse.y"
    { 
	    addIntOp(OP_FOR, (yyvsp[(2) - (8)].number));
	  ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 602 "basicParse.y"
    { addIntOp(OP_NEXT, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 605 "basicParse.y"
    { addIntOp(OP_GOTO, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 608 "basicParse.y"
    { addIntOp(OP_GOSUB, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 612 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OFFERROR); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 616 "basicParse.y"
    { addIntOp(OP_ONERROR, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 619 "basicParse.y"
    { addOp(OP_RETURN); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 622 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 623 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 624 "basicParse.y"
    { addOp(OP_SETCOLORINT); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 625 "basicParse.y"
    { addOp(OP_SETCOLORINT); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 628 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 629 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 630 "basicParse.y"
    { addIntOp(OP_SOUND_LIST, listlen); listlen=0; ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 631 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 632 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 635 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 636 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 639 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 640 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 644 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 645 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 648 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 649 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 652 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 653 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 654 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 655 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 658 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 659 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 662 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 663 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 664 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 667 "basicParse.y"
    { addOp(OP_SYSTEM); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 668 "basicParse.y"
    { addOp(OP_SYSTEM); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 671 "basicParse.y"
    { addOp(OP_VOLUME); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 674 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 675 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 676 "basicParse.y"
    { addIntOp(OP_POLY_LIST, listlen); listlen=0; ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 679 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(8) - (8)].number)); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 680 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(9) - (10)].number)); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 681 "basicParse.y"
    { addIntOp(OP_STAMP_S_LIST, listlen); listlen=0; ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 682 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(6) - (6)].number)); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 683 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(7) - (8)].number)); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 684 "basicParse.y"
    { addIntOp(OP_STAMP_LIST, listlen); listlen=0; ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 685 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(10) - (10)].number)); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 686 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(11) - (12)].number)); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 687 "basicParse.y"
    { addIntOp(OP_STAMP_SR_LIST, listlen); listlen=0; ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 690 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_OPEN); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 691 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_OPEN); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 692 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 693 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 696 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITE); ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 697 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITE); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 698 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 699 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 702 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITELINE); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 703 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITELINE); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 704 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 705 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 708 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 709 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 710 "basicParse.y"
    { addOp(OP_CLOSE); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 713 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 714 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 715 "basicParse.y"
    { addOp(OP_RESET); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 718 "basicParse.y"
    {addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP);addOp(OP_SEEK);  ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 719 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 720 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 723 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 724 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 725 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 726 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_ARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 727 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRINGASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 728 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 729 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 730 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_NUMASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 731 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 732 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 735 "basicParse.y"
    { addOp(OP_PRINT);  addOp(OP_INPUT); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 736 "basicParse.y"
    { addOp(OP_PRINT);  addOp(OP_INPUT); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 739 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_PRINTN); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 740 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 741 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 742 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 743 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 744 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 745 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 748 "basicParse.y"
    {addOp(OP_WAVPLAY);  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 749 "basicParse.y"
    { addOp(OP_WAVPLAY); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 752 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 753 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 756 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 757 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 760 "basicParse.y"
    {addOp(OP_PUTSLICE);  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 761 "basicParse.y"
    { addOp(OP_PUTSLICE); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 762 "basicParse.y"
    {addOp(OP_PUTSLICEMASK);  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 763 "basicParse.y"
    { addOp(OP_PUTSLICEMASK); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 765 "basicParse.y"
    {addOp(OP_IMGLOAD);  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 766 "basicParse.y"
    { addOp(OP_IMGLOAD); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 767 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 768 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 769 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 770 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 773 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEDIM); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 776 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD);  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 777 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 780 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE);  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 781 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 784 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE);  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 785 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 788 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE);  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 789 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 792 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEHIDE); ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 795 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESHOW); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 798 "basicParse.y"
    {addOp(OP_CLICKCLEAR);  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 799 "basicParse.y"
    { addOp(OP_CLICKCLEAR); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 802 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CHANGEDIR); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 803 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CHANGEDIR); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 806 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DECIMAL); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 809 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPEN); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 810 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPEN); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 813 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 814 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 817 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBEXECUTE); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 818 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBEXECUTE); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 821 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPENSET); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 822 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPENSET); ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 825 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 826 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 835 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 836 "basicParse.y"
    { listlen++; ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 839 "basicParse.y"
    { (yyval.floatnum) = (yyvsp[(2) - (3)].floatnum); ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 840 "basicParse.y"
    { addOp(OP_ADD); ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 841 "basicParse.y"
    { addOp(OP_SUB); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 842 "basicParse.y"
    { addOp(OP_MUL); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 843 "basicParse.y"
    { addOp(OP_MOD); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 844 "basicParse.y"
    { addOp(OP_INTDIV); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 845 "basicParse.y"
    { addOp(OP_DIV); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 846 "basicParse.y"
    { addOp(OP_EXP); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 847 "basicParse.y"
    { addOp(OP_NEGATE); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 848 "basicParse.y"
    {addOp(OP_AND); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 849 "basicParse.y"
    { addOp(OP_OR); ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 850 "basicParse.y"
    { addOp(OP_XOR); ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 851 "basicParse.y"
    { addOp(OP_NOT); ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 852 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 853 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 854 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 855 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 856 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 857 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 858 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 859 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 860 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 861 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 862 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 863 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 864 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum)); ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 865 "basicParse.y"
    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 866 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 867 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 868 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 869 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 870 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 871 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 872 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 873 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 875 "basicParse.y"
    { 
	     if ((yyvsp[(1) - (1)].number) < 0)
	       {
		 return -1;
	       }
	     else
	       {
		 addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
	       }
	   ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 885 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 886 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 887 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 888 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 889 "basicParse.y"
    { addOp(OP_LENGTH); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 890 "basicParse.y"
    { addOp(OP_ASC); ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 891 "basicParse.y"
    { addOp(OP_INSTR); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 892 "basicParse.y"
    { addOp(OP_CEIL); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 893 "basicParse.y"
    { addOp(OP_FLOOR); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 894 "basicParse.y"
    { addOp(OP_SIN); ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 895 "basicParse.y"
    { addOp(OP_COS); ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 896 "basicParse.y"
    { addOp(OP_TAN); ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 897 "basicParse.y"
    { addOp(OP_ASIN); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 898 "basicParse.y"
    { addOp(OP_ACOS); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 899 "basicParse.y"
    { addOp(OP_ATAN); ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 900 "basicParse.y"
    { addOp(OP_DEGREES); ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 901 "basicParse.y"
    { addOp(OP_RADIANS); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 902 "basicParse.y"
    { addOp(OP_LOG); ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 903 "basicParse.y"
    { addOp(OP_LOGTEN); ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 904 "basicParse.y"
    { addOp(OP_ABS); ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 905 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 906 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 907 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 908 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 909 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 910 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 911 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 912 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 913 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 914 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 915 "basicParse.y"
    { addOp(OP_EOF); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 916 "basicParse.y"
    { addOp(OP_EXISTS); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 917 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 918 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 919 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 920 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 921 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 922 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 923 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 924 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 925 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 926 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 927 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 928 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 929 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 930 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 931 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 932 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 933 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 934 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 935 "basicParse.y"
    { addOp(OP_SIZE); ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 936 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 937 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 938 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 939 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 940 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 941 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 942 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 943 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 944 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 945 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 946 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 947 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 948 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 949 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 950 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 951 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 952 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 953 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 954 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 955 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 956 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 957 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 958 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 959 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 960 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 961 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 962 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 963 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 964 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 965 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 966 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 967 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 968 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 969 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 970 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 971 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 972 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 973 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 974 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 975 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 976 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 977 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 978 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 979 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 980 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 981 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 982 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 983 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 984 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 985 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 986 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 987 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 988 "basicParse.y"
    { addOp(OP_PIXEL); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 989 "basicParse.y"
    { addOp(OP_RGB); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 990 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 991 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 992 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITECOLLIDE); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 993 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEX); ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 994 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEY); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 995 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEH); ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 996 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEW); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 997 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEV); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 998 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBROW); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 999 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBINT); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 1000 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBFLOAT); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 1001 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 1002 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 1003 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 1004 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 1007 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 1008 "basicParse.y"
    { listlen++; ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 1011 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1012 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1013 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1014 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1015 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1016 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1018 "basicParse.y"
    { 
	      if ((yyvsp[(1) - (1)].number) < 0)
		{
		  return -1;
		}
	      else
		{
		  addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	    ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1028 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1029 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1030 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1031 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1032 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1033 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1034 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1035 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1036 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1037 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1038 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1039 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1040 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1041 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1042 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1043 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1045 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1046 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1047 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1048 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;



/* Line 1455 of yacc.c  */
#line 7932 "basicParse.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1051 "basicParse.y"


int
yyerror(const char *msg) {
  errorcode = -1;
  if (yytext[0] == '\n') { linenumber--; }	// error happened on previous line
  return -1;
}


