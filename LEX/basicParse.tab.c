
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
     B256PRINT = 258,
     B256INPUT = 259,
     B256KEY = 260,
     B256PIXEL = 261,
     B256RGB = 262,
     B256PLOT = 263,
     B256CIRCLE = 264,
     B256RECT = 265,
     B256POLY = 266,
     B256STAMP = 267,
     B256LINE = 268,
     B256FASTGRAPHICS = 269,
     B256GRAPHSIZE = 270,
     B256REFRESH = 271,
     B256CLS = 272,
     B256CLG = 273,
     B256IF = 274,
     B256THEN = 275,
     B256ELSE = 276,
     B256ENDIF = 277,
     B256WHILE = 278,
     B256ENDWHILE = 279,
     B256DO = 280,
     B256UNTIL = 281,
     B256FOR = 282,
     B256TO = 283,
     B256STEP = 284,
     B256NEXT = 285,
     B256OPEN = 286,
     B256READ = 287,
     B256WRITE = 288,
     B256CLOSE = 289,
     B256RESET = 290,
     B256GOTO = 291,
     B256GOSUB = 292,
     B256RETURN = 293,
     B256REM = 294,
     B256END = 295,
     B256SETCOLOR = 296,
     B256GTE = 297,
     B256LTE = 298,
     B256NE = 299,
     B256DIM = 300,
     B256REDIM = 301,
     B256NOP = 302,
     B256TOINT = 303,
     B256TOSTRING = 304,
     B256LENGTH = 305,
     B256MID = 306,
     B256LEFT = 307,
     B256RIGHT = 308,
     B256UPPER = 309,
     B256LOWER = 310,
     B256INSTR = 311,
     B256CEIL = 312,
     B256FLOOR = 313,
     B256RAND = 314,
     B256SIN = 315,
     B256COS = 316,
     B256TAN = 317,
     B256ASIN = 318,
     B256ACOS = 319,
     B256ATAN = 320,
     B256ABS = 321,
     B256PI = 322,
     B256DEGREES = 323,
     B256RADIANS = 324,
     B256LOG = 325,
     B256LOGTEN = 326,
     B256AND = 327,
     B256OR = 328,
     B256XOR = 329,
     B256NOT = 330,
     B256PAUSE = 331,
     B256SOUND = 332,
     B256ASC = 333,
     B256CHR = 334,
     B256TOFLOAT = 335,
     B256READLINE = 336,
     B256WRITELINE = 337,
     B256BOOLEOF = 338,
     B256MOD = 339,
     B256INTDIV = 340,
     B256YEAR = 341,
     B256MONTH = 342,
     B256DAY = 343,
     B256HOUR = 344,
     B256MINUTE = 345,
     B256SECOND = 346,
     B256TEXT = 347,
     B256FONT = 348,
     B256SAY = 349,
     B256SYSTEM = 350,
     B256VOLUME = 351,
     B256GRAPHWIDTH = 352,
     B256GRAPHHEIGHT = 353,
     B256GETSLICE = 354,
     B256PUTSLICE = 355,
     B256IMGLOAD = 356,
     B256SPRITEDIM = 357,
     B256SPRITELOAD = 358,
     B256SPRITESLICE = 359,
     B256SPRITEMOVE = 360,
     B256SPRITEHIDE = 361,
     B256SPRITESHOW = 362,
     B256SPRITEPLACE = 363,
     B256SPRITECOLLIDE = 364,
     B256SPRITEX = 365,
     B256SPRITEY = 366,
     B256SPRITEH = 367,
     B256SPRITEW = 368,
     B256SPRITEV = 369,
     B256WAVPLAY = 370,
     B256WAVSTOP = 371,
     B256WAVWAIT = 372,
     B256SIZE = 373,
     B256SEEK = 374,
     B256EXISTS = 375,
     B256BOOLTRUE = 376,
     B256BOOLFALSE = 377,
     B256MOUSEX = 378,
     B256MOUSEY = 379,
     B256MOUSEB = 380,
     B256CLICKCLEAR = 381,
     B256CLICKX = 382,
     B256CLICKY = 383,
     B256CLICKB = 384,
     B256GETCOLOR = 385,
     B256CLEAR = 386,
     B256BLACK = 387,
     B256WHITE = 388,
     B256RED = 389,
     B256DARKRED = 390,
     B256GREEN = 391,
     B256DARKGREEN = 392,
     B256BLUE = 393,
     B256DARKBLUE = 394,
     B256CYAN = 395,
     B256DARKCYAN = 396,
     B256PURPLE = 397,
     B256DARKPURPLE = 398,
     B256YELLOW = 399,
     B256DARKYELLOW = 400,
     B256ORANGE = 401,
     B256DARKORANGE = 402,
     B256GREY = 403,
     B256DARKGREY = 404,
     B256CHANGEDIR = 405,
     B256CURRENTDIR = 406,
     B256DECIMAL = 407,
     B256DBOPEN = 408,
     B256DBCLOSE = 409,
     B256DBEXECUTE = 410,
     B256DBOPENSET = 411,
     B256DBCLOSESET = 412,
     B256DBROW = 413,
     B256DBINT = 414,
     B256DBFLOAT = 415,
     B256DBSTRING = 416,
     B256ONERROR = 417,
     B256OFFERROR = 418,
     B256LASTERROR = 419,
     B256LASTERRORMESSAGE = 420,
     B256LASTERRORLINE = 421,
     B256LASTERROREXTRA = 422,
     B256NETLISTEN = 423,
     B256NETCONNECT = 424,
     B256NETREAD = 425,
     B256NETWRITE = 426,
     B256NETCLOSE = 427,
     B256NETDATA = 428,
     B256NETADDRESS = 429,
     B256KILL = 430,
     B256MD5 = 431,
     B256SETSETTING = 432,
     B256GETSETTING = 433,
     B256PORTIN = 434,
     B256PORTOUT = 435,
     B256BINARYOR = 436,
     B256BINARYAND = 437,
     B256BINARYNOT = 438,
     B256IMGSAVE = 439,
     B256LINENUM = 440,
     B256INTEGER = 441,
     B256FLOAT = 442,
     B256STRING = 443,
     B256VARIABLE = 444,
     B256STRINGVAR = 445,
     B256NEWVAR = 446,
     B256COLOR = 447,
     B256LABEL = 448,
     B256UMINUS = 449
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 288 "basicParse.y"

  int number;
  double floatnum;
  char *string;



/* Line 214 of yacc.c  */
#line 544 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 556 "basicParse.tab.c"

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
#define YYFINAL  378
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   17677

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  214
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  93
/* YYNRULES -- Number of rules.  */
#define YYNRULES  467
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1114

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   449

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     203,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     205,   207,   199,   198,   206,   197,     2,   200,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   204,   210,
     194,   196,   195,   213,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   208,     2,   209,   202,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   211,     2,   212,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   201
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
     191,   193,   195,   197,   199,   201,   203,   205,   207,   209,
     213,   217,   225,   233,   237,   241,   249,   257,   260,   262,
     264,   266,   271,   278,   280,   282,   285,   292,   301,   305,
     312,   321,   325,   329,   333,   340,   349,   352,   355,   358,
     360,   363,   365,   372,   381,   384,   389,   392,   395,   402,
     407,   412,   419,   428,   439,   446,   455,   464,   475,   482,
     491,   498,   507,   514,   523,   526,   529,   532,   535,   538,
     543,   546,   555,   566,   575,   582,   591,   598,   609,   622,
     633,   636,   643,   648,   651,   658,   663,   666,   673,   678,
     680,   684,   687,   689,   693,   696,   699,   706,   711,   715,
     722,   726,   733,   736,   742,   750,   753,   759,   767,   770,
     772,   775,   778,   782,   786,   789,   791,   795,   797,   801,
     808,   817,   826,   837,   844,   853,   862,   873,   884,   897,
     900,   905,   912,   923,   936,   943,   952,   959,   968,   971,
     974,   976,   980,   983,   986,   989,   991,   995,   998,  1001,
    1003,  1007,  1010,  1017,  1022,  1027,  1034,  1041,  1050,  1053,
    1060,  1065,  1067,  1071,  1074,  1077,  1084,  1091,  1100,  1105,
    1112,  1115,  1122,  1127,  1131,  1135,  1137,  1141,  1145,  1149,
    1153,  1157,  1161,  1165,  1169,  1173,  1177,  1181,  1184,  1187,
    1191,  1195,  1199,  1202,  1206,  1210,  1214,  1218,  1222,  1226,
    1230,  1234,  1238,  1242,  1246,  1250,  1252,  1254,  1259,  1264,
    1270,  1276,  1282,  1288,  1293,  1300,  1302,  1307,  1312,  1317,
    1322,  1327,  1332,  1339,  1344,  1349,  1354,  1359,  1364,  1369,
    1374,  1379,  1384,  1389,  1394,  1399,  1404,  1406,  1410,  1412,
    1416,  1418,  1422,  1424,  1428,  1430,  1434,  1439,  1444,  1446,
    1450,  1452,  1456,  1458,  1462,  1464,  1468,  1470,  1474,  1476,
    1480,  1482,  1486,  1488,  1492,  1494,  1498,  1503,  1505,  1509,
    1511,  1515,  1517,  1521,  1523,  1527,  1529,  1533,  1535,  1539,
    1541,  1545,  1547,  1551,  1553,  1557,  1559,  1563,  1565,  1569,
    1571,  1575,  1577,  1581,  1583,  1587,  1589,  1593,  1595,  1599,
    1601,  1605,  1607,  1611,  1613,  1617,  1619,  1623,  1625,  1629,
    1631,  1635,  1637,  1641,  1643,  1647,  1649,  1653,  1655,  1659,
    1666,  1675,  1677,  1681,  1688,  1693,  1698,  1703,  1708,  1713,
    1717,  1722,  1727,  1729,  1733,  1735,  1739,  1741,  1745,  1750,
    1755,  1757,  1761,  1765,  1769,  1773,  1777,  1779,  1784,  1791,
    1793,  1798,  1803,  1808,  1813,  1822,  1829,  1836,  1847,  1849,
    1853,  1858,  1860,  1864,  1869,  1871,  1875,  1880,  1882,  1886,
    1888,  1892,  1894,  1898,  1903,  1905,  1909,  1914
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     215,     0,    -1,   216,   203,    -1,   216,   203,   215,    -1,
     217,   218,    -1,   218,    -1,   193,    -1,   219,    -1,   220,
      -1,   221,    -1,   223,    -1,   224,    -1,   226,    -1,   227,
      -1,   228,    -1,   229,    -1,    -1,   239,    20,   229,    -1,
     239,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     222,    -1,    23,   304,    -1,    24,    -1,    40,    23,    -1,
     225,    -1,    25,    -1,    26,   304,    -1,   230,    -1,   229,
     204,   230,    -1,   246,    -1,   247,    -1,   248,    -1,   249,
      -1,   250,    -1,   272,    -1,   253,    -1,   255,    -1,   256,
      -1,   262,    -1,   263,    -1,   254,    -1,   242,    -1,   243,
      -1,   244,    -1,   245,    -1,   251,    -1,   270,    -1,   238,
      -1,   234,    -1,   237,    -1,   235,    -1,   236,    -1,   231,
      -1,   232,    -1,   233,    -1,   241,    -1,   240,    -1,   264,
      -1,   265,    -1,   266,    -1,   267,    -1,   268,    -1,   297,
      -1,   252,    -1,   257,    -1,   258,    -1,   259,    -1,   260,
      -1,   261,    -1,   273,    -1,   274,    -1,   275,    -1,   276,
      -1,   277,    -1,   278,    -1,   279,    -1,   280,    -1,   281,
      -1,   282,    -1,   283,    -1,   284,    -1,   269,    -1,   285,
      -1,   286,    -1,   287,    -1,   288,    -1,   289,    -1,   290,
      -1,   291,    -1,   292,    -1,   293,    -1,   294,    -1,   295,
      -1,   296,    -1,   298,    -1,   299,    -1,   300,    -1,    45,
     189,   304,    -1,    45,   190,   304,    -1,    45,   189,   205,
     304,   206,   304,   207,    -1,    45,   190,   205,   304,   206,
     304,   207,    -1,    46,   189,   304,    -1,    46,   190,   304,
      -1,    46,   189,   205,   304,   206,   304,   207,    -1,    46,
     190,   205,   304,   206,   304,   207,    -1,    76,   304,    -1,
      17,    -1,    18,    -1,    14,    -1,    15,   304,   206,   304,
      -1,    15,   205,   304,   206,   304,   207,    -1,    16,    -1,
      40,    -1,    19,   304,    -1,   190,   208,   304,   209,   196,
     306,    -1,   190,   208,   304,   206,   304,   209,   196,   306,
      -1,   190,   196,   301,    -1,   189,   208,   304,   209,   196,
     304,    -1,   189,   208,   304,   206,   304,   209,   196,   304,
      -1,   189,   196,   302,    -1,   189,   196,   304,    -1,   190,
     196,   306,    -1,    27,   189,   196,   304,    28,   304,    -1,
      27,   189,   196,   304,    28,   304,    29,   304,    -1,    30,
     189,    -1,    36,   189,    -1,    37,   189,    -1,   163,    -1,
     162,   189,    -1,    38,    -1,    41,   304,   206,   304,   206,
     304,    -1,    41,   205,   304,   206,   304,   206,   304,   207,
      -1,    41,   304,    -1,    77,   205,   189,   207,    -1,    77,
     189,    -1,    77,   302,    -1,    77,   205,   304,   206,   304,
     207,    -1,    77,   304,   206,   304,    -1,     8,   304,   206,
     304,    -1,     8,   205,   304,   206,   304,   207,    -1,    13,
     304,   206,   304,   206,   304,   206,   304,    -1,    13,   205,
     304,   206,   304,   206,   304,   206,   304,   207,    -1,     9,
     304,   206,   304,   206,   304,    -1,     9,   205,   304,   206,
     304,   206,   304,   207,    -1,    10,   304,   206,   304,   206,
     304,   206,   304,    -1,    10,   205,   304,   206,   304,   206,
     304,   206,   304,   207,    -1,    92,   304,   206,   304,   206,
     306,    -1,    92,   205,   304,   206,   304,   206,   306,   207,
      -1,    92,   304,   206,   304,   206,   304,    -1,    92,   205,
     304,   206,   304,   206,   304,   207,    -1,    93,   306,   206,
     304,   206,   304,    -1,    93,   205,   306,   206,   304,   206,
     304,   207,    -1,    94,   306,    -1,    94,   304,    -1,    95,
     306,    -1,    96,   304,    -1,    11,   189,    -1,    11,   205,
     189,   207,    -1,    11,   302,    -1,    12,   304,   206,   304,
     206,   304,   206,   189,    -1,    12,   205,   304,   206,   304,
     206,   304,   206,   189,   207,    -1,    12,   304,   206,   304,
     206,   304,   206,   302,    -1,    12,   304,   206,   304,   206,
     189,    -1,    12,   205,   304,   206,   304,   206,   189,   207,
      -1,    12,   304,   206,   304,   206,   302,    -1,    12,   304,
     206,   304,   206,   304,   206,   304,   206,   189,    -1,    12,
     205,   304,   206,   304,   206,   304,   206,   304,   206,   189,
     207,    -1,    12,   304,   206,   304,   206,   304,   206,   304,
     206,   302,    -1,    31,   306,    -1,    31,   205,   304,   206,
     306,   207,    -1,    31,   304,   206,   306,    -1,    33,   306,
      -1,    33,   205,   304,   206,   306,   207,    -1,    33,   304,
     206,   306,    -1,    82,   306,    -1,    82,   205,   304,   206,
     306,   207,    -1,    82,   304,   206,   306,    -1,    34,    -1,
      34,   205,   207,    -1,    34,   304,    -1,    35,    -1,    35,
     205,   207,    -1,    35,   304,    -1,   119,   304,    -1,   119,
     205,   304,   206,   304,   207,    -1,   119,   304,   206,   304,
      -1,   271,   206,   190,    -1,   271,   206,   190,   208,   304,
     209,    -1,   271,   206,   189,    -1,   271,   206,   189,   208,
     304,   209,    -1,     4,   190,    -1,     4,   190,   208,   304,
     209,    -1,     4,   190,   208,   304,   206,   304,   209,    -1,
       4,   189,    -1,     4,   189,   208,   304,   209,    -1,     4,
     189,   208,   304,   206,   304,   209,    -1,     4,   306,    -1,
       3,    -1,     3,   306,    -1,     3,   304,    -1,     3,   306,
     210,    -1,     3,   304,   210,    -1,   115,   306,    -1,   116,
      -1,   116,   205,   207,    -1,   117,    -1,   117,   205,   207,
      -1,   100,   304,   206,   304,   206,   306,    -1,   100,   205,
     304,   206,   304,   206,   306,   207,    -1,   100,   304,   206,
     304,   206,   306,   206,   304,    -1,   100,   205,   304,   206,
     304,   206,   306,   206,   304,   207,    -1,   101,   304,   206,
     304,   206,   306,    -1,   101,   205,   304,   206,   304,   206,
     306,   207,    -1,   101,   304,   206,   304,   206,   304,   206,
     306,    -1,   101,   205,   304,   206,   304,   206,   304,   206,
     306,   207,    -1,   101,   304,   206,   304,   206,   304,   206,
     304,   206,   306,    -1,   101,   205,   304,   206,   304,   206,
     304,   206,   304,   206,   306,   207,    -1,   102,   304,    -1,
     103,   304,   206,   306,    -1,   103,   205,   304,   206,   306,
     207,    -1,   104,   304,   206,   304,   206,   304,   206,   304,
     206,   304,    -1,   104,   205,   304,   206,   304,   206,   304,
     206,   304,   206,   304,   207,    -1,   108,   304,   206,   304,
     206,   304,    -1,   108,   205,   304,   206,   304,   206,   304,
     207,    -1,   105,   304,   206,   304,   206,   304,    -1,   103,
     205,   304,   206,   304,   206,   304,   207,    -1,   106,   304,
      -1,   107,   304,    -1,   126,    -1,   126,   205,   207,    -1,
     150,   306,    -1,   152,   304,    -1,   153,   306,    -1,   154,
      -1,   154,   205,   207,    -1,   155,   306,    -1,   156,   306,
      -1,   157,    -1,   157,   205,   207,    -1,   168,   304,    -1,
     168,   205,   304,   206,   304,   207,    -1,   168,   304,   206,
     304,    -1,   169,   306,   206,   304,    -1,   169,   205,   306,
     206,   304,   207,    -1,   169,   304,   206,   306,   206,   304,
      -1,   169,   205,   304,   206,   306,   206,   304,   207,    -1,
     171,   306,    -1,   171,   205,   304,   206,   306,   207,    -1,
     171,   304,   206,   306,    -1,   172,    -1,   172,   205,   207,
      -1,   172,   304,    -1,   175,   306,    -1,   175,   205,   304,
     206,   306,   207,    -1,   177,   306,   206,   306,   206,   306,
      -1,   177,   205,   306,   206,   306,   206,   306,   207,    -1,
     180,   304,   206,   304,    -1,   180,   205,   304,   206,   304,
     207,    -1,   184,   306,    -1,   184,   205,   306,   206,   306,
     207,    -1,   184,   306,   206,   306,    -1,   211,   305,   212,
      -1,   211,   303,   212,    -1,   304,    -1,   304,   206,   303,
      -1,   205,   304,   207,    -1,   304,   198,   304,    -1,   304,
     197,   304,    -1,   304,   199,   304,    -1,   304,    84,   304,
      -1,   304,    85,   304,    -1,   304,   200,   304,    -1,   304,
     202,   304,    -1,   304,   181,   304,    -1,   304,   182,   304,
      -1,   183,   304,    -1,   197,   304,    -1,   304,    72,   304,
      -1,   304,    73,   304,    -1,   304,    74,   304,    -1,    75,
     304,    -1,   306,   196,   306,    -1,   306,    44,   306,    -1,
     306,   194,   306,    -1,   306,   195,   306,    -1,   306,    42,
     306,    -1,   306,    43,   306,    -1,   304,   196,   304,    -1,
     304,    44,   304,    -1,   304,   194,   304,    -1,   304,   195,
     304,    -1,   304,    42,   304,    -1,   304,    43,   304,    -1,
     187,    -1,   186,    -1,   189,   208,   213,   209,    -1,   190,
     208,   213,   209,    -1,   189,   208,   213,   206,   209,    -1,
     190,   208,   213,   206,   209,    -1,   189,   208,   206,   213,
     209,    -1,   190,   208,   206,   213,   209,    -1,   189,   208,
     304,   209,    -1,   189,   208,   304,   206,   304,   209,    -1,
     189,    -1,    48,   205,   304,   207,    -1,    48,   205,   306,
     207,    -1,    80,   205,   304,   207,    -1,    80,   205,   306,
     207,    -1,    50,   205,   306,   207,    -1,    78,   205,   306,
     207,    -1,    56,   205,   306,   206,   306,   207,    -1,    57,
     205,   304,   207,    -1,    58,   205,   304,   207,    -1,    60,
     205,   304,   207,    -1,    61,   205,   304,   207,    -1,    62,
     205,   304,   207,    -1,    63,   205,   304,   207,    -1,    64,
     205,   304,   207,    -1,    65,   205,   304,   207,    -1,    68,
     205,   304,   207,    -1,    69,   205,   304,   207,    -1,    70,
     205,   304,   207,    -1,    71,   205,   304,   207,    -1,    66,
     205,   304,   207,    -1,    59,    -1,    59,   205,   207,    -1,
      67,    -1,    67,   205,   207,    -1,   121,    -1,   121,   205,
     207,    -1,   122,    -1,   122,   205,   207,    -1,    83,    -1,
      83,   205,   207,    -1,    83,   205,   304,   207,    -1,   120,
     205,   306,   207,    -1,    86,    -1,    86,   205,   207,    -1,
      87,    -1,    87,   205,   207,    -1,    88,    -1,    88,   205,
     207,    -1,    89,    -1,    89,   205,   207,    -1,    90,    -1,
      90,   205,   207,    -1,    91,    -1,    91,   205,   207,    -1,
      97,    -1,    97,   205,   207,    -1,    98,    -1,    98,   205,
     207,    -1,   118,    -1,   118,   205,   207,    -1,   118,   205,
     304,   207,    -1,     5,    -1,     5,   205,   207,    -1,   123,
      -1,   123,   205,   207,    -1,   124,    -1,   124,   205,   207,
      -1,   125,    -1,   125,   205,   207,    -1,   127,    -1,   127,
     205,   207,    -1,   128,    -1,   128,   205,   207,    -1,   129,
      -1,   129,   205,   207,    -1,   131,    -1,   131,   205,   207,
      -1,   132,    -1,   132,   205,   207,    -1,   133,    -1,   133,
     205,   207,    -1,   134,    -1,   134,   205,   207,    -1,   135,
      -1,   135,   205,   207,    -1,   136,    -1,   136,   205,   207,
      -1,   137,    -1,   137,   205,   207,    -1,   138,    -1,   138,
     205,   207,    -1,   139,    -1,   139,   205,   207,    -1,   140,
      -1,   140,   205,   207,    -1,   141,    -1,   141,   205,   207,
      -1,   142,    -1,   142,   205,   207,    -1,   143,    -1,   143,
     205,   207,    -1,   144,    -1,   144,   205,   207,    -1,   145,
      -1,   145,   205,   207,    -1,   146,    -1,   146,   205,   207,
      -1,   147,    -1,   147,   205,   207,    -1,   148,    -1,   148,
     205,   207,    -1,   149,    -1,   149,   205,   207,    -1,     6,
     205,   304,   206,   304,   207,    -1,     7,   205,   304,   206,
     304,   206,   304,   207,    -1,   130,    -1,   130,   205,   207,
      -1,   109,   205,   304,   206,   304,   207,    -1,   110,   205,
     304,   207,    -1,   111,   205,   304,   207,    -1,   112,   205,
     304,   207,    -1,   113,   205,   304,   207,    -1,   114,   205,
     304,   207,    -1,   158,   205,   207,    -1,   159,   205,   304,
     207,    -1,   160,   205,   304,   207,    -1,   164,    -1,   164,
     205,   207,    -1,   166,    -1,   166,   205,   207,    -1,   173,
      -1,   173,   205,   207,    -1,   173,   205,   304,   207,    -1,
     179,   205,   304,   207,    -1,   306,    -1,   306,   206,   305,
      -1,   205,   306,   207,    -1,   306,   198,   306,    -1,   304,
     198,   306,    -1,   306,   198,   304,    -1,   188,    -1,   190,
     208,   304,   209,    -1,   190,   208,   304,   206,   304,   209,
      -1,   190,    -1,    79,   205,   304,   207,    -1,    49,   205,
     304,   207,    -1,    54,   205,   306,   207,    -1,    55,   205,
     306,   207,    -1,    51,   205,   306,   206,   304,   206,   304,
     207,    -1,    52,   205,   306,   206,   304,   207,    -1,    53,
     205,   306,   206,   304,   207,    -1,    99,   205,   304,   206,
     304,   206,   304,   206,   304,   207,    -1,    32,    -1,    32,
     205,   207,    -1,    32,   205,   304,   207,    -1,    81,    -1,
      81,   205,   207,    -1,    81,   205,   304,   207,    -1,   151,
      -1,   151,   205,   207,    -1,   161,   205,   304,   207,    -1,
     165,    -1,   165,   205,   207,    -1,   167,    -1,   167,   205,
     207,    -1,   170,    -1,   170,   205,   207,    -1,   170,   205,
     304,   207,    -1,   174,    -1,   174,   205,   207,    -1,   176,
     205,   306,   207,    -1,   178,   205,   306,   206,   306,   207,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   322,   322,   323,   326,   327,   330,   332,   333,   334,
     335,   336,   343,   344,   351,   352,   353,   356,   371,   377,
     398,   399,   401,   416,   429,   430,   432,   450,   457,   468,
     468,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   512,   513,   514,   515,   516,   517,   518,   519,
     520,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,   533,   534,   535,   536,   537,   538,   541,
     542,   543,   544,   547,   548,   549,   550,   553,   556,   557,
     560,   563,   564,   567,   570,   573,   587,   588,   589,   592,
     593,   594,   598,   601,   604,   609,   616,   619,   622,   626,
     630,   633,   636,   637,   638,   641,   642,   643,   644,   645,
     648,   649,   652,   653,   657,   658,   661,   662,   665,   666,
     667,   668,   671,   672,   675,   676,   679,   682,   685,   686,
     687,   690,   691,   692,   693,   694,   695,   696,   697,   698,
     701,   702,   703,   706,   707,   708,   711,   712,   713,   716,
     717,   718,   721,   722,   723,   726,   727,   728,   731,   732,
     733,   734,   735,   736,   737,   738,   739,   740,   743,   746,
     747,   748,   749,   750,   753,   756,   757,   760,   761,   764,
     765,   766,   767,   769,   770,   771,   772,   773,   774,   777,
     780,   781,   784,   785,   788,   789,   792,   793,   796,   799,
     802,   803,   806,   809,   812,   815,   816,   819,   822,   825,
     826,   829,   830,   831,   834,   835,   836,   837,   840,   841,
     842,   845,   846,   847,   850,   851,   854,   855,   858,   859,
     862,   863,   864,   868,   871,   874,   875,   878,   879,   880,
     881,   882,   883,   884,   885,   886,   887,   888,   889,   890,
     891,   892,   893,   894,   895,   896,   897,   898,   899,   900,
     901,   902,   903,   904,   905,   906,   907,   908,   909,   910,
     911,   912,   913,   914,   915,   916,   927,   928,   929,   930,
     931,   932,   933,   934,   935,   936,   937,   938,   939,   940,
     941,   942,   943,   944,   945,   946,   947,   948,   949,   950,
     951,   952,   953,   954,   955,   956,   957,   958,   959,   960,
     961,   962,   963,   964,   965,   966,   967,   968,   969,   970,
     971,   972,   973,   974,   975,   976,   977,   978,   979,   980,
     981,   982,   983,   984,   985,   986,   987,   988,   989,   990,
     991,   992,   993,   994,   995,   996,   997,   998,   999,  1000,
    1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,  1039,  1040,
    1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1053,  1054,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,
    1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "B256PRINT", "B256INPUT", "B256KEY",
  "B256PIXEL", "B256RGB", "B256PLOT", "B256CIRCLE", "B256RECT", "B256POLY",
  "B256STAMP", "B256LINE", "B256FASTGRAPHICS", "B256GRAPHSIZE",
  "B256REFRESH", "B256CLS", "B256CLG", "B256IF", "B256THEN", "B256ELSE",
  "B256ENDIF", "B256WHILE", "B256ENDWHILE", "B256DO", "B256UNTIL",
  "B256FOR", "B256TO", "B256STEP", "B256NEXT", "B256OPEN", "B256READ",
  "B256WRITE", "B256CLOSE", "B256RESET", "B256GOTO", "B256GOSUB",
  "B256RETURN", "B256REM", "B256END", "B256SETCOLOR", "B256GTE", "B256LTE",
  "B256NE", "B256DIM", "B256REDIM", "B256NOP", "B256TOINT", "B256TOSTRING",
  "B256LENGTH", "B256MID", "B256LEFT", "B256RIGHT", "B256UPPER",
  "B256LOWER", "B256INSTR", "B256CEIL", "B256FLOOR", "B256RAND", "B256SIN",
  "B256COS", "B256TAN", "B256ASIN", "B256ACOS", "B256ATAN", "B256ABS",
  "B256PI", "B256DEGREES", "B256RADIANS", "B256LOG", "B256LOGTEN",
  "B256AND", "B256OR", "B256XOR", "B256NOT", "B256PAUSE", "B256SOUND",
  "B256ASC", "B256CHR", "B256TOFLOAT", "B256READLINE", "B256WRITELINE",
  "B256BOOLEOF", "B256MOD", "B256INTDIV", "B256YEAR", "B256MONTH",
  "B256DAY", "B256HOUR", "B256MINUTE", "B256SECOND", "B256TEXT",
  "B256FONT", "B256SAY", "B256SYSTEM", "B256VOLUME", "B256GRAPHWIDTH",
  "B256GRAPHHEIGHT", "B256GETSLICE", "B256PUTSLICE", "B256IMGLOAD",
  "B256SPRITEDIM", "B256SPRITELOAD", "B256SPRITESLICE", "B256SPRITEMOVE",
  "B256SPRITEHIDE", "B256SPRITESHOW", "B256SPRITEPLACE",
  "B256SPRITECOLLIDE", "B256SPRITEX", "B256SPRITEY", "B256SPRITEH",
  "B256SPRITEW", "B256SPRITEV", "B256WAVPLAY", "B256WAVSTOP",
  "B256WAVWAIT", "B256SIZE", "B256SEEK", "B256EXISTS", "B256BOOLTRUE",
  "B256BOOLFALSE", "B256MOUSEX", "B256MOUSEY", "B256MOUSEB",
  "B256CLICKCLEAR", "B256CLICKX", "B256CLICKY", "B256CLICKB",
  "B256GETCOLOR", "B256CLEAR", "B256BLACK", "B256WHITE", "B256RED",
  "B256DARKRED", "B256GREEN", "B256DARKGREEN", "B256BLUE", "B256DARKBLUE",
  "B256CYAN", "B256DARKCYAN", "B256PURPLE", "B256DARKPURPLE", "B256YELLOW",
  "B256DARKYELLOW", "B256ORANGE", "B256DARKORANGE", "B256GREY",
  "B256DARKGREY", "B256CHANGEDIR", "B256CURRENTDIR", "B256DECIMAL",
  "B256DBOPEN", "B256DBCLOSE", "B256DBEXECUTE", "B256DBOPENSET",
  "B256DBCLOSESET", "B256DBROW", "B256DBINT", "B256DBFLOAT",
  "B256DBSTRING", "B256ONERROR", "B256OFFERROR", "B256LASTERROR",
  "B256LASTERRORMESSAGE", "B256LASTERRORLINE", "B256LASTERROREXTRA",
  "B256NETLISTEN", "B256NETCONNECT", "B256NETREAD", "B256NETWRITE",
  "B256NETCLOSE", "B256NETDATA", "B256NETADDRESS", "B256KILL", "B256MD5",
  "B256SETSETTING", "B256GETSETTING", "B256PORTIN", "B256PORTOUT",
  "B256BINARYOR", "B256BINARYAND", "B256BINARYNOT", "B256IMGSAVE",
  "B256LINENUM", "B256INTEGER", "B256FLOAT", "B256STRING", "B256VARIABLE",
  "B256STRINGVAR", "B256NEWVAR", "B256COLOR", "B256LABEL", "'<'", "'>'",
  "'='", "'-'", "'+'", "'*'", "'/'", "B256UMINUS", "'^'", "'\\n'", "':'",
  "'('", "','", "')'", "'['", "']'", "';'", "'{'", "'}'", "'?'", "$accept",
  "program", "validline", "label", "validstatement", "compoundifstmt",
  "ifstmt", "elsestmt", "endifexpr", "endifstmt", "whilestmt",
  "endwhileexpr", "endwhilestmt", "dostmt", "untilstmt", "compoundstmt",
  "statement", "dimstmt", "redimstmt", "pausestmt", "clearstmt",
  "fastgraphicsstmt", "graphsizestmt", "refreshstmt", "endstmt", "ifexpr",
  "strarrayassign", "arrayassign", "numassign", "stringassign", "forstmt",
  "nextstmt", "gotostmt", "gosubstmt", "offerrorstmt", "onerrorstmt",
  "returnstmt", "colorstmt", "soundstmt", "plotstmt", "linestmt",
  "circlestmt", "rectstmt", "textstmt", "fontstmt", "saystmt",
  "systemstmt", "volumestmt", "polystmt", "stampstmt", "openstmt",
  "writestmt", "writelinestmt", "closestmt", "resetstmt", "seekstmt",
  "inputstmt", "inputexpr", "printstmt", "wavplaystmt", "wavstopstmt",
  "wavwaitstmt", "putslicestmt", "imgloadstmt", "spritedimstmt",
  "spriteloadstmt", "spriteslicestmt", "spriteplacestmt", "spritemovestmt",
  "spritehidestmt", "spriteshowstmt", "clickclearstmt", "changedirstmt",
  "decimalstmt", "dbopenstmt", "dbclosestmt", "dbexecutestmt",
  "dbopensetstmt", "dbclosesetstmt", "netlistenstmt", "netconnectstmt",
  "netwritestmt", "netclosestmt", "killstmt", "setsettingstmt",
  "portoutstmt", "imgsavestmt", "immediatestrlist", "immediatelist",
  "floatlist", "floatexpr", "stringlist", "stringexpr", 0
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,    60,    62,    61,    45,    43,    42,
      47,   449,    94,    10,    58,    40,    44,    41,    91,    93,
      59,   123,   125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   214,   215,   215,   216,   216,   217,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   219,   220,   221,
     222,   222,   223,   224,   225,   225,   226,   227,   228,   229,
     229,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   231,
     231,   231,   231,   232,   232,   232,   232,   233,   234,   234,
     235,   236,   236,   237,   238,   239,   240,   240,   240,   241,
     241,   241,   242,   243,   244,   244,   245,   246,   247,   248,
     249,   250,   251,   251,   251,   252,   252,   252,   252,   252,
     253,   253,   254,   254,   255,   255,   256,   256,   257,   257,
     257,   257,   258,   258,   259,   259,   260,   261,   262,   262,
     262,   263,   263,   263,   263,   263,   263,   263,   263,   263,
     264,   264,   264,   265,   265,   265,   266,   266,   266,   267,
     267,   267,   268,   268,   268,   269,   269,   269,   270,   270,
     270,   270,   270,   270,   270,   270,   270,   270,   271,   272,
     272,   272,   272,   272,   273,   274,   274,   275,   275,   276,
     276,   276,   276,   277,   277,   277,   277,   277,   277,   278,
     279,   279,   280,   280,   281,   281,   282,   282,   283,   284,
     285,   285,   286,   287,   288,   289,   289,   290,   291,   292,
     292,   293,   293,   293,   294,   294,   294,   294,   295,   295,
     295,   296,   296,   296,   297,   297,   298,   298,   299,   299,
     300,   300,   300,   301,   302,   303,   303,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     305,   305,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     7,     7,     3,     3,     7,     7,     2,     1,     1,
       1,     4,     6,     1,     1,     2,     6,     8,     3,     6,
       8,     3,     3,     3,     6,     8,     2,     2,     2,     1,
       2,     1,     6,     8,     2,     4,     2,     2,     6,     4,
       4,     6,     8,    10,     6,     8,     8,    10,     6,     8,
       6,     8,     6,     8,     2,     2,     2,     2,     2,     4,
       2,     8,    10,     8,     6,     8,     6,    10,    12,    10,
       2,     6,     4,     2,     6,     4,     2,     6,     4,     1,
       3,     2,     1,     3,     2,     2,     6,     4,     3,     6,
       3,     6,     2,     5,     7,     2,     5,     7,     2,     1,
       2,     2,     3,     3,     2,     1,     3,     1,     3,     6,
       8,     8,    10,     6,     8,     8,    10,    10,    12,     2,
       4,     6,    10,    12,     6,     8,     6,     8,     2,     2,
       1,     3,     2,     2,     2,     1,     3,     2,     2,     1,
       3,     2,     6,     4,     4,     6,     6,     8,     2,     6,
       4,     1,     3,     2,     2,     6,     6,     8,     4,     6,
       2,     6,     4,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     4,     4,     5,
       5,     5,     5,     4,     6,     1,     4,     4,     4,     4,
       4,     4,     6,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     4,     4,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     4,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     6,
       8,     1,     3,     6,     4,     4,     4,     4,     4,     3,
       4,     4,     1,     3,     1,     3,     1,     3,     4,     4,
       1,     3,     3,     3,     3,     3,     1,     4,     6,     1,
       4,     4,     4,     4,     8,     6,     6,    10,     1,     3,
       4,     1,     3,     4,     1,     3,     4,     1,     3,     1,
       3,     1,     3,     4,     1,     3,     4,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      16,   199,     0,     0,     0,     0,     0,     0,     0,   110,
       0,   113,   108,   109,     0,    19,    20,     0,    24,    27,
       0,     0,     0,     0,     0,   179,   182,     0,     0,   131,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   207,     0,   230,     0,     0,     0,   235,
       0,     0,   239,     0,   129,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     6,     0,     0,    16,     5,
       7,     8,     9,    22,    10,    11,    26,    12,    13,    14,
      15,    29,    54,    55,    56,    50,    52,    53,    51,    49,
       0,    58,    57,    43,    44,    45,    46,    31,    32,    33,
      34,    35,    47,    65,    37,    42,    38,    39,    66,    67,
      68,    69,    70,    40,    41,    59,    60,    61,    62,    63,
      83,    48,     0,    36,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    64,    96,
      97,    98,   357,     0,     0,   448,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,     0,     0,
       0,     0,     0,     0,     0,   328,     0,     0,     0,     0,
       0,     0,     0,     0,   451,   334,   338,   340,   342,   344,
     346,   348,   350,   352,     0,     0,     0,     0,     0,     0,
       0,   354,     0,   330,   332,   359,   361,   363,   365,   367,
     369,   411,   371,   373,   375,   377,   379,   381,   383,   385,
     387,   389,   391,   393,   395,   397,   399,   401,   403,   405,
     407,   454,     0,     0,     0,     0,   422,   457,   424,   459,
     461,   426,   464,     0,     0,     0,     0,   296,   295,   436,
     305,   439,     0,     0,   201,   200,   305,   439,     0,   198,
       0,     0,     0,     0,     0,     0,     0,   158,     0,     0,
     160,     0,     0,     0,     0,     0,     0,   115,    23,    28,
       0,   126,     0,     0,   170,     0,     0,   173,     0,   181,
       0,   184,   127,   128,    21,    25,     0,   134,     0,     0,
       0,     0,   107,   305,     0,   137,     0,     0,     0,   176,
       0,     0,     0,     0,   155,   154,   156,   157,     0,     0,
       0,     0,   219,     0,     0,     0,     0,     0,   228,   229,
       0,     0,   204,     0,     0,     0,   185,     0,   232,   233,
     234,     0,   237,   238,     0,   130,     0,   241,     0,     0,
       0,     0,     0,   248,     0,   253,     0,   254,     0,     0,
       0,     0,     0,   260,     0,     0,     0,     0,     1,     2,
       4,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   277,     0,
       0,   278,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   203,     0,     0,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   183,     0,     0,     0,    99,     0,
     100,     0,   103,     0,   104,   305,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   206,   208,     0,     0,   231,
     236,   240,     0,     0,     0,     0,     0,     0,     0,     0,
     252,     0,     0,     0,     0,     0,     0,     0,   121,   122,
       0,     0,   118,   123,     0,     3,   114,    30,    17,   190,
     188,   358,     0,     0,   449,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   327,     0,
       0,     0,     0,     0,     0,     0,   329,     0,     0,     0,
       0,     0,     0,     0,     0,   452,     0,   335,     0,   339,
     341,   343,   345,   347,   349,   351,   353,     0,     0,     0,
       0,     0,     0,     0,   355,     0,     0,   331,   333,   360,
     362,   364,   366,   368,   370,   412,   372,   374,   376,   378,
     380,   382,   384,   386,   388,   390,   392,   394,   396,   398,
     400,   402,   404,   406,   408,   455,   419,     0,     0,     0,
     423,   458,   425,   460,   462,     0,   427,     0,   465,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   267,   432,
     293,   294,   290,   279,   280,   281,   271,   272,   275,   276,
     291,   292,   289,   269,   268,   434,   270,   273,   274,   287,
     288,   284,   285,   286,   283,   435,   433,     0,     0,     0,
     140,     0,     0,     0,     0,   159,   264,     0,     0,     0,
       0,     0,     0,   111,     0,     0,   172,     0,   175,     0,
       0,     0,     0,     0,     0,   135,     0,   139,     0,   178,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   220,
       0,     0,     0,     0,     0,     0,   187,     0,   243,     0,
       0,     0,   244,     0,   250,     0,     0,     0,     0,   258,
       0,   262,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,   450,   306,   307,   441,   310,     0,     0,     0,
     442,   443,     0,   313,   314,   315,   316,   317,   318,   319,
     320,   325,   321,   322,   323,   324,   311,   440,   308,   309,
     453,   336,     0,     0,   414,   415,   416,   417,   418,   356,
     337,   420,   421,   456,   463,   428,   466,     0,   429,     0,
       0,   297,     0,   303,     0,     0,   298,     0,   437,     0,
     303,     0,   437,     0,     0,     0,     0,     0,   266,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   263,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   301,   299,
       0,   302,   300,     0,     0,     0,   141,     0,   144,     0,
       0,     0,   305,   166,     0,     0,     0,   112,   124,   171,
     174,     0,   132,     0,     0,     0,     0,   138,   177,     0,
     150,   148,     0,   152,     0,   209,     0,     0,   213,     0,
     221,     0,     0,   226,     0,   224,   186,   242,     0,   245,
     246,   249,   255,     0,   256,   259,   261,     0,   119,   431,
       0,   116,   191,   189,   409,     0,     0,   445,   446,   312,
       0,   413,   467,   304,   438,   304,   438,     0,     0,     0,
     305,     0,     0,     0,     0,     0,     0,   101,   102,   105,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,   146,   165,     0,   305,   163,     0,     0,   142,
     125,   133,   151,   149,   153,     0,   210,   211,     0,   214,
       0,   215,   227,     0,     0,   225,   247,   257,   120,   117,
     410,   444,     0,     0,   305,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   162,     0,   167,   169,
     143,   212,     0,   216,   217,     0,   222,   447,     0,     0,
       0,   168,   218,   223
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   592,   280,   520,
     268,   804,   272
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -295
static const yytype_int16 yypact[] =
{
   11062,  4368,  4543,  4718,  4893,  5068,  -184,  5243,  5418,  -295,
    5593,  -295,  -295,  -295,  4368,  -295,  -295,  4368,  -295,  -295,
    4368,  -188,  -158,  5768,  5943,  6118,  6293,  -150,  -137,  -295,
      40,  6468,  -172,  -129,  4368,  2005,  6643,  6818,  6993,  4368,
    4368,  4368,  7168,  7343,  4368,  7518,  7693,  4368,  4368,  4368,
    7868,  4368,  -151,  -131,  8043,  -104,  4368,  4368,  4368,   -98,
    4368,  4368,   -95,  -101,  -295,  8218,  8393,  8568,  8743,  8918,
    9093,  9268,  9443,  -182,  -176,  -295,    82,   -92, 11250,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
     -91,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
      92,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,   -90,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,   -88,   -63,   -58,   -53,   -51,   -50,   -40,   -35,
     -34,   -30,   -29,   -28,   -20,   -19,   -18,    -9,    15,    17,
      18,    19,    27,    28,    33,    34,    36,    37,    42,    59,
    4368,    60,    61,    63,    65,    66,    68,    72,    74,    76,
      83,    84,    85,    87,    88,    90,    91,    93,    98,    99,
     101,   106,   107,   109,   110,   112,   113,   115,   117,   118,
     128,   133,   135,   136,   137,   138,   140,   141,   143,   147,
     148,   149,   151,   153,   154,   156,   157,   159,   161,   163,
     165,   167,   170,   175,   177,   178,   186,   187,   188,   189,
     190,   193,   201,   206,   207,   212,  4368,  -295,  -295,  -295,
     -94,   -93,  4368,  4368,  1012,   -32,  -153,  -146, 17409,   258,
    4368, 14979,   258,  4368, 15012,  4368, 15045,  -295,   146,  4368,
    -295,  4368, 15078,  4368, 15114,  4368, 15147, 17409, 17409, 17409,
      49,  -295,  4368, 15180,   258,  4368, 15213,   258,   538, 17409,
    2880, 17409,  -295,  -295,  -295,  -295,  4368, 15249,  9618,  9793,
    9968, 10143, 17409,  -135, 10318,  -295, 15282,  4368, 15315,   258,
    4368, 15348,  4368,   379, 17409,   258,   258, 17409,  4368, 15384,
    4368, 15417, 17409,  4368, 15450,  4368, 15483, 15519, 17409, 17409,
    4368, 15552,   258,   211,   213,  4368, 15585,   217,   258, 17409,
     258,   226,   258,   258,   227,  -295,  4368, 15618,  4368, 15654,
     949,  4368, 15687,   258,  3066, 17409,  4368,   258,  4368,  1057,
    4368, 15720,  4368,  1290,  2180,  4368,  2355,  4368,  -295, 10866,
    -295,  1785,  1785,  -118,   228,  4368,  4368,  3252,  4368,  4368,
    4368,  4368,  4368,  4368,  4368,  4368,  4368,  4368,  4368,   229,
    4368,  4368,  4368,  4368,  4368,  4368,  4368,   233,  4368,  4368,
    4368,  4368,   240,  4368,  4368,  4368,  3438,  3624,   244,   248,
     250,   254,   255,   256,   260,   261,  4368,  4368,  4368,  4368,
    4368,  4368,  4368,  3810,  4368,   262,   263,   266,   267,   268,
     270,   273,   274,   275,   297,   299,   300,   302,   305,   309,
     311,   312,   313,   314,   316,   318,   321,   322,   323,   324,
     325,   326,   327,   330,   332,  4368,  4368,  4368,   334,   340,
     341,   342,  3996,  4182,   344,  4368,  4368,  4368,   240,    70,
    1495,   240, 12816,    56,  4368,  4368,  4368,  4368,  4368,  4368,
    4368,  4368,  4368,  4368,  4368,  4368,  4368,  4368,  4368,  4368,
    4368,  4368,  -295,  4368,  4368,  4368,  4368,  4368,  4368,  4368,
    -295,    70,  1495, 11735,  4368, 11772,  4368, 11805,  4368,   346,
     235, 15753, 11854,  4368, 11908,  4368, 11941,  4368,  4368, 11985,
    4368, 12044,  4368,  -295,  -295, 12077,  4368,  4368, 17409,  4368,
   17409,  4368, 17409,  4368, 17409,  -117, 12116,  4368, 12163,  4368,
   12218,  4368,   130,  4368, 12252,  4368, 12299,  4368, 12337,  4368,
   12388,  4368,  4368, 12440,  4368,  -295,  -295, 12473,  4368,  -295,
    -295,  -295, 12507,  4368, 12529,   183,  4368,  4368, 12576,  4368,
    -295, 12609,   243,  4368, 12648,  4368,   416,  4368,  -295, 17409,
    1215,  4368,  -295,   258,  1316,  -295,  -295,  -295,   -91,   347,
     349,  -295, 15789, 15822,  -295, 12843, 12881,   307, 12935,   649,
    1660,  1791,  1796,   720,   769,  1821, 12962, 12984,  -295, 13017,
   13054, 13103, 13153, 13186, 13222, 13244,  -295, 13289, 13322, 13388,
   13421,   867, 13458, 13491,   895,  -295, 13524,  -295, 13557,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295, 15855, 15888, 13590,
   13627, 13660, 13693, 13726,  -295, 13759,   913,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295, 13796, 13829, 13862,
    -295,  -295,  -295,  -295,  -295, 13895,  -295, 13928,  -295,   970,
    1827, 13965,   345,  -122, 11256,   351,  -120, 11305,  -295,  -295,
     638,   638,   638, 11331, 17475, 17442,   240,   240,   246,   246,
     638,   638,   638,   -56,   -56,  -295,   240,   240,  -295,   221,
     221,   221,   221,   221,   221,   -56,  -295, 11399, 11436,  4368,
   17409,  4368, 15924,  4368, 15957,  -295,  -295,  4368,  4368, 15990,
    4368, 16023,  4368, 17409,   954,  4368,   258,  4368,   258,  4368,
   16059, 12675, 12712, 12750, 12794,  -295,  4368, 17409,  4368,   258,
    4368, 16092,  4368, 16125,  4368, 16158,  4368, 16194,  4368,   258,
    4368, 16227, 16260,  4368, 16293,  4368, 17409,  4368, 17409,  4368,
    4368,  1840, 17409,  4368,   258,  4368,  4368,  2078,  4368, 17409,
    4368,   258,  4368,   363,   350,  2177,  4368,   369,  4368,  4368,
    4368,  4368,  -295,  -295,  -295,  -295,  -295,  4368,  4368,  4368,
    -295,  -295,  4368,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  4368,  4368,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  4368,  -295,   357,
     359,  -295,  4368,  -295,   360,   362,  -295,  4368,  -295,  4368,
    -111,  4368,  -109, 13998, 16329,  4368, 16362,  4368,  -295, 16395,
    2530, 16428,  4368, 14031,  4368,  1079,  1098, 16464,  4368,  4368,
    4368,  4368,  4368, 14064,  1129, 16497,  4368, 16530,  4368, 16563,
    4368, 16599,  4368, 16632,  1173, 16665,  4368,  4368, 16698,  4368,
   14097, 14134,  2253, 14167,  4368,  1197,  1310,  2352,  4368, 14200,
    1393, 11473,  4368,  -295,  4368, 11498,  4368, 11542, 11567, 14233,
   16734, 16767, 14266, 14303,  1403, 16800, 14336,  1492,  -295,  -295,
   11604,  -295,  -295, 11641, 11686, 11710,  -295,  4368, 17409,  4368,
   16833, 10493,  -125,  -295, 16869,  4368, 16902,  -295,  1003,  -295,
    -295,  4368, 17409, 14369, 14402, 14435, 14472,  -295,  -295,  4368,
   17409,   258,  4368, 17409,  4368,  2428,  4368, 16935,   258,  4368,
    -295,  4368, 16968, 17409,  4368, 17409,  -295,  -295,  4368,  -295,
   17409,  -295,  -295,  4368,   258,  -295,  -295,   382, 17409,  -295,
     383,   258,  -295,  -295,  -295,  4368,  4368,  -295,  -295,  -295,
    4368,  -295,  -295,  -295,  -295,  -107,  -100, 14505, 17004,  4368,
    -102, 17037,  2705, 17070,  4368,  4368, 14538,  -295,  -295,  -295,
    -295, 14571,  1525, 14604,   596,  4368, 17103,  1653,  4368, 14641,
   17139,  4368, 14674, 14707,  1711,  4368,  4368, 14740, 14773, 17172,
    -295,  4368, 17409,  -295, 10668,  -123,  -295, 17205,  4368, 17409,
   17409,  -295,  -295,  -295,  -295,  4368,  -295, 17409,  4368,  -295,
   17238,   258,  -295,  4368, 17274,  -295,  -295,  -295, 17409,   258,
    -295,  -295,  4368, 14810,   -99, 17307,  -174, 14843, 14876, 17340,
    1718,  4368, 17373,  4368, 14909,  -295,  -295,   391,  -295,  -295,
    -295,  -295,  4368,  -295,   258,  4368, 17409,  -295,   374,  1738,
   14942,  -295,  -295,  -295
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -295,   203,  -295,  -295,   505,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,   238,   234,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,   -27,  -116,
      -1,  -294,   950
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -198
static const yytype_int16 yytable[] =
{
     264,   290,   271,   274,   276,   277,   282,   284,   315,   286,
     503,   504,   505,   287,   374,  1098,   288,   308,   309,   289,
     376,   278,   293,   296,   299,   301,   375,   279,   490,   491,
     307,   291,   377,   312,   316,   318,   321,   279,   324,   302,
     327,   329,   331,   332,   334,   336,   337,   338,   339,   341,
    -195,  -195,   303,   346,   343,   511,   349,  -192,  -192,   304,
     310,   311,   512,   305,   357,   359,   362,   365,  -136,  -136,
     371,   599,   600,   479,   344,   162,   163,   164,  -164,  -164,
    -161,  -161,   378,   479,   860,   479,   865,   861,   355,   866,
     765,   479,  -196,  -196,  -193,  -193,  -197,  -197,   503,   504,
     505,   347,   165,  -194,  -194,  1053,   479,   351,  1096,   479,
     354,   379,   382,   381,   479,   480,   383,   384,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   385,   499,   500,   190,   501,   386,   191,   192,
     193,   194,   387,   195,   388,   389,   196,   197,   198,   199,
     200,   201,   506,   507,   508,   390,   509,   202,   203,   204,
     391,   392,   503,   504,   505,   393,   394,   395,   510,   205,
     206,   207,   208,   209,   210,   396,   397,   398,   211,   412,
     212,   213,   214,   215,   216,   217,   399,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     400,   241,   401,   402,   403,   503,   504,   505,   242,   243,
     244,   245,   404,   405,   246,   247,   248,   249,   406,   407,
     250,   408,   409,   251,   252,   528,   253,   410,   254,   255,
     506,   507,   508,   256,   509,   478,   257,   258,   259,   260,
     261,   481,   482,   709,   411,   413,   414,   262,   415,   513,
     416,   417,   515,   418,   517,   263,   702,   419,   521,   420,
     522,   421,   524,   703,   526,   503,   504,   505,   422,   423,
     424,   529,   425,   426,   531,   427,   428,   482,   429,   482,
     503,   504,   505,   430,   431,   535,   432,   538,   540,   542,
     544,   433,   434,   546,   435,   436,   548,   437,   438,   550,
     439,   482,   440,   441,   506,   507,   508,   554,   509,   556,
     490,   491,   558,   442,   560,   519,   772,   709,   443,   563,
     444,   445,   446,   447,   567,   448,   449,   588,   450,   503,
     504,   505,   451,   452,   453,   572,   454,   574,   455,   456,
     578,   457,   458,   482,   459,   581,   460,   482,   461,   584,
     462,   482,   463,   589,   590,   464,   594,   506,   507,   508,
     465,   509,   466,   467,   602,   603,   605,   606,   608,   790,
     709,   468,   469,   470,   471,   472,   616,   617,   473,   619,
     620,   621,   622,   623,   624,   625,   474,   627,   628,   629,
     630,   475,   476,   632,   633,   636,   638,   477,   565,   509,
     566,   503,   504,   505,   569,   647,   648,   649,   650,   651,
     652,   653,   655,   570,   571,   601,   618,   506,   507,   508,
     626,   509,   501,   497,   498,   499,   500,   746,   501,   796,
     709,   639,   506,   507,   508,   640,   509,   641,   503,   504,
     505,   642,   643,   644,   687,   688,   689,   645,   646,   657,
     658,   695,   697,   659,   660,   661,   701,   662,   704,   707,
     663,   664,   665,   710,   711,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   724,   726,   727,
     728,   506,   507,   508,   666,   509,   667,   668,   735,   669,
     737,   738,   670,   740,   814,   742,   671,   744,   672,   673,
     674,   675,   749,   676,   751,   677,   753,   754,   678,   679,
     680,   681,   682,   683,   684,   760,   761,   685,   762,   686,
     763,   690,   764,   162,   163,   164,   767,   691,   692,   693,
     771,   698,   773,   745,   775,   808,   777,   809,   859,   922,
     781,   782,   923,   784,   864,   926,   938,   786,   939,   941,
     165,   942,   788,   506,   507,   508,   792,   509,  1045,  1046,
    1108,  1111,   595,   380,   799,   553,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     506,   507,   508,   190,   509,   597,   191,   192,   193,   194,
     598,   195,   800,   709,   196,   197,   198,   199,   200,   201,
     999,   878,     0,     0,     0,   202,   203,   204,   503,   504,
     505,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,   503,   504,   505,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,     0,     0,
       0,   256,   490,   491,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,   873,     0,
     874,     0,   876,   263,     0,   533,   521,   879,     0,   881,
       0,   883,     0,     0,     0,     0,     0,     0,   887,     0,
       0,     0,   503,   504,   505,   893,     0,     0,     0,   895,
       0,   897,     0,   899,     0,   901,     0,   903,     0,   905,
       0,     0,   908,     0,   910,     0,   911,     0,     0,   913,
     506,   507,   508,     0,   509,     0,     0,   919,     0,     0,
       0,   921,  1065,  1066,     0,   925,     0,   927,   928,   929,
     930,   503,   504,   505,     0,     0,   931,   932,   933,   492,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,   498,   499,   500,     0,
     501,   935,   936,   506,   507,   508,     0,   509,     0,     0,
       0,     0,     0,   953,     0,     0,   816,     0,     0,     0,
       0,   940,     0,     0,     0,     0,   943,     0,   944,     0,
     945,     0,     0,     0,   948,     0,   950,     0,     0,   954,
       0,   956,     0,   958,     0,     0,     0,   962,   963,   964,
     965,   966,     0,     0,     0,   970,     0,   973,     0,     0,
       0,   977,     0,     0,     0,   982,   983,     0,   985,   503,
     504,   505,     0,   990,   506,   507,   508,     0,   509,     0,
       0,   998,     0,     0,     0,     0,     0,   820,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   503,   504,   505,
       0,     0,     0,     0,     0,     0,  1017,     0,  1018,     0,
    1021,   265,   269,     0,  1023,   503,   504,   505,     0,     0,
    1026,     0,     0,   506,   507,   508,     0,   509,  1031,     0,
       0,  1033,     0,   294,   297,  1036,   821,     0,  1039,     0,
    1040,     0,   884,  1042,     0,     0,   319,  1043,   323,   325,
     326,   503,   504,   505,     0,  1056,   484,   485,   486,     0,
       0,   342,     0,     0,  1047,  1048,   348,     0,   350,  1049,
     352,   353,   503,   504,   505,     0,   360,   363,  1052,   367,
     369,  1057,   373,  1059,  1060,     0,   487,   488,   489,     0,
       0,     0,  1025,     0,  1067,     0,     0,  1070,   490,   491,
    1074,     0,     0,     0,  1078,   484,   485,   486,     0,     0,
    1083,     0,     0,  1085,   484,   485,   486,  1087,     0,  1099,
       0,   506,   507,   508,  1088,   509,     0,  1089,     0,     0,
       0,     0,  1092,     0,   836,   487,   488,   489,     0,     0,
       0,  1094,     0,     0,   487,   488,   489,   490,   491,   506,
     507,   508,  1106,   509,     0,     0,   490,   491,     0,   503,
     504,   505,   839,     0,  1110,     0,     0,   506,   507,   508,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
     850,   503,   504,   505,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   492,   493,     0,     0,     0,
     503,   504,   505,   506,   507,   508,     0,   509,   494,   495,
     496,   497,   498,   499,   500,   577,   501,     0,     0,     0,
       0,     0,     0,     0,   506,   507,   508,     0,   509,     0,
       0,   503,   504,   505,     0,     0,     0,   856,     0,     0,
       0,     0,     0,     0,   492,   493,     0,     0,     0,     0,
       0,     0,     0,   492,   493,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,   494,   495,   496,   497,
     498,   499,   500,   483,   501,   503,   504,   505,     0,     0,
     483,     0,   502,   483,     0,   483,     0,     0,     0,     0,
       0,   483,     0,   483,     0,   483,     0,     0,     0,   503,
     504,   505,   483,     0,     0,   483,     0,     0,   483,     0,
     483,   506,   507,   508,     0,   509,   483,   484,   485,   486,
       0,     0,     0,   583,   483,     0,     0,   483,     0,     0,
     483,     0,   552,   506,   507,   508,     0,   509,   483,     0,
     483,     0,     0,   483,     0,   483,   959,   487,   488,   489,
     483,     0,   506,   507,   508,   483,   509,     0,     0,   490,
     491,     0,     0,     0,     0,   960,   483,     0,   575,     0,
       0,   483,     0,     0,   483,     0,   483,     0,   582,     0,
     483,     0,   586,   506,   507,   508,   593,   509,     0,     0,
       0,     0,   503,   504,   505,     0,   968,     0,   607,     0,
     609,   610,   611,   612,   613,   614,   615,     0,     0,     0,
       0,     0,   503,   504,   505,     0,     0,     0,   484,   485,
     486,     0,     0,   631,     0,   634,     0,   506,   507,   508,
       0,   509,     0,     0,     0,     0,     0,     0,     0,     0,
     980,     0,     0,     0,   656,     0,     0,     0,   487,   488,
     489,   506,   507,   508,     0,   509,   492,   493,     0,     0,
     490,   491,     0,     0,   991,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   802,     0,     0,   803,   699,   700,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,     0,     0,
       0,     0,     0,     0,     0,   503,   504,   505,   725,     0,
       0,     0,     0,   729,   730,   731,   732,   733,   734,   736,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     756,     0,   758,     0,   506,   507,   508,   483,   509,   483,
       0,   483,     0,   483,     0,     0,   587,   492,   493,   769,
     162,   163,   164,     0,   506,   507,   508,     0,   509,   779,
     494,   495,   496,   497,   498,   499,   500,   992,   501,     0,
       0,     0,   806,     0,     0,   807,   791,   165,     0,   794,
       0,     0,     0,   797,   503,   504,   505,   801,     0,     0,
       0,   805,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   503,   504,   505,
     190,     0,     0,   191,   192,   193,   194,     0,   195,     0,
       0,   196,   197,   198,   199,   200,   201,   506,   507,   508,
       0,   509,   202,   203,   204,     0,     0,   506,   507,   508,
     996,   509,     0,     0,   205,   206,   207,   208,   209,   210,
    1009,     0,     0,   211,     0,   212,   213,   214,   215,   216,
     217,     0,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,   244,   245,     0,     0,   246,
     247,   248,   249,     0,     0,   250,     0,     0,   251,   252,
       0,   253,     0,   254,   255,     0,     0,     0,   256,     0,
       0,   257,   258,   259,   260,   261,   506,   507,   508,     0,
     509,     0,   262,     0,     0,   503,   504,   505,     0,  1012,
     263,   705,   503,   504,   505,   885,     0,   886,   706,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   894,   506,
     507,   508,     0,   509,     0,     0,     0,     0,   904,     0,
       0,     0,  1063,     0,     0,     0,     0,     0,     0,   912,
       0,     0,     0,   915,     0,   916,   917,     0,     0,     0,
     920,     0,     0,   503,   504,   505,     0,     0,     0,     0,
     503,   504,   505,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   934,     0,     0,     0,     0,     0,     0,     0,
     503,   504,   505,     0,     0,     0,     0,     0,     1,     2,
       0,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,     0,     0,     0,   937,     0,     0,
       0,     0,    21,     0,     0,    22,    23,     0,    24,    25,
      26,    27,    28,    29,     0,   596,    31,     0,     0,     0,
      32,    33,     0,   503,   504,   505,     0,     0,   503,   504,
     505,     0,     0,     0,     0,     0,   971,   506,   507,   508,
     975,   509,   978,     0,   506,   507,   508,     0,   509,     0,
    1069,    34,    35,   503,   504,   505,   817,    36,   994,   503,
     504,   505,     0,     0,   805,     0,  1001,    37,    38,    39,
      40,    41,   503,   504,   505,    42,    43,    44,    45,    46,
      47,    48,    49,    50,     0,     0,     0,     0,     0,     0,
      51,    52,    53,     0,    54,   506,   507,   508,     0,   509,
       0,    55,   506,   507,   508,     0,   509,     0,  1077,  1032,
       0,     0,     0,     0,  1034,  1103,  1037,     0,     0,     0,
       0,     0,   506,   507,   508,    56,   509,    57,    58,    59,
      60,    61,    62,  1044,     0,  1112,     0,    63,    64,     0,
       0,     0,     0,    65,    66,     0,    67,    68,     0,     0,
      69,     0,    70,     0,     0,    71,     0,     0,     0,    72,
       0,     0,     0,     0,    73,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,   507,   508,  1071,   509,
     506,   507,   508,     0,   509,     0,  1079,   818,     0,     0,
       0,     0,   819,     0,     0,     0,     0,     0,     0,     0,
     162,   163,   164,     0,     0,   506,   507,   508,  1090,   509,
       0,   506,   507,   508,     0,   509,     0,   822,     0,     0,
       0,     0,     0,   857,   506,   507,   508,   165,   509,     0,
       0,  1104,     0,     0,     0,     0,   914,     0,     0,     0,
       0,     0,  1109,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,     0,     0,
     190,     0,     0,   191,   192,   193,   194,     0,   195,     0,
       0,   196,   197,   198,   199,   200,   201,     0,     0,     0,
       0,     0,   202,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   205,   206,   207,   208,   209,   210,
     503,   504,   505,   211,     0,   212,   213,   214,   215,   216,
     217,     0,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,   244,   245,     0,     0,   246,
     247,   248,   249,     0,     0,   250,     0,     0,   251,   252,
       0,   253,     0,   254,   255,   162,   163,   164,   256,     0,
       0,   257,   258,   259,   313,   261,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
     314,     0,   165,     0,     0,     0,   279,     0,     0,   503,
     504,   505,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,     0,     0,   190,     0,     0,   191,   192,
     193,   194,     0,   195,     0,     0,   196,   197,   198,   199,
     200,   201,   506,   507,   508,     0,   509,   202,   203,   204,
       0,     0,     0,     0,   918,     0,     0,     0,     0,   205,
     206,   207,   208,   209,   210,   503,   504,   505,   211,     0,
     212,   213,   214,   215,   216,   217,     0,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
     244,   245,     0,     0,   246,   247,   248,   249,     0,     0,
     250,     0,     0,   251,   252,     0,   253,     0,   254,   255,
     162,   163,   164,   256,     0,     0,   257,   258,   259,   260,
     261,   506,   507,   508,     0,   509,     0,   262,     0,     0,
       0,     0,     0,   924,     0,   263,     0,   165,     0,     0,
       0,   279,     0,     0,   503,   504,   505,     0,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,     0,     0,
     190,     0,     0,   191,   192,   193,   194,     0,   195,     0,
       0,   196,   197,   198,   199,   200,   201,   506,   507,   508,
       0,   509,   202,   203,   204,     0,     0,     0,     0,   988,
       0,     0,     0,     0,   205,   206,   207,   208,   209,   210,
     503,   504,   505,   211,     0,   212,   213,   214,   215,   216,
     217,     0,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,   244,   245,     0,     0,   246,
     247,   248,   249,     0,     0,   250,     0,     0,   251,   252,
       0,   253,     0,   254,   255,   162,   163,   164,   256,     0,
       0,   257,   258,   259,   260,   261,   506,   507,   508,     0,
     509,     0,   262,     0,     0,     0,     0,     0,   993,     0,
     263,     0,   165,     0,     0,     0,   591,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,     0,     0,   190,     0,     0,   191,   192,
     193,   194,     0,   195,     0,     0,   196,   197,   198,   199,
     200,   201,   506,   507,   508,     0,   509,   202,   203,   204,
       0,     0,     0,     0,  1035,     0,     0,     0,     0,   205,
     206,   207,   208,   209,   210,     0,     0,     0,   211,     0,
     212,   213,   214,   215,   216,   217,     0,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
     244,   245,     0,     0,   246,   247,   248,   249,     0,     0,
     250,     0,     0,   251,   252,     0,   253,     0,   254,   255,
     162,   163,   164,   256,     0,     0,   257,   258,   259,   952,
     261,     0,     0,     0,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   165,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,     0,     0,
     190,     0,     0,   191,   192,   193,   194,     0,   195,     0,
       0,   196,   197,   198,   199,   200,   201,     0,     0,     0,
       0,     0,   202,   203,   204,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,     0,   211,     0,   212,   213,   214,   215,   216,
     217,     0,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,   244,   245,     0,     0,   246,
     247,   248,   249,     0,     0,   250,     0,     0,   251,   252,
       0,   253,     0,   254,   255,   162,   163,   164,   256,     0,
       0,   257,   258,   259,  1055,   261,     0,     0,     0,     0,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   165,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,     0,     0,   190,     0,     0,   191,   192,
     193,   194,     0,   195,     0,     0,   196,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,   202,   203,   204,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
     206,   207,   208,   209,   210,     0,     0,     0,   211,     0,
     212,   213,   214,   215,   216,   217,     0,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
     244,   245,     0,     0,   246,   247,   248,   249,     0,     0,
     250,     0,     0,   251,   252,     0,   253,     0,   254,   255,
       0,     0,     0,   256,     0,     0,   257,   258,   259,   260,
     261,   162,   163,   164,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   534,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,     0,     0,
       0,   190,     0,     0,   191,   192,   193,   194,     0,   195,
       0,     0,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,   202,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,   206,   207,   208,   209,
     210,     0,     0,     0,   211,     0,   212,   213,   214,   215,
     216,   217,     0,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,   244,   245,     0,     0,
     246,   247,   248,   249,     0,     0,   250,     0,     0,   251,
     252,     0,   253,     0,   254,   255,     0,     0,     0,   256,
       0,     0,   257,   258,   259,   260,   261,   162,   163,   164,
       0,     0,     0,   262,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   580,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,     0,     0,   190,     0,     0,
     191,   192,   193,   194,     0,   195,     0,     0,   196,   197,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,   206,   207,   208,   209,   210,     0,     0,     0,
     211,     0,   212,   213,   214,   215,   216,   217,     0,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,   244,   245,     0,     0,   246,   247,   248,   249,
       0,     0,   250,     0,     0,   251,   252,     0,   253,     0,
     254,   255,     0,     0,     0,   256,     0,     0,   257,   258,
     259,   260,   261,   162,   163,   164,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,     0,   263,     0,   604,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,     0,     0,
       0,   256,     0,     0,   257,   258,   259,   260,   261,   162,
     163,   164,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,     0,     0,   190,
       0,     0,   191,   192,   193,   194,     0,   195,     0,     0,
     196,   197,   198,   199,   200,   201,     0,     0,     0,     0,
       0,   202,   203,   204,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   205,   206,   207,   208,   209,   210,     0,
       0,     0,   211,     0,   212,   213,   214,   215,   216,   217,
       0,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,   244,   245,     0,     0,   246,   247,
     248,   249,     0,     0,   250,     0,     0,   251,   252,     0,
     253,     0,   254,   255,     0,     0,     0,   256,     0,     0,
     257,   258,   259,   260,   261,   162,   163,   164,     0,     0,
       0,   262,     0,     0,     0,     0,     0,     0,     0,   263,
       0,   637,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,     0,     0,     0,   190,     0,     0,   191,   192,
     193,   194,     0,   195,     0,     0,   196,   197,   198,   199,
     200,   201,     0,     0,     0,     0,     0,   202,   203,   204,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
     206,   207,   208,   209,   210,     0,     0,     0,   211,     0,
     212,   213,   214,   215,   216,   217,     0,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
       0,   241,     0,     0,     0,     0,     0,     0,   242,   243,
     244,   245,     0,     0,   246,   247,   248,   249,     0,     0,
     250,     0,     0,   251,   252,     0,   253,     0,   254,   255,
       0,     0,     0,   256,     0,     0,   257,   258,   259,   260,
     261,   162,   163,   164,     0,     0,     0,   262,     0,     0,
       0,     0,     0,     0,     0,   263,     0,   654,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,     0,     0,
       0,   190,     0,     0,   191,   192,   193,   194,     0,   195,
       0,     0,   196,   197,   198,   199,   200,   201,     0,     0,
       0,     0,     0,   202,   203,   204,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,   206,   207,   208,   209,
     210,     0,     0,     0,   211,     0,   212,   213,   214,   215,
     216,   217,     0,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,     0,   241,     0,     0,
       0,     0,     0,     0,   242,   243,   244,   245,     0,     0,
     246,   247,   248,   249,     0,     0,   250,     0,     0,   251,
     252,     0,   253,     0,   254,   255,     0,     0,     0,   256,
       0,     0,   257,   258,   259,   260,   261,   162,   163,   164,
       0,     0,     0,   262,     0,     0,     0,     0,     0,     0,
       0,   263,     0,   694,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,     0,     0,   190,     0,     0,
     191,   192,   193,   194,     0,   195,     0,     0,   196,   197,
     198,   199,   200,   201,     0,     0,     0,     0,     0,   202,
     203,   204,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,   206,   207,   208,   209,   210,     0,     0,     0,
     211,     0,   212,   213,   214,   215,   216,   217,     0,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,     0,   241,     0,     0,     0,     0,     0,     0,
     242,   243,   244,   245,     0,     0,   246,   247,   248,   249,
       0,     0,   250,     0,     0,   251,   252,     0,   253,     0,
     254,   255,     0,     0,     0,   256,     0,     0,   257,   258,
     259,   260,   261,   162,   163,   164,     0,     0,     0,   262,
       0,     0,     0,     0,     0,     0,     0,   263,     0,   696,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   266,   267,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   263,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   270,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   273,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   275,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   281,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   283,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   285,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   292,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   295,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   298,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   300,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   306,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   317,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   320,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   322,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   328,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   330,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   333,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   335,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   340,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   345,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   356,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   358,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   361,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   364,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   366,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   368,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   370,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   372,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   537,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   539,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   260,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   541,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,   260,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   543,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,   162,   163,
     164,   256,     0,     0,   257,   258,   259,   545,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,   263,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,     0,     0,     0,   190,     0,
       0,   191,   192,   193,   194,     0,   195,     0,     0,   196,
     197,   198,   199,   200,   201,     0,     0,     0,     0,     0,
     202,   203,   204,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,   211,     0,   212,   213,   214,   215,   216,   217,     0,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,   244,   245,     0,     0,   246,   247,   248,
     249,     0,     0,   250,     0,     0,   251,   252,     0,   253,
       0,   254,   255,   162,   163,   164,   256,     0,     0,   257,
     258,   259,  1020,   261,     0,     0,     0,     0,     0,     0,
     262,     0,     0,     0,     0,     0,     0,     0,   263,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,     0,     0,   190,     0,     0,   191,   192,   193,   194,
       0,   195,     0,     0,   196,   197,   198,   199,   200,   201,
       0,     0,     0,     0,     0,   202,   203,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,   211,     0,   212,   213,
     214,   215,   216,   217,     0,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,     0,   241,
       0,     0,     0,     0,     0,     0,   242,   243,   244,   245,
       0,     0,   246,   247,   248,   249,     0,     0,   250,     0,
       0,   251,   252,     0,   253,     0,   254,   255,     0,     0,
       0,   256,     0,     0,   257,   258,   259,  1084,   261,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     1,
       2,     0,     0,   263,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    22,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,     0,
       0,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,    35,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,     0,     0,     0,
       0,    51,    52,    53,     0,    54,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,    57,    58,
      59,    60,    61,    62,     0,     0,     0,     0,    63,    64,
       0,     0,     0,     0,    65,    66,     0,    67,    68,     0,
       0,    69,     0,    70,     0,     0,    71,     0,     0,     0,
      72,     0,     0,     0,     0,    73,    74,     0,     0,    75,
       0,     0,     0,     0,     0,     1,     2,     0,     0,   -16,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,     0,    15,    16,    17,    18,    19,    20,    21,
       0,     0,    22,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,     0,     0,    32,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,    35,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,     0,
       0,     0,    42,    43,    44,    45,    46,    47,    48,    49,
      50,     0,     0,     0,     0,     0,     0,    51,    52,    53,
       0,    54,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    57,    58,    59,    60,    61,    62,
       0,     0,     0,     0,    63,    64,     0,     0,     0,     0,
      65,    66,     0,    67,    68,     0,     0,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,     0,     0,
       0,    73,    74,     1,     2,    75,     0,     0,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       0,    15,    16,    17,    18,    19,    20,    21,     0,     0,
      22,    23,     0,    24,    25,    26,    27,    28,    29,     0,
      30,    31,     0,     0,     0,    32,    33,     0,   484,   485,
     486,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    35,   487,   488,
     489,     0,    36,     0,     0,     0,     0,     0,     0,     0,
     490,   491,    37,    38,    39,    40,    41,   484,   485,   486,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,     0,    54,
       0,     0,     0,   484,   485,   486,    55,   487,   488,   489,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,    57,    58,    59,    60,    61,    62,     0,     0,
       0,     0,    63,    64,     0,   490,   491,     0,    65,    66,
       0,    67,    68,     0,     0,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,     0,   492,   493,    73,
      74,   484,   485,   486,     0,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,   862,     0,     0,   863,     0,     0,     0,     0,
       0,   487,   488,   489,     0,     0,     0,     0,   484,   485,
     486,     0,     0,   490,   491,     0,   492,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,   487,   488,
     489,   867,   492,   493,   868,   484,   485,   486,     0,     0,
     490,   491,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,     0,     0,     0,
     484,   485,   486,     0,     0,   487,   488,   489,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     487,   488,   489,     0,     0,     0,     0,     0,     0,     0,
     492,   493,   490,   491,   484,   485,   486,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   869,     0,     0,   870,   484,
     485,   486,     0,     0,   487,   488,   489,   492,   493,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,   487,
     488,   489,   871,     0,     0,   872,   484,   485,   486,     0,
       0,   490,   491,     0,   492,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,   487,   488,   489,   492,
     493,     0,   997,   484,   485,   486,     0,     0,   490,   491,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,     0,     0,     0,  1000,     0,     0,
       0,     0,     0,   487,   488,   489,     0,     0,     0,     0,
       0,     0,     0,   492,   493,   490,   491,     0,   484,   485,
     486,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   492,   493,
       0,  1002,   484,   485,   486,     0,     0,     0,   487,   488,
     489,   494,   495,   496,   497,   498,   499,   500,     0,   501,
     490,   491,     0,     0,     0,     0,  1003,   484,   485,   486,
       0,     0,   487,   488,   489,   492,   493,     0,     0,     0,
       0,     0,     0,     0,   490,   491,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,   487,   488,   489,
       0,     0,     0,  1013,   484,   485,   486,     0,     0,   490,
     491,     0,   492,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,   487,   488,   489,   484,   485,   486,
    1014,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   492,   493,     0,
       0,     0,     0,     0,     0,     0,     0,   487,   488,   489,
     494,   495,   496,   497,   498,   499,   500,     0,   501,   490,
     491,   492,   493,     0,     0,  1015,   484,   485,   486,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   492,   493,     0,  1016,
       0,     0,     0,     0,     0,     0,   487,   488,   489,   494,
     495,   496,   497,   498,   499,   500,     0,   501,   490,   491,
       0,   739,   708,     0,     0,     0,     0,     0,     0,     0,
     484,   485,   486,   492,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   741,   708,
     487,   488,   489,   484,   485,   486,   492,   493,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   743,   708,   487,   488,   489,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   490,   491,   484,   485,   486,
       0,     0,     0,     0,     0,   492,   493,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,   487,   488,   489,
     748,   708,     0,     0,     0,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   484,   485,   486,   492,
     493,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   750,   708,   487,   488,   489,   484,
     485,   486,   492,   493,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,   752,   708,   487,
     488,   489,     0,     0,     0,     0,     0,     0,   484,   485,
     486,   490,   491,     0,     0,     0,   492,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,   487,   488,
     489,   755,   708,     0,     0,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,   484,   485,   486,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   492,   493,     0,     0,     0,
       0,     0,     0,     0,     0,   487,   488,   489,   494,   495,
     496,   497,   498,   499,   500,     0,   501,   490,   491,     0,
     757,   708,     0,     0,     0,     0,     0,     0,   492,   493,
     484,   485,   486,     0,     0,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   759,   708,     0,     0,     0,     0,     0,
     487,   488,   489,     0,   484,   485,   486,   492,   493,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,   766,   708,   487,   488,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   490,   491,     0,     0,
       0,   484,   485,   486,   492,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   768,
     708,   487,   488,   489,     0,     0,     0,     0,     0,   484,
     485,   486,     0,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   492,
     493,     0,     0,     0,     0,     0,     0,     0,     0,   487,
     488,   489,   494,   495,   496,   497,   498,   499,   500,     0,
     501,   490,   491,     0,   770,   708,     0,     0,     0,     0,
     484,   485,   486,   492,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   774,   708,
     487,   488,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
     492,   493,   484,   485,   486,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   776,   708,     0,     0,     0,
       0,     0,   487,   488,   489,   484,   485,   486,   492,   493,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   778,   708,   487,   488,   489,     0,   484,
     485,   486,     0,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   492,
     493,   484,   485,   486,     0,     0,     0,     0,     0,   487,
     488,   489,   494,   495,   496,   497,   498,   499,   500,     0,
     501,   490,   491,     0,   780,   708,     0,     0,     0,     0,
       0,   487,   488,   489,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   490,   491,     0,     0,     0,   484,   485,
     486,   492,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   783,   708,   487,   488,
     489,   484,   485,   486,   492,   493,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   785,
     708,   487,   488,   489,     0,     0,     0,     0,   492,   493,
     484,   485,   486,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
     492,   493,     0,   787,   708,     0,     0,   484,   485,   486,
     487,   488,   489,   494,   495,   496,   497,   498,   499,   500,
       0,   501,   490,   491,     0,   789,   708,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   487,   488,   489,
       0,     0,     0,     0,   484,   485,   486,   492,   493,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,   793,   708,   487,   488,   489,     0,     0,     0,
     492,   493,   484,   485,   486,     0,   490,   491,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   795,   708,     0,     0,     0,
       0,     0,   487,   488,   489,     0,     0,     0,     0,   492,
     493,     0,     0,     0,   490,   491,   484,   485,   486,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   798,   708,   492,   493,   484,   485,
     486,     0,     0,     0,     0,     0,   487,   488,   489,   494,
     495,   496,   497,   498,   499,   500,     0,   501,   490,   491,
       0,   889,   708,     0,     0,   484,   485,   486,   487,   488,
     489,     0,     0,   492,   493,     0,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,   487,   488,   489,   890,   708,
       0,     0,     0,   484,   485,   486,     0,   490,   491,     0,
       0,   492,   493,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,   487,   488,   489,   891,   708,     0,     0,
       0,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,   492,   493,   484,   485,   486,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,   492,   493,     0,
     892,   708,     0,     0,   484,   485,   486,   487,   488,   489,
     494,   495,   496,   497,   498,   499,   500,     0,   501,   490,
     491,     0,     0,   708,   492,   493,   484,   485,   486,     0,
       0,     0,     0,     0,   487,   488,   489,   494,   495,   496,
     497,   498,   499,   500,     0,   501,   490,   491,     0,     0,
     812,     0,     0,     0,     0,     0,   487,   488,   489,   484,
     485,   486,   492,   493,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,     0,   813,   487,
     488,   489,     0,     0,     0,     0,   484,   485,   486,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   492,   493,     0,     0,
       0,     0,     0,     0,     0,     0,   487,   488,   489,   494,
     495,   496,   497,   498,   499,   500,     0,   501,   490,   491,
       0,     0,   815,   492,   493,   484,   485,   486,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,   492,   493,     0,     0,   823,
       0,     0,     0,     0,     0,   487,   488,   489,   494,   495,
     496,   497,   498,   499,   500,     0,   501,   490,   491,     0,
       0,   824,     0,     0,     0,   484,   485,   486,   492,   493,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,     0,   825,   487,   488,   489,   484,   485,
     486,     0,     0,     0,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,   487,   488,
     489,   826,     0,     0,   484,   485,   486,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   492,   493,   484,   485,   486,     0,
       0,     0,     0,     0,   487,   488,   489,   494,   495,   496,
     497,   498,   499,   500,     0,   501,   490,   491,     0,     0,
     827,     0,     0,     0,     0,     0,   487,   488,   489,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   490,   491,
       0,   484,   485,   486,   492,   493,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,     0,
     828,   487,   488,   489,   484,   485,   486,   492,   493,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,     0,   829,   487,   488,   489,     0,     0,     0,
       0,     0,     0,   492,   493,     0,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,   492,   493,     0,     0,   830,
     484,   485,   486,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
       0,   831,     0,     0,     0,     0,     0,     0,     0,     0,
     487,   488,   489,   484,   485,   486,     0,     0,     0,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,   487,   488,   489,   832,     0,     0,     0,
     484,   485,   486,   492,   493,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,     0,   833,
     487,   488,   489,   484,   485,   486,     0,     0,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   487,   488,   489,   484,   485,   486,   492,
     493,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,     0,   834,   487,   488,   489,   484,
     485,   486,   492,   493,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,     0,   835,   487,
     488,   489,   484,   485,   486,     0,     0,     0,     0,   492,
     493,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,   487,   488,   489,   837,     0,     0,     0,   484,
     485,   486,   492,   493,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,     0,   838,   487,
     488,   489,   484,   485,   486,   492,   493,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
       0,   840,   487,   488,   489,   484,   485,   486,   492,   493,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,     0,   841,   487,   488,   489,   484,   485,
     486,   492,   493,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,     0,   844,   487,   488,
     489,   484,   485,   486,     0,     0,     0,     0,   492,   493,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,   487,   488,   489,   845,     0,     0,     0,   484,   485,
     486,   492,   493,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,     0,   846,   487,   488,
     489,   484,   485,   486,   492,   493,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,     0,
     847,   487,   488,   489,   484,   485,   486,   492,   493,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,     0,   848,   487,   488,   489,   484,   485,   486,
     492,   493,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,     0,   849,   487,   488,   489,
     484,   485,   486,     0,     0,     0,     0,   492,   493,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
     487,   488,   489,   851,     0,     0,     0,   484,   485,   486,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,     0,   852,   487,   488,   489,
     484,   485,   486,   492,   493,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,     0,   853,
     487,   488,   489,   484,   485,   486,   492,   493,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,     0,   854,   487,   488,   489,   484,   485,   486,   492,
     493,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,     0,   855,   487,   488,   489,   484,
     485,   486,     0,     0,     0,     0,   492,   493,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,   487,
     488,   489,   858,     0,     0,     0,   484,   485,   486,   492,
     493,   490,   491,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,     0,   946,   487,   488,   489,   484,
     485,   486,   492,   493,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,     0,   957,   487,
     488,   489,   484,   485,   486,   492,   493,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
       0,   967,   487,   488,   489,   484,   485,   486,   492,   493,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,     0,   986,   487,   488,   489,   484,   485,
     486,     0,     0,     0,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,   487,   488,
     489,   987,     0,     0,     0,   484,   485,   486,   492,   493,
     490,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,     0,   989,   487,   488,   489,   484,   485,
     486,   492,   493,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,     0,   995,   487,   488,
     489,   484,   485,   486,   492,   493,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,     0,
    1004,   487,   488,   489,   484,   485,   486,   492,   493,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,     0,  1007,   487,   488,   489,   484,   485,   486,
       0,     0,     0,     0,   492,   493,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,   487,   488,   489,
    1008,     0,     0,     0,   484,   485,   486,   492,   493,   490,
     491,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,     0,  1011,   487,   488,   489,   484,   485,   486,
     492,   493,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,     0,  1027,   487,   488,   489,
     484,   485,   486,   492,   493,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,     0,  1028,
     487,   488,   489,   484,   485,   486,   492,   493,     0,     0,
       0,     0,   490,   491,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,     0,  1029,   487,   488,   489,   484,   485,   486,     0,
       0,     0,     0,   492,   493,   490,   491,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,   487,   488,   489,  1030,
       0,     0,     0,   484,   485,   486,   492,   493,   490,   491,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,     0,  1050,   487,   488,   489,   484,   485,   486,   492,
     493,     0,     0,     0,     0,   490,   491,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,     0,  1061,   487,   488,   489,   484,
     485,   486,   492,   493,     0,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,     0,     0,     0,  1062,   487,
     488,   489,   484,   485,   486,   492,   493,     0,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
       0,  1064,   487,   488,   489,   484,   485,   486,     0,     0,
       0,     0,   492,   493,   490,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   494,   495,   496,   497,   498,
     499,   500,     0,   501,     0,   487,   488,   489,  1072,     0,
       0,     0,   484,   485,   486,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
       0,  1075,   487,   488,   489,   484,   485,   486,   492,   493,
       0,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,     0,  1076,   487,   488,   489,   484,   485,
     486,   492,   493,     0,     0,     0,     0,   490,   491,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,     0,  1080,   487,   488,
     489,   484,   485,   486,   492,   493,     0,     0,     0,     0,
     490,   491,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,     0,
    1081,   487,   488,   489,   484,   485,   486,     0,     0,     0,
       0,   492,   493,   490,   491,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,   487,   488,   489,  1095,     0,     0,
       0,   484,   485,   486,   492,   493,   490,   491,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,     0,
    1100,   487,   488,   489,   484,   485,   486,   492,   493,     0,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
     494,   495,   496,   497,   498,   499,   500,     0,   501,     0,
       0,     0,     0,  1101,   487,   488,   489,   484,   485,   486,
     492,   493,     0,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,     0,  1107,   487,   488,   489,
     484,   485,   486,   492,   493,     0,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,     0,  1113,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   514,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   516,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   518,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   523,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
     525,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   527,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   530,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   532,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   536,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   547,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   549,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   551,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
     555,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   557,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   559,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   561,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   562,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   564,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   568,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   573,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
     576,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   579,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   585,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   747,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   810,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   811,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   842,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   843,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
     875,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   877,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   880,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   882,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   888,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   896,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   898,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   900,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
     902,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   906,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   907,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   909,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   947,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   949,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   951,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   955,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
     961,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,   969,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,   972,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,   974,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,   976,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,   979,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,   981,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,   984,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
    1005,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,  1006,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,  1010,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,  1019,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,  1022,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,  1024,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,  1038,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,  1041,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
    1051,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,  1054,   487,   488,   489,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,  1058,   487,   488,   489,
     484,   485,   486,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,  1068,
     487,   488,   489,     0,     0,     0,   484,   485,   486,     0,
     492,   493,   490,   491,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,  1073,   487,   488,   489,   484,
     485,   486,     0,   492,   493,     0,     0,     0,   490,   491,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,  1082,   487,
     488,   489,   484,   485,   486,     0,   492,   493,     0,     0,
       0,   490,   491,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501,     0,     0,
       0,  1086,   487,   488,   489,   484,   485,   486,     0,   492,
     493,     0,     0,     0,   490,   491,     0,     0,     0,     0,
       0,     0,   494,   495,   496,   497,   498,   499,   500,     0,
     501,     0,     0,     0,  1091,   487,   488,   489,     0,     0,
       0,   484,   485,   486,     0,   492,   493,   490,   491,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   494,   495,
     496,   497,   498,   499,   500,     0,   501,     0,     0,     0,
    1093,   487,   488,   489,   484,   485,   486,     0,   492,   493,
       0,     0,     0,   490,   491,     0,     0,     0,     0,     0,
       0,   494,   495,   496,   497,   498,   499,   500,     0,   501,
       0,     0,     0,  1097,   487,   488,     0,   484,   485,   486,
       0,   492,   493,     0,     0,     0,   490,   491,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   497,   498,   499,
     500,     0,   501,     0,     0,     0,  1102,   487,     0,     0,
       0,     0,     0,     0,   492,   493,     0,     0,     0,   490,
     491,     0,     0,     0,     0,     0,     0,   494,   495,   496,
     497,   498,   499,   500,     0,   501,     0,     0,     0,  1105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     492,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,   495,   496,   497,   498,   499,   500,
       0,   501,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   492,   493,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,   499,   500,     0,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   492,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   494,
     495,   496,   497,   498,   499,   500,     0,   501
};

static const yytype_int16 yycheck[] =
{
       1,   189,     3,     4,     5,   189,     7,     8,    35,    10,
      42,    43,    44,    14,   196,   189,    17,   189,   190,    20,
     196,   205,    23,    24,    25,    26,   208,   211,    84,    85,
      31,   189,   208,    34,    35,    36,    37,   211,    39,   189,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     203,   204,   189,    54,   205,   208,    57,   203,   204,    19,
     189,   190,   208,    23,    65,    66,    67,    68,   203,   204,
      71,   189,   190,   208,   205,     5,     6,     7,   203,   204,
     203,   204,     0,   208,   206,   208,   206,   209,   189,   209,
     207,   208,   203,   204,   203,   204,   203,   204,    42,    43,
      44,   205,    32,   203,   204,   207,   208,   205,   207,   208,
     205,   203,    20,   204,   208,   208,   206,   205,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,   205,   199,   200,    75,   202,   205,    78,    79,
      80,    81,   205,    83,   205,   205,    86,    87,    88,    89,
      90,    91,   194,   195,   196,   205,   198,    97,    98,    99,
     205,   205,    42,    43,    44,   205,   205,   205,   210,   109,
     110,   111,   112,   113,   114,   205,   205,   205,   118,   190,
     120,   121,   122,   123,   124,   125,   205,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     205,   151,   205,   205,   205,    42,    43,    44,   158,   159,
     160,   161,   205,   205,   164,   165,   166,   167,   205,   205,
     170,   205,   205,   173,   174,   196,   176,   205,   178,   179,
     194,   195,   196,   183,   198,   256,   186,   187,   188,   189,
     190,   262,   263,   207,   205,   205,   205,   197,   205,   270,
     205,   205,   273,   205,   275,   205,   206,   205,   279,   205,
     281,   205,   283,   213,   285,    42,    43,    44,   205,   205,
     205,   292,   205,   205,   295,   205,   205,   298,   205,   300,
      42,    43,    44,   205,   205,   306,   205,   308,   309,   310,
     311,   205,   205,   314,   205,   205,   317,   205,   205,   320,
     205,   322,   205,   205,   194,   195,   196,   328,   198,   330,
      84,    85,   333,   205,   335,   189,   206,   207,   205,   340,
     205,   205,   205,   205,   345,   205,   205,   374,   205,    42,
      43,    44,   205,   205,   205,   356,   205,   358,   205,   205,
     361,   205,   205,   364,   205,   366,   205,   368,   205,   370,
     205,   372,   205,   374,   375,   205,   377,   194,   195,   196,
     205,   198,   205,   205,   385,   386,   387,   388,   389,   206,
     207,   205,   205,   205,   205,   205,   397,   398,   205,   400,
     401,   402,   403,   404,   405,   406,   205,   408,   409,   410,
     411,   205,   205,   414,   415,   416,   417,   205,   207,   198,
     207,    42,    43,    44,   207,   426,   427,   428,   429,   430,
     431,   432,   433,   207,   207,   207,   207,   194,   195,   196,
     207,   198,   202,   197,   198,   199,   200,   212,   202,   206,
     207,   207,   194,   195,   196,   207,   198,   207,    42,    43,
      44,   207,   207,   207,   465,   466,   467,   207,   207,   207,
     207,   472,   473,   207,   207,   207,   477,   207,   479,   480,
     207,   207,   207,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   499,   500,
     501,   194,   195,   196,   207,   198,   207,   207,   509,   207,
     511,   512,   207,   514,   207,   516,   207,   518,   207,   207,
     207,   207,   523,   207,   525,   207,   527,   528,   207,   207,
     207,   207,   207,   207,   207,   536,   537,   207,   539,   207,
     541,   207,   543,     5,     6,     7,   547,   207,   207,   207,
     551,   207,   553,   207,   555,   208,   557,   208,   213,   196,
     561,   562,   212,   564,   213,   196,   209,   568,   209,   209,
      32,   209,   573,   194,   195,   196,   577,   198,   196,   196,
     189,   207,   379,    78,   585,   206,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
     194,   195,   196,    75,   198,   381,    78,    79,    80,    81,
     382,    83,   206,   207,    86,    87,    88,    89,    90,    91,
     924,   747,    -1,    -1,    -1,    97,    98,    99,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,
     122,   123,   124,   125,    -1,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
      -1,    42,    43,    44,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,    -1,    -1,
      -1,   183,    84,    85,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,   739,    -1,
     741,    -1,   743,   205,    -1,   207,   747,   748,    -1,   750,
      -1,   752,    -1,    -1,    -1,    -1,    -1,    -1,   759,    -1,
      -1,    -1,    42,    43,    44,   766,    -1,    -1,    -1,   770,
      -1,   772,    -1,   774,    -1,   776,    -1,   778,    -1,   780,
      -1,    -1,   783,    -1,   785,    -1,   787,    -1,    -1,   790,
     194,   195,   196,    -1,   198,    -1,    -1,   798,    -1,    -1,
      -1,   802,   206,   207,    -1,   806,    -1,   808,   809,   810,
     811,    42,    43,    44,    -1,    -1,   817,   818,   819,   181,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   197,   198,   199,   200,    -1,
     202,   842,   843,   194,   195,   196,    -1,   198,    -1,    -1,
      -1,    -1,    -1,   880,    -1,    -1,   207,    -1,    -1,    -1,
      -1,   862,    -1,    -1,    -1,    -1,   867,    -1,   869,    -1,
     871,    -1,    -1,    -1,   875,    -1,   877,    -1,    -1,   880,
      -1,   882,    -1,   884,    -1,    -1,    -1,   888,   889,   890,
     891,   892,    -1,    -1,    -1,   896,    -1,   898,    -1,    -1,
      -1,   902,    -1,    -1,    -1,   906,   907,    -1,   909,    42,
      43,    44,    -1,   914,   194,   195,   196,    -1,   198,    -1,
      -1,   922,    -1,    -1,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,   947,    -1,   949,    -1,
     951,     1,     2,    -1,   955,    42,    43,    44,    -1,    -1,
     961,    -1,    -1,   194,   195,   196,    -1,   198,   969,    -1,
      -1,   972,    -1,    23,    24,   976,   207,    -1,   979,    -1,
     981,    -1,    28,   984,    -1,    -1,    36,   988,    38,    39,
      40,    42,    43,    44,    -1,  1022,    42,    43,    44,    -1,
      -1,    51,    -1,    -1,  1005,  1006,    56,    -1,    58,  1010,
      60,    61,    42,    43,    44,    -1,    66,    67,  1019,    69,
      70,  1022,    72,  1024,  1025,    -1,    72,    73,    74,    -1,
      -1,    -1,    29,    -1,  1035,    -1,    -1,  1038,    84,    85,
    1041,    -1,    -1,    -1,  1045,    42,    43,    44,    -1,    -1,
    1051,    -1,    -1,  1054,    42,    43,    44,  1058,    -1,  1086,
      -1,   194,   195,   196,  1065,   198,    -1,  1068,    -1,    -1,
      -1,    -1,  1073,    -1,   207,    72,    73,    74,    -1,    -1,
      -1,  1082,    -1,    -1,    72,    73,    74,    84,    85,   194,
     195,   196,  1093,   198,    -1,    -1,    84,    85,    -1,    42,
      43,    44,   207,    -1,  1105,    -1,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,
      42,    43,    44,   194,   195,   196,    -1,   198,   194,   195,
     196,   197,   198,   199,   200,   206,   202,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,    -1,   198,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,   194,   195,   196,   197,
     198,   199,   200,   263,   202,    42,    43,    44,    -1,    -1,
     270,    -1,   210,   273,    -1,   275,    -1,    -1,    -1,    -1,
      -1,   281,    -1,   283,    -1,   285,    -1,    -1,    -1,    42,
      43,    44,   292,    -1,    -1,   295,    -1,    -1,   298,    -1,
     300,   194,   195,   196,    -1,   198,   306,    42,    43,    44,
      -1,    -1,    -1,   206,   314,    -1,    -1,   317,    -1,    -1,
     320,    -1,   322,   194,   195,   196,    -1,   198,   328,    -1,
     330,    -1,    -1,   333,    -1,   335,   207,    72,    73,    74,
     340,    -1,   194,   195,   196,   345,   198,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,   207,   356,    -1,   358,    -1,
      -1,   361,    -1,    -1,   364,    -1,   366,    -1,   368,    -1,
     370,    -1,   372,   194,   195,   196,   376,   198,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,   207,    -1,   388,    -1,
     390,   391,   392,   393,   394,   395,   396,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,   413,    -1,   415,    -1,   194,   195,   196,
      -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,    -1,    -1,    -1,   434,    -1,    -1,    -1,    72,    73,
      74,   194,   195,   196,    -1,   198,   181,   182,    -1,    -1,
      84,    85,    -1,    -1,   207,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    -1,    -1,   209,   475,   476,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,   498,    -1,
      -1,    -1,    -1,   503,   504,   505,   506,   507,   508,   509,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     530,    -1,   532,    -1,   194,   195,   196,   537,   198,   539,
      -1,   541,    -1,   543,    -1,    -1,   206,   181,   182,   549,
       5,     6,     7,    -1,   194,   195,   196,    -1,   198,   559,
     194,   195,   196,   197,   198,   199,   200,   207,   202,    -1,
      -1,    -1,   206,    -1,    -1,   209,   576,    32,    -1,   579,
      -1,    -1,    -1,   583,    42,    43,    44,   587,    -1,    -1,
      -1,   591,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    42,    43,    44,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,   194,   195,   196,
      -1,   198,    97,    98,    99,    -1,    -1,   194,   195,   196,
     207,   198,    -1,    -1,   109,   110,   111,   112,   113,   114,
     207,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,   174,
      -1,   176,    -1,   178,   179,    -1,    -1,    -1,   183,    -1,
      -1,   186,   187,   188,   189,   190,   194,   195,   196,    -1,
     198,    -1,   197,    -1,    -1,    42,    43,    44,    -1,   207,
     205,   206,    42,    43,    44,   755,    -1,   757,   213,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   768,   194,
     195,   196,    -1,   198,    -1,    -1,    -1,    -1,   778,    -1,
      -1,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,   789,
      -1,    -1,    -1,   793,    -1,   795,   796,    -1,    -1,    -1,
     800,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   822,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,     3,     4,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,   857,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    30,    31,    -1,    33,    34,
      35,    36,    37,    38,    -1,    40,    41,    -1,    -1,    -1,
      45,    46,    -1,    42,    43,    44,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,   896,   194,   195,   196,
     900,   198,   902,    -1,   194,   195,   196,    -1,   198,    -1,
     207,    76,    77,    42,    43,    44,   206,    82,   918,    42,
      43,    44,    -1,    -1,   924,    -1,   926,    92,    93,    94,
      95,    96,    42,    43,    44,   100,   101,   102,   103,   104,
     105,   106,   107,   108,    -1,    -1,    -1,    -1,    -1,    -1,
     115,   116,   117,    -1,   119,   194,   195,   196,    -1,   198,
      -1,   126,   194,   195,   196,    -1,   198,    -1,   207,   969,
      -1,    -1,    -1,    -1,   974,   207,   976,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   150,   198,   152,   153,   154,
     155,   156,   157,   993,    -1,   207,    -1,   162,   163,    -1,
      -1,    -1,    -1,   168,   169,    -1,   171,   172,    -1,    -1,
     175,    -1,   177,    -1,    -1,   180,    -1,    -1,    -1,   184,
      -1,    -1,    -1,    -1,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,  1038,   198,
     194,   195,   196,    -1,   198,    -1,  1046,   206,    -1,    -1,
      -1,    -1,   206,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       5,     6,     7,    -1,    -1,   194,   195,   196,  1068,   198,
      -1,   194,   195,   196,    -1,   198,    -1,   206,    -1,    -1,
      -1,    -1,    -1,   206,   194,   195,   196,    32,   198,    -1,
      -1,  1091,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,
      -1,    -1,  1102,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      42,    43,    44,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,   174,
      -1,   176,    -1,   178,   179,     5,     6,     7,   183,    -1,
      -1,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,    -1,    32,    -1,    -1,    -1,   211,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,   194,   195,   196,    -1,   198,    97,    98,    99,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    42,    43,    44,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,    -1,   173,   174,    -1,   176,    -1,   178,   179,
       5,     6,     7,   183,    -1,    -1,   186,   187,   188,   189,
     190,   194,   195,   196,    -1,   198,    -1,   197,    -1,    -1,
      -1,    -1,    -1,   206,    -1,   205,    -1,    32,    -1,    -1,
      -1,   211,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,   194,   195,   196,
      -1,   198,    97,    98,    99,    -1,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      42,    43,    44,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,   174,
      -1,   176,    -1,   178,   179,     5,     6,     7,   183,    -1,
      -1,   186,   187,   188,   189,   190,   194,   195,   196,    -1,
     198,    -1,   197,    -1,    -1,    -1,    -1,    -1,   206,    -1,
     205,    -1,    32,    -1,    -1,    -1,   211,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,   194,   195,   196,    -1,   198,    97,    98,    99,
      -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,    -1,   173,   174,    -1,   176,    -1,   178,   179,
       5,     6,     7,   183,    -1,    -1,   186,   187,   188,   189,
     190,    -1,    -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,    -1,    32,    -1,    -1,
      -1,   211,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     145,   146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,   174,
      -1,   176,    -1,   178,   179,     5,     6,     7,   183,    -1,
      -1,   186,   187,   188,   189,   190,    -1,    -1,    -1,    -1,
      -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     205,    -1,    32,    -1,    -1,    -1,   211,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,    -1,   173,   174,    -1,   176,    -1,   178,   179,
      -1,    -1,    -1,   183,    -1,    -1,   186,   187,   188,   189,
     190,     5,     6,     7,    -1,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,
     174,    -1,   176,    -1,   178,   179,    -1,    -1,    -1,   183,
      -1,    -1,   186,   187,   188,   189,   190,     5,     6,     7,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,    -1,
     178,   179,    -1,    -1,    -1,   183,    -1,    -1,   186,   187,
     188,   189,   190,     5,     6,     7,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,    -1,    -1,
      -1,   183,    -1,    -1,   186,   187,   188,   189,   190,     5,
       6,     7,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,   207,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,
     176,    -1,   178,   179,    -1,    -1,    -1,   183,    -1,    -1,
     186,   187,   188,   189,   190,     5,     6,     7,    -1,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,    -1,   173,   174,    -1,   176,    -1,   178,   179,
      -1,    -1,    -1,   183,    -1,    -1,   186,   187,   188,   189,
     190,     5,     6,     7,    -1,    -1,    -1,   197,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   205,    -1,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,
     174,    -1,   176,    -1,   178,   179,    -1,    -1,    -1,   183,
      -1,    -1,   186,   187,   188,   189,   190,     5,     6,     7,
      -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   205,    -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,    -1,
     178,   179,    -1,    -1,    -1,   183,    -1,    -1,   186,   187,
     188,   189,   190,     5,     6,     7,    -1,    -1,    -1,   197,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,     5,     6,
       7,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   205,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,   174,    -1,   176,
      -1,   178,   179,     5,     6,     7,   183,    -1,    -1,   186,
     187,   188,   189,   190,    -1,    -1,    -1,    -1,    -1,    -1,
     197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   205,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,   174,    -1,   176,    -1,   178,   179,    -1,    -1,
      -1,   183,    -1,    -1,   186,   187,   188,   189,   190,    -1,
      -1,    -1,    -1,    -1,    -1,   197,    -1,    -1,    -1,     3,
       4,    -1,    -1,   205,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    -1,    40,    41,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,    93,
      94,    95,    96,    -1,    -1,    -1,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    -1,    -1,    -1,    -1,    -1,
      -1,   115,   116,   117,    -1,   119,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,    -1,    -1,    -1,    -1,   162,   163,
      -1,    -1,    -1,    -1,   168,   169,    -1,   171,   172,    -1,
      -1,   175,    -1,   177,    -1,    -1,   180,    -1,    -1,    -1,
     184,    -1,    -1,    -1,    -1,   189,   190,    -1,    -1,   193,
      -1,    -1,    -1,    -1,    -1,     3,     4,    -1,    -1,   203,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      -1,    -1,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    93,    94,    95,    96,    -1,
      -1,    -1,   100,   101,   102,   103,   104,   105,   106,   107,
     108,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,   117,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   150,    -1,   152,   153,   154,   155,   156,   157,
      -1,    -1,    -1,    -1,   162,   163,    -1,    -1,    -1,    -1,
     168,   169,    -1,   171,   172,    -1,    -1,   175,    -1,   177,
      -1,    -1,   180,    -1,    -1,    -1,   184,    -1,    -1,    -1,
      -1,   189,   190,     3,     4,   193,    -1,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    -1,    -1,
      30,    31,    -1,    33,    34,    35,    36,    37,    38,    -1,
      40,    41,    -1,    -1,    -1,    45,    46,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    72,    73,
      74,    -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    92,    93,    94,    95,    96,    42,    43,    44,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,    -1,   119,
      -1,    -1,    -1,    42,    43,    44,   126,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     150,    -1,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,   162,   163,    -1,    84,    85,    -1,   168,   169,
      -1,   171,   172,    -1,    -1,   175,    -1,   177,    -1,    -1,
     180,    -1,    -1,    -1,   184,    -1,    -1,   181,   182,   189,
     190,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,   206,    -1,    -1,   209,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    84,    85,    -1,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    72,    73,
      74,   206,   181,   182,   209,    42,    43,    44,    -1,    -1,
      84,    85,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,    84,    85,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    -1,    -1,   209,    42,
      43,    44,    -1,    -1,    72,    73,    74,   181,   182,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    72,
      73,    74,   206,    -1,    -1,   209,    42,    43,    44,    -1,
      -1,    84,    85,    -1,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    72,    73,    74,   181,
     182,    -1,   209,    42,    43,    44,    -1,    -1,    84,    85,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    84,    85,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   181,   182,
      -1,   209,    42,    43,    44,    -1,    -1,    -1,    72,    73,
      74,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      84,    85,    -1,    -1,    -1,    -1,   209,    42,    43,    44,
      -1,    -1,    72,    73,    74,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    72,    73,    74,
      -1,    -1,    -1,   209,    42,    43,    44,    -1,    -1,    84,
      85,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    72,    73,    74,    42,    43,    44,
     209,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    84,
      85,   181,   182,    -1,    -1,   209,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   181,   182,    -1,   209,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    84,    85,
      -1,   206,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,   207,
      72,    73,    74,    42,    43,    44,   181,   182,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,   207,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    72,    73,    74,
     206,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,   181,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,   207,    72,    73,    74,    42,
      43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,   206,   207,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    84,    85,    -1,    -1,    -1,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    72,    73,
      74,   206,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    84,    85,    -1,
     206,   207,    -1,    -1,    -1,    -1,    -1,    -1,   181,   182,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,   207,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    42,    43,    44,   181,   182,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,   206,   207,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
     207,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    84,    85,    -1,   206,   207,    -1,    -1,    -1,    -1,
      42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,   207,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,   207,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    42,    43,    44,   181,   182,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,   207,    72,    73,    74,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   181,
     182,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    84,    85,    -1,   206,   207,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    42,    43,
      44,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,   207,    72,    73,
      74,    42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
     207,    72,    73,    74,    -1,    -1,    -1,    -1,   181,   182,
      42,    43,    44,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
     181,   182,    -1,   206,   207,    -1,    -1,    42,    43,    44,
      72,    73,    74,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    84,    85,    -1,   206,   207,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    42,    43,    44,   181,   182,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,   206,   207,    72,    73,    74,    -1,    -1,    -1,
     181,   182,    42,    43,    44,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,   207,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    42,    43,    44,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,   207,   181,   182,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    84,    85,
      -1,   206,   207,    -1,    -1,    42,    43,    44,    72,    73,
      74,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    72,    73,    74,   206,   207,
      -1,    -1,    -1,    42,    43,    44,    -1,    84,    85,    -1,
      -1,   181,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    72,    73,    74,   206,   207,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,   182,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,   181,   182,    -1,
     206,   207,    -1,    -1,    42,    43,    44,    72,    73,    74,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    84,
      85,    -1,    -1,   207,   181,   182,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    84,    85,    -1,    -1,
     207,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    42,
      43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,
      73,    74,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    84,    85,
      -1,    -1,   207,   181,   182,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,   181,   182,    -1,    -1,   207,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    84,    85,    -1,
      -1,   207,    -1,    -1,    -1,    42,    43,    44,   181,   182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,    43,
      44,    -1,    -1,    -1,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    72,    73,
      74,   207,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    84,    85,    -1,    -1,
     207,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,
     207,    72,    73,    74,    42,    43,    44,   181,   182,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,    -1,   207,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,   181,   182,    -1,    -1,   207,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
      -1,   207,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    42,    43,    44,    -1,    -1,    -1,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    72,    73,    74,   207,    -1,    -1,    -1,
      42,    43,    44,   181,   182,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,
      72,    73,    74,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    42,    43,    44,   181,
     182,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,
      43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,
      73,    74,    42,    43,    44,    -1,    -1,    -1,    -1,   181,
     182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    72,    73,    74,   207,    -1,    -1,    -1,    42,
      43,    44,   181,   182,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,
      73,    74,    42,    43,    44,   181,   182,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
      -1,   207,    72,    73,    74,    42,    43,    44,   181,   182,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,    43,
      44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,
      74,    42,    43,    44,    -1,    -1,    -1,    -1,   181,   182,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    72,    73,    74,   207,    -1,    -1,    -1,    42,    43,
      44,   181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,
      74,    42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,
     207,    72,    73,    74,    42,    43,    44,   181,   182,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,    -1,   207,    72,    73,    74,    42,    43,    44,
     181,   182,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,
      42,    43,    44,    -1,    -1,    -1,    -1,   181,   182,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      72,    73,    74,   207,    -1,    -1,    -1,    42,    43,    44,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,
      42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,
      72,    73,    74,    42,    43,    44,   181,   182,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,    -1,   207,    72,    73,    74,    42,    43,    44,   181,
     182,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,
      43,    44,    -1,    -1,    -1,    -1,   181,   182,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    72,
      73,    74,   207,    -1,    -1,    -1,    42,    43,    44,   181,
     182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,
      43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,
      73,    74,    42,    43,    44,   181,   182,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
      -1,   207,    72,    73,    74,    42,    43,    44,   181,   182,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,    43,
      44,    -1,    -1,    -1,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    72,    73,
      74,   207,    -1,    -1,    -1,    42,    43,    44,   181,   182,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,    43,
      44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,
      74,    42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,
     207,    72,    73,    74,    42,    43,    44,   181,   182,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,    -1,   207,    72,    73,    74,    42,    43,    44,
      -1,    -1,    -1,    -1,   181,   182,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    72,    73,    74,
     207,    -1,    -1,    -1,    42,    43,    44,   181,   182,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,    -1,   207,    72,    73,    74,    42,    43,    44,
     181,   182,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,
      42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,
      72,    73,    74,    42,    43,    44,   181,   182,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,    -1,   207,    72,    73,    74,    42,    43,    44,    -1,
      -1,    -1,    -1,   181,   182,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    72,    73,    74,   207,
      -1,    -1,    -1,    42,    43,    44,   181,   182,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,    -1,   207,    72,    73,    74,    42,    43,    44,   181,
     182,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,
      43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,
      73,    74,    42,    43,    44,   181,   182,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
      -1,   207,    72,    73,    74,    42,    43,    44,    -1,    -1,
      -1,    -1,   181,   182,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,
     199,   200,    -1,   202,    -1,    72,    73,    74,   207,    -1,
      -1,    -1,    42,    43,    44,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
      -1,   207,    72,    73,    74,    42,    43,    44,   181,   182,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,    -1,   207,    72,    73,    74,    42,    43,
      44,   181,   182,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,
      74,    42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,
     207,    72,    73,    74,    42,    43,    44,    -1,    -1,    -1,
      -1,   181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    72,    73,    74,   207,    -1,    -1,
      -1,    42,    43,    44,   181,   182,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,
     207,    72,    73,    74,    42,    43,    44,   181,   182,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
     194,   195,   196,   197,   198,   199,   200,    -1,   202,    -1,
      -1,    -1,    -1,   207,    72,    73,    74,    42,    43,    44,
     181,   182,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,    -1,   207,    72,    73,    74,
      42,    43,    44,   181,   182,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,   207,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    74,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,
      42,    43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      72,    73,    74,    -1,    -1,    -1,    42,    43,    44,    -1,
     181,   182,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,   206,    72,    73,    74,    42,
      43,    44,    -1,   181,   182,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,    72,
      73,    74,    42,    43,    44,    -1,   181,   182,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202,    -1,    -1,
      -1,   206,    72,    73,    74,    42,    43,    44,    -1,   181,
     182,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,   194,   195,   196,   197,   198,   199,   200,    -1,
     202,    -1,    -1,    -1,   206,    72,    73,    74,    -1,    -1,
      -1,    42,    43,    44,    -1,   181,   182,    84,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,
     196,   197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,
     206,    72,    73,    74,    42,    43,    44,    -1,   181,   182,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,
      -1,   194,   195,   196,   197,   198,   199,   200,    -1,   202,
      -1,    -1,    -1,   206,    72,    73,    -1,    42,    43,    44,
      -1,   181,   182,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,   194,   195,   196,   197,   198,   199,
     200,    -1,   202,    -1,    -1,    -1,   206,    72,    -1,    -1,
      -1,    -1,    -1,    -1,   181,   182,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,
     197,   198,   199,   200,    -1,   202,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     181,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,   196,   197,   198,   199,   200,
      -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   181,   182,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,    -1,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   181,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,   196,   197,   198,   199,   200,    -1,   202
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
     155,   156,   157,   162,   163,   168,   169,   171,   172,   175,
     177,   180,   184,   189,   190,   193,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,     5,     6,     7,    32,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      75,    78,    79,    80,    81,    83,    86,    87,    88,    89,
      90,    91,    97,    98,    99,   109,   110,   111,   112,   113,
     114,   118,   120,   121,   122,   123,   124,   125,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   151,   158,   159,   160,   161,   164,   165,   166,   167,
     170,   173,   174,   176,   178,   179,   183,   186,   187,   188,
     189,   190,   197,   205,   304,   306,   189,   190,   304,   306,
     205,   304,   306,   205,   304,   205,   304,   189,   205,   211,
     302,   205,   304,   205,   304,   205,   304,   304,   304,   304,
     189,   189,   205,   304,   306,   205,   304,   306,   205,   304,
     205,   304,   189,   189,    19,    23,   205,   304,   189,   190,
     189,   190,   304,   189,   205,   302,   304,   205,   304,   306,
     205,   304,   205,   306,   304,   306,   306,   304,   205,   304,
     205,   304,   304,   205,   304,   205,   304,   304,   304,   304,
     205,   304,   306,   205,   205,   205,   304,   205,   306,   304,
     306,   205,   306,   306,   205,   189,   205,   304,   205,   304,
     306,   205,   304,   306,   205,   304,   205,   306,   205,   306,
     205,   304,   205,   306,   196,   208,   196,   208,     0,   203,
     218,   204,    20,   206,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   304,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   304,   208,
     208,   304,   304,   306,    42,    43,    44,    72,    73,    74,
      84,    85,   181,   182,   194,   195,   196,   197,   198,   199,
     200,   202,   210,    42,    43,    44,   194,   195,   196,   198,
     210,   208,   208,   304,   206,   304,   206,   304,   206,   189,
     303,   304,   304,   206,   304,   206,   304,   206,   196,   304,
     206,   304,   206,   207,   207,   304,   206,   205,   304,   205,
     304,   205,   304,   205,   304,   189,   304,   206,   304,   206,
     304,   206,   306,   206,   304,   206,   304,   206,   304,   206,
     304,   206,   206,   304,   206,   207,   207,   304,   206,   207,
     207,   207,   304,   206,   304,   306,   206,   206,   304,   206,
     207,   304,   306,   206,   304,   206,   306,   206,   302,   304,
     304,   211,   301,   306,   304,   215,    40,   230,   229,   189,
     190,   207,   304,   304,   207,   304,   304,   306,   304,   306,
     306,   306,   306,   306,   306,   306,   304,   304,   207,   304,
     304,   304,   304,   304,   304,   304,   207,   304,   304,   304,
     304,   306,   304,   304,   306,   207,   304,   207,   304,   207,
     207,   207,   207,   207,   207,   207,   207,   304,   304,   304,
     304,   304,   304,   304,   207,   304,   306,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   304,   304,   304,
     207,   207,   207,   207,   207,   304,   207,   304,   207,   306,
     306,   304,   206,   213,   304,   206,   213,   304,   207,   207,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   306,   304,   304,   304,   306,
     306,   306,   306,   306,   306,   304,   306,   304,   304,   206,
     304,   206,   304,   206,   304,   207,   212,   206,   206,   304,
     206,   304,   206,   304,   304,   206,   306,   206,   306,   206,
     304,   304,   304,   304,   304,   207,   206,   304,   206,   306,
     206,   304,   206,   304,   206,   304,   206,   304,   206,   306,
     206,   304,   304,   206,   304,   206,   304,   206,   304,   206,
     206,   306,   304,   206,   306,   206,   206,   306,   206,   304,
     206,   306,   206,   209,   305,   306,   206,   209,   208,   208,
     206,   206,   207,   207,   207,   207,   207,   206,   206,   206,
     207,   207,   206,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   206,   206,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   206,   207,   213,
     206,   209,   206,   209,   213,   206,   209,   206,   209,   206,
     209,   206,   209,   304,   304,   206,   304,   206,   303,   304,
     206,   304,   206,   304,    28,   306,   306,   304,   206,   206,
     206,   206,   206,   304,   306,   304,   206,   304,   206,   304,
     206,   304,   206,   304,   306,   304,   206,   206,   304,   206,
     304,   304,   306,   304,   206,   306,   306,   306,   206,   304,
     306,   304,   196,   212,   206,   304,   196,   304,   304,   304,
     304,   304,   304,   304,   306,   304,   304,   306,   209,   209,
     304,   209,   209,   304,   304,   304,   207,   206,   304,   206,
     304,   206,   189,   302,   304,   206,   304,   207,   304,   207,
     207,   206,   304,   304,   304,   304,   304,   207,   207,   206,
     304,   306,   206,   304,   206,   306,   206,   304,   306,   206,
     207,   206,   304,   304,   206,   304,   207,   207,   206,   207,
     304,   207,   207,   206,   306,   207,   207,   209,   304,   305,
     209,   306,   209,   209,   207,   206,   206,   207,   207,   207,
     206,   207,   207,   209,   209,   209,   209,   304,   304,   206,
     189,   304,   206,   304,   206,    29,   304,   207,   207,   207,
     207,   304,   306,   304,   306,   206,   304,   306,   206,   304,
     304,   206,   304,   304,   306,   196,   196,   304,   304,   304,
     207,   206,   304,   207,   206,   189,   302,   304,   206,   304,
     304,   207,   207,   207,   207,   206,   207,   304,   206,   207,
     304,   306,   207,   206,   304,   207,   207,   207,   304,   306,
     207,   207,   206,   304,   189,   304,   206,   304,   304,   304,
     306,   206,   304,   206,   304,   207,   207,   206,   189,   302,
     207,   207,   206,   207,   306,   206,   304,   207,   189,   306,
     304,   207,   207,   207
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
#line 330 "basicParse.y"
    { labeltable[(yyvsp[(1) - (1)].number)] = byteOffset; lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 332 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 333 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 334 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 335 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 336 "basicParse.y"
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
#line 343 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 344 "basicParse.y"
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
#line 351 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 352 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 353 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 357 "basicParse.y"
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
#line 372 "basicParse.y"
    {
		// there is nothing to do with a multi line if (ifexp handles it)
	;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 378 "basicParse.y"
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
#line 402 "basicParse.y"
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
#line 417 "basicParse.y"
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
#line 433 "basicParse.y"
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
#line 451 "basicParse.y"
    { 
		 // need nothing done at top of a do
         ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 458 "basicParse.y"
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

  case 99:

/* Line 1455 of yacc.c  */
#line 541 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 542 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 543 "basicParse.y"
    { addIntOp(OP_DIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 544 "basicParse.y"
    { addIntOp(OP_DIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 547 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 548 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 549 "basicParse.y"
    { addIntOp(OP_REDIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 550 "basicParse.y"
    { addIntOp(OP_REDIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 553 "basicParse.y"
    { addOp(OP_PAUSE); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 556 "basicParse.y"
    { addOp(OP_CLS); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 557 "basicParse.y"
    { addOp(OP_CLG); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 560 "basicParse.y"
    { addOp(OP_FASTGRAPHICS); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 563 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 564 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 567 "basicParse.y"
    { addOp(OP_REFRESH); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 570 "basicParse.y"
    { addOp(OP_END); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 574 "basicParse.y"
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

  case 116:

/* Line 1455 of yacc.c  */
#line 587 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 588 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 589 "basicParse.y"
    { addInt2Op(OP_STRARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 592 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 593 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 594 "basicParse.y"
    { addInt2Op(OP_ARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 598 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 601 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 605 "basicParse.y"
    { 
	    addIntOp(OP_PUSHINT, 1); //step
	    addIntOp(OP_FOR, (yyvsp[(2) - (6)].number));
	  ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 610 "basicParse.y"
    { 
	    addIntOp(OP_FOR, (yyvsp[(2) - (8)].number));
	  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 616 "basicParse.y"
    { addIntOp(OP_NEXT, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 619 "basicParse.y"
    { addIntOp(OP_GOTO, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 622 "basicParse.y"
    { addIntOp(OP_GOSUB, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 626 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OFFERROR); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 630 "basicParse.y"
    { addIntOp(OP_ONERROR, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 633 "basicParse.y"
    { addOp(OP_RETURN); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 636 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 637 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 638 "basicParse.y"
    { addOp(OP_SETCOLORINT); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 641 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 642 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 643 "basicParse.y"
    { addIntOp(OP_SOUND_LIST, listlen); listlen=0; ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 644 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 645 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 648 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 649 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 652 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 653 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 657 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 658 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 661 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 662 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 665 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 666 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 667 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 668 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 671 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 672 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 675 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 676 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 679 "basicParse.y"
    { addOp(OP_SYSTEM); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 682 "basicParse.y"
    { addOp(OP_VOLUME); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 685 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 686 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 687 "basicParse.y"
    { addIntOp(OP_POLY_LIST, listlen); listlen=0; ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 690 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(8) - (8)].number)); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 691 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(9) - (10)].number)); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 692 "basicParse.y"
    { addIntOp(OP_STAMP_S_LIST, listlen); listlen=0; ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 693 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(6) - (6)].number)); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 694 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(7) - (8)].number)); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 695 "basicParse.y"
    { addIntOp(OP_STAMP_LIST, listlen); listlen=0; ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 696 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(10) - (10)].number)); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 697 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(11) - (12)].number)); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 698 "basicParse.y"
    { addIntOp(OP_STAMP_SR_LIST, listlen); listlen=0; ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 701 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_OPEN); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 702 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 703 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 706 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITE); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 707 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 708 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 711 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITELINE); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 712 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 713 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 716 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 717 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 718 "basicParse.y"
    { addOp(OP_CLOSE); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 721 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 722 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 723 "basicParse.y"
    { addOp(OP_RESET); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 726 "basicParse.y"
    {addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP);addOp(OP_SEEK);  ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 727 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 728 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 731 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 732 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 733 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 734 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_ARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 735 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRINGASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 736 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 737 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 738 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_NUMASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 739 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 740 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 743 "basicParse.y"
    { addOp(OP_PRINT);  addOp(OP_INPUT); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 746 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_PRINTN); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 747 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 748 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 749 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 750 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 753 "basicParse.y"
    {addOp(OP_WAVPLAY);  ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 756 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 757 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 760 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 761 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 764 "basicParse.y"
    {addOp(OP_PUTSLICE);  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 765 "basicParse.y"
    { addOp(OP_PUTSLICE); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 766 "basicParse.y"
    {addOp(OP_PUTSLICEMASK);  ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 767 "basicParse.y"
    { addOp(OP_PUTSLICEMASK); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 769 "basicParse.y"
    {addOp(OP_IMGLOAD);  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 770 "basicParse.y"
    { addOp(OP_IMGLOAD); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 771 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 772 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 773 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 774 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 777 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEDIM); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 780 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD);  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 781 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 784 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE);  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 785 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 788 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE);  ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 789 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 792 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE);  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 793 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 796 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEHIDE); ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 799 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESHOW); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 802 "basicParse.y"
    {addOp(OP_CLICKCLEAR);  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 803 "basicParse.y"
    { addOp(OP_CLICKCLEAR); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 806 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CHANGEDIR); ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 809 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DECIMAL); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 812 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPEN); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 815 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 816 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 819 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBEXECUTE); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 822 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPENSET); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 825 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 826 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 829 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 830 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 831 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 834 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 835 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 836 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 837 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 840 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 841 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 842 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 845 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 846 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 847 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 850 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_KILL); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 851 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_KILL); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 854 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SETSETTING); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 855 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SETSETTING); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 858 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTOUT); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 859 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTOUT); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 862 "basicParse.y"
    {addStringOp(OP_PUSHSTRING, "PNG"); addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 863 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 864 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 874 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 875 "basicParse.y"
    { listlen++; ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 878 "basicParse.y"
    { (yyval.floatnum) = (yyvsp[(2) - (3)].floatnum); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 879 "basicParse.y"
    { addOp(OP_ADD); ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 880 "basicParse.y"
    { addOp(OP_SUB); ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 881 "basicParse.y"
    { addOp(OP_MUL); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 882 "basicParse.y"
    { addOp(OP_MOD); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 883 "basicParse.y"
    { addOp(OP_INTDIV); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 884 "basicParse.y"
    { addOp(OP_DIV); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 885 "basicParse.y"
    { addOp(OP_EXP); ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 886 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYOR); ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 887 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYAND); ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 888 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYNOT); ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 889 "basicParse.y"
    { addOp(OP_NEGATE); ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 890 "basicParse.y"
    {addOp(OP_AND); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 891 "basicParse.y"
    { addOp(OP_OR); ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 892 "basicParse.y"
    { addOp(OP_XOR); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 893 "basicParse.y"
    { addOp(OP_NOT); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 894 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 895 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 896 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 897 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 898 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 899 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 900 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 901 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 902 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 903 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 904 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 905 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 906 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum)); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 907 "basicParse.y"
    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 908 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 909 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 910 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 911 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 912 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 913 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 914 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 915 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 917 "basicParse.y"
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

  case 306:

/* Line 1455 of yacc.c  */
#line 927 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 928 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 929 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 930 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 931 "basicParse.y"
    { addOp(OP_LENGTH); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 932 "basicParse.y"
    { addOp(OP_ASC); ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 933 "basicParse.y"
    { addOp(OP_INSTR); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 934 "basicParse.y"
    { addOp(OP_CEIL); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 935 "basicParse.y"
    { addOp(OP_FLOOR); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 936 "basicParse.y"
    { addOp(OP_SIN); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 937 "basicParse.y"
    { addOp(OP_COS); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 938 "basicParse.y"
    { addOp(OP_TAN); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 939 "basicParse.y"
    { addOp(OP_ASIN); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 940 "basicParse.y"
    { addOp(OP_ACOS); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 941 "basicParse.y"
    { addOp(OP_ATAN); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 942 "basicParse.y"
    { addOp(OP_DEGREES); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 943 "basicParse.y"
    { addOp(OP_RADIANS); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 944 "basicParse.y"
    { addOp(OP_LOG); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 945 "basicParse.y"
    { addOp(OP_LOGTEN); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 946 "basicParse.y"
    { addOp(OP_ABS); ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 947 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 948 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 949 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 950 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 951 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 952 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 953 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 954 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 955 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 956 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 957 "basicParse.y"
    { addOp(OP_EOF); ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 958 "basicParse.y"
    { addOp(OP_EXISTS); ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 959 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 960 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 961 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 962 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 963 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 964 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 965 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 966 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 967 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 968 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 969 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 970 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 971 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 972 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 973 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 974 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 975 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 976 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 977 "basicParse.y"
    { addOp(OP_SIZE); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 978 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 979 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 980 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 981 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 982 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 983 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 984 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 985 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 986 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 987 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 988 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 989 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 990 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 991 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 992 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 993 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 994 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 995 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 996 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 997 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 998 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 999 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 1000 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 1001 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 1002 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 1003 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 1004 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 1005 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 1006 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 1007 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 1008 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 1009 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 1010 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 1011 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 1012 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 1013 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 1014 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 1015 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 1016 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 1017 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 1018 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 1019 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 1020 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 1021 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 1022 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 1023 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 1024 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 1025 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 1026 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 1027 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 1028 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 1029 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 1030 "basicParse.y"
    { addOp(OP_PIXEL); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1031 "basicParse.y"
    { addOp(OP_RGB); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1032 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1033 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1034 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITECOLLIDE); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1035 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEX); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1036 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEY); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1037 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEH); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1038 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEW); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1039 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEV); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1040 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBROW); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1041 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBINT); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1042 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBFLOAT); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1043 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1045 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1046 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1047 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp( OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1048 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1049 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1050 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTIN); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1053 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1054 "basicParse.y"
    { listlen++; ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1057 "basicParse.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1058 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1059 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1060 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1061 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1062 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1063 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1065 "basicParse.y"
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

  case 440:

/* Line 1455 of yacc.c  */
#line 1075 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1076 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1077 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 1078 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1079 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1080 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1081 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1082 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1083 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 1084 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1085 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1086 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1087 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1088 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1089 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1090 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1091 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1092 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1093 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1094 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1095 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1096 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1097 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1098 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1099 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1100 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1101 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MD5); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1102 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_GETSETTING); ;}
    break;



/* Line 1455 of yacc.c  */
#line 8916 "basicParse.tab.c"
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
#line 1105 "basicParse.y"


int
yyerror(const char *msg) {
  errorcode = -1;
  if (yytext[0] == '\n') { linenumber--; }	// error happened on previous line
  return -1;
}


