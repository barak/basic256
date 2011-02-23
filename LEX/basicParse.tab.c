
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
unsigned int lastLineOffset = 0; // store the byte offset for the end of the last line - use in loops
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

void
clearIfTable() {
	int j;
	for (j = 0; j < IFTABLESIZE; j++) {
		iftable[j] = -1;
	}
	numifs = 0;
}

void
clearLabelTable() {
	int j;
	for (j = 0; j < SYMTABLESIZE; j++) {
		labeltable[j] = -1;
	}
	numlabels = 0;
}

void
clearSymbolTable() {
	int j;
	if (numsyms == 0) {
		for (j = 0; j < SYMTABLESIZE; j++) {
			symtable[j] = 0;
		}
	}
	for (j = 0; j < numsyms; j++) {
		if (symtable[j]) {
			free(symtable[j]);
		}
		symtable[j] = 0;
	}
	numsyms = 0;
}

int
getSymbol(char *name) {
	int i;
	for (i = 0; i < numsyms; i++) {
		if (symtable[i] && !strcmp(name, symtable[i]))
			return i;
	}
	return -1;
}

int
newSymbol(char *name) {
	symtable[numsyms] = name;
	numsyms++;
	return numsyms - 1;
}

int
newByteCode(unsigned int size) {
	if (byteCode) {
		free(byteCode);
	}
	maxbyteoffset = 1024;
	byteCode = malloc(maxbyteoffset);

	if (byteCode) {
		memset(byteCode, 0, maxbyteoffset);
		byteOffset = 0;
		return 0;
	}
	return -1;
}

void
checkByteMem(unsigned int addedbytes) {
	if (byteOffset + addedbytes + 1 >= maxbyteoffset) {
		maxbyteoffset += maxbyteoffset + addedbytes + 32;
		byteCode = realloc(byteCode, maxbyteoffset);
		memset(byteCode + byteOffset, 0, maxbyteoffset - byteOffset);
	}
}

void
addOp(char op) {
	checkByteMem(sizeof(char));
	byteCode[byteOffset] = op;
	byteOffset++;
}

void
addExtendedOp(char extgroup, char extop) {
	addOp(extgroup);
	addOp(extop);
}


unsigned int
addInt(int data) {
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
addIntOp(char op, int data) {
	int *temp = NULL;
	checkByteMem(sizeof(char) + sizeof(int));
	byteCode[byteOffset] = op;
	byteOffset++;

	temp = (int *) (byteCode + byteOffset);
	*temp = data;
	byteOffset += sizeof(int);
}

void
addInt2Op(char op, int data1, int data2) {
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
addFloatOp(char op, double data) {
	double *temp = NULL;
	checkByteMem(sizeof(char) + sizeof(double));
	byteCode[byteOffset] = op;
	byteOffset++;

	temp = (double *) (byteCode + byteOffset);
	*temp = data;
	byteOffset += sizeof(double);
}

void
addStringOp(char op, char *data) {
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
#line 281 "basicParse.tab.c"

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
     B256INSTRX = 312,
     B256CEIL = 313,
     B256FLOOR = 314,
     B256RAND = 315,
     B256SIN = 316,
     B256COS = 317,
     B256TAN = 318,
     B256ASIN = 319,
     B256ACOS = 320,
     B256ATAN = 321,
     B256ABS = 322,
     B256PI = 323,
     B256DEGREES = 324,
     B256RADIANS = 325,
     B256LOG = 326,
     B256LOGTEN = 327,
     B256SQR = 328,
     B256EXP = 329,
     B256AND = 330,
     B256OR = 331,
     B256XOR = 332,
     B256NOT = 333,
     B256PAUSE = 334,
     B256SOUND = 335,
     B256ASC = 336,
     B256CHR = 337,
     B256TOFLOAT = 338,
     B256READLINE = 339,
     B256WRITELINE = 340,
     B256BOOLEOF = 341,
     B256MOD = 342,
     B256INTDIV = 343,
     B256YEAR = 344,
     B256MONTH = 345,
     B256DAY = 346,
     B256HOUR = 347,
     B256MINUTE = 348,
     B256SECOND = 349,
     B256TEXT = 350,
     B256FONT = 351,
     B256SAY = 352,
     B256SYSTEM = 353,
     B256VOLUME = 354,
     B256GRAPHWIDTH = 355,
     B256GRAPHHEIGHT = 356,
     B256GETSLICE = 357,
     B256PUTSLICE = 358,
     B256IMGLOAD = 359,
     B256SPRITEDIM = 360,
     B256SPRITELOAD = 361,
     B256SPRITESLICE = 362,
     B256SPRITEMOVE = 363,
     B256SPRITEHIDE = 364,
     B256SPRITESHOW = 365,
     B256SPRITEPLACE = 366,
     B256SPRITECOLLIDE = 367,
     B256SPRITEX = 368,
     B256SPRITEY = 369,
     B256SPRITEH = 370,
     B256SPRITEW = 371,
     B256SPRITEV = 372,
     B256WAVPLAY = 373,
     B256WAVSTOP = 374,
     B256WAVWAIT = 375,
     B256SIZE = 376,
     B256SEEK = 377,
     B256EXISTS = 378,
     B256BOOLTRUE = 379,
     B256BOOLFALSE = 380,
     B256MOUSEX = 381,
     B256MOUSEY = 382,
     B256MOUSEB = 383,
     B256CLICKCLEAR = 384,
     B256CLICKX = 385,
     B256CLICKY = 386,
     B256CLICKB = 387,
     B256GETCOLOR = 388,
     B256CLEAR = 389,
     B256BLACK = 390,
     B256WHITE = 391,
     B256RED = 392,
     B256DARKRED = 393,
     B256GREEN = 394,
     B256DARKGREEN = 395,
     B256BLUE = 396,
     B256DARKBLUE = 397,
     B256CYAN = 398,
     B256DARKCYAN = 399,
     B256PURPLE = 400,
     B256DARKPURPLE = 401,
     B256YELLOW = 402,
     B256DARKYELLOW = 403,
     B256ORANGE = 404,
     B256DARKORANGE = 405,
     B256GREY = 406,
     B256DARKGREY = 407,
     B256CHANGEDIR = 408,
     B256CURRENTDIR = 409,
     B256DIR = 410,
     B256DECIMAL = 411,
     B256DBOPEN = 412,
     B256DBCLOSE = 413,
     B256DBEXECUTE = 414,
     B256DBOPENSET = 415,
     B256DBCLOSESET = 416,
     B256DBROW = 417,
     B256DBINT = 418,
     B256DBFLOAT = 419,
     B256DBSTRING = 420,
     B256ONERROR = 421,
     B256OFFERROR = 422,
     B256LASTERROR = 423,
     B256LASTERRORMESSAGE = 424,
     B256LASTERRORLINE = 425,
     B256LASTERROREXTRA = 426,
     B256NETLISTEN = 427,
     B256NETCONNECT = 428,
     B256NETREAD = 429,
     B256NETWRITE = 430,
     B256NETCLOSE = 431,
     B256NETDATA = 432,
     B256NETADDRESS = 433,
     B256KILL = 434,
     B256MD5 = 435,
     B256SETSETTING = 436,
     B256GETSETTING = 437,
     B256PORTIN = 438,
     B256PORTOUT = 439,
     B256BINARYOR = 440,
     B256BINARYAND = 441,
     B256BINARYNOT = 442,
     B256IMGSAVE = 443,
     B256REPLACE = 444,
     B256COUNT = 445,
     B256EXPLODE = 446,
     B256REPLACEX = 447,
     B256COUNTX = 448,
     B256EXPLODEX = 449,
     B256IMPLODE = 450,
     B256OSTYPE = 451,
     B256MSEC = 452,
     B256LINENUM = 453,
     B256INTEGER = 454,
     B256FLOAT = 455,
     B256STRING = 456,
     B256VARIABLE = 457,
     B256STRINGVAR = 458,
     B256NEWVAR = 459,
     B256COLOR = 460,
     B256LABEL = 461,
     B256UMINUS = 462
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 265 "basicParse.y"

	int number;
	double floatnum;
	char *string;



/* Line 214 of yacc.c  */
#line 532 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 544 "basicParse.tab.c"

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
#define YYFINAL  389
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   19294

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  227
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  93
/* YYNRULES -- Number of rules.  */
#define YYNRULES  495
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1224

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   462

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     216,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     218,   220,   212,   211,   219,   210,     2,   213,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   217,   223,
     207,   209,   208,   226,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   221,     2,   222,   215,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   224,     2,   225,     2,     2,     2,     2,
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
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   214
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
     314,   325,   334,   341,   350,   354,   363,   374,   383,   387,
     391,   398,   407,   410,   413,   416,   418,   421,   423,   430,
     439,   442,   447,   450,   453,   460,   465,   470,   477,   486,
     497,   504,   513,   522,   533,   540,   549,   556,   565,   572,
     581,   584,   587,   590,   593,   596,   601,   604,   613,   624,
     633,   640,   649,   656,   667,   680,   691,   694,   701,   706,
     709,   716,   721,   724,   731,   736,   738,   742,   745,   747,
     751,   754,   757,   764,   769,   773,   780,   784,   791,   794,
     800,   808,   811,   817,   825,   828,   830,   833,   836,   840,
     844,   847,   849,   853,   855,   859,   866,   875,   884,   895,
     902,   911,   920,   931,   942,   955,   958,   963,   970,   981,
     994,  1001,  1010,  1017,  1026,  1029,  1032,  1034,  1038,  1041,
    1044,  1047,  1049,  1053,  1056,  1059,  1061,  1065,  1068,  1075,
    1080,  1085,  1092,  1099,  1108,  1111,  1118,  1123,  1125,  1129,
    1132,  1135,  1142,  1149,  1158,  1163,  1170,  1173,  1180,  1185,
    1189,  1193,  1195,  1199,  1203,  1207,  1211,  1215,  1219,  1223,
    1227,  1231,  1235,  1239,  1242,  1245,  1249,  1253,  1257,  1260,
    1264,  1268,  1272,  1276,  1280,  1284,  1288,  1292,  1296,  1300,
    1304,  1308,  1310,  1312,  1317,  1322,  1328,  1334,  1340,  1346,
    1351,  1358,  1360,  1365,  1370,  1375,  1380,  1385,  1390,  1397,
    1406,  1417,  1424,  1433,  1438,  1443,  1448,  1453,  1458,  1463,
    1468,  1473,  1478,  1483,  1488,  1493,  1498,  1503,  1508,  1510,
    1514,  1516,  1520,  1522,  1526,  1528,  1532,  1534,  1538,  1543,
    1548,  1550,  1554,  1556,  1560,  1562,  1566,  1568,  1572,  1574,
    1578,  1580,  1584,  1586,  1590,  1592,  1596,  1598,  1602,  1607,
    1609,  1613,  1615,  1619,  1621,  1625,  1627,  1631,  1633,  1637,
    1639,  1643,  1645,  1649,  1651,  1655,  1657,  1661,  1663,  1667,
    1669,  1673,  1675,  1679,  1681,  1685,  1687,  1691,  1693,  1697,
    1699,  1703,  1705,  1709,  1711,  1715,  1717,  1721,  1723,  1727,
    1729,  1733,  1735,  1739,  1741,  1745,  1747,  1751,  1753,  1757,
    1759,  1763,  1770,  1779,  1781,  1785,  1792,  1797,  1802,  1807,
    1812,  1817,  1821,  1826,  1831,  1833,  1837,  1839,  1843,  1845,
    1849,  1854,  1859,  1866,  1875,  1882,  1884,  1888,  1890,  1894,
    1896,  1900,  1904,  1908,  1912,  1916,  1918,  1923,  1930,  1932,
    1937,  1942,  1947,  1952,  1961,  1968,  1975,  1986,  1988,  1992,
    1997,  1999,  2003,  2008,  2010,  2014,  2019,  2021,  2025,  2027,
    2031,  2033,  2037,  2042,  2044,  2048,  2053,  2060,  2065,  2069,
    2078,  2089,  2098,  2103,  2110,  2115
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     228,     0,    -1,   229,   216,    -1,   229,   216,   228,    -1,
     230,   231,    -1,   231,    -1,   206,    -1,   232,    -1,   233,
      -1,   234,    -1,   236,    -1,   237,    -1,   239,    -1,   240,
      -1,   241,    -1,   242,    -1,    -1,   252,    20,   242,    -1,
     252,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     235,    -1,    23,   317,    -1,    24,    -1,    40,    23,    -1,
     238,    -1,    25,    -1,    26,   317,    -1,   243,    -1,   242,
     217,   243,    -1,   259,    -1,   260,    -1,   261,    -1,   262,
      -1,   263,    -1,   285,    -1,   266,    -1,   268,    -1,   269,
      -1,   275,    -1,   276,    -1,   267,    -1,   255,    -1,   256,
      -1,   257,    -1,   258,    -1,   264,    -1,   283,    -1,   251,
      -1,   247,    -1,   250,    -1,   248,    -1,   249,    -1,   244,
      -1,   245,    -1,   246,    -1,   254,    -1,   253,    -1,   277,
      -1,   278,    -1,   279,    -1,   280,    -1,   281,    -1,   310,
      -1,   265,    -1,   270,    -1,   271,    -1,   272,    -1,   273,
      -1,   274,    -1,   286,    -1,   287,    -1,   288,    -1,   289,
      -1,   290,    -1,   291,    -1,   292,    -1,   293,    -1,   294,
      -1,   295,    -1,   296,    -1,   297,    -1,   282,    -1,   298,
      -1,   299,    -1,   300,    -1,   301,    -1,   302,    -1,   303,
      -1,   304,    -1,   305,    -1,   306,    -1,   307,    -1,   308,
      -1,   309,    -1,   311,    -1,   312,    -1,   313,    -1,    45,
     202,   317,    -1,    45,   203,   317,    -1,    45,   202,   218,
     317,   219,   317,   220,    -1,    45,   203,   218,   317,   219,
     317,   220,    -1,    46,   202,   317,    -1,    46,   203,   317,
      -1,    46,   202,   218,   317,   219,   317,   220,    -1,    46,
     203,   218,   317,   219,   317,   220,    -1,    79,   317,    -1,
      17,    -1,    18,    -1,    14,    -1,    15,   317,   219,   317,
      -1,    15,   218,   317,   219,   317,   220,    -1,    16,    -1,
      40,    -1,    19,   317,    -1,   203,   221,   317,   222,   209,
     319,    -1,   203,   221,   317,   219,   317,   222,   209,   319,
      -1,   203,   209,   314,    -1,   203,   209,   191,   218,   319,
     219,   319,   220,    -1,   203,   209,   191,   218,   319,   219,
     319,   219,   317,   220,    -1,   203,   209,   194,   218,   319,
     219,   319,   220,    -1,   202,   221,   317,   222,   209,   317,
      -1,   202,   221,   317,   219,   317,   222,   209,   317,    -1,
     202,   209,   315,    -1,   202,   209,   191,   218,   319,   219,
     319,   220,    -1,   202,   209,   191,   218,   319,   219,   319,
     219,   317,   220,    -1,   202,   209,   194,   218,   319,   219,
     319,   220,    -1,   202,   209,   317,    -1,   203,   209,   319,
      -1,    27,   202,   209,   317,    28,   317,    -1,    27,   202,
     209,   317,    28,   317,    29,   317,    -1,    30,   202,    -1,
      36,   202,    -1,    37,   202,    -1,   167,    -1,   166,   202,
      -1,    38,    -1,    41,   317,   219,   317,   219,   317,    -1,
      41,   218,   317,   219,   317,   219,   317,   220,    -1,    41,
     317,    -1,    80,   218,   202,   220,    -1,    80,   202,    -1,
      80,   315,    -1,    80,   218,   317,   219,   317,   220,    -1,
      80,   317,   219,   317,    -1,     8,   317,   219,   317,    -1,
       8,   218,   317,   219,   317,   220,    -1,    13,   317,   219,
     317,   219,   317,   219,   317,    -1,    13,   218,   317,   219,
     317,   219,   317,   219,   317,   220,    -1,     9,   317,   219,
     317,   219,   317,    -1,     9,   218,   317,   219,   317,   219,
     317,   220,    -1,    10,   317,   219,   317,   219,   317,   219,
     317,    -1,    10,   218,   317,   219,   317,   219,   317,   219,
     317,   220,    -1,    95,   317,   219,   317,   219,   319,    -1,
      95,   218,   317,   219,   317,   219,   319,   220,    -1,    95,
     317,   219,   317,   219,   317,    -1,    95,   218,   317,   219,
     317,   219,   317,   220,    -1,    96,   319,   219,   317,   219,
     317,    -1,    96,   218,   319,   219,   317,   219,   317,   220,
      -1,    97,   319,    -1,    97,   317,    -1,    98,   319,    -1,
      99,   317,    -1,    11,   202,    -1,    11,   218,   202,   220,
      -1,    11,   315,    -1,    12,   317,   219,   317,   219,   317,
     219,   202,    -1,    12,   218,   317,   219,   317,   219,   317,
     219,   202,   220,    -1,    12,   317,   219,   317,   219,   317,
     219,   315,    -1,    12,   317,   219,   317,   219,   202,    -1,
      12,   218,   317,   219,   317,   219,   202,   220,    -1,    12,
     317,   219,   317,   219,   315,    -1,    12,   317,   219,   317,
     219,   317,   219,   317,   219,   202,    -1,    12,   218,   317,
     219,   317,   219,   317,   219,   317,   219,   202,   220,    -1,
      12,   317,   219,   317,   219,   317,   219,   317,   219,   315,
      -1,    31,   319,    -1,    31,   218,   317,   219,   319,   220,
      -1,    31,   317,   219,   319,    -1,    33,   319,    -1,    33,
     218,   317,   219,   319,   220,    -1,    33,   317,   219,   319,
      -1,    85,   319,    -1,    85,   218,   317,   219,   319,   220,
      -1,    85,   317,   219,   319,    -1,    34,    -1,    34,   218,
     220,    -1,    34,   317,    -1,    35,    -1,    35,   218,   220,
      -1,    35,   317,    -1,   122,   317,    -1,   122,   218,   317,
     219,   317,   220,    -1,   122,   317,   219,   317,    -1,   284,
     219,   203,    -1,   284,   219,   203,   221,   317,   222,    -1,
     284,   219,   202,    -1,   284,   219,   202,   221,   317,   222,
      -1,     4,   203,    -1,     4,   203,   221,   317,   222,    -1,
       4,   203,   221,   317,   219,   317,   222,    -1,     4,   202,
      -1,     4,   202,   221,   317,   222,    -1,     4,   202,   221,
     317,   219,   317,   222,    -1,     4,   319,    -1,     3,    -1,
       3,   319,    -1,     3,   317,    -1,     3,   319,   223,    -1,
       3,   317,   223,    -1,   118,   319,    -1,   119,    -1,   119,
     218,   220,    -1,   120,    -1,   120,   218,   220,    -1,   103,
     317,   219,   317,   219,   319,    -1,   103,   218,   317,   219,
     317,   219,   319,   220,    -1,   103,   317,   219,   317,   219,
     319,   219,   317,    -1,   103,   218,   317,   219,   317,   219,
     319,   219,   317,   220,    -1,   104,   317,   219,   317,   219,
     319,    -1,   104,   218,   317,   219,   317,   219,   319,   220,
      -1,   104,   317,   219,   317,   219,   317,   219,   319,    -1,
     104,   218,   317,   219,   317,   219,   317,   219,   319,   220,
      -1,   104,   317,   219,   317,   219,   317,   219,   317,   219,
     319,    -1,   104,   218,   317,   219,   317,   219,   317,   219,
     317,   219,   319,   220,    -1,   105,   317,    -1,   106,   317,
     219,   319,    -1,   106,   218,   317,   219,   319,   220,    -1,
     107,   317,   219,   317,   219,   317,   219,   317,   219,   317,
      -1,   107,   218,   317,   219,   317,   219,   317,   219,   317,
     219,   317,   220,    -1,   111,   317,   219,   317,   219,   317,
      -1,   111,   218,   317,   219,   317,   219,   317,   220,    -1,
     108,   317,   219,   317,   219,   317,    -1,   106,   218,   317,
     219,   317,   219,   317,   220,    -1,   109,   317,    -1,   110,
     317,    -1,   129,    -1,   129,   218,   220,    -1,   153,   319,
      -1,   156,   317,    -1,   157,   319,    -1,   158,    -1,   158,
     218,   220,    -1,   159,   319,    -1,   160,   319,    -1,   161,
      -1,   161,   218,   220,    -1,   172,   317,    -1,   172,   218,
     317,   219,   317,   220,    -1,   172,   317,   219,   317,    -1,
     173,   319,   219,   317,    -1,   173,   218,   319,   219,   317,
     220,    -1,   173,   317,   219,   319,   219,   317,    -1,   173,
     218,   317,   219,   319,   219,   317,   220,    -1,   175,   319,
      -1,   175,   218,   317,   219,   319,   220,    -1,   175,   317,
     219,   319,    -1,   176,    -1,   176,   218,   220,    -1,   176,
     317,    -1,   179,   319,    -1,   179,   218,   317,   219,   319,
     220,    -1,   181,   319,   219,   319,   219,   319,    -1,   181,
     218,   319,   219,   319,   219,   319,   220,    -1,   184,   317,
     219,   317,    -1,   184,   218,   317,   219,   317,   220,    -1,
     188,   319,    -1,   188,   218,   319,   219,   319,   220,    -1,
     188,   319,   219,   319,    -1,   224,   318,   225,    -1,   224,
     316,   225,    -1,   317,    -1,   317,   219,   316,    -1,   218,
     317,   220,    -1,   317,   211,   317,    -1,   317,   210,   317,
      -1,   317,   212,   317,    -1,   317,    87,   317,    -1,   317,
      88,   317,    -1,   317,   213,   317,    -1,   317,   215,   317,
      -1,   317,   185,   317,    -1,   317,   186,   317,    -1,   187,
     317,    -1,   210,   317,    -1,   317,    75,   317,    -1,   317,
      76,   317,    -1,   317,    77,   317,    -1,    78,   317,    -1,
     319,   209,   319,    -1,   319,    44,   319,    -1,   319,   207,
     319,    -1,   319,   208,   319,    -1,   319,    42,   319,    -1,
     319,    43,   319,    -1,   317,   209,   317,    -1,   317,    44,
     317,    -1,   317,   207,   317,    -1,   317,   208,   317,    -1,
     317,    42,   317,    -1,   317,    43,   317,    -1,   200,    -1,
     199,    -1,   202,   221,   226,   222,    -1,   203,   221,   226,
     222,    -1,   202,   221,   226,   219,   222,    -1,   203,   221,
     226,   219,   222,    -1,   202,   221,   219,   226,   222,    -1,
     203,   221,   219,   226,   222,    -1,   202,   221,   317,   222,
      -1,   202,   221,   317,   219,   317,   222,    -1,   202,    -1,
      48,   218,   317,   220,    -1,    48,   218,   319,   220,    -1,
      83,   218,   317,   220,    -1,    83,   218,   319,   220,    -1,
      50,   218,   319,   220,    -1,    81,   218,   319,   220,    -1,
      56,   218,   319,   219,   319,   220,    -1,    56,   218,   319,
     219,   319,   219,   317,   220,    -1,    56,   218,   319,   219,
     319,   219,   317,   219,   317,   220,    -1,    57,   218,   319,
     219,   319,   220,    -1,    57,   218,   319,   219,   319,   219,
     317,   220,    -1,    58,   218,   317,   220,    -1,    59,   218,
     317,   220,    -1,    61,   218,   317,   220,    -1,    62,   218,
     317,   220,    -1,    63,   218,   317,   220,    -1,    64,   218,
     317,   220,    -1,    65,   218,   317,   220,    -1,    66,   218,
     317,   220,    -1,    69,   218,   317,   220,    -1,    70,   218,
     317,   220,    -1,    71,   218,   317,   220,    -1,    72,   218,
     317,   220,    -1,    73,   218,   317,   220,    -1,    74,   218,
     317,   220,    -1,    67,   218,   317,   220,    -1,    60,    -1,
      60,   218,   220,    -1,    68,    -1,    68,   218,   220,    -1,
     124,    -1,   124,   218,   220,    -1,   125,    -1,   125,   218,
     220,    -1,    86,    -1,    86,   218,   220,    -1,    86,   218,
     317,   220,    -1,   123,   218,   319,   220,    -1,    89,    -1,
      89,   218,   220,    -1,    90,    -1,    90,   218,   220,    -1,
      91,    -1,    91,   218,   220,    -1,    92,    -1,    92,   218,
     220,    -1,    93,    -1,    93,   218,   220,    -1,    94,    -1,
      94,   218,   220,    -1,   100,    -1,   100,   218,   220,    -1,
     101,    -1,   101,   218,   220,    -1,   121,    -1,   121,   218,
     220,    -1,   121,   218,   317,   220,    -1,     5,    -1,     5,
     218,   220,    -1,   126,    -1,   126,   218,   220,    -1,   127,
      -1,   127,   218,   220,    -1,   128,    -1,   128,   218,   220,
      -1,   130,    -1,   130,   218,   220,    -1,   131,    -1,   131,
     218,   220,    -1,   132,    -1,   132,   218,   220,    -1,   134,
      -1,   134,   218,   220,    -1,   135,    -1,   135,   218,   220,
      -1,   136,    -1,   136,   218,   220,    -1,   137,    -1,   137,
     218,   220,    -1,   138,    -1,   138,   218,   220,    -1,   139,
      -1,   139,   218,   220,    -1,   140,    -1,   140,   218,   220,
      -1,   141,    -1,   141,   218,   220,    -1,   142,    -1,   142,
     218,   220,    -1,   143,    -1,   143,   218,   220,    -1,   144,
      -1,   144,   218,   220,    -1,   145,    -1,   145,   218,   220,
      -1,   146,    -1,   146,   218,   220,    -1,   147,    -1,   147,
     218,   220,    -1,   148,    -1,   148,   218,   220,    -1,   149,
      -1,   149,   218,   220,    -1,   150,    -1,   150,   218,   220,
      -1,   151,    -1,   151,   218,   220,    -1,   152,    -1,   152,
     218,   220,    -1,     6,   218,   317,   219,   317,   220,    -1,
       7,   218,   317,   219,   317,   219,   317,   220,    -1,   133,
      -1,   133,   218,   220,    -1,   112,   218,   317,   219,   317,
     220,    -1,   113,   218,   317,   220,    -1,   114,   218,   317,
     220,    -1,   115,   218,   317,   220,    -1,   116,   218,   317,
     220,    -1,   117,   218,   317,   220,    -1,   162,   218,   220,
      -1,   163,   218,   317,   220,    -1,   164,   218,   317,   220,
      -1,   168,    -1,   168,   218,   220,    -1,   170,    -1,   170,
     218,   220,    -1,   177,    -1,   177,   218,   220,    -1,   177,
     218,   317,   220,    -1,   183,   218,   317,   220,    -1,   190,
     218,   319,   219,   319,   220,    -1,   190,   218,   319,   219,
     319,   219,   317,   220,    -1,   193,   218,   319,   219,   319,
     220,    -1,   196,    -1,   196,   218,   220,    -1,   197,    -1,
     197,   218,   220,    -1,   319,    -1,   319,   219,   318,    -1,
     218,   319,   220,    -1,   319,   211,   319,    -1,   317,   211,
     319,    -1,   319,   211,   317,    -1,   201,    -1,   203,   221,
     317,   222,    -1,   203,   221,   317,   219,   317,   222,    -1,
     203,    -1,    82,   218,   317,   220,    -1,    49,   218,   317,
     220,    -1,    54,   218,   319,   220,    -1,    55,   218,   319,
     220,    -1,    51,   218,   319,   219,   317,   219,   317,   220,
      -1,    52,   218,   319,   219,   317,   220,    -1,    53,   218,
     319,   219,   317,   220,    -1,   102,   218,   317,   219,   317,
     219,   317,   219,   317,   220,    -1,    32,    -1,    32,   218,
     220,    -1,    32,   218,   317,   220,    -1,    84,    -1,    84,
     218,   220,    -1,    84,   218,   317,   220,    -1,   154,    -1,
     154,   218,   220,    -1,   165,   218,   317,   220,    -1,   169,
      -1,   169,   218,   220,    -1,   171,    -1,   171,   218,   220,
      -1,   174,    -1,   174,   218,   220,    -1,   174,   218,   317,
     220,    -1,   178,    -1,   178,   218,   220,    -1,   180,   218,
     319,   220,    -1,   182,   218,   319,   219,   319,   220,    -1,
     155,   218,   319,   220,    -1,   155,   218,   220,    -1,   189,
     218,   319,   219,   319,   219,   319,   220,    -1,   189,   218,
     319,   219,   319,   219,   319,   219,   317,   220,    -1,   192,
     218,   319,   219,   319,   219,   319,   220,    -1,   195,   218,
     203,   220,    -1,   195,   218,   203,   219,   319,   220,    -1,
     195,   218,   202,   220,    -1,   195,   218,   202,   219,   319,
     220,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   297,   297,   298,   301,   302,   305,   307,   308,   309,
     310,   311,   318,   319,   326,   327,   328,   331,   345,   351,
     371,   372,   374,   388,   401,   402,   404,   421,   427,   437,
     437,   440,   441,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   468,
     469,   470,   471,   472,   473,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   510,
     511,   512,   513,   516,   517,   518,   519,   522,   525,   526,
     529,   532,   533,   536,   539,   542,   556,   557,   558,   559,
     560,   561,   564,   565,   566,   567,   568,   569,   573,   576,
     579,   584,   590,   593,   596,   599,   602,   605,   608,   609,
     610,   613,   614,   615,   616,   617,   620,   621,   624,   625,
     629,   630,   633,   634,   637,   638,   639,   640,   643,   644,
     647,   648,   651,   654,   657,   658,   659,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   673,   674,   675,   678,
     679,   680,   683,   684,   685,   688,   689,   690,   693,   694,
     695,   698,   699,   700,   703,   704,   705,   706,   707,   708,
     709,   710,   711,   712,   715,   718,   719,   720,   721,   722,
     725,   728,   729,   732,   733,   736,   737,   738,   739,   741,
     742,   743,   744,   745,   746,   749,   752,   753,   756,   757,
     760,   761,   764,   765,   768,   771,   774,   775,   778,   781,
     784,   787,   788,   791,   794,   797,   798,   801,   802,   803,
     806,   807,   808,   809,   812,   813,   814,   817,   818,   819,
     822,   823,   826,   827,   830,   831,   834,   835,   836,   840,
     843,   846,   847,   850,   851,   852,   853,   854,   855,   856,
     857,   858,   859,   860,   861,   862,   863,   864,   865,   866,
     867,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   896,   897,   898,   899,   900,   901,   902,   903,
     904,   905,   906,   907,   908,   909,   910,   911,   912,   913,
     914,   915,   916,   917,   918,   919,   920,   921,   922,   923,
     924,   925,   926,   927,   928,   929,   930,   931,   932,   933,
     934,   935,   936,   937,   938,   939,   940,   941,   942,   943,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   955,   956,   957,   958,   959,   960,   961,   962,   963,
     964,   965,   966,   967,   968,   969,   970,   971,   972,   973,
     974,   975,   976,   977,   978,   979,   980,   981,   982,   983,
     984,   985,   986,   987,   988,   989,   990,   991,   992,   993,
     994,   995,   996,   997,   998,   999,  1000,  1001,  1002,  1003,
    1004,  1005,  1006,  1007,  1008,  1009,  1010,  1011,  1012,  1013,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,  1023,
    1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,  1035,
    1036,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090
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
  "B256LOWER", "B256INSTR", "B256INSTRX", "B256CEIL", "B256FLOOR",
  "B256RAND", "B256SIN", "B256COS", "B256TAN", "B256ASIN", "B256ACOS",
  "B256ATAN", "B256ABS", "B256PI", "B256DEGREES", "B256RADIANS", "B256LOG",
  "B256LOGTEN", "B256SQR", "B256EXP", "B256AND", "B256OR", "B256XOR",
  "B256NOT", "B256PAUSE", "B256SOUND", "B256ASC", "B256CHR", "B256TOFLOAT",
  "B256READLINE", "B256WRITELINE", "B256BOOLEOF", "B256MOD", "B256INTDIV",
  "B256YEAR", "B256MONTH", "B256DAY", "B256HOUR", "B256MINUTE",
  "B256SECOND", "B256TEXT", "B256FONT", "B256SAY", "B256SYSTEM",
  "B256VOLUME", "B256GRAPHWIDTH", "B256GRAPHHEIGHT", "B256GETSLICE",
  "B256PUTSLICE", "B256IMGLOAD", "B256SPRITEDIM", "B256SPRITELOAD",
  "B256SPRITESLICE", "B256SPRITEMOVE", "B256SPRITEHIDE", "B256SPRITESHOW",
  "B256SPRITEPLACE", "B256SPRITECOLLIDE", "B256SPRITEX", "B256SPRITEY",
  "B256SPRITEH", "B256SPRITEW", "B256SPRITEV", "B256WAVPLAY",
  "B256WAVSTOP", "B256WAVWAIT", "B256SIZE", "B256SEEK", "B256EXISTS",
  "B256BOOLTRUE", "B256BOOLFALSE", "B256MOUSEX", "B256MOUSEY",
  "B256MOUSEB", "B256CLICKCLEAR", "B256CLICKX", "B256CLICKY", "B256CLICKB",
  "B256GETCOLOR", "B256CLEAR", "B256BLACK", "B256WHITE", "B256RED",
  "B256DARKRED", "B256GREEN", "B256DARKGREEN", "B256BLUE", "B256DARKBLUE",
  "B256CYAN", "B256DARKCYAN", "B256PURPLE", "B256DARKPURPLE", "B256YELLOW",
  "B256DARKYELLOW", "B256ORANGE", "B256DARKORANGE", "B256GREY",
  "B256DARKGREY", "B256CHANGEDIR", "B256CURRENTDIR", "B256DIR",
  "B256DECIMAL", "B256DBOPEN", "B256DBCLOSE", "B256DBEXECUTE",
  "B256DBOPENSET", "B256DBCLOSESET", "B256DBROW", "B256DBINT",
  "B256DBFLOAT", "B256DBSTRING", "B256ONERROR", "B256OFFERROR",
  "B256LASTERROR", "B256LASTERRORMESSAGE", "B256LASTERRORLINE",
  "B256LASTERROREXTRA", "B256NETLISTEN", "B256NETCONNECT", "B256NETREAD",
  "B256NETWRITE", "B256NETCLOSE", "B256NETDATA", "B256NETADDRESS",
  "B256KILL", "B256MD5", "B256SETSETTING", "B256GETSETTING", "B256PORTIN",
  "B256PORTOUT", "B256BINARYOR", "B256BINARYAND", "B256BINARYNOT",
  "B256IMGSAVE", "B256REPLACE", "B256COUNT", "B256EXPLODE", "B256REPLACEX",
  "B256COUNTX", "B256EXPLODEX", "B256IMPLODE", "B256OSTYPE", "B256MSEC",
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
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,    60,    62,    61,
      45,    43,    42,    47,   462,    94,    10,    58,    40,    44,
      41,    91,    93,    59,   123,   125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   227,   228,   228,   229,   229,   230,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   232,   233,   234,
     235,   235,   236,   237,   238,   238,   239,   240,   241,   242,
     242,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   244,
     244,   244,   244,   245,   245,   245,   245,   246,   247,   247,
     248,   249,   249,   250,   251,   252,   253,   253,   253,   253,
     253,   253,   254,   254,   254,   254,   254,   254,   255,   256,
     257,   257,   258,   259,   260,   261,   262,   263,   264,   264,
     264,   265,   265,   265,   265,   265,   266,   266,   267,   267,
     268,   268,   269,   269,   270,   270,   270,   270,   271,   271,
     272,   272,   273,   274,   275,   275,   275,   276,   276,   276,
     276,   276,   276,   276,   276,   276,   277,   277,   277,   278,
     278,   278,   279,   279,   279,   280,   280,   280,   281,   281,
     281,   282,   282,   282,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   284,   285,   285,   285,   285,   285,
     286,   287,   287,   288,   288,   289,   289,   289,   289,   290,
     290,   290,   290,   290,   290,   291,   292,   292,   293,   293,
     294,   294,   295,   295,   296,   297,   298,   298,   299,   300,
     301,   302,   302,   303,   304,   305,   305,   306,   306,   306,
     307,   307,   307,   307,   308,   308,   308,   309,   309,   309,
     310,   310,   311,   311,   312,   312,   313,   313,   313,   314,
     315,   316,   316,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   318,
     318,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319
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
       1,     4,     6,     1,     1,     2,     6,     8,     3,     8,
      10,     8,     6,     8,     3,     8,    10,     8,     3,     3,
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
       2,     6,     6,     8,     4,     6,     2,     6,     4,     3,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     4,     4,     5,     5,     5,     5,     4,
       6,     1,     4,     4,     4,     4,     4,     4,     6,     8,
      10,     6,     8,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     4,     4,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     4,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     6,     8,     1,     3,     6,     4,     4,     4,     4,
       4,     3,     4,     4,     1,     3,     1,     3,     1,     3,
       4,     4,     6,     8,     6,     1,     3,     1,     3,     1,
       3,     3,     3,     3,     3,     1,     4,     6,     1,     4,
       4,     4,     4,     8,     6,     6,    10,     1,     3,     4,
       1,     3,     4,     1,     3,     4,     1,     3,     1,     3,
       1,     3,     4,     1,     3,     4,     6,     4,     3,     8,
      10,     8,     4,     6,     4,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      16,   205,     0,     0,     0,     0,     0,     0,     0,   110,
       0,   113,   108,   109,     0,    19,    20,     0,    24,    27,
       0,     0,     0,     0,     0,   185,   188,     0,     0,   137,
     114,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   211,   213,     0,   236,     0,     0,     0,   241,
       0,     0,   245,     0,   135,     0,     0,     0,   257,     0,
       0,     0,     0,     0,     0,     6,     0,     0,    16,     5,
       7,     8,     9,    22,    10,    11,    26,    12,    13,    14,
      15,    29,    54,    55,    56,    50,    52,    53,    51,    49,
       0,    58,    57,    43,    44,    45,    46,    31,    32,    33,
      34,    35,    47,    65,    37,    42,    38,    39,    66,    67,
      68,    69,    70,    40,    41,    59,    60,    61,    62,    63,
      83,    48,     0,    36,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    64,    96,
      97,    98,   369,     0,     0,   467,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   470,   346,   350,
     352,   354,   356,   358,   360,   362,   364,     0,     0,     0,
       0,     0,     0,     0,   366,     0,   342,   344,   371,   373,
     375,   377,   379,   381,   423,   383,   385,   387,   389,   391,
     393,   395,   397,   399,   401,   403,   405,   407,   409,   411,
     413,   415,   417,   419,   473,     0,     0,     0,     0,     0,
     434,   476,   436,   478,   480,   438,   483,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,   447,   302,   301,
     455,   311,   458,     0,     0,   207,   206,   311,   458,     0,
     204,     0,     0,     0,     0,     0,     0,     0,   164,     0,
       0,   166,     0,     0,     0,     0,     0,     0,   115,    23,
      28,     0,   132,     0,     0,   176,     0,     0,   179,     0,
     187,     0,   190,   133,   134,    21,    25,     0,   140,     0,
       0,     0,     0,   107,   311,     0,   143,     0,     0,     0,
     182,     0,     0,     0,     0,   161,   160,   162,   163,     0,
       0,     0,     0,   225,     0,     0,     0,     0,     0,   234,
     235,     0,     0,   210,     0,     0,     0,   191,     0,   238,
     239,   240,     0,   243,   244,     0,   136,     0,   247,     0,
       0,     0,     0,     0,   254,     0,   259,     0,   260,     0,
       0,     0,     0,     0,   266,     0,     0,     0,     0,     1,
       2,     4,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   283,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   284,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209,     0,     0,     0,     0,     0,
       0,     0,   208,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   271,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   186,   189,     0,     0,     0,
      99,     0,   100,     0,   103,     0,   104,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,   214,     0,
       0,   237,   242,   246,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   124,   128,     0,     0,     0,     0,   118,   129,
       0,     3,   114,    30,    17,   196,   194,   370,     0,     0,
     468,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,   341,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   471,     0,   347,     0,   351,   353,
     355,   357,   359,   361,   363,   365,     0,     0,     0,     0,
       0,     0,     0,   367,     0,     0,   343,   345,   372,   374,
     376,   378,   380,   382,   424,   384,   386,   388,   390,   392,
     394,   396,   398,   400,   402,   404,   406,   408,   410,   412,
     414,   416,   418,   420,   474,   488,     0,   431,     0,     0,
       0,   435,   477,   437,   479,   481,     0,   439,     0,   484,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   446,
     448,     0,     0,     0,     0,     0,     0,   273,   451,   299,
     300,   296,   285,   286,   287,   277,   278,   281,   282,   297,
     298,   295,   275,   274,   453,   276,   279,   280,   293,   294,
     290,   291,   292,   289,   454,   452,     0,     0,     0,   146,
       0,     0,     0,     0,   165,   270,     0,     0,     0,     0,
       0,     0,   111,     0,     0,   178,     0,   181,     0,     0,
       0,     0,     0,     0,   141,     0,   145,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   226,     0,
       0,     0,     0,     0,     0,   193,     0,   249,     0,     0,
       0,   250,     0,   256,     0,     0,     0,     0,   264,     0,
     268,     0,     0,     0,     0,     0,     0,     0,   449,     0,
       0,     0,     0,     0,     0,   469,   312,   313,   460,   316,
       0,     0,     0,   461,   462,     0,     0,   323,   324,   325,
     326,   327,   328,   329,   330,   337,   331,   332,   333,   334,
     335,   336,   317,   459,   314,   315,   472,   348,     0,     0,
     426,   427,   428,   429,   430,   368,   349,   487,   432,   433,
     475,   482,   440,   485,     0,   441,     0,     0,     0,     0,
       0,   494,     0,   492,     0,     0,   303,     0,   309,     0,
       0,   304,     0,   456,     0,   309,     0,   456,     0,     0,
       0,     0,     0,   272,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   269,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   307,   305,     0,   308,   306,     0,
       0,     0,   147,     0,   150,     0,     0,     0,   311,   172,
       0,     0,     0,   112,   130,   177,   180,     0,   138,     0,
       0,     0,     0,   144,   183,     0,   156,   154,     0,   158,
       0,   215,     0,     0,   219,     0,   227,     0,     0,   232,
       0,   230,   192,   248,     0,   251,   252,   255,   261,     0,
     262,   265,   267,     0,     0,     0,   122,     0,     0,   450,
       0,   116,   197,   195,   421,     0,     0,   464,   465,     0,
     318,     0,   321,     0,   425,   486,     0,     0,   442,     0,
     444,   495,   493,   310,   457,   310,   457,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,   101,   102,   105,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   151,
       0,   152,   171,     0,   311,   169,     0,     0,   148,   131,
     139,   157,   155,   159,     0,   216,   217,     0,   220,     0,
     221,   233,     0,     0,   231,   253,   263,     0,   125,   127,
     123,     0,   119,   121,   117,   422,   463,     0,   319,   322,
       0,     0,   489,   443,   491,     0,   311,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   153,   168,     0,   173,   175,   149,   218,     0,   222,
     223,     0,   228,   126,   120,   320,   466,   490,     0,     0,
       0,   174,   224,   229
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
     155,   156,   157,   158,   159,   160,   161,   618,   291,   542,
     279,   847,   283
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -441
static const yytype_int16 yypact[] =
{
   11433,  4778,  4957,  5136,  5315,  5494,  -197,  5673,  5852,  -441,
    6031,  -441,  -441,  -441,  4778,  -441,  -441,  4778,  -441,  -441,
    4778,  -176,  -171,  6210,  6389,  6568,  6747,  -165,  -163,  -441,
      74,  6926,  -191,  -185,  4778,  2451,  7105,  7284,  7463,  4778,
    4778,  4778,  7642,  7821,  4778,  8000,  8179,  4778,  4778,  4778,
    8358,  4778,  -155,  -146,  8537,  -133,  4778,  4778,  4778,  -124,
    4778,  4778,   -98,   -84,  -441,  8716,  8895,  9074,  9253,  9432,
    9611,  9790,  9969,  -201,  -150,  -441,   126,   -88, 11634,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
     -87,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
     116,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,   -78,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,   -69,   -63,   -37,   -32,   -25,   -16,   -15,   -14,
     -13,   -12,   -11,   -10,    -9,    -8,    -7,    -6,    -5,    -4,
       2,    11,    12,    15,    16,    17,    18,    21,    24,    25,
      27,    29,    30,  4778,    32,    33,    34,    36,    37,    38,
      39,    40,    42,    43,    44,    45,    46,    47,    48,    51,
      52,    56,    57,    58,    59,    60,    61,    64,    66,    72,
      76,    81,    83,    85,    86,    88,    91,    93,    95,    96,
      97,    99,   104,   105,   107,   108,   110,   111,   113,   115,
     117,   118,   119,   121,   123,   124,   128,   129,   130,   131,
     134,   135,   136,   138,   139,   140,   141,   145,   147,   149,
    4778,   151,   152,   157,   159,   161,   163,   165,  -441,  -441,
    -441,   -54,    71,  4778,  4778,  1664,    89,  -159,  -102, 19063,
    1208,  4778, 16387,  1208,  4778, 16423,  4778, 16499,  -441,   -43,
    4778,  -441,  4778, 16535,  4778, 16571,  4778, 16585, 19063, 19063,
   19063,    98,  -441,  4778, 16647,  1208,  4778, 16685,  1208,  2988,
   19063,  3167, 19063,  -441,  -441,  -441,  -441,  4778, 16732, 10148,
   10327, 10506, 10685, 19063,  -100, 10864,  -441, 16792,  4778, 16832,
    1208,  4778, 16846,  4778,   362, 19063,  1208,  1208, 19063,  4778,
   16882,  4778, 16921, 19063,  4778, 16937,  4778, 16994, 17033, 19063,
   19063,  4778, 17082,  1208,   114,   166,  4778, 17099,   168,  1208,
   19063,  1208,   169,  1208,  1208,   170,  -441,  4778, 17139,  4778,
   17178,   557,  4778, 17194,  1208,  3346, 19063,  4778,  1208,  4778,
     622,  4778, 17244,  4778,   862,   560,  4778,  2272,  4778,  -441,
      65,  -441, 11816, 11816,  -142,   171,  4778,  4778,  3525,  4778,
    4778,  4778,  4778,  4778,  4778,  4778,  4778,  4778,  4778,  4778,
    4778,   172,  4778,  4778,  4778,  4778,  4778,  4778,  4778,   173,
    4778,  4778,  4778,  4778,  4778,  4778,   179,  4778,  4778,  4778,
    3704,  3883,   180,   181,   182,   183,   187,   190,   198,   205,
    4778,  4778,  4778,  4778,  4778,  4778,  4778,  4062,  4778,   206,
     211,   212,   213,   214,   215,   216,   217,   218,   227,   228,
     229,   230,   234,   236,   237,   238,   239,   240,   241,   242,
     245,   246,   247,   248,   249,   253,   254,   255,  4241,   256,
    4778,  4778,  4778,   257,   258,   262,   263,  4420,  4599,   264,
    4778,  4778,  4778,   179,  4778,  4778,  4778,  4778,   -68,   265,
     268,  1502,  2093,   179, 13467,   702,  4778,  4778,  4778,  4778,
    4778,  4778,  4778,  4778,  4778,  4778,  4778,  4778,  4778,  4778,
    4778,  4778,  4778,  4778,  -441,  4778,  4778,  4778,  4778,  4778,
    4778,  4778,  -441,  1502,  2093, 12287,  4778, 12301,  4778, 12351,
    4778,   269,   267, 17284, 12387,  4778, 12401,  4778, 12480,  4778,
    4778, 12547,  4778, 12561,  4778,  -441,  -441, 12597,  4778,  4778,
   19063,  4778, 19063,  4778, 19063,  4778, 19063,  -112, 12633,  4778,
   12709,  4778, 12745,  4778,     8,  4778, 12781,  4778, 12795,  4778,
   12857,  4778, 12895,  4778,  4778, 12942,  4778,  -441,  -441, 13007,
    4778,  -441,  -441,  -441, 13043,  4778, 13057,   153,  4778,  4778,
   13093,  4778,  -441, 13157,   244,  4778, 13205,  4778,   428,  4778,
     272,   275,  -441, 19063,  1711,   276,   277,  4778,  -441,  1208,
    1769,  -441,  -441,  -441,   -87,   278,   302,  -441, 17323, 17339,
    -441, 13503, 13539,   847, 13553,   951,  1330,  1778,  1868,  1139,
    1145,  2449,  2628, 13615, 13653,  -441, 13700, 13765, 13801, 13815,
   13851, 13915, 13963,  -441, 13999, 14063, 14077, 14113, 14149, 14225,
    1187, 14261, 14297,  1203,  -441, 14311,  -441, 14373,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441, 17389, 17429, 14411, 14458,
   14523, 14559, 14573,  -441, 14609,  1225,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  1280,  -441, 14673, 14721,
   14757,  -441,  -441,  -441,  -441,  -441, 14821,  -441, 14835,  -441,
    1400,  2807, 14871,  2986,  3950,  4129,  4308,   -72,   -62,  -441,
    -441,   270,  -115,  1818,   271,   -76,  1859,  -441,  -441,   -73,
     -73,   -73, 11477, 12848, 19079,   179,   179,   -59,   -59,   -73,
     -73,   -73,   -33,   -33,  -441,   179,   179,  -441,   133,   133,
     133,   133,   133,   133,   -33,  -441, 11659, 11841,  4778, 19063,
    4778, 17468,  4778, 17484,  -441,  -441,  4778,  4778, 17534,  4778,
   17574,  4778, 19063,   989,  4778,  1208,  4778,  1208,  4778, 17613,
   13241, 13305, 13319, 13355,  -441,  4778, 19063,  4778,  1208,  4778,
   17629,  4778, 17679,  4778, 17719,  4778, 17758,  4778,  1208,  4778,
   17774, 17824,  4778, 17864,  4778, 19063,  4778, 19063,  4778,  4778,
    4487, 19063,  4778,  1208,  4778,  4778,  4666,  4778, 19063,  4778,
    1208,  4778,  4778,  4778,   289,  4778,  4778,   299,  4845,  4778,
     316,  4778,  4778,  4778,  4778,  -441,  -441,  -441,  -441,  -441,
    4778,  4778,  4778,  -441,  -441,  4778,  4778,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  4778,  4778,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  4778,  -441,  4778,  4778,  4778,  4778,
    4778,  -441,  4778,  -441,   304,   305,  -441,  4778,  -441,   306,
     307,  -441,  4778,  -441,  4778,   -51,  4778,   -39, 14907, 17903,
    4778, 17919,  4778,  -441, 17969,  2630, 18009,  4778, 14983,  4778,
    1616,  1644, 18048,  4778,  4778,  4778,  4778,  4778, 15019,  1741,
   18064,  4778, 18114,  4778, 18154,  4778, 18193,  4778, 18209,  1749,
   18259,  4778,  4778, 18299,  4778, 15055, 15069,  4955, 15131,  4778,
    1885,  1895,  5024,  4778, 15169,  1900,  5134,  5203, 11936,  4778,
    5313,  5382,  -441,  4778, 11989,  4778, 12027, 12043, 15216, 18338,
   18354, 15281, 15317,   460,   674, 18404, 15331,  1920,  5492,   802,
    5561,  1992,  2090,  2339,  -441,  -441, 12084,  -441,  -441, 12139,
   12201, 12239,  -441,  4778, 19063,  4778, 18444, 11043,   -94,  -441,
   18483,  4778, 18499,  -441,  1178,  -441,  -441,  4778, 19063, 15367,
   15431, 15479, 15515,  -441,  -441,  4778, 19063,  1208,  4778, 19063,
    4778,  5671,  4778, 18549,  1208,  4778,  -441,  4778, 18589, 19063,
    4778, 19063,  -441,  -441,  4778,  -441, 19063,  -441,  -441,  4778,
    1208,  -441,  -441,  4778,  4778,   322, 19063,  4778,  4778,  -441,
     325,  1208,  -441,  -441,  -441,  4778,  4778,  -441,  -441,  4778,
    -441,  4778,  -441,  4778,  -441,  -441,  4778,  4778,  -441,  4778,
    -441,  -441,  -441,  -441,  -441,   -28,   -26, 15579, 18628,  4778,
     -22, 18644,  2809, 18694,  4778,  4778, 15593,  -441,  -441,  -441,
    -441, 15629,  2518, 15665,   831,  4778, 18734,  2697,  4778, 15741,
   18773,  4778, 15777, 15813,  2876,   876,  3055,  4778,  1085,  3234,
    4778, 15827, 15889, 13391, 15927, 18789,  1108, 15974,  3413,  -441,
    4778, 19063,  -441, 11222,   -92,  -441, 18839,  4778, 19063, 19063,
    -441,  -441,  -441,  -441,  4778,  -441, 19063,  4778,  -441, 18879,
    1208,  -441,  4778, 18918,  -441,  -441,  -441,  4778,  -441,  -441,
   19063,  4778,  -441,  -441,  1208,  -441,  -441,  4778,  -441,  -441,
    4778,  4778,  -441,  -441,  -441, 16039,   -20, 18934,  -192, 16075,
   16089, 18984,  3592,  4778, 19024,  4778, 16125, 16189, 16237, 16273,
   16337,  -441,  -441,   334,  -441,  -441,  -441,  -441,  4778,  -441,
    1208,  4778, 19063,  -441,  -441,  -441,  -441,  -441,   318,  3771,
   16351,  -441,  -441,  -441
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -441,   150,  -441,  -441,   463,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,   154,   158,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,
    -441,  -441,  -441,  -441,  -441,  -441,  -441,  -441,   -34,  -244,
      -1,  -440,   931
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -204
static const yytype_int16 yytable[] =
{
     275,   326,   282,   285,   287,   288,   293,   295,   385,   297,
    1204,   319,   320,   298,   512,   513,   299,   321,   322,   300,
     386,   289,   304,   307,   310,   312,   301,   290,   512,   513,
     318,   302,   290,   323,   327,   329,   332,   313,   335,   314,
     338,   340,   342,   343,   345,   347,   348,   349,   350,   352,
     525,   526,   527,   357,   512,   513,   360,  -201,  -201,   387,
     625,   626,   533,   354,   368,   370,   373,   376,     1,     2,
     382,   388,   355,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,   358,    15,    16,    17,    18,
      19,    20,    21,   315,   362,    22,    23,   316,    24,    25,
      26,    27,    28,    29,   915,    30,    31,   916,   804,   501,
      32,    33,   514,   515,  -198,  -198,  -142,  -142,   366,   534,
     365,   501,  -170,  -170,  -167,  -167,   389,   501,   390,   501,
     392,   525,   526,   527,   737,   738,   393,   519,   520,   521,
     522,   394,   523,   920,    34,    35,   921,   910,   911,   395,
      36,   519,   520,   521,   522,   396,   523,   912,   913,   541,
      37,    38,    39,    40,    41,  -202,  -202,   501,    42,    43,
      44,    45,    46,    47,    48,    49,    50,  -199,  -199,   521,
     522,   397,   523,    51,    52,    53,   398,    54,  -203,  -203,
    -200,  -200,   426,   399,    55,   525,   526,   527,  1142,   501,
    1202,   501,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   528,   529,   530,    56,   531,
     413,    57,    58,    59,    60,    61,    62,   811,   748,   414,
     415,    63,    64,   416,   417,   418,   419,    65,    66,   420,
      67,    68,   421,   422,    69,   423,    70,   424,   425,    71,
     427,   428,   429,    72,   430,   431,   432,   433,   434,   493,
     435,   436,   437,   438,   439,   440,   441,    73,    74,   442,
     443,    75,   503,   504,   444,   445,   446,   447,   448,   449,
     535,   -16,   450,   537,   451,   539,   525,   526,   527,   543,
     452,   544,   502,   546,   453,   548,   528,   529,   530,   454,
     531,   455,   551,   456,   457,   553,   458,   550,   504,   459,
     504,   460,   532,   461,   462,   463,   557,   464,   560,   562,
     564,   566,   465,   466,   568,   467,   468,   570,   469,   470,
     572,   471,   504,   472,   587,   473,   474,   475,   576,   476,
     578,   477,   478,   580,   531,   582,   479,   480,   481,   482,
     585,   612,   483,   484,   485,   589,   486,   487,   488,   489,
     528,   529,   530,   490,   531,   491,   594,   492,   596,   494,
     495,   600,   829,   748,   504,   496,   603,   497,   504,   498,
     606,   499,   504,   500,   613,   614,   588,   620,   591,   592,
     593,   627,   645,   653,   523,   628,   629,   631,   632,   634,
     668,   669,   670,   671,   525,   526,   527,   672,   643,   644,
     673,   646,   647,   648,   649,   650,   651,   652,   674,   654,
     655,   656,   657,   658,   659,   675,   686,   661,   662,   665,
     667,   687,   688,   689,   690,   691,   692,   693,   694,   676,
     677,   678,   679,   680,   681,   682,   684,   695,   696,   697,
     698,   528,   529,   530,   699,   531,   700,   701,   702,   703,
     704,   705,   706,   835,   748,   707,   708,   709,   710,   711,
     525,   526,   527,   712,   713,   714,   717,   721,   722,   718,
     719,   720,   723,   724,   729,   739,   726,   728,   740,   784,
     841,   732,   785,   842,   845,   846,   914,   919,   979,   851,
     743,   746,   525,   526,   527,   749,   750,   751,   752,   753,
     754,   755,   756,   757,   758,   759,   760,   761,   762,   763,
     765,   766,   767,   852,   982,   985,  1004,  1005,  1007,  1008,
     774,  1127,   776,   777,  1130,   779,  1218,   781,  1221,   783,
     621,   391,   933,  1069,   788,     0,   790,   624,   792,   793,
     623,     0,     0,     0,     0,     0,     0,   799,   800,     0,
     801,     0,   802,     0,   803,   162,   163,   164,   806,   528,
     529,   530,   810,   531,   812,     0,   814,     0,   816,     0,
       0,   575,   820,   821,     0,   823,     0,     0,     0,   825,
       0,     0,   165,     0,   827,     0,     0,     0,   831,   525,
     526,   527,     0,     0,     0,     0,   838,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   528,   529,   530,   193,   531,
       0,   194,   195,   196,   197,     0,   198,   839,   748,   199,
     200,   201,   202,   203,   204,     0,     0,     0,     0,     0,
     205,   206,   207,     0,   525,   526,   527,   528,   529,   530,
       0,   531,   208,   209,   210,   211,   212,   213,     0,  1079,
    1080,   214,     0,   215,   216,   217,   218,   219,   220,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   244,   245,   525,   526,   527,     0,
       0,     0,   246,   247,   248,   249,     0,     0,   250,   251,
     252,   253,     0,     0,   254,     0,     0,   255,   256,     0,
     257,     0,   258,   259,   525,   526,   527,   260,     0,   261,
     262,   610,   263,   264,   611,   265,   266,   267,     0,   268,
     269,   270,   271,   272,   528,   529,   530,     0,   531,     0,
     273,     0,     0,     0,     0,     0,   599,   928,   274,   929,
       0,   931,     0,     0,   290,   543,   934,     0,   936,     0,
     938,     0,     0,     0,     0,     0,     0,   942,     0,     0,
       0,     0,     0,     0,   948,     0,     0,     0,   950,     0,
     952,     0,   954,     0,   956,     0,   958,     0,   960,     0,
       0,   963,     0,   965,     0,   966,     0,     0,   968,   528,
     529,   530,     0,   531,     0,     0,   974,     0,     0,     0,
       0,   605,   978,     0,   525,   526,   527,     0,   984,     0,
     986,   987,   988,   989,     0,     0,     0,     0,     0,   990,
     991,   992,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,     0,     0,     0,     0,
       0,   528,   529,   530,     0,   531,     0,   995,   996,   525,
     526,   527,     0,  1081,  1082,     0,     0,     0,     0,     0,
       0,  1019,     0,     0,   525,   526,   527,     0,     0,   528,
     529,   530,     0,   531,     0,     0,  1006,     0,   525,   526,
     527,  1009,   748,  1010,     0,  1011,     0,     0,     0,  1014,
       0,  1016,   276,   280,  1020,     0,  1022,     0,  1024,     0,
       0,     0,  1028,  1029,  1030,  1031,  1032,     0,     0,     0,
    1036,     0,  1039,     0,   305,   308,  1043,     0,     0,     0,
    1048,  1049,     0,  1051,     0,     0,     0,   330,  1056,   334,
     336,   337,     0,     0,     0,     0,     0,     0,  1066,     0,
       0,     0,   353,     0,     0,     0,     0,   359,     0,   361,
       0,   363,   364,   525,   526,   527,     0,   371,   374,     0,
     378,   380,     0,   384,     0,     0,     0,     0,     0,   528,
     529,   530,  1097,   531,  1098,     0,  1101,   939,     0,     0,
    1103,  1087,  1088,     0,     0,     0,  1106,     0,     0,     0,
       0,   506,   507,   508,  1111,     0,     0,  1113,   528,   529,
     530,  1116,   531,     0,  1119,     0,  1120,     0,     0,  1122,
    1154,  1155,     0,  1123,   528,   529,   530,     0,   531,     0,
       0,     0,     0,     0,   509,   510,   511,   857,  1145,   528,
     529,   530,     0,   531,  1131,  1132,   512,   513,  1133,     0,
    1134,   609,  1135,   528,   529,   530,  1137,   531,     0,     0,
       0,     0,     0,     0,     0,  1167,  1168,     0,  1141,     0,
       0,  1146,     0,  1148,  1149,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1156,     0,     0,  1159,     0,     0,
    1163,     0,     0,     0,     0,     0,  1170,   525,   526,   527,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1185,
       0,     0,  1187,     0,     0,     0,  1189,     0,     0,     0,
     525,   526,   527,  1190,  1205,     0,  1191,     0,   528,   529,
     530,  1194,   531,     0,     0,     0,  1196,     0,     0,     0,
    1197,   859,     0,     0,   514,   515,  1198,     0,     0,  1199,
    1200,   525,   526,   527,     0,     0,     0,   525,   526,   527,
       0,     0,     0,     0,  1212,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   505,     0,  1105,     0,     0,
    1220,     0,   505,     0,     0,   505,     0,   505,     0,     0,
     506,   507,   508,   505,     0,   505,     0,   505,     0,   525,
     526,   527,     0,     0,   505,     0,     0,   505,     0,     0,
     505,     0,   505,     0,     0,   525,   526,   527,   505,     0,
     525,   526,   527,   509,   510,   511,   505,     0,     0,   505,
       0,     0,   505,     0,   574,   512,   513,   525,   526,   527,
     505,     0,   505,     0,     0,   505,     0,   505,     0,     0,
       0,     0,   505,     0,     0,     0,     0,   505,     0,     0,
       0,     0,   528,   529,   530,     0,   531,     0,   505,     0,
     597,     0,     0,   505,  1171,  1172,   505,     0,   505,     0,
     604,     0,   505,     0,   608,   528,   529,   530,   619,   531,
       0,     0,   525,   526,   527,     0,     0,  1181,  1182,     0,
     633,     0,   635,   636,   637,   638,   639,   640,   641,   642,
       0,     0,     0,     0,     0,     0,   528,   529,   530,     0,
     531,     0,   528,   529,   530,     0,   531,     0,   660,   863,
     663,     0,     0,   514,   515,   864,     0,     0,     0,     0,
       0,     0,   525,   526,   527,     0,     0,     0,     0,   685,
       0,     0,     0,     0,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,   528,   529,   530,     0,   531,     0,
       0,     0,     0,     0,     0,     0,     0,   882,     0,   716,
     528,   529,   530,     0,   531,   528,   529,   530,     0,   531,
       0,   730,   731,   885,     0,   733,   734,   735,   736,     0,
       0,     0,   528,   529,   530,     0,   531,     0,     0,     0,
       0,     0,   525,   526,   527,   896,     0,     0,     0,     0,
       0,   764,     0,     0,     0,     0,   768,   769,   770,   771,
     772,   773,   775,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   795,     0,   797,     0,   528,   529,   530,
     505,   531,   505,     0,   505,     0,   505,     0,     0,     0,
     897,     0,   808,     0,     0,     0,     0,   162,   163,   164,
       0,     0,   818,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   830,
       0,     0,   833,     0,   165,     0,   836,   528,   529,   530,
     840,   531,     0,     0,     0,     0,     0,     0,   848,   860,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,   206,   207,     0,     0,   528,   529,   530,
       0,   531,     0,     0,   208,   209,   210,   211,   212,   213,
     903,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,   525,   526,
     527,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,   525,   526,   527,   260,
       0,   261,   262,     0,   263,   264,     0,   265,   266,   267,
       0,   268,   269,   270,   271,   272,   506,   507,   508,     0,
       0,     0,   273,     0,     0,     0,     0,     0,     0,     0,
     274,   741,     0,     0,     0,   940,     0,   941,   742,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   949,   509,
     510,   511,     0,     0,     0,     0,     0,     0,   959,     0,
       0,   512,   513,   506,   507,   508,     0,     0,     0,   967,
       0,     0,     0,   970,     0,   971,   972,     0,     0,     0,
     975,     0,   976,   977,     0,     0,   980,   981,     0,     0,
       0,     0,     0,   525,   526,   527,   509,   510,   511,     0,
       0,   525,   526,   527,     0,     0,   993,   994,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,     0,   531,     0,     0,
       0,     0,     0,     0,     0,   997,  1025,   998,   999,  1000,
    1001,  1002,     0,  1003,   509,   510,   511,     0,     0,   514,
     515,   528,   529,   530,     0,   531,   512,   513,     0,     0,
     506,   507,   508,     0,  1026,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,  1037,     0,     0,     0,  1041,   524,  1044,     0,
       0,     0,     0,   509,   510,   511,   514,   515,     0,     0,
       0,   506,   507,   508,  1060,   512,   513,     0,     0,     0,
     525,   526,   527,     0,   848,     0,  1071,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,   525,   526,   527,
     843,     0,     0,   844,   509,   510,   511,   525,   526,   527,
       0,     0,   525,   526,   527,     0,   512,   513,   528,   529,
     530,     0,   531,     0,   514,   515,   528,   529,   530,     0,
     531,  1034,   525,   526,   527,     0,  1112,     0,     0,  1046,
       0,  1114,     0,  1117,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   528,   529,   530,   849,   531,
    1124,   850,     0,     0,  1125,  1126,     0,   861,  1128,  1129,
       0,     0,     0,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1136,     0,     0,
    1138,     0,     0,     0,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,   525,   526,   527,   917,     0,     0,
     918,     0,     0,     0,   514,   515,     0,     0,     0,  1160,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1174,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   528,   529,   530,   922,   531,
       0,   923,     0,     0,     0,     0,     0,   862,  1192,     0,
       0,     0,   528,   529,   530,     0,   531,     0,   162,   163,
     164,     0,   528,   529,   530,  1057,   531,   528,   529,   530,
       0,   531,     0,     0,     0,  1058,     0,     0,     0,     0,
    1062,     0,     0,     0,  1210,   165,     0,   528,   529,   530,
       0,   531,   525,   526,   527,     0,     0,     0,     0,  1219,
    1085,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,   193,     0,     0,   194,   195,   196,   197,     0,   198,
       0,     0,   199,   200,   201,   202,   203,   204,     0,     0,
       0,     0,     0,   205,   206,   207,     0,     0,     0,   528,
     529,   530,     0,   531,     0,   208,   209,   210,   211,   212,
     213,     0,  1090,     0,   214,     0,   215,   216,   217,   218,
     219,   220,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,     0,
       0,     0,     0,     0,     0,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,     0,     0,   254,     0,     0,
     255,   256,     0,   257,     0,   258,   259,   162,   163,   164,
     260,     0,   261,   262,     0,   263,   264,     0,   265,   266,
     267,     0,   268,   269,   270,   271,   272,   528,   529,   530,
       0,   531,     0,   273,   165,     0,     0,     0,     0,     0,
    1091,   274,   744,     0,     0,     0,     0,     0,     0,   745,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,   206,   207,     0,     0,     0,     0,     0,
       0,   525,   526,   527,   208,   209,   210,   211,   212,   213,
       0,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,   162,   163,   164,   260,
       0,   261,   262,   615,   263,   264,   616,   265,   266,   267,
       0,   268,   269,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   273,   165,     0,     0,     0,     0,     0,     0,
     274,   525,   526,   527,     0,     0,   617,     0,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
       0,     0,   194,   195,   196,   197,     0,   198,     0,     0,
     199,   200,   201,   202,   203,   204,   528,   529,   530,     0,
     531,   205,   206,   207,     0,     0,     0,     0,     0,  1092,
     525,   526,   527,   208,   209,   210,   211,   212,   213,     0,
       0,     0,   214,     0,   215,   216,   217,   218,   219,   220,
       0,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,     0,     0,     0,
       0,     0,     0,   246,   247,   248,   249,     0,     0,   250,
     251,   252,   253,     0,     0,   254,     0,     0,   255,   256,
       0,   257,     0,   258,   259,   162,   163,   164,   260,     0,
     261,   262,     0,   263,   264,     0,   265,   266,   267,     0,
     268,   269,   270,   324,   272,     0,   528,   529,   530,     0,
     531,   273,   165,     0,     0,     0,     0,     0,   865,   325,
     525,   526,   527,     0,     0,   290,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,     0,
       0,   194,   195,   196,   197,     0,   198,     0,     0,   199,
     200,   201,   202,   203,   204,   528,   529,   530,     0,   531,
     205,   206,   207,     0,     0,     0,     0,     0,  1152,   525,
     526,   527,   208,   209,   210,   211,   212,   213,     0,     0,
       0,   214,     0,   215,   216,   217,   218,   219,   220,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   244,   245,     0,     0,     0,     0,
       0,     0,   246,   247,   248,   249,     0,     0,   250,   251,
     252,   253,     0,     0,   254,     0,     0,   255,   256,     0,
     257,     0,   258,   259,   162,   163,   164,   260,     0,   261,
     262,     0,   263,   264,     0,   265,   266,   267,     0,   268,
     269,   270,  1018,   272,     0,   528,   529,   530,     0,   531,
     273,   165,     0,     0,     0,     0,     0,   866,   274,   525,
     526,   527,     0,     0,   290,     0,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,     0,     0,
     194,   195,   196,   197,     0,   198,     0,     0,   199,   200,
     201,   202,   203,   204,   528,   529,   530,     0,   531,   205,
     206,   207,     0,     0,     0,     0,     0,  1158,   525,   526,
     527,   208,   209,   210,   211,   212,   213,     0,     0,     0,
     214,     0,   215,   216,   217,   218,   219,   220,     0,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,     0,     0,     0,     0,     0,
       0,   246,   247,   248,   249,     0,     0,   250,   251,   252,
     253,     0,     0,   254,     0,     0,   255,   256,     0,   257,
       0,   258,   259,   162,   163,   164,   260,     0,   261,   262,
       0,   263,   264,     0,   265,   266,   267,     0,   268,   269,
     270,  1144,   272,     0,   528,   529,   530,     0,   531,   273,
     165,     0,     0,     0,     0,     0,   904,   274,   525,   526,
     527,     0,     0,   290,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,     0,     0,   193,     0,     0,   194,
     195,   196,   197,     0,   198,     0,     0,   199,   200,   201,
     202,   203,   204,   528,   529,   530,     0,   531,   205,   206,
     207,     0,     0,     0,     0,     0,  1166,   525,   526,   527,
     208,   209,   210,   211,   212,   213,     0,     0,     0,   214,
       0,   215,   216,   217,   218,   219,   220,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   244,   245,     0,     0,     0,     0,     0,     0,
     246,   247,   248,   249,     0,     0,   250,   251,   252,   253,
       0,     0,   254,     0,     0,   255,   256,     0,   257,     0,
     258,   259,   162,   163,   164,   260,     0,   261,   262,     0,
     263,   264,     0,   265,   266,   267,     0,   268,   269,   270,
     271,   272,     0,   528,   529,   530,     0,   531,   273,   165,
       0,     0,     0,     0,     0,   906,   274,     0,   555,     0,
       0,     0,     0,     0,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,     0,     0,   194,   195,
     196,   197,     0,   198,     0,     0,   199,   200,   201,   202,
     203,   204,   528,   529,   530,     0,   531,   205,   206,   207,
       0,     0,     0,     0,     0,  1169,   525,   526,   527,   208,
     209,   210,   211,   212,   213,     0,     0,     0,   214,     0,
     215,   216,   217,   218,   219,   220,     0,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,     0,     0,     0,     0,     0,     0,   246,
     247,   248,   249,     0,     0,   250,   251,   252,   253,     0,
       0,   254,     0,     0,   255,   256,     0,   257,     0,   258,
     259,   162,   163,   164,   260,     0,   261,   262,     0,   263,
     264,     0,   265,   266,   267,     0,   268,   269,   270,   271,
     272,     0,     0,     0,     0,     0,     0,   273,   165,     0,
       0,     0,     0,     0,     0,   274,     0,   556,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,     0,     0,   194,   195,   196,
     197,     0,   198,     0,     0,   199,   200,   201,   202,   203,
     204,   528,   529,   530,     0,   531,   205,   206,   207,     0,
       0,     0,     0,     0,  1173,   525,   526,   527,   208,   209,
     210,   211,   212,   213,     0,     0,     0,   214,     0,   215,
     216,   217,   218,   219,   220,     0,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     244,   245,     0,     0,     0,     0,     0,     0,   246,   247,
     248,   249,     0,     0,   250,   251,   252,   253,     0,     0,
     254,     0,     0,   255,   256,     0,   257,     0,   258,   259,
     162,   163,   164,   260,     0,   261,   262,     0,   263,   264,
       0,   265,   266,   267,     0,   268,   269,   270,   271,   272,
       0,     0,     0,     0,     0,     0,   273,   165,     0,     0,
       0,     0,     0,     0,   274,     0,   602,     0,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,     0,     0,   193,     0,     0,   194,   195,   196,   197,
       0,   198,     0,     0,   199,   200,   201,   202,   203,   204,
     528,   529,   530,     0,   531,   205,   206,   207,     0,     0,
       0,     0,     0,  1184,   525,   526,   527,   208,   209,   210,
     211,   212,   213,     0,     0,     0,   214,     0,   215,   216,
     217,   218,   219,   220,     0,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,     0,     0,     0,     0,     0,     0,   246,   247,   248,
     249,     0,     0,   250,   251,   252,   253,     0,     0,   254,
       0,     0,   255,   256,     0,   257,     0,   258,   259,   162,
     163,   164,   260,     0,   261,   262,     0,   263,   264,     0,
     265,   266,   267,     0,   268,   269,   270,   271,   272,     0,
       0,     0,     0,     0,     0,   273,   165,     0,     0,     0,
       0,     0,     0,   274,     0,   630,     0,     0,     0,     0,
       0,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
       0,     0,   193,     0,     0,   194,   195,   196,   197,     0,
     198,     0,     0,   199,   200,   201,   202,   203,   204,   528,
     529,   530,     0,   531,   205,   206,   207,     0,     0,     0,
       0,     0,  1209,   525,   526,   527,   208,   209,   210,   211,
     212,   213,     0,     0,     0,   214,     0,   215,   216,   217,
     218,   219,   220,     0,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,     0,   244,   245,
       0,     0,     0,     0,     0,     0,   246,   247,   248,   249,
       0,     0,   250,   251,   252,   253,     0,     0,   254,     0,
       0,   255,   256,     0,   257,     0,   258,   259,   162,   163,
     164,   260,     0,   261,   262,     0,   263,   264,     0,   265,
     266,   267,     0,   268,   269,   270,   271,   272,     0,     0,
       0,     0,     0,     0,   273,   165,     0,     0,     0,     0,
       0,     0,   274,     0,   664,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,   193,     0,     0,   194,   195,   196,   197,     0,   198,
       0,     0,   199,   200,   201,   202,   203,   204,   528,   529,
     530,     0,   531,   205,   206,   207,     0,     0,     0,     0,
       0,  1222,   525,   526,   527,   208,   209,   210,   211,   212,
     213,     0,     0,     0,   214,     0,   215,   216,   217,   218,
     219,   220,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,     0,
       0,     0,     0,     0,     0,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,     0,     0,   254,     0,     0,
     255,   256,     0,   257,     0,   258,   259,   162,   163,   164,
     260,     0,   261,   262,     0,   263,   264,     0,   265,   266,
     267,     0,   268,   269,   270,   271,   272,     0,     0,     0,
       0,     0,     0,   273,   165,     0,     0,     0,     0,     0,
       0,   274,     0,   666,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,   528,   529,   530,
       0,   531,   205,   206,   207,     0,     0,     0,     0,   907,
       0,   525,   526,   527,   208,   209,   210,   211,   212,   213,
       0,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,   162,   163,   164,   260,
       0,   261,   262,     0,   263,   264,     0,   265,   266,   267,
       0,   268,   269,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   273,   165,     0,     0,     0,     0,     0,     0,
     274,     0,   683,     0,     0,     0,     0,     0,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
       0,     0,   194,   195,   196,   197,     0,   198,     0,     0,
     199,   200,   201,   202,   203,   204,   528,   529,   530,     0,
     531,   205,   206,   207,     0,     0,     0,     0,   908,     0,
     525,   526,   527,   208,   209,   210,   211,   212,   213,     0,
       0,     0,   214,     0,   215,   216,   217,   218,   219,   220,
       0,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,     0,     0,     0,
       0,     0,     0,   246,   247,   248,   249,     0,     0,   250,
     251,   252,   253,     0,     0,   254,     0,     0,   255,   256,
       0,   257,     0,   258,   259,   162,   163,   164,   260,     0,
     261,   262,     0,   263,   264,     0,   265,   266,   267,     0,
     268,   269,   270,   271,   272,     0,     0,     0,     0,     0,
       0,   273,   165,     0,     0,     0,     0,     0,     0,   274,
       0,   715,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,     0,
       0,   194,   195,   196,   197,     0,   198,     0,     0,   199,
     200,   201,   202,   203,   204,   528,   529,   530,     0,   531,
     205,   206,   207,     0,     0,     0,     0,   909,     0,   525,
     526,   527,   208,   209,   210,   211,   212,   213,     0,     0,
       0,   214,     0,   215,   216,   217,   218,   219,   220,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   244,   245,     0,     0,     0,     0,
       0,     0,   246,   247,   248,   249,     0,     0,   250,   251,
     252,   253,     0,     0,   254,     0,     0,   255,   256,     0,
     257,     0,   258,   259,   162,   163,   164,   260,     0,   261,
     262,     0,   263,   264,     0,   265,   266,   267,     0,   268,
     269,   270,   271,   272,     0,     0,     0,     0,     0,     0,
     273,   165,     0,     0,     0,     0,     0,     0,   274,     0,
     725,     0,     0,     0,     0,     0,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,     0,     0,
     194,   195,   196,   197,     0,   198,     0,     0,   199,   200,
     201,   202,   203,   204,   528,   529,   530,     0,   531,   205,
     206,   207,     0,     0,     0,     0,   969,     0,   525,   526,
     527,   208,   209,   210,   211,   212,   213,     0,     0,     0,
     214,     0,   215,   216,   217,   218,   219,   220,     0,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,     0,     0,     0,     0,     0,
       0,   246,   247,   248,   249,     0,     0,   250,   251,   252,
     253,     0,     0,   254,     0,     0,   255,   256,     0,   257,
       0,   258,   259,   162,   163,   164,   260,     0,   261,   262,
       0,   263,   264,     0,   265,   266,   267,     0,   268,   269,
     270,   271,   272,     0,     0,     0,     0,     0,     0,   273,
     165,     0,     0,     0,     0,     0,     0,   274,     0,   727,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,     0,     0,   193,     0,     0,   194,
     195,   196,   197,     0,   198,     0,     0,   199,   200,   201,
     202,   203,   204,   528,   529,   530,     0,   531,   205,   206,
     207,     0,     0,     0,     0,   973,     0,   525,   526,   527,
     208,   209,   210,   211,   212,   213,     0,     0,     0,   214,
       0,   215,   216,   217,   218,   219,   220,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   244,   245,     0,     0,     0,     0,     0,     0,
     246,   247,   248,   249,     0,     0,   250,   251,   252,   253,
       0,     0,   254,     0,     0,   255,   256,     0,   257,     0,
     258,   259,   162,   163,   164,   260,     0,   261,   262,     0,
     263,   264,     0,   265,   266,   267,     0,   268,   269,   270,
     271,   272,     0,     0,     0,     0,     0,     0,   273,   165,
       0,     0,     0,     0,     0,     0,   274,   525,   526,   527,
       0,     0,     0,     0,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,     0,     0,   194,   195,
     196,   197,     0,   198,     0,     0,   199,   200,   201,   202,
     203,   204,   528,   529,   530,     0,   531,   205,   206,   207,
       0,     0,     0,     0,   983,     0,   525,   526,   527,   208,
     209,   210,   211,   212,   213,     0,     0,     0,   214,     0,
     215,   216,   217,   218,   219,   220,     0,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,     0,     0,     0,     0,     0,     0,   246,
     247,   248,   249,     0,     0,   250,   251,   252,   253,     0,
       0,   254,     0,     0,   255,   256,     0,   257,     0,   258,
     259,   162,   163,   164,   260,     0,   261,   262,     0,   263,
     264,     0,   265,   266,   267,     0,   268,   269,   270,   277,
     278,     0,   528,   529,   530,     0,   531,   273,   165,     0,
       0,     0,     0,     0,  1054,   274,   525,   526,   527,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,     0,     0,   194,   195,   196,
     197,     0,   198,     0,     0,   199,   200,   201,   202,   203,
     204,   528,   529,   530,     0,   531,   205,   206,   207,     0,
       0,     0,     0,  1059,     0,   525,   526,   527,   208,   209,
     210,   211,   212,   213,     0,     0,     0,   214,     0,   215,
     216,   217,   218,   219,   220,     0,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     244,   245,     0,     0,     0,     0,     0,     0,   246,   247,
     248,   249,     0,     0,   250,   251,   252,   253,     0,     0,
     254,     0,     0,   255,   256,     0,   257,     0,   258,   259,
     162,   163,   164,   260,     0,   261,   262,     0,   263,   264,
       0,   265,   266,   267,     0,   268,   269,   270,   271,   272,
       0,   528,   529,   530,     0,   531,   273,   165,     0,     0,
       0,     0,     0,  1063,   281,   525,   526,   527,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,     0,     0,   193,     0,     0,   194,   195,   196,   197,
       0,   198,     0,     0,   199,   200,   201,   202,   203,   204,
     528,   529,   530,     0,   531,   205,   206,   207,     0,     0,
       0,     0,  1064,     0,   525,   526,   527,   208,   209,   210,
     211,   212,   213,     0,     0,     0,   214,     0,   215,   216,
     217,   218,   219,   220,     0,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,     0,     0,     0,     0,     0,     0,   246,   247,   248,
     249,     0,     0,   250,   251,   252,   253,     0,     0,   254,
       0,     0,   255,   256,     0,   257,     0,   258,   259,   162,
     163,   164,   260,     0,   261,   262,     0,   263,   264,     0,
     265,   266,   267,     0,   268,   269,   270,   271,   272,     0,
     528,   529,   530,     0,   531,   273,   165,     0,     0,     0,
       0,     0,  1067,   284,   525,   526,   527,     0,     0,     0,
       0,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
       0,     0,   193,     0,     0,   194,   195,   196,   197,     0,
     198,     0,     0,   199,   200,   201,   202,   203,   204,   528,
     529,   530,     0,   531,   205,   206,   207,     0,     0,     0,
       0,  1068,     0,   525,   526,   527,   208,   209,   210,   211,
     212,   213,     0,     0,     0,   214,     0,   215,   216,   217,
     218,   219,   220,     0,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,     0,   244,   245,
       0,     0,     0,     0,     0,     0,   246,   247,   248,   249,
       0,     0,   250,   251,   252,   253,     0,     0,   254,     0,
       0,   255,   256,     0,   257,     0,   258,   259,   162,   163,
     164,   260,     0,   261,   262,     0,   263,   264,     0,   265,
     266,   267,     0,   268,   269,   270,   271,   272,     0,   528,
     529,   530,     0,   531,   273,   165,     0,     0,     0,     0,
       0,  1086,   286,   525,   526,   527,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,   193,     0,     0,   194,   195,   196,   197,     0,   198,
       0,     0,   199,   200,   201,   202,   203,   204,   528,   529,
     530,     0,   531,   205,   206,   207,     0,     0,     0,     0,
    1089,     0,     0,     0,     0,   208,   209,   210,   211,   212,
     213,     0,     0,     0,   214,     0,   215,   216,   217,   218,
     219,   220,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,     0,
       0,     0,     0,     0,     0,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,     0,     0,   254,     0,     0,
     255,   256,     0,   257,     0,   258,   259,   162,   163,   164,
     260,     0,   261,   262,     0,   263,   264,     0,   265,   266,
     267,     0,   268,   269,   270,   271,   272,     0,   528,   529,
     530,     0,   531,   273,   165,     0,     0,     0,     0,     0,
    1115,   292,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,   206,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,   209,   210,   211,   212,   213,
       0,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,   162,   163,   164,   260,
       0,   261,   262,     0,   263,   264,     0,   265,   266,   267,
       0,   268,   269,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   273,   165,     0,     0,     0,     0,     0,     0,
     294,     0,     0,     0,     0,     0,     0,     0,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
       0,     0,   194,   195,   196,   197,     0,   198,     0,     0,
     199,   200,   201,   202,   203,   204,     0,     0,     0,     0,
       0,   205,   206,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   209,   210,   211,   212,   213,     0,
       0,     0,   214,     0,   215,   216,   217,   218,   219,   220,
       0,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,     0,     0,     0,
       0,     0,     0,   246,   247,   248,   249,     0,     0,   250,
     251,   252,   253,     0,     0,   254,     0,     0,   255,   256,
       0,   257,     0,   258,   259,   162,   163,   164,   260,     0,
     261,   262,     0,   263,   264,     0,   265,   266,   267,     0,
     268,   269,   270,   271,   272,     0,     0,     0,     0,     0,
       0,   273,   165,     0,     0,     0,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,     0,
       0,   194,   195,   196,   197,     0,   198,     0,     0,   199,
     200,   201,   202,   203,   204,     0,     0,     0,     0,     0,
     205,   206,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,   209,   210,   211,   212,   213,     0,     0,
       0,   214,     0,   215,   216,   217,   218,   219,   220,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   244,   245,     0,     0,     0,     0,
       0,     0,   246,   247,   248,   249,     0,     0,   250,   251,
     252,   253,     0,     0,   254,     0,     0,   255,   256,     0,
     257,     0,   258,   259,   162,   163,   164,   260,     0,   261,
     262,     0,   263,   264,     0,   265,   266,   267,     0,   268,
     269,   270,   271,   272,     0,     0,     0,     0,     0,     0,
     273,   165,     0,     0,     0,     0,     0,     0,   303,     0,
       0,     0,     0,     0,     0,     0,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,     0,     0,
     194,   195,   196,   197,     0,   198,     0,     0,   199,   200,
     201,   202,   203,   204,     0,     0,     0,     0,     0,   205,
     206,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   208,   209,   210,   211,   212,   213,     0,     0,     0,
     214,     0,   215,   216,   217,   218,   219,   220,     0,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,     0,     0,     0,     0,     0,
       0,   246,   247,   248,   249,     0,     0,   250,   251,   252,
     253,     0,     0,   254,     0,     0,   255,   256,     0,   257,
       0,   258,   259,   162,   163,   164,   260,     0,   261,   262,
       0,   263,   264,     0,   265,   266,   267,     0,   268,   269,
     270,   271,   272,     0,     0,     0,     0,     0,     0,   273,
     165,     0,     0,     0,     0,     0,     0,   306,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,     0,     0,   193,     0,     0,   194,
     195,   196,   197,     0,   198,     0,     0,   199,   200,   201,
     202,   203,   204,     0,     0,     0,     0,     0,   205,   206,
     207,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,   209,   210,   211,   212,   213,     0,     0,     0,   214,
       0,   215,   216,   217,   218,   219,   220,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   244,   245,     0,     0,     0,     0,     0,     0,
     246,   247,   248,   249,     0,     0,   250,   251,   252,   253,
       0,     0,   254,     0,     0,   255,   256,     0,   257,     0,
     258,   259,   162,   163,   164,   260,     0,   261,   262,     0,
     263,   264,     0,   265,   266,   267,     0,   268,   269,   270,
     271,   272,     0,     0,     0,     0,     0,     0,   273,   165,
       0,     0,     0,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,     0,     0,   194,   195,
     196,   197,     0,   198,     0,     0,   199,   200,   201,   202,
     203,   204,     0,     0,     0,     0,     0,   205,   206,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
     209,   210,   211,   212,   213,     0,     0,     0,   214,     0,
     215,   216,   217,   218,   219,   220,     0,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,     0,     0,     0,     0,     0,     0,   246,
     247,   248,   249,     0,     0,   250,   251,   252,   253,     0,
       0,   254,     0,     0,   255,   256,     0,   257,     0,   258,
     259,   162,   163,   164,   260,     0,   261,   262,     0,   263,
     264,     0,   265,   266,   267,     0,   268,   269,   270,   271,
     272,     0,     0,     0,     0,     0,     0,   273,   165,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,     0,     0,   194,   195,   196,
     197,     0,   198,     0,     0,   199,   200,   201,   202,   203,
     204,     0,     0,     0,     0,     0,   205,   206,   207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,   209,
     210,   211,   212,   213,     0,     0,     0,   214,     0,   215,
     216,   217,   218,   219,   220,     0,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     244,   245,     0,     0,     0,     0,     0,     0,   246,   247,
     248,   249,     0,     0,   250,   251,   252,   253,     0,     0,
     254,     0,     0,   255,   256,     0,   257,     0,   258,   259,
     162,   163,   164,   260,     0,   261,   262,     0,   263,   264,
       0,   265,   266,   267,     0,   268,   269,   270,   271,   272,
       0,     0,     0,     0,     0,     0,   273,   165,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,     0,     0,   193,     0,     0,   194,   195,   196,   197,
       0,   198,     0,     0,   199,   200,   201,   202,   203,   204,
       0,     0,     0,     0,     0,   205,   206,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,   209,   210,
     211,   212,   213,     0,     0,     0,   214,     0,   215,   216,
     217,   218,   219,   220,     0,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,     0,     0,     0,     0,     0,     0,   246,   247,   248,
     249,     0,     0,   250,   251,   252,   253,     0,     0,   254,
       0,     0,   255,   256,     0,   257,     0,   258,   259,   162,
     163,   164,   260,     0,   261,   262,     0,   263,   264,     0,
     265,   266,   267,     0,   268,   269,   270,   271,   272,     0,
       0,     0,     0,     0,     0,   273,   165,     0,     0,     0,
       0,     0,     0,   328,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
       0,     0,   193,     0,     0,   194,   195,   196,   197,     0,
     198,     0,     0,   199,   200,   201,   202,   203,   204,     0,
       0,     0,     0,     0,   205,   206,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208,   209,   210,   211,
     212,   213,     0,     0,     0,   214,     0,   215,   216,   217,
     218,   219,   220,     0,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,     0,   244,   245,
       0,     0,     0,     0,     0,     0,   246,   247,   248,   249,
       0,     0,   250,   251,   252,   253,     0,     0,   254,     0,
       0,   255,   256,     0,   257,     0,   258,   259,   162,   163,
     164,   260,     0,   261,   262,     0,   263,   264,     0,   265,
     266,   267,     0,   268,   269,   270,   271,   272,     0,     0,
       0,     0,     0,     0,   273,   165,     0,     0,     0,     0,
       0,     0,   331,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,   193,     0,     0,   194,   195,   196,   197,     0,   198,
       0,     0,   199,   200,   201,   202,   203,   204,     0,     0,
       0,     0,     0,   205,   206,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,   209,   210,   211,   212,
     213,     0,     0,     0,   214,     0,   215,   216,   217,   218,
     219,   220,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,     0,
       0,     0,     0,     0,     0,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,     0,     0,   254,     0,     0,
     255,   256,     0,   257,     0,   258,   259,   162,   163,   164,
     260,     0,   261,   262,     0,   263,   264,     0,   265,   266,
     267,     0,   268,   269,   270,   271,   272,     0,     0,     0,
       0,     0,     0,   273,   165,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,   206,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,   209,   210,   211,   212,   213,
       0,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,   162,   163,   164,   260,
       0,   261,   262,     0,   263,   264,     0,   265,   266,   267,
       0,   268,   269,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   273,   165,     0,     0,     0,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
       0,     0,   194,   195,   196,   197,     0,   198,     0,     0,
     199,   200,   201,   202,   203,   204,     0,     0,     0,     0,
       0,   205,   206,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   209,   210,   211,   212,   213,     0,
       0,     0,   214,     0,   215,   216,   217,   218,   219,   220,
       0,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,     0,     0,     0,
       0,     0,     0,   246,   247,   248,   249,     0,     0,   250,
     251,   252,   253,     0,     0,   254,     0,     0,   255,   256,
       0,   257,     0,   258,   259,   162,   163,   164,   260,     0,
     261,   262,     0,   263,   264,     0,   265,   266,   267,     0,
     268,   269,   270,   271,   272,     0,     0,     0,     0,     0,
       0,   273,   165,     0,     0,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,     0,
       0,   194,   195,   196,   197,     0,   198,     0,     0,   199,
     200,   201,   202,   203,   204,     0,     0,     0,     0,     0,
     205,   206,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,   209,   210,   211,   212,   213,     0,     0,
       0,   214,     0,   215,   216,   217,   218,   219,   220,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   244,   245,     0,     0,     0,     0,
       0,     0,   246,   247,   248,   249,     0,     0,   250,   251,
     252,   253,     0,     0,   254,     0,     0,   255,   256,     0,
     257,     0,   258,   259,   162,   163,   164,   260,     0,   261,
     262,     0,   263,   264,     0,   265,   266,   267,     0,   268,
     269,   270,   271,   272,     0,     0,     0,     0,     0,     0,
     273,   165,     0,     0,     0,     0,     0,     0,   344,     0,
       0,     0,     0,     0,     0,     0,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,     0,     0,
     194,   195,   196,   197,     0,   198,     0,     0,   199,   200,
     201,   202,   203,   204,     0,     0,     0,     0,     0,   205,
     206,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   208,   209,   210,   211,   212,   213,     0,     0,     0,
     214,     0,   215,   216,   217,   218,   219,   220,     0,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,     0,     0,     0,     0,     0,
       0,   246,   247,   248,   249,     0,     0,   250,   251,   252,
     253,     0,     0,   254,     0,     0,   255,   256,     0,   257,
       0,   258,   259,   162,   163,   164,   260,     0,   261,   262,
       0,   263,   264,     0,   265,   266,   267,     0,   268,   269,
     270,   271,   272,     0,     0,     0,     0,     0,     0,   273,
     165,     0,     0,     0,     0,     0,     0,   346,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,     0,     0,   193,     0,     0,   194,
     195,   196,   197,     0,   198,     0,     0,   199,   200,   201,
     202,   203,   204,     0,     0,     0,     0,     0,   205,   206,
     207,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,   209,   210,   211,   212,   213,     0,     0,     0,   214,
       0,   215,   216,   217,   218,   219,   220,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   244,   245,     0,     0,     0,     0,     0,     0,
     246,   247,   248,   249,     0,     0,   250,   251,   252,   253,
       0,     0,   254,     0,     0,   255,   256,     0,   257,     0,
     258,   259,   162,   163,   164,   260,     0,   261,   262,     0,
     263,   264,     0,   265,   266,   267,     0,   268,   269,   270,
     271,   272,     0,     0,     0,     0,     0,     0,   273,   165,
       0,     0,     0,     0,     0,     0,   351,     0,     0,     0,
       0,     0,     0,     0,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,     0,     0,   194,   195,
     196,   197,     0,   198,     0,     0,   199,   200,   201,   202,
     203,   204,     0,     0,     0,     0,     0,   205,   206,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
     209,   210,   211,   212,   213,     0,     0,     0,   214,     0,
     215,   216,   217,   218,   219,   220,     0,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,     0,     0,     0,     0,     0,     0,   246,
     247,   248,   249,     0,     0,   250,   251,   252,   253,     0,
       0,   254,     0,     0,   255,   256,     0,   257,     0,   258,
     259,   162,   163,   164,   260,     0,   261,   262,     0,   263,
     264,     0,   265,   266,   267,     0,   268,   269,   270,   271,
     272,     0,     0,     0,     0,     0,     0,   273,   165,     0,
       0,     0,     0,     0,     0,   356,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,     0,     0,   194,   195,   196,
     197,     0,   198,     0,     0,   199,   200,   201,   202,   203,
     204,     0,     0,     0,     0,     0,   205,   206,   207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,   209,
     210,   211,   212,   213,     0,     0,     0,   214,     0,   215,
     216,   217,   218,   219,   220,     0,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     244,   245,     0,     0,     0,     0,     0,     0,   246,   247,
     248,   249,     0,     0,   250,   251,   252,   253,     0,     0,
     254,     0,     0,   255,   256,     0,   257,     0,   258,   259,
     162,   163,   164,   260,     0,   261,   262,     0,   263,   264,
       0,   265,   266,   267,     0,   268,   269,   270,   271,   272,
       0,     0,     0,     0,     0,     0,   273,   165,     0,     0,
       0,     0,     0,     0,   367,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,     0,     0,   193,     0,     0,   194,   195,   196,   197,
       0,   198,     0,     0,   199,   200,   201,   202,   203,   204,
       0,     0,     0,     0,     0,   205,   206,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,   209,   210,
     211,   212,   213,     0,     0,     0,   214,     0,   215,   216,
     217,   218,   219,   220,     0,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,     0,     0,     0,     0,     0,     0,   246,   247,   248,
     249,     0,     0,   250,   251,   252,   253,     0,     0,   254,
       0,     0,   255,   256,     0,   257,     0,   258,   259,   162,
     163,   164,   260,     0,   261,   262,     0,   263,   264,     0,
     265,   266,   267,     0,   268,   269,   270,   271,   272,     0,
       0,     0,     0,     0,     0,   273,   165,     0,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
       0,     0,   193,     0,     0,   194,   195,   196,   197,     0,
     198,     0,     0,   199,   200,   201,   202,   203,   204,     0,
       0,     0,     0,     0,   205,   206,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208,   209,   210,   211,
     212,   213,     0,     0,     0,   214,     0,   215,   216,   217,
     218,   219,   220,     0,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,     0,   244,   245,
       0,     0,     0,     0,     0,     0,   246,   247,   248,   249,
       0,     0,   250,   251,   252,   253,     0,     0,   254,     0,
       0,   255,   256,     0,   257,     0,   258,   259,   162,   163,
     164,   260,     0,   261,   262,     0,   263,   264,     0,   265,
     266,   267,     0,   268,   269,   270,   271,   272,     0,     0,
       0,     0,     0,     0,   273,   165,     0,     0,     0,     0,
       0,     0,   372,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,   193,     0,     0,   194,   195,   196,   197,     0,   198,
       0,     0,   199,   200,   201,   202,   203,   204,     0,     0,
       0,     0,     0,   205,   206,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,   209,   210,   211,   212,
     213,     0,     0,     0,   214,     0,   215,   216,   217,   218,
     219,   220,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,     0,
       0,     0,     0,     0,     0,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,     0,     0,   254,     0,     0,
     255,   256,     0,   257,     0,   258,   259,   162,   163,   164,
     260,     0,   261,   262,     0,   263,   264,     0,   265,   266,
     267,     0,   268,   269,   270,   271,   272,     0,     0,     0,
       0,     0,     0,   273,   165,     0,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,   206,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,   209,   210,   211,   212,   213,
       0,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,   162,   163,   164,   260,
       0,   261,   262,     0,   263,   264,     0,   265,   266,   267,
       0,   268,   269,   270,   271,   272,     0,     0,     0,     0,
       0,     0,   273,   165,     0,     0,     0,     0,     0,     0,
     377,     0,     0,     0,     0,     0,     0,     0,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     0,     0,     0,   193,
       0,     0,   194,   195,   196,   197,     0,   198,     0,     0,
     199,   200,   201,   202,   203,   204,     0,     0,     0,     0,
       0,   205,   206,   207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,   209,   210,   211,   212,   213,     0,
       0,     0,   214,     0,   215,   216,   217,   218,   219,   220,
       0,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,     0,   244,   245,     0,     0,     0,
       0,     0,     0,   246,   247,   248,   249,     0,     0,   250,
     251,   252,   253,     0,     0,   254,     0,     0,   255,   256,
       0,   257,     0,   258,   259,   162,   163,   164,   260,     0,
     261,   262,     0,   263,   264,     0,   265,   266,   267,     0,
     268,   269,   270,   271,   272,     0,     0,     0,     0,     0,
       0,   273,   165,     0,     0,     0,     0,     0,     0,   379,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     0,     0,     0,   193,     0,
       0,   194,   195,   196,   197,     0,   198,     0,     0,   199,
     200,   201,   202,   203,   204,     0,     0,     0,     0,     0,
     205,   206,   207,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   208,   209,   210,   211,   212,   213,     0,     0,
       0,   214,     0,   215,   216,   217,   218,   219,   220,     0,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,     0,   244,   245,     0,     0,     0,     0,
       0,     0,   246,   247,   248,   249,     0,     0,   250,   251,
     252,   253,     0,     0,   254,     0,     0,   255,   256,     0,
     257,     0,   258,   259,   162,   163,   164,   260,     0,   261,
     262,     0,   263,   264,     0,   265,   266,   267,     0,   268,
     269,   270,   271,   272,     0,     0,     0,     0,     0,     0,
     273,   165,     0,     0,     0,     0,     0,     0,   381,     0,
       0,     0,     0,     0,     0,     0,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     0,     0,     0,   193,     0,     0,
     194,   195,   196,   197,     0,   198,     0,     0,   199,   200,
     201,   202,   203,   204,     0,     0,     0,     0,     0,   205,
     206,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   208,   209,   210,   211,   212,   213,     0,     0,     0,
     214,     0,   215,   216,   217,   218,   219,   220,     0,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,     0,   244,   245,     0,     0,     0,     0,     0,
       0,   246,   247,   248,   249,     0,     0,   250,   251,   252,
     253,     0,     0,   254,     0,     0,   255,   256,     0,   257,
       0,   258,   259,   162,   163,   164,   260,     0,   261,   262,
       0,   263,   264,     0,   265,   266,   267,     0,   268,   269,
     270,   271,   272,     0,     0,     0,     0,     0,     0,   273,
     165,     0,     0,     0,     0,     0,     0,   383,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     0,     0,     0,   193,     0,     0,   194,
     195,   196,   197,     0,   198,     0,     0,   199,   200,   201,
     202,   203,   204,     0,     0,     0,     0,     0,   205,   206,
     207,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,   209,   210,   211,   212,   213,     0,     0,     0,   214,
       0,   215,   216,   217,   218,   219,   220,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   244,   245,     0,     0,     0,     0,     0,     0,
     246,   247,   248,   249,     0,     0,   250,   251,   252,   253,
       0,     0,   254,     0,     0,   255,   256,     0,   257,     0,
     258,   259,   162,   163,   164,   260,     0,   261,   262,     0,
     263,   264,     0,   265,   266,   267,     0,   268,   269,   270,
     271,   272,     0,     0,     0,     0,     0,     0,   273,   165,
       0,     0,     0,     0,     0,     0,   559,     0,     0,     0,
       0,     0,     0,     0,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     0,     0,     0,   193,     0,     0,   194,   195,
     196,   197,     0,   198,     0,     0,   199,   200,   201,   202,
     203,   204,     0,     0,     0,     0,     0,   205,   206,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
     209,   210,   211,   212,   213,     0,     0,     0,   214,     0,
     215,   216,   217,   218,   219,   220,     0,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,     0,     0,     0,     0,     0,     0,   246,
     247,   248,   249,     0,     0,   250,   251,   252,   253,     0,
       0,   254,     0,     0,   255,   256,     0,   257,     0,   258,
     259,   162,   163,   164,   260,     0,   261,   262,     0,   263,
     264,     0,   265,   266,   267,     0,   268,   269,   270,   271,
     272,     0,     0,     0,     0,     0,     0,   273,   165,     0,
       0,     0,     0,     0,     0,   561,     0,     0,     0,     0,
       0,     0,     0,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     0,     0,     0,   193,     0,     0,   194,   195,   196,
     197,     0,   198,     0,     0,   199,   200,   201,   202,   203,
     204,     0,     0,     0,     0,     0,   205,   206,   207,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,   209,
     210,   211,   212,   213,     0,     0,     0,   214,     0,   215,
     216,   217,   218,   219,   220,     0,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,     0,
     244,   245,     0,     0,     0,     0,     0,     0,   246,   247,
     248,   249,     0,     0,   250,   251,   252,   253,     0,     0,
     254,     0,     0,   255,   256,     0,   257,     0,   258,   259,
     162,   163,   164,   260,     0,   261,   262,     0,   263,   264,
       0,   265,   266,   267,     0,   268,   269,   270,   271,   272,
       0,     0,     0,     0,     0,     0,   273,   165,     0,     0,
       0,     0,     0,     0,   563,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       0,     0,     0,   193,     0,     0,   194,   195,   196,   197,
       0,   198,     0,     0,   199,   200,   201,   202,   203,   204,
       0,     0,     0,     0,     0,   205,   206,   207,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,   209,   210,
     211,   212,   213,     0,     0,     0,   214,     0,   215,   216,
     217,   218,   219,   220,     0,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,     0,   244,
     245,     0,     0,     0,     0,     0,     0,   246,   247,   248,
     249,     0,     0,   250,   251,   252,   253,     0,     0,   254,
       0,     0,   255,   256,     0,   257,     0,   258,   259,   162,
     163,   164,   260,     0,   261,   262,     0,   263,   264,     0,
     265,   266,   267,     0,   268,   269,   270,   271,   272,     0,
       0,     0,     0,     0,     0,   273,   165,     0,     0,     0,
       0,     0,     0,   565,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     0,
       0,     0,   193,     0,     0,   194,   195,   196,   197,     0,
     198,     0,     0,   199,   200,   201,   202,   203,   204,     0,
       0,     0,     0,     0,   205,   206,   207,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208,   209,   210,   211,
     212,   213,     0,     0,     0,   214,     0,   215,   216,   217,
     218,   219,   220,     0,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,     0,   244,   245,
       0,     0,     0,     0,     0,     0,   246,   247,   248,   249,
       0,     0,   250,   251,   252,   253,     0,     0,   254,     0,
       0,   255,   256,     0,   257,     0,   258,   259,   162,   163,
     164,   260,     0,   261,   262,     0,   263,   264,     0,   265,
     266,   267,     0,   268,   269,   270,   567,   272,     0,     0,
       0,     0,     0,     0,   273,   165,     0,     0,     0,     0,
       0,     0,   274,     0,     0,     0,     0,     0,     0,     0,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,   193,     0,     0,   194,   195,   196,   197,     0,   198,
       0,     0,   199,   200,   201,   202,   203,   204,     0,     0,
       0,     0,     0,   205,   206,   207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,   209,   210,   211,   212,
     213,     0,     0,     0,   214,     0,   215,   216,   217,   218,
     219,   220,     0,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,     0,   244,   245,     0,
       0,     0,     0,     0,     0,   246,   247,   248,   249,     0,
       0,   250,   251,   252,   253,     0,     0,   254,     0,     0,
     255,   256,     0,   257,     0,   258,   259,   162,   163,   164,
     260,     0,   261,   262,     0,   263,   264,     0,   265,   266,
     267,     0,   268,   269,   270,  1100,   272,     0,     0,     0,
       0,     0,     0,   273,   165,     0,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,     0,     0,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     0,     0,     0,
     193,     0,     0,   194,   195,   196,   197,     0,   198,     0,
       0,   199,   200,   201,   202,   203,   204,     0,     0,     0,
       0,     0,   205,   206,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,   209,   210,   211,   212,   213,
       0,     0,     0,   214,     0,   215,   216,   217,   218,   219,
     220,     0,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,     0,   244,   245,     0,     0,
       0,     0,     0,     0,   246,   247,   248,   249,     0,     0,
     250,   251,   252,   253,     0,     0,   254,     0,     0,   255,
     256,     0,   257,     0,   258,   259,     0,     0,     0,   260,
       0,   261,   262,     0,   263,   264,     0,   265,   266,   267,
       0,   268,   269,   270,  1186,   272,     0,     0,     0,     0,
       0,     0,   273,     0,     0,     0,     1,     2,     0,     0,
     274,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,     0,     0,    22,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,     0,     0,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,    35,     0,     0,     0,     0,    36,   506,
     507,   508,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,     0,     0,     0,
       0,    51,    52,    53,     0,    54,     0,     0,     0,     0,
       0,     0,    55,     0,   512,   513,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,    57,
      58,    59,    60,    61,    62,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,    65,    66,     0,    67,    68,
       0,     0,    69,     0,    70,     0,     0,    71,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    73,    74,     1,     2,    75,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,    15,    16,    17,    18,    19,
      20,    21,   514,   515,    22,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,     0,     0,    32,
      33,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    35,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,   509,   510,   511,    42,    43,    44,
      45,    46,    47,    48,    49,    50,   512,   513,     0,     0,
       0,     0,    51,    52,    53,     0,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
      57,    58,    59,    60,    61,    62,     0,     0,     0,     0,
      63,    64,     0,     0,     0,     0,    65,    66,     0,    67,
      68,     0,     0,    69,     0,    70,     0,     0,    71,     1,
       2,     0,    72,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,    73,    74,     0,     0,
       0,     0,     0,    21,   514,   515,    22,    23,     0,    24,
      25,    26,    27,    28,    29,     0,   622,    31,     0,     0,
       0,    32,    33,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   924,     0,
       0,   925,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    35,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,   509,   510,   511,    42,
      43,    44,    45,    46,    47,    48,    49,    50,   512,   513,
       0,     0,     0,     0,    51,    52,    53,     0,    54,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,    57,    58,    59,    60,    61,    62,   506,   507,
     508,     0,    63,    64,     0,     0,     0,     0,    65,    66,
       0,    67,    68,     0,     0,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,   509,   510,   511,     0,     0,     0,     0,    73,    74,
       0,     0,     0,   512,   513,     0,   514,   515,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     926,     0,     0,   927,   509,   510,   511,     0,     0,   506,
     507,   508,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,   509,   510,
     511,   514,   515,     0,     0,     0,   506,   507,   508,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   516,   517,   518,   519,   520,   521,   522,
       0,   523,     0,     0,     0,     0,     0,     0,  1065,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,     0,     0,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,     0,
       0,  1070,   514,   515,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,   514,   515,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,   506,   507,   508,     0,     0,     0,  1072,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,     0,     0,  1073,     0,     0,     0,   514,
     515,     0,     0,     0,     0,     0,   509,   510,   511,     0,
       0,   506,   507,   508,     0,     0,     0,     0,   512,   513,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,     0,     0,     0,  1093,     0,     0,     0,
       0,     0,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   512,   513,     0,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,     0,
       0,  1094,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   512,   513,
       0,     0,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,     0,     0,  1095,   514,   515,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,     0,
       0,  1096,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   512,   513,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   512,   513,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   778,   747,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     780,   747,   506,   507,   508,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   509,   510,   511,   516,   517,
     518,   519,   520,   521,   522,     0,   523,   512,   513,     0,
     782,   747,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,   506,
     507,   508,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,   506,   507,   508,   787,   747,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     789,   747,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   514,   515,     0,     0,     0,
       0,     0,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,   516,   517,   518,
     519,   520,   521,   522,     0,   523,     0,     0,     0,   791,
     747,     0,     0,     0,     0,     0,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   794,   747,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     796,   747,   514,   515,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   798,   747,   514,   515,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,   506,   507,   508,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   805,   747,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,   513,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
     506,   507,   508,     0,   514,   515,     0,     0,     0,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,   509,   523,     0,     0,     0,   807,   747,
     514,   515,   509,   510,   511,   512,   513,   506,   507,   508,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   809,   747,   514,   515,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   512,   513,   506,   507,   508,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     813,   747,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   815,   747,     0,   509,   510,   511,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   512,
     513,     0,     0,   514,   515,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,     0,     0,   506,
     507,   508,     0,     0,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   817,   747,     0,     0,
     514,   515,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,   506,
     507,   508,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   819,   747,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,   514,   515,     0,
     512,   513,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,   516,
     517,   518,   519,   520,   521,   522,     0,   523,     0,     0,
       0,   822,   747,     0,     0,     0,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,     0,     0,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   824,   747,   514,   515,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   512,   513,     0,   506,   507,   508,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   826,   747,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   828,   747,   514,   515,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   832,   747,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   834,   747,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   512,   513,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   837,   747,   514,   515,     0,     0,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     944,   747,     0,     0,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,   506,
     507,   508,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   945,   747,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   946,   747,
     514,   515,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   947,   747,   514,   515,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,   506,   507,   508,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
    1177,  1178,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,   747,   514,   515,
     509,   510,   511,     0,     0,   506,   507,   508,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   855,   514,   515,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     512,   513,   506,   507,   508,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,   856,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   858,     0,   509,   510,   511,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   512,   513,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,     0,     0,     0,     0,     0,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,   867,     0,     0,   514,   515,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,   506,   507,   508,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   868,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,   514,   515,     0,   512,   513,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,   516,   517,   518,
     519,   520,   521,   522,     0,   523,     0,     0,     0,     0,
     869,     0,     0,     0,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,     0,     0,     0,     0,     0,   506,   507,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,   870,   514,   515,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   512,   513,     0,   506,   507,   508,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   871,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,   872,   514,   515,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   873,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,     0,     0,     0,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   506,
     507,   508,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,   874,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     512,   513,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   875,   514,   515,     0,     0,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,   876,
       0,     0,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,   506,   507,   508,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   877,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,   878,   514,   515,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   879,   514,   515,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,   880,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,     0,     0,     0,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,   881,   514,   515,   509,   510,
     511,     0,     0,   506,   507,   508,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   883,   514,   515,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   512,   513,
     506,   507,   508,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,   884,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   886,     0,   509,   510,   511,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   512,   513,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
       0,     0,     0,     0,     0,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   887,     0,     0,   514,   515,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,   506,   507,   508,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   890,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,   514,   515,     0,   512,   513,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,     0,   891,     0,
       0,     0,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
       0,     0,     0,     0,     0,   506,   507,   508,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   892,   514,   515,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     512,   513,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,   893,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   894,   514,   515,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,   895,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
       0,     0,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   507,   508,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,   898,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   512,   513,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   899,   514,   515,     0,     0,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,   900,     0,     0,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,     0,     0,     0,   506,   507,   508,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   901,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,   902,   514,   515,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,   905,   514,   515,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1012,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
       0,     0,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,  1023,   514,   515,   509,   510,   511,     0,
       0,   506,   507,   508,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1033,
     514,   515,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   512,   513,   506,   507,
     508,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,  1052,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1053,
       0,   509,   510,   511,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   512,   513,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,     0,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,  1055,     0,     0,   514,   515,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1061,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,   514,   515,     0,   512,   513,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,   516,   517,   518,   519,   520,   521,   522,
       0,   523,     0,     0,     0,     0,  1074,     0,     0,     0,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,     0,     0,   506,   507,   508,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,  1077,   514,   515,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   512,   513,
       0,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1078,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,  1084,   514,   515,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1107,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   506,   507,   508,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,  1108,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   512,   513,   509,   510,
     511,   506,   507,   508,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1109,
     514,   515,     0,     0,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,  1110,     0,     0,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
       0,     0,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1139,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,  1150,   514,   515,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1151,
     514,   515,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,   506,
     507,   508,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,  1153,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,  1161,   514,   515,   509,   510,   511,     0,     0,   506,
     507,   508,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1164,   514,   515,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   512,   513,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,  1165,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1175,     0,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1176,
       0,     0,   514,   515,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1179,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,   514,
     515,     0,   512,   513,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,     0,  1183,     0,     0,     0,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,     0,
       0,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1201,
     514,   515,     0,     0,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   512,   513,     0,   506,
     507,   508,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,  1206,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1207,
     514,   515,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,     0,  1213,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   506,   507,   508,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,     0,  1214,
       0,     0,   509,   510,   511,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   512,   513,   509,   510,   511,   506,
     507,   508,     0,     0,     0,     0,     0,     0,   512,   513,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1215,   514,   515,
       0,     0,   509,   510,   511,   506,   507,   508,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,  1216,     0,     0,     0,     0,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,     0,     0,
       0,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,     0,  1217,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
       0,  1223,   514,   515,   509,   510,   511,   506,   507,   508,
       0,     0,     0,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   536,     0,   514,   515,
     509,   510,   511,   506,   507,   508,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,   506,   507,   508,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   538,     0,     0,     0,   509,   510,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   512,   513,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   540,     0,
     514,   515,   509,   510,   511,     0,     0,   506,   507,   508,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   545,     0,   514,   515,     0,     0,
     509,   510,   511,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   512,   513,   506,   507,   508,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     547,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   549,     0,     0,   509,   510,   511,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   512,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   506,   507,   508,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   552,   509,   510,   511,
     514,   515,     0,     0,   506,   507,   508,     0,     0,   512,
     513,     0,     0,     0,     0,     0,     0,     0,   506,   507,
     508,     0,   516,   517,   518,   519,   520,   521,   522,     0,
     523,     0,     0,     0,   554,     0,     0,   509,   510,   511,
       0,     0,     0,     0,     0,     0,     0,   514,   515,   512,
     513,   509,   510,   511,   506,   507,   508,     0,     0,     0,
       0,     0,     0,   512,   513,     0,     0,     0,     0,   516,
     517,   518,   519,   520,   521,   522,     0,   523,     0,     0,
       0,   558,     0,     0,     0,     0,     0,   509,   510,   511,
       0,     0,     0,   506,   507,   508,     0,     0,     0,   512,
     513,     0,     0,     0,     0,     0,     0,   514,   515,   506,
     507,   508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   509,   510,   511,   516,
     517,   518,   519,   520,   521,   522,     0,   523,   512,   513,
       0,   569,   509,   510,   511,     0,     0,   514,   515,     0,
       0,     0,     0,     0,   512,   513,     0,     0,     0,     0,
       0,   514,   515,     0,     0,     0,   506,   507,   508,   516,
     517,   518,   519,   520,   521,   522,     0,   523,     0,     0,
       0,   571,     0,   516,   517,   518,   519,   520,   521,   522,
       0,   523,     0,     0,     0,   573,     0,   514,   515,   509,
     510,   511,     0,     0,     0,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   516,
     517,   518,   519,   520,   521,   522,     0,   523,     0,     0,
       0,   577,     0,     0,     0,     0,   514,   515,   509,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,   513,   514,   515,   506,   507,   508,     0,   516,   517,
     518,   519,   520,   521,   522,     0,   523,     0,     0,     0,
     579,   506,   507,   508,   516,   517,   518,   519,   520,   521,
     522,     0,   523,     0,     0,     0,   581,   509,   510,   511,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   512,
     513,     0,     0,     0,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   583,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   584,   509,   510,   511,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   512,   513,   514,   515,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,   516,
     517,   518,   519,   520,   521,   522,     0,   523,     0,     0,
       0,   586,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   590,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   595,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,   598,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,   601,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   607,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   786,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   514,   515,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   853,   509,   510,   511,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   512,   513,     0,   854,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   888,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   889,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,   930,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,   932,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   935,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   937,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   514,   515,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,   943,   509,   510,   511,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   512,   513,     0,   951,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   953,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,   955,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,   957,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,   961,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   962,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,   964,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   514,   515,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,  1013,   509,   510,   511,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   512,   513,     0,  1015,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1017,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1021,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,  1027,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,  1035,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1038,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1040,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   514,   515,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,  1042,   509,   510,   511,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   512,   513,     0,  1045,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1047,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1050,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,  1075,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,  1076,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1083,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1099,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   514,   515,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,  1102,   509,   510,   511,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   512,   513,     0,  1104,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1118,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1121,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,  1140,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,  1143,   509,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,   506,   507,   508,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1147,   509,   510,   511,     0,     0,   514,
     515,   506,   507,   508,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1157,   509,   510,   511,     0,   514,   515,
     506,   507,   508,     0,     0,     0,   512,   513,     0,     0,
       0,     0,     0,     0,   514,   515,   506,   507,   508,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,  1162,   509,   510,   511,   516,   517,   518,   519,
     520,   521,   522,     0,   523,   512,   513,     0,  1180,   509,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   513,     0,   514,   515,   506,   507,   508,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1188,   509,
     510,   511,     0,     0,   514,   515,   506,   507,   508,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523,     0,     0,     0,  1193,   509,
     510,   511,     0,   514,   515,   506,   507,   508,     0,     0,
       0,   512,   513,     0,     0,     0,     0,     0,     0,   514,
     515,   506,   507,   508,     0,   516,   517,   518,   519,   520,
     521,   522,     0,   523,     0,     0,     0,  1195,   509,   510,
     511,   516,   517,   518,   519,   520,   521,   522,     0,   523,
     512,   513,     0,  1203,   509,   510,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   512,   513,     0,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1208,     0,     0,     0,     0,     0,   514,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,   517,   518,   519,   520,   521,   522,     0,   523,
       0,     0,     0,  1211,     0,     0,     0,     0,   514,   515,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,     0,     0,     0,     0,
     516,   517,   518,   519,   520,   521,   522,     0,   523,     0,
       0,     0,     0,     0,     0,     0,   516,   517,   518,   519,
     520,   521,   522,     0,   523
};

static const yytype_int16 yycheck[] =
{
       1,    35,     3,     4,     5,   202,     7,     8,   209,    10,
     202,   202,   203,    14,    87,    88,    17,   202,   203,    20,
     221,   218,    23,    24,    25,    26,   202,   224,    87,    88,
      31,   202,   224,    34,    35,    36,    37,   202,    39,   202,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      42,    43,    44,    54,    87,    88,    57,   216,   217,   209,
     202,   203,   221,   218,    65,    66,    67,    68,     3,     4,
      71,   221,   218,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   218,    21,    22,    23,    24,
      25,    26,    27,    19,   218,    30,    31,    23,    33,    34,
      35,    36,    37,    38,   219,    40,    41,   222,   220,   221,
      45,    46,   185,   186,   216,   217,   216,   217,   202,   221,
     218,   221,   216,   217,   216,   217,     0,   221,   216,   221,
     217,    42,    43,    44,   202,   203,    20,   210,   211,   212,
     213,   219,   215,   219,    79,    80,   222,   219,   220,   218,
      85,   210,   211,   212,   213,   218,   215,   219,   220,   202,
      95,    96,    97,    98,    99,   216,   217,   221,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   216,   217,   212,
     213,   218,   215,   118,   119,   120,   218,   122,   216,   217,
     216,   217,   193,   218,   129,    42,    43,    44,   220,   221,
     220,   221,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   207,   208,   209,   153,   211,
     218,   156,   157,   158,   159,   160,   161,   219,   220,   218,
     218,   166,   167,   218,   218,   218,   218,   172,   173,   218,
     175,   176,   218,   218,   179,   218,   181,   218,   218,   184,
     218,   218,   218,   188,   218,   218,   218,   218,   218,   260,
     218,   218,   218,   218,   218,   218,   218,   202,   203,   218,
     218,   206,   273,   274,   218,   218,   218,   218,   218,   218,
     281,   216,   218,   284,   218,   286,    42,    43,    44,   290,
     218,   292,   221,   294,   218,   296,   207,   208,   209,   218,
     211,   218,   303,   218,   218,   306,   218,   209,   309,   218,
     311,   218,   223,   218,   218,   218,   317,   218,   319,   320,
     321,   322,   218,   218,   325,   218,   218,   328,   218,   218,
     331,   218,   333,   218,   220,   218,   218,   218,   339,   218,
     341,   218,   218,   344,   211,   346,   218,   218,   218,   218,
     351,   385,   218,   218,   218,   356,   218,   218,   218,   218,
     207,   208,   209,   218,   211,   218,   367,   218,   369,   218,
     218,   372,   219,   220,   375,   218,   377,   218,   379,   218,
     381,   218,   383,   218,   385,   386,   220,   388,   220,   220,
     220,   220,   220,   220,   215,   396,   397,   398,   399,   400,
     220,   220,   220,   220,    42,    43,    44,   220,   409,   410,
     220,   412,   413,   414,   415,   416,   417,   418,   220,   420,
     421,   422,   423,   424,   425,   220,   220,   428,   429,   430,
     431,   220,   220,   220,   220,   220,   220,   220,   220,   440,
     441,   442,   443,   444,   445,   446,   447,   220,   220,   220,
     220,   207,   208,   209,   220,   211,   220,   220,   220,   220,
     220,   220,   220,   219,   220,   220,   220,   220,   220,   220,
      42,    43,    44,   220,   220,   220,   220,   220,   220,   480,
     481,   482,   220,   220,   220,   220,   487,   488,   220,   220,
     218,   492,   225,   218,   218,   218,   226,   226,   209,   221,
     501,   502,    42,    43,    44,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   221,   225,   209,   222,   222,   222,   222,
     531,   209,   533,   534,   209,   536,   202,   538,   220,   540,
     390,    78,   786,   983,   545,    -1,   547,   393,   549,   550,
     392,    -1,    -1,    -1,    -1,    -1,    -1,   558,   559,    -1,
     561,    -1,   563,    -1,   565,     5,     6,     7,   569,   207,
     208,   209,   573,   211,   575,    -1,   577,    -1,   579,    -1,
      -1,   219,   583,   584,    -1,   586,    -1,    -1,    -1,   590,
      -1,    -1,    32,    -1,   595,    -1,    -1,    -1,   599,    42,
      43,    44,    -1,    -1,    -1,    -1,   607,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,   207,   208,   209,    78,   211,
      -1,    81,    82,    83,    84,    -1,    86,   219,   220,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,    -1,    42,    43,    44,   207,   208,   209,
      -1,   211,   112,   113,   114,   115,   116,   117,    -1,   219,
     220,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    42,    43,    44,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,    42,    43,    44,   187,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,   197,    -1,   199,
     200,   201,   202,   203,   207,   208,   209,    -1,   211,    -1,
     210,    -1,    -1,    -1,    -1,    -1,   219,   778,   218,   780,
      -1,   782,    -1,    -1,   224,   786,   787,    -1,   789,    -1,
     791,    -1,    -1,    -1,    -1,    -1,    -1,   798,    -1,    -1,
      -1,    -1,    -1,    -1,   805,    -1,    -1,    -1,   809,    -1,
     811,    -1,   813,    -1,   815,    -1,   817,    -1,   819,    -1,
      -1,   822,    -1,   824,    -1,   826,    -1,    -1,   829,   207,
     208,   209,    -1,   211,    -1,    -1,   837,    -1,    -1,    -1,
      -1,   219,   843,    -1,    42,    43,    44,    -1,   849,    -1,
     851,   852,   853,   854,    -1,    -1,    -1,    -1,    -1,   860,
     861,   862,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,    -1,   211,    -1,   888,   889,    42,
      43,    44,    -1,   219,   220,    -1,    -1,    -1,    -1,    -1,
      -1,   935,    -1,    -1,    42,    43,    44,    -1,    -1,   207,
     208,   209,    -1,   211,    -1,    -1,   917,    -1,    42,    43,
      44,   922,   220,   924,    -1,   926,    -1,    -1,    -1,   930,
      -1,   932,     1,     2,   935,    -1,   937,    -1,   939,    -1,
      -1,    -1,   943,   944,   945,   946,   947,    -1,    -1,    -1,
     951,    -1,   953,    -1,    23,    24,   957,    -1,    -1,    -1,
     961,   962,    -1,   964,    -1,    -1,    -1,    36,   969,    38,
      39,    40,    -1,    -1,    -1,    -1,    -1,    -1,   979,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    60,    61,    42,    43,    44,    -1,    66,    67,    -1,
      69,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,   207,
     208,   209,  1013,   211,  1015,    -1,  1017,    28,    -1,    -1,
    1021,   219,   220,    -1,    -1,    -1,  1027,    -1,    -1,    -1,
      -1,    42,    43,    44,  1035,    -1,    -1,  1038,   207,   208,
     209,  1042,   211,    -1,  1045,    -1,  1047,    -1,    -1,  1050,
     219,   220,    -1,  1054,   207,   208,   209,    -1,   211,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,   220,  1102,   207,
     208,   209,    -1,   211,  1075,  1076,    87,    88,  1079,    -1,
    1081,   219,  1083,   207,   208,   209,  1087,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   219,   220,    -1,  1099,    -1,
      -1,  1102,    -1,  1104,  1105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1115,    -1,    -1,  1118,    -1,    -1,
    1121,    -1,    -1,    -1,    -1,    -1,  1127,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1140,
      -1,    -1,  1143,    -1,    -1,    -1,  1147,    -1,    -1,    -1,
      42,    43,    44,  1154,  1188,    -1,  1157,    -1,   207,   208,
     209,  1162,   211,    -1,    -1,    -1,  1167,    -1,    -1,    -1,
    1171,   220,    -1,    -1,   185,   186,  1177,    -1,    -1,  1180,
    1181,    42,    43,    44,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,  1195,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   274,    -1,    29,    -1,    -1,
    1211,    -1,   281,    -1,    -1,   284,    -1,   286,    -1,    -1,
      42,    43,    44,   292,    -1,   294,    -1,   296,    -1,    42,
      43,    44,    -1,    -1,   303,    -1,    -1,   306,    -1,    -1,
     309,    -1,   311,    -1,    -1,    42,    43,    44,   317,    -1,
      42,    43,    44,    75,    76,    77,   325,    -1,    -1,   328,
      -1,    -1,   331,    -1,   333,    87,    88,    42,    43,    44,
     339,    -1,   341,    -1,    -1,   344,    -1,   346,    -1,    -1,
      -1,    -1,   351,    -1,    -1,    -1,    -1,   356,    -1,    -1,
      -1,    -1,   207,   208,   209,    -1,   211,    -1,   367,    -1,
     369,    -1,    -1,   372,   219,   220,   375,    -1,   377,    -1,
     379,    -1,   381,    -1,   383,   207,   208,   209,   387,   211,
      -1,    -1,    42,    43,    44,    -1,    -1,   219,   220,    -1,
     399,    -1,   401,   402,   403,   404,   405,   406,   407,   408,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,    -1,
     211,    -1,   207,   208,   209,    -1,   211,    -1,   427,   220,
     429,    -1,    -1,   185,   186,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,   448,
      -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   207,   208,   209,    -1,   211,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   220,    -1,   478,
     207,   208,   209,    -1,   211,   207,   208,   209,    -1,   211,
      -1,   490,   491,   220,    -1,   494,   495,   496,   497,    -1,
      -1,    -1,   207,   208,   209,    -1,   211,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,   220,    -1,    -1,    -1,    -1,
      -1,   520,    -1,    -1,    -1,    -1,   525,   526,   527,   528,
     529,   530,   531,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   552,    -1,   554,    -1,   207,   208,   209,
     559,   211,   561,    -1,   563,    -1,   565,    -1,    -1,    -1,
     220,    -1,   571,    -1,    -1,    -1,    -1,     5,     6,     7,
      -1,    -1,   581,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   598,
      -1,    -1,   601,    -1,    32,    -1,   605,   207,   208,   209,
     609,   211,    -1,    -1,    -1,    -1,    -1,    -1,   617,   219,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,   207,   208,   209,
      -1,   211,    -1,    -1,   112,   113,   114,   115,   116,   117,
     220,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    42,    43,
      44,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,    42,    43,    44,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    42,    43,    44,    -1,
      -1,    -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     218,   219,    -1,    -1,    -1,   794,    -1,   796,   226,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   807,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,   817,    -1,
      -1,    87,    88,    42,    43,    44,    -1,    -1,    -1,   828,
      -1,    -1,    -1,   832,    -1,   834,   835,    -1,    -1,    -1,
     839,    -1,   841,   842,    -1,    -1,   845,   846,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    75,    76,    77,    -1,
      -1,    42,    43,    44,    -1,    -1,   865,   866,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,   207,   208,   209,    -1,   211,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   904,   220,   906,   907,   908,
     909,   910,    -1,   912,    75,    76,    77,    -1,    -1,   185,
     186,   207,   208,   209,    -1,   211,    87,    88,    -1,    -1,
      42,    43,    44,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,   951,    -1,    -1,    -1,   955,   223,   957,    -1,
      -1,    -1,    -1,    75,    76,    77,   185,   186,    -1,    -1,
      -1,    42,    43,    44,   973,    87,    88,    -1,    -1,    -1,
      42,    43,    44,    -1,   983,    -1,   985,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    42,    43,    44,
     219,    -1,    -1,   222,    75,    76,    77,    42,    43,    44,
      -1,    -1,    42,    43,    44,    -1,    87,    88,   207,   208,
     209,    -1,   211,    -1,   185,   186,   207,   208,   209,    -1,
     211,   220,    42,    43,    44,    -1,  1035,    -1,    -1,   220,
      -1,  1040,    -1,  1042,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   207,   208,   209,   219,   211,
    1059,   222,    -1,    -1,  1063,  1064,    -1,   219,  1067,  1068,
      -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1086,    -1,    -1,
    1089,    -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    42,    43,    44,   219,    -1,    -1,
     222,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,  1118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1130,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,   207,   208,   209,   219,   211,
      -1,   222,    -1,    -1,    -1,    -1,    -1,   219,  1157,    -1,
      -1,    -1,   207,   208,   209,    -1,   211,    -1,     5,     6,
       7,    -1,   207,   208,   209,   220,   211,   207,   208,   209,
      -1,   211,    -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,
     220,    -1,    -1,    -1,  1193,    32,    -1,   207,   208,   209,
      -1,   211,    42,    43,    44,    -1,    -1,    -1,    -1,  1208,
     220,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,   207,
     208,   209,    -1,   211,    -1,   112,   113,   114,   115,   116,
     117,    -1,   220,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,   199,   200,   201,   202,   203,   207,   208,   209,
      -1,   211,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,
     220,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,   191,   192,   193,   194,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    42,    43,    44,    -1,    -1,   224,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,   207,   208,   209,    -1,
     211,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,   220,
      42,    43,    44,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
     199,   200,   201,   202,   203,    -1,   207,   208,   209,    -1,
     211,   210,    32,    -1,    -1,    -1,    -1,    -1,   219,   218,
      42,    43,    44,    -1,    -1,   224,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,   207,   208,   209,    -1,   211,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,   220,    42,
      43,    44,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,   199,
     200,   201,   202,   203,    -1,   207,   208,   209,    -1,   211,
     210,    32,    -1,    -1,    -1,    -1,    -1,   219,   218,    42,
      43,    44,    -1,    -1,   224,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,   207,   208,   209,    -1,   211,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,   220,    42,    43,
      44,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,   199,   200,
     201,   202,   203,    -1,   207,   208,   209,    -1,   211,   210,
      32,    -1,    -1,    -1,    -1,    -1,   219,   218,    42,    43,
      44,    -1,    -1,   224,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,   207,   208,   209,    -1,   211,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,   220,    42,    43,    44,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,   199,   200,   201,
     202,   203,    -1,   207,   208,   209,    -1,   211,   210,    32,
      -1,    -1,    -1,    -1,    -1,   219,   218,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,   207,   208,   209,    -1,   211,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,   220,    42,    43,    44,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,   199,   200,   201,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,   220,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,   207,   208,   209,    -1,   211,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,   220,    42,    43,    44,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,   199,   200,   201,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
     207,   208,   209,    -1,   211,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,   220,    42,    43,    44,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,   199,   200,   201,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,   207,
     208,   209,    -1,   211,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,   220,    42,    43,    44,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,   199,   200,   201,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,   207,   208,
     209,    -1,   211,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,   220,    42,    43,    44,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,   207,   208,   209,
      -1,   211,   100,   101,   102,    -1,    -1,    -1,    -1,   219,
      -1,    42,    43,    44,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,   207,   208,   209,    -1,
     211,   100,   101,   102,    -1,    -1,    -1,    -1,   219,    -1,
      42,    43,    44,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
     199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,   220,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,   207,   208,   209,    -1,   211,
     100,   101,   102,    -1,    -1,    -1,    -1,   219,    -1,    42,
      43,    44,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,   199,
     200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,   207,   208,   209,    -1,   211,   100,
     101,   102,    -1,    -1,    -1,    -1,   219,    -1,    42,    43,
      44,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,   199,   200,
     201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,   220,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,   207,   208,   209,    -1,   211,   100,   101,
     102,    -1,    -1,    -1,    -1,   219,    -1,    42,    43,    44,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,   199,   200,   201,
     202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,   207,   208,   209,    -1,   211,   100,   101,   102,
      -1,    -1,    -1,    -1,   219,    -1,    42,    43,    44,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,   199,   200,   201,   202,
     203,    -1,   207,   208,   209,    -1,   211,   210,    32,    -1,
      -1,    -1,    -1,    -1,   219,   218,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,   207,   208,   209,    -1,   211,   100,   101,   102,    -1,
      -1,    -1,    -1,   219,    -1,    42,    43,    44,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,   199,   200,   201,   202,   203,
      -1,   207,   208,   209,    -1,   211,   210,    32,    -1,    -1,
      -1,    -1,    -1,   219,   218,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
     207,   208,   209,    -1,   211,   100,   101,   102,    -1,    -1,
      -1,    -1,   219,    -1,    42,    43,    44,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,   199,   200,   201,   202,   203,    -1,
     207,   208,   209,    -1,   211,   210,    32,    -1,    -1,    -1,
      -1,    -1,   219,   218,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,   207,
     208,   209,    -1,   211,   100,   101,   102,    -1,    -1,    -1,
      -1,   219,    -1,    42,    43,    44,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,   199,   200,   201,   202,   203,    -1,   207,
     208,   209,    -1,   211,   210,    32,    -1,    -1,    -1,    -1,
      -1,   219,   218,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,   207,   208,
     209,    -1,   211,   100,   101,   102,    -1,    -1,    -1,    -1,
     219,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,   199,   200,   201,   202,   203,    -1,   207,   208,
     209,    -1,   211,   210,    32,    -1,    -1,    -1,    -1,    -1,
     219,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
     199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,   199,
     200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,   199,   200,
     201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,   199,   200,   201,
     202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,   199,   200,   201,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,   199,   200,   201,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,   199,   200,   201,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,   199,   200,   201,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
     199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,   199,
     200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,   199,   200,
     201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,   199,   200,   201,
     202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,   199,   200,   201,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,   199,   200,   201,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,   199,   200,   201,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,   199,   200,   201,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
     199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,
      -1,   210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,   199,
     200,   201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,
     210,    32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,   199,   200,
     201,   202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,   199,   200,   201,
     202,   203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,   199,   200,   201,   202,
     203,    -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,   199,   200,   201,   202,   203,
      -1,    -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,   199,   200,   201,   202,   203,    -1,
      -1,    -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,   199,   200,   201,   202,   203,    -1,    -1,
      -1,    -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,
      -1,    -1,    -1,   210,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,   100,   101,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,    -1,    -1,    -1,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,    -1,    -1,    -1,    -1,
      -1,    -1,   210,    -1,    -1,    -1,     3,     4,    -1,    -1,
     218,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    31,    -1,    33,    34,    35,    36,
      37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    85,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    -1,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,   120,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,   166,
     167,    -1,    -1,    -1,    -1,   172,   173,    -1,   175,   176,
      -1,    -1,   179,    -1,   181,    -1,    -1,   184,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   202,   203,     3,     4,   206,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,   185,   186,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,    75,    76,    77,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    87,    88,    -1,    -1,
      -1,    -1,   118,   119,   120,    -1,   122,    -1,    -1,    -1,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
     166,   167,    -1,    -1,    -1,    -1,   172,   173,    -1,   175,
     176,    -1,    -1,   179,    -1,   181,    -1,    -1,   184,     3,
       4,    -1,   188,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,   202,   203,    -1,    -1,
      -1,    -1,    -1,    27,   185,   186,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    -1,    40,    41,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    -1,
      -1,   222,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,    75,    76,    77,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    87,    88,
      -1,    -1,    -1,    -1,   118,   119,   120,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,    42,    43,
      44,    -1,   166,   167,    -1,    -1,    -1,    -1,   172,   173,
      -1,   175,   176,    -1,    -1,   179,    -1,   181,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,   202,   203,
      -1,    -1,    -1,    87,    88,    -1,   185,   186,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,    -1,    -1,   222,    75,    76,    77,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    75,    76,
      77,   185,   186,    -1,    -1,    -1,    42,    43,    44,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,   222,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,   222,   185,   186,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,   185,   186,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    42,    43,    44,    -1,    -1,    -1,   222,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    87,    88,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,   222,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    87,    88,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,   222,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,    88,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,   222,   185,   186,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,   222,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    87,    88,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,    88,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,   220,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    87,    88,    -1,
     219,   220,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    42,
      43,    44,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    42,    43,    44,   219,   220,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,   220,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,
     220,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,   220,   185,   186,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,   185,   186,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,   220,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,   185,   186,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    75,   215,    -1,    -1,    -1,   219,   220,
     185,   186,    75,    76,    77,    87,    88,    42,    43,    44,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,   220,   185,   186,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    87,    88,    42,    43,    44,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,   220,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,   220,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,    -1,    -1,
     185,   186,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,   220,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
      -1,   219,   220,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,   185,   186,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    87,    88,    -1,    42,    43,    44,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,   220,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,   185,   186,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,   220,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,   220,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    87,    88,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,   220,   185,   186,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,   220,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    42,
      43,    44,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,   220,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,   220,
     185,   186,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,   220,   185,   186,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    42,    43,    44,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,   220,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   185,   186,
      75,    76,    77,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      87,    88,    42,    43,    44,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,    -1,    -1,   185,   186,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    87,    88,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,
     220,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,   185,   186,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    87,    88,    -1,    42,    43,    44,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,   185,   186,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    42,    43,    44,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   185,   186,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,   185,   186,    75,    76,
      77,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,    88,
      42,    43,    44,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,    -1,    -1,   185,   186,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    42,    43,    44,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    -1,    87,    88,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      87,    88,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,    88,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    42,    43,    44,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,   185,   186,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    75,    76,    77,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     185,   186,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    87,    88,    42,    43,
      44,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,    -1,    -1,   185,   186,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    -1,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,    -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,    88,
      -1,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    87,    88,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     185,   186,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   185,   186,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     185,   186,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    75,    76,    77,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   185,   186,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    87,    88,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,    -1,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    -1,   185,   186,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    -1,    87,    88,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     185,   186,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    87,    88,    -1,    42,
      43,    44,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
     185,   186,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,    -1,   220,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,    -1,   220,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   185,   186,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,    -1,   220,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
      -1,   220,   185,   186,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,    -1,   185,   186,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    -1,
     185,   186,    75,    76,    77,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,    -1,   185,   186,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    87,    88,    42,    43,    44,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,    77,
     185,   186,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,   207,   208,   209,   210,   211,   212,   213,    -1,
     215,    -1,    -1,    -1,   219,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,
      88,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    87,    88,
      -1,   219,    75,    76,    77,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,   185,   186,    -1,    -1,    -1,    42,    43,    44,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
      -1,   219,    -1,   207,   208,   209,   210,   211,   212,   213,
      -1,   215,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,   185,   186,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,   185,   186,    42,    43,    44,    -1,   207,   208,
     209,   210,   211,   212,   213,    -1,   215,    -1,    -1,    -1,
     219,    42,    43,    44,   207,   208,   209,   210,   211,   212,
     213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,   207,
     208,   209,   210,   211,   212,   213,    -1,   215,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    87,    88,    -1,   219,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    87,    88,    -1,   219,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    87,    88,    -1,   219,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    87,    88,    -1,   219,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    87,    88,    -1,   219,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    75,    76,    77,    -1,   185,   186,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    42,    43,    44,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,   219,    75,    76,    77,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    87,    88,    -1,   219,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,    -1,   185,   186,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,
      76,    77,    -1,   185,   186,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,   207,   208,   209,   210,   211,
     212,   213,    -1,   215,    -1,    -1,    -1,   219,    75,    76,
      77,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      87,    88,    -1,   219,    75,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
     207,   208,   209,   210,   211,   212,   213,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   207,   208,   209,   210,
     211,   212,   213,    -1,   215
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    21,    22,    23,    24,    25,
      26,    27,    30,    31,    33,    34,    35,    36,    37,    38,
      40,    41,    45,    46,    79,    80,    85,    95,    96,    97,
      98,    99,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   118,   119,   120,   122,   129,   153,   156,   157,   158,
     159,   160,   161,   166,   167,   172,   173,   175,   176,   179,
     181,   184,   188,   202,   203,   206,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   273,   274,   275,   276,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,     5,     6,     7,    32,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    78,    81,    82,    83,    84,    86,    89,
      90,    91,    92,    93,    94,   100,   101,   102,   112,   113,
     114,   115,   116,   117,   121,   123,   124,   125,   126,   127,
     128,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   154,   155,   162,   163,   164,   165,
     168,   169,   170,   171,   174,   177,   178,   180,   182,   183,
     187,   189,   190,   192,   193,   195,   196,   197,   199,   200,
     201,   202,   203,   210,   218,   317,   319,   202,   203,   317,
     319,   218,   317,   319,   218,   317,   218,   317,   202,   218,
     224,   315,   218,   317,   218,   317,   218,   317,   317,   317,
     317,   202,   202,   218,   317,   319,   218,   317,   319,   218,
     317,   218,   317,   202,   202,    19,    23,   218,   317,   202,
     203,   202,   203,   317,   202,   218,   315,   317,   218,   317,
     319,   218,   317,   218,   319,   317,   319,   319,   317,   218,
     317,   218,   317,   317,   218,   317,   218,   317,   317,   317,
     317,   218,   317,   319,   218,   218,   218,   317,   218,   319,
     317,   319,   218,   319,   319,   218,   202,   218,   317,   218,
     317,   319,   218,   317,   319,   218,   317,   218,   319,   218,
     319,   218,   317,   218,   319,   209,   221,   209,   221,     0,
     216,   231,   217,    20,   219,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   317,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   317,   218,   218,   218,   218,   218,   218,
     218,   221,   221,   317,   317,   319,    42,    43,    44,    75,
      76,    77,    87,    88,   185,   186,   207,   208,   209,   210,
     211,   212,   213,   215,   223,    42,    43,    44,   207,   208,
     209,   211,   223,   221,   221,   317,   219,   317,   219,   317,
     219,   202,   316,   317,   317,   219,   317,   219,   317,   219,
     209,   317,   219,   317,   219,   220,   220,   317,   219,   218,
     317,   218,   317,   218,   317,   218,   317,   202,   317,   219,
     317,   219,   317,   219,   319,   219,   317,   219,   317,   219,
     317,   219,   317,   219,   219,   317,   219,   220,   220,   317,
     219,   220,   220,   220,   317,   219,   317,   319,   219,   219,
     317,   219,   220,   317,   319,   219,   317,   219,   319,   219,
     191,   194,   315,   317,   317,   191,   194,   224,   314,   319,
     317,   228,    40,   243,   242,   202,   203,   220,   317,   317,
     220,   317,   317,   319,   317,   319,   319,   319,   319,   319,
     319,   319,   319,   317,   317,   220,   317,   317,   317,   317,
     317,   317,   317,   220,   317,   317,   317,   317,   317,   317,
     319,   317,   317,   319,   220,   317,   220,   317,   220,   220,
     220,   220,   220,   220,   220,   220,   317,   317,   317,   317,
     317,   317,   317,   220,   317,   319,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   319,   220,   317,   317,
     317,   220,   220,   220,   220,   220,   317,   220,   317,   220,
     319,   319,   317,   319,   319,   319,   319,   202,   203,   220,
     220,   219,   226,   317,   219,   226,   317,   220,   220,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   319,   317,   317,   317,   319,   319,
     319,   319,   319,   319,   317,   319,   317,   317,   219,   317,
     219,   317,   219,   317,   220,   225,   219,   219,   317,   219,
     317,   219,   317,   317,   219,   319,   219,   319,   219,   317,
     317,   317,   317,   317,   220,   219,   317,   219,   319,   219,
     317,   219,   317,   219,   317,   219,   317,   219,   319,   219,
     317,   317,   219,   317,   219,   317,   219,   317,   219,   219,
     319,   317,   219,   319,   219,   219,   319,   219,   317,   219,
     319,   218,   218,   219,   222,   218,   218,   318,   319,   219,
     222,   221,   221,   219,   219,   220,   220,   220,   220,   220,
     219,   219,   219,   220,   220,   219,   219,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   219,   219,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   219,   220,   219,   219,   219,   219,
     219,   220,   219,   220,   226,   219,   222,   219,   222,   226,
     219,   222,   219,   222,   219,   222,   219,   222,   317,   317,
     219,   317,   219,   316,   317,   219,   317,   219,   317,    28,
     319,   319,   317,   219,   219,   219,   219,   219,   317,   319,
     317,   219,   317,   219,   317,   219,   317,   219,   317,   319,
     317,   219,   219,   317,   219,   317,   317,   319,   317,   219,
     319,   319,   319,   219,   317,   319,   319,   319,   317,   209,
     319,   319,   225,   219,   317,   209,   317,   317,   317,   317,
     317,   317,   317,   319,   319,   317,   317,   319,   319,   319,
     319,   319,   319,   319,   222,   222,   317,   222,   222,   317,
     317,   317,   220,   219,   317,   219,   317,   219,   202,   315,
     317,   219,   317,   220,   317,   220,   220,   219,   317,   317,
     317,   317,   317,   220,   220,   219,   317,   319,   219,   317,
     219,   319,   219,   317,   319,   219,   220,   219,   317,   317,
     219,   317,   220,   220,   219,   220,   317,   220,   220,   219,
     319,   220,   220,   219,   219,   222,   317,   219,   219,   318,
     222,   319,   222,   222,   220,   219,   219,   220,   220,   219,
     220,   219,   220,   219,   220,   220,   219,   219,   220,   219,
     220,   220,   220,   222,   222,   222,   222,   317,   317,   219,
     202,   317,   219,   317,   219,    29,   317,   220,   220,   220,
     220,   317,   319,   317,   319,   219,   317,   319,   219,   317,
     317,   219,   317,   317,   319,   319,   319,   209,   319,   319,
     209,   317,   317,   317,   317,   317,   319,   317,   319,   220,
     219,   317,   220,   219,   202,   315,   317,   219,   317,   317,
     220,   220,   220,   220,   219,   220,   317,   219,   220,   317,
     319,   220,   219,   317,   220,   220,   220,   219,   220,   220,
     317,   219,   220,   220,   319,   220,   220,   219,   220,   220,
     219,   219,   220,   220,   220,   317,   202,   317,   219,   317,
     317,   317,   319,   219,   317,   219,   317,   317,   317,   317,
     317,   220,   220,   219,   202,   315,   220,   220,   219,   220,
     319,   219,   317,   220,   220,   220,   220,   220,   202,   319,
     317,   220,   220,   220
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
#line 305 "basicParse.y"
    { labeltable[(yyvsp[(1) - (1)].number)] = byteOffset; lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 307 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 308 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 309 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 310 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 311 "basicParse.y"
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
#line 318 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 319 "basicParse.y"
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
#line 326 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 327 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 328 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 332 "basicParse.y"
    {
	// if there is an if branch or jump on the iftable stack get where it is
	// in the bytecode array and then put the current bytecode address there
	// - so we can jump over code
	if (numifs>0) {
		unsigned int *temp = NULL;
		numifs--;
		temp = (unsigned int *) (byteCode + iftable[numifs]);
		*temp = byteOffset;
	}
;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 346 "basicParse.y"
    {
		// there is nothing to do with a multi line if (ifexp handles it)
	;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 352 "basicParse.y"
    {
	unsigned int elsegototemp = 0;
	// on else create a jump point to the endif
	addIntOp(OP_PUSHINT, 0);	// false - always jump before else to endif
	addOp(OP_BRANCH);
	elsegototemp = addInt(0);
	// resolve the false jump on the if to the current location
	if (numifs>0) {
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
#line 375 "basicParse.y"
    {
	// if there is an if branch or jump on the iftable stack get where it is
	// in the bytecode array and then put the current bytecode address there
	// - so we can jump over code
	if (numifs>0) {
		unsigned int *temp = NULL;
		numifs--;
		temp = (unsigned int *) (byteCode + iftable[numifs]);
		*temp = byteOffset; 
	}
;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 389 "basicParse.y"
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
#line 405 "basicParse.y"
    {
	// there should be two bytecode locations.  the TOP is the
	// location to jump to at the top of the loopthe , TOP-1 is the location
	// the exit jump needs to be written back to jump point on WHILE
	if (numifs>1) {
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
#line 422 "basicParse.y"
    {
		// need nothing done at top of a do
	;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 428 "basicParse.y"
    {
	// create temp
	//if If false, go to to the corresponding do.
	if (numifs>0) {
		addIntOp(OP_BRANCH, iftable[numifs-1]);
		numifs--;
	}
;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 510 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 511 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 512 "basicParse.y"
    { addIntOp(OP_DIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 513 "basicParse.y"
    { addIntOp(OP_DIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 516 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 517 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 518 "basicParse.y"
    { addIntOp(OP_REDIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 519 "basicParse.y"
    { addIntOp(OP_REDIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 522 "basicParse.y"
    { addOp(OP_PAUSE); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 525 "basicParse.y"
    { addOp(OP_CLS); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 526 "basicParse.y"
    { addOp(OP_CLG); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 529 "basicParse.y"
    { addOp(OP_FASTGRAPHICS); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 532 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 533 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 536 "basicParse.y"
    { addOp(OP_REFRESH); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 539 "basicParse.y"
    { addOp(OP_END); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 543 "basicParse.y"
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
#line 556 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 557 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 558 "basicParse.y"
    { addInt2Op(OP_STRARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 559 "basicParse.y"
    { addIntOp(OP_EXPLODESTR, (yyvsp[(1) - (8)].number));;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 560 "basicParse.y"
    { addIntOp(OP_EXPLODESTR_C, (yyvsp[(1) - (10)].number)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 561 "basicParse.y"
    { addIntOp(OP_EXPLODEXSTR, (yyvsp[(1) - (8)].number));;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 564 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 565 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 566 "basicParse.y"
    { addInt2Op(OP_ARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 567 "basicParse.y"
    { addIntOp(OP_EXPLODE, (yyvsp[(1) - (8)].number));;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 568 "basicParse.y"
    { addIntOp(OP_EXPLODE_C, (yyvsp[(1) - (10)].number)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 569 "basicParse.y"
    { addIntOp(OP_EXPLODEX, (yyvsp[(1) - (8)].number));;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 573 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 576 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 580 "basicParse.y"
    {
	addIntOp(OP_PUSHINT, 1); //step
	addIntOp(OP_FOR, (yyvsp[(2) - (6)].number));
	;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 585 "basicParse.y"
    {
	addIntOp(OP_FOR, (yyvsp[(2) - (8)].number));
	;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 590 "basicParse.y"
    { addIntOp(OP_NEXT, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 593 "basicParse.y"
    { addIntOp(OP_GOTO, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 596 "basicParse.y"
    { addIntOp(OP_GOSUB, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 599 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OFFERROR); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 602 "basicParse.y"
    { addIntOp(OP_ONERROR, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 605 "basicParse.y"
    { addOp(OP_RETURN); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 608 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 609 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 610 "basicParse.y"
    { addOp(OP_SETCOLORINT); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 613 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 614 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 615 "basicParse.y"
    { addIntOp(OP_SOUND_LIST, listlen); listlen=0; ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 616 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 617 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 620 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 621 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 624 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 625 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 629 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 630 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 633 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 634 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 637 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 638 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 639 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 640 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 643 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 644 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 647 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 648 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 651 "basicParse.y"
    { addOp(OP_SYSTEM); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 654 "basicParse.y"
    { addOp(OP_VOLUME); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 657 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 658 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 659 "basicParse.y"
    { addIntOp(OP_POLY_LIST, listlen); listlen=0; ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 662 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(8) - (8)].number)); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 663 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(9) - (10)].number)); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 664 "basicParse.y"
    { addIntOp(OP_STAMP_S_LIST, listlen); listlen=0; ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 665 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(6) - (6)].number)); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 666 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(7) - (8)].number)); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 667 "basicParse.y"
    { addIntOp(OP_STAMP_LIST, listlen); listlen=0; ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 668 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(10) - (10)].number)); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 669 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(11) - (12)].number)); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 670 "basicParse.y"
    { addIntOp(OP_STAMP_SR_LIST, listlen); listlen=0; ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 673 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_OPEN); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 674 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 675 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 678 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITE); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 679 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 680 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 683 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITELINE); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 684 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 685 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 688 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 689 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 690 "basicParse.y"
    { addOp(OP_CLOSE); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 693 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 694 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 695 "basicParse.y"
    { addOp(OP_RESET); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 698 "basicParse.y"
    {addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP);addOp(OP_SEEK); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 699 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 700 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 703 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 704 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 705 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 706 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_ARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 707 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRINGASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 708 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 709 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 710 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_NUMASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 711 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 712 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 715 "basicParse.y"
    { addOp(OP_PRINT);  addOp(OP_INPUT); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 718 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_PRINTN); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 719 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 720 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 721 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 722 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 725 "basicParse.y"
    {addOp(OP_WAVPLAY);  ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 728 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 729 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 732 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 733 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 736 "basicParse.y"
    {addOp(OP_PUTSLICE);  ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 737 "basicParse.y"
    { addOp(OP_PUTSLICE); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 738 "basicParse.y"
    {addOp(OP_PUTSLICEMASK);  ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 739 "basicParse.y"
    { addOp(OP_PUTSLICEMASK); ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 741 "basicParse.y"
    {addOp(OP_IMGLOAD);  ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 742 "basicParse.y"
    { addOp(OP_IMGLOAD); ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 743 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 744 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 745 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 746 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 749 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEDIM); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 752 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD);  ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 753 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 756 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE);  ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 757 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 760 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE);  ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 761 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 764 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE);  ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 765 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 768 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEHIDE); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 771 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESHOW); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 774 "basicParse.y"
    {addOp(OP_CLICKCLEAR);  ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 775 "basicParse.y"
    { addOp(OP_CLICKCLEAR); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 778 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CHANGEDIR); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 781 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DECIMAL); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 784 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPEN); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 787 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 788 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 791 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBEXECUTE); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 794 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPENSET); ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 797 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 798 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 801 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 802 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 803 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 806 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 807 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 808 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 809 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 812 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 813 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 814 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 817 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 818 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 819 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 822 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_KILL); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 823 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_KILL); ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 826 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SETSETTING); ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 827 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SETSETTING); ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 830 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTOUT); ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 831 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTOUT); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 834 "basicParse.y"
    {addStringOp(OP_PUSHSTRING, "PNG"); addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 835 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 836 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 846 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 847 "basicParse.y"
    { listlen++; ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 850 "basicParse.y"
    { (yyval.floatnum) = (yyvsp[(2) - (3)].floatnum); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 851 "basicParse.y"
    { addOp(OP_ADD); ;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 852 "basicParse.y"
    { addOp(OP_SUB); ;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 853 "basicParse.y"
    { addOp(OP_MUL); ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 854 "basicParse.y"
    { addOp(OP_MOD); ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 855 "basicParse.y"
    { addOp(OP_INTDIV); ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 856 "basicParse.y"
    { addOp(OP_DIV); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 857 "basicParse.y"
    { addOp(OP_EX); ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 858 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYOR); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 859 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYAND); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 860 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYNOT); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 861 "basicParse.y"
    { addOp(OP_NEGATE); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 862 "basicParse.y"
    {addOp(OP_AND); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 863 "basicParse.y"
    { addOp(OP_OR); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 864 "basicParse.y"
    { addOp(OP_XOR); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 865 "basicParse.y"
    { addOp(OP_NOT); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 866 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 867 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 868 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 869 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 870 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 871 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 872 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 873 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 874 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 875 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 876 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 877 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 878 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum)); ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 879 "basicParse.y"
    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 880 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 881 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 882 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 883 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 884 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 885 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 886 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 887 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 889 "basicParse.y"
    {
		if ((yyvsp[(1) - (1)].number) < 0) {
			return -1;
		} else {
			addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 896 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 897 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 898 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 899 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 900 "basicParse.y"
    { addOp(OP_LENGTH); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 901 "basicParse.y"
    { addOp(OP_ASC); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 902 "basicParse.y"
    { addOp(OP_INSTR); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 903 "basicParse.y"
    { addOp(OP_INSTR_S); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 904 "basicParse.y"
    { addOp(OP_INSTR_SC); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 905 "basicParse.y"
    { addOp(OP_INSTRX); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 906 "basicParse.y"
    { addOp(OP_INSTRX_S); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 907 "basicParse.y"
    { addOp(OP_CEIL); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 908 "basicParse.y"
    { addOp(OP_FLOOR); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 909 "basicParse.y"
    { addOp(OP_SIN); ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 910 "basicParse.y"
    { addOp(OP_COS); ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 911 "basicParse.y"
    { addOp(OP_TAN); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 912 "basicParse.y"
    { addOp(OP_ASIN); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 913 "basicParse.y"
    { addOp(OP_ACOS); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 914 "basicParse.y"
    { addOp(OP_ATAN); ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 915 "basicParse.y"
    { addOp(OP_DEGREES); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 916 "basicParse.y"
    { addOp(OP_RADIANS); ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 917 "basicParse.y"
    { addOp(OP_LOG); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 918 "basicParse.y"
    { addOp(OP_LOGTEN); ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 919 "basicParse.y"
    { addOp(OP_SQR); ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 920 "basicParse.y"
    { addOp(OP_EXP); ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 921 "basicParse.y"
    { addOp(OP_ABS); ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 922 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 923 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 924 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 925 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 926 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 927 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 928 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 929 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 930 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 931 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 932 "basicParse.y"
    { addOp(OP_EOF); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 933 "basicParse.y"
    { addOp(OP_EXISTS); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 934 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 935 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 936 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 937 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 938 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 939 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 940 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 941 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 942 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 943 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 944 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 945 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 946 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 947 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 948 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 949 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 950 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 951 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 952 "basicParse.y"
    { addOp(OP_SIZE); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 953 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 954 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 955 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 956 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 957 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 958 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 959 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 960 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 961 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 962 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 963 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 964 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 965 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 966 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 967 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 968 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 969 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 970 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 971 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 972 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 973 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 974 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 975 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 976 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 977 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 978 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 979 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 980 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 981 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 982 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 983 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 984 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 985 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 986 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 987 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 988 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 989 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 990 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 991 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 992 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 993 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 994 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 995 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 996 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 997 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 998 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 999 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1000 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1001 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1002 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1003 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1004 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1005 "basicParse.y"
    { addOp(OP_PIXEL); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1006 "basicParse.y"
    { addOp(OP_RGB); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1007 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1008 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1009 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITECOLLIDE); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1010 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEX); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1011 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEY); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1012 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEH); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1013 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEW); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1014 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEV); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1015 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBROW); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1016 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBINT); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1017 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBFLOAT); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1018 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1019 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1020 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1021 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1022 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp( OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1023 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 1024 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1025 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTIN); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1026 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNT); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 1027 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNT_C); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1028 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNTX); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1029 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OSTYPE); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1030 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OSTYPE); ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1031 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MSEC); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1032 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MSEC); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 1035 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1036 "basicParse.y"
    { listlen++; ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1039 "basicParse.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1040 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1041 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1042 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1043 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1045 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1047 "basicParse.y"
    {
		if ((yyvsp[(1) - (1)].number) < 0) {
			return -1;
		} else {
			addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1054 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1055 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1056 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1057 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1058 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1059 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1060 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1061 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1062 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 1063 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 1064 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 1065 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 1066 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 1067 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 1068 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 1069 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 1070 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 1071 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 1072 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 1073 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 1074 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 1075 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 1076 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 1077 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 1078 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 1079 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 1080 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MD5); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 1081 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_GETSETTING); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 1082 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 1083 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 1084 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 1085 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE_C); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 1086 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACEX); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 1087 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 1088 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 1089 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 1090 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 9483 "basicParse.tab.c"
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
#line 1094 "basicParse.y"


int
yyerror(const char *msg) {
	errorcode = -1;
	if (yytext[0] == '\n') { linenumber--; } // error happened on previous line
	return -1;
}

