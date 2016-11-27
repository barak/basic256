/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 19 "basicParse.y" /* yacc.c:339  */


#ifdef __cplusplus
	extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../WordCodes.h"
#include "../CompileErrors.h"
#include "../ErrorCodes.h"
#include "../Version.h"


#define SYMTABLESIZE 2000
#define IFTABLESIZE 1000
#define PARSEWARNINGTABLESIZE 10

extern int yylex();
extern char *yytext;
int yyerror(const char *);
int errorcode;
extern int column;
extern int linenumber;
extern char *lexingfilename;
extern int numincludes;

int *wordCode = NULL;
unsigned int maxwordoffset = 0;		// size of the current wordCode array
unsigned int wordOffset = 0;		// current location on the WordCode array

unsigned int listlen = 0;

int functionDefSymbol = -1;	// if in a function definition (what is the symbol number) -1 = not in fundef
int functionType;		// function return type (used in return)
int subroutineDefSymbol = -1;	// if in a subroutine definition (what is the symbol number) -1 = not in fundef

#define FUNCTIONTYPEFLOAT 0
#define FUNCTIONTYPESTRING 1


struct label
{
	char *name;
	int offset;
};

char *EMPTYSTR = "";
char *symtable[SYMTABLESIZE];
int labeltable[SYMTABLESIZE];
int numsyms = 0;


// array to hold stack of if statement branch locations
// that need to have final jump location added to them
// the iftable is also used by for, subroutine, and function to insure
// that no if,do,while,... is nested incorrectly
unsigned int iftablesourceline[IFTABLESIZE];
unsigned int iftabletype[IFTABLESIZE];
int iftableid[IFTABLESIZE];			// used to store a sequential number for this if - unique label creation
int iftableincludes[IFTABLESIZE];			// used to store the include depth of the code
int numifs = 0;
int nextifid;

#define IFTABLETYPEIF 1
#define IFTABLETYPEELSE 2
#define IFTABLETYPEDO 3
#define IFTABLETYPEWHILE 4
#define IFTABLETYPEFOR 5
#define IFTABLETYPEFUNCTION 6
#define IFTABLETYPETRY 7
#define IFTABLETYPECATCH 8
#define IFTABLETYPEBEGINCASE 9
#define IFTABLETYPECASE 10


// store the function variables here during a function definition
unsigned int args[100];
unsigned int argstype[100];
int numargs = 0;

#define ARGSTYPEINT 0
#define ARGSTYPESTR 1
#define ARGSTYPEVARREF 2
#define ARGSTYPEVARREFSTR 3

// compiler workings - store in array so that interperter can display all of them
int parsewarningtable[PARSEWARNINGTABLESIZE];
int parsewarningtablelinenumber[PARSEWARNINGTABLESIZE];
int parsewarningtablecolumn[PARSEWARNINGTABLESIZE];
char *parsewarningtablelexingfilename[PARSEWARNINGTABLESIZE];
int numparsewarnings = 0;

int
basicParse(char *);

void checkWordMem(unsigned int addedwords) {
	unsigned int t;
	if (wordOffset + addedwords + 1 >= maxwordoffset) {
		maxwordoffset += maxwordoffset + addedwords + 1024;
		wordCode = realloc(wordCode, maxwordoffset * sizeof(int));
		for (t=wordOffset; t<maxwordoffset; t++) {
			*(wordCode+t) = 0;
		}
	}
}

int bytesToFullWords(unsigned int size) {
	// return how many words will be needed to store "size" bytes
	return((size + sizeof(int) - 1) / sizeof(int));
}

void addOp(int op) {
	checkWordMem(1);
	wordCode[wordOffset] = op;
	wordOffset++;
}

void addIntOp(int op, int data) {
	addOp(op);
	addOp(data);
}


void addFloatOp(int op, double data) {
	addOp(op);
	unsigned int wlen = bytesToFullWords(sizeof(double));
	checkWordMem(wlen);
	double *temp = (double *) (wordCode + wordOffset);
	*temp = data;
	wordOffset += wlen;
}

void addStringOp(int op, char *data) {
	addOp(op);
	unsigned int len = strlen(data) + 1;
	unsigned int wlen = bytesToFullWords(len);
	checkWordMem(wlen);
	strncpy((char *) (wordCode + wordOffset), data, len);
	wordOffset += wlen;
}

void
clearIfTable() {
	int j;
	for (j = 0; j < IFTABLESIZE; j++) {
		iftablesourceline[j] = -1;
		iftabletype[j] = -1;
		iftableid[j] = -1;
		iftableincludes[j] = -1;
	}
	numifs = 0;
	nextifid = 0;
}

int testIfOnTable(int includelevel) {
	// return line number if there is an unfinished while.do.if.else
	// or send back -1
	if (numifs >=1 ) {
		if (iftableincludes[numifs-1]>=includelevel) {
			return iftablesourceline[numifs-1];
		}
	}	
	return -1;
}

int testIfOnTableError(int includelevel) {
	// return Error number if there is an unfinished while.do.if.else
	// or send back 0
	if (numifs >=1 ) {
		if (iftableincludes[numifs-1]>=includelevel) {
			if (iftabletype[numifs-1]==IFTABLETYPEIF) return COMPERR_IFNOEND;
			if (iftabletype[numifs-1]==IFTABLETYPEELSE) return COMPERR_ELSENOEND;
			if (iftabletype[numifs-1]==IFTABLETYPEDO) return COMPERR_DONOEND;
			if (iftabletype[numifs-1]==IFTABLETYPEWHILE) return COMPERR_WHILENOEND;
			if (iftabletype[numifs-1]==IFTABLETYPEFOR) return COMPERR_FORNOEND;
			if (iftabletype[numifs-1]==IFTABLETYPEFUNCTION) return COMPERR_FUNCTIONNOEND;
			if (iftabletype[numifs-1]==IFTABLETYPETRY) return COMPERR_TRYNOEND;
			if (iftabletype[numifs-1]==IFTABLETYPECATCH) return COMPERR_CATCHNOEND;
			if (iftabletype[numifs-1]==IFTABLETYPEBEGINCASE) return COMPERR_BEGINCASENOEND;
			if (iftabletype[numifs-1]==IFTABLETYPECASE) return COMPERR_CASENOEND;
		}
	}	
	return 0;
}

void
clearLabelTable() {
	int j;
	for (j = 0; j < SYMTABLESIZE; j++) {
		labeltable[j] = -1;
	}
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

int newIf(int sourceline, int type) {
	iftablesourceline[numifs] = sourceline;
	iftabletype[numifs] = type;
	iftableid[numifs] = nextifid;
	iftableincludes[numifs] = numincludes;
	nextifid++;
	numifs++;
	return numifs - 1;
}

void newParseWarning(int type) {
	// add warning to warnings table (if not maximum)
	if (numparsewarnings<PARSEWARNINGTABLESIZE) {
		parsewarningtable[numparsewarnings] = type;
		parsewarningtablelinenumber[numparsewarnings] = linenumber;
		parsewarningtablecolumn[numparsewarnings] = column;
		parsewarningtablelexingfilename[numparsewarnings] = strdup(lexingfilename);
		numparsewarnings++;
	} else {
		parsewarningtable[numparsewarnings-1] = COMPWARNING_MAXIMUMWARNINGS;
		parsewarningtablelinenumber[numparsewarnings-1] = 0;
		parsewarningtablecolumn[numparsewarnings-1] = 0;
		free(parsewarningtablelexingfilename[numparsewarnings-1]);
		parsewarningtablelexingfilename[numparsewarnings-1] = strdup("");
	}
}

int getSymbol(char *name) {
	// get a symbol if it exists or create a new one on the symbol table
	int i;
	for (i = 0; i < numsyms; i++) {
		if (symtable[i] && !strcmp(name, symtable[i]))
			return i;
	}
	symtable[numsyms] = strdup(name);
	numsyms++;
	return numsyms - 1;
}

#define INTERNALSYMBOLEXIT 0 //at the end of the loop - all done
#define INTERNALSYMBOLCONTINUE 1 //at the test of the loop
#define INTERNALSYMBOLTOP 2 // at the end of the loop - all done

int getInternalSymbol(int id, int type) {
	// an internal symbol used to jump an if
	char name[32];
	sprintf(name,"___%d_%d", id, type);
	return getSymbol(name);
}

int newWordCode() {
	unsigned int t;
	if (wordCode) {
		free(wordCode);
	}
	maxwordoffset = 1024;
	wordCode = malloc(maxwordoffset * sizeof(int));

	if (wordCode) {
		for (t=0; t<maxwordoffset; t++) {
			*(wordCode+t) = 0;
		}
		wordOffset = 0;
		linenumber = 1;
		addIntOp(OP_CURRLINE, numincludes * 0x1000000 + linenumber);
		return 0; 	// success in creating and filling
	}
	return -1;
}



#ifdef __cplusplus
}
#endif


#line 356 "basicParse.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "basicParse.tab.h".  */
#ifndef YY_YY_BASICPARSE_TAB_H_INCLUDED
# define YY_YY_BASICPARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    B256BEGINCASE = 278,
    B256CASE = 279,
    B256ENDCASE = 280,
    B256WHILE = 281,
    B256ENDWHILE = 282,
    B256DO = 283,
    B256UNTIL = 284,
    B256FOR = 285,
    B256TO = 286,
    B256STEP = 287,
    B256NEXT = 288,
    B256OPEN = 289,
    B256OPENB = 290,
    B256OPENSERIAL = 291,
    B256READ = 292,
    B256WRITE = 293,
    B256CLOSE = 294,
    B256RESET = 295,
    B256GOTO = 296,
    B256GOSUB = 297,
    B256RETURN = 298,
    B256REM = 299,
    B256END = 300,
    B256SETCOLOR = 301,
    B256GTE = 302,
    B256LTE = 303,
    B256NE = 304,
    B256DIM = 305,
    B256REDIM = 306,
    B256NOP = 307,
    B256TOINT = 308,
    B256TOSTRING = 309,
    B256LENGTH = 310,
    B256MID = 311,
    B256LEFT = 312,
    B256RIGHT = 313,
    B256UPPER = 314,
    B256LOWER = 315,
    B256INSTR = 316,
    B256INSTRX = 317,
    B256MIDX = 318,
    B256CEIL = 319,
    B256FLOOR = 320,
    B256RAND = 321,
    B256SIN = 322,
    B256COS = 323,
    B256TAN = 324,
    B256ASIN = 325,
    B256ACOS = 326,
    B256ATAN = 327,
    B256ABS = 328,
    B256PI = 329,
    B256DEGREES = 330,
    B256RADIANS = 331,
    B256LOG = 332,
    B256LOGTEN = 333,
    B256SQR = 334,
    B256EXP = 335,
    B256AND = 336,
    B256OR = 337,
    B256XOR = 338,
    B256NOT = 339,
    B256PAUSE = 340,
    B256SOUND = 341,
    B256ASC = 342,
    B256CHR = 343,
    B256TOFLOAT = 344,
    B256READLINE = 345,
    B256WRITELINE = 346,
    B256BOOLEOF = 347,
    B256MOD = 348,
    B256INTDIV = 349,
    B256YEAR = 350,
    B256MONTH = 351,
    B256DAY = 352,
    B256HOUR = 353,
    B256MINUTE = 354,
    B256SECOND = 355,
    B256TEXT = 356,
    B256FONT = 357,
    B256TEXTWIDTH = 358,
    B256TEXTHEIGHT = 359,
    B256SAY = 360,
    B256SYSTEM = 361,
    B256VOLUME = 362,
    B256GRAPHWIDTH = 363,
    B256GRAPHHEIGHT = 364,
    B256GETSLICE = 365,
    B256PUTSLICE = 366,
    B256IMGLOAD = 367,
    B256SPRITEDIM = 368,
    B256SPRITELOAD = 369,
    B256SPRITESLICE = 370,
    B256SPRITEMOVE = 371,
    B256SPRITEHIDE = 372,
    B256SPRITESHOW = 373,
    B256SPRITEPLACE = 374,
    B256SPRITECOLLIDE = 375,
    B256SPRITEX = 376,
    B256SPRITEY = 377,
    B256SPRITEH = 378,
    B256SPRITEW = 379,
    B256SPRITEV = 380,
    B256SPRITEPOLY = 381,
    B256SPRITER = 382,
    B256SPRITES = 383,
    B256WAVLENGTH = 384,
    B256WAVPAUSE = 385,
    B256WAVPOS = 386,
    B256WAVPLAY = 387,
    B256WAVSTATE = 388,
    B256WAVSEEK = 389,
    B256WAVSTOP = 390,
    B256WAVWAIT = 391,
    B256SIZE = 392,
    B256SEEK = 393,
    B256EXISTS = 394,
    B256BOOLTRUE = 395,
    B256BOOLFALSE = 396,
    B256MOUSEX = 397,
    B256MOUSEY = 398,
    B256MOUSEB = 399,
    B256CLICKCLEAR = 400,
    B256CLICKX = 401,
    B256CLICKY = 402,
    B256CLICKB = 403,
    B256GETCOLOR = 404,
    B256CLEAR = 405,
    B256BLACK = 406,
    B256WHITE = 407,
    B256RED = 408,
    B256DARKRED = 409,
    B256GREEN = 410,
    B256DARKGREEN = 411,
    B256BLUE = 412,
    B256DARKBLUE = 413,
    B256CYAN = 414,
    B256DARKCYAN = 415,
    B256PURPLE = 416,
    B256DARKPURPLE = 417,
    B256YELLOW = 418,
    B256DARKYELLOW = 419,
    B256ORANGE = 420,
    B256DARKORANGE = 421,
    B256GREY = 422,
    B256DARKGREY = 423,
    B256CHANGEDIR = 424,
    B256CURRENTDIR = 425,
    B256DIR = 426,
    B256DECIMAL = 427,
    B256DBOPEN = 428,
    B256DBCLOSE = 429,
    B256DBEXECUTE = 430,
    B256DBOPENSET = 431,
    B256DBCLOSESET = 432,
    B256DBROW = 433,
    B256DBINT = 434,
    B256DBFLOAT = 435,
    B256DBSTRING = 436,
    B256ONERROR = 437,
    B256OFFERROR = 438,
    B256LASTERROR = 439,
    B256LASTERRORMESSAGE = 440,
    B256LASTERRORLINE = 441,
    B256LASTERROREXTRA = 442,
    B256NETLISTEN = 443,
    B256NETCONNECT = 444,
    B256NETREAD = 445,
    B256NETWRITE = 446,
    B256NETCLOSE = 447,
    B256NETDATA = 448,
    B256NETADDRESS = 449,
    B256KILL = 450,
    B256MD5 = 451,
    B256SETSETTING = 452,
    B256GETSETTING = 453,
    B256PORTIN = 454,
    B256PORTOUT = 455,
    B256BINARYOR = 456,
    B256BINARYAND = 457,
    B256BINARYNOT = 458,
    B256IMGSAVE = 459,
    B256REPLACE = 460,
    B256COUNT = 461,
    B256EXPLODE = 462,
    B256REPLACEX = 463,
    B256COUNTX = 464,
    B256EXPLODEX = 465,
    B256IMPLODE = 466,
    B256OSTYPE = 467,
    B256MSEC = 468,
    B256EDITVISIBLE = 469,
    B256GRAPHVISIBLE = 470,
    B256OUTPUTVISIBLE = 471,
    B256EDITSIZE = 472,
    B256OUTPUTSIZE = 473,
    B256FUNCTION = 474,
    B256ENDFUNCTION = 475,
    B256THROWERROR = 476,
    B256SUBROUTINE = 477,
    B256ENDSUBROUTINE = 478,
    B256CALL = 479,
    B256GLOBAL = 480,
    B256READBYTE = 481,
    B256WRITEBYTE = 482,
    B256ADD1 = 483,
    B256SUB1 = 484,
    B256ADDEQUAL = 485,
    B256SUBEQUAL = 486,
    B256MULEQUAL = 487,
    B256DIVEQUAL = 488,
    B256REF = 489,
    B256FREEDB = 490,
    B256FREEFILE = 491,
    B256FREENET = 492,
    B256FREEDBSET = 493,
    B256DBNULL = 494,
    B256ARC = 495,
    B256CHORD = 496,
    B256PIE = 497,
    B256PENWIDTH = 498,
    B256GETPENWIDTH = 499,
    B256GETBRUSHCOLOR = 500,
    B256VERSION = 501,
    B256ALERT = 502,
    B256CONFIRM = 503,
    B256PROMPT = 504,
    B256FROMBINARY = 505,
    B256FROMHEX = 506,
    B256FROMOCTAL = 507,
    B256FROMRADIX = 508,
    B256TOBINARY = 509,
    B256TOHEX = 510,
    B256TOOCTAL = 511,
    B256TORADIX = 512,
    B256DEBUGINFO = 513,
    B256CONTINUEDO = 514,
    B256CONTINUEFOR = 515,
    B256CONTINUEWHILE = 516,
    B256EXITDO = 517,
    B256EXITFOR = 518,
    B256EXITWHILE = 519,
    B256PRINTERPAGE = 520,
    B256PRINTERON = 521,
    B256PRINTEROFF = 522,
    B256PRINTERCANCEL = 523,
    B256TRY = 524,
    B256CATCH = 525,
    B256ENDTRY = 526,
    B256LET = 527,
    B256ERROR_NONE = 528,
    B256ERROR_FOR1 = 529,
    B256ERROR_FOR2 = 530,
    B256ERROR_FILENUMBER = 531,
    B256ERROR_FILEOPEN = 532,
    B256ERROR_FILENOTOPEN = 533,
    B256ERROR_FILEWRITE = 534,
    B256ERROR_FILERESET = 535,
    B256ERROR_ARRAYSIZELARGE = 536,
    B256ERROR_ARRAYSIZESMALL = 537,
    B256ERROR_NOSUCHVARIABLE = 538,
    B256ERROR_ARRAYINDEX = 539,
    B256ERROR_STRNEGLEN = 540,
    B256ERROR_STRSTART = 541,
    B256ERROR_NONNUMERIC = 542,
    B256ERROR_RGB = 543,
    B256ERROR_PUTBITFORMAT = 544,
    B256ERROR_POLYARRAY = 545,
    B256ERROR_POLYPOINTS = 546,
    B256ERROR_IMAGEFILE = 547,
    B256ERROR_SPRITENUMBER = 548,
    B256ERROR_SPRITENA = 549,
    B256ERROR_SPRITESLICE = 550,
    B256ERROR_FOLDER = 551,
    B256ERROR_INFINITY = 552,
    B256ERROR_DBOPEN = 553,
    B256ERROR_DBQUERY = 554,
    B256ERROR_DBNOTOPEN = 555,
    B256ERROR_DBCOLNO = 556,
    B256ERROR_DBNOTSET = 557,
    B256ERROR_NETSOCK = 558,
    B256ERROR_NETHOST = 559,
    B256ERROR_NETCONN = 560,
    B256ERROR_NETREAD = 561,
    B256ERROR_NETNONE = 562,
    B256ERROR_NETWRITE = 563,
    B256ERROR_NETSOCKOPT = 564,
    B256ERROR_NETBIND = 565,
    B256ERROR_NETACCEPT = 566,
    B256ERROR_NETSOCKNUMBER = 567,
    B256ERROR_PERMISSION = 568,
    B256ERROR_IMAGESAVETYPE = 569,
    B256ERROR_DIVZERO = 570,
    B256ERROR_BYREF = 571,
    B256ERROR_BYREFTYPE = 572,
    B256ERROR_FREEFILE = 573,
    B256ERROR_FREENET = 574,
    B256ERROR_FREEDB = 575,
    B256ERROR_DBCONNNUMBER = 576,
    B256ERROR_FREEDBSET = 577,
    B256ERROR_DBSETNUMBER = 578,
    B256ERROR_DBNOTSETROW = 579,
    B256ERROR_PENWIDTH = 580,
    B256ERROR_COLORNUMBER = 581,
    B256ERROR_ARRAYINDEXMISSING = 582,
    B256ERROR_IMAGESCALE = 583,
    B256ERROR_FONTSIZE = 584,
    B256ERROR_FONTWEIGHT = 585,
    B256ERROR_RADIXSTRING = 586,
    B256ERROR_RADIX = 587,
    B256ERROR_LOGRANGE = 588,
    B256ERROR_STRINGMAXLEN = 589,
    B256ERROR_NOTANUMBER = 590,
    B256ERROR_PRINTERNOTON = 591,
    B256ERROR_PRINTERNOTOFF = 592,
    B256ERROR_PRINTEROPEN = 593,
    B256ERROR_WAVFILEFORMAT = 594,
    B256ERROR_WAVNOTOPEN = 595,
    B256ERROR_NOTIMPLEMENTED = 596,
    B256WARNING_TYPECONV = 597,
    B256WARNING_WAVNODURATION = 598,
    B256WARNING_WAVNOTSEEKABLE = 599,
    B256REGEXMINIMAL = 600,
    B256LINENUM = 601,
    B256INTEGER = 602,
    B256FLOAT = 603,
    B256STRING = 604,
    B256HEXCONST = 605,
    B256BINCONST = 606,
    B256OCTCONST = 607,
    B256VARIABLE = 608,
    B256STRINGVAR = 609,
    B256NEWVAR = 610,
    B256COLOR = 611,
    B256LABEL = 612,
    B256UMINUS = 613
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 381 "basicParse.y" /* yacc.c:355  */

	int number;
	double floatnum;
	char *string;

#line 761 "basicParse.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BASICPARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 776 "basicParse.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  617
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   40997

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  378
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  137
/* YYNRULES -- Number of rules.  */
#define YYNRULES  790
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1797

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   613

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     367,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     368,   369,   363,   362,   370,   361,     2,   364,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   371,   374,
     358,   360,   359,   377,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   372,     2,   373,   366,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   375,     2,   376,     2,     2,     2,     2,
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
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   365
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   416,   416,   417,   421,   428,   429,   430,   431,   432,
     433,   434,   435,   438,   448,   452,   456,   460,   467,   468,
     473,   474,   478,   479,   483,   484,   485,   486,   487,   488,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   517,   518,
     519,   520,   521,   522,   523,   524,   525,   526,   527,   528,
     529,   530,   531,   532,   533,   534,   535,   536,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   588,
     589,   590,   591,   596,   602,   626,   636,   664,   674,   720,
     751,   773,   796,   820,   831,   842,   851,   865,   875,   894,
     905,   912,   921,   922,   923,   924,   925,   926,   927,   928,
     931,   935,   939,   942,   945,   949,   953,   956,   961,   965,
     969,   972,   975,   979,   983,   986,   991,   997,  1002,  1005,
    1011,  1017,  1020,  1026,  1031,  1037,  1040,  1050,  1053,  1064,
    1069,  1073,  1076,  1082,  1083,  1084,  1085,  1086,  1087,  1088,
    1089,  1090,  1091,  1092,  1096,  1099,  1107,  1115,  1125,  1135,
    1145,  1155,  1158,  1165,  1172,  1182,  1193,  1203,  1214,  1219,
    1223,  1226,  1229,  1236,  1237,  1238,  1239,  1240,  1244,  1247,
    1253,  1259,  1264,  1270,  1275,  1281,  1288,  1291,  1299,  1307,
    1316,  1335,  1344,  1353,  1356,  1359,  1365,  1378,  1390,  1402,
    1417,  1434,  1441,  1448,  1452,  1455,  1460,  1464,  1469,  1473,
    1479,  1482,  1487,  1490,  1497,  1500,  1505,  1508,  1514,  1517,
    1523,  1526,  1532,  1535,  1541,  1544,  1547,  1550,  1556,  1559,
    1564,  1570,  1576,  1582,  1586,  1590,  1597,  1601,  1605,  1610,
    1615,  1619,  1623,  1628,  1633,  1637,  1641,  1646,  1653,  1659,
    1663,  1667,  1673,  1677,  1681,  1689,  1697,  1704,  1711,  1717,
    1723,  1728,  1733,  1737,  1741,  1744,  1750,  1755,  1758,  1764,
    1769,  1772,  1778,  1783,  1786,  1791,  1795,  1799,  1804,  1808,
    1812,  1817,  1822,  1825,  1830,  1834,  1839,  1843,  1848,  1853,
    1858,  1863,  1868,  1873,  1878,  1883,  1891,  1895,  1898,  1904,
    1910,  1914,  1919,  1925,  1931,  1937,  1940,  1943,  1946,  1952,
    1960,  1968,  1974,  1980,  1984,  1991,  1997,  2000,  2006,  2009,
    2015,  2019,  2023,  2028,  2036,  2041,  2046,  2050,  2054,  2057,
    2063,  2068,  2073,  2077,  2081,  2084,  2090,  2096,  2102,  2108,
    2114,  2119,  2122,  2128,  2132,  2136,  2142,  2147,  2150,  2156,
    2163,  2168,  2173,  2176,  2182,  2187,  2192,  2196,  2199,  2205,
    2210,  2213,  2219,  2224,  2229,  2232,  2238,  2243,  2246,  2252,
    2256,  2260,  2265,  2271,  2274,  2280,  2283,  2289,  2293,  2296,
    2302,  2308,  2314,  2320,  2335,  2341,  2347,  2363,  2379,  2395,
    2411,  2427,  2443,  2449,  2455,  2461,  2467,  2504,  2544,  2579,
    2607,  2631,  2645,  2649,  2653,  2654,  2658,  2659,  2663,  2664,
    2668,  2669,  2678,  2679,  2682,  2685,  2688,  2691,  2694,  2697,
    2698,  2699,  2700,  2701,  2702,  2703,  2704,  2705,  2706,  2707,
    2708,  2709,  2710,  2711,  2712,  2713,  2714,  2715,  2716,  2717,
    2718,  2719,  2720,  2721,  2722,  2723,  2724,  2725,  2726,  2727,
    2738,  2749,  2759,  2769,  2770,  2781,  2792,  2802,  2812,  2816,
    2820,  2827,  2834,  2841,  2848,  2855,  2856,  2857,  2858,  2859,
    2864,  2868,  2869,  2873,  2874,  2875,  2876,  2877,  2878,  2879,
    2880,  2881,  2882,  2883,  2884,  2885,  2886,  2887,  2888,  2889,
    2890,  2891,  2892,  2893,  2897,  2901,  2902,  2903,  2904,  2905,
    2906,  2907,  2908,  2909,  2910,  2911,  2912,  2913,  2914,  2915,
    2916,  2917,  2918,  2919,  2920,  2921,  2928,  2935,  2942,  2949,
    2956,  2963,  2970,  2977,  2984,  2991,  2998,  3005,  3012,  3019,
    3026,  3033,  3040,  3047,  3054,  3055,  3059,  3062,  3063,  3064,
    3065,  3066,  3067,  3068,  3069,  3070,  3071,  3072,  3073,  3078,
    3083,  3087,  3090,  3096,  3100,  3102,  3108,  3112,  3114,  3120,
    3124,  3126,  3132,  3136,  3138,  3144,  3148,  3150,  3156,  3160,
    3162,  3163,  3164,  3165,  3166,  3167,  3168,  3172,  3173,  3174,
    3175,  3176,  3177,  3178,  3179,  3180,  3181,  3182,  3183,  3184,
    3188,  3192,  3193,  3194,  3195,  3196,  3200,  3203,  3207,  3211,
    3215,  3218,  3223,  3228,  3233,  3234,  3235,  3237,  3238,  3239,
    3240,  3241,  3242,  3243,  3244,  3245,  3246,  3247,  3248,  3249,
    3250,  3251,  3252,  3253,  3254,  3255,  3256,  3257,  3258,  3259,
    3260,  3261,  3262,  3263,  3264,  3265,  3266,  3267,  3268,  3269,
    3270,  3271,  3272,  3273,  3274,  3275,  3276,  3277,  3278,  3279,
    3280,  3281,  3282,  3283,  3284,  3285,  3286,  3287,  3288,  3289,
    3290,  3291,  3292,  3293,  3294,  3295,  3296,  3297,  3298,  3299,
    3300,  3301,  3302,  3303,  3304,  3305,  3307,  3308,  3309,  3318,
    3319,  3320,  3321,  3322,  3323,  3324,  3325,  3326,  3327,  3334,
    3335,  3336,  3337,  3338,  3339,  3340,  3341,  3342,  3343,  3344,
    3345,  3346,  3347,  3348,  3349,  3350,  3351,  3352,  3358,  3362,
    3364,  3370,  3374,  3376,  3377,  3378,  3379,  3380,  3381,  3382,
    3383,  3384,  3385,  3386,  3387,  3388,  3389,  3390,  3394,  3395,
    3396,  3400,  3401,  3405,  3406,  3409,  3411,  3415,  3419,  3423,
    3426
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "B256PRINT", "B256INPUT", "B256KEY",
  "B256PIXEL", "B256RGB", "B256PLOT", "B256CIRCLE", "B256RECT", "B256POLY",
  "B256STAMP", "B256LINE", "B256FASTGRAPHICS", "B256GRAPHSIZE",
  "B256REFRESH", "B256CLS", "B256CLG", "B256IF", "B256THEN", "B256ELSE",
  "B256ENDIF", "B256BEGINCASE", "B256CASE", "B256ENDCASE", "B256WHILE",
  "B256ENDWHILE", "B256DO", "B256UNTIL", "B256FOR", "B256TO", "B256STEP",
  "B256NEXT", "B256OPEN", "B256OPENB", "B256OPENSERIAL", "B256READ",
  "B256WRITE", "B256CLOSE", "B256RESET", "B256GOTO", "B256GOSUB",
  "B256RETURN", "B256REM", "B256END", "B256SETCOLOR", "B256GTE", "B256LTE",
  "B256NE", "B256DIM", "B256REDIM", "B256NOP", "B256TOINT", "B256TOSTRING",
  "B256LENGTH", "B256MID", "B256LEFT", "B256RIGHT", "B256UPPER",
  "B256LOWER", "B256INSTR", "B256INSTRX", "B256MIDX", "B256CEIL",
  "B256FLOOR", "B256RAND", "B256SIN", "B256COS", "B256TAN", "B256ASIN",
  "B256ACOS", "B256ATAN", "B256ABS", "B256PI", "B256DEGREES",
  "B256RADIANS", "B256LOG", "B256LOGTEN", "B256SQR", "B256EXP", "B256AND",
  "B256OR", "B256XOR", "B256NOT", "B256PAUSE", "B256SOUND", "B256ASC",
  "B256CHR", "B256TOFLOAT", "B256READLINE", "B256WRITELINE", "B256BOOLEOF",
  "B256MOD", "B256INTDIV", "B256YEAR", "B256MONTH", "B256DAY", "B256HOUR",
  "B256MINUTE", "B256SECOND", "B256TEXT", "B256FONT", "B256TEXTWIDTH",
  "B256TEXTHEIGHT", "B256SAY", "B256SYSTEM", "B256VOLUME",
  "B256GRAPHWIDTH", "B256GRAPHHEIGHT", "B256GETSLICE", "B256PUTSLICE",
  "B256IMGLOAD", "B256SPRITEDIM", "B256SPRITELOAD", "B256SPRITESLICE",
  "B256SPRITEMOVE", "B256SPRITEHIDE", "B256SPRITESHOW", "B256SPRITEPLACE",
  "B256SPRITECOLLIDE", "B256SPRITEX", "B256SPRITEY", "B256SPRITEH",
  "B256SPRITEW", "B256SPRITEV", "B256SPRITEPOLY", "B256SPRITER",
  "B256SPRITES", "B256WAVLENGTH", "B256WAVPAUSE", "B256WAVPOS",
  "B256WAVPLAY", "B256WAVSTATE", "B256WAVSEEK", "B256WAVSTOP",
  "B256WAVWAIT", "B256SIZE", "B256SEEK", "B256EXISTS", "B256BOOLTRUE",
  "B256BOOLFALSE", "B256MOUSEX", "B256MOUSEY", "B256MOUSEB",
  "B256CLICKCLEAR", "B256CLICKX", "B256CLICKY", "B256CLICKB",
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
  "B256EDITSIZE", "B256OUTPUTSIZE", "B256FUNCTION", "B256ENDFUNCTION",
  "B256THROWERROR", "B256SUBROUTINE", "B256ENDSUBROUTINE", "B256CALL",
  "B256GLOBAL", "B256READBYTE", "B256WRITEBYTE", "B256ADD1", "B256SUB1",
  "B256ADDEQUAL", "B256SUBEQUAL", "B256MULEQUAL", "B256DIVEQUAL",
  "B256REF", "B256FREEDB", "B256FREEFILE", "B256FREENET", "B256FREEDBSET",
  "B256DBNULL", "B256ARC", "B256CHORD", "B256PIE", "B256PENWIDTH",
  "B256GETPENWIDTH", "B256GETBRUSHCOLOR", "B256VERSION", "B256ALERT",
  "B256CONFIRM", "B256PROMPT", "B256FROMBINARY", "B256FROMHEX",
  "B256FROMOCTAL", "B256FROMRADIX", "B256TOBINARY", "B256TOHEX",
  "B256TOOCTAL", "B256TORADIX", "B256DEBUGINFO", "B256CONTINUEDO",
  "B256CONTINUEFOR", "B256CONTINUEWHILE", "B256EXITDO", "B256EXITFOR",
  "B256EXITWHILE", "B256PRINTERPAGE", "B256PRINTERON", "B256PRINTEROFF",
  "B256PRINTERCANCEL", "B256TRY", "B256CATCH", "B256ENDTRY", "B256LET",
  "B256ERROR_NONE", "B256ERROR_FOR1", "B256ERROR_FOR2",
  "B256ERROR_FILENUMBER", "B256ERROR_FILEOPEN", "B256ERROR_FILENOTOPEN",
  "B256ERROR_FILEWRITE", "B256ERROR_FILERESET", "B256ERROR_ARRAYSIZELARGE",
  "B256ERROR_ARRAYSIZESMALL", "B256ERROR_NOSUCHVARIABLE",
  "B256ERROR_ARRAYINDEX", "B256ERROR_STRNEGLEN", "B256ERROR_STRSTART",
  "B256ERROR_NONNUMERIC", "B256ERROR_RGB", "B256ERROR_PUTBITFORMAT",
  "B256ERROR_POLYARRAY", "B256ERROR_POLYPOINTS", "B256ERROR_IMAGEFILE",
  "B256ERROR_SPRITENUMBER", "B256ERROR_SPRITENA", "B256ERROR_SPRITESLICE",
  "B256ERROR_FOLDER", "B256ERROR_INFINITY", "B256ERROR_DBOPEN",
  "B256ERROR_DBQUERY", "B256ERROR_DBNOTOPEN", "B256ERROR_DBCOLNO",
  "B256ERROR_DBNOTSET", "B256ERROR_NETSOCK", "B256ERROR_NETHOST",
  "B256ERROR_NETCONN", "B256ERROR_NETREAD", "B256ERROR_NETNONE",
  "B256ERROR_NETWRITE", "B256ERROR_NETSOCKOPT", "B256ERROR_NETBIND",
  "B256ERROR_NETACCEPT", "B256ERROR_NETSOCKNUMBER", "B256ERROR_PERMISSION",
  "B256ERROR_IMAGESAVETYPE", "B256ERROR_DIVZERO", "B256ERROR_BYREF",
  "B256ERROR_BYREFTYPE", "B256ERROR_FREEFILE", "B256ERROR_FREENET",
  "B256ERROR_FREEDB", "B256ERROR_DBCONNNUMBER", "B256ERROR_FREEDBSET",
  "B256ERROR_DBSETNUMBER", "B256ERROR_DBNOTSETROW", "B256ERROR_PENWIDTH",
  "B256ERROR_COLORNUMBER", "B256ERROR_ARRAYINDEXMISSING",
  "B256ERROR_IMAGESCALE", "B256ERROR_FONTSIZE", "B256ERROR_FONTWEIGHT",
  "B256ERROR_RADIXSTRING", "B256ERROR_RADIX", "B256ERROR_LOGRANGE",
  "B256ERROR_STRINGMAXLEN", "B256ERROR_NOTANUMBER",
  "B256ERROR_PRINTERNOTON", "B256ERROR_PRINTERNOTOFF",
  "B256ERROR_PRINTEROPEN", "B256ERROR_WAVFILEFORMAT",
  "B256ERROR_WAVNOTOPEN", "B256ERROR_NOTIMPLEMENTED",
  "B256WARNING_TYPECONV", "B256WARNING_WAVNODURATION",
  "B256WARNING_WAVNOTSEEKABLE", "B256REGEXMINIMAL", "B256LINENUM",
  "B256INTEGER", "B256FLOAT", "B256STRING", "B256HEXCONST", "B256BINCONST",
  "B256OCTCONST", "B256VARIABLE", "B256STRINGVAR", "B256NEWVAR",
  "B256COLOR", "B256LABEL", "'<'", "'>'", "'='", "'-'", "'+'", "'*'",
  "'/'", "B256UMINUS", "'^'", "'\\n'", "'('", "')'", "','", "':'", "'['",
  "']'", "';'", "'{'", "'}'", "'?'", "$accept", "program",
  "programnewline", "programline", "label", "functionvariable",
  "functionvariablelist", "functionvariables", "compoundstmt", "statement",
  "begincasestmt", "caseexpr", "casestmt", "catchstmt", "dostmt",
  "elsestmt", "endcasestmt", "endifstmt", "endtrystmt", "endwhilestmt",
  "ifstmt", "ifthenstmt", "ifthenelsestmt", "ifthenelse", "trystmt",
  "until", "untilstmt", "while", "whilestmt", "letstmt", "dimstmt",
  "redimstmt", "pausestmt", "throwerrorstmt", "clearstmt",
  "fastgraphicsstmt", "graphsizestmt", "refreshstmt", "endstmt",
  "strarrayassign", "arrayassignerrors", "arrayassign", "numassignerrors",
  "numassign", "stringassign", "forstmt", "nextstmt", "gotostmt",
  "gosubstmt", "callstmt", "offerrorstmt", "onerrorstmt", "returnstmt",
  "colorstmt", "soundstmt", "plotstmt", "linestmt", "circlestmt",
  "arcstmt", "chordstmt", "piestmt", "rectstmt", "textstmt", "fontstmt",
  "saystmt", "systemstmt", "volumestmt", "polystmt", "stampstmt",
  "openstmt", "writestmt", "writelinestmt", "writebytestmt", "closestmt",
  "resetstmt", "seekstmt", "inputstmt", "printstmt", "wavpausestmt",
  "wavplaystmt", "wavseekstmt", "wavstopstmt", "wavwaitstmt",
  "putslicestmt", "imgloadstmt", "spritedimstmt", "spriteloadstmt",
  "spriteslicestmt", "spritepolystmt", "spriteplacestmt", "spritemovestmt",
  "spritehidestmt", "spriteshowstmt", "clickclearstmt", "changedirstmt",
  "dbopenstmt", "dbclosestmt", "dbexecutestmt", "dbopensetstmt",
  "dbclosesetstmt", "netlistenstmt", "netconnectstmt", "netwritestmt",
  "netclosestmt", "killstmt", "setsettingstmt", "portoutstmt",
  "imgsavestmt", "editvisiblestmt", "graphvisiblestmt",
  "outputvisiblestmt", "globalstmt", "penwidthstmt", "alertstmt",
  "continuedostmt", "continueforstmt", "continuewhilestmt", "exitdostmt",
  "exitforstmt", "exitwhilestmt", "printercancelstmt", "printeroffstmt",
  "printeronstmt", "printerpagestmt", "functionstmt", "subroutinestmt",
  "endfunctionstmt", "endsubroutinestmt", "regexminimalstmt",
  "immediatestrlist", "immediatelist", "floatlist", "stringlist",
  "explist", "expr", "floatexpr", "stringexpr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
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
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,    60,    62,
      61,    45,    43,    42,    47,   613,    94,    10,    40,    41,
      44,    58,    91,    93,    59,   123,   125,    63
};
# endif

#define YYPACT_NINF -800

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-800)))

#define YYTABLE_NINF -346

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const int yypact[] =
{
    1915, 14183, 14533, 14883, 15233, 15583,  -292, 15933, 16283,  -800,
   16633,  -800,  -800,  -800, 14183,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -322,  -308, 16983, 17333, 17683, 18033,
   18383, 18733,  -290,  -253, 14183,  -800,  -800, 19083,  -336,  -202,
   14183,  4733, 19433, 19783, 20133, 14183, 14183, 14183, 20483, 20833,
   14183, 21183, 21533, 21883, 14183, 14183, 22233, 22583,  -800, 14183,
   14183,  -800,  -800, 22933,  -800, 14183, 23283, 23633, 23983, 24333,
   24683,  -249,  -800, 25033, 25383, 25733, 26083, 14183, 26433, 26783,
   27133, 14183, 14183, 14183,  -197,  -800, 14183,  -244,  -800,  -240,
    -135, 27483, 27833, 28183, 28533, 14183, 14183,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -191, 14183,   -63,  -222,  -800,   117,  -219, 28894,    79,  -218,
    -800, 14183,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
   29164,  -800,  -800, 29164,  -800, 14183,  -800, 14183,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -235,  -204,  -182,  -175,  -173,  -146,  -138,  -137,  -136,  -134,
    -133,  -130,  -129,  -127,  -126,  -125,  -800,  -122,  -121,  -120,
    -118,  -117,  -116,  -115,  -800,  -114,  -113,  -112,  -111,  -110,
    -109, 14183,  -108,  -107,  -106,  -105,  -104,  -800,  -800,  -800,
    -800,  -800,  -800,  -100,  -800,  -800,  -800,   -99,   -95,   -94,
     -93,   -85,   -83,   -80,   -79,   -73,  -800,  -800,  -800,   -66,
     -65,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,   -64,
     -60,   -55,   -52,   -51,   -49,  -800,   -48,  -800,   -47,   -46,
     -43,   -42,   -41,   -40,   -32, 14183,   -31,   -30,   -23,   -22,
     -21,  -800,  -800,   -20,   -35,    -4,   -18,  -800,  -800,  -800,
     -15,   -14,  -800,  -800,  -800,    -8,    -6,    -3,    -2,     0,
       1,     2,     3,     4,     5,     6,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -214,  -334, 14183, 14183,   -76, 40583,
     176,    16,   173, 40583,   924, 14183, 32234,   176, 14183, 32275,
   14183, 32323,  -800,    10, 14183,  -800, 14183, 32360, 14183, 32400,
   14183, 32448,    78,     7,  -800, 14183, 32489,   176, 14183, 32526,
     176, 14183, 32574, 14183, 32614,   176,  7883, 40583,  8233, 40583,
    -800,  -800, 40583,   176, 14183, 32655,  6483,  6833,  7183,  7533,
   40583,    43, 14183,  -800, 32696, 14183, 32744,   176, 14183, 32781,
   14183,   944,  -800,   176, 40583, 14183, 32821, 14183, 32869, 40583,
   14183, 32910, 14183, 32947, 14183, 32995, 40583, 40583, 14183, 33035,
   14183, 33076,   176, 40583, 14183, 33117,   176, 14183, 33165,   176,
    8583, 40583, 14183, 33202,   176, 14183, 33242,   176,  8933, 33290,
    -800, 14183, 33331, 14183, 33368,   953, 14183, 33416,   176,  9283,
   40583,   176, 14183,     8, 14183, 33456, 14183,   984, 40583, 40583,
   40583,     9,     9, 40583,     9,    11,    12,  -800,  -800,    15,
    -800, 14183, 33497, 14183, 33538, 14183, 33586, 14183, 33623, 40583,
     176,  -800,  -800,  -800,  -800, 40583,  -800,  -800, 14183, 14183,
   14183, 14183,  4033, 14183, 14183,  4383, 14183,  -800,  -800,  1915,
    -800,   189,  -800, 29164, 40583,   334,  -800, 40583, 40583, 14183,
   14183,  9633, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183,
   14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183,
   14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183,    20, 14183,
   14183, 14183,  9983, 10333, 14183, 14183, 14183, 14183, 14183, 14183,
   14183, 14183, 14183, 14183, 10683, 14183,    13, 11033, 11383, 14183,
   14183, 14183,    24,    25, 11733, 12083,    26, 14183, 14183, 14183,
      20, 14183, 14183, 14183, 14183,  -179, 12433,    17,    27,  -169,
   12783, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183,
   14183, 14183, 14183,  -800,  -800, 13133,  3333, 13483,  3683,    20,
    1785,   938,  -800, 14183, 14183, 14183, 14183, 14183, 14183, 14183,
   14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183,
   14183, 14183, 14183, 14183, 14183, 14183, 14183, 14183,  3333,  3683,
    -163, 29787, 14183, 29828, 14183, 29876, 14183,    28,   -12, 33663,
   29891, 14183, 29916, 14183, 29953, 14183,  -800, 14183, 30001, 14183,
   30026, 14183, 30042, 14183, 30082, 14183,  -800,  -800, 30123, 14183,
   14183, 14183, 40583, 14183, 14183, 40583, 14183, 14183, 40583, 14183,
   14183, 40583, 30215, 14183, 30252, 14183, 30300, 14183,   -27, 14183,
   30381, 14183, 30422, 14183, 30447, 14183, 30470, 14183, 30507, 14183,
   30547, 14183, 30595,  -269, 30636, 14183, 30673, 14183,  -800, 30721,
   14183, 30802, 14183,  -800, 30843, 14183, 30891, 14183, 30928,   140,
   14183, 14183, 30968, 14183,  -800,    30,  1785,   938, 14183, 31016,
   14183,   229, 14183,  -177,  -800,  -800,  -800, 13833,  -155,  -135,
   31057, 14183, 31094, 14183, 31142, 14183, 31182, 14183, 40583,   176,
   40583,   176, 40583,   176, 40583,   176,    33,    34, 14183,  -800,
    -800, 40583,   176,   787,  -800,    35,    36, 14183,  -800,  -800,
     812,  -800,  -800,  -800,  -800, 33711, 33752,  -800,  2817,    29,
      37,    38,    39,    42,    46,    44,    48,    51,    52,    53,
   37157, 37205, 37245, 37286, 37327, 37375, 37414, 37452, 37500, 37541,
   37580, 37622, 37670, 37707, 37746,    49, 37760,    55,  -800, 37797,
    -800, 37836,  1088, 33789, 33837, 37873, 37926, 37963, 38002, 38050,
   38092, 38129,  -800, 38172,  1117,  -800,  -800,  1444,  -800, 31223,
   31264,  1469, 31312,  1515, 31352,  1544,  -800,  -800,  -800, 38220,
    -800, 38258,  -800,  1619,    57, 38295,    61,   997,    62,  1010,
    -241,  -161,  -800, 38309, 14183, 14183,    59,    64,  -800, 38346,
   31389,  1633,   343,   399,  1707,  1838,  1850,  1024, 38386, 38424,
   38475, 33877, 38512,  -800,    65,    75,    19,  -265,   840,  -800,
      66,    31,  -224,  1329,  -800,  -800,    -5,    -5,    -5, 29055,
   40631, 40622,    20,    20,   -70,   -70,    -5,    -5,    -5,   -82,
     -82,  -800,    20,    20,  -800,    14,    14,    14,    14,    14,
      14,   -82,  -800,  2112,  2348,    71,    77, 14183, 40583, 14183,
   33918, 14183, 33959,  -800,  -800, 14183, 14183, 34007, 14183, 34044,
   14183, 40583,   218, 14183,   176, 14183,   176, 14183,  1181, 14183,
     176, 14183, 34084, 31437,  2417, 31478,  2543, 31518,  2559, 31559,
    2609, 14183, 40583, 14183,   176, 14183, 34132, 14183, 34173, 14183,
   34210, 14183, 34258, 14183,   176, 14183, 34298, 14183, 34339, 14183,
   34380,  -238,  -800,  -800, 14183, 40583, 14183,   176, 14183,   176,
   14183, 34428,   176, 14183, 40583, 14183, 40583, 14183, 14183,  1186,
   40583, 14183,   176, 14183,    80, 14183, 40583, 14183,   176,  -800,
      82,  -800,    83,    92,    94,  -800, 14183, 40583, 14183, 34465,
   14183, 34505, 14183, 34553, 14183, 14183,    89,  1228, 14183,   -50,
   14183, 14183, 14183,  -225, 14183, 14183,  -800,  -800,  -800,  -800,
   14183, 14183, 14183,  -800,  -800, 14183, 14183, 14183,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800, 14183,
   14183,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800, 14183,  -800, 14183,  -800,  -800, 14183,  -800,  -800,
   14183,  -800,  -800,  -800,  -800, 14183,  -800, 14183, 14183, 14183,
   14183,  -800, 14183,  -800, 14183,  -800,  2775,  2791,  -800,  -800,
    -800,  -800, 14183,  -800,  -800, 14183,  -800, 14183,  -800,  -800,
    -800, 14183,  -800,  -800,  -800, 14183,  -800,  -800, 14183,    32,
      98,  -800, 14183,   -28,  -800,   107,   108,  -800, 14183,  -800,
   14183,    95, 14183,   103, 14183, 14183, 38552, 34594, 14183, 34631,
   14183,  -800, 34679,  5083, 34719, 14183, 38600, 14183,  1923,  2031,
     555, 14183,  2053, 31607, 14183, 14183, 14183,  -800, 14183, 14183,
    -800, 14183, 14183,  -800, 14183, 14183,  -800, 38641,  2143, 34760,
   14183, 34801, 14183, 34849, 14183, 34886, 14183,  2240, 34926, 14183,
   34974, 14183, 35015, 14183,   114,   116, 38678,  2285,  2301, 35052,
    2315, 14183, 38726, 38766,  1300, 38807, 14183,  2396,   106, 14183,
   38848,  2580,  -800,  -800,  -800,  -800, 38896, 35100, 14183, 35140,
   14183, 35181, 14183,   118,   119,  -800, 14183, 29170,  -800,  -800,
   14183, 14183, 14183, 14183, 14183,   121,   123, 29237, 14183, 14183,
   38935, 35222, 35270, 38973, 39021,  -157,   578,   649, 35307, 39062,
   39101, 31644,  2646, 31684,  2651, 31732,  2682,   125,   126,   693,
    1356,  2690,  2704,  2749, 14183,  -800, 14183,  -800, 31773,  2754,
   39143,  2833, 39191, 39228,  -800,  -800,  -800, 29361,  -800,  -800,
    -800,  -800, 29403, 29419, 29444,  2904,  2988,  -800, 14183, 40583,
   14183, 35347,  5433,    58,  -800, 35395, 14183, 35436,  -800,   258,
    -800,  -800,  -800, 14183, 35473,  -800,  -800, 14183, 40583, 39267,
   29467, 39281, 29492, 39318, 29530, 39357, 29569,  -800,  -800, 14183,
   40583,   176, 14183, 40583, 14183,  1391, 14183, 35521,   176,  -800,
   14183, 35561, 14183, 35602, 14183, 35643,  -800,  -800,  -800,  -800,
    -800, 14183,  -800,   176,  -800,  -800, 14183,  -800, 40583,  -800,
   14183,  -800,  -800,  -800,  -800, 14183, 35691, 14183, 35728, 14183,
   35768, 14183, 14183,  -800,   -26, 40583,   176, 40583,   176, 40583,
     176, 40583,   176, 40583,   176, 14183, 14183,  -220,  -800,  -800,
    -800, 14183, 14183,  -800,  -800,  -800, 14183,  -800, 14183,  -800,
   14183, 14183,  -800,  -800,  -800, 14183,  -800,  -800, 14183,  -800,
    -800, 14183,  -800,  -800, 14183,  -800, 14183, 14183,  -800,  -800,
    -800, 29589, 29617,  -800, 14183,  -800,  -800,  -800,  -800,  -800,
     -13,  -800,   101,   111, 14183,  -800, 14183,  -800, 39394, 35816,
   14183,  -158,   132, 35857,  5783, 35894, 14183, 14183, 31810, 14183,
   39447,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800, 39484,
    2841, 39523,   726, 14183, 35942,  2913, 14183, 35982, 14183, 31858,
   14183, 31898, 14183,  2941, 39571,   135, 36023, 14183, 36064, 14183,
   36112, 14183,  -149,  2947,  -800,  -800, 14183, 14183, 14183, 14183,
   14183,  -143,  2974, 14183, 14183, 31939, 39613, 31980, 39650, 39693,
   36149, 39741,  3009, 39779,  3189, 39816,  3236,  -141, 39830,   144,
    -800,  -800, 39867,  3459,  -800,  -800, 29658, 29706,  -800, 14183,
   40583,  -800,  -800,  6133,    86,  -800, 36189, 14183, 40583, 40583,
    -800, 14183, 36237,  -800,  -800,  -800,  -800,  -800, 14183, 40583,
   14183,  -800, 36278,   176, 14183, 36315,  -800, 14183, 36363,  -800,
   14183, 36403,  -800,  -800,  -800, 14183, 36444, 14183, 36485, 14183,
   36533,  -800, 14183,  -800, 40583,   176, 40583,   176, 40583,   176,
   40583,   176, 40583,   176,  -800, 14183,  -800,  -800,  -800,  -800,
   14183,  -800,  -800, 14183,  -800,  -800, 14183,  -800,  -800,  -800,
    -800,  -800,  -800,  -800, 14183,  -800,  -800,  -800,  -800,  -800,
    -800, 39907,  -132,   146, 36570,  -234, 39945, 32028, 14183, 39996,
   36610,  3809, 14183, 36658, 14183, 32068, 14183, 32105, 14183, 36699,
   14183, 36736, 14183, 36784, 14183, 40033, 40073, 40121, 40162, 40199,
   40247,  -800,  -800,  -800,  -232,  -800,  -800,  -800,  -800, 14183,
   36824,  -800, 14183,  -800,   176, 14183, 40583,  -800, 14183, 40583,
    -800, 14183, 40583, 14183, 36865, 14183, 36906, 14183, 36954,  -800,
    -800,  -800,  -800,  -800,  -800,   147,   149, 32153, 14183,  3895,
   40287, 40328, 40369, 36991, 14183, 37031, 14183, 37079, 14183,  -800,
    -800,  -800, 14183, 37120,  -800,  -800,  -800,  -800, 14183, 40583,
   14183, 40583, 14183, 40583, 32194, 14183, 40417, 40456, 40494,  -800,
   14183, 40583,  -800,  -800,  -800, 40542,  -800
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      12,   346,     0,     0,     0,     0,     0,     0,     0,   180,
       0,   183,   178,   179,     0,   138,   140,   133,   134,   139,
     150,   142,   137,   148,     0,     0,     0,     0,     0,     0,
     325,   328,     0,     0,   248,    11,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,   350,
       0,   353,   354,     0,   388,     0,     0,   393,     0,     0,
     404,     0,   246,     0,     0,     0,   419,     0,     0,     0,
       0,     0,     0,     0,     0,   449,     0,     0,   450,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   437,   438,
     439,   440,   441,   445,   444,   443,   442,   147,   136,   141,
       0,     0,     0,     0,    13,     0,     3,     9,     7,    22,
      26,     0,    28,    29,    46,    48,    49,    51,    54,    55,
      68,    69,    70,     0,   121,     0,   122,     0,   129,    75,
      45,    99,    86,   120,    33,    59,    66,   100,    52,   159,
     222,   158,   235,   156,   157,    61,    81,    65,    64,    27,
      82,    83,   103,    36,   107,    89,    76,    32,    25,    31,
      88,    98,   119,    60,   104,   118,   123,    90,   116,    84,
     132,   131,   130,    35,   102,   105,    73,    96,   124,   125,
     126,   127,   128,    97,    71,   108,   110,   115,   113,   112,
     111,   109,   114,    34,    30,    44,    41,    42,    43,    40,
      79,    78,    80,    77,    74,   106,    91,    72,    47,    67,
      85,    63,    87,    24,    37,    38,    39,    56,    57,    58,
      92,    93,    94,    95,    62,   117,    50,    53,   101,   558,
       0,     0,   749,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,     0,     0,     0,
       0,     0,     0,     0,   540,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   543,   547,   548,   549,
     550,   551,   552,     0,   632,   553,   554,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   654,   655,   656,   555,
       0,   541,   542,   559,   560,   561,   562,   563,   564,   587,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   755,
     776,   598,     0,     0,     0,   620,   763,   621,   765,   767,
     622,   770,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   629,   630,   633,     0,     0,     0,   638,   642,   643,
     639,     0,   589,   588,   644,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   657,   658,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   702,   703,   704,   705,   706,   707,   708,   709,   710,
     711,   712,   713,   714,   715,   716,   717,   718,   719,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   491,   490,
     733,   652,   651,   653,   510,   738,     0,     0,   347,   460,
     461,   510,   738,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   283,     0,     0,   285,     0,     0,     0,     0,
       0,     0,     0,     0,   240,     0,     0,   298,     0,     0,
     301,     0,     0,     0,     0,   316,     0,   327,     0,   330,
     241,   242,   249,   250,     0,   253,     0,     0,     0,     0,
     176,   510,     0,   257,     0,     0,     0,   319,     0,     0,
       0,     0,   280,   281,   282,     0,     0,     0,     0,   365,
       0,     0,     0,     0,     0,     0,   386,   387,     0,     0,
       0,     0,   351,   352,     0,   331,   389,     0,     0,   390,
       0,   395,     0,     0,   396,     0,     0,   399,     0,   406,
     247,     0,   409,     0,     0,     0,     0,     0,   416,     0,
     421,   422,     0,     0,     0,     0,     0,   427,   430,   431,
     432,     0,     0,   177,     0,   243,     0,    14,    15,    20,
     433,     0,   322,     0,     0,     0,     0,     0,     0,   434,
     435,   155,   154,   152,   153,   451,   229,   230,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     4,    12,
      10,     5,     8,     0,   135,   144,   145,   149,   151,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     472,     0,     0,     0,     0,     0,     0,   513,   514,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   511,   512,     0,     0,     0,     0,   473,
       0,     0,   348,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   454,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   329,     0,     0,
       0,     0,   160,     0,     0,   161,     0,     0,   168,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   420,     0,   460,   461,     0,     0,
       0,     0,     0,     0,   446,   447,   448,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   231,   224,
     232,   225,   233,   226,   234,   227,     0,     0,     0,   203,
     218,   228,   223,     0,   237,     0,     0,     0,   189,   236,
       0,     2,     6,    23,   146,     0,     0,   750,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   753,     0,
     544,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   556,     0,     0,   756,   775,     0,   599,     0,
       0,     0,     0,     0,     0,     0,   764,   766,   768,     0,
     623,     0,   771,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   634,     0,     0,     0,     0,     0,   640,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   509,     0,   458,     0,     0,     0,   737,
       0,     0,     0,     0,   462,   729,   486,   488,   480,   474,
     475,   476,   466,   467,   470,   471,   482,   484,   478,   464,
     463,   731,   465,   468,   469,   487,   489,   481,   483,   485,
     479,   732,   730,     0,     0,   336,   334,     0,   260,     0,
       0,     0,     0,   284,   453,     0,     0,     0,     0,     0,
       0,   181,     0,     0,   300,     0,   303,     0,   304,     0,
     318,     0,   254,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   259,     0,   321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   366,     0,     0,     0,     0,     0,
       0,     0,   370,   372,     0,   333,     0,   391,     0,   397,
       0,     0,   400,     0,   408,     0,   411,     0,     0,     0,
     412,     0,   418,     0,     0,     0,   425,     0,   429,    18,
       0,   244,     0,     0,     0,    21,     0,   324,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   456,     0,     0,
       0,     0,     0,     0,     0,     0,   751,   515,   740,   517,
       0,     0,     0,   741,   742,     0,     0,     0,   524,   525,
     526,   527,   528,   529,   530,   531,   538,   532,   533,   534,
     535,   536,   537,   518,   739,   516,   754,   545,   631,     0,
       0,   591,   592,   593,   594,   595,   596,   597,   557,   546,
     774,   600,     0,   602,     0,   605,   608,     0,   611,   757,
       0,   760,   769,   624,   772,     0,   625,     0,     0,     0,
       0,   782,     0,   780,     0,   635,     0,     0,   636,   637,
     641,   614,     0,   617,   645,     0,   784,     0,   647,   648,
     649,     0,   786,   787,   788,     0,   790,   508,     0,     0,
       0,   492,     0,   498,   736,     0,     0,   493,     0,   734,
       0,   498,     0,   734,     0,     0,     0,     0,     0,     0,
       0,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   164,     0,     0,
     165,     0,     0,   172,     0,     0,   173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    19,   245,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   452,     0,     0,   205,   206,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   501,     0,   502,     0,     0,
       0,     0,     0,     0,   459,   496,   494,     0,   499,   500,
     497,   495,     0,     0,     0,     0,     0,   261,     0,   264,
       0,     0,     0,   510,   292,     0,     0,     0,   182,   238,
     299,   302,   305,     0,   306,   317,   255,     0,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   320,     0,
     276,   274,     0,   278,     0,   355,     0,     0,   359,   367,
       0,     0,     0,   380,     0,   374,   371,   373,   332,   392,
     398,     0,   401,   402,   407,   410,     0,   413,   414,   417,
       0,   423,   426,   428,   323,     0,     0,     0,     0,     0,
       0,     0,     0,   457,     0,   207,   195,   208,   197,   209,
     199,   210,   201,   204,   193,     0,     0,     0,   186,   185,
     584,     0,     0,   746,   747,   519,     0,   522,     0,   744,
       0,     0,   590,   601,   603,     0,   606,   609,     0,   612,
     758,     0,   761,   773,     0,   626,     0,     0,   628,   783,
     781,     0,     0,   615,     0,   618,   646,   785,   650,   789,
     503,   735,   503,   735,     0,   340,     0,   338,     0,     0,
       0,   510,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   162,   166,   163,   167,   170,   174,   171,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   212,   213,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     506,   507,     0,     0,   504,   505,     0,     0,   265,     0,
     272,   291,   293,     0,   510,   288,     0,     0,   262,   239,
     307,     0,   308,   252,   277,   275,   279,   356,     0,   357,
       0,   360,     0,   361,     0,     0,   381,     0,   382,   375,
       0,   376,   403,   415,   424,     0,     0,     0,     0,     0,
       0,   219,     0,   221,   214,   196,   215,   198,   216,   200,
     217,   202,   211,   194,   190,     0,   192,   188,   187,   585,
       0,   743,   520,     0,   523,   745,     0,   604,   607,   610,
     613,   759,   762,   777,     0,   627,   779,   616,   619,   344,
     342,     0,   510,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   273,   287,   289,     0,   294,   296,   263,   309,     0,
     310,   358,     0,   362,   363,     0,   368,   383,     0,   384,
     377,     0,   378,     0,     0,     0,     0,     0,     0,   220,
     191,   586,   521,   748,   778,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   295,
     297,   311,     0,   312,   364,   369,   385,   379,     0,   266,
       0,   268,     0,   270,     0,     0,     0,     0,     0,   313,
       0,   314,   267,   269,   271,     0,   315
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -800,   -97,  -800,  -800,  -800,  -800,  -440,  -731,   -98,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,   265,
    -800,   271,  -800,   376,   410,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,
    -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,  -800,   -92,
      73,  -514,  -799,  -716,  1221,    -1,  1746
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   115,   619,   116,   117,   589,   844,   590,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   869,
     475,   758,  1126,   984,   985,   463,   467
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     459,   990,   466,   469,   471,  1338,   477,   479,   614,   481,
    1583,   729,   730,   482,   713,   714,  -337,   506,   507,   621,
     741,   742,   743,   729,   730,   486,   489,   492,   494,   497,
     499,   483,   625,   502,   717,   626,   505,  -337,   718,   510,
     514,   516,   519,  -256,   459,   484,   524,   526,   528,   529,
     531,   533,   535,   536,   537,   539,   541,   586,  -290,   543,
    -337,   472,   545,   500,  -256,   548,   551,   553,   556,   559,
     713,   714,   562,   564,   567,   570,   473,   459,   575,  -290,
     578,   579,   580,   474,  1082,   583,  -286,  -256,   729,   730,
     592,   594,   596,   598,   599,  -341,   713,   714,   766,   586,
     501,  -345,  -290,  -339,   560,  1230,   474,  -286,  1231,   584,
     605,  -343,  1110,   585,   513,  1294,  -341,   617,  1115,  1725,
     624,  1755,  -345,   622,  -339,   723,   724,   725,  1201,  1202,
    -286,  1112,  -343,   629,   627,  1339,   628,   474,   615,  -341,
    1584,   474,   845,   474,   846,  -345,  1236,  -339,   618,  1237,
     616,   508,   509,   623,   715,  -343,   581,   582,   716,   726,
     727,   728,   112,   113,   630,   606,   607,   608,   609,   610,
     611,   729,   730,  -335,   960,   961,   587,   588,  1328,  1329,
    1330,  1331,  1332,  1333,   966,   967,   631,   741,   742,   743,
    1025,  1026,  1109,   632,  -335,   633,   731,   732,  1113,  1114,
    1378,  1379,  1574,  1575,  1576,  1577,  1578,  1579,  1203,  1204,
     715,  1611,  1485,  1486,   716,  1604,  1605,  -335,   587,   588,
    1651,  1652,   634,   741,   742,   743,  1664,  1665,  1683,  1684,
     635,   636,   637,   882,   638,   639,   715,  1722,   640,   641,
     716,   642,   643,   644,   713,   714,   645,   646,   647,  1257,
     648,   649,   650,   651,   652,   653,   654,   655,   656,   657,
     659,   660,   661,   662,   663,   723,   724,   725,   664,   665,
     658,   713,   714,   666,   667,   668,   741,   742,   743,   731,
     732,   738,   739,   669,   740,   670,   713,   714,   671,   672,
    1537,   736,   737,   738,   739,   673,   740,   612,   722,   726,
     727,   728,   674,   675,   676,   723,   724,   725,   677,   613,
    1334,   729,   730,   678,   713,   714,   679,   680,   697,   681,
     682,   683,   684,  1378,  1379,   685,   686,   687,   688,  1604,
    1605,   744,   745,   746,  1580,   747,   689,   691,   692,   726,
     727,   728,   995,  1067,   690,   693,   694,   695,   696,   698,
     699,   729,   730,   700,   701,   884,   736,   737,   738,   739,
     702,   740,   703,   757,  1034,   704,   705,   767,   706,   707,
     708,   709,   710,   711,   712,   601,   747,   843,   838,   847,
     848,   602,   935,  -337,   715,   849,   740,  -337,   748,   964,
     741,   742,   743,   946,   947,   952,  1229,  1033,  1137,   965,
    1103,  1124,  1125,  1130,  1131,  1375,  1138,  1139,  1235,  1140,
    -256,   715,  1141,  1143,  -256,   716,  1142,  1144,  1163,   731,
     732,  1145,  1146,  1147,  1165,  -290,   715,  1195,  1208,  -290,
     716,  1197,  1199,  1209,  1227,  1234,   733,   734,   735,   736,
     737,   738,   739,  1244,   740,  1228,   741,   742,   743,  1245,
    1309,  1312,  1313,  -286,   715,   719,   720,  -286,   716,   731,
     732,  1314,  -341,  1315,   751,  1325,  -341,   753,  -345,   755,
    -339,  1376,  -345,   759,  -339,   760,  1450,   762,  -343,   764,
    1380,  1381,  -343,  1436,   768,  1437,   603,   770,  1461,  1462,
     772,  1475,   774,  1476,  1503,   720,  1504,   720,   744,   745,
     746,  1612,   747,   778,  1644,   782,   785,   788,   791,   995,
    1098,   792,  1374,  1686,   794,  1723,  1769,   796,  1770,   720,
     604,  1251,   881,   878,   800,   883,   802,  1463,     0,   804,
       0,   806,     0,   808,   744,   745,   746,   810,   747,   812,
    -335,   717,     0,   814,  -335,   749,   816,     0,     0,   720,
       0,   819,     0,     0,   821,     0,     0,   824,     0,     0,
     826,     0,   828,     0,     0,   832,     0,     0,   720,     0,
       0,   836,     0,   839,     0,   720,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,   744,   745,   746,
     850,   747,   852,     0,   854,     0,   856,     0,   995,  1107,
       0,     0,   741,   742,   743,     0,     0,   858,   860,   862,
     864,   871,   873,   459,   459,   880,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   741,   742,   743,   885,   886,
     888,   459,   459,   459,   459,   459,   459,   459,   459,   459,
     459,   459,   900,   901,   902,   903,   904,   905,   906,   907,
     908,   909,   910,   911,   912,   913,   914,     0,   459,   916,
     459,   919,   921,     0,   923,   924,   925,   926,   927,   928,
     929,   930,   931,   933,     0,     0,     0,   939,   940,   942,
     944,     0,     0,   949,   951,   870,     0,   459,   955,     0,
     459,     0,   459,     0,     0,   963,   741,   742,   743,   969,
     970,   744,   745,   746,     0,   747,     0,   978,   979,   980,
     981,   982,  1214,  1215,   459,   988,   459,   993,     0,     0,
       0,     0,   996,   997,   998,   999,  1000,  1001,  1002,  1003,
    1004,  1005,  1006,  1007,  1008,  1009,  1010,  1012,  1013,  1014,
     741,   742,   743,     0,     0,     0,  1021,  1023,  1024,     0,
       0,  1028,     0,  1030,     0,  1032,     0,   744,   745,   746,
    1037,   747,  1039,     0,  1041,     0,  1042,     0,  1216,  1217,
       0,     0,     0,   741,   742,   743,     0,     0,  1052,  1053,
    1054,     0,  1055,  1056,     0,  1057,  1058,     0,  1059,  1060,
       0,     0,  1062,     0,     0,     0,  1066,     0,  1068,     0,
    1070,     0,  1072,     0,     0,     0,  1076,     0,  1078,     0,
    1080,     0,     0,     0,  1085,     0,     0,     0,     0,     0,
       0,  1091,     0,     0,  1094,     0,  1096,     0,     0,     0,
    1100,     0,     0,     0,   723,   724,   725,   459,     0,  1106,
       0,     0,     0,     0,     0,     0,   459,     0,     0,     0,
    1117,     0,  1119,     0,  1121,     0,  1123,     0,     0,   723,
     724,   725,     0,     0,     0,     0,     0,   759,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,  1083,   723,   724,   725,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,   744,   745,   746,     0,   747,     0,     0,
       0,   726,   727,   728,  1402,  1403,     0,     0,     0,     0,
       0,     0,     0,   729,   730,     0,   744,   745,   746,     0,
     747,     0,     0,     0,     0,     0,     0,  1487,  1488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1206,  1207,     0,     0,     0,     0,     0,
       0,   741,   742,   743,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   741,   742,   743,   731,   732,
       0,   741,   742,   743,     0,     0,     0,     0,     0,     0,
     741,   742,   743,     0,     0,     0,     0,   744,   745,   746,
       0,   747,     0,   731,   732,     0,     0,     0,  1489,  1490,
       0,     0,     0,     0,     0,     0,  1246,     0,  1247,     0,
    1249,   741,   742,   743,   759,  1252,     0,  1254,     0,  1256,
       0,   731,   732,     0,   741,   742,   743,     0,     0,     0,
    1263,   744,   745,   746,     0,   747,     0,   741,   742,   743,
    1277,     0,  1505,  1506,  1279,     0,  1281,     0,  1283,     0,
    1285,   741,   742,   743,  1288,     0,  1290,     0,  1292,     0,
       0,     0,     0,  1296,   744,   745,   746,     0,   747,  1299,
       0,     0,  1302,     0,  1303,  1627,  1628,  1305,     0,     0,
       0,     0,   459,     0,  1310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1316,     0,  1317,     0,  1319,
       0,  1321,     0,   459,   459,     0,     0,  1327,     0,   459,
     459,  1337,     0,  1340,  1341,   741,   742,   743,     0,  1342,
    1343,  1344,     0,     0,   459,   733,   734,   735,   736,   737,
     738,   739,     0,   740,  1295,     0,     0,  1128,     0,     0,
    1129,     0,     0,     0,   741,   742,   743,     0,  1348,  1349,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1350,  1132,  1351,     0,  1133,  1353,     0,     0,  1355,
       0,     0,     0,     0,   459,     0,   459,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
    1232,  1368,     0,  1233,  1370,     0,     0,     0,     0,     0,
    1372,     0,   458,     0,  1373,     0,     0,   459,   741,   742,
     743,  1377,     0,   741,   742,   743,     0,  1382,     0,  1383,
       0,  1384,     0,  1385,  1386,     0,     0,  1389,     0,  1391,
       0,     0,  1395,     0,  1397,     0,  1399,     0,     0,     0,
    1404,     0,     0,  1408,  1409,  1410,   522,  1411,  1412,     0,
    1413,  1414,     0,  1415,  1416,   741,   742,   743,     0,  1420,
       0,  1423,   744,   745,   746,  1427,   747,     0,  1431,     0,
    1433,     0,  1435,     0,   750,     0,   744,   745,   746,   573,
     747,     0,   744,   745,   746,  1448,   747,   995,   459,     0,
       0,   744,   745,   746,   799,   747,     0,  1456,     0,  1458,
       0,  1460,     0,   831,     0,     0,  1394,     0,     0,  1465,
    1467,  1469,  1471,  1473,     0,     0,     0,   459,   459,     0,
       0,     0,   744,   745,   746,     0,   747,   741,   742,   743,
       0,     0,     0,     0,   842,   744,   745,   746,     0,   747,
       0,     0,     0,  1511,     0,  1512,     0,  1198,   744,   745,
     746,     0,   747,     0,     0,     0,   723,   724,   725,     0,
    1200,     0,   744,   745,   746,     0,   747,  1528,     0,  1529,
       0,  1533,     0,     0,  1221,  1535,     0,     0,     0,     0,
       0,     0,  1538,   741,   742,   743,  1540,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,  1549,     0,
       0,  1551,   729,   730,     0,  1554,     0,     0,     0,  1557,
       0,  1559,     0,  1561,     0,     0,     0,     0,   741,   742,
     743,     0,     0,     0,     0,  1564,   744,   745,   746,   459,
     747,     0,     0,     0,  1566,     0,  1568,  1168,  1570,     0,
     459,     0,     0,     0,     0,  1532,     0,     0,     0,     0,
       0,     0,     0,     0,   459,   744,   745,   746,     0,   747,
    1585,  1586,     0,     0,     0,  1587,  1179,  1588,     0,  1589,
    1590,   741,   742,   743,  1591,     0,     0,  1593,     0,     0,
    1595,     0,     0,   459,     0,  1598,   459,     0,     0,     0,
       0,     0,     0,  1602,     0,     0,   741,   742,   743,     0,
       0,     0,     0,  1606,     0,  1607,     0,     0,     0,  1610,
     731,   732,     0,  1616,     0,  1618,  1619,     0,  1622,   744,
     745,   746,     0,   747,   744,   745,   746,     0,   747,     0,
       0,  1261,  1629,     0,     0,  1632,  1306,  1635,     0,  1638,
       0,  1641,   741,   742,   743,     0,  1646,     0,  1648,     0,
    1650,     0,     0,     0,     0,  1654,  1656,  1658,  1660,  1662,
       0,     0,   459,   459,     0,     0,   744,   745,   746,     0,
     747,   741,   742,   743,     0,     0,     0,     0,  1326,     0,
       0,     0,     0,     0,     0,     0,     0,  1615,  1691,     0,
       0,     0,  1694,     0,     0,     0,  1696,     0,     0,     0,
    1697,     0,     0,     0,     0,     0,     0,  1699,     0,  1700,
       0,     0,     0,  1703,     0,     0,  1705,     0,     0,  1707,
       0,     0,     0,     0,  1709,     0,  1711,     0,  1713,     0,
       0,  1715,     0,     0,     0,     0,     0,     0,   744,   745,
     746,     0,   747,     0,  1716,     0,   741,   742,   743,  1717,
    1446,     0,  1718,     0,     0,  1719,     0,     0,     0,     0,
     741,   742,   743,  1720,     0,     0,  1693,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,  1730,     0,  1238,
       0,     0,  1239,  1736,     0,  1739,     0,  1742,     0,  1744,
       0,  1746,     0,  1748,   744,   745,   746,     0,   747,     0,
       0,     0,     0,     0,     0,     0,  1507,     0,  1757,     0,
       0,     0,     0,     0,  1760,     0,     0,  1761,     0,     0,
    1762,     0,  1763,     0,  1765,     0,  1767,   460,   464,   744,
     745,   746,     0,   747,   741,   742,   743,  1773,     0,     0,
       0,  1553,     0,  1779,     0,  1781,     0,  1783,  1726,     0,
       0,  1784,   487,   490,     0,   495,     0,  1786,     0,  1787,
     503,  1788,     0,     0,  1791,     0,     0,     0,   517,  1795,
     521,   460,   523,   835,     0,     0,     0,  1756,     0,     0,
       0,     0,   744,   745,   746,   542,   747,     0,     0,     0,
       0,   546,   549,  1180,   554,   557,     0,     0,     0,     0,
     565,   568,     0,   571,   460,     0,   577,   744,   745,   746,
       0,   747,   723,   724,   725,   874,   879,     0,  1185,     0,
       0,     0,   600,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   889,   890,   891,   892,   893,   894,   895,
     896,   897,   898,   899,     0,     0,   726,   727,   728,     0,
       0,     0,     0,   744,   745,   746,     0,   747,   729,   730,
     915,     0,   917,     0,  1188,   741,   742,   743,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   741,   742,   743,
       0,     0,   744,   745,   746,     0,   747,     0,     0,   954,
       0,     0,   956,  1191,   958,     0,     0,     0,     1,     2,
       0,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,     0,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,    26,
      27,    28,     0,    29,    30,    31,    32,    33,    34,    35,
      36,    37,     0,     0,     0,    38,    39,     0,     0,     0,
     741,   742,   743,     0,     0,     0,     0,   744,   745,   746,
       0,   747,     0,     0,     0,     0,   731,   732,  1194,     0,
       0,   744,   745,   746,     0,   747,     0,     0,     0,     0,
      40,    41,  1213,     0,     0,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    43,    44,     0,     0,
      45,    46,    47,     0,     0,     0,    48,    49,    50,    51,
      52,    53,    54,    55,    56,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,    58,     0,    59,     0,    60,
      61,    62,     0,    63,     0,     0,     0,     0,     0,  1104,
      64,     0,     0,     0,     0,   744,   745,   746,     0,   747,
       0,     0,     0,     0,     0,     0,  1218,     0,   741,   742,
     743,     0,     0,     0,    65,     0,     0,     0,    66,    67,
      68,    69,    70,     0,     0,     0,     0,    71,    72,     0,
     741,   742,   743,    73,    74,     0,    75,    76,     0,     0,
      77,     0,    78,     0,     0,    79,     0,     0,     0,    80,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    81,
      82,    83,     0,     0,    84,    85,    86,    87,    88,    89,
      90,     0,    91,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,   994,    92,    93,    94,    95,   723,
     724,   725,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,     0,     0,
     741,   742,   743,   726,   727,   728,   744,   745,   746,     0,
     747,     0,     0,   721,     0,   729,   730,  1219,   744,   745,
     746,   721,   747,     0,   721,     0,   721,     0,     0,  1220,
       0,     0,   721,     0,   721,     0,   721,     0,     0,     0,
       0,   721,     0,     0,   721,     0,     0,   721,     0,   721,
       0,     0,   721,     0,   721,     0,     0,     0,     0,     0,
     721,     0,     0,     0,     0,     0,     0,     0,   721,     0,
     111,   721,     0,     0,   721,     0,   798,     0,   112,   113,
       0,   721,   114,   721,     0,     0,   721,     0,   721,     0,
     721,   744,   745,   746,   721,   747,   721,   741,   742,   743,
     721,     0,  1400,   721,     0,     0,   721,     0,   721,     0,
       0,   721,     0,     0,   721,     0,     0,   721,     0,   829,
       0,     0,   721,   731,   732,   721,     0,     0,   837,     0,
     721,     0,   841,     0,  1308,     0,     0,     0,     0,     0,
       0,     0,   741,   742,   743,     0,     0,   721,     0,   721,
       0,   721,     0,   721,     0,  1323,  1324,     0,   741,   742,
     743,  1335,  1336,     0,   859,   861,   863,   865,   872,     0,
     460,   460,   741,   742,   743,     0,  1345,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   460,   460,
     460,   460,   460,   460,   460,   460,   460,   460,   460,   744,
     745,   746,     0,   747,     0,   723,   724,   725,     0,     0,
    1401,     0,     0,     0,     0,   460,     0,   460,     0,     0,
     922,   744,   745,   746,     0,   747,  1357,     0,  1358,     0,
       0,   934,  1405,   937,     0,   941,   943,   945,     0,   726,
     727,   728,     0,   953,   460,     0,     0,   460,   957,   460,
     959,   729,   730,   741,   742,   743,     0,   971,   972,   973,
     974,   975,   976,   977,     0,     0,     0,     0,     0,     0,
       0,   460,     0,   460,   723,   724,   725,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,     0,  1240,  1011,     0,  1241,     0,  1015,  1016,  1017,
    1018,  1019,  1020,  1022,     0,     0,     0,     0,   726,   727,
     728,   744,   745,   746,     0,   747,     0,     0,     0,     0,
     729,   730,  1418,     0,     0,  1044,     0,  1046,     0,  1048,
       0,  1050,     0,     0,     0,     0,   721,     0,     0,   721,
    1451,     0,   721,     0,     0,   721,     0,     0,     0,     0,
       0,  1064,     0,     0,     0,     0,     0,     0,     0,   731,
     732,  1074,     0,     0,     0,     0,     0,     0,     0,  1478,
    1479,     0,     0,  1087,     0,     0,  1089,     0,  1092,     0,
       0,     0,     0,     0,     0,     0,  1099,     0,     0,  1102,
       0,     0,     0,     0,   460,     0,     0,     0,  1108,     0,
     723,   724,   725,   460,     0,     0,     0,     0,   744,   745,
     746,     0,   747,     0,     0,     0,   723,   724,   725,  1429,
       0,     0,     0,     0,  1127,     0,     0,     0,   731,   732,
       0,     0,     0,  1127,   726,   727,   728,   741,   742,   743,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
     726,   727,   728,   744,   745,   746,     0,   747,     0,     0,
       0,     0,   729,   730,  1439,     0,   723,   724,   725,   744,
     745,   746,     0,   747,     0,     0,     0,     0,     0,     0,
    1440,  1565,     0,   744,   745,   746,     0,   747,     0,     0,
       0,     0,  1572,     0,  1442,     0,     0,     0,     0,     0,
     726,   727,   728,   741,   742,   743,  1581,     0,   741,   742,
     743,     0,   729,   730,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,  1242,     0,
       0,  1243,     0,     0,     0,  1597,     0,     0,  1599,   741,
     742,   743,     0,     0,     0,     0,     0,   741,   742,   743,
       0,     0,     0,     0,   731,   732,     0,     0,     0,     0,
       0,   741,   742,   743,   744,   745,   746,     0,   747,     0,
     731,   732,     0,     0,     0,  1449,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,     0,  1266,     0,  1258,
    1267,  1259,     0,  1260,     0,  1262,   741,   742,   743,     0,
       0,   741,   742,   743,  1667,  1668,     0,     0,     0,  1278,
     731,   732,     0,     0,     0,     0,     0,     0,     0,  1287,
       0,     0,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,  1297,     0,  1298,     0,  1300,     0,   723,   724,
     725,     0,     0,  1304,     0,     0,     0,  1307,     0,   460,
       0,     0,     0,  1311,     0,     0,   726,   727,   728,     0,
       0,     0,     0,     0,   723,   724,   725,     0,   729,   730,
     460,   460,   726,   727,   728,     0,   460,   460,     0,     0,
     741,   742,   743,     0,   729,   730,     0,     0,   741,   742,
     743,   460,  1346,  1347,     0,     0,     0,     0,   726,   727,
     728,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     729,   730,     0,  1269,     0,     0,  1270,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,     0,  1272,
    1352,     0,  1273,  1354,     0,     0,  1356,     0,   744,   745,
     746,   460,   747,   460,  1359,  1360,  1361,     0,  1362,  1453,
    1363,   723,   724,   725,     0,     0,     0,     0,  1369,     0,
     741,   742,   743,  1371,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,   460,   740,   731,   732,     0,  1275,
       0,     0,  1276,     0,     0,   726,   727,   728,   741,   742,
     743,     0,   731,   732,   741,   742,   743,   729,   730,     0,
       0,     0,     0,     0,   744,   745,   746,     0,   747,   744,
     745,   746,     0,   747,     0,  1496,     0,     0,   731,   732,
    1499,   741,   742,   743,     0,     0,  1421,     0,     0,     0,
    1425,     0,  1428,     0,     0,   723,   724,   725,     0,     0,
     744,   745,   746,     0,   747,     0,     0,  1443,   744,   745,
     746,  1502,   747,     0,     0,   460,   741,   742,   743,  1508,
       0,     0,   744,   745,   746,     0,   747,     0,     0,   726,
     727,   728,  1127,  1509,     0,     0,  1466,  1468,  1470,  1472,
    1474,   729,   730,     0,   460,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   731,   732,   744,   745,   746,
       0,   747,   744,   745,   746,     0,   747,     0,  1510,     0,
       0,     0,     0,  1515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,     0,  1364,     0,     0,  1365,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,  1366,     0,     0,  1367,  1550,     0,     0,     0,     0,
    1552,     0,  1555,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,  1136,  1563,     0,   731,
     732,   744,   745,   746,     0,   747,   460,     0,     0,   744,
     745,   746,  1517,   747,     0,     0,     0,   460,  1573,     0,
    1625,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   460,  1582,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   741,   742,   743,     0,
       0,  1592,     0,     0,  1594,     0,     0,  1596,     0,     0,
     460,     0,     0,   460,     0,     0,     0,     0,     0,     0,
    1603,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   744,   745,   746,  1524,   747,     0,  1525,     0,     0,
       0,     0,  1631,   741,   742,   743,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   744,
     745,   746,  1633,   747,     0,   744,   745,   746,     0,   747,
    1642,     0,     0,     0,     0,     0,  1653,     0,     0,     0,
       0,     0,  1655,  1657,  1659,  1661,  1663,     0,     0,   460,
     460,     0,   744,   745,   746,     0,   747,     0,   239,   240,
     241,     0,     0,  1666,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,  1526,     0,
       0,  1527,     0,     0,     0,     0,     0,   744,   745,   746,
     242,   747,     0,     0,     0,     0,  1701,     0,  1678,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,  1734,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,  1759,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,   741,   742,   743,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,   744,   745,   746,
       0,   747,     0,     0,     0,     0,     0,     0,  1680,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,   744,   745,   746,     0,   747,     0,
       0,     0,     0,     0,     0,  1682,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,   986,     0,     0,     0,     0,     0,     0,
     987,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,   744,   745,   746,
     299,   747,   300,   301,   302,   303,   304,   305,  1688,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,   741,   742,   743,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   741,   742,   743,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,   991,     0,     0,     0,     0,     0,     0,
     992,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,   744,   745,   746,
     299,   747,   300,   301,   302,   303,   304,   305,  1733,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
     866,   348,   349,   867,   350,   351,   352,     0,     0,     0,
       0,     0,     0,   744,   745,   746,     0,   747,     0,   353,
       0,   354,   355,     0,  1774,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   868,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
     875,   348,   349,   876,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   877,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   511,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   512,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,  1393,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,  1531,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,  1614,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,  1692,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,   474,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   780,     0,     0,     0,   781,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   783,     0,     0,     0,   784,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   786,     0,     0,     0,   787,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   789,     0,     0,     0,   790,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   776,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   777,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   818,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   823,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   834,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   887,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   918,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   920,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   932,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   936,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   938,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   948,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   950,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   962,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   968,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   983,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,   989,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,  1111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   461,   462,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   465,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   468,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   476,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   480,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   488,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   493,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   496,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   498,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   504,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   515,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   518,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   520,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   525,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   527,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   532,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   534,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   538,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   544,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   547,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   550,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   552,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   555,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   558,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   561,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   563,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   566,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   572,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   574,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   576,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   591,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   593,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,   239,   240,
     241,     0,     0,     0,   456,     0,     0,     0,     0,     0,
       0,   595,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,     0,     0,   271,     0,     0,
     272,   273,   274,   275,     0,   276,     0,     0,   277,   278,
     279,   280,   281,   282,     0,     0,   283,   284,     0,     0,
       0,   285,   286,   287,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   288,   289,   290,   291,   292,   293,     0,
     294,   295,   296,     0,   297,     0,   298,     0,     0,     0,
     299,     0,   300,   301,   302,   303,   304,   305,     0,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,     0,   329,   330,     0,     0,     0,     0,     0,
       0,   331,   332,   333,   334,     0,     0,   335,   336,   337,
     338,     0,     0,   339,     0,     0,   340,   341,     0,   342,
       0,   343,   344,     0,     0,     0,   345,     0,   346,   347,
       0,   348,   349,     0,   350,   351,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   353,
       0,   354,   355,     0,     0,     0,     0,   356,   357,   358,
     359,   360,   361,     0,     0,     0,     0,   362,   363,   364,
       0,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,     0,     0,
     448,   449,   450,   451,   452,   453,   454,   455,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     1,     2,     0,
       0,   597,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,    26,    27,
      28,     0,    29,    30,    31,    32,    33,    34,   620,    36,
      37,     0,     0,     0,    38,    39,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    40,
      41,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,     0,     0,    45,
      46,    47,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,    55,    56,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,    58,     0,    59,     0,    60,    61,
      62,     0,    63,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    65,     0,     0,     0,    66,    67,    68,
      69,    70,     0,     0,     0,     0,    71,    72,     0,     0,
       0,     0,    73,    74,     0,    75,    76,     0,     0,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
       0,     0,   723,   724,   725,     0,     0,     0,    81,    82,
      83,     0,     0,    84,    85,    86,    87,    88,    89,    90,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,     0,     0,
       0,    96,     0,     0,     0,     0,     0,     0,   729,   730,
       0,     0,     0,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,     1,     2,     0,
       0,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,    26,    27,
      28,     0,    29,    30,    31,    32,    33,    34,     0,    36,
      37,     0,     0,     0,    38,    39,     0,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
       0,     0,     0,     0,     0,     0,     0,   112,   113,    40,
      41,   726,   727,   728,     0,    42,   731,   732,     0,     0,
       0,     0,     0,   729,   730,    43,    44,     0,     0,    45,
      46,    47,     0,     0,     0,    48,    49,    50,    51,    52,
      53,    54,    55,    56,   723,   724,   725,     0,     0,     0,
      57,     0,     0,     0,    58,     0,    59,     0,    60,    61,
      62,     0,    63,     0,     0,     0,     0,     0,     0,    64,
       0,     0,     0,     0,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,     0,    65,     0,     0,     0,    66,    67,    68,
      69,    70,     0,     0,     0,     0,    71,    72,     0,     0,
       0,     0,    73,    74,     0,    75,    76,     0,     0,    77,
       0,    78,     0,     0,    79,     0,     0,     0,    80,     0,
       0,   731,   732,     0,     0,     0,     0,     0,    81,    82,
      83,     0,     0,    84,    85,    86,    87,    88,    89,    90,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,   723,   724,
     725,    96,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,     0,   731,   732,
       0,     0,   726,   727,   728,     0,     0,     0,     0,     0,
     723,   724,   725,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   723,   724,   725,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,   723,   724,   725,     0,     0,   729,   730,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,   111,
       0,     0,   729,   730,   723,   724,   725,   112,   113,     0,
       0,     0,     0,     0,     0,   726,   727,   728,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   729,   730,   723,
     724,   725,     0,  1464,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   731,   732,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   726,   727,   728,     0,   723,   724,   725,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,     0,     0,     0,     0,
    1477,   726,   727,   728,     0,     0,   723,   724,   725,     0,
     731,   732,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   723,   724,   725,     0,
       0,     0,     0,     0,     0,   731,   732,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,   731,   732,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   731,   732,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,   731,   732,     0,  1520,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,     0,     0,     0,  1521,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,   726,   727,   728,
     731,   732,  1522,     0,     0,     0,     0,     0,     0,   729,
     730,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,     0,     0,     0,     0,  1523,   731,   732,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   723,   724,   725,     0,     0,     0,
    1542,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,     0,     0,     0,     0,  1544,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
       0,     0,     0,  1546,     0,     0,     0,   731,   732,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,   723,   724,
     725,     0,  1548,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,   726,   727,   728,
       0,     0,  1600,   723,   724,   725,     0,     0,     0,   729,
     730,     0,   726,   727,   728,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   729,   730,     0,     0,   731,   732,
    1601,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,     0,   731,
     732,  1689,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   723,   724,   725,     0,   731,   732,  1690,
       0,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,   731,   732,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,   731,   732,   729,
     730,     0,     0,   726,   727,   728,     0,     0,     0,   723,
     724,   725,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,   994,  1027,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,   994,  1029,     0,
       0,     0,   731,   732,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   731,   732,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,   994,  1031,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
     994,  1036,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,   994,  1038,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   726,   727,   728,   723,
     724,   725,     0,     0,     0,     0,     0,     0,   729,   730,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,   994,  1040,   731,   732,     0,     0,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
     994,  1043,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,   727,   728,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   729,   730,   994,  1045,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,   994,  1047,     0,     0,     0,   731,   732,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,   994,  1049,   731,   732,     0,     0,     0,     0,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,     0,     0,   729,   730,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,   994,  1051,   723,   724,   725,     0,     0,     0,
       0,   731,   732,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,   994,  1061,     0,     0,   726,   727,
     728,     0,     0,     0,   723,   724,   725,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,   994,  1063,   731,   732,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   723,   724,   725,     0,     0,     0,   731,   732,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,   994,
    1065,   731,   732,     0,     0,     0,   726,   727,   728,     0,
       0,     0,     0,   723,   724,   725,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
     994,  1069,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,   994,  1071,     0,     0,     0,   731,   732,     0,     0,
       0,     0,   726,   727,   728,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   729,   730,   994,  1073,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,   994,
    1075,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,   994,  1077,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,   994,  1079,     0,     0,
       0,     0,   731,   732,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,   994,  1081,     0,     0,     0,     0,
       0,     0,   726,   727,   728,   723,   724,   725,     0,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,   994,  1084,     0,     0,   726,
     727,   728,     0,     0,     0,   723,   724,   725,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,   994,  1086,   731,   732,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
     994,  1088,   731,   732,     0,     0,     0,   726,   727,   728,
       0,     0,     0,     0,   723,   724,   725,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   731,
     732,     0,     0,     0,     0,     0,     0,     0,   726,   727,
     728,   723,   724,   725,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,   994,  1090,     0,     0,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,   994,  1093,     0,     0,     0,   731,   732,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,   723,
     724,   725,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
     994,  1095,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,   994,  1097,     0,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,   723,   724,   725,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,   994,  1101,     0,
       0,     0,     0,   731,   732,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,   994,  1105,     0,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,   723,
     724,   725,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,   994,  1116,     0,     0,
       0,     0,     0,   726,   727,   728,   723,   724,   725,     0,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,   994,  1118,   731,   732,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,   994,  1120,   731,   732,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,   994,  1122,   731,   732,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,   723,   724,   725,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,  1181,  1182,     0,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,  1183,  1184,     0,     0,     0,   731,   732,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1186,  1187,     0,     0,     0,     0,     0,   726,   727,
     728,   723,   724,   725,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1189,  1190,     0,     0,   726,   727,   728,     0,     0,
       0,   723,   724,   725,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,  1211,  1212,
     731,   732,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,   994,  1265,   731,   732,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,   994,  1268,     0,
       0,     0,     0,     0,   726,   727,   728,   723,   724,   725,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,   994,  1271,     0,
       0,   726,   727,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   730,   723,   724,   725,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,   994,  1274,
       0,     0,     0,   731,   732,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,   723,   724,   725,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1406,  1407,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,  1494,  1495,     0,     0,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,   723,   724,   725,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,  1497,  1498,     0,     0,     0,     0,   731,
     732,   726,   727,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   730,   723,   724,   725,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1500,  1501,     0,     0,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,   723,   724,   725,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,  1513,  1514,     0,     0,     0,     0,     0,   726,
     727,   728,   723,   724,   725,     0,     0,     0,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,  1620,
    1621,   731,   732,     0,     0,     0,   726,   727,   728,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,   730,
     723,   724,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,  1636,  1637,   731,
     732,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,   723,   724,   725,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,  1639,  1640,   731,
     732,     0,     0,     0,     0,   726,   727,   728,     0,     0,
       0,   723,   724,   725,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,  1669,  1670,
       0,     0,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,   723,   724,   725,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,  1672,
    1673,     0,     0,     0,   731,   732,   726,   727,   728,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,   730,
     723,   724,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,  1728,  1729,     0,
       0,     0,     0,     0,   726,   727,   728,   723,   724,   725,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,  1737,  1738,     0,
       0,   726,   727,   728,     0,     0,     0,   723,   724,   725,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,  1740,  1741,   731,   732,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   730,   723,   724,   725,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1771,  1772,   731,   732,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,  1789,  1790,     0,     0,     0,     0,     0,
     726,   727,   728,   723,   724,   725,     0,     0,     0,     0,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,   752,     0,     0,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   723,   724,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,     0,   754,     0,     0,     0,   731,
     732,     0,     0,     0,     0,   726,   727,   728,     0,     0,
       0,   723,   724,   725,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,   756,     0,   726,   727,   728,     0,     0,
       0,     0,   723,   724,   725,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
     761,     0,     0,     0,     0,     0,   726,   727,   728,     0,
       0,     0,     0,   723,   724,   725,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
     763,     0,     0,     0,     0,   731,   732,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   723,   724,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,     0,   765,     0,
       0,     0,     0,     0,     0,   726,   727,   728,   723,   724,
     725,     0,     0,     0,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,   769,
       0,     0,   726,   727,   728,     0,     0,     0,   723,   724,
     725,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,     0,   771,   731,   732,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   729,   730,   723,   724,   725,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,     0,   773,   731,   732,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,   723,   724,   725,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,   731,   732,   775,     0,     0,     0,     0,     0,
       0,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,     0,   779,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,     0,   793,     0,     0,     0,
     731,   732,     0,     0,     0,     0,   726,   727,   728,     0,
       0,     0,   723,   724,   725,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,   795,     0,   726,   727,   728,     0,
       0,     0,     0,   723,   724,   725,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
       0,   797,     0,     0,     0,     0,     0,   726,   727,   728,
       0,     0,     0,     0,   723,   724,   725,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,   801,     0,     0,     0,     0,   731,   732,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,   803,
       0,     0,     0,     0,     0,     0,   726,   727,   728,   723,
     724,   725,     0,     0,     0,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
     805,     0,     0,   726,   727,   728,     0,     0,     0,   723,
     724,   725,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,     0,   807,   731,   732,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,     0,   809,   731,   732,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,   723,   724,
     725,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,   731,   732,   811,     0,     0,     0,     0,
       0,     0,   726,   727,   728,   723,   724,   725,     0,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,     0,   813,     0,     0,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,     0,   815,     0,     0,
       0,   731,   732,     0,     0,     0,     0,   726,   727,   728,
       0,     0,     0,   723,   724,   725,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,     0,   817,     0,   726,   727,   728,
       0,     0,     0,     0,   723,   724,   725,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,     0,   820,     0,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,     0,   822,     0,     0,     0,     0,   731,   732,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
     825,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
       0,   827,     0,     0,   726,   727,   728,     0,     0,     0,
     723,   724,   725,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,   830,   731,
     732,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,     0,   833,   731,   732,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,   731,   732,   840,     0,     0,     0,
       0,     0,     0,   726,   727,   728,   723,   724,   725,     0,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,     0,   851,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,   853,     0,
       0,     0,   731,   732,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,   723,   724,   725,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,     0,   855,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,   857,     0,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,     0,  1035,     0,     0,     0,     0,   731,   732,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
       0,  1134,     0,     0,     0,     0,     0,     0,   726,   727,
     728,   723,   724,   725,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,     0,  1135,     0,     0,   726,   727,   728,     0,     0,
       0,   723,   724,   725,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,     0,  1169,
     731,   732,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,     0,  1170,   731,   732,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,   731,   732,  1225,     0,     0,
       0,     0,     0,     0,   726,   727,   728,   723,   724,   725,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,     0,  1248,     0,
       0,   726,   727,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   730,   723,   724,   725,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,     0,  1250,
       0,     0,     0,   731,   732,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,   723,   724,   725,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,     0,  1253,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,  1255,     0,     0,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,   723,   724,   725,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,     0,  1264,     0,     0,     0,     0,   731,
     732,   726,   727,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   730,   723,   724,   725,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,     0,  1280,     0,     0,     0,     0,     0,     0,   726,
     727,   728,   723,   724,   725,     0,     0,     0,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,  1282,     0,     0,   726,   727,   728,     0,
       0,     0,   723,   724,   725,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
    1284,   731,   732,     0,     0,     0,   726,   727,   728,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,   730,
     723,   724,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,  1286,   731,
     732,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,   723,   724,   725,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,   731,   732,  1289,     0,
       0,     0,     0,     0,     0,   726,   727,   728,   723,   724,
     725,     0,     0,     0,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,  1291,
       0,     0,   726,   727,   728,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   729,   730,   723,   724,   725,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
    1293,     0,     0,     0,   731,   732,     0,     0,     0,     0,
     726,   727,   728,     0,     0,     0,   723,   724,   725,     0,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,     0,  1301,     0,
     726,   727,   728,     0,     0,     0,     0,   723,   724,   725,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,     0,  1318,     0,     0,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,   723,   724,
     725,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,     0,  1320,     0,     0,     0,     0,
     731,   732,   726,   727,   728,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   729,   730,   723,   724,   725,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,  1322,     0,     0,     0,     0,     0,     0,
     726,   727,   728,   723,   724,   725,     0,     0,     0,     0,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,  1388,     0,     0,   726,   727,   728,
       0,     0,     0,   723,   724,   725,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,  1390,   731,   732,     0,     0,     0,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   723,   724,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,     0,  1392,
     731,   732,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,   723,   724,   725,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,   731,   732,  1396,
       0,     0,     0,     0,     0,     0,   726,   727,   728,   723,
     724,   725,     0,     0,     0,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
    1419,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,  1422,     0,     0,     0,   731,   732,     0,     0,     0,
       0,   726,   727,   728,     0,     0,     0,   723,   724,   725,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,  1424,
       0,   726,   727,   728,     0,     0,     0,     0,   723,   724,
     725,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,     0,  1426,     0,     0,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,     0,  1430,     0,     0,     0,
       0,   731,   732,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,  1432,     0,     0,     0,     0,     0,
       0,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,     0,  1434,     0,     0,   726,   727,
     728,     0,     0,     0,   723,   724,   725,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,     0,  1441,   731,   732,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
    1455,   731,   732,     0,     0,     0,   726,   727,   728,     0,
       0,     0,     0,   723,   724,   725,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,   731,   732,
    1457,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
       0,  1459,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,     0,  1481,     0,     0,     0,   731,   732,     0,     0,
       0,     0,   726,   727,   728,     0,     0,     0,   723,   724,
     725,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
    1482,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,     0,  1491,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,     0,  1530,     0,     0,
       0,     0,   731,   732,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,     0,  1534,     0,     0,     0,     0,
       0,     0,   726,   727,   728,   723,   724,   725,     0,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,     0,  1536,     0,     0,   726,
     727,   728,     0,     0,     0,   723,   724,   725,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,     0,  1539,   731,   732,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,  1556,   731,   732,     0,     0,     0,   726,   727,   728,
       0,     0,     0,     0,   723,   724,   725,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,   731,
     732,  1558,     0,     0,     0,     0,     0,     0,   726,   727,
     728,   723,   724,   725,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,     0,  1560,     0,     0,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,     0,  1562,     0,     0,     0,   731,   732,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,   723,
     724,   725,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
       0,  1567,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,     0,  1569,     0,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,   723,   724,   725,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,     0,  1571,     0,
       0,     0,     0,   731,   732,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,     0,  1609,     0,     0,     0,
       0,     0,     0,   726,   727,   728,   723,   724,   725,     0,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,     0,  1613,     0,     0,
     726,   727,   728,     0,     0,     0,   723,   724,   725,     0,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,     0,  1617,   731,   732,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,     0,  1630,   731,   732,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
     731,   732,  1634,     0,     0,     0,     0,     0,     0,   726,
     727,   728,   723,   724,   725,     0,     0,     0,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,  1645,     0,     0,   726,   727,   728,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,   730,
     723,   724,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,     0,  1647,     0,     0,     0,   731,   732,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
     723,   724,   725,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,     0,  1649,     0,   726,   727,   728,     0,     0,     0,
       0,   723,   724,   725,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,  1676,
       0,     0,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,   723,   724,   725,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,     0,  1695,
       0,     0,     0,     0,   731,   732,   726,   727,   728,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,   730,
     723,   724,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,     0,  1698,     0,     0,
       0,     0,     0,     0,   726,   727,   728,   723,   724,   725,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,     0,  1702,     0,
       0,   726,   727,   728,     0,     0,     0,   723,   724,   725,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,     0,  1704,   731,   732,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   729,   730,   723,   724,   725,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,     0,  1706,   731,   732,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,   731,   732,  1708,     0,     0,     0,     0,     0,     0,
     726,   727,   728,   723,   724,   725,     0,     0,     0,     0,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,  1710,     0,     0,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   723,   724,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,     0,  1712,     0,     0,     0,   731,
     732,     0,     0,     0,     0,   726,   727,   728,     0,     0,
       0,   723,   724,   725,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,     0,  1714,     0,   726,   727,   728,     0,     0,
       0,     0,   723,   724,   725,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
    1724,     0,     0,     0,     0,     0,   726,   727,   728,     0,
       0,     0,     0,   723,   724,   725,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,     0,
    1732,     0,     0,     0,     0,   731,   732,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   723,   724,   725,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,     0,  1735,     0,
       0,     0,     0,     0,     0,   726,   727,   728,   723,   724,
     725,     0,     0,     0,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,  1743,
       0,     0,   726,   727,   728,     0,     0,     0,   723,   724,
     725,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,     0,  1745,   731,   732,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   729,   730,   723,   724,   725,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,     0,  1747,   731,   732,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,   723,   724,   725,
       0,     0,   729,   730,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,   731,   732,  1758,     0,     0,     0,     0,     0,
       0,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,     0,  1764,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,     0,  1766,     0,     0,     0,
     731,   732,     0,     0,     0,     0,   726,   727,   728,     0,
       0,     0,   723,   724,   725,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,     0,  1768,     0,   726,   727,   728,     0,
       0,     0,     0,   723,   724,   725,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
       0,  1778,     0,     0,     0,     0,     0,   726,   727,   728,
       0,     0,     0,     0,   723,   724,   725,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
       0,  1780,     0,     0,     0,     0,   731,   732,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   723,   724,   725,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,     0,  1782,
       0,     0,     0,     0,     0,     0,   726,   727,   728,     0,
       0,   723,   724,   725,     0,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,     0,
    1785,     0,     0,     0,     0,   726,   727,   728,     0,   723,
     724,   725,     0,     0,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,  1148,     0,   731,   732,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,  1149,     0,   731,   732,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,   723,   724,
     725,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,  1150,   731,   732,     0,     0,     0,
       0,     0,   726,   727,   728,     0,     0,   723,   724,   725,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1151,     0,     0,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,     0,   723,
     724,   725,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,  1152,     0,     0,     0,
       0,   731,   732,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,  1153,     0,     0,     0,     0,     0,
       0,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,  1154,     0,     0,     0,     0,   726,   727,
     728,     0,     0,   723,   724,   725,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,   723,   724,   725,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1155,     0,   731,   732,     0,     0,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,  1156,
       0,   731,   732,     0,     0,     0,     0,     0,   726,   727,
     728,     0,     0,   723,   724,   725,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1157,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1158,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,   731,   732,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,   723,   724,   725,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1159,     0,     0,     0,     0,     0,     0,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1160,
       0,     0,     0,     0,   726,   727,   728,     0,     0,   723,
     724,   725,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1161,     0,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,  1162,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1164,
       0,   726,   727,   728,     0,     0,     0,     0,     0,   723,
     724,   725,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1166,     0,     0,     0,
       0,     0,     0,   726,   727,   728,   723,   724,   725,     0,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1167,     0,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,   723,
     724,   725,   729,   730,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1171,     0,     0,     0,     0,     0,     0,     0,
       0,   731,   732,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1172,     0,     0,     0,     0,
       0,   726,   727,   728,     0,   723,   724,   725,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,  1173,     0,     0,     0,     0,     0,     0,   726,
     727,   728,   723,   724,   725,     0,     0,     0,     0,     0,
       0,   729,   730,     0,     0,     0,   723,   724,   725,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1174,     0,   731,   732,     0,   726,   727,   728,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   729,   730,
     726,   727,   728,   723,   724,   725,     0,     0,     0,     0,
       0,     0,   729,   730,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,     0,     0,  1175,
       0,   731,   732,     0,     0,     0,     0,   726,   727,   728,
       0,     0,     0,   723,   724,   725,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1176,     0,     0,     0,     0,     0,   726,   727,   728,
       0,   723,   724,   725,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,  1177,     0,
       0,     0,     0,     0,     0,   726,   727,   728,     0,     0,
     731,   732,     0,     0,     0,     0,     0,   729,   730,     0,
       0,     0,   723,   724,   725,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1178,     0,     0,     0,     0,     0,   731,   732,     0,
       0,     0,     0,     0,     0,     0,   726,   727,   728,   723,
     724,   725,     0,     0,     0,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1192,
       0,     0,     0,   726,   727,   728,     0,     0,     0,   723,
     724,   725,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,  1193,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   729,   730,   723,   724,   725,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,  1196,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,  1205,     0,
       0,   726,   727,   728,     0,     0,     0,     0,   723,   724,
     725,     0,     0,   729,   730,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1210,     0,     0,     0,     0,
       0,     0,   726,   727,   728,   723,   724,   725,     0,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1222,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,  1223,     0,     0,     0,     0,     0,     0,
       0,   731,   732,     0,     0,     0,     0,   726,   727,   728,
       0,     0,     0,   723,   724,   725,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,  1224,     0,     0,   726,   727,   728,
       0,     0,     0,     0,   723,   724,   725,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1226,     0,     0,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1387,     0,     0,     0,     0,     0,   731,   732,   726,
     727,   728,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   729,   730,   723,   724,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1398,
       0,     0,     0,     0,     0,     0,     0,   726,   727,   728,
       0,     0,   723,   724,   725,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1417,     0,     0,     0,     0,     0,   726,   727,   728,     0,
     723,   724,   725,     0,     0,     0,     0,     0,   729,   730,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,  1438,     0,   731,
     732,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,  1444,     0,   731,   732,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,  1445,   731,   732,     0,     0,
       0,     0,     0,   726,   727,   728,     0,     0,   723,   724,
     725,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1447,     0,     0,     0,
       0,     0,   726,   727,   728,     0,     0,     0,     0,     0,
     723,   724,   725,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,     0,     0,  1452,     0,     0,
       0,     0,   731,   732,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1454,     0,     0,     0,     0,
       0,     0,   726,   727,   728,   723,   724,   725,     0,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,   731,   732,  1480,     0,     0,     0,     0,   726,
     727,   728,     0,     0,   723,   724,   725,     0,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,   723,   724,
     725,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1483,     0,   731,   732,     0,     0,   726,   727,
     728,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     729,   730,   726,   727,   728,   723,   724,   725,     0,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
    1484,     0,   731,   732,     0,     0,     0,     0,     0,   726,
     727,   728,     0,     0,   723,   724,   725,     0,     0,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1492,     0,     0,     0,     0,     0,     0,   726,   727,
     728,   723,   724,   725,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1493,     0,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,   731,   732,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,   723,   724,   725,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1516,     0,     0,     0,     0,     0,     0,   731,
     732,     0,     0,     0,     0,     0,     0,     0,   726,   727,
     728,   723,   724,   725,     0,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1518,     0,     0,     0,     0,   726,   727,   728,     0,     0,
     723,   724,   725,     0,     0,     0,     0,   729,   730,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,  1519,     0,     0,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,  1541,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1543,     0,   726,   727,   728,     0,     0,     0,     0,     0,
     723,   724,   725,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,  1545,     0,     0,
       0,     0,     0,     0,   726,   727,   728,   723,   724,   725,
       0,     0,     0,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1547,     0,     0,     0,
       0,   726,   727,   728,     0,     0,     0,     0,     0,     0,
     723,   724,   725,   729,   730,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,  1608,     0,     0,     0,     0,     0,     0,
       0,     0,   731,   732,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1623,     0,     0,     0,
       0,     0,   726,   727,   728,     0,   723,   724,   725,     0,
       0,     0,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,   731,   732,  1624,     0,     0,     0,     0,     0,     0,
     726,   727,   728,   723,   724,   725,     0,     0,     0,     0,
       0,     0,   729,   730,     0,     0,     0,   723,   724,   725,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1626,     0,   731,   732,     0,   726,   727,   728,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   729,
     730,   726,   727,   728,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   729,   730,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,     0,     0,
    1643,     0,   731,   732,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,   723,   724,   725,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,  1671,     0,     0,     0,     0,     0,   726,   727,
     728,     0,   723,   724,   725,     0,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1674,
       0,     0,     0,     0,     0,     0,   726,   727,   728,     0,
       0,   731,   732,     0,     0,     0,     0,     0,   729,   730,
       0,     0,     0,   723,   724,   725,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1675,     0,     0,     0,     0,     0,   731,   732,
       0,     0,     0,     0,     0,     0,     0,   726,   727,   728,
     723,   724,   725,     0,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1677,     0,     0,     0,   726,   727,   728,     0,     0,     0,
     723,   724,   725,     0,     0,     0,   729,   730,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,   731,   732,  1679,     0,
       0,     0,     0,     0,   726,   727,   728,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,     0,     0,  1681,     0,     0,   733,   734,
     735,   736,   737,   738,   739,     0,   740,   731,   732,  1685,
       0,     0,   726,   727,   728,     0,     0,     0,     0,   723,
     724,   725,     0,     0,   729,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1687,     0,     0,     0,
       0,     0,     0,   726,   727,   728,   723,   724,   725,     0,
       0,     0,     0,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1721,     0,     0,     0,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,   733,   734,   735,   736,   737,   738,   739,
       0,   740,     0,     0,  1727,     0,     0,     0,     0,     0,
       0,     0,   731,   732,     0,     0,     0,     0,   726,   727,
     728,     0,     0,     0,   723,   724,   725,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1731,     0,     0,   726,   727,
     728,     0,     0,     0,     0,   723,   724,   725,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
     731,   732,  1749,     0,     0,     0,     0,     0,     0,   726,
     727,   728,     0,     0,     0,     0,   723,   724,   725,     0,
       0,   729,   730,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,  1750,     0,     0,     0,     0,     0,   731,   732,
     726,   727,   728,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   729,   730,   723,   724,   725,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   733,
     734,   735,   736,   737,   738,   739,     0,   740,   731,   732,
    1751,     0,     0,     0,     0,     0,     0,     0,   726,   727,
     728,     0,     0,   723,   724,   725,     0,     0,     0,     0,
     729,   730,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   731,
     732,  1752,     0,     0,     0,     0,     0,   726,   727,   728,
       0,   723,   724,   725,     0,     0,     0,     0,     0,   729,
     730,     0,     0,     0,     0,     0,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,  1753,     0,
     731,   732,     0,     0,     0,   726,   727,   728,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   729,   730,   723,
     724,   725,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,  1754,     0,   731,   732,
       0,     0,     0,   726,   727,   728,     0,     0,     0,     0,
     723,   724,   725,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,     0,     0,  1775,   731,   732,     0,
       0,     0,     0,     0,   726,   727,   728,     0,     0,   723,
     724,   725,     0,     0,     0,     0,   729,   730,   723,   724,
     725,     0,     0,     0,     0,     0,   733,   734,   735,   736,
     737,   738,   739,     0,   740,   731,   732,  1776,     0,     0,
       0,     0,     0,   726,   727,     0,     0,     0,     0,     0,
       0,     0,   726,     0,     0,   729,   730,     0,     0,     0,
       0,     0,     0,     0,   729,   730,     0,   733,   734,   735,
     736,   737,   738,   739,     0,   740,     0,     0,  1777,     0,
       0,     0,     0,   731,   732,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   733,   734,   735,   736,   737,
     738,   739,     0,   740,   731,   732,  1792,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   733,   734,   735,   736,   737,   738,
     739,     0,   740,   731,   732,  1793,     0,     0,     0,     0,
       0,     0,   731,   732,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   733,   734,   735,   736,   737,   738,   739,     0,
     740,     0,     0,  1794,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,     0,
       0,  1796,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   733,   734,   735,   736,   737,   738,   739,     0,   740,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     733,   734,   735,   736,   737,   738,   739,     0,   740,   733,
     734,   735,   736,   737,   738,   739,     0,   740
};

static const yytype_int16 yycheck[] =
{
       1,   717,     3,     4,     5,   230,     7,     8,   230,    10,
     230,    93,    94,    14,   228,   229,     0,   353,   354,   117,
      47,    48,    49,    93,    94,    26,    27,    28,    29,    30,
      31,   353,   130,    34,   368,   133,    37,    21,   372,    40,
      41,    42,    43,     0,    45,   353,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,   234,     0,    60,
      44,   353,    63,   353,    21,    66,    67,    68,    69,    70,
     228,   229,    73,    74,    75,    76,   368,    78,    79,    21,
      81,    82,    83,   375,   353,    86,     0,    44,    93,    94,
      91,    92,    93,    94,    95,     0,   228,   229,    20,   234,
     353,     0,    44,     0,   353,   370,   375,    21,   373,   353,
     111,     0,   843,   353,    41,   353,    21,     0,   849,   353,
     121,   353,    21,    44,    21,    47,    48,    49,   369,   370,
      44,   847,    21,   368,   135,   360,   137,   375,   360,    44,
     360,   375,   582,   375,   584,    44,   370,    44,   367,   373,
     372,   353,   354,   371,   368,    44,   353,   354,   372,    81,
      82,    83,   353,   354,   368,   228,   229,   230,   231,   232,
     233,    93,    94,     0,   353,   354,   353,   354,   228,   229,
     230,   231,   232,   233,   353,   354,   368,    47,    48,    49,
     353,   354,   369,   368,    21,   368,   201,   202,   353,   354,
     228,   229,   228,   229,   230,   231,   232,   233,   369,   370,
     368,   369,   369,   370,   372,   228,   229,    44,   353,   354,
     369,   370,   368,    47,    48,    49,   369,   370,   369,   370,
     368,   368,   368,    44,   368,   368,   368,   369,   368,   368,
     372,   368,   368,   368,   228,   229,   368,   368,   368,    31,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,    47,    48,    49,   368,   368,
     271,   228,   229,   368,   368,   368,    47,    48,    49,   201,
     202,   363,   364,   368,   366,   368,   228,   229,   368,   368,
      32,   361,   362,   363,   364,   368,   366,   360,   374,    81,
      82,    83,   368,   368,   368,    47,    48,    49,   368,   372,
     360,    93,    94,   368,   228,   229,   368,   368,   353,   368,
     368,   368,   368,   228,   229,   368,   368,   368,   368,   228,
     229,   358,   359,   360,   360,   362,   368,   368,   368,    81,
      82,    83,   369,   370,   345,   368,   368,   368,   368,   353,
     368,    93,    94,   368,   368,    21,   361,   362,   363,   364,
     368,   366,   368,   353,   376,   368,   368,   360,   368,   368,
     368,   368,   368,   368,   368,   110,   362,   368,   370,   368,
     368,   110,   369,   367,   368,   370,   366,   371,   372,   372,
      47,    48,    49,   369,   369,   369,   377,   369,   369,   372,
     370,   368,   368,   368,   368,   373,   369,   369,   377,   370,
     367,   368,   370,   369,   371,   372,   370,   369,   369,   201,
     202,   370,   370,   370,   369,   367,   368,   370,   369,   371,
     372,   370,   370,   369,   369,   369,   358,   359,   360,   361,
     362,   363,   364,   372,   366,   370,    47,    48,    49,   372,
     370,   369,   369,   367,   368,   456,   457,   371,   372,   201,
     202,   369,   367,   369,   465,   376,   371,   468,   367,   470,
     367,   373,   371,   474,   371,   476,   370,   478,   367,   480,
     373,   373,   371,   369,   485,   369,   110,   488,   370,   370,
     491,   370,   493,   370,   369,   496,   370,   498,   358,   359,
     360,   369,   362,   504,   369,   506,   507,   508,   509,   369,
     370,   512,  1228,   369,   515,   369,   369,   518,   369,   520,
     110,  1035,   619,   615,   525,   623,   527,  1326,    -1,   530,
      -1,   532,    -1,   534,   358,   359,   360,   538,   362,   540,
     367,   368,    -1,   544,   371,   372,   547,    -1,    -1,   550,
      -1,   552,    -1,    -1,   555,    -1,    -1,   558,    -1,    -1,
     561,    -1,   563,    -1,    -1,   566,    -1,    -1,   569,    -1,
      -1,   572,    -1,   574,    -1,   576,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   358,   359,   360,
     591,   362,   593,    -1,   595,    -1,   597,    -1,   369,   370,
      -1,    -1,    47,    48,    49,    -1,    -1,   608,   609,   610,
     611,   612,   613,   614,   615,   616,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    47,    48,    49,   629,   630,
     631,   632,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,   655,   656,   657,    -1,   659,   660,
     661,   662,   663,    -1,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   674,    -1,    -1,    -1,   678,   679,   680,
     681,    -1,    -1,   684,   685,   612,    -1,   688,   689,    -1,
     691,    -1,   693,    -1,    -1,   696,    47,    48,    49,   700,
     701,   358,   359,   360,    -1,   362,    -1,   708,   709,   710,
     711,   712,   369,   370,   715,   716,   717,   718,    -1,    -1,
      -1,    -1,   723,   724,   725,   726,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   740,
      47,    48,    49,    -1,    -1,    -1,   747,   748,   749,    -1,
      -1,   752,    -1,   754,    -1,   756,    -1,   358,   359,   360,
     761,   362,   763,    -1,   765,    -1,   767,    -1,   369,   370,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,   779,   780,
     781,    -1,   783,   784,    -1,   786,   787,    -1,   789,   790,
      -1,    -1,   793,    -1,    -1,    -1,   797,    -1,   799,    -1,
     801,    -1,   803,    -1,    -1,    -1,   807,    -1,   809,    -1,
     811,    -1,    -1,    -1,   815,    -1,    -1,    -1,    -1,    -1,
      -1,   822,    -1,    -1,   825,    -1,   827,    -1,    -1,    -1,
     831,    -1,    -1,    -1,    47,    48,    49,   838,    -1,   840,
      -1,    -1,    -1,    -1,    -1,    -1,   847,    -1,    -1,    -1,
     851,    -1,   853,    -1,   855,    -1,   857,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,   868,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,   813,    47,    48,    49,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,    -1,   362,    -1,    -1,
      -1,    81,    82,    83,   369,   370,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,   358,   359,   360,    -1,
     362,    -1,    -1,    -1,    -1,    -1,    -1,   369,   370,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   964,   965,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    47,    48,    49,   201,   202,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,   358,   359,   360,
      -1,   362,    -1,   201,   202,    -1,    -1,    -1,   369,   370,
      -1,    -1,    -1,    -1,    -1,    -1,  1027,    -1,  1029,    -1,
    1031,    47,    48,    49,  1035,  1036,    -1,  1038,    -1,  1040,
      -1,   201,   202,    -1,    47,    48,    49,    -1,    -1,    -1,
    1051,   358,   359,   360,    -1,   362,    -1,    47,    48,    49,
    1061,    -1,   369,   370,  1065,    -1,  1067,    -1,  1069,    -1,
    1071,    47,    48,    49,  1075,    -1,  1077,    -1,  1079,    -1,
      -1,    -1,    -1,  1084,   358,   359,   360,    -1,   362,  1090,
      -1,    -1,  1093,    -1,  1095,   369,   370,  1098,    -1,    -1,
      -1,    -1,  1103,    -1,  1105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1116,    -1,  1118,    -1,  1120,
      -1,  1122,    -1,  1124,  1125,    -1,    -1,  1128,    -1,  1130,
    1131,  1132,    -1,  1134,  1135,    47,    48,    49,    -1,  1140,
    1141,  1142,    -1,    -1,  1145,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,  1081,    -1,    -1,   370,    -1,    -1,
     373,    -1,    -1,    -1,    47,    48,    49,    -1,  1169,  1170,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,  1182,   370,  1184,    -1,   373,  1187,    -1,    -1,  1190,
      -1,    -1,    -1,    -1,  1195,    -1,  1197,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
     370,  1212,    -1,   373,  1215,    -1,    -1,    -1,    -1,    -1,
    1221,    -1,     1,    -1,  1225,    -1,    -1,  1228,    47,    48,
      49,  1232,    -1,    47,    48,    49,    -1,  1238,    -1,  1240,
      -1,  1242,    -1,  1244,  1245,    -1,    -1,  1248,    -1,  1250,
      -1,    -1,  1253,    -1,  1255,    -1,  1257,    -1,    -1,    -1,
    1261,    -1,    -1,  1264,  1265,  1266,    45,  1268,  1269,    -1,
    1271,  1272,    -1,  1274,  1275,    47,    48,    49,    -1,  1280,
      -1,  1282,   358,   359,   360,  1286,   362,    -1,  1289,    -1,
    1291,    -1,  1293,    -1,   370,    -1,   358,   359,   360,    78,
     362,    -1,   358,   359,   360,  1306,   362,   369,  1309,    -1,
      -1,   358,   359,   360,   370,   362,    -1,  1318,    -1,  1320,
      -1,  1322,    -1,   370,    -1,    -1,  1253,    -1,    -1,  1330,
    1331,  1332,  1333,  1334,    -1,    -1,    -1,  1338,  1339,    -1,
      -1,    -1,   358,   359,   360,    -1,   362,    47,    48,    49,
      -1,    -1,    -1,    -1,   370,   358,   359,   360,    -1,   362,
      -1,    -1,    -1,  1364,    -1,  1366,    -1,   370,   358,   359,
     360,    -1,   362,    -1,    -1,    -1,    47,    48,    49,    -1,
     370,    -1,   358,   359,   360,    -1,   362,  1388,    -1,  1390,
      -1,  1392,    -1,    -1,   370,  1396,    -1,    -1,    -1,    -1,
      -1,    -1,  1403,    47,    48,    49,  1407,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,  1419,    -1,
      -1,  1422,    93,    94,    -1,  1426,    -1,    -1,    -1,  1430,
      -1,  1432,    -1,  1434,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    -1,  1446,   358,   359,   360,  1450,
     362,    -1,    -1,    -1,  1455,    -1,  1457,   369,  1459,    -1,
    1461,    -1,    -1,    -1,    -1,  1392,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1475,   358,   359,   360,    -1,   362,
    1481,  1482,    -1,    -1,    -1,  1486,   369,  1488,    -1,  1490,
    1491,    47,    48,    49,  1495,    -1,    -1,  1498,    -1,    -1,
    1501,    -1,    -1,  1504,    -1,  1506,  1507,    -1,    -1,    -1,
      -1,    -1,    -1,  1514,    -1,    -1,    47,    48,    49,    -1,
      -1,    -1,    -1,  1524,    -1,  1526,    -1,    -1,    -1,  1530,
     201,   202,    -1,  1534,    -1,  1536,  1537,    -1,  1539,   358,
     359,   360,    -1,   362,   358,   359,   360,    -1,   362,    -1,
      -1,   370,  1553,    -1,    -1,  1556,   370,  1558,    -1,  1560,
      -1,  1562,    47,    48,    49,    -1,  1567,    -1,  1569,    -1,
    1571,    -1,    -1,    -1,    -1,  1576,  1577,  1578,  1579,  1580,
      -1,    -1,  1583,  1584,    -1,    -1,   358,   359,   360,    -1,
     362,    47,    48,    49,    -1,    -1,    -1,    -1,   370,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1534,  1609,    -1,
      -1,    -1,  1613,    -1,    -1,    -1,  1617,    -1,    -1,    -1,
    1621,    -1,    -1,    -1,    -1,    -1,    -1,  1628,    -1,  1630,
      -1,    -1,    -1,  1634,    -1,    -1,  1637,    -1,    -1,  1640,
      -1,    -1,    -1,    -1,  1645,    -1,  1647,    -1,  1649,    -1,
      -1,  1652,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,    -1,   362,    -1,  1665,    -1,    47,    48,    49,  1670,
     370,    -1,  1673,    -1,    -1,  1676,    -1,    -1,    -1,    -1,
      47,    48,    49,  1684,    -1,    -1,  1613,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,  1698,    -1,   370,
      -1,    -1,   373,  1704,    -1,  1706,    -1,  1708,    -1,  1710,
      -1,  1712,    -1,  1714,   358,   359,   360,    -1,   362,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   370,    -1,  1729,    -1,
      -1,    -1,    -1,    -1,  1735,    -1,    -1,  1738,    -1,    -1,
    1741,    -1,  1743,    -1,  1745,    -1,  1747,     1,     2,   358,
     359,   360,    -1,   362,    47,    48,    49,  1758,    -1,    -1,
      -1,   370,    -1,  1764,    -1,  1766,    -1,  1768,  1695,    -1,
      -1,  1772,    26,    27,    -1,    29,    -1,  1778,    -1,  1780,
      34,  1782,    -1,    -1,  1785,    -1,    -1,    -1,    42,  1790,
      44,    45,    46,   572,    -1,    -1,    -1,  1724,    -1,    -1,
      -1,    -1,   358,   359,   360,    59,   362,    -1,    -1,    -1,
      -1,    65,    66,   369,    68,    69,    -1,    -1,    -1,    -1,
      74,    75,    -1,    77,    78,    -1,    80,   358,   359,   360,
      -1,   362,    47,    48,    49,   614,   615,    -1,   369,    -1,
      -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,   358,   359,   360,    -1,   362,    93,    94,
     659,    -1,   661,    -1,   369,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,   358,   359,   360,    -1,   362,    -1,    -1,   688,
      -1,    -1,   691,   369,   693,    -1,    -1,    -1,     3,     4,
      -1,    -1,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    -1,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,   358,   359,   360,
      -1,   362,    -1,    -1,    -1,    -1,   201,   202,   369,    -1,
      -1,   358,   359,   360,    -1,   362,    -1,    -1,    -1,    -1,
      85,    86,   369,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,
     105,   106,   107,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,
     135,   136,    -1,   138,    -1,    -1,    -1,    -1,    -1,   838,
     145,    -1,    -1,    -1,    -1,   358,   359,   360,    -1,   362,
      -1,    -1,    -1,    -1,    -1,    -1,   369,    -1,    47,    48,
      49,    -1,    -1,    -1,   169,    -1,    -1,    -1,   173,   174,
     175,   176,   177,    -1,    -1,    -1,    -1,   182,   183,    -1,
      47,    48,    49,   188,   189,    -1,   191,   192,    -1,    -1,
     195,    -1,   197,    -1,    -1,   200,    -1,    -1,    -1,   204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   214,
     215,   216,    -1,    -1,   219,   220,   221,   222,   223,   224,
     225,    -1,   227,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,   240,   241,   242,   243,    47,
      48,    49,   247,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    -1,    -1,
      47,    48,    49,    81,    82,    83,   358,   359,   360,    -1,
     362,    -1,    -1,   457,    -1,    93,    94,   369,   358,   359,
     360,   465,   362,    -1,   468,    -1,   470,    -1,    -1,   369,
      -1,    -1,   476,    -1,   478,    -1,   480,    -1,    -1,    -1,
      -1,   485,    -1,    -1,   488,    -1,    -1,   491,    -1,   493,
      -1,    -1,   496,    -1,   498,    -1,    -1,    -1,    -1,    -1,
     504,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   512,    -1,
     345,   515,    -1,    -1,   518,    -1,   520,    -1,   353,   354,
      -1,   525,   357,   527,    -1,    -1,   530,    -1,   532,    -1,
     534,   358,   359,   360,   538,   362,   540,    47,    48,    49,
     544,    -1,   369,   547,    -1,    -1,   550,    -1,   552,    -1,
      -1,   555,    -1,    -1,   558,    -1,    -1,   561,    -1,   563,
      -1,    -1,   566,   201,   202,   569,    -1,    -1,   572,    -1,
     574,    -1,   576,    -1,  1103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,   591,    -1,   593,
      -1,   595,    -1,   597,    -1,  1124,  1125,    -1,    47,    48,
      49,  1130,  1131,    -1,   608,   609,   610,   611,   612,    -1,
     614,   615,    47,    48,    49,    -1,  1145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   632,   633,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   358,
     359,   360,    -1,   362,    -1,    47,    48,    49,    -1,    -1,
     369,    -1,    -1,    -1,    -1,   659,    -1,   661,    -1,    -1,
     664,   358,   359,   360,    -1,   362,  1195,    -1,  1197,    -1,
      -1,   675,   369,   677,    -1,   679,   680,   681,    -1,    81,
      82,    83,    -1,   687,   688,    -1,    -1,   691,   692,   693,
     694,    93,    94,    47,    48,    49,    -1,   701,   702,   703,
     704,   705,   706,   707,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   715,    -1,   717,    47,    48,    49,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,    -1,   370,   737,    -1,   373,    -1,   741,   742,   743,
     744,   745,   746,   747,    -1,    -1,    -1,    -1,    81,    82,
      83,   358,   359,   360,    -1,   362,    -1,    -1,    -1,    -1,
      93,    94,   369,    -1,    -1,   769,    -1,   771,    -1,   773,
      -1,   775,    -1,    -1,    -1,    -1,   780,    -1,    -1,   783,
    1309,    -1,   786,    -1,    -1,   789,    -1,    -1,    -1,    -1,
      -1,   795,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,   805,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1338,
    1339,    -1,    -1,   817,    -1,    -1,   820,    -1,   822,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   830,    -1,    -1,   833,
      -1,    -1,    -1,    -1,   838,    -1,    -1,    -1,   842,    -1,
      47,    48,    49,   847,    -1,    -1,    -1,    -1,   358,   359,
     360,    -1,   362,    -1,    -1,    -1,    47,    48,    49,   369,
      -1,    -1,    -1,    -1,   868,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,   877,    81,    82,    83,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      81,    82,    83,   358,   359,   360,    -1,   362,    -1,    -1,
      -1,    -1,    93,    94,   369,    -1,    47,    48,    49,   358,
     359,   360,    -1,   362,    -1,    -1,    -1,    -1,    -1,    -1,
     369,  1450,    -1,   358,   359,   360,    -1,   362,    -1,    -1,
      -1,    -1,  1461,    -1,   369,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    47,    48,    49,  1475,    -1,    47,    48,
      49,    -1,    93,    94,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,
      -1,   373,    -1,    -1,    -1,  1504,    -1,    -1,  1507,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
      -1,    47,    48,    49,   358,   359,   360,    -1,   362,    -1,
     201,   202,    -1,    -1,    -1,   369,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,  1043,
     373,  1045,    -1,  1047,    -1,  1049,    47,    48,    49,    -1,
      -1,    47,    48,    49,  1583,  1584,    -1,    -1,    -1,  1063,
     201,   202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1073,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1086,    -1,  1088,    -1,  1090,    -1,    47,    48,
      49,    -1,    -1,  1097,    -1,    -1,    -1,  1101,    -1,  1103,
      -1,    -1,    -1,  1107,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    93,    94,
    1124,  1125,    81,    82,    83,    -1,  1130,  1131,    -1,    -1,
      47,    48,    49,    -1,    93,    94,    -1,    -1,    47,    48,
      49,  1145,  1146,  1147,    -1,    -1,    -1,    -1,    81,    82,
      83,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      93,    94,    -1,   370,    -1,    -1,   373,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,
    1184,    -1,   373,  1187,    -1,    -1,  1190,    -1,   358,   359,
     360,  1195,   362,  1197,  1198,  1199,  1200,    -1,  1202,   369,
    1204,    47,    48,    49,    -1,    -1,    -1,    -1,  1212,    -1,
      47,    48,    49,  1217,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,  1228,   366,   201,   202,    -1,   370,
      -1,    -1,   373,    -1,    -1,    81,    82,    83,    47,    48,
      49,    -1,   201,   202,    47,    48,    49,    93,    94,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,    -1,   362,   358,
     359,   360,    -1,   362,    -1,   369,    -1,    -1,   201,   202,
     369,    47,    48,    49,    -1,    -1,  1280,    -1,    -1,    -1,
    1284,    -1,  1286,    -1,    -1,    47,    48,    49,    -1,    -1,
     358,   359,   360,    -1,   362,    -1,    -1,  1301,   358,   359,
     360,   369,   362,    -1,    -1,  1309,    47,    48,    49,   369,
      -1,    -1,   358,   359,   360,    -1,   362,    -1,    -1,    81,
      82,    83,  1326,   369,    -1,    -1,  1330,  1331,  1332,  1333,
    1334,    93,    94,    -1,  1338,  1339,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,   358,   359,   360,
      -1,   362,   358,   359,   360,    -1,   362,    -1,   369,    -1,
      -1,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,    -1,   370,    -1,    -1,   373,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   370,    -1,    -1,   373,  1419,    -1,    -1,    -1,    -1,
    1424,    -1,  1426,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,  1441,    -1,   201,
     202,   358,   359,   360,    -1,   362,  1450,    -1,    -1,   358,
     359,   360,   369,   362,    -1,    -1,    -1,  1461,  1462,    -1,
     369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1475,  1476,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,  1495,    -1,    -1,  1498,    -1,    -1,  1501,    -1,    -1,
    1504,    -1,    -1,  1507,    -1,    -1,    -1,    -1,    -1,    -1,
    1514,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   358,   359,   360,   370,   362,    -1,   373,    -1,    -1,
      -1,    -1,   369,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,  1556,   362,    -1,   358,   359,   360,    -1,   362,
     369,    -1,    -1,    -1,    -1,    -1,   369,    -1,    -1,    -1,
      -1,    -1,  1576,  1577,  1578,  1579,  1580,    -1,    -1,  1583,
    1584,    -1,   358,   359,   360,    -1,   362,    -1,     5,     6,
       7,    -1,    -1,   369,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,
      -1,   373,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
      37,   362,    -1,    -1,    -1,    -1,  1630,    -1,   369,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,  1702,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,  1732,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    47,    48,    49,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,   358,   359,   360,
      -1,   362,    -1,    -1,    -1,    -1,    -1,    -1,   369,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,   358,   359,   360,    -1,   362,    -1,
      -1,    -1,    -1,    -1,    -1,   369,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,   370,    -1,    -1,    -1,    -1,    -1,    -1,
     377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,   358,   359,   360,
     137,   362,   139,   140,   141,   142,   143,   144,   369,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    47,    48,    49,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,   370,    -1,    -1,    -1,    -1,    -1,    -1,
     377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,   358,   359,   360,
     137,   362,   139,   140,   141,   142,   143,   144,   369,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,    -1,   362,    -1,   226,
      -1,   228,   229,    -1,   369,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
     207,   208,   209,   210,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,   372,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,     5,     6,
       7,    -1,    -1,    -1,   361,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    -1,    -1,    -1,    84,    -1,    -1,
      87,    88,    89,    90,    -1,    92,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,    -1,   103,   104,    -1,    -1,
      -1,   108,   109,   110,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,    -1,   131,    -1,   133,    -1,    -1,    -1,
     137,    -1,   139,   140,   141,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   179,   180,   181,    -1,    -1,   184,   185,   186,
     187,    -1,    -1,   190,    -1,    -1,   193,   194,    -1,   196,
      -1,   198,   199,    -1,    -1,    -1,   203,    -1,   205,   206,
      -1,   208,   209,    -1,   211,   212,   213,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   226,
      -1,   228,   229,    -1,    -1,    -1,    -1,   234,   235,   236,
     237,   238,   239,    -1,    -1,    -1,    -1,   244,   245,   246,
      -1,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,    -1,    -1,
     347,   348,   349,   350,   351,   352,   353,   354,    -1,    -1,
      -1,    -1,    -1,    -1,   361,    -1,    -1,     3,     4,    -1,
      -1,   368,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    -1,    -1,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    85,
      86,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,   102,    -1,    -1,   105,
     106,   107,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,   135,
     136,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,   182,   183,    -1,    -1,
      -1,    -1,   188,   189,    -1,   191,   192,    -1,    -1,   195,
      -1,   197,    -1,    -1,   200,    -1,    -1,    -1,   204,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,   219,   220,   221,   222,   223,   224,   225,
      -1,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   240,   241,   242,   243,    -1,    -1,
      -1,   247,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,     3,     4,    -1,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    51,    -1,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   345,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   353,   354,    85,
      86,    81,    82,    83,    -1,    91,   201,   202,    -1,    -1,
      -1,    -1,    -1,    93,    94,   101,   102,    -1,    -1,   105,
     106,   107,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    47,    48,    49,    -1,    -1,    -1,
     126,    -1,    -1,    -1,   130,    -1,   132,    -1,   134,   135,
     136,    -1,   138,    -1,    -1,    -1,    -1,    -1,    -1,   145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,   169,    -1,    -1,    -1,   173,   174,   175,
     176,   177,    -1,    -1,    -1,    -1,   182,   183,    -1,    -1,
      -1,    -1,   188,   189,    -1,   191,   192,    -1,    -1,   195,
      -1,   197,    -1,    -1,   200,    -1,    -1,    -1,   204,    -1,
      -1,   201,   202,    -1,    -1,    -1,    -1,    -1,   214,   215,
     216,    -1,    -1,   219,   220,   221,   222,   223,   224,   225,
      -1,   227,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   240,   241,   242,   243,    47,    48,
      49,   247,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,    -1,   201,   202,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,   345,
      -1,    -1,    93,    94,    47,    48,    49,   353,   354,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    93,    94,    47,
      48,    49,    -1,   373,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,    -1,    -1,    -1,    -1,
     373,    81,    82,    83,    -1,    -1,    47,    48,    49,    -1,
     201,   202,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,   201,   202,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   201,   202,    -1,   373,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,    -1,    -1,    -1,   373,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    81,    82,    83,
     201,   202,   373,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,    -1,    -1,    -1,    -1,   373,   201,   202,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    47,    48,    49,    -1,    -1,    -1,
     373,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,    -1,    -1,    -1,    -1,   373,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
      -1,    -1,    -1,   373,    -1,    -1,    -1,   201,   202,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,    47,    48,
      49,    -1,   373,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    81,    82,    83,
      -1,    -1,   373,    47,    48,    49,    -1,    -1,    -1,    93,
      94,    -1,    81,    82,    83,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    93,    94,    -1,    -1,   201,   202,
     373,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,    -1,   201,
     202,   373,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    47,    48,    49,    -1,   201,   202,   373,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,   201,   202,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    93,
      94,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,   370,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   369,   370,    -1,
      -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,   370,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
     369,   370,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,   370,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,   370,   201,   202,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
     369,   370,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    93,    94,   369,   370,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,   370,    -1,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,   370,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,   370,    47,    48,    49,    -1,    -1,    -1,
      -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,   369,   370,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,   370,   201,   202,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    47,    48,    49,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,   369,
     370,   201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,   370,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,   370,    -1,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    81,    82,    83,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    93,    94,   369,   370,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
     370,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,   370,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,   370,    -1,    -1,
      -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,   370,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,   370,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,   370,   201,   202,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
     369,   370,   201,   202,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,   370,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,   370,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,   370,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,   370,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   369,   370,    -1,
      -1,    -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,   370,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,   370,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,   369,   370,   201,   202,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,   370,   201,   202,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,   370,   201,   202,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,   369,   370,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,   369,   370,    -1,    -1,    -1,   201,   202,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,   370,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,   370,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,   369,   370,
     201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,   370,   201,   202,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,   370,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,   370,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,   369,   370,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,   370,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,   369,   370,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,   369,   370,    -1,    -1,    -1,    -1,   201,
     202,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,   370,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,   369,   370,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,   369,
     370,   201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   369,   370,   201,
     202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   369,   370,   201,
     202,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,   369,   370,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,   369,
     370,    -1,    -1,    -1,   201,   202,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,   370,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,   370,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,   370,   201,   202,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,   370,   201,   202,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,   369,   370,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,    -1,   370,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,   370,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
     370,    -1,    -1,    -1,    -1,   201,   202,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,    -1,   370,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,    -1,   370,   201,   202,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,    -1,   370,   201,   202,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,   201,   202,   370,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,    -1,   370,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,    -1,   370,    -1,    -1,    -1,
     201,   202,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
      -1,   370,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   370,    -1,    -1,    -1,    -1,   201,   202,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,    -1,   370,   201,   202,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,    -1,   370,   201,   202,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,   201,   202,   370,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,    -1,   370,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,    -1,   370,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,    -1,   370,    -1,    -1,    -1,    -1,   201,   202,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
      -1,   370,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,   201,
     202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,    -1,   370,   201,   202,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,   201,   202,   370,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,    -1,   370,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,   370,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,    -1,   370,    -1,    -1,    -1,    -1,   201,   202,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
      -1,   370,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,    -1,   370,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,
     201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,    -1,   370,   201,   202,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,   201,   202,   370,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,    -1,   370,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,    -1,   370,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,    -1,   370,    -1,    -1,    -1,    -1,   201,
     202,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,    -1,   370,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,   370,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
     370,   201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,   201,
     202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,   201,   202,   370,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
     370,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,    -1,   370,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,    -1,   370,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,    -1,   370,    -1,    -1,    -1,    -1,
     201,   202,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,   370,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   370,   201,   202,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,
     201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,   201,   202,   370,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   370,    -1,    -1,    -1,   201,   202,    -1,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,    -1,   370,    -1,    -1,    -1,
      -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,    -1,   370,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,    -1,   370,   201,   202,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
     370,   201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,   201,   202,
     370,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
      -1,   370,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,    -1,   370,    -1,    -1,    -1,   201,   202,    -1,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,    -1,
      -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,    -1,   370,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,    -1,   370,   201,   202,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   370,   201,   202,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,   201,
     202,   370,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,    -1,   370,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,    -1,   370,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
      -1,   370,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,    -1,   370,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,    -1,
      -1,    -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,    -1,   370,   201,   202,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,    -1,   370,   201,   202,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
     201,   202,   370,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,   370,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,    -1,   370,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,    -1,   370,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,   370,
      -1,    -1,    -1,    -1,   201,   202,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,    -1,   370,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,    -1,   370,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,    -1,   370,   201,   202,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,    -1,   370,   201,   202,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,   201,   202,   370,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,    -1,   370,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,    -1,   370,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,    -1,
     370,    -1,    -1,    -1,    -1,   201,   202,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,    -1,   370,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,    -1,   370,   201,   202,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,    -1,   370,   201,   202,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,   201,   202,   370,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,    -1,   370,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,    -1,   370,    -1,    -1,    -1,
     201,   202,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,    -1,   370,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
      -1,   370,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
      -1,   370,    -1,    -1,    -1,    -1,   201,   202,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,    -1,   370,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,    -1,
     370,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,    -1,   201,   202,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,    -1,   201,   202,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,   201,   202,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    47,    48,    49,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,    -1,    -1,    -1,
      -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,   369,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,    -1,   201,   202,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,   369,
      -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,   369,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,
      -1,    81,    82,    83,    -1,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,   369,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    47,    48,    49,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,    -1,   201,   202,    -1,    81,    82,    83,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      81,    82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,    -1,    -1,   369,
      -1,   201,   202,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,   369,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
     201,   202,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,    49,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,   369,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   201,   202,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,   369,    -1,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,    -1,    -1,    -1,    -1,    -1,   201,   202,    81,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   369,    -1,   201,
     202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,   369,    -1,   201,   202,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,   369,   201,   202,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    47,    48,
      49,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,    -1,    -1,   369,    -1,    -1,
      -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    47,    48,
      49,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,    -1,   201,   202,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    81,    82,    83,    47,    48,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
     369,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      47,    48,    49,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    47,    48,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,
      47,    48,    49,    93,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    -1,    81,    82,    83,    -1,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,   201,   202,   369,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    82,    83,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    47,    48,    49,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,    -1,   201,   202,    -1,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    81,    82,    83,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,    -1,    -1,
     369,    -1,   201,   202,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,   369,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,    -1,
      -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,   201,   202,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,   201,   202,   369,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,    -1,    -1,   369,    -1,    -1,   358,   359,
     360,   361,   362,   363,   364,    -1,   366,   201,   202,   369,
      -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    82,    83,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,   358,   359,   360,   361,   362,   363,   364,
      -1,   366,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    81,    82,
      83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
     201,   202,   369,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      82,    83,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,   201,   202,
      81,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    47,    48,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   358,
     359,   360,   361,   362,   363,   364,    -1,   366,   201,   202,
     369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,
      83,    -1,    -1,    47,    48,    49,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   201,
     202,   369,    -1,    -1,    -1,    -1,    -1,    81,    82,    83,
      -1,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,   369,    -1,
     201,   202,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    47,
      48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,    -1,   201,   202,
      -1,    -1,    -1,    81,    82,    83,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,    -1,    -1,   369,   201,   202,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    -1,    47,
      48,    49,    -1,    -1,    -1,    -1,    93,    94,    47,    48,
      49,    -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,
     362,   363,   364,    -1,   366,   201,   202,   369,    -1,    -1,
      -1,    -1,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,   358,   359,   360,
     361,   362,   363,   364,    -1,   366,    -1,    -1,   369,    -1,
      -1,    -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,
     363,   364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   358,   359,   360,   361,   362,   363,
     364,    -1,   366,   201,   202,   369,    -1,    -1,    -1,    -1,
      -1,    -1,   201,   202,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   358,   359,   360,   361,   362,   363,   364,    -1,
     366,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,    -1,
      -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   358,   359,   360,   361,   362,   363,   364,    -1,   366,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     358,   359,   360,   361,   362,   363,   364,    -1,   366,   358,
     359,   360,   361,   362,   363,   364,    -1,   366
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     3,     4,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    33,    34,    35,    36,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    50,    51,
      85,    86,    91,   101,   102,   105,   106,   107,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   126,   130,   132,
     134,   135,   136,   138,   145,   169,   173,   174,   175,   176,
     177,   182,   183,   188,   189,   191,   192,   195,   197,   200,
     204,   214,   215,   216,   219,   220,   221,   222,   223,   224,
     225,   227,   240,   241,   242,   243,   247,   259,   260,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   270,   271,
     272,   345,   353,   354,   357,   379,   381,   382,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   399,   400,   401,   402,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,     5,
       6,     7,    37,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    84,    87,    88,    89,    90,    92,    95,    96,    97,
      98,    99,   100,   103,   104,   108,   109,   110,   120,   121,
     122,   123,   124,   125,   127,   128,   129,   131,   133,   137,
     139,   140,   141,   142,   143,   144,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   170,
     171,   178,   179,   180,   181,   184,   185,   186,   187,   190,
     193,   194,   196,   198,   199,   203,   205,   206,   208,   209,
     211,   212,   213,   226,   228,   229,   234,   235,   236,   237,
     238,   239,   244,   245,   246,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   347,   348,
     349,   350,   351,   352,   353,   354,   361,   368,   512,   513,
     514,   353,   354,   513,   514,   368,   513,   514,   368,   513,
     368,   513,   353,   368,   375,   508,   368,   513,   368,   513,
     368,   513,   513,   353,   353,   368,   513,   514,   368,   513,
     514,   368,   513,   368,   513,   514,   368,   513,   368,   513,
     353,   353,   513,   514,   368,   513,   353,   354,   353,   354,
     513,   353,   368,   508,   513,   368,   513,   514,   368,   513,
     368,   514,   512,   514,   513,   368,   513,   368,   513,   513,
     368,   513,   368,   513,   368,   513,   513,   513,   368,   513,
     368,   513,   514,   513,   368,   513,   514,   368,   513,   514,
     368,   513,   368,   513,   514,   368,   513,   514,   368,   513,
     353,   368,   513,   368,   513,   514,   368,   513,   514,   368,
     513,   514,   368,   512,   368,   513,   368,   514,   513,   513,
     513,   353,   354,   513,   353,   353,   234,   353,   354,   383,
     385,   368,   513,   368,   513,   368,   513,   368,   513,   513,
     514,   417,   419,   421,   422,   513,   228,   229,   230,   231,
     232,   233,   360,   372,   230,   360,   372,     0,   367,   380,
      44,   386,    44,   371,   513,   386,   386,   513,   513,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   513,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     513,   368,   368,   368,   368,   368,   368,   353,   353,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   228,   229,   368,   372,   368,   372,   513,
     513,   514,   374,    47,    48,    49,    81,    82,    83,    93,
      94,   201,   202,   358,   359,   360,   361,   362,   363,   364,
     366,    47,    48,    49,   358,   359,   360,   362,   372,   372,
     370,   513,   370,   513,   370,   513,   370,   353,   509,   513,
     513,   370,   513,   370,   513,   370,    20,   360,   513,   370,
     513,   370,   513,   370,   513,   370,   369,   369,   513,   370,
     368,   372,   513,   368,   372,   513,   368,   372,   513,   368,
     372,   513,   513,   370,   513,   370,   513,   370,   514,   370,
     513,   370,   513,   370,   513,   370,   513,   370,   513,   370,
     513,   370,   513,   370,   513,   370,   513,   370,   369,   513,
     370,   513,   370,   369,   513,   370,   513,   370,   513,   514,
     370,   370,   513,   370,   369,   512,   513,   514,   370,   513,
     370,   514,   370,   368,   384,   384,   384,   368,   368,   370,
     513,   370,   513,   370,   513,   370,   513,   370,   513,   514,
     513,   514,   513,   514,   513,   514,   207,   210,   375,   507,
     508,   513,   514,   513,   512,   207,   210,   375,   507,   512,
     513,   379,    44,   386,    21,   513,   513,   369,   513,   512,
     512,   512,   512,   512,   512,   512,   512,   512,   512,   512,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   512,   513,   512,   369,   513,
     369,   513,   514,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   369,   513,   514,   369,   369,   514,   369,   513,
     513,   514,   513,   514,   513,   514,   369,   369,   369,   513,
     369,   513,   369,   514,   512,   513,   512,   514,   512,   514,
     353,   354,   369,   513,   372,   372,   353,   354,   369,   513,
     513,   514,   514,   514,   514,   514,   514,   514,   513,   513,
     513,   513,   513,   369,   511,   512,   370,   377,   513,   369,
     511,   370,   377,   513,   369,   369,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   514,   513,   513,   513,   514,   514,   514,   514,   514,
     514,   513,   514,   513,   513,   353,   354,   370,   513,   370,
     513,   370,   513,   369,   376,   370,   370,   513,   370,   513,
     370,   513,   513,   370,   514,   370,   514,   370,   514,   370,
     514,   370,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   370,   513,   370,   514,   370,   513,   370,   513,   370,
     513,   370,   513,   370,   514,   370,   513,   370,   513,   370,
     513,   370,   353,   508,   370,   513,   370,   514,   370,   514,
     370,   513,   514,   370,   513,   370,   513,   370,   370,   514,
     513,   370,   514,   370,   512,   370,   513,   370,   514,   369,
     385,   369,   511,   353,   354,   385,   370,   513,   370,   513,
     370,   513,   370,   513,   368,   368,   510,   514,   370,   373,
     368,   368,   370,   373,   370,   370,   369,   369,   369,   369,
     370,   370,   370,   369,   369,   370,   370,   370,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   370,
     370,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   370,   369,   370,   369,   369,   370,   369,   369,
     370,   369,   369,   369,   369,   370,   369,   370,   370,   370,
     370,   369,   370,   369,   370,   369,   513,   513,   369,   369,
     369,   369,   370,   369,   369,   370,   369,   370,   369,   369,
     369,   370,   369,   369,   369,   370,   369,   369,   370,   377,
     370,   373,   370,   373,   369,   377,   370,   373,   370,   373,
     370,   373,   370,   373,   372,   372,   513,   513,   370,   513,
     370,   509,   513,   370,   513,   370,   513,    31,   514,   514,
     514,   370,   514,   513,   370,   370,   370,   373,   370,   370,
     373,   370,   370,   373,   370,   370,   373,   513,   514,   513,
     370,   513,   370,   513,   370,   513,   370,   514,   513,   370,
     513,   370,   513,   370,   353,   508,   513,   514,   514,   513,
     514,   370,   513,   513,   514,   513,   370,   514,   512,   370,
     513,   514,   369,   369,   369,   369,   513,   513,   370,   513,
     370,   513,   370,   512,   512,   376,   370,   513,   228,   229,
     230,   231,   232,   233,   360,   512,   512,   513,   230,   360,
     513,   513,   513,   513,   513,   512,   514,   514,   513,   513,
     513,   513,   514,   513,   514,   513,   514,   512,   512,   514,
     514,   514,   514,   514,   370,   373,   370,   373,   513,   514,
     513,   514,   513,   513,   511,   373,   373,   513,   228,   229,
     373,   373,   513,   513,   513,   513,   513,   369,   370,   513,
     370,   513,   370,   353,   508,   513,   370,   513,   369,   513,
     369,   369,   369,   370,   513,   369,   369,   370,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   369,   369,   370,
     513,   514,   370,   513,   370,   514,   370,   513,   514,   369,
     370,   513,   370,   513,   370,   513,   369,   369,   369,   369,
     369,   370,   369,   514,   369,   369,   370,   369,   513,   369,
     370,   512,   369,   369,   369,   370,   513,   370,   513,   370,
     513,   370,   370,   510,   373,   513,   514,   513,   514,   513,
     514,   513,   514,   513,   514,   370,   370,   373,   512,   512,
     369,   370,   370,   369,   369,   369,   370,   369,   370,   369,
     370,   370,   369,   369,   369,   370,   369,   369,   370,   369,
     369,   370,   369,   369,   370,   369,   370,   370,   369,   369,
     369,   513,   513,   369,   370,   369,   369,   369,   369,   369,
     373,   373,   373,   373,   370,   373,   370,   373,   513,   513,
     370,   353,   508,   513,   370,   513,   370,    32,   513,   370,
     513,   369,   373,   369,   373,   369,   373,   369,   373,   513,
     514,   513,   514,   370,   513,   514,   370,   513,   370,   513,
     370,   513,   370,   514,   513,   512,   513,   370,   513,   370,
     513,   370,   512,   514,   228,   229,   230,   231,   232,   233,
     360,   512,   514,   230,   360,   513,   513,   513,   513,   513,
     513,   513,   514,   513,   514,   513,   514,   512,   513,   512,
     373,   373,   513,   514,   228,   229,   513,   513,   369,   370,
     513,   369,   369,   370,   353,   508,   513,   370,   513,   513,
     369,   370,   513,   369,   369,   369,   369,   369,   370,   513,
     370,   369,   513,   514,   370,   513,   369,   370,   513,   369,
     370,   513,   369,   369,   369,   370,   513,   370,   513,   370,
     513,   369,   370,   369,   513,   514,   513,   514,   513,   514,
     513,   514,   513,   514,   369,   370,   369,   512,   512,   369,
     370,   369,   369,   370,   369,   369,   370,   369,   369,   369,
     369,   369,   369,   369,   370,   369,   369,   369,   369,   373,
     373,   513,   353,   508,   513,   370,   513,   513,   370,   513,
     513,   514,   370,   513,   370,   513,   370,   513,   370,   513,
     370,   513,   370,   513,   370,   513,   513,   513,   513,   513,
     513,   369,   369,   369,   370,   353,   508,   369,   369,   370,
     513,   369,   370,   369,   514,   370,   513,   369,   370,   513,
     369,   370,   513,   370,   513,   370,   513,   370,   513,   369,
     369,   369,   369,   369,   369,   353,   508,   513,   370,   514,
     513,   513,   513,   513,   370,   513,   370,   513,   370,   369,
     369,   369,   370,   513,   369,   369,   369,   369,   370,   513,
     370,   513,   370,   513,   513,   370,   513,   513,   513,   369,
     370,   513,   369,   369,   369,   513,   369
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   378,   379,   379,   380,   381,   381,   381,   381,   381,
     381,   381,   381,   382,   383,   383,   383,   383,   384,   384,
     385,   385,   386,   386,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   407,   407,   407,   407,   407,   407,   407,
     408,   408,   408,   408,   408,   408,   408,   408,   409,   409,
     409,   409,   409,   409,   409,   409,   410,   411,   412,   412,
     413,   414,   414,   415,   416,   417,   417,   417,   417,   417,
     417,   417,   417,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   420,   420,   420,   420,   420,   421,   421,
     421,   421,   421,   421,   421,   421,   422,   422,   423,   423,
     424,   425,   426,   427,   427,   427,   428,   429,   430,   430,
     430,   431,   431,   431,   431,   431,   432,   432,   432,   432,
     433,   433,   434,   434,   435,   435,   436,   436,   437,   437,
     438,   438,   439,   439,   440,   440,   440,   440,   441,   441,
     442,   443,   444,   445,   445,   445,   446,   446,   446,   446,
     446,   446,   446,   446,   446,   446,   446,   446,   447,   447,
     447,   447,   447,   447,   447,   447,   447,   447,   447,   447,
     447,   447,   447,   447,   447,   447,   448,   448,   448,   449,
     449,   449,   450,   450,   450,   451,   451,   451,   452,   452,
     452,   453,   453,   453,   454,   454,   454,   454,   454,   454,
     454,   454,   454,   454,   454,   454,   455,   455,   455,   456,
     457,   457,   458,   459,   460,   461,   461,   461,   461,   462,
     462,   462,   462,   462,   462,   463,   464,   464,   465,   465,
     466,   466,   466,   466,   467,   467,   467,   467,   467,   467,
     468,   468,   468,   468,   468,   468,   469,   470,   471,   472,
     473,   473,   473,   474,   474,   474,   475,   475,   475,   476,
     476,   476,   476,   476,   477,   477,   477,   477,   477,   478,
     478,   478,   479,   479,   479,   479,   480,   480,   480,   481,
     481,   481,   482,   483,   483,   484,   484,   485,   485,   485,
     486,   487,   488,   489,   490,   491,   492,   493,   494,   495,
     496,   497,   498,   499,   500,   501,   502,   502,   503,   504,
     505,   506,   507,   508,   509,   509,   510,   510,   511,   511,
     512,   512,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   513,
     513,   513,   513,   513,   513,   513,   513,   513,   513,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514,   514,   514,   514,   514,   514,   514,   514,   514,   514,
     514
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     1,     1,     2,     3,     1,     2,     1,
       2,     1,     0,     1,     1,     1,     4,     4,     2,     3,
       1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     2,     3,     1,     1,     2,
       1,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       3,     3,     7,     7,     5,     5,     7,     7,     3,     3,
       7,     7,     5,     5,     7,     7,     2,     2,     1,     1,
       1,     4,     6,     1,     1,     6,     6,     8,     8,     3,
       8,    10,     8,     6,     8,     6,     8,     6,     8,     6,
       8,     6,     8,     3,     6,     5,     5,     6,     6,     6,
       6,     8,     7,     7,     8,     8,     8,     8,     3,     8,
      10,     8,     1,     3,     3,     3,     3,     3,     3,     2,
       2,     3,     3,     3,     3,     1,     3,     3,     6,     8,
       2,     2,     2,     2,     4,     5,     1,     2,     1,     2,
       2,     6,     8,     2,     4,     6,     2,     2,     6,     4,
       4,     6,     8,    10,     6,     8,    12,    14,    12,    14,
      12,    14,     8,    10,     6,     8,     6,     8,     6,     8,
       2,     2,     2,     2,     4,     2,     8,    10,     8,    10,
       6,     8,     6,     8,    10,    12,    10,    12,     2,     6,
       4,     2,     6,     4,     4,     6,     6,     8,     8,    10,
      10,    12,    12,    14,    14,    16,     2,     6,     4,     2,
       6,     4,     2,     6,     4,     1,     3,     2,     1,     3,
       2,     2,     6,     4,     4,     2,     4,     2,     7,     5,
       7,     5,     9,     7,     9,     7,     1,     2,     3,     1,
       1,     2,     2,     1,     1,     6,     8,     8,    10,     6,
       8,     8,    10,    10,    12,     2,     4,     6,    10,    12,
       4,     6,     4,     6,     6,     8,     8,    10,    10,    12,
       6,     8,     8,    10,    10,    12,     2,     2,     1,     2,
       2,     4,     6,     1,     3,     2,     2,     4,     6,     2,
       4,     6,     6,     8,     1,     3,     2,     6,     4,     2,
       6,     4,     4,     6,     6,     8,     2,     6,     4,     1,
       3,     2,     2,     6,     8,     4,     6,     2,     6,     4,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     1,
       1,     2,     3,     3,     1,     3,     1,     3,     1,     3,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     4,     4,     5,     5,     5,     5,     4,     5,
       5,     5,     5,     6,     7,     7,     7,     7,     4,     3,
       1,     2,     2,     2,     2,     4,     4,     4,     4,     6,
       8,    10,     6,     8,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     1,
       1,     1,     1,     1,     3,     4,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     8,    10,     1,     1,     1,
       6,     4,     4,     4,     4,     4,     4,     4,     1,     3,
       4,     6,     4,     6,     8,     4,     6,     8,     4,     6,
       8,     4,     6,     8,     4,     6,     8,     4,     6,     8,
       1,     1,     1,     3,     4,     4,     6,     8,     6,     1,
       1,     4,     1,     1,     3,     4,     4,     4,     1,     1,
       3,     4,     1,     1,     1,     4,     6,     4,     4,     4,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     3,     3,     1,     4,     6,     4,     3,     1,     4,
       4,     4,     4,     8,     6,     8,     6,     6,    10,     1,
       3,     4,     1,     3,     4,     1,     3,     4,     6,     8,
       4,     6,     8,     1,     3,     1,     3,     1,     3,     4,
       1,     3,     4,     6,     4,     3,     1,     8,    10,     8,
       4,     6,     4,     6,     4,     6,     4,     4,     4,     6,
       4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
        case 4:
#line 421 "basicParse.y" /* yacc.c:1646  */
    {
					linenumber++;
					column=0;
					addIntOp(OP_CURRLINE, numincludes * 0x1000000 + linenumber);
				}
#line 11036 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 438 "basicParse.y" /* yacc.c:1646  */
    {
					if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
						errorcode = COMPERR_FUNCTIONGOTO;
						return -1;
					}
					labeltable[(yyvsp[0].number)] = wordOffset; 
				}
#line 11048 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 448 "basicParse.y" /* yacc.c:1646  */
    {
				args[numargs] = (yyvsp[0].number); argstype[numargs] = ARGSTYPEINT; numargs++;
				//printf("functionvariable %i %i %i\n", args[numargs-1], argstype[numargs-1],numargs); 
			}
#line 11057 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 452 "basicParse.y" /* yacc.c:1646  */
    {
				args[numargs] = (yyvsp[0].number); argstype[numargs] = ARGSTYPESTR; numargs++;
				//printf("functionvariable %i %i %i\n", args[numargs-1], argstype[numargs-1],numargs); 
			}
#line 11066 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 456 "basicParse.y" /* yacc.c:1646  */
    {
				args[numargs] = (yyvsp[-1].number); argstype[numargs] = ARGSTYPEVARREF; numargs++;
				//printf("functionvariable %i %i %i\n", args[numargs-1], argstype[numargs-1],numargs); 
			}
#line 11075 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 460 "basicParse.y" /* yacc.c:1646  */
    {
				args[numargs] = (yyvsp[-1].number); argstype[numargs] = ARGSTYPEVARREFSTR; numargs++;
				//printf("functionvariable %i %i %i\n", args[numargs-1], argstype[numargs-1],numargs); 
			}
#line 11084 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 596 "basicParse.y" /* yacc.c:1646  */
    {
				// start a case block
				newIf(linenumber, IFTABLETYPEBEGINCASE);
			}
#line 11093 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 602 "basicParse.y" /* yacc.c:1646  */
    {
				// if not first case then add jump to to "endcase" and resolve the branchfalse
				if (numifs>1) {
					if (iftabletype[numifs-1]==IFTABLETYPECASE) {
						if (iftabletype[numifs-2]==IFTABLETYPEBEGINCASE) {
							//
							// create jump around from end of the CASE to end of the END CASE
							addIntOp(OP_GOTO, getInternalSymbol(iftableid[numifs-2],INTERNALSYMBOLEXIT));
						} else {
							errorcode = COMPERR_ENDBEGINCASE;
							linenumber = iftablesourceline[numifs-1];
							return -1;
						}
						//
						// resolve branchfalse from previous case
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						//
						numifs--;
					}
				}
				//
			}
#line 11120 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 626 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// add branch to the end if false
				addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// put new CASE on the frame for the IF
				newIf(linenumber, IFTABLETYPECASE);
			}
#line 11133 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 636 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// create jump around from end of the TRY to end of the CATCH
				addOp(OP_OFFERROR);
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPETRY) {
						//
						// resolve the try onerrorcatch to the catch address
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
						//
						// put new if on the frame for the catch
						newIf(linenumber, IFTABLETYPECATCH);
						addOp(OP_OFFERROR);
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_CATCH;
					return -1;
				}
			}
#line 11164 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 664 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// create internal symbol and add to the label table for the top of the loop
				labeltable[getInternalSymbol(nextifid,INTERNALSYMBOLTOP)] = wordOffset; 
				//
				// add to if frame
				newIf(linenumber, IFTABLETYPEDO);
			}
#line 11177 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 674 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// create jump around from end of the THEN to end of the ELSE
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEIF) {
						//
						// resolve the label on the if to the current location
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
						//
						// put new if on the frame for the else
						newIf(linenumber, IFTABLETYPEELSE);
					} else if (iftabletype[numifs-1]==IFTABLETYPECASE) {
						if (numifs>1) {
							if (iftabletype[numifs-2]==IFTABLETYPEBEGINCASE) {
								//
								// create jump around from end of the CASE to end of the END CASE
								addIntOp(OP_GOTO, getInternalSymbol(iftableid[numifs-2],INTERNALSYMBOLEXIT));
							} else {
								errorcode = COMPERR_ENDBEGINCASE;
								linenumber = iftablesourceline[numifs-1];
								return -1;
							}
							//
							// resolve branchfalse from previous case
							labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
							//
							numifs--;
							// put new if on the frame for the else
							newIf(linenumber, IFTABLETYPEELSE);
						}
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_ELSE;
					return -1;
				}
			}
#line 11225 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 720 "basicParse.y" /* yacc.c:1646  */
    {
				// add label for last case branchfalse to jump to
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPECASE || iftabletype[numifs-1]==IFTABLETYPEELSE) {
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
					} else {
						errorcode = COMPERR_ENDENDCASE;
						return -1;
					}
				} else {
					errorcode = COMPERR_ENDENDCASE;
					return -1;
				}
				// add label for all cases to jump to after execution
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEBEGINCASE) {
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_ENDENDCASEBEGIN;
					return -1;
				}
			}
#line 11259 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 751 "basicParse.y" /* yacc.c:1646  */
    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then put the current wordcode address there
				// - so we can jump over code
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEIF||iftabletype[numifs-1]==IFTABLETYPEELSE) {
						//
						// resolve the label on the if/else to the current location
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_ENDIF;
					return -1;
				}
			}
#line 11284 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 773 "basicParse.y" /* yacc.c:1646  */
    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then put the current wordcode address there
				// - so we can jump over code
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPECATCH) {
						//
						// resolve the label on the Catch to the current location
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_ENDTRY;
					return -1;
				}
			}
#line 11309 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 796 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEWHILE) {
						//
						// jump to the top
						addIntOp(OP_GOTO, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE));
						//
						// resolve the label to the bottom
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						//
						// remove the single placeholder from the if frame
						numifs--;
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_ENDWHILE;
					return -1;
				}
			}
#line 11336 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 820 "basicParse.y" /* yacc.c:1646  */
    {
					//
					// add branch to the end if false
					addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
					//
					// put new if on the frame for the IF
					newIf(linenumber, IFTABLETYPEIF);
			}
#line 11349 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 831 "basicParse.y" /* yacc.c:1646  */
    {
				// if there is an if branch or jump on the iftable stack get where it is
				// in the wordcode array and then resolve the lable
				if (numifs>0) {
					labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
					numifs--;
				}
			}
#line 11362 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 842 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// resolve the label on the else to the current location
				labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
				numifs--;
			}
#line 11373 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 851 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// create jump around from end of the THEN to end of the ELSE
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// jump point for else
				labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
				numifs--;
				//
				// put new if on the frame for the else
				newIf(linenumber, IFTABLETYPEELSE);
			}
#line 11390 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 865 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// add on error branch
				addIntOp(OP_ONERRORCATCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// put new if on the frame for the TRY
				newIf(linenumber, IFTABLETYPETRY);
			}
#line 11403 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 875 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEDO) {
						//
						// create label for CONTINUE DO
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset; 
						//
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_UNTIL;
					return -1;
				}
			}
#line 11425 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 894 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// branch back to top if condition holds
				addIntOp(OP_BRANCH, getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLTOP));
				//
				// create label for EXIT DO
				labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
				numifs--;
			}
#line 11439 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 905 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// create internal symbol and add to the label table for the top of the loop
				labeltable[getInternalSymbol(nextifid,INTERNALSYMBOLCONTINUE)] = wordOffset; 
			}
#line 11449 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 912 "basicParse.y" /* yacc.c:1646  */
    {
				//
				// add branch to end if false
				addIntOp(OP_BRANCH, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// add to if frame
				newIf(linenumber, IFTABLETYPEWHILE);
			}
#line 11462 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 931 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_DIM, (yyvsp[-1].number));
			}
#line 11471 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 935 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_DIMSTR, (yyvsp[-1].number));
			}
#line 11480 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 939 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_DIM, (yyvsp[-5].number));
			}
#line 11488 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 942 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_DIMSTR, (yyvsp[-5].number));
			}
#line 11496 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 945 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_DIM, (yyvsp[-3].number));
			}
#line 11505 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 949 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_DIMSTR, (yyvsp[-3].number));
			}
#line 11514 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 953 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_DIM, (yyvsp[-5].number));
			}
#line 11522 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 956 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_DIMSTR, (yyvsp[-5].number));
			}
#line 11530 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 961 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_REDIM, (yyvsp[-1].number));
			}
#line 11539 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 965 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_REDIMSTR, (yyvsp[-1].number));
			}
#line 11548 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 969 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_REDIM, (yyvsp[-5].number));
			}
#line 11556 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 972 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_REDIMSTR, (yyvsp[-5].number));
			}
#line 11564 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 975 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_REDIM, (yyvsp[-3].number));
			}
#line 11573 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 979 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);
				addIntOp(OP_REDIMSTR, (yyvsp[-3].number));
			}
#line 11582 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 983 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_REDIM, (yyvsp[-5].number));
			}
#line 11590 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 986 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_REDIMSTR, (yyvsp[-5].number));
			}
#line 11598 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 991 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PAUSE);
			}
#line 11606 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 997 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_THROWERROR);
			}
#line 11614 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 1002 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CLS);
			}
#line 11622 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 1005 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CLG);
			}
#line 11630 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 1011 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_FASTGRAPHICS);
			}
#line 11638 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1017 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_GRAPHSIZE);
			}
#line 11646 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1020 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_GRAPHSIZE);
			}
#line 11654 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 1026 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_REFRESH);
			}
#line 11662 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 1031 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_END);
			}
#line 11670 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1037 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_STRARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11678 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1040 "basicParse.y" /* yacc.c:1646  */
    {
				// a$[n] += s$
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-5].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKTOPTO2);
				addOp(OP_CONCAT);
				addIntOp(OP_STRARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11693 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 1050 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11701 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 1053 "basicParse.y" /* yacc.c:1646  */
    {
				// a$[b,c] += s$
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-7].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_CONCAT);
				addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11717 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1064 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen = 0;
				addIntOp(OP_STRARRAYLISTASSIGN, (yyvsp[-2].number));
			}
#line 11727 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 1069 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);	// case sensitive flag
				addIntOp(OP_EXPLODESTR, (yyvsp[-7].number));
			}
#line 11736 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1073 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_EXPLODESTR, (yyvsp[-9].number));
			}
#line 11744 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1076 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_EXPLODEXSTR, (yyvsp[-7].number));
			}
#line 11752 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1096 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11760 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1099 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n]++ (Statement)
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-4].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-4].number));
			}
#line 11773 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1107 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n]-- (Statement)
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-4].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-4].number));
			}
#line 11786 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1115 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n] += n
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-5].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKTOPTO2);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11801 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1125 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n] -= n
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-5].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKTOPTO2);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11816 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1135 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n] *= n
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-5].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKTOPTO2);
				addOp(OP_MUL);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11831 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1145 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n] /= n
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF,(yyvsp[-5].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKTOPTO2);
				addOp(OP_DIV);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-5].number));
			}
#line 11846 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1155 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11854 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1158 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-6].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-6].number));
			}
#line 11866 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1165 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-6].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-6].number));
			}
#line 11878 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1172 "basicParse.y" /* yacc.c:1646  */
    {
				// a[b,c] += n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-7].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11893 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1182 "basicParse.y" /* yacc.c:1646  */
    {
				// a[b,c] -= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-7].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11909 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1193 "basicParse.y" /* yacc.c:1646  */
    {
				// a[b,c] *= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-7].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_MUL);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11924 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1203 "basicParse.y" /* yacc.c:1646  */
    {
				// a[b,c] /= n
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D,(yyvsp[-7].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKSWAP2);
				addOp(OP_STACKSWAP);
				addOp(OP_DIV);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-7].number));
			}
#line 11940 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1214 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen = 0;
				addIntOp(OP_ARRAYLISTASSIGN, (yyvsp[-2].number));
			}
#line 11950 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1219 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);	// case sensitive flag
				addIntOp(OP_EXPLODE, (yyvsp[-7].number));
			}
#line 11959 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1223 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_EXPLODE, (yyvsp[-9].number));
			}
#line 11967 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1226 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_EXPLODEX, (yyvsp[-7].number));
			}
#line 11975 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1229 "basicParse.y" /* yacc.c:1646  */
    {
				errorcode = COMPERR_ASSIGNS2N;
				return -1;
			}
#line 11984 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1244 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_NUMASSIGN, (yyvsp[-2].number));
			}
#line 11992 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1247 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-1].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_NUMASSIGN, (yyvsp[-1].number));
			}
#line 12003 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1253 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-1].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_NUMASSIGN, (yyvsp[-1].number));
			}
#line 12014 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1259 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-2].number));
				addOp(OP_ADD);
				addIntOp(OP_NUMASSIGN, (yyvsp[-2].number));
			}
#line 12024 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1264 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-2].number));
				addOp(OP_STACKSWAP);
				addOp(OP_SUB);
				addIntOp(OP_NUMASSIGN, (yyvsp[-2].number));
			}
#line 12035 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1270 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-2].number));
				addOp(OP_MUL);
				addIntOp(OP_NUMASSIGN, (yyvsp[-2].number));
			}
#line 12045 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1275 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-2].number));
				addOp(OP_STACKSWAP);
				addOp(OP_DIV);
				addIntOp(OP_NUMASSIGN, (yyvsp[-2].number));
			}
#line 12056 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1281 "basicParse.y" /* yacc.c:1646  */
    {
				errorcode = COMPERR_ASSIGNS2N;
				return -1;
			}
#line 12065 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1288 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_STRINGASSIGN, (yyvsp[-2].number));
			}
#line 12073 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1291 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-2].number));
				addOp(OP_STACKSWAP);
				addOp(OP_CONCAT);
				addIntOp(OP_STRINGASSIGN, (yyvsp[-2].number));
			}
#line 12084 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1299 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); //step
				addIntOp(OP_FOR, (yyvsp[-4].number));
				// add to iftable to make sure it is not broken with an if
				// do, while, else, and to report if it is
				// next ed before end of program
				newIf(linenumber, IFTABLETYPEFOR);
			}
#line 12097 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1307 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_FOR, (yyvsp[-6].number));
				// add to iftable to make sure it is not broken with an if
				// do, while, else, and to report if it is
				// next ed before end of program
				newIf(linenumber, IFTABLETYPEFOR);
			}
#line 12109 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1316 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
					if (iftabletype[numifs-1]==IFTABLETYPEFOR) {
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLCONTINUE)] = wordOffset; 
						addIntOp(OP_NEXT, (yyvsp[0].number));
						labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
						numifs--;
					} else {
						errorcode = testIfOnTableError(numincludes);
						linenumber = testIfOnTable(numincludes);
						return -1;
					}
				} else {
					errorcode = COMPERR_NEXT;
					return -1;
				}
			}
#line 12131 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1335 "basicParse.y" /* yacc.c:1646  */
    {
				if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
					errorcode = COMPERR_FUNCTIONGOTO;
					return -1;
				}
				addIntOp(OP_GOTO, (yyvsp[0].number));
			}
#line 12143 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1344 "basicParse.y" /* yacc.c:1646  */
    {
				if (functionDefSymbol != -1 || subroutineDefSymbol !=-1) {
					errorcode = COMPERR_FUNCTIONGOTO;
					return -1;
				}
				addIntOp(OP_GOSUB, (yyvsp[0].number));
			}
#line 12155 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1353 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_GOSUB, (yyvsp[0].number));
			}
#line 12163 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1356 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_GOSUB, (yyvsp[-2].number));
			}
#line 12171 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1359 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_GOSUB, (yyvsp[-3].number));
			}
#line 12179 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1365 "basicParse.y" /* yacc.c:1646  */
    { 
				int i;
				for(i=0; i < numifs; i++) {
					if (iftabletype[i]==IFTABLETYPETRY || iftabletype[i]==IFTABLETYPECATCH) {
						errorcode = COMPERR_NOTINTRYCATCH;
						return -1;
					}
				}
				addOp(OP_OFFERROR);
			}
#line 12194 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1378 "basicParse.y" /* yacc.c:1646  */
    {
				int i;
				for(i=0; i < numifs; i++) {
					if (iftabletype[i]==IFTABLETYPETRY || iftabletype[i]==IFTABLETYPECATCH) {
						errorcode = COMPERR_NOTINTRYCATCH;
						return -1;
					}
				}
				addIntOp(OP_ONERRORGOSUB, (yyvsp[0].number));
			}
#line 12209 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1390 "basicParse.y" /* yacc.c:1646  */
    { 
				if (functionDefSymbol!=-1) {
					// if we are defining a function return pushes a variable value
					addIntOp(OP_FUNCRETURN, functionDefSymbol);
					addOp(OP_DECREASERECURSE);
				}
				if (subroutineDefSymbol!=-1) {
					// if we are defining a subroutine
					addOp(OP_DECREASERECURSE);
				}
				addOp(OP_RETURN);
			}
#line 12226 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1402 "basicParse.y" /* yacc.c:1646  */
    { 
				if (functionDefSymbol!=-1) {
					if (functionType==FUNCTIONTYPEFLOAT) {
						// value on stack gets returned
						addOp(OP_DECREASERECURSE);
						addOp(OP_RETURN);
					} else {
						errorcode = COMPERR_RETURNTYPE;
						return -1;
					}
				} else {
					errorcode = COMPERR_RETURNVALUE;
					return -1;
				}
			}
#line 12246 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1417 "basicParse.y" /* yacc.c:1646  */
    { 
				if (functionDefSymbol!=-1) {
					if (functionType==FUNCTIONTYPESTRING) {
						// value on stack gets returned
						addOp(OP_DECREASERECURSE);
						addOp(OP_RETURN);
					} else {
						errorcode = COMPERR_RETURNTYPE;
						return -1;
					}
				} else {
					errorcode = COMPERR_RETURNVALUE;
					return -1;
				}
			}
#line 12266 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1434 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 255); 
				addOp(OP_RGB);
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
				newParseWarning(COMPWARNING_DEPRECATED_FORM);
			}
#line 12278 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1441 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 255); 
				addOp(OP_RGB);
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
				newParseWarning(COMPWARNING_DEPRECATED_FORM);
			}
#line 12290 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1448 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKDUP);
				addOp(OP_SETCOLOR);
			}
#line 12299 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1452 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SETCOLOR);
			}
#line 12307 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1455 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SETCOLOR);
			}
#line 12315 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1460 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK,(yyvsp[0].number));
				addOp(OP_SOUND_LIST);
			}
#line 12324 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1464 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen = 0;
				addOp(OP_SOUND_LIST);
			}
#line 12334 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1469 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 2);
				addOp(OP_SOUND_LIST);
			}
#line 12343 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1473 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 2);
				addOp(OP_SOUND_LIST);
			}
#line 12352 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1479 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PLOT);
			}
#line 12360 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1482 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PLOT);
			}
#line 12368 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1487 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_LINE);
			}
#line 12376 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1490 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_LINE);
			}
#line 12384 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1497 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CIRCLE);
			}
#line 12392 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1500 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CIRCLE);
			}
#line 12400 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1505 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_ARC);
			}
#line 12408 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1508 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_ARC);
			}
#line 12416 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1514 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CHORD);
			}
#line 12424 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1517 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CHORD);
			}
#line 12432 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1523 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PIE);
			}
#line 12440 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1526 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PIE);
			}
#line 12448 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1532 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_RECT);
			}
#line 12456 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1535 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_RECT);
			}
#line 12464 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1541 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_TEXT);
			}
#line 12472 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1544 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_TEXT);
			}
#line 12480 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1547 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_TEXT);
			}
#line 12488 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1550 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_TEXT);
			}
#line 12496 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1556 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_FONT);
			}
#line 12504 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1559 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_FONT);
			}
#line 12512 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1564 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SAY);
			}
#line 12520 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1570 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SYSTEM);
			}
#line 12528 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1576 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_VOLUME);
			}
#line 12536 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1582 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[0].number));
				addOp(OP_POLY_LIST);
			}
#line 12545 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1586 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[-1].number));
				addOp(OP_POLY_LIST);
			}
#line 12554 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1590 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_POLY_LIST); 
			}
#line 12564 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1597 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[0].number));
				addOp(OP_STAMP_S_LIST);
			}
#line 12573 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1601 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[-1].number));
				addOp(OP_STAMP_S_LIST);
			}
#line 12582 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1605 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_STAMP_S_LIST);
			}
#line 12592 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1610 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_STAMP_S_LIST);
			}
#line 12602 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1615 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[0].number));
				addOp(OP_STAMP_LIST);
			}
#line 12611 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1619 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[-1].number));
				addOp(OP_STAMP_LIST);
			}
#line 12620 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1623 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_STAMP_LIST);
			}
#line 12630 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1628 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_STAMP_LIST);
			}
#line 12640 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1633 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[0].number));
				addOp(OP_STAMP_SR_LIST);
			}
#line 12649 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1637 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[-1].number));
				addOp(OP_STAMP_SR_LIST);
			}
#line 12658 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1641 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_STAMP_SR_LIST);
			}
#line 12668 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1646 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_STAMP_SR_LIST);
			}
#line 12678 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1653 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // file number zero
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			}
#line 12689 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1659 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			}
#line 12698 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1663 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // not binary
				addOp(OP_OPEN);
			}
#line 12707 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1667 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // file number zero
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			}
#line 12718 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1673 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			}
#line 12727 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1677 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); // binary
				addOp(OP_OPEN);
			}
#line 12736 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1681 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 9600); // baud
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12749 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1689 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 9600); // baud
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12762 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1697 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12774 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1704 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 8); // data bits
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12786 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1711 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12797 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1717 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); // stop bits
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12808 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1723 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12818 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1728 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // parity
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12828 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1733 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12837 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1737 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // flow
				addOp(OP_OPENSERIAL);
			}
#line 12846 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1741 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_OPENSERIAL);
			}
#line 12854 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1744 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_OPENSERIAL);
			}
#line 12862 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1750 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);  // file number zero
				addOp(OP_STACKSWAP);
				addOp(OP_WRITE);
			}
#line 12872 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1755 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WRITE);
			}
#line 12880 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1758 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WRITE);
			}
#line 12888 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1764 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_WRITELINE);
			}
#line 12898 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1769 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WRITELINE);
			}
#line 12906 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1772 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WRITELINE);
			}
#line 12914 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1778 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_WRITEBYTE);
			}
#line 12924 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1783 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WRITEBYTE);
			}
#line 12932 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1786 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WRITEBYTE);
			}
#line 12940 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1791 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_CLOSE);
			}
#line 12949 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1795 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_CLOSE);
			}
#line 12958 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1799 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CLOSE);
			}
#line 12966 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1804 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_RESET);
			}
#line 12975 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1808 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_RESET);
			}
#line 12984 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1812 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_RESET);
			}
#line 12992 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1817 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_SEEK);
			}
#line 13002 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1822 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SEEK);
			}
#line 13010 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1825 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SEEK);
			}
#line 13018 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1830 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_INPUT);
				addIntOp(OP_STRINGASSIGN, (yyvsp[0].number));
			}
#line 13027 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1834 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_INPUT);
				addIntOp(OP_STRINGASSIGN, (yyvsp[0].number));
			}
#line 13037 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1839 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_INPUT);
				addIntOp(OP_NUMASSIGN, (yyvsp[0].number));
			}
#line 13046 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1843 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_INPUT);
				addIntOp(OP_NUMASSIGN, (yyvsp[0].number));
			}
#line 13056 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1848 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKSWAP);		// bring prompt to top
				addOp(OP_INPUT);
				addIntOp(OP_STRARRAYASSIGN, (yyvsp[-3].number));
			}
#line 13066 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1853 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_INPUT);
				addIntOp(OP_STRARRAYASSIGN, (yyvsp[-3].number));
			}
#line 13076 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1858 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKSWAP);		// bring prompt to top
				addOp(OP_INPUT);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-3].number));
			}
#line 13086 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1863 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_INPUT);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-3].number));
			}
#line 13096 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1868 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addOp(OP_INPUT);
				addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[-5].number));
			}
#line 13106 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1873 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_INPUT);
				addIntOp(OP_STRARRAYASSIGN2D, (yyvsp[-5].number));
			}
#line 13116 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1878 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_STACKTOPTO2); addOp(OP_STACKTOPTO2);		// bring prompt to top
				addOp(OP_INPUT);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-5].number));
			}
#line 13126 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1883 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_INPUT);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-5].number));
			}
#line 13136 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1891 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_PRINTN);
			}
#line 13145 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1895 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PRINTN);
			}
#line 13153 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1898 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PRINT);
			}
#line 13161 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1904 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WAVPAUSE);
			}
#line 13169 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1910 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");
				addOp(OP_WAVPLAY);
			}
#line 13178 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1914 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WAVPLAY);
			}
#line 13186 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1919 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WAVSEEK);
			}
#line 13194 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1925 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WAVSTOP);
			}
#line 13202 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1931 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_WAVWAIT);
			}
#line 13210 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1937 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PUTSLICE);
			}
#line 13218 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1940 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PUTSLICE);
			}
#line 13226 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1943 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PUTSLICEMASK);
			}
#line 13234 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1946 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PUTSLICEMASK);
			}
#line 13242 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1953 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); // scale
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			}
#line 13254 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1961 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1); // scale
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			}
#line 13266 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1969 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			}
#line 13276 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1975 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // rotate
				addOp(OP_STACKSWAP);
				addOp(OP_IMGLOAD);
			}
#line 13286 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1981 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_IMGLOAD);
			}
#line 13294 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1985 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_IMGLOAD);
			}
#line 13302 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1991 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITEDIM);
			}
#line 13310 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1997 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITELOAD);
			}
#line 13318 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 2000 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITELOAD);
			}
#line 13326 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 2006 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITESLICE);
			}
#line 13334 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 2009 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITESLICE);
			}
#line 13342 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 2015 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[0].number));
				addOp(OP_SPRITEPOLY_LIST);
			}
#line 13351 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 2019 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_ARRAY2STACK, (yyvsp[-1].number));
				addOp(OP_SPRITEPOLY_LIST);
			}
#line 13360 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 2023 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_SPRITEPOLY_LIST);
			}
#line 13370 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 2028 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, listlen);
				listlen=0;
				addOp(OP_SPRITEPOLY_LIST);
			}
#line 13380 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 2036 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,1);	// scale
				addIntOp(OP_PUSHINT,0);	// rotate
				addOp(OP_SPRITEPLACE);
			}
#line 13390 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 2041 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,1);	// scale
				addIntOp(OP_PUSHINT,0);	// rotate
				addOp(OP_SPRITEPLACE);
			}
#line 13400 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 2046 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// rotate
				addOp(OP_SPRITEPLACE);
			}
#line 13409 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 2050 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// rotate
				addOp(OP_SPRITEPLACE);
			}
#line 13418 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 2054 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITEPLACE);
			}
#line 13426 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 2057 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITEPLACE);
			}
#line 13434 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 2063 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// scale (change in scale)
				addIntOp(OP_PUSHINT,0);	// rotate (change in rotation)
				addOp(OP_SPRITEMOVE);
			}
#line 13444 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 2068 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// scale (change in scale)
				addIntOp(OP_PUSHINT,0);	// rotate (change in rotation)
				addOp(OP_SPRITEMOVE);
			}
#line 13454 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 2073 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// rotate (change in rotation)
				addOp(OP_SPRITEMOVE);
			}
#line 13463 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 2077 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// rotate (change in rotation)
				addOp(OP_SPRITEMOVE);
			}
#line 13472 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 2081 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITEMOVE);
			}
#line 13480 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 385:
#line 2084 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITEMOVE);
			}
#line 13488 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 386:
#line 2090 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITEHIDE);
			}
#line 13496 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 387:
#line 2096 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SPRITESHOW);
			}
#line 13504 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 388:
#line 2102 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CLICKCLEAR);
			}
#line 13512 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 389:
#line 2108 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CHANGEDIR);
			}
#line 13520 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 390:
#line 2114 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPEN);
			}
#line 13530 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 391:
#line 2119 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBOPEN);
			}
#line 13538 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 392:
#line 2122 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBOPEN);
			}
#line 13546 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 393:
#line 2128 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_DBCLOSE);
			}
#line 13555 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 394:
#line 2132 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_DBCLOSE);
			}
#line 13564 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 395:
#line 2136 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBCLOSE);
			}
#line 13572 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 396:
#line 2142 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addOp(OP_DBEXECUTE);
			}
#line 13582 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 397:
#line 2147 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBEXECUTE);
			}
#line 13590 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 398:
#line 2150 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBEXECUTE);
			}
#line 13598 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 399:
#line 2156 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			}
#line 13610 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 400:
#line 2163 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			}
#line 13620 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 401:
#line 2168 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBOPENSET);
			}
#line 13630 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 402:
#line 2173 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBOPENSET);
			}
#line 13638 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 403:
#line 2176 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBOPENSET);
			}
#line 13646 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 404:
#line 2182 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			}
#line 13656 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 2187 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			}
#line 13666 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 2192 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBCLOSESET);
			}
#line 13675 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 2196 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBCLOSESET);
			}
#line 13683 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 2199 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBCLOSESET);
			}
#line 13691 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 2205 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_NETLISTEN);
			}
#line 13701 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 2210 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETLISTEN);
			}
#line 13709 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 2213 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETLISTEN);
			}
#line 13717 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 2219 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKTOPTO2);
				addOp(OP_NETCONNECT);
			}
#line 13727 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 2224 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKTOPTO2);
				addOp(OP_NETCONNECT);
			}
#line 13737 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 2229 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETCONNECT);
			}
#line 13745 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 2232 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETCONNECT);
			}
#line 13753 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 2238 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_STACKSWAP);
				addOp(OP_NETWRITE);
			}
#line 13763 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 2243 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETWRITE);
			}
#line 13771 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 418:
#line 2246 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETWRITE);
			}
#line 13779 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 419:
#line 2252 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_NETCLOSE);
			}
#line 13788 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 2256 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_NETCLOSE);
			}
#line 13797 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 2260 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_NETCLOSE);
			}
#line 13805 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 2265 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_KILL);
			}
#line 13813 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 2271 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SETSETTING);
			}
#line 13821 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 2274 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SETSETTING);
			}
#line 13829 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 2280 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PORTOUT);
			}
#line 13837 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 2283 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PORTOUT);
			}
#line 13845 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 2289 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "PNG");
				addOp(OP_IMGSAVE);
			}
#line 13854 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 2293 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_IMGSAVE);
			}
#line 13862 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 2296 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_IMGSAVE);
			}
#line 13870 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 2302 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_EDITVISIBLE);
			}
#line 13878 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 2308 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_GRAPHVISIBLE);
			}
#line 13886 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 2314 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_OUTPUTVISIBLE);
			}
#line 13894 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 2320 "basicParse.y" /* yacc.c:1646  */
    {
				// create ops to make all of the variables listed globals
				if (numifs>0) {
					errorcode = COMPERR_GLOBALNOTHERE;
					return -1;
				}
				int t;
				for(t=numargs-1;t>=0;t--) {
					addIntOp(OP_GLOBAL, args[t]);
				}
				numargs=0;	// clear the list for next function
			}
#line 13911 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 2335 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PENWIDTH);
			}
#line 13919 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 435:
#line 2341 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_ALERT);
			}
#line 13927 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 436:
#line 2347 "basicParse.y" /* yacc.c:1646  */
    {
				// find most recent DO and jump to CONTINUE
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEDO) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLCONTINUE));
				} else {
					errorcode = COMPERR_CONTINUEDO;
					return -1;
				}
			}
#line 13945 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 2363 "basicParse.y" /* yacc.c:1646  */
    {
				// find most recent FOR and jump to CONTINUE
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEFOR) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLCONTINUE));
				} else {
					errorcode = COMPERR_CONTINUEFOR;
					return -1;
				}
			}
#line 13963 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 2379 "basicParse.y" /* yacc.c:1646  */
    {
				// find most recent WHILE and jump to CONTINUE
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEWHILE) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLCONTINUE));
				} else {
					errorcode = COMPERR_EXITWHILE;
					return -1;
				}
			}
#line 13981 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 2395 "basicParse.y" /* yacc.c:1646  */
    {
				// find most recent DO and jump to exit
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEDO) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLEXIT));
				} else {
					errorcode = COMPERR_EXITDO;
					return -1;
				}
			}
#line 13999 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 440:
#line 2411 "basicParse.y" /* yacc.c:1646  */
    {
				// find most recent FOR and jump to exit
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEFOR) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLEXIT));
				} else {
					errorcode = COMPERR_EXITFOR;
					return -1;
				}
			}
#line 14017 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 441:
#line 2427 "basicParse.y" /* yacc.c:1646  */
    {
				// find most recent WHILE and jump to exit
				int n=numifs-1;
				while(n>=0&&iftabletype[n]!=IFTABLETYPEWHILE) {
					n--;
				}
				if (n>=0) {
					addIntOp(OP_GOTO, getInternalSymbol(iftableid[n],INTERNALSYMBOLEXIT));
				} else {
					errorcode = COMPERR_EXITWHILE;
					return -1;
				}
			}
#line 14035 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 442:
#line 2443 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PRINTERCANCEL);
			}
#line 14043 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 443:
#line 2449 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PRINTEROFF);
			}
#line 14051 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 444:
#line 2455 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PRINTERON);
			}
#line 14059 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 445:
#line 2461 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PRINTERPAGE);
			}
#line 14067 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 446:
#line 2467 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
					errorcode = COMPERR_FUNCTIONNOTHERE;
					return -1;
				}
				//
				// $2 is the symbol for the function - add the start to the label table
				functionDefSymbol = (yyvsp[-1].number);
				functionType = FUNCTIONTYPEFLOAT;
				//
				// create jump around function definition (use nextifid and 0 for jump after)
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// create the new if frame for this function
				labeltable[(yyvsp[-1].number)] = wordOffset;
				newIf(linenumber, IFTABLETYPEFUNCTION);
				//
				// initialize return variable
				addIntOp(OP_PUSHINT, 0);
				addIntOp(OP_NUMASSIGN, (yyvsp[-1].number));
				//
				// check to see if there are enough values on the stack
				addIntOp(OP_PUSHINT, numargs);
				addOp(OP_ARGUMENTCOUNTTEST);
				//
				// add the assigns of the function arguments
				addOp(OP_INCREASERECURSE);
				{ 	int t;
					for(t=numargs-1;t>=0;t--) {
						if (argstype[t]==ARGSTYPEINT) addIntOp(OP_NUMASSIGN, args[t]);
						if (argstype[t]==ARGSTYPESTR) addIntOp(OP_STRINGASSIGN, args[t]);
						if (argstype[t]==ARGSTYPEVARREF) addIntOp(OP_VARREFASSIGN, args[t]);
						if (argstype[t]==ARGSTYPEVARREFSTR) addIntOp(OP_VARREFSTRASSIGN, args[t]);
					}
				}
				numargs=0;	// clear the list for next function
			}
#line 14109 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 447:
#line 2504 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
					errorcode = COMPERR_FUNCTIONNOTHERE;
					return -1;
				}
				//
				// $2 is the symbol for the function - add the start to the label table
				functionDefSymbol = (yyvsp[-1].number);
				functionType = FUNCTIONTYPESTRING;
				//
				// create jump around function definition (use nextifid and 0 for jump after)
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				//
				// create the new if frame for this function
				labeltable[(yyvsp[-1].number)] = wordOffset;
				newIf(linenumber, IFTABLETYPEFUNCTION);
				// 
				// initialize return variable
				addStringOp(OP_PUSHSTRING, "");
				addIntOp(OP_STRINGASSIGN, (yyvsp[-1].number));
				//
				// check to see if there are enough values on the stack
				addIntOp(OP_PUSHINT, numargs);
				addOp(OP_ARGUMENTCOUNTTEST);
				//
				// add the assigns of the function arguments
				addOp(OP_INCREASERECURSE);
				{ 	int t;
					for(t=numargs-1;t>=0;t--) {
						if (argstype[t]==ARGSTYPEINT) addIntOp(OP_NUMASSIGN, args[t]);
						if (argstype[t]==ARGSTYPESTR) addIntOp(OP_STRINGASSIGN, args[t]);
						if (argstype[t]==ARGSTYPEVARREF) addIntOp(OP_VARREFASSIGN, args[t]);
						if (argstype[t]==ARGSTYPEVARREFSTR) addIntOp(OP_VARREFSTRASSIGN, args[t]);
					}
				}
				numargs=0;	// clear the list for next function
			}
#line 14151 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 448:
#line 2544 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
					errorcode = COMPERR_FUNCTIONNOTHERE;
					return -1;
				}
				//
				// $2 is the symbol for the subroutine - add the start to the label table
				subroutineDefSymbol = (yyvsp[-1].number);
				//
				// create jump around subroutine definition (use nextifid and 0 for jump after)
				addIntOp(OP_GOTO, getInternalSymbol(nextifid,INTERNALSYMBOLEXIT));
				// 
				// create the new if frame for this subroutine
				labeltable[(yyvsp[-1].number)] = wordOffset;
				newIf(linenumber, IFTABLETYPEFUNCTION);
				//
				// check to see if there are enough values on the stack
				addIntOp(OP_PUSHINT, numargs);
				addOp(OP_ARGUMENTCOUNTTEST);
				//
				// add the assigns of the function arguments
				addOp(OP_INCREASERECURSE);
				{ 	int t;
					for(t=numargs-1;t>=0;t--) {
						if (argstype[t]==ARGSTYPEINT) addIntOp(OP_NUMASSIGN, args[t]);
						if (argstype[t]==ARGSTYPESTR) addIntOp(OP_STRINGASSIGN, args[t]);
						if (argstype[t]==ARGSTYPEVARREF) addIntOp(OP_VARREFASSIGN, args[t]);
						if (argstype[t]==ARGSTYPEVARREFSTR) addIntOp(OP_VARREFSTRASSIGN, args[t]);
					}
				}
				numargs=0;	// clear the list for next function
			}
#line 14188 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 449:
#line 2579 "basicParse.y" /* yacc.c:1646  */
    {
				if (numifs>0) {
				if (iftabletype[numifs-1]==IFTABLETYPEFUNCTION) {
					//
					// add return if there is not one
					addIntOp(OP_FUNCRETURN, functionDefSymbol);
					addOp(OP_DECREASERECURSE);
					addOp(OP_RETURN);
					//
					// add address for jump around function definition
					labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
					functionDefSymbol = -1; 
					//
					numifs--;
				// 
				} else {
					errorcode = testIfOnTableError(numincludes);
					linenumber = testIfOnTable(numincludes);
					return -1;
				}
			} else {
				errorcode = COMPERR_ENDFUNCTION;
				return -1;
			}
		}
#line 14218 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 450:
#line 2607 "basicParse.y" /* yacc.c:1646  */
    {
			if (numifs>0) {
				if (iftabletype[numifs-1]==IFTABLETYPEFUNCTION) {
					addOp(OP_DECREASERECURSE);
					addOp(OP_RETURN);
					//
					// add address for jump around function definition
					labeltable[getInternalSymbol(iftableid[numifs-1],INTERNALSYMBOLEXIT)] = wordOffset; 
					subroutineDefSymbol = -1; 
					//
					numifs--;
				} else {
					errorcode = testIfOnTableError(numincludes);
					linenumber = testIfOnTable(numincludes);
					return -1;
				}
			} else {
				errorcode = COMPERR_ENDFUNCTION;
				return -1;
			}
		}
#line 14244 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 451:
#line 2631 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_REGEXMINIMAL);
			}
#line 14252 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 454:
#line 2653 "basicParse.y" /* yacc.c:1646  */
    { listlen = 1; }
#line 14258 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 455:
#line 2654 "basicParse.y" /* yacc.c:1646  */
    { listlen++; }
#line 14264 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 456:
#line 2658 "basicParse.y" /* yacc.c:1646  */
    { listlen = 1; }
#line 14270 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 457:
#line 2659 "basicParse.y" /* yacc.c:1646  */
    { listlen++; }
#line 14276 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 458:
#line 2663 "basicParse.y" /* yacc.c:1646  */
    { listlen = 1; }
#line 14282 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 459:
#line 2664 "basicParse.y" /* yacc.c:1646  */
    {listlen++;}
#line 14288 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 462:
#line 2678 "basicParse.y" /* yacc.c:1646  */
    { (yyval.floatnum) = (yyvsp[-1].floatnum); }
#line 14294 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 463:
#line 2679 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_ADD);
			}
#line 14302 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 464:
#line 2682 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_SUB);
			}
#line 14310 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 465:
#line 2685 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_MUL);
			}
#line 14318 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 466:
#line 2688 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_MOD);
			}
#line 14326 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 467:
#line 2691 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_INTDIV);
			}
#line 14334 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 468:
#line 2694 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DIV);
			}
#line 14342 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 469:
#line 2697 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_EX); }
#line 14348 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 470:
#line 2698 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_BINARYOR); }
#line 14354 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 471:
#line 2699 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_BINARYAND); }
#line 14360 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 472:
#line 2700 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_BINARYNOT); }
#line 14366 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 473:
#line 2701 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NEGATE); }
#line 14372 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 474:
#line 2702 "basicParse.y" /* yacc.c:1646  */
    {addOp(OP_AND); }
#line 14378 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 475:
#line 2703 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_OR); }
#line 14384 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 476:
#line 2704 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_XOR); }
#line 14390 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 477:
#line 2705 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NOT); }
#line 14396 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 478:
#line 2706 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_EQUAL); }
#line 14402 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 479:
#line 2707 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_EQUAL); }
#line 14408 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 480:
#line 2708 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NEQUAL); }
#line 14414 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 481:
#line 2709 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NEQUAL); }
#line 14420 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 482:
#line 2710 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LT); }
#line 14426 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 483:
#line 2711 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LT); }
#line 14432 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 484:
#line 2712 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GT); }
#line 14438 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 485:
#line 2713 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GT); }
#line 14444 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 486:
#line 2714 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GTE); }
#line 14450 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 487:
#line 2715 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GTE); }
#line 14456 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 488:
#line 2716 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LTE); }
#line 14462 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 489:
#line 2717 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LTE); }
#line 14468 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 490:
#line 2718 "basicParse.y" /* yacc.c:1646  */
    { addFloatOp(OP_PUSHFLOAT, (yyvsp[0].floatnum)); }
#line 14474 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 491:
#line 2719 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, (yyvsp[0].number)); }
#line 14480 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 492:
#line 2720 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_ALEN, (yyvsp[-3].number)); }
#line 14486 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 493:
#line 2721 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_ALEN, (yyvsp[-3].number)); }
#line 14492 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 494:
#line 2722 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_ALENX, (yyvsp[-4].number)); }
#line 14498 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 495:
#line 2723 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_ALENX, (yyvsp[-4].number)); }
#line 14504 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 496:
#line 2724 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_ALENY, (yyvsp[-4].number)); }
#line 14510 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 497:
#line 2725 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_ALENY, (yyvsp[-4].number)); }
#line 14516 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 498:
#line 2726 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_DEREF, (yyvsp[-3].number)); }
#line 14522 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 499:
#line 2727 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n]++
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF, (yyvsp[-4].number));
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF, (yyvsp[-4].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-4].number));
			}
#line 14538 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 500:
#line 2738 "basicParse.y" /* yacc.c:1646  */
    {
				// a[n]--
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF, (yyvsp[-4].number));
				addOp(OP_STACKSWAP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF, (yyvsp[-4].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-4].number));
			}
#line 14554 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 501:
#line 2749 "basicParse.y" /* yacc.c:1646  */
    {
				// ++a[n]
				addOp(OP_STACKDUP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF, (yyvsp[-3].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-3].number));
				addIntOp(OP_DEREF, (yyvsp[-3].number));
			}
#line 14569 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 502:
#line 2759 "basicParse.y" /* yacc.c:1646  */
    {
				// --a[n]
				addOp(OP_STACKDUP);
				addOp(OP_STACKDUP);
				addIntOp(OP_DEREF, (yyvsp[-3].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN, (yyvsp[-3].number));
				addIntOp(OP_DEREF, (yyvsp[-3].number));
			}
#line 14584 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 503:
#line 2769 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_DEREF2D, (yyvsp[-5].number)); }
#line 14590 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 504:
#line 2770 "basicParse.y" /* yacc.c:1646  */
    {
				// a[b,c]++
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D, (yyvsp[-6].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D, (yyvsp[-6].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-6].number));
			}
#line 14606 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 505:
#line 2781 "basicParse.y" /* yacc.c:1646  */
    {
				// a[b,c]--
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D, (yyvsp[-6].number));
				addOp(OP_STACKTOPTO2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D, (yyvsp[-6].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-6].number));
			}
#line 14622 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 506:
#line 2792 "basicParse.y" /* yacc.c:1646  */
    {
				// ++a[b,c]
				addOp(OP_STACKDUP2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D, (yyvsp[-5].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-5].number));
				addIntOp(OP_DEREF2D, (yyvsp[-5].number));
			}
#line 14637 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 507:
#line 2802 "basicParse.y" /* yacc.c:1646  */
    {
				// --a[b,c]
				addOp(OP_STACKDUP2);
				addOp(OP_STACKDUP2);
				addIntOp(OP_DEREF2D, (yyvsp[-5].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_ARRAYASSIGN2D, (yyvsp[-5].number));
				addIntOp(OP_DEREF2D, (yyvsp[-5].number));
			}
#line 14652 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 508:
#line 2812 "basicParse.y" /* yacc.c:1646  */
    {
				// function call with arguments
				addIntOp(OP_GOSUB, (yyvsp[-3].number));
			}
#line 14661 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 509:
#line 2816 "basicParse.y" /* yacc.c:1646  */
    {
				// function call without arguments
				addIntOp(OP_GOSUB, (yyvsp[-2].number));
			}
#line 14670 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 510:
#line 2820 "basicParse.y" /* yacc.c:1646  */
    {
				if ((yyvsp[0].number) < 0) {
					return -1;
				} else {
					addIntOp(OP_PUSHVAR, (yyvsp[0].number));
				}
			}
#line 14682 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 511:
#line 2827 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-1].number));
				addIntOp(OP_PUSHVAR,(yyvsp[-1].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_NUMASSIGN, (yyvsp[-1].number));
			}
#line 14694 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 512:
#line 2834 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[-1].number));
				addIntOp(OP_PUSHVAR,(yyvsp[-1].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_NUMASSIGN, (yyvsp[-1].number));
			}
#line 14706 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 513:
#line 2841 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[0].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_ADD);
				addIntOp(OP_NUMASSIGN, (yyvsp[0].number));
				addIntOp(OP_PUSHVAR,(yyvsp[0].number));
			}
#line 14718 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 514:
#line 2848 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHVAR,(yyvsp[0].number));
				addIntOp(OP_PUSHINT,1);
				addOp(OP_SUB);
				addIntOp(OP_NUMASSIGN, (yyvsp[0].number));
				addIntOp(OP_PUSHVAR,(yyvsp[0].number));
			}
#line 14730 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 515:
#line 2855 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_INT); }
#line 14736 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 516:
#line 2856 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_FLOAT); }
#line 14742 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 517:
#line 2857 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LENGTH); }
#line 14748 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 518:
#line 2858 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_ASC); }
#line 14754 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 519:
#line 2859 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);	// start
				addIntOp(OP_PUSHINT, 0);	// case sens flag
				addOp(OP_INSTR);
			}
#line 14764 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 520:
#line 2864 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);	// case sens flag
				addOp(OP_INSTR);
			 }
#line 14773 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 521:
#line 2868 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_INSTR); }
#line 14779 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 522:
#line 2869 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 1);	//start
				addOp(OP_INSTRX);
			}
#line 14788 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 523:
#line 2873 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_INSTRX); }
#line 14794 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 524:
#line 2874 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CEIL); }
#line 14800 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 525:
#line 2875 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_FLOOR); }
#line 14806 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 526:
#line 2876 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SIN); }
#line 14812 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 527:
#line 2877 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_COS); }
#line 14818 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 528:
#line 2878 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_TAN); }
#line 14824 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 529:
#line 2879 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_ASIN); }
#line 14830 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 530:
#line 2880 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_ACOS); }
#line 14836 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 531:
#line 2881 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_ATAN); }
#line 14842 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 532:
#line 2882 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_DEGREES); }
#line 14848 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 533:
#line 2883 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_RADIANS); }
#line 14854 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 534:
#line 2884 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LOG); }
#line 14860 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 535:
#line 2885 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LOGTEN); }
#line 14866 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 536:
#line 2886 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SQR); }
#line 14872 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 537:
#line 2887 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_EXP); }
#line 14878 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 538:
#line 2888 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_ABS); }
#line 14884 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 539:
#line 2889 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_RAND); }
#line 14890 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 540:
#line 2890 "basicParse.y" /* yacc.c:1646  */
    { addFloatOp(OP_PUSHFLOAT, 3.14159265358979323846); }
#line 14896 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 541:
#line 2891 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 1); }
#line 14902 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 542:
#line 2892 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); }
#line 14908 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 543:
#line 2893 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_EOF);
			}
#line 14917 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 544:
#line 2897 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);
				addOp(OP_EOF);
			}
#line 14926 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 545:
#line 2901 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_EOF); }
#line 14932 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 546:
#line 2902 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_EXISTS); }
#line 14938 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 547:
#line 2903 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_YEAR); }
#line 14944 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 548:
#line 2904 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MONTH); }
#line 14950 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 549:
#line 2905 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_DAY); }
#line 14956 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 550:
#line 2906 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_HOUR); }
#line 14962 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 551:
#line 2907 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MINUTE); }
#line 14968 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 552:
#line 2908 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SECOND); }
#line 14974 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 553:
#line 2909 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GRAPHWIDTH); }
#line 14980 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 554:
#line 2910 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GRAPHHEIGHT); }
#line 14986 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 555:
#line 2911 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); }
#line 14992 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 556:
#line 2912 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_SIZE); }
#line 14998 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 557:
#line 2913 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SIZE); }
#line 15004 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 558:
#line 2914 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_KEY); }
#line 15010 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 559:
#line 2915 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MOUSEX); }
#line 15016 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 560:
#line 2916 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MOUSEY); }
#line 15022 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 561:
#line 2917 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MOUSEB); }
#line 15028 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 562:
#line 2918 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CLICKX); }
#line 15034 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 563:
#line 2919 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CLICKY); }
#line 15040 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 564:
#line 2920 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CLICKB); }
#line 15046 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 565:
#line 2921 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addOp(OP_RGB);
				}
#line 15058 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 566:
#line 2928 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15070 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 567:
#line 2935 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xf8);
				addIntOp(OP_PUSHINT, 0xf8);
				addIntOp(OP_PUSHINT, 0xf8);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15082 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 568:
#line 2942 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xff);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15094 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 569:
#line 2949 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15106 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 570:
#line 2956 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15118 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 571:
#line 2963 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15130 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 572:
#line 2970 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15142 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 573:
#line 2977 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15154 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 574:
#line 2984 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15166 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 575:
#line 2991 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15178 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 576:
#line 2998 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15190 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 577:
#line 3005 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15202 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 578:
#line 3012 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15214 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 579:
#line 3019 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15226 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 580:
#line 3026 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0x66);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15238 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 581:
#line 3033 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xFF);
				addIntOp(OP_PUSHINT, 0x33);
				addIntOp(OP_PUSHINT, 0x00);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15250 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 582:
#line 3040 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xA4);
				addIntOp(OP_PUSHINT, 0xA4);
				addIntOp(OP_PUSHINT, 0xA4);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15262 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 583:
#line 3047 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0x80);
				addIntOp(OP_PUSHINT, 0xff);
				addOp(OP_RGB);
				}
#line 15274 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 584:
#line 3054 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_PIXEL); }
#line 15280 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 585:
#line 3055 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0xff); 
				addOp(OP_RGB);
			}
#line 15289 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 586:
#line 3059 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_RGB);
			}
#line 15297 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 587:
#line 3062 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GETCOLOR); }
#line 15303 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 588:
#line 3063 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GETBRUSHCOLOR); }
#line 15309 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 589:
#line 3064 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GETPENWIDTH); }
#line 15315 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 590:
#line 3065 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITECOLLIDE); }
#line 15321 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 591:
#line 3066 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITEX); }
#line 15327 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 592:
#line 3067 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITEY); }
#line 15333 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 593:
#line 3068 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITEH); }
#line 15339 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 594:
#line 3069 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITEW); }
#line 15345 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 595:
#line 3070 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITEV); }
#line 15351 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 596:
#line 3071 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITER); }
#line 15357 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 597:
#line 3072 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_SPRITES); }
#line 15363 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 598:
#line 3073 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBROW);
			}
#line 15373 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 599:
#line 3078 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBROW);
			}
#line 15383 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 600:
#line 3083 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_DBROW);
			}
#line 15392 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 601:
#line 3087 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBROW);
			}
#line 15400 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 602:
#line 3090 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBINT); }
#line 15411 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 603:
#line 3096 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBINT); }
#line 15420 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 604:
#line 3100 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBINT); }
#line 15427 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 605:
#line 3102 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBINTS); }
#line 15438 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 606:
#line 3108 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBINTS); }
#line 15447 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 607:
#line 3112 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBINTS); }
#line 15454 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 608:
#line 3114 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBFLOAT); }
#line 15465 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 609:
#line 3120 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBFLOAT); }
#line 15474 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 610:
#line 3124 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBFLOAT); }
#line 15481 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 611:
#line 3126 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBFLOATS); }
#line 15492 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 612:
#line 3132 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBFLOATS); }
#line 15501 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 613:
#line 3136 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBFLOATS); }
#line 15508 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 614:
#line 3138 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBNULL); }
#line 15519 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 615:
#line 3144 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBNULL); }
#line 15528 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 616:
#line 3148 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBNULL); }
#line 15535 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 617:
#line 3150 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBNULLS); }
#line 15546 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 618:
#line 3156 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBNULLS); }
#line 15555 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 619:
#line 3160 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBNULLS); }
#line 15562 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 620:
#line 3162 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LASTERROR); }
#line 15568 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 621:
#line 3163 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LASTERRORLINE); }
#line 15574 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 622:
#line 3164 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETDATA); }
#line 15580 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 623:
#line 3165 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETDATA); }
#line 15586 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 624:
#line 3166 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NETDATA); }
#line 15592 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 625:
#line 3167 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_PORTIN); }
#line 15598 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 626:
#line 3168 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0); // case sens flag
				addOp(OP_COUNT);
			 }
#line 15607 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 627:
#line 3172 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_COUNT); }
#line 15613 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 628:
#line 3173 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_COUNTX); }
#line 15619 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 629:
#line 3174 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_OSTYPE); }
#line 15625 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 630:
#line 3175 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MSEC); }
#line 15631 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 631:
#line 3176 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_TEXTWIDTH); }
#line 15637 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 632:
#line 3177 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_TEXTHEIGHT); }
#line 15643 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 633:
#line 3178 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READBYTE); }
#line 15649 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 634:
#line 3179 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READBYTE); }
#line 15655 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 635:
#line 3180 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_READBYTE); }
#line 15661 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 636:
#line 3181 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHVARREF, (yyvsp[-1].number)); }
#line 15667 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 637:
#line 3182 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHVARREFSTR, (yyvsp[-1].number)); }
#line 15673 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 638:
#line 3183 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_FREEDB); }
#line 15679 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 639:
#line 3184 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_FREEDBSET);
			}
#line 15688 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 640:
#line 3188 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_FREEDBSET);
			}
#line 15697 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 641:
#line 3192 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_FREEDBSET); }
#line 15703 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 642:
#line 3193 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_FREEFILE); }
#line 15709 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 643:
#line 3194 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_FREENET); }
#line 15715 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 644:
#line 3195 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, VERSIONSIGNATURE); }
#line 15721 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 645:
#line 3196 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,-1);	// no default
				addOp(OP_CONFIRM);
			}
#line 15730 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 646:
#line 3200 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_CONFIRM);
			}
#line 15738 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 647:
#line 3203 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,2);	// radix
				addOp(OP_FROMRADIX);
			}
#line 15747 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 648:
#line 3207 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,16);	// radix
				addOp(OP_FROMRADIX);
			}
#line 15756 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 649:
#line 3211 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,8);	// radix
				addOp(OP_FROMRADIX);
			}
#line 15765 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 650:
#line 3215 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_FROMRADIX);
			}
#line 15773 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 651:
#line 3218 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, (yyvsp[0].string));
				addIntOp(OP_PUSHINT,2);	// radix
				addOp(OP_FROMRADIX);
			}
#line 15783 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 652:
#line 3223 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, (yyvsp[0].string));
				addIntOp(OP_PUSHINT,16);	// radix
				addOp(OP_FROMRADIX);
			}
#line 15793 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 653:
#line 3228 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, (yyvsp[0].string));
				addIntOp(OP_PUSHINT,8);	// radix
				addOp(OP_FROMRADIX);
			}
#line 15803 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 654:
#line 3233 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_WAVLENGTH); }
#line 15809 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 655:
#line 3234 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_WAVPOS); }
#line 15815 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 656:
#line 3235 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_WAVSTATE); }
#line 15821 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 657:
#line 3237 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NONE); }
#line 15827 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 658:
#line 3238 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FOR1); }
#line 15833 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 659:
#line 3239 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FOR2); }
#line 15839 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 660:
#line 3240 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FILENUMBER); }
#line 15845 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 661:
#line 3241 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FILEOPEN); }
#line 15851 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 662:
#line 3242 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FILENOTOPEN); }
#line 15857 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 663:
#line 3243 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FILEWRITE); }
#line 15863 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 664:
#line 3244 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FILERESET); }
#line 15869 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 665:
#line 3245 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_ARRAYSIZELARGE); }
#line 15875 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 666:
#line 3246 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_ARRAYSIZESMALL); }
#line 15881 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 667:
#line 3247 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NOSUCHVARIABLE); }
#line 15887 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 668:
#line 3248 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_ARRAYINDEX); }
#line 15893 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 669:
#line 3249 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_STRNEGLEN); }
#line 15899 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 670:
#line 3250 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_STRSTART); }
#line 15905 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 671:
#line 3251 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NONNUMERIC); }
#line 15911 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 672:
#line 3252 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_RGB); }
#line 15917 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 673:
#line 3253 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_PUTBITFORMAT); }
#line 15923 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 674:
#line 3254 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_POLYARRAY); }
#line 15929 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 675:
#line 3255 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_POLYPOINTS); }
#line 15935 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 676:
#line 3256 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_IMAGEFILE); }
#line 15941 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 677:
#line 3257 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_SPRITENUMBER); }
#line 15947 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 678:
#line 3258 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_SPRITENA); }
#line 15953 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 679:
#line 3259 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_SPRITESLICE); }
#line 15959 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 680:
#line 3260 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FOLDER); }
#line 15965 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 681:
#line 3261 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_INFINITY); }
#line 15971 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 682:
#line 3262 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBOPEN); }
#line 15977 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 683:
#line 3263 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBQUERY); }
#line 15983 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 684:
#line 3264 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBNOTOPEN); }
#line 15989 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 685:
#line 3265 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBCOLNO); }
#line 15995 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 686:
#line 3266 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBNOTSET); }
#line 16001 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 687:
#line 3267 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETSOCK); }
#line 16007 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 688:
#line 3268 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETHOST); }
#line 16013 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 689:
#line 3269 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETCONN); }
#line 16019 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 690:
#line 3270 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETREAD); }
#line 16025 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 691:
#line 3271 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETNONE); }
#line 16031 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 692:
#line 3272 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETWRITE); }
#line 16037 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 693:
#line 3273 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETSOCKOPT); }
#line 16043 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 694:
#line 3274 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETBIND); }
#line 16049 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 695:
#line 3275 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETACCEPT); }
#line 16055 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 696:
#line 3276 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NETSOCKNUMBER); }
#line 16061 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 697:
#line 3277 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_PERMISSION); }
#line 16067 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 698:
#line 3278 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_IMAGESAVETYPE); }
#line 16073 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 699:
#line 3279 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DIVZERO); }
#line 16079 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 700:
#line 3280 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_BYREF); }
#line 16085 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 701:
#line 3281 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_BYREFTYPE); }
#line 16091 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 702:
#line 3282 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FREEFILE); }
#line 16097 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 703:
#line 3283 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FREENET); }
#line 16103 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 704:
#line 3284 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FREEDB); }
#line 16109 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 705:
#line 3285 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBCONNNUMBER); }
#line 16115 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 706:
#line 3286 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FREEDBSET); }
#line 16121 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 707:
#line 3287 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBSETNUMBER); }
#line 16127 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 708:
#line 3288 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_DBNOTSETROW); }
#line 16133 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 709:
#line 3289 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_PENWIDTH); }
#line 16139 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 710:
#line 3290 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_COLORNUMBER); }
#line 16145 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 711:
#line 3291 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_ARRAYINDEXMISSING); }
#line 16151 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 712:
#line 3292 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_IMAGESCALE); }
#line 16157 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 713:
#line 3293 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FONTSIZE); }
#line 16163 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 714:
#line 3294 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_FONTWEIGHT); }
#line 16169 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 715:
#line 3295 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_RADIXSTRING); }
#line 16175 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 716:
#line 3296 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_RADIX); }
#line 16181 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 717:
#line 3297 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_LOGRANGE); }
#line 16187 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 718:
#line 3298 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_STRINGMAXLEN); }
#line 16193 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 719:
#line 3299 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NOTANUMBER); }
#line 16199 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 720:
#line 3300 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_PRINTERNOTON); }
#line 16205 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 721:
#line 3301 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_PRINTERNOTOFF); }
#line 16211 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 722:
#line 3302 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_PRINTEROPEN); }
#line 16217 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 723:
#line 3303 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_WAVFILEFORMAT); }
#line 16223 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 724:
#line 3304 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_WAVNOTOPEN); }
#line 16229 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 725:
#line 3305 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, ERROR_NOTIMPLEMENTED); }
#line 16235 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 726:
#line 3307 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, WARNING_TYPECONV); }
#line 16241 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 727:
#line 3308 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, WARNING_WAVNODURATION); }
#line 16247 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 728:
#line 3309 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, WARNING_WAVNOTSEEKABLE); }
#line 16253 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 729:
#line 3318 "basicParse.y" /* yacc.c:1646  */
    { (yyval.string) = (yyvsp[-1].string); }
#line 16259 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 730:
#line 3319 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CONCAT); }
#line 16265 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 731:
#line 3320 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CONCAT); }
#line 16271 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 732:
#line 3321 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CONCAT); }
#line 16277 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 733:
#line 3322 "basicParse.y" /* yacc.c:1646  */
    { addStringOp(OP_PUSHSTRING, (yyvsp[0].string)); }
#line 16283 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 734:
#line 3323 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_DEREF, (yyvsp[-3].number)); }
#line 16289 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 735:
#line 3324 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_DEREF2D, (yyvsp[-5].number)); }
#line 16295 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 736:
#line 3325 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_GOSUB, (yyvsp[-3].number)); }
#line 16301 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 737:
#line 3326 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_GOSUB, (yyvsp[-2].number)); }
#line 16307 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 738:
#line 3327 "basicParse.y" /* yacc.c:1646  */
    {
				if ((yyvsp[0].number) < 0) {
					return -1;
				} else {
					addIntOp(OP_PUSHVAR, (yyvsp[0].number));
				}
			}
#line 16319 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 739:
#line 3334 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CHR); }
#line 16325 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 740:
#line 3335 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_STRING); }
#line 16331 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 741:
#line 3336 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_UPPER); }
#line 16337 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 742:
#line 3337 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LOWER); }
#line 16343 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 743:
#line 3338 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MID); }
#line 16349 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 744:
#line 3339 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 1); addOp(OP_MIDX); }
#line 16355 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 745:
#line 3340 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MIDX); }
#line 16361 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 746:
#line 3341 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LEFT); }
#line 16367 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 747:
#line 3342 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_RIGHT); }
#line 16373 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 748:
#line 3343 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GETSLICE); }
#line 16379 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 749:
#line 3344 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); }
#line 16385 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 750:
#line 3345 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READ); }
#line 16391 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 751:
#line 3346 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_READ); }
#line 16397 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 752:
#line 3347 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); }
#line 16403 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 753:
#line 3348 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_READLINE); }
#line 16409 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 754:
#line 3349 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_READLINE); }
#line 16415 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 755:
#line 3350 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CURRENTDIR); }
#line 16421 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 756:
#line 3351 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_CURRENTDIR); }
#line 16427 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 757:
#line 3352 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBSTRING); }
#line 16438 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 758:
#line 3358 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBSTRING); }
#line 16447 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 759:
#line 3362 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBSTRING); }
#line 16454 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 760:
#line 3364 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default db number
				addOp(OP_STACKSWAP);
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBSTRINGS); }
#line 16465 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 761:
#line 3370 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,0);	// default dbset number
				addOp(OP_STACKSWAP);
				addOp(OP_DBSTRINGS); }
#line 16474 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 762:
#line 3374 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DBSTRINGS); }
#line 16481 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 763:
#line 3376 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LASTERRORMESSAGE); }
#line 16487 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 764:
#line 3377 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LASTERRORMESSAGE); }
#line 16493 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 765:
#line 3378 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LASTERROREXTRA); }
#line 16499 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 766:
#line 3379 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_LASTERROREXTRA); }
#line 16505 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 767:
#line 3380 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETREAD); }
#line 16511 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 768:
#line 3381 "basicParse.y" /* yacc.c:1646  */
    { addIntOp(OP_PUSHINT, 0); addOp(OP_NETREAD); }
#line 16517 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 769:
#line 3382 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NETREAD); }
#line 16523 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 770:
#line 3383 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NETADDRESS); }
#line 16529 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 771:
#line 3384 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_NETADDRESS); }
#line 16535 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 772:
#line 3385 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_MD5); }
#line 16541 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 773:
#line 3386 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_GETSETTING); }
#line 16547 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 774:
#line 3387 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_DIR); }
#line 16553 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 775:
#line 3388 "basicParse.y" /* yacc.c:1646  */
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_DIR); }
#line 16559 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 776:
#line 3389 "basicParse.y" /* yacc.c:1646  */
    { addStringOp(OP_PUSHSTRING, ""); addOp(OP_DIR); }
#line 16565 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 777:
#line 3390 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT, 0);	// case sens flag
				addOp(OP_REPLACE);
			}
#line 16574 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 778:
#line 3394 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_REPLACE); }
#line 16580 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 779:
#line 3395 "basicParse.y" /* yacc.c:1646  */
    { addOp(OP_REPLACEX); }
#line 16586 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 780:
#line 3396 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, ""); // no delimiter
				addIntOp(OP_IMPLODE, (yyvsp[-1].number));
			}
#line 16595 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 781:
#line 3400 "basicParse.y" /* yacc.c:1646  */
    {  addIntOp(OP_IMPLODE, (yyvsp[-3].number)); }
#line 16601 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 782:
#line 3401 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, ""); // no delimiter
				addIntOp(OP_IMPLODE, (yyvsp[-1].number));
			}
#line 16610 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 783:
#line 3405 "basicParse.y" /* yacc.c:1646  */
    {  addIntOp(OP_IMPLODE, (yyvsp[-3].number)); }
#line 16616 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 784:
#line 3406 "basicParse.y" /* yacc.c:1646  */
    {
				addStringOp(OP_PUSHSTRING, "");	
				addOp(OP_PROMPT); }
#line 16624 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 785:
#line 3409 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_PROMPT); }
#line 16631 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 786:
#line 3411 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,2);	// radix
				addOp(OP_TORADIX);
			}
#line 16640 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 787:
#line 3415 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,16);	// radix
				addOp(OP_TORADIX);
			}
#line 16649 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 788:
#line 3419 "basicParse.y" /* yacc.c:1646  */
    {
				addIntOp(OP_PUSHINT,8);	// radix
				addOp(OP_TORADIX);
			}
#line 16658 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 789:
#line 3423 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_TORADIX);
			}
#line 16666 "basicParse.tab.c" /* yacc.c:1646  */
    break;

  case 790:
#line 3426 "basicParse.y" /* yacc.c:1646  */
    {
				addOp(OP_DEBUGINFO);
			}
#line 16674 "basicParse.tab.c" /* yacc.c:1646  */
    break;


#line 16678 "basicParse.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
  return yyresult;
}
#line 3436 "basicParse.y" /* yacc.c:1906  */


int
yyerror(const char *msg) {
	errorcode = COMPERR_SYNTAX;
	return -1;
}
