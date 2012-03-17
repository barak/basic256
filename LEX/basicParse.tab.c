
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
     B256TEXTWIDTH = 352,
     B256SAY = 353,
     B256SYSTEM = 354,
     B256VOLUME = 355,
     B256GRAPHWIDTH = 356,
     B256GRAPHHEIGHT = 357,
     B256GETSLICE = 358,
     B256PUTSLICE = 359,
     B256IMGLOAD = 360,
     B256SPRITEDIM = 361,
     B256SPRITELOAD = 362,
     B256SPRITESLICE = 363,
     B256SPRITEMOVE = 364,
     B256SPRITEHIDE = 365,
     B256SPRITESHOW = 366,
     B256SPRITEPLACE = 367,
     B256SPRITECOLLIDE = 368,
     B256SPRITEX = 369,
     B256SPRITEY = 370,
     B256SPRITEH = 371,
     B256SPRITEW = 372,
     B256SPRITEV = 373,
     B256WAVPLAY = 374,
     B256WAVSTOP = 375,
     B256WAVWAIT = 376,
     B256SIZE = 377,
     B256SEEK = 378,
     B256EXISTS = 379,
     B256BOOLTRUE = 380,
     B256BOOLFALSE = 381,
     B256MOUSEX = 382,
     B256MOUSEY = 383,
     B256MOUSEB = 384,
     B256CLICKCLEAR = 385,
     B256CLICKX = 386,
     B256CLICKY = 387,
     B256CLICKB = 388,
     B256GETCOLOR = 389,
     B256CLEAR = 390,
     B256BLACK = 391,
     B256WHITE = 392,
     B256RED = 393,
     B256DARKRED = 394,
     B256GREEN = 395,
     B256DARKGREEN = 396,
     B256BLUE = 397,
     B256DARKBLUE = 398,
     B256CYAN = 399,
     B256DARKCYAN = 400,
     B256PURPLE = 401,
     B256DARKPURPLE = 402,
     B256YELLOW = 403,
     B256DARKYELLOW = 404,
     B256ORANGE = 405,
     B256DARKORANGE = 406,
     B256GREY = 407,
     B256DARKGREY = 408,
     B256CHANGEDIR = 409,
     B256CURRENTDIR = 410,
     B256DIR = 411,
     B256DECIMAL = 412,
     B256DBOPEN = 413,
     B256DBCLOSE = 414,
     B256DBEXECUTE = 415,
     B256DBOPENSET = 416,
     B256DBCLOSESET = 417,
     B256DBROW = 418,
     B256DBINT = 419,
     B256DBFLOAT = 420,
     B256DBSTRING = 421,
     B256ONERROR = 422,
     B256OFFERROR = 423,
     B256LASTERROR = 424,
     B256LASTERRORMESSAGE = 425,
     B256LASTERRORLINE = 426,
     B256LASTERROREXTRA = 427,
     B256NETLISTEN = 428,
     B256NETCONNECT = 429,
     B256NETREAD = 430,
     B256NETWRITE = 431,
     B256NETCLOSE = 432,
     B256NETDATA = 433,
     B256NETADDRESS = 434,
     B256KILL = 435,
     B256MD5 = 436,
     B256SETSETTING = 437,
     B256GETSETTING = 438,
     B256PORTIN = 439,
     B256PORTOUT = 440,
     B256BINARYOR = 441,
     B256BINARYAND = 442,
     B256BINARYNOT = 443,
     B256IMGSAVE = 444,
     B256REPLACE = 445,
     B256COUNT = 446,
     B256EXPLODE = 447,
     B256REPLACEX = 448,
     B256COUNTX = 449,
     B256EXPLODEX = 450,
     B256IMPLODE = 451,
     B256OSTYPE = 452,
     B256MSEC = 453,
     B256EDITVISIBLE = 454,
     B256GRAPHVISIBLE = 455,
     B256OUTPUTVISIBLE = 456,
     B256EDITSIZE = 457,
     B256OUTPUTSIZE = 458,
     B256LINENUM = 459,
     B256INTEGER = 460,
     B256FLOAT = 461,
     B256STRING = 462,
     B256VARIABLE = 463,
     B256STRINGVAR = 464,
     B256NEWVAR = 465,
     B256COLOR = 466,
     B256LABEL = 467,
     B256UMINUS = 468
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
#line 538 "basicParse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 550 "basicParse.tab.c"

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
#define YYFINAL  399
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   19125

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  233
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  502
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1237

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   468

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     222,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     224,   226,   218,   217,   225,   216,     2,   219,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   223,   229,
     213,   215,   214,   232,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   227,     2,   228,   221,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   230,     2,   231,     2,     2,     2,     2,
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
     205,   206,   207,   208,   209,   210,   211,   212,   220
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
    1890,  1897,  1899,  1903,  1905,  1909,  1914,  1916,  1920,  1924,
    1928,  1932,  1936,  1938,  1943,  1950,  1952,  1957,  1962,  1967,
    1972,  1981,  1988,  1995,  2006,  2008,  2012,  2017,  2019,  2023,
    2028,  2030,  2034,  2039,  2041,  2045,  2047,  2051,  2053,  2057,
    2062,  2064,  2068,  2073,  2080,  2085,  2089,  2098,  2109,  2118,
    2123,  2130,  2135
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     234,     0,    -1,   235,   222,    -1,   235,   222,   234,    -1,
     236,   237,    -1,   237,    -1,   212,    -1,   238,    -1,   239,
      -1,   240,    -1,   242,    -1,   243,    -1,   245,    -1,   246,
      -1,   247,    -1,   248,    -1,    -1,   258,    20,   248,    -1,
     258,    20,    -1,    21,    -1,    22,    -1,    40,    19,    -1,
     241,    -1,    23,   326,    -1,    24,    -1,    40,    23,    -1,
     244,    -1,    25,    -1,    26,   326,    -1,   249,    -1,   248,
     223,   249,    -1,   265,    -1,   266,    -1,   267,    -1,   268,
      -1,   269,    -1,   291,    -1,   272,    -1,   274,    -1,   275,
      -1,   281,    -1,   282,    -1,   273,    -1,   261,    -1,   262,
      -1,   263,    -1,   264,    -1,   270,    -1,   289,    -1,   257,
      -1,   253,    -1,   256,    -1,   254,    -1,   255,    -1,   250,
      -1,   251,    -1,   252,    -1,   260,    -1,   259,    -1,   283,
      -1,   284,    -1,   285,    -1,   286,    -1,   287,    -1,   316,
      -1,   271,    -1,   276,    -1,   277,    -1,   278,    -1,   279,
      -1,   280,    -1,   292,    -1,   293,    -1,   294,    -1,   295,
      -1,   296,    -1,   297,    -1,   298,    -1,   299,    -1,   300,
      -1,   301,    -1,   302,    -1,   303,    -1,   288,    -1,   304,
      -1,   305,    -1,   306,    -1,   307,    -1,   308,    -1,   309,
      -1,   310,    -1,   311,    -1,   312,    -1,   313,    -1,   314,
      -1,   315,    -1,   317,    -1,   318,    -1,   319,    -1,   320,
      -1,   321,    -1,   322,    -1,    45,   208,   326,    -1,    45,
     209,   326,    -1,    45,   208,   224,   326,   225,   326,   226,
      -1,    45,   209,   224,   326,   225,   326,   226,    -1,    46,
     208,   326,    -1,    46,   209,   326,    -1,    46,   208,   224,
     326,   225,   326,   226,    -1,    46,   209,   224,   326,   225,
     326,   226,    -1,    79,   326,    -1,    17,    -1,    18,    -1,
      14,    -1,    15,   326,   225,   326,    -1,    15,   224,   326,
     225,   326,   226,    -1,    16,    -1,    40,    -1,    19,   326,
      -1,   209,   227,   326,   228,   215,   328,    -1,   209,   227,
     326,   225,   326,   228,   215,   328,    -1,   209,   215,   323,
      -1,   209,   215,   192,   224,   328,   225,   328,   226,    -1,
     209,   215,   192,   224,   328,   225,   328,   225,   326,   226,
      -1,   209,   215,   195,   224,   328,   225,   328,   226,    -1,
     208,   227,   326,   228,   215,   326,    -1,   208,   227,   326,
     225,   326,   228,   215,   326,    -1,   208,   215,   324,    -1,
     208,   215,   192,   224,   328,   225,   328,   226,    -1,   208,
     215,   192,   224,   328,   225,   328,   225,   326,   226,    -1,
     208,   215,   195,   224,   328,   225,   328,   226,    -1,   208,
     215,   326,    -1,   209,   215,   328,    -1,    27,   208,   215,
     326,    28,   326,    -1,    27,   208,   215,   326,    28,   326,
      29,   326,    -1,    30,   208,    -1,    36,   208,    -1,    37,
     208,    -1,   168,    -1,   167,   208,    -1,    38,    -1,    41,
     326,   225,   326,   225,   326,    -1,    41,   224,   326,   225,
     326,   225,   326,   226,    -1,    41,   326,    -1,    80,   224,
     208,   226,    -1,    80,   208,    -1,    80,   324,    -1,    80,
     224,   326,   225,   326,   226,    -1,    80,   326,   225,   326,
      -1,     8,   326,   225,   326,    -1,     8,   224,   326,   225,
     326,   226,    -1,    13,   326,   225,   326,   225,   326,   225,
     326,    -1,    13,   224,   326,   225,   326,   225,   326,   225,
     326,   226,    -1,     9,   326,   225,   326,   225,   326,    -1,
       9,   224,   326,   225,   326,   225,   326,   226,    -1,    10,
     326,   225,   326,   225,   326,   225,   326,    -1,    10,   224,
     326,   225,   326,   225,   326,   225,   326,   226,    -1,    95,
     326,   225,   326,   225,   328,    -1,    95,   224,   326,   225,
     326,   225,   328,   226,    -1,    95,   326,   225,   326,   225,
     326,    -1,    95,   224,   326,   225,   326,   225,   326,   226,
      -1,    96,   328,   225,   326,   225,   326,    -1,    96,   224,
     328,   225,   326,   225,   326,   226,    -1,    98,   328,    -1,
      98,   326,    -1,    99,   328,    -1,   100,   326,    -1,    11,
     208,    -1,    11,   224,   208,   226,    -1,    11,   324,    -1,
      12,   326,   225,   326,   225,   326,   225,   208,    -1,    12,
     224,   326,   225,   326,   225,   326,   225,   208,   226,    -1,
      12,   326,   225,   326,   225,   326,   225,   324,    -1,    12,
     326,   225,   326,   225,   208,    -1,    12,   224,   326,   225,
     326,   225,   208,   226,    -1,    12,   326,   225,   326,   225,
     324,    -1,    12,   326,   225,   326,   225,   326,   225,   326,
     225,   208,    -1,    12,   224,   326,   225,   326,   225,   326,
     225,   326,   225,   208,   226,    -1,    12,   326,   225,   326,
     225,   326,   225,   326,   225,   324,    -1,    31,   328,    -1,
      31,   224,   326,   225,   328,   226,    -1,    31,   326,   225,
     328,    -1,    33,   328,    -1,    33,   224,   326,   225,   328,
     226,    -1,    33,   326,   225,   328,    -1,    85,   328,    -1,
      85,   224,   326,   225,   328,   226,    -1,    85,   326,   225,
     328,    -1,    34,    -1,    34,   224,   226,    -1,    34,   326,
      -1,    35,    -1,    35,   224,   226,    -1,    35,   326,    -1,
     123,   326,    -1,   123,   224,   326,   225,   326,   226,    -1,
     123,   326,   225,   326,    -1,   290,   225,   209,    -1,   290,
     225,   209,   227,   326,   228,    -1,   290,   225,   208,    -1,
     290,   225,   208,   227,   326,   228,    -1,     4,   209,    -1,
       4,   209,   227,   326,   228,    -1,     4,   209,   227,   326,
     225,   326,   228,    -1,     4,   208,    -1,     4,   208,   227,
     326,   228,    -1,     4,   208,   227,   326,   225,   326,   228,
      -1,     4,   328,    -1,     3,    -1,     3,   328,    -1,     3,
     326,    -1,     3,   328,   229,    -1,     3,   326,   229,    -1,
     119,   328,    -1,   120,    -1,   120,   224,   226,    -1,   121,
      -1,   121,   224,   226,    -1,   104,   326,   225,   326,   225,
     328,    -1,   104,   224,   326,   225,   326,   225,   328,   226,
      -1,   104,   326,   225,   326,   225,   328,   225,   326,    -1,
     104,   224,   326,   225,   326,   225,   328,   225,   326,   226,
      -1,   105,   326,   225,   326,   225,   328,    -1,   105,   224,
     326,   225,   326,   225,   328,   226,    -1,   105,   326,   225,
     326,   225,   326,   225,   328,    -1,   105,   224,   326,   225,
     326,   225,   326,   225,   328,   226,    -1,   105,   326,   225,
     326,   225,   326,   225,   326,   225,   328,    -1,   105,   224,
     326,   225,   326,   225,   326,   225,   326,   225,   328,   226,
      -1,   106,   326,    -1,   107,   326,   225,   328,    -1,   107,
     224,   326,   225,   328,   226,    -1,   108,   326,   225,   326,
     225,   326,   225,   326,   225,   326,    -1,   108,   224,   326,
     225,   326,   225,   326,   225,   326,   225,   326,   226,    -1,
     112,   326,   225,   326,   225,   326,    -1,   112,   224,   326,
     225,   326,   225,   326,   226,    -1,   109,   326,   225,   326,
     225,   326,    -1,   107,   224,   326,   225,   326,   225,   326,
     226,    -1,   110,   326,    -1,   111,   326,    -1,   130,    -1,
     130,   224,   226,    -1,   154,   328,    -1,   157,   326,    -1,
     158,   328,    -1,   159,    -1,   159,   224,   226,    -1,   160,
     328,    -1,   161,   328,    -1,   162,    -1,   162,   224,   226,
      -1,   173,   326,    -1,   173,   224,   326,   225,   326,   226,
      -1,   173,   326,   225,   326,    -1,   174,   328,   225,   326,
      -1,   174,   224,   328,   225,   326,   226,    -1,   174,   326,
     225,   328,   225,   326,    -1,   174,   224,   326,   225,   328,
     225,   326,   226,    -1,   176,   328,    -1,   176,   224,   326,
     225,   328,   226,    -1,   176,   326,   225,   328,    -1,   177,
      -1,   177,   224,   226,    -1,   177,   326,    -1,   180,   328,
      -1,   180,   224,   326,   225,   328,   226,    -1,   182,   328,
     225,   328,   225,   328,    -1,   182,   224,   328,   225,   328,
     225,   328,   226,    -1,   185,   326,   225,   326,    -1,   185,
     224,   326,   225,   326,   226,    -1,   189,   328,    -1,   189,
     224,   328,   225,   328,   226,    -1,   189,   328,   225,   328,
      -1,   199,   326,    -1,   200,   326,    -1,   201,   326,    -1,
     230,   327,   231,    -1,   230,   325,   231,    -1,   326,    -1,
     326,   225,   325,    -1,   224,   326,   226,    -1,   326,   217,
     326,    -1,   326,   216,   326,    -1,   326,   218,   326,    -1,
     326,    87,   326,    -1,   326,    88,   326,    -1,   326,   219,
     326,    -1,   326,   221,   326,    -1,   326,   186,   326,    -1,
     326,   187,   326,    -1,   188,   326,    -1,   216,   326,    -1,
     326,    75,   326,    -1,   326,    76,   326,    -1,   326,    77,
     326,    -1,    78,   326,    -1,   328,   215,   328,    -1,   328,
      44,   328,    -1,   328,   213,   328,    -1,   328,   214,   328,
      -1,   328,    42,   328,    -1,   328,    43,   328,    -1,   326,
     215,   326,    -1,   326,    44,   326,    -1,   326,   213,   326,
      -1,   326,   214,   326,    -1,   326,    42,   326,    -1,   326,
      43,   326,    -1,   206,    -1,   205,    -1,   208,   227,   232,
     228,    -1,   209,   227,   232,   228,    -1,   208,   227,   232,
     225,   228,    -1,   209,   227,   232,   225,   228,    -1,   208,
     227,   225,   232,   228,    -1,   209,   227,   225,   232,   228,
      -1,   208,   227,   326,   228,    -1,   208,   227,   326,   225,
     326,   228,    -1,   208,    -1,    48,   224,   326,   226,    -1,
      48,   224,   328,   226,    -1,    83,   224,   326,   226,    -1,
      83,   224,   328,   226,    -1,    50,   224,   328,   226,    -1,
      81,   224,   328,   226,    -1,    56,   224,   328,   225,   328,
     226,    -1,    56,   224,   328,   225,   328,   225,   326,   226,
      -1,    56,   224,   328,   225,   328,   225,   326,   225,   326,
     226,    -1,    57,   224,   328,   225,   328,   226,    -1,    57,
     224,   328,   225,   328,   225,   326,   226,    -1,    58,   224,
     326,   226,    -1,    59,   224,   326,   226,    -1,    61,   224,
     326,   226,    -1,    62,   224,   326,   226,    -1,    63,   224,
     326,   226,    -1,    64,   224,   326,   226,    -1,    65,   224,
     326,   226,    -1,    66,   224,   326,   226,    -1,    69,   224,
     326,   226,    -1,    70,   224,   326,   226,    -1,    71,   224,
     326,   226,    -1,    72,   224,   326,   226,    -1,    73,   224,
     326,   226,    -1,    74,   224,   326,   226,    -1,    67,   224,
     326,   226,    -1,    60,    -1,    60,   224,   226,    -1,    68,
      -1,    68,   224,   226,    -1,   125,    -1,   125,   224,   226,
      -1,   126,    -1,   126,   224,   226,    -1,    86,    -1,    86,
     224,   226,    -1,    86,   224,   326,   226,    -1,   124,   224,
     328,   226,    -1,    89,    -1,    89,   224,   226,    -1,    90,
      -1,    90,   224,   226,    -1,    91,    -1,    91,   224,   226,
      -1,    92,    -1,    92,   224,   226,    -1,    93,    -1,    93,
     224,   226,    -1,    94,    -1,    94,   224,   226,    -1,   101,
      -1,   101,   224,   226,    -1,   102,    -1,   102,   224,   226,
      -1,   122,    -1,   122,   224,   226,    -1,   122,   224,   326,
     226,    -1,     5,    -1,     5,   224,   226,    -1,   127,    -1,
     127,   224,   226,    -1,   128,    -1,   128,   224,   226,    -1,
     129,    -1,   129,   224,   226,    -1,   131,    -1,   131,   224,
     226,    -1,   132,    -1,   132,   224,   226,    -1,   133,    -1,
     133,   224,   226,    -1,   135,    -1,   135,   224,   226,    -1,
     136,    -1,   136,   224,   226,    -1,   137,    -1,   137,   224,
     226,    -1,   138,    -1,   138,   224,   226,    -1,   139,    -1,
     139,   224,   226,    -1,   140,    -1,   140,   224,   226,    -1,
     141,    -1,   141,   224,   226,    -1,   142,    -1,   142,   224,
     226,    -1,   143,    -1,   143,   224,   226,    -1,   144,    -1,
     144,   224,   226,    -1,   145,    -1,   145,   224,   226,    -1,
     146,    -1,   146,   224,   226,    -1,   147,    -1,   147,   224,
     226,    -1,   148,    -1,   148,   224,   226,    -1,   149,    -1,
     149,   224,   226,    -1,   150,    -1,   150,   224,   226,    -1,
     151,    -1,   151,   224,   226,    -1,   152,    -1,   152,   224,
     226,    -1,   153,    -1,   153,   224,   226,    -1,     6,   224,
     326,   225,   326,   226,    -1,     7,   224,   326,   225,   326,
     225,   326,   226,    -1,   134,    -1,   134,   224,   226,    -1,
     113,   224,   326,   225,   326,   226,    -1,   114,   224,   326,
     226,    -1,   115,   224,   326,   226,    -1,   116,   224,   326,
     226,    -1,   117,   224,   326,   226,    -1,   118,   224,   326,
     226,    -1,   163,   224,   226,    -1,   164,   224,   326,   226,
      -1,   165,   224,   326,   226,    -1,   169,    -1,   169,   224,
     226,    -1,   171,    -1,   171,   224,   226,    -1,   178,    -1,
     178,   224,   226,    -1,   178,   224,   326,   226,    -1,   184,
     224,   326,   226,    -1,   191,   224,   328,   225,   328,   226,
      -1,   191,   224,   328,   225,   328,   225,   326,   226,    -1,
     194,   224,   328,   225,   328,   226,    -1,   197,    -1,   197,
     224,   226,    -1,   198,    -1,   198,   224,   226,    -1,    97,
     224,   328,   226,    -1,   328,    -1,   328,   225,   327,    -1,
     224,   328,   226,    -1,   328,   217,   328,    -1,   326,   217,
     328,    -1,   328,   217,   326,    -1,   207,    -1,   209,   227,
     326,   228,    -1,   209,   227,   326,   225,   326,   228,    -1,
     209,    -1,    82,   224,   326,   226,    -1,    49,   224,   326,
     226,    -1,    54,   224,   328,   226,    -1,    55,   224,   328,
     226,    -1,    51,   224,   328,   225,   326,   225,   326,   226,
      -1,    52,   224,   328,   225,   326,   226,    -1,    53,   224,
     328,   225,   326,   226,    -1,   103,   224,   326,   225,   326,
     225,   326,   225,   326,   226,    -1,    32,    -1,    32,   224,
     226,    -1,    32,   224,   326,   226,    -1,    84,    -1,    84,
     224,   226,    -1,    84,   224,   326,   226,    -1,   155,    -1,
     155,   224,   226,    -1,   166,   224,   326,   226,    -1,   170,
      -1,   170,   224,   226,    -1,   172,    -1,   172,   224,   226,
      -1,   175,    -1,   175,   224,   226,    -1,   175,   224,   326,
     226,    -1,   179,    -1,   179,   224,   226,    -1,   181,   224,
     328,   226,    -1,   183,   224,   328,   225,   328,   226,    -1,
     156,   224,   328,   226,    -1,   156,   224,   226,    -1,   190,
     224,   328,   225,   328,   225,   328,   226,    -1,   190,   224,
     328,   225,   328,   225,   328,   225,   326,   226,    -1,   193,
     224,   328,   225,   328,   225,   328,   226,    -1,   196,   224,
     209,   226,    -1,   196,   224,   209,   225,   328,   226,    -1,
     196,   224,   208,   226,    -1,   196,   224,   208,   225,   328,
     226,    -1
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
     839,   840,   843,   846,   849,   853,   856,   859,   860,   863,
     864,   865,   866,   867,   868,   869,   870,   871,   872,   873,
     874,   875,   876,   877,   878,   879,   880,   881,   882,   883,
     884,   885,   886,   887,   888,   889,   890,   891,   892,   893,
     894,   895,   896,   897,   898,   899,   900,   901,   909,   910,
     911,   912,   913,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   923,   924,   925,   926,   927,   928,   929,   930,
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
    1041,  1042,  1043,  1044,  1045,  1046,  1051,  1052,  1055,  1056,
    1057,  1058,  1059,  1060,  1061,  1062,  1070,  1071,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,
    1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,
    1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106
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
  "B256SECOND", "B256TEXT", "B256FONT", "B256TEXTWIDTH", "B256SAY",
  "B256SYSTEM", "B256VOLUME", "B256GRAPHWIDTH", "B256GRAPHHEIGHT",
  "B256GETSLICE", "B256PUTSLICE", "B256IMGLOAD", "B256SPRITEDIM",
  "B256SPRITELOAD", "B256SPRITESLICE", "B256SPRITEMOVE", "B256SPRITEHIDE",
  "B256SPRITESHOW", "B256SPRITEPLACE", "B256SPRITECOLLIDE", "B256SPRITEX",
  "B256SPRITEY", "B256SPRITEH", "B256SPRITEW", "B256SPRITEV",
  "B256WAVPLAY", "B256WAVSTOP", "B256WAVWAIT", "B256SIZE", "B256SEEK",
  "B256EXISTS", "B256BOOLTRUE", "B256BOOLFALSE", "B256MOUSEX",
  "B256MOUSEY", "B256MOUSEB", "B256CLICKCLEAR", "B256CLICKX", "B256CLICKY",
  "B256CLICKB", "B256GETCOLOR", "B256CLEAR", "B256BLACK", "B256WHITE",
  "B256RED", "B256DARKRED", "B256GREEN", "B256DARKGREEN", "B256BLUE",
  "B256DARKBLUE", "B256CYAN", "B256DARKCYAN", "B256PURPLE",
  "B256DARKPURPLE", "B256YELLOW", "B256DARKYELLOW", "B256ORANGE",
  "B256DARKORANGE", "B256GREY", "B256DARKGREY", "B256CHANGEDIR",
  "B256CURRENTDIR", "B256DIR", "B256DECIMAL", "B256DBOPEN", "B256DBCLOSE",
  "B256DBEXECUTE", "B256DBOPENSET", "B256DBCLOSESET", "B256DBROW",
  "B256DBINT", "B256DBFLOAT", "B256DBSTRING", "B256ONERROR",
  "B256OFFERROR", "B256LASTERROR", "B256LASTERRORMESSAGE",
  "B256LASTERRORLINE", "B256LASTERROREXTRA", "B256NETLISTEN",
  "B256NETCONNECT", "B256NETREAD", "B256NETWRITE", "B256NETCLOSE",
  "B256NETDATA", "B256NETADDRESS", "B256KILL", "B256MD5", "B256SETSETTING",
  "B256GETSETTING", "B256PORTIN", "B256PORTOUT", "B256BINARYOR",
  "B256BINARYAND", "B256BINARYNOT", "B256IMGSAVE", "B256REPLACE",
  "B256COUNT", "B256EXPLODE", "B256REPLACEX", "B256COUNTX", "B256EXPLODEX",
  "B256IMPLODE", "B256OSTYPE", "B256MSEC", "B256EDITVISIBLE",
  "B256GRAPHVISIBLE", "B256OUTPUTVISIBLE", "B256EDITSIZE",
  "B256OUTPUTSIZE", "B256LINENUM", "B256INTEGER", "B256FLOAT",
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
  "killstmt", "setsettingstmt", "portoutstmt", "imgsavestmt",
  "editvisiblestmt", "graphvisiblestmt", "outputvisiblestmt",
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
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,    60,    62,    61,    45,    43,    42,    47,
     468,    94,    10,    58,    40,    44,    41,    91,    93,    59,
     123,   125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   233,   234,   234,   235,   235,   236,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   238,   239,   240,
     241,   241,   242,   243,   244,   244,   245,   246,   247,   248,
     248,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     249,   249,   250,   250,   250,   250,   251,   251,   251,   251,
     252,   253,   253,   254,   255,   255,   256,   257,   258,   259,
     259,   259,   259,   259,   259,   260,   260,   260,   260,   260,
     260,   261,   262,   263,   263,   264,   265,   266,   267,   268,
     269,   270,   270,   270,   271,   271,   271,   271,   271,   272,
     272,   273,   273,   274,   274,   275,   275,   276,   276,   276,
     276,   277,   277,   278,   278,   279,   280,   281,   281,   281,
     282,   282,   282,   282,   282,   282,   282,   282,   282,   283,
     283,   283,   284,   284,   284,   285,   285,   285,   286,   286,
     286,   287,   287,   287,   288,   288,   288,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   289,   290,   291,   291,
     291,   291,   291,   292,   293,   293,   294,   294,   295,   295,
     295,   295,   296,   296,   296,   296,   296,   296,   297,   298,
     298,   299,   299,   300,   300,   301,   301,   302,   303,   304,
     304,   305,   306,   307,   308,   308,   309,   310,   311,   311,
     312,   312,   312,   313,   313,   313,   313,   314,   314,   314,
     315,   315,   315,   316,   316,   317,   317,   318,   318,   319,
     319,   319,   320,   321,   322,   323,   324,   325,   325,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   327,   327,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328
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
       6,     1,     3,     1,     3,     4,     1,     3,     3,     3,
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
       0,   474,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   344,     0,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   477,   352,   356,   358,   360,   362,   364,
     366,     0,   368,   370,     0,     0,     0,     0,     0,     0,
       0,   372,     0,   348,   350,   377,   379,   381,   383,   385,
     387,   429,   389,   391,   393,   395,   397,   399,   401,   403,
     405,   407,   409,   411,   413,   415,   417,   419,   421,   423,
     425,   480,     0,     0,     0,     0,     0,   440,   483,   442,
     485,   487,   444,   490,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   451,   453,   308,   307,   462,   317,   465,
       0,     0,   210,   209,   317,   465,     0,   207,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,   169,     0,
       0,     0,     0,     0,     0,   118,    23,    28,     0,   135,
       0,     0,   179,     0,     0,   182,     0,   190,     0,   193,
     136,   137,    21,    25,     0,   143,     0,     0,     0,     0,
     110,   317,     0,   146,     0,     0,     0,   185,     0,     0,
       0,     0,   164,   163,   165,   166,     0,     0,     0,     0,
     228,     0,     0,     0,     0,     0,   237,   238,     0,     0,
     213,     0,     0,     0,   194,     0,   241,   242,   243,     0,
     246,   247,     0,   139,     0,   250,     0,     0,     0,     0,
       0,   257,     0,   262,     0,   263,     0,     0,     0,     0,
       0,   269,   272,   273,   274,     0,     0,     0,     0,     1,
       2,     4,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   289,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   290,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   212,     0,     0,     0,     0,
       0,     0,     0,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   192,     0,     0,
       0,   102,     0,   103,     0,   106,     0,   107,   317,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   217,
       0,     0,   240,   245,   249,     0,     0,     0,     0,     0,
       0,     0,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   127,   131,     0,     0,     0,     0,   121,
     132,     0,     3,   117,    30,    17,   199,   197,   376,     0,
       0,   475,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   345,     0,     0,     0,
       0,     0,     0,     0,   347,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   478,     0,   353,     0,   357,
     359,   361,   363,   365,   367,     0,   369,   371,     0,     0,
       0,     0,     0,     0,     0,   373,     0,     0,   349,   351,
     378,   380,   382,   384,   386,   388,   430,   390,   392,   394,
     396,   398,   400,   402,   404,   406,   408,   410,   412,   414,
     416,   418,   420,   422,   424,   426,   481,   495,     0,   437,
       0,     0,     0,   441,   484,   443,   486,   488,     0,   445,
       0,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   452,   454,     0,     0,     0,     0,     0,     0,   279,
     458,   305,   306,   302,   291,   292,   293,   283,   284,   287,
     288,   303,   304,   301,   281,   280,   460,   282,   285,   286,
     299,   300,   296,   297,   298,   295,   461,   459,     0,     0,
       0,   149,     0,     0,     0,     0,   168,   276,     0,     0,
       0,     0,     0,     0,   114,     0,     0,   181,     0,   184,
       0,     0,     0,     0,     0,     0,   144,     0,   148,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     229,     0,     0,     0,     0,     0,     0,   196,     0,   252,
       0,     0,     0,   253,     0,   259,     0,     0,     0,     0,
     267,     0,   271,     0,     0,     0,     0,     0,     0,     0,
     456,     0,     0,     0,     0,     0,     0,   476,   318,   319,
     467,   322,     0,     0,     0,   468,   469,     0,     0,   329,
     330,   331,   332,   333,   334,   335,   336,   343,   337,   338,
     339,   340,   341,   342,   323,   466,   320,   321,   479,   354,
     455,     0,     0,   432,   433,   434,   435,   436,   374,   355,
     494,   438,   439,   482,   489,   446,   492,     0,   447,     0,
       0,     0,     0,     0,   501,     0,   499,     0,     0,   309,
       0,   315,     0,     0,   310,     0,   463,     0,   315,     0,
     463,     0,     0,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   313,   311,     0,
     314,   312,     0,     0,     0,   150,     0,   153,     0,     0,
       0,   317,   175,     0,     0,     0,   115,   133,   180,   183,
       0,   141,     0,     0,     0,     0,   147,   186,     0,   159,
     157,     0,   161,     0,   218,     0,     0,   222,     0,   230,
       0,     0,   235,     0,   233,   195,   251,     0,   254,   255,
     258,   264,     0,   265,   268,   270,     0,     0,     0,   125,
       0,     0,   457,     0,   119,   200,   198,   427,     0,     0,
     471,   472,     0,   324,     0,   327,     0,   431,   493,     0,
       0,   448,     0,   450,   502,   500,   316,   464,   316,   464,
       0,     0,     0,   317,     0,     0,     0,     0,     0,     0,
     104,   105,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   154,     0,   155,   174,     0,   317,   172,     0,
       0,   151,   134,   142,   160,   158,   162,     0,   219,   220,
       0,   223,     0,   224,   236,     0,     0,   234,   256,   266,
       0,   128,   130,   126,     0,   122,   124,   120,   428,   470,
       0,   325,   328,     0,     0,   496,   449,   498,     0,   317,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   156,   171,     0,   176,   178,   152,
     221,     0,   225,   226,     0,   231,   129,   123,   326,   473,
     497,     0,     0,     0,   177,   227,   232
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
     629,   298,   553,   286,   859,   290
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -406
static const yytype_int16 yypact[] =
{
   11607,  4695,  4875,  5055,  5235,  5415,  -203,  5595,  5775,  -406,
    5955,  -406,  -406,  -406,  4695,  -406,  -406,  4695,  -406,  -406,
    4695,  -179,  -177,  6135,  6315,  6495,  6675,  -169,  -156,  -406,
       9,  6855,  -191,  -149,  4695,  2313,  7035,  7215,  7395,  4695,
    4695,  4695,  7575,  7755,  4695,  7935,  8115,  4695,  4695,  4695,
    8295,  4695,  -170,  -163,  8475,  -161,  4695,  4695,  4695,  -155,
    4695,  4695,  -145,  -127,  -406,  8655,  8835,  9015,  9195,  9375,
    9555,  9735,  9915,  4695,  4695,  4695,  -207,  -201,  -406,    89,
    -108, 11814,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -130,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,   101,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -103,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -101,  -100,
     -72,   -71,   -70,   -68,   -67,   -62,   -60,   -59,   -52,   -51,
     -49,   -48,   -47,   -40,   -38,   -37,   -36,   -35,   -28,   -24,
     -17,     7,    14,    20,    21,    30,    33,    35,    38,  4695,
      39,    40,    52,    53,    54,    57,    64,    65,    67,    70,
      71,    73,    75,    82,    84,    86,    87,    90,    92,    94,
      95,    96,    97,    98,   100,   105,   106,   108,   109,   111,
     112,   114,   116,   117,   118,   119,   120,   122,   124,   125,
     127,   129,   130,   131,   132,   134,   135,   136,   142,   143,
     144,   145,   146,   147,   148,   150,   152,   153,   155,   156,
     158,   160,   162,   164,   169,   172,   174,  4695,   175,   176,
     177,   178,   179,   180,   186,  -406,  -406,  -406,   184,   185,
    4695,  4695,  1732,   -32,  -172,  -165, 18739,   401,  4695, 16078,
     401,  4695, 16239,  4695, 16258,  -406,   205,  4695,  -406,  4695,
   16275,  4695, 16294,  4695, 16311, 18739, 18739, 18739,   199,  -406,
    4695, 16330,   401,  4695, 16347,   401,  2895, 18739,  3075, 18739,
    -406,  -406,  -406,  -406,  4695, 16498, 10095, 10275, 10455, 10635,
   18739,  -147, 10815,  -406, 16514,  4695, 16534,   401,  4695, 16550,
    4695,   807, 18739,   401,   401, 18739,  4695, 16570,  4695, 16586,
   18739,  4695, 16606,  4695, 16622, 16773, 18739, 18739,  4695, 16790,
     401,   189,   190,  4695, 16809,   191,   401, 18739,   401,   194,
     401,   401,   202,  -406,  4695, 16826,  4695, 16845,   943,  4695,
   16862,   401,  3255, 18739,  4695,   401,  4695,  1129,  4695, 16881,
    4695, 11292, 18739, 18739, 18739,    77,  4695,  2119,  4695,  -406,
   11392,  -406, 12016, 12016,  -131,   209,  4695,  4695,  3435,  4695,
    4695,  4695,  4695,  4695,  4695,  4695,  4695,  4695,  4695,  4695,
    4695,   210,  4695,  4695,  4695,  4695,  4695,  4695,  4695,   215,
    4695,  4695,  4695,  4695,  4695,  4695,   221,  4695,  4695,  4695,
    3615,  3795,   220,   222,   223,   232,   233,   234,  4695,   235,
     236,  4695,  4695,  4695,  4695,  4695,  4695,  4695,  3975,  4695,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   251,   253,   254,   255,   256,   257,   258,   259,   262,
     263,   267,   268,   269,   270,   273,   274,   275,   277,  4155,
     278,  4695,  4695,  4695,   279,   280,   284,   287,  4335,  4515,
     288,  4695,  4695,  4695,   221,  4695,  4695,  4695,  4695,  -113,
     289,   311,   571,  1533,   221, 13378,   321,  4695,  4695,  4695,
    4695,  4695,  4695,  4695,  4695,  4695,  4695,  4695,  4695,  4695,
    4695,  4695,  4695,  4695,  4695,  -406,  4695,  4695,  4695,  4695,
    4695,  4695,  4695,  -406,   571,  1533, 12323,  4695, 12382,  4695,
   12402,  4695,   313,   216, 17042, 12470,  4695, 12487,  4695, 12537,
    4695,  4695, 12562,  4695, 12622,  4695,  -406,  -406, 12684,  4695,
    4695, 18739,  4695, 18739,  4695, 18739,  4695, 18739,  -129, 12725,
    4695, 12742,  4695, 12778,  4695,   261,  4695, 12802,  4695, 12819,
    4695, 12838,  4695, 12999,  4695,  4695, 13018,  4695,  -406,  -406,
   13035,  4695,  -406,  -406,  -406, 13054,  4695, 13071,   465,  4695,
    4695, 13090,  4695,  -406, 13107,   838,  4695, 13270,  4695,   854,
    4695,   316,   318,  -406, 18739,  1750,   325,   327,  4695,  -406,
     401,  1768,  -406,  -406,  -406,  -130,   326,   329,  -406, 17061,
   17078,  -406, 13539, 13558,   348, 13575,   568, 11318, 11441, 11476,
     689,   775, 11545, 11734, 13594, 13611,  -406, 13630, 13647, 13810,
   13827, 13846, 13863, 13882,  -406, 13899, 13918, 14079, 14098, 14115,
   14134,  1227, 14151, 14170,  1308,  -406, 14187,  -406, 14350,  -406,
    -406,  -406,  -406,  -406,  -406,  1353,  -406,  -406, 17097, 17114,
   14367, 14386, 14403, 14422, 14439,  -406, 14458,  1519,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  1718,  -406,
   14619, 14638, 14655,  -406,  -406,  -406,  -406,  -406, 14674,  -406,
   14691,  -406,  1791, 11862, 14710, 11941, 12089, 12283, 12429,  -126,
    -124,  -406,  -406,    81,  -157,  1816,   320,  -134,  1840,  -406,
    -406,   669,   669,   669, 18807, 18904, 18756,   221,   221,    18,
      18,   669,   669,   669,    32,    32,  -406,   221,   221,  -406,
     328,   328,   328,   328,   328,   328,    32,  -406, 11656, 11853,
    4695, 18739,  4695, 17133,  4695, 17150,  -406,  -406,  4695,  4695,
   17301,  4695, 17317,  4695, 18739,   998,  4695,   401,  4695,   401,
    4695, 17337, 13287, 13306, 13323, 13342,  -406,  4695, 18739,  4695,
     401,  4695, 17353,  4695, 17373,  4695, 17389,  4695, 17409,  4695,
     401,  4695, 17425, 17576,  4695, 17593,  4695, 18739,  4695, 18739,
    4695,  4695, 12438, 18739,  4695,   401,  4695,  4695, 12517,  4695,
   18739,  4695,   401,  4695,  4695,  4695,   339,  4695,  4695,   333,
   12590,  4695,   343,  4695,  4695,  4695,  4695,  -406,  -406,  -406,
    -406,  -406,  4695,  4695,  4695,  -406,  -406,  4695,  4695,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  4695,  4695,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  4695,  -406,  4695,
    4695,  4695,  4695,  4695,  -406,  4695,  -406,   338,   342,  -406,
    4695,  -406,   344,   352,  -406,  4695,  -406,  4695,  -119,  4695,
    -115, 14727, 17612,  4695, 17629,  4695,  -406, 17648,  2507, 17665,
    4695, 14890,  4695,  1797,  1834, 17684,  4695,  4695,  4695,  4695,
    4695, 14907,  1856, 17845,  4695, 17864,  4695, 17881,  4695, 17900,
    4695, 17917,  1863, 17936,  4695,  4695, 17953,  4695, 14926, 14943,
   12875, 14962,  4695,  1878,  1914, 12939,  4695, 14979,  1930, 13126,
   13155, 12042,  4695, 13395, 13666,  -406,  4695, 12065,  4695, 12143,
   12160, 14998, 18104, 18120, 15159, 15178,   883,   928, 18140, 15195,
    2116, 13695,  1060, 13935,  2175,  2310,  2369,  -406,  -406, 12200,
    -406,  -406, 12222, 12247, 12307,  -406,  4695, 18739,  4695, 18156,
   10995,  -137,  -406, 18176,  4695, 18192,  -406,  1152,  -406,  -406,
    4695, 18739, 15214, 15231, 15250, 15267,  -406,  -406,  4695, 18739,
     401,  4695, 18739,  4695, 14206,  4695, 18212,   401,  4695,  -406,
    4695, 18228, 18739,  4695, 18739,  -406,  -406,  4695,  -406, 18739,
    -406,  -406,  4695,   401,  -406,  -406,  4695,  4695,   366, 18739,
    4695,  4695,  -406,   367,   401,  -406,  -406,  -406,  4695,  4695,
    -406,  -406,  4695,  -406,  4695,  -406,  4695,  -406,  -406,  4695,
    4695,  -406,  4695,  -406,  -406,  -406,  -406,  -406,  -112,  -110,
   15430, 18379,  4695,  -111, 18396,  2701, 18415,  4695,  4695, 15447,
    -406,  -406,  -406,  -406, 15466,  2504, 15483,  1161,  4695, 18432,
    2563,  4695, 15502, 18451,  4695, 15519, 15538,  2698,  1206,  2757,
    4695,  1222, 11172,  4695, 15699, 15718, 13359, 15735, 18468,  1245,
   15754, 11231,  -406,  4695, 18739,  -406, 11175,  -135,  -406, 18487,
    4695, 18739, 18739,  -406,  -406,  -406,  -406,  4695,  -406, 18739,
    4695,  -406, 18648,   401,  -406,  4695, 18667,  -406,  -406,  -406,
    4695,  -406,  -406, 18739,  4695,  -406,  -406,   401,  -406,  -406,
    4695,  -406,  -406,  4695,  4695,  -406,  -406,  -406, 15771,  -109,
   18684,  -193, 15790, 15807, 18703, 11237,  4695, 18720,  4695, 15970,
   15987, 16006, 16023, 16042,  -406,  -406,   359,  -406,  -406,  -406,
    -406,  4695,  -406,   401,  4695, 18739,  -406,  -406,  -406,  -406,
    -406,   358, 11252, 16059,  -406,  -406,  -406
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -406,   188,  -406,  -406,   505,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,   187,   193,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,  -406,
    -406,   -34,  -206,    -1,  -405,   944
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -207
static const yytype_int16 yytable[] =
{
     282,   333,   289,   292,   294,   295,   300,   302,   395,   304,
     536,   537,   538,   305,   397,  1217,   306,   326,   327,   307,
     396,   296,   311,   314,   317,   319,   398,   297,   322,   308,
     325,   309,   323,   330,   334,   336,   339,   297,   342,   320,
     345,   347,   349,   350,   352,   354,   355,   356,   357,   359,
    -204,  -204,   321,   364,   361,   544,   367,  -201,  -201,   328,
     329,   362,   545,   365,   375,   377,   380,   383,   928,   369,
     389,   929,   392,   393,   394,  -145,  -145,   636,   637,   372,
     512,   373,   168,   169,   170,  -173,  -173,  -170,  -170,   399,
     512,   933,   512,   402,   934,   749,   750,   816,   512,   923,
     924,   925,   926,  -205,  -205,   523,   524,  -202,  -202,   171,
    -206,  -206,  -203,  -203,   400,  1155,   512,  1215,   512,   523,
     524,   403,   404,   405,   406,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   407,   408,   409,   199,   410,   411,   200,   201,
     202,   203,   412,   204,   413,   414,   205,   206,   207,   208,
     209,   210,   415,   416,   211,   417,   418,   419,   212,   213,
     214,   539,   540,   541,   420,   542,   421,   422,   423,   424,
     215,   216,   217,   218,   219,   220,   425,   543,   436,   221,
     426,   222,   223,   224,   225,   226,   227,   427,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   428,   251,   252,   530,   531,   532,   533,   429,   534,
     253,   254,   255,   256,   430,   431,   257,   258,   259,   260,
     532,   533,   261,   534,   432,   262,   263,   433,   264,   434,
     265,   266,   435,   437,   438,   267,   504,   268,   269,   621,
     270,   271,   622,   272,   273,   274,   439,   440,   441,   514,
     515,   442,   275,   276,   277,   278,   279,   546,   443,   444,
     548,   445,   550,   280,   446,   447,   554,   448,   555,   449,
     557,   281,   559,   536,   537,   538,   450,   297,   451,   562,
     452,   453,   564,   927,   454,   515,   455,   515,   456,   457,
     458,   459,   460,   568,   461,   571,   573,   575,   577,   462,
     463,   579,   464,   465,   581,   466,   467,   583,   468,   515,
     469,   470,   471,   472,   473,   587,   474,   589,   475,   476,
     591,   477,   593,   478,   479,   480,   481,   596,   482,   483,
     484,   623,   600,   536,   537,   538,   485,   486,   487,   488,
     489,   490,   491,   605,   492,   607,   493,   494,   611,   495,
     496,   515,   497,   614,   498,   515,   499,   617,   500,   515,
     536,   537,   538,   501,   624,   625,   502,   631,   503,   505,
     506,   507,   508,   509,   510,   639,   640,   642,   643,   645,
     511,   512,   513,   552,   561,   598,   599,   602,   654,   655,
     603,   657,   658,   659,   660,   661,   662,   663,   604,   665,
     666,   667,   668,   669,   670,   638,   656,   672,   673,   676,
     678,   664,   534,   536,   537,   538,   679,   797,   680,   681,
     688,   689,   690,   691,   692,   693,   694,   696,   682,   683,
     684,   686,   687,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   539,   540,   541,   709,   542,   710,
     711,   712,   713,   714,   715,   716,   823,   760,   717,   718,
     730,   731,   732,   719,   720,   721,   722,   738,   740,   723,
     724,   725,   744,   726,   729,   733,   734,   536,   537,   538,
     735,   755,   758,   736,   741,   751,   761,   762,   763,   764,
     765,   766,   767,   768,   769,   770,   771,   772,   773,   774,
     775,   777,   778,   779,   539,   540,   541,   752,   542,   796,
     853,   786,   854,   788,   789,   542,   791,   760,   793,   857,
     795,   858,   932,   863,   992,   800,   864,   802,   998,   804,
     805,   539,   540,   541,   995,   542,  1017,  1231,   811,   812,
    1018,   813,  1020,   814,   869,   815,   168,   169,   170,   818,
    1021,  1140,  1143,   822,  1234,   824,   401,   826,   632,   828,
     635,  1082,   946,   832,   833,   634,   835,     0,     0,     0,
     837,     0,     0,   171,     0,   839,     0,     0,     0,   843,
     536,   537,   538,     0,   539,   540,   541,   850,   542,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,     0,     0,     0,   199,
       0,     0,   200,   201,   202,   203,     0,   204,     0,     0,
     205,   206,   207,   208,   209,   210,     0,     0,   211,     0,
       0,     0,   212,   213,   214,     0,     0,     0,   539,   540,
     541,     0,   542,     0,   215,   216,   217,   218,   219,   220,
     841,   760,     0,   221,     0,   222,   223,   224,   225,   226,
     227,     0,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,     0,     0,
       0,   536,   537,   538,   253,   254,   255,   256,     0,     0,
     257,   258,   259,   260,     0,     0,   261,     0,     0,   262,
     263,     0,   264,     0,   265,   266,   523,   524,     0,   267,
       0,   268,   269,     0,   270,   271,     0,   272,   273,   274,
       0,     0,     0,     0,     0,     0,   275,   276,   277,   278,
     279,   539,   540,   541,     0,   542,     0,   280,     0,   941,
       0,   942,     0,   944,   871,   281,   753,   554,   947,     0,
     949,     0,   951,   754,     0,     0,     0,     0,     0,   955,
       0,     0,     0,     0,     0,     0,   961,   536,   537,   538,
     963,     0,   965,     0,   967,     0,   969,     0,   971,     0,
     973,     0,     0,   976,     0,   978,     0,   979,     0,     0,
     981,     0,     0,     0,     0,     0,     0,     0,   987,   536,
     537,   538,     0,     0,   991,   525,   526,     0,     0,     0,
     997,     0,   999,  1000,  1001,  1002,     0,     0,     0,     0,
       0,  1003,  1004,  1005,     0,     0,     0,     0,     0,     0,
     536,   537,   538,     0,     0,   530,   531,   532,   533,     0,
     534,     0,     0,     0,     0,     0,   536,   537,   538,     0,
    1008,  1009,   539,   540,   541,     0,   542,     0,     0,     0,
       0,     0,     0,     0,  1032,   875,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   536,   537,   538,     0,  1019,
       0,     0,     0,     0,  1022,     0,  1023,     0,  1024,     0,
       0,     0,  1027,     0,  1029,   283,   287,  1033,     0,  1035,
       0,  1037,     0,     0,     0,  1041,  1042,  1043,  1044,  1045,
       0,     0,     0,  1049,     0,  1052,     0,   312,   315,  1056,
     536,   537,   538,  1061,  1062,     0,  1064,     0,     0,     0,
     337,  1069,   341,   343,   344,   536,   537,   538,   539,   540,
     541,  1079,   542,     0,     0,   360,     0,     0,     0,     0,
     366,   876,   368,     0,   370,   371,     0,     0,     0,     0,
     378,   381,     0,   385,   387,     0,   391,     0,     0,     0,
     539,   540,   541,     0,   542,  1110,   952,  1111,     0,  1114,
       0,     0,   586,  1116,     0,     0,     0,     0,     0,  1119,
     517,   518,   519,     0,     0,     0,     0,  1124,     0,     0,
    1126,   539,   540,   541,  1129,   542,     0,  1132,     0,  1133,
       0,     0,  1135,   847,   760,     0,  1136,   539,   540,   541,
       0,   542,     0,   520,   521,   522,     0,     0,     0,   851,
     760,  1158,     0,     0,     0,   523,   524,  1144,  1145,     0,
       0,  1146,     0,  1147,     0,  1148,   539,   540,   541,  1150,
     542,     0,   536,   537,   538,     0,     0,     0,  1092,  1093,
       0,  1154,     0,     0,  1159,     0,  1161,  1162,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1169,     0,     0,
    1172,     0,     0,  1176,     0,     0,     0,     0,     0,  1183,
       0,   539,   540,   541,     0,   542,     0,     0,     0,     0,
       0,     0,  1198,  1094,  1095,  1200,   539,   540,   541,  1202,
     542,     0,     0,     0,     0,     0,  1203,  1218,   610,  1204,
       0,   536,   537,   538,  1207,     0,     0,     0,     0,  1209,
       0,  1118,     0,  1210,   525,   526,     0,     0,     0,  1211,
       0,     0,  1212,  1213,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   536,   537,   538,     0,  1225,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
       0,     0,     0,  1233,     0,   516,     0,   520,   521,   522,
       0,     0,   516,     0,     0,   516,     0,   516,     0,   523,
     524,     0,     0,   516,     0,   516,     0,   516,   536,   537,
     538,     0,     0,     0,   516,     0,     0,   516,     0,     0,
     516,     0,   516,     0,   536,   537,   538,     0,   516,   536,
     537,   538,     0,   539,   540,   541,   516,   542,     0,   516,
       0,     0,   516,     0,   585,  1100,  1101,   536,   537,   538,
     516,     0,   516,     0,     0,   516,     0,   516,     0,     0,
       0,     0,   516,     0,     0,     0,     0,   516,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
     608,     0,     0,   516,     0,     0,   516,     0,   516,     0,
     615,     0,   516,     0,   619,     0,     0,     0,   525,   526,
       0,   630,   539,   540,   541,     0,   542,     0,     0,     0,
     536,   537,   538,   644,   616,   646,   647,   648,   649,   650,
     651,   652,   653,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   539,   540,   541,     0,   542,     0,
       0,   671,     0,   674,     0,     0,  1167,  1168,     0,     0,
       0,     0,   685,     0,     0,   536,   537,   538,     0,     0,
       0,     0,     0,   697,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   539,
     540,   541,     0,   542,     0,     0,     0,     0,     0,     0,
       0,  1180,  1181,   728,     0,   539,   540,   541,     0,   542,
     539,   540,   541,     0,   542,   742,   743,  1184,  1185,   745,
     746,   747,   748,   894,     0,     0,     0,     0,   539,   540,
     541,     0,   542,     0,     0,     0,     0,     0,     0,     0,
    1194,  1195,     0,     0,     0,   776,     0,     0,     0,     0,
     780,   781,   782,   783,   784,   785,   787,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   807,     0,   809,
       0,     0,     0,     0,   516,     0,   516,     0,   516,     0,
     516,   539,   540,   541,     0,   542,   820,     0,     0,     0,
       0,     0,     0,     0,   897,     0,   830,     0,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   842,     0,     0,   845,     0,     0,     0,
     848,   536,   537,   538,   852,   171,   539,   540,   541,     0,
     542,     0,   860,     0,     0,     0,     0,     0,     0,   900,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     0,     0,
       0,   199,     0,     0,   200,   201,   202,   203,     0,   204,
       0,     0,   205,   206,   207,   208,   209,   210,     0,     0,
     211,     0,     0,     0,   212,   213,   214,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   215,   216,   217,   218,
     219,   220,     0,     0,     0,   221,     0,   222,   223,   224,
     225,   226,   227,     0,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
       0,     0,     0,     0,     0,     0,   253,   254,   255,   256,
       0,     0,   257,   258,   259,   260,     0,     0,   261,     0,
       0,   262,   263,     0,   264,     0,   265,   266,     0,     0,
       0,   267,     0,   268,   269,     0,   270,   271,     0,   272,
     273,   274,   539,   540,   541,     0,   542,     0,   275,   276,
     277,   278,   279,     0,     0,   909,     0,     0,     0,   280,
     953,     0,   954,     0,     0,     0,     0,   281,   756,     0,
     536,   537,   538,   962,     0,   757,     0,     0,     0,     0,
       0,     0,     0,   972,   517,   518,   519,     0,     0,     0,
       0,     0,     0,     0,   980,     0,     0,     0,   983,     0,
     984,   985,   517,   518,   519,   988,     0,   989,   990,     0,
       0,   993,   994,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,  1006,  1007,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,   536,   537,   538,     0,   523,   524,   536,
     537,   538,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,   517,   518,
     519,  1010,     0,  1011,  1012,  1013,  1014,  1015,     0,  1016,
       0,     0,     0,     0,     0,     0,   536,   537,   538,     0,
       0,     0,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   520,   521,   522,     0,     0,     0,     0,   536,   537,
     538,     0,     0,   523,   524,   536,   537,   538,  1050,     0,
       0,     0,  1054,     0,  1057,   520,   521,   522,   525,   526,
     536,   537,   538,     0,     0,     0,     0,   523,   524,     0,
    1073,   539,   540,   541,     0,   542,   525,   526,     0,     0,
     860,     0,  1084,     0,   910,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,   536,   537,   538,     0,
       0,   535,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   536,   537,   538,   855,     0,     0,   856,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
       0,     0,  1125,   861,     0,     0,   862,  1127,     0,  1130,
       0,     0,   525,   526,   539,   540,   541,     0,   542,     0,
     539,   540,   541,     0,   542,     0,  1137,   916,     0,     0,
    1138,  1139,     0,  1038,  1141,  1142,   525,   526,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,     0,     0,
       0,   930,     0,  1149,   931,     0,  1151,   539,   540,   541,
       0,   542,     0,   527,   528,   529,   530,   531,   532,   533,
    1039,   534,     0,     0,     0,   935,     0,     0,   936,   539,
     540,   541,     0,   542,     0,  1173,   539,   540,   541,     0,
     542,     0,  1047,     0,     0,     0,     0,  1187,     0,  1059,
       0,   539,   540,   541,     0,   542,     0,     0,     0,     0,
       0,     0,     0,     0,  1070,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1205,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   168,   169,   170,   539,   540,   541,
       0,   542,     0,     0,     0,     0,     0,     0,     0,     0,
    1071,     0,     0,   539,   540,   541,     0,   542,     0,     0,
    1223,   171,     0,     0,     0,     0,  1075,     0,   536,   537,
     538,     0,     0,     0,     0,  1232,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,     0,     0,     0,   199,     0,     0,
     200,   201,   202,   203,     0,   204,     0,     0,   205,   206,
     207,   208,   209,   210,     0,     0,   211,   536,   537,   538,
     212,   213,   214,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   215,   216,   217,   218,   219,   220,     0,     0,
       0,   221,     0,   222,   223,   224,   225,   226,   227,     0,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,     0,   251,   252,     0,     0,     0,     0,
       0,     0,   253,   254,   255,   256,     0,     0,   257,   258,
     259,   260,     0,     0,   261,     0,     0,   262,   263,     0,
     264,     0,   265,   266,     0,     0,     0,   267,     0,   268,
     269,   626,   270,   271,   627,   272,   273,   274,   168,   169,
     170,     0,     0,     0,   275,   276,   277,   278,   279,   539,
     540,   541,     0,   542,     0,   280,     0,     0,     0,     0,
       0,     0,  1098,   281,     0,   171,     0,     0,     0,   628,
       0,     0,   536,   537,   538,     0,     0,     0,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   539,   540,
     541,   199,   542,     0,   200,   201,   202,   203,     0,   204,
       0,  1103,   205,   206,   207,   208,   209,   210,     0,     0,
     211,   536,   537,   538,   212,   213,   214,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   215,   216,   217,   218,
     219,   220,     0,     0,     0,   221,     0,   222,   223,   224,
     225,   226,   227,     0,   228,   229,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,     0,   251,   252,
       0,     0,     0,     0,     0,     0,   253,   254,   255,   256,
       0,     0,   257,   258,   259,   260,     0,     0,   261,     0,
       0,   262,   263,     0,   264,     0,   265,   266,     0,     0,
       0,   267,     0,   268,   269,     0,   270,   271,     0,   272,
     273,   274,   168,   169,   170,     0,     0,     0,   275,   276,
     277,   331,   279,   539,   540,   541,     0,   542,     0,   280,
       0,     0,     0,     0,     0,     0,  1104,   332,     0,   171,
       0,     0,     0,   297,     0,     0,   536,   537,   538,     0,
       0,     0,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   539,   540,   541,   199,   542,     0,   200,   201,
     202,   203,     0,   204,     0,  1105,   205,   206,   207,   208,
     209,   210,     0,     0,   211,   536,   537,   538,   212,   213,
     214,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   216,   217,   218,   219,   220,     0,     0,     0,   221,
       0,   222,   223,   224,   225,   226,   227,     0,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,     0,   251,   252,     0,     0,     0,     0,     0,     0,
     253,   254,   255,   256,     0,     0,   257,   258,   259,   260,
       0,     0,   261,     0,     0,   262,   263,     0,   264,     0,
     265,   266,     0,     0,     0,   267,     0,   268,   269,     0,
     270,   271,     0,   272,   273,   274,   168,   169,   170,     0,
       0,     0,   275,   276,   277,  1031,   279,   539,   540,   541,
       0,   542,     0,   280,     0,     0,     0,     0,     0,     0,
    1165,   281,     0,   171,     0,     0,     0,   297,     0,     0,
     536,   537,   538,     0,     0,     0,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   539,   540,   541,   199,
     542,     0,   200,   201,   202,   203,     0,   204,     0,  1171,
     205,   206,   207,   208,   209,   210,     0,     0,   211,   536,
     537,   538,   212,   213,   214,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   215,   216,   217,   218,   219,   220,
       0,     0,     0,   221,     0,   222,   223,   224,   225,   226,
     227,     0,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,     0,   251,   252,     0,     0,
       0,     0,     0,     0,   253,   254,   255,   256,     0,     0,
     257,   258,   259,   260,     0,     0,   261,     0,     0,   262,
     263,     0,   264,     0,   265,   266,     0,     0,     0,   267,
       0,   268,   269,     0,   270,   271,     0,   272,   273,   274,
     168,   169,   170,     0,     0,     0,   275,   276,   277,  1157,
     279,   539,   540,   541,     0,   542,     0,   280,     0,     0,
       0,     0,     0,     0,  1179,   281,     0,   171,     0,     0,
       0,   297,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     539,   540,   541,   199,   542,     0,   200,   201,   202,   203,
       0,   204,     0,  1182,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   566,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   567,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   613,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   641,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   675,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   677,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   695,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   727,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   737,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,   739,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   284,   285,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   288,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   291,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   293,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   299,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   301,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   303,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   310,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   313,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   316,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   318,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   324,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   335,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   338,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   340,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   346,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   348,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   351,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   353,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   358,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   374,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   376,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   379,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   382,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   384,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   386,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   388,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   390,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   570,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   572,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   574,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   278,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   576,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,   578,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,     0,     0,     0,     0,     0,   281,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,     0,     0,     0,   212,   213,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,     0,     0,     0,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,     0,     0,     0,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     168,   169,   170,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,  1113,   279,     0,     0,   171,     0,     0,
       0,   280,     0,     0,   536,   537,   538,     0,     0,   281,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
       0,     0,     0,   199,     0,     0,   200,   201,   202,   203,
       0,   204,     0,     0,   205,   206,   207,   208,   209,   210,
       0,     0,   211,   536,   537,   538,   212,   213,   214,   536,
     537,   538,     0,     0,     0,     0,     0,     0,   215,   216,
     217,   218,   219,   220,   536,   537,   538,   221,     0,   222,
     223,   224,   225,   226,   227,     0,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,     0,
     251,   252,     0,     0,   536,   537,   538,     0,   253,   254,
     255,   256,     0,     0,   257,   258,   259,   260,     0,     0,
     261,     0,     0,   262,   263,     0,   264,     0,   265,   266,
     536,   537,   538,   267,     0,   268,   269,     0,   270,   271,
       0,   272,   273,   274,     0,     0,     0,     0,     0,     0,
     275,   276,   277,  1199,   279,   539,   540,   541,     0,   542,
       0,   280,     0,     0,     0,     1,     2,     0,  1186,   281,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,     0,    15,    16,    17,    18,    19,    20,    21,
       0,     0,    22,    23,     0,    24,    25,    26,    27,    28,
      29,     0,    30,    31,     0,     0,     0,    32,    33,     0,
       0,     0,     0,     0,   539,   540,   541,     0,   542,     0,
     539,   540,   541,     0,   542,     0,     0,  1197,     0,     0,
       0,     0,     0,  1222,     0,   539,   540,   541,     0,   542,
       0,    34,    35,     0,     0,     0,     0,    36,  1235,     0,
       0,     0,     0,   536,   537,   538,     0,    37,    38,     0,
      39,    40,    41,     0,     0,     0,    42,    43,    44,    45,
      46,    47,    48,    49,    50,   539,   540,   541,     0,   542,
       0,    51,    52,    53,     0,    54,     0,   620,   536,   537,
     538,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,   539,   540,   541,     0,   542,     0,     0,     0,     0,
       0,     0,     0,   872,     0,     0,    56,     0,     0,    57,
      58,    59,    60,    61,    62,     0,     0,     0,     0,    63,
      64,     0,     0,     0,     0,    65,    66,     0,    67,    68,
       0,     0,    69,     0,    70,     0,     0,    71,     0,     0,
       0,    72,     0,     0,     0,     0,     0,   536,   537,   538,
       0,    73,    74,    75,     0,     0,     0,     0,     0,     0,
      76,    77,     0,     0,    78,     0,     0,     0,     0,     0,
       1,     2,     0,     0,   -16,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    18,    19,    20,    21,     0,     0,    22,    23,     0,
      24,    25,    26,    27,    28,    29,     0,    30,    31,     0,
       0,     0,    32,    33,   539,   540,   541,     0,   542,     0,
       0,     0,     0,     0,     0,     0,   873,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,    35,     0,   539,
     540,   541,    36,   542,     0,     0,     0,     0,   517,   518,
     519,   874,    37,    38,     0,    39,    40,    41,     0,     0,
       0,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       0,     0,     0,     0,     0,     0,    51,    52,    53,     0,
      54,   520,   521,   522,     0,     0,     0,    55,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   539,   540,
     541,    56,   542,     0,    57,    58,    59,    60,    61,    62,
     877,     0,     0,     0,    63,    64,   536,   537,   538,     0,
      65,    66,     0,    67,    68,     0,     0,    69,     0,    70,
       0,     0,    71,     0,     0,     0,    72,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,     0,
       0,     0,     0,     0,     0,    76,    77,     1,     2,    78,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,    15,    16,    17,    18,    19,
      20,    21,   525,   526,    22,    23,     0,    24,    25,    26,
      27,    28,    29,     0,    30,    31,     0,     0,     0,    32,
      33,     0,     0,     0,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,     0,     0,
       0,   937,     0,     0,   938,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    35,   517,   518,   519,     0,    36,
       0,     0,     0,     0,   536,   537,   538,     0,     0,    37,
      38,     0,    39,    40,    41,     0,     0,     0,    42,    43,
      44,    45,    46,    47,    48,    49,    50,     0,   520,   521,
     522,     0,     0,    51,    52,    53,     0,    54,     0,     0,
     523,   524,     0,     0,    55,     0,     0,   539,   540,   541,
       0,   542,     0,     0,     0,     0,     0,     0,     0,   878,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,    57,    58,    59,    60,    61,    62,     0,     0,     0,
       0,    63,    64,   536,   537,   538,     0,    65,    66,     0,
      67,    68,     0,     0,    69,     0,    70,     0,     0,    71,
       0,     0,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,     0,     0,     0,     1,
       2,     0,    76,    77,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,     0,     0,     0,   525,
     526,     0,     0,    21,     0,     0,    22,    23,     0,    24,
      25,    26,    27,    28,    29,     0,   633,    31,     0,     0,
       0,    32,    33,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   539,   540,   541,   939,   542,
       0,   940,     0,     0,   517,   518,   519,   917,     0,     0,
       0,     0,     0,     0,     0,    34,    35,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,   517,   518,   519,
       0,    37,    38,     0,    39,    40,    41,   520,   521,   522,
      42,    43,    44,    45,    46,    47,    48,    49,    50,   523,
     524,   536,   537,   538,     0,    51,    52,    53,     0,    54,
     520,   521,   522,     0,     0,     0,    55,     0,     0,     0,
       0,     0,   523,   524,   539,   540,   541,     0,   542,     0,
       0,     0,     0,     0,     0,     0,   919,     0,     0,     0,
      56,     0,     0,    57,    58,    59,    60,    61,    62,     0,
       0,     0,     0,    63,    64,   517,   518,   519,     0,    65,
      66,     0,    67,    68,     0,     0,    69,     0,    70,     0,
       0,    71,   517,   518,   519,    72,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,   520,   521,
     522,     0,     0,     0,    76,    77,     0,     0,   525,   526,
     523,   524,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,   517,   518,   519,     0,     0,   523,   524,     0,
       0,   525,   526,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   517,   518,   519,     0,     0,     0,
    1078,     0,     0,     0,     0,   520,   521,   522,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   523,   524,   517,
     518,   519,     0,  1083,     0,     0,     0,   520,   521,   522,
       0,     0,   539,   540,   541,     0,   542,     0,     0,   523,
     524,     0,     0,     0,   920,     0,     0,     0,     0,     0,
       0,     0,   520,   521,   522,   536,   537,   538,     0,   525,
     526,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   525,   526,     0,   517,
     518,   519,     0,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   517,   518,   519,     0,     0,
       0,  1085,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   520,   521,   522,     0,   525,   526,  1086,     0,
       0,     0,     0,     0,   523,   524,     0,     0,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,   525,   526,
     523,   524,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,     0,     0,   517,   518,   519,     0,  1106,     0,
       0,     0,     0,   525,   526,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   517,   518,   519,     0,     0,     0,
    1107,     0,     0,     0,     0,     0,     0,   520,   521,   522,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   523,
     524,   536,   537,   538,     0,  1108,     0,   520,   521,   522,
     536,   537,   538,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,   525,   526,     0,   539,   540,   541,     0,
     542,     0,     0,     0,     0,     0,     0,     0,   921,   525,
     526,     0,   517,   518,   519,     0,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,     0,     0,     0,  1109,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   520,   521,   522,   790,   759,
       0,     0,     0,     0,     0,     0,     0,   523,   524,   536,
     537,   538,   520,   521,   522,     0,     0,     0,   525,   526,
       0,     0,     0,     0,   523,   524,     0,     0,     0,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   525,   526,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   517,   518,   519,   792,   759,     0,
       0,     0,   520,   521,   522,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   523,   524,     0,   794,   759,     0,
       0,     0,   536,   537,   538,     0,     0,   520,   521,   522,
       0,     0,   539,   540,   541,     0,   542,     0,     0,   523,
     524,   539,   540,   541,   922,   542,   525,   526,     0,     0,
       0,     0,     0,   982,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,     0,     0,     0,   799,   759,   520,   521,   522,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   523,
     524,     0,   801,   759,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   525,   526,     0,   517,   518,   519,     0,
     539,   540,   541,     0,   542,     0,     0,     0,     0,     0,
       0,     0,   986,     0,     0,     0,     0,     0,   525,   526,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   520,
     521,   522,   803,   759,     0,     0,     0,   517,   518,   519,
       0,   523,   524,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   517,   518,   519,   806,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,   521,   522,   539,   540,   541,     0,   542,   525,   526,
       0,     0,   523,   524,     0,   996,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   517,   518,   519,   808,   759,     0,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,   517,   518,   519,     0,   523,   524,     0,     0,     0,
     525,   526,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   523,   524,     0,   810,
     759,   525,   526,   520,   521,   522,     0,   536,   537,   538,
       0,     0,     0,     0,     0,   523,   524,     0,   525,   526,
       0,     0,     0,     0,     0,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,     0,     0,     0,
     817,   759,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,   819,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   536,   537,   538,     0,     0,     0,     0,   525,   526,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
       0,     0,     0,   821,   759,   525,   526,     0,     0,     0,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,   825,   759,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,   827,   759,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,   829,   759,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,   539,   540,
     541,     0,   542,   520,   521,   522,   517,   518,   519,     0,
    1067,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,   539,   540,   541,     0,   542,     0,   523,   524,
       0,     0,     0,     0,  1072,   520,   521,   522,   536,   537,
     538,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,   536,   537,   538,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,   831,   759,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,   834,   759,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
     836,   759,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,   838,
     759,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,   840,   759,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   517,   518,   519,   844,   759,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,   846,   759,     0,     0,     0,     0,     0,   539,
     540,   541,     0,   542,     0,   520,   521,   522,   517,   518,
     519,  1076,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,   539,   540,
     541,     0,   542,     0,   523,   524,     0,     0,     0,     0,
    1077,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,   536,   537,   538,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,   849,   759,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,   957,   759,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,   958,   759,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,   959,   759,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,   960,   759,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,  1190,  1191,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,     0,   759,     0,     0,     0,   539,   540,
     541,     0,   542,     0,   520,   521,   522,   517,   518,   519,
    1080,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,   536,   537,
     538,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,   536,   537,   538,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,     0,   867,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,     0,   868,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
       0,   870,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,     0,
     879,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,     0,   880,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   517,   518,   519,     0,   881,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,     0,   882,     0,     0,     0,     0,     0,   539,
     540,   541,     0,   542,     0,   520,   521,   522,   517,   518,
     519,  1081,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,   539,   540,
     541,     0,   542,     0,   523,   524,     0,     0,     0,     0,
    1099,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,   536,   537,   538,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,     0,   883,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,     0,   884,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,     0,   885,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,     0,   886,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,     0,   887,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,     0,   888,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,     0,   889,     0,     0,     0,   539,   540,
     541,     0,   542,     0,   520,   521,   522,   517,   518,   519,
    1102,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,   536,   537,
     538,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,     0,   890,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,     0,   891,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
       0,   892,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,     0,
     893,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,     0,   895,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   517,   518,   519,     0,   896,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,     0,   898,     0,     0,     0,     0,     0,   539,
     540,   541,     0,   542,     0,   520,   521,   522,   517,   518,
     519,  1128,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,     0,   899,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,     0,   903,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,     0,   904,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,     0,   905,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,     0,   906,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,     0,   907,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,     0,   908,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,     0,   911,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,     0,   912,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
       0,   913,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,     0,
     914,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,     0,   915,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   517,   518,   519,     0,   918,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,     0,  1025,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,     0,  1036,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,     0,  1046,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,     0,  1065,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,     0,  1066,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,     0,  1068,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,     0,  1074,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,     0,  1087,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,     0,  1090,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,     0,  1091,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
       0,  1097,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,     0,
    1120,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,     0,  1121,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   517,   518,   519,     0,  1122,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,     0,  1123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,     0,  1152,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,     0,  1163,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,     0,  1164,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,     0,  1166,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,     0,  1174,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,     0,  1177,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,     0,  1178,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,     0,  1188,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,     0,  1189,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
       0,  1192,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,     0,
    1196,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,     0,  1214,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   517,   518,   519,     0,  1219,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,     0,  1220,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,     0,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,     0,  1226,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,     0,  1227,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,     0,  1228,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,     0,  1229,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,     0,  1230,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   517,   518,   519,     0,  1236,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,   547,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,     0,     0,   525,   526,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,   549,     0,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,   551,     0,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
     556,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,   558,
       0,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,   560,     0,     0,     0,
     517,   518,   519,   527,   528,   529,   530,   531,   532,   533,
       0,   534,     0,     0,     0,   563,   517,   518,   519,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,     0,
       0,     0,   565,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,   520,   521,   522,   525,   526,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,   520,   521,   522,
     525,   526,     0,     0,     0,     0,     0,     0,     0,   523,
     524,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,   569,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,   580,
       0,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,   582,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,   584,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,   588,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,   590,     0,     0,     0,   517,   518,   519,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,     0,     0,
       0,   592,   517,   518,   519,   527,   528,   529,   530,   531,
     532,   533,     0,   534,     0,     0,     0,   594,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,   520,   521,   522,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,   520,   521,   522,   525,
     526,     0,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,     0,   525,   526,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,   595,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,   597,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,   601,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,   606,     0,     0,     0,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
     609,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   517,   518,   519,   612,     0,     0,
       0,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   517,   518,   519,   618,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,   520,   521,
     522,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,   520,   521,   522,   525,   526,
       0,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,     0,     0,   525,   526,     0,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,   798,     0,     0,
       0,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,   865,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,   866,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,   901,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,   902,
       0,     0,     0,   517,   518,   519,   527,   528,   529,   530,
     531,   532,   533,     0,   534,     0,     0,     0,   943,   517,
     518,   519,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,     0,     0,     0,   945,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,     0,   520,   521,   522,   525,   526,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
     520,   521,   522,   525,   526,     0,     0,     0,     0,     0,
       0,     0,   523,   524,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,   948,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,   950,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,   956,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,   964,     0,
       0,     0,     0,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,   966,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,   968,     0,     0,     0,   517,   518,
     519,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,     0,     0,     0,   970,   517,   518,   519,   527,   528,
     529,   530,   531,   532,   533,     0,   534,     0,     0,     0,
     974,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
     520,   521,   522,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   525,   526,     0,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,     0,   525,
     526,     0,     0,     0,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,   975,     0,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,   977,     0,
       0,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,  1026,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,  1028,     0,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,  1030,     0,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   517,   518,   519,
    1034,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   517,   518,   519,  1040,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,   520,   521,   522,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,   520,   521,
     522,   525,   526,     0,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,     0,     0,     0,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
    1048,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,  1051,
       0,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,  1053,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,  1055,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,  1058,     0,     0,     0,   517,   518,   519,   527,
     528,   529,   530,   531,   532,   533,     0,   534,     0,     0,
       0,  1060,   517,   518,   519,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,     0,     0,     0,  1063,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,     0,   520,   521,   522,
     525,   526,     0,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,   520,   521,   522,   525,   526,     0,     0,
       0,     0,     0,     0,     0,   523,   524,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,  1088,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,  1089,     0,     0,     0,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,  1096,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,  1112,     0,     0,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,  1115,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,  1117,     0,     0,
       0,   517,   518,   519,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,     0,     0,     0,  1131,   517,   518,
     519,   527,   528,   529,   530,   531,   532,   533,     0,   534,
       0,     0,     0,  1134,   520,   521,   522,   517,   518,   519,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,   520,   521,   522,   517,   518,   519,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,     0,
     520,   521,   522,   517,   518,   519,     0,     0,     0,     0,
       0,     0,   523,   524,     0,     0,     0,   520,   521,   522,
     517,   518,   519,     0,     0,     0,     0,     0,     0,   523,
     524,     0,     0,     0,     0,     0,   520,   521,   522,   517,
     518,   519,     0,     0,     0,     0,     0,     0,   523,   524,
       0,     0,     0,   520,   521,   522,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,   520,   521,   522,   525,   526,     0,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,     0,
       0,     0,   525,   526,     0,     0,     0,     0,     0,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,   525,   526,     0,  1153,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,   525,   526,
       0,  1156,     0,     0,     0,     0,     0,     0,   527,   528,
     529,   530,   531,   532,   533,     0,   534,   525,   526,     0,
    1160,     0,     0,     0,     0,   527,   528,   529,   530,   531,
     532,   533,     0,   534,   525,   526,     0,  1170,     0,     0,
       0,     0,     0,     0,   527,   528,   529,   530,   531,   532,
     533,     0,   534,   525,   526,     0,  1175,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     517,   518,   519,  1193,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   517,
     518,   519,  1201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   520,   521,   522,   517,   518,   519,     0,
       0,     0,     0,     0,     0,   523,   524,     0,     0,     0,
       0,     0,   520,   521,   522,   517,   518,   519,     0,     0,
       0,     0,     0,     0,   523,   524,     0,     0,     0,   520,
     521,   522,   517,   518,   519,     0,     0,     0,     0,     0,
       0,   523,   524,     0,     0,     0,     0,     0,   520,   521,
     522,   517,   518,   519,     0,     0,     0,     0,     0,     0,
     523,   524,     0,     0,     0,   520,   521,   522,   517,   518,
     519,     0,     0,     0,     0,     0,     0,   523,   524,     0,
       0,     0,     0,     0,   520,   521,   522,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   523,   524,     0,     0,
       0,   520,   521,     0,   525,   526,     0,     0,     0,     0,
       0,     0,     0,   523,   524,     0,     0,     0,     0,   517,
     518,   519,     0,   525,   526,     0,     0,     0,     0,     0,
       0,   527,   528,   529,   530,   531,   532,   533,     0,   534,
     525,   526,     0,  1206,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,   525,
     526,     0,  1208,     0,   523,   524,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534,   525,   526,     0,  1216,
       0,     0,     0,     0,     0,     0,   527,   528,   529,   530,
     531,   532,   533,     0,   534,   525,   526,     0,  1221,     0,
       0,     0,     0,   527,   528,   529,   530,   531,   532,   533,
       0,   534,   525,   526,     0,  1224,   517,   518,   519,     0,
       0,     0,   527,   528,   529,   530,   531,   532,   533,     0,
     534,     0,     0,     0,     0,     0,     0,     0,     0,   527,
     528,   529,   530,   531,   532,   533,     0,   534,     0,   520,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   523,   524,   525,   526,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     527,   528,   529,   530,   531,   532,   533,     0,   534,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,   526,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   527,   528,   529,
     530,   531,   532,   533,     0,   534
};

static const yytype_int16 yycheck[] =
{
       1,    35,     3,     4,     5,   208,     7,     8,   215,    10,
      42,    43,    44,    14,   215,   208,    17,   208,   209,    20,
     227,   224,    23,    24,    25,    26,   227,   230,    19,   208,
      31,   208,    23,    34,    35,    36,    37,   230,    39,   208,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
     222,   223,   208,    54,   224,   227,    57,   222,   223,   208,
     209,   224,   227,   224,    65,    66,    67,    68,   225,   224,
      71,   228,    73,    74,    75,   222,   223,   208,   209,   224,
     227,   208,     5,     6,     7,   222,   223,   222,   223,     0,
     227,   225,   227,   223,   228,   208,   209,   226,   227,   225,
     226,   225,   226,   222,   223,    87,    88,   222,   223,    32,
     222,   223,   222,   223,   222,   226,   227,   226,   227,    87,
      88,    20,   225,   224,   224,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   224,   224,   224,    78,   224,   224,    81,    82,
      83,    84,   224,    86,   224,   224,    89,    90,    91,    92,
      93,    94,   224,   224,    97,   224,   224,   224,   101,   102,
     103,   213,   214,   215,   224,   217,   224,   224,   224,   224,
     113,   114,   115,   116,   117,   118,   224,   229,   199,   122,
     224,   124,   125,   126,   127,   128,   129,   224,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   224,   155,   156,   216,   217,   218,   219,   224,   221,
     163,   164,   165,   166,   224,   224,   169,   170,   171,   172,
     218,   219,   175,   221,   224,   178,   179,   224,   181,   224,
     183,   184,   224,   224,   224,   188,   267,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   224,   224,   224,   280,
     281,   224,   205,   206,   207,   208,   209,   288,   224,   224,
     291,   224,   293,   216,   224,   224,   297,   224,   299,   224,
     301,   224,   303,    42,    43,    44,   224,   230,   224,   310,
     224,   224,   313,   232,   224,   316,   224,   318,   224,   224,
     224,   224,   224,   324,   224,   326,   327,   328,   329,   224,
     224,   332,   224,   224,   335,   224,   224,   338,   224,   340,
     224,   224,   224,   224,   224,   346,   224,   348,   224,   224,
     351,   224,   353,   224,   224,   224,   224,   358,   224,   224,
     224,   395,   363,    42,    43,    44,   224,   224,   224,   224,
     224,   224,   224,   374,   224,   376,   224,   224,   379,   224,
     224,   382,   224,   384,   224,   386,   224,   388,   224,   390,
      42,    43,    44,   224,   395,   396,   224,   398,   224,   224,
     224,   224,   224,   224,   224,   406,   407,   408,   409,   410,
     224,   227,   227,   208,   215,   226,   226,   226,   419,   420,
     226,   422,   423,   424,   425,   426,   427,   428,   226,   430,
     431,   432,   433,   434,   435,   226,   226,   438,   439,   440,
     441,   226,   221,    42,    43,    44,   226,   231,   226,   226,
     451,   452,   453,   454,   455,   456,   457,   458,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   213,   214,   215,   226,   217,   226,
     226,   226,   226,   226,   226,   226,   225,   226,   226,   226,
     491,   492,   493,   226,   226,   226,   226,   498,   499,   226,
     226,   226,   503,   226,   226,   226,   226,    42,    43,    44,
     226,   512,   513,   226,   226,   226,   517,   518,   519,   520,
     521,   522,   523,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   213,   214,   215,   226,   217,   226,
     224,   542,   224,   544,   545,   217,   547,   226,   549,   224,
     551,   224,   232,   227,   215,   556,   227,   558,   215,   560,
     561,   213,   214,   215,   231,   217,   228,   208,   569,   570,
     228,   572,   228,   574,   226,   576,     5,     6,     7,   580,
     228,   215,   215,   584,   226,   586,    81,   588,   400,   590,
     403,   996,   798,   594,   595,   402,   597,    -1,    -1,    -1,
     601,    -1,    -1,    32,    -1,   606,    -1,    -1,    -1,   610,
      42,    43,    44,    -1,   213,   214,   215,   618,   217,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    -1,    -1,    -1,    78,
      -1,    -1,    81,    82,    83,    84,    -1,    86,    -1,    -1,
      89,    90,    91,    92,    93,    94,    -1,    -1,    97,    -1,
      -1,    -1,   101,   102,   103,    -1,    -1,    -1,   213,   214,
     215,    -1,   217,    -1,   113,   114,   115,   116,   117,   118,
     225,   226,    -1,   122,    -1,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,   155,   156,    -1,    -1,
      -1,    42,    43,    44,   163,   164,   165,   166,    -1,    -1,
     169,   170,   171,   172,    -1,    -1,   175,    -1,    -1,   178,
     179,    -1,   181,    -1,   183,   184,    87,    88,    -1,   188,
      -1,   190,   191,    -1,   193,   194,    -1,   196,   197,   198,
      -1,    -1,    -1,    -1,    -1,    -1,   205,   206,   207,   208,
     209,   213,   214,   215,    -1,   217,    -1,   216,    -1,   790,
      -1,   792,    -1,   794,   226,   224,   225,   798,   799,    -1,
     801,    -1,   803,   232,    -1,    -1,    -1,    -1,    -1,   810,
      -1,    -1,    -1,    -1,    -1,    -1,   817,    42,    43,    44,
     821,    -1,   823,    -1,   825,    -1,   827,    -1,   829,    -1,
     831,    -1,    -1,   834,    -1,   836,    -1,   838,    -1,    -1,
     841,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   849,    42,
      43,    44,    -1,    -1,   855,   186,   187,    -1,    -1,    -1,
     861,    -1,   863,   864,   865,   866,    -1,    -1,    -1,    -1,
      -1,   872,   873,   874,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    44,    -1,    -1,   216,   217,   218,   219,    -1,
     221,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
     901,   902,   213,   214,   215,    -1,   217,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   948,   226,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,   930,
      -1,    -1,    -1,    -1,   935,    -1,   937,    -1,   939,    -1,
      -1,    -1,   943,    -1,   945,     1,     2,   948,    -1,   950,
      -1,   952,    -1,    -1,    -1,   956,   957,   958,   959,   960,
      -1,    -1,    -1,   964,    -1,   966,    -1,    23,    24,   970,
      42,    43,    44,   974,   975,    -1,   977,    -1,    -1,    -1,
      36,   982,    38,    39,    40,    42,    43,    44,   213,   214,
     215,   992,   217,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      56,   226,    58,    -1,    60,    61,    -1,    -1,    -1,    -1,
      66,    67,    -1,    69,    70,    -1,    72,    -1,    -1,    -1,
     213,   214,   215,    -1,   217,  1026,    28,  1028,    -1,  1030,
      -1,    -1,   225,  1034,    -1,    -1,    -1,    -1,    -1,  1040,
      42,    43,    44,    -1,    -1,    -1,    -1,  1048,    -1,    -1,
    1051,   213,   214,   215,  1055,   217,    -1,  1058,    -1,  1060,
      -1,    -1,  1063,   225,   226,    -1,  1067,   213,   214,   215,
      -1,   217,    -1,    75,    76,    77,    -1,    -1,    -1,   225,
     226,  1115,    -1,    -1,    -1,    87,    88,  1088,  1089,    -1,
      -1,  1092,    -1,  1094,    -1,  1096,   213,   214,   215,  1100,
     217,    -1,    42,    43,    44,    -1,    -1,    -1,   225,   226,
      -1,  1112,    -1,    -1,  1115,    -1,  1117,  1118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1128,    -1,    -1,
    1131,    -1,    -1,  1134,    -1,    -1,    -1,    -1,    -1,  1140,
      -1,   213,   214,   215,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,  1153,   225,   226,  1156,   213,   214,   215,  1160,
     217,    -1,    -1,    -1,    -1,    -1,  1167,  1201,   225,  1170,
      -1,    42,    43,    44,  1175,    -1,    -1,    -1,    -1,  1180,
      -1,    29,    -1,  1184,   186,   187,    -1,    -1,    -1,  1190,
      -1,    -1,  1193,  1194,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,  1208,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      -1,    -1,    -1,  1224,    -1,   281,    -1,    75,    76,    77,
      -1,    -1,   288,    -1,    -1,   291,    -1,   293,    -1,    87,
      88,    -1,    -1,   299,    -1,   301,    -1,   303,    42,    43,
      44,    -1,    -1,    -1,   310,    -1,    -1,   313,    -1,    -1,
     316,    -1,   318,    -1,    42,    43,    44,    -1,   324,    42,
      43,    44,    -1,   213,   214,   215,   332,   217,    -1,   335,
      -1,    -1,   338,    -1,   340,   225,   226,    42,    43,    44,
     346,    -1,   348,    -1,    -1,   351,    -1,   353,    -1,    -1,
      -1,    -1,   358,    -1,    -1,    -1,    -1,   363,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   374,    -1,
     376,    -1,    -1,   379,    -1,    -1,   382,    -1,   384,    -1,
     386,    -1,   388,    -1,   390,    -1,    -1,    -1,   186,   187,
      -1,   397,   213,   214,   215,    -1,   217,    -1,    -1,    -1,
      42,    43,    44,   409,   225,   411,   412,   413,   414,   415,
     416,   417,   418,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   213,   214,   215,    -1,   217,    -1,
      -1,   437,    -1,   439,    -1,    -1,   225,   226,    -1,    -1,
      -1,    -1,   448,    -1,    -1,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,   459,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   225,   226,   489,    -1,   213,   214,   215,    -1,   217,
     213,   214,   215,    -1,   217,   501,   502,   225,   226,   505,
     506,   507,   508,   226,    -1,    -1,    -1,    -1,   213,   214,
     215,    -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     225,   226,    -1,    -1,    -1,   531,    -1,    -1,    -1,    -1,
     536,   537,   538,   539,   540,   541,   542,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   563,    -1,   565,
      -1,    -1,    -1,    -1,   570,    -1,   572,    -1,   574,    -1,
     576,   213,   214,   215,    -1,   217,   582,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,    -1,   592,    -1,     5,     6,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   609,    -1,    -1,   612,    -1,    -1,    -1,
     616,    42,    43,    44,   620,    32,   213,   214,   215,    -1,
     217,    -1,   628,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    78,    -1,    -1,    81,    82,    83,    84,    -1,    86,
      -1,    -1,    89,    90,    91,    92,    93,    94,    -1,    -1,
      97,    -1,    -1,    -1,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,   170,   171,   172,    -1,    -1,   175,    -1,
      -1,   178,   179,    -1,   181,    -1,   183,   184,    -1,    -1,
      -1,   188,    -1,   190,   191,    -1,   193,   194,    -1,   196,
     197,   198,   213,   214,   215,    -1,   217,    -1,   205,   206,
     207,   208,   209,    -1,    -1,   226,    -1,    -1,    -1,   216,
     806,    -1,   808,    -1,    -1,    -1,    -1,   224,   225,    -1,
      42,    43,    44,   819,    -1,   232,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   829,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   840,    -1,    -1,    -1,   844,    -1,
     846,   847,    42,    43,    44,   851,    -1,   853,   854,    -1,
      -1,   857,   858,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,   877,   878,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    87,    88,    42,
      43,    44,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    42,    43,
      44,   917,    -1,   919,   920,   921,   922,   923,    -1,   925,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    42,    43,
      44,    -1,    -1,    87,    88,    42,    43,    44,   964,    -1,
      -1,    -1,   968,    -1,   970,    75,    76,    77,   186,   187,
      42,    43,    44,    -1,    -1,    -1,    -1,    87,    88,    -1,
     986,   213,   214,   215,    -1,   217,   186,   187,    -1,    -1,
     996,    -1,   998,    -1,   226,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    42,    43,    44,    -1,
      -1,   229,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,   225,    -1,    -1,   228,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      -1,    -1,  1048,   225,    -1,    -1,   228,  1053,    -1,  1055,
      -1,    -1,   186,   187,   213,   214,   215,    -1,   217,    -1,
     213,   214,   215,    -1,   217,    -1,  1072,   226,    -1,    -1,
    1076,  1077,    -1,   226,  1080,  1081,   186,   187,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,    -1,    -1,
      -1,   225,    -1,  1099,   228,    -1,  1102,   213,   214,   215,
      -1,   217,    -1,   213,   214,   215,   216,   217,   218,   219,
     226,   221,    -1,    -1,    -1,   225,    -1,    -1,   228,   213,
     214,   215,    -1,   217,    -1,  1131,   213,   214,   215,    -1,
     217,    -1,   226,    -1,    -1,    -1,    -1,  1143,    -1,   226,
      -1,   213,   214,   215,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     5,     6,     7,   213,   214,   215,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     226,    -1,    -1,   213,   214,   215,    -1,   217,    -1,    -1,
    1206,    32,    -1,    -1,    -1,    -1,   226,    -1,    42,    43,
      44,    -1,    -1,    -1,    -1,  1221,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    -1,    -1,    -1,    78,    -1,    -1,
      81,    82,    83,    84,    -1,    86,    -1,    -1,    89,    90,
      91,    92,    93,    94,    -1,    -1,    97,    42,    43,    44,
     101,   102,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
      -1,   122,    -1,   124,   125,   126,   127,   128,   129,    -1,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,    -1,   155,   156,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,   170,
     171,   172,    -1,    -1,   175,    -1,    -1,   178,   179,    -1,
     181,    -1,   183,   184,    -1,    -1,    -1,   188,    -1,   190,
     191,   192,   193,   194,   195,   196,   197,   198,     5,     6,
       7,    -1,    -1,    -1,   205,   206,   207,   208,   209,   213,
     214,   215,    -1,   217,    -1,   216,    -1,    -1,    -1,    -1,
      -1,    -1,   226,   224,    -1,    32,    -1,    -1,    -1,   230,
      -1,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,   213,   214,
     215,    78,   217,    -1,    81,    82,    83,    84,    -1,    86,
      -1,   226,    89,    90,    91,    92,    93,    94,    -1,    -1,
      97,    42,    43,    44,   101,   102,   103,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,    -1,   122,    -1,   124,   125,   126,
     127,   128,   129,    -1,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,    -1,   155,   156,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,   170,   171,   172,    -1,    -1,   175,    -1,
      -1,   178,   179,    -1,   181,    -1,   183,   184,    -1,    -1,
      -1,   188,    -1,   190,   191,    -1,   193,   194,    -1,   196,
     197,   198,     5,     6,     7,    -1,    -1,    -1,   205,   206,
     207,   208,   209,   213,   214,   215,    -1,   217,    -1,   216,
      -1,    -1,    -1,    -1,    -1,    -1,   226,   224,    -1,    32,
      -1,    -1,    -1,   230,    -1,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,   213,   214,   215,    78,   217,    -1,    81,    82,
      83,    84,    -1,    86,    -1,   226,    89,    90,    91,    92,
      93,    94,    -1,    -1,    97,    42,    43,    44,   101,   102,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     113,   114,   115,   116,   117,   118,    -1,    -1,    -1,   122,
      -1,   124,   125,   126,   127,   128,   129,    -1,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,    -1,   155,   156,    -1,    -1,    -1,    -1,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,   170,   171,   172,
      -1,    -1,   175,    -1,    -1,   178,   179,    -1,   181,    -1,
     183,   184,    -1,    -1,    -1,   188,    -1,   190,   191,    -1,
     193,   194,    -1,   196,   197,   198,     5,     6,     7,    -1,
      -1,    -1,   205,   206,   207,   208,   209,   213,   214,   215,
      -1,   217,    -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,
     226,   224,    -1,    32,    -1,    -1,    -1,   230,    -1,    -1,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,   213,   214,   215,    78,
     217,    -1,    81,    82,    83,    84,    -1,    86,    -1,   226,
      89,    90,    91,    92,    93,    94,    -1,    -1,    97,    42,
      43,    44,   101,   102,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,   115,   116,   117,   118,
      -1,    -1,    -1,   122,    -1,   124,   125,   126,   127,   128,
     129,    -1,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,    -1,   155,   156,    -1,    -1,
      -1,    -1,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,   170,   171,   172,    -1,    -1,   175,    -1,    -1,   178,
     179,    -1,   181,    -1,   183,   184,    -1,    -1,    -1,   188,
      -1,   190,   191,    -1,   193,   194,    -1,   196,   197,   198,
       5,     6,     7,    -1,    -1,    -1,   205,   206,   207,   208,
     209,   213,   214,   215,    -1,   217,    -1,   216,    -1,    -1,
      -1,    -1,    -1,    -1,   226,   224,    -1,    32,    -1,    -1,
      -1,   230,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
     213,   214,   215,    78,   217,    -1,    81,    82,    83,    84,
      -1,    86,    -1,   226,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,   226,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    -1,    -1,    -1,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,    -1,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
       5,     6,     7,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,    -1,    -1,    32,    -1,    -1,
      -1,   216,    -1,    -1,    42,    43,    44,    -1,    -1,   224,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      -1,    -1,    -1,    78,    -1,    -1,    81,    82,    83,    84,
      -1,    86,    -1,    -1,    89,    90,    91,    92,    93,    94,
      -1,    -1,    97,    42,    43,    44,   101,   102,   103,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
     115,   116,   117,   118,    42,    43,    44,   122,    -1,   124,
     125,   126,   127,   128,   129,    -1,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,    -1,
     155,   156,    -1,    -1,    42,    43,    44,    -1,   163,   164,
     165,   166,    -1,    -1,   169,   170,   171,   172,    -1,    -1,
     175,    -1,    -1,   178,   179,    -1,   181,    -1,   183,   184,
      42,    43,    44,   188,    -1,   190,   191,    -1,   193,   194,
      -1,   196,   197,   198,    -1,    -1,    -1,    -1,    -1,    -1,
     205,   206,   207,   208,   209,   213,   214,   215,    -1,   217,
      -1,   216,    -1,    -1,    -1,     3,     4,    -1,   226,   224,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      -1,    -1,    30,    31,    -1,    33,    34,    35,    36,    37,
      38,    -1,    40,    41,    -1,    -1,    -1,    45,    46,    -1,
      -1,    -1,    -1,    -1,   213,   214,   215,    -1,   217,    -1,
     213,   214,   215,    -1,   217,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   226,    -1,   213,   214,   215,    -1,   217,
      -1,    79,    80,    -1,    -1,    -1,    -1,    85,   226,    -1,
      -1,    -1,    -1,    42,    43,    44,    -1,    95,    96,    -1,
      98,    99,   100,    -1,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   213,   214,   215,    -1,   217,
      -1,   119,   120,   121,    -1,   123,    -1,   225,    42,    43,
      44,    -1,   130,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,    -1,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   225,    -1,    -1,   154,    -1,    -1,   157,
     158,   159,   160,   161,   162,    -1,    -1,    -1,    -1,   167,
     168,    -1,    -1,    -1,    -1,   173,   174,    -1,   176,   177,
      -1,    -1,   180,    -1,   182,    -1,    -1,   185,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,   199,   200,   201,    -1,    -1,    -1,    -1,    -1,    -1,
     208,   209,    -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,
       3,     4,    -1,    -1,   222,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    30,    31,    -1,
      33,    34,    35,    36,    37,    38,    -1,    40,    41,    -1,
      -1,    -1,    45,    46,   213,   214,   215,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,   213,
     214,   215,    85,   217,    -1,    -1,    -1,    -1,    42,    43,
      44,   225,    95,    96,    -1,    98,    99,   100,    -1,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,    -1,    -1,    -1,    -1,   119,   120,   121,    -1,
     123,    75,    76,    77,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,
     215,   154,   217,    -1,   157,   158,   159,   160,   161,   162,
     225,    -1,    -1,    -1,   167,   168,    42,    43,    44,    -1,
     173,   174,    -1,   176,   177,    -1,    -1,   180,    -1,   182,
      -1,    -1,   185,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   199,   200,   201,    -1,
      -1,    -1,    -1,    -1,    -1,   208,   209,     3,     4,   212,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,   186,   187,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    -1,    40,    41,    -1,    -1,    -1,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,    -1,    -1,
      -1,   225,    -1,    -1,   228,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    79,    80,    42,    43,    44,    -1,    85,
      -1,    -1,    -1,    -1,    42,    43,    44,    -1,    -1,    95,
      96,    -1,    98,    99,   100,    -1,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,    75,    76,
      77,    -1,    -1,   119,   120,   121,    -1,   123,    -1,    -1,
      87,    88,    -1,    -1,   130,    -1,    -1,   213,   214,   215,
      -1,   217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
      -1,   157,   158,   159,   160,   161,   162,    -1,    -1,    -1,
      -1,   167,   168,    42,    43,    44,    -1,   173,   174,    -1,
     176,   177,    -1,    -1,   180,    -1,   182,    -1,    -1,   185,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   199,   200,   201,    -1,    -1,    -1,     3,
       4,    -1,   208,   209,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,   186,
     187,    -1,    -1,    27,    -1,    -1,    30,    31,    -1,    33,
      34,    35,    36,    37,    38,    -1,    40,    41,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   213,   214,   215,   225,   217,
      -1,   228,    -1,    -1,    42,    43,    44,   225,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    42,    43,    44,
      -1,    95,    96,    -1,    98,    99,   100,    75,    76,    77,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    87,
      88,    42,    43,    44,    -1,   119,   120,   121,    -1,   123,
      75,    76,    77,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    87,    88,   213,   214,   215,    -1,   217,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   225,    -1,    -1,    -1,
     154,    -1,    -1,   157,   158,   159,   160,   161,   162,    -1,
      -1,    -1,    -1,   167,   168,    42,    43,    44,    -1,   173,
     174,    -1,   176,   177,    -1,    -1,   180,    -1,   182,    -1,
      -1,   185,    42,    43,    44,   189,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   199,   200,   201,    75,    76,
      77,    -1,    -1,    -1,   208,   209,    -1,    -1,   186,   187,
      87,    88,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    87,    88,    -1,
      -1,   186,   187,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    42,    43,    44,    -1,    -1,    -1,
     228,    -1,    -1,    -1,    -1,    75,    76,    77,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,    87,    88,    42,
      43,    44,    -1,   228,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,   213,   214,   215,    -1,   217,    -1,    -1,    87,
      88,    -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,   186,
     187,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,    42,    43,    44,    -1,    -1,
      -1,   228,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    75,    76,    77,    -1,   186,   187,   228,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
      87,    88,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    -1,    -1,    42,    43,    44,    -1,   228,    -1,
      -1,    -1,    -1,   186,   187,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    42,    43,    44,    -1,    -1,    -1,
     228,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    87,
      88,    42,    43,    44,    -1,   228,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,   186,   187,    -1,   213,   214,   215,    -1,
     217,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   225,   186,
     187,    -1,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,    -1,    -1,    -1,   228,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,    75,    76,    77,   225,   226,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    42,
      43,    44,    75,    76,    77,    -1,    -1,    -1,   186,   187,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    42,    43,    44,   225,   226,    -1,
      -1,    -1,    75,    76,    77,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    87,    88,    -1,   225,   226,    -1,
      -1,    -1,    42,    43,    44,    -1,    -1,    75,    76,    77,
      -1,    -1,   213,   214,   215,    -1,   217,    -1,    -1,    87,
      88,   213,   214,   215,   225,   217,   186,   187,    -1,    -1,
      -1,    -1,    -1,   225,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    -1,    -1,    -1,   225,   226,    75,    76,    77,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    87,
      88,    -1,   225,   226,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    42,    43,    44,    -1,
     213,   214,   215,    -1,   217,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   225,    -1,    -1,    -1,    -1,    -1,   186,   187,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    75,
      76,    77,   225,   226,    -1,    -1,    -1,    42,    43,    44,
      -1,    87,    88,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    42,    43,    44,   225,   226,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,   213,   214,   215,    -1,   217,   186,   187,
      -1,    -1,    87,    88,    -1,   225,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    42,    43,    44,   225,   226,    -1,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    87,    88,    -1,    -1,    -1,
     186,   187,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,    87,    88,    -1,   225,
     226,   186,   187,    75,    76,    77,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,   186,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,    -1,    -1,    -1,
     225,   226,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,   226,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    -1,    -1,    -1,    -1,   186,   187,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      -1,    -1,    -1,   225,   226,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,   225,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,   225,   226,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,   213,   214,
     215,    -1,   217,    75,    76,    77,    42,    43,    44,    -1,
     225,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,   213,   214,   215,    -1,   217,    -1,    87,    88,
      -1,    -1,    -1,    -1,   225,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,   225,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,   225,   226,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
     225,   226,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
     226,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,   225,   226,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,   225,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,   225,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,    -1,   217,    -1,    75,    76,    77,    42,    43,
      44,   225,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,   213,   214,
     215,    -1,   217,    -1,    87,    88,    -1,    -1,    -1,    -1,
     225,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,   225,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,   226,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,   225,   226,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,   225,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,    -1,   217,    -1,    75,    76,    77,    42,    43,    44,
     225,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    42,    43,    44,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
      -1,   226,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,    -1,
     226,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,    -1,   217,    -1,    75,    76,    77,    42,    43,
      44,   225,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,   213,   214,
     215,    -1,   217,    -1,    87,    88,    -1,    -1,    -1,    -1,
     225,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,    -1,   226,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,    -1,   226,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,    -1,   217,    -1,    75,    76,    77,    42,    43,    44,
     225,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
      -1,   226,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,    -1,
     226,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,    -1,   217,    -1,    75,    76,    77,    42,    43,
      44,   225,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,    -1,   226,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,    -1,   226,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
      -1,   226,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,    -1,
     226,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,    -1,   226,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,    -1,   226,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
      -1,   226,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,    -1,
     226,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,    -1,   226,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,    -1,   226,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,    -1,   226,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
      -1,   226,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,    -1,
     226,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,    -1,   226,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,    -1,   226,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,    -1,   226,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,    -1,   226,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,    -1,   226,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,    -1,   226,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    42,    43,    44,    -1,   226,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,   225,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,   225,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,
      42,    43,    44,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    -1,    -1,    -1,   225,    42,    43,    44,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    -1,
      -1,    -1,   225,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    75,    76,    77,
     186,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,   225,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,    -1,    -1,    -1,    42,    43,    44,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,    -1,    -1,
      -1,   225,    42,    43,    44,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    -1,    -1,    -1,   225,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,   186,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,   225,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
     225,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    42,    43,    44,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,    42,    43,    44,   225,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    75,    76,    77,   186,   187,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,    -1,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,   225,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,    42,    43,    44,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,    -1,    -1,    -1,   225,    42,
      43,    44,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,    -1,    -1,    -1,   225,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,   186,   187,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      75,    76,    77,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,   225,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,   225,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,   225,    -1,    -1,    -1,    42,    43,
      44,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    -1,    -1,    -1,   225,    42,    43,    44,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,    -1,    -1,    -1,
     225,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,   186,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,   186,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,    -1,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,   225,    -1,
      -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,   225,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,    42,    43,    44,
     225,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,    42,    43,    44,   225,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,
      77,   186,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
     225,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,   225,    -1,    -1,    -1,    42,    43,    44,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,    -1,    -1,
      -1,   225,    42,    43,    44,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,    -1,    -1,    -1,   225,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
     186,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,
      -1,    42,    43,    44,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,    -1,    -1,    -1,   225,    42,    43,
      44,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      -1,    -1,    -1,   225,    75,    76,    77,    42,    43,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    75,    76,    77,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      75,    76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    75,    76,    77,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,    42,
      43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,   186,   187,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,   186,   187,    -1,   225,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,   186,   187,
      -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,
     215,   216,   217,   218,   219,    -1,   221,   186,   187,    -1,
     225,    -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,
      -1,    -1,    -1,    -1,   213,   214,   215,   216,   217,   218,
     219,    -1,   221,   186,   187,    -1,   225,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
      42,    43,    44,   225,    -1,    -1,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    42,
      43,    44,   225,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    76,    77,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    75,
      76,    77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    75,    76,
      77,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    75,    76,    77,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    75,    76,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    75,    76,    -1,   186,   187,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    42,
      43,    44,    -1,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,   213,   214,   215,   216,   217,   218,   219,    -1,   221,
     186,   187,    -1,   225,    -1,    -1,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,   186,
     187,    -1,   225,    -1,    87,    88,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221,   186,   187,    -1,   225,
      -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,   216,
     217,   218,   219,    -1,   221,   186,   187,    -1,   225,    -1,
      -1,    -1,    -1,   213,   214,   215,   216,   217,   218,   219,
      -1,   221,   186,   187,    -1,   225,    42,    43,    44,    -1,
      -1,    -1,   213,   214,   215,   216,   217,   218,   219,    -1,
     221,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,
     214,   215,   216,   217,   218,   219,    -1,   221,    -1,    75,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    88,   186,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     213,   214,   215,   216,   217,   218,   219,    -1,   221,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     186,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,   214,   215,
     216,   217,   218,   219,    -1,   221
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    21,    22,    23,    24,    25,
      26,    27,    30,    31,    33,    34,    35,    36,    37,    38,
      40,    41,    45,    46,    79,    80,    85,    95,    96,    98,
      99,   100,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   119,   120,   121,   123,   130,   154,   157,   158,   159,
     160,   161,   162,   167,   168,   173,   174,   176,   177,   180,
     182,   185,   189,   199,   200,   201,   208,   209,   212,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,     5,     6,
       7,    32,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    78,
      81,    82,    83,    84,    86,    89,    90,    91,    92,    93,
      94,    97,   101,   102,   103,   113,   114,   115,   116,   117,
     118,   122,   124,   125,   126,   127,   128,   129,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   155,   156,   163,   164,   165,   166,   169,   170,   171,
     172,   175,   178,   179,   181,   183,   184,   188,   190,   191,
     193,   194,   196,   197,   198,   205,   206,   207,   208,   209,
     216,   224,   326,   328,   208,   209,   326,   328,   224,   326,
     328,   224,   326,   224,   326,   208,   224,   230,   324,   224,
     326,   224,   326,   224,   326,   326,   326,   326,   208,   208,
     224,   326,   328,   224,   326,   328,   224,   326,   224,   326,
     208,   208,    19,    23,   224,   326,   208,   209,   208,   209,
     326,   208,   224,   324,   326,   224,   326,   328,   224,   326,
     224,   328,   326,   328,   328,   326,   224,   326,   224,   326,
     326,   224,   326,   224,   326,   326,   326,   326,   224,   326,
     328,   224,   224,   224,   326,   224,   328,   326,   328,   224,
     328,   328,   224,   208,   224,   326,   224,   326,   328,   224,
     326,   328,   224,   326,   224,   328,   224,   328,   224,   326,
     224,   328,   326,   326,   326,   215,   227,   215,   227,     0,
     222,   237,   223,    20,   225,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   326,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   326,   224,   224,   224,   224,   224,
     224,   224,   227,   227,   326,   326,   328,    42,    43,    44,
      75,    76,    77,    87,    88,   186,   187,   213,   214,   215,
     216,   217,   218,   219,   221,   229,    42,    43,    44,   213,
     214,   215,   217,   229,   227,   227,   326,   225,   326,   225,
     326,   225,   208,   325,   326,   326,   225,   326,   225,   326,
     225,   215,   326,   225,   326,   225,   226,   226,   326,   225,
     224,   326,   224,   326,   224,   326,   224,   326,   208,   326,
     225,   326,   225,   326,   225,   328,   225,   326,   225,   326,
     225,   326,   225,   326,   225,   225,   326,   225,   226,   226,
     326,   225,   226,   226,   226,   326,   225,   326,   328,   225,
     225,   326,   225,   226,   326,   328,   225,   326,   225,   328,
     225,   192,   195,   324,   326,   326,   192,   195,   230,   323,
     328,   326,   234,    40,   249,   248,   208,   209,   226,   326,
     326,   226,   326,   326,   328,   326,   328,   328,   328,   328,
     328,   328,   328,   328,   326,   326,   226,   326,   326,   326,
     326,   326,   326,   326,   226,   326,   326,   326,   326,   326,
     326,   328,   326,   326,   328,   226,   326,   226,   326,   226,
     226,   226,   226,   226,   226,   328,   226,   226,   326,   326,
     326,   326,   326,   326,   326,   226,   326,   328,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   328,   226,
     326,   326,   326,   226,   226,   226,   226,   226,   326,   226,
     326,   226,   328,   328,   326,   328,   328,   328,   328,   208,
     209,   226,   226,   225,   232,   326,   225,   232,   326,   226,
     226,   326,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   328,   326,   326,   326,
     328,   328,   328,   328,   328,   328,   326,   328,   326,   326,
     225,   326,   225,   326,   225,   326,   226,   231,   225,   225,
     326,   225,   326,   225,   326,   326,   225,   328,   225,   328,
     225,   326,   326,   326,   326,   326,   226,   225,   326,   225,
     328,   225,   326,   225,   326,   225,   326,   225,   326,   225,
     328,   225,   326,   326,   225,   326,   225,   326,   225,   326,
     225,   225,   328,   326,   225,   328,   225,   225,   328,   225,
     326,   225,   328,   224,   224,   225,   228,   224,   224,   327,
     328,   225,   228,   227,   227,   225,   225,   226,   226,   226,
     226,   226,   225,   225,   225,   226,   226,   225,   225,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   225,   225,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   225,   226,   225,
     225,   225,   225,   225,   226,   225,   226,   232,   225,   228,
     225,   228,   232,   225,   228,   225,   228,   225,   228,   225,
     228,   326,   326,   225,   326,   225,   325,   326,   225,   326,
     225,   326,    28,   328,   328,   326,   225,   225,   225,   225,
     225,   326,   328,   326,   225,   326,   225,   326,   225,   326,
     225,   326,   328,   326,   225,   225,   326,   225,   326,   326,
     328,   326,   225,   328,   328,   328,   225,   326,   328,   328,
     328,   326,   215,   328,   328,   231,   225,   326,   215,   326,
     326,   326,   326,   326,   326,   326,   328,   328,   326,   326,
     328,   328,   328,   328,   328,   328,   328,   228,   228,   326,
     228,   228,   326,   326,   326,   226,   225,   326,   225,   326,
     225,   208,   324,   326,   225,   326,   226,   326,   226,   226,
     225,   326,   326,   326,   326,   326,   226,   226,   225,   326,
     328,   225,   326,   225,   328,   225,   326,   328,   225,   226,
     225,   326,   326,   225,   326,   226,   226,   225,   226,   326,
     226,   226,   225,   328,   226,   226,   225,   225,   228,   326,
     225,   225,   327,   228,   328,   228,   228,   226,   225,   225,
     226,   226,   225,   226,   225,   226,   225,   226,   226,   225,
     225,   226,   225,   226,   226,   226,   228,   228,   228,   228,
     326,   326,   225,   208,   326,   225,   326,   225,    29,   326,
     226,   226,   226,   226,   326,   328,   326,   328,   225,   326,
     328,   225,   326,   326,   225,   326,   326,   328,   328,   328,
     215,   328,   328,   215,   326,   326,   326,   326,   326,   328,
     326,   328,   226,   225,   326,   226,   225,   208,   324,   326,
     225,   326,   326,   226,   226,   226,   226,   225,   226,   326,
     225,   226,   326,   328,   226,   225,   326,   226,   226,   226,
     225,   226,   226,   326,   225,   226,   226,   328,   226,   226,
     225,   226,   226,   225,   225,   226,   226,   226,   326,   208,
     326,   225,   326,   326,   326,   328,   225,   326,   225,   326,
     326,   326,   326,   326,   226,   226,   225,   208,   324,   226,
     226,   225,   226,   328,   225,   326,   226,   226,   226,   226,
     226,   208,   328,   326,   226,   226,   226
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
#line 859 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 860 "basicParse.y"
    { listlen++; ;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 863 "basicParse.y"
    { (yyval.floatnum) = (yyvsp[(2) - (3)].floatnum); ;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 864 "basicParse.y"
    { addOp(OP_ADD); ;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 865 "basicParse.y"
    { addOp(OP_SUB); ;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 866 "basicParse.y"
    { addOp(OP_MUL); ;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 867 "basicParse.y"
    { addOp(OP_MOD); ;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 868 "basicParse.y"
    { addOp(OP_INTDIV); ;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 869 "basicParse.y"
    { addOp(OP_DIV); ;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 870 "basicParse.y"
    { addOp(OP_EX); ;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 871 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYOR); ;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 872 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYAND); ;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 873 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_BINARYNOT); ;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 874 "basicParse.y"
    { addOp(OP_NEGATE); ;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 875 "basicParse.y"
    {addOp(OP_AND); ;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 876 "basicParse.y"
    { addOp(OP_OR); ;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 877 "basicParse.y"
    { addOp(OP_XOR); ;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 878 "basicParse.y"
    { addOp(OP_NOT); ;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 879 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 880 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 881 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 882 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 883 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 884 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 885 "basicParse.y"
    { addOp(OP_EQUAL); ;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 886 "basicParse.y"
    { addOp(OP_NEQUAL); ;}
    break;

  case 303:

/* Line 1455 of yacc.c  */
#line 887 "basicParse.y"
    { addOp(OP_LT); ;}
    break;

  case 304:

/* Line 1455 of yacc.c  */
#line 888 "basicParse.y"
    { addOp(OP_GT); ;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 889 "basicParse.y"
    { addOp(OP_GTE); ;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 890 "basicParse.y"
    { addOp(OP_LTE); ;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 891 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[(1) - (1)].floatnum)); ;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 892 "basicParse.y"
    { addIntOp(OP_PUSHINT, (yyvsp[(1) - (1)].number)); ;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 893 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 894 "basicParse.y"
    { addIntOp(OP_ALEN, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 895 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 896 "basicParse.y"
    { addIntOp(OP_ALENX, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 897 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 898 "basicParse.y"
    { addIntOp(OP_ALENY, (yyvsp[(1) - (5)].number)); ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 899 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 900 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 902 "basicParse.y"
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
#line 909 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 910 "basicParse.y"
    { addOp(OP_INT); ;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 911 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 912 "basicParse.y"
    { addOp(OP_FLOAT); ;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 913 "basicParse.y"
    { addOp(OP_LENGTH); ;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 914 "basicParse.y"
    { addOp(OP_ASC); ;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 915 "basicParse.y"
    { addOp(OP_INSTR); ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 916 "basicParse.y"
    { addOp(OP_INSTR_S); ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 917 "basicParse.y"
    { addOp(OP_INSTR_SC); ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 918 "basicParse.y"
    { addOp(OP_INSTRX); ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 919 "basicParse.y"
    { addOp(OP_INSTRX_S); ;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 920 "basicParse.y"
    { addOp(OP_CEIL); ;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 921 "basicParse.y"
    { addOp(OP_FLOOR); ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 922 "basicParse.y"
    { addOp(OP_SIN); ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 923 "basicParse.y"
    { addOp(OP_COS); ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 924 "basicParse.y"
    { addOp(OP_TAN); ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 925 "basicParse.y"
    { addOp(OP_ASIN); ;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 926 "basicParse.y"
    { addOp(OP_ACOS); ;}
    break;

  case 336:

/* Line 1455 of yacc.c  */
#line 927 "basicParse.y"
    { addOp(OP_ATAN); ;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 928 "basicParse.y"
    { addOp(OP_DEGREES); ;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 929 "basicParse.y"
    { addOp(OP_RADIANS); ;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 930 "basicParse.y"
    { addOp(OP_LOG); ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 931 "basicParse.y"
    { addOp(OP_LOGTEN); ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 932 "basicParse.y"
    { addOp(OP_SQR); ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 933 "basicParse.y"
    { addOp(OP_EXP); ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 934 "basicParse.y"
    { addOp(OP_ABS); ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 935 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 936 "basicParse.y"
    { addOp(OP_RAND); ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 937 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 938 "basicParse.y"
    { addFloatOp(OP_PUSHFLOAT, 3.14159265); ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 939 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 940 "basicParse.y"
    { addIntOp(OP_PUSHINT, 1); ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 941 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 942 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); ;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 943 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 944 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_EOF); ;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 945 "basicParse.y"
    { addOp(OP_EOF); ;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 946 "basicParse.y"
    { addOp(OP_EXISTS); ;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 947 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 948 "basicParse.y"
    { addOp(OP_YEAR); ;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 949 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 950 "basicParse.y"
    { addOp(OP_MONTH); ;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 951 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 952 "basicParse.y"
    { addOp(OP_DAY); ;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 953 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 954 "basicParse.y"
    { addOp(OP_HOUR); ;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 955 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 956 "basicParse.y"
    { addOp(OP_MINUTE); ;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 957 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 958 "basicParse.y"
    { addOp(OP_SECOND); ;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 959 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 960 "basicParse.y"
    { addOp(OP_GRAPHWIDTH); ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 961 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 962 "basicParse.y"
    { addOp(OP_GRAPHHEIGHT); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 963 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 964 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 965 "basicParse.y"
    { addOp(OP_SIZE); ;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 966 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 967 "basicParse.y"
    { addOp(OP_KEY); ;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 968 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 969 "basicParse.y"
    { addOp(OP_MOUSEX); ;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 970 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 971 "basicParse.y"
    { addOp(OP_MOUSEY); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 972 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 973 "basicParse.y"
    { addOp(OP_MOUSEB); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 974 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 975 "basicParse.y"
    { addOp(OP_CLICKX); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 976 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 977 "basicParse.y"
    { addOp(OP_CLICKY); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 978 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 979 "basicParse.y"
    { addOp(OP_CLICKB); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 980 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 981 "basicParse.y"
    { addIntOp(OP_PUSHINT, -1); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 982 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 983 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000000); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 984 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 985 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xf8f8f8); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 986 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 987 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff0000); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 988 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 989 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800000); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 990 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 991 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ff00); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 992 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 993 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008000); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 994 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 995 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x0000ff); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 996 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 997 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x000080); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 998 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 999 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x00ffff); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 1000 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1001 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x008080); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1002 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1003 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff00ff); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1004 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1005 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x800080); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1006 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1007 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xffff00); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1008 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1009 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808000); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1010 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1011 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xff6600); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1012 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1013 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xaa3300); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1014 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1015 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0xa4a4a4); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1016 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1017 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0x808080); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1018 "basicParse.y"
    { addOp(OP_PIXEL); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1019 "basicParse.y"
    { addOp(OP_RGB); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1020 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1021 "basicParse.y"
    { addOp(OP_GETCOLOR); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1022 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITECOLLIDE); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1023 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEX); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1024 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEY); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1025 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEH); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1026 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEW); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1027 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_SPRITEV); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1028 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBROW); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1029 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBINT); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1030 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBFLOAT); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 1031 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1032 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROR); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1033 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 1034 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORLINE); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1035 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp( OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1036 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1037 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETDATA); ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1038 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_PORTIN); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1039 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNT); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 1040 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNT_C); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1041 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_COUNTX); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1042 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OSTYPE); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1043 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_OSTYPE); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1044 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MSEC); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1045 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MSEC); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1046 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_TEXTWIDTH); ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1051 "basicParse.y"
    { listlen = 1; ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1052 "basicParse.y"
    { listlen++; ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1055 "basicParse.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1056 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1057 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1058 "basicParse.y"
    { addOp(OP_CONCAT); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1059 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1060 "basicParse.y"
    { addIntOp(OP_DEREF, (yyvsp[(1) - (4)].number)); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1061 "basicParse.y"
    { addIntOp(OP_DEREF2D, (yyvsp[(1) - (6)].number)); ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1063 "basicParse.y"
    {
		if ((yyvsp[(1) - (1)].number) < 0) {
			return -1;
		} else {
			addIntOp(OP_PUSHVAR, (yyvsp[(1) - (1)].number));
		}
	;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1070 "basicParse.y"
    { addOp(OP_CHR); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1071 "basicParse.y"
    { addOp(OP_STRING); ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 1072 "basicParse.y"
    { addOp(OP_UPPER); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 1073 "basicParse.y"
    { addOp(OP_LOWER); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 1074 "basicParse.y"
    { addOp(OP_MID); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 1075 "basicParse.y"
    { addOp(OP_LEFT); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 1076 "basicParse.y"
    { addOp(OP_RIGHT); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 1077 "basicParse.y"
    { addOp(OP_GETSLICE); ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 1078 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 1079 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 1080 "basicParse.y"
    { addOp(OP_READ); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 1081 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 1082 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 1083 "basicParse.y"
    { addOp(OP_READLINE); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 1084 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 1085 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_CURRENTDIR); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 1086 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DBSTRING); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 1087 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 1088 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERRORMESSAGE); ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 1089 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 1090 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_LASTERROREXTRA); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 1091 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 1092 "basicParse.y"
    { addIntOp(OP_PUSHINT, 0); addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 1093 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETREAD); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 1094 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 1095 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_NETADDRESS); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 1096 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_MD5); ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 1097 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_GETSETTING); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 1098 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 1099 "basicParse.y"
    { addStringOp(OP_PUSHSTRING, ""); addExtendedOp(OP_EXTENDED_0,OP_DIR); ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 1100 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE); ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 1101 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACE_C); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 1102 "basicParse.y"
    { addExtendedOp(OP_EXTENDED_0,OP_REPLACEX); ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 1103 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 1104 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 1105 "basicParse.y"
    {  addStringOp(OP_PUSHSTRING, ""); addIntOp(OP_IMPLODE, (yyvsp[(3) - (4)].number)); ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 1106 "basicParse.y"
    {  addIntOp(OP_IMPLODE, (yyvsp[(3) - (6)].number)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 9497 "basicParse.tab.c"
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
#line 1110 "basicParse.y"


int
yyerror(const char *msg) {
	errorcode = -1;
	if (yytext[0] == '\n') { linenumber--; } // error happened on previous line
	return -1;
}

