
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
     B256EDITVISIBLE = 453,
     B256GRAPHVISIBLE = 454,
     B256OUTPUTVISIBLE = 455,
     B256EDITSIZE = 456,
     B256OUTPUTSIZE = 457,
     B256LINENUM = 458,
     B256INTEGER = 459,
     B256FLOAT = 460,
     B256STRING = 461,
     B256VARIABLE = 462,
     B256STRINGVAR = 463,
     B256NEWVAR = 464,
     B256COLOR = 465,
     B256LABEL = 466,
     B256UMINUS = 467
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 266 "basicParse.y"

	int number;
	double floatnum;
	char *string;



/* Line 214 of yacc.c  */
#line 537 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 549 "basicParse.tab.c"

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
#define YYFINAL  398
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   19099

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  232
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  501
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1233

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   467

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     221,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     223,   225,   217,   216,   224,   215,     2,   218,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   222,   228,
     212,   214,   213,   231,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   226,     2,   227,   220,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   229,     2,   230,     2,     2,     2,     2,
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
     205,   206,   207,   208,   209,   210,   211,   219
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
     211,   213,   215,   219,   223,   231,   239,   243,   247,   255,
     263,   266,   268,   270,   272,   277,   284,   286,   288,   291,
     298,   307,   311,   320,   331,   340,   347,   356,   360,   369,
     380,   389,   393,   397,   404,   413,   416,   419,   422,   424,
     427,   429,   436,   445,   448,   453,   456,   459,   466,   471,
     476,   483,   492,   503,   510,   519,   528,   539,   546,   555,
     562,   571,   578,   587,   590,   593,   596,   599,   602,   607,
     610,   619,   630,   639,   646,   655,   662,   673,   686,   697,
     700,   707,   712,   715,   722,   727,   730,   737,   742,   744,
     748,   751,   753,   757,   760,   763,   770,   775,   779,   786,
     790,   797,   800,   806,   814,   817,   823,   831,   834,   836,
     839,   842,   846,   850,   853,   855,   859,   861,   865,   872,
     881,   890,   901,   908,   917,   926,   937,   948,   961,   964,
     969,   976,   987,  1000,  1007,  1016,  1023,  1032,  1035,  1038,
    1040,  1044,  1047,  1050,  1053,  1055,  1059,  1062,  1065,  1067,
    1071,  1074,  1081,  1086,  1091,  1098,  1105,  1114,  1117,  1124,
    1129,  1131,  1135,  1138,  1141,  1148,  1155,  1164,  1169,  1176,
    1179,  1186,  1191,  1194,  1197,  1200,  1204,  1208,  1210,  1214,
    1218,  1222,  1226,  1230,  1234,  1238,  1242,  1246,  1250,  1254,
    1257,  1260,  1264,  1268,  1272,  1275,  1279,  1283,  1287,  1291,
    1295,  1299,  1303,  1307,  1311,  1315,  1319,  1323,  1325,  1327,
    1332,  1337,  1343,  1349,  1355,  1361,  1366,  1373,  1375,  1380,
    1385,  1390,  1395,  1400,  1405,  1412,  1421,  1432,  1439,  1448,
    1453,  1458,  1463,  1468,  1473,  1478,  1483,  1488,  1493,  1498,
    1503,  1508,  1513,  1518,  1523,  1525,  1529,  1531,  1535,  1537,
    1541,  1543,  1547,  1549,  1553,  1558,  1563,  1565,  1569,  1571,
    1575,  1577,  1581,  1583,  1587,  1589,  1593,  1595,  1599,  1601,
    1605,  1607,  1611,  1613,  1617,  1622,  1624,  1628,  1630,  1634,
    1636,  1640,  1642,  1646,  1648,  1652,  1654,  1658,  1660,  1664,
    1666,  1670,  1672,  1676,  1678,  1682,  1684,  1688,  1690,  1694,
    1696,  1700,  1702,  1706,  1708,  1712,  1714,  1718,  1720,  1724,
    1726,  1730,  1732,  1736,  1738,  1742,  1744,  1748,  1750,  1754,
    1756,  1760,  1762,  1766,  1768,  1772,  1774,  1778,  1785,  1794,
    1796,  1800,  1807,  1812,  1817,  1822,  1827,  1832,  1836,  1841,
    1846,  1848,  1852,  1854,  1858,  1860,  1864,  1869,  1874,  1881,
    1890,  1897,  1899,  1903,  1905,  1909,  1911,  1915,  1919,  1923,
    1927,  1931,  1933,  1938,  1945,  1947,  1952,  1957,  1962,  1967,
    1976,  1983,  1990,  2001,  2003,  2007,  2012,  2014,  2018,  2023,
    2025,  2029,  2034,  2036,  2040,  2042,  2046,  2048,  2052,  2057,
    2059,  2063,  2068,  2075,  2080,  2084,  2093,  2104,  2113,  2118,
    2125,  2130
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     233,     0,    -1,   234,   221,    -1,   234,   221,   233,    -1,
     235,   236,    -1,   236,    -1,   211,    -1,   237,    -1,   238,
      -1,   239,    -1,   241,    -1,   242,    -1,   244,    -1,   245,
      -1,   246,    -1,   247,    -1,    -1,   257,    20,   247,    -1,
     257,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     240,    -1,    23,   325,    -1,    24,    -1,    40,    23,    -1,
     243,    -1,    25,    -1,    26,   325,    -1,   248,    -1,   247,
     222,   248,    -1,   264,    -1,   265,    -1,   266,    -1,   267,
      -1,   268,    -1,   290,    -1,   271,    -1,   273,    -1,   274,
      -1,   280,    -1,   281,    -1,   272,    -1,   260,    -1,   261,
      -1,   262,    -1,   263,    -1,   269,    -1,   288,    -1,   256,
      -1,   252,    -1,   255,    -1,   253,    -1,   254,    -1,   249,
      -1,   250,    -1,   251,    -1,   259,    -1,   258,    -1,   282,
      -1,   283,    -1,   284,    -1,   285,    -1,   286,    -1,   315,
      -1,   270,    -1,   275,    -1,   276,    -1,   277,    -1,   278,
      -1,   279,    -1,   291,    -1,   292,    -1,   293,    -1,   294,
      -1,   295,    -1,   296,    -1,   297,    -1,   298,    -1,   299,
      -1,   300,    -1,   301,    -1,   302,    -1,   287,    -1,   303,
      -1,   304,    -1,   305,    -1,   306,    -1,   307,    -1,   308,
      -1,   309,    -1,   310,    -1,   311,    -1,   312,    -1,   313,
      -1,   314,    -1,   316,    -1,   317,    -1,   318,    -1,   319,
      -1,   320,    -1,   321,    -1,    45,   207,   325,    -1,    45,
     208,   325,    -1,    45,   207,   223,   325,   224,   325,   225,
      -1,    45,   208,   223,   325,   224,   325,   225,    -1,    46,
     207,   325,    -1,    46,   208,   325,    -1,    46,   207,   223,
     325,   224,   325,   225,    -1,    46,   208,   223,   325,   224,
     325,   225,    -1,    79,   325,    -1,    17,    -1,    18,    -1,
      14,    -1,    15,   325,   224,   325,    -1,    15,   223,   325,
     224,   325,   225,    -1,    16,    -1,    40,    -1,    19,   325,
      -1,   208,   226,   325,   227,   214,   327,    -1,   208,   226,
     325,   224,   325,   227,   214,   327,    -1,   208,   214,   322,
      -1,   208,   214,   191,   223,   327,   224,   327,   225,    -1,
     208,   214,   191,   223,   327,   224,   327,   224,   325,   225,
      -1,   208,   214,   194,   223,   327,   224,   327,   225,    -1,
     207,   226,   325,   227,   214,   325,    -1,   207,   226,   325,
     224,   325,   227,   214,   325,    -1,   207,   214,   323,    -1,
     207,   214,   191,   223,   327,   224,   327,   225,    -1,   207,
     214,   191,   223,   327,   224,   327,   224,   325,   225,    -1,
     207,   214,   194,   223,   327,   224,   327,   225,    -1,   207,
     214,   325,    -1,   208,   214,   327,    -1,    27,   207,   214,
     325,    28,   325,    -1,    27,   207,   214,   325,    28,   325,
      29,   325,    -1,    30,   207,    -1,    36,   207,    -1,    37,
     207,    -1,   167,    -1,   166,   207,    -1,    38,    -1,    41,
     325,   224,   325,   224,   325,    -1,    41,   223,   325,   224,
     325,   224,   325,   225,    -1,    41,   325,    -1,    80,   223,
     207,   225,    -1,    80,   207,    -1,    80,   323,    -1,    80,
     223,   325,   224,   325,   225,    -1,    80,   325,   224,   325,
      -1,     8,   325,   224,   325,    -1,     8,   223,   325,   224,
     325,   225,    -1,    13,   325,   224,   325,   224,   325,   224,
     325,    -1,    13,   223,   325,   224,   325,   224,   325,   224,
     325,   225,    -1,     9,   325,   224,   325,   224,   325,    -1,
       9,   223,   325,   224,   325,   224,   325,   225,    -1,    10,
     325,   224,   325,   224,   325,   224,   325,    -1,    10,   223,
     325,   224,   325,   224,   325,   224,   325,   225,    -1,    95,
     325,   224,   325,   224,   327,    -1,    95,   223,   325,   224,
     325,   224,   327,   225,    -1,    95,   325,   224,   325,   224,
     325,    -1,    95,   223,   325,   224,   325,   224,   325,   225,
      -1,    96,   327,   224,   325,   224,   325,    -1,    96,   223,
     327,   224,   325,   224,   325,   225,    -1,    97,   327,    -1,
      97,   325,    -1,    98,   327,    -1,    99,   325,    -1,    11,
     207,    -1,    11,   223,   207,   225,    -1,    11,   323,    -1,
      12,   325,   224,   325,   224,   325,   224,   207,    -1,    12,
     223,   325,   224,   325,   224,   325,   224,   207,   225,    -1,
      12,   325,   224,   325,   224,   325,   224,   323,    -1,    12,
     325,   224,   325,   224,   207,    -1,    12,   223,   325,   224,
     325,   224,   207,   225,    -1,    12,   325,   224,   325,   224,
     323,    -1,    12,   325,   224,   325,   224,   325,   224,   325,
     224,   207,    -1,    12,   223,   325,   224,   325,   224,   325,
     224,   325,   224,   207,   225,    -1,    12,   325,   224,   325,
     224,   325,   224,   325,   224,   323,    -1,    31,   327,    -1,
      31,   223,   325,   224,   327,   225,    -1,    31,   325,   224,
     327,    -1,    33,   327,    -1,    33,   223,   325,   224,   327,
     225,    -1,    33,   325,   224,   327,    -1,    85,   327,    -1,
      85,   223,   325,   224,   327,   225,    -1,    85,   325,   224,
     327,    -1,    34,    -1,    34,   223,   225,    -1,    34,   325,
      -1,    35,    -1,    35,   223,   225,    -1,    35,   325,    -1,
     122,   325,    -1,   122,   223,   325,   224,   325,   225,    -1,
     122,   325,   224,   325,    -1,   289,   224,   208,    -1,   289,
     224,   208,   226,   325,   227,    -1,   289,   224,   207,    -1,
     289,   224,   207,   226,   325,   227,    -1,     4,   208,    -1,
       4,   208,   226,   325,   227,    -1,     4,   208,   226,   325,
     224,   325,   227,    -1,     4,   207,    -1,     4,   207,   226,
     325,   227,    -1,     4,   207,   226,   325,   224,   325,   227,
      -1,     4,   327,    -1,     3,    -1,     3,   327,    -1,     3,
     325,    -1,     3,   327,   228,    -1,     3,   325,   228,    -1,
     118,   327,    -1,   119,    -1,   119,   223,   225,    -1,   120,
      -1,   120,   223,   225,    -1,   103,   325,   224,   325,   224,
     327,    -1,   103,   223,   325,   224,   325,   224,   327,   225,
      -1,   103,   325,   224,   325,   224,   327,   224,   325,    -1,
     103,   223,   325,   224,   325,   224,   327,   224,   325,   225,
      -1,   104,   325,   224,   325,   224,   327,    -1,   104,   223,
     325,   224,   325,   224,   327,   225,    -1,   104,   325,   224,
     325,   224,   325,   224,   327,    -1,   104,   223,   325,   224,
     325,   224,   325,   224,   327,   225,    -1,   104,   325,   224,
     325,   224,   325,   224,   325,   224,   327,    -1,   104,   223,
     325,   224,   325,   224,   325,   224,   325,   224,   327,   225,
      -1,   105,   325,    -1,   106,   325,   224,   327,    -1,   106,
     223,   325,   224,   327,   225,    -1,   107,   325,   224,   325,
     224,   325,   224,   325,   224,   325,    -1,   107,   223,   325,
     224,   325,   224,   325,   224,   325,   224,   325,   225,    -1,
     111,   325,   224,   325,   224,   325,    -1,   111,   223,   325,
     224,   325,   224,   325,   225,    -1,   108,   325,   224,   325,
     224,   325,    -1,   106,   223,   325,   224,   325,   224,   325,
     225,    -1,   109,   325,    -1,   110,   325,    -1,   129,    -1,
     129,   223,   225,    -1,   153,   327,    -1,   156,   325,    -1,
     157,   327,    -1,   158,    -1,   158,   223,   225,    -1,   159,
     327,    -1,   160,   327,    -1,   161,    -1,   161,   223,   225,
      -1,   172,   325,    -1,   172,   223,   325,   224,   325,   225,
      -1,   172,   325,   224,   325,    -1,   173,   327,   224,   325,
      -1,   173,   223,   327,   224,   325,   225,    -1,   173,   325,
     224,   327,   224,   325,    -1,   173,   223,   325,   224,   327,
     224,   325,   225,    -1,   175,   327,    -1,   175,   223,   325,
     224,   327,   225,    -1,   175,   325,   224,   327,    -1,   176,
      -1,   176,   223,   225,    -1,   176,   325,    -1,   179,   327,
      -1,   179,   223,   325,   224,   327,   225,    -1,   181,   327,
     224,   327,   224,   327,    -1,   181,   223,   327,   224,   327,
     224,   327,   225,    -1,   184,   325,   224,   325,    -1,   184,
     223,   325,   224,   325,   225,    -1,   188,   327,    -1,   188,
     223,   327,   224,   327,   225,    -1,   188,   327,   224,   327,
      -1,   198,   325,    -1,   199,   325,    -1,   200,   325,    -1,
     229,   326,   230,    -1,   229,   324,   230,    -1,   325,    -1,
     325,   224,   324,    -1,   223,   325,   225,    -1,   325,   216,
     325,    -1,   325,   215,   325,    -1,   325,   217,   325,    -1,
     325,    87,   325,    -1,   325,    88,   325,    -1,   325,   218,
     325,    -1,   325,   220,   325,    -1,   325,   185,   325,    -1,
     325,   186,   325,    -1,   187,   325,    -1,   215,   325,    -1,
     325,    75,   325,    -1,   325,    76,   325,    -1,   325,    77,
     325,    -1,    78,   325,    -1,   327,   214,   327,    -1,   327,
      44,   327,    -1,   327,   212,   327,    -1,   327,   213,   327,
      -1,   327,    42,   327,    -1,   327,    43,   327,    -1,   325,
     214,   325,    -1,   325,    44,   325,    -1,   325,   212,   325,
      -1,   325,   213,   325,    -1,   325,    42,   325,    -1,   325,
      43,   325,    -1,   205,    -1,   204,    -1,   207,   226,   231,
     227,    -1,   208,   226,   231,   227,    -1,   207,   226,   231,
     224,   227,    -1,   208,   226,   231,   224,   227,    -1,   207,
     226,   224,   231,   227,    -1,   208,   226,   224,   231,   227,
      -1,   207,   226,   325,   227,    -1,   207,   226,   325,   224,
     325,   227,    -1,   207,    -1,    48,   223,   325,   225,    -1,
      48,   223,   327,   225,    -1,    83,   223,   325,   225,    -1,
      83,   223,   327,   225,    -1,    50,   223,   327,   225,    -1,
      81,   223,   327,   225,    -1,    56,   223,   327,   224,   327,
     225,    -1,    56,   223,   327,   224,   327,   224,   325,   225,
      -1,    56,   223,   327,   224,   327,   224,   325,   224,   325,
     225,    -1,    57,   223,   327,   224,   327,   225,    -1,    57,
     223,   327,   224,   327,   224,   325,   225,    -1,    58,   223,
     325,   225,    -1,    59,   223,   325,   225,    -1,    61,   223,
     325,   225,    -1,    62,   223,   325,   225,    -1,    63,   223,
     325,   225,    -1,    64,   223,   325,   225,    -1,    65,   223,
     325,   225,    -1,    66,   223,   325,   225,    -1,    69,   223,
     325,   225,    -1,    70,   223,   325,   225,    -1,    71,   223,
     325,   225,    -1,    72,   223,   325,   225,    -1,    73,   223,
     325,   225,    -1,    74,   223,   325,   225,    -1,    67,   223,
     325,   225,    -1,    60,    -1,    60,   223,   225,    -1,    68,
      -1,    68,   223,   225,    -1,   124,    -1,   124,   223,   225,
      -1,   125,    -1,   125,   223,   225,    -1,    86,    -1,    86,
     223,   225,    -1,    86,   223,   325,   225,    -1,   123,   223,
     327,   225,    -1,    89,    -1,    89,   223,   225,    -1,    90,
      -1,    90,   223,   225,    -1,    91,    -1,    91,   223,   225,
      -1,    92,    -1,    92,   223,   225,    -1,    93,    -1,    93,
     223,   225,    -1,    94,    -1,    94,   223,   225,    -1,   100,
      -1,   100,   223,   225,    -1,   101,    -1,   101,   223,   225,
      -1,   121,    -1,   121,   223,   225,    -1,   121,   223,   325,
     225,    -1,     5,    -1,     5,   223,   225,    -1,   126,    -1,
     126,   223,   225,    -1,   127,    -1,   127,   223,   225,    -1,
     128,    -1,   128,   223,   225,    -1,   130,    -1,   130,   223,
     225,    -1,   131,    -1,   131,   223,   225,    -1,   132,    -1,
     132,   223,   225,    -1,   134,    -1,   134,   223,   225,    -1,
     135,    -1,   135,   223,   225,    -1,   136,    -1,   136,   223,
     225,    -1,   137,    -1,   137,   223,   225,    -1,   138,    -1,
     138,   223,   225,    -1,   139,    -1,   139,   223,   225,    -1,
     140,    -1,   140,   223,   225,    -1,   141,    -1,   141,   223,
     225,    -1,   142,    -1,   142,   223,   225,    -1,   143,    -1,
     143,   223,   225,    -1,   144,    -1,   144,   223,   225,    -1,
     145,    -1,   145,   223,   225,    -1,   146,    -1,   146,   223,
     225,    -1,   147,    -1,   147,   223,   225,    -1,   148,    -1,
     148,   223,   225,    -1,   149,    -1,   149,   223,   225,    -1,
     150,    -1,   150,   223,   225,    -1,   151,    -1,   151,   223,
     225,    -1,   152,    -1,   152,   223,   225,    -1,     6,   223,
     325,   224,   325,   225,    -1,     7,   223,   325,   224,   325,
     224,   325,   225,    -1,   133,    -1,   133,   223,   225,    -1,
     112,   223,   325,   224,   325,   225,    -1,   113,   223,   325,
     225,    -1,   114,   223,   325,   225,    -1,   115,   223,   325,
     225,    -1,   116,   223,   325,   225,    -1,   117,   223,   325,
     225,    -1,   162,   223,   225,    -1,   163,   223,   325,   225,
      -1,   164,   223,   325,   225,    -1,   168,    -1,   168,   223,
     225,    -1,   170,    -1,   170,   223,   225,    -1,   177,    -1,
     177,   223,   225,    -1,   177,   223,   325,   225,    -1,   183,
     223,   325,   225,    -1,   190,   223,   327,   224,   327,   225,
      -1,   190,   223,   327,   224,   327,   224,   325,   225,    -1,
     193,   223,   327,   224,   327,   225,    -1,   196,    -1,   196,
     223,   225,    -1,   197,    -1,   197,   223,   225,    -1,   327,
      -1,   327,   224,   326,    -1,   223,   327,   225,    -1,   327,
     216,   327,    -1,   325,   216,   327,    -1,   327,   216,   325,
      -1,   206,    -1,   208,   226,   325,   227,    -1,   208,   226,
     325,   224,   325,   227,    -1,   208,    -1,    82,   223,   325,
     225,    -1,    49,   223,   325,   225,    -1,    54,   223,   327,
     225,    -1,    55,   223,   327,   225,    -1,    51,   223,   327,
     224,   325,   224,   325,   225,    -1,    52,   223,   327,   224,
     325,   225,    -1,    53,   223,   327,   224,   325,   225,    -1,
     102,   223,   325,   224,   325,   224,   325,   224,   325,   225,
      -1,    32,    -1,    32,   223,   225,    -1,    32,   223,   325,
     225,    -1,    84,    -1,    84,   223,   225,    -1,    84,   223,
     325,   225,    -1,   154,    -1,   154,   223,   225,    -1,   165,
     223,   325,   225,    -1,   169,    -1,   169,   223,   225,    -1,
     171,    -1,   171,   223,   225,    -1,   174,    -1,   174,   223,
     225,    -1,   174,   223,   325,   225,    -1,   178,    -1,   178,
     223,   225,    -1,   180,   223,   327,   225,    -1,   182,   223,
     327,   224,   327,   225,    -1,   155,   223,   327,   225,    -1,
     155,   223,   225,    -1,   189,   223,   327,   224,   327,   224,
     327,   225,    -1,   189,   223,   327,   224,   327,   224,   327,
     224,   325,   225,    -1,   192,   223,   327,   224,   327,   224,
     327,   225,    -1,   195,   223,   208,   225,    -1,   195,   223,
     208,   224,   327,   225,    -1,   195,   223,   207,   225,    -1,
     195,   223,   207,   224,   327,   225,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   298,   298,   299,   302,   303,   306,   308,   309,   310,
     311,   312,   319,   320,   327,   328,   329,   332,   346,   352,
     372,   373,   375,   389,   402,   403,   405,   422,   428,   438,
     438,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   484,   485,   486,   487,   488,   489,
     490,   491,   492,   493,   494,   495,   496,   497,   498,   499,
     500,   501,   502,   503,   504,   505,   506,   507,   508,   509,
     510,   511,   514,   515,   516,   517,   520,   521,   522,   523,
     526,   529,   530,   533,   536,   537,   540,   543,   546,   560,
     561,   562,   563,   564,   565,   568,   569,   570,   571,   572,
     573,   577,   580,   583,   588,   594,   597,   600,   603,   606,
     609,   612,   613,   614,   617,   618,   619,   620,   621,   624,
     625,   628,   629,   633,   634,   637,   638,   641,   642,   643,
     644,   647,   648,   651,   652,   655,   658,   661,   662,   663,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   677,
     678,   679,   682,   683,   684,   687,   688,   689,   692,   693,
     694,   697,   698,   699,   702,   703,   704,   707,   708,   709,
     710,   711,   712,   713,   714,   715,   716,   719,   722,   723,
     724,   725,   726,   729,   732,   733,   736,   737,   740,   741,
     742,   743,   745,   746,   747,   748,   749,   750,   753,   756,
     757,   760,   761,   764,   765,   768,   769,   772,   775,   778,
     779,   782,   785,   788,   791,   792,   795,   798,   801,   802,
     805,   806,   807,   810,   811,   812,   813,   816,   817,   818,
     821,   822,   823,   826,   827,   830,   831,   834,   835,   838,
     839,   840,   843,   846,   849,   852,   855,   858,   859,   862,
     863,   864,   865,   866,   867,   868,   869,   870,   871,   872,
     873,   874,   875,   876,   877,   878,   879,   880,   881,   882,
     883,   884,   885,   886,   887,   888,   889,   890,   891,   892,
     893,   894,   895,   896,   897,   898,   899,   900,   908,   909,
     910,   911,   912,   913,   914,   915,   916,   917,   918,   919,
     920,   921,   922,   923,   924,   925,   926,   927,   928,   929,
     930,   931,   932,   933,   934,   935,   936,   937,   938,   939,
     940,   941,   942,   943,   944,   945,   946,   947,   948,   949,
     950,   951,   952,   953,   954,   955,   956,   957,   958,   959,
     960,   961,   962,   963,   964,   965,   966,   967,   968,   969,
     970,   971,   972,   973,   974,   975,   976,   977,   978,   979,
     980,   981,   982,   983,   984,   985,   986,   987,   988,   989,
     990,   991,   992,   993,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,  1009,
    1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,
    1020,  1021,  1022,  1023,  1024,  1025,  1026,  1027,  1028,  1029,
    1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,  1039,
    1040,  1041,  1042,  1043,  1044,  1047,  1048,  1051,  1052,  1053,
    1054,  1055,  1056,  1057,  1058,  1066,  1067,  1068,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,  1100,
    1101,  1102
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
  "B256EDITVISIBLE", "B256GRAPHVISIBLE", "B256OUTPUTVISIBLE",
  "B256EDITSIZE", "B256OUTPUTSIZE", "B256LINENUM", "B256INTEGER",
  "B256FLOAT", "B256STRING", "B256VARIABLE", "B256STRINGVAR", "B256NEWVAR",
  "B256COLOR", "B256LABEL", "'<'", "'>'", "'='", "'-'", "'+'", "'*'",
  "'/'", "B256UMINUS", "'^'", "'\\n'", "':'", "'('", "','", "')'", "'['",
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
  "dbclosesetstmt", "netlistenstmt", "netconnectstmt", "netwritestmt",
  "netclosestmt", "killstmt", "setsettingstmt", "portoutstmt",
  "imgsavestmt", "editvisiblestmt", "graphvisiblestmt",
  "outputvisiblestmt", "immediatestrlist", "immediatelist", "floatlist",
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
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,    60,    62,    61,    45,    43,    42,    47,   467,
      94,    10,    58,    40,    44,    41,    91,    93,    59,   123,
     125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   232,   233,   233,   234,   234,   235,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   237,   238,   239,
     240,   240,   241,   242,   243,   243,   244,   245,   246,   247,
     247,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   249,   249,   249,   249,   250,   250,   250,   250,
     251,   252,   252,   253,   254,   254,   255,   256,   257,   258,
     258,   258,   258,   258,   258,   259,   259,   259,   259,   259,
     259,   260,   261,   262,   262,   263,   264,   265,   266,   267,
     268,   269,   269,   269,   270,   270,   270,   270,   270,   271,
     271,   272,   272,   273,   273,   274,   274,   275,   275,   275,
     275,   276,   276,   277,   277,   278,   279,   280,   280,   280,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   282,
     282,   282,   283,   283,   283,   284,   284,   284,   285,   285,
     285,   286,   286,   286,   287,   287,   287,   288,   288,   288,
     288,   288,   288,   288,   288,   288,   288,   289,   290,   290,
     290,   290,   290,   291,   292,   292,   293,   293,   294,   294,
     294,   294,   295,   295,   295,   295,   295,   295,   296,   297,
     297,   298,   298,   299,   299,   300,   300,   301,   302,   303,
     303,   304,   305,   306,   307,   307,   308,   309,   310,   310,
     311,   311,   311,   312,   312,   312,   312,   313,   313,   313,
     314,   314,   314,   315,   315,   316,   316,   317,   317,   318,
     318,   318,   319,   320,   321,   322,   323,   324,   324,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   326,   326,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   327,   327,
     327,   327
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     7,     7,     3,     3,     7,     7,
       2,     1,     1,     1,     4,     6,     1,     1,     2,     6,
       8,     3,     8,    10,     8,     6,     8,     3,     8,    10,
       8,     3,     3,     6,     8,     2,     2,     2,     1,     2,
       1,     6,     8,     2,     4,     2,     2,     6,     4,     4,
       6,     8,    10,     6,     8,     8,    10,     6,     8,     6,
       8,     6,     8,     2,     2,     2,     2,     2,     4,     2,
       8,    10,     8,     6,     8,     6,    10,    12,    10,     2,
       6,     4,     2,     6,     4,     2,     6,     4,     1,     3,
       2,     1,     3,     2,     2,     6,     4,     3,     6,     3,
       6,     2,     5,     7,     2,     5,     7,     2,     1,     2,
       2,     3,     3,     2,     1,     3,     1,     3,     6,     8,
       8,    10,     6,     8,     8,    10,    10,    12,     2,     4,
       6,    10,    12,     6,     8,     6,     8,     2,     2,     1,
       3,     2,     2,     2,     1,     3,     2,     2,     1,     3,
       2,     6,     4,     4,     6,     6,     8,     2,     6,     4,
       1,     3,     2,     2,     6,     6,     8,     4,     6,     2,
       6,     4,     2,     2,     2,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     4,
       4,     5,     5,     5,     5,     4,     6,     1,     4,     4,
       4,     4,     4,     4,     6,     8,    10,     6,     8,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     4,     4,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     4,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     6,     8,     1,
       3,     6,     4,     4,     4,     4,     4,     3,     4,     4,
       1,     3,     1,     3,     1,     3,     4,     4,     6,     8,
       6,     1,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     1,     4,     6,     1,     4,     4,     4,     4,     8,
       6,     6,    10,     1,     3,     4,     1,     3,     4,     1,
       3,     4,     1,     3,     1,     3,     1,     3,     4,     1,
       3,     4,     6,     4,     3,     8,    10,     8,     4,     6,
       4,     6
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      16,   208,     0,     0,     0,     0,     0,     0,     0,   113,
       0,   116,   111,   112,     0,    19,    20,     0,    24,    27,
       0,     0,     0,     0,     0,   188,   191,     0,     0,   140,
     117,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,   216,     0,   239,     0,     0,     0,   244,
       0,     0,   248,     0,   138,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     0,
       0,    16,     5,     7,     8,     9,    22,    10,    11,    26,
      12,    13,    14,    15,    29,    54,    55,    56,    50,    52,
      53,    51,    49,     0,    58,    57,    43,    44,    45,    46,
      31,    32,    33,    34,    35,    47,    65,    37,    42,    38,
      39,    66,    67,    68,    69,    70,    40,    41,    59,    60,
      61,    62,    63,    83,    48,     0,    36,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    64,    96,    97,    98,    99,   100,   101,   375,     0,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,     0,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   476,   352,   356,   358,   360,   362,   364,
     366,   368,   370,     0,     0,     0,     0,     0,     0,     0,
     372,     0,   348,   350,   377,   379,   381,   383,   385,   387,
     429,   389,   391,   393,   395,   397,   399,   401,   403,   405,
     407,   409,   411,   413,   415,   417,   419,   421,   423,   425,
     479,     0,     0,     0,     0,     0,   440,   482,   442,   484,
     486,   444,   489,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   451,   453,   308,   307,   461,   317,   464,     0,
       0,   210,   209,   317,   464,     0,   207,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,   169,     0,     0,
       0,     0,     0,     0,   118,    23,    28,     0,   135,     0,
       0,   179,     0,     0,   182,     0,   190,     0,   193,   136,
     137,    21,    25,     0,   143,     0,     0,     0,     0,   110,
     317,     0,   146,     0,     0,     0,   185,     0,     0,     0,
       0,   164,   163,   165,   166,     0,     0,     0,     0,   228,
       0,     0,     0,     0,     0,   237,   238,     0,     0,   213,
       0,     0,     0,   194,     0,   241,   242,   243,     0,   246,
     247,     0,   139,     0,   250,     0,     0,     0,     0,     0,
     257,     0,   262,     0,   263,     0,     0,     0,     0,     0,
     269,   272,   273,   274,     0,     0,     0,     0,     1,     2,
       4,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   294,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   212,     0,     0,     0,     0,     0,     0,
       0,   211,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   189,   192,     0,     0,     0,   102,
       0,   103,     0,   106,     0,   107,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   215,   217,     0,     0,
     240,   245,   249,     0,     0,     0,     0,     0,     0,     0,
       0,   261,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   127,   131,     0,     0,     0,     0,   121,   132,     0,
       3,   117,    30,    17,   199,   197,   376,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,     0,   347,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   477,     0,   353,     0,   357,   359,   361,
     363,   365,   367,   369,   371,     0,     0,     0,     0,     0,
       0,     0,   373,     0,     0,   349,   351,   378,   380,   382,
     384,   386,   388,   430,   390,   392,   394,   396,   398,   400,
     402,   404,   406,   408,   410,   412,   414,   416,   418,   420,
     422,   424,   426,   480,   494,     0,   437,     0,     0,     0,
     441,   483,   443,   485,   487,     0,   445,     0,   490,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   452,   454,
       0,     0,     0,     0,     0,     0,   279,   457,   305,   306,
     302,   291,   292,   293,   283,   284,   287,   288,   303,   304,
     301,   281,   280,   459,   282,   285,   286,   299,   300,   296,
     297,   298,   295,   460,   458,     0,     0,     0,   149,     0,
       0,     0,     0,   168,   276,     0,     0,     0,     0,     0,
       0,   114,     0,     0,   181,     0,   184,     0,     0,     0,
       0,     0,     0,   144,     0,   148,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,     0,     0,
       0,     0,     0,     0,   196,     0,   252,     0,     0,     0,
     253,     0,   259,     0,     0,     0,     0,   267,     0,   271,
       0,     0,     0,     0,     0,     0,     0,   455,     0,     0,
       0,     0,     0,     0,   475,   318,   319,   466,   322,     0,
       0,     0,   467,   468,     0,     0,   329,   330,   331,   332,
     333,   334,   335,   336,   343,   337,   338,   339,   340,   341,
     342,   323,   465,   320,   321,   478,   354,     0,     0,   432,
     433,   434,   435,   436,   374,   355,   493,   438,   439,   481,
     488,   446,   491,     0,   447,     0,     0,     0,     0,     0,
     500,     0,   498,     0,     0,   309,     0,   315,     0,     0,
     310,     0,   462,     0,   315,     0,   462,     0,     0,     0,
       0,     0,   278,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,   311,     0,   314,   312,     0,     0,
       0,   150,     0,   153,     0,     0,     0,   317,   175,     0,
       0,     0,   115,   133,   180,   183,     0,   141,     0,     0,
       0,     0,   147,   186,     0,   159,   157,     0,   161,     0,
     218,     0,     0,   222,     0,   230,     0,     0,   235,     0,
     233,   195,   251,     0,   254,   255,   258,   264,     0,   265,
     268,   270,     0,     0,     0,   125,     0,     0,   456,     0,
     119,   200,   198,   427,     0,     0,   470,   471,     0,   324,
       0,   327,     0,   431,   492,     0,     0,   448,     0,   450,
     501,   499,   316,   463,   316,   463,     0,     0,     0,   317,
       0,     0,     0,     0,     0,     0,   104,   105,   108,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   154,     0,
     155,   174,     0,   317,   172,     0,     0,   151,   134,   142,
     160,   158,   162,     0,   219,   220,     0,   223,     0,   224,
     236,     0,     0,   234,   256,   266,     0,   128,   130,   126,
       0,   122,   124,   120,   428,   469,     0,   325,   328,     0,
       0,   495,   449,   497,     0,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     156,   171,     0,   176,   178,   152,   221,     0,   225,   226,
       0,   231,   129,   123,   326,   472,   496,     0,     0,     0,
     177,   227,   232
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     627,   297,   551,   285,   856,   289
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -315
static const yytype_int16 yypact[] =
{
   11547,  4673,  4852,  5031,  5210,  5389,  -202,  5568,  5747,  -315,
    5926,  -315,  -315,  -315,  4673,  -315,  -315,  4673,  -315,  -315,
    4673,  -195,  -178,  6105,  6284,  6463,  6642,  -176,  -168,  -315,
       9,  6821,  -190,  -153,  4673,  2304,  7000,  7179,  7358,  4673,
    4673,  4673,  7537,  7716,  4673,  7895,  8074,  4673,  4673,  4673,
    8253,  4673,  -160,  -154,  8432,  -132,  4673,  4673,  4673,   -99,
    4673,  4673,   -91,   -85,  -315,  8611,  8790,  8969,  9148,  9327,
    9506,  9685,  9864,  4673,  4673,  4673,  -206,  -200,  -315,   142,
     -73, 11753,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,   -78,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,   136,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,   -59,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,   -56,   -49,
       1,     3,    13,    16,    23,    24,    25,    34,    36,    38,
      39,    40,    41,    49,    54,    61,    64,    67,    73,    75,
      77,    79,    80,    81,    82,    83,    84,    86,    87,  4673,
      89,    90,    92,    94,    95,   105,   106,   111,   112,   114,
     116,   117,   118,   119,   120,   122,   124,   125,   127,   129,
     130,   131,   132,   134,   135,   143,   144,   147,   148,   150,
     152,   153,   155,   156,   158,   160,   162,   164,   169,   172,
     174,   179,   186,   187,   188,   189,   190,   191,   192,   193,
     196,   204,   211,   212,   217,   218,   219,   223,   234,   235,
     236,   237,   238,   242,   244,   245,  4673,   246,   247,   248,
     249,   253,   254,   255,  -315,  -315,  -315,    62,   133,  4673,
    4673,  1410,    17,  -164,  -146, 18843,   960,  4673, 16010,   960,
    4673, 16046,  4673, 16125,  -315,   266,  4673,  -315,  4673, 16161,
    4673, 16197,  4673, 16211, 18843, 18843, 18843,   151,  -315,  4673,
   16247,   960,  4673, 16283,   960,  2883, 18843,  3062, 18843,  -315,
    -315,  -315,  -315,  4673, 16361, 10043, 10222, 10401, 10580, 18843,
    -143, 10759,  -315, 16397,  4673, 16433,   960,  4673, 16447,  4673,
    1125, 18843,   960,   960, 18843,  4673, 16483,  4673, 16519, 18843,
    4673, 16597,  4673, 16633, 16669, 18843, 18843,  4673, 16683,   960,
     256,   257,  4673, 16719,   258,   960, 18843,   960,   259,   960,
     960,   260,  -315,  4673, 16755,  4673, 16833,  1517,  4673, 16869,
     960,  3241, 18843,  4673,   960,  4673,  1707,  4673, 16905,  4673,
    3303, 18843, 18843, 18843,  1519,  4673,  2111,  4673,  -315, 11333,
    -315, 11954, 11954,  -126,   261,  4673,  4673,  3420,  4673,  4673,
    4673,  4673,  4673,  4673,  4673,  4673,  4673,  4673,  4673,  4673,
     262,  4673,  4673,  4673,  4673,  4673,  4673,  4673,   267,  4673,
    4673,  4673,  4673,  4673,  4673,   271,  4673,  4673,  4673,  3599,
    3778,   268,   269,   272,   273,   274,   276,   277,   278,  4673,
    4673,  4673,  4673,  4673,  4673,  4673,  3957,  4673,   279,   283,
     286,   287,   288,   310,   312,   313,   315,   318,   322,   324,
     325,   326,   327,   329,   331,   337,   339,   340,   343,   345,
     353,   354,   355,   357,   359,   361,   363,  4136,   364,  4673,
    4673,  4673,   365,   368,   370,   371,  4315,  4494,   372,  4673,
    4673,  4673,   271,  4673,  4673,  4673,  4673,  -118,   374,   375,
      45,   569,   271, 13288,   231,  4673,  4673,  4673,  4673,  4673,
    4673,  4673,  4673,  4673,  4673,  4673,  4673,  4673,  4673,  4673,
    4673,  4673,  4673,  -315,  4673,  4673,  4673,  4673,  4673,  4673,
    4673,  -315,    45,   569,  1677,  4673, 12262,  4673, 12276,  4673,
     377,   376, 16919, 12326,  4673, 12351,  4673, 12422,  4673,  4673,
   12441,  4673, 12505,  4673,  -315,  -315, 12541,  4673,  4673, 18843,
    4673, 18843,  4673, 18843,  4673, 18843,   -71, 12556,  4673, 12606,
    4673, 12620,  4673,    42,  4673, 12656,  4673, 12707,  4673, 12771,
    4673, 12807,  4673,  4673, 12821,  4673,  -315,  -315, 12858,  4673,
    -315,  -315,  -315, 12872,  4673, 12894,   107,  4673,  4673, 12972,
    4673,  -315, 13051,   250,  4673, 13073,  4673,   463,  4673,   381,
     382,  -315, 18843,  1725,   388,   389,  4673,  -315,   960,  1743,
    -315,  -315,  -315,   -78,   387,   390,  -315, 16955, 16991,  -315,
   13324, 13338,   320, 13360,   347,  3482,  3661,  3840,   566,   772,
    4019,  4198, 13401, 13439,  -315, 13517, 13553, 13590, 13604, 13640,
   13676, 13755,  -315, 13791, 13827, 13841, 13877, 13913, 13992,   851,
   14028, 14064,   924,  -315, 14078,  -315, 14114,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315, 17069, 17105, 14150, 14229, 14265,
   14301, 14315,  -315, 14351,   931,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  1157,  -315, 14387, 14466, 14502,
    -315,  -315,  -315,  -315,  -315, 14538,  -315, 14552,  -315,  1201,
    4377, 14588,  4556,  4735,  4914,  5093,   -61,   -23,  -315,  -315,
     383,  -156,  1761,   413,   -94,  1837,  -315,  -315,   183,   183,
     183, 18868, 18879, 18829,   271,   271,    65,    65,   183,   183,
     183,   -77,   -77,  -315,   271,   271,  -315,   263,   263,   263,
     263,   263,   263,   -77,  -315, 11596, 11792,  4673, 18843,  4673,
   17141,  4673, 17155,  -315,  -315,  4673,  4673, 17191,  4673, 17227,
    4673, 18843,   998,  4673,   960,  4673,   960,  4673, 17305, 13087,
   13123, 13137, 13173,  -315,  4673, 18843,  4673,   960,  4673, 17341,
    4673, 17377,  4673, 17391,  4673, 17427,  4673,   960,  4673, 17463,
   17541,  4673, 17577,  4673, 18843,  4673, 18843,  4673,  4673,  5272,
   18843,  4673,   960,  4673,  4673,  5451,  4673, 18843,  4673,   960,
    4673,  4673,  4673,   431,  4673,  4673,   416,  5630,  4673,   434,
    4673,  4673,  4673,  4673,  -315,  -315,  -315,  -315,  -315,  4673,
    4673,  4673,  -315,  -315,  4673,  4673,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  4673,  4673,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  4673,  -315,  4673,  4673,  4673,  4673,  4673,
    -315,  4673,  -315,   422,   427,  -315,  4673,  -315,   429,   430,
    -315,  4673,  -315,  4673,   -18,  4673,   -16, 14624, 17613,  4673,
   17627,  4673,  -315, 17663,  2497, 17699,  4673, 14703,  4673,  1216,
    1222, 17777,  4673,  4673,  4673,  4673,  4673, 14739,  1303, 17813,
    4673, 17849,  4673, 17863,  4673, 17899,  4673, 17935,  1661, 18013,
    4673,  4673, 18049,  4673, 14775, 14789,  5809, 14825,  4673,  1780,
    1785,  5988,  4673, 14861,  1790,  6167,  6346, 11811,  4673,  6525,
    6704,  -315,  4673, 11993,  4673, 12012, 12080, 14940, 18085, 18099,
   14976, 15012,   630,   806, 18135, 15026,  1873,  6883,   835,  7062,
    1922,  2004,  2108,  -315,  -315, 12097,  -315,  -315, 12116, 12172,
   12192,  -315,  4673, 18843,  4673, 18171, 10938,  -134,  -315, 18249,
    4673, 18285,  -315,  1148,  -315,  -315,  4673, 18843, 15062, 15098,
   15177, 15213,  -315,  -315,  4673, 18843,   960,  4673, 18843,  4673,
    7241,  4673, 18321,   960,  4673,  -315,  4673, 18335, 18843,  4673,
   18843,  -315,  -315,  4673,  -315, 18843,  -315,  -315,  4673,   960,
    -315,  -315,  4673,  4673,   450, 18843,  4673,  4673,  -315,   451,
     960,  -315,  -315,  -315,  4673,  4673,  -315,  -315,  4673,  -315,
    4673,  -315,  4673,  -315,  -315,  4673,  4673,  -315,  4673,  -315,
    -315,  -315,  -315,  -315,   -10,    -4, 15249, 18371,  4673,    -5,
   18407,  2690, 18485,  4673,  4673, 15263,  -315,  -315,  -315,  -315,
   15299,  2301, 15335,   856,  4673, 18521,  2380,  4673, 15414, 18557,
    4673, 15450, 15486,  2494,   880,  2573,  4673,   906,  2687,  4673,
   15500, 15536, 13238, 15572, 18571,  1056, 15651,  2766,  -315,  4673,
   18843,  -315, 11117,  -101,  -315, 18607,  4673, 18843, 18843,  -315,
    -315,  -315,  -315,  4673,  -315, 18843,  4673,  -315, 18643,   960,
    -315,  4673, 18721,  -315,  -315,  -315,  4673,  -315,  -315, 18843,
    4673,  -315,  -315,   960,  -315,  -315,  4673,  -315,  -315,  4673,
    4673,  -315,  -315,  -315, 15687,    18, 18757,  -192, 15723, 15737,
   18793,  2945,  4673, 18807,  4673, 15773, 15809, 15888, 15924, 15960,
    -315,  -315,   459,  -315,  -315,  -315,  -315,  4673,  -315,   960,
    4673, 18843,  -315,  -315,  -315,  -315,  -315,   442,  3124, 15974,
    -315,  -315,  -315
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -315,   281,  -315,  -315,   399,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,   289,   297,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,  -315,
    -315,   -34,  -127,    -1,  -314,   940
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -207
static const yytype_int16 yytable[] =
{
     281,   332,   288,   291,   293,   294,   299,   301,   394,   303,
     521,   522,   307,   304,   396,  1213,   305,   325,   326,   306,
     395,   295,   310,   313,   316,   318,   397,   296,   321,   308,
     324,   319,   322,   329,   333,   335,   338,   296,   341,   320,
     344,   346,   348,   349,   351,   353,   354,   355,   356,   358,
     168,   169,   170,   363,   327,   328,   366,  -204,  -204,   534,
     535,   536,   542,   360,   374,   376,   379,   382,   924,   361,
     388,   925,   391,   392,   393,  -201,  -201,   171,  -145,  -145,
     543,   634,   635,   510,   534,   535,   536,  -173,  -173,   746,
     747,   364,   510,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
    -170,  -170,   372,   199,   368,   510,   200,   201,   202,   203,
     929,   204,   371,   930,   205,   206,   207,   208,   209,   210,
     530,   531,   398,   532,   401,   211,   212,   213,   399,   534,
     535,   536,   521,   522,   813,   510,   402,   214,   215,   216,
     217,   218,   219,   919,   920,   403,   220,   404,   221,   222,
     223,   224,   225,   226,   405,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   435,   250,
     251,   921,   922,  -205,  -205,  -202,  -202,   252,   253,   254,
     255,  -206,  -206,   256,   257,   258,   259,  -203,  -203,   260,
    1151,   510,   261,   262,   406,   263,   407,   264,   265,   537,
     538,   539,   266,   540,   267,   268,   408,   269,   270,   409,
     271,   272,   273,  1211,   510,   541,   410,   411,   412,   274,
     275,   276,   277,   278,   537,   538,   539,   413,   540,   414,
     279,   415,   416,   417,   418,   502,   820,   757,   280,   750,
     521,   522,   419,   534,   535,   536,   751,   420,   512,   513,
     528,   529,   530,   531,   421,   532,   544,   422,   510,   546,
     423,   548,   534,   535,   536,   552,   424,   553,   425,   555,
     426,   557,   427,   428,   429,   430,   431,   432,   560,   433,
     434,   562,   436,   437,   513,   438,   513,   439,   440,   537,
     538,   539,   566,   540,   569,   571,   573,   575,   441,   442,
     577,   838,   757,   579,   443,   444,   581,   445,   513,   446,
     447,   448,   449,   450,   585,   451,   587,   452,   453,   589,
     454,   591,   455,   456,   457,   458,   594,   459,   460,   511,
     621,   598,   534,   535,   536,   559,   461,   462,   523,   524,
     463,   464,   603,   465,   605,   466,   467,   609,   468,   469,
     513,   470,   612,   471,   513,   472,   615,   473,   513,   534,
     535,   536,   474,   622,   623,   475,   629,   476,   528,   529,
     530,   531,   477,   532,   637,   638,   640,   641,   643,   478,
     479,   480,   481,   482,   483,   484,   485,   652,   653,   486,
     655,   656,   657,   658,   659,   660,   661,   487,   663,   664,
     665,   666,   667,   668,   488,   489,   670,   671,   674,   676,
     490,   491,   492,   537,   538,   539,   493,   540,   685,   686,
     687,   688,   689,   690,   691,   693,   757,   494,   495,   496,
     497,   498,   537,   538,   539,   499,   540,   500,   501,   503,
     504,   505,   506,   550,   844,   757,   507,   508,   509,   540,
     400,   596,   597,   600,   601,   602,   636,   654,   727,   728,
     729,   532,   662,   677,   678,   735,   737,   679,   680,   681,
     741,   682,   683,   684,   695,   534,   535,   536,   696,   752,
     755,   697,   698,   699,   758,   759,   760,   761,   762,   763,
     764,   765,   766,   767,   768,   769,   770,   771,   772,   774,
     775,   776,   537,   538,   539,   700,   540,   701,   702,   783,
     703,   785,   786,   704,   788,   866,   790,   705,   792,   706,
     707,   708,   709,   797,   710,   799,   711,   801,   802,   537,
     538,   539,   712,   540,   713,   714,   808,   809,   715,   810,
     716,   811,   868,   812,   168,   169,   170,   815,   717,   718,
     719,   819,   720,   821,   721,   823,   722,   825,   723,   726,
     730,   829,   830,   731,   832,   732,   733,   738,   834,   748,
     749,   171,   793,   836,   850,   851,   794,   840,   534,   535,
     536,   854,   855,   860,   923,   847,   861,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   928,   988,   991,   199,   994,  1013,
     200,   201,   202,   203,  1014,   204,  1016,  1017,   205,   206,
     207,   208,   209,   210,  1136,  1139,  1227,  1230,   942,   211,
     212,   213,   534,   535,   536,   537,   538,   539,  1078,   540,
     630,   214,   215,   216,   217,   218,   219,   848,   757,     0,
     220,   633,   221,   222,   223,   224,   225,   226,   632,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,     0,     0,     0,   266,     0,   267,   268,
       0,   269,   270,     0,   271,   272,   273,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,   278,   537,   538,
     539,     0,   540,     0,   279,     0,   937,     0,   938,     0,
     940,   872,   280,   753,   552,   943,     0,   945,     0,   947,
     754,     0,     0,     0,     0,     0,   951,     0,     0,     0,
       0,     0,     0,   957,   534,   535,   536,   959,     0,   961,
       0,   963,     0,   965,     0,   967,     0,   969,     0,     0,
     972,     0,   974,     0,   975,     0,     0,   977,     0,     0,
       0,     0,   537,   538,   539,   983,   540,     0,   534,   535,
     536,   987,     0,     0,  1088,  1089,     0,   993,     0,   995,
     996,   997,   998,     0,     0,     0,     0,     0,   999,  1000,
    1001,     0,     0,     0,     0,     0,     0,   534,   535,   536,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   534,   535,   536,  1004,  1005,   534,   535,
     536,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1028,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   534,   535,   536,  1015,     0,     0,     0,     0,
    1018,     0,  1019,     0,  1020,     0,     0,     0,  1023,     0,
    1025,   282,   286,  1029,     0,  1031,     0,  1033,   534,   535,
     536,  1037,  1038,  1039,  1040,  1041,     0,     0,     0,  1045,
       0,  1048,     0,   311,   314,  1052,   534,   535,   536,  1057,
    1058,     0,  1060,   534,   535,   536,   336,  1065,   340,   342,
     343,     0,     0,     0,   537,   538,   539,  1075,   540,     0,
       0,   359,     0,     0,     0,     0,   365,   873,   367,     0,
     369,   370,   534,   535,   536,     0,   377,   380,     0,   384,
     386,     0,   390,     0,     0,     0,     0,     0,   537,   538,
     539,  1106,   540,  1107,     0,  1110,   948,     0,     0,  1112,
    1090,  1091,     0,     0,     0,  1115,     0,     0,     0,     0,
     515,   516,   517,  1120,     0,     0,  1122,   537,   538,   539,
    1125,   540,     0,  1128,     0,  1129,     0,     0,  1131,  1096,
    1097,     0,  1132,   537,   538,   539,     0,   540,   537,   538,
     539,     0,   540,   518,   519,   520,   891,  1154,     0,     0,
    1163,  1164,     0,  1140,  1141,   521,   522,  1142,     0,  1143,
       0,  1144,   537,   538,   539,  1146,   540,     0,   534,   535,
     536,     0,     0,     0,  1176,  1177,     0,  1150,     0,     0,
    1155,     0,  1157,  1158,     0,     0,     0,     0,   537,   538,
     539,     0,   540,  1165,     0,     0,  1168,     0,     0,  1172,
    1180,  1181,     0,     0,     0,  1179,   537,   538,   539,     0,
     540,     0,     0,   537,   538,   539,     0,   540,  1194,   894,
       0,  1196,     0,     0,     0,  1198,   905,     0,     0,     0,
       0,     0,  1199,  1214,     0,  1200,     0,   534,   535,   536,
    1203,     0,   537,   538,   539,  1205,   540,  1114,     0,  1206,
       0,     0,     0,   523,   524,  1207,     0,     0,  1208,  1209,
     515,   516,   517,     0,     0,     0,     0,     0,     0,   534,
     535,   536,     0,  1221,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,  1229,
     514,     0,     0,   518,   519,   520,     0,   514,     0,     0,
     514,     0,   514,     0,     0,   521,   522,     0,   514,     0,
     514,     0,   514,   534,   535,   536,     0,     0,     0,   514,
       0,     0,   514,     0,     0,   514,     0,   514,   534,   535,
     536,     0,     0,   514,   534,   535,   536,     0,   537,   538,
     539,   514,   540,     0,   514,     0,     0,   514,     0,   583,
    1190,  1191,     0,     0,     0,   514,     0,   514,     0,     0,
     514,     0,   514,     0,     0,     0,     0,   514,     0,     0,
       0,     0,   514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   514,     0,   606,     0,     0,   514,     0,
       0,   514,     0,   514,     0,   613,     0,   514,     0,   617,
       0,     0,     0,   523,   524,     0,   628,   537,   538,   539,
       0,   540,     0,     0,     0,   534,   535,   536,   642,   584,
     644,   645,   646,   647,   648,   649,   650,   651,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   537,
     538,   539,     0,   540,     0,     0,   669,     0,   672,     0,
       0,     0,   906,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   694,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   537,   538,   539,     0,   540,     0,     0,
       0,     0,     0,     0,     0,     0,   912,   725,   537,   538,
     539,     0,   540,     0,   537,   538,   539,     0,   540,   739,
     740,  1034,     0,   742,   743,   744,   745,  1035,     0,     0,
       0,     0,   515,   516,   517,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   773,
       0,     0,     0,     0,   777,   778,   779,   780,   781,   782,
     784,     0,     0,     0,     0,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   521,   522,     0,
       0,   804,     0,   806,     0,     0,     0,     0,   514,     0,
     514,     0,   514,     0,   514,   537,   538,   539,     0,   540,
     817,     0,     0,     0,   168,   169,   170,     0,  1043,     0,
     827,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   839,     0,     0,
     842,   171,     0,     0,   845,     0,     0,     0,   849,   534,
     535,   536,     0,     0,     0,     0,   857,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,   523,   524,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,   211,
     212,   213,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   214,   215,   216,   217,   218,   219,     0,   533,     0,
     220,     0,   221,   222,   223,   224,   225,   226,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,   534,   535,   536,   266,     0,   267,   268,
     619,   269,   270,   620,   271,   272,   273,     0,     0,   515,
     516,   517,     0,   274,   275,   276,   277,   278,     0,   537,
     538,   539,     0,   540,   279,     0,     0,     0,     0,     0,
       0,   608,   280,   949,     0,   950,     0,     0,   296,   534,
     535,   536,   518,   519,   520,     0,   958,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   968,   515,   516,   517,
       0,     0,     0,     0,     0,     0,     0,   976,     0,     0,
       0,   979,     0,   980,   981,   515,   516,   517,   984,     0,
     985,   986,     0,     0,   989,   990,     0,     0,     0,     0,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,  1002,  1003,     0,     0,   518,   519,
     520,     0,   534,   535,   536,     0,     0,   534,   535,   536,
     521,   522,   534,   535,   536,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,  1006,     0,  1007,  1008,  1009,  1010,  1011,
       0,  1012,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   537,   538,   539,     0,   540,     0,   515,
     516,   517,     0,     0,     0,     0,  1055,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
    1046,   787,   756,     0,  1050,     0,  1053,     0,     0,     0,
     523,   524,   518,   519,   520,   534,   535,   536,     0,   537,
     538,   539,  1069,   540,   521,   522,     0,     0,   523,   524,
       0,   614,   857,     0,  1080,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   852,
       0,     0,   853,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   534,   535,   536,   858,     0,     0,
     859,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,  1121,   926,     0,     0,   927,  1123,
       0,  1126,   537,   538,   539,     0,   540,   537,   538,   539,
       0,   540,   537,   538,   539,  1066,   540,     0,  1133,     0,
    1067,     0,  1134,  1135,     0,  1071,  1137,  1138,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1145,     0,     0,  1147,     0,
       0,     0,     0,     0,     0,     0,   534,   535,   536,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,   931,     0,     0,   932,     0,     0,  1169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1183,
       0,     0,     0,     0,     0,   537,   538,   539,     0,   540,
       0,     0,     0,     0,     0,     0,     0,     0,  1094,     0,
       0,     0,     0,     0,     0,     0,  1201,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   537,   538,   539,     0,   540,     0,
       0,     0,  1219,   171,     0,     0,     0,  1099,     0,     0,
     534,   535,   536,     0,     0,     0,     0,  1228,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,     0,     0,     0,     0,
       0,   211,   212,   213,     0,     0,   537,   538,   539,     0,
     540,     0,     0,   214,   215,   216,   217,   218,   219,  1100,
       0,     0,   220,     0,   221,   222,   223,   224,   225,   226,
       0,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,   250,   251,     0,     0,     0,
       0,     0,     0,   252,   253,   254,   255,     0,     0,   256,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   263,     0,   264,   265,     0,     0,     0,   266,     0,
     267,   268,   624,   269,   270,   625,   271,   272,   273,   168,
     169,   170,     0,     0,     0,   274,   275,   276,   277,   278,
     537,   538,   539,     0,   540,     0,   279,     0,     0,     0,
       0,     0,     0,  1101,   280,     0,   171,     0,     0,     0,
     626,     0,     0,   534,   535,   536,     0,     0,     0,     0,
       0,     0,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   199,     0,     0,   200,   201,   202,   203,     0,
     204,     0,     0,   205,   206,   207,   208,   209,   210,     0,
       0,     0,     0,     0,   211,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214,   215,   216,   217,
     218,   219,   534,   535,   536,   220,     0,   221,   222,   223,
     224,   225,   226,     0,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,     0,   250,   251,
       0,     0,     0,     0,     0,     0,   252,   253,   254,   255,
       0,     0,   256,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,     0,   263,     0,   264,   265,     0,     0,
       0,   266,     0,   267,   268,     0,   269,   270,     0,   271,
     272,   273,   168,   169,   170,     0,     0,     0,   274,   275,
     276,   330,   278,   537,   538,   539,     0,   540,     0,   279,
       0,     0,     0,     0,     0,     0,  1161,   331,     0,   171,
       0,     0,     0,   296,     0,     0,   534,   535,   536,     0,
       0,     0,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,     0,     0,   199,     0,     0,   200,   201,
     202,   203,     0,   204,     0,     0,   205,   206,   207,   208,
     209,   210,   537,   538,   539,     0,   540,   211,   212,   213,
       0,     0,     0,     0,     0,  1167,     0,     0,     0,   214,
     215,   216,   217,   218,   219,   534,   535,   536,   220,     0,
     221,   222,   223,   224,   225,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,   250,   251,     0,     0,     0,     0,     0,     0,   252,
     253,   254,   255,     0,     0,   256,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   263,     0,   264,
     265,     0,     0,     0,   266,     0,   267,   268,     0,   269,
     270,     0,   271,   272,   273,   168,   169,   170,     0,     0,
       0,   274,   275,   276,  1027,   278,   537,   538,   539,     0,
     540,     0,   279,     0,     0,     0,     0,     0,     0,  1175,
     280,     0,   171,     0,     0,     0,   296,     0,     0,   534,
     535,   536,     0,     0,     0,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,     0,     0,   199,     0,
       0,   200,   201,   202,   203,     0,   204,     0,     0,   205,
     206,   207,   208,   209,   210,   537,   538,   539,     0,   540,
     211,   212,   213,     0,     0,     0,     0,     0,  1178,     0,
       0,     0,   214,   215,   216,   217,   218,   219,   534,   535,
     536,   220,     0,   221,   222,   223,   224,   225,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,   250,   251,     0,     0,     0,     0,
       0,     0,   252,   253,   254,   255,     0,     0,   256,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,     0,
     263,     0,   264,   265,     0,     0,     0,   266,     0,   267,
     268,     0,   269,   270,     0,   271,   272,   273,   168,   169,
     170,     0,     0,     0,   274,   275,   276,  1153,   278,   537,
     538,   539,     0,   540,     0,   279,     0,     0,     0,     0,
       0,     0,  1182,   280,     0,   171,     0,     0,     0,   296,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,     0,
       0,   199,     0,     0,   200,   201,   202,   203,     0,   204,
       0,     0,   205,   206,   207,   208,   209,   210,   537,   538,
     539,     0,   540,   211,   212,   213,     0,   534,   535,   536,
       0,  1193,     0,     0,     0,   214,   215,   216,   217,   218,
     219,     0,     0,     0,   220,     0,   221,   222,   223,   224,
     225,   226,     0,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,     0,   250,   251,     0,
       0,     0,     0,     0,     0,   252,   253,   254,   255,     0,
       0,   256,   257,   258,   259,     0,     0,   260,     0,     0,
     261,   262,     0,   263,     0,   264,   265,   168,   169,   170,
     266,     0,   267,   268,     0,   269,   270,     0,   271,   272,
     273,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   278,     0,     0,   171,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,   280,     0,   564,     0,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,     0,
     199,     0,     0,   200,   201,   202,   203,     0,   204,     0,
       0,   205,   206,   207,   208,   209,   210,   537,   538,   539,
       0,   540,   211,   212,   213,     0,   534,   535,   536,     0,
    1218,     0,     0,     0,   214,   215,   216,   217,   218,   219,
       0,     0,     0,   220,     0,   221,   222,   223,   224,   225,
     226,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,     0,   250,   251,     0,     0,
       0,     0,     0,     0,   252,   253,   254,   255,     0,     0,
     256,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,   263,     0,   264,   265,   168,   169,   170,   266,
       0,   267,   268,     0,   269,   270,     0,   271,   272,   273,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   277,
     278,     0,     0,   171,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,   280,     0,   565,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,   537,   538,   539,     0,
     540,   211,   212,   213,     0,   534,   535,   536,     0,  1231,
       0,     0,     0,   214,   215,   216,   217,   218,   219,     0,
       0,     0,   220,     0,   221,   222,   223,   224,   225,   226,
       0,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,   250,   251,     0,     0,     0,
       0,     0,     0,   252,   253,   254,   255,     0,     0,   256,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   263,     0,   264,   265,   168,   169,   170,   266,     0,
     267,   268,     0,   269,   270,     0,   271,   272,   273,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   277,   278,
       0,     0,   171,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,   280,     0,   611,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,     0,     0,   199,     0,
       0,   200,   201,   202,   203,     0,   204,     0,     0,   205,
     206,   207,   208,   209,   210,   537,   538,   539,     0,   540,
     211,   212,   213,     0,   534,   535,   536,   618,     0,     0,
       0,     0,   214,   215,   216,   217,   218,   219,     0,     0,
       0,   220,     0,   221,   222,   223,   224,   225,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,   250,   251,     0,     0,     0,     0,
       0,     0,   252,   253,   254,   255,     0,     0,   256,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,     0,
     263,     0,   264,   265,   168,   169,   170,   266,     0,   267,
     268,     0,   269,   270,     0,   271,   272,   273,     0,     0,
       0,     0,     0,     0,   274,   275,   276,   277,   278,     0,
       0,   171,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,   280,     0,   639,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,   537,   538,   539,     0,   540,   211,
     212,   213,     0,   534,   535,   536,   869,     0,     0,     0,
       0,   214,   215,   216,   217,   218,   219,     0,     0,     0,
     220,     0,   221,   222,   223,   224,   225,   226,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,   168,   169,   170,   266,     0,   267,   268,
       0,   269,   270,     0,   271,   272,   273,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,   278,     0,     0,
     171,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,     0,   280,     0,   673,     0,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,     0,     0,   199,     0,     0,   200,
     201,   202,   203,     0,   204,     0,     0,   205,   206,   207,
     208,   209,   210,   537,   538,   539,     0,   540,   211,   212,
     213,     0,   534,   535,   536,   870,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,     0,     0,     0,   220,
       0,   221,   222,   223,   224,   225,   226,     0,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,     0,   250,   251,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   255,     0,     0,   256,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,     0,   263,     0,
     264,   265,   168,   169,   170,   266,     0,   267,   268,     0,
     269,   270,     0,   271,   272,   273,     0,     0,     0,     0,
       0,     0,   274,   275,   276,   277,   278,     0,     0,   171,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,   280,     0,   675,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,     0,     0,   199,     0,     0,   200,   201,
     202,   203,     0,   204,     0,     0,   205,   206,   207,   208,
     209,   210,   537,   538,   539,     0,   540,   211,   212,   213,
       0,   534,   535,   536,   871,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,     0,     0,     0,   220,     0,
     221,   222,   223,   224,   225,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,   250,   251,     0,     0,     0,     0,     0,     0,   252,
     253,   254,   255,     0,     0,   256,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   263,     0,   264,
     265,   168,   169,   170,   266,     0,   267,   268,     0,   269,
     270,     0,   271,   272,   273,     0,     0,     0,     0,     0,
       0,   274,   275,   276,   277,   278,     0,     0,   171,     0,
       0,     0,   279,     0,     0,     0,     0,     0,     0,     0,
     280,     0,   692,     0,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,     0,     0,   199,     0,     0,   200,   201,   202,
     203,     0,   204,     0,     0,   205,   206,   207,   208,   209,
     210,   537,   538,   539,     0,   540,   211,   212,   213,     0,
     534,   535,   536,   874,     0,     0,     0,     0,   214,   215,
     216,   217,   218,   219,     0,     0,     0,   220,     0,   221,
     222,   223,   224,   225,   226,     0,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,     0,
     250,   251,     0,     0,     0,     0,     0,     0,   252,   253,
     254,   255,     0,     0,   256,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,     0,   263,     0,   264,   265,
     168,   169,   170,   266,     0,   267,   268,     0,   269,   270,
       0,   271,   272,   273,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   277,   278,     0,     0,   171,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,   280,
       0,   724,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
     537,   538,   539,     0,   540,   211,   212,   213,     0,   534,
     535,   536,   875,     0,     0,     0,     0,   214,   215,   216,
     217,   218,   219,     0,     0,     0,   220,     0,   221,   222,
     223,   224,   225,   226,     0,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,     0,   250,
     251,     0,     0,     0,     0,     0,     0,   252,   253,   254,
     255,     0,     0,   256,   257,   258,   259,     0,     0,   260,
       0,     0,   261,   262,     0,   263,     0,   264,   265,   168,
     169,   170,   266,     0,   267,   268,     0,   269,   270,     0,
     271,   272,   273,     0,     0,     0,     0,     0,     0,   274,
     275,   276,   277,   278,     0,     0,   171,     0,     0,     0,
     279,     0,     0,     0,     0,     0,     0,     0,   280,     0,
     734,     0,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   199,     0,     0,   200,   201,   202,   203,     0,
     204,     0,     0,   205,   206,   207,   208,   209,   210,   537,
     538,   539,     0,   540,   211,   212,   213,     0,   534,   535,
     536,   913,     0,     0,     0,     0,   214,   215,   216,   217,
     218,   219,     0,     0,     0,   220,     0,   221,   222,   223,
     224,   225,   226,     0,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,     0,   250,   251,
       0,     0,     0,     0,     0,     0,   252,   253,   254,   255,
       0,     0,   256,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,     0,   263,     0,   264,   265,   168,   169,
     170,   266,     0,   267,   268,     0,   269,   270,     0,   271,
     272,   273,     0,     0,     0,     0,     0,     0,   274,   275,
     276,   277,   278,     0,     0,   171,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,   280,     0,   736,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,     0,
       0,   199,     0,     0,   200,   201,   202,   203,     0,   204,
       0,     0,   205,   206,   207,   208,   209,   210,   537,   538,
     539,     0,   540,   211,   212,   213,     0,   534,   535,   536,
     915,     0,     0,     0,     0,   214,   215,   216,   217,   218,
     219,     0,     0,     0,   220,     0,   221,   222,   223,   224,
     225,   226,     0,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,     0,   250,   251,     0,
       0,     0,     0,     0,     0,   252,   253,   254,   255,     0,
       0,   256,   257,   258,   259,     0,     0,   260,     0,     0,
     261,   262,     0,   263,     0,   264,   265,   168,   169,   170,
     266,     0,   267,   268,     0,   269,   270,     0,   271,   272,
     273,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   278,     0,     0,   171,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,   280,     0,     0,     0,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,     0,
     199,     0,     0,   200,   201,   202,   203,     0,   204,     0,
       0,   205,   206,   207,   208,   209,   210,   537,   538,   539,
       0,   540,   211,   212,   213,     0,   534,   535,   536,   916,
       0,     0,     0,     0,   214,   215,   216,   217,   218,   219,
       0,     0,     0,   220,     0,   221,   222,   223,   224,   225,
     226,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,     0,   250,   251,     0,     0,
       0,     0,     0,     0,   252,   253,   254,   255,     0,     0,
     256,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,   263,     0,   264,   265,   168,   169,   170,   266,
       0,   267,   268,     0,   269,   270,     0,   271,   272,   273,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   283,
     284,     0,     0,   171,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,   280,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,   537,   538,   539,     0,
     540,   211,   212,   213,     0,   534,   535,   536,   917,     0,
       0,     0,     0,   214,   215,   216,   217,   218,   219,     0,
       0,     0,   220,     0,   221,   222,   223,   224,   225,   226,
       0,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,   250,   251,     0,     0,     0,
       0,     0,     0,   252,   253,   254,   255,     0,     0,   256,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   263,     0,   264,   265,   168,   169,   170,   266,     0,
     267,   268,     0,   269,   270,     0,   271,   272,   273,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   277,   278,
       0,     0,   171,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,   287,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,     0,     0,   199,     0,
       0,   200,   201,   202,   203,     0,   204,     0,     0,   205,
     206,   207,   208,   209,   210,   537,   538,   539,     0,   540,
     211,   212,   213,     0,   534,   535,   536,   918,     0,     0,
       0,     0,   214,   215,   216,   217,   218,   219,     0,     0,
       0,   220,     0,   221,   222,   223,   224,   225,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,   250,   251,     0,     0,     0,     0,
       0,     0,   252,   253,   254,   255,     0,     0,   256,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,     0,
     263,     0,   264,   265,   168,   169,   170,   266,     0,   267,
     268,     0,   269,   270,     0,   271,   272,   273,     0,     0,
       0,     0,     0,     0,   274,   275,   276,   277,   278,     0,
       0,   171,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,   290,     0,     0,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,   537,   538,   539,     0,   540,   211,
     212,   213,     0,   534,   535,   536,   978,     0,     0,     0,
       0,   214,   215,   216,   217,   218,   219,     0,     0,     0,
     220,     0,   221,   222,   223,   224,   225,   226,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,   168,   169,   170,   266,     0,   267,   268,
       0,   269,   270,     0,   271,   272,   273,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,   278,     0,     0,
     171,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,     0,   292,     0,     0,     0,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,     0,     0,   199,     0,     0,   200,
     201,   202,   203,     0,   204,     0,     0,   205,   206,   207,
     208,   209,   210,   537,   538,   539,     0,   540,   211,   212,
     213,     0,   534,   535,   536,   982,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,     0,     0,     0,   220,
       0,   221,   222,   223,   224,   225,   226,     0,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,     0,   250,   251,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   255,     0,     0,   256,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,     0,   263,     0,
     264,   265,   168,   169,   170,   266,     0,   267,   268,     0,
     269,   270,     0,   271,   272,   273,     0,     0,     0,     0,
       0,     0,   274,   275,   276,   277,   278,     0,     0,   171,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,   298,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,     0,     0,   199,     0,     0,   200,   201,
     202,   203,     0,   204,     0,     0,   205,   206,   207,   208,
     209,   210,   537,   538,   539,     0,   540,   211,   212,   213,
       0,   534,   535,   536,   992,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,     0,     0,     0,   220,     0,
     221,   222,   223,   224,   225,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,   250,   251,     0,     0,     0,     0,     0,     0,   252,
     253,   254,   255,     0,     0,   256,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   263,     0,   264,
     265,   168,   169,   170,   266,     0,   267,   268,     0,   269,
     270,     0,   271,   272,   273,     0,     0,     0,     0,     0,
       0,   274,   275,   276,   277,   278,     0,     0,   171,     0,
       0,     0,   279,     0,     0,     0,     0,     0,     0,     0,
     300,     0,     0,     0,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,     0,     0,   199,     0,     0,   200,   201,   202,
     203,     0,   204,     0,     0,   205,   206,   207,   208,   209,
     210,   537,   538,   539,     0,   540,   211,   212,   213,     0,
     534,   535,   536,  1063,     0,     0,     0,     0,   214,   215,
     216,   217,   218,   219,     0,     0,     0,   220,     0,   221,
     222,   223,   224,   225,   226,     0,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,     0,
     250,   251,     0,     0,     0,     0,     0,     0,   252,   253,
     254,   255,     0,     0,   256,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,     0,   263,     0,   264,   265,
     168,   169,   170,   266,     0,   267,   268,     0,   269,   270,
       0,   271,   272,   273,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   277,   278,     0,     0,   171,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,   302,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
     537,   538,   539,     0,   540,   211,   212,   213,     0,   534,
     535,   536,  1068,     0,     0,     0,     0,   214,   215,   216,
     217,   218,   219,     0,     0,     0,   220,     0,   221,   222,
     223,   224,   225,   226,     0,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,     0,   250,
     251,     0,     0,     0,     0,     0,     0,   252,   253,   254,
     255,     0,     0,   256,   257,   258,   259,     0,     0,   260,
       0,     0,   261,   262,     0,   263,     0,   264,   265,   168,
     169,   170,   266,     0,   267,   268,     0,   269,   270,     0,
     271,   272,   273,     0,     0,     0,     0,     0,     0,   274,
     275,   276,   277,   278,     0,     0,   171,     0,     0,     0,
     279,     0,     0,     0,     0,     0,     0,     0,   309,     0,
       0,     0,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   199,     0,     0,   200,   201,   202,   203,     0,
     204,     0,     0,   205,   206,   207,   208,   209,   210,   537,
     538,   539,     0,   540,   211,   212,   213,     0,   534,   535,
     536,  1072,     0,     0,     0,     0,   214,   215,   216,   217,
     218,   219,     0,     0,     0,   220,     0,   221,   222,   223,
     224,   225,   226,     0,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,     0,   250,   251,
       0,     0,     0,     0,     0,     0,   252,   253,   254,   255,
       0,     0,   256,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,     0,   263,     0,   264,   265,   168,   169,
     170,   266,     0,   267,   268,     0,   269,   270,     0,   271,
     272,   273,     0,     0,     0,     0,     0,     0,   274,   275,
     276,   277,   278,     0,     0,   171,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,     0,
       0,   199,     0,     0,   200,   201,   202,   203,     0,   204,
       0,     0,   205,   206,   207,   208,   209,   210,   537,   538,
     539,     0,   540,   211,   212,   213,     0,   534,   535,   536,
    1073,     0,     0,     0,     0,   214,   215,   216,   217,   218,
     219,     0,     0,     0,   220,     0,   221,   222,   223,   224,
     225,   226,     0,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,     0,   250,   251,     0,
       0,     0,     0,     0,     0,   252,   253,   254,   255,     0,
       0,   256,   257,   258,   259,     0,     0,   260,     0,     0,
     261,   262,     0,   263,     0,   264,   265,   168,   169,   170,
     266,     0,   267,   268,     0,   269,   270,     0,   271,   272,
     273,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   278,     0,     0,   171,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,   315,     0,     0,     0,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,     0,
     199,     0,     0,   200,   201,   202,   203,     0,   204,     0,
       0,   205,   206,   207,   208,   209,   210,   537,   538,   539,
       0,   540,   211,   212,   213,     0,   534,   535,   536,  1076,
       0,     0,     0,     0,   214,   215,   216,   217,   218,   219,
       0,     0,     0,   220,     0,   221,   222,   223,   224,   225,
     226,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,     0,   250,   251,     0,     0,
       0,     0,     0,     0,   252,   253,   254,   255,     0,     0,
     256,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,   263,     0,   264,   265,   168,   169,   170,   266,
       0,   267,   268,     0,   269,   270,     0,   271,   272,   273,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   277,
     278,     0,     0,   171,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,   317,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,   537,   538,   539,     0,
     540,   211,   212,   213,     0,   534,   535,   536,  1077,     0,
       0,     0,     0,   214,   215,   216,   217,   218,   219,     0,
       0,     0,   220,     0,   221,   222,   223,   224,   225,   226,
       0,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,   250,   251,     0,     0,     0,
       0,     0,     0,   252,   253,   254,   255,     0,     0,   256,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   263,     0,   264,   265,   168,   169,   170,   266,     0,
     267,   268,     0,   269,   270,     0,   271,   272,   273,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   277,   278,
       0,     0,   171,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,   323,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,     0,     0,   199,     0,
       0,   200,   201,   202,   203,     0,   204,     0,     0,   205,
     206,   207,   208,   209,   210,   537,   538,   539,     0,   540,
     211,   212,   213,     0,   534,   535,   536,  1095,     0,     0,
       0,     0,   214,   215,   216,   217,   218,   219,     0,     0,
       0,   220,     0,   221,   222,   223,   224,   225,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,   250,   251,     0,     0,     0,     0,
       0,     0,   252,   253,   254,   255,     0,     0,   256,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,     0,
     263,     0,   264,   265,   168,   169,   170,   266,     0,   267,
     268,     0,   269,   270,     0,   271,   272,   273,     0,     0,
       0,     0,     0,     0,   274,   275,   276,   277,   278,     0,
       0,   171,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,   334,     0,     0,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,   537,   538,   539,     0,   540,   211,
     212,   213,     0,   534,   535,   536,  1098,     0,     0,     0,
       0,   214,   215,   216,   217,   218,   219,     0,     0,     0,
     220,     0,   221,   222,   223,   224,   225,   226,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,   168,   169,   170,   266,     0,   267,   268,
       0,   269,   270,     0,   271,   272,   273,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,   278,     0,     0,
     171,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,     0,   337,     0,     0,     0,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,     0,     0,   199,     0,     0,   200,
     201,   202,   203,     0,   204,     0,     0,   205,   206,   207,
     208,   209,   210,   537,   538,   539,     0,   540,   211,   212,
     213,     0,     0,     0,     0,  1124,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,     0,     0,     0,   220,
       0,   221,   222,   223,   224,   225,   226,     0,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,     0,   250,   251,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   255,     0,     0,   256,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,     0,   263,     0,
     264,   265,   168,   169,   170,   266,     0,   267,   268,     0,
     269,   270,     0,   271,   272,   273,     0,     0,     0,     0,
       0,     0,   274,   275,   276,   277,   278,     0,     0,   171,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,   339,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,     0,     0,   199,     0,     0,   200,   201,
     202,   203,     0,   204,     0,     0,   205,   206,   207,   208,
     209,   210,     0,     0,     0,     0,     0,   211,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,     0,     0,     0,   220,     0,
     221,   222,   223,   224,   225,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,   250,   251,     0,     0,     0,     0,     0,     0,   252,
     253,   254,   255,     0,     0,   256,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   263,     0,   264,
     265,   168,   169,   170,   266,     0,   267,   268,     0,   269,
     270,     0,   271,   272,   273,     0,     0,     0,     0,     0,
       0,   274,   275,   276,   277,   278,     0,     0,   171,     0,
       0,     0,   279,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,     0,     0,   199,     0,     0,   200,   201,   202,
     203,     0,   204,     0,     0,   205,   206,   207,   208,   209,
     210,     0,     0,     0,     0,     0,   211,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   214,   215,
     216,   217,   218,   219,     0,     0,     0,   220,     0,   221,
     222,   223,   224,   225,   226,     0,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,     0,
     250,   251,     0,     0,     0,     0,     0,     0,   252,   253,
     254,   255,     0,     0,   256,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,     0,   263,     0,   264,   265,
     168,   169,   170,   266,     0,   267,   268,     0,   269,   270,
       0,   271,   272,   273,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   277,   278,     0,     0,   171,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,   347,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,     0,     0,     0,   211,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   214,   215,   216,
     217,   218,   219,     0,     0,     0,   220,     0,   221,   222,
     223,   224,   225,   226,     0,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,     0,   250,
     251,     0,     0,     0,     0,     0,     0,   252,   253,   254,
     255,     0,     0,   256,   257,   258,   259,     0,     0,   260,
       0,     0,   261,   262,     0,   263,     0,   264,   265,   168,
     169,   170,   266,     0,   267,   268,     0,   269,   270,     0,
     271,   272,   273,     0,     0,     0,     0,     0,     0,   274,
     275,   276,   277,   278,     0,     0,   171,     0,     0,     0,
     279,     0,     0,     0,     0,     0,     0,     0,   350,     0,
       0,     0,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   199,     0,     0,   200,   201,   202,   203,     0,
     204,     0,     0,   205,   206,   207,   208,   209,   210,     0,
       0,     0,     0,     0,   211,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214,   215,   216,   217,
     218,   219,     0,     0,     0,   220,     0,   221,   222,   223,
     224,   225,   226,     0,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,     0,   250,   251,
       0,     0,     0,     0,     0,     0,   252,   253,   254,   255,
       0,     0,   256,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,     0,   263,     0,   264,   265,   168,   169,
     170,   266,     0,   267,   268,     0,   269,   270,     0,   271,
     272,   273,     0,     0,     0,     0,     0,     0,   274,   275,
     276,   277,   278,     0,     0,   171,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,   352,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,     0,
       0,   199,     0,     0,   200,   201,   202,   203,     0,   204,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,     0,     0,   211,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,   215,   216,   217,   218,
     219,     0,     0,     0,   220,     0,   221,   222,   223,   224,
     225,   226,     0,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,     0,   250,   251,     0,
       0,     0,     0,     0,     0,   252,   253,   254,   255,     0,
       0,   256,   257,   258,   259,     0,     0,   260,     0,     0,
     261,   262,     0,   263,     0,   264,   265,   168,   169,   170,
     266,     0,   267,   268,     0,   269,   270,     0,   271,   272,
     273,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   278,     0,     0,   171,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,   357,     0,     0,     0,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,     0,
     199,     0,     0,   200,   201,   202,   203,     0,   204,     0,
       0,   205,   206,   207,   208,   209,   210,     0,     0,     0,
       0,     0,   211,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   214,   215,   216,   217,   218,   219,
       0,     0,     0,   220,     0,   221,   222,   223,   224,   225,
     226,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,     0,   250,   251,     0,     0,
       0,     0,     0,     0,   252,   253,   254,   255,     0,     0,
     256,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,   263,     0,   264,   265,   168,   169,   170,   266,
       0,   267,   268,     0,   269,   270,     0,   271,   272,   273,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   277,
     278,     0,     0,   171,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,   362,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,     0,     0,     0,     0,
       0,   211,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,   215,   216,   217,   218,   219,     0,
       0,     0,   220,     0,   221,   222,   223,   224,   225,   226,
       0,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,   250,   251,     0,     0,     0,
       0,     0,     0,   252,   253,   254,   255,     0,     0,   256,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   263,     0,   264,   265,   168,   169,   170,   266,     0,
     267,   268,     0,   269,   270,     0,   271,   272,   273,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   277,   278,
       0,     0,   171,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,   373,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,     0,     0,   199,     0,
       0,   200,   201,   202,   203,     0,   204,     0,     0,   205,
     206,   207,   208,   209,   210,     0,     0,     0,     0,     0,
     211,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,   215,   216,   217,   218,   219,     0,     0,
       0,   220,     0,   221,   222,   223,   224,   225,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,   250,   251,     0,     0,     0,     0,
       0,     0,   252,   253,   254,   255,     0,     0,   256,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,     0,
     263,     0,   264,   265,   168,   169,   170,   266,     0,   267,
     268,     0,   269,   270,     0,   271,   272,   273,     0,     0,
       0,     0,     0,     0,   274,   275,   276,   277,   278,     0,
       0,   171,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,   375,     0,     0,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,   211,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   214,   215,   216,   217,   218,   219,     0,     0,     0,
     220,     0,   221,   222,   223,   224,   225,   226,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,   168,   169,   170,   266,     0,   267,   268,
       0,   269,   270,     0,   271,   272,   273,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   277,   278,     0,     0,
     171,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,     0,   378,     0,     0,     0,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,     0,     0,   199,     0,     0,   200,
     201,   202,   203,     0,   204,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,     0,     0,   211,   212,
     213,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,     0,     0,     0,   220,
       0,   221,   222,   223,   224,   225,   226,     0,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,     0,   250,   251,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   255,     0,     0,   256,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,     0,   263,     0,
     264,   265,   168,   169,   170,   266,     0,   267,   268,     0,
     269,   270,     0,   271,   272,   273,     0,     0,     0,     0,
       0,     0,   274,   275,   276,   277,   278,     0,     0,   171,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,   381,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,     0,     0,   199,     0,     0,   200,   201,
     202,   203,     0,   204,     0,     0,   205,   206,   207,   208,
     209,   210,     0,     0,     0,     0,     0,   211,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,     0,     0,     0,   220,     0,
     221,   222,   223,   224,   225,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,   250,   251,     0,     0,     0,     0,     0,     0,   252,
     253,   254,   255,     0,     0,   256,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   263,     0,   264,
     265,   168,   169,   170,   266,     0,   267,   268,     0,   269,
     270,     0,   271,   272,   273,     0,     0,     0,     0,     0,
       0,   274,   275,   276,   277,   278,     0,     0,   171,     0,
       0,     0,   279,     0,     0,     0,     0,     0,     0,     0,
     383,     0,     0,     0,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,     0,     0,     0,   199,     0,     0,   200,   201,   202,
     203,     0,   204,     0,     0,   205,   206,   207,   208,   209,
     210,     0,     0,     0,     0,     0,   211,   212,   213,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   214,   215,
     216,   217,   218,   219,     0,     0,     0,   220,     0,   221,
     222,   223,   224,   225,   226,     0,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,     0,
     250,   251,     0,     0,     0,     0,     0,     0,   252,   253,
     254,   255,     0,     0,   256,   257,   258,   259,     0,     0,
     260,     0,     0,   261,   262,     0,   263,     0,   264,   265,
     168,   169,   170,   266,     0,   267,   268,     0,   269,   270,
       0,   271,   272,   273,     0,     0,     0,     0,     0,     0,
     274,   275,   276,   277,   278,     0,     0,   171,     0,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,   385,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,     0,     0,     0,   211,   212,   213,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   214,   215,   216,
     217,   218,   219,     0,     0,     0,   220,     0,   221,   222,
     223,   224,   225,   226,     0,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,     0,   250,
     251,     0,     0,     0,     0,     0,     0,   252,   253,   254,
     255,     0,     0,   256,   257,   258,   259,     0,     0,   260,
       0,     0,   261,   262,     0,   263,     0,   264,   265,   168,
     169,   170,   266,     0,   267,   268,     0,   269,   270,     0,
     271,   272,   273,     0,     0,     0,     0,     0,     0,   274,
     275,   276,   277,   278,     0,     0,   171,     0,     0,     0,
     279,     0,     0,     0,     0,     0,     0,     0,   387,     0,
       0,     0,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,     0,
       0,     0,   199,     0,     0,   200,   201,   202,   203,     0,
     204,     0,     0,   205,   206,   207,   208,   209,   210,     0,
       0,     0,     0,     0,   211,   212,   213,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   214,   215,   216,   217,
     218,   219,     0,     0,     0,   220,     0,   221,   222,   223,
     224,   225,   226,     0,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,     0,   250,   251,
       0,     0,     0,     0,     0,     0,   252,   253,   254,   255,
       0,     0,   256,   257,   258,   259,     0,     0,   260,     0,
       0,   261,   262,     0,   263,     0,   264,   265,   168,   169,
     170,   266,     0,   267,   268,     0,   269,   270,     0,   271,
     272,   273,     0,     0,     0,     0,     0,     0,   274,   275,
     276,   277,   278,     0,     0,   171,     0,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,   389,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,     0,
       0,   199,     0,     0,   200,   201,   202,   203,     0,   204,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
       0,     0,     0,   211,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   214,   215,   216,   217,   218,
     219,     0,     0,     0,   220,     0,   221,   222,   223,   224,
     225,   226,     0,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,     0,   250,   251,     0,
       0,     0,     0,     0,     0,   252,   253,   254,   255,     0,
       0,   256,   257,   258,   259,     0,     0,   260,     0,     0,
     261,   262,     0,   263,     0,   264,   265,   168,   169,   170,
     266,     0,   267,   268,     0,   269,   270,     0,   271,   272,
     273,     0,     0,     0,     0,     0,     0,   274,   275,   276,
     277,   278,     0,     0,   171,     0,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,   568,     0,     0,     0,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,     0,     0,     0,
     199,     0,     0,   200,   201,   202,   203,     0,   204,     0,
       0,   205,   206,   207,   208,   209,   210,     0,     0,     0,
       0,     0,   211,   212,   213,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   214,   215,   216,   217,   218,   219,
       0,     0,     0,   220,     0,   221,   222,   223,   224,   225,
     226,     0,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,     0,   250,   251,     0,     0,
       0,     0,     0,     0,   252,   253,   254,   255,     0,     0,
     256,   257,   258,   259,     0,     0,   260,     0,     0,   261,
     262,     0,   263,     0,   264,   265,   168,   169,   170,   266,
       0,   267,   268,     0,   269,   270,     0,   271,   272,   273,
       0,     0,     0,     0,     0,     0,   274,   275,   276,   277,
     278,     0,     0,   171,     0,     0,     0,   279,     0,     0,
       0,     0,     0,     0,     0,   570,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,     0,     0,     0,     0,
       0,   211,   212,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   214,   215,   216,   217,   218,   219,     0,
       0,     0,   220,     0,   221,   222,   223,   224,   225,   226,
       0,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,     0,   250,   251,     0,     0,     0,
       0,     0,     0,   252,   253,   254,   255,     0,     0,   256,
     257,   258,   259,     0,     0,   260,     0,     0,   261,   262,
       0,   263,     0,   264,   265,   168,   169,   170,   266,     0,
     267,   268,     0,   269,   270,     0,   271,   272,   273,     0,
       0,     0,     0,     0,     0,   274,   275,   276,   277,   278,
       0,     0,   171,     0,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,   572,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,     0,     0,     0,   199,     0,
       0,   200,   201,   202,   203,     0,   204,     0,     0,   205,
     206,   207,   208,   209,   210,     0,     0,     0,     0,     0,
     211,   212,   213,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,   215,   216,   217,   218,   219,     0,     0,
       0,   220,     0,   221,   222,   223,   224,   225,   226,     0,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,     0,   250,   251,     0,     0,     0,     0,
       0,     0,   252,   253,   254,   255,     0,     0,   256,   257,
     258,   259,     0,     0,   260,     0,     0,   261,   262,     0,
     263,     0,   264,   265,   168,   169,   170,   266,     0,   267,
     268,     0,   269,   270,     0,   271,   272,   273,     0,     0,
       0,     0,     0,     0,   274,   275,   276,   277,   278,     0,
       0,   171,     0,     0,     0,   279,     0,     0,     0,     0,
       0,     0,     0,   574,     0,     0,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,     0,     0,     0,     0,     0,   211,
     212,   213,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   214,   215,   216,   217,   218,   219,     0,     0,     0,
     220,     0,   221,   222,   223,   224,   225,   226,     0,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,     0,   250,   251,     0,     0,     0,     0,     0,
       0,   252,   253,   254,   255,     0,     0,   256,   257,   258,
     259,     0,     0,   260,     0,     0,   261,   262,     0,   263,
       0,   264,   265,   168,   169,   170,   266,     0,   267,   268,
       0,   269,   270,     0,   271,   272,   273,     0,     0,     0,
       0,     0,     0,   274,   275,   276,   576,   278,     0,     0,
     171,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,     0,   280,     0,     0,     0,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,     0,     0,     0,   199,     0,     0,   200,
     201,   202,   203,     0,   204,     0,     0,   205,   206,   207,
     208,   209,   210,     0,     0,     0,     0,     0,   211,   212,
     213,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,   215,   216,   217,   218,   219,     0,     0,     0,   220,
       0,   221,   222,   223,   224,   225,   226,     0,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,     0,   250,   251,     0,     0,     0,     0,     0,     0,
     252,   253,   254,   255,     0,     0,   256,   257,   258,   259,
       0,     0,   260,     0,     0,   261,   262,     0,   263,     0,
     264,   265,   168,   169,   170,   266,     0,   267,   268,     0,
     269,   270,     0,   271,   272,   273,     0,     0,     0,     0,
       0,     0,   274,   275,   276,  1109,   278,     0,     0,   171,
       0,     0,     0,   279,     0,     0,     0,     0,     0,     0,
       0,   280,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,     0,     0,     0,   199,     0,     0,   200,   201,
     202,   203,     0,   204,     0,     0,   205,   206,   207,   208,
     209,   210,     0,     0,     0,     0,     0,   211,   212,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   214,
     215,   216,   217,   218,   219,     0,     0,     0,   220,     0,
     221,   222,   223,   224,   225,   226,     0,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
       0,   250,   251,     0,     0,     0,     0,     0,     0,   252,
     253,   254,   255,     0,     0,   256,   257,   258,   259,     0,
       0,   260,     0,     0,   261,   262,     0,   263,     0,   264,
     265,     0,     0,     0,   266,     0,   267,   268,     0,   269,
     270,     0,   271,   272,   273,     0,     0,     0,     0,     0,
       0,   274,   275,   276,  1195,   278,     0,     0,     0,     0,
       0,     0,   279,     0,     0,     0,     1,     2,     0,     0,
     280,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,     0,     0,    22,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,     0,     0,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,    35,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,     0,     0,     0,
       0,    51,    52,    53,     0,    54,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,    57,
      58,    59,    60,    61,    62,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,    65,    66,     0,    67,    68,
       0,     0,    69,     0,    70,     0,     0,    71,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,     0,     0,     0,     0,     0,     0,
      76,    77,     0,     0,    78,     0,     0,     0,     0,     0,
       1,     2,     0,     0,   -16,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,    20,    21,     0,     0,    22,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
       0,     0,    32,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    35,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,   515,   516,
     517,     0,    37,    38,    39,    40,    41,     0,     0,     0,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,     0,     0,     0,     0,    51,    52,    53,     0,    54,
       0,   518,   519,   520,     0,     0,    55,     0,     0,     0,
       0,     0,     0,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,    57,    58,    59,    60,    61,    62,     0,
       0,     0,     0,    63,    64,     0,     0,     0,     0,    65,
      66,     0,    67,    68,     0,     0,    69,     0,    70,     0,
       0,    71,     0,     0,     0,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,     0,     0,
       0,     0,     0,     0,    76,    77,     1,     2,    78,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,     0,    15,    16,    17,    18,    19,    20,
      21,   523,   524,    22,    23,     0,    24,    25,    26,    27,
      28,    29,     0,    30,    31,     0,     0,     0,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
     933,     0,     0,   934,     0,     0,     0,     0,     0,     0,
       0,     0,    34,    35,   515,   516,   517,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,   515,   516,   517,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     0,     0,   518,   519,   520,
       0,    51,    52,    53,     0,    54,     0,     0,     0,   521,
     522,     0,    55,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,    56,     0,     0,    57,
      58,    59,    60,    61,    62,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,    65,    66,     0,    67,    68,
       0,     0,    69,     0,    70,     0,     0,    71,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,     0,     0,     0,     1,     2,     0,
      76,    77,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,     0,     0,     0,     0,   523,   524,     0,
       0,    21,     0,     0,    22,    23,     0,    24,    25,    26,
      27,    28,    29,     0,   631,    31,   523,   524,     0,    32,
      33,     0,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,     0,     0,     0,   935,     0,     0,   936,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,    34,    35,   515,   516,   517,  1074,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,   515,   516,   517,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,     0,   518,   519,
     520,     0,    51,    52,    53,     0,    54,     0,     0,     0,
     521,   522,     0,    55,     0,     0,     0,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
     522,     0,     0,     0,     0,     0,     0,    56,     0,     0,
      57,    58,    59,    60,    61,    62,     0,     0,     0,     0,
      63,    64,   515,   516,   517,     0,    65,    66,     0,    67,
      68,     0,     0,    69,     0,    70,     0,     0,    71,   515,
     516,   517,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,   518,   519,   520,   515,   516,
     517,    76,    77,     0,     0,     0,     0,   521,   522,     0,
       0,     0,   518,   519,   520,     0,     0,     0,   523,   524,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,   518,   519,   520,     0,     0,     0,   523,   524,     0,
       0,     0,     0,   521,   522,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   515,   516,   517,     0,     0,     0,
    1079,     0,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,     0,   515,   516,   517,     0,     0,  1081,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
     522,     0,     0,     0,     0,   523,   524,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
     522,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,   515,   516,   517,  1082,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   515,   516,
     517,     0,     0,     0,  1102,     0,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   518,   519,   520,
       0,     0,     0,  1103,     0,     0,     0,     0,     0,   521,
     522,   518,   519,   520,     0,     0,     0,   523,   524,     0,
       0,     0,     0,   521,   522,     0,     0,     0,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   515,   516,   517,     0,     0,     0,  1104,
       0,   518,   519,   520,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   521,   522,     0,     0,     0,     0,  1105,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,   515,   516,   517,     0,     0,     0,
       0,     0,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   515,   516,   517,   789,   756,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   518,   519,   520,
     791,   756,     0,     0,     0,     0,     0,     0,     0,   521,
     522,   523,   524,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,   523,   524,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   515,   516,   517,
     796,   756,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,   798,   756,     0,     0,     0,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,   523,   524,   521,   522,
       0,   518,   519,   520,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   521,   522,     0,   800,   756,   515,   516,
     517,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   515,   516,   517,   803,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,     0,     0,     0,     0,     0,
     523,   524,     0,   521,   522,   518,   519,   520,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   805,
     756,   518,   519,   520,     0,     0,     0,     0,     0,     0,
       0,   523,   524,   521,   522,     0,     0,     0,     0,   515,
     516,   517,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,   807,   756,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
     814,   756,   518,   519,   520,     0,     0,     0,     0,     0,
       0,   523,   524,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   515,   516,   517,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
     816,   756,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,   818,   756,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,   515,   516,   517,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
     822,   756,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,   523,   524,   521,   522,   518,   519,   520,     0,
     515,   516,   517,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,   515,   516,   517,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,   824,   756,   518,   519,   520,   515,   516,   517,     0,
       0,     0,     0,     0,     0,   521,   522,   518,   519,   520,
       0,     0,     0,     0,     0,     0,   523,   524,     0,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   521,   522,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   826,   756,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,   515,   516,   517,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,   828,   756,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,   523,   524,   831,   756,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,   523,   524,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   523,
     524,     0,   833,   756,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   515,   516,   517,   835,   756,     0,     0,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   515,   516,   517,   837,   756,
       0,     0,     0,     0,     0,     0,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,   523,   524,     0,
     521,   522,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,   515,
     516,   517,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,     0,     0,     0,   841,   756,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,   523,   524,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   843,   756,     0,     0,     0,
     515,   516,   517,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,   846,   756,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   953,   756,   518,   519,   520,     0,     0,     0,     0,
       0,     0,   523,   524,     0,   521,   522,     0,     0,     0,
     515,   516,   517,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,   954,   756,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   955,   756,   518,   519,   520,   515,   516,   517,     0,
       0,     0,     0,     0,     0,   521,   522,     0,     0,     0,
     515,   516,   517,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,   956,   756,   518,
     519,   520,   515,   516,   517,     0,     0,     0,     0,     0,
       0,   521,   522,   518,   519,   520,     0,     0,     0,     0,
       0,     0,     0,   523,   524,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,     0,
       0,     0,     0,   515,   516,   517,     0,   521,   522,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,     0,
       0,     0,  1186,  1187,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,   518,   519,   520,     0,
       0,   515,   516,   517,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   523,
     524,     0,     0,   756,   518,   519,   520,     0,     0,     0,
       0,     0,     0,   523,   524,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   523,   524,     0,     0,   864,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   515,
     516,   517,     0,   865,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,     0,     0,     0,     0,   867,   523,   524,     0,     0,
       0,     0,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,   523,   524,   876,     0,   518,   519,
     520,     0,   515,   516,   517,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,   515,   516,   517,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,     0,   877,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   521,   522,   518,
     519,   520,   515,   516,   517,     0,     0,     0,     0,     0,
       0,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   521,   522,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,     0,   878,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,   523,   524,     0,   879,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,   515,   516,   517,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,     0,     0,     0,     0,   880,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   523,   524,     0,     0,   881,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,     0,   882,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,   515,   516,   517,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
       0,   883,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,     0,
     884,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,     0,   885,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,   515,   516,   517,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,     0,   886,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,     0,   887,   518,   519,   520,
     515,   516,   517,     0,     0,     0,     0,     0,     0,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,     0,   888,   518,   519,   520,   515,   516,   517,     0,
       0,     0,     0,     0,     0,   521,   522,     0,     0,     0,
     515,   516,   517,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,     0,   889,   518,
     519,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   521,   522,   518,   519,   520,   515,   516,   517,     0,
       0,     0,     0,     0,     0,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,   515,   516,   517,     0,     0,     0,     0,     0,
       0,   521,   522,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   523,   524,     0,     0,   890,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   521,   522,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   523,
     524,     0,     0,   892,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,   515,   516,   517,     0,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,     0,     0,     0,     0,   893,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   523,
     524,     0,     0,   895,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   523,   524,     0,     0,   896,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,   515,   516,   517,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,     0,     0,     0,     0,   899,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,     0,   900,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,     0,
     901,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,   515,   516,
     517,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,     0,   902,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,     0,
     903,   518,   519,   520,   515,   516,   517,     0,     0,     0,
       0,     0,     0,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,     0,   904,   518,   519,   520,
     515,   516,   517,     0,     0,     0,     0,     0,     0,   521,
     522,     0,     0,     0,   515,   516,   517,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,     0,   907,   518,   519,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   521,   522,   518,   519,   520,
     515,   516,   517,     0,     0,     0,     0,     0,     0,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,   515,   516,   517,     0,
       0,     0,     0,     0,     0,   521,   522,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   523,   524,     0,
       0,   908,     0,     0,     0,     0,     0,     0,     0,   518,
     519,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   521,   522,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   523,   524,     0,     0,   909,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,   515,   516,   517,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,     0,
       0,     0,     0,   910,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   523,   524,     0,     0,   911,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   523,
     524,     0,     0,   914,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,   515,   516,   517,     0,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,     0,     0,     0,     0,  1021,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,     0,  1032,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,     0,  1042,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,   515,   516,   517,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,     0,     0,     0,     0,
    1061,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,     0,  1062,   518,   519,   520,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,     0,
    1064,   518,   519,   520,   515,   516,   517,     0,     0,     0,
       0,     0,     0,   521,   522,     0,     0,     0,   515,   516,
     517,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,     0,  1070,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
     522,   518,   519,   520,   515,   516,   517,     0,     0,     0,
       0,     0,     0,   521,   522,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
     515,   516,   517,     0,     0,     0,     0,     0,     0,   521,
     522,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,     0,  1083,     0,     0,     0,     0,
       0,     0,     0,   518,   519,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   521,   522,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   523,   524,     0,
       0,  1086,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,   515,
     516,   517,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,     0,     0,     0,     0,  1087,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   523,   524,     0,
       0,  1093,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,   523,   524,     0,     0,  1116,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,   515,   516,   517,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,     0,
       0,     0,     0,  1117,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,     0,  1118,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,     0,  1119,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,   515,   516,   517,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,     0,  1148,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,     0,  1159,   518,
     519,   520,   515,   516,   517,     0,     0,     0,     0,     0,
       0,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,     0,  1160,   518,   519,   520,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   521,   522,     0,
       0,     0,   515,   516,   517,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,     0,     0,     0,     0,
    1162,   518,   519,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,   518,   519,   520,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   521,   522,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,   515,   516,   517,     0,     0,     0,
       0,     0,     0,   521,   522,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   523,   524,     0,     0,  1170,
       0,     0,     0,     0,     0,     0,     0,   518,   519,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   521,
     522,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,     0,  1173,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   515,   516,   517,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
       0,  1174,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,     0,  1184,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,   523,   524,     0,
       0,  1185,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,   515,
     516,   517,     0,     0,   525,   526,   527,   528,   529,   530,
     531,     0,   532,     0,     0,     0,     0,  1188,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,     0,  1192,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,     0,  1210,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
     515,   516,   517,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,     0,  1215,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,     0,  1216,   518,   519,   520,   515,   516,   517,     0,
       0,     0,     0,     0,     0,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,     0,  1222,   518,
     519,   520,   515,   516,   517,     0,     0,     0,     0,     0,
       0,   521,   522,     0,     0,     0,   515,   516,   517,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,     0,  1223,   518,   519,   520,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   521,   522,   518,
     519,   520,   515,   516,   517,     0,     0,     0,     0,     0,
       0,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   518,   519,   520,   515,   516,
     517,     0,     0,     0,     0,     0,     0,   521,   522,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,   523,
     524,     0,     0,  1224,     0,     0,     0,     0,     0,     0,
       0,   518,   519,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   521,   522,     0,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   523,   524,     0,     0,  1225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,   515,   516,   517,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,     0,     0,     0,     0,  1226,   525,   526,   527,   528,
     529,   530,   531,     0,   532,   523,   524,     0,     0,  1232,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,   525,   526,   527,   528,   529,   530,   531,     0,
     532,   523,   524,     0,   545,     0,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,   515,   516,   517,     0,     0,   525,   526,
     527,   528,   529,   530,   531,     0,   532,     0,     0,     0,
     547,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   515,   516,   517,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,   556,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   558,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   561,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,   515,
     516,   517,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,   563,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   567,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   578,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   515,
     516,   517,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,   580,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   582,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,   586,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,   515,   516,   517,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,   588,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   590,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,   592,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   515,   516,   517,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,   593,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,   595,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,   599,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,   515,   516,   517,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,     0,     0,     0,   604,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,   607,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,   610,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   515,   516,   517,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,     0,     0,     0,   616,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,   795,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   862,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,   515,   516,   517,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,   863,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,   897,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   898,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   515,   516,   517,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,   939,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   941,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   944,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,   515,   516,   517,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,   946,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,   952,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   960,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   515,   516,   517,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,   962,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   964,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   966,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,   515,
     516,   517,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,   970,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,   971,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,   973,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   515,
     516,   517,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,  1022,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,  1024,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,  1026,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,   515,   516,   517,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,  1030,   518,   519,   520,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,  1036,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,  1044,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   515,   516,   517,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,  1047,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,  1049,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,  1051,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,   515,   516,   517,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,     0,     0,     0,  1054,
     518,   519,   520,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   521,   522,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,  1056,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,  1059,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   515,   516,   517,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,     0,     0,     0,  1084,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,  1085,   518,   519,   520,   515,   516,   517,
       0,     0,     0,     0,     0,     0,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,  1092,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,   515,   516,   517,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,  1108,   518,   519,   520,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   521,   522,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
     523,   524,     0,  1111,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,  1113,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   515,   516,   517,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,     0,     0,     0,  1127,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,  1130,
     518,   519,   520,   515,   516,   517,     0,     0,     0,     0,
       0,     0,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,  1149,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,   515,   516,   517,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,  1152,   518,   519,   520,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   521,   522,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,   525,   526,   527,
     528,   529,   530,   531,     0,   532,   523,   524,     0,  1156,
       0,     0,     0,     0,     0,     0,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,  1166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   515,   516,   517,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,     0,     0,
       0,  1171,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,  1189,   518,   519,   520,   515,
     516,   517,     0,     0,     0,     0,     0,     0,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,  1197,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,   515,
     516,   517,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,     0,     0,     0,  1202,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,   518,   519,   520,   515,   516,   517,     0,     0,
       0,     0,     0,     0,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   519,   523,   524,     0,     0,
     515,   516,   517,     0,     0,     0,   521,   522,   518,   519,
     520,   515,   516,   517,     0,     0,     0,     0,     0,     0,
     521,   522,     0,   525,   526,   527,   528,   529,   530,   531,
       0,   532,   523,   524,     0,  1204,     0,     0,     0,     0,
       0,     0,     0,     0,   518,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,   522,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,  1212,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,  1217,     0,   525,
     526,   527,   528,   529,   530,   531,     0,   532,   523,   524,
       0,  1220,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532,
       0,     0,     0,   523,   524,   525,   526,   527,   528,   529,
     530,   531,     0,   532,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,   527,   528,   529,   530,   531,     0,   532,     0,
       0,   525,   526,   527,   528,   529,   530,   531,     0,   532
};

static const yytype_int16 yycheck[] =
{
       1,    35,     3,     4,     5,   207,     7,     8,   214,    10,
      87,    88,   207,    14,   214,   207,    17,   207,   208,    20,
     226,   223,    23,    24,    25,    26,   226,   229,    19,   207,
      31,   207,    23,    34,    35,    36,    37,   229,    39,   207,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       5,     6,     7,    54,   207,   208,    57,   221,   222,    42,
      43,    44,   226,   223,    65,    66,    67,    68,   224,   223,
      71,   227,    73,    74,    75,   221,   222,    32,   221,   222,
     226,   207,   208,   226,    42,    43,    44,   221,   222,   207,
     208,   223,   226,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
     221,   222,   207,    78,   223,   226,    81,    82,    83,    84,
     224,    86,   223,   227,    89,    90,    91,    92,    93,    94,
     217,   218,     0,   220,   222,   100,   101,   102,   221,    42,
      43,    44,    87,    88,   225,   226,    20,   112,   113,   114,
     115,   116,   117,   224,   225,   224,   121,   223,   123,   124,
     125,   126,   127,   128,   223,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   199,   154,
     155,   224,   225,   221,   222,   221,   222,   162,   163,   164,
     165,   221,   222,   168,   169,   170,   171,   221,   222,   174,
     225,   226,   177,   178,   223,   180,   223,   182,   183,   212,
     213,   214,   187,   216,   189,   190,   223,   192,   193,   223,
     195,   196,   197,   225,   226,   228,   223,   223,   223,   204,
     205,   206,   207,   208,   212,   213,   214,   223,   216,   223,
     215,   223,   223,   223,   223,   266,   224,   225,   223,   224,
      87,    88,   223,    42,    43,    44,   231,   223,   279,   280,
     215,   216,   217,   218,   223,   220,   287,   223,   226,   290,
     223,   292,    42,    43,    44,   296,   223,   298,   223,   300,
     223,   302,   223,   223,   223,   223,   223,   223,   309,   223,
     223,   312,   223,   223,   315,   223,   317,   223,   223,   212,
     213,   214,   323,   216,   325,   326,   327,   328,   223,   223,
     331,   224,   225,   334,   223,   223,   337,   223,   339,   223,
     223,   223,   223,   223,   345,   223,   347,   223,   223,   350,
     223,   352,   223,   223,   223,   223,   357,   223,   223,   226,
     394,   362,    42,    43,    44,   214,   223,   223,   185,   186,
     223,   223,   373,   223,   375,   223,   223,   378,   223,   223,
     381,   223,   383,   223,   385,   223,   387,   223,   389,    42,
      43,    44,   223,   394,   395,   223,   397,   223,   215,   216,
     217,   218,   223,   220,   405,   406,   407,   408,   409,   223,
     223,   223,   223,   223,   223,   223,   223,   418,   419,   223,
     421,   422,   423,   424,   425,   426,   427,   223,   429,   430,
     431,   432,   433,   434,   223,   223,   437,   438,   439,   440,
     223,   223,   223,   212,   213,   214,   223,   216,   449,   450,
     451,   452,   453,   454,   455,   456,   225,   223,   223,   223,
     223,   223,   212,   213,   214,   223,   216,   223,   223,   223,
     223,   223,   223,   207,   224,   225,   223,   223,   223,   216,
      81,   225,   225,   225,   225,   225,   225,   225,   489,   490,
     491,   220,   225,   225,   225,   496,   497,   225,   225,   225,
     501,   225,   225,   225,   225,    42,    43,    44,   225,   510,
     511,   225,   225,   225,   515,   516,   517,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   212,   213,   214,   225,   216,   225,   225,   540,
     225,   542,   543,   225,   545,   225,   547,   225,   549,   225,
     225,   225,   225,   554,   225,   556,   225,   558,   559,   212,
     213,   214,   225,   216,   225,   225,   567,   568,   225,   570,
     225,   572,   225,   574,     5,     6,     7,   578,   225,   225,
     225,   582,   225,   584,   225,   586,   225,   588,   225,   225,
     225,   592,   593,   225,   595,   225,   225,   225,   599,   225,
     225,    32,   225,   604,   223,   223,   230,   608,    42,    43,
      44,   223,   223,   226,   231,   616,   226,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,   231,   214,   230,    78,   214,   227,
      81,    82,    83,    84,   227,    86,   227,   227,    89,    90,
      91,    92,    93,    94,   214,   214,   207,   225,   795,   100,
     101,   102,    42,    43,    44,   212,   213,   214,   992,   216,
     399,   112,   113,   114,   115,   116,   117,   224,   225,    -1,
     121,   402,   123,   124,   125,   126,   127,   128,   401,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,    -1,    -1,    -1,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,   207,   208,   212,   213,
     214,    -1,   216,    -1,   215,    -1,   787,    -1,   789,    -1,
     791,   225,   223,   224,   795,   796,    -1,   798,    -1,   800,
     231,    -1,    -1,    -1,    -1,    -1,   807,    -1,    -1,    -1,
      -1,    -1,    -1,   814,    42,    43,    44,   818,    -1,   820,
      -1,   822,    -1,   824,    -1,   826,    -1,   828,    -1,    -1,
     831,    -1,   833,    -1,   835,    -1,    -1,   838,    -1,    -1,
      -1,    -1,   212,   213,   214,   846,   216,    -1,    42,    43,
      44,   852,    -1,    -1,   224,   225,    -1,   858,    -1,   860,
     861,   862,   863,    -1,    -1,    -1,    -1,    -1,   869,   870,
     871,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,   897,   898,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     944,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,   926,    -1,    -1,    -1,    -1,
     931,    -1,   933,    -1,   935,    -1,    -1,    -1,   939,    -1,
     941,     1,     2,   944,    -1,   946,    -1,   948,    42,    43,
      44,   952,   953,   954,   955,   956,    -1,    -1,    -1,   960,
      -1,   962,    -1,    23,    24,   966,    42,    43,    44,   970,
     971,    -1,   973,    42,    43,    44,    36,   978,    38,    39,
      40,    -1,    -1,    -1,   212,   213,   214,   988,   216,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    56,   225,    58,    -1,
      60,    61,    42,    43,    44,    -1,    66,    67,    -1,    69,
      70,    -1,    72,    -1,    -1,    -1,    -1,    -1,   212,   213,
     214,  1022,   216,  1024,    -1,  1026,    28,    -1,    -1,  1030,
     224,   225,    -1,    -1,    -1,  1036,    -1,    -1,    -1,    -1,
      42,    43,    44,  1044,    -1,    -1,  1047,   212,   213,   214,
    1051,   216,    -1,  1054,    -1,  1056,    -1,    -1,  1059,   224,
     225,    -1,  1063,   212,   213,   214,    -1,   216,   212,   213,
     214,    -1,   216,    75,    76,    77,   225,  1111,    -1,    -1,
     224,   225,    -1,  1084,  1085,    87,    88,  1088,    -1,  1090,
      -1,  1092,   212,   213,   214,  1096,   216,    -1,    42,    43,
      44,    -1,    -1,    -1,   224,   225,    -1,  1108,    -1,    -1,
    1111,    -1,  1113,  1114,    -1,    -1,    -1,    -1,   212,   213,
     214,    -1,   216,  1124,    -1,    -1,  1127,    -1,    -1,  1130,
     224,   225,    -1,    -1,    -1,  1136,   212,   213,   214,    -1,
     216,    -1,    -1,   212,   213,   214,    -1,   216,  1149,   225,
      -1,  1152,    -1,    -1,    -1,  1156,   225,    -1,    -1,    -1,
      -1,    -1,  1163,  1197,    -1,  1166,    -1,    42,    43,    44,
    1171,    -1,   212,   213,   214,  1176,   216,    29,    -1,  1180,
      -1,    -1,    -1,   185,   186,  1186,    -1,    -1,  1189,  1190,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,  1204,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,  1220,
     280,    -1,    -1,    75,    76,    77,    -1,   287,    -1,    -1,
     290,    -1,   292,    -1,    -1,    87,    88,    -1,   298,    -1,
     300,    -1,   302,    42,    43,    44,    -1,    -1,    -1,   309,
      -1,    -1,   312,    -1,    -1,   315,    -1,   317,    42,    43,
      44,    -1,    -1,   323,    42,    43,    44,    -1,   212,   213,
     214,   331,   216,    -1,   334,    -1,    -1,   337,    -1,   339,
     224,   225,    -1,    -1,    -1,   345,    -1,   347,    -1,    -1,
     350,    -1,   352,    -1,    -1,    -1,    -1,   357,    -1,    -1,
      -1,    -1,   362,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   373,    -1,   375,    -1,    -1,   378,    -1,
      -1,   381,    -1,   383,    -1,   385,    -1,   387,    -1,   389,
      -1,    -1,    -1,   185,   186,    -1,   396,   212,   213,   214,
      -1,   216,    -1,    -1,    -1,    42,    43,    44,   408,   224,
     410,   411,   412,   413,   414,   415,   416,   417,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   212,
     213,   214,    -1,   216,    -1,    -1,   436,    -1,   438,    -1,
      -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   457,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   487,   212,   213,
     214,    -1,   216,    -1,   212,   213,   214,    -1,   216,   499,
     500,   225,    -1,   503,   504,   505,   506,   225,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   529,
      -1,    -1,    -1,    -1,   534,   535,   536,   537,   538,   539,
     540,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,   561,    -1,   563,    -1,    -1,    -1,    -1,   568,    -1,
     570,    -1,   572,    -1,   574,   212,   213,   214,    -1,   216,
     580,    -1,    -1,    -1,     5,     6,     7,    -1,   225,    -1,
     590,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   607,    -1,    -1,
     610,    32,    -1,    -1,   614,    -1,    -1,    -1,   618,    42,
      43,    44,    -1,    -1,    -1,    -1,   626,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,   185,   186,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,
     101,   102,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   112,   113,   114,   115,   116,   117,    -1,   228,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,    42,    43,    44,   187,    -1,   189,   190,
     191,   192,   193,   194,   195,   196,   197,    -1,    -1,    42,
      43,    44,    -1,   204,   205,   206,   207,   208,    -1,   212,
     213,   214,    -1,   216,   215,    -1,    -1,    -1,    -1,    -1,
      -1,   224,   223,   803,    -1,   805,    -1,    -1,   229,    42,
      43,    44,    75,    76,    77,    -1,   816,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,   826,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   837,    -1,    -1,
      -1,   841,    -1,   843,   844,    42,    43,    44,   848,    -1,
     850,   851,    -1,    -1,   854,   855,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,   874,   875,    -1,    -1,    75,    76,
      77,    -1,    42,    43,    44,    -1,    -1,    42,    43,    44,
      87,    88,    42,    43,    44,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,   913,    -1,   915,   916,   917,   918,   919,
      -1,   921,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,    -1,   216,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,   225,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
     960,   224,   225,    -1,   964,    -1,   966,    -1,    -1,    -1,
     185,   186,    75,    76,    77,    42,    43,    44,    -1,   212,
     213,   214,   982,   216,    87,    88,    -1,    -1,   185,   186,
      -1,   224,   992,    -1,   994,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      -1,    -1,   227,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    42,    43,    44,   224,    -1,    -1,
     227,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,  1044,   224,    -1,    -1,   227,  1049,
      -1,  1051,   212,   213,   214,    -1,   216,   212,   213,   214,
      -1,   216,   212,   213,   214,   225,   216,    -1,  1068,    -1,
     225,    -1,  1072,  1073,    -1,   225,  1076,  1077,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1095,    -1,    -1,  1098,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,    -1,    -1,   227,    -1,    -1,  1127,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1139,
      -1,    -1,    -1,    -1,    -1,   212,   213,   214,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1166,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     5,     6,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,   213,   214,    -1,   216,    -1,
      -1,    -1,  1202,    32,    -1,    -1,    -1,   225,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,  1217,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    -1,    -1,
      -1,   100,   101,   102,    -1,    -1,   212,   213,   214,    -1,
     216,    -1,    -1,   112,   113,   114,   115,   116,   117,   225,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,    -1,    -1,    -1,   187,    -1,
     189,   190,   191,   192,   193,   194,   195,   196,   197,     5,
       6,     7,    -1,    -1,    -1,   204,   205,   206,   207,   208,
     212,   213,   214,    -1,   216,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,   225,   223,    -1,    32,    -1,    -1,    -1,
     229,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,    -1,
      -1,    -1,    -1,    -1,   100,   101,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,    42,    43,    44,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,    -1,    -1,
      -1,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,     5,     6,     7,    -1,    -1,    -1,   204,   205,
     206,   207,   208,   212,   213,   214,    -1,   216,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,   225,   223,    -1,    32,
      -1,    -1,    -1,   229,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,   212,   213,   214,    -1,   216,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    42,    43,    44,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,    -1,    -1,    -1,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,     5,     6,     7,    -1,    -1,
      -1,   204,   205,   206,   207,   208,   212,   213,   214,    -1,
     216,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,   225,
     223,    -1,    32,    -1,    -1,    -1,   229,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,   212,   213,   214,    -1,   216,
     100,   101,   102,    -1,    -1,    -1,    -1,    -1,   225,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    42,    43,
      44,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,    -1,    -1,    -1,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,     5,     6,
       7,    -1,    -1,    -1,   204,   205,   206,   207,   208,   212,
     213,   214,    -1,   216,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,   225,   223,    -1,    32,    -1,    -1,    -1,   229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,   212,   213,
     214,    -1,   216,   100,   101,   102,    -1,    42,    43,    44,
      -1,   225,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,
     207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,   225,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,   212,   213,   214,
      -1,   216,   100,   101,   102,    -1,    42,    43,    44,    -1,
     225,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,
     208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   223,    -1,   225,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,   212,   213,   214,    -1,
     216,   100,   101,   102,    -1,    42,    43,    44,    -1,   225,
      -1,    -1,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,
      -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   223,    -1,   225,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,   212,   213,   214,    -1,   216,
     100,   101,   102,    -1,    42,    43,    44,   224,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,
      -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,    -1,   225,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,   212,   213,   214,    -1,   216,   100,
     101,   102,    -1,    42,    43,    44,   224,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,
      32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   223,    -1,   225,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,   212,   213,   214,    -1,   216,   100,   101,
     102,    -1,    42,    43,    44,   224,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,    32,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   223,    -1,   225,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,   212,   213,   214,    -1,   216,   100,   101,   102,
      -1,    42,    43,    44,   224,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,   206,   207,   208,    -1,    -1,    32,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,    -1,   225,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,   212,   213,   214,    -1,   216,   100,   101,   102,    -1,
      42,    43,    44,   224,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,
      -1,   225,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
     212,   213,   214,    -1,   216,   100,   101,   102,    -1,    42,
      43,    44,   224,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,
     225,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,   212,
     213,   214,    -1,   216,   100,   101,   102,    -1,    42,    43,
      44,   224,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
     206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,   225,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,   212,   213,
     214,    -1,   216,   100,   101,   102,    -1,    42,    43,    44,
     224,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,
     207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,   212,   213,   214,
      -1,   216,   100,   101,   102,    -1,    42,    43,    44,   224,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,
     208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,   212,   213,   214,    -1,
     216,   100,   101,   102,    -1,    42,    43,    44,   224,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,
      -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,   212,   213,   214,    -1,   216,
     100,   101,   102,    -1,    42,    43,    44,   224,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,
      -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,   212,   213,   214,    -1,   216,   100,
     101,   102,    -1,    42,    43,    44,   224,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,
      32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,   212,   213,   214,    -1,   216,   100,   101,
     102,    -1,    42,    43,    44,   224,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,    32,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,
      83,    84,    -1,    86,    -1,    -1,    89,    90,    91,    92,
      93,    94,   212,   213,   214,    -1,   216,   100,   101,   102,
      -1,    42,    43,    44,   224,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,     5,     6,     7,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,   206,   207,   208,    -1,    -1,    32,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,
      84,    -1,    86,    -1,    -1,    89,    90,    91,    92,    93,
      94,   212,   213,   214,    -1,   216,   100,   101,   102,    -1,
      42,    43,    44,   224,    -1,    -1,    -1,    -1,   112,   113,
     114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,
     124,   125,   126,   127,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,    -1,
     154,   155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
     174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,
       5,     6,     7,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
     212,   213,   214,    -1,   216,   100,   101,   102,    -1,    42,
      43,    44,   224,    -1,    -1,    -1,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,
     125,   126,   127,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,    -1,   154,
     155,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,
      -1,    -1,   177,   178,    -1,   180,    -1,   182,   183,     5,
       6,     7,   187,    -1,   189,   190,    -1,   192,   193,    -1,
     195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    -1,
      -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,
      86,    -1,    -1,    89,    90,    91,    92,    93,    94,   212,
     213,   214,    -1,   216,   100,   101,   102,    -1,    42,    43,
      44,   224,    -1,    -1,    -1,    -1,   112,   113,   114,   115,
     116,   117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,
     126,   127,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,    -1,   154,   155,
      -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,
      -1,   177,   178,    -1,   180,    -1,   182,   183,     5,     6,
       7,   187,    -1,   189,   190,    -1,   192,   193,    -1,   195,
     196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
     206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,   212,   213,
     214,    -1,   216,   100,   101,   102,    -1,    42,    43,    44,
     224,    -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,
     127,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,    -1,   154,   155,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,
     177,   178,    -1,   180,    -1,   182,   183,     5,     6,     7,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
     197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,
     207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    -1,    -1,    -1,
      78,    -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    89,    90,    91,    92,    93,    94,   212,   213,   214,
      -1,   216,   100,   101,   102,    -1,    42,    43,    44,   224,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
      -1,    -1,    -1,   121,    -1,   123,   124,   125,   126,   127,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,   154,   155,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,   169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,
     178,    -1,   180,    -1,   182,   183,     5,     6,     7,   187,
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,   197,
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,
     208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,   212,   213,   214,    -1,
     216,   100,   101,   102,    -1,    42,    43,    44,   224,    -1,
      -1,    -1,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,    -1,   121,    -1,   123,   124,   125,   126,   127,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,
      -1,   180,    -1,   182,   183,     5,     6,     7,   187,    -1,
     189,   190,    -1,   192,   193,    -1,   195,   196,   197,    -1,
      -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,
      -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,
      -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,    89,
      90,    91,    92,    93,    94,   212,   213,   214,    -1,   216,
     100,   101,   102,    -1,    42,    43,    44,   224,    -1,    -1,
      -1,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
      -1,   121,    -1,   123,   124,   125,   126,   127,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,
     180,    -1,   182,   183,     5,     6,     7,   187,    -1,   189,
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,
      -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,   212,   213,   214,    -1,   216,   100,
     101,   102,    -1,    42,    43,    44,   224,    -1,    -1,    -1,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,    -1,
     121,    -1,   123,   124,   125,   126,   127,   128,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,   169,   170,
     171,    -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,
      -1,   182,   183,     5,     6,     7,   187,    -1,   189,   190,
      -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,
      32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,    81,
      82,    83,    84,    -1,    86,    -1,    -1,    89,    90,    91,
      92,    93,    94,   212,   213,   214,    -1,   216,   100,   101,
     102,    -1,    -1,    -1,    -1,   224,    -1,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,     5,     6,     7,   187,    -1,   189,   190,    -1,
     192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,    32,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,    52,
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
     193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,   206,   207,   208,    -1,    -1,    32,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
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
      -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,
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
     195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,
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
     196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
     206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,
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
     197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,
     207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,
     208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,
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
      -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,
      -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,
      -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,
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
      -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,
      32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,
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
     192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,    32,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,    52,
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
     193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,   206,   207,   208,    -1,    -1,    32,    -1,
      -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     223,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
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
      -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,
     204,   205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,
      -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,
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
     195,   196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,
     205,   206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,
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
     196,   197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,
     206,   207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,
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
     197,    -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,
     207,   208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,
     208,    -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,
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
      -1,    -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,
      -1,    -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,
      -1,    -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,
      -1,    32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,
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
      -1,   192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,
      -1,    -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,
      32,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,
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
     192,   193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,
      -1,    -1,   204,   205,   206,   207,   208,    -1,    -1,    32,
      -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   223,    -1,    -1,    -1,    48,    49,    50,    51,    52,
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
     183,    -1,    -1,    -1,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,   197,    -1,    -1,    -1,    -1,    -1,
      -1,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,   215,    -1,    -1,    -1,     3,     4,    -1,    -1,
     223,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    -1,    -1,    30,    31,    -1,    33,    34,    35,    36,
      37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,    -1,    -1,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,   118,   119,   120,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,   166,
     167,    -1,    -1,    -1,    -1,   172,   173,    -1,   175,   176,
      -1,    -1,   179,    -1,   181,    -1,    -1,   184,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,   199,   200,    -1,    -1,    -1,    -1,    -1,    -1,
     207,   208,    -1,    -1,   211,    -1,    -1,    -1,    -1,    -1,
       3,     4,    -1,    -1,   221,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    31,    -1,
      33,    34,    35,    36,    37,    38,    -1,    40,    41,    -1,
      -1,    -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    95,    96,    97,    98,    99,    -1,    -1,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,   118,   119,   120,    -1,   122,
      -1,    75,    76,    77,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,    -1,
      -1,    -1,    -1,   166,   167,    -1,    -1,    -1,    -1,   172,
     173,    -1,   175,   176,    -1,    -1,   179,    -1,   181,    -1,
      -1,   184,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   198,   199,   200,    -1,    -1,
      -1,    -1,    -1,    -1,   207,   208,     3,     4,   211,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,   185,   186,    30,    31,    -1,    33,    34,    35,    36,
      37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
     224,    -1,    -1,   227,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    80,    42,    43,    44,    -1,    85,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,    42,    43,    44,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,    -1,    75,    76,    77,
      -1,   118,   119,   120,    -1,   122,    -1,    -1,    -1,    87,
      88,    -1,   129,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,   156,
     157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,   166,
     167,    -1,    -1,    -1,    -1,   172,   173,    -1,   175,   176,
      -1,    -1,   179,    -1,   181,    -1,    -1,   184,    -1,    -1,
      -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   198,   199,   200,    -1,    -1,    -1,     3,     4,    -1,
     207,   208,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    27,    -1,    -1,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    -1,    40,    41,   185,   186,    -1,    45,
      46,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    -1,    -1,    -1,   224,    -1,    -1,   227,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    79,    80,    42,    43,    44,   227,    85,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,    42,    43,    44,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,    -1,    75,    76,
      77,    -1,   118,   119,   120,    -1,   122,    -1,    -1,    -1,
      87,    88,    -1,   129,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,   153,    -1,    -1,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
     166,   167,    42,    43,    44,    -1,   172,   173,    -1,   175,
     176,    -1,    -1,   179,    -1,   181,    -1,    -1,   184,    42,
      43,    44,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   198,   199,   200,    75,    76,    77,    42,    43,
      44,   207,   208,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    87,    88,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    42,    43,    44,    -1,    -1,    -1,
     227,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    -1,    42,    43,    44,    -1,    -1,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,    42,    43,    44,   227,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    42,    43,
      44,    -1,    -1,    -1,   227,    -1,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    75,    76,    77,
      -1,    -1,    -1,   227,    -1,    -1,    -1,    -1,    -1,    87,
      88,    75,    76,    77,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    42,    43,    44,    -1,    -1,    -1,   227,
      -1,    75,    76,    77,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    87,    88,    -1,    -1,    -1,    -1,   227,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    42,    43,    44,   224,   225,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    75,    76,    77,
     224,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,   185,   186,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    42,    43,    44,
     224,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,   224,   225,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,    88,
      -1,    75,    76,    77,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    87,    88,    -1,   224,   225,    42,    43,
      44,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    42,    43,    44,   224,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    87,    88,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
     225,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    87,    88,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,   224,   225,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
     224,   225,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
     224,   225,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,   224,   225,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
     224,   225,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    87,    88,    75,    76,    77,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,   225,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,   225,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,   185,   186,   224,   225,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   185,
     186,    -1,   224,   225,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    42,    43,    44,   224,   225,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,    42,    43,    44,   224,   225,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    -1,    -1,    -1,   224,   225,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,   225,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,   225,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,   225,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    87,    88,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,   225,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,   225,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,   225,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    87,    88,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,    -1,
      -1,    -1,   224,   225,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    -1,    75,    76,    77,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   185,
     186,    -1,    -1,   225,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   185,   186,    -1,    -1,   225,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,    42,
      43,    44,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,    -1,    -1,    -1,    -1,   225,   185,   186,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,   185,   186,   225,    -1,    75,    76,
      77,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,    -1,   225,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,   185,   186,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,    -1,    -1,    -1,    -1,   225,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   185,   186,    -1,    -1,   225,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,    -1,   225,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
      -1,   225,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,    -1,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,    -1,   225,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,    -1,   225,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,    -1,   225,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,    -1,   225,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,   185,   186,    -1,    -1,   225,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   185,
     186,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,    -1,    -1,    -1,    -1,   225,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   185,
     186,    -1,    -1,   225,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   185,   186,    -1,    -1,   225,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,    -1,    -1,    -1,    -1,   225,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,    -1,   225,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,    -1,
     225,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,    -1,   225,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,    -1,   225,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,   185,   186,    -1,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,   185,   186,    -1,    -1,   225,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,    -1,
      -1,    -1,    -1,   225,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,   185,   186,    -1,    -1,   225,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   185,
     186,    -1,    -1,   225,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,    -1,    -1,    -1,    -1,   225,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,    -1,
     225,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,    -1,   225,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,    -1,
     225,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,    -1,   225,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,   185,   186,    -1,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    -1,    -1,    -1,    -1,   225,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,   185,   186,    -1,
      -1,   225,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,   185,   186,    -1,    -1,   225,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,    -1,
      -1,    -1,    -1,   225,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,    -1,   225,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,    -1,   225,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,    -1,   225,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,    -1,
     225,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   185,   186,    -1,    -1,   225,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
      -1,   225,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,    -1,   225,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,   185,   186,    -1,
      -1,   225,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,   212,   213,   214,   215,   216,   217,
     218,    -1,   220,    -1,    -1,    -1,    -1,   225,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,    -1,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,    -1,   225,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,    -1,   225,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,    -1,   225,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,    -1,   225,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,   185,
     186,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   185,   186,    -1,    -1,   225,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,    -1,    -1,    -1,    -1,   225,   212,   213,   214,   215,
     216,   217,   218,    -1,   220,   185,   186,    -1,    -1,   225,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   212,   213,   214,   215,   216,   217,   218,    -1,
     220,   185,   186,    -1,   224,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,   212,   213,
     214,   215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,
     224,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,   224,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,   224,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,   224,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,   224,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,   224,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,   224,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,   224,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,   224,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,   224,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,    -1,    -1,    -1,   224,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,   224,    75,    76,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
     185,   186,    -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,    -1,    -1,    -1,   224,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,   212,   213,   214,
     215,   216,   217,   218,    -1,   220,   185,   186,    -1,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,    -1,    -1,
      -1,   224,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,    -1,    -1,    -1,   224,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,   185,   186,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    87,    88,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   212,   213,   214,   215,   216,   217,   218,
      -1,   220,   185,   186,    -1,   224,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,   224,    -1,   212,
     213,   214,   215,   216,   217,   218,    -1,   220,   185,   186,
      -1,   224,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220,
      -1,    -1,    -1,   185,   186,   212,   213,   214,   215,   216,
     217,   218,    -1,   220,   185,   186,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   213,   214,   215,   216,   217,   218,    -1,   220,    -1,
      -1,   212,   213,   214,   215,   216,   217,   218,    -1,   220
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
     181,   184,   188,   198,   199,   200,   207,   208,   211,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,     5,     6,
       7,    32,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    78,
      81,    82,    83,    84,    86,    89,    90,    91,    92,    93,
      94,   100,   101,   102,   112,   113,   114,   115,   116,   117,
     121,   123,   124,   125,   126,   127,   128,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     154,   155,   162,   163,   164,   165,   168,   169,   170,   171,
     174,   177,   178,   180,   182,   183,   187,   189,   190,   192,
     193,   195,   196,   197,   204,   205,   206,   207,   208,   215,
     223,   325,   327,   207,   208,   325,   327,   223,   325,   327,
     223,   325,   223,   325,   207,   223,   229,   323,   223,   325,
     223,   325,   223,   325,   325,   325,   325,   207,   207,   223,
     325,   327,   223,   325,   327,   223,   325,   223,   325,   207,
     207,    19,    23,   223,   325,   207,   208,   207,   208,   325,
     207,   223,   323,   325,   223,   325,   327,   223,   325,   223,
     327,   325,   327,   327,   325,   223,   325,   223,   325,   325,
     223,   325,   223,   325,   325,   325,   325,   223,   325,   327,
     223,   223,   223,   325,   223,   327,   325,   327,   223,   327,
     327,   223,   207,   223,   325,   223,   325,   327,   223,   325,
     327,   223,   325,   223,   327,   223,   327,   223,   325,   223,
     327,   325,   325,   325,   214,   226,   214,   226,     0,   221,
     236,   222,    20,   224,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   325,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   325,   223,   223,   223,   223,   223,   223,   223,
     226,   226,   325,   325,   327,    42,    43,    44,    75,    76,
      77,    87,    88,   185,   186,   212,   213,   214,   215,   216,
     217,   218,   220,   228,    42,    43,    44,   212,   213,   214,
     216,   228,   226,   226,   325,   224,   325,   224,   325,   224,
     207,   324,   325,   325,   224,   325,   224,   325,   224,   214,
     325,   224,   325,   224,   225,   225,   325,   224,   223,   325,
     223,   325,   223,   325,   223,   325,   207,   325,   224,   325,
     224,   325,   224,   327,   224,   325,   224,   325,   224,   325,
     224,   325,   224,   224,   325,   224,   225,   225,   325,   224,
     225,   225,   225,   325,   224,   325,   327,   224,   224,   325,
     224,   225,   325,   327,   224,   325,   224,   327,   224,   191,
     194,   323,   325,   325,   191,   194,   229,   322,   327,   325,
     233,    40,   248,   247,   207,   208,   225,   325,   325,   225,
     325,   325,   327,   325,   327,   327,   327,   327,   327,   327,
     327,   327,   325,   325,   225,   325,   325,   325,   325,   325,
     325,   325,   225,   325,   325,   325,   325,   325,   325,   327,
     325,   325,   327,   225,   325,   225,   325,   225,   225,   225,
     225,   225,   225,   225,   225,   325,   325,   325,   325,   325,
     325,   325,   225,   325,   327,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   327,   225,   325,   325,   325,
     225,   225,   225,   225,   225,   325,   225,   325,   225,   327,
     327,   325,   327,   327,   327,   327,   207,   208,   225,   225,
     224,   231,   325,   224,   231,   325,   225,   225,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   327,   325,   325,   325,   327,   327,   327,
     327,   327,   327,   325,   327,   325,   325,   224,   325,   224,
     325,   224,   325,   225,   230,   224,   224,   325,   224,   325,
     224,   325,   325,   224,   327,   224,   327,   224,   325,   325,
     325,   325,   325,   225,   224,   325,   224,   327,   224,   325,
     224,   325,   224,   325,   224,   325,   224,   327,   224,   325,
     325,   224,   325,   224,   325,   224,   325,   224,   224,   327,
     325,   224,   327,   224,   224,   327,   224,   325,   224,   327,
     223,   223,   224,   227,   223,   223,   326,   327,   224,   227,
     226,   226,   224,   224,   225,   225,   225,   225,   225,   224,
     224,   224,   225,   225,   224,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   224,   224,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   224,   225,   224,   224,   224,   224,   224,
     225,   224,   225,   231,   224,   227,   224,   227,   231,   224,
     227,   224,   227,   224,   227,   224,   227,   325,   325,   224,
     325,   224,   324,   325,   224,   325,   224,   325,    28,   327,
     327,   325,   224,   224,   224,   224,   224,   325,   327,   325,
     224,   325,   224,   325,   224,   325,   224,   325,   327,   325,
     224,   224,   325,   224,   325,   325,   327,   325,   224,   327,
     327,   327,   224,   325,   327,   327,   327,   325,   214,   327,
     327,   230,   224,   325,   214,   325,   325,   325,   325,   325,
     325,   325,   327,   327,   325,   325,   327,   327,   327,   327,
     327,   327,   327,   227,   227,   325,   227,   227,   325,   325,
     325,   225,   224,   325,   224,   325,   224,   207,   323,   325,
     224,   325,   225,   325,   225,   225,   224,   325,   325,   325,
     325,   325,   225,   225,   224,   325,   327,   224,   325,   224,
     327,   224,   325,   327,   224,   225,   224,   325,   325,   224,
     325,   225,   225,   224,   225,   325,   225,   225,   224,   327,
     225,   225,   224,   224,   227,   325,   224,   224,   326,   227,
     327,   227,   227,   225,   224,   224,   225,   225,   224,   225,
     224,   225,   224,   225,   225,   224,   224,   225,   224,   225,
     225,   225,   227,   227,   227,   227,   325,   325,   224,   207,
     325,   224,   325,   224,    29,   325,   225,   225,   225,   225,
     325,   327,   325,   327,   224,   325,   327,   224,   325,   325,
     224,   325,   325,   327,   327,   327,   214,   327,   327,   214,
     325,   325,   325,   325,   325,   327,   325,   327,   225,   224,
     325,   225,   224,   207,   323,   325,   224,   325,   325,   225,
     225,   225,   225,   224,   225,   325,   224,   225,   325,   327,
     225,   224,   325,   225,   225,   225,   224,   225,   225,   325,
     224,   225,   225,   327,   225,   225,   224,   225,   225,   224,
     224,   225,   225,   225,   325,   207,   325,   224,   325,   325,
     325,   327,   224,   325,   224,   325,   325,   325,   325,   325,
     225,   225,   224,   207,   323,   225,   225,   224,   225,   327,
     224,   325,   225,   225,   225,   225,   225,   207,   327,   325,
     225,   225,   225
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
#line 306 "basicParse.y"
    { labeltable[(yyvsp[(1) - (1)].number)] = byteOffset; lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 308 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 309 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 310 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 311 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 312 "basicParse.y"
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
#line 319 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 320 "basicParse.y"
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
#line 327 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 328 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 329 "basicParse.y"
    { lastLineOffset = byteOffset; addIntOp(OP_CURRLINE, linenumber); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 333 "basicParse.y"
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
#line 347 "basicParse.y"
    {
		// there is nothing to do with a multi line if (ifexp handles it)
	;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 353 "basicParse.y"
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
#line 376 "basicParse.y"
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
#line 390 "basicParse.y"
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
#line 406 "basicParse.y"
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
#line 423 "basicParse.y"
    {
		// need nothing done at top of a do
	;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 429 "basicParse.y"
    {
	// create temp
	//if If false, go to to the corresponding do.
	if (numifs>0) {
		addIntOp(OP_BRANCH, iftable[numifs-1]);
		numifs--;
	}
;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 514 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 515 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_DIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 516 "basicParse.y"
    { addIntOp(OP_DIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 517 "basicParse.y"
    { addIntOp(OP_DIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 520 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIM, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 521 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); addIntOp(OP_REDIMSTR, (yyvsp[(2) - (3)].number)); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 522 "basicParse.y"
    { addIntOp(OP_REDIM, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 523 "basicParse.y"
    { addIntOp(OP_REDIMSTR, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 526 "basicParse.y"
    { addOp(OP_PAUSE); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 529 "basicParse.y"
    { addOp(OP_CLS); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 530 "basicParse.y"
    { addOp(OP_CLG); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 533 "basicParse.y"
    { addOp(OP_FASTGRAPHICS); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 536 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 537 "basicParse.y"
    { addOp(OP_GRAPHSIZE); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 540 "basicParse.y"
    { addOp(OP_REFRESH); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 543 "basicParse.y"
    { addOp(OP_END); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 547 "basicParse.y"
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

  case 119:

/* Line 1455 of yacc.c  */
#line 560 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 561 "basicParse.y"
    { addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 562 "basicParse.y"
    { addInt2Op(OP_STRARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 563 "basicParse.y"
    { addIntOp(OP_EXPLODESTR, (yyvsp[(1) - (8)].number));;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 564 "basicParse.y"
    { addIntOp(OP_EXPLODESTR_C, (yyvsp[(1) - (10)].number)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 565 "basicParse.y"
    { addIntOp(OP_EXPLODEXSTR, (yyvsp[(1) - (8)].number));;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 568 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 569 "basicParse.y"
    { addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(1) - (8)].number)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 570 "basicParse.y"
    { addInt2Op(OP_ARRAYLISTASSIGN, (yyvsp[(1) - (3)].number), listlen); listlen = 0; ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 571 "basicParse.y"
    { addIntOp(OP_EXPLODE, (yyvsp[(1) - (8)].number));;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 572 "basicParse.y"
    { addIntOp(OP_EXPLODE_C, (yyvsp[(1) - (10)].number)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 573 "basicParse.y"
    { addIntOp(OP_EXPLODEX, (yyvsp[(1) - (8)].number));;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 577 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 580 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(1) - (3)].number)); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 584 "basicParse.y"
    {
	addIntOp(OP_PUSHINT, 1); //step
	addIntOp(OP_FOR, (yyvsp[(2) - (6)].number));
	;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 589 "basicParse.y"
    {
	addIntOp(OP_FOR, (yyvsp[(2) - (8)].number));
	;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 594 "basicParse.y"
    { addIntOp(OP_NEXT, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 597 "basicParse.y"
    { addIntOp(OP_GOTO, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 600 "basicParse.y"
    { addIntOp(OP_GOSUB, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 603 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OFFERROR); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 606 "basicParse.y"
    { addIntOp(OP_ONERROR, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 609 "basicParse.y"
    { addOp(OP_RETURN); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 612 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 613 "basicParse.y"
    { addOp(OP_SETCOLORRGB); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 614 "basicParse.y"
    { addOp(OP_SETCOLORINT); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 617 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 618 "basicParse.y"
    { addIntOp(OP_SOUND_ARRAY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 619 "basicParse.y"
    { addIntOp(OP_SOUND_LIST, listlen); listlen=0; ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 620 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 621 "basicParse.y"
    { addOp(OP_SOUND); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 624 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 625 "basicParse.y"
    { addOp(OP_PLOT); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 628 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 629 "basicParse.y"
    { addOp(OP_LINE); ;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 633 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 634 "basicParse.y"
    { addOp(OP_CIRCLE); ;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 637 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 638 "basicParse.y"
    { addOp(OP_RECT); ;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 641 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 642 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 643 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 644 "basicParse.y"
    { addOp(OP_TEXT); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 647 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 648 "basicParse.y"
    { addOp(OP_FONT); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 651 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 652 "basicParse.y"
    { addOp(OP_SAY); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 655 "basicParse.y"
    { addOp(OP_SYSTEM); ;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 658 "basicParse.y"
    { addOp(OP_VOLUME); ;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 661 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 662 "basicParse.y"
    { addIntOp(OP_POLY, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 663 "basicParse.y"
    { addIntOp(OP_POLY_LIST, listlen); listlen=0; ;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 666 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(8) - (8)].number)); ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 667 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(9) - (10)].number)); ;}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 668 "basicParse.y"
    { addIntOp(OP_STAMP_S_LIST, listlen); listlen=0; ;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 669 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(6) - (6)].number)); ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 670 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 1); addFloatOp(OP_PUSHFLOAT, 0); addIntOp(OP_STAMP, (yyvsp[(7) - (8)].number)); ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 671 "basicParse.y"
    { addIntOp(OP_STAMP_LIST, listlen); listlen=0; ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 672 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(10) - (10)].number)); ;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 673 "basicParse.y"
    { addIntOp(OP_STAMP, (yyvsp[(11) - (12)].number)); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 674 "basicParse.y"
    { addIntOp(OP_STAMP_SR_LIST, listlen); listlen=0; ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 677 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_OPEN); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 678 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 679 "basicParse.y"
    { addOp(OP_OPEN); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 682 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITE); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 683 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 684 "basicParse.y"
    { addOp(OP_WRITE); ;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 687 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addOp(OP_WRITELINE); ;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 688 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 689 "basicParse.y"
    { addOp(OP_WRITELINE); ;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 692 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 693 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_CLOSE); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 694 "basicParse.y"
    { addOp(OP_CLOSE); ;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 697 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 698 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_RESET); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 699 "basicParse.y"
    { addOp(OP_RESET); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 702 "basicParse.y"
    {addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP);addOp(OP_SEEK); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 703 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 704 "basicParse.y"
    { addOp(OP_SEEK); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 707 "basicParse.y"
    { addIntOp(OP_STRINGASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 708 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 709 "basicParse.y"
    { addIntOp(OP_NUMASSIGN, (yyvsp[(3) - (3)].number)); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 710 "basicParse.y"
    { addOp(OP_STACKSWAP); addIntOp(OP_ARRAYASSIGN, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 711 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRINGASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 712 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 713 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 714 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_NUMASSIGN, (yyvsp[(2) - (2)].number)); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 715 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN, (yyvsp[(2) - (5)].number)); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 716 "basicParse.y"
    { addOp(OP_INPUT); addIntOp(OP_ARRAYASSIGN2D, (yyvsp[(2) - (7)].number)); ;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 719 "basicParse.y"
    { addOp(OP_PRINT);  addOp(OP_INPUT); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 722 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_PRINTN); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 723 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 724 "basicParse.y"
    { addOp(OP_PRINTN); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 725 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 726 "basicParse.y"
    { addOp(OP_PRINT); ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 729 "basicParse.y"
    {addOp(OP_WAVPLAY);  ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 732 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 733 "basicParse.y"
    { addOp(OP_WAVSTOP); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 736 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 737 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_WAVWAIT); ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 740 "basicParse.y"
    {addOp(OP_PUTSLICE);  ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 741 "basicParse.y"
    { addOp(OP_PUTSLICE); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 742 "basicParse.y"
    {addOp(OP_PUTSLICEMASK);  ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 743 "basicParse.y"
    { addOp(OP_PUTSLICEMASK); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 745 "basicParse.y"
    {addOp(OP_IMGLOAD);  ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 746 "basicParse.y"
    { addOp(OP_IMGLOAD); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 747 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 748 "basicParse.y"
    { addOp(OP_IMGLOAD_S); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 749 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 750 "basicParse.y"
    { addOp(OP_IMGLOAD_SR); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 753 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEDIM); ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 756 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD);  ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 757 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITELOAD); ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 760 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE);  ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 761 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESLICE); ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 764 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE);  ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 765 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEPLACE); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 768 "basicParse.y"
    {addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE);  ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 769 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEMOVE); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 772 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEHIDE); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 775 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITESHOW); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 778 "basicParse.y"
    {addOp(OP_CLICKCLEAR);  ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 779 "basicParse.y"
    { addOp(OP_CLICKCLEAR); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 782 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CHANGEDIR); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 785 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DECIMAL); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 788 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPEN); ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 791 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 792 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSE); ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 795 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBEXECUTE); ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 798 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBOPENSET); ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 801 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 802 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBCLOSESET); ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 805 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 806 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 807 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETLISTEN); ;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 810 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 811 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKTOPTO2); addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 812 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 813 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCONNECT); ;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 816 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_STACKSWAP); addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 817 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 818 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETWRITE); ;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 821 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 822 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 823 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETCLOSE); ;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 826 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_KILL); ;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 827 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_KILL); ;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 830 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SETSETTING); ;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 831 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SETSETTING); ;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 834 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTOUT); ;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 835 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTOUT); ;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 838 "basicParse.y"
    {addStringOp(OP_PUSHSTRING, "PNG"); addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 839 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 840 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_IMGSAVE); ;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 843 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_EDITVISIBLE); ;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 846 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_GRAPHVISIBLE); ;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 849 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OUTPUTVISIBLE); ;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 858 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 859 "basicParse.y"
    { listlen++; ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 862 "basicParse.y"
    { (yyval.floatnum) = (yyvsp[(2) - (3)].floatnum); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 863 "basicParse.y"
    { addOp(OP_ADD); ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 864 "basicParse.y"
    { addOp(OP_SUB); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 865 "basicParse.y"
    { addOp(OP_MUL); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 866 "basicParse.y"
    { addOp(OP_MOD); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 867 "basicParse.y"
    { addOp(OP_INTDIV); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 868 "basicParse.y"
    { addOp(OP_DIV); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 869 "basicParse.y"
    { addOp(OP_EX); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 870 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYOR); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 871 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYAND); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 872 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYNOT); ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 873 "basicParse.y"
    { addOp(OP_NEGATE); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 874 "basicParse.y"
    {addOp(OP_AND); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 875 "basicParse.y"
    { addOp(OP_OR); ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 876 "basicParse.y"
    { addOp(OP_XOR); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 877 "basicParse.y"
    { addOp(OP_NOT); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 878 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 879 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 880 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 881 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 882 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 883 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 884 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 885 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 886 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 887 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 888 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 889 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 890 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum)); ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 891 "basicParse.y"
    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 892 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 893 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 894 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 895 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 896 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 897 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 898 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 899 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 901 "basicParse.y"
    {
		if ((yyvsp[(1) - (1)].number) < 0) {
			return -1;
		} else {
			addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 908 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 909 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 910 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 911 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 912 "basicParse.y"
    { addOp(OP_LENGTH); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 913 "basicParse.y"
    { addOp(OP_ASC); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 914 "basicParse.y"
    { addOp(OP_INSTR); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 915 "basicParse.y"
    { addOp(OP_INSTR_S); ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 916 "basicParse.y"
    { addOp(OP_INSTR_SC); ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 917 "basicParse.y"
    { addOp(OP_INSTRX); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 918 "basicParse.y"
    { addOp(OP_INSTRX_S); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 919 "basicParse.y"
    { addOp(OP_CEIL); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 920 "basicParse.y"
    { addOp(OP_FLOOR); ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 921 "basicParse.y"
    { addOp(OP_SIN); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 922 "basicParse.y"
    { addOp(OP_COS); ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 923 "basicParse.y"
    { addOp(OP_TAN); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 924 "basicParse.y"
    { addOp(OP_ASIN); ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 925 "basicParse.y"
    { addOp(OP_ACOS); ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 926 "basicParse.y"
    { addOp(OP_ATAN); ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 927 "basicParse.y"
    { addOp(OP_DEGREES); ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 928 "basicParse.y"
    { addOp(OP_RADIANS); ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 929 "basicParse.y"
    { addOp(OP_LOG); ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 930 "basicParse.y"
    { addOp(OP_LOGTEN); ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 931 "basicParse.y"
    { addOp(OP_SQR); ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 932 "basicParse.y"
    { addOp(OP_EXP); ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 933 "basicParse.y"
    { addOp(OP_ABS); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 934 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 935 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 936 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 937 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 938 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 939 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 940 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 941 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 942 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 943 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 944 "basicParse.y"
    { addOp(OP_EOF); ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 945 "basicParse.y"
    { addOp(OP_EXISTS); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 946 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 947 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 948 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 949 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 950 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 951 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 952 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 953 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 954 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 955 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 956 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 957 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 958 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 959 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 960 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 961 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 962 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 963 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 964 "basicParse.y"
    { addOp(OP_SIZE); ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 965 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 966 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 967 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 968 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 969 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 970 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 971 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 972 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 973 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 974 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 975 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 976 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 977 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 978 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 979 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 980 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 981 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 982 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 983 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 984 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 985 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 986 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 987 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 988 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 989 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 990 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 991 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 992 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 993 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 994 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 995 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 996 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 997 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 998 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 999 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1000 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1001 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1002 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1003 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1004 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1005 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1006 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1007 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1008 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1009 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1010 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1011 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1012 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1013 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1014 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1015 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1016 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1017 "basicParse.y"
    { addOp(OP_PIXEL); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1018 "basicParse.y"
    { addOp(OP_RGB); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1019 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1020 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1021 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITECOLLIDE); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1022 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEX); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1023 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEY); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1024 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEH); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1025 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEW); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1026 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEV); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1027 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBROW); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1028 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBINT); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1029 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBFLOAT); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 1030 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1031 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1032 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 1033 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1034 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp( OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1035 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1036 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1037 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTIN); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1038 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNT); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 1039 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNT_C); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1040 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNTX); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1041 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OSTYPE); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1042 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OSTYPE); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1043 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MSEC); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MSEC); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1047 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1048 "basicParse.y"
    { listlen++; ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1051 "basicParse.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1052 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1053 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1054 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1055 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1056 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1057 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1059 "basicParse.y"
    {
		if ((yyvsp[(1) - (1)].number) < 0) {
			return -1;
		} else {
			addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1066 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1067 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1068 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 1069 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 1070 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 1071 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 1072 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 1073 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 1074 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 1075 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 1076 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 1077 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 1078 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 1079 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 1080 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 1081 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 1082 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 1083 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 1084 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 1085 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 1086 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 1087 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 1088 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 1089 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 1090 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 1091 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 1092 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MD5); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 1093 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_GETSETTING); ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 1094 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 1095 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 1096 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE); ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 1097 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE_C); ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 1098 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACEX); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 1099 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 1100 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 1101 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 1102 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 9481 "basicParse.tab.c"
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
#line 1106 "basicParse.y"


int
yyerror(const char *msg) {
	errorcode = -1;
	if (yytext[0] == '\n') { linenumber--; } // error happened on previous line
	return -1;
}

