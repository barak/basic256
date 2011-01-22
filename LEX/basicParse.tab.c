
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
     B256LINENUM = 452,
     B256INTEGER = 453,
     B256FLOAT = 454,
     B256STRING = 455,
     B256VARIABLE = 456,
     B256STRINGVAR = 457,
     B256NEWVAR = 458,
     B256COLOR = 459,
     B256LABEL = 460,
     B256UMINUS = 461
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
#line 531 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 543 "basicParse.tab.c"

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
#define YYFINAL  388
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   18738

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  226
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  93
/* YYNRULES -- Number of rules.  */
#define YYNRULES  492
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1221

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   461

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     215,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     217,   219,   211,   210,   218,   209,     2,   212,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   216,   222,
     206,   208,   207,   225,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   220,     2,   221,   214,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   223,     2,   224,     2,     2,     2,     2,
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
     205,   213
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
    1849,  1854,  1859,  1866,  1875,  1882,  1886,  1888,  1892,  1896,
    1900,  1904,  1908,  1910,  1915,  1922,  1924,  1929,  1934,  1939,
    1944,  1953,  1960,  1967,  1978,  1980,  1984,  1989,  1991,  1995,
    2000,  2002,  2006,  2011,  2013,  2017,  2019,  2023,  2025,  2029,
    2034,  2036,  2040,  2045,  2052,  2057,  2061,  2070,  2081,  2090,
    2095,  2102,  2107
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     227,     0,    -1,   228,   215,    -1,   228,   215,   227,    -1,
     229,   230,    -1,   230,    -1,   205,    -1,   231,    -1,   232,
      -1,   233,    -1,   235,    -1,   236,    -1,   238,    -1,   239,
      -1,   240,    -1,   241,    -1,    -1,   251,    20,   241,    -1,
     251,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     234,    -1,    23,   316,    -1,    24,    -1,    40,    23,    -1,
     237,    -1,    25,    -1,    26,   316,    -1,   242,    -1,   241,
     216,   242,    -1,   258,    -1,   259,    -1,   260,    -1,   261,
      -1,   262,    -1,   284,    -1,   265,    -1,   267,    -1,   268,
      -1,   274,    -1,   275,    -1,   266,    -1,   254,    -1,   255,
      -1,   256,    -1,   257,    -1,   263,    -1,   282,    -1,   250,
      -1,   246,    -1,   249,    -1,   247,    -1,   248,    -1,   243,
      -1,   244,    -1,   245,    -1,   253,    -1,   252,    -1,   276,
      -1,   277,    -1,   278,    -1,   279,    -1,   280,    -1,   309,
      -1,   264,    -1,   269,    -1,   270,    -1,   271,    -1,   272,
      -1,   273,    -1,   285,    -1,   286,    -1,   287,    -1,   288,
      -1,   289,    -1,   290,    -1,   291,    -1,   292,    -1,   293,
      -1,   294,    -1,   295,    -1,   296,    -1,   281,    -1,   297,
      -1,   298,    -1,   299,    -1,   300,    -1,   301,    -1,   302,
      -1,   303,    -1,   304,    -1,   305,    -1,   306,    -1,   307,
      -1,   308,    -1,   310,    -1,   311,    -1,   312,    -1,    45,
     201,   316,    -1,    45,   202,   316,    -1,    45,   201,   217,
     316,   218,   316,   219,    -1,    45,   202,   217,   316,   218,
     316,   219,    -1,    46,   201,   316,    -1,    46,   202,   316,
      -1,    46,   201,   217,   316,   218,   316,   219,    -1,    46,
     202,   217,   316,   218,   316,   219,    -1,    79,   316,    -1,
      17,    -1,    18,    -1,    14,    -1,    15,   316,   218,   316,
      -1,    15,   217,   316,   218,   316,   219,    -1,    16,    -1,
      40,    -1,    19,   316,    -1,   202,   220,   316,   221,   208,
     318,    -1,   202,   220,   316,   218,   316,   221,   208,   318,
      -1,   202,   208,   313,    -1,   202,   208,   191,   217,   318,
     218,   318,   219,    -1,   202,   208,   191,   217,   318,   218,
     318,   218,   316,   219,    -1,   202,   208,   194,   217,   318,
     218,   318,   219,    -1,   201,   220,   316,   221,   208,   316,
      -1,   201,   220,   316,   218,   316,   221,   208,   316,    -1,
     201,   208,   314,    -1,   201,   208,   191,   217,   318,   218,
     318,   219,    -1,   201,   208,   191,   217,   318,   218,   318,
     218,   316,   219,    -1,   201,   208,   194,   217,   318,   218,
     318,   219,    -1,   201,   208,   316,    -1,   202,   208,   318,
      -1,    27,   201,   208,   316,    28,   316,    -1,    27,   201,
     208,   316,    28,   316,    29,   316,    -1,    30,   201,    -1,
      36,   201,    -1,    37,   201,    -1,   167,    -1,   166,   201,
      -1,    38,    -1,    41,   316,   218,   316,   218,   316,    -1,
      41,   217,   316,   218,   316,   218,   316,   219,    -1,    41,
     316,    -1,    80,   217,   201,   219,    -1,    80,   201,    -1,
      80,   314,    -1,    80,   217,   316,   218,   316,   219,    -1,
      80,   316,   218,   316,    -1,     8,   316,   218,   316,    -1,
       8,   217,   316,   218,   316,   219,    -1,    13,   316,   218,
     316,   218,   316,   218,   316,    -1,    13,   217,   316,   218,
     316,   218,   316,   218,   316,   219,    -1,     9,   316,   218,
     316,   218,   316,    -1,     9,   217,   316,   218,   316,   218,
     316,   219,    -1,    10,   316,   218,   316,   218,   316,   218,
     316,    -1,    10,   217,   316,   218,   316,   218,   316,   218,
     316,   219,    -1,    95,   316,   218,   316,   218,   318,    -1,
      95,   217,   316,   218,   316,   218,   318,   219,    -1,    95,
     316,   218,   316,   218,   316,    -1,    95,   217,   316,   218,
     316,   218,   316,   219,    -1,    96,   318,   218,   316,   218,
     316,    -1,    96,   217,   318,   218,   316,   218,   316,   219,
      -1,    97,   318,    -1,    97,   316,    -1,    98,   318,    -1,
      99,   316,    -1,    11,   201,    -1,    11,   217,   201,   219,
      -1,    11,   314,    -1,    12,   316,   218,   316,   218,   316,
     218,   201,    -1,    12,   217,   316,   218,   316,   218,   316,
     218,   201,   219,    -1,    12,   316,   218,   316,   218,   316,
     218,   314,    -1,    12,   316,   218,   316,   218,   201,    -1,
      12,   217,   316,   218,   316,   218,   201,   219,    -1,    12,
     316,   218,   316,   218,   314,    -1,    12,   316,   218,   316,
     218,   316,   218,   316,   218,   201,    -1,    12,   217,   316,
     218,   316,   218,   316,   218,   316,   218,   201,   219,    -1,
      12,   316,   218,   316,   218,   316,   218,   316,   218,   314,
      -1,    31,   318,    -1,    31,   217,   316,   218,   318,   219,
      -1,    31,   316,   218,   318,    -1,    33,   318,    -1,    33,
     217,   316,   218,   318,   219,    -1,    33,   316,   218,   318,
      -1,    85,   318,    -1,    85,   217,   316,   218,   318,   219,
      -1,    85,   316,   218,   318,    -1,    34,    -1,    34,   217,
     219,    -1,    34,   316,    -1,    35,    -1,    35,   217,   219,
      -1,    35,   316,    -1,   122,   316,    -1,   122,   217,   316,
     218,   316,   219,    -1,   122,   316,   218,   316,    -1,   283,
     218,   202,    -1,   283,   218,   202,   220,   316,   221,    -1,
     283,   218,   201,    -1,   283,   218,   201,   220,   316,   221,
      -1,     4,   202,    -1,     4,   202,   220,   316,   221,    -1,
       4,   202,   220,   316,   218,   316,   221,    -1,     4,   201,
      -1,     4,   201,   220,   316,   221,    -1,     4,   201,   220,
     316,   218,   316,   221,    -1,     4,   318,    -1,     3,    -1,
       3,   318,    -1,     3,   316,    -1,     3,   318,   222,    -1,
       3,   316,   222,    -1,   118,   318,    -1,   119,    -1,   119,
     217,   219,    -1,   120,    -1,   120,   217,   219,    -1,   103,
     316,   218,   316,   218,   318,    -1,   103,   217,   316,   218,
     316,   218,   318,   219,    -1,   103,   316,   218,   316,   218,
     318,   218,   316,    -1,   103,   217,   316,   218,   316,   218,
     318,   218,   316,   219,    -1,   104,   316,   218,   316,   218,
     318,    -1,   104,   217,   316,   218,   316,   218,   318,   219,
      -1,   104,   316,   218,   316,   218,   316,   218,   318,    -1,
     104,   217,   316,   218,   316,   218,   316,   218,   318,   219,
      -1,   104,   316,   218,   316,   218,   316,   218,   316,   218,
     318,    -1,   104,   217,   316,   218,   316,   218,   316,   218,
     316,   218,   318,   219,    -1,   105,   316,    -1,   106,   316,
     218,   318,    -1,   106,   217,   316,   218,   318,   219,    -1,
     107,   316,   218,   316,   218,   316,   218,   316,   218,   316,
      -1,   107,   217,   316,   218,   316,   218,   316,   218,   316,
     218,   316,   219,    -1,   111,   316,   218,   316,   218,   316,
      -1,   111,   217,   316,   218,   316,   218,   316,   219,    -1,
     108,   316,   218,   316,   218,   316,    -1,   106,   217,   316,
     218,   316,   218,   316,   219,    -1,   109,   316,    -1,   110,
     316,    -1,   129,    -1,   129,   217,   219,    -1,   153,   318,
      -1,   156,   316,    -1,   157,   318,    -1,   158,    -1,   158,
     217,   219,    -1,   159,   318,    -1,   160,   318,    -1,   161,
      -1,   161,   217,   219,    -1,   172,   316,    -1,   172,   217,
     316,   218,   316,   219,    -1,   172,   316,   218,   316,    -1,
     173,   318,   218,   316,    -1,   173,   217,   318,   218,   316,
     219,    -1,   173,   316,   218,   318,   218,   316,    -1,   173,
     217,   316,   218,   318,   218,   316,   219,    -1,   175,   318,
      -1,   175,   217,   316,   218,   318,   219,    -1,   175,   316,
     218,   318,    -1,   176,    -1,   176,   217,   219,    -1,   176,
     316,    -1,   179,   318,    -1,   179,   217,   316,   218,   318,
     219,    -1,   181,   318,   218,   318,   218,   318,    -1,   181,
     217,   318,   218,   318,   218,   318,   219,    -1,   184,   316,
     218,   316,    -1,   184,   217,   316,   218,   316,   219,    -1,
     188,   318,    -1,   188,   217,   318,   218,   318,   219,    -1,
     188,   318,   218,   318,    -1,   223,   317,   224,    -1,   223,
     315,   224,    -1,   316,    -1,   316,   218,   315,    -1,   217,
     316,   219,    -1,   316,   210,   316,    -1,   316,   209,   316,
      -1,   316,   211,   316,    -1,   316,    87,   316,    -1,   316,
      88,   316,    -1,   316,   212,   316,    -1,   316,   214,   316,
      -1,   316,   185,   316,    -1,   316,   186,   316,    -1,   187,
     316,    -1,   209,   316,    -1,   316,    75,   316,    -1,   316,
      76,   316,    -1,   316,    77,   316,    -1,    78,   316,    -1,
     318,   208,   318,    -1,   318,    44,   318,    -1,   318,   206,
     318,    -1,   318,   207,   318,    -1,   318,    42,   318,    -1,
     318,    43,   318,    -1,   316,   208,   316,    -1,   316,    44,
     316,    -1,   316,   206,   316,    -1,   316,   207,   316,    -1,
     316,    42,   316,    -1,   316,    43,   316,    -1,   199,    -1,
     198,    -1,   201,   220,   225,   221,    -1,   202,   220,   225,
     221,    -1,   201,   220,   225,   218,   221,    -1,   202,   220,
     225,   218,   221,    -1,   201,   220,   218,   225,   221,    -1,
     202,   220,   218,   225,   221,    -1,   201,   220,   316,   221,
      -1,   201,   220,   316,   218,   316,   221,    -1,   201,    -1,
      48,   217,   316,   219,    -1,    48,   217,   318,   219,    -1,
      83,   217,   316,   219,    -1,    83,   217,   318,   219,    -1,
      50,   217,   318,   219,    -1,    81,   217,   318,   219,    -1,
      56,   217,   318,   218,   318,   219,    -1,    56,   217,   318,
     218,   318,   218,   316,   219,    -1,    56,   217,   318,   218,
     318,   218,   316,   218,   316,   219,    -1,    57,   217,   318,
     218,   318,   219,    -1,    57,   217,   318,   218,   318,   218,
     316,   219,    -1,    58,   217,   316,   219,    -1,    59,   217,
     316,   219,    -1,    61,   217,   316,   219,    -1,    62,   217,
     316,   219,    -1,    63,   217,   316,   219,    -1,    64,   217,
     316,   219,    -1,    65,   217,   316,   219,    -1,    66,   217,
     316,   219,    -1,    69,   217,   316,   219,    -1,    70,   217,
     316,   219,    -1,    71,   217,   316,   219,    -1,    72,   217,
     316,   219,    -1,    73,   217,   316,   219,    -1,    74,   217,
     316,   219,    -1,    67,   217,   316,   219,    -1,    60,    -1,
      60,   217,   219,    -1,    68,    -1,    68,   217,   219,    -1,
     124,    -1,   124,   217,   219,    -1,   125,    -1,   125,   217,
     219,    -1,    86,    -1,    86,   217,   219,    -1,    86,   217,
     316,   219,    -1,   123,   217,   318,   219,    -1,    89,    -1,
      89,   217,   219,    -1,    90,    -1,    90,   217,   219,    -1,
      91,    -1,    91,   217,   219,    -1,    92,    -1,    92,   217,
     219,    -1,    93,    -1,    93,   217,   219,    -1,    94,    -1,
      94,   217,   219,    -1,   100,    -1,   100,   217,   219,    -1,
     101,    -1,   101,   217,   219,    -1,   121,    -1,   121,   217,
     219,    -1,   121,   217,   316,   219,    -1,     5,    -1,     5,
     217,   219,    -1,   126,    -1,   126,   217,   219,    -1,   127,
      -1,   127,   217,   219,    -1,   128,    -1,   128,   217,   219,
      -1,   130,    -1,   130,   217,   219,    -1,   131,    -1,   131,
     217,   219,    -1,   132,    -1,   132,   217,   219,    -1,   134,
      -1,   134,   217,   219,    -1,   135,    -1,   135,   217,   219,
      -1,   136,    -1,   136,   217,   219,    -1,   137,    -1,   137,
     217,   219,    -1,   138,    -1,   138,   217,   219,    -1,   139,
      -1,   139,   217,   219,    -1,   140,    -1,   140,   217,   219,
      -1,   141,    -1,   141,   217,   219,    -1,   142,    -1,   142,
     217,   219,    -1,   143,    -1,   143,   217,   219,    -1,   144,
      -1,   144,   217,   219,    -1,   145,    -1,   145,   217,   219,
      -1,   146,    -1,   146,   217,   219,    -1,   147,    -1,   147,
     217,   219,    -1,   148,    -1,   148,   217,   219,    -1,   149,
      -1,   149,   217,   219,    -1,   150,    -1,   150,   217,   219,
      -1,   151,    -1,   151,   217,   219,    -1,   152,    -1,   152,
     217,   219,    -1,     6,   217,   316,   218,   316,   219,    -1,
       7,   217,   316,   218,   316,   218,   316,   219,    -1,   133,
      -1,   133,   217,   219,    -1,   112,   217,   316,   218,   316,
     219,    -1,   113,   217,   316,   219,    -1,   114,   217,   316,
     219,    -1,   115,   217,   316,   219,    -1,   116,   217,   316,
     219,    -1,   117,   217,   316,   219,    -1,   162,   217,   219,
      -1,   163,   217,   316,   219,    -1,   164,   217,   316,   219,
      -1,   168,    -1,   168,   217,   219,    -1,   170,    -1,   170,
     217,   219,    -1,   177,    -1,   177,   217,   219,    -1,   177,
     217,   316,   219,    -1,   183,   217,   316,   219,    -1,   190,
     217,   318,   218,   318,   219,    -1,   190,   217,   318,   218,
     318,   218,   316,   219,    -1,   193,   217,   318,   218,   318,
     219,    -1,   196,   217,   219,    -1,   318,    -1,   318,   218,
     317,    -1,   217,   318,   219,    -1,   318,   210,   318,    -1,
     316,   210,   318,    -1,   318,   210,   316,    -1,   200,    -1,
     202,   220,   316,   221,    -1,   202,   220,   316,   218,   316,
     221,    -1,   202,    -1,    82,   217,   316,   219,    -1,    49,
     217,   316,   219,    -1,    54,   217,   318,   219,    -1,    55,
     217,   318,   219,    -1,    51,   217,   318,   218,   316,   218,
     316,   219,    -1,    52,   217,   318,   218,   316,   219,    -1,
      53,   217,   318,   218,   316,   219,    -1,   102,   217,   316,
     218,   316,   218,   316,   218,   316,   219,    -1,    32,    -1,
      32,   217,   219,    -1,    32,   217,   316,   219,    -1,    84,
      -1,    84,   217,   219,    -1,    84,   217,   316,   219,    -1,
     154,    -1,   154,   217,   219,    -1,   165,   217,   316,   219,
      -1,   169,    -1,   169,   217,   219,    -1,   171,    -1,   171,
     217,   219,    -1,   174,    -1,   174,   217,   219,    -1,   174,
     217,   316,   219,    -1,   178,    -1,   178,   217,   219,    -1,
     180,   217,   318,   219,    -1,   182,   217,   318,   218,   318,
     219,    -1,   155,   217,   318,   219,    -1,   155,   217,   219,
      -1,   189,   217,   318,   218,   318,   218,   318,   219,    -1,
     189,   217,   318,   218,   318,   218,   318,   218,   316,   219,
      -1,   192,   217,   318,   218,   318,   218,   318,   219,    -1,
     195,   217,   202,   219,    -1,   195,   217,   202,   218,   318,
     219,    -1,   195,   217,   201,   219,    -1,   195,   217,   201,
     218,   318,   219,    -1
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
    1024,  1025,  1026,  1027,  1028,  1029,  1032,  1033,  1036,  1037,
    1038,  1039,  1040,  1041,  1042,  1043,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087
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
  "B256COUNTX", "B256EXPLODEX", "B256IMPLODE", "B256OSTYPE", "B256LINENUM",
  "B256INTEGER", "B256FLOAT", "B256STRING", "B256VARIABLE",
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
     455,   456,   457,   458,   459,   460,    60,    62,    61,    45,
      43,    42,    47,   461,    94,    10,    58,    40,    44,    41,
      91,    93,    59,   123,   125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   226,   227,   227,   228,   228,   229,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   231,   232,   233,
     234,   234,   235,   236,   237,   237,   238,   239,   240,   241,
     241,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   243,
     243,   243,   243,   244,   244,   244,   244,   245,   246,   246,
     247,   248,   248,   249,   250,   251,   252,   252,   252,   252,
     252,   252,   253,   253,   253,   253,   253,   253,   254,   255,
     256,   256,   257,   258,   259,   260,   261,   262,   263,   263,
     263,   264,   264,   264,   264,   264,   265,   265,   266,   266,
     267,   267,   268,   268,   269,   269,   269,   269,   270,   270,
     271,   271,   272,   273,   274,   274,   274,   275,   275,   275,
     275,   275,   275,   275,   275,   275,   276,   276,   276,   277,
     277,   277,   278,   278,   278,   279,   279,   279,   280,   280,
     280,   281,   281,   281,   282,   282,   282,   282,   282,   282,
     282,   282,   282,   282,   283,   284,   284,   284,   284,   284,
     285,   286,   286,   287,   287,   288,   288,   288,   288,   289,
     289,   289,   289,   289,   289,   290,   291,   291,   292,   292,
     293,   293,   294,   294,   295,   296,   297,   297,   298,   299,
     300,   301,   301,   302,   303,   304,   304,   305,   305,   305,
     306,   306,   306,   306,   307,   307,   307,   308,   308,   308,
     309,   309,   310,   310,   311,   311,   312,   312,   312,   313,
     314,   315,   315,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   317,   317,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318
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
       4,     4,     6,     8,     6,     3,     1,     3,     3,     3,
       3,     3,     1,     4,     6,     1,     4,     4,     4,     4,
       8,     6,     6,    10,     1,     3,     4,     1,     3,     4,
       1,     3,     4,     1,     3,     1,     3,     1,     3,     4,
       1,     3,     4,     6,     4,     3,     8,    10,     8,     4,
       6,     4,     6
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
      97,    98,   369,     0,     0,   464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   338,     0,
       0,     0,     0,     0,     0,     0,   340,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   467,   346,   350,
     352,   354,   356,   358,   360,   362,   364,     0,     0,     0,
       0,     0,     0,     0,   366,     0,   342,   344,   371,   373,
     375,   377,   379,   381,   423,   383,   385,   387,   389,   391,
     393,   395,   397,   399,   401,   403,   405,   407,   409,   411,
     413,   415,   417,   419,   470,     0,     0,     0,     0,     0,
     434,   473,   436,   475,   477,   438,   480,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,   301,   452,
     311,   455,     0,     0,   207,   206,   311,   455,     0,   204,
       0,     0,     0,     0,     0,     0,     0,   164,     0,     0,
     166,     0,     0,     0,     0,     0,     0,   115,    23,    28,
       0,   132,     0,     0,   176,     0,     0,   179,     0,   187,
       0,   190,   133,   134,    21,    25,     0,   140,     0,     0,
       0,     0,   107,   311,     0,   143,     0,     0,     0,   182,
       0,     0,     0,     0,   161,   160,   162,   163,     0,     0,
       0,     0,   225,     0,     0,     0,     0,     0,   234,   235,
       0,     0,   210,     0,     0,     0,   191,     0,   238,   239,
     240,     0,   243,   244,     0,   136,     0,   247,     0,     0,
       0,     0,     0,   254,     0,   259,     0,   260,     0,     0,
       0,     0,     0,   266,     0,     0,     0,     0,     1,     2,
       4,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   288,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   283,     0,     0,     0,     0,     0,     0,     0,
       0,   284,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,     0,     0,     0,     0,     0,     0,     0,
     208,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,   189,     0,     0,     0,    99,     0,
     100,     0,   103,     0,   104,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,   214,     0,     0,   237,
     242,   246,     0,     0,     0,     0,     0,     0,     0,     0,
     258,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     124,   128,     0,     0,     0,     0,   118,   129,     0,     3,
     114,    30,    17,   196,   194,   370,     0,     0,   465,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,   341,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   468,     0,   347,     0,   351,   353,   355,   357,
     359,   361,   363,   365,     0,     0,     0,     0,     0,     0,
       0,   367,     0,     0,   343,   345,   372,   374,   376,   378,
     380,   382,   424,   384,   386,   388,   390,   392,   394,   396,
     398,   400,   402,   404,   406,   408,   410,   412,   414,   416,
     418,   420,   471,   485,     0,   431,     0,     0,     0,   435,
     474,   437,   476,   478,     0,   439,     0,   481,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,     0,     0,
       0,     0,     0,     0,   273,   448,   299,   300,   296,   285,
     286,   287,   277,   278,   281,   282,   297,   298,   295,   275,
     274,   450,   276,   279,   280,   293,   294,   290,   291,   292,
     289,   451,   449,     0,     0,     0,   146,     0,     0,     0,
       0,   165,   270,     0,     0,     0,     0,     0,     0,   111,
       0,     0,   178,     0,   181,     0,     0,     0,     0,     0,
       0,   141,     0,   145,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   226,     0,     0,     0,     0,
       0,     0,   193,     0,   249,     0,     0,     0,   250,     0,
     256,     0,     0,     0,     0,   264,     0,   268,     0,     0,
       0,     0,     0,     0,     0,   446,     0,     0,     0,     0,
       0,     0,   466,   312,   313,   457,   316,     0,     0,     0,
     458,   459,     0,     0,   323,   324,   325,   326,   327,   328,
     329,   330,   337,   331,   332,   333,   334,   335,   336,   317,
     456,   314,   315,   469,   348,     0,     0,   426,   427,   428,
     429,   430,   368,   349,   484,   432,   433,   472,   479,   440,
     482,     0,   441,     0,     0,     0,     0,     0,   491,     0,
     489,     0,     0,   303,     0,   309,     0,     0,   304,     0,
     453,     0,   309,     0,   453,     0,     0,     0,     0,     0,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   307,   305,     0,   308,   306,     0,     0,     0,   147,
       0,   150,     0,     0,     0,   311,   172,     0,     0,     0,
     112,   130,   177,   180,     0,   138,     0,     0,     0,     0,
     144,   183,     0,   156,   154,     0,   158,     0,   215,     0,
       0,   219,     0,   227,     0,     0,   232,     0,   230,   192,
     248,     0,   251,   252,   255,   261,     0,   262,   265,   267,
       0,     0,     0,   122,     0,     0,   447,     0,   116,   197,
     195,   421,     0,     0,   461,   462,     0,   318,     0,   321,
       0,   425,   483,     0,     0,   442,     0,   444,   492,   490,
     310,   454,   310,   454,     0,     0,     0,   311,     0,     0,
       0,     0,     0,     0,   101,   102,   105,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   151,     0,   152,   171,
       0,   311,   169,     0,     0,   148,   131,   139,   157,   155,
     159,     0,   216,   217,     0,   220,     0,   221,   233,     0,
       0,   231,   253,   263,     0,   125,   127,   123,     0,   119,
     121,   117,   422,   460,     0,   319,   322,     0,     0,   486,
     443,   488,     0,   311,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,   168,
       0,   173,   175,   149,   218,     0,   222,   223,     0,   228,
     126,   120,   320,   463,   487,     0,     0,     0,   174,   224,
     229
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
     155,   156,   157,   158,   159,   160,   161,   616,   290,   540,
     278,   844,   282
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -433
static const yytype_int16 yypact[] =
{
   11426,  4772,  4951,  5130,  5309,  5488,  -196,  5667,  5846,  -433,
    6025,  -433,  -433,  -433,  4772,  -433,  -433,  4772,  -433,  -433,
    4772,  -189,  -183,  6204,  6383,  6562,  6741,  -173,  -164,  -433,
      38,  6920,  -191,  -150,  4772,  2445,  7099,  7278,  7457,  4772,
    4772,  4772,  7636,  7815,  4772,  7994,  8173,  4772,  4772,  4772,
    8352,  4772,  -132,  -113,  8531,  -108,  4772,  4772,  4772,   -97,
    4772,  4772,   -89,  -141,  -433,  8710,  8889,  9068,  9247,  9426,
    9605,  9784,  9963,  -200,  -158,  -433,    93,   -75, 11626,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
     -98,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
     106,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,   -88,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,   -60,   -58,   -40,   -37,   -35,   -31,   -24,   -22,
     -16,   -15,   -14,   -13,   -12,   -11,   -10,    -9,    -8,    -7,
      -6,    -5,    -4,    -3,    -2,     0,     2,     3,    10,    11,
      12,    13,    16,  4772,    17,    18,    19,    22,    25,    26,
      28,    30,    31,    33,    34,    35,    37,    39,    40,    41,
      43,    44,    45,    46,    47,    48,    51,    52,    56,    57,
      58,    59,    60,    61,    64,    66,    72,    74,    76,    81,
      83,    85,    86,    88,    91,    95,    96,    97,    99,   104,
     105,   107,   108,   110,   111,   113,   115,   116,   117,   118,
     119,   121,   123,   124,   128,   129,   130,   131,   134,   135,
    4772,   136,   138,   139,   146,   149,   151,  -433,  -433,  -433,
     -53,    90,  4772,  4772,   818,    89,  -157,  -101, 18458,   800,
    4772, 15770,   800,  4772, 15810,  4772, 15846,  -433,    15,  4772,
    -433,  4772, 15882,  4772, 15922,  4772, 15958, 18458, 18458, 18458,
      98,  -433,  4772, 15994,   800,  4772, 16034,   800,  2982, 18458,
    3161, 18458,  -433,  -433,  -433,  -433,  4772, 16070, 10142, 10321,
   10500, 10679, 18458,   -99, 10858,  -433, 16106,  4772, 16146,   800,
    4772, 16182,  4772,   154, 18458,   800,   800, 18458,  4772, 16218,
    4772, 16258, 18458,  4772, 16294,  4772, 16330, 16370, 18458, 18458,
    4772, 16406,   800,    36,   150,  4772, 16442,   152,   800, 18458,
     800,   155,   800,   800,   157,  -433,  4772, 16482,  4772, 16518,
     555,  4772, 16554,   800,  3340, 18458,  4772,   800,  4772,   860,
    4772, 16594,  4772,  1732,   558,  4772,  2266,  4772,  -433,    65,
    -433, 11808, 11808,  -130,   159,  4772,  4772,  3519,  4772,  4772,
    4772,  4772,  4772,  4772,  4772,  4772,  4772,  4772,  4772,  4772,
     161,  4772,  4772,  4772,  4772,  4772,  4772,  4772,   163,  4772,
    4772,  4772,  4772,  4772,  4772,   171,  4772,  4772,  4772,  3698,
    3877,   168,   169,   170,   174,   180,   181,   182,   185,  4772,
    4772,  4772,  4772,  4772,  4772,  4772,  4056,  4772,   186,   187,
     190,   198,   205,   206,   211,   212,   213,   214,   215,   216,
     217,   218,   227,   228,   229,   233,   235,   236,   237,   238,
     239,   240,   241,   244,   245,   246,   247,  4235,   248,  4772,
    4772,  4772,   249,   253,   254,   255,  4414,  4593,   256,  4772,
    4772,  4772,   171,  4772,  4772,  4772,  4772,   -94,   257,  1497,
    2087,   171, 13157,  1136,  4772,  4772,  4772,  4772,  4772,  4772,
    4772,  4772,  4772,  4772,  4772,  4772,  4772,  4772,  4772,  4772,
    4772,  4772,  -433,  4772,  4772,  4772,  4772,  4772,  4772,  4772,
    -433,  1497,  2087, 12129,  4772, 12190,  4772, 12229,  4772,   258,
     259, 16630, 12269,  4772, 12283,  4772, 12330,  4772,  4772, 12344,
    4772, 12383,  4772,  -433,  -433, 12423,  4772,  4772, 18458,  4772,
   18458,  4772, 18458,  4772, 18458,   -85, 12484,  4772, 12523,  4772,
   12537,  4772,   243,  4772, 12573,  4772, 12612,  4772, 12629,  4772,
   12677,  4772,  4772, 12718,  4772,  -433,  -433, 12757,  4772,  -433,
    -433,  -433, 12822,  4772, 12858,   427,  4772,  4772, 12872,  4772,
    -433, 12908,   622,  4772, 12938,  4772,   672,  4772,   264,   265,
    -433, 18458,  1152,   267,   270,  4772,  -433,   800,  1307,  -433,
    -433,  -433,   -98,   268,   269,  -433, 16666, 16706,  -433, 13193,
   13234,  1221, 13270,  1269,  2264,  7096,  7275,  1276,  1366,  7454,
    7633, 13306, 13346,  -433, 13382, 13418, 13458, 13494, 13530, 13570,
   13606,  -433, 13642, 13682, 13718, 13754, 13794, 13830,  1611, 13866,
   13906,  1681,  -433, 13942,  -433, 13978,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433, 16742, 16778, 14018, 14054, 14090, 14130,
   14166,  -433, 14202,  1756,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  1766,  -433, 14242, 14278, 14314,  -433,
    -433,  -433,  -433,  -433, 14354,  -433, 14390,  -433,  1818,  7812,
   14426,  7991,  8170,  8349,  8528,   -76,   -72,  -433,   266,  -192,
    1660,   271,  -124,  1707,  -433,  -433,   -73,   -73,   -73, 11746,
   18524, 18494,   171,   171,   -56,   -56,   -73,   -73,   -73,   -33,
     -33,  -433,   171,   171,  -433,   282,   282,   282,   282,   282,
     282,   -33,  -433,  1737,  1809,  4772, 18458,  4772, 16818,  4772,
   16854,  -433,  -433,  4772,  4772, 16890,  4772, 16930,  4772, 18458,
     315,  4772,   800,  4772,   800,  4772, 16966, 12974, 13012, 13053,
   13089,  -433,  4772, 18458,  4772,   800,  4772, 17002,  4772, 17042,
    4772, 17078,  4772, 17114,  4772,   800,  4772, 17154, 17190,  4772,
   17226,  4772, 18458,  4772, 18458,  4772,  4772,  8707, 18458,  4772,
     800,  4772,  4772,  8886,  4772, 18458,  4772,   800,  4772,  4772,
    4772,   285,  4772,  4772,   273,  9065,  4772,   286,  4772,  4772,
    4772,  4772,  -433,  -433,  -433,  -433,  -433,  4772,  4772,  4772,
    -433,  -433,  4772,  4772,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  4772,  4772,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  4772,  -433,  4772,  4772,  4772,  4772,  4772,  -433,  4772,
    -433,   274,   281,  -433,  4772,  -433,   311,   313,  -433,  4772,
    -433,  4772,   -67,  4772,   -64, 14466, 17266,  4772, 17302,  4772,
    -433, 17338,  2624, 17378,  4772, 14502,  4772,  1861,  1956, 17414,
    4772,  4772,  4772,  4772,  4772, 14538,  2084, 17450,  4772, 17490,
    4772, 17526,  4772, 17562,  4772, 17602,  2442, 17638,  4772,  4772,
   17674,  4772, 14578, 14614,  9244, 14650,  4772,  2621,  2800,  9423,
    4772, 14690,  2979,  9602,  9781,  1893,  4772,  9960, 10139,  -433,
    4772, 11651,  4772, 11833, 11928, 14726, 17714, 17750, 14762, 14802,
     829,   902, 17786, 14838,  4948, 10318,   919, 10497,  5127,  5306,
    5485,  -433,  -433, 11980,  -433,  -433, 12017, 12033, 12075,  -433,
    4772, 18458,  4772, 17826, 11037,   -93,  -433, 17862,  4772, 17898,
    -433,   976,  -433,  -433,  4772, 18458, 14874, 14914, 14950, 14986,
    -433,  -433,  4772, 18458,   800,  4772, 18458,  4772, 10676,  4772,
   17938,   800,  4772,  -433,  4772, 17974, 18458,  4772, 18458,  -433,
    -433,  4772,  -433, 18458,  -433,  -433,  4772,   800,  -433,  -433,
    4772,  4772,   328, 18458,  4772,  4772,  -433,   330,   800,  -433,
    -433,  -433,  4772,  4772,  -433,  -433,  4772,  -433,  4772,  -433,
    4772,  -433,  -433,  4772,  4772,  -433,  4772,  -433,  -433,  -433,
    -433,  -433,   -50,   -27, 15026, 18010,  4772,   -29, 18050,  2803,
   18086,  4772,  4772, 15062,  -433,  -433,  -433,  -433, 15098,  5664,
   15138,   938,  4772, 18122,  5843,  4772, 15174, 18162,  4772, 15210,
   15250,  6022,  1043,  6201,  4772,  1161,  6380,  4772, 15286, 15322,
   13119, 15362, 18198,  1182, 15398,  6559,  -433,  4772, 18458,  -433,
   11216,   -91,  -433, 18234,  4772, 18458, 18458,  -433,  -433,  -433,
    -433,  4772,  -433, 18458,  4772,  -433, 18274,   800,  -433,  4772,
   18310,  -433,  -433,  -433,  4772,  -433,  -433, 18458,  4772,  -433,
    -433,   800,  -433,  -433,  4772,  -433,  -433,  4772,  4772,  -433,
    -433,  -433, 15434,   -20, 18346,  -184, 15474, 15510, 18386,  6738,
    4772, 18422,  4772, 15546, 15586, 15622, 15658, 15698,  -433,  -433,
     338,  -433,  -433,  -433,  -433,  4772,  -433,   800,  4772, 18458,
    -433,  -433,  -433,  -433,  -433,   321,  6917, 15734,  -433,  -433,
    -433
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -433,   156,  -433,  -433,   463,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,   158,   160,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,
    -433,  -433,  -433,  -433,  -433,  -433,  -433,  -433,   -34,  -240,
      -1,  -432,   928
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -204
static const yytype_int16 yytable[] =
{
     274,   325,   281,   284,   286,   287,   292,   294,   384,   296,
     318,   319,   300,   297,   510,   511,   298,  1201,   301,   299,
     385,   288,   303,   306,   309,   311,   912,   289,   312,   913,
     317,   510,   511,   322,   326,   328,   331,   313,   334,   289,
     337,   339,   341,   342,   344,   346,   347,   348,   349,   351,
     386,   320,   321,   356,   510,   511,   359,   314,  -201,  -201,
     365,   315,   387,   531,   367,   369,   372,   375,     1,     2,
     381,   623,   624,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,   353,    15,    16,    17,    18,
      19,    20,    21,   388,   917,    22,    23,   918,    24,    25,
      26,    27,    28,    29,   354,    30,    31,   735,   736,   357,
      32,    33,   512,   513,  -198,  -198,  -142,  -142,   391,   532,
     361,   499,  -170,  -170,  -167,  -167,   392,   499,   364,   499,
     393,   523,   524,   525,   801,   499,   517,   518,   519,   520,
     389,   521,   907,   908,    34,    35,   909,   910,  -202,  -202,
      36,  -199,  -199,   517,   518,   519,   520,   394,   521,   395,
      37,    38,    39,    40,    41,  -203,  -203,   499,    42,    43,
      44,    45,    46,    47,    48,    49,    50,   396,   519,   520,
     397,   521,   398,    51,    52,    53,   399,    54,  -200,  -200,
    1139,   499,   425,   400,    55,   401,   523,   524,   525,  1199,
     499,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   539,   417,    56,   418,
     419,    57,    58,    59,    60,    61,    62,   420,   421,   422,
     423,    63,    64,   424,   426,   427,   428,    65,    66,   429,
      67,    68,   430,   431,    69,   432,    70,   433,   434,    71,
     435,   436,   437,    72,   438,   585,   439,   440,   441,   492,
     442,   443,   444,   445,   446,   447,    73,    74,   448,   449,
      75,   501,   502,   450,   451,   452,   453,   454,   455,   533,
     -16,   456,   535,   457,   537,   523,   524,   525,   541,   458,
     542,   459,   544,   460,   546,   526,   527,   528,   461,   529,
     462,   549,   463,   464,   551,   465,   548,   502,   466,   502,
     500,   530,   467,   468,   469,   555,   470,   558,   560,   562,
     564,   471,   472,   566,   473,   474,   568,   475,   476,   570,
     477,   502,   478,   479,   480,   481,   482,   574,   483,   576,
     484,   485,   578,   936,   580,   486,   487,   488,   489,   583,
     610,   490,   491,   493,   587,   494,   495,   504,   505,   506,
     526,   527,   528,   496,   529,   592,   497,   594,   498,   586,
     598,   589,   573,   502,   590,   601,   591,   502,   625,   604,
     643,   502,   651,   611,   612,   521,   618,   666,   667,   668,
     507,   508,   509,   669,   626,   627,   629,   630,   632,   670,
     671,   672,   510,   511,   673,   684,   685,   641,   642,   686,
     644,   645,   646,   647,   648,   649,   650,   687,   652,   653,
     654,   655,   656,   657,   688,   689,   659,   660,   663,   665,
     690,   691,   692,   693,   694,   695,   696,   697,   674,   675,
     676,   677,   678,   679,   680,   682,   698,   699,   700,   526,
     527,   528,   701,   529,   702,   703,   704,   705,   706,   707,
     708,   808,   745,   709,   710,   711,   712,   715,   719,   523,
     524,   525,   720,   721,   722,   727,   737,   781,   716,   717,
     718,   838,   839,   782,   842,   724,   726,   843,   848,   849,
     730,   911,   529,   976,   982,  1001,   916,   979,   740,   743,
     512,   513,  1002,   746,   747,   748,   749,   750,   751,   752,
     753,   754,   755,   756,   757,   758,   759,   760,   762,   763,
     764,   514,   515,   516,   517,   518,   519,   520,   771,   521,
     773,   774,  1004,   776,  1005,   778,  1124,   780,  1127,  1215,
    1218,   390,   785,   930,   787,   619,   789,   790,  1066,     0,
     622,   621,     0,     0,     0,   796,   797,     0,   798,     0,
     799,     0,   800,   162,   163,   164,   803,     0,     0,     0,
     807,     0,   809,     0,   811,     0,   813,     0,     0,     0,
     817,   818,     0,   820,     0,     0,     0,   822,     0,     0,
     165,     0,   824,     0,     0,     0,   828,   523,   524,   525,
       0,     0,     0,     0,   835,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   526,   527,   528,   193,   529,     0,   194,
     195,   196,   197,     0,   198,   826,   745,   199,   200,   201,
     202,   203,   204,     0,     0,     0,     0,     0,   205,   206,
     207,     0,     0,     0,   523,   524,   525,     0,     0,     0,
     208,   209,   210,   211,   212,   213,     0,     0,     0,   214,
       0,   215,   216,   217,   218,   219,   220,     0,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,     0,   244,   245,   523,   524,   525,     0,     0,     0,
     246,   247,   248,   249,     0,     0,   250,   251,   252,   253,
       0,     0,   254,     0,     0,   255,   256,     0,   257,     0,
     258,   259,     0,     0,     0,   260,     0,   261,   262,   608,
     263,   264,   609,   265,   266,     0,   267,   268,   269,   270,
     271,   526,   527,   528,     0,   529,     0,   272,     0,     0,
       0,     0,     0,   597,   925,   273,   926,     0,   928,     0,
       0,   289,   541,   931,     0,   933,     0,   935,     0,     0,
       0,     0,     0,     0,   939,     0,     0,     0,     0,     0,
       0,   945,     0,     0,     0,   947,     0,   949,     0,   951,
       0,   953,     0,   955,     0,   957,     0,     0,   960,     0,
     962,     0,   963,     0,     0,   965,     0,     0,   526,   527,
     528,     0,   529,   971,     0,     0,     0,     0,     0,   975,
     832,   745,   523,   524,   525,   981,     0,   983,   984,   985,
     986,     0,     0,     0,     0,     0,   987,   988,   989,     0,
     504,   505,   506,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,   525,     0,     0,     0,     0,   526,   527,
     528,     0,   529,     0,   992,   993,     0,     0,     0,     0,
     836,   745,     0,   507,   508,   509,     0,     0,  1016,     0,
       0,     0,   523,   524,   525,   510,   511,     0,     0,     0,
       0,     0,     0,  1003,     0,     0,     0,     0,  1006,     0,
    1007,     0,  1008,     0,     0,     0,  1011,     0,  1013,   275,
     279,  1017,     0,  1019,     0,  1021,     0,     0,     0,  1025,
    1026,  1027,  1028,  1029,   523,   524,   525,  1033,     0,  1036,
       0,   304,   307,  1040,     0,     0,     0,  1045,  1046,     0,
    1048,   523,   524,   525,   329,  1053,   333,   335,   336,     0,
       0,     0,     0,     0,     0,  1063,     0,     0,     0,   352,
     523,   524,   525,     0,   358,     0,   360,     0,   362,   363,
       0,     0,     0,     0,   370,   373,     0,   377,   379,     0,
     383,     0,     0,   512,   513,  1102,   526,   527,   528,  1094,
     529,  1095,     0,  1098,     0,     0,     0,  1100,   504,   505,
     506,     0,     0,  1103,   514,   515,   516,   517,   518,   519,
     520,  1108,   521,     0,  1110,   526,   527,   528,  1113,   529,
     522,  1116,     0,  1117,     0,     0,  1119,  1076,  1077,     0,
    1120,   507,   508,   509,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   510,   511,  1142,   526,   527,   528,     0,
     529,  1128,  1129,     0,     0,  1130,     0,  1131,   603,  1132,
       0,     0,     0,  1134,     0,   523,   524,   525,     0,     0,
       0,     0,     0,     0,     0,  1138,     0,     0,  1143,     0,
    1145,  1146,     0,     0,     0,     0,     0,     0,   526,   527,
     528,  1153,   529,     0,  1156,     0,     0,  1160,     0,     0,
    1078,  1079,     0,  1167,     0,   526,   527,   528,     0,   529,
       0,     0,     0,     0,     0,     0,  1182,  1084,  1085,  1184,
       0,     0,     0,  1186,   526,   527,   528,     0,   529,     0,
    1187,  1202,     0,  1188,     0,     0,  1151,  1152,  1191,     0,
       0,   512,   513,  1193,     0,     0,     0,  1194,     0,     0,
       0,     0,     0,  1195,     0,     0,  1196,  1197,   523,   524,
     525,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,  1209,     0,     0,   504,   505,   506,     0,     0,     0,
       0,   503,     0,   523,   524,   525,     0,  1217,   503,     0,
       0,   503,     0,   503,     0,     0,     0,     0,     0,   503,
       0,   503,     0,   503,   523,   524,   525,   507,   508,   509,
     503,     0,     0,   503,     0,     0,   503,     0,   503,   510,
     511,     0,     0,     0,   503,     0,     0,     0,     0,   526,
     527,   528,   503,   529,     0,   503,     0,     0,   503,     0,
     572,  1164,  1165,   523,   524,   525,   503,     0,   503,     0,
       0,   503,     0,   503,     0,     0,     0,     0,   503,     0,
       0,     0,     0,   503,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   503,     0,   595,     0,     0,   503,
       0,     0,   503,     0,   503,     0,   602,     0,   503,     0,
     606,   523,   524,   525,   617,     0,     0,     0,   523,   524,
     525,     0,     0,     0,     0,     0,   631,     0,   633,   634,
     635,   636,   637,   638,   639,   640,     0,   512,   513,     0,
       0,     0,   526,   527,   528,     0,   529,     0,     0,   504,
     505,   506,     0,     0,   658,   745,   661,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,   526,   527,   528,
     840,   529,     0,   841,     0,   683,     0,     0,     0,  1168,
    1169,     0,   507,   508,   509,     0,     0,     0,   526,   527,
     528,     0,   529,     0,   510,   511,     0,     0,     0,     0,
    1178,  1179,     0,     0,     0,   714,     0,     0,   523,   524,
     525,     0,     0,     0,     0,     0,     0,   728,   729,     0,
       0,   731,   732,   733,   734,     0,     0,   526,   527,   528,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
     854,     0,     0,     0,     0,     0,   761,     0,     0,     0,
       0,   765,   766,   767,   768,   769,   770,   772,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   526,   527,   528,   792,   529,
     794,     0,   526,   527,   528,   503,   529,   503,   856,   503,
       0,   503,   512,   513,     0,   860,     0,   805,     0,     0,
       0,     0,   162,   163,   164,     0,     0,   815,     0,     0,
       0,     0,     0,   514,   515,   516,   517,   518,   519,   520,
       0,   521,     0,     0,   827,   846,     0,   830,   847,   165,
       0,   833,     0,     0,     0,   837,     0,     0,     0,     0,
       0,     0,     0,   845,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   526,   527,   528,   193,   529,     0,   194,   195,
     196,   197,     0,   198,     0,   861,   199,   200,   201,   202,
     203,   204,     0,     0,     0,     0,     0,   205,   206,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
     209,   210,   211,   212,   213,     0,     0,     0,   214,     0,
     215,   216,   217,   218,   219,   220,     0,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
       0,   244,   245,   523,   524,   525,     0,     0,     0,   246,
     247,   248,   249,     0,     0,   250,   251,   252,   253,     0,
       0,   254,     0,     0,   255,   256,     0,   257,     0,   258,
     259,     0,     0,     0,   260,     0,   261,   262,     0,   263,
     264,     0,   265,   266,     0,   267,   268,   269,   270,   271,
       0,     0,   504,   505,   506,     0,   272,     0,     0,     0,
       0,     0,     0,     0,   273,   738,     0,     0,     0,   937,
       0,   938,   739,   523,   524,   525,     0,     0,     0,     0,
       0,     0,   946,     0,     0,   507,   508,   509,     0,     0,
       0,     0,   956,     0,     0,     0,     0,   510,   511,   504,
     505,   506,     0,   964,     0,     0,     0,   967,     0,   968,
     969,     0,     0,     0,   972,     0,   973,   974,     0,     0,
     977,   978,     0,     0,   523,   524,   525,     0,     0,   504,
     505,   506,   507,   508,   509,     0,     0,     0,     0,     0,
     990,   991,     0,     0,   510,   511,     0,     0,   523,   524,
     525,     0,     0,     0,     0,     0,     0,     0,   523,   524,
     525,     0,   507,   508,   509,     0,     0,   526,   527,   528,
       0,   529,     0,     0,   510,   511,     0,     0,     0,   994,
     879,   995,   996,   997,   998,   999,     0,  1000,     0,     0,
       0,     0,     0,     0,     0,   512,   513,     0,     0,     0,
       0,   504,   505,   506,     0,     0,     0,     0,     0,     0,
     523,   524,   525,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,  1034,     0,   914,     0,
    1038,   915,  1041,     0,   507,   508,   509,   526,   527,   528,
       0,   529,   512,   513,     0,     0,   510,   511,  1057,     0,
     882,     0,     0,   523,   524,   525,     0,     0,   845,     0,
    1068,     0,     0,   514,   515,   516,   517,   518,   519,   520,
       0,   521,   512,   513,     0,   919,     0,     0,   920,     0,
       0,     0,     0,     0,     0,   504,   505,   506,   526,   527,
     528,     0,   529,   514,   515,   516,   517,   518,   519,   520,
     607,   521,     0,     0,     0,   921,     0,     0,   922,     0,
    1109,     0,   526,   527,   528,  1111,   529,  1114,   507,   508,
     509,     0,   526,   527,   528,   893,   529,     0,     0,     0,
     510,   511,     0,     0,  1121,   894,     0,     0,  1122,  1123,
       0,     0,  1125,  1126,   512,   513,     0,     0,   523,   524,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1133,     0,     0,  1135,   514,   515,   516,   517,   518,
     519,   520,     0,   521,   526,   527,   528,   923,   529,     0,
     924,     0,     0,     0,     0,     0,     0,   900,     0,     0,
       0,     0,     0,  1157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   526,   527,   528,
       0,   529,     0,     0,     0,     0,     0,     0,   512,   513,
    1022,     0,  1189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   163,   164,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,     0,     0,     0,  1062,     0,     0,     0,  1207,   165,
       0,     0,     0,     0,     0,     0,   523,   524,   525,     0,
       0,     0,     0,  1216,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   526,   527,   528,   193,   529,     0,   194,   195,
     196,   197,     0,   198,     0,  1023,   199,   200,   201,   202,
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
     264,     0,   265,   266,     0,   267,   268,   269,   270,   271,
     526,   527,   528,     0,   529,     0,   272,     0,   165,     0,
       0,     0,     0,  1031,   273,   741,   523,   524,   525,     0,
       0,     0,   742,     0,   166,   167,   168,   169,   170,   171,
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
     162,   163,   164,   260,     0,   261,   262,   613,   263,   264,
     614,   265,   266,     0,   267,   268,   269,   270,   271,     0,
     526,   527,   528,     0,   529,   272,     0,   165,     0,     0,
       0,     0,   857,   273,   523,   524,   525,     0,     0,   615,
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
     265,   266,     0,   267,   268,   269,   323,   271,   526,   527,
     528,     0,   529,     0,   272,     0,   165,     0,     0,     0,
       0,  1043,   324,   523,   524,   525,     0,     0,   289,     0,
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
     266,     0,   267,   268,   269,  1015,   271,   526,   527,   528,
       0,   529,     0,   272,     0,   165,     0,     0,     0,     0,
    1054,   273,   523,   524,   525,     0,     0,   289,     0,     0,
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
       0,   267,   268,   269,  1141,   271,   526,   527,   528,     0,
     529,     0,   272,     0,   165,     0,     0,     0,     0,  1055,
     273,   523,   524,   525,     0,     0,   289,     0,     0,     0,
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
       0,   261,   262,     0,   263,   264,     0,   265,   266,     0,
     267,   268,   269,   270,   271,   526,   527,   528,     0,   529,
       0,   272,     0,   165,     0,     0,     0,     0,  1059,   273,
       0,   553,     0,     0,     0,     0,     0,     0,     0,   166,
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
     261,   262,     0,   263,   264,     0,   265,   266,     0,   267,
     268,   269,   270,   271,     0,     0,     0,     0,     0,     0,
     272,     0,   165,     0,     0,     0,     0,     0,   273,     0,
     554,     0,     0,     0,     0,     0,     0,     0,   166,   167,
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
     262,     0,   263,   264,     0,   265,   266,     0,   267,   268,
     269,   270,   271,     0,     0,     0,     0,     0,     0,   272,
       0,   165,     0,     0,     0,     0,     0,   273,     0,   600,
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
       0,   263,   264,     0,   265,   266,     0,   267,   268,   269,
     270,   271,     0,     0,     0,     0,     0,     0,   272,     0,
     165,     0,     0,     0,     0,     0,   273,     0,   628,     0,
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
     263,   264,     0,   265,   266,     0,   267,   268,   269,   270,
     271,     0,     0,     0,     0,     0,     0,   272,     0,   165,
       0,     0,     0,     0,     0,   273,     0,   662,     0,     0,
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
     264,     0,   265,   266,     0,   267,   268,   269,   270,   271,
       0,     0,     0,     0,     0,     0,   272,     0,   165,     0,
       0,     0,     0,     0,   273,     0,   664,     0,     0,     0,
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
       0,   265,   266,     0,   267,   268,   269,   270,   271,     0,
       0,     0,     0,     0,     0,   272,     0,   165,     0,     0,
       0,     0,     0,   273,     0,   681,     0,     0,     0,     0,
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
     265,   266,     0,   267,   268,   269,   270,   271,     0,     0,
       0,     0,     0,     0,   272,     0,   165,     0,     0,     0,
       0,     0,   273,     0,   713,     0,     0,     0,     0,     0,
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
     266,     0,   267,   268,   269,   270,   271,     0,     0,     0,
       0,     0,     0,   272,     0,   165,     0,     0,     0,     0,
       0,   273,     0,   723,     0,     0,     0,     0,     0,     0,
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
       0,   267,   268,   269,   270,   271,     0,     0,     0,     0,
       0,     0,   272,     0,   165,     0,     0,     0,     0,     0,
     273,     0,   725,     0,     0,     0,     0,     0,     0,     0,
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
       0,   261,   262,     0,   263,   264,     0,   265,   266,     0,
     267,   268,   269,   270,   271,     0,     0,     0,     0,     0,
       0,   272,     0,   165,     0,     0,     0,     0,     0,   273,
     523,   524,   525,     0,     0,     0,     0,     0,     0,   166,
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
     261,   262,     0,   263,   264,     0,   265,   266,     0,   267,
     268,   269,   276,   277,   526,   527,   528,     0,   529,     0,
     272,     0,   165,     0,     0,     0,     0,  1082,   273,   523,
     524,   525,     0,     0,     0,     0,     0,     0,   166,   167,
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
     262,     0,   263,   264,     0,   265,   266,     0,   267,   268,
     269,   270,   271,   526,   527,   528,     0,   529,     0,   272,
       0,   165,     0,     0,     0,     0,  1087,   280,   523,   524,
     525,     0,     0,     0,     0,     0,     0,   166,   167,   168,
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
       0,   263,   264,     0,   265,   266,     0,   267,   268,   269,
     270,   271,   526,   527,   528,     0,   529,     0,   272,     0,
     165,     0,     0,     0,     0,  1088,   283,   523,   524,   525,
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
     263,   264,     0,   265,   266,     0,   267,   268,   269,   270,
     271,   526,   527,   528,     0,   529,     0,   272,     0,   165,
       0,     0,     0,     0,  1089,   285,   523,   524,   525,     0,
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
     264,     0,   265,   266,     0,   267,   268,   269,   270,   271,
     526,   527,   528,     0,   529,     0,   272,     0,   165,     0,
       0,     0,     0,  1149,   291,   523,   524,   525,     0,     0,
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
       0,   265,   266,     0,   267,   268,   269,   270,   271,   526,
     527,   528,     0,   529,     0,   272,     0,   165,     0,     0,
       0,     0,  1155,   293,   523,   524,   525,     0,     0,     0,
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
     265,   266,     0,   267,   268,   269,   270,   271,   526,   527,
     528,     0,   529,     0,   272,     0,   165,     0,     0,     0,
       0,  1163,   295,   523,   524,   525,     0,     0,     0,     0,
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
     266,     0,   267,   268,   269,   270,   271,   526,   527,   528,
       0,   529,     0,   272,     0,   165,     0,     0,     0,     0,
    1166,   302,   523,   524,   525,     0,     0,     0,     0,     0,
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
       0,   267,   268,   269,   270,   271,   526,   527,   528,     0,
     529,     0,   272,     0,   165,     0,     0,     0,     0,  1170,
     305,   523,   524,   525,     0,     0,     0,     0,     0,     0,
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
       0,   261,   262,     0,   263,   264,     0,   265,   266,     0,
     267,   268,   269,   270,   271,   526,   527,   528,     0,   529,
       0,   272,     0,   165,     0,     0,     0,     0,  1181,   308,
     523,   524,   525,     0,     0,     0,     0,     0,     0,   166,
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
     261,   262,     0,   263,   264,     0,   265,   266,     0,   267,
     268,   269,   270,   271,   526,   527,   528,     0,   529,     0,
     272,     0,   165,     0,     0,     0,     0,  1206,   310,   523,
     524,   525,     0,     0,     0,     0,     0,     0,   166,   167,
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
     262,     0,   263,   264,     0,   265,   266,     0,   267,   268,
     269,   270,   271,   526,   527,   528,     0,   529,     0,   272,
       0,   165,     0,     0,     0,     0,  1219,   316,   523,   524,
     525,     0,     0,     0,     0,     0,     0,   166,   167,   168,
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
       0,   263,   264,     0,   265,   266,     0,   267,   268,   269,
     270,   271,   526,   527,   528,     0,   529,     0,   272,     0,
     165,     0,     0,     0,   858,     0,   327,   523,   524,   525,
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
     263,   264,     0,   265,   266,     0,   267,   268,   269,   270,
     271,   526,   527,   528,     0,   529,     0,   272,     0,   165,
       0,     0,     0,   859,     0,   330,   523,   524,   525,     0,
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
     264,     0,   265,   266,     0,   267,   268,   269,   270,   271,
     526,   527,   528,     0,   529,     0,   272,     0,   165,     0,
       0,     0,   862,     0,   332,   523,   524,   525,     0,     0,
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
       0,   265,   266,     0,   267,   268,   269,   270,   271,   526,
     527,   528,     0,   529,     0,   272,     0,   165,     0,     0,
       0,   863,     0,   338,   523,   524,   525,     0,     0,     0,
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
     265,   266,     0,   267,   268,   269,   270,   271,   526,   527,
     528,     0,   529,     0,   272,     0,   165,     0,     0,     0,
     901,     0,   340,   523,   524,   525,     0,     0,     0,     0,
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
     266,     0,   267,   268,   269,   270,   271,   526,   527,   528,
       0,   529,     0,   272,     0,   165,     0,     0,     0,   903,
       0,   343,   523,   524,   525,     0,     0,     0,     0,     0,
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
       0,   267,   268,   269,   270,   271,   526,   527,   528,     0,
     529,     0,   272,     0,   165,     0,     0,     0,   904,     0,
     345,   523,   524,   525,     0,     0,     0,     0,     0,     0,
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
       0,   261,   262,     0,   263,   264,     0,   265,   266,     0,
     267,   268,   269,   270,   271,   526,   527,   528,     0,   529,
       0,   272,     0,   165,     0,     0,     0,   905,     0,   350,
     523,   524,   525,     0,     0,     0,     0,     0,     0,   166,
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
     261,   262,     0,   263,   264,     0,   265,   266,     0,   267,
     268,   269,   270,   271,   526,   527,   528,     0,   529,     0,
     272,     0,   165,     0,     0,     0,   906,     0,   355,   523,
     524,   525,     0,     0,     0,     0,     0,     0,   166,   167,
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
     262,     0,   263,   264,     0,   265,   266,     0,   267,   268,
     269,   270,   271,   526,   527,   528,     0,   529,     0,   272,
       0,   165,     0,     0,     0,   966,     0,   366,   523,   524,
     525,     0,     0,     0,     0,     0,     0,   166,   167,   168,
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
       0,   263,   264,     0,   265,   266,     0,   267,   268,   269,
     270,   271,   526,   527,   528,     0,   529,     0,   272,     0,
     165,     0,     0,     0,   970,     0,   368,   523,   524,   525,
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
     263,   264,     0,   265,   266,     0,   267,   268,   269,   270,
     271,   526,   527,   528,     0,   529,     0,   272,     0,   165,
       0,     0,     0,   980,     0,   371,   523,   524,   525,     0,
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
     264,     0,   265,   266,     0,   267,   268,   269,   270,   271,
     526,   527,   528,     0,   529,     0,   272,     0,   165,     0,
       0,     0,  1051,     0,   374,   523,   524,   525,     0,     0,
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
       0,   265,   266,     0,   267,   268,   269,   270,   271,   526,
     527,   528,     0,   529,     0,   272,     0,   165,     0,     0,
       0,  1056,     0,   376,   523,   524,   525,     0,     0,     0,
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
     265,   266,     0,   267,   268,   269,   270,   271,   526,   527,
     528,     0,   529,     0,   272,     0,   165,     0,     0,     0,
    1060,     0,   378,   523,   524,   525,     0,     0,     0,     0,
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
     266,     0,   267,   268,   269,   270,   271,   526,   527,   528,
       0,   529,     0,   272,     0,   165,     0,     0,     0,  1061,
       0,   380,   523,   524,   525,     0,     0,     0,     0,     0,
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
       0,   267,   268,   269,   270,   271,   526,   527,   528,     0,
     529,     0,   272,     0,   165,     0,     0,     0,  1064,     0,
     382,   523,   524,   525,     0,     0,     0,     0,     0,     0,
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
       0,   261,   262,     0,   263,   264,     0,   265,   266,     0,
     267,   268,   269,   270,   271,   526,   527,   528,     0,   529,
       0,   272,     0,   165,     0,     0,     0,  1065,     0,   557,
     523,   524,   525,     0,     0,     0,     0,     0,     0,   166,
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
     261,   262,     0,   263,   264,     0,   265,   266,     0,   267,
     268,   269,   270,   271,   526,   527,   528,     0,   529,     0,
     272,     0,   165,     0,     0,     0,  1083,     0,   559,   523,
     524,   525,     0,     0,     0,     0,     0,     0,   166,   167,
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
     262,     0,   263,   264,     0,   265,   266,     0,   267,   268,
     269,   270,   271,   526,   527,   528,     0,   529,     0,   272,
       0,   165,     0,     0,     0,  1086,     0,   561,   523,   524,
     525,     0,     0,     0,     0,     0,     0,   166,   167,   168,
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
       0,   263,   264,     0,   265,   266,     0,   267,   268,   269,
     270,   271,   526,   527,   528,     0,   529,     0,   272,     0,
     165,     0,     0,     0,  1112,     0,   563,     0,     0,     0,
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
     263,   264,     0,   265,   266,     0,   267,   268,   269,   565,
     271,     0,     0,     0,     0,     0,     0,   272,     0,   165,
       0,     0,     0,     0,     0,   273,     0,     0,     0,     0,
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
     264,     0,   265,   266,     0,   267,   268,   269,  1097,   271,
       0,     0,     0,     0,     0,     0,   272,     0,   165,     0,
       0,     0,     0,     0,   273,     0,     0,     0,     0,     0,
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
       0,     0,     0,   260,     0,   261,   262,     0,   263,   264,
       0,   265,   266,     0,   267,   268,   269,  1183,   271,     0,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     1,
       2,     0,     0,   273,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    22,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,     0,
       0,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    35,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,     0,     0,     0,    42,
      43,    44,    45,    46,    47,    48,    49,    50,     0,     0,
       0,     0,     0,     0,    51,    52,    53,     0,    54,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,    57,    58,    59,    60,    61,    62,     0,     0,
       0,     0,    63,    64,     0,     0,     0,     0,    65,    66,
       0,    67,    68,     0,     0,    69,     0,    70,     0,     0,
      71,     0,     0,     0,    72,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    73,    74,     1,
       2,    75,     0,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,     0,    15,    16,    17,
      18,    19,    20,    21,     0,     0,    22,    23,     0,    24,
      25,    26,    27,    28,    29,     0,    30,    31,     0,     0,
       0,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   505,   506,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,    35,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,   507,   508,   509,    42,
      43,    44,    45,    46,    47,    48,    49,    50,   510,   511,
       0,     0,     0,     0,    51,    52,    53,     0,    54,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,    57,    58,    59,    60,    61,    62,   504,   505,
     506,     0,    63,    64,     0,     0,     0,     0,    65,    66,
       0,    67,    68,     0,     0,    69,     0,    70,     0,     0,
      71,     1,     2,     0,    72,     0,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    73,    74,     0,
       0,     0,     0,   510,   511,    21,   512,   513,    22,    23,
       0,    24,    25,    26,    27,    28,    29,     0,   620,    31,
       0,     0,     0,    32,    33,     0,     0,   514,   515,   516,
     517,   518,   519,   520,     0,   521,     0,     0,     0,     0,
       0,     0,  1067,     0,     0,   504,   505,   506,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,    35,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,   507,   508,
     509,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     510,   511,     0,     0,     0,     0,    51,    52,    53,     0,
      54,   512,   513,     0,     0,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,    56,     0,     0,    57,    58,    59,    60,    61,    62,
     504,   505,   506,     0,    63,    64,     0,     0,     0,     0,
      65,    66,     0,    67,    68,     0,     0,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,   507,   508,   509,     0,     0,     0,    73,
      74,     0,     0,     0,     0,   510,   511,     0,   512,   513,
       0,     0,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,     0,     0,     0,  1069,   507,   508,   509,     0,   504,
     505,   506,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,     0,   504,   505,   506,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   508,   509,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   510,   511,     0,     0,   507,   508,
     509,     0,     0,   512,   513,     0,     0,   504,   505,   506,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,     0,     0,  1070,
     507,   508,   509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   510,   511,     0,   512,   513,     0,     0,     0,
       0,   504,   505,   506,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,     0,
       0,  1090,   512,   513,   507,   508,   509,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   512,   513,
       0,     0,     0,   514,   515,   516,   517,   518,   519,   520,
       0,   521,   504,   505,   506,     0,     0,     0,  1091,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,     0,     0,     0,  1092,     0,     0,     0,     0,     0,
     512,   513,     0,     0,     0,   507,   508,   509,     0,     0,
       0,   504,   505,   506,     0,     0,     0,   510,   511,     0,
       0,   514,   515,   516,   517,   518,   519,   520,     0,   521,
       0,     0,     0,     0,     0,     0,  1093,     0,     0,     0,
       0,     0,     0,     0,   507,   508,   509,     0,     0,     0,
       0,   504,   505,   506,   512,   513,   510,   511,     0,     0,
       0,     0,     0,     0,     0,   504,   505,   506,     0,     0,
       0,     0,     0,     0,     0,   514,   515,   516,   517,   518,
     519,   520,     0,   521,   507,   508,   509,   775,   744,     0,
       0,     0,     0,     0,     0,     0,   510,   511,   507,   508,
     509,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     510,   511,   504,   505,   506,   512,   513,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,   506,     0,
       0,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,   507,   508,   509,   777,   744,
       0,     0,     0,     0,   512,   513,     0,   510,   511,   507,
     508,   509,     0,     0,     0,   504,   505,   506,     0,     0,
       0,   510,   511,     0,     0,   514,   515,   516,   517,   518,
     519,   520,     0,   521,     0,     0,     0,   779,   744,     0,
       0,     0,     0,     0,   512,   513,     0,     0,   507,   508,
     509,     0,     0,     0,     0,   504,   505,   506,   512,   513,
     510,   511,     0,     0,     0,   514,   515,   516,   517,   518,
     519,   520,     0,   521,     0,     0,     0,   784,   744,   514,
     515,   516,   517,   518,   519,   520,     0,   521,   507,   508,
     509,   786,   744,     0,     0,     0,     0,     0,     0,     0,
     510,   511,     0,     0,     0,   512,   513,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   504,   505,   506,   512,
     513,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   788,   744,
     514,   515,   516,   517,   518,   519,   520,     0,   521,   507,
     508,   509,   791,   744,     0,   504,   505,   506,   512,   513,
       0,   510,   511,     0,     0,     0,     0,     0,     0,   504,
     505,   506,     0,     0,     0,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,   507,   508,
     509,   793,   744,     0,     0,     0,     0,     0,   512,   513,
     510,   511,   507,   508,   509,   504,   505,   506,     0,     0,
       0,     0,     0,     0,   510,   511,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,   795,   744,     0,     0,     0,     0,     0,   507,   508,
     509,     0,     0,     0,   504,   505,   506,     0,     0,     0,
     510,   511,     0,     0,     0,     0,     0,     0,     0,   512,
     513,   504,   505,   506,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   507,   508,   509,
     514,   515,   516,   517,   518,   519,   520,     0,   521,   510,
     511,     0,   802,   744,   507,   508,   509,     0,   512,   513,
       0,     0,     0,     0,     0,     0,   510,   511,     0,   504,
     505,   506,   512,   513,     0,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,   804,   744,   514,   515,   516,   517,   518,   519,   520,
       0,   521,   507,   508,   509,   806,   744,     0,   512,   513,
     504,   505,   506,     0,   510,   511,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,   810,   744,   507,   508,   509,     0,   512,   513,   504,
     505,   506,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,     0,     0,   512,   513,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     812,   744,   507,   508,   509,   514,   515,   516,   517,   518,
     519,   520,     0,   521,   510,   511,     0,   814,   744,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   512,   513,   504,   505,   506,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   514,   515,   516,   517,   518,   519,   520,
       0,   521,     0,     0,     0,   816,   744,   507,   508,   509,
     504,   505,   506,   512,   513,     0,     0,     0,     0,   510,
     511,     0,     0,     0,   504,   505,   506,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,   507,   508,   509,   819,   744,     0,     0,
       0,     0,   512,   513,     0,   510,   511,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,   514,   515,   516,   517,   518,   519,   520,
       0,   521,     0,     0,     0,   821,   744,     0,     0,     0,
     504,   505,   506,   507,   508,   509,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   512,   513,     0,
       0,     0,     0,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     823,   744,     0,   512,   513,     0,     0,     0,     0,   507,
     508,   509,     0,     0,   504,   505,   506,   512,   513,     0,
       0,   510,   511,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   825,   744,   514,   515,
     516,   517,   518,   519,   520,     0,   521,   507,   508,   509,
     829,   744,     0,   512,   513,   504,   505,   506,     0,   510,
     511,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,   512,   513,     0,   831,   744,   507,   508,
     509,   504,   505,   506,     0,     0,     0,     0,     0,     0,
     510,   511,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   834,   744,     0,   512,
     513,   504,   505,   506,   507,   508,   509,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   510,   511,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   941,   744,   507,   508,   509,   512,   513,   504,
     505,   506,     0,     0,     0,     0,   510,   511,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     942,   744,   507,   508,   509,   504,   505,   506,   512,   513,
       0,     0,     0,     0,   510,   511,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,   507,   508,
     509,   943,   744,     0,   512,   513,   504,   505,   506,     0,
     510,   511,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   514,   515,   516,   517,   518,
     519,   520,     0,   521,   512,   513,     0,   944,   744,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,   514,   515,   516,   517,   518,
     519,   520,     0,   521,     0,     0,     0,  1174,  1175,     0,
       0,     0,   512,   513,     0,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,   514,   515,   516,   517,   518,   519,   520,
       0,   521,     0,     0,     0,     0,   744,     0,   512,   513,
       0,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,   514,
     515,   516,   517,   518,   519,   520,     0,   521,     0,     0,
       0,     0,   852,     0,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   853,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   855,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   864,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   865,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,   866,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,   867,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,   868,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   869,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   870,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   871,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   872,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,   873,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,   874,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,   875,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   876,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   877,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   878,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   880,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   881,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,   883,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,   884,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,   887,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   888,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   889,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   890,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   891,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,   892,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,   895,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,   896,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   897,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,   898,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,   899,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,   902,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1009,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1020,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1030,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1049,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1050,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,  1052,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,  1058,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1071,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1074,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1075,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1081,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1104,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1105,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,  1106,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1107,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1136,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1147,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1148,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1150,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1158,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,  1161,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,  1162,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1172,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1173,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1176,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1180,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1198,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1203,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,     0,  1204,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1210,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,     0,  1211,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
       0,  1212,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1213,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,  1214,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,     0,  1220,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   534,     0,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   536,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   538,     0,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     543,     0,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     545,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   547,     0,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   550,     0,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   552,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   556,     0,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   567,     0,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   569,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     571,     0,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   575,     0,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   577,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   579,     0,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   581,     0,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   582,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   584,     0,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     588,     0,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     593,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   596,     0,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   599,     0,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   605,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   783,     0,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   850,     0,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   851,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     885,     0,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   886,     0,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   927,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   929,     0,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   932,     0,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   934,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   940,     0,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     948,     0,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
     950,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,   952,     0,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   954,     0,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,   958,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,   959,     0,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,   961,     0,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,  1010,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1012,     0,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,  1014,     0,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,  1018,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,  1024,     0,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,  1032,     0,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,  1035,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,  1037,     0,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1039,     0,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1042,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,  1044,     0,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,  1047,     0,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,  1072,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,  1073,     0,
       0,   512,   513,   507,   508,   509,     0,     0,     0,     0,
     504,   505,   506,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,  1080,     0,     0,     0,     0,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,  1096,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1099,     0,     0,   512,   513,   507,   508,   509,     0,     0,
       0,     0,   504,   505,   506,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,  1101,     0,     0,     0,
       0,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,  1115,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,  1118,     0,     0,   512,   513,   507,   508,   509,
       0,     0,     0,     0,   504,   505,   506,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,  1137,     0,
       0,     0,     0,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,  1140,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,  1144,     0,     0,   512,   513,   507,
     508,   509,     0,     0,     0,     0,   504,   505,   506,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1154,     0,     0,     0,     0,     0,     0,   512,   513,   507,
     508,   509,   504,   505,   506,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1159,     0,     0,   512,   513,   507,   508,   509,   504,   505,
     506,     0,     0,     0,     0,     0,     0,   510,   511,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,  1177,     0,     0,   512,
     513,   507,   508,   509,     0,     0,     0,     0,   504,   505,
     506,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,  1185,     0,     0,     0,     0,     0,     0,   512,
     513,   507,   508,   509,   504,   505,   506,     0,     0,     0,
       0,     0,     0,   510,   511,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,     0,
       0,     0,  1190,     0,     0,   512,   513,   507,   508,   509,
     504,   505,   506,     0,     0,     0,     0,     0,     0,   510,
     511,     0,     0,     0,     0,     0,   514,   515,   516,   517,
     518,   519,   520,     0,   521,     0,     0,     0,  1192,     0,
       0,   512,   513,   507,   508,   509,   504,   505,   506,     0,
       0,     0,     0,     0,     0,   510,   511,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,     0,
     521,     0,     0,     0,  1200,     0,   504,   505,   506,   507,
     508,   512,   513,     0,     0,     0,     0,     0,     0,     0,
       0,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   514,   515,   516,   517,   518,   519,   520,   507,
     521,     0,     0,     0,  1205,     0,     0,   512,   513,     0,
       0,   510,   511,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   515,
     516,   517,   518,   519,   520,     0,   521,     0,     0,     0,
    1208,     0,     0,   512,   513,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   514,   515,   516,   517,   518,   519,
     520,     0,   521,     0,     0,     0,     0,     0,     0,   512,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521,   512,
     513,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     514,   515,   516,   517,   518,   519,   520,     0,   521
};

static const yytype_int16 yycheck[] =
{
       1,    35,     3,     4,     5,   201,     7,     8,   208,    10,
     201,   202,   201,    14,    87,    88,    17,   201,   201,    20,
     220,   217,    23,    24,    25,    26,   218,   223,   201,   221,
      31,    87,    88,    34,    35,    36,    37,   201,    39,   223,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     208,   201,   202,    54,    87,    88,    57,    19,   215,   216,
     201,    23,   220,   220,    65,    66,    67,    68,     3,     4,
      71,   201,   202,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   217,    21,    22,    23,    24,
      25,    26,    27,     0,   218,    30,    31,   221,    33,    34,
      35,    36,    37,    38,   217,    40,    41,   201,   202,   217,
      45,    46,   185,   186,   215,   216,   215,   216,   216,   220,
     217,   220,   215,   216,   215,   216,    20,   220,   217,   220,
     218,    42,    43,    44,   219,   220,   209,   210,   211,   212,
     215,   214,   218,   219,    79,    80,   218,   219,   215,   216,
      85,   215,   216,   209,   210,   211,   212,   217,   214,   217,
      95,    96,    97,    98,    99,   215,   216,   220,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   217,   211,   212,
     217,   214,   217,   118,   119,   120,   217,   122,   215,   216,
     219,   220,   193,   217,   129,   217,    42,    43,    44,   219,
     220,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   201,   217,   153,   217,
     217,   156,   157,   158,   159,   160,   161,   217,   217,   217,
     217,   166,   167,   217,   217,   217,   217,   172,   173,   217,
     175,   176,   217,   217,   179,   217,   181,   217,   217,   184,
     217,   217,   217,   188,   217,   219,   217,   217,   217,   260,
     217,   217,   217,   217,   217,   217,   201,   202,   217,   217,
     205,   272,   273,   217,   217,   217,   217,   217,   217,   280,
     215,   217,   283,   217,   285,    42,    43,    44,   289,   217,
     291,   217,   293,   217,   295,   206,   207,   208,   217,   210,
     217,   302,   217,   217,   305,   217,   208,   308,   217,   310,
     220,   222,   217,   217,   217,   316,   217,   318,   319,   320,
     321,   217,   217,   324,   217,   217,   327,   217,   217,   330,
     217,   332,   217,   217,   217,   217,   217,   338,   217,   340,
     217,   217,   343,    28,   345,   217,   217,   217,   217,   350,
     384,   217,   217,   217,   355,   217,   217,    42,    43,    44,
     206,   207,   208,   217,   210,   366,   217,   368,   217,   219,
     371,   219,   218,   374,   219,   376,   219,   378,   219,   380,
     219,   382,   219,   384,   385,   214,   387,   219,   219,   219,
      75,    76,    77,   219,   395,   396,   397,   398,   399,   219,
     219,   219,    87,    88,   219,   219,   219,   408,   409,   219,
     411,   412,   413,   414,   415,   416,   417,   219,   419,   420,
     421,   422,   423,   424,   219,   219,   427,   428,   429,   430,
     219,   219,   219,   219,   219,   219,   219,   219,   439,   440,
     441,   442,   443,   444,   445,   446,   219,   219,   219,   206,
     207,   208,   219,   210,   219,   219,   219,   219,   219,   219,
     219,   218,   219,   219,   219,   219,   219,   219,   219,    42,
      43,    44,   219,   219,   219,   219,   219,   219,   479,   480,
     481,   217,   217,   224,   217,   486,   487,   217,   220,   220,
     491,   225,   210,   208,   208,   221,   225,   224,   499,   500,
     185,   186,   221,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,   514,   515,   516,   517,   518,   519,   520,
     521,   206,   207,   208,   209,   210,   211,   212,   529,   214,
     531,   532,   221,   534,   221,   536,   208,   538,   208,   201,
     219,    78,   543,   783,   545,   389,   547,   548,   980,    -1,
     392,   391,    -1,    -1,    -1,   556,   557,    -1,   559,    -1,
     561,    -1,   563,     5,     6,     7,   567,    -1,    -1,    -1,
     571,    -1,   573,    -1,   575,    -1,   577,    -1,    -1,    -1,
     581,   582,    -1,   584,    -1,    -1,    -1,   588,    -1,    -1,
      32,    -1,   593,    -1,    -1,    -1,   597,    42,    43,    44,
      -1,    -1,    -1,    -1,   605,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,   206,   207,   208,    78,   210,    -1,    81,
      82,    83,    84,    -1,    86,   218,   219,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,
     102,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
     112,   113,   114,   115,   116,   117,    -1,    -1,    -1,   121,
      -1,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,    42,    43,    44,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,
     182,   183,    -1,    -1,    -1,   187,    -1,   189,   190,   191,
     192,   193,   194,   195,   196,    -1,   198,   199,   200,   201,
     202,   206,   207,   208,    -1,   210,    -1,   209,    -1,    -1,
      -1,    -1,    -1,   218,   775,   217,   777,    -1,   779,    -1,
      -1,   223,   783,   784,    -1,   786,    -1,   788,    -1,    -1,
      -1,    -1,    -1,    -1,   795,    -1,    -1,    -1,    -1,    -1,
      -1,   802,    -1,    -1,    -1,   806,    -1,   808,    -1,   810,
      -1,   812,    -1,   814,    -1,   816,    -1,    -1,   819,    -1,
     821,    -1,   823,    -1,    -1,   826,    -1,    -1,   206,   207,
     208,    -1,   210,   834,    -1,    -1,    -1,    -1,    -1,   840,
     218,   219,    42,    43,    44,   846,    -1,   848,   849,   850,
     851,    -1,    -1,    -1,    -1,    -1,   857,   858,   859,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,   206,   207,
     208,    -1,   210,    -1,   885,   886,    -1,    -1,    -1,    -1,
     218,   219,    -1,    75,    76,    77,    -1,    -1,   932,    -1,
      -1,    -1,    42,    43,    44,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   914,    -1,    -1,    -1,    -1,   919,    -1,
     921,    -1,   923,    -1,    -1,    -1,   927,    -1,   929,     1,
       2,   932,    -1,   934,    -1,   936,    -1,    -1,    -1,   940,
     941,   942,   943,   944,    42,    43,    44,   948,    -1,   950,
      -1,    23,    24,   954,    -1,    -1,    -1,   958,   959,    -1,
     961,    42,    43,    44,    36,   966,    38,    39,    40,    -1,
      -1,    -1,    -1,    -1,    -1,   976,    -1,    -1,    -1,    51,
      42,    43,    44,    -1,    56,    -1,    58,    -1,    60,    61,
      -1,    -1,    -1,    -1,    66,    67,    -1,    69,    70,    -1,
      72,    -1,    -1,   185,   186,    29,   206,   207,   208,  1010,
     210,  1012,    -1,  1014,    -1,    -1,    -1,  1018,    42,    43,
      44,    -1,    -1,  1024,   206,   207,   208,   209,   210,   211,
     212,  1032,   214,    -1,  1035,   206,   207,   208,  1039,   210,
     222,  1042,    -1,  1044,    -1,    -1,  1047,   218,   219,    -1,
    1051,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,  1099,   206,   207,   208,    -1,
     210,  1072,  1073,    -1,    -1,  1076,    -1,  1078,   218,  1080,
      -1,    -1,    -1,  1084,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1096,    -1,    -1,  1099,    -1,
    1101,  1102,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,  1112,   210,    -1,  1115,    -1,    -1,  1118,    -1,    -1,
     218,   219,    -1,  1124,    -1,   206,   207,   208,    -1,   210,
      -1,    -1,    -1,    -1,    -1,    -1,  1137,   218,   219,  1140,
      -1,    -1,    -1,  1144,   206,   207,   208,    -1,   210,    -1,
    1151,  1185,    -1,  1154,    -1,    -1,   218,   219,  1159,    -1,
      -1,   185,   186,  1164,    -1,    -1,    -1,  1168,    -1,    -1,
      -1,    -1,    -1,  1174,    -1,    -1,  1177,  1178,    42,    43,
      44,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,  1192,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,   273,    -1,    42,    43,    44,    -1,  1208,   280,    -1,
      -1,   283,    -1,   285,    -1,    -1,    -1,    -1,    -1,   291,
      -1,   293,    -1,   295,    42,    43,    44,    75,    76,    77,
     302,    -1,    -1,   305,    -1,    -1,   308,    -1,   310,    87,
      88,    -1,    -1,    -1,   316,    -1,    -1,    -1,    -1,   206,
     207,   208,   324,   210,    -1,   327,    -1,    -1,   330,    -1,
     332,   218,   219,    42,    43,    44,   338,    -1,   340,    -1,
      -1,   343,    -1,   345,    -1,    -1,    -1,    -1,   350,    -1,
      -1,    -1,    -1,   355,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   366,    -1,   368,    -1,    -1,   371,
      -1,    -1,   374,    -1,   376,    -1,   378,    -1,   380,    -1,
     382,    42,    43,    44,   386,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,   398,    -1,   400,   401,
     402,   403,   404,   405,   406,   407,    -1,   185,   186,    -1,
      -1,    -1,   206,   207,   208,    -1,   210,    -1,    -1,    42,
      43,    44,    -1,    -1,   426,   219,   428,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,   206,   207,   208,
     218,   210,    -1,   221,    -1,   447,    -1,    -1,    -1,   218,
     219,    -1,    75,    76,    77,    -1,    -1,    -1,   206,   207,
     208,    -1,   210,    -1,    87,    88,    -1,    -1,    -1,    -1,
     218,   219,    -1,    -1,    -1,   477,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,   489,   490,    -1,
      -1,   493,   494,   495,   496,    -1,    -1,   206,   207,   208,
      -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     219,    -1,    -1,    -1,    -1,    -1,   518,    -1,    -1,    -1,
      -1,   523,   524,   525,   526,   527,   528,   529,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,   208,   550,   210,
     552,    -1,   206,   207,   208,   557,   210,   559,   219,   561,
      -1,   563,   185,   186,    -1,   219,    -1,   569,    -1,    -1,
      -1,    -1,     5,     6,     7,    -1,    -1,   579,    -1,    -1,
      -1,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,    -1,    -1,   596,   218,    -1,   599,   221,    32,
      -1,   603,    -1,    -1,    -1,   607,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   615,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   206,   207,   208,    78,   210,    -1,    81,    82,
      83,    84,    -1,    86,    -1,   219,    89,    90,    91,    92,
      93,    94,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,    -1,   121,    -1,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
      -1,   154,   155,    42,    43,    44,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,   174,    -1,    -1,   177,   178,    -1,   180,    -1,   182,
     183,    -1,    -1,    -1,   187,    -1,   189,   190,    -1,   192,
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
      -1,    -1,    42,    43,    44,    -1,   209,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   217,   218,    -1,    -1,    -1,   791,
      -1,   793,   225,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,   804,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,   814,    -1,    -1,    -1,    -1,    87,    88,    42,
      43,    44,    -1,   825,    -1,    -1,    -1,   829,    -1,   831,
     832,    -1,    -1,    -1,   836,    -1,   838,   839,    -1,    -1,
     842,   843,    -1,    -1,    42,    43,    44,    -1,    -1,    42,
      43,    44,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
     862,   863,    -1,    -1,    87,    88,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    75,    76,    77,    -1,    -1,   206,   207,   208,
      -1,   210,    -1,    -1,    87,    88,    -1,    -1,    -1,   901,
     219,   903,   904,   905,   906,   907,    -1,   909,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,   948,    -1,   218,    -1,
     952,   221,   954,    -1,    75,    76,    77,   206,   207,   208,
      -1,   210,   185,   186,    -1,    -1,    87,    88,   970,    -1,
     219,    -1,    -1,    42,    43,    44,    -1,    -1,   980,    -1,
     982,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,   185,   186,    -1,   218,    -1,    -1,   221,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,   206,   207,
     208,    -1,   210,   206,   207,   208,   209,   210,   211,   212,
     218,   214,    -1,    -1,    -1,   218,    -1,    -1,   221,    -1,
    1032,    -1,   206,   207,   208,  1037,   210,  1039,    75,    76,
      77,    -1,   206,   207,   208,   219,   210,    -1,    -1,    -1,
      87,    88,    -1,    -1,  1056,   219,    -1,    -1,  1060,  1061,
      -1,    -1,  1064,  1065,   185,   186,    -1,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1083,    -1,    -1,  1086,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,   206,   207,   208,   218,   210,    -1,
     221,    -1,    -1,    -1,    -1,    -1,    -1,   219,    -1,    -1,
      -1,    -1,    -1,  1115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,
      -1,   210,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     219,    -1,  1154,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     5,     6,     7,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,  1190,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,  1205,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   206,   207,   208,    78,   210,    -1,    81,    82,
      83,    84,    -1,    86,    -1,   219,    89,    90,    91,    92,
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
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
     206,   207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,
      -1,    -1,    -1,   219,   217,   218,    42,    43,    44,    -1,
      -1,    -1,   225,    -1,    48,    49,    50,    51,    52,    53,
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
       5,     6,     7,   187,    -1,   189,   190,   191,   192,   193,
     194,   195,   196,    -1,   198,   199,   200,   201,   202,    -1,
     206,   207,   208,    -1,   210,   209,    -1,    32,    -1,    -1,
      -1,    -1,   218,   217,    42,    43,    44,    -1,    -1,   223,
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
     195,   196,    -1,   198,   199,   200,   201,   202,   206,   207,
     208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,
      -1,   219,   217,    42,    43,    44,    -1,    -1,   223,    -1,
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
     196,    -1,   198,   199,   200,   201,   202,   206,   207,   208,
      -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,    -1,
     219,   217,    42,    43,    44,    -1,    -1,   223,    -1,    -1,
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
      -1,   198,   199,   200,   201,   202,   206,   207,   208,    -1,
     210,    -1,   209,    -1,    32,    -1,    -1,    -1,    -1,   219,
     217,    42,    43,    44,    -1,    -1,   223,    -1,    -1,    -1,
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
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,    -1,
     198,   199,   200,   201,   202,   206,   207,   208,    -1,   210,
      -1,   209,    -1,    32,    -1,    -1,    -1,    -1,   219,   217,
      -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
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
     189,   190,    -1,   192,   193,    -1,   195,   196,    -1,   198,
     199,   200,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,
     209,    -1,    32,    -1,    -1,    -1,    -1,    -1,   217,    -1,
     219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,    -1,   198,   199,
     200,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,   209,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,
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
      -1,   192,   193,    -1,   195,   196,    -1,   198,   199,   200,
     201,   202,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,
      32,    -1,    -1,    -1,    -1,    -1,   217,    -1,   219,    -1,
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
     192,   193,    -1,   195,   196,    -1,   198,   199,   200,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   217,    -1,   219,    -1,    -1,
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
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   217,    -1,   219,    -1,    -1,    -1,
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
      -1,   195,   196,    -1,   198,   199,   200,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   217,    -1,   219,    -1,    -1,    -1,    -1,
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
     195,   196,    -1,   198,   199,   200,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,   209,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   217,    -1,   219,    -1,    -1,    -1,    -1,    -1,
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
     196,    -1,   198,   199,   200,   201,   202,    -1,    -1,    -1,
      -1,    -1,    -1,   209,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   217,    -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   198,   199,   200,   201,   202,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    32,    -1,    -1,    -1,    -1,    -1,
     217,    -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,    -1,
     198,   199,   200,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,   209,    -1,    32,    -1,    -1,    -1,    -1,    -1,   217,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,
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
     189,   190,    -1,   192,   193,    -1,   195,   196,    -1,   198,
     199,   200,   201,   202,   206,   207,   208,    -1,   210,    -1,
     209,    -1,    32,    -1,    -1,    -1,    -1,   219,   217,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,    -1,   198,   199,
     200,   201,   202,   206,   207,   208,    -1,   210,    -1,   209,
      -1,    32,    -1,    -1,    -1,    -1,   219,   217,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
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
      -1,   192,   193,    -1,   195,   196,    -1,   198,   199,   200,
     201,   202,   206,   207,   208,    -1,   210,    -1,   209,    -1,
      32,    -1,    -1,    -1,    -1,   219,   217,    42,    43,    44,
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
     192,   193,    -1,   195,   196,    -1,   198,   199,   200,   201,
     202,   206,   207,   208,    -1,   210,    -1,   209,    -1,    32,
      -1,    -1,    -1,    -1,   219,   217,    42,    43,    44,    -1,
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
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
     206,   207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,
      -1,    -1,    -1,   219,   217,    42,    43,    44,    -1,    -1,
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
      -1,   195,   196,    -1,   198,   199,   200,   201,   202,   206,
     207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,
      -1,    -1,   219,   217,    42,    43,    44,    -1,    -1,    -1,
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
     195,   196,    -1,   198,   199,   200,   201,   202,   206,   207,
     208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,
      -1,   219,   217,    42,    43,    44,    -1,    -1,    -1,    -1,
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
     196,    -1,   198,   199,   200,   201,   202,   206,   207,   208,
      -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,    -1,
     219,   217,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
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
      -1,   198,   199,   200,   201,   202,   206,   207,   208,    -1,
     210,    -1,   209,    -1,    32,    -1,    -1,    -1,    -1,   219,
     217,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,    -1,
     198,   199,   200,   201,   202,   206,   207,   208,    -1,   210,
      -1,   209,    -1,    32,    -1,    -1,    -1,    -1,   219,   217,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,
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
     189,   190,    -1,   192,   193,    -1,   195,   196,    -1,   198,
     199,   200,   201,   202,   206,   207,   208,    -1,   210,    -1,
     209,    -1,    32,    -1,    -1,    -1,    -1,   219,   217,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,    -1,   198,   199,
     200,   201,   202,   206,   207,   208,    -1,   210,    -1,   209,
      -1,    32,    -1,    -1,    -1,    -1,   219,   217,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
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
      -1,   192,   193,    -1,   195,   196,    -1,   198,   199,   200,
     201,   202,   206,   207,   208,    -1,   210,    -1,   209,    -1,
      32,    -1,    -1,    -1,   218,    -1,   217,    42,    43,    44,
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
     192,   193,    -1,   195,   196,    -1,   198,   199,   200,   201,
     202,   206,   207,   208,    -1,   210,    -1,   209,    -1,    32,
      -1,    -1,    -1,   218,    -1,   217,    42,    43,    44,    -1,
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
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
     206,   207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,
      -1,    -1,   218,    -1,   217,    42,    43,    44,    -1,    -1,
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
      -1,   195,   196,    -1,   198,   199,   200,   201,   202,   206,
     207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,
      -1,   218,    -1,   217,    42,    43,    44,    -1,    -1,    -1,
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
     195,   196,    -1,   198,   199,   200,   201,   202,   206,   207,
     208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,
     218,    -1,   217,    42,    43,    44,    -1,    -1,    -1,    -1,
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
     196,    -1,   198,   199,   200,   201,   202,   206,   207,   208,
      -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,   218,
      -1,   217,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
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
      -1,   198,   199,   200,   201,   202,   206,   207,   208,    -1,
     210,    -1,   209,    -1,    32,    -1,    -1,    -1,   218,    -1,
     217,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,    -1,
     198,   199,   200,   201,   202,   206,   207,   208,    -1,   210,
      -1,   209,    -1,    32,    -1,    -1,    -1,   218,    -1,   217,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,
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
     189,   190,    -1,   192,   193,    -1,   195,   196,    -1,   198,
     199,   200,   201,   202,   206,   207,   208,    -1,   210,    -1,
     209,    -1,    32,    -1,    -1,    -1,   218,    -1,   217,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,    -1,   198,   199,
     200,   201,   202,   206,   207,   208,    -1,   210,    -1,   209,
      -1,    32,    -1,    -1,    -1,   218,    -1,   217,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
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
      -1,   192,   193,    -1,   195,   196,    -1,   198,   199,   200,
     201,   202,   206,   207,   208,    -1,   210,    -1,   209,    -1,
      32,    -1,    -1,    -1,   218,    -1,   217,    42,    43,    44,
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
     192,   193,    -1,   195,   196,    -1,   198,   199,   200,   201,
     202,   206,   207,   208,    -1,   210,    -1,   209,    -1,    32,
      -1,    -1,    -1,   218,    -1,   217,    42,    43,    44,    -1,
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
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
     206,   207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,
      -1,    -1,   218,    -1,   217,    42,    43,    44,    -1,    -1,
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
      -1,   195,   196,    -1,   198,   199,   200,   201,   202,   206,
     207,   208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,
      -1,   218,    -1,   217,    42,    43,    44,    -1,    -1,    -1,
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
     195,   196,    -1,   198,   199,   200,   201,   202,   206,   207,
     208,    -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,
     218,    -1,   217,    42,    43,    44,    -1,    -1,    -1,    -1,
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
     196,    -1,   198,   199,   200,   201,   202,   206,   207,   208,
      -1,   210,    -1,   209,    -1,    32,    -1,    -1,    -1,   218,
      -1,   217,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
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
      -1,   198,   199,   200,   201,   202,   206,   207,   208,    -1,
     210,    -1,   209,    -1,    32,    -1,    -1,    -1,   218,    -1,
     217,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   189,   190,    -1,   192,   193,    -1,   195,   196,    -1,
     198,   199,   200,   201,   202,   206,   207,   208,    -1,   210,
      -1,   209,    -1,    32,    -1,    -1,    -1,   218,    -1,   217,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,
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
     189,   190,    -1,   192,   193,    -1,   195,   196,    -1,   198,
     199,   200,   201,   202,   206,   207,   208,    -1,   210,    -1,
     209,    -1,    32,    -1,    -1,    -1,   218,    -1,   217,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
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
     190,    -1,   192,   193,    -1,   195,   196,    -1,   198,   199,
     200,   201,   202,   206,   207,   208,    -1,   210,    -1,   209,
      -1,    32,    -1,    -1,    -1,   218,    -1,   217,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
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
      -1,   192,   193,    -1,   195,   196,    -1,   198,   199,   200,
     201,   202,   206,   207,   208,    -1,   210,    -1,   209,    -1,
      32,    -1,    -1,    -1,   218,    -1,   217,    -1,    -1,    -1,
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
     192,   193,    -1,   195,   196,    -1,   198,   199,   200,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    32,
      -1,    -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,
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
     193,    -1,   195,   196,    -1,   198,   199,   200,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   187,    -1,   189,   190,    -1,   192,   193,
      -1,   195,   196,    -1,   198,   199,   200,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,     3,
       4,    -1,    -1,   217,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    -1,    40,    41,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,    -1,    -1,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118,   119,   120,    -1,   122,    -1,
      -1,    -1,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   153,
      -1,    -1,   156,   157,   158,   159,   160,   161,    -1,    -1,
      -1,    -1,   166,   167,    -1,    -1,    -1,    -1,   172,   173,
      -1,   175,   176,    -1,    -1,   179,    -1,   181,    -1,    -1,
     184,    -1,    -1,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,     3,
       4,   205,    -1,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    -1,    40,    41,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,
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
     184,     3,     4,    -1,   188,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,   201,   202,    -1,
      -1,    -1,    -1,    87,    88,    27,   185,   186,    30,    31,
      -1,    33,    34,    35,    36,    37,    38,    -1,    40,    41,
      -1,    -1,    -1,    45,    46,    -1,    -1,   206,   207,   208,
     209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,
      -1,    -1,   221,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,
      -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,    75,    76,
      77,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      87,    88,    -1,    -1,    -1,    -1,   118,   119,   120,    -1,
     122,   185,   186,    -1,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
      42,    43,    44,    -1,   166,   167,    -1,    -1,    -1,    -1,
     172,   173,    -1,   175,   176,    -1,    -1,   179,    -1,   181,
      -1,    -1,   184,    -1,    -1,    -1,   188,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    87,    88,    -1,   185,   186,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,    -1,    -1,    -1,   221,    75,    76,    77,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    75,    76,
      77,    -1,    -1,   185,   186,    -1,    -1,    42,    43,    44,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,   221,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,   185,   186,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,    -1,
      -1,   221,   185,   186,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,   185,   186,
      -1,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,    42,    43,    44,    -1,    -1,    -1,   221,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,
     185,   186,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    87,    88,    -1,
      -1,   206,   207,   208,   209,   210,   211,   212,    -1,   214,
      -1,    -1,    -1,    -1,    -1,    -1,   221,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    42,    43,    44,   185,   186,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,    75,    76,    77,   218,   219,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    42,    43,    44,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    75,    76,    77,   218,   219,
      -1,    -1,    -1,    -1,   185,   186,    -1,    87,    88,    75,
      76,    77,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    87,    88,    -1,    -1,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,    -1,    -1,    -1,   218,   219,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    42,    43,    44,   185,   186,
      87,    88,    -1,    -1,    -1,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,    -1,    -1,    -1,   218,   219,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    75,    76,
      77,   218,   219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,   185,
     186,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,   219,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    75,
      76,    77,   218,   219,    -1,    42,    43,    44,   185,   186,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    75,    76,
      77,   218,   219,    -1,    -1,    -1,    -1,    -1,   185,   186,
      87,    88,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,   218,   219,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    87,
      88,    -1,   218,   219,    75,    76,    77,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    42,
      43,    44,   185,   186,    -1,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,   218,   219,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,    75,    76,    77,   218,   219,    -1,   185,   186,
      42,    43,    44,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,   218,   219,    75,    76,    77,    -1,   185,   186,    42,
      43,    44,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,   219,    75,    76,    77,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,    87,    88,    -1,   218,   219,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,    -1,    -1,    -1,   218,   219,    75,    76,    77,
      42,    43,    44,   185,   186,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    75,    76,    77,   218,   219,    -1,    -1,
      -1,    -1,   185,   186,    -1,    87,    88,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,    -1,    -1,    -1,   218,   219,    -1,    -1,    -1,
      42,    43,    44,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,   219,    -1,   185,   186,    -1,    -1,    -1,    -1,    75,
      76,    77,    -1,    -1,    42,    43,    44,   185,   186,    -1,
      -1,    87,    88,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,   219,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    75,    76,    77,
     218,   219,    -1,   185,   186,    42,    43,    44,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,   185,   186,    -1,   218,   219,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,   219,    -1,   185,
     186,    42,    43,    44,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,   219,    75,    76,    77,   185,   186,    42,
      43,    44,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,   219,    75,    76,    77,    42,    43,    44,   185,   186,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    75,    76,
      77,   218,   219,    -1,   185,   186,    42,    43,    44,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,   185,   186,    -1,   218,   219,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,   206,   207,   208,   209,   210,
     211,   212,    -1,   214,    -1,    -1,    -1,   218,   219,    -1,
      -1,    -1,   185,   186,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,   206,   207,   208,   209,   210,   211,   212,
      -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,   206,
     207,   208,   209,   210,   211,   212,    -1,   214,    -1,    -1,
      -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,    -1,   219,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,    -1,   219,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
      -1,   219,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,   219,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,    -1,   219,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    75,
      76,    77,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,   218,    -1,    -1,   185,
     186,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,    -1,
      -1,    -1,   218,    -1,    -1,   185,   186,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,   206,   207,   208,   209,
     210,   211,   212,    -1,   214,    -1,    -1,    -1,   218,    -1,
      -1,   185,   186,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    -1,
     214,    -1,    -1,    -1,   218,    -1,    42,    43,    44,    75,
      76,   185,   186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   206,   207,   208,   209,   210,   211,   212,    75,
     214,    -1,    -1,    -1,   218,    -1,    -1,   185,   186,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,    -1,    -1,    -1,
     218,    -1,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214,   185,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     206,   207,   208,   209,   210,   211,   212,    -1,   214
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
     181,   184,   188,   201,   202,   205,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,     5,     6,     7,    32,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    78,    81,    82,    83,    84,    86,    89,
      90,    91,    92,    93,    94,   100,   101,   102,   112,   113,
     114,   115,   116,   117,   121,   123,   124,   125,   126,   127,
     128,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   154,   155,   162,   163,   164,   165,
     168,   169,   170,   171,   174,   177,   178,   180,   182,   183,
     187,   189,   190,   192,   193,   195,   196,   198,   199,   200,
     201,   202,   209,   217,   316,   318,   201,   202,   316,   318,
     217,   316,   318,   217,   316,   217,   316,   201,   217,   223,
     314,   217,   316,   217,   316,   217,   316,   316,   316,   316,
     201,   201,   217,   316,   318,   217,   316,   318,   217,   316,
     217,   316,   201,   201,    19,    23,   217,   316,   201,   202,
     201,   202,   316,   201,   217,   314,   316,   217,   316,   318,
     217,   316,   217,   318,   316,   318,   318,   316,   217,   316,
     217,   316,   316,   217,   316,   217,   316,   316,   316,   316,
     217,   316,   318,   217,   217,   217,   316,   217,   318,   316,
     318,   217,   318,   318,   217,   201,   217,   316,   217,   316,
     318,   217,   316,   318,   217,   316,   217,   318,   217,   318,
     217,   316,   217,   318,   208,   220,   208,   220,     0,   215,
     230,   216,    20,   218,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   316,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   316,   217,   217,   217,   217,   217,   217,   220,
     220,   316,   316,   318,    42,    43,    44,    75,    76,    77,
      87,    88,   185,   186,   206,   207,   208,   209,   210,   211,
     212,   214,   222,    42,    43,    44,   206,   207,   208,   210,
     222,   220,   220,   316,   218,   316,   218,   316,   218,   201,
     315,   316,   316,   218,   316,   218,   316,   218,   208,   316,
     218,   316,   218,   219,   219,   316,   218,   217,   316,   217,
     316,   217,   316,   217,   316,   201,   316,   218,   316,   218,
     316,   218,   318,   218,   316,   218,   316,   218,   316,   218,
     316,   218,   218,   316,   218,   219,   219,   316,   218,   219,
     219,   219,   316,   218,   316,   318,   218,   218,   316,   218,
     219,   316,   318,   218,   316,   218,   318,   218,   191,   194,
     314,   316,   316,   191,   194,   223,   313,   318,   316,   227,
      40,   242,   241,   201,   202,   219,   316,   316,   219,   316,
     316,   318,   316,   318,   318,   318,   318,   318,   318,   318,
     318,   316,   316,   219,   316,   316,   316,   316,   316,   316,
     316,   219,   316,   316,   316,   316,   316,   316,   318,   316,
     316,   318,   219,   316,   219,   316,   219,   219,   219,   219,
     219,   219,   219,   219,   316,   316,   316,   316,   316,   316,
     316,   219,   316,   318,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   318,   219,   316,   316,   316,   219,
     219,   219,   219,   219,   316,   219,   316,   219,   318,   318,
     316,   318,   318,   318,   318,   201,   202,   219,   218,   225,
     316,   218,   225,   316,   219,   219,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   318,   316,   316,   316,   318,   318,   318,   318,   318,
     318,   316,   318,   316,   316,   218,   316,   218,   316,   218,
     316,   219,   224,   218,   218,   316,   218,   316,   218,   316,
     316,   218,   318,   218,   318,   218,   316,   316,   316,   316,
     316,   219,   218,   316,   218,   318,   218,   316,   218,   316,
     218,   316,   218,   316,   218,   318,   218,   316,   316,   218,
     316,   218,   316,   218,   316,   218,   218,   318,   316,   218,
     318,   218,   218,   318,   218,   316,   218,   318,   217,   217,
     218,   221,   217,   217,   317,   318,   218,   221,   220,   220,
     218,   218,   219,   219,   219,   219,   219,   218,   218,   218,
     219,   219,   218,   218,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   218,   218,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   218,   219,   218,   218,   218,   218,   218,   219,   218,
     219,   225,   218,   221,   218,   221,   225,   218,   221,   218,
     221,   218,   221,   218,   221,   316,   316,   218,   316,   218,
     315,   316,   218,   316,   218,   316,    28,   318,   318,   316,
     218,   218,   218,   218,   218,   316,   318,   316,   218,   316,
     218,   316,   218,   316,   218,   316,   318,   316,   218,   218,
     316,   218,   316,   316,   318,   316,   218,   318,   318,   318,
     218,   316,   318,   318,   318,   316,   208,   318,   318,   224,
     218,   316,   208,   316,   316,   316,   316,   316,   316,   316,
     318,   318,   316,   316,   318,   318,   318,   318,   318,   318,
     318,   221,   221,   316,   221,   221,   316,   316,   316,   219,
     218,   316,   218,   316,   218,   201,   314,   316,   218,   316,
     219,   316,   219,   219,   218,   316,   316,   316,   316,   316,
     219,   219,   218,   316,   318,   218,   316,   218,   318,   218,
     316,   318,   218,   219,   218,   316,   316,   218,   316,   219,
     219,   218,   219,   316,   219,   219,   218,   318,   219,   219,
     218,   218,   221,   316,   218,   218,   317,   221,   318,   221,
     221,   219,   218,   218,   219,   219,   218,   219,   218,   219,
     218,   219,   219,   218,   218,   219,   218,   219,   219,   219,
     221,   221,   221,   221,   316,   316,   218,   201,   316,   218,
     316,   218,    29,   316,   219,   219,   219,   219,   316,   318,
     316,   318,   218,   316,   318,   218,   316,   316,   218,   316,
     316,   318,   318,   318,   208,   318,   318,   208,   316,   316,
     316,   316,   316,   318,   316,   318,   219,   218,   316,   219,
     218,   201,   314,   316,   218,   316,   316,   219,   219,   219,
     219,   218,   219,   316,   218,   219,   316,   318,   219,   218,
     316,   219,   219,   219,   218,   219,   219,   316,   218,   219,
     219,   318,   219,   219,   218,   219,   219,   218,   218,   219,
     219,   219,   316,   201,   316,   218,   316,   316,   316,   318,
     218,   316,   218,   316,   316,   316,   316,   316,   219,   219,
     218,   201,   314,   219,   219,   218,   219,   318,   218,   316,
     219,   219,   219,   219,   219,   201,   318,   316,   219,   219,
     219
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
#line 1032 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1033 "basicParse.y"
    { listlen++; ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1036 "basicParse.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 1037 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1038 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1039 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1040 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1041 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1042 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    {
		if ((yyvsp[(1) - (1)].number) < 0) {
			return -1;
		} else {
			addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1051 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1052 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1053 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1054 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1055 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1056 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1057 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1058 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1059 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1060 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1061 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1062 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 1063 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 1064 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 1065 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 1066 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 1067 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 1068 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 1069 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 1070 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 1071 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 1072 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 1073 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 1074 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 1075 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 1076 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 1077 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MD5); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 1078 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_GETSETTING); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 1079 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 1080 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 1081 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 1082 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE_C); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 1083 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACEX); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 1084 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 1085 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 1086 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 1087 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 9348 "basicParse.tab.c"
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
#line 1091 "basicParse.y"


int
yyerror(const char *msg) {
	errorcode = -1;
	if (yytext[0] == '\n') { linenumber--; } // error happened on previous line
	return -1;
}

