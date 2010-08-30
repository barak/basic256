
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
     B256LINENUM = 429,
     B256INTEGER = 430,
     B256FLOAT = 431,
     B256STRING = 432,
     B256VARIABLE = 433,
     B256STRINGVAR = 434,
     B256NEWVAR = 435,
     B256COLOR = 436,
     B256LABEL = 437,
     B256UMINUS = 438
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 284 "basicParse.y"

  int number;
  double floatnum;
  char *string;



/* Line 214 of yacc.c  */
#line 533 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 545 "basicParse.tab.c"

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
#define YYFINAL  357
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   13982

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  203
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  446
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1049

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   438

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     192,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     194,   196,   188,   187,   195,   186,     2,   189,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   193,   199,
     183,   185,   184,   202,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   197,     2,   198,   191,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   200,     2,   201,     2,     2,     2,     2,
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
     175,   176,   177,   178,   179,   180,   181,   182,   190
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
     191,   193,   195,   197,   199,   201,   205,   209,   217,   225,
     229,   233,   241,   249,   252,   254,   256,   258,   263,   270,
     272,   274,   277,   284,   293,   297,   304,   313,   317,   321,
     325,   332,   341,   344,   347,   350,   352,   355,   357,   364,
     373,   376,   381,   384,   387,   394,   399,   404,   411,   420,
     431,   438,   447,   456,   467,   474,   483,   490,   499,   506,
     515,   518,   521,   524,   527,   530,   535,   538,   547,   558,
     567,   574,   583,   590,   601,   614,   625,   628,   635,   640,
     643,   650,   655,   658,   665,   670,   672,   676,   679,   681,
     685,   688,   691,   698,   703,   707,   714,   718,   725,   728,
     734,   742,   745,   751,   759,   762,   764,   767,   770,   774,
     778,   781,   783,   787,   789,   793,   800,   809,   818,   829,
     836,   845,   854,   865,   876,   889,   892,   897,   904,   915,
     928,   935,   944,   951,   960,   963,   966,   968,   972,   975,
     978,   981,   983,   987,   990,   993,   995,   999,  1002,  1009,
    1014,  1019,  1026,  1033,  1042,  1045,  1052,  1057,  1059,  1063,
    1066,  1070,  1074,  1076,  1080,  1084,  1088,  1092,  1096,  1100,
    1104,  1108,  1112,  1115,  1119,  1123,  1127,  1130,  1134,  1138,
    1142,  1146,  1150,  1154,  1158,  1162,  1166,  1170,  1174,  1178,
    1180,  1182,  1187,  1192,  1198,  1204,  1210,  1216,  1221,  1228,
    1230,  1235,  1240,  1245,  1250,  1255,  1260,  1267,  1272,  1277,
    1282,  1287,  1292,  1297,  1302,  1307,  1312,  1317,  1322,  1327,
    1332,  1334,  1338,  1340,  1344,  1346,  1350,  1352,  1356,  1358,
    1362,  1367,  1372,  1374,  1378,  1380,  1384,  1386,  1390,  1392,
    1396,  1398,  1402,  1404,  1408,  1410,  1414,  1416,  1420,  1422,
    1426,  1431,  1433,  1437,  1439,  1443,  1445,  1449,  1451,  1455,
    1457,  1461,  1463,  1467,  1469,  1473,  1475,  1479,  1481,  1485,
    1487,  1491,  1493,  1497,  1499,  1503,  1505,  1509,  1511,  1515,
    1517,  1521,  1523,  1527,  1529,  1533,  1535,  1539,  1541,  1545,
    1547,  1551,  1553,  1557,  1559,  1563,  1565,  1569,  1571,  1575,
    1577,  1581,  1583,  1587,  1594,  1603,  1605,  1609,  1616,  1621,
    1626,  1631,  1636,  1641,  1645,  1650,  1655,  1657,  1661,  1663,
    1667,  1669,  1673,  1678,  1680,  1684,  1688,  1692,  1696,  1700,
    1702,  1707,  1714,  1716,  1721,  1726,  1731,  1736,  1745,  1752,
    1759,  1770,  1772,  1776,  1781,  1783,  1787,  1792,  1794,  1798,
    1803,  1805,  1809,  1811,  1815,  1817,  1821
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     204,     0,    -1,   205,   192,    -1,   205,   192,   204,    -1,
     206,   207,    -1,   207,    -1,   182,    -1,   208,    -1,   209,
      -1,   210,    -1,   212,    -1,   213,    -1,   215,    -1,   216,
      -1,   217,    -1,   218,    -1,    -1,   228,    20,   218,    -1,
     228,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     211,    -1,    23,   289,    -1,    24,    -1,    40,    23,    -1,
     214,    -1,    25,    -1,    26,   289,    -1,   219,    -1,   218,
     193,   219,    -1,   235,    -1,   236,    -1,   237,    -1,   238,
      -1,   239,    -1,   261,    -1,   242,    -1,   244,    -1,   245,
      -1,   251,    -1,   252,    -1,   243,    -1,   231,    -1,   232,
      -1,   233,    -1,   234,    -1,   240,    -1,   259,    -1,   227,
      -1,   223,    -1,   226,    -1,   224,    -1,   225,    -1,   220,
      -1,   221,    -1,   222,    -1,   230,    -1,   229,    -1,   253,
      -1,   254,    -1,   255,    -1,   256,    -1,   257,    -1,   241,
      -1,   246,    -1,   247,    -1,   248,    -1,   249,    -1,   250,
      -1,   262,    -1,   263,    -1,   264,    -1,   265,    -1,   266,
      -1,   267,    -1,   268,    -1,   269,    -1,   270,    -1,   271,
      -1,   272,    -1,   273,    -1,   258,    -1,   274,    -1,   275,
      -1,   276,    -1,   277,    -1,   278,    -1,   279,    -1,   280,
      -1,   281,    -1,   282,    -1,   283,    -1,   284,    -1,   285,
      -1,    45,   178,   289,    -1,    45,   179,   289,    -1,    45,
     178,   194,   289,   195,   289,   196,    -1,    45,   179,   194,
     289,   195,   289,   196,    -1,    46,   178,   289,    -1,    46,
     179,   289,    -1,    46,   178,   194,   289,   195,   289,   196,
      -1,    46,   179,   194,   289,   195,   289,   196,    -1,    76,
     289,    -1,    17,    -1,    18,    -1,    14,    -1,    15,   289,
     195,   289,    -1,    15,   194,   289,   195,   289,   196,    -1,
      16,    -1,    40,    -1,    19,   289,    -1,   179,   197,   289,
     198,   185,   291,    -1,   179,   197,   289,   195,   289,   198,
     185,   291,    -1,   179,   185,   286,    -1,   178,   197,   289,
     198,   185,   289,    -1,   178,   197,   289,   195,   289,   198,
     185,   289,    -1,   178,   185,   287,    -1,   178,   185,   289,
      -1,   179,   185,   291,    -1,    27,   178,   185,   289,    28,
     289,    -1,    27,   178,   185,   289,    28,   289,    29,   289,
      -1,    30,   178,    -1,    36,   178,    -1,    37,   178,    -1,
     163,    -1,   162,   178,    -1,    38,    -1,    41,   289,   195,
     289,   195,   289,    -1,    41,   194,   289,   195,   289,   195,
     289,   196,    -1,    41,   289,    -1,    77,   194,   178,   196,
      -1,    77,   178,    -1,    77,   287,    -1,    77,   194,   289,
     195,   289,   196,    -1,    77,   289,   195,   289,    -1,     8,
     289,   195,   289,    -1,     8,   194,   289,   195,   289,   196,
      -1,    13,   289,   195,   289,   195,   289,   195,   289,    -1,
      13,   194,   289,   195,   289,   195,   289,   195,   289,   196,
      -1,     9,   289,   195,   289,   195,   289,    -1,     9,   194,
     289,   195,   289,   195,   289,   196,    -1,    10,   289,   195,
     289,   195,   289,   195,   289,    -1,    10,   194,   289,   195,
     289,   195,   289,   195,   289,   196,    -1,    92,   289,   195,
     289,   195,   291,    -1,    92,   194,   289,   195,   289,   195,
     291,   196,    -1,    92,   289,   195,   289,   195,   289,    -1,
      92,   194,   289,   195,   289,   195,   289,   196,    -1,    93,
     291,   195,   289,   195,   289,    -1,    93,   194,   291,   195,
     289,   195,   289,   196,    -1,    94,   291,    -1,    94,   289,
      -1,    95,   291,    -1,    96,   289,    -1,    11,   178,    -1,
      11,   194,   178,   196,    -1,    11,   287,    -1,    12,   289,
     195,   289,   195,   289,   195,   178,    -1,    12,   194,   289,
     195,   289,   195,   289,   195,   178,   196,    -1,    12,   289,
     195,   289,   195,   289,   195,   287,    -1,    12,   289,   195,
     289,   195,   178,    -1,    12,   194,   289,   195,   289,   195,
     178,   196,    -1,    12,   289,   195,   289,   195,   287,    -1,
      12,   289,   195,   289,   195,   289,   195,   289,   195,   178,
      -1,    12,   194,   289,   195,   289,   195,   289,   195,   289,
     195,   178,   196,    -1,    12,   289,   195,   289,   195,   289,
     195,   289,   195,   287,    -1,    31,   291,    -1,    31,   194,
     289,   195,   291,   196,    -1,    31,   289,   195,   291,    -1,
      33,   291,    -1,    33,   194,   289,   195,   291,   196,    -1,
      33,   289,   195,   291,    -1,    82,   291,    -1,    82,   194,
     289,   195,   291,   196,    -1,    82,   289,   195,   291,    -1,
      34,    -1,    34,   194,   196,    -1,    34,   289,    -1,    35,
      -1,    35,   194,   196,    -1,    35,   289,    -1,   119,   289,
      -1,   119,   194,   289,   195,   289,   196,    -1,   119,   289,
     195,   289,    -1,   260,   195,   179,    -1,   260,   195,   179,
     197,   289,   198,    -1,   260,   195,   178,    -1,   260,   195,
     178,   197,   289,   198,    -1,     4,   179,    -1,     4,   179,
     197,   289,   198,    -1,     4,   179,   197,   289,   195,   289,
     198,    -1,     4,   178,    -1,     4,   178,   197,   289,   198,
      -1,     4,   178,   197,   289,   195,   289,   198,    -1,     4,
     291,    -1,     3,    -1,     3,   291,    -1,     3,   289,    -1,
       3,   291,   199,    -1,     3,   289,   199,    -1,   115,   291,
      -1,   116,    -1,   116,   194,   196,    -1,   117,    -1,   117,
     194,   196,    -1,   100,   289,   195,   289,   195,   291,    -1,
     100,   194,   289,   195,   289,   195,   291,   196,    -1,   100,
     289,   195,   289,   195,   291,   195,   289,    -1,   100,   194,
     289,   195,   289,   195,   291,   195,   289,   196,    -1,   101,
     289,   195,   289,   195,   291,    -1,   101,   194,   289,   195,
     289,   195,   291,   196,    -1,   101,   289,   195,   289,   195,
     289,   195,   291,    -1,   101,   194,   289,   195,   289,   195,
     289,   195,   291,   196,    -1,   101,   289,   195,   289,   195,
     289,   195,   289,   195,   291,    -1,   101,   194,   289,   195,
     289,   195,   289,   195,   289,   195,   291,   196,    -1,   102,
     289,    -1,   103,   289,   195,   291,    -1,   103,   194,   289,
     195,   291,   196,    -1,   104,   289,   195,   289,   195,   289,
     195,   289,   195,   289,    -1,   104,   194,   289,   195,   289,
     195,   289,   195,   289,   195,   289,   196,    -1,   108,   289,
     195,   289,   195,   289,    -1,   108,   194,   289,   195,   289,
     195,   289,   196,    -1,   105,   289,   195,   289,   195,   289,
      -1,   103,   194,   289,   195,   289,   195,   289,   196,    -1,
     106,   289,    -1,   107,   289,    -1,   126,    -1,   126,   194,
     196,    -1,   150,   291,    -1,   152,   289,    -1,   153,   291,
      -1,   154,    -1,   154,   194,   196,    -1,   155,   291,    -1,
     156,   291,    -1,   157,    -1,   157,   194,   196,    -1,   168,
     289,    -1,   168,   194,   289,   195,   289,   196,    -1,   168,
     289,   195,   289,    -1,   169,   291,   195,   289,    -1,   169,
     194,   291,   195,   289,   196,    -1,   169,   289,   195,   291,
     195,   289,    -1,   169,   194,   289,   195,   291,   195,   289,
     196,    -1,   171,   291,    -1,   171,   194,   289,   195,   291,
     196,    -1,   171,   289,   195,   291,    -1,   172,    -1,   172,
     194,   196,    -1,   172,   289,    -1,   200,   290,   201,    -1,
     200,   288,   201,    -1,   289,    -1,   289,   195,   288,    -1,
     194,   289,   196,    -1,   289,   187,   289,    -1,   289,   186,
     289,    -1,   289,   188,   289,    -1,   289,    84,   289,    -1,
     289,    85,   289,    -1,   289,   189,   289,    -1,   289,   191,
     289,    -1,   186,   289,    -1,   289,    72,   289,    -1,   289,
      73,   289,    -1,   289,    74,   289,    -1,    75,   289,    -1,
     291,   185,   291,    -1,   291,    44,   291,    -1,   291,   183,
     291,    -1,   291,   184,   291,    -1,   291,    42,   291,    -1,
     291,    43,   291,    -1,   289,   185,   289,    -1,   289,    44,
     289,    -1,   289,   183,   289,    -1,   289,   184,   289,    -1,
     289,    42,   289,    -1,   289,    43,   289,    -1,   176,    -1,
     175,    -1,   178,   197,   202,   198,    -1,   179,   197,   202,
     198,    -1,   178,   197,   202,   195,   198,    -1,   179,   197,
     202,   195,   198,    -1,   178,   197,   195,   202,   198,    -1,
     179,   197,   195,   202,   198,    -1,   178,   197,   289,   198,
      -1,   178,   197,   289,   195,   289,   198,    -1,   178,    -1,
      48,   194,   289,   196,    -1,    48,   194,   291,   196,    -1,
      80,   194,   289,   196,    -1,    80,   194,   291,   196,    -1,
      50,   194,   291,   196,    -1,    78,   194,   291,   196,    -1,
      56,   194,   291,   195,   291,   196,    -1,    57,   194,   289,
     196,    -1,    58,   194,   289,   196,    -1,    60,   194,   289,
     196,    -1,    61,   194,   289,   196,    -1,    62,   194,   289,
     196,    -1,    63,   194,   289,   196,    -1,    64,   194,   289,
     196,    -1,    65,   194,   289,   196,    -1,    68,   194,   289,
     196,    -1,    69,   194,   289,   196,    -1,    70,   194,   289,
     196,    -1,    71,   194,   289,   196,    -1,    66,   194,   289,
     196,    -1,    59,    -1,    59,   194,   196,    -1,    67,    -1,
      67,   194,   196,    -1,   121,    -1,   121,   194,   196,    -1,
     122,    -1,   122,   194,   196,    -1,    83,    -1,    83,   194,
     196,    -1,    83,   194,   289,   196,    -1,   120,   194,   291,
     196,    -1,    86,    -1,    86,   194,   196,    -1,    87,    -1,
      87,   194,   196,    -1,    88,    -1,    88,   194,   196,    -1,
      89,    -1,    89,   194,   196,    -1,    90,    -1,    90,   194,
     196,    -1,    91,    -1,    91,   194,   196,    -1,    97,    -1,
      97,   194,   196,    -1,    98,    -1,    98,   194,   196,    -1,
     118,    -1,   118,   194,   196,    -1,   118,   194,   289,   196,
      -1,     5,    -1,     5,   194,   196,    -1,   123,    -1,   123,
     194,   196,    -1,   124,    -1,   124,   194,   196,    -1,   125,
      -1,   125,   194,   196,    -1,   127,    -1,   127,   194,   196,
      -1,   128,    -1,   128,   194,   196,    -1,   129,    -1,   129,
     194,   196,    -1,   131,    -1,   131,   194,   196,    -1,   132,
      -1,   132,   194,   196,    -1,   133,    -1,   133,   194,   196,
      -1,   134,    -1,   134,   194,   196,    -1,   135,    -1,   135,
     194,   196,    -1,   136,    -1,   136,   194,   196,    -1,   137,
      -1,   137,   194,   196,    -1,   138,    -1,   138,   194,   196,
      -1,   139,    -1,   139,   194,   196,    -1,   140,    -1,   140,
     194,   196,    -1,   141,    -1,   141,   194,   196,    -1,   142,
      -1,   142,   194,   196,    -1,   143,    -1,   143,   194,   196,
      -1,   144,    -1,   144,   194,   196,    -1,   145,    -1,   145,
     194,   196,    -1,   146,    -1,   146,   194,   196,    -1,   147,
      -1,   147,   194,   196,    -1,   148,    -1,   148,   194,   196,
      -1,   149,    -1,   149,   194,   196,    -1,     6,   194,   289,
     195,   289,   196,    -1,     7,   194,   289,   195,   289,   195,
     289,   196,    -1,   130,    -1,   130,   194,   196,    -1,   109,
     194,   289,   195,   289,   196,    -1,   110,   194,   289,   196,
      -1,   111,   194,   289,   196,    -1,   112,   194,   289,   196,
      -1,   113,   194,   289,   196,    -1,   114,   194,   289,   196,
      -1,   158,   194,   196,    -1,   159,   194,   289,   196,    -1,
     160,   194,   289,   196,    -1,   164,    -1,   164,   194,   196,
      -1,   166,    -1,   166,   194,   196,    -1,   173,    -1,   173,
     194,   196,    -1,   173,   194,   289,   196,    -1,   291,    -1,
     291,   195,   290,    -1,   194,   291,   196,    -1,   291,   187,
     291,    -1,   289,   187,   291,    -1,   291,   187,   289,    -1,
     177,    -1,   179,   197,   289,   198,    -1,   179,   197,   289,
     195,   289,   198,    -1,   179,    -1,    79,   194,   289,   196,
      -1,    49,   194,   289,   196,    -1,    54,   194,   291,   196,
      -1,    55,   194,   291,   196,    -1,    51,   194,   291,   195,
     289,   195,   289,   196,    -1,    52,   194,   291,   195,   289,
     196,    -1,    53,   194,   291,   195,   289,   196,    -1,    99,
     194,   289,   195,   289,   195,   289,   195,   289,   196,    -1,
      32,    -1,    32,   194,   196,    -1,    32,   194,   289,   196,
      -1,    81,    -1,    81,   194,   196,    -1,    81,   194,   289,
     196,    -1,   151,    -1,   151,   194,   196,    -1,   161,   194,
     289,   196,    -1,   165,    -1,   165,   194,   196,    -1,   167,
      -1,   167,   194,   196,    -1,   170,    -1,   170,   194,   196,
      -1,   170,   194,   289,   196,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   317,   317,   318,   321,   322,   325,   327,   328,   329,
     330,   331,   338,   339,   346,   347,   348,   351,   366,   372,
     393,   394,   396,   411,   424,   425,   427,   445,   452,   463,
     463,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   532,   533,   534,   535,   538,
     539,   540,   541,   544,   547,   548,   551,   554,   555,   558,
     561,   564,   578,   579,   580,   583,   584,   585,   589,   592,
     595,   600,   607,   610,   613,   617,   621,   624,   627,   628,
     629,   632,   633,   634,   635,   636,   639,   640,   643,   644,
     648,   649,   652,   653,   656,   657,   658,   659,   662,   663,
     666,   667,   670,   673,   676,   677,   678,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   692,   693,   694,   697,
     698,   699,   702,   703,   704,   707,   708,   709,   712,   713,
     714,   717,   718,   719,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   731,   734,   737,   738,   739,   740,   741,
     744,   747,   748,   751,   752,   755,   756,   757,   758,   760,
     761,   762,   763,   764,   765,   768,   771,   772,   775,   776,
     779,   780,   783,   784,   787,   790,   793,   794,   797,   800,
     803,   806,   807,   810,   813,   816,   817,   820,   821,   822,
     825,   826,   827,   828,   831,   832,   833,   836,   837,   838,
     841,   844,   847,   848,   851,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     897,   898,   899,   900,   901,   902,   903,   904,   905,   906,
     907,   908,   909,   910,   911,   912,   913,   914,   915,   916,
     917,   918,   919,   920,   921,   922,   923,   924,   925,   926,
     927,   928,   929,   930,   931,   932,   933,   934,   935,   936,
     937,   938,   939,   940,   941,   942,   943,   944,   945,   946,
     947,   948,   949,   950,   951,   952,   953,   954,   955,   956,
     957,   958,   959,   960,   961,   962,   963,   964,   965,   966,
     967,   968,   969,   970,   971,   972,   973,   974,   975,   976,
     977,   978,   979,   980,   981,   982,   983,   984,   985,   986,
     987,   988,   989,   990,   991,   992,   993,   994,   995,   996,
     997,   998,   999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,
    1007,  1008,  1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,
    1017,  1018,  1019,  1022,  1023,  1026,  1027,  1028,  1029,  1030,
    1031,  1032,  1033,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,
    1061,  1062,  1063,  1064,  1065,  1066,  1067
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
  "B256NETCLOSE", "B256NETDATA", "B256LINENUM", "B256INTEGER", "B256FLOAT",
  "B256STRING", "B256VARIABLE", "B256STRINGVAR", "B256NEWVAR", "B256COLOR",
  "B256LABEL", "'<'", "'>'", "'='", "'-'", "'+'", "'*'", "'/'",
  "B256UMINUS", "'^'", "'\\n'", "':'", "'('", "','", "')'", "'['", "']'",
  "';'", "'{'", "'}'", "'?'", "$accept", "program", "validline", "label",
  "validstatement", "compoundifstmt", "ifstmt", "elsestmt", "endifexpr",
  "endifstmt", "whilestmt", "endwhileexpr", "endwhilestmt", "dostmt",
  "untilstmt", "compoundstmt", "statement", "dimstmt", "redimstmt",
  "pausestmt", "clearstmt", "fastgraphicsstmt", "graphsizestmt",
  "refreshstmt", "endstmt", "ifexpr", "strarrayassign", "arrayassign",
  "numassign", "stringassign", "forstmt", "nextstmt", "gotostmt",
  "gosubstmt", "offerrorstmt", "onerrorstmt", "returnstmt", "colorstmt",
  "soundstmt", "plotstmt", "linestmt", "circlestmt", "rectstmt",
  "textstmt", "fontstmt", "saystmt", "systemstmt", "volumestmt",
  "polystmt", "stampstmt", "openstmt", "writestmt", "writelinestmt",
  "closestmt", "resetstmt", "seekstmt", "inputstmt", "inputexpr",
  "printstmt", "wavplaystmt", "wavstopstmt", "wavwaitstmt", "putslicestmt",
  "imgloadstmt", "spritedimstmt", "spriteloadstmt", "spriteslicestmt",
  "spriteplacestmt", "spritemovestmt", "spritehidestmt", "spriteshowstmt",
  "clickclearstmt", "changedirstmt", "decimalstmt", "dbopenstmt",
  "dbclosestmt", "dbexecutestmt", "dbopensetstmt", "dbclosesetstmt",
  "netlistenstmt", "netconnectstmt", "netwritestmt", "netclosestmt",
  "immediatestrlist", "immediatelist", "floatlist", "floatexpr",
  "stringlist", "stringexpr", 0
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
     435,   436,   437,    60,    62,    61,    45,    43,    42,    47,
     438,    94,    10,    58,    40,    44,    41,    91,    93,    59,
     123,   125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   203,   204,   204,   205,   205,   206,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   208,   209,   210,
     211,   211,   212,   213,   214,   214,   215,   216,   217,   218,
     218,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   220,   220,   220,   220,   221,
     221,   221,   221,   222,   223,   223,   224,   225,   225,   226,
     227,   228,   229,   229,   229,   230,   230,   230,   231,   232,
     233,   233,   234,   235,   236,   237,   238,   239,   240,   240,
     240,   241,   241,   241,   241,   241,   242,   242,   243,   243,
     244,   244,   245,   245,   246,   246,   246,   246,   247,   247,
     248,   248,   249,   250,   251,   251,   251,   252,   252,   252,
     252,   252,   252,   252,   252,   252,   253,   253,   253,   254,
     254,   254,   255,   255,   255,   256,   256,   256,   257,   257,
     257,   258,   258,   258,   259,   259,   259,   259,   259,   259,
     259,   259,   259,   259,   260,   261,   261,   261,   261,   261,
     262,   263,   263,   264,   264,   265,   265,   265,   265,   266,
     266,   266,   266,   266,   266,   267,   268,   268,   269,   269,
     270,   270,   271,   271,   272,   273,   274,   274,   275,   276,
     277,   278,   278,   279,   280,   281,   281,   282,   282,   282,
     283,   283,   283,   283,   284,   284,   284,   285,   285,   285,
     286,   287,   288,   288,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   290,   290,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291
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
       1,     1,     1,     1,     1,     3,     3,     7,     7,     3,
       3,     7,     7,     2,     1,     1,     1,     4,     6,     1,
       1,     2,     6,     8,     3,     6,     8,     3,     3,     3,
       6,     8,     2,     2,     2,     1,     2,     1,     6,     8,
       2,     4,     2,     2,     6,     4,     4,     6,     8,    10,
       6,     8,     8,    10,     6,     8,     6,     8,     6,     8,
       2,     2,     2,     2,     2,     4,     2,     8,    10,     8,
       6,     8,     6,    10,    12,    10,     2,     6,     4,     2,
       6,     4,     2,     6,     4,     1,     3,     2,     1,     3,
       2,     2,     6,     4,     3,     6,     3,     6,     2,     5,
       7,     2,     5,     7,     2,     1,     2,     2,     3,     3,
       2,     1,     3,     1,     3,     6,     8,     8,    10,     6,
       8,     8,    10,    10,    12,     2,     4,     6,    10,    12,
       6,     8,     6,     8,     2,     2,     1,     3,     2,     2,
       2,     1,     3,     2,     2,     1,     3,     2,     6,     4,
       4,     6,     6,     8,     2,     6,     4,     1,     3,     2,
       3,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     4,     4,     5,     5,     5,     5,     4,     6,     1,
       4,     4,     4,     4,     4,     4,     6,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       4,     4,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       4,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     6,     8,     1,     3,     6,     4,     4,
       4,     4,     4,     3,     4,     4,     1,     3,     1,     3,
       1,     3,     4,     1,     3,     3,     3,     3,     3,     1,
       4,     6,     1,     4,     4,     4,     4,     8,     6,     6,
      10,     1,     3,     4,     1,     3,     4,     1,     3,     4,
       1,     3,     1,     3,     1,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      16,   195,     0,     0,     0,     0,     0,     0,     0,   106,
       0,   109,   104,   105,     0,    19,    20,     0,    24,    27,
       0,     0,     0,     0,     0,   175,   178,     0,     0,   127,
     110,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   201,   203,     0,   226,     0,     0,     0,   231,
       0,     0,   235,     0,   125,     0,     0,     0,   247,     0,
       0,     6,     0,     0,    16,     5,     7,     8,     9,    22,
      10,    11,    26,    12,    13,    14,    15,    29,    54,    55,
      56,    50,    52,    53,    51,    49,     0,    58,    57,    43,
      44,    45,    46,    31,    32,    33,    34,    35,    47,    64,
      37,    42,    38,    39,    65,    66,    67,    68,    69,    40,
      41,    59,    60,    61,    62,    63,    82,    48,     0,    36,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,   341,     0,     0,   431,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   318,   322,   324,
     326,   328,   330,   332,   334,   336,     0,     0,     0,     0,
       0,     0,     0,   338,     0,   314,   316,   343,   345,   347,
     349,   351,   353,   395,   355,   357,   359,   361,   363,   365,
     367,   369,   371,   373,   375,   377,   379,   381,   383,   385,
     387,   389,   391,   437,     0,     0,     0,     0,   406,   440,
     408,   442,   444,   410,   280,   279,   419,   289,   422,     0,
       0,   197,   196,   289,   422,     0,   194,     0,     0,     0,
       0,     0,     0,     0,   154,     0,     0,   156,     0,     0,
       0,     0,     0,     0,   111,    23,    28,     0,   122,     0,
       0,   166,     0,     0,   169,     0,   177,     0,   180,   123,
     124,    21,    25,     0,   130,     0,     0,     0,     0,   103,
     289,     0,   133,     0,     0,     0,   172,     0,     0,     0,
       0,   151,   150,   152,   153,     0,     0,     0,     0,   215,
       0,     0,     0,     0,     0,   224,   225,     0,     0,   200,
       0,     0,     0,   181,     0,   228,   229,   230,     0,   233,
     234,     0,   126,     0,   237,     0,     0,     0,     0,     0,
     244,     0,   249,     0,     0,     0,     0,     1,     2,     4,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   199,     0,     0,     0,     0,     0,
       0,     0,   198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   176,   179,     0,     0,     0,
      95,     0,    96,     0,    99,     0,   100,   289,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   202,   204,     0,
       0,   227,   232,   236,     0,     0,     0,     0,     0,     0,
       0,     0,   248,   117,   118,     0,     0,   114,   119,     0,
       3,   110,    30,    17,   186,   184,   342,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
       0,   313,     0,     0,     0,     0,     0,     0,     0,     0,
     435,     0,   319,     0,   323,   325,   327,   329,   331,   333,
     335,   337,     0,     0,     0,     0,     0,     0,     0,   339,
       0,     0,   315,   317,   344,   346,   348,   350,   352,   354,
     396,   356,   358,   360,   362,   364,   366,   368,   370,   372,
     374,   376,   378,   380,   382,   384,   386,   388,   390,   392,
     438,   403,     0,     0,     0,   407,   441,   409,   443,   445,
       0,   411,     0,     0,     0,     0,     0,     0,     0,   254,
     415,   277,   278,   274,   263,   264,   265,   258,   259,   275,
     276,   273,   256,   255,   417,   257,   260,   261,   271,   272,
     268,   269,   270,   267,   418,   416,     0,     0,     0,   136,
       0,     0,     0,     0,   155,   251,     0,     0,     0,     0,
       0,     0,   107,     0,     0,   168,     0,   171,     0,     0,
       0,     0,     0,     0,   131,     0,   135,     0,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   216,     0,
       0,     0,     0,     0,     0,   183,     0,   239,     0,     0,
       0,   240,     0,   246,     0,     0,     0,   413,     0,     0,
       0,     0,     0,     0,   433,   290,   291,   424,   294,     0,
       0,     0,   425,   426,     0,   297,   298,   299,   300,   301,
     302,   303,   304,   309,   305,   306,   307,   308,   295,   423,
     292,   293,   436,   320,     0,     0,   398,   399,   400,   401,
     402,   340,   321,   404,   405,   439,   446,   412,     0,     0,
     281,     0,   287,     0,     0,   282,     0,   420,     0,   287,
       0,   420,     0,     0,     0,     0,     0,   253,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   285,   283,     0,   286,   284,     0,     0,     0,   137,
       0,   140,     0,     0,     0,   289,   162,     0,     0,     0,
     108,   120,   167,   170,     0,   128,     0,     0,     0,     0,
     134,   173,     0,   146,   144,     0,   148,     0,   205,     0,
       0,   209,     0,   217,     0,     0,   222,     0,   220,   182,
     238,     0,   241,   242,   245,     0,   115,   414,     0,   112,
     187,   185,   393,     0,     0,   428,   429,   296,     0,   397,
     288,   421,   288,   421,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,    97,    98,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   141,     0,   142,   161,
       0,   289,   159,     0,     0,   138,   121,   129,   147,   145,
     149,     0,   206,   207,     0,   210,     0,   211,   223,     0,
       0,   221,   243,   116,   113,   394,   427,     0,     0,   289,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     143,   158,     0,   163,   165,   139,   208,     0,   212,   213,
       0,   218,   430,     0,     0,     0,   164,   214,   219
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   557,   267,   492,   255,   756,   259
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -333
static const yytype_int16 yypact[] =
{
    8780,   510,  4005,  4153,  4301,  4449,  -163,  4597,  4745,  -333,
    4893,  -333,  -333,  -333,   510,  -333,  -333,   510,  -333,  -333,
     510,  -161,  -157,  5041,  5189,  5337,  5485,  -149,  -128,  -333,
      -5,  5633,  -167,   -96,   510,    64,  5781,  5929,  6077,   510,
     510,   510,  6225,  6373,   510,  6521,  6669,   510,   510,   510,
    6817,   510,  -193,  -142,  6965,  -131,   510,   510,   510,  -126,
     510,   510,  -116,  -102,  -333,  7113,  7261,  7409,  7557,  -177,
    -146,  -333,    88,   -91,  8957,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,   -87,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,    87,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,   -86,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,   -84,   -83,   -58,   -57,   -56,   -48,
     -46,   -45,   -38,   -37,   -36,   -35,   -34,   -29,   -28,   -24,
     -22,   -15,   -14,    -4,    20,    25,    26,    27,    32,    33,
      38,    39,   510,    41,    42,    44,    52,    57,    58,    59,
      60,    61,    63,    66,    68,    69,    72,    74,    76,    81,
      82,    83,    85,    86,    89,    91,    96,    97,    99,   104,
     105,   107,   108,   110,   111,   113,   115,   116,   117,   118,
     119,   121,   126,   128,   130,   131,   133,   134,   135,   136,
     141,   142,   143,   144,   145,   146,   147,   149,   151,   152,
     154,   155,   157,   160,  -333,  -333,  -333,   -89,   -33,   510,
     510,  1226,   -16,  -138,  -135, 13745,   683,   510, 11863,   683,
     510, 11889,   510, 11910,  -333,   178,   510,  -333,   510, 11936,
     510, 11957,   510, 11983, 13745, 13745, 13745,   175,  -333,   510,
   12018,   683,   510, 12044,   683,  2664, 13745,  2813, 13745,  -333,
    -333,  -333,  -333,   510, 12065,  7705,  7853,  8001,  8149, 13745,
    -120,  8297,  -333, 12091,   510, 12111,   683,   510, 12138,   510,
    1515, 13745,   683,   683, 13745,   510, 12173,   510, 12199, 13745,
     510, 12219,   510, 12246, 12265, 13745, 13745,   510, 12327,   683,
     166,   172,   510, 12354,   176,   683, 13745,   683,   177,   683,
     683,   181,  -333,   510, 12373,   510, 12400,  1544,   510, 12419,
     683,  2962, 13745,  1964,   510,  2139,   510,  -333,  1753,  -333,
    9129,  9129,   -88,   189,   510,   510,  3111,   510,   510,   510,
     510,   510,   510,   510,   510,   510,   510,   510,   194,   510,
     510,   510,   510,   510,   510,   510,   195,   510,   510,   510,
     510,   180,   510,   510,   510,  3260,  3409,   200,   201,   202,
     203,   204,   205,   206,   207,   510,   510,   510,   510,   510,
     510,   510,  3558,   510,   216,   220,   222,   227,   231,   235,
     237,   238,   239,   240,   241,   242,   243,   246,   250,   251,
     252,   253,   258,   259,   260,   280,   282,   283,   285,   288,
     292,   294,   295,   296,   510,   510,   510,   297,   299,   301,
     304,  3707,  3856,   999,  1485,   180, 10282,   290,   510,   510,
     510,   510,   510,   510,   510,   510,   510,   510,   510,   510,
     510,   510,   510,   510,  -333,   510,   510,   510,   510,   510,
     510,   510,  -333,   999,  1485,  8835,   510,  9512,   510,  9550,
     510,   305,   173, 12435,  9571,   510,  9632,   510,  9646,   510,
     510,  9679,   510,  9700,   510,  -333,  -333,  9726,   510,   510,
   13745,   510, 13745,   510, 13745,   510, 13745,  -136,  9808,   510,
    9834,   510,  9855,   510,   174,   510,  9881,   510,  9899,   510,
    9916,   510,  9963,   510,   510,  9989,   510,  -333,  -333, 10036,
     510,  -333,  -333,  -333, 10054,   510, 10071,   230,   510,   510,
   10118,   510,  -333,  -333, 13745,  1623,   510,  -333,   683,  8994,
    -333,  -333,  -333,   -87,   306,   307,  -333, 12481, 12508,  -333,
   10299, 10317,   507, 10334,   571,  1549,  1558,  1759,   620,   748,
    1789, 10390, 10407,  -333, 10454, 10468, 10515, 10540, 10562, 10588,
   10623,  -333, 10648, 10670, 10696, 10717,   753, 10743, 10778,   769,
    -333, 10804,  -333, 10825,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333, 12527, 12573, 10851, 10872, 10898, 10933, 10959,  -333,
   10980,   920,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333, 11006, 11027, 11053,  -333,  -333,  -333,  -333,  -333,
   11088,  -333, 11114,    80,  -111,  9012,   300,  -109,  9184,  -333,
    -333,   233,   233,   233,  8738,  8826, 13791,   180,   180,   233,
     233,   233,    56,    56,  -333,   180,   180,  -333,   318,   318,
     318,   318,   318,   318,    56,  -333,  9221,  9245,   510, 13745,
     510, 12589,   510, 12635,  -333,  -333,   510,   510, 12658,   510,
   12681,   510, 13745,   835,   510,   683,   510,   683,   510, 12697,
   10144, 10162, 10209, 10226,  -333,   510, 13745,   510,   683,   510,
   12743,   510, 12766,   510, 12789,   510, 12835,   510,   683,   510,
   12851, 12874,   510, 12897,   510, 13745,   510, 13745,   510,   510,
    1796, 13745,   510,   683,   510,   321,   308,  1853,   510,   326,
     510,   510,   510,   510,  -333,  -333,  -333,  -333,  -333,   510,
     510,   510,  -333,  -333,   510,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,   510,   510,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,   315,   322,
    -333,   510,  -333,   323,   325,  -333,   510,  -333,   510,  -100,
     510,   -98, 11135, 12920,   510, 12943,   510,  -333, 12959,  2314,
   13005,   510, 11161,   510,  1029,  1176, 13028,   510,   510,   510,
     510,   510, 11182,  1186, 13051,   510, 13097,   510, 13113,   510,
   13136,   510, 13159,  1205, 13182,   510,   510, 13205,   510, 11208,
   11243,  1884, 11269,   510,  1220,  9267,   510,  -333,   510,  9292,
     510,  9311,  9419, 11290, 13221, 13267, 11316, 11337,  1243, 13290,
   11363,  -333,  -333,  9442,  -333,  -333,  9463,  9479,  9496,  -333,
     510, 13745,   510, 13313,  8445,  -118,  -333, 13359,   510, 13375,
    -333,  1167,  -333,  -333,   510, 13745, 11398, 11424, 11445, 11471,
    -333,  -333,   510, 13745,   683,   510, 13745,   510,  2203,   510,
   13398,   683,   510,  -333,   510, 13421, 13745,   510, 13745,  -333,
    -333,   510,  -333, 13745,  -333,   334, 13745,  -333,   340,   683,
    -333,  -333,  -333,   510,   510,  -333,  -333,  -333,   510,  -333,
    -333,  -333,   -95,   -93, 11492, 13444,   510,   -94, 13467,  2489,
   13483,   510,   510, 11518,  -333,  -333,  -333,  -333, 11553,  1284,
   11579,   245,   510, 13529,  1290,   510, 11600, 13552,   510, 11626,
   11647,   510,   510, 11673, 11708, 13575,  -333,   510, 13745,  -333,
    8593,  -112,  -333, 13621,   510, 13745, 13745,  -333,  -333,  -333,
    -333,   510,  -333, 13745,   510,  -333, 13637,   683,  -333,   510,
   13660,  -333,  -333, 13745,   683,  -333,  -333,   510, 11734,   -92,
   13683,  -168, 11755, 11781, 13706,  1336,   510, 13729,   510, 11802,
    -333,  -333,   349,  -333,  -333,  -333,  -333,   510,  -333,   683,
     510, 13745,  -333,   333,  1394, 11828,  -333,  -333,  -333
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -333,   179,  -333,  -333,   456,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,   170,   183,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,  -333,
    -333,  -333,  -333,  -333,   -30,  -172,    -1,  -332,   974
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -194
static const yytype_int16 yytable[] =
{
     251,   330,   258,   261,   263,   302,   269,   271,   353,   273,
    1033,   295,   296,   274,   291,   264,   275,   277,   292,   276,
     354,   278,   280,   283,   286,   288,   475,   476,   477,   289,
     294,   265,   266,   299,   303,   305,   308,   266,   311,   355,
     314,   316,   318,   319,   321,   323,   324,   325,   326,   328,
     290,   356,   331,   333,  -191,  -191,   336,  -188,  -188,   483,
     724,   453,   484,   334,   344,   346,   349,   352,   338,   154,
     155,   156,  -132,  -132,  -160,  -160,   342,   453,   341,   453,
    -157,  -157,   297,   298,   809,   453,   814,   810,   357,   815,
     564,   565,  -192,  -192,  -189,  -189,   157,  -193,  -193,  -190,
    -190,   358,   989,   453,  1031,   453,   360,   361,   453,   362,
     363,   364,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   365,   366,   367,   182,
     464,   465,   183,   184,   185,   186,   368,   187,   369,   370,
     188,   189,   190,   191,   192,   193,   371,   372,   373,   374,
     375,   194,   195,   196,   454,   376,   377,   478,   479,   480,
     378,   481,   379,   197,   198,   199,   200,   201,   202,   380,
     381,   391,   203,   482,   204,   205,   206,   207,   208,   209,
     382,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   383,   233,   475,   476,   477,   384,
     385,   386,   234,   235,   236,   237,   387,   388,   238,   239,
     240,   241,   389,   390,   242,   392,   393,   243,   394,   244,
     245,   246,   300,   248,   471,   472,   395,   473,   455,   456,
     249,   396,   397,   398,   399,   400,   485,   401,   301,   487,
     402,   489,   403,   404,   266,   493,   405,   494,   406,   496,
     407,   498,   475,   476,   477,   408,   409,   410,   501,   411,
     412,   503,   808,   413,   456,   414,   456,   475,   476,   477,
     415,   416,   507,   417,   510,   512,   514,   516,   418,   419,
     518,   420,   421,   520,   422,   423,   522,   424,   456,   425,
     426,   427,   428,   429,   526,   430,   528,   464,   465,   530,
     431,   532,   432,   553,   433,   434,   535,   435,   436,   437,
     438,   539,   475,   476,   477,   439,   440,   441,   442,   443,
     444,   445,   544,   446,   546,   447,   448,   550,   449,   450,
     456,   451,   554,   555,   452,   559,   491,   478,   479,   480,
     500,   481,   537,   567,   568,   570,   571,   573,   538,   731,
     670,   473,   541,   542,   705,   581,   582,   543,   584,   585,
     586,   587,   588,   589,   590,   566,   592,   593,   594,   595,
     583,   591,   597,   598,   601,   603,   604,   605,   606,   607,
     608,   609,   610,   611,   612,   613,   614,   615,   616,   617,
     618,   620,   622,   478,   479,   480,   623,   481,   624,   469,
     470,   471,   472,   625,   473,   749,   670,   626,   478,   479,
     480,   627,   481,   628,   629,   630,   631,   632,   633,   634,
    1001,  1002,   635,   652,   653,   654,   636,   637,   638,   639,
     660,   662,   665,   668,   640,   641,   642,   671,   672,   673,
     674,   675,   676,   677,   678,   679,   680,   681,   682,   683,
     685,   686,   687,   478,   479,   480,   643,   481,   644,   645,
     694,   646,   696,   697,   647,   699,   670,   701,   648,   703,
     649,   650,   651,   655,   708,   656,   710,   657,   712,   713,
     658,   704,   813,   760,   761,   481,   866,   719,   720,   867,
     721,   870,   722,   881,   723,   154,   155,   156,   726,   981,
     882,   884,   730,   885,   732,   982,   734,  1043,   736,  1046,
     359,   563,   740,   741,   827,   743,   937,   560,     0,   745,
       0,     0,   157,   562,   747,     0,     0,     0,   751,   475,
     476,   477,     0,     0,     0,     0,     0,     0,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,     0,   182,     0,     0,   183,   184,
     185,   186,     0,   187,     0,     0,   188,   189,   190,   191,
     192,   193,     0,     0,     0,     0,     0,   194,   195,   196,
       0,     0,     0,   475,   476,   477,     0,     0,     0,   197,
     198,   199,   200,   201,   202,     0,     0,     0,   203,     0,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   233,   475,   476,   477,     0,     0,     0,   234,   235,
     236,   237,     0,     0,   238,   239,   240,   241,     0,     0,
     242,     0,     0,   243,     0,   244,   245,   246,   247,   248,
     478,   479,   480,     0,   481,     0,   249,   822,     0,   823,
       0,   825,     0,   766,   250,   493,   828,     0,   830,     0,
     832,     0,     0,     0,     0,     0,     0,   836,     0,     0,
       0,     0,     0,     0,   842,   475,   476,   477,   844,     0,
     846,     0,   848,     0,   850,     0,   852,     0,   854,     0,
       0,   857,     0,   859,     0,   860,     0,     0,   862,     0,
       0,     0,     0,   865,   478,   479,   480,   869,   481,   871,
     872,   873,   874,     0,     0,     0,     0,   768,   875,   876,
     877,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     475,   476,   477,   879,   880,   475,   476,   477,     0,   896,
       0,     0,     0,   478,   479,   480,     0,   481,     0,     0,
     883,   475,   476,   477,     0,   886,   772,   887,     0,   888,
       0,     0,     0,   891,     0,   893,     0,     0,   897,     0,
     899,     0,   901,     0,     0,     0,   905,   906,   907,   908,
     909,     0,     0,     0,   913,     0,   916,     0,     0,     0,
     920,     0,     0,     0,   925,   926,     0,   928,     0,     0,
       0,     0,   933,   833,     0,   936,   478,   479,   480,     0,
     481,     0,     0,     0,     0,     0,     0,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   954,
       0,   955,     0,   958,     0,     0,     0,   960,     0,     0,
       0,     0,     0,   963,     0,     0,     0,   461,   462,   463,
       0,   968,     0,     0,   970,     0,     0,     0,   973,   464,
     465,   976,     0,   977,     0,     0,   979,     0,     0,   992,
     980,   478,   479,   480,     0,   481,   478,   479,   480,     0,
     481,     0,   983,   984,   773,     0,     0,   985,     0,   788,
       0,     0,   478,   479,   480,   988,   481,     0,   993,     0,
     995,   996,   475,   476,   477,   791,     0,     0,     0,     0,
       0,  1003,     0,     0,  1006,   252,   256,  1010,     0,     0,
    1013,     0,     0,     0,     0,     0,  1018,     0,     0,  1020,
       0,  1034,     0,  1022,     0,     0,     0,   281,   284,     0,
    1023,     0,     0,  1024,   154,   155,   156,     0,  1027,     0,
     306,     0,   310,   312,   313,     0,  1029,     0,   466,   467,
     468,   469,   470,   471,   472,   329,   473,  1041,     0,     0,
     335,   157,   337,     0,   339,   340,     0,     0,     0,  1045,
     347,   350,     0,     0,     0,     0,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   475,   476,   477,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,   478,   479,   480,     0,   481,   197,   198,
     199,   200,   201,   202,     0,     0,   802,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,     0,     0,     0,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,     0,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   250,   663,     0,   962,     0,     0,     0,
       0,   664,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,   478,   479,   480,     0,   481,     0,   475,   476,
     477,     0,     0,     0,   457,   902,     0,     0,   475,   476,
     477,   457,     0,     0,   457,     0,   457,     0,     0,   461,
     462,   463,   457,     0,   457,     0,   457,   475,   476,   477,
       0,   464,   465,   457,     0,     0,   457,     0,     0,   457,
       0,   457,   475,   476,   477,     0,     0,   457,   458,   459,
     460,     0,     0,     0,     0,   457,     0,     0,   457,     0,
       0,   457,     0,   524,     0,   475,   476,   477,     0,   457,
       0,   457,     0,     0,   457,     0,   457,     0,   461,   462,
     463,   457,     0,     0,     0,     0,   457,     0,     0,     0,
     464,   465,     0,     0,     0,     0,     0,   457,     0,   547,
       0,     0,   457,     0,     0,   457,   475,   476,   477,   558,
       0,     0,   475,   476,   477,     0,     0,     0,     0,     0,
       0,   572,     0,   574,   575,   576,   577,   578,   579,   580,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   478,
     479,   480,     0,   481,     0,     0,   596,     0,   599,   478,
     479,   480,   903,   481,     0,     0,     0,     0,   475,   476,
     477,     0,   911,     0,     0,     0,     0,   621,   478,   479,
     480,     0,   481,     0,     0,     0,     0,     0,     0,     0,
       0,   923,     0,   478,   479,   480,     0,   481,     0,   466,
     467,   468,   469,   470,   471,   472,   934,   473,     0,     0,
       0,     0,     0,     0,     0,   474,   478,   479,   480,     0,
     481,     0,     0,     0,     0,     0,   475,   476,   477,   947,
       0,     0,     0,     0,   684,     0,     0,     0,     0,   688,
     689,   690,   691,   692,   693,   695,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   478,   479,   480,
       0,   481,     0,   478,   479,   480,   715,   481,   717,     0,
     999,     0,     0,   457,     0,   457,  1005,   457,     0,   457,
     154,   155,   156,     0,     0,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   738,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   157,     0,   478,
     479,   480,   750,   481,     0,   753,     0,     0,     0,     0,
     757,     0,  1038,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   475,   476,   477,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,   478,   479,   480,
       0,   481,   194,   195,   196,     0,   475,   476,   477,     0,
    1047,   475,   476,   477,   197,   198,   199,   200,   201,   202,
     475,   476,   477,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,     0,     0,
       0,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   458,   459,   460,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   250,
     666,     0,     0,     0,     0,     0,     0,   667,   834,     0,
     835,     0,     0,     0,     0,   461,   462,   463,   478,   479,
     480,   843,   481,     0,     0,     0,     0,   464,   465,     0,
     525,   853,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   861,     0,     0,     0,   864,   478,   479,   480,
       0,   481,   478,   479,   480,     0,   481,     0,     0,   549,
       0,   478,   479,   480,   769,   481,     0,     0,   878,     0,
       0,     0,     0,   770,     0,     0,     1,     2,     0,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,     0,     0,    22,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,     0,     0,    32,    33,
       0,   475,   476,   477,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,     0,     0,     0,   754,   914,
       0,   755,     0,   918,     0,   921,     0,     0,     0,    34,
      35,   475,   476,   477,     0,    36,     0,     0,   475,   476,
     477,     0,   757,     0,   939,    37,    38,    39,    40,    41,
       0,     0,     0,    42,    43,    44,    45,    46,    47,    48,
      49,    50,     0,     0,     0,     0,     0,     0,    51,    52,
      53,     0,    54,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,   969,     0,     0,     0,
       0,   971,     0,   974,     0,   475,   476,   477,     0,     0,
       0,     0,     0,    56,     0,    57,    58,    59,    60,    61,
      62,     0,     0,     0,     0,    63,    64,     0,     0,     0,
       0,    65,    66,     0,    67,    68,   475,   476,   477,     0,
       0,    69,    70,     0,     0,    71,     0,     0,     0,     0,
       0,     0,   478,   479,   480,   -16,   481,     0,     0,  1007,
       0,     0,     0,     0,   771,     0,  1014,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     155,   156,   478,   479,   480,     0,   481,     0,  1025,   478,
     479,   480,     0,   481,   774,     0,     0,     0,     0,     0,
       0,   863,     0,     0,     0,     0,   157,     0,     0,     0,
    1039,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1044,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   478,   479,   480,   182,
     481,     0,   183,   184,   185,   186,     0,   187,   868,     0,
     188,   189,   190,   191,   192,   193,     0,     0,     0,     0,
       0,   194,   195,   196,     0,     0,     0,   478,   479,   480,
       0,   481,     0,   197,   198,   199,   200,   201,   202,   931,
       0,     0,   203,     0,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,     0,   233,     0,     0,     0,     0,
       0,     0,   234,   235,   236,   237,     0,     0,   238,   239,
     240,   241,     0,     0,   242,     0,     0,   243,     0,   244,
     245,   246,   247,   248,   154,   155,   156,     0,     0,     0,
     249,     0,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,   266,     0,     0,     0,     0,     0,
       0,   157,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,   475,   476,   477,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,     0,     0,     0,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   154,
     155,   156,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,     0,   556,
       0,     0,     0,     0,     0,     0,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   478,   479,   480,   182,
     481,     0,   183,   184,   185,   186,     0,   187,   972,     0,
     188,   189,   190,   191,   192,   193,     0,     0,     0,     0,
       0,   194,   195,   196,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,   198,   199,   200,   201,   202,     0,
       0,     0,   203,     0,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,     0,   233,     0,     0,     0,     0,
       0,     0,   234,   235,   236,   237,     0,     0,   238,   239,
     240,   241,     0,     0,   242,     0,     0,   243,     0,   244,
     245,   246,   895,   248,   154,   155,   156,     0,     0,     0,
     249,     0,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,   266,     0,     0,     0,     0,     0,
       0,   157,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,     0,     0,     0,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   991,   248,   154,
     155,   156,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   250,     0,     0,     0,     0,     0,   266,
       0,     0,     0,     0,     0,     0,   157,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,     0,     0,     0,   182,
       0,     0,   183,   184,   185,   186,     0,   187,     0,     0,
     188,   189,   190,   191,   192,   193,     0,     0,     0,     0,
       0,   194,   195,   196,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   197,   198,   199,   200,   201,   202,     0,
       0,     0,   203,     0,   204,   205,   206,   207,   208,   209,
       0,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,     0,   233,     0,     0,   154,   155,
     156,     0,   234,   235,   236,   237,     0,     0,   238,   239,
     240,   241,     0,     0,   242,     0,     0,   243,     0,   244,
     245,   246,   247,   248,     0,   157,     0,     0,     0,     0,
     249,     0,     0,     0,     0,     0,     0,     0,   250,     0,
     505,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,     0,   154,   155,   156,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,     0,   157,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   250,     0,   506,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,     0,     0,     0,   182,     0,     0,
     183,   184,   185,   186,     0,   187,     0,     0,   188,   189,
     190,   191,   192,   193,     0,     0,     0,     0,     0,   194,
     195,   196,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   197,   198,   199,   200,   201,   202,     0,     0,     0,
     203,     0,   204,   205,   206,   207,   208,   209,     0,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,     0,   233,     0,     0,   154,   155,   156,     0,
     234,   235,   236,   237,     0,     0,   238,   239,   240,   241,
       0,     0,   242,     0,     0,   243,     0,   244,   245,   246,
     247,   248,     0,   157,     0,     0,     0,     0,   249,     0,
       0,     0,     0,     0,     0,     0,   250,     0,   552,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,     0,   154,   155,   156,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,     0,   157,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   250,     0,   569,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,     0,     0,     0,   182,     0,     0,   183,   184,
     185,   186,     0,   187,     0,     0,   188,   189,   190,   191,
     192,   193,     0,     0,     0,     0,     0,   194,   195,   196,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     198,   199,   200,   201,   202,     0,     0,     0,   203,     0,
     204,   205,   206,   207,   208,   209,     0,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
       0,   233,     0,     0,   154,   155,   156,     0,   234,   235,
     236,   237,     0,     0,   238,   239,   240,   241,     0,     0,
     242,     0,     0,   243,     0,   244,   245,   246,   247,   248,
       0,   157,     0,     0,     0,     0,   249,     0,     0,     0,
       0,     0,     0,     0,   250,     0,   600,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,     0,   154,   155,   156,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,     0,
     157,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   250,     0,   602,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
       0,     0,     0,   182,     0,     0,   183,   184,   185,   186,
       0,   187,     0,     0,   188,   189,   190,   191,   192,   193,
       0,     0,     0,     0,     0,   194,   195,   196,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   197,   198,   199,
     200,   201,   202,     0,     0,     0,   203,     0,   204,   205,
     206,   207,   208,   209,     0,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,     0,   233,
       0,     0,   154,   155,   156,     0,   234,   235,   236,   237,
       0,     0,   238,   239,   240,   241,     0,     0,   242,     0,
       0,   243,     0,   244,   245,   246,   247,   248,     0,   157,
       0,     0,     0,     0,   249,     0,     0,     0,     0,     0,
       0,     0,   250,     0,   619,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
       0,   154,   155,   156,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   247,   248,     0,   157,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   250,     0,   659,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
       0,   182,     0,     0,   183,   184,   185,   186,     0,   187,
       0,     0,   188,   189,   190,   191,   192,   193,     0,     0,
       0,     0,     0,   194,   195,   196,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,   198,   199,   200,   201,
     202,     0,     0,     0,   203,     0,   204,   205,   206,   207,
     208,   209,     0,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,     0,   233,     0,     0,
     154,   155,   156,     0,   234,   235,   236,   237,     0,     0,
     238,   239,   240,   241,     0,     0,   242,     0,     0,   243,
       0,   244,   245,   246,   247,   248,     0,   157,     0,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,     0,
     250,     0,   661,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   253,   254,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   250,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,   154,   155,   156,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   157,     0,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   257,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,   154,   155,   156,     0,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   157,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,   154,   155,   156,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   157,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   262,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
     154,   155,   156,     0,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   247,   248,   157,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   268,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   270,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,   154,   155,   156,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   157,     0,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   272,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,   154,   155,   156,     0,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   157,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   279,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,   154,   155,   156,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   157,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   282,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
     154,   155,   156,     0,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   247,   248,   157,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   285,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   287,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,   154,   155,   156,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   157,     0,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   293,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,   154,   155,   156,     0,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   157,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   304,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,   154,   155,   156,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   157,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   307,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
     154,   155,   156,     0,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   247,   248,   157,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   309,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   315,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,   154,   155,   156,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   157,     0,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   317,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,   154,   155,   156,     0,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   157,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   320,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,   154,   155,   156,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   157,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   322,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
     154,   155,   156,     0,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   247,   248,   157,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   327,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   332,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,   154,   155,   156,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   157,     0,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   343,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,   154,   155,   156,     0,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   157,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   345,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,   154,   155,   156,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   157,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   348,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
     154,   155,   156,     0,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   247,   248,   157,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   351,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   247,   248,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   509,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,   154,   155,   156,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,   247,   248,   157,     0,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,   511,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,     0,   182,     0,     0,   183,
     184,   185,   186,     0,   187,     0,     0,   188,   189,   190,
     191,   192,   193,     0,     0,     0,     0,     0,   194,   195,
     196,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     197,   198,   199,   200,   201,   202,     0,     0,     0,   203,
       0,   204,   205,   206,   207,   208,   209,     0,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,     0,   233,     0,   154,   155,   156,     0,     0,   234,
     235,   236,   237,     0,     0,   238,   239,   240,   241,     0,
       0,   242,     0,     0,   243,     0,   244,   245,   246,   247,
     248,   157,     0,     0,     0,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,   513,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,     0,     0,     0,   182,     0,     0,   183,   184,   185,
     186,     0,   187,     0,     0,   188,   189,   190,   191,   192,
     193,     0,     0,     0,     0,     0,   194,   195,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   197,   198,
     199,   200,   201,   202,     0,     0,     0,   203,     0,   204,
     205,   206,   207,   208,   209,     0,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,     0,
     233,     0,   154,   155,   156,     0,     0,   234,   235,   236,
     237,     0,     0,   238,   239,   240,   241,     0,     0,   242,
       0,     0,   243,     0,   244,   245,   246,   247,   248,   157,
       0,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,     0,     0,   515,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,     0,
       0,     0,   182,     0,     0,   183,   184,   185,   186,     0,
     187,     0,     0,   188,   189,   190,   191,   192,   193,     0,
       0,     0,     0,     0,   194,   195,   196,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   197,   198,   199,   200,
     201,   202,     0,     0,     0,   203,     0,   204,   205,   206,
     207,   208,   209,     0,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,     0,   233,     0,
     154,   155,   156,     0,     0,   234,   235,   236,   237,     0,
       0,   238,   239,   240,   241,     0,     0,   242,     0,     0,
     243,     0,   244,   245,   246,   517,   248,   157,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,     0,
       0,   250,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,     0,     0,     0,
     182,     0,     0,   183,   184,   185,   186,     0,   187,     0,
       0,   188,   189,   190,   191,   192,   193,     0,     0,     0,
       0,     0,   194,   195,   196,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   197,   198,   199,   200,   201,   202,
       0,     0,     0,   203,     0,   204,   205,   206,   207,   208,
     209,     0,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,     0,   233,     0,   154,   155,
     156,     0,     0,   234,   235,   236,   237,     0,     0,   238,
     239,   240,   241,     0,     0,   242,     0,     0,   243,     0,
     244,   245,   246,   957,   248,   157,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   250,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,     0,   182,     0,
       0,   183,   184,   185,   186,     0,   187,     0,     0,   188,
     189,   190,   191,   192,   193,     0,     0,     0,     0,     0,
     194,   195,   196,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   197,   198,   199,   200,   201,   202,     0,     0,
       0,   203,     0,   204,   205,   206,   207,   208,   209,     0,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,     0,   233,     0,     0,     0,     0,     0,
       0,   234,   235,   236,   237,     0,     0,   238,   239,   240,
     241,     0,     0,   242,     0,     0,   243,     0,   244,   245,
     246,  1019,   248,     0,     0,     0,     0,     0,     0,   249,
     458,   459,   460,     1,     2,     0,     0,   250,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
       0,    15,    16,    17,    18,    19,    20,    21,     0,     0,
      22,    23,     0,    24,    25,    26,    27,    28,    29,     0,
      30,    31,   464,   465,     0,    32,    33,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    35,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,   458,   459,
     460,     0,    37,    38,    39,    40,    41,   458,   459,   460,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,   461,    54,
       0,     0,     0,     0,     0,     0,    55,   461,   462,   463,
     464,   465,     0,     0,     0,     0,     0,     0,     0,   464,
     465,   466,   467,   468,   469,   470,   471,   472,     0,   473,
      56,     0,    57,    58,    59,    60,    61,    62,     0,     0,
       0,     0,    63,    64,     0,     0,     0,     0,    65,    66,
       0,    67,    68,     0,     0,     0,     0,     0,    69,    70,
       1,     2,    71,     0,     0,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,    20,    21,     0,     0,    22,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
       0,     0,    32,    33,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   466,   467,
     468,   469,   470,   471,   472,     0,   473,     0,     0,     0,
     698,   669,     0,    34,    35,     0,   458,   459,   460,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,   458,   459,   460,    42,    43,    44,
      45,    46,    47,    48,    49,    50,   461,   462,   463,     0,
       0,     0,    51,    52,    53,     0,    54,     0,   464,   465,
       0,     0,     0,    55,   461,   462,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,    57,
      58,    59,    60,    61,    62,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,    65,    66,     0,    67,    68,
       0,     0,     1,     2,     0,    69,    70,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     0,     0,
       0,     0,     0,     0,     0,     0,    21,     0,     0,    22,
      23,     0,    24,    25,    26,    27,    28,    29,     0,   561,
      31,     0,     0,     0,    32,    33,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,   758,
       0,     0,   759,     0,     0,   466,   467,   468,   469,   470,
     471,   472,     0,   473,     0,    34,    35,   811,     0,     0,
     812,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,   458,   459,   460,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,     0,    54,     0,
       0,     0,     0,     0,     0,    55,   461,   462,   463,     0,
       0,     0,     0,   458,   459,   460,     0,     0,   464,   465,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,    57,    58,    59,    60,    61,    62,   458,   459,   460,
       0,    63,    64,   461,   462,   463,     0,    65,    66,     0,
      67,    68,     0,     0,     0,   464,   465,    69,    70,   458,
     459,   460,     0,     0,     0,     0,     0,   461,   462,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   464,
     465,     0,     0,     0,   458,   459,   460,     0,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   465,   458,   459,   460,     0,     0,     0,     0,
       0,     0,     0,     0,   461,   462,   463,   466,   467,   468,
     469,   470,   471,   472,     0,   473,   464,   465,     0,   816,
       0,     0,   817,   461,   462,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   464,   465,     0,     0,     0,
       0,     0,     0,     0,   466,   467,   468,   469,   470,   471,
     472,     0,   473,     0,     0,     0,   818,     0,     0,   819,
       0,     0,     0,     0,     0,     0,     0,     0,   466,   467,
     468,   469,   470,   471,   472,     0,   473,     0,     0,     0,
     820,     0,     0,   821,     0,     0,     0,     0,     0,     0,
     466,   467,   468,   469,   470,   471,   472,     0,   473,     0,
       0,   458,   459,   460,     0,   935,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   458,   459,   460,     0,     0,     0,
     938,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,   458,   459,   460,     0,   940,
       0,     0,     0,     0,   461,   462,   463,     0,     0,     0,
       0,   458,   459,   460,     0,     0,   464,   465,     0,     0,
       0,     0,     0,     0,     0,   461,   462,   463,   458,   459,
     460,     0,     0,     0,     0,     0,     0,   464,   465,     0,
       0,   461,   462,   463,   458,   459,   460,     0,     0,     0,
       0,     0,     0,   464,   465,     0,     0,     0,   461,   462,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     464,   465,     0,     0,   461,   462,   463,     0,     0,     0,
       0,     0,   458,   459,   460,     0,   464,   465,     0,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,   458,   459,   460,     0,   941,     0,     0,
       0,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,     0,     0,
     950,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,     0,     0,
       0,   951,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   458,   459,   460,   952,     0,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   458,   459,
     460,     0,     0,     0,   953,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   461,   462,   463,   700,   669,     0,
       0,     0,     0,     0,     0,     0,   464,   465,   461,   462,
     463,   458,   459,   460,     0,     0,     0,     0,     0,     0,
     464,   465,     0,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   458,   459,   460,   702,   669,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   707,   669,   458,   459,
     460,     0,   461,   462,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   464,   465,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
     463,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     464,   465,     0,     0,     0,   466,   467,   468,   469,   470,
     471,   472,     0,   473,     0,     0,     0,   709,   669,   466,
     467,   468,   469,   470,   471,   472,     0,   473,     0,     0,
       0,   711,   669,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   714,   669,   458,   459,   460,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,   716,   669,   458,   459,   460,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,   718,   669,   458,   459,   460,     0,   461,   462,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   464,
     465,   458,   459,   460,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,   463,     0,     0,   458,   459,
     460,     0,     0,     0,     0,   464,   465,     0,     0,     0,
       0,   461,   462,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   464,   465,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,   725,   669,   458,   459,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,   727,
     669,   458,   459,   460,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     729,   669,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   733,   669,   458,   459,
     460,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   735,   669,   458,   459,   460,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   461,   462,
     463,   737,   669,   458,   459,   460,     0,     0,     0,     0,
     464,   465,     0,     0,     0,     0,   461,   462,   463,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   464,   465,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   739,   669,
     458,   459,   460,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   742,   669,   458,   459,   460,     0,
     461,   462,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   464,   465,   458,   459,   460,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,   744,   669,     0,   461,   462,   463,   466,   467,   468,
     469,   470,   471,   472,     0,   473,   464,   465,     0,   746,
     669,   458,   459,   460,   466,   467,   468,   469,   470,   471,
     472,     0,   473,     0,     0,     0,   748,   669,   458,   459,
     460,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   464,   465,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,   752,   669,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   458,   459,   460,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,   838,
     669,   458,   459,   460,     0,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   461,   462,   463,   839,   669,   458,
     459,   460,     0,     0,     0,     0,   464,   465,     0,     0,
       0,   461,   462,   463,     0,     0,   458,   459,   460,     0,
       0,     0,     0,   464,   465,     0,     0,     0,     0,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   840,   669,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,   841,   669,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   458,   459,   460,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   458,
     459,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,   669,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,     0,   764,   458,   459,   460,     0,
     466,   467,   468,   469,   470,   471,   472,     0,   473,     0,
     458,   459,   460,   765,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,   461,   462,   463,     0,
     767,     0,     0,     0,     0,     0,     0,     0,   464,   465,
     461,   462,   463,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   464,   465,     0,     0,     0,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   458,   459,   460,     0,   775,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,     0,   776,   458,   459,   460,     0,     0,     0,
       0,     0,   461,   462,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   464,   465,     0,     0,     0,     0,
     458,   459,   460,     0,   461,   462,   463,   466,   467,   468,
     469,   470,   471,   472,     0,   473,   464,   465,     0,     0,
     777,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     461,   462,   463,     0,   778,   458,   459,   460,     0,     0,
       0,     0,   464,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     458,   459,   460,     0,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
       0,   779,   458,   459,   460,     0,     0,     0,     0,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,     0,   780,     0,   458,   459,
     460,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,   781,   458,
     459,   460,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,     0,   782,   458,   459,   460,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   465,     0,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   461,   462,   463,     0,   783,
     458,   459,   460,     0,     0,     0,     0,   464,   465,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
       0,     0,     0,     0,   784,     0,   458,   459,   460,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,     0,   785,   458,   459,   460,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,     0,   786,   458,   459,   460,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,     0,   787,   458,   459,   460,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,     0,   789,
     458,   459,   460,     0,   461,   462,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   465,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     461,   462,   463,     0,   790,   458,   459,   460,     0,     0,
       0,     0,   464,   465,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,     0,
     792,   458,   459,   460,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
       0,   793,   458,   459,   460,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,     0,   796,   458,   459,
     460,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,   797,   458,
     459,   460,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,     0,   798,   458,   459,   460,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   465,     0,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   461,   462,   463,     0,   799,
     458,   459,   460,     0,     0,     0,     0,   464,   465,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,     0,   800,   458,   459,   460,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,     0,   801,   458,   459,   460,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,     0,   803,   458,   459,   460,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,     0,   804,   458,   459,   460,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,     0,   805,
     458,   459,   460,     0,   461,   462,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   465,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     461,   462,   463,     0,   806,   458,   459,   460,     0,     0,
       0,     0,   464,   465,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,     0,
     807,   458,   459,   460,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
       0,   889,   458,   459,   460,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,     0,   900,   458,   459,
     460,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,   910,   458,
     459,   460,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,     0,   929,   458,   459,   460,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   465,     0,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   461,   462,   463,     0,   930,
     458,   459,   460,     0,     0,     0,     0,   464,   465,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,     0,   932,   458,   459,   460,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,     0,   942,   458,   459,   460,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,     0,   945,   458,   459,   460,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,     0,   946,   458,   459,   460,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,     0,   949,
     458,   459,   460,     0,   461,   462,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   465,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     461,   462,   463,     0,   964,   458,   459,   460,     0,     0,
       0,     0,   464,   465,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,     0,
     965,   458,   459,   460,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
       0,   966,   458,   459,   460,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,     0,   967,   458,   459,
     460,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,   986,   458,
     459,   460,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,     0,   997,   458,   459,   460,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   465,     0,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   461,   462,   463,     0,   998,
     458,   459,   460,     0,     0,     0,     0,   464,   465,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,     0,  1000,   458,   459,   460,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,     0,  1008,   458,   459,   460,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,     0,  1011,   458,   459,   460,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,     0,  1012,   458,   459,   460,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,     0,  1015,
     458,   459,   460,     0,   461,   462,   463,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   464,   465,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     461,   462,   463,     0,  1016,   458,   459,   460,     0,     0,
       0,     0,   464,   465,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,     0,
    1030,   458,   459,   460,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
       0,  1035,   458,   459,   460,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,     0,  1036,   458,   459,
     460,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,     0,  1042,   458,
     459,   460,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,     0,  1048,   458,   459,   460,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   465,     0,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   461,   462,   463,   486,     0,
     458,   459,   460,     0,     0,     0,     0,   464,   465,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   488,     0,   458,   459,   460,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,   490,     0,   458,   459,   460,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,   495,     0,   458,   459,   460,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   497,   458,   459,   460,     0,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   499,     0,
     458,   459,   460,   461,   462,   463,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   464,   465,     0,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     461,   462,   463,   502,     0,   458,   459,   460,     0,     0,
       0,     0,   464,   465,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,   504,
       0,   458,   459,   460,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     508,   458,   459,   460,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   519,     0,   458,   459,
     460,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   521,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,   523,     0,     0,     0,   461,   462,   463,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   464,
     465,     0,     0,     0,     0,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,     0,     0,     0,   527,   458,
     459,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   529,     0,   458,   459,   460,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   531,   458,   459,   460,     0,     0,
       0,     0,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,   533,   458,   459,   460,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     534,   458,   459,   460,     0,     0,     0,     0,     0,     0,
       0,     0,   461,   462,   463,     0,     0,   458,   459,   460,
       0,     0,     0,     0,   464,   465,     0,     0,     0,     0,
       0,   461,   462,   463,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   464,   465,     0,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   536,   458,   459,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,   540,
     458,   459,   460,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   545,   458,
     459,   460,     0,     0,     0,     0,     0,     0,     0,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,   548,     0,     0,     0,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   551,   458,   459,   460,   466,   467,
     468,   469,   470,   471,   472,     0,   473,     0,     0,     0,
     706,   458,   459,   460,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,   462,   463,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   464,   465,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   762,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     458,   459,   460,   763,     0,     0,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   794,   458,   459,   460,     0,     0,     0,     0,
     461,   462,   463,     0,     0,     0,     0,     0,     0,   458,
     459,   460,   464,   465,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   795,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   824,   458,   459,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   458,   459,
     460,     0,     0,     0,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     826,   458,   459,   460,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,   829,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   831,   458,   459,   460,
     466,   467,   468,   469,   470,   471,   472,     0,   473,     0,
       0,     0,   837,   458,   459,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   461,   462,   463,
       0,     0,     0,     0,     0,     0,   458,   459,   460,   464,
     465,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   845,   458,
     459,   460,     0,     0,     0,     0,   461,   462,   463,   466,
     467,   468,   469,   470,   471,   472,     0,   473,   464,   465,
       0,   847,   458,   459,   460,     0,     0,     0,     0,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   849,   458,   459,   460,     0,     0,
       0,     0,   461,   462,   463,     0,     0,     0,     0,     0,
       0,   458,   459,   460,   464,   465,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     851,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   855,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,   466,   467,   468,
     469,   470,   471,   472,     0,   473,     0,     0,     0,   856,
     458,   459,   460,     0,     0,     0,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   858,   458,   459,   460,     0,     0,     0,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,   890,     0,     0,     0,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   892,   458,
     459,   460,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,   894,   458,   459,   460,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,   463,     0,     0,     0,     0,     0,     0,   458,   459,
     460,   464,   465,     0,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     898,   458,   459,   460,     0,     0,     0,     0,   461,   462,
     463,   466,   467,   468,   469,   470,   471,   472,     0,   473,
     464,   465,     0,   904,   458,   459,   460,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   912,   458,   459,   460,
       0,     0,     0,     0,   461,   462,   463,     0,     0,     0,
       0,     0,     0,   458,   459,   460,   464,   465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   915,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   917,   458,
     459,   460,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,   471,   472,     0,   473,     0,     0,
       0,   919,   458,   459,   460,     0,     0,     0,     0,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   922,   458,   459,   460,     0,     0,
       0,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,   924,     0,     0,
       0,     0,     0,     0,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     927,   458,   459,   460,   466,   467,   468,   469,   470,   471,
     472,     0,   473,     0,     0,     0,   943,   458,   459,   460,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,     0,     0,     0,     0,     0,     0,
     458,   459,   460,   464,   465,     0,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   944,   458,   459,   460,     0,     0,     0,     0,
     461,   462,   463,   466,   467,   468,   469,   470,   471,   472,
       0,   473,   464,   465,     0,   948,   458,   459,   460,     0,
       0,     0,     0,   461,   462,   463,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,   956,   458,
     459,   460,     0,     0,     0,     0,   461,   462,   463,     0,
       0,     0,     0,     0,     0,   458,   459,   460,   464,   465,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,   959,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
     961,   458,   459,   460,     0,     0,     0,     0,     0,     0,
       0,   466,   467,   468,   469,   470,   471,   472,     0,   473,
       0,     0,     0,   975,   458,   459,   460,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,   978,   458,   459,   460,
       0,     0,     0,     0,   461,   462,   463,   466,   467,   468,
     469,   470,   471,   472,     0,   473,   464,   465,     0,   987,
       0,     0,     0,     0,     0,     0,     0,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,   990,   458,   459,   460,   466,   467,   468,   469,
     470,   471,   472,     0,   473,     0,     0,     0,   994,   458,
     459,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   461,   462,   463,     0,     0,     0,     0,
       0,     0,   458,   459,   460,   464,   465,     0,     0,   461,
     462,   463,   466,   467,   468,   469,   470,   471,   472,     0,
     473,   464,   465,     0,  1004,   458,   459,   460,     0,     0,
       0,     0,   461,   462,   463,   466,   467,   468,   469,   470,
     471,   472,     0,   473,   464,   465,     0,  1009,   458,   459,
     460,     0,     0,     0,     0,   461,   462,   463,   466,   467,
     468,   469,   470,   471,   472,     0,   473,   464,   465,     0,
    1017,   458,   459,   460,     0,     0,     0,     0,   461,   462,
     463,     0,     0,     0,     0,     0,     0,   458,   459,   460,
     464,   465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   461,   462,   463,   466,   467,   468,   469,   470,   471,
     472,     0,   473,   464,   465,     0,  1021,   461,   462,   463,
     466,   467,   468,   469,   470,   471,   472,     0,   473,   464,
     465,     0,  1026,   458,   459,   460,     0,     0,     0,     0,
       0,     0,     0,   466,   467,   468,   469,   470,   471,   472,
       0,   473,     0,     0,     0,  1028,     0,     0,     0,     0,
       0,     0,     0,   461,   462,     0,   466,   467,   468,   469,
     470,   471,   472,     0,   473,   464,   465,     0,  1032,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   466,
     467,   468,   469,   470,   471,   472,     0,   473,     0,     0,
       0,  1037,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   466,   467,   468,   469,   470,   471,   472,     0,
     473,     0,     0,     0,  1040,     0,     0,     0,   466,   467,
     468,   469,   470,   471,   472,     0,   473,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   466,   467,   468,   469,   470,   471,
     472,     0,   473
};

static const yytype_int16 yycheck[] =
{
       1,   194,     3,     4,     5,    35,     7,     8,   185,    10,
     178,   178,   179,    14,    19,   178,    17,   178,    23,    20,
     197,   178,    23,    24,    25,    26,    42,    43,    44,   178,
      31,   194,   200,    34,    35,    36,    37,   200,    39,   185,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     178,   197,   194,    54,   192,   193,    57,   192,   193,   197,
     196,   197,   197,   194,    65,    66,    67,    68,   194,     5,
       6,     7,   192,   193,   192,   193,   178,   197,   194,   197,
     192,   193,   178,   179,   195,   197,   195,   198,     0,   198,
     178,   179,   192,   193,   192,   193,    32,   192,   193,   192,
     193,   192,   196,   197,   196,   197,   193,    20,   197,   195,
     194,   194,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,   194,   194,   194,    75,
      84,    85,    78,    79,    80,    81,   194,    83,   194,   194,
      86,    87,    88,    89,    90,    91,   194,   194,   194,   194,
     194,    97,    98,    99,   197,   194,   194,   183,   184,   185,
     194,   187,   194,   109,   110,   111,   112,   113,   114,   194,
     194,   182,   118,   199,   120,   121,   122,   123,   124,   125,
     194,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   194,   151,    42,    43,    44,   194,
     194,   194,   158,   159,   160,   161,   194,   194,   164,   165,
     166,   167,   194,   194,   170,   194,   194,   173,   194,   175,
     176,   177,   178,   179,   188,   189,   194,   191,   249,   250,
     186,   194,   194,   194,   194,   194,   257,   194,   194,   260,
     194,   262,   194,   194,   200,   266,   194,   268,   194,   270,
     194,   272,    42,    43,    44,   194,   194,   194,   279,   194,
     194,   282,   202,   194,   285,   194,   287,    42,    43,    44,
     194,   194,   293,   194,   295,   296,   297,   298,   194,   194,
     301,   194,   194,   304,   194,   194,   307,   194,   309,   194,
     194,   194,   194,   194,   315,   194,   317,    84,    85,   320,
     194,   322,   194,   353,   194,   194,   327,   194,   194,   194,
     194,   332,    42,    43,    44,   194,   194,   194,   194,   194,
     194,   194,   343,   194,   345,   194,   194,   348,   194,   194,
     351,   194,   353,   354,   194,   356,   178,   183,   184,   185,
     185,   187,   196,   364,   365,   366,   367,   368,   196,   195,
     196,   191,   196,   196,   201,   376,   377,   196,   379,   380,
     381,   382,   383,   384,   385,   196,   387,   388,   389,   390,
     196,   196,   393,   394,   395,   396,   196,   196,   196,   196,
     196,   196,   196,   196,   405,   406,   407,   408,   409,   410,
     411,   412,   196,   183,   184,   185,   196,   187,   196,   186,
     187,   188,   189,   196,   191,   195,   196,   196,   183,   184,
     185,   196,   187,   196,   196,   196,   196,   196,   196,   196,
     195,   196,   196,   444,   445,   446,   196,   196,   196,   196,
     451,   452,   453,   454,   196,   196,   196,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   472,   473,   183,   184,   185,   196,   187,   196,   196,
     481,   196,   483,   484,   196,   486,   196,   488,   196,   490,
     196,   196,   196,   196,   495,   196,   497,   196,   499,   500,
     196,   196,   202,   197,   197,   187,   185,   508,   509,   201,
     511,   185,   513,   198,   515,     5,     6,     7,   519,   185,
     198,   198,   523,   198,   525,   185,   527,   178,   529,   196,
      74,   361,   533,   534,   706,   536,   868,   358,    -1,   540,
      -1,    -1,    32,   360,   545,    -1,    -1,    -1,   549,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,
      80,    81,    -1,    83,    -1,    -1,    86,    87,    88,    89,
      90,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,   109,
     110,   111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,
     120,   121,   122,   123,   124,   125,    -1,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
      -1,   151,    42,    43,    44,    -1,    -1,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,    -1,   173,    -1,   175,   176,   177,   178,   179,
     183,   184,   185,    -1,   187,    -1,   186,   698,    -1,   700,
      -1,   702,    -1,   196,   194,   706,   707,    -1,   709,    -1,
     711,    -1,    -1,    -1,    -1,    -1,    -1,   718,    -1,    -1,
      -1,    -1,    -1,    -1,   725,    42,    43,    44,   729,    -1,
     731,    -1,   733,    -1,   735,    -1,   737,    -1,   739,    -1,
      -1,   742,    -1,   744,    -1,   746,    -1,    -1,   749,    -1,
      -1,    -1,    -1,   754,   183,   184,   185,   758,   187,   760,
     761,   762,   763,    -1,    -1,    -1,    -1,   196,   769,   770,
     771,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,   794,   795,    42,    43,    44,    -1,   829,
      -1,    -1,    -1,   183,   184,   185,    -1,   187,    -1,    -1,
     811,    42,    43,    44,    -1,   816,   196,   818,    -1,   820,
      -1,    -1,    -1,   824,    -1,   826,    -1,    -1,   829,    -1,
     831,    -1,   833,    -1,    -1,    -1,   837,   838,   839,   840,
     841,    -1,    -1,    -1,   845,    -1,   847,    -1,    -1,    -1,
     851,    -1,    -1,    -1,   855,   856,    -1,   858,    -1,    -1,
      -1,    -1,   863,    28,    -1,   866,   183,   184,   185,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   890,
      -1,   892,    -1,   894,    -1,    -1,    -1,   898,    -1,    -1,
      -1,    -1,    -1,   904,    -1,    -1,    -1,    72,    73,    74,
      -1,   912,    -1,    -1,   915,    -1,    -1,    -1,   919,    84,
      85,   922,    -1,   924,    -1,    -1,   927,    -1,    -1,   959,
     931,   183,   184,   185,    -1,   187,   183,   184,   185,    -1,
     187,    -1,   943,   944,   196,    -1,    -1,   948,    -1,   196,
      -1,    -1,   183,   184,   185,   956,   187,    -1,   959,    -1,
     961,   962,    42,    43,    44,   196,    -1,    -1,    -1,    -1,
      -1,   972,    -1,    -1,   975,     1,     2,   978,    -1,    -1,
     981,    -1,    -1,    -1,    -1,    -1,   987,    -1,    -1,   990,
      -1,  1021,    -1,   994,    -1,    -1,    -1,    23,    24,    -1,
    1001,    -1,    -1,  1004,     5,     6,     7,    -1,  1009,    -1,
      36,    -1,    38,    39,    40,    -1,  1017,    -1,   183,   184,
     185,   186,   187,   188,   189,    51,   191,  1028,    -1,    -1,
      56,    32,    58,    -1,    60,    61,    -1,    -1,    -1,  1040,
      66,    67,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    42,    43,    44,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    -1,    -1,   183,   184,   185,    -1,   187,   109,   110,
     111,   112,   113,   114,    -1,    -1,   196,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,   195,    -1,    29,    -1,    -1,    -1,
      -1,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,   183,   184,   185,    -1,   187,    -1,    42,    43,
      44,    -1,    -1,    -1,   250,   196,    -1,    -1,    42,    43,
      44,   257,    -1,    -1,   260,    -1,   262,    -1,    -1,    72,
      73,    74,   268,    -1,   270,    -1,   272,    42,    43,    44,
      -1,    84,    85,   279,    -1,    -1,   282,    -1,    -1,   285,
      -1,   287,    42,    43,    44,    -1,    -1,   293,    42,    43,
      44,    -1,    -1,    -1,    -1,   301,    -1,    -1,   304,    -1,
      -1,   307,    -1,   309,    -1,    42,    43,    44,    -1,   315,
      -1,   317,    -1,    -1,   320,    -1,   322,    -1,    72,    73,
      74,   327,    -1,    -1,    -1,    -1,   332,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    -1,   343,    -1,   345,
      -1,    -1,   348,    -1,    -1,   351,    42,    43,    44,   355,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,   367,    -1,   369,   370,   371,   372,   373,   374,   375,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,   183,
     184,   185,    -1,   187,    -1,    -1,   392,    -1,   394,   183,
     184,   185,   196,   187,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,   196,    -1,    -1,    -1,    -1,   413,   183,   184,
     185,    -1,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   196,    -1,   183,   184,   185,    -1,   187,    -1,   183,
     184,   185,   186,   187,   188,   189,   196,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   199,   183,   184,   185,    -1,
     187,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,   196,
      -1,    -1,    -1,    -1,   470,    -1,    -1,    -1,    -1,   475,
     476,   477,   478,   479,   480,   481,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
      -1,   187,    -1,   183,   184,   185,   502,   187,   504,    -1,
     196,    -1,    -1,   509,    -1,   511,   196,   513,    -1,   515,
       5,     6,     7,    -1,    -1,   521,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   531,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   183,
     184,   185,   548,   187,    -1,   551,    -1,    -1,    -1,    -1,
     556,    -1,   196,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    42,    43,    44,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,   183,   184,   185,
      -1,   187,    97,    98,    99,    -1,    42,    43,    44,    -1,
     196,    42,    43,    44,   109,   110,   111,   112,   113,   114,
      42,    43,    44,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    42,    43,    44,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,   202,   714,    -1,
     716,    -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,
     185,   727,   187,    -1,    -1,    -1,    -1,    84,    85,    -1,
     195,   737,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   748,    -1,    -1,    -1,   752,   183,   184,   185,
      -1,   187,   183,   184,   185,    -1,   187,    -1,    -1,   195,
      -1,   183,   184,   185,   195,   187,    -1,    -1,   774,    -1,
      -1,    -1,    -1,   195,    -1,    -1,     3,     4,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    31,    -1,    33,    34,    35,    36,
      37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,
      -1,    42,    43,    44,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,   845,
      -1,   198,    -1,   849,    -1,   851,    -1,    -1,    -1,    76,
      77,    42,    43,    44,    -1,    82,    -1,    -1,    42,    43,
      44,    -1,   868,    -1,   870,    92,    93,    94,    95,    96,
      -1,    -1,    -1,   100,   101,   102,   103,   104,   105,   106,
     107,   108,    -1,    -1,    -1,    -1,    -1,    -1,   115,   116,
     117,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,   912,    -1,    -1,    -1,
      -1,   917,    -1,   919,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,    -1,   162,   163,    -1,    -1,    -1,
      -1,   168,   169,    -1,   171,   172,    42,    43,    44,    -1,
      -1,   178,   179,    -1,    -1,   182,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   192,   187,    -1,    -1,   975,
      -1,    -1,    -1,    -1,   195,    -1,   982,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     5,
       6,     7,   183,   184,   185,    -1,   187,    -1,  1004,   183,
     184,   185,    -1,   187,   195,    -1,    -1,    -1,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
    1026,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1037,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,   183,   184,   185,    75,
     187,    -1,    78,    79,    80,    81,    -1,    83,   195,    -1,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    -1,    -1,    -1,   183,   184,   185,
      -1,   187,    -1,   109,   110,   111,   112,   113,   114,   195,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,
     176,   177,   178,   179,     5,     6,     7,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,
      -1,    -1,    -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    -1,    -1,    75,    -1,    -1,    78,    79,    80,
      81,    -1,    83,    -1,    -1,    86,    87,    88,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,   109,   110,
     111,   112,   113,   114,    -1,    -1,    -1,   118,    -1,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,     5,
       6,     7,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,   200,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,   183,   184,   185,    75,
     187,    -1,    78,    79,    80,    81,    -1,    83,   195,    -1,
      86,    87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   109,   110,   111,   112,   113,   114,    -1,
      -1,    -1,   118,    -1,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,
     176,   177,   178,   179,     5,     6,     7,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,
      -1,    -1,    -1,    -1,   200,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
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
     151,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,     5,
       6,     7,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,   200,
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
     146,   147,   148,   149,    -1,   151,    -1,    -1,     5,     6,
       7,    -1,   158,   159,   160,   161,    -1,    -1,   164,   165,
     166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,
     176,   177,   178,   179,    -1,    32,    -1,    -1,    -1,    -1,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,
     196,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    -1,    -1,    75,    -1,
      -1,    78,    79,    80,    81,    -1,    83,    -1,    -1,    86,
      87,    88,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,   112,   113,   114,    -1,    -1,
      -1,   118,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,    -1,   151,    -1,    -1,     5,     6,     7,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    -1,    32,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,
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
     148,   149,    -1,   151,    -1,    -1,     5,     6,     7,    -1,
     158,   159,   160,   161,    -1,    -1,   164,   165,   166,   167,
      -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,
     178,   179,    -1,    32,    -1,    -1,    -1,    -1,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    48,
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
     149,    -1,   151,    -1,    -1,     5,     6,     7,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    -1,    32,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,   196,    48,    49,
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
      -1,   151,    -1,    -1,     5,     6,     7,    -1,   158,   159,
     160,   161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,
     170,    -1,    -1,   173,    -1,   175,   176,   177,   178,   179,
      -1,    32,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   194,    -1,   196,    48,    49,    50,
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
     151,    -1,    -1,     5,     6,     7,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    -1,
      32,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,   196,    48,    49,    50,    51,
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
      -1,    -1,     5,     6,     7,    -1,   158,   159,   160,   161,
      -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,
      -1,   173,    -1,   175,   176,   177,   178,   179,    -1,    32,
      -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   194,    -1,   196,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
      -1,     5,     6,     7,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    -1,    32,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,   196,    48,    49,    50,    51,    52,    53,
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
       5,     6,     7,    -1,   158,   159,   160,   161,    -1,    -1,
     164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,
      -1,   175,   176,   177,   178,   179,    -1,    32,    -1,    -1,
      -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     194,    -1,   196,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     147,   148,   149,    -1,   151,    -1,     5,     6,     7,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    32,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    48,
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
     149,    -1,   151,    -1,     5,     6,     7,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,    48,    49,    50,
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
     151,    -1,     5,     6,     7,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    32,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
       5,     6,     7,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    32,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     147,   148,   149,    -1,   151,    -1,     5,     6,     7,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    32,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    48,
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
     149,    -1,   151,    -1,     5,     6,     7,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,    48,    49,    50,
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
     151,    -1,     5,     6,     7,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    32,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
       5,     6,     7,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    32,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     147,   148,   149,    -1,   151,    -1,     5,     6,     7,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    32,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    48,
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
     149,    -1,   151,    -1,     5,     6,     7,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,    48,    49,    50,
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
     151,    -1,     5,     6,     7,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    32,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
       5,     6,     7,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    32,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     147,   148,   149,    -1,   151,    -1,     5,     6,     7,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    32,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    48,
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
     149,    -1,   151,    -1,     5,     6,     7,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,    48,    49,    50,
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
     151,    -1,     5,     6,     7,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    32,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
       5,     6,     7,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    32,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     147,   148,   149,    -1,   151,    -1,     5,     6,     7,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    32,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    48,
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
     149,    -1,   151,    -1,     5,     6,     7,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,    48,    49,    50,
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
     151,    -1,     5,     6,     7,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    32,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
       5,     6,     7,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    32,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     147,   148,   149,    -1,   151,    -1,     5,     6,     7,    -1,
      -1,   158,   159,   160,   161,    -1,    -1,   164,   165,   166,
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    32,    -1,    -1,    -1,    -1,    -1,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,    -1,    48,
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
     149,    -1,   151,    -1,     5,     6,     7,    -1,    -1,   158,
     159,   160,   161,    -1,    -1,   164,   165,   166,   167,    -1,
      -1,   170,    -1,    -1,   173,    -1,   175,   176,   177,   178,
     179,    32,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   194,    -1,    48,    49,    50,
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
     151,    -1,     5,     6,     7,    -1,    -1,   158,   159,   160,
     161,    -1,    -1,   164,   165,   166,   167,    -1,    -1,   170,
      -1,    -1,   173,    -1,   175,   176,   177,   178,   179,    32,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   194,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    75,    -1,    -1,    78,    79,    80,    81,    -1,
      83,    -1,    -1,    86,    87,    88,    89,    90,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,
     113,   114,    -1,    -1,    -1,   118,    -1,   120,   121,   122,
     123,   124,   125,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,    -1,   151,    -1,
       5,     6,     7,    -1,    -1,   158,   159,   160,   161,    -1,
      -1,   164,   165,   166,   167,    -1,    -1,   170,    -1,    -1,
     173,    -1,   175,   176,   177,   178,   179,    32,    -1,    -1,
      -1,    -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   194,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    78,    79,    80,    81,    -1,    83,    -1,
      -1,    86,    87,    88,    89,    90,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,   110,   111,   112,   113,   114,
      -1,    -1,    -1,   118,    -1,   120,   121,   122,   123,   124,
     125,    -1,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,    -1,   151,    -1,     5,     6,
       7,    -1,    -1,   158,   159,   160,   161,    -1,    -1,   164,
     165,   166,   167,    -1,    -1,   170,    -1,    -1,   173,    -1,
     175,   176,   177,   178,   179,    32,    -1,    -1,    -1,    -1,
      -1,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   194,
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
     167,    -1,    -1,   170,    -1,    -1,   173,    -1,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,    -1,    -1,   186,
      42,    43,    44,     3,     4,    -1,    -1,   194,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    21,    22,    23,    24,    25,    26,    27,    -1,    -1,
      30,    31,    -1,    33,    34,    35,    36,    37,    38,    -1,
      40,    41,    84,    85,    -1,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    92,    93,    94,    95,    96,    42,    43,    44,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,    72,   119,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    72,    73,    74,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
     150,    -1,   152,   153,   154,   155,   156,   157,    -1,    -1,
      -1,    -1,   162,   163,    -1,    -1,    -1,    -1,   168,   169,
      -1,   171,   172,    -1,    -1,    -1,    -1,    -1,   178,   179,
       3,     4,   182,    -1,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    31,    -1,
      33,    34,    35,    36,    37,    38,    -1,    40,    41,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,
     195,   196,    -1,    76,    77,    -1,    42,    43,    44,    82,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    92,
      93,    94,    95,    96,    42,    43,    44,   100,   101,   102,
     103,   104,   105,   106,   107,   108,    72,    73,    74,    -1,
      -1,    -1,   115,   116,   117,    -1,   119,    -1,    84,    85,
      -1,    -1,    -1,   126,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,    -1,   152,
     153,   154,   155,   156,   157,    -1,    -1,    -1,    -1,   162,
     163,    -1,    -1,    -1,    -1,   168,   169,    -1,   171,   172,
      -1,    -1,     3,     4,    -1,   178,   179,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    -1,    30,
      31,    -1,    33,    34,    35,    36,    37,    38,    -1,    40,
      41,    -1,    -1,    -1,    45,    46,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,
      -1,    -1,   198,    -1,    -1,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    -1,    76,    77,   195,    -1,    -1,
     198,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    42,    43,    44,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,    -1,
      -1,    -1,    -1,    -1,   115,   116,   117,    -1,   119,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    72,    73,    74,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,    42,    43,    44,
      -1,   162,   163,    72,    73,    74,    -1,   168,   169,    -1,
     171,   172,    -1,    -1,    -1,    84,    85,   178,   179,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    84,    85,    -1,   195,
      -1,    -1,   198,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    -1,    -1,    -1,   195,    -1,    -1,   198,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,
     195,    -1,    -1,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    -1,
      -1,    42,    43,    44,    -1,   198,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    42,    43,    44,    -1,    -1,    -1,
     198,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    42,    43,    44,    -1,   198,
      -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    72,    73,    74,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    84,    85,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    42,    43,    44,    -1,   198,    -1,    -1,
      -1,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,    -1,    -1,
     198,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,    -1,    -1,
      -1,   198,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,    42,    43,    44,   198,    -1,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    42,    43,
      44,    -1,    -1,    -1,   198,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    72,    73,    74,   195,   196,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    72,    73,
      74,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    42,    43,    44,   195,   196,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,   196,    42,    43,
      44,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    -1,    -1,    -1,   195,   196,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    -1,    -1,
      -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,   196,    42,    43,    44,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,   195,   196,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,   195,   196,    42,    43,    44,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,   195,   196,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,
     196,    42,    43,    44,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,   196,    42,    43,
      44,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,   196,    42,    43,    44,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    72,    73,
      74,   195,   196,    42,    43,    44,    -1,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,   196,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,   196,    42,    43,    44,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,   195,   196,    -1,    72,    73,    74,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    84,    85,    -1,   195,
     196,    42,    43,    44,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    -1,    -1,    -1,   195,   196,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,   195,   196,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    44,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,
     196,    42,    43,    44,    -1,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    72,    73,    74,   195,   196,    42,
      43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,   196,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,   195,   196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,   196,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,    -1,   196,    42,    43,    44,    -1,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    -1,
      42,    43,    44,   196,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    72,    73,    74,    -1,
     196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    42,    43,    44,    -1,   196,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,    -1,   196,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    72,    73,    74,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    84,    85,    -1,    -1,
     196,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      72,    73,    74,    -1,   196,    42,    43,    44,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
      -1,   196,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,    -1,   196,    -1,    42,    43,
      44,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,   196,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,    -1,   196,    42,    43,    44,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    72,    73,    74,    -1,   196,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      -1,    -1,    -1,    -1,   196,    -1,    42,    43,    44,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,    -1,   196,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,    -1,   196,    42,    43,    44,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,    -1,   196,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,    -1,   196,
      42,    43,    44,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      72,    73,    74,    -1,   196,    42,    43,    44,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,    -1,
     196,    42,    43,    44,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
      -1,   196,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,    -1,   196,    42,    43,
      44,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,   196,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,    -1,   196,    42,    43,    44,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    72,    73,    74,    -1,   196,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,    -1,   196,    42,    43,    44,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,    -1,   196,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,    -1,   196,    42,    43,    44,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,    -1,   196,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,    -1,   196,
      42,    43,    44,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      72,    73,    74,    -1,   196,    42,    43,    44,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,    -1,
     196,    42,    43,    44,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
      -1,   196,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,    -1,   196,    42,    43,
      44,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,   196,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,    -1,   196,    42,    43,    44,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    72,    73,    74,    -1,   196,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,    -1,   196,    42,    43,    44,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,    -1,   196,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,    -1,   196,    42,    43,    44,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,    -1,   196,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,    -1,   196,
      42,    43,    44,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      72,    73,    74,    -1,   196,    42,    43,    44,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,    -1,
     196,    42,    43,    44,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
      -1,   196,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,    -1,   196,    42,    43,
      44,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,   196,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,    -1,   196,    42,    43,    44,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    72,    73,    74,    -1,   196,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,    -1,   196,    42,    43,    44,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,    -1,   196,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,    -1,   196,    42,    43,    44,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,    -1,   196,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,    -1,   196,
      42,    43,    44,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      72,    73,    74,    -1,   196,    42,    43,    44,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,    -1,
     196,    42,    43,    44,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
      -1,   196,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,    -1,   196,    42,    43,
      44,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,    -1,   196,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,    -1,   196,    42,    43,    44,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    72,    73,    74,   195,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,    -1,    42,    43,    44,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,   195,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,   195,    -1,    42,    43,    44,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    -1,
      42,    43,    44,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      72,    73,    74,   195,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,
      -1,    42,    43,    44,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    -1,    42,    43,
      44,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,   195,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,    -1,    42,    43,    44,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,   195,    42,    43,    44,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    -1,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,
      42,    43,    44,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,   195,    -1,    -1,    -1,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    42,    43,    44,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    84,    85,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      42,    43,    44,   195,    -1,    -1,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    42,    43,    44,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    -1,
      -1,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    84,
      85,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    42,
      43,    44,    -1,    -1,    -1,    -1,    72,    73,    74,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    84,    85,
      -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    42,    43,    44,    -1,    -1,
      -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    84,    85,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,
      42,    43,    44,    -1,    -1,    -1,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    42,
      43,    44,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    84,    85,    -1,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    72,    73,
      74,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      84,    85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    42,    43,    44,
      -1,    -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    -1,    -1,
      -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    42,    43,    44,    -1,    -1,
      -1,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,   195,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    -1,    -1,    -1,   195,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    84,    85,    -1,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      72,    73,    74,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    84,    85,    -1,   195,    42,    43,    44,    -1,
      -1,    -1,    -1,    72,    73,    74,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    42,
      43,    44,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    84,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   183,   184,   185,   186,   187,   188,   189,    -1,   191,
      -1,    -1,    -1,   195,    42,    43,    44,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    42,    43,    44,
      -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,   185,
     186,   187,   188,   189,    -1,   191,    84,    85,    -1,   195,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    -1,    -1,    -1,   195,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    74,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    84,    85,    -1,    -1,    72,
      73,    74,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    84,    85,    -1,   195,    42,    43,    44,    -1,    -1,
      -1,    -1,    72,    73,    74,   183,   184,   185,   186,   187,
     188,   189,    -1,   191,    84,    85,    -1,   195,    42,    43,
      44,    -1,    -1,    -1,    -1,    72,    73,    74,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    84,    85,    -1,
     195,    42,    43,    44,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    72,    73,    74,   183,   184,   185,   186,   187,   188,
     189,    -1,   191,    84,    85,    -1,   195,    72,    73,    74,
     183,   184,   185,   186,   187,   188,   189,    -1,   191,    84,
      85,    -1,   195,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,   187,   188,   189,
      -1,   191,    -1,    -1,    -1,   195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    73,    -1,   183,   184,   185,   186,
     187,   188,   189,    -1,   191,    84,    85,    -1,   195,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   183,
     184,   185,   186,   187,   188,   189,    -1,   191,    -1,    -1,
      -1,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   183,   184,   185,   186,   187,   188,   189,    -1,
     191,    -1,    -1,    -1,   195,    -1,    -1,    -1,   183,   184,
     185,   186,   187,   188,   189,    -1,   191,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   187,   188,
     189,    -1,   191
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
     155,   156,   157,   162,   163,   168,   169,   171,   172,   178,
     179,   182,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,     5,     6,     7,    32,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    75,    78,    79,    80,    81,    83,    86,    87,
      88,    89,    90,    91,    97,    98,    99,   109,   110,   111,
     112,   113,   114,   118,   120,   121,   122,   123,   124,   125,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   151,   158,   159,   160,   161,   164,   165,
     166,   167,   170,   173,   175,   176,   177,   178,   179,   186,
     194,   289,   291,   178,   179,   289,   291,   194,   289,   291,
     194,   289,   194,   289,   178,   194,   200,   287,   194,   289,
     194,   289,   194,   289,   289,   289,   289,   178,   178,   194,
     289,   291,   194,   289,   291,   194,   289,   194,   289,   178,
     178,    19,    23,   194,   289,   178,   179,   178,   179,   289,
     178,   194,   287,   289,   194,   289,   291,   194,   289,   194,
     291,   289,   291,   291,   289,   194,   289,   194,   289,   289,
     194,   289,   194,   289,   289,   289,   289,   194,   289,   291,
     194,   194,   194,   289,   194,   291,   289,   291,   194,   291,
     291,   194,   178,   194,   289,   194,   289,   291,   194,   289,
     291,   194,   289,   185,   197,   185,   197,     0,   192,   207,
     193,    20,   195,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   289,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   197,   197,   289,   289,   291,    42,    43,
      44,    72,    73,    74,    84,    85,   183,   184,   185,   186,
     187,   188,   189,   191,   199,    42,    43,    44,   183,   184,
     185,   187,   199,   197,   197,   289,   195,   289,   195,   289,
     195,   178,   288,   289,   289,   195,   289,   195,   289,   195,
     185,   289,   195,   289,   195,   196,   196,   289,   195,   194,
     289,   194,   289,   194,   289,   194,   289,   178,   289,   195,
     289,   195,   289,   195,   291,   195,   289,   195,   289,   195,
     289,   195,   289,   195,   195,   289,   195,   196,   196,   289,
     195,   196,   196,   196,   289,   195,   289,   291,   195,   195,
     289,   195,   196,   287,   289,   289,   200,   286,   291,   289,
     204,    40,   219,   218,   178,   179,   196,   289,   289,   196,
     289,   289,   291,   289,   291,   291,   291,   291,   291,   291,
     291,   289,   289,   196,   289,   289,   289,   289,   289,   289,
     289,   196,   289,   289,   289,   289,   291,   289,   289,   291,
     196,   289,   196,   289,   196,   196,   196,   196,   196,   196,
     196,   196,   289,   289,   289,   289,   289,   289,   289,   196,
     289,   291,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   289,   289,   289,   196,   196,   196,   196,   196,
     289,   196,   289,   195,   202,   289,   195,   202,   289,   196,
     196,   289,   289,   289,   289,   289,   289,   289,   289,   289,
     289,   289,   289,   289,   291,   289,   289,   289,   291,   291,
     291,   291,   291,   291,   289,   291,   289,   289,   195,   289,
     195,   289,   195,   289,   196,   201,   195,   195,   289,   195,
     289,   195,   289,   289,   195,   291,   195,   291,   195,   289,
     289,   289,   289,   289,   196,   195,   289,   195,   291,   195,
     289,   195,   289,   195,   289,   195,   289,   195,   291,   195,
     289,   289,   195,   289,   195,   289,   195,   289,   195,   195,
     291,   289,   195,   291,   195,   198,   290,   291,   195,   198,
     197,   197,   195,   195,   196,   196,   196,   196,   196,   195,
     195,   195,   196,   196,   195,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   202,   195,
     198,   195,   198,   202,   195,   198,   195,   198,   195,   198,
     195,   198,   289,   289,   195,   289,   195,   288,   289,   195,
     289,   195,   289,    28,   291,   291,   289,   195,   195,   195,
     195,   195,   289,   291,   289,   195,   289,   195,   289,   195,
     289,   195,   289,   291,   289,   195,   195,   289,   195,   289,
     289,   291,   289,   195,   291,   289,   185,   201,   195,   289,
     185,   289,   289,   289,   289,   289,   289,   289,   291,   289,
     289,   198,   198,   289,   198,   198,   289,   289,   289,   196,
     195,   289,   195,   289,   195,   178,   287,   289,   195,   289,
     196,   289,   196,   196,   195,   289,   289,   289,   289,   289,
     196,   196,   195,   289,   291,   195,   289,   195,   291,   195,
     289,   291,   195,   196,   195,   289,   289,   195,   289,   196,
     196,   195,   196,   289,   196,   198,   289,   290,   198,   291,
     198,   198,   196,   195,   195,   196,   196,   196,   195,   196,
     198,   198,   198,   198,   289,   289,   195,   178,   289,   195,
     289,   195,    29,   289,   196,   196,   196,   196,   289,   291,
     289,   291,   195,   289,   291,   195,   289,   289,   195,   289,
     289,   185,   185,   289,   289,   289,   196,   195,   289,   196,
     195,   178,   287,   289,   195,   289,   289,   196,   196,   196,
     196,   195,   196,   289,   195,   196,   289,   291,   196,   195,
     289,   196,   196,   289,   291,   196,   196,   195,   289,   178,
     289,   195,   289,   289,   289,   291,   195,   289,   195,   289,
     196,   196,   195,   178,   287,   196,   196,   195,   196,   291,
     195,   289,   196,   178,   291,   289,   196,   196,   196
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
#line 325 "basicParse.y"
    { labeltable[(yyvsp[(1) - (1)].number)] = byteOffset; lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 327 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 328 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 329 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 330 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 331 "basicParse.y"
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
#line 338 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 339 "basicParse.y"
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
#line 346 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 347 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 348 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 352 "basicParse.y"
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
#line 367 "basicParse.y"
    {
		// there is nothing to do with a multi line if (ifexp handles it)
	;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 373 "basicParse.y"
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
#line 397 "basicParse.y"
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
#line 412 "basicParse.y"
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
#line 428 "basicParse.y"
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
#line 446 "basicParse.y"
    { 
		 // need nothing done at top of a do
         ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 453 "basicParse.y"
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

  case 95:

/* Line 1455 of yacc.c  */
#line 532 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 533 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 534 "basicParse.y"
    { addIntOp(OP_DIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 535 "basicParse.y"
    { addIntOp(OP_DIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 538 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 539 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 540 "basicParse.y"
    { addIntOp(OP_REDIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 541 "basicParse.y"
    { addIntOp(OP_REDIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 544 "basicParse.y"
    { addOp(OP_PAUSE); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 547 "basicParse.y"
    { addOp(OP_CLS); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 548 "basicParse.y"
    { addOp(OP_CLG); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 551 "basicParse.y"
    { addOp(OP_FASTGRAPHICS); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 554 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 555 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 558 "basicParse.y"
    { addOp(OP_REFRESH); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 561 "basicParse.y"
    { addOp(OP_END); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 565 "basicParse.y"
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

  case 112:

/* Line 1455 of yacc.c  */
#line 578 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 579 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 580 "basicParse.y"
    { addInt2Op(OP_STRARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 583 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 584 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 585 "basicParse.y"
    { addInt2Op(OP_ARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 589 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 592 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 596 "basicParse.y"
    { 
	    addIntOp(OP_PUSHINT, 1); //step
	    addIntOp(OP_FOR, (yyvsp[(2) - (6)].number));
	  ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 601 "basicParse.y"
    { 
	    addIntOp(OP_FOR, (yyvsp[(2) - (8)].number));
	  ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 607 "basicParse.y"
    { addIntOp(OP_NEXT, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 610 "basicParse.y"
    { addIntOp(OP_GOTO, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 613 "basicParse.y"
    { addIntOp(OP_GOSUB, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 617 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OFFERROR); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 621 "basicParse.y"
    { addIntOp(OP_ONERROR, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 624 "basicParse.y"
    { addOp(OP_RETURN); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 627 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 628 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 629 "basicParse.y"
    { addOp(OP_SETCOLORINT); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 632 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 633 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 634 "basicParse.y"
    { addIntOp(OP_SOUND_LIST, listlen); listlen=0; ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 635 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 636 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 639 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 640 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 643 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 644 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 648 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 649 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 652 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 653 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 656 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 657 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 658 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 659 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 662 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 663 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 666 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 667 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 670 "basicParse.y"
    { addOp(OP_SYSTEM); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 673 "basicParse.y"
    { addOp(OP_VOLUME); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 676 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 677 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 678 "basicParse.y"
    { addIntOp(OP_POLY_LIST, listlen); listlen=0; ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 681 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(8) - (8)].number)); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 682 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(9) - (10)].number)); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 683 "basicParse.y"
    { addIntOp(OP_STAMP_S_LIST, listlen); listlen=0; ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 684 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(6) - (6)].number)); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 685 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(7) - (8)].number)); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 686 "basicParse.y"
    { addIntOp(OP_STAMP_LIST, listlen); listlen=0; ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 687 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(10) - (10)].number)); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 688 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(11) - (12)].number)); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 689 "basicParse.y"
    { addIntOp(OP_STAMP_SR_LIST, listlen); listlen=0; ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 692 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_OPEN); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 693 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 694 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 697 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITE); ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 698 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 699 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 702 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITELINE); ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 703 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 704 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 707 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 708 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 709 "basicParse.y"
    { addOp(OP_CLOSE); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 712 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 713 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 714 "basicParse.y"
    { addOp(OP_RESET); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 717 "basicParse.y"
    {addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP);addOp(OP_SEEK);  ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 718 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 719 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 722 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 723 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 724 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 725 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_ARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 726 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRINGASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 727 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 728 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 729 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_NUMASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 730 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 731 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 734 "basicParse.y"
    { addOp(OP_PRINT);  addOp(OP_INPUT); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 737 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_PRINTN); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 738 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 739 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 740 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 741 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 744 "basicParse.y"
    {addOp(OP_WAVPLAY);  ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 747 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 748 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 751 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 752 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 755 "basicParse.y"
    {addOp(OP_PUTSLICE);  ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 756 "basicParse.y"
    { addOp(OP_PUTSLICE); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 757 "basicParse.y"
    {addOp(OP_PUTSLICEMASK);  ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 758 "basicParse.y"
    { addOp(OP_PUTSLICEMASK); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 760 "basicParse.y"
    {addOp(OP_IMGLOAD);  ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 761 "basicParse.y"
    { addOp(OP_IMGLOAD); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 762 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 763 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 764 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 765 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 768 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEDIM); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 771 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD);  ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 772 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 775 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE);  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 776 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 779 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE);  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 780 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 783 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE);  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 784 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 787 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEHIDE); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 790 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESHOW); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 793 "basicParse.y"
    {addOp(OP_CLICKCLEAR);  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 794 "basicParse.y"
    { addOp(OP_CLICKCLEAR); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 797 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CHANGEDIR); ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 800 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DECIMAL); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 803 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPEN); ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 806 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 807 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 810 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBEXECUTE); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 813 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPENSET); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 816 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 817 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 820 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 821 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 822 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 825 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 826 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 827 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 828 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 831 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 832 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 833 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 836 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 837 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 838 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 847 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 848 "basicParse.y"
    { listlen++; ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 851 "basicParse.y"
    { (yyval.floatnum) = (yyvsp[(2) - (3)].floatnum); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 852 "basicParse.y"
    { addOp(OP_ADD); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 853 "basicParse.y"
    { addOp(OP_SUB); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 854 "basicParse.y"
    { addOp(OP_MUL); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 855 "basicParse.y"
    { addOp(OP_MOD); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 856 "basicParse.y"
    { addOp(OP_INTDIV); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 857 "basicParse.y"
    { addOp(OP_DIV); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 858 "basicParse.y"
    { addOp(OP_EXP); ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 859 "basicParse.y"
    { addOp(OP_NEGATE); ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 860 "basicParse.y"
    {addOp(OP_AND); ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 861 "basicParse.y"
    { addOp(OP_OR); ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 862 "basicParse.y"
    { addOp(OP_XOR); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 863 "basicParse.y"
    { addOp(OP_NOT); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 864 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 865 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 866 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 867 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 868 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 869 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 870 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 871 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 872 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 873 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 874 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 875 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 876 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum)); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 877 "basicParse.y"
    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 878 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 879 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 880 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 881 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 882 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 883 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 884 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 885 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 887 "basicParse.y"
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

  case 290:

/* Line 1455 of yacc.c  */
#line 897 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 898 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 899 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 900 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 901 "basicParse.y"
    { addOp(OP_LENGTH); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 902 "basicParse.y"
    { addOp(OP_ASC); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 903 "basicParse.y"
    { addOp(OP_INSTR); ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 904 "basicParse.y"
    { addOp(OP_CEIL); ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 905 "basicParse.y"
    { addOp(OP_FLOOR); ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 906 "basicParse.y"
    { addOp(OP_SIN); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 907 "basicParse.y"
    { addOp(OP_COS); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 908 "basicParse.y"
    { addOp(OP_TAN); ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 909 "basicParse.y"
    { addOp(OP_ASIN); ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 910 "basicParse.y"
    { addOp(OP_ACOS); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 911 "basicParse.y"
    { addOp(OP_ATAN); ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 912 "basicParse.y"
    { addOp(OP_DEGREES); ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 913 "basicParse.y"
    { addOp(OP_RADIANS); ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 914 "basicParse.y"
    { addOp(OP_LOG); ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 915 "basicParse.y"
    { addOp(OP_LOGTEN); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 916 "basicParse.y"
    { addOp(OP_ABS); ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 917 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 918 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 919 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 920 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 921 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 922 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 923 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 924 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 925 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 926 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 927 "basicParse.y"
    { addOp(OP_EOF); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 928 "basicParse.y"
    { addOp(OP_EXISTS); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 929 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 930 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 931 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 932 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 933 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 934 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 935 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 936 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 937 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 938 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 939 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 940 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 941 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 942 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 943 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 944 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 945 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 946 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 947 "basicParse.y"
    { addOp(OP_SIZE); ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 948 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 949 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 950 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 951 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 952 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 953 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 954 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 955 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 956 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 957 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 958 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 959 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 960 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 961 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 962 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 963 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 964 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 965 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 966 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 967 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 968 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 969 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 970 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 971 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 972 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 973 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 974 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 975 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 976 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 977 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 978 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 979 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 980 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 981 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 982 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 983 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 984 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 985 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 986 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 987 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 988 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 989 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 990 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 991 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 992 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 993 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 994 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 995 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 996 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 997 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 998 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 999 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 1000 "basicParse.y"
    { addOp(OP_PIXEL); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 1001 "basicParse.y"
    { addOp(OP_RGB); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 1002 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 1003 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 1004 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITECOLLIDE); ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 1005 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEX); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 1006 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEY); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 1007 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEH); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 1008 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEW); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 1009 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEV); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 1010 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBROW); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 1011 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBINT); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 1012 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBFLOAT); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 1013 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 1014 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 1015 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 1016 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1017 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp( OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1018 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1019 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1022 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1023 "basicParse.y"
    { listlen++; ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1026 "basicParse.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1027 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1028 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1029 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1030 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1031 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1032 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1034 "basicParse.y"
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

  case 423:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1045 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1046 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1047 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1048 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1049 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1050 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1051 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1052 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1053 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1054 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1055 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1056 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1057 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1058 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1059 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1060 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 1061 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1062 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1063 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 1064 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1065 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1066 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1067 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;



/* Line 1455 of yacc.c  */
#line 8002 "basicParse.tab.c"
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
#line 1070 "basicParse.y"


int
yyerror(const char *msg) {
  errorcode = -1;
  if (yytext[0] == '\n') { linenumber--; }	// error happened on previous line
  return -1;
}


